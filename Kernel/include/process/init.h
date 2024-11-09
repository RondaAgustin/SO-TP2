#ifndef __INIT_PROCESS_H__
#define __INIT_PROCESS_H__

#include <stdint.h>
#include <scheduler/process_manager.h>
#include <scheduler/execute_process_wrapper.h>
#include <userland_starter.h>

uint64_t init(int argc, char* argv[]);


#endif