#include <process.h>

uint64_t process(int argc, char *argv[]){
    char* message = argv[0];
    while(1) {
        puts(message);
    }
}