#include <stdint.h>
#include <test_prio.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 0  // TODO: Change as required
#define MEDIUM 1  // TODO: Change as required
#define HIGHEST 2 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio() {
    int64_t pids[TOTAL_PROCESSES];
    char *argv[] = {"endless_loop_print", 0};
    uint64_t i;

    puts("CREATING PROCESSES...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = sys_create_process((uint64_t) process, 1, argv, 1);

    bussy_wait(WAIT);
    puts("\nCHANGING PRIORITIES...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sys_modify_priority(pids[i], prio[i]);

    bussy_wait(WAIT);
    puts("\nBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sys_block_process(pids[i]);

    puts("CHANGING PRIORITIES WHILE BLOCKED...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sys_modify_priority(pids[i], MEDIUM);

    puts("UNBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sys_unblock_process(pids[i]);

    bussy_wait(WAIT);
    puts("\nKILLING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sys_kill_process(pids[i]);

    puts("FINISHED");
}