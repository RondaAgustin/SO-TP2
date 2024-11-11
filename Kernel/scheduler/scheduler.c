// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <scheduler/scheduler.h>
#include <interrupt_handlers/interrupts.h>
#include <lib.h>

typedef struct Scheduler {
    ListCircularADT scheduling_process;
    PCB* current;
} Scheduler;

Scheduler* scheduler = NULL;

int cmp(DataType d1, DataType d2){
    return d1 == d2 ? 0 : 1;
}

int8_t init_scheduler(){
    _cli();

    scheduler = mm_malloc(sizeof(Scheduler));
    if (scheduler == NULL) {
        return -1;
    }
    
    scheduler->current = NULL;
    scheduler->scheduling_process = list_create();

    if (scheduler->scheduling_process == NULL) {
        mm_free(scheduler);
        return -1;
    }

    _sti();
    return 0;
}

uint64_t context_switch(uint64_t rsp){
    _cli();
    if (scheduler != NULL) {
        if (scheduler->current != NULL && scheduler->current->base >= rsp && scheduler->current->limit <= rsp){
            scheduler->current->sp = rsp;
            if (scheduler->current->state == RUNNING){
                scheduler->current->state = READY;
            }
        } 
        PCB* temp = list_next(scheduler->scheduling_process);

        if(temp->valid != VALID_CHECK) {
            return rsp;
        }

        while(temp->state != READY) {
            list_remove(scheduler->scheduling_process, temp, cmp);
            temp = list_next(scheduler->scheduling_process);
        }

        scheduler->current = temp;

        scheduler->current->state = RUNNING;
        _sti();
        return scheduler->current->sp;
    }
    _sti();
    return rsp;
}

void add_ready_process(PCB* process_pcb){
    for (int i = 0; i < process_pcb->priority; i++) {
        list_add(scheduler->scheduling_process, process_pcb);  
    }
}

void remove_ready_process(PCB *process_pcb){
    for (int i = 0; i < process_pcb->priority; i++) {
        list_remove(scheduler->scheduling_process, process_pcb, cmp);
    }
}

PCB* get_running_process(){
    return scheduler->current;
}

void yield() {
    delay_ticks(1);
}