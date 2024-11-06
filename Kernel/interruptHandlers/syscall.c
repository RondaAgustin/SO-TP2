#include <stdint.h>
#include <registers.h>
#include <interruptHandlers/syscall.h>

uint64_t (*syscalls[])(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t) = {
    sys_read, sys_write, sys_put_text, 
    sys_set_font_size, sys_draw_square, sys_get_screen_width, 
    sys_get_screen_height, sys_get_time, sys_get_key_pressed, 
    sys_get_character_pressed, sys_clear_text_buffer, sys_get_cpu_vendor, 
    sys_beep, sys_delay, sys_print_registers, 
    sys_clear_screen, sys_mm_malloc, sys_mm_free, 
    sys_mm_get_total_memory, sys_mm_get_used_memory, sys_mm_get_free_memory,
    sys_create_process, sys_get_pid, sys_block_process,
    sys_unblock_process, sys_kill_process, sys_priority_process,
    sys_wait, sys_yield, sys_ps,
    sys_sem_open, sys_sem_close, sys_sem_wait,
    sys_sem_post, sys_pipe, sys_pipe_open,
    sys_pipe_close, sys_set_process_readfd, sys_set_process_writefd
};

uint64_t syscall_handler(const registers64_t *registers) {
    if (registers->rax >= sizeof(syscalls) / sizeof(syscalls[0])) {
        return 0;
    }

    return syscalls[registers->rax](registers->rdi, registers->rsi, registers->rdx,
                                    registers->r10, registers->r8, registers->r9);
}


// rdi: filedes | rsi: buffer | rdx: max bytes to read
uint64_t sys_read(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    int i = 0;

    PCB* pcb = get_running_process();

    char fd;

    if (rdi == STDIN) {
        fd = pcb->readfd;
    } else {
        fd = rdi;
    }

    if (fd == STDOUT) {
        return 0;
    }

    if (fd == STDIN) {
        i = read_from_keyboard((char*) rsi, rdx);
    } else {
        i = read_from_pipe(fd, (char*) rsi, rdx);
    }

    return i;
}

// rdi: filedes | rsi: buffer | rdx: bytes to write | r10: color
uint64_t sys_write(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    int i = 0;
    
    PCB* pcb = get_running_process();

    char fd;

    if (rdi == STDOUT) {
        fd = pcb->writefd;
    } else {
        fd = rdi;
    }

    if(fd == STDIN) {
        return 0;
    }

    if (fd == STDOUT) {
        write_to_video_text_buffer((char*) rsi, rdx, r10);
        i = rdx;
    } else {
        i = write_to_pipe(fd, (char*) rsi, rdx);
    }

    return i;
}

uint64_t sys_set_font_size(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    set_font_size(rdi);
    return 0;
}

uint64_t sys_put_text(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    draw_string((char*) rdi, rsi, rdx, r10, r8);
    return 0;
}

uint64_t sys_draw_square(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    draw_square(rdi, rsi, rdx, r10);
    return 0;
}

uint64_t sys_get_screen_width(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return get_video_buffer_width();
}

uint64_t sys_get_screen_height(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return get_video_buffer_height();
}

uint64_t sys_get_time(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return (uint64_t) get_time((int) rdi);
}

uint64_t sys_get_key_pressed(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return get_key_pending();
}

uint64_t sys_get_character_pressed(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return get_pressed_character();
}

uint64_t sys_clear_text_buffer(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    clear_video_text_buffer();
    return 0;
}

uint64_t sys_get_cpu_vendor(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    cpuVendor((char*)rdi);
    return 0;
}

uint64_t sys_beep(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    beep(rdi, rsi);
    return 0;
}

uint64_t sys_delay(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    delay(rdi);
    return 0;
}

uint64_t sys_clear_screen(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    clear_screen(rdi);
    return 0;
}

uint64_t sys_print_registers(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    print_stored_registers();
    return 0;
}

uint64_t sys_mm_malloc(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return (uint64_t) mm_malloc(rdi);
}

uint64_t sys_mm_free(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    mm_free((void *) rdi);
    return 0;
}

uint64_t sys_mm_get_total_memory(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return mm_get_total_memory();
}

uint64_t sys_mm_get_used_memory(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return mm_get_used_memory();
}

uint64_t sys_mm_get_free_memory(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return mm_get_free_memory();
}

uint64_t sys_create_process(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return execute_process_wrapper(rdi, rsi, (char**) rdx, r10, r8, (char*)r9);
}

uint64_t sys_get_pid(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return get_pid();
}

uint64_t sys_block_process(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return block_process(rdi);
}

uint64_t sys_unblock_process(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return unblock_process(rdi);
}

uint64_t sys_kill_process(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return kill_process(rdi);
}

uint64_t sys_priority_process(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return modify_process_priority(rdi, rsi);
}

uint64_t sys_wait(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    wait(rdi);
    return 0;
}

uint64_t sys_yield(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    yield();
    return 0;
}

uint64_t sys_ps(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    ps();
    return 0;
}

uint64_t sys_sem_open(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return sem_open(rdi);
}

uint64_t sys_sem_close(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return sem_close(rdi);
}

uint64_t sys_sem_wait(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    sem_wait(rdi);
    return 0;
}

uint64_t sys_sem_post(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    sem_post(rdi);
    return 0;
}

uint64_t sys_pipe(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return create_pipe((char*) rdi);
}

uint64_t sys_pipe_open(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return open_pipe((char*) rdi);
}

uint64_t sys_pipe_close(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    close_pipe((char) rdi);
    return 0;
}

uint64_t sys_set_process_readfd(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    set_process_readfd(rdi, rsi);
    return 0;
}

uint64_t sys_set_process_writefd(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    set_process_writefd(rdi, rsi);
    return 0;
}
