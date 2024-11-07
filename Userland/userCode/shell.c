// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>

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
    {"nice", "change process priority", modify_priority},
    {"process1", "process that prints Process 1", create_process_1},
    {"process2", "process that prints Process 2", create_process_2},
    {"loop", "imprime su pid en bucle", loop},
    {"ps", "prints processes list and their details", ps},
    {"test_synchro", "test sync", test_synchro},
    {"test_no_synchro", "test no sync", test_no_synchro},
    {"mem", "display memory info", mem_info},
    {"producer", "prints to stdout every 1.5s", producer},
    {"consumer", "reads from stdin and prints", consumer},
    {"cat", "prints data received by stdin", cat},
    {"wc", "prints number of lines, words and chars of input", wc},
    {"filter", "shows only vocals from input", filter},
    {"filter_2", "shows only consonants from input", filter2},
    {"phylo", "starts philosophers problem", phylo},
};

static int current_font_size = 1;

int64_t execute_command(char* cmd_name, uint32_t priority, uint8_t foreground, char fds[]) {
    for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++) {
        if (strcmp(cmd_name, modules[i].module_name) == 0){
            char* argv[] = {modules[i].module_name, NULL};
            return sys_create_process((uint64_t) modules[i].module, 1, argv, priority, foreground, fds);
        }
    }
    return -1;
}

void run_shell() {

    char shell_input[MAX_SHELL_INPUT];
    shell_input[0] = 0;

    char **shell_args = sys_mm_malloc(MAX_SHELL_ARGS * sizeof(char*));
    shell_args[0] = sys_mm_malloc(MAX_SHELL_INPUT * sizeof(char));
    shell_args[1] = sys_mm_malloc(MAX_SHELL_INPUT * sizeof(char));
    shell_args[2] = sys_mm_malloc(MAX_SHELL_INPUT * sizeof(char));


    int shell_args_count = 0;

    uint8_t foreground = 1;
    uint32_t priority = 1;

    while (strcmp(shell_input, "exit") != 0) {
        sys_set_font_size(current_font_size);
        puts_with_color("shell> ", 0x006fb5fb);
        scanf("%s", shell_input);
        
        shell_args_count = split(shell_input, ' ', shell_args, MAX_SHELL_ARGS);

        if(shell_args_count == 3 && strcmp(shell_args[1], "|") == 0) {
            char pipe = sys_pipe("shell_pipe");

            // Tengo que crear el proceso de la izquierda con la salida redirigida al pipe
            char fds_left[2] = {STDIN, pipe};
            int64_t pid_left = execute_command(shell_args[0], 1, 0, fds_left);

            if (pid_left == -1) {
                sys_pipe_close(pipe);
                continue;
            }

            // Tengo que crear el proceso de la derecha con la entrada redirigida al pipe
            char fds_right[2] = {pipe, STDOUT};
            int64_t pid_right = execute_command(shell_args[2], 1, 1, fds_right);

            if (pid_right == -1) {
                sys_kill_process(pid_left);
                sys_pipe_close(pipe);
                continue;
            }

            sys_wait(pid_left);
            sys_kill_process(pid_right);
            sys_pipe_close(pipe);
        } else {
            if(shell_args_count > 1 && strcmp(shell_args[1], "&") == 0) {
                priority = 1;
                foreground = 0;
            } else {
                priority = 20;
                foreground = 1;
            }

            int64_t pid = execute_command(shell_args[0], priority, foreground, NULL);
            if (pid != -1) {
                if (foreground) {
                    sys_wait(pid);
                }
            }
        }
    }

    sys_mm_free(shell_args[0]);
    sys_mm_free(shell_args[1]);
    sys_mm_free(shell_args[2]);
    sys_mm_free(shell_args);
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
    int dir = 0;
    printf("Address: ");
    scanf("%d", &dir);
    printf("Jumping to address %d\n", dir);
    jump_to_dir(dir);
}

void ps() {
    sys_ps();
}

void test_memory(){
    char* argv[] = {"20971520", NULL};
    test_mm(1, argv);
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
    char* argv[] = {"Process 1", NULL};
    process(1, argv);
}

void create_process_2(){
    char* argv[] = {"Process 2", NULL};
    process(1, argv);
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

void loop(){
    while (1){
        printf("Loop process pid: %d\n", sys_get_pid());
    }
}

void get_pid(){
    printf("Shell pid = %d\n", sys_get_pid());
}

void test_scheduler_processes(){
    char* argv[] = {"60", NULL};
    test_processes(1, argv);
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
    char* argv[] = {"test_prio", NULL};
    test_prio(1, argv);
}

void test_synchro(){
    char* argv[] = {"5", "1", NULL};
    test_sync(2 ,argv);
}

void test_no_synchro(){
    char* argv[] = {"5", "0", NULL};
    test_sync(2, argv);
}

void mem_info(){
    printf("Total memory: %d bytes\n", sys_mm_get_total_memory());
    printf("Used memory: %d bytes\n", sys_mm_get_used_memory());
    printf("Free memory: %d bytes\n", sys_mm_get_free_memory());
}

void cat() {
    char buffer[2048] = {0};
    int i = 0;
    while (1) {
        i = sys_read(STDIN, buffer, 2048);
        if (i == -1) {
            break;
        }
        sys_write(STDOUT, buffer, i, 0x00FFFFFF);
    }
}

void wc() {
    char buffer[2048] = {0};
    int i = 0;
    int words = 0;
    int lines = 0;
    int chars = 0;
    
    while (1) {
        i = sys_read(STDIN, buffer, 2048);
        if (i <= -1) {
            break;
        }
        for (int j = 0; j < i; j++) {
            if (buffer[j] == ' ' || buffer[j] == '\n') {
                words++;
            }
            if (buffer[j] == '\n') {
                lines++;
            }
            chars++;
        }
    }
    
    printf("\nWords: %d\n", words);
    printf("Lines: %d\n", lines);
    printf("Chars: %d\n", chars);
}

void filter() {
    char buffer[2048] = {0};
    int i = 0;
    while (1) {
        i = sys_read(STDIN, buffer, 2048);
        if (i <= 0) {
            break;
        }
        for (int j = 0; j < i; j++) {
            if (buffer[j] == 'a' || buffer[j] == 'e' || buffer[j] == 'i' || buffer[j] == 'o' || buffer[j] == 'u') {
                sys_write(STDOUT, buffer + j, 1, 0x00FFFFFF);
            }
        }
    }
}

void filter2() {
    char buffer[2048] = {0};
    int i = 0;
    while (1) {
        i = sys_read(STDIN, buffer, 2048);
        if (i <= 0) {
            break;
        }
        for (int j = 0; j < i; j++) {
            if (buffer[j] != 'a' && buffer[j] != 'e' && buffer[j] != 'i' && buffer[j] != 'o' && buffer[j] != 'u') {
                sys_write(STDOUT, buffer + j, 1, 0x00FFFFFF);
            }
        }
    }
}

void producer() {
    int count = 0;
    while (count < 10) {
        char* data = "test";
        sys_write(STDOUT, data, strlen(data), 0x00FF0000);
        count++;
        sys_delay(1500);
    }
}

void consumer() {
    int i = 0;
    while (1) {
        char data[5] = {0};
        i = sys_read(STDIN, data, 5);
        if(i <= 0) {
            break;
        }
        sys_write(STDOUT, data, i, 0x0000FF00);
    }
}

void phylo() {
    start_phylos();
}