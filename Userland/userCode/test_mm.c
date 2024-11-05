// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <test_mm.h>

// uint64_t test_mm(uint64_t argc, char *argv[]);

// int main(int argc, char *argv[]) {
//   char buffer[BLOCK_SIZE * BLOCK_COUNT];
//   mm_init(buffer, BLOCK_SIZE * BLOCK_COUNT);
//   test_mm(argc - 1, argv + 1);
//   return 0;
// }

uint64_t test_mm(uint64_t argc, char *argv[]) {
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;

  uint64_t count = 0;
  
  while (1) {
    rq = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && sys_mm_get_free_memory() > 0) {
      mm_rqs[rq].size = GetUniform(sys_mm_get_free_memory() - 1) + 1;
      mm_rqs[rq].address = sys_mm_malloc(mm_rqs[rq].size);

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
          puts("test_mm ERROR\n");
          return -1;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        sys_mm_free(mm_rqs[i].address);
    count++;
  }
}
