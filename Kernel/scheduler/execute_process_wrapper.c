#include <scheduler/execute_process_wrapper.h>
#include <scheduler/process_manager.h>

char execute_process_wrapper(uint64_t entry_point, uint64_t argc, char** argv, uint64_t priority) {
    return create_process((uint64_t) _main, entry_point, argc, argv, priority);
}

void _main(Function entry_point, int argc, char* argv[], pid_t pid) {
    entry_point(argc, argv);
    kill_process(pid);
    return;
}