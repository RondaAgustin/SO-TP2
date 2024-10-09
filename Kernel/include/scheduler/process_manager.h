#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <stdlib.h>
#include <stdint.h>

#define MAX_PROCESSES 64
#define STACK_SIZE 4096

typedef enum State {
    READY,
    RUNNING,
    BLOCKED,
    EXITED
} State;

typedef struct {
    char pid;
    char father_pid;
    char* process_name;
    char priority;
    State state;
    uint64_t sp;
} PCB;

typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
    uint64_t align;
} InitialStack;

extern PCB* process_table;

void create_process(char* process_name, uint64_t entry_point, char* argv[], uint32_t priority);

#endif