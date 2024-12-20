#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <stdlib.h>
#include <stdint.h>
#include <types.h>
#include <lib.h>
#include <memory_manager/memory_manager.h>
#include <scheduler/scheduler.h>
#include <scheduler/shared_process_structures.h>
#include <ipc/ipc.h>

#define MAX_PROCESSES 64
#define STACK_SIZE 8192

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

typedef struct {
    uint64_t pid;
    char* process_name;
    char* state;
    int64_t priority;
    uint64_t sp;
    uint64_t bp;
} ProcessInfo;

int8_t init_processes();

char create_process(uint64_t wrapper_entry_point, uint64_t entry_point, uint32_t argc, char* argv[], uint32_t priority, uint8_t fg, char fds[]);

void set_process_readfd(pid_t pid, char fd);
void set_process_writefd(pid_t pid, char fd);
int8_t unblock_process(pid_t pid);
int8_t block_process(pid_t pid);
int8_t modify_process_priority(pid_t pid, uint32_t priority);
void unblock_waiting_processes(pid_t pid);
void wait(pid_t pid);
uint64_t ps(ProcessInfo processes_info[]);

int32_t find_process_by_name(char *name);

int8_t kill_process(pid_t pid);
void kill_foreground_process();
int8_t kill_process_in_kernel(pid_t pid);

pid_t get_pid();
uint64_t get_free_processes();
uint64_t get_existing_processes();

PCB* find_pcb_by_pid(pid_t pid);

#endif