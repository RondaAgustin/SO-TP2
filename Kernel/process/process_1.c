#include <process/process_1.h>

uint64_t process_1(int argc, char* argv[]){
    while (1)
        write_to_video_text_buffer("Process1\n", 10, HEX_RED);
}