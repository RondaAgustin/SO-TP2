#include "syscall.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../memory_manager.h"
#include "test_util.h"

#define MAX_BLOCKS 128

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv[]);

int main(int argc, char *argv[]) {
  char buffer[BLOCK_SIZE * BLOCK_COUNT];
  mm_init(buffer, BLOCK_SIZE * BLOCK_COUNT);
  test_mm(argc - 1, argv + 1);
  return 0;
}

uint64_t test_mm(uint64_t argc, char *argv[]) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc != 1)
    return -1;

  if ((max_memory = satoi(argv[0])) <= 0)
    return -1;

  int count = 0;
  
  while (1) {

    printf("Iteracion %d\n", count++);

    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = mm_malloc(mm_rqs[rq].size);

      if (mm_rqs[rq].address) {
        // Cambiamos esta suma para que se adapte a nuestra forma de reservar memoria en bloques
        total += ((mm_rqs[rq].size + BLOCK_SIZE - 1) / BLOCK_SIZE) * BLOCK_SIZE;
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
