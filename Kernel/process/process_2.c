#include <process/process_2.h>

uint64_t process_2(int argc, char* argv[]){
    while (1)
        write_to_video_text_buffer("Process2/n", 10, HEX_RED);
}