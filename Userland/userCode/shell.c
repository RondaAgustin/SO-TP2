#include <syscall_adapters.h>
#include <shell.h>
#include <std.h>
#include <cucaracha.h>
#include <eliminator.h>
#include <play_sem.h>
#include <lib.h>
#include <test_mm.h>
#include <types.h>


typedef struct {
    char* module_name;
    char* module_description;
    void (*module)();
} ModuleDescriptor;

ModuleDescriptor modules[] = {
    {"help", "displays available modules", help}, 
    {"clear", "clears the screens text buffer", cls}, 
    {"sysinfo", "displays system information", info},
    {"fontsize", "change font size", font_size},
    {"time", "display current time", time},
    {"regs", "displays captured registers (ESC key to capture)", regs},
    {"beep", "beeps", beep},
    {"song", "plays a short tune while displaying graphics", song},
    {"calculator", "positive integer calculator", calculator},
    {"eliminator", "eliminator game", eliminator},
    {"jump", "jumps to address given by user in decimal (1407583 causes invalid opcode >:) )", jump},
    {"test_mm", "test memory manager", test_memory},
    {"test_processes", "test processes", test_scheduler_processes},
    {"test_prio", "test priority", test_priority_processes},
    {"shell_pid", "get shell pid", get_pid},
    {"block", "block process with specific pid", block_process},
    {"unblock", "unblock process with specific pid", unblock_process},
    {"kill", "kill process with sepecific pid", kill_process},
    {"priority", "change process priority", modify_priority},
    {"process1", "process that prints Process 1", create_process_1},
    {"process2", "process that prints Process 2", create_process_2},
    {"while", "while 1", while_1},
    {"ps", "prints processes list and their details", ps},
    {"test_synchro", "test sync", test_synchro},
    {"test_no_synchro", "test no sync", test_no_synchro},
    {"sem", "play sem", use_play_sem},
    {"mem", "display memory info", mem_info}
    };

static int current_font_size = 1;

void run_shell() {

    char shell_input[MAX_SHELL_INPUT];
    shell_input[0] = 0;

    while (strcmp(shell_input, "exit") != 0) {
        sys_set_font_size(current_font_size);
        puts_with_color("shell> ", 0x006fb5fb);
        scanf("%s", shell_input);
        for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++) 
            if (strcmp(shell_input, modules[i].module_name) == 0)
                modules[i].module();
    }
}

void help() {
    puts("available modules:\n");

    puts("\t- exit: exit to kernel.\n");

    for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++) {
        puts("\t- ");
        puts(modules[i].module_name);
        puts(": ");
        puts(modules[i].module_description);
        putchar('\n');
        // printf("\t- %s: %s.\n", modules[i].module_name, modules[i].module_description);
    }

    putchar('\n');
}

void cls() {
    sys_clear_text_buffer();
}

void info() {
    puts("screen info:\n");
    puts("width: ");
    char buffer[10];
    itoa(sys_get_screen_width(), buffer, 10);
    puts(buffer);
    puts(" pixels\n");
    puts("height: ");
    itoa(sys_get_screen_height(), buffer, 10);
    puts(buffer);
    puts(" pixels\n\n");

    char cpu_vendor_buff[200];
    sys_get_cpu_vendor(cpu_vendor_buff);
    puts("cpu vendor: ");
    puts(cpu_vendor_buff);
    putchar('\n');
    putchar('\n');
}

void font_size(){
    int n = 0;
    printf("Choose font size (1-5) or 0 to exit: ");
    scanf("%d", &n);
    while(n < 0 || n >= 6){
        printf("Choose a valid font size (1-5) or 0 to exit: ");
        scanf("%d", &n);
    }
    if(n != 0){
        current_font_size = n;
        printf("Font size set to %d\n", n);
    }
}

void time(){
    char * time = sys_get_time(-3);
    printf("%s\n", time);
}

void regs(){
    sys_print_registers();
}

void song(){
    play_la_cucaracha();
}

void beep() {
    sys_beep(500, 100);
    sys_beep(2000, 100);
}

