#include <scheduler/scheduler.h>

#include <interruptHandlers/interrupts.h>
#include <drivers/videoDriver.h>

#include <lib.h>

typedef struct Scheduler {
    ListCircularADT scheduling_process;
    PCB* current;
} Scheduler;

Scheduler* scheduler = NULL;

int cmp(DataType d1, DataType d2){
    return d1 == d2 ? 0 : 1;
}


void create_scheduler(){
    _cli();
    scheduler = mm_malloc(sizeof(Scheduler));
    scheduler->current = NULL;
    scheduler->scheduling_process = list_create();

    process_table = mm_malloc(MAX_PROCESSES * sizeof(PCB));
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1;
        process_table[i].state = EXITED;
    }   

    char* argv_shell[] = {"shell", NULL};
    create_process((uint64_t) start_shell, 1, argv_shell, 20);

    char* argv_idle[] = {"idle", NULL};
    create_process((uint64_t) idle, 1, argv_idle, 1);
    
    _sti();
    return scheduler;
}

uint64_t context_switch(uint64_t rsp){
    _cli();
    if (scheduler != NULL) {
        if (scheduler->current != NULL && scheduler->current->base >= rsp && scheduler->current->limit <= rsp){
            scheduler->current->sp = rsp;
            scheduler->current->state = READY;
        }
        scheduler->current = list_next(scheduler->scheduling_process);
        scheduler->current->state = RUNNING;
        // TODO DEJAR DE IMPRIMIR EL NOMBRE DE LOS PROCESOS EXTRAS ACA
        // if (scheduler->current->pid >= 2){
        //     update_screen_text_buffer();
        //     write_to_video_text_buffer(scheduler->current->process_name, 10, HEX_RED);
        // }
        _sti();
        return scheduler->current->sp;
    }
    _sti();
    return rsp;
}

void add_ready_process(PCB* process_pcb){
    for (int i = 0; i < process_pcb->priority; i++)
        list_add(scheduler->scheduling_process, process_pcb);  
}

void remove_ready_process(PCB *process_pcb){
    for (int i = 0; i < process_pcb->priority; i++)
        list_remove(scheduler->scheduling_process, process_pcb, cmp);
    // list_remove_all(scheduler->scheduling_process, process_pcb, cmp);
}

PCB* get_running_process(){
    return scheduler->current;
}