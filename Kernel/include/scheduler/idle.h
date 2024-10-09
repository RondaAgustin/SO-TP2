#ifndef __IDLE__
#define __IDLE__

#include <stdint.h>
#include "interruptHandlers/interrupts.h"
#include "drivers/videoDriver.h"


uint64_t idle(int argc, char* argv[]);

#endif