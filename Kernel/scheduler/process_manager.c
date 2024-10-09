#include <scheduler/process_manager.h>

PCB* process_table;
extern SchedulerADT my_scheduler;

char create_process(uint64_t entry_point, uint32_t argc, char* argv[], uint32_t priority) {
    
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
    process_table[i].sp = (uint64_t) mm_malloc(STACK_SIZE) + STACK_SIZE;
    
    // Inicializamos el stack pointer
    uint64_t* stack_ptr = (uint64_t*) process_table[i].sp;

    // Ajustamos el stack pointer para dejar espacio para InitialStack
    stack_ptr -= sizeof(InitialStack) / sizeof(uint64_t);

    // Inicializamos la estructura InitialStack
    InitialStack* stack = (InitialStack*) stack_ptr;
    stack->rax = 0;
    stack->rbx = 0;
    stack->rcx = 0;
    stack->rdx = 0;
    stack->rbp = process_table[i].sp - sizeof(uint64_t);
    stack->rdi = argc;
    stack->rsi = argv;
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
    stack->rip = entry_point;
    stack->cs = 0x08;
    stack->rflags = 0x202;
    stack->ss = 0;
    stack->align = 0;

    // Actualizamos el stack pointer del proceso
    process_table[i].sp = (uint64_t) stack_ptr;

    add_ready_process(my_scheduler, &process_table[i]);
    return 0;
}