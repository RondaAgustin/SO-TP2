#include "memory_manager.h"
#include <stdlib.h>
#include <math.h>

typedef struct MemoryZone
{
    void * start;
    int size;
}MemoryZone;

typedef struct MemoryManager{
    void * start;
    int size;

    MemoryZone reservedMemoryZone[BLOCK_COUNT];
    int reservedMemoryZoneIndex;

    int bitmap[BLOCK_COUNT];
} MemoryManager;

MemoryManager memoryManager;

void mm_init(void * p, int s){
    memoryManager.start = p;
    memoryManager.size = s; 
    memoryManager.reservedMemoryZoneIndex = 0; 

    for (int i = 0; i < BLOCK_COUNT; i++){
        MemoryZone memoryZone;
        memoryZone.size = 0;
        memoryZone.start = NULL;
        
        memoryManager.reservedMemoryZone[i] = memoryZone;
        
        memoryManager.bitmap[i] = FREE;
    }
}

void * malloc(uint32_t size){
    int c = ceil(size / BLOCK_SIZE);
    
    size_t i, j, flag = 0;

    for (i = 0; i < BLOCK_COUNT; i++){
        for (j = 0; j < c; j++){
            if (memoryManager.bitmap[i+j] == RESERVED){
                break;
            }
        }
        if(j == c){
            flag = 1;
            break;
        }
    }
    if (flag){
        for (j = 0; j < c; j++){
            memoryManager.bitmap[i+j] = RESERVED;
        }

        void * response = memoryManager.start + i * BLOCK_SIZE;

        MemoryZone memoryZone;

        memoryZone.size = c;
        memoryZone.start = response;

        memoryManager.reservedMemoryZone[memoryManager.reservedMemoryZoneIndex++] = memoryZone;

        return response;
    }

    return NULL;
}

