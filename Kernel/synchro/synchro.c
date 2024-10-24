#include <synchro/synchro.h>

Semaphore* semaphores;

void init_synchro() {}
char sys_sem_open(uint64_t initialValue) {}
void sys_sem_close(char semId) {}
void sys_sem_post(char semId) {}
void sys_sem_wait(char semId) {}