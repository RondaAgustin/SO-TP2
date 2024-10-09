#ifndef __SCHEDULER__
#define __SCHEDULER__


#include <stdint.h>

#include <memoryManager/memory_manager.h>

#include <utils/list.h>

#include <scheduler/shared_process_structures.h>
#include <scheduler/process_manager.h>
#include <scheduler/idle.h>

typedef struct SchedulerCDT* SchedulerADT;

SchedulerADT create_scheduler();
uint64_t* context_switch(SchedulerADT scheduler, uint64_t* rsp);
void add_ready_process(SchedulerADT scheduler, PCB* process_pcb);

#endif