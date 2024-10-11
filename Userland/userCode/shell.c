#include <syscall_adapters.h>
#include <shell.h>
#include <std.h>
#include <cucaracha.h>
#include <eliminator.h>
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
    {"shell_pid", "get shell pid", get_pid},
    {"block_second", "block second process", block_second_process},
    {"unblock_second", "unblock second process", unblock_second_process},
    {"process1", "process that prints Process 1", create_process_1},
    {"process2", "process that prints Process 2", create_process_2},
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

    for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++)
        printf("\t- %s: %s.\n", modules[i].module_name, modules[i].module_description);

    putchar('\n');
}

void cls() {
    sys_clear_text_buffer();
}

void info() {
    puts("screen info:\n");
    printf("width: %d pixels\n", sys_get_screen_width());
    printf("height: %d pixels\n\n", sys_get_screen_height());

    char cpu_vendor_buff[200];
    sys_get_cpu_vendor(cpu_vendor_buff);
    printf("cpu vendor: %s\n\n", cpu_vendor_buff);
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

void test_memory(){
    char *argv[] = {"20971520", NULL};
    test_mm(1, argv);
    // sys_create_process(test_mm, 1, argv, 1);
}

void block_second_process(){
    sys_block_process(2);
}

void unblock_second_process(){
    sys_unblock_process(2);
}

void create_process_1(){
    char* argv_process[] = {"Process 1\n", NULL};
    sys_create_process((uint64_t) process, 1, argv_process, 1);
}

void create_process_2(){
    char* argv_process[] = {"Process 2\n", NULL};
    sys_create_process((uint64_t) process, 1, argv_process, 1);
}

void get_pid(){
    printf("Shell pid = %d\n", sys_get_pid());
}