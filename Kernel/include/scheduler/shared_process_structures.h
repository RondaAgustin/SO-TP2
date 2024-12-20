#ifndef __SHARED_PROCESS_STRUCTURES_H__
#define __SHARED_PROCESS_STRUCTURES_H__

#include <stdint.h>

#define VALID_CHECK 79

typedef enum State {
    READY,
    RUNNING,
    BLOCKED,
    EXITED
} State;

typedef struct {
    uint32_t pid;
    uint8_t fg;         // 1 if the process is in the foreground, 0 otherwise
    char readfd;
    char writefd;
    char father_pid;
    char* process_name;
    char** argv;
    uint32_t argc;
    uint32_t priority;
    State state;
    uint64_t sp;
    uint64_t base;
    uint64_t limit;
    ListCircularADT processes_blocked_by_me;
    uint64_t last_reference;
    char valid;
} PCB;

#endif