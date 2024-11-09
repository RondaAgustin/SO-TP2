// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <eliminator.h>

static uint32_t screen_width;
static uint32_t screen_height;

static uint8_t players = 1;

static PlayerData player_1_data = {"PLAYER 1", 8, 0, 0, 0, HEX_RED, 0, 1, 1};
static PlayerData player_2_data = {"PLAYER 1", 8, 0, 0, 0, HEX_GREEN, 0, 1, 1};

static uint8_t speed = 1;

void init_presentation() {
    sys_clear_text_buffer(0);
    sys_set_font_size(3);
    sys_put_text("ELIMINATOR", 10, HEX_RED, 100, 100);
    sys_put_text("by los egrep", 12, HEX_RED, 100, 170);
    sys_put_text("Press any key to start", 23, HEX_RED, 100, 240);

    while(sys_get_key_pressed() == 0);
}

void init_menu() {
    sys_clear_text_buffer(0);

    print_setting("PLAYERS: ", 10, players, HEX_RED, 100, 100);
    print_setting("SPEED: ", 8, speed, HEX_RED, 100, 150);

    sys_put_text("[SPACE] to begin game", 21, HEX_RED, 100, 250);
    sys_put_text("[ENTER] to change", 17, HEX_RED, 100, 300);
    sys_put_text("[X] to exit game", 16, HEX_RED, 100, 350);
}

void change_settings_and_play() {

    sys_clear_text_buffer(0);

    player_1_data.score = 0;
    player_2_data.score = 0;

    sys_put_text("SPEED (1-5): ", 13, HEX_RED, 100, 100);
    char key = 0;
    while(key < '1' || key > '5') {
        key = sys_get_character_pressed();
    }
    speed = key - '0';
    
    print_setting("SPEED (1-5): ", 14, speed, HEX_RED, 100, 100);

    sys_put_text("PLAYERS (1-2): ", 15, HEX_RED, 100, 150);
    key = 0;
    while(key != '1' && key != '2') {
        key = sys_get_character_pressed();
    }
    players = key - '0';
    print_setting("PLAYERS (1-2): ", 16, players, HEX_RED, 100, 150);

    scan_player_name("PLAYER 1: ", 10, 1, &player_1_data);

    if(players == 2) {
        scan_player_name("PLAYER 2: ", 10, 2, &player_2_data);
    }

    sys_put_text("Press any key to start", 22, HEX_RED, 100, 400);

    while(sys_get_character_pressed() == 0);

    play();
}

void scan_player_name(char * text, int len, uint8_t player_number, PlayerData* player) {
    char dest[len + MAX_LENGTH_PLAYER_NAME + 1];
    sys_put_text(text, len, HEX_RED, 100, 250 + 50 * (player_number - 1));
    char key = 0;
    int i;
    for(i = 0; i < MAX_LENGTH_PLAYER_NAME && key != '\n'; i++) {
        key = 0;
        while(!isAlpha(key) && key != '\n') {
            key = sys_get_character_pressed();
        }
        if(key != '\n') player->name[i] = key;
        else player->name[i] = '\0';
        strcat(dest, text, player->name);
        sys_put_text(dest, len + i + 1, HEX_RED, 100, 250 + 50 * (player_number - 1));
    }
    player->name[i] = '\0';
    player->name_len = i;
}

void print_setting(char * text, uint8_t max_text_size, uint8_t value, uint32_t hex_color, uint32_t pos_x, uint32_t pos_y) {
    char dest[max_text_size + 1];
    char number[2] = {value + '0', '\0'};
    strcat(dest, text, number);
    sys_put_text(dest, max_text_size, hex_color, pos_x, pos_y);
}

void changeDirection(char key, int* p1_dir, int* p2_dir){
    switch (key)
    {
    case 0:
        break;
    case W_KEY_CODE_PRESSED:
        if(player_1_data.direction != DOWN_DIRECTION) *p1_dir = UP_DIRECTION;
        break;
    case D_KEY_CODE_PRESSED:
        if(player_1_data.direction != LEFT_DIRECTION) *p1_dir = RIGHT_DIRECTION;
        break;
    case S_KEY_CODE_PRESSED:
        if(player_1_data.direction != UP_DIRECTION) *p1_dir = DOWN_DIRECTION;
        break;
    case A_KEY_CODE_PRESSED:
        if(player_1_data.direction != RIGHT_DIRECTION) *p1_dir = LEFT_DIRECTION;
        break;
    case UP_ARROW_CODE_PRESSED:
        if(player_2_data.direction != DOWN_DIRECTION) *p2_dir = UP_DIRECTION;
        break;
    case RIGHT_ARROW_CODE_PRESSED:
        if(player_2_data.direction != LEFT_DIRECTION) *p2_dir = RIGHT_DIRECTION;
        break;
    case DOWN_ARROW_CODE_PRESSED:
        if(player_2_data.direction != UP_DIRECTION) *p2_dir = DOWN_DIRECTION;
        break;
    case LEFT_ARROW_CODE_PRESSED:
        if(player_2_data.direction != RIGHT_DIRECTION) *p2_dir = LEFT_DIRECTION;
        break;
    default:
        break;
    }
}

