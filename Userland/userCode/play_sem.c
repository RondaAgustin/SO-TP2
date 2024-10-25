#include <play_sem.h>

void producer(char sem_id){
        sys_sem_post(0);
        puts("Producer\n");
}

void consumer(char sem_id){
        sys_sem_wait(0);
        puts("Consumer\n");
}

void play_sem(){
    char sem_id = sys_sem_open(0);

    char* argv = sys_mm_malloc(sizeof(char*) * 2);
    argv[0] = "sem_id";
    argv[1] = NULL;

    sys_create_process((uint64_t) consumer, 1, argv, 1);
    sys_create_process((uint64_t) producer, 1, argv, 1);
}