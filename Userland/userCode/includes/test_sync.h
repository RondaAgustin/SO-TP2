#ifndef __TEST_SYNC_H__
#define __TEST_SYNC_H__

#include <stdint.h>
#include <std.h>
#include <syscall_adapters.h>
#include "test_util.h"

void slowInc(int64_t *p, int64_t inc);
uint64_t my_process_inc(uint64_t argc, char *argv[]);
uint64_t test_sync(uint64_t argc, char *argv[]);

#endif