void play() {
    int keep_playing = 1;
    while(keep_playing == 1){
        sys_clear_screen(0);

        uint32_t normalized_screen_width = screen_width - screen_width % SQUARE_SIZE;
        uint32_t normalized_screen_height = screen_height - screen_height % SQUARE_SIZE;

        for(uint32_t x = SQUARE_SIZE; x < normalized_screen_width - SQUARE_SIZE; x += SQUARE_SIZE){
            sys_draw_square(HEX_RED, x, SQUARE_SIZE, SQUARE_SIZE);
            sys_draw_square(HEX_RED, x, normalized_screen_height - SQUARE_SIZE * 2, SQUARE_SIZE);
        }

        for(uint32_t y = SQUARE_SIZE; y < normalized_screen_height - SQUARE_SIZE; y += SQUARE_SIZE){
            sys_draw_square(HEX_RED, SQUARE_SIZE, y, SQUARE_SIZE);
            sys_draw_square(HEX_RED, normalized_screen_width - SQUARE_SIZE * 2, y, SQUARE_SIZE);
        }

        uint32_t buffer_width = normalized_screen_width/SQUARE_SIZE - 4;
        uint32_t buffer_height = normalized_screen_height/SQUARE_SIZE - 4;
        uint32_t buffer[buffer_height][buffer_width];
        for(int i=0; i<buffer_height; i++){
            for(int j=0; j<buffer_width; j++){
                buffer[i][j] = 0;
            }
        }

        sys_beep(400, 300);

        player_1_data.x = buffer_width / 2;
        player_1_data.y = STARTING_HEIGHT;
        player_1_data.direction = DOWN_DIRECTION;
        player_1_data.alive = 1;

        player_2_data.x = buffer_width / 2;
        player_2_data.y = buffer_height - 1 - STARTING_HEIGHT;
        player_2_data.direction = UP_DIRECTION;
        player_2_data.alive = 1;

        while(player_1_data.alive == 1 && (players != 2 || player_2_data.alive == 1)) {
            char key;
            int player_1_dir = player_1_data.direction;
            int player_2_dir = player_2_data.direction;
            while((key = sys_get_key_pressed()) != 0){
                changeDirection(key, &player_1_dir, &player_2_dir);
            }
            player_1_data.direction = player_1_dir;
            player_2_data.direction = player_2_dir;

            player_1_data.x += (player_1_data.direction == 1) - (player_1_data.direction == 3);
            player_1_data.y += (player_1_data.direction == 2) - (player_1_data.direction == 0);

            player_2_data.x += (player_2_data.direction == 1) - (player_2_data.direction == 3);
            player_2_data.y += (player_2_data.direction == 2) - (player_2_data.direction == 0);

            if(player_1_data.x < 0 || player_1_data.x > buffer_width - 1 || player_1_data.y < 0 || player_1_data.y > buffer_height - 1 || buffer[player_1_data.y][player_1_data.x] == 1) {
                player_1_data.alive = 0;
            }

            if(player_2_data.x < 0 || player_2_data.x > buffer_width - 1 || player_2_data.y < 0 || player_2_data.y > buffer_height - 1 || buffer[player_2_data.y][player_2_data.x] == 1) {
                player_2_data.alive = 0;
            }

            if(player_1_data.x == player_2_data.x && player_1_data.y == player_2_data.y) {
                player_1_data.alive = 0;
                player_2_data.alive = 0;
            }

            if(player_1_data.alive){
                buffer[player_1_data.y][player_1_data.x] = 1;
                sys_draw_square(player_1_data.color, (player_1_data.x + 2) * SQUARE_SIZE, (player_1_data.y + 2) * SQUARE_SIZE, SQUARE_SIZE);
            }

            if(players == 2 && player_2_data.alive) {
                buffer[player_2_data.y][player_2_data.x] = 1;
                sys_draw_square(player_2_data.color, (player_2_data.x + 2) * SQUARE_SIZE, (player_2_data.y + 2) * SQUARE_SIZE, SQUARE_SIZE);
            }

            sys_delay(MAX_DELAY / speed);
        }
        keep_playing = endGame();
    }
    init_menu();
}

int endGame() {
    sys_beep(400, 300);

    if (players == 2)
    {
        if(player_1_data.alive == 0 && player_2_data.alive == 1) {
            player_2_data.score += 1;
            int n = 1;
            int i;
            for(i=1; n*10 <= player_2_data.score && i<MAX_DIGITS_SCORE; i++) n*=10;
            player_2_data.score_digits = i;
        } else if (player_1_data.alive == 1 && player_2_data.alive == 0) {
            player_1_data.score += 1;
            int n = 1;
            int i;
            for(i=1; n*10 <= player_1_data.score && i<MAX_DIGITS_SCORE; i++) n*=10;
            player_1_data.score_digits = i;
        }
    }
    
    char dest[MAX_LENGTH_PLAYER_NAME + 2 + MAX_DIGITS_SCORE + 1] = {0};
    char number[MAX_DIGITS_SCORE + 1] = {0};
    strcat(dest, player_1_data.name, ": ");
    strcat(dest, dest, itoa(player_1_data.score, number, player_1_data.score_digits));
    sys_put_text(dest, player_1_data.name_len + 2 + player_1_data.score_digits, player_1_data.color, 100, 100);

    if(players == 2) {
        strcat(dest, player_2_data.name, ": ");
        strcat(dest, dest, itoa(player_2_data.score, number, player_2_data.score_digits));
        sys_put_text(dest, player_2_data.name_len + 2 + player_2_data.score_digits, player_2_data.color, 100, 150);
    }

    sys_put_text("Press [SPACE BAR] to play again", 31, HEX_WHITE, 50, 250);
    sys_put_text("Or [ENTER] to go back to menu", 29, HEX_WHITE, 70, 300);

    while(1) {
        char key = sys_get_character_pressed();
        if(key == ' ') {
            return 1;
        } else if (key == '\n') {
            return 0;
        }
    }
}

void play_eliminator() {
    
    screen_width = sys_get_screen_width();
    screen_height = sys_get_screen_height();
    
    init_presentation();
    
    init_menu();
    char key = sys_get_character_pressed();

    while(key != 'x' && key != 'X') {
        if(key == ' ') {
            play();
        } else if (key == '\n') {
            change_settings_and_play();
        }
        key = sys_get_character_pressed();
    }
}
