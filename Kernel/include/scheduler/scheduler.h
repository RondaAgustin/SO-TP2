#ifndef __SCHEDULER__
#define __SCHEDULER__


#include <stdint.h>

#include <memoryManager/memory_manager.h>

#include <shell_caller.h>
#include <utils/list.h>
#include <types.h>

#include <scheduler/shared_process_structures.h>
#include <scheduler/process_manager.h>
#include <scheduler/idle.h>
#include <scheduler/process.h>

typedef struct Scheduler* SchedulerADT;

void create_scheduler();
uint64_t context_switch(uint64_t rsp);
void add_ready_process(PCB* process_pcb);
void remove_ready_process(PCB *process_pcb);
PCB* get_running_process();

#endif