#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "../bitmapManager/bitmap_memory_manager.h"
#include "../buddyManager/buddy_memory_manager.h"
#include "test_util.h"

#define MAX_BLOCKS 128

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv[]);

int main(int argc, char *argv[]) {
  char buffer[MEMORY_SIZE];
  mm_init(buffer, MEMORY_SIZE);

  test_mm(argc - 1, argv + 1);
  return 0;
}

uint64_t test_mm(uint64_t argc, char *argv[]) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;

  int count = 0;
  
  while (1) {
    printf("Iteracion %d\n", count++);

    rq = 0;

    // Request as many blocks as we can    
    while (rq < MAX_BLOCKS && mm_get_free_memory() > 0) {
      mm_rqs[rq].size = GetUniform(mm_get_free_memory() - 1) + 1;
      mm_rqs[rq].address = mm_malloc(mm_rqs[rq].size);

      if (mm_rqs[rq].address) {
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          printf("test_mm ERROR\n");
          return -1;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        mm_free(mm_rqs[i].address);

  }
}
