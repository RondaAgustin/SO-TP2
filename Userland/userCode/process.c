#include <process.h>

uint64_t process(int argc, char *argv[]){
    char* message = argv[0];
    if(strcmp(message, "q") == 0) {
        puts_with_color("Ending process\n", 0xFF0000);
        return 0;
    }
    while(1) {
        puts_with_color(message, 0xFF0000);
    }
}