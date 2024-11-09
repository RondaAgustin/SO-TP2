// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscall_adapters.h>
#include <std.h>
#include <shell.h>

static uint8_t show_initial_info = 1;

int main() {

    if (show_initial_info) {
        puts_with_color("[SYSTEM INFO]\n\n", 0x006fb6fb);
        info();
        puts_with_color("[HELP]\n\n", 0x006fb6fb);
        help();
        show_initial_info = 0;
    }

    char* argv_shell[] = {"shell", NULL};
    sys_create_process((uint64_t) run_shell, 1, argv_shell, 20, 0, NULL);

	return 0xDEADBEEF;
}
