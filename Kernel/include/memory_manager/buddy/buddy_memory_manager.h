#ifndef __BUDDY_MEMORY_MANAGER__
#define __BUDDY_MEMORY_MANAGER__

#include <stdint.h>
#include <stdlib.h>
#include <lib.h>

#define MEM_SIZE 4194304
#define MAX_LEVELS 32 

typedef struct BlockHeader {
    struct BlockHeader* next;
    uint8_t level; 
    int free;
} BlockHeader;

typedef struct BuddyManager{
    void* memory_base;
    uint32_t memory_size;    
    BlockHeader* free_lists[MAX_LEVELS];  // Listas de bloques libres por nivel
    uint32_t used_memory;       
} BuddyManager;

int8_t mm_init(void * p, uint32_t s);
void * mm_malloc(uint32_t size);
void mm_free(void *p);
uint32_t mm_get_total_memory();
uint32_t mm_get_used_memory();
uint32_t mm_get_free_memory();

#endif