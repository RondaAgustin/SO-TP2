#ifndef _IPC_H_
#define _IPC_H_

#include <stdint.h>
#include <drivers/keyboardDriver.h>
#include <synchro/synchro.h>

// Dividimos entre 4 porque cada pipe tiene 2 semaforos, pudiendo llegar a consumir la mitad de los semaforos.
#define MAX_FDS (MAX_SEMAPHORES / 4)

#define STDIN 0
#define STDOUT 1

typedef struct {
    char id;
    char * name;
    char sem_data_available;
    char mutex;
    char * buffer;
    int buffer_size;
    int first_key_index;
} FileDescriptor;

extern FileDescriptor* fds;

char init_fds();
char create_pipe(char * name);
char open_pipe(char * name);
void close_pipe(char id);
FileDescriptor* get_pipe(char id);
int read_from_pipe(char pipe_id, char* buffer, int buffer_size);
int write_to_pipe(char pipe_id, char* buffer, int buffer_size);

#endif