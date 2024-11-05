// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <process/idle.h>



uint64_t idle(int argc, char* argv[]){
    while (1){   
        _hlt();
    }
}