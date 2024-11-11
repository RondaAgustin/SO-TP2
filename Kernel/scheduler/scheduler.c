// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <scheduler/scheduler.h>
#include <interrupt_handlers/interrupts.h>
#include <lib.h>

#define TICKS_TO_CHECK_SHELL 110

typedef struct Scheduler {
    ListCircularADT scheduling_process;
    PCB* current;
} Scheduler;

Scheduler* scheduler = NULL;

int cmp(DataType d1, DataType d2){
    return d1 == d2 ? 0 : 1;
}

void check_shell(){
    uint64_t ticks = ticks_elapsed();
    int64_t shell_pid = find_process_by_name("shell");

    if(shell_pid != -1){
        PCB * shell_pcb = find_pcb_by_pid(shell_pid);

        if(shell_pcb != NULL && shell_pcb->state == READY && ticks - shell_pcb->last_reference > TICKS_TO_CHECK_SHELL){
            if (list_contains(scheduler->scheduling_process, shell_pcb, cmp) == 0) {
                list_add(scheduler->scheduling_process, shell_pcb);
            }
        }
    }
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

        check_shell();
    
        PCB* temp = list_next(scheduler->scheduling_process);

        if(temp->valid != VALID_CHECK) {
            return rsp;
        }

        while(temp->state != READY) {
            list_remove(scheduler->scheduling_process, temp, cmp);
            temp = list_next(scheduler->scheduling_process);
        }

        scheduler->current = temp;
        scheduler->current->last_reference = ticks_elapsed();
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