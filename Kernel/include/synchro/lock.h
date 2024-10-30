#ifndef __LOCK_H__
#define __LOCK_H__

#include <stdint.h>

extern void acquire(uint8_t *lock);
extern void release(uint8_t *lock);

#endif