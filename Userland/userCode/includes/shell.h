#ifndef __SHELL__
#define __SHELL__

#define MAX_SHELL_INPUT 1024
#define MAX_SHELL_ARGS 3

#include <stdint.h>

#include <syscall_adapters.h>

#include <process.h>
#include <test_processes.h>
#include <test_prio.h>
#include <test_sync.h>

#include <std.h>
#include <cucaracha.h>
#include <eliminator.h>
#include <lib.h>
#include <test_mm.h>
#include <types.h>
#include <phylo.h>

void help();
void cls();
void info();
void font_size();
void time();
void regs();
void beep();
void song();
void calculator();
void eliminator();
void jump();
void ps();
void mem_info();

void test_priority_processes();
void test_scheduler_processes();
void test_memory();
void test_synchro();
void test_no_synchro();

void block_process();
void unblock_process();
void kill_process();

void get_pid();
void create_process_1();
void create_process_2();
void modify_priority();

void loop();

void producer();
void consumer();
void cat();
void wc();
void filter();
void filter2();
void phylo();

int64_t execute_command(char* cmd_name, uint32_t priority, uint8_t foreground, char fds[]);
void run_shell();

#endif
