#include <scheduler/process_manager.h>
#include <interruptHandlers/interrupts.h>


PCB* process_table;
pid_t foreground_process_pid = -1;

static int8_t basic_kill_process(pid_t pid);
static void free_argv(char** argv, uint32_t argc);

int8_t init_processes() {
    process_table = mm_malloc(MAX_PROCESSES * sizeof(PCB));
    if (process_table == NULL) return -1;
    
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1;
        process_table[i].state = EXITED;
    }   

    return 0;
}

char create_process(uint64_t wrapper_entry_point, uint64_t entry_point, uint32_t argc, char* shell_argv[], uint32_t priority, uint8_t fg, char fds[]) {
    if (process_table == NULL) return -1;
    
    _cli();

    if (argc <= 0 || shell_argv == NULL || shell_argv[0] == NULL) 
        return -1;

    char** argv = mm_malloc(sizeof(char*) * (argc + 1));

    if (argv == NULL) return -1;

    for (int i = 0; i < argc; i++) {
        argv[i] = mm_malloc(my_strlen(shell_argv[i]) + 1);
        if (argv[i] == NULL) {
            free_argv(argv, i);
            return -1;
        }
    }

    for (int i = 0; i < argc; i++)
        my_strcpy(argv[i], shell_argv[i]);
    
    
    argv[argc] = NULL;
    
    // Busco un lugar libre en la tabla de procesos
    int i;
    char found = 0;
    for (i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state == EXITED) {
            found = 1;
            break;
        }
    }

    if (!found) {
        return -1;
    }
        

    PCB* running_process = get_running_process();
    int father_pid;

    char process_fds[2];

    if(running_process == NULL) {
        father_pid = 0;
        process_fds[0] = STDIN;
        process_fds[1] = STDOUT;
    } else {
        father_pid = running_process->pid;
        process_fds[0] = running_process->readfd;
        process_fds[1] = running_process->writefd;
    }

    if(fds != NULL) {
        process_fds[0] = fds[0];
        process_fds[1] = fds[1];
    }

    process_table[i].pid = i;
    process_table[i].fg = fg;
    process_table[i].readfd = process_fds[0];
    process_table[i].writefd = process_fds[1];
    process_table[i].father_pid = father_pid;
    process_table[i].process_name = argv[0];
    process_table[i].argv = argv;
    process_table[i].argc = argc;
    process_table[i].priority = priority;
    process_table[i].state = READY;
    
    process_table[i].limit = (uint64_t) mm_malloc(STACK_SIZE);

    if ((void *) process_table[i].limit == NULL){
        free_argv(argv, argc);
        process_table[i].state = EXITED;
        return -1;
    }
    
    process_table[i].sp = process_table[i].limit + STACK_SIZE;
    process_table[i].base = process_table[i].sp - sizeof(uint64_t);
    process_table[i].processes_blocked_by_me = list_create();

    if (process_table[i].processes_blocked_by_me == NULL){
        mm_free((void *) process_table[i].limit);
        free_argv(argv, argc);
        process_table[i].state = EXITED;
        return -1;
    }
    
    
    // Inicializamos el stack pointer
    uint64_t* stack_ptr = (uint64_t*) process_table[i].sp;

    // Ajustamos el stack pointer para dejar espacio para InitialStack
    stack_ptr -= sizeof(InitialStack) / sizeof(uint64_t);

    // Inicializamos la estructura InitialStack
    InitialStack* stack = (InitialStack*) stack_ptr;
    stack->rax = 0;
    stack->rbx = 0;
    stack->rcx = i;
    stack->rdx = (uint64_t) argv;
    stack->rbp = process_table[i].sp - sizeof(uint64_t);
    stack->rdi = entry_point;
    stack->rsi = argc;
    stack->r8 = 0;
    stack->r9 = 0;
    stack->r10 = 0;
    stack->r11 = 0;
    stack->r12 = 0;
    stack->r13 = 0;
    stack->r14 = 0;
    stack->r15 = 0;

    // Configuramos los registros de interrupción y de estado
    stack->rsp = process_table[i].sp;
    stack->rip = wrapper_entry_point;
    stack->cs = 0x08;
    stack->rflags = 0x202;
    stack->ss = 0;
    stack->align = 0;

    // Actualizamos el stack pointer del proceso
    process_table[i].sp = (uint64_t) stack_ptr;

    add_ready_process(&process_table[i]);

    if (fg == 1) 
        foreground_process_pid = i;

    _sti();
    return process_table[i].pid;
}

void set_process_readfd(pid_t pid, char fd) {
    PCB* process = find_pcb_by_pid(pid);
    if (process != NULL) {
        process->readfd = fd;
    }
}

void set_process_writefd(pid_t pid, char fd) {
    PCB* process = find_pcb_by_pid(pid);
    if (process != NULL) {
        process->writefd = fd;
    }
}

int8_t block_process(pid_t pid){
    if (process_table == NULL) return -1;

    PCB* process_to_block = find_pcb_by_pid(pid);

    if (process_to_block == NULL) return -1;

    State state = process_to_block->state;

    if (state == EXITED || state == BLOCKED) return -1;
    
    process_to_block->state = BLOCKED;
    remove_ready_process(process_to_block);

    if(state == RUNNING) 
        yield();
    
    return 0;
}

int8_t unblock_process(pid_t pid){
    if (process_table == NULL) return -1;

    PCB* blocked_process = find_pcb_by_pid(pid);

    if (blocked_process == NULL) return -1;

    State state = blocked_process->state;
    
    if (state != BLOCKED) return -1;

    blocked_process->state = READY;
    add_ready_process(blocked_process);
    return 0;
}

