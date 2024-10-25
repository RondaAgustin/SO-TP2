#include <play_sem.h>

void producer(char sem_id){
        int i = 0;
        while (1){
                // sys_yield();
                sys_sem_post(0);
                printf("Producer %d\n", i++);
        }  
}

void consumer(char sem_id){
        int i = 0;
        while (1){
                sys_sem_wait(0);
                printf("Consumer %d\n", i++);
        }
}

void play_sem(){
    char sem_id = sys_sem_open(0);

    if (sem_id == -1)
    {
        printf("Cannot Open Semaphore\n");
    }
    

    char* argv = sys_mm_malloc(sizeof(char*) * 2);
    argv[0] = "sem_id";
    argv[1] = NULL;

        if (sys_create_process((uint64_t) consumer, 1, argv, 10) == -1)
        {
                printf("Cannot Create Consumer\n");
        }
        
        if (sys_create_process((uint64_t) producer, 1, argv, 1) == -1)
        {
                printf("Cannot Create Producer\n");
        }
        

    ;
}