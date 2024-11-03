#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <drivers/videoDriver.h>
#include <drivers/pitDriver.h>
#include <shell_caller.h>
#include <memoryManager/memory_manager.h>
#include <scheduler/scheduler.h>
#include <synchro/synchro.h>
#include <process/init.h>
#include <process/idle.h>

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

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		SHELL_CODE_ADDRESS,
		SHELL_DATA_ADDRESS
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

extern uint64_t test_mm(uint64_t argc, char *argv[]);

int main() {
	initialize_pit(60);
	load_idt();
    set_font_size(1);
    clear_video_text_buffer();

  	mm_init((void *)0x1000000, MEM_SIZE);

    write_to_video_text_buffer("GRUPO 9\n", 8, 0x006fb5fb);
    write_to_video_text_buffer("Kernel initialized\nRunning user code...\n\n", 41, HEX_GRAY);

	if (init_synchro() == -1)
		write_to_video_text_buffer("Error initializing synchro\n", 28, HEX_RED);

	if (init_scheduler() == -1)
		write_to_video_text_buffer("Error initializing processes\n", 30, HEX_RED);
	else 
		init_processes();

	char *init_argv[] = {"init", NULL};

	execute_process_wrapper((uint64_t) init, 1, init_argv, 1, 0);

	delay(18);

    write_to_video_text_buffer("Back in kernel...\n", 18, HEX_GRAY);

	delay(1000);

	return 0;
}
