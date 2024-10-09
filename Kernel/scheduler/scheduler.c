#include <scheduler/process_manager.h>
#include <memoryManager/memory_manager.h>
#include <scheduler/scheduler.h>

typedef struct schedulerCDT{

} schedulerCDT;

schedulerADT create_scheduler(){
    process_table = mm_malloc(MAX_PROCESSES * sizeof(PCB));
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1;
        process_table[i].state = EXITED;
    }   
    return mm_malloc(sizeof(schedulerCDT));
}

uint64_t* context_switch(schedulerADT schedulerADT, uint64_t* rsp){
    return rsp;
}