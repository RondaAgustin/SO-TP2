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
    sys_unblock_process, sys_kill_process, sys_priority_process
};

uint64_t syscall_handler(const registers64_t *registers){
    if (registers->rax >= sizeof(syscalls) / sizeof(syscalls[0]))
            return 0;

    return syscalls[registers->rax](registers->rdi, registers->rsi, registers->rdx,
                                    registers->r10, registers->r8, registers->r9);
}


uint64_t sys_read(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    char c = 0;
    int i = 0;
    char* out_buffer = (char*) rdi;
    while(c != '\n' && i < rsi){
        if(keys_pending()){
            c = get_pressed_character();
            if(c == '\b'){
                char * backs = "\b\b\b\b";
                if(--i < 0) i = 0;
                else write_to_video_text_buffer(backs, out_buffer[i] == '\t'? 4 : 1, HEX_WHITE);
            }
            else{
                out_buffer[i++] = c;
                write_to_video_text_buffer(&c, 1, HEX_WHITE);
            }
        }
    }
    return i;
}

uint64_t sys_write(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    write_to_video_text_buffer((char*)rsi, rdx, rdi);
    return rdx;
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
    return mm_malloc(rdi);
}

uint64_t sys_mm_free(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    mm_free(rdi);
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
    return create_process(rdi, rsi, (char**) rdx, r10);
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