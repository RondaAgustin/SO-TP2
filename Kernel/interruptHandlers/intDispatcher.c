#include <drivers/pitDriver.h>
#include <drivers/keyboardDriver.h>
#include <interruptHandlers/syscall.h>
#include <stdint.h>

static void (*interrupts[])(const registers64_t *) = {timer_handler, keyboard_handler};

uint64_t irqDispatcher(uint64_t irq, const registers64_t *registers) {
    if (irq >= sizeof(interrupts) / sizeof(interrupts[0]))
        return;

    uint64_t response = (uint64_t) registers;

    if (irq == 0){
        response = timer_handler(response);
    } else {
        interrupts[irq](registers);
    }

    return response;
}

uint64_t softIntDispatcher(const registers64_t *registers){
    return syscall_handler(registers);
}
