#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__

#define BLOCK_SIZE 1024
#define BLOCK_COUNT 10

#define RESERVED 1
#define FREE 0

void mm_init(void * p, size_t s);
void * mm_malloc(uint32_t size);
void mm_free(void *p);
size_t mm_get_total_memory();
size_t mm_get_used_memory();
size_t mm_get_free_memory();

#endif