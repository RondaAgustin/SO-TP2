section .text

global sys_read
global sys_write
global sys_set_font_size
global sys_put_text
global sys_draw_square
global sys_get_screen_width
global sys_get_screen_height
global sys_get_time
global sys_get_key_pressed
global sys_get_character_pressed
global sys_clear_text_buffer
global sys_get_cpu_vendor
global sys_beep
global sys_delay
global sys_print_registers
global sys_clear_screen
global sys_mm_malloc
global sys_mm_free
global sys_mm_get_total_memory
global sys_mm_get_used_memory
global sys_mm_get_free_memory
global sys_create_process
global sys_get_pid
global sys_block_process
global sys_unblock_process
global sys_kill_process
global sys_modify_priority
global sys_wait
global sys_yield
global sys_ps
global sys_sem_open
global sys_sem_close
global sys_sem_wait
global sys_sem_post

%macro syscall 1
    push rbp
    mov rbp, rsp

    mov rax, %1 
    mov r10, rcx
    int 0x80

    mov rsp, rbp
    pop rbp
    ret
%endmacro

sys_read:
    syscall 0

sys_write:
    syscall 1

sys_put_text:
    syscall 2

sys_set_font_size:
    syscall 3

sys_draw_square:
    syscall 4

sys_get_screen_width:
    syscall 5

sys_get_screen_height:
    syscall 6

sys_get_time:
    syscall 7

sys_get_key_pressed:
    syscall 8

sys_get_character_pressed:
    syscall 9

sys_clear_text_buffer:
    syscall 10

sys_get_cpu_vendor:
    syscall 11

sys_beep:
    syscall 12

sys_delay:
    syscall 13

sys_print_registers:
    syscall 14

sys_clear_screen:
    syscall 15

sys_mm_malloc:
    syscall 16

sys_mm_free:
    syscall 17

sys_mm_get_total_memory:
    syscall 18

sys_mm_get_used_memory:
    syscall 19
    
sys_mm_get_free_memory:
    syscall 20

sys_create_process:
    syscall 21

sys_get_pid:
    syscall 22

sys_block_process:
    syscall 23

sys_unblock_process:
    syscall 24

sys_kill_process:
    syscall 25

sys_modify_priority:
    syscall 26

sys_wait:
    syscall 27

sys_yield:
    syscall 28

sys_ps:
    syscall 29

sys_sem_open:
    syscall 30

sys_sem_close:
    syscall 31

sys_sem_wait:
    syscall 32

sys_sem_post:
    syscall 33

sys_pipe:
    syscall 34

sys_pipe_open:
    syscall 35

sys_pipe_close:
    syscall 36