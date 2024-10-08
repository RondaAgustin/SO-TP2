#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <drivers/videoDriver.h>
#include <drivers/pitDriver.h>
#include <shell_caller.h>
#include <memoryManager/memory_manager.h>
#include <scheduler/scheduler.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

schedulerADT scheduler = NULL;

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

int main()
{	
	initialize_pit(60);
	load_idt();
    set_font_size(1);
    clear_video_text_buffer();

	// write_to_video_text_buffer("Realizando pruebas de memoria\n", 30, HEX_GRAY);

  	mm_init((void *)0x1000000, BLOCK_COUNT * BLOCK_SIZE);
	// char *argv[] = {"10240"};
  	// uint64_t result = test_mm(1, argv);

	// if(result == -1) 
	// 	write_to_video_text_buffer("Error en las pruebas\n", 21, HEX_RED);

	scheduler = create_scheduler();
	

    write_to_video_text_buffer("GRUPO 9\n", 9, 0x006fb5fb);
    write_to_video_text_buffer("Kernel initialized\nRunning user code...\n\n", 41, HEX_GRAY);

    start_shell();

    write_to_video_text_buffer("Back in kernel...\n", 18, HEX_GRAY);

	delay(1000);

	return 0;
}
