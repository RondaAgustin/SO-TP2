#ifndef _EXECUTE_PROCESS_LOADER_H_
#define _EXECUTE_PROCESS_LOADER_H_

#include <stdint.h>

typedef uint64_t (*Function)(int, char**);

char execute_process_wrapper(uint64_t entry_point, uint64_t argc, char** argv, uint64_t priority);
void _main(Function entry_point, int argc, char* argv[]);

#endif