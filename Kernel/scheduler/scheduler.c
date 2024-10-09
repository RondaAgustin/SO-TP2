#include <scheduler/scheduler.h>

#include <shell_caller.h>
#include <drivers/videoDriver.h>

typedef struct Scheduler {
    PCB* current;
} Scheduler;

Scheduler* scheduler = NULL;

void create_scheduler(){
    scheduler = mm_malloc(sizeof(Scheduler));
    scheduler->current = NULL;

    process_table = mm_malloc(MAX_PROCESSES * sizeof(PCB));
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1;
        process_table[i].state = EXITED;
    }   

    char* argv[] = {"shell", NULL};
    create_process((uint64_t) start_shell, 1, argv, 1);

    return scheduler;
}

uint64_t context_switch(uint64_t rsp){
    if (scheduler != NULL && scheduler->current != NULL) {
        if (scheduler->current->base >= rsp && scheduler->current->limit <= rsp){
            scheduler->current->sp = rsp;
        }
        return scheduler->current->sp;
    }
    return rsp;
}

void add_ready_process(PCB* process_pcb){
    scheduler->current = process_pcb;
}