#ifndef __SHARED_PROCESS_STRUCTURES_H__
#define __SHARED_PROCESS_STRUCTURES_H__

#include <stdint.h>

typedef enum State {
    READY,
    RUNNING,
    BLOCKED,
    EXITED
} State;

typedef struct {
    uint32_t pid;
    char father_pid;
    char* process_name;
    uint32_t priority;
    State state;
    uint64_t sp;
    uint64_t base;
    uint64_t limit;
} PCB;

#endif