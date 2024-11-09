// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <module_loader.h>
#include <drivers/video_driver.h>
#include <drivers/pit_driver.h>
#include <userland_starter.h>
#include <memory_manager/memory_manager.h>
#include <scheduler/scheduler.h>
#include <synchro/synchro.h>
#include <process/init.h>
#include <process/idle.h>
#include <ipc/ipc.h>
#include <drivers/keyboard_driver.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

SchedulerADT my_scheduler = NULL;

static const uint64_t PageSize = 0x1000;

typedef int (*EntryPoint)();

void load_idt();

void clearBSS(void * bss_address, uint64_t bss_size)
{
	memset(bss_address, 0, bss_size);
}

void * get_stack_base()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initialize_kernel_binary()
{
	void * moduleAddresses[] = {
		USERLAND_CODE_ADDRESS,
		USERLAND_DATA_ADDRESS
	};

	load_modules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return get_stack_base();
}

int main() {
	initialize_pit(60);
	load_idt();
    set_font_size(1);
    clear_video_text_buffer();

  	mm_init((void *)0x1000000, MEM_SIZE);

	if (init_fds() == -1)
		write_to_video_text_buffer("Error initializing file descriptors\n", 36, HEX_RED);

	if (init_synchro() == -1)
		write_to_video_text_buffer("Error initializing synchro\n", 28, HEX_RED);

	if (init_keyboard_driver() == -1)
		write_to_video_text_buffer("Error initializing keyboard driver\n", 35, HEX_RED);

	if (init_scheduler() == -1)
		write_to_video_text_buffer("Error initializing processes\n", 30, HEX_RED);
	else 
		init_processes();

	char *init_argv[] = {"init", NULL};

	char fds[2] = {STDIN, STDOUT};
	execute_process_wrapper((uint64_t) init, 1, init_argv, 1, 0, fds);

	delay(1000);

	return 0;
}
