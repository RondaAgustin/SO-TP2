// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <drivers/pitDriver.h>
#include <drivers/videoDriver.h>
#include <scheduler/scheduler.h>
#include <lib.h>

extern SchedulerADT my_scheduler;

static uint32_t TICKS_PER_SECOND;
static volatile unsigned long ticks = 0;

static uint64_t UPDATE_SCREEN_RATE;

void initialize_pit(uint32_t frequency){
	TICKS_PER_SECOND = frequency;
    UPDATE_SCREEN_RATE = (TICKS_PER_SECOND >= UPDATE_SCREEN_FREQ) ? (TICKS_PER_SECOND / UPDATE_SCREEN_FREQ) : TICKS_PER_SECOND;
    uint16_t divisor = MAX_FREQUENCY / TICKS_PER_SECOND;
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

uint64_t timer_handler(const uint64_t rsp) {
	ticks++;

    return context_switch(rsp);
}

void delay(uint64_t milis){
	uint64_t start = ticks_elapsed();
	while(ticks - start < (TICKS_PER_SECOND * milis) / 1000);
}

void delay_ticks(uint64_t ticks_to_wait){
	uint64_t start = ticks_elapsed();
	while(ticks - start < ticks_to_wait);
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
