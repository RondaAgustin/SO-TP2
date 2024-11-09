// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <process.h>

uint64_t process(int argc, char *argv[]) {
    char* message = argv[0];
    while(1) {
        puts_with_color(message, 0xFF0000);
    }
}