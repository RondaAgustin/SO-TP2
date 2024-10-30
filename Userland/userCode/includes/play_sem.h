#ifndef __PLAY_SEM_H__
#define __PLAY_SEM_H__

#include <syscall_adapters.h>
#include <stdint.h>
#include <types.h>
#include <std.h>

void play_sem();
void producer(char sem_id);
void consumer(char sem_id);

#endif