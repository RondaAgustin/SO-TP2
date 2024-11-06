#include <ipc/ipc.h>
#include <memoryManager/memory_manager.h>
#include <lib.h>

FileDescriptor* fds;

char init_fds() {
    fds = (FileDescriptor*) mm_malloc(MAX_FDS * sizeof(FileDescriptor));
    
    if (fds == NULL) {
        return -1;
    }

    for (int i = 0; i < MAX_FDS; i++) {
        fds[i].id = i + 2;
        fds[i].name = NULL;
        fds[i].sem_data_available = -1;
        fds[i].mutex = -1;
        fds[i].buffer = NULL;
        fds[i].buffer_size = 0;
        fds[i].first_key_index = 0;
    }

    return 0;
}

char create_pipe(char * name) {
    
    uint32_t len = my_strlen(name);
    char* pipeName = (char*) mm_malloc(len + 1);
    if (pipeName == NULL) {
        return -1;
    }
    my_strcpy(pipeName, name);

    for (int i = 0; i < MAX_FDS; i++) {
        if (fds[i].name == NULL) {
            fds[i].name = pipeName;
            char sem_data_available = sem_open(1);
            if (sem_data_available == -1) {
                mm_free(pipeName);
                return -1;
            }
            fds[i].sem_data_available = sem_data_available;
            char mutex = sem_open(1);
            if (mutex == -1) {
                mm_free(pipeName);
                sem_close(sem_data_available);
                return -1;
            }
            fds[i].mutex = mutex;
            char* buffer = (char*) mm_malloc(MAX_SIZE_KEY_BUFFER);
            if (buffer == NULL) {
                mm_free(pipeName);
                sem_close(sem_data_available);
                sem_close(mutex);
                return -1;
            }
            fds[i].buffer = buffer;
            return i + 2;
        }
    }
    return -1;
}

char open_pipe(char * name) {
    for (int i = 0; i < MAX_FDS; i++) {
        if (fds[i].name != NULL && my_strcmp(fds[i].name, name) == 0) {
            return i + 2;
        }
    }
    return -1;
}

void close_pipe(char pipe_id) {
    uint8_t id = (uint8_t) pipe_id;
    id -= 2;
    if (id < 0 || id >= MAX_FDS) {
        return;
    }
    if (fds[id].name != NULL) {
        mm_free(fds[id].name);
        sem_close(fds[id].sem_data_available);
        sem_close(fds[id].mutex);
        mm_free(fds[id].buffer);
        fds[id].name = NULL;
        fds[id].sem_data_available = -1;
        fds[id].mutex = -1;
        fds[id].buffer = NULL;
        fds[id].buffer_size = 0;
        fds[id].first_key_index = 0;
    }
}

FileDescriptor* get_pipe(char pipe_id) {
    uint8_t id = (uint8_t) pipe_id;
    id -= 2;
    if (id < 0 || id >= MAX_FDS) {
        return NULL;
    }
    if (fds[id].name == NULL) {
        return NULL;
    }
    return &fds[id];
}

int read_from_pipe(char pipe_id, char* buffer, int buffer_size) {
    uint8_t id = (uint8_t) pipe_id;
    FileDescriptor* pipe = get_pipe(id);
    if (pipe == NULL) {
        return 0;
    }
    sem_wait(pipe->sem_data_available);
    sem_wait(pipe->mutex);
    int i = 0;
    while (i < buffer_size && pipe->buffer_size > 0) {
        buffer[i++] = pipe->buffer[pipe->first_key_index];
        pipe->first_key_index = (pipe->first_key_index + 1) % MAX_SIZE_KEY_BUFFER;
        pipe->buffer_size--;
    }
    sem_post(pipe->mutex);
    return i;
}

int write_to_pipe(char pipe_id, char* buffer, int buffer_size) {
    uint8_t id = (uint8_t) pipe_id;
    FileDescriptor* pipe = get_pipe(id);
    if (pipe == NULL) {
        return 0;
    }
    sem_wait(pipe->mutex);
    for (int i = 0; i < buffer_size; i++) {
        pipe->buffer[(pipe->first_key_index + pipe->buffer_size++) % MAX_SIZE_KEY_BUFFER] = buffer[i];
    }
    sem_post(pipe->mutex);
    sem_post(pipe->sem_data_available);
    return buffer_size;
}
