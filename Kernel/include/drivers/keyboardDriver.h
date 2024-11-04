#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

#include <scheduler/process_manager.h>

#define CAPS_LOCK_CODE_PRESSED 0x3A
#define LEFT_SHIFT_CODE_PRESSED 0x36
#define RIGHT_SHIFT_CODE_PRESSED 0x2A
#define LEFT_SHIFT_CODE_RELEASED 0xAA
#define RIGHT_SHIFT_CODE_RELEASED 0xB6
#define ESCAPE_CODE_PRESSED 0x01
#define CTRL_CODE_PRESSED 0x1D
#define CTRL_CODE_RELEASED 0x9D
#define C_CODE_PRESSED 0x2E


#define MAX_SIZE_KEY_BUFFER 100

char init_keyboard_driver();
void keyboard_handler();
int keys_pending();
uint8_t get_key_pending();
uint8_t get_scan_code();
char get_pressed_character();
int read_from_keyboard(char* buffer, int buffer_size);

#endif
