#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <stdint.h>
#include <scheduler/process_manager.h>

#define MAX_SEMAPHORES 64

typedef struct {
    uint8_t lock;
    uint64_t value;
    void * blockedProcesses; // TODO: esto tiene que ser una queue
    char open;
} Semaphore;

void init_synchro();
char sem_open(uint64_t initialValue);
char sem_close(char semId);
void sem_post(char semId);
void sem_wait(char semId);

#endif