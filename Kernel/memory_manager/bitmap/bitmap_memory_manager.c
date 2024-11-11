// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memory_manager/memory_manager.h>

#define BLOCK_SIZE 1024
#define BLOCK_COUNT MEM_SIZE / BLOCK_SIZE

#define RESERVED 1
#define FREE 0

typedef struct MemoryZone {
    void * start;
    uint32_t size;
} MemoryZone;

typedef struct MemoryManager {
    void * start;
    uint32_t size;
    uint32_t usedMemory;

    MemoryZone reservedMemoryZone[BLOCK_COUNT];

    unsigned char bitmap[BLOCK_COUNT];
} MemoryManager;

MemoryManager memory_manager;

int8_t mm_init(void * p, uint32_t s) {
    memory_manager.start = p;
    memory_manager.size = s; 
    memory_manager.usedMemory = 0;

    for (int i = 0; i < BLOCK_COUNT; i++) {
        MemoryZone memory_zone;
        memory_zone.size = 0;
        memory_zone.start = NULL;
        memory_manager.reservedMemoryZone[i] = memory_zone;
        memory_manager.bitmap[i] = FREE;
    }
    
    return 0;
}

void * mm_malloc(uint32_t size) {
    uint32_t c = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    
    uint32_t i, j, flag = 0;

    for (i = 0; i < BLOCK_COUNT; i++) {
        for (j = 0; j < c; j++) {
            if (i + j >= BLOCK_COUNT) {
                return NULL;
            }
            if (memory_manager.bitmap[i+j] == RESERVED) {
                break;
            }
        }
        if(j == c) {
            flag = 1;
            break;
        }
    }
    if (flag) {
        for (j = 0; j < c; j++) {
            memory_manager.bitmap[i+j] = RESERVED;
        }

        void * response = memory_manager.start + i * BLOCK_SIZE;

        MemoryZone memory_zone;

        memory_zone.size = c;
        memory_zone.start = response;

        memory_manager.reservedMemoryZone[i] = memory_zone;

        memory_manager.usedMemory += c * BLOCK_SIZE;

        return response;
    }

    return NULL;
}

void mm_free(void * p) {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        if (memory_manager.reservedMemoryZone[i].start == p) {
            for (int j = 0; j < memory_manager.reservedMemoryZone[i].size; j++) {
                memory_manager.bitmap[i+j] = FREE;
                memory_manager.usedMemory -= BLOCK_SIZE;
            }
            memory_manager.reservedMemoryZone[i].size = 0;
            memory_manager.reservedMemoryZone[i].start = NULL;
            break;
        }
    }
}

uint32_t mm_get_total_memory() {
    return memory_manager.size;
}

uint32_t mm_get_used_memory() {
    return memory_manager.usedMemory;
}

uint32_t mm_get_free_memory() {
    return mm_get_total_memory() - mm_get_used_memory();
}