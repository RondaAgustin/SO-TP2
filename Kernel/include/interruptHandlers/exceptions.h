#ifndef __exceptions__h__
#define __exceptions__h__

#include <registers.h>
#include <scheduler/process_manager.h>

void exceptionDispatcher(int id, const registers64_t *registers);

#endif