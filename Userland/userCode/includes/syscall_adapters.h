#ifndef __syscall_adapters__
#define __syscall_adapters__

#include <stdint.h>
#include <types.h>

uint64_t sys_read(char fd, const char* buff, uint64_t len);
uint64_t sys_write(char fd, const char* buff, uint64_t len, uint32_t hexColor);
void sys_put_text(const char* str, uint32_t len, uint32_t hexColor, uint32_t posX, uint32_t posY);
void sys_set_font_size(uint32_t font_size);
void sys_draw_square(uint32_t hexColor, uint32_t posX, uint32_t posY, uint32_t size);
char * sys_get_time(int time_zone);
uint32_t sys_get_screen_width();
uint32_t sys_get_screen_height();
uint8_t sys_get_key_pressed();
char sys_get_character_pressed();
void sys_clear_text_buffer();
void sys_get_cpu_vendor(char* buff);
void sys_beep(uint64_t freq, uint64_t milis);
void sys_delay(uint64_t milis);
void sys_clear_screen(uint32_t clearColor);
void sys_print_registers();
// Memory Manager
void* sys_mm_malloc(uint32_t size);
void sys_mm_free(void* p);
uint32_t sys_mm_get_total_memory();
uint32_t sys_mm_get_used_memory();
uint32_t sys_mm_get_free_memory();
// Process Manager
char sys_create_process(uint64_t entry_point, uint32_t argc, char* argv[], uint32_t priority, uint8_t fg, char fds[]);
pid_t sys_get_pid();
int8_t sys_block_process(pid_t pid);
int8_t sys_unblock_process(pid_t pid);
int8_t sys_kill_process(pid_t pid);
int8_t sys_modify_priority(pid_t pid, uint32_t priority);
void sys_wait(pid_t pid);
void sys_yield();
void sys_ps();
char sys_sem_open(uint64_t initialValue);
void sys_sem_close(char semId);
void sys_sem_post(char semId);
void sys_sem_wait(char semId);
char sys_pipe(char* name);
char sys_pipe_open(char* name);
void sys_pipe_close(char pipeId);
void sys_set_process_readfd(pid_t pid, char pipeId);
void sys_set_process_writefd(pid_t pid, char pipeId);

#endif
