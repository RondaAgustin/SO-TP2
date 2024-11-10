#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>

typedef struct {
    uint64_t pid;
    char* process_name;
    char* state;
    int64_t priority;
    uint64_t sp;
    uint64_t bp;
} ProcessInfo;

typedef uint32_t pid_t;
#define NULL ((void*)0)

#endif