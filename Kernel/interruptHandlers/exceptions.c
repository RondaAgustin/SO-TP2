// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>
#include <interruptHandlers/exceptions.h>
#include <userland_starter.h>
#include <stddef.h>
#include <scheduler/execute_process_wrapper.h>


void exceptionDispatcher(int id, const registers64_t *registers) {
	_cli();
	write_to_video_text_buffer("\n", 1, HEX_WHITE);
	write_to_video_text_buffer("Exception caught: ", 18, HEX_RED);
	switch (id)
	{
		case 0:
			// Divide by zero
			write_to_video_text_buffer("Divide by zero exception\n", 25, HEX_WHITE);
			break;
		case 6:
			// Invalid opcode
			write_to_video_text_buffer("Invalid opcode exception\n", 25, HEX_WHITE);
			break;
		default:
			break;
	}

	print_registers(registers, HEX_RED);

	int32_t shell_pid = find_process_by_name("shell");
	if(shell_pid != -1) {
		kill_process_in_kernel(shell_pid);
	}

	char* argv_userland[] = {"userland", NULL};
    execute_process_wrapper((uint64_t) start_userland, 1, argv_userland, 20, 0, NULL);
	_sti();

	int32_t running_pid = get_pid();
	kill_process(running_pid);
}
