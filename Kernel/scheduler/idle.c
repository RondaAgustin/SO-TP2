#include <scheduler/idle.h>
#include <scheduler/process_manager.h>
#include <scheduler/process.h>
#include <drivers/videoDriver.h>


uint64_t idle(int argc, char* argv[]){
    create_process(process, 1, argv, 3);
    while (1){   
        _hlt();
    }
}