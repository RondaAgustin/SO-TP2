#include "memory_manager.h"
#include <stdlib.h>
#include <math.h>

typedef struct MemoryZone {
    void * start;
    size_t size;
} MemoryZone;

typedef struct MemoryManager {
    void * start;
    size_t size;
    size_t usedMemory;

    MemoryZone reservedMemoryZone[BLOCK_COUNT];

    unsigned char bitmap[BLOCK_COUNT];
} MemoryManager;

MemoryManager memoryManager;

void mm_init(void * p, size_t s) {
    memoryManager.start = p;
    memoryManager.size = s; 
    memoryManager.usedMemory = 0;

    for (int i = 0; i < BLOCK_COUNT; i++) {
        MemoryZone memoryZone;
        memoryZone.size = 0;
        memoryZone.start = NULL;
        memoryManager.reservedMemoryZone[i] = memoryZone;
        memoryManager.bitmap[i] = FREE;
    }
}

void * mm_malloc(uint32_t size) {
    int c = ceil(size / BLOCK_SIZE);
    
    size_t i, j, flag = 0;

    for (i = 0; i < BLOCK_COUNT; i++) {
        for (j = 0; j < c; j++) {
            if (memoryManager.bitmap[i+j] == RESERVED) {
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
            memoryManager.bitmap[i+j] = RESERVED;
        }

        void * response = memoryManager.start + i * BLOCK_SIZE;

        MemoryZone memoryZone;

        memoryZone.size = c;
        memoryZone.start = response;

        memoryManager.reservedMemoryZone[i] = memoryZone;

        memoryManager.usedMemory += c * BLOCK_SIZE;

        return response;
    }

    return NULL;
}

void mm_free(void * p) {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        if (memoryManager.reservedMemoryZone[i].start == p) {
            for (int j = 0; j < memoryManager.reservedMemoryZone[i].size; j++) {
                memoryManager.bitmap[i+j] = FREE;
                memoryManager.usedMemory -= BLOCK_SIZE;
            }
            memoryManager.reservedMemoryZone[i].size = 0;
            memoryManager.reservedMemoryZone[i].start = NULL;
            break;
        }
    }
}

size_t mm_get_total_memory() {
    return memoryManager.size;
}

size_t mm_get_used_memory() {
    return memoryManager.usedMemory;
}

size_t mm_get_free_memory() {
    return mm_get_total_memory() - mm_get_used_memory();
}