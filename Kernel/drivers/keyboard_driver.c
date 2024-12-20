// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <drivers/keyboard_driver.h>
#include <registers.h>
#include <lib.h>

uint8_t key_buffer[MAX_SIZE_KEY_BUFFER];
char sem;
static int first_key_index = 0;
static int buffer_size = 0;
static int caps_enabled = 0;
static int ctrl_pressed = 0;
const int caps_offset = 85;
static char map_to_ascii[256] = {
    0, '~', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 'c', 
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 's', '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 's', '*', 'a', ' ', 'c',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'n', 's', '7', '8', '9',
    '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\e',
    0, '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 'c', 
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 's', '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 's', '*', 'a', ' ', 'c',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'n', 's', '7', '8', '9',
    '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\e'
};

char init_keyboard_driver() {
    sem = sem_open(1);
    if(sem == -1) {
        return -1;
    }
    return 0;
}

void keyboard_handler(const registers64_t * registers){
    uint8_t scan_code = get_scan_code();
    if(scan_code == CAPS_LOCK_CODE_PRESSED ||
     scan_code == LEFT_SHIFT_CODE_PRESSED || scan_code == RIGHT_SHIFT_CODE_PRESSED ||
     scan_code == LEFT_SHIFT_CODE_RELEASED || scan_code == RIGHT_SHIFT_CODE_RELEASED){
        caps_enabled = !caps_enabled;
        return;
    }
    if(scan_code == ESCAPE_CODE_PRESSED){
        save_registers(registers);
        return;
    } else if (scan_code == CTRL_CODE_PRESSED) { 
        ctrl_pressed = 1;
        return;
    } else if (scan_code == CTRL_CODE_RELEASED) { 
        ctrl_pressed = 0;
        return;
    } else if (ctrl_pressed && (scan_code == C_CODE_PRESSED)) {
        kill_foreground_process();
        return;
    } else if (ctrl_pressed && (scan_code == D_CODE_PRESSED)) {
        scan_code = 84; // EOF (lo llamamos '\e')
    }

    if(scan_code > 0x80 || buffer_size >= MAX_SIZE_KEY_BUFFER) {
        return;
    } 
        
    key_buffer[(first_key_index + buffer_size++) % MAX_SIZE_KEY_BUFFER] = scan_code;
    sem_post(sem);
}

int keys_pending(){
    return buffer_size > 0;
}

uint8_t get_key_pending(){
    if(!keys_pending()) {
        return 0;
    }
    uint8_t key = key_buffer[first_key_index];
    first_key_index = (first_key_index + 1) % MAX_SIZE_KEY_BUFFER;
    buffer_size--;
    return key;
}

char get_pressed_character(){
    while(1) {
        if(keys_pending()) {
            return map_to_ascii[get_key_pending() + caps_enabled * caps_offset];
        } else {
            sem_wait(sem);
        }
    }
}

int read_from_keyboard(char* buffer, int buffer_size) {
    int i = 0;
    char c = 0;
    while (c != '\n' && i < buffer_size) {
        c = get_pressed_character();
        if (c == '\b') {
            char * backs = "\b\b\b\b";
            if (--i < 0) {
                i = 0;
            } else {
                write_to_video_text_buffer(backs, buffer[i] == '\t' ? 4 : 1, HEX_WHITE);
            }
        } else if (c == '\e') {
            return -1;
        } else {
            buffer[i++] = c;
            write_to_video_text_buffer(&c, 1, HEX_WHITE);
        }
    }
    return i;
}