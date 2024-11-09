// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <process/init.h>


uint64_t init(int argc, char* argv[]){
    char* argv_idle[] = {"idle", NULL};
    execute_process_wrapper((uint64_t) idle, 1, argv_idle, 1, 0, NULL);

    char* argv_shell[] = {"userland", NULL};
    execute_process_wrapper((uint64_t) start_userland, 1, argv_shell, 20, 0, NULL);

    while (1)
        _hlt();
}