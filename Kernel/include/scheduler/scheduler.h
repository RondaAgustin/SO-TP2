#ifndef __SCHEDULER__
#define __SCHEDULER__

#include <stdint.h>
#include <memoryManager/memory_manager.h>

typedef struct schedulerCDT* schedulerADT;

schedulerADT create_scheduler();
uint64_t* context_switch(schedulerADT schedulerADT, uint64_t* rsp);

#endif