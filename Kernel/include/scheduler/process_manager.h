#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <stdlib.h>
#include <stdint.h>
#include <types.h>
#include <lib.h>
#include <memoryManager/memory_manager.h>
#include <scheduler/scheduler.h>
#include <scheduler/shared_process_structures.h>

#define MAX_PROCESSES 64
#define STACK_SIZE 4096*2

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

int8_t init_processes();

char create_process(uint64_t wrapper_entry_point, uint64_t entry_point, uint32_t argc, char* argv[], uint32_t priority, uint8_t fg);

int8_t unblock_process(pid_t pid);
int8_t block_process(pid_t pid);
int8_t kill_process(pid_t pid);
int8_t modify_process_priority(pid_t pid, uint32_t priority);
void unblock_waiting_processes(pid_t pid);
void wait(pid_t pid);
void ps();
void kill_foreground_process();

pid_t get_pid();

PCB* find_pcb_by_pid(pid_t pid);

#endif