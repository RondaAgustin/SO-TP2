#ifndef __BITMAP_MEMORY_MANAGER__
#define __BITMAP_MEMORY_MANAGER__

#include <stdint.h>
#include <stdlib.h>

// 20MB de memoria
#define BLOCK_SIZE 1024
#define BLOCK_COUNT 20480

#define MEM_SIZE BLOCK_SIZE * BLOCK_COUNT

#define RESERVED 1
#define FREE 0

int8_t mm_init(void * p, uint32_t s);
void * mm_malloc(uint32_t size);
void mm_free(void *p);
uint32_t mm_get_total_memory();
uint32_t mm_get_used_memory();
uint32_t mm_get_free_memory();

#endif