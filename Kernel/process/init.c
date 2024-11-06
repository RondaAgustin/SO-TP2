#include <process/init.h>


uint64_t init(int argc, char* argv[]){
    char* argv_idle[] = {"idle", NULL};
    execute_process_wrapper((uint64_t) idle, 1, argv_idle, 1, 0, NULL);

    char* argv_shell[] = {"start_shell", NULL};
    execute_process_wrapper((uint64_t) start_shell, 1, argv_shell, 20, 0, NULL);

    while (1)
        _hlt();
}