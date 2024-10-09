#include <scheduler/process_manager.h>
#include <memoryManager/memory_manager.h>

void init_scheduler() {
    process_table = mm_malloc(MAX_PROCESSES * sizeof(PCB));

    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1;
        process_table[i].state = EXITED;
    }

}