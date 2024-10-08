#include <scheduler/scheduler.h>

typedef struct schedulerCDT{

} schedulerCDT;

schedulerADT create_scheduler(){

    return mm_malloc(sizeof(schedulerCDT));
}

uint64_t* context_switch(schedulerADT schedulerADT, uint64_t* rsp){
    return rsp;
}