#include <scheduler/idle.h>
#include <drivers/pitDriver.h>


uint64_t idle(int argc, char* argv[]){
    while (1)
    {   
        write_to_video_text_buffer("Proceso\n", 8, HEX_RED);
        delay(10000);
        // _hlt();
    }
}