void calculator(){
    uint32_t num1 = 0;
    uint32_t num2 = 0;
    uint32_t response = 0;

    char operator;

    printf("Enter only positive numbers.\n");

    printf("First number: ");
    scanf("%d", &num1);
    
    printf("Operator: ");
    scanf("%c", &operator);

    printf("Second number: ");
    scanf("%d", &num2);

    switch (operator) {
        case '+':
            response = num1 + num2;
            break;
        case '-':
            response = num1 - num2;
            break;
        case '*':
            response = num1 * num2;
            break;
        case '/':
            response = num1 / num2;
            break;
        default:
            printf("Invalid operator.\n");
            return;
    }

    printf("Result: %d\n", response);
}

void eliminator() {
    play_eliminator();
}

void jump() {
    uint64_t dir = 0;
    printf("Address: ");
    scanf("%d", &dir);
    printf("Jumping to address %d\n", dir);
    jump_to_dir(dir);
}

void ps() {
    sys_ps();
}

void test_memory(){
    char** argv = sys_mm_malloc(sizeof(char*) * 2);

    argv[0] = "20971520";
    argv[1] = NULL;

    sys_create_process(test_mm, 1, argv, 2);
}

void block_process(){
    pid_t pid;

    printf("Pid to block: ");
    scanf("%d", &pid);

    char result = sys_block_process(pid);

    if (result == 0){
        puts("Process blocked correctly\n");
        return;
    }
    puts("Error to block process\n");
}

void unblock_process(){
    pid_t pid;

    printf("Pid to unblock: ");
    scanf("%d", &pid);

    char result = sys_unblock_process(pid);

    if (result == 0){
        puts("Process unblocked correctly\n");
        return;
    }
    puts("Error to unblock process\n");
    
}

void create_process_1(){
    char** argv_process = sys_mm_malloc(sizeof(char*) * 2);
    argv_process[0] = "Process 1";
    argv_process[1] = NULL;
    sys_create_process((uint64_t) process, 1, argv_process, 1);
}

void create_process_2(){
    char** argv_process = sys_mm_malloc(sizeof(char*) * 2);
    argv_process[0] = "q";
    argv_process[1] = NULL;
    sys_create_process((uint64_t) process, 1, argv_process, 2);
}

void kill_process(){
    pid_t pid;

    printf("Pid to kill: ");
    scanf("%d", &pid);

    char result = sys_kill_process(pid);

    if (result == 0){
        puts("Process killed correctly\n");
        return;
    }
    puts("Error to kill process\n");
    

}

void while_1(){
    while (1){
        puts("While 1\n");
    }
    
}

void get_pid(){
    printf("Shell pid = %d\n", sys_get_pid());
}

void test_scheduler_processes(){
    char** argv = sys_mm_malloc(sizeof(char*) * 2);
    argv[0] = "61";
    argv[1] = NULL;

    sys_create_process((uint64_t) test_processes, 1, argv, 5);
}

void modify_priority(){
    pid_t pid;
    uint32_t priority;

    printf("Pid to modify: ");
    scanf("%d", &pid);

    printf("New priority: ");
    scanf("%d", &priority);

    char result = sys_modify_priority(pid, priority);

    if (result == 0){
        puts("Priority modified\n");
        return;
    }
    puts("Error to modify priority\n");
}

void test_priority_processes() {
    char** argv = sys_mm_malloc(sizeof(char*) * 2);
    argv[0] = "test_prio";
    argv[1] = NULL;

    sys_create_process((uint64_t) test_prio, 1, argv, 5);
}

void use_play_sem(){
    puts("Playing sem\n");

    char** argv = sys_mm_malloc(sizeof(char*) * 2);
    argv[0] = "play_sem";
    argv[1] = NULL;
    sys_create_process((uint64_t) play_sem, 1, argv, 10);
}

void test_synchro(){
    char** argv = sys_mm_malloc(sizeof(char*) * 2);
    argv[0] = "5";
    argv[1] = "1";
    argv[2] = NULL;

    sys_create_process((uint64_t) test_sync, 2, argv, 1);
}

void test_no_synchro(){
    char** argv = sys_mm_malloc(sizeof(char*) * 2);
    argv[0] = "5";
    argv[1] = "0";
    argv[2] = NULL;

    sys_create_process((uint64_t) test_sync, 2, argv, 1);
}

void mem_info(){
    printf("Total memory: %d bits\n", sys_mm_get_total_memory());
    printf("Used memory: %d bits\n", sys_mm_get_used_memory());
    printf("Free memory: %d bits\n", sys_mm_get_free_memory());
}