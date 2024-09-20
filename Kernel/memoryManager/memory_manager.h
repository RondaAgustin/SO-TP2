#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__

#define BLOCK_SIZE 1024
#define BLOCK_COUNT 10

#define RESERVED 1
#define FREE 0

void * malloc();
void free(void *p);

#endif