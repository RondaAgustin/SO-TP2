#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__

#define MEM_SIZE 4194304

#include <stdint.h>
#include <stdlib.h>
#include <lib.h>

int8_t mm_init(void * p, uint32_t s);
void * mm_malloc(uint32_t size);
void mm_free(void *p);
uint32_t mm_get_total_memory();
uint32_t mm_get_used_memory();
uint32_t mm_get_free_memory();

#endif