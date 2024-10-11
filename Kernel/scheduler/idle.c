#include <scheduler/idle.h>
#include <scheduler/process_manager.h>
#include <scheduler/process.h>
#include <drivers/videoDriver.h>


uint64_t idle(int argc, char* argv[]){
    while (1){   
        write_to_video_text_buffer("Idle\n", 5, HEX_RED);
        _hlt();
    }
}