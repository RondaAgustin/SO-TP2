#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define BLOCK_SIZE 1024
#define BLOCK_COUNT 10

#define RESERVED 1
#define FREE 0

void mm_init(void * p, uint32_t s);
void * mm_malloc(uint32_t size);
void mm_free(void *p);
uint32_t mm_get_total_memory();
uint32_t mm_get_used_memory();
uint32_t mm_get_free_memory();

#endif