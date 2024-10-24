#include <synchro/synchro.h>

Semaphore* semaphores;

void init_synchro() {
    semaphores = (Semaphore*) mm_malloc(sizeof(Semaphore) * MAX_SEMAPHORES);
    for (int i = 0; i < MAX_SEMAPHORES; i++) {
        semaphores[i].open = 0;
    }
}

char sem_open(uint64_t initialValue) {
    for (int i = 0; i < MAX_SEMAPHORES; i++) {
        if (!semaphores[i].open) {
            semaphores[i].open = 1;
            semaphores[i].lock = 1;
            semaphores[i].value = initialValue;
            semaphores[i].blockedProcesses = NULL;
            return i;
        }
    }
    return -1;
}

char sem_close(char semId) {
    if(semaphores[semId].open == 0) return -1;
    semaphores[semId].open = 0;
    semaphores[semId].value = 0;
    semaphores[semId].blockedProcesses = NULL;
    semaphores[semId].lock = 0;
    return 0;
}

void sem_post(char semId) {}

void sem_wait(char semId) {}