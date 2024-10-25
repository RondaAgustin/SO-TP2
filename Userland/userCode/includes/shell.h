#ifndef __SHELL__
#define __SHELL__

#define MAX_SHELL_INPUT 1024

#include <stdint.h>

#include <process.h>
#include <test_processes.h>
#include <test_prio.h>

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

void test_priority_processes();
void test_scheduler_processes();
void test_memory();

void block_process();
void unblock_process();
void kill_process();

void get_pid();
void create_process_1();
void create_process_2();
void modify_priority();

void while_1();
void use_play_sem();

void run_shell();

#endif