static int cmp_pid(pid_t d1, pid_t d2){
    return d1 == d2 ? 0 : 1;
}

void unblock_waiting_processes(pid_t pid) {
    if (process_table == NULL) return;

    PCB* process = find_pcb_by_pid(pid);
    if (process == NULL) return;
    ListCircularADT waiting_processes = process->processes_blocked_by_me;
    while(list_size(waiting_processes) > 0){
        pid_t pid = (pid_t)(uint64_t) list_next(waiting_processes);
        unblock_process(pid);
        list_remove(waiting_processes, (DataType)(uint64_t) pid, (void *) cmp_pid);
    }
}

int8_t kill_process(pid_t pid){
    if (process_table == NULL) return -1;

    if (basic_kill_process(pid) == -1) return -1;

    PCB* process_to_kill = find_pcb_by_pid(pid);    

    if (process_to_kill == get_running_process()) 
        yield();
    
    return 0;
}

int8_t modify_process_priority(pid_t pid, uint32_t priority){
    if (process_table == NULL) return -1;

    PCB* process = find_pcb_by_pid(pid);

    if (process == NULL) return -1;

    State state = process->state;

    if (state == EXITED) return -1;

    if(state == BLOCKED) {
        process->priority = priority;
        return 0;
    }

    remove_ready_process(process);

    process->priority = priority;

    add_ready_process(process);

    return 0;
}

PCB* find_pcb_by_pid(pid_t pid){
    if (process_table == NULL) return NULL;
    if (pid >= MAX_PROCESSES) return NULL;
    
    return &process_table[pid];
}

pid_t get_pid(){
    PCB* running_process = get_running_process();
    return running_process->pid;
}

void wait(pid_t pid){
    if (process_table == NULL) return;

    PCB* process_to_wait = find_pcb_by_pid(pid);

    if (process_to_wait == NULL) return;

    if (process_to_wait->state == EXITED) return;

    pid_t pid_running = get_pid();

    list_add(process_to_wait->processes_blocked_by_me, (DataType)(uint64_t) pid_running);

    block_process(pid_running);
}

void ps() {
    if (process_table == NULL) return;

    char buffer[100];
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state != EXITED) {

            write_to_video_text_buffer("PID: ", 5, 0xFFFFFFFF);
            my_itoa(process_table[i].pid, buffer, 10);
            write_to_video_text_buffer(buffer, my_strlen(buffer), 0xFFFFFFFF);
            write_to_video_text_buffer(" | ", 3, 0xFFFFFFFF);

            write_to_video_text_buffer("NAME: ", 6, 0xFFFFFFFF);
            write_to_video_text_buffer(process_table[i].process_name, my_strlen(process_table[i].process_name), 0xFFFFFFFF);
            write_to_video_text_buffer(" | ", 3, 0xFFFFFFFF);

            write_to_video_text_buffer("STATE: ", 7, 0xFFFFFFFF);
            switch (process_table[i].state) {
                case READY:
                    write_to_video_text_buffer("READY", 5, 0xFFdbfa2d);
                    break;
                case RUNNING:
                    write_to_video_text_buffer("RUNNING", 7, 0xFF00e381);
                    break;
                case BLOCKED:
                    write_to_video_text_buffer("BLOCKED", 7, 0xFFf71b07);
                    break;
                case EXITED:
                    write_to_video_text_buffer("EXITED", 6, 0xFFf71b07);
                    break;
            }
            write_to_video_text_buffer(" | ", 3, 0xFFFFFFFF);

            write_to_video_text_buffer("PRIORITY: ", 10, 0xFFFFFFFF);
            my_itoa(process_table[i].priority, buffer, 10);
            write_to_video_text_buffer(buffer, my_strlen(buffer), 0xFF00FF00);
            write_to_video_text_buffer(" | ", 3, 0xFFFFFFFF);

            write_to_video_text_buffer("SP: 0x", 6, 0xFFFFFFFF);
            uint64_to_hex_string(process_table[i].sp, buffer, 17);
            write_to_video_text_buffer(buffer, my_strlen(buffer), 0xFFFFFFFF);
            write_to_video_text_buffer(" | ", 3, 0xFFFFFFFF);

            write_to_video_text_buffer("BP: 0x", 6, 0xFFFFFFFF);
            uint64_to_hex_string(process_table[i].base, buffer, 17);
            write_to_video_text_buffer(buffer, my_strlen(buffer), 0xFFFFFFFF);

            write_to_video_text_buffer("\n", 1, 0xFFFFFFFF);
        }
    }
}

void kill_foreground_process() {
    if (process_table == NULL) return;

    if (foreground_process_pid != -1){
        if (process_table[foreground_process_pid].state != EXITED && process_table[foreground_process_pid].fg == 1){
            basic_kill_process(foreground_process_pid);
        }
    }
}

static int8_t basic_kill_process(pid_t pid){
    PCB* process_to_kill = find_pcb_by_pid(pid);
    
    if (process_to_kill == NULL) return -1;

    State state = process_to_kill->state;
    
    if (state == EXITED) return -1;

    process_to_kill->state = EXITED;

    unblock_waiting_processes(pid);

    if (state == READY || state == RUNNING)
        remove_ready_process(process_to_kill);

    mm_free((void *) process_to_kill->limit);
    list_destroy(process_to_kill->processes_blocked_by_me, NULL);
    
    for (int i = 0; i <= process_to_kill->argc ; i++) 
        mm_free(process_to_kill->argv[i]);
    
    mm_free(process_to_kill->argv);

    return 0;
}

static void free_argv(char** argv, uint32_t argc) {
    for (int i = 0; i < argc; i++) {
        mm_free(argv[i]);
    }
    mm_free(argv);
}