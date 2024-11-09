// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <drivers/pit_driver.h>
#include <drivers/sound_driver.h>
#include <lib.h>

void beep(uint64_t freq, uint64_t milis) {
    if(freq == 0) return;
    play_sound(freq);
    delay(milis);
    stop_sound();
}
