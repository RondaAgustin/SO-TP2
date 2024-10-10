#ifndef __PROCESS__
#define __PROCESS__

#include <stdint.h>
#include "drivers/videoDriver.h"
#include "interruptHandlers/interrupts.h"


uint64_t process(int argc, char *argv[]);
#endif