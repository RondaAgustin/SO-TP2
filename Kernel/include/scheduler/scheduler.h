#ifndef __SCHEDULER__
#define __SCHEDULER__


#include <stdint.h>

#include <memory_manager/memory_manager.h>

#include <utils/list.h>
#include <types.h>

#include <scheduler/shared_process_structures.h>
#include <scheduler/process_manager.h>
#include <scheduler/execute_process_wrapper.h>

#include <drivers/pit_driver.h>

#include <process/idle.h>
#include <userland_starter.h>

typedef struct Scheduler* SchedulerADT;

int8_t init_scheduler();
uint64_t context_switch(uint64_t rsp);
void add_ready_process(PCB* process_pcb);
void remove_ready_process(PCB *process_pcb);
PCB* get_running_process();
void yield();

#endif