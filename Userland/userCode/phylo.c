// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <phylo.h>

#define MAX_PHILOSOPHERS 10
#define INITIAL_PHILOSOPHERS 5

#define PRINT_STATUS_TO_EAT 5

typedef enum {THINKING, EATING, HUNGRY} State;

typedef struct {
    pid_t pid;
    State state;
    uint16_t left_fork;
    uint16_t right_fork;
    uint16_t previous_left_fork;
    uint16_t previous_right_fork;
    uint64_t last_status_print;
} Philosopher;

Philosopher philosophers[MAX_PHILOSOPHERS];

uint16_t print_mutex;
uint16_t philo_qty_mutex; 
uint16_t status_mutex;

uint64_t philo_qty = 0;
uint64_t print_status_counter = 0;

uint64_t right_philosopher(uint64_t index) {
    return (index + 1) % philo_qty;
}

uint64_t left_philosopher(uint64_t index) {
    return (index == 0) ? (philo_qty - 1) : (index - 1);
}


void print_status() {
    sys_sem_wait(print_mutex);
    Philosopher *p;
    
    sys_sem_wait(status_mutex);
    print_status_counter++;
    
    sys_sem_wait(philo_qty_mutex);

    for (int i = 0; i < philo_qty; i++){
        p = &philosophers[i];
        
        switch (p->state) {
            case THINKING:
                printf(". ");
                break;
            case EATING:
                p->last_status_print = print_status_counter;
                printf("E ");
                break;
            case HUNGRY:
                printf(". ");
                break;
        }

        if (print_status_counter - p->last_status_print > PRINT_STATUS_TO_EAT) {
            p->state = HUNGRY;
        }
    }
    
    sys_sem_post(philo_qty_mutex);
    sys_sem_post(status_mutex);
    
    printf("\n");
    
    sys_sem_post(print_mutex);
}

void think() {
    print_status();
    sys_delay(500);
}

void eat() {
    print_status();
    sys_delay(500);
}

uint8_t pick_forks(uint64_t id) {
    Philosopher* p = &philosophers[id];

    sys_sem_wait(status_mutex);
    if (p->state != HUNGRY && (philosophers[left_philosopher(id)].state == HUNGRY || philosophers[right_philosopher(id)].state == HUNGRY)) {
        sys_sem_post(status_mutex);
        return 0;
    }
    sys_sem_post(status_mutex);
    
    uint16_t initial_left_fork;
    uint16_t initial_right_fork;

    if (id % 2 == 0) {
        initial_left_fork = p->left_fork;
        sys_sem_wait(initial_left_fork);
        initial_right_fork = p->right_fork;
        sys_sem_wait(initial_right_fork);
    } else {
        initial_right_fork = p->right_fork;
        sys_sem_wait(initial_right_fork);
        initial_left_fork = p->left_fork;
        sys_sem_wait(initial_left_fork);
    }

    sys_sem_wait(status_mutex); 
    p->state = EATING;
    sys_sem_post(status_mutex);

    p->previous_left_fork = initial_left_fork;
    p->previous_right_fork = initial_right_fork;

    return 1;
}

void put_forks(uint64_t id) {
    Philosopher* p = &philosophers[id];
    
    sys_sem_wait(status_mutex);
    if (p->state != EATING) {
        sys_sem_post(status_mutex);
        return;
    }
    
    sys_sem_post(p->previous_left_fork);
    sys_sem_post(p->previous_right_fork);
    
    p->state = THINKING;
    sys_sem_post(status_mutex);
}

void philosoper(uint64_t argc, char* argv[]) {
    uint64_t id = atoi(argv[1]);

    while (1) {
        think();

        if (pick_forks(id) == 1){
            eat();
        
            put_forks(id);
        }
    }
}

void create_philosopher(uint64_t id) {  
    sys_sem_wait(philo_qty_mutex);

    if (id >= MAX_PHILOSOPHERS) {
        printf("Max philosophers reached\n");
        return;
    }
    
    Philosopher * p = &philosophers[id];

    philo_qty = philo_qty + 1;

    if(id == 0) {
        p->left_fork = sys_sem_open(1);
        p->right_fork = sys_sem_open(1);
    } else if (id == 1) {
        p->left_fork = philosophers[0].right_fork;
        p->right_fork = philosophers[0].left_fork;
    } else {
        Philosopher* right = &philosophers[right_philosopher(id)];
        p->right_fork = right->left_fork;
        Philosopher* left = &philosophers[left_philosopher(id)];
        left->right_fork = sys_sem_open(1);
        p->left_fork = left->right_fork;
    }

    int argc = 2;
    int number_lenght = 3;

    char id_string[number_lenght];
    itoa(id, id_string, number_lenght);
    char* argv[] = {"philosoper", id_string, NULL};

    int aux_pid = sys_create_process((uint64_t) philosoper, argc, argv, 1, 0, NULL);

    p->pid = aux_pid;

    p->state = THINKING;
    p->last_status_print = print_status_counter;

    sys_sem_post(philo_qty_mutex);
}

void remove_philosopher(uint64_t id) {
    sys_sem_wait(philo_qty_mutex);

    if (philo_qty == 0) {
        printf("No philosophers to remove\n");
        return;
    }

    Philosopher* p = &philosophers[id];

    sys_kill_process(p->pid);

    if (p->state == EATING) {   // Tiene los tenedores
        put_forks(id);
    }

    if (id == 0) {
        sys_sem_close(p->left_fork);
        sys_sem_close(p->right_fork);
    } else {
        Philosopher* right = &philosophers[right_philosopher(id)];
        Philosopher* left = &philosophers[left_philosopher(id)];
        left->right_fork = right->left_fork;

        sys_sem_close(p->left_fork);
    }

    p->pid = 0;
    p->state = THINKING;
    p->left_fork = 0;
    p->right_fork = 0;
    p->previous_left_fork = 0;
    p->previous_right_fork = 0;
    p->last_status_print = 0;

    philo_qty = philo_qty - 1;

    sys_sem_post(philo_qty_mutex);
}

void catch_input(){
    char c;
    while (1) {
        c = getchar();
        if (c == 'a' || c == 'A') {
            printf("\n\nAdding philosopher.\n\n");
            create_philosopher(philo_qty);
        } else if (c == 'r' || c == 'R') {
            printf("\n\nRemoving philosopher.\n\n");
            remove_philosopher(philo_qty - 1);
        }    
    }
}

void start_phylos() {
    char print_mutex_aux = sys_sem_open(1);
    char philo_qty_mutex_aux = sys_sem_open(1);
    char status_mutex_aux = sys_sem_open(1);

    if (print_mutex_aux == -1 || philo_qty_mutex_aux == -1 || status_mutex_aux == -1) {
        printf("Error creating mutexs\n");
        return;
    }

    print_mutex = print_mutex_aux;
    philo_qty_mutex = philo_qty_mutex_aux;
    status_mutex = status_mutex_aux;

    for (int i = 0; i < INITIAL_PHILOSOPHERS; i++) {
        create_philosopher(i);
    }

    catch_input();
}