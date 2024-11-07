// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <synchro/synchro.h>

Semaphore* semaphores;

int cmpProsId(const DataType a, const DataType b) {
    return a == b ? 0 : 1;            
}

int8_t init_synchro() {
    semaphores = (Semaphore*) mm_malloc(sizeof(Semaphore) * MAX_SEMAPHORES);
    if (semaphores == NULL) return -1;
    for (int i = 0; i < MAX_SEMAPHORES; i++) {
        semaphores[i].open = 0;
        semaphores[i].lock = 1;
    }
    return 0;
}

char sem_open(uint64_t initialValue) {
    if (semaphores == NULL) return -1;
    
    for (int i = 0; i < MAX_SEMAPHORES; i++) {
        acquire(&semaphores[i].lock);
        if (!semaphores[i].open) {
            semaphores[i].open = 1;
            semaphores[i].lock = 1;
            semaphores[i].value = initialValue;
            semaphores[i].blockedProcesses = list_create();
            release(&semaphores[i].lock);
            return i;
        }
        release(&semaphores[i].lock);
    }
    return -1;
}

char sem_close(uint8_t semId) {
    if (semaphores == NULL) return -1;

    acquire(&semaphores[semId].lock);
    if(semaphores[semId].open == 0) return -1;
    semaphores[semId].open = 0;
    semaphores[semId].value = 0;
    list_destroy(semaphores[semId].blockedProcesses, NULL);
    semaphores[semId].blockedProcesses = NULL;
    release(&semaphores[semId].lock);
    return 0;
}

void sem_post(uint8_t semId) {
    if (semaphores == NULL) return;

    Semaphore* sem = &semaphores[semId];
    if (sem->open){
        acquire(&sem->lock);
        sem->value++;
        if (list_size(sem->blockedProcesses) > 0) {
            pid_t pid = (pid_t)(uint64_t) list_get_first(sem->blockedProcesses);
            list_remove(sem->blockedProcesses, (DataType)(uint64_t) pid, cmpProsId);
            unblock_process(pid);
        }
        release(&sem->lock);
    }
    
}

void sem_wait(uint8_t semId) {
    if (semaphores == NULL) return;

    Semaphore* sem = &semaphores[semId];
    if (sem->open){
        acquire(&(sem->lock));
        while (1) {
            if (sem->value > 0) {
                sem->value--;
                release(&sem->lock);
                return;
            }
            pid_t pid = get_pid();
            list_add(sem->blockedProcesses, (DataType)(uint64_t) pid);
            release(&(sem->lock));
            block_process(pid);
        }
    }
}

int64_t sem_value(uint8_t semId) {
    if (semaphores == NULL || semId >= MAX_SEMAPHORES || !semaphores[semId].open) return -1;
    return semaphores[semId].value;
}