#ifndef __SHELL__
#define __SHELL__

#define MAX_SHELL_INPUT 1024

#include <process.h>

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
void test_memory();
void block_second_process();
void unblock_second_process();
void get_pid();
void create_process_1();
void create_process_2();


void run_shell();

#endif
