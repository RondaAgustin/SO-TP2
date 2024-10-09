#include "./test_util.h"
#include <syscall_adapters.h>
#include <std.h>

#define MAX_BLOCKS 128
#define BLOCK_SIZE 1024
#define BLOCK_COUNT 20480

uint64_t test_mm(uint64_t argc, char *argv[]);

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;