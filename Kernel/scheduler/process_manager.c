#include <scheduler/process_manager.h>

void create_process(char* process_name, uint64_t entry_point, char* argv[], uint32_t priority) {
    // Busco un lugar libre en la tabla de procesos
    int i;
    for (i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state == EXITED) {
            break;
        }
    }

    // TODO: logica para saber el father_pid
    int father_pid = 0;
    // TODO: logica para saber el father_pid

    process_table[i].pid = i;
    process_table[i].father_pid = father_pid;
    process_table[i].process_name = process_name;
    process_table[i].priority = priority;
    process_table[i].state = READY;
    process_table[i].sp = (uint64_t) mm_malloc(STACK_SIZE) + STACK_SIZE;

    // Inicializamos el stack pointer
    uint64_t* stack_ptr = (uint64_t*) process_table[i].sp;

    // Contamos el número de argumentos
    int argc = 0;
    while (argv != NULL && argv[argc] != NULL) {
        argc++;
    }

    // Colocamos los parámetros adicionales en el stack
    for (int i = argc - 1; i >= 6; i--) {
        stack_ptr--;
        *stack_ptr = (uint64_t) argv[i];
    }

    // Guardamos el stack pointer actual para los registros de interrupción y de estado
    uint64_t rsp_for_interrupts = (uint64_t) stack_ptr;

    // Ajustamos el stack pointer para dejar espacio para InitialStack
    stack_ptr -= sizeof(InitialStack) / sizeof(uint64_t);

    // Inicializamos la estructura InitialStack
    InitialStack* stack = (InitialStack*) stack_ptr;
    stack->rax = 0;
    stack->rbx = 0;
    stack->rcx = 0;
    stack->rdx = 0;
    stack->rbp = 0;
    stack->rdi = 0;
    stack->rsi = 0;
    stack->r8 = 0;
    stack->r9 = 0;
    stack->r10 = 0;
    stack->r11 = 0;
    stack->r12 = 0;
    stack->r13 = 0;
    stack->r14 = 0;
    stack->r15 = 0;

    // Pasamos los primeros 6 argumentos por registros
    if (argc > 0) stack->rdi = (uint64_t) argv[0];
    if (argc > 1) stack->rsi = (uint64_t) argv[1];
    if (argc > 2) stack->rdx = (uint64_t) argv[2];
    if (argc > 3) stack->rcx = (uint64_t) argv[3];
    if (argc > 4) stack->r8 = (uint64_t) argv[4];
    if (argc > 5) stack->r9 = (uint64_t) argv[5];

    // Configuramos los registros de interrupción y de estado
    stack->rsp = (uint64_t) rsp_for_interrupts;
    stack->rip = entry_point;
    stack->cs = 0x08;
    stack->rflags = 0x202;
    stack->ss = 0;

    // Actualizamos el stack pointer del proceso
    process_table[i].sp = (uint64_t) stack_ptr;
}