// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <drivers/pit_driver.h>
#include <drivers/keyboard_driver.h>
#include <interrupt_handlers/syscall.h>
#include <stdint.h>

static void (*interrupts[])(const registers64_t *) = {(void *) timer_handler, (void *) keyboard_handler};

uint64_t irq_dispatcher(uint64_t irq, const registers64_t *registers) {
    if (irq >= sizeof(interrupts) / sizeof(interrupts[0]))
        return -1;      // TODO: ver bien que habria que retornar o si habria que generar error

    uint64_t response = (uint64_t) registers;

    if (irq == 0){
        response = timer_handler(response);
    } else {
        interrupts[irq](registers);
    }

    return response;
}

uint64_t soft_int_dispatcher(const registers64_t *registers){
    return syscall_handler(registers);
}
