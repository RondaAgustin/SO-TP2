#include <phylo.h>

#define MAX_FILOSOFOS 10
#define THINK_TIME 2000
#define EAT_TIME 3000

pid_t filosofos[MAX_FILOSOFOS];
char tenedores[MAX_FILOSOFOS];
int num_filosofos = 5;
char print_mutex;

typedef enum { ESPERANDO, COMIENDO } Estado;
Estado estados[MAX_FILOSOFOS];

void mostrar_mesa() {
    sys_sem_wait(print_mutex);
    for (int i = 0; i < num_filosofos; i++) {
        if (estados[i] == COMIENDO) {
            printf("E ");
        } else {
            printf(". ");
        }
    }
    printf("\n");
    sys_sem_post(print_mutex);
}

void filosofo(int argc, char *argv[]) {

    int id = atoi(argv[0]);

    int izquierda = id;
    int derecha = (id + 1) % num_filosofos;

    while(1) {
        estados[id] = ESPERANDO;
        mostrar_mesa();
        sys_delay(THINK_TIME);

        sys_sem_wait(tenedores[izquierda]);
        sys_sem_wait(tenedores[derecha]);  

        estados[id] = COMIENDO;
        mostrar_mesa();
        sys_delay(EAT_TIME);

        sys_sem_post(tenedores[izquierda]);
        sys_sem_post(tenedores[derecha]);
    }
}

void crear_filosofo(int i) {
    tenedores[i] = sys_sem_open(1);
    if (tenedores[i] == -1) {
        printf("\n\nError al abrir semaforo\n\n");
        return;
    }
    char buffer[3];
    itoa(i, buffer, 3);
    char* argv[] = {buffer, NULL};
    char pid = sys_create_process((uint64_t)filosofo, 1, argv, 1, 0, NULL);
    if (pid == -1) {
        printf("\n\nError al crear proceso\n\n");
        return;
    }
    filosofos[i] = pid;
}

void agregar_filosofo() {
    if(num_filosofos == MAX_FILOSOFOS) {
        printf("\n\nNo se pueden agregar mas filosofos\n\n");
        return;
    }

    printf("\n\nAgregando filosofo\n\n");
    num_filosofos++;
    crear_filosofo(num_filosofos - 1);
}

void borrar_filosofo() {
    if(num_filosofos == 0) {
        printf("\n\nNo hay filosofos para borrar\n\n");
        return;
    }

    printf("\n\nBorrando filosofo\n\n");
    num_filosofos--;
    sys_kill_process(filosofos[num_filosofos]);
    sys_sem_close(tenedores[num_filosofos]);
}

void capturar_entrada() {
    char c;
    while (1) {
        c = getchar();
        if (c == 'a') {
            agregar_filosofo();
        } else if (c == 'q') {
            borrar_filosofo();
        }
    }
}

void start_phylos() {
    print_mutex = sys_sem_open(1);

    if (print_mutex == -1) {
        return;
    }

    int i;
    for (i = 0; i < num_filosofos; i++) {
        crear_filosofo(i);
    }

    capturar_entrada();
}
