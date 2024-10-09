#include <scheduler/scheduler.h>

#include <shell_caller.h>
#include <drivers/videoDriver.h>

typedef struct SchedulerCDT{
    PCB* current;
} SchedulerCDT;

SchedulerADT create_scheduler(){
    SchedulerADT scheduler = mm_malloc(sizeof(SchedulerCDT));

    process_table = mm_malloc(MAX_PROCESSES * sizeof(PCB));
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1;
        process_table[i].state = EXITED;
    }   

    char* argv[] = {"idle", NULL};
    create_process((uint64_t) idle, 1, argv, 1);

    return scheduler;
}

uint64_t* context_switch(SchedulerADT scheduler, uint64_t* rsp){
    if (scheduler != NULL){
        return scheduler->current->sp;
    }
    return rsp;
}

void add_ready_process(SchedulerADT scheduler, PCB* process_pcb){
    scheduler->current = process_pcb;
}