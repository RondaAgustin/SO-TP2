#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <stdint.h>
#include <scheduler/process_manager.h>
#include <memoryManager/memory_manager.h>
#include <synchro/lock.h>
#include <utils/list.h>

#define MAX_SEMAPHORES 64

typedef struct {
    uint8_t lock;
    uint64_t value;
    ListCircularADT blockedProcesses;
    char open;
} Semaphore;

void init_synchro();
char sem_open(uint64_t initialValue);
char sem_close(uint8_t semId);
void sem_post(uint8_t semId);
void sem_wait(uint8_t semId);

#endif