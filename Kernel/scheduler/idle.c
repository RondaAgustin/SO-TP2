#include <scheduler/idle.h>
#include <drivers/videoDriver.h>


uint64_t idle(int argc, char* argv[]){
    while (1){   
        _hlt();
    }
}