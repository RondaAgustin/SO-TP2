#ifndef __IDLE__
#define __IDLE__

#include <stdint.h>
#include "interrupt_handlers/interrupts.h"
#include "drivers/video_driver.h"

uint64_t idle(int argc, char* argv[]);

#endif