#include <synchro/synchro.h>

Semaphore* semaphores;

int cmpProsId(const DataType a, const DataType b) {
    return a == b ? 0 : 1;            
}

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
            semaphores[i].blockedProcesses = list_create();
            return i;
        }
    }
    return -1;
}

char sem_close(char semId) {
    if(semaphores[semId].open == 0) return -1;
    semaphores[semId].open = 0;
    semaphores[semId].value = 0;
    list_destroy(semaphores[semId].blockedProcesses, NULL);
    semaphores[semId].blockedProcesses = NULL;
    semaphores[semId].lock = 0;
    return 0;
}

void sem_post(char semId) {
    Semaphore* sem = &semaphores[semId];
    if (sem->open){
        acquire(&sem->lock);
        sem->value++;
        if (list_size(sem->blockedProcesses) > 0) {
            pid_t pid = (pid_t) list_get_first(sem->blockedProcesses);
            list_remove(sem->blockedProcesses, (DataType) pid, cmpProsId);
            unblock_process(pid);
        }
        release(&sem->lock);
    }
    
}

void sem_wait(char semId) {
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
            list_add(sem->blockedProcesses, (DataType) pid);
            release(&(sem->lock));
            block_process(pid);
        }
    }
}