#include <scheduler/process_manager.h>
#include <interruptHandlers/interrupts.h>


PCB* process_table;

char create_process(uint64_t wrapper_entry_point, uint64_t entry_point, uint32_t argc, char* argv[], uint32_t priority) {
    _cli();

    if (argc <= 0 || argv == NULL || argv[0] == NULL) {
        return -1;
    }
    
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

    // TODO: logica para saber el father_pid
    int father_pid = 0;
    // TODO: logica para saber el father_pid

    process_table[i].pid = i;
    process_table[i].father_pid = father_pid;
    process_table[i].process_name = argv[0];
    process_table[i].priority = priority;
    process_table[i].state = READY;
    process_table[i].limit = (uint64_t) mm_malloc(STACK_SIZE);
    process_table[i].sp = process_table[i].limit + STACK_SIZE;
    process_table[i].base = process_table[i].sp - sizeof(uint64_t);
    
    // Inicializamos el stack pointer
    uint64_t* stack_ptr = (uint64_t*) process_table[i].sp;

    // Ajustamos el stack pointer para dejar espacio para InitialStack
    stack_ptr -= sizeof(InitialStack) / sizeof(uint64_t);

    // Inicializamos la estructura InitialStack
    InitialStack* stack = (InitialStack*) stack_ptr;
    stack->rax = 0;
    stack->rbx = 0;
    stack->rcx = i;
    stack->rdx = argv;
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
    _sti();
    return process_table[i].pid;
}

uint8_t block_process(pid_t pid){
    PCB* process_to_block = find_pcb_by_pid(pid);

    if (process_to_block == NULL) return -1;

    State state = process_to_block->state;

    if (state == EXITED || state == BLOCKED) return -1;
    
    
    process_to_block->state = BLOCKED;
    remove_ready_process(process_to_block);
    return 0;
}

uint8_t unblock_process(pid_t pid){
    PCB* blocked_process = find_pcb_by_pid(pid);

    if (blocked_process == NULL) return -1;

    State state = blocked_process->state;
    
    if (state != BLOCKED) return -1;

    blocked_process->state = READY;
    add_ready_process(blocked_process);
    return 0;
}

uint8_t kill_process(pid_t pid){
    PCB* process_to_kill = find_pcb_by_pid(pid);
    
    if (process_to_kill == NULL) return -1;

    State state = process_to_kill->state;
    
    if (state == EXITED) return -1;

    process_to_kill->state = EXITED;

    if (state == READY || state == RUNNING)
        remove_ready_process(process_to_kill);
    

    if (process_to_kill == get_running_process()){
        _irq00Handler();
    }
    
    return 0;
}

uint8_t modify_process_priority(pid_t pid, uint32_t priority){
    PCB* process = find_pcb_by_pid(pid);

    if (process == NULL) return -1;

    State state = process->state;

    if (state == EXITED) return -1;

    remove_ready_process(process);

    process->priority = priority;

    add_ready_process(process);

    return 0;
}

PCB* find_pcb_by_pid(pid_t pid){
    return &process_table[pid];
}

pid_t get_pid(){
    PCB* running_process = get_running_process();
    return running_process->pid;
}