// March 12, 2025

//              ╔═══════╗ ═════════════════════════════════════════════════════════════════════════════════════════ ╔═══════╗
//              ║▓▓▓▓▓▓▓║                                                                                           ║▓▓▓▓▓▓▓║
//               ▓  ║  ▓    ╔████  ╔████ ╔██   ╔██  ╔████████  ╔██████  ╔██      ╔██  ╔████████  ╔██      ╔██████    ▓  ║  ▓ 
//                ║ ║ ║     ║██║██║██║██  ║██ ║██   ║██          ║██     ║██    ║██   ║██   ║██  ║██      ║██         ║ ║ ║  
//                ║ ║ ║     ║▓▓ ║▓▓▓ ║▓▓   ║▓▓▓▓    ║▓▓▓▓▓▓▓▓    ║▓▓      ║▓▓  ║▓▓    ║▓▓▓▓▓▓▓▓  ║▓▓      ║▓▓▓▓       ║ ║ ║  
//                ║ ║ ║     ║▓▓  ║▓  ║▓▓    ║▓▓           ║▓▓    ║▓▓       ║▓▓║▓▓     ║▓▓   ║▓▓  ║▓▓      ║▓▓         ║ ║ ║  
//               ╠═════╣    ║▒▒  ╚╝  ║▒▒    ║▒▒     ╔▒▒▒▒▒▒▒▒    ║▒▒        ║▒▒▒      ║▒▒   ║▒▒  ║▒▒▒▒▒▒  ║▒▒▒▒▒▒    ╠═════╣ 
//              ║▒▒▒▒▒▒▒║   ╚═╝      ╚═╝    ╚═╝     ╚═══════╝    ╚═╝        ╚══╝      ╚═╝   ╚═╝  ╚═════╝  ╚═════╝   ║▒▒▒▒▒▒▒║
//              ╚═══════╝ ═════════════════════════════════════════════════════════════════════════════════════════ ╚═══════╝

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>
#include <signal.h>

#define size_4 4
#define size_5 5
#define expanse_size 5
#define menu_size 3
#define skill_size 3
#define race_size 4
#define class_size 4
#define place_size 11

#define tier1_diff 9

#define TERMINAL_WIDTH 130
#define TERMINAL_HEIGHT 25

#define ESC 27

#define DEEP_CRIMSON "\x1b[38;5;124m" 
#define STEEL_BLUE   "\x1b[38;5;67m" 
#define GOLDEN_BRASS "\x1b[38;5;178m"
#define DARK_TIMBER  "\x1b[38;5;94m" 
#define MOUNTAIN_GRAY "\x1b[38;5;246m" 
#define IVORY_WHITE  "\x1b[38;5;255m"
#define FORGE_FIRE   "\x1b[38;5;202m"
#define MOLTEN_METAL "\x1b[38;5;208m" 
#define HEATED_STEEL "\x1b[38;5;172m"
#define ANVIL_IRON   "\x1b[38;5;239m"
#define HAMMER_STEEL "\x1b[38;5;250m" 
#define COAL_BLACK   "\x1b[38;5;234m" 
#define COPPER_SHEEN "\x1b[38;5;166m" 
#define COSMIC_BLUE "\x1b[38;5;63m"
#define NEBULA      "\x1b[38;5;135m"
#define STARDUST    "\x1b[38;5;147m"
#define CELESTIAL   "\x1b[38;5;51m"
#define STARLIGHT   "\x1b[38;5;231m"
#define SILVER      "\x1b[38;5;251m"
#define GOLD      "\x1b[38;5;220m"
#define BRONZE    "\x1b[38;5;172m"
#define COPPER    "\x1b[38;5;166m"
#define MAROON    "\x1b[38;5;88m"
#define SANDSTONE "\x1b[38;5;180m"
#define CLAY      "\x1b[38;5;137m"
#define BRICK     "\x1b[38;5;160m"
#define IRON      "\x1b[38;5;240m"
#define PARCHMENT "\x1b[38;5;223m"
#define GREEN     "\033[32m"
#define RESET     "\x1b[0m"

// define functions for cross platform
#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN "cls"
    #define SLEEP(ms) Sleep(ms)  // Sleep in milliseconds (Windows)
#else
    #include <time.h>
    #define CLEAR_SCREEN "clear"
    void sleep_ms(int milliseconds) {
        struct timespec ts;
        ts.tv_sec = milliseconds / 1000;
        ts.tv_nsec = (milliseconds % 1000) * 1000000;
        nanosleep(&ts, NULL);
    }
    #define SLEEP(ms) sleep_ms(ms)
#endif

// declare global variables that are mainly used for selections
int highlight = 0, ch, max_hp = 100, hp_divisor = 4, max_exp = 100, level_divisor = 4;

// character array for yes or no selections
char *yesOrNo[2] = {
    "            Yes           ",
    "            No            "
};

char *returnTo[1] = {
    "          Return          "
};

char *continueTo[1] = {
    "         Continue         "
};

// player structure
struct player {
    char race[10], clss[12], place[32], wpn[32], skill[32];
    int raceID, classID;
    int placeID, wpnID, wpn_type;
    int hp, exp, dmg, skill_dmg, trig_die, skill_cd;
    int def, agl;
    int level, gold, unlock;
    int bag[14], quest;
};
struct quest {
    char name[32];
    char dscrpt[64];
    int exp, gold, req, ID, killID, prog, activ, plantID;
};
struct beast {
    char name[32];
    int hp, trig_die, dmg, skill_dmg, def, hit_die, wpn_type;
    int agl, exp_drop, gold_drop, beast_ID;
};

struct beast tier1_beast;
struct quest tier1_quest;

// declare string menu & manipulation functions
void resize_terminal();
void drawMenu(int highlight, char *temp_array[], int size);
void drawMenu2d(int highlight, char temp_array[][100], int size);
void ascii_styles(int choice);
void center_text(const char *text, int row);

// declare loading and saving game functions
void check_data(int type);
void decoder(struct player *player1);
void loading();
void load_game(struct player *player1);
void save_game(struct player *player1);
void read_commonfolk(char cfolk[][100], int a, int b, int c, int d, int place);
void printDialog(int highlight, int a, int b, int c, int d, int place);

// declare game functions
void race_select(struct player *player1);
void class_select(struct player *player1);
void spawn(struct player *player1, int place);
void city_hall(int place);
void library(int place);
void blacksmith(struct player *player1, int place, char weapons[][100], int highlight, int type);
void check_inventory(struct player *player1, int type);
void expanse(int place, struct player *player1);
void low_encounter(struct player *player1, int place);
void battleAI(struct player *player1, int place);
void attack_beast(struct player *player1, int *turn, int *skill_activ, int *duration, int place);
void attack_player(struct player *player1, int *trig_skill, int *beast_skl_activ, int *beast_skl_dur);
void show_health_bar(struct player *player1);
void drink_potion(struct player *player1);
void check_level(struct player *player1);
void storyline(struct player *player1, int type);
void quest_board(struct player *player1, int quest_rng1, int quest_rng2, int quest_rng3);
void rest_area(struct player *player1);
void guild_shop(struct player *player1);
void check_quest(struct player *player1);
void expanse1(int place, struct player *player1);
void expanse2(int place, struct player *player1);
void med_encounter(struct player *player1, int place, int return_here);

void dungeon1(struct player *player1, int place, int return_here);
void dungeon2(struct player *player1, int place, int return_here);
void dungeon3(struct player *player1, int place, int return_here);
void dungeon4(struct player *player1, int place, int return_here);
void dungeon5(struct player *player1, int place, int return_here);
// main function

void handle_sigwinch(int sig) {
    ungetch('a');
}

// main function
int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    system(CLEAR_SCREEN);
    // set the terminal to UTF-8 to render blocks correctly
    if (!setlocale(LC_ALL, "C.UTF-8")) {
        setlocale(LC_ALL, "en_US.UTF-8");
    }

    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    // initialize the player structure
    struct player player1;

    char *menu_options[menu_size] = {
        "        Start Game        ",
        "        Load Game         ",
        "           Exit           "
    };

    signal(SIGWINCH, handle_sigwinch);
    highlight = 0;
    ascii_styles(0);
    printf(GOLD);
    ungetch('a');
    center_text("Press up ↑ / down ↓ to choose", 15);
    center_text("Press ENTER to select", 16);
    drawMenu(highlight, menu_options, menu_size);
    while ((ch = getch()) != '\n') {
        system(CLEAR_SCREEN);
        ascii_styles(0);
        printf(GOLD);
        center_text("Press up ↑ / down ↓ to choose", 15);
        center_text("Press ENTER to select", 16);
        switch (ch) {
        case KEY_UP:
            highlight = (highlight - 1 + menu_size) % menu_size;
            
            break;
        case KEY_DOWN:
            highlight = (highlight + 1) % menu_size;
            
            break;
        case ESC:
            system(CLEAR_SCREEN);
            endwin();
            exit(0);
        }
        printf(PARCHMENT);
        drawMenu(highlight, menu_options, menu_size);
    }

    switch (highlight){
    case 0:
        check_data(0);
        race_select(&player1);
        class_select(&player1);
        player1.level = 1;
        player1.gold = 100;
        player1.wpnID = 0;
        player1.hp = 100;
        player1.exp = 0;
        player1.skill_dmg = 0;
        player1.dmg = 0;
        player1.trig_die = 0;
        player1.skill_cd = 0;
        player1.wpn_type = 0;
        strcpy(player1.wpn, "None");
        strcpy(player1.skill, "None");
        strcpy(tier1_quest.name, "None");
        tier1_quest.exp = 0;
        tier1_quest.gold = 0;
        tier1_quest.req = 0;
        tier1_quest.ID = 0;
        tier1_quest.prog = 0;
        tier1_quest.activ = 0;
        tier1_quest.plantID = 0;

        player1.unlock = 0;
        for (int i = 0; i < 14; i++) {
            player1.bag[i] = 0;
        }
        save_game(&player1);
        decoder(&player1);
        storyline(&player1, 0);
        break;

    case 1:
        check_data(1);
        loading();
        decoder(&player1);
        load_game(&player1);
        break;

    case 2:
        system(CLEAR_SCREEN);
        endwin();
        exit(0);
        break;
        
    default:
        break;
    }

    switch (player1.placeID) {
    case 50100000:
        spawn(&player1, 1);
        break;
    case 50300000:
        spawn(&player1, 3);
        break;
    case 50400000:
        spawn(&player1, 4);
        break;
    default:
        spawn(&player1, 2);
        break;
    }

    return 0;
}

void loading() {
    system(CLEAR_SCREEN);
    
    FILE *ifp = fopen("resources/loadingtips.txt", "rt");
    char tips[20][100];
    int j = 52;
    char enter;

    for (int i = 0; i < 12; i++) {
        fgets(tips[i], sizeof(tips[i]), ifp);
    }
    
    srand((unsigned int)time(NULL));

    int d11 = rand() % 11 + 1;

    center_text("_________________________ ", 11);
    center_text("Loading", 12);

    for (int i = 0; i < 13; i++) {
        SLEEP(50);
        printf("\033[10;%dH", j);
        printf("█ ");
        fflush(stdout);
        j+=2;
    }

    center_text(tips[d11], 15);
    center_text("Press any key to continue", 20);
    fflush(stdout);
    getch();
    system(CLEAR_SCREEN);
};

// draw menu and selection function
void drawMenu(int highlight, char *temp_array[], int size) {

    for (int i = 0; i < size; i++) {
        // Move cursor to row i+1, column 1
        printf("\033[%d;48H", 22 + i + 1);
        if (i == highlight) {
            printf("\x1b[48;5;236m");
            printf(GOLD"->|%-10s|<-"RESET, temp_array[i]); // Highlighted
            
        } else
            printf(PARCHMENT"   %-10s   ", temp_array[i]); // Normal
    }
    fflush(stdout);
}

void drawMenu2d(int highlight, char temp_array[][100], int size) {

    for (int i = 0; i < size; i++) {
        // Move cursor to row i+1, column 1
        printf("\033[%d;50H", 20 + i + 1);
        if (i == highlight) {
            printf("\x1b[48;5;236m");
            printf(GOLD"->|%-10s|<-"RESET, temp_array[i]); // Highlighted
        } else
            printf(PARCHMENT"   %-10s   ", temp_array[i]); // Normal
    }
    fflush(stdout);
}

// function ascii letterings for UI
void ascii_styles(int choice) {
    switch (choice) {
    case 0:
        printf(GOLD   "\033[3;10H╔═══════╗ ═════════════════════════════════════════════════════════════════════════════════════════ ╔═══════╗");
        printf(BRONZE "\033[4;10H║▓▓▓▓▓▓▓║                                                                                           ║▓▓▓▓▓▓▓║");
        printf(COPPER "\033[5;10H ▓  ║  ▓    ╔████  ╔████ ╔██   ╔██  ╔████████  ╔██████  ╔██      ╔██  ╔████████  ╔██      ╔██████    ▓  ║  ▓ ");
        printf(MAROON "\033[6;10H  ║ ║ ║     ║██║██║██║██  ║██ ║██   ║██          ║██     ║██    ║██   ║██   ║██  ║██      ║██         ║ ║ ║  ");
        printf(BRONZE "\033[7;10H  ║ ║ ║     ║▓▓ ║▓▓▓ ║▓▓   ║▓▓▓▓    ║▓▓▓▓▓▓▓▓    ║▓▓      ║▓▓  ║▓▓    ║▓▓▓▓▓▓▓▓  ║▓▓      ║▓▓▓▓       ║ ║ ║  ");
        printf(MAROON "\033[8;10H  ║ ║ ║     ║▓▓  ║▓  ║▓▓    ║▓▓           ║▓▓    ║▓▓       ║▓▓║▓▓     ║▓▓   ║▓▓  ║▓▓      ║▓▓         ║ ║ ║  ");
        printf(COPPER "\033[9;10H ╠═════╣    ║▒▒  ╚╝  ║▒▒    ║▒▒     ╔▒▒▒▒▒▒▒▒    ║▒▒        ║▒▒▒      ║▒▒   ║▒▒  ║▒▒▒▒▒▒  ║▒▒▒▒▒▒    ╠═════╣ ");
        printf(BRONZE"\033[10;10H║▒▒▒▒▒▒▒║   ╚═╝      ╚═╝    ╚═╝     ╚═══════╝    ╚═╝        ╚══╝      ╚═╝   ╚═╝  ╚═════╝  ╚═════╝   ║▒▒▒▒▒▒▒║");
        printf(GOLD  "\033[11;10H╚═══════╝ ═════════════════════════════════════════════════════════════════════════════════════════ ╚═══════╝"); 
        break;
    
    case 1:
        printf(SANDSTONE "\033[3;9H    ╔██╗ ════════════════════════════════════════════════════════════════════════════════════════════════ ╔██╗");
        printf(CLAY      "\033[4;9H    ║██║      ╔████████  ╔██      ╔██  ╔████████  ╔██       ╔███████   ╔████████  ╔██████  ╔████████      ║██║");
        printf(BRICK     "\033[5;9H    ║▓▓║      ║██   ║██   ║██    ║██   ║██   ║██  ║██      ╔██    ║██  ║██   ║██    ║██    ║██   ║██      ║▓▓║");
        printf(IRON      "\033[6;9H    ║▓▓║      ║▓▓▓▓▓▓▓▓    ║▓▓  ║▓▓    ║▓▓▓▓▓▓▓▓  ║▓▓      ║▓▓    ║▓▓  ║▓▓▓▓▓▓▓▓    ║▓▓    ║▓▓▓▓▓▓▓▓      ║▓▓║");
        printf(CLAY      "\033[7;9H    ║▓▓║      ║▓▓▓▓▓▓▓▓    ║▓▓  ║▓▓    ║▓▓▓▓▓▓▓▓  ║▓▓      ║▓▓    ║▓▓  ║▓▓▓▓▓▓▓▓    ║▓▓    ║▓▓▓▓▓▓▓▓      ║▓▓║");
        printf(IRON      "\033[8;9H╔═══╩▓▓╩═══╗  ║▓▓   ║▓▓     ║▓▓║▓▓     ║▓▓   ║▓▓  ║▓▓      ║▓▓    ║▓▓  ║▓▓  ║▓▓     ║▓▓    ║▓▓   ║▓▓  ╔═══╩▓▓╩═══╗");
        printf(BRICK     "\033[9;9H╚═══╦▒▒╦═══╝  ║▒▒   ║▒▒      ║▒▒▒      ║▒▒   ║▒▒  ║▒▒▒▒▒▒   ║▒▒▒▒▒▒▒   ║▒▒   ║▒▒  ║▒▒▒▒▒▒  ║▒▒   ║▒▒  ╚═══╦▒▒╦═══╝");
        printf(CLAY     "\033[10;9H    ║▒▒║      ╚═╝   ╚═╝      ╚══╝      ╚═╝   ╚═╝  ╚═════╝   ╚═════╝    ╚═╝   ╚═╝  ╚═════╝  ╚═╝   ╚═╝      ║▒▒║");
        printf(SANDSTONE"\033[11;9H    ╚▒▒╝ ════════════════════════════════════════════════════════════════════════════════════════════════ ╚▒▒╝");
        break;

    case 2:
        printf(SILVER     "\033[3;11H    ║██║ ═════════════════════════════════════════════════════════════════════════════════════════  ║██║     ");
        printf(CELESTIAL  "\033[4;11H═══ █  █ ═══  ╔████████  ╔███████  ╔██████  ╔██   ╔██  ╔███████  ╔████████  ╔██████  ╔████████  ═══ █  █ ═══ ");
        printf(NEBULA     "\033[5;11H║ ▓▓ ║║ ▓▓ ║  ║██   ║██  ║██         ║██    ║██   ║██  ║██       ║██   ║██    ║██    ║██        ║ ▓▓ ║║ ▓▓ ║");
        printf(COSMIC_BLUE"\033[6;11H ║ ▓▓  ▓▓ ║   ║▓▓▓▓▓▓▓▓  ║▓▓▓▓▓      ║▓▓    ║▓▓▓▓▓▓▓▓  ║▓▓▓▓▓    ║▓▓▓▓▓▓▓▓    ║▓▓    ║▓▓▓▓▓▓▓▓   ║ ▓▓  ▓▓ ║  ");
        printf(CELESTIAL  "\033[7;11H║▒▒▒▒▒▒▒▒▒▒║  ║▓▓   ║▓▓  ║▓▓         ║▓▓    ║▓▓▓▓▓▓▓▓  ║▓▓       ║▓▓  ║▓▓     ║▓▓          ║▓▓  ║▒▒▒▒▒▒▒▒▒▒║ ");
        printf(COSMIC_BLUE"\033[8;11H    ▒▒▒▒      ║▒▒   ║▒▒  ║▓▓         ║▒▒    ║▒▒   ║▒▒  ║▓▓       ║▓▓  ║▓▓     ║▒▒          ║▓▓      ▒▒▒▒     ");
        printf(NEBULA     "\033[9;11H    ║▓▓║      ║▒▒   ║▒▒  ║▒▒▒▒▒▒▒    ║▒▒    ║▒▒   ║▒▒  ║▒▒▒▒▒▒▒  ║▒▒   ║▒▒  ║▒▒▒▒▒▒  ╔▒▒▒▒▒▒▒▒      ║▓▓║    ");
        printf(CELESTIAL "\033[10;11H    ║██║      ╚═╝   ╚═╝  ╚══════╝    ╚═╝    ╚═╝   ╚═╝  ╚══════╝  ╚═╝   ╚═╝  ╚═════╝  ╚═══════╝      ║▓▓║     ");
        printf(SILVER    "\033[11;11H     ██  ═════════════════════════════════════════════════════════════════════════════════════════   ██      ");
        break;
    case 3:
        printf(ANVIL_IRON  "\033[3;1H╠██ ║█████║ ██╣  ═══════════════════════════════════════════════════════════════════════════════════════════════  ╠██ ║█████║ ██╣");
        printf(FORGE_FIRE  "\033[4;1H╠█████▒ ▒█████╣  ╔██████  ╔██   ╔██  ╔████████  ╔██       ╔███   ║██   ╔███████   ╔████████  ╔███████  ╔████████  ╠█████▒ ▒█████╣");
        printf(MOLTEN_METAL"\033[5;1H╠█████▒ ▒█████╣    ║██    ║██   ║██  ║██   ║██  ║██       ║████  ║██  ╔██    ║██  ║██   ║██    ║███    ║██   ║██  ╠█████▒ ▒█████╣");
        printf(HEATED_STEEL"\033[6;1H╠▓▓ ║▓▓▓▓▓║ ▓▓╣    ║▓▓    ║▓▓▓▓▓▓▓▓  ║▓▓▓▓▓▓▓▓  ║▓▓       ║▓▓║▓▓ ║▓▓  ║▓▓    ║▓▓  ║▓▓▓▓▓▓▓▓    ║▓▓▓    ║▓▓▓▓▓▓▓▓  ╠▓▓ ║▓▓▓▓▓║ ▓▓╣");
        printf(FORGE_FIRE  "\033[7;1H      ▓▓▓          ║▓▓    ║▓▓▓▓▓▓▓▓  ║▓▓   ║▓▓  ║▓▓       ║▓▓ ║▓▓║▓▓  ║▓▓    ║▓▓  ║▓▓  ║▓▓     ║▓▓▓    ║▓▓   ║▓▓        ▓▓▓      ");
        printf(HEATED_STEEL"\033[8;1H      ▓▓▓          ║▓▓    ║▓▓▓▓▓▓▓▓  ║▓▓   ║▓▓  ║▓▓       ║▓▓ ║▓▓║▓▓  ║▓▓    ║▓▓  ║▓▓  ║▓▓     ║▓▓▓    ║▓▓   ║▓▓        ▓▓▓      ");
        printf(MOLTEN_METAL"\033[9;1H      ▒▒▒          ║▒▒    ║▒▒   ║▒▒  ║▒▒   ║▒▒  ║▒▒▒▒▒▒▒  ║▒▒   ║▒▒▒   ║▒▒▒▒▒▒▒   ║▒▒   ║▒▒  ║▒▒▒▒▒▒▒  ║▒▒   ║▒▒        ▒▒▒      ");
        printf(FORGE_FIRE "\033[10;1H      ███          ╚═╝    ╚═╝   ╚═╝  ╚═╝   ╚═╝  ╚══════╝  ╚═╝   ╚══╝   ╚═════╝    ╚═╝   ╚═╝  ╚══════╝  ╚═╝   ╚═╝        ███      ");
        printf(ANVIL_IRON "\033[11;1H      ╚█╝  ═══════════════════════════════════════════════════════════════════════════════════════════════════════════  ╚█╝      "); 
        break;

    case 4:
        printf(MOUNTAIN_GRAY "\033[3;6H║██████████║  ═════════════════════════════════════════════════════════════════════════════════════════  ║██████████║");
        printf(DEEP_CRIMSON  "\033[4;6H║██████████║  ╔██        ╔██  ╔████████  ╔██       ╔███████  ╔████████  ╔███████  ╔██    ╔██  ╔████████  ║██████████║");
        printf(STEEL_BLUE    "\033[5;6H  ║▓▓▓▓▓▓║     ║██      ║██   ║██   ║██  ║██       ║██       ║██   ║██    ║███    ║██    ║██  ║██          ║▓▓▓▓▓▓║  ");
        printf(DARK_TIMBER   "\033[6;6H  ║▓▓▓▓▓▓║      ║▓▓    ║▓▓    ║▓▓▓▓▓▓▓▓  ║▓▓       ║▓▓▓▓▓    ║▓▓▓▓▓▓▓▓    ║▓▓▓    ║▓▓    ║▓▓  ║▓▓▓▓▓▓▓▓    ║▓▓▓▓▓▓║  ");
        printf(DEEP_CRIMSON  "\033[7;6H  ╠══════╣       ║▓▓  ║▓▓     ║▓▓   ║▓▓  ║▓▓       ║▓▓       ║▓▓  ║▓▓     ║▓▓▓    ║▓▓    ║▓▓        ║▓▓    ╠══════╣  ");
        printf(DARK_TIMBER   "\033[8;6H  ║▒▒▒▒▒▒║        ║▒▒║▒▒      ║▒▒   ║▒▒  ║▒▒       ║▒▒       ║▒▒   ║▒▒    ║▒▒▒    ║▒▒    ║▒▒        ║▒▒    ║▒▒▒▒▒▒║  ");
        printf(STEEL_BLUE    "\033[9;6H  ║▒▒▒▒▒▒║         ║▒▒▒       ║▒▒   ║▒▒  ║▒▒▒▒▒▒▒  ║▒▒▒▒▒▒▒  ║▒▒   ║▒▒  ║▒▒▒▒▒▒▒   ║▒▒▒▒▒▒▒   ╔▒▒▒▒▒▒▒▒    ║▒▒▒▒▒▒║  ");
        printf(DEEP_CRIMSON "\033[10;6H║██████████║       ╚══╝       ╚═╝   ╚═╝  ╚══════╝  ╚══════╝  ╚═╝   ╚═╝  ╚══════╝   ╚══════╝   ╚═══════╝  ║██████████║");
        printf(MOUNTAIN_GRAY"\033[11;6H║██████████║  ═════════════════════════════════════════════════════════════════════════════════════════  ║██████████║"); 
        break;

    default:
        break;
    }
}
// function to center the texts/dialogs
void center_text(const char *text, int row) {
    int textLen = strlen(text);
    int padding = (TERMINAL_WIDTH - textLen) / 2;
    if (padding < 0) padding = 0;
    printf("\033[%d;%dH", row, padding);
    printf("%s\n", text);
}
// function to load the save data
void load_game(struct player *player1) {
    system(CLEAR_SCREEN);
    decoder(player1);
    check_inventory(player1, 1);
    drawMenu(highlight, yesOrNo, 2);
    printf(PARCHMENT);
    while ((ch = getch()) != '\n') {
        switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + 2) % 2;

                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 2;

                break;
            case ESC:
                endwin();
        }
        drawMenu(highlight, yesOrNo, 2);
    }
    
    if (highlight == 1) {
        main();
    } else {}
}

void check_inventory(struct player *player1, int type) {
    check_level(player1);
    system(CLEAR_SCREEN);

    int pcol, ecol;
    int tempphp, tempexp;
    char tmp_msg[64];
    char item_holder[14][10];
    int itemnum_holder[14];
    int row = 12, tmppot, tmpflora;

    char *pots[] = {
        "Lesser Healing Potion:  ",
        "Healing Potion:         ",
        "Greater Healing Potion: ",
        "Super Healing Potion:   "
    };

    char *flora[] = {
        "Veilrose:      ",
        "Yormen Flora:  ",
        "Trawnshroom:   ",
        "Lumen Flora:   ",
        "Orbroot:       ",
        "Dreamvine:     ",
        "Skyroot:       ",
        "Wyrmblossom:   "
    };

    char *inventory_options[] = {
        "        Save Game         ",
        "       Drink Potion       ",
        "         Return           "
    };

    pcol = 10;
    ecol = 40;
    tempphp = player1->hp / hp_divisor;
    tempexp = player1->exp / level_divisor;

    center_text("Inventory", 6);
    printf("\033[9;10HHealth: %d/%d", player1->hp, max_hp);
    for (int i = 0; i < 25; i++) {
        printf("\033[10;%dH", pcol);
        printf(IRON"█"RESET);
        pcol++;
    }
    pcol = 10;
    for (int i = 0; i < tempphp; i++) {
        printf("\033[10;%dH", pcol);
        printf(GREEN"█"RESET);
        pcol++;
    }
    printf(PARCHMENT"\033[9;40HExp: %d/%d", player1->exp, max_exp);
    for (int i = 0; i < 25; i++) {
        printf("\033[10;%dH", ecol);
        printf(IRON"█"RESET);
        ecol++;
    }
    ecol = 40;
    for (int i = 0; i < tempexp; i++) {
        printf("\033[10;%dH", ecol);
        printf(COSMIC_BLUE"█"RESET);
        ecol++;
    }

    printf(COSMIC_BLUE"\033[12;10HRace:    %s", player1->race);
    printf(COSMIC_BLUE"\033[13;10HClass:   %s", player1->clss);
    printf(COSMIC_BLUE"\033[14;10HPlace:   %s", player1->place);   
    printf(COSMIC_BLUE"\033[15;10HWeapon:");
    printf(STARDUST"\033[16;10H%s"RESET, player1->wpn);
    printf(COSMIC_BLUE"\033[17;10HSkill:");
    printf(STARDUST"\033[18;10H%s"RESET, player1->skill);
    printf(COSMIC_BLUE"\033[19;10HSkill trigger rate: %d"RESET, player1->trig_die);
    
    row = 11;
    tmpflora = 0;
    for (int i = 0; i < 8; i++) {
        if (player1->bag[tmpflora] > 0) {
            printf(CELESTIAL"\033[%d;70H%s%dx",row, flora[i], player1->bag[tmpflora]); 
            row++;
        }
        tmpflora++;
        fflush(stdout);
    }

    row = 11;
    tmppot = 9;
    for (int i = 0; i < 4; i++) {
        if (player1->bag[tmppot] > 0) {
            printf(CELESTIAL"\033[%d;90H%s%dx", row, pots[i], player1->bag[tmppot]); 
            row++;
        }
        tmppot++;
        fflush(stdout);
    }

    printf(STARDUST"\033[12;40HAgility:   %d", player1->agl);
    printf(STARDUST"\033[13;40HArmor:     %d", player1->def);
    printf(STARDUST"\033[14;40HLevel:     %d", player1->level);
    printf(STARDUST"\033[15;40HDmg:       %d", player1->dmg);
    printf(STARDUST"\033[16;40HSkill Dmg: %d", player1->skill_dmg);
    printf(STARDUST"\033[17;40HSkill CD:  %d"RESET, player1->skill_cd);
    printf(GOLD"\033[18;40HGold:      %d"RESET, player1->gold);

    if (tier1_quest.activ == 1) {
        strcpy(tmp_msg, "Quest: ");
        strcat(tmp_msg, tier1_quest.name);
        center_text(tmp_msg, 20);
        center_text(tier1_quest.dscrpt, 21);
    } else {
        center_text("No quest taken", 20);
        center_text("--------------", 20);
    }

    if (type == 1) {
        return;
    }
    
    highlight = 0;
    drawMenu(highlight, inventory_options, 3);
    while ((ch = getch()) != '\n') {
        switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + 3) % 3;

                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 3;

                break;
            case ESC:
                endwin();
        }
        drawMenu(highlight, inventory_options, 3);

    }
    switch (highlight) {
    case 0:
        save_game(player1);
        break;
    case 1:
        drink_potion(player1);
        break;
    case 2:
        break;
    
    default:
        break;
    }
}

void decoder(struct player *player1) {
    system(CLEAR_SCREEN);
    FILE *save_file = fopen("resources/plid/slot1.txt", "rt");
    FILE *bag_file = fopen("resources/plid/bag.txt", "rt");
    FILE *quest_file = fopen("resources/plid/questfile.txt", "rt");

    char temp[32];
    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->raceID);

    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->classID);
    
    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->placeID);
    
    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->level);
    
    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->gold);

    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->wpnID);

    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->hp);

    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->def);

    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->agl);

    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->exp);

    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->dmg);

    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->skill_dmg);

    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->trig_die);

    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->skill_cd);

    fgets(player1->wpn, sizeof(player1->wpn), save_file);
    fgets(player1->skill, sizeof(player1->skill), save_file);

    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &player1->wpn_type);

    for (int i = 0; i < 14; i++) {
        if (fgets(temp, sizeof(temp), bag_file) != NULL) {
            sscanf(temp, "%d", &player1->bag[i]);
        } else {
            player1->bag[i] = 0;
        }
    }

    fgets(tier1_quest.name, sizeof(tier1_quest.name), quest_file);

    fgets(temp, sizeof(temp), quest_file);
    sscanf(temp, "%d", &tier1_quest.prog);

    fgets(temp, sizeof(temp), quest_file);
    sscanf(temp, "%d", &tier1_quest.activ);

    fgets(temp, sizeof(temp), quest_file);
    sscanf(temp, "%d", &tier1_quest.exp);

    fgets(temp, sizeof(temp), quest_file);
    sscanf(temp, "%d", &tier1_quest.gold);

    fgets(temp, sizeof(temp), quest_file);
    sscanf(temp, "%d", &tier1_quest.ID);

    fgets(temp, sizeof(temp), quest_file);
    sscanf(temp, "%d", &tier1_quest.req);
    
    fgets(tier1_quest.dscrpt, sizeof(tier1_quest.dscrpt), quest_file);

    fclose(bag_file);
    fclose(quest_file);

    switch (player1->raceID) {
    case 10000001:
        strcpy(player1->race, "Human");
        break;
    case 10000002:
        strcpy(player1->race, "Elf");
        break;
    case 10000003:
        strcpy(player1->race, "Dwarf");
        break;
    case 10000004:
        strcpy(player1->race, "Giant");
        break;
    default:
        exit(1);
        break;
    }

    switch (player1->classID) {
    case 20000001:
        strcpy(player1->clss, "Warrior");
        break;
    case 20000002:
        strcpy(player1->clss, "Mage");
        break;
    case 20000003:
        strcpy(player1->clss, "Ranger");
        break;
    case 20000004:
        strcpy(player1->clss, "Assassin");
        break;
    default:
        exit(1);
        break;
    }

    switch (player1->placeID) {
    case 50100000:
        strcpy(player1->place, "Avaloria");
        break;
    case 50300000:
        strcpy(player1->place, "Thalnoria");
        break;
    
    case 50400000:
        strcpy(player1->place, "Valerius");
        break;
    
    default:
        strcpy(player1->place, "Aetheris");
        break;
    }

    fclose(save_file);
}

void check_data(int type) {
    system(CLEAR_SCREEN);
    FILE *save_file = fopen("resources/plid/slot1.txt", "rt");
    char temp[32];
    int tempData;

    fgets(temp, sizeof(temp), save_file);
    sscanf(temp, "%d", &tempData);
    fclose(save_file);

    switch (tempData) {
    case 10000001:
    case 10000002:
    case 10000003:
    case 10000004:
        break;
    default:
        if (type == 1) {
            center_text("No save file detected", 10);
            center_text("Proceed to Start game to create a save file", 11);
            SLEEP(1000);
            main();
        } else
            return;
    } 

    if (type == 0) {
        center_text("Save file detected, Overwrite existing data?", 10);
        drawMenu(highlight, yesOrNo, 2);
        printf(PARCHMENT);
        while ((ch = getch()) != '\n') {
            switch (ch) {
                case KEY_UP:
                    highlight = (highlight - 1 + 2) % 2;

                    break;
                case KEY_DOWN:
                    highlight = (highlight + 1) % 2;

                    break;
                case ESC:
                    endwin();
            }
            drawMenu(highlight, yesOrNo, 2);
            printf(PARCHMENT);
        }

        if (highlight == 1) {
            main();
        }
    }
}

void save_game(struct player *player1) {
    switch (player1->placeID) {
    case 50100000:
    case 50200000:
    case 50300000:
    case 50400000:
        check_level(player1);

        FILE *save_file = fopen("resources/plid/slot1.txt", "w");
        FILE *unlock = fopen("resources/unlock.txt", "w");
        FILE *bag_file = fopen("resources/plid/bag.txt", "w");
        FILE *quest_file = fopen("resources/plid/questfile.txt", "w");

        player1->wpn[strcspn(player1->wpn, "\n")] = '\0';
        player1->skill[strcspn(player1->skill, "\n")] = '\0';


        fprintf(unlock, "%d\n", player1->unlock);
        fprintf(save_file, "%d\n", player1->raceID);
        fprintf(save_file, "%d\n", player1->classID);
        fprintf(save_file, "%d\n", player1->placeID);
        fprintf(save_file, "%d\n", player1->level);
        fprintf(save_file, "%d\n", player1->gold);
        fprintf(save_file, "%d\n", player1->wpnID);
        fprintf(save_file, "%d\n", player1->hp);
        fprintf(save_file, "%d\n", player1->def);
        fprintf(save_file, "%d\n", player1->agl);
        fprintf(save_file, "%d\n", player1->exp);
        fprintf(save_file, "%d\n", player1->dmg);
        fprintf(save_file, "%d\n", player1->skill_dmg);
        fprintf(save_file, "%d\n", player1->trig_die);
        fprintf(save_file, "%d\n", player1->skill_cd);
        fprintf(save_file, "%s\n", player1->wpn);
        fprintf(save_file, "%s\n", player1->skill);
        fprintf(save_file, "%d\n", player1->wpn_type);
        for (int i = 0; i < 14; i++) {
            fprintf(bag_file, "%d\n", player1->bag[i]);
        }
        fprintf(quest_file, "%s\n", tier1_quest.name);
        fprintf(quest_file, "%d\n", tier1_quest.prog);
        fprintf(quest_file, "%d\n", tier1_quest.activ);
        fprintf(quest_file, "%d\n", tier1_quest.exp);
        fprintf(quest_file, "%d\n", tier1_quest.gold);
        fprintf(quest_file, "%d\n", tier1_quest.ID);
        fprintf(quest_file, "%d\n", tier1_quest.req);
        fprintf(quest_file, "%s\n", tier1_quest.dscrpt);

        fclose(unlock);
        fclose(bag_file);
        fclose(save_file);
        fclose(quest_file);

        system(CLEAR_SCREEN);
        loading();
        center_text("Successfully saved the game.\n", 10);
        SLEEP(1000);

        break;
        
    default:
        system(CLEAR_SCREEN);
        center_text("You can only save the game if you're in a kingdom", 10);
        center_text("Press any key to continue", 17);
        getch();
        break;
    }
}

// function for race selection
void race_select(struct player *player1) {
    char *race_options[] = {
        "           Human          ",
        "            Elf           ",
        "           Dwarf          ",
        "           Giant          "
    };
    char tmp_msg[42];

    while (1) {
        system(CLEAR_SCREEN);
        
        highlight = 0;
        center_text("Welcome to Character Creation!", 10);
        center_text("Please select your race.", 11);

        drawMenu(highlight, race_options, race_size);
        while ((ch = getch()) != '\n') {
            switch (ch) {
                case KEY_UP:
                    highlight = (highlight - 1 + race_size) % race_size;
                    break;
                case KEY_DOWN:
                    highlight = (highlight + 1) % race_size;
                    break;
                case ESC:
                    endwin();
            }
            system(CLEAR_SCREEN);
            switch (highlight) {
                case 0:
                    strcpy(player1->race, "Human");
                    center_text("Human", 10);
                    center_text("Adaptable and ambitious, humans thrive in all walks of life,", 11);
                    center_text("excelling through ingenuity and resilience.\n", 12);
                    break;
                case 1:
                    strcpy(player1->race, "Elf");
                    center_text("Elf", 10);
                    center_text("Graceful and long-lived, elves are attuned to nature and magic,", 11);
                    center_text("known for their agility and keen senses.\n", 12);
                    break;
                case 2:
                    strcpy(player1->race, "Dwarf");
                    center_text("Dwarf", 10);
                    center_text("Sturdy and stubborn, dwarves are expert craftsmen and warriors,", 11);
                    center_text("thriving in mountains and deep halls.\n", 12);
                    break;
                case 3:
                    strcpy(player1->race, "Giant");
                    player1->def = 8;
                    center_text("Giant", 10);
                    center_text("Towering and powerful, giants possess immense strength and a", 11);
                    center_text("deep connection to ancient traditions.\n", 12);
                    break;
                default:
                    break;
                }

            drawMenu(highlight, race_options, race_size);
            printf(PARCHMENT);
        }

        switch (highlight) {
        case 0:
            strcpy(player1->race, "Human");
            player1->placeID = 50100000;
            player1->raceID = 10000001;
            player1->agl = 9;
            player1->def = 6;
            break;
        case 1:
            strcpy(player1->race, "Elf");
            player1->placeID = 50200000;
            player1->raceID = 10000002;
            player1->agl = 11;
            player1->def = 5;
            break;
        case 2:
            strcpy(player1->race, "Dwarf");
            player1->placeID = 50300000;
            player1->raceID = 10000003;
            player1->agl = 10;
            player1->def = 6;
            break;
        case 3:
            strcpy(player1->race, "Giant");
            player1->placeID = 50400000;
            player1->raceID = 10000004;
            player1->agl = 7;
            player1->def = 8;
            break;
        default:
            exit(1);
        }
        system(CLEAR_SCREEN);
        highlight = 0;
        strcpy(tmp_msg, "Are you sure you want to be a/n ");
        strcat(tmp_msg, player1->race);
        center_text(tmp_msg, 10);
        drawMenu(highlight, yesOrNo, 2);
        printf(PARCHMENT);

        while ((ch = getch()) != '\n') {
            switch (ch) {
                case KEY_UP:
                    highlight = (highlight - 1 + 2) % 2;

                    break;
                case KEY_DOWN:
                    highlight = (highlight + 1) % 2;

                    break;
                case ESC:
                    endwin();
            }
            drawMenu(highlight, yesOrNo, 2);
        }
        if (highlight == 0) {
            break;
        }
    }

}

// function for class selection
void class_select(struct player *player1) {
    char *class_options[] = {
        "         Warrior          ",
        "          Mage            ",
        "         Ranger           ",
        "         Assasin          "
    };
    char tmp_msg[42];
    while (1) {
        system(CLEAR_SCREEN);
        center_text("Welcome to Character Creation!", 10);
        center_text("Please select your class.", 11);
        drawMenu(highlight, class_options, class_size);
        while ((ch = getch()) != '\n') {
            switch (ch) {
                case KEY_UP:
                    highlight = (highlight - 1 + class_size) % class_size;

                    break;
                case KEY_DOWN:
                    highlight = (highlight + 1) % class_size;

                    break;
                case ESC:
                    endwin();
            }
            system(CLEAR_SCREEN);
            switch (highlight) {
                case 0:
                    strcpy(player1->clss, "Warrior");
                    player1->classID = 20000001;
                    center_text("Warrior", 10);
                    center_text("A master of combat, warriors rely on strength and skill", 11);
                    center_text("to dominate the battlefield with weapons and armor.\n", 12);
                    break;
                case 1:
                    strcpy(player1->clss, "Mage");
                    player1->classID = 20000002;
                    center_text("Mage", 10);
                    center_text("Wielding arcane power, mages cast devastating spells,", 11);
                    center_text("bending the elements and reality to their will.\n", 12);
                    break;
                case 2:
                    strcpy(player1->clss, "Ranger");
                    player1->classID = 20000003;
                    center_text("Ranger", 10);
                    center_text("Masters of the wild, rangers are skilled hunters and scouts,", 11);
                    center_text("striking swiftly with bows and blades.\n", 12);
                    break;
                case 3:
                    strcpy(player1->clss, "Assassin");
                    player1->classID = 20000004;
                    center_text("Assassin", 10);
                    center_text("Silent and deadly, assassins strike from the shadows,", 11);
                    center_text("using stealth and precision to eliminate their targets.\n", 12);
                    break;
                default:
                    break;
            }    

            drawMenu(highlight, class_options, class_size);
            printf(PARCHMENT);
        }
        switch (highlight) {
            case 0:
                strcpy(player1->clss, "Warrior");
                player1->classID = 20000001;
                break;
            case 1:
                strcpy(player1->clss, "Mage");
                player1->classID = 20000002;
                break;
            case 2:
                strcpy(player1->clss, "Ranger");
                player1->classID = 20000003;
                break;
            case 3:
                strcpy(player1->clss, "Assassin");
                player1->classID = 20000004;
                break;
            default:
                break;
        }    

        system(CLEAR_SCREEN);
        highlight = 0;
        strcpy(tmp_msg, "Are you sure you want to be a/n ");
        strcat(tmp_msg, player1->clss);
        center_text(tmp_msg, 10);
        drawMenu(highlight, yesOrNo, 2);
        printf(PARCHMENT);
        while ((ch = getch()) != '\n') {
            switch (ch) {
                case KEY_UP:
                    highlight = (highlight - 1 + 2) % 2;

                    break;
                case KEY_DOWN:
                    highlight = (highlight + 1) % 2;

                    break;
                case ESC:
                    endwin();
            }
            drawMenu(highlight, yesOrNo, 2);
            printf(PARCHMENT);
        }

        if (highlight == 0) {
            break;
        }
    }
}

void spawn(struct player *player1, int place) {
    char *destination[] = {
        "         Avaloria         ",
        "         Aetheris         ",
        "        Thanlnoria        ",
        "         Valerius         ",
        "    Return to kingdom     "
    };

    char *avaloria_places[] = {
        "           Guide          ",
        "         City Hall        ",
        "        City Square       ",
        "          Library         ",
        "     Ironknights Dock     ",
        "    Adventurer's Guild    ",
        "     Valor's Smithing     ",
        "       Potion Master      ",
        "       Lorian Woods       ",
        "        Inventory         ",
        "   Return to Main Menu    "
    };

    char *aetheris_places[] = {
        "           Guide          ",
        "         City Hall        ",
        "        City Square       ",
        "          Library         ",
        "     Whisperwind Dock     ",
        "    Adventurer's Guild    ",
        "     Blacksmith shop      ",
        "       Potion Master      ",
        "       Thorwyn Glade      ",
        "        Inventory         ",
        "   Return to Main Menu    "
    };

    char *thalnoria_places[] = {
        "           Guide          ",
        "         City Hall        ",
        "        City Square       ",
        "          Library         ",
        "      Seaforge Docks      ",
        "    Adventurer's Guild    ",
        "     Blacksmith shop      ",
        "       Potion Master      ",
        "      Lenoria Glade       ",
        "        Inventory         ",
        "   Return to Main Menu    "
    };

    char *valerius_places[] = {
        "           Guide          ",
        "         City Hall        ",
        "        City Square       ",
        "          Library         ",
        "      Seaforge Docks      ",
        "    Adventurer's Guild    ",
        "     Blacksmith shop      ",
        "       Potion Master      ",
        "       Virian Hills       ",
        "        Inventory         ",
        "   Return to Main Menu    "
    };

    char *potions[] = {
        "  Lesser Healing Potion   ",
        "      Healing Potion      ",
        "  Greater Healing Potion  ",
        "   Super Healing Potion   ",
        "   Return to Main Menu    "
    };

    char *guild_hall[] = {
        "       Quest Board        ",
        "        Guild Shop        ",
        "        Rest Area         ",
        "   Return to Main Menu    "
    };
    char **selected_place = NULL;

    srand((unsigned int)time(NULL));
    int a = rand() % 3;
    int b = rand() % 3 + 3;
    int c = rand() % 3 + 6;
    int d = rand() % 3 + 9;

    int quest_rng1 = (rand() % 3) + 1;
    int quest_rng2 = (rand() % 3) + 4;
    int quest_rng3 = (rand() % 3) + 7;
    
    int style;
    int potion_price, line1, line2, line3;

    char cfolk[size_5][100], folk_dialog[size_4][100];
    char tmp_msg[50], potion_cart[32];
    char weapons[3][100];
    char title[100];
    int tempHold, tempgold;

    switch (place) {
    case 1:
        selected_place = avaloria_places;
        player1->placeID = 50100000;
        style = 1;
        break;
    case 3:
        selected_place = thalnoria_places;
        player1->placeID = 50300000;
        style = 3;
        break;
    case 4:
        selected_place = valerius_places;
        player1->placeID = 50400000;
        style = 4;
        break;
    default:
        selected_place = aetheris_places;
        player1->placeID = 50200000;
        style = 2;
        break;
    }

    while (1) {
        system(CLEAR_SCREEN);
        highlight = 0;
        drawMenu(highlight, selected_place, place_size);
        ascii_styles(style);
        ungetch('a');
        ungetch('a');
        while ((ch = getch()) != '\n') {
            system(CLEAR_SCREEN);
            ascii_styles(style);
            drawMenu(highlight, selected_place, place_size);
            switch (ch) {
                case KEY_UP:
                    highlight = (highlight - 1 + place_size) % place_size;

                    break;
                case KEY_DOWN:
                    highlight = (highlight + 1) % place_size;

                    break;
                case ESC:
                    endwin();
            }
            drawMenu(highlight, selected_place, place_size);
        }

        system(CLEAR_SCREEN);

        switch (highlight) {
            case 0:
                storyline(player1, 1);
                break;
            case 1:
                // to get to the kingdom's city hall
                highlight = 0;
                city_hall(place);
                drawMenu(highlight, returnTo, 1);
                while ((ch = getch()) != '\n') {
                    system(CLEAR_SCREEN);
                    center_text("City Hall", 10);
                    switch (ch) {
                        case KEY_UP:
                            highlight = (highlight - 1 + 1) % 1;
        
                            break;
                        case KEY_DOWN:
                            highlight = (highlight + 1) % 1;
        
                            break;
                        case ESC:
                            endwin();
                    }
                    city_hall(place);
                    drawMenu(highlight, returnTo, 1);
                };
                break;
            case 2:
                // to explore the kingdom's city square
                highlight = 0;
                read_commonfolk(cfolk, a, b, c, d, place);
                drawMenu2d(highlight, cfolk, size_5);
                while ((ch = getch()) != '\n') {
                    system(CLEAR_SCREEN);
                    switch (ch) {
                        case KEY_UP:
                            highlight = (highlight - 1 + size_5) % size_5;
        
                            break;
                        case KEY_DOWN:
                            highlight = (highlight + 1) % size_5;
        
                            break;
                        case ESC:
                            endwin();
                    }
                    printf(PARCHMENT);
                    printDialog(highlight, a, b, c, d, place);
                    drawMenu2d(highlight, cfolk, size_5);
                    printf(PARCHMENT);
                }
                break;
            case 3:
                // to get to the kingdom's city library
                highlight = 0;
                library(place);
                break;
            case 4:
                // to travel around kingdoms faster
                highlight = 0;
                system(CLEAR_SCREEN);
                printf(CELESTIAL"\033[10;10H(Port Warden Bened-Dict): Aye! Where to?"PARCHMENT);
                drawMenu(highlight, destination, 5);
                while ((ch = getch()) != '\n') {
                    system(CLEAR_SCREEN);
                    switch (ch) {
                        case KEY_UP:
                            highlight = (highlight - 1 + 5) % 5;
        
                            break;
                        case KEY_DOWN:
                            highlight = (highlight + 1) % 5;
        
                            break;
                        case ESC:
                            endwin();
                    }
                    printf(CELESTIAL);
                    printf("\033[10;10H(Port Warden Bened-Dict): Travelling costs 120 Gold pieces");
                    printf("\033[11;10H(Port Warden Bened-Dict): Too expensive!? Price before was 150 Gold pieces!");
                    printf(PARCHMENT);
                    drawMenu(highlight, destination, 5);
                }

                system(CLEAR_SCREEN);

                if (highlight + 1 == place || highlight == 4) {
                    printf(CELESTIAL);
                    printf("\033[10;10H(Port Warden Bened-Dict): Don't want to see other kingdoms?");
                    printf("\033[11;10H(Port Warden Bened-Dict): That's a shame, well, Come back if you change yer mind aye!");
                    printf(PARCHMENT);
                    center_text("Press any key to continue", 17);
                    (void)getch();
                    break;
                }

                if  (player1->gold < 120) {
                    printf(CELESTIAL"\033[10;10H(Port Warden Bened-Dict): Might want to fill up your pockets first."PARCHMENT);
                    center_text("Press any key to continue", 17);
                    (void)getch();
                    break;
                } else {
                    switch (highlight) {
                    case 0:
                        spawn(player1, 1);
                        break;
                    case 1:
                        spawn(player1, 2);
                        break;
                    case 2:
                        spawn(player1, 3);
                        break;
                    default:
                        spawn(player1, 4);
                        break; 
                    }
                }
                
                break;
                
            case 5:
                // function for all adventurer guild events in all places, should take place as argument
                highlight = 0;
                center_text("Welcome to the Guild Hall", 10);
                center_text("Where hunters unite", 11);

                drawMenu(highlight, guild_hall, 4);
                while ((ch = getch()) != '\n') {
                    switch (ch) {
                        case KEY_UP:
                            highlight = (highlight - 1 + 4) % 4;
        
                            break;
                        case KEY_DOWN:
                            highlight = (highlight + 1) % 4;
        
                            break;
                        case ESC:
                            endwin();
                    }
                    drawMenu(highlight, guild_hall, 4);
                }

                switch (highlight) {
                case 0:
                    quest_board(player1, quest_rng1, quest_rng2, quest_rng3);
                    break;
                case 1:
                    guild_shop(player1);
                    break;
                case 2:
                    rest_area(player1);
                    break;
                case 3:
                default:
                    break;
                }

                break;
            case 6:
                // To get to the blacksmith shop
                highlight = 2;
                while (1) {
                    system(CLEAR_SCREEN);   
                    printf(STARDUST);
                    center_text("Welcome to the blacksmith's shop", 9);
                    printf(PARCHMENT);
                    blacksmith(player1, place, weapons, highlight, 0);
                    drawMenu2d(highlight, weapons, 3);
                    while ((ch = getch()) != '\n') {
                        system(CLEAR_SCREEN);
                        switch (ch) {
                            case KEY_UP:
                                highlight = (highlight - 1 + 3) % 3;
        
                                break;
                            case KEY_DOWN:
                                highlight = (highlight + 1) % 3;
        
                                break;
                            case ESC:
                                endwin();
                        }
                        printf(PARCHMENT);
                        blacksmith(player1, place, weapons, highlight, 1);
                        printf(STARDUST);
                        center_text("Welcome to the blacksmith's shop", 9);
                        drawMenu2d(highlight, weapons, 3);
                        printf(PARCHMENT);
                    }
        
                    system(CLEAR_SCREEN);
                    if (highlight == 2) {
                        break;
                    }
        
                    tempHold = highlight;
                    highlight = 0;
                    center_text("Are you sure you want to buy this item?", 14);
                    drawMenu(highlight, yesOrNo, 2);
                    printf(PARCHMENT);
                    while ((ch = getch()) != '\n') {
                        switch (ch) {
                            case KEY_UP:
                                highlight = (highlight - 1 + 2) % 2;
        
                                break;
                            case KEY_DOWN:
                                highlight = (highlight + 1) % 2;
        
                                break;
                            case ESC:
                                endwin();
                        }
                        drawMenu(highlight, yesOrNo, 2);
                        printf(PARCHMENT);
                    }
        
                    if (highlight == 0) {
                        blacksmith(player1, place, weapons, tempHold, 2);
                    } else {
                        system(CLEAR_SCREEN);
                    }
                }
                break;
            case 7:
                // Potion Master
                highlight = 0;
                printf(STARDUST);
                center_text("Potion Shop", 10);
                printf(PARCHMENT);
                drawMenu(highlight, potions, size_5);
                while ((ch = getch()) != '\n') {
                    system(CLEAR_SCREEN);
                    printf(STARDUST);
                    center_text("Potion Shop", 10);
                    switch (ch) {
                        case KEY_UP:
                            highlight = (highlight - 1 + size_5) % size_5;
        
                            break;
                        case KEY_DOWN:
                            highlight = (highlight + 1) % size_5;
        
                            break;
                        case ESC:
                            endwin();
                    }
                    switch (highlight) {
                    case 0:
                        printf(GOLD);                        
                        center_text("Price: 20", 13);
                        printf(COSMIC_BLUE);
                        center_text("Regenerates 30 Health Points instantly", 14);                        
                        break;
                    case 1:
                        printf(GOLD);
                        center_text("Price: 40", 13);
                        printf(COSMIC_BLUE);
                        center_text("Regenerates 45 Health Points instantly", 14);
                        break;
                    case 2:
                        printf(GOLD);
                        center_text("Price: 80", 13);
                        printf(COSMIC_BLUE);
                        center_text("Regenerates 60 Health Points instantly", 14);
                        break;
                    case 3:
                        printf(GOLD);
                        center_text("Price: 110", 13);
                        printf(COSMIC_BLUE);
                        center_text("Regenerates 80 Health Points instantly", 14);
                        break;
                    
                    }
                    printf(PARCHMENT);
                    drawMenu(highlight, potions, size_5);
                }
                switch (highlight) {
                case 0:
                    player1->bag[9]+=1;
                    strcpy(potion_cart, "Lesser Healing Potion");
                    potion_price = 20;
                    break;
                case 1:
                    player1->bag[10]+=1;
                    strcpy(potion_cart, "Healing Potion");
                    potion_price = 40;
                    break;
                case 2:
                    player1->bag[11]+=1;
                    strcpy(potion_cart, "Greater Healing Potion");
                    potion_price = 80;
                    break;
                case 3:
                    player1->bag[12]+=1;
                    strcpy(potion_cart, "Super Healing Potion");
                    potion_price = 110;
                    break;
                
                default:
                    break;
                }
                
                if (highlight == 4) {
                    break;
                }
                
                system(CLEAR_SCREEN);
                tempgold = player1->gold - potion_price;
                if (tempgold >= 0) {
                    player1->gold-=potion_price;
                    strcpy(tmp_msg, "You bought the ");
                    strcat(tmp_msg, potion_cart);
                    center_text(tmp_msg, 10);
                    center_text("Press any key to continue...", 17);
                    (void)getch();
                    save_game(player1);
                } else {
                    center_text("You do not have enough gold", 10);
                    center_text("Press any key to continue...", 17);
                }

                (void)getch();
                break;
            case 8:
                // function for all Exit city events in all places, should take place as argument
                system(CLEAR_SCREEN);
                highlight = 0;
                if (player1->wpnID == 0) {
                    printf(STARDUST);
                    center_text("Please buy a weapon first from the blacksmith shop before exiting the city.", 10);
                    printf(PARCHMENT);
                    SLEEP(1200);
                    spawn(player1, place);
                }
                expanse(place, player1);
                break;
            case 9:
                // function for all inventory events in all places, should take place as argument
                highlight = 0;
                check_inventory(player1, 0);
                break;
            case 10:
                // functio to return to main menu
                main();
            default:
                break;
        }

        highlight = 0;
    }

}

void city_hall (int place) {
    FILE *ifp_1 = fopen("resources/hall/city_hall_1.txt", "rt");

    char dialog_container[16][90];
    int count, max, j = 15;

    for (int i = 0; i < 16; i++) {
        fgets(dialog_container[i], sizeof(dialog_container), ifp_1);
    }
    
    switch (place) {
    case 1:
        count = 0;
        max = 3;
        break;
    
    case 3:
        count = 8;
        max = 11;
        break;

    case 4:
        count = 12;
        max = 15;
        break;

    default:
        count = 4;
        max = 7;
        break;    
    }

    for (int i = count; i < max; i++) {
        printf(PARCHMENT);
        center_text(dialog_container[i], j);
        j++;
    }

}

void read_commonfolk(char cfolk[][100], int a, int b, int c, int d, int place) {

    char folks[12][100]; // Store 12 lines from file

    FILE *ifp = fopen("resources/city_square/avalorian.txt", "rt");
    FILE *ifp_2 = fopen("resources/city_square/aetherisian.txt", "rt");
    FILE *ifp_3 = fopen("resources/city_square/thalnorian.txt", "rt");
    FILE *ifp_4 = fopen("resources/city_square/valerian.txt", "rt");
    
    FILE *file = NULL;

    switch (place) {
    case 1:
        file = ifp;
        break;
    case 3:
        file = ifp_3;
        break;
    case 4:
        file = ifp_4;
        break;
    default:
        file = ifp_2;
        break;
    }

    for (int i = 0; i < 12; i++) {
        fgets(folks[i], sizeof(folks[i]), file);
    }

    fclose(file);

    strcpy(cfolk[0], folks[a]);
    strcpy(cfolk[1], folks[b]);
    strcpy(cfolk[2], folks[c]);
    strcpy(cfolk[3], folks[d]);
    strcpy(cfolk[4], "          Return          ");

    for (int i = 0; i < 4; i++) {
        cfolk[i][27] = '\0';
    }

}

void printDialog(int highlight, int a, int b, int c, int d, int place) {
    char line_1[130];
    char line_2[130];
    char cfolk_dialog[12][200];
    FILE *ifp = fopen("resources/city_square/avalorianDialog.txt", "rt");
    FILE *ifp_2 = fopen("resources/city_square/aetherisianDialog.txt", "rt");
    FILE *ifp_3 = fopen("resources/city_square/thalnorianDialog.txt", "rt");
    FILE *ifp_4 = fopen("resources/city_square/valerianDialog.txt", "rt");

    FILE *file = NULL;

    switch (place) {
    case 1:
        file = ifp;
        break;
    case 3:
        file = ifp_3;
        break;
    case 4:
        file = ifp_4;
        break;
    default:
        file = ifp_2;
        break;
    }

    for (int i = 0; i < 12; i++) {
        fgets(cfolk_dialog[i], sizeof(cfolk_dialog[i]), file);
    }

    fclose(file);

    switch (highlight) {
    case 0:
        strncpy(line_1, cfolk_dialog[a], 125);
        strncpy(line_2, cfolk_dialog[a] + 125, 70);
        center_text(line_1, 12);
        center_text(line_2, 13);
        break;

    case 1:
        strncpy(line_1, cfolk_dialog[b], 125);
        strncpy(line_2, cfolk_dialog[b] + 125, 70);
        center_text(line_1, 12);
        center_text(line_2, 13);
        break;

    case 2:
        strncpy(line_1, cfolk_dialog[c], 125);
        strncpy(line_2, cfolk_dialog[c] + 125, 70);
        center_text(line_1, 12);
        center_text(line_2, 13);
        break;

    case 3:
        strncpy(line_1, cfolk_dialog[d], 125);
        strncpy(line_2, cfolk_dialog[d] + 125, 70);
        center_text(line_1, 12);
        center_text(line_2, 13);
        break;

    default:
        break;
    }

}

void library(int place) {
    char story[7][110];
    int j = 10;
    FILE *ifp = NULL;
    char *library_options[] = {
        "          Flora           ",
        "         Bestiary         ",
        "         History          ",
        "    Return to kingdom     "
    };

    while (1) {
        system(CLEAR_SCREEN);
        printf(STARDUST);
        printf("\033[8;10HWelcome to the kingdom's library");
        highlight = 0;
        drawMenu(highlight, library_options, 4);
        while ((ch = getch()) != '\n') {
            system(CLEAR_SCREEN);
            printf(STARDUST);
            printf("\033[8;10HWelcome to the kingdom's library");
            switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + 4) % 4;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 4;
                break;
            case ESC:
                endwin();
            }
            switch (highlight) {
                case 0:
                    center_text("Read about the plants of Eldermyst?", 12);
                    break;
                case 1:
                    center_text("Read about the beasts of Eldermyst?", 12);
                    break;
                case 2:
                    center_text("Read about the History of Eldermyst?", 12);
                    break;
                default:
                    break;
                }
            drawMenu(highlight, library_options, 4);
        }

        system(CLEAR_SCREEN);

        switch (highlight) {
        case 0:
            ifp = fopen("resources/library/flora.txt", "rt");    
            break;
        case 1:
            ifp = fopen("resources/library/bestiary.txt", "rt");
            break;
        case 2:
            ifp = fopen("resources/library/history.txt", "rt");
            break;
        default:
            break;
        }
        if (highlight == 3) {
            return;
        }

        printf(STARDUST);
        j = 10;
        for (int i = 0; i < 7; i++) {
            fgets(story[i], sizeof(story[i]), ifp);
            center_text(story[i], j);
            j++;
        }

        fclose(ifp);
        printf(PARCHMENT);
        center_text("Press any key to continue...", 21);
        getch();
    }
}

void blacksmith(struct player *player1, int place, char weapons[][100], int highlight, int type) {
    char temp[22][230];
    int cutoff;
    int newline;
    char line_1[111];
    char line_2[211];
    char tmp_msg[12];
    char tmp_num[6];
    int price[2];
    int temp_wpnID[2];
    int temp_attr[10];

    FILE *t1sw = NULL;
    FILE *t1hm = NULL;
    FILE *t1bo = NULL;
    FILE *t1gu = NULL;
    FILE *t1st = NULL;
    FILE *t1to = NULL;
    FILE *t1si = NULL;
    FILE *t1da = NULL;
    
    switch (place) {
    case 1:
        t1sw = fopen("resources/blacksmith/AV/t1sw.txt", "rt");
        t1hm = fopen("resources/blacksmith/AV/t1hm.txt", "rt");
        t1bo = fopen("resources/blacksmith/AV/t1bo.txt", "rt");
        t1gu = fopen("resources/blacksmith/AV/t1gu.txt", "rt");
        t1st = fopen("resources/blacksmith/AV/t1st.txt", "rt");
        t1to = fopen("resources/blacksmith/AV/t1to.txt", "rt");
        t1si = fopen("resources/blacksmith/AV/t1si.txt", "rt");
        t1da = fopen("resources/blacksmith/AV/t1da.txt", "rt");
        break;
    case 3:
        t1sw = fopen("resources/blacksmith/TH/t1sw.txt", "rt");
        t1hm = fopen("resources/blacksmith/TH/t1hm.txt", "rt");
        t1bo = fopen("resources/blacksmith/TH/t1bo.txt", "rt");
        t1gu = fopen("resources/blacksmith/TH/t1gu.txt", "rt");
        t1st = fopen("resources/blacksmith/TH/t1st.txt", "rt");
        t1to = fopen("resources/blacksmith/TH/t1to.txt", "rt");
        t1si = fopen("resources/blacksmith/TH/t1si.txt", "rt");
        t1da = fopen("resources/blacksmith/TH/t1da.txt", "rt");
        break;
    case 4:
        t1sw = fopen("resources/blacksmith/VA/t1sw.txt", "rt");
        t1hm = fopen("resources/blacksmith/VA/t1hm.txt", "rt");
        t1bo = fopen("resources/blacksmith/VA/t1bo.txt", "rt");
        t1gu = fopen("resources/blacksmith/VA/t1gu.txt", "rt");
        t1st = fopen("resources/blacksmith/VA/t1st.txt", "rt");
        t1to = fopen("resources/blacksmith/VA/t1to.txt", "rt");
        t1si = fopen("resources/blacksmith/VA/t1si.txt", "rt");
        t1da = fopen("resources/blacksmith/VA/t1da.txt", "rt");
        break;
    default:
        t1sw = fopen("resources/blacksmith/AE/t1sw.txt", "rt");
        t1hm = fopen("resources/blacksmith/AE/t1hm.txt", "rt");
        t1bo = fopen("resources/blacksmith/AE/t1bo.txt", "rt");
        t1gu = fopen("resources/blacksmith/AE/t1gu.txt", "rt");
        t1st = fopen("resources/blacksmith/AE/t1st.txt", "rt");
        t1to = fopen("resources/blacksmith/AE/t1to.txt", "rt");
        t1si = fopen("resources/blacksmith/AE/t1si.txt", "rt");
        t1da = fopen("resources/blacksmith/AE/t1da.txt", "rt");
        break;
    }

    switch (player1->classID) {
        case 20000001:
            fgets(temp[0], sizeof(temp[0]), t1sw);
            fgets(temp[2], sizeof(temp[2]), t1sw);
            fgets(temp[4], sizeof(temp[4]), t1sw);
            fgets(temp[6], sizeof(temp[6]), t1sw);
            fgets(temp[8], sizeof(temp[8]), t1sw);
            fgets(temp[10], sizeof(temp[10]), t1sw);
            fgets(temp[12], sizeof(temp[12]), t1sw);
            fgets(temp[14], sizeof(temp[14]), t1sw);
            fgets(temp[16], sizeof(temp[16]), t1sw);
            fgets(temp[18], sizeof(temp[18]), t1sw);
            fgets(temp[20], sizeof(temp[20]), t1sw);

            fgets(temp[1], sizeof(temp[1]), t1hm);
            fgets(temp[3], sizeof(temp[3]), t1hm);
            fgets(temp[5], sizeof(temp[5]), t1hm);
            fgets(temp[7], sizeof(temp[7]), t1hm);
            fgets(temp[9], sizeof(temp[9]), t1hm);
            fgets(temp[11], sizeof(temp[11]), t1hm);
            fgets(temp[13], sizeof(temp[13]), t1hm);
            fgets(temp[15], sizeof(temp[15]), t1hm);
            fgets(temp[17], sizeof(temp[17]), t1hm);
            fgets(temp[19], sizeof(temp[19]), t1hm);
            fgets(temp[21], sizeof(temp[21]), t1hm);

            fclose(t1sw);
            fclose(t1hm);
            break;

        case 20000003:
            fgets(temp[0], sizeof(temp[0]), t1bo);
            fgets(temp[2], sizeof(temp[2]), t1bo);
            fgets(temp[4], sizeof(temp[4]), t1bo);
            fgets(temp[6], sizeof(temp[6]), t1bo);
            fgets(temp[8], sizeof(temp[8]), t1bo);
            fgets(temp[10], sizeof(temp[10]), t1bo);
            fgets(temp[12], sizeof(temp[12]), t1bo);
            fgets(temp[14], sizeof(temp[14]), t1bo);
            fgets(temp[16], sizeof(temp[16]), t1bo);
            fgets(temp[18], sizeof(temp[18]), t1bo);
            fgets(temp[20], sizeof(temp[20]), t1bo);

            fgets(temp[1], sizeof(temp[1]), t1gu);
            fgets(temp[3], sizeof(temp[3]), t1gu);
            fgets(temp[5], sizeof(temp[5]), t1gu);
            fgets(temp[7], sizeof(temp[7]), t1gu);
            fgets(temp[9], sizeof(temp[9]), t1gu);
            fgets(temp[11], sizeof(temp[11]), t1gu);
            fgets(temp[13], sizeof(temp[13]), t1gu);
            fgets(temp[15], sizeof(temp[15]), t1gu);
            fgets(temp[17], sizeof(temp[17]), t1gu);
            fgets(temp[19], sizeof(temp[19]), t1gu);
            fgets(temp[21], sizeof(temp[21]), t1gu);

            fclose(t1bo);
            fclose(t1gu);
            break;
        case 20000004:
            fgets(temp[0], sizeof(temp[0]), t1da);
            fgets(temp[2], sizeof(temp[2]), t1da);
            fgets(temp[4], sizeof(temp[4]), t1da);
            fgets(temp[6], sizeof(temp[6]), t1da);
            fgets(temp[8], sizeof(temp[8]), t1da);
            fgets(temp[10], sizeof(temp[10]), t1da);
            fgets(temp[12], sizeof(temp[12]), t1da);
            fgets(temp[14], sizeof(temp[14]), t1da);
            fgets(temp[16], sizeof(temp[16]), t1da);
            fgets(temp[18], sizeof(temp[18]), t1da);
            fgets(temp[20], sizeof(temp[20]), t1da);

            fgets(temp[1], sizeof(temp[1]), t1si);
            fgets(temp[3], sizeof(temp[3]), t1si);
            fgets(temp[5], sizeof(temp[5]), t1si);
            fgets(temp[7], sizeof(temp[7]), t1si);
            fgets(temp[9], sizeof(temp[9]), t1si);
            fgets(temp[11], sizeof(temp[11]), t1si);
            fgets(temp[13], sizeof(temp[13]), t1si);
            fgets(temp[15], sizeof(temp[15]), t1si);
            fgets(temp[17], sizeof(temp[17]), t1si);
            fgets(temp[19], sizeof(temp[19]), t1si);
            fgets(temp[21], sizeof(temp[21]), t1si);

            fclose(t1si);
            fclose(t1da);
            break;
        default:
            fgets(temp[0], sizeof(temp[0]), t1st);
            fgets(temp[2], sizeof(temp[2]), t1st);
            fgets(temp[4], sizeof(temp[4]), t1st);
            fgets(temp[6], sizeof(temp[6]), t1st);
            fgets(temp[8], sizeof(temp[8]), t1st);
            fgets(temp[10], sizeof(temp[10]), t1st);
            fgets(temp[12], sizeof(temp[12]), t1st);
            fgets(temp[14], sizeof(temp[14]), t1st);
            fgets(temp[16], sizeof(temp[16]), t1st);
            fgets(temp[18], sizeof(temp[18]), t1st);
            fgets(temp[20], sizeof(temp[20]), t1st);

            fgets(temp[1], sizeof(temp[1]), t1to);
            fgets(temp[3], sizeof(temp[3]), t1to);
            fgets(temp[5], sizeof(temp[5]), t1to);
            fgets(temp[7], sizeof(temp[7]), t1to);
            fgets(temp[9], sizeof(temp[9]), t1to);
            fgets(temp[11], sizeof(temp[11]), t1to);
            fgets(temp[13], sizeof(temp[13]), t1to);
            fgets(temp[15], sizeof(temp[15]), t1to);
            fgets(temp[17], sizeof(temp[17]), t1to);
            fgets(temp[19], sizeof(temp[19]), t1to);
            fgets(temp[21], sizeof(temp[21]), t1to);

            fclose(t1st);
            fclose(t1to);
            break;
    }

    temp[0][strcspn(temp[0], "\n")] = 0;
    temp[1][strcspn(temp[1], "\n")] = 0;
    temp[2][strcspn(temp[2], "\n")] = 0;
    temp[3][strcspn(temp[3], "\n")] = 0;

    strcpy(weapons[0], temp[0]);
    strcpy(weapons[1], temp[1]);

    sscanf(temp[4], "%d", &price[0]);
    sscanf(temp[5], "%d", &price[1]);

    sscanf(temp[6], "%d", &temp_wpnID[0]);
    sscanf(temp[7], "%d", &temp_wpnID[1]);

    sscanf(temp[8], "%d", &temp_attr[0]);
    sscanf(temp[9], "%d", &temp_attr[1]);
    
    sscanf(temp[10], "%d", &temp_attr[2]);
    sscanf(temp[11], "%d", &temp_attr[3]);
    
    sscanf(temp[12], "%d", &temp_attr[4]);
    sscanf(temp[13], "%d", &temp_attr[5]);

    sscanf(temp[14], "%d", &temp_attr[6]);
    sscanf(temp[15], "%d", &temp_attr[7]);

    sscanf(temp[18], "%d", &temp_attr[8]);
    sscanf(temp[19], "%d", &temp_attr[9]);

    strcpy(weapons[2], "          Return          ");

    switch (type) {
    case 1:
        switch (highlight) {
        case 0:
            cutoff = strlen(temp[2]);
            if (cutoff > 110) {
                newline = 110;
                strncpy(line_1, temp[2], newline);
                newline = cutoff - 110;
                strncpy(line_2, temp[2] + 110, newline);
            } else {
                strncpy(line_1, temp[2], newline);
                strcpy(line_2, "");
            }

            printf(STARDUST);
            center_text("Item Description", 15);

            printf(COSMIC_BLUE);
            printf("\033[12;10H%s",line_1);
            printf("\033[13;10H%s",line_2);

            strncpy(line_1, temp[20], 110);
            strncpy(line_2, temp[20] + 110, 110);

            printf(STARDUST);
            center_text("Skill Description", 15);
            
            printf(COSMIC_BLUE);
            center_text(line_1, 16);
            center_text(line_2, 17);

            strcpy(tmp_msg, "Price: ");
            sprintf(tmp_num, "%d", price[0]);
            strcat(tmp_msg, tmp_num);
            printf(GOLD);
            center_text(tmp_msg, 19);
            printf(PARCHMENT);
            break;

        case 1:
            cutoff = strlen(temp[2]);
            if (cutoff > 110) {
                strncpy(line_1, temp[2], 110);
                line_1[110] = '\0';
                strncpy(line_2, temp[2] + 110, cutoff - 110);
                line_2[cutoff - 110] = '\0';
            } else {
                strncpy(line_1, temp[2], cutoff);
                line_1[cutoff] = '\0';
                line_2[0] = '\0';
            }

            printf(STARDUST);
            center_text("Item Description", 15);

            printf(COSMIC_BLUE);
            printf("\033[12;10H%s",line_1);
            printf("\033[13;10H%s",line_2);

            strncpy(line_1, temp[21], 110);
            strncpy(line_2, temp[21] + 110, 110);

            printf(STARDUST);
            center_text("Skill Description", 15);
            
            printf(COSMIC_BLUE);
            center_text(line_1, 16);
            center_text(line_2, 17);

            strcpy(tmp_msg, "Price: ");
            sprintf(tmp_num, "%d", price[1]);
            strcat(tmp_msg, tmp_num);
            printf(GOLD);
            center_text(tmp_msg, 19);
            printf(PARCHMENT);
            break;

        default:
            system(CLEAR_SCREEN);
            break;
        }

        break;
    
    case 2:
        system(CLEAR_SCREEN);
        if (highlight == 0 && temp_wpnID[0] == player1->wpnID) {
            center_text("You already own this weapon.", 16);
            SLEEP(1000);
            break;
        }
        
        if (highlight == 1 && temp_wpnID[1] == player1->wpnID) {
            center_text("You already own this weapon.", 16);
            SLEEP(1000);
            break;
        }
        
        if (highlight == 0 && player1->gold < price[0]) {
            center_text("Not enough gold!", 16);
            SLEEP(1000);
            break;
        }
        
        if (highlight == 1 && player1->gold < price[1]) {
            center_text("Not enough gold!", 16);
            SLEEP(1000);
            break;
        }

        switch (highlight) {
        case 0:
            player1->dmg = temp_attr[0];
            player1->skill_dmg = temp_attr[2];
            player1->trig_die = temp_attr[4];
            player1->skill_cd = temp_attr[6];
            player1->wpnID = temp_wpnID[0];
            player1->wpn_type = temp_attr[8];
            strcpy(player1->wpn, weapons[0]);
            strcpy(player1->skill, temp[16]);
            player1->gold-=price[0];
            break;
        
        case 1:
            player1->dmg = temp_attr[1];
            player1->skill_dmg = temp_attr[3];
            player1->trig_die = temp_attr[5];
            player1->skill_cd = temp_attr[7];
            player1->wpnID = temp_wpnID[1];
            player1->wpn_type = temp_attr[9];
            strcpy(player1->wpn, weapons[1]);
            strcpy(player1->skill, temp[17]);
            player1->gold-=price[1];
            break;
        
        default:
            break;
        }

        system(CLEAR_SCREEN);
        center_text("Successfully bought Item", 16);
        SLEEP(1000);
        save_game(player1);
        break;

    default:
        break;
    }
}

void low_encounter(struct player *player1, int place) {
    system(CLEAR_SCREEN);

    srand((unsigned int)time(NULL));
    int encounter_rate = rand() % 25 + 1;
    int encounter_type = rand() % 2 + 1;
    // int encounter_type = 2;

    char cont_array[15][150];
    char tmp_msg[100];
    char tmp_msg2[100];
    int item_no;
    FILE *container = NULL;
    int temp;

    switch (encounter_type) {
    case 1:
        switch (encounter_rate) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                container = fopen("resources/flora/veilrose.txt", "rt");
                item_no = 0;
                break;
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                container = fopen("resources/flora/yormen_flora.txt", "rt");
                item_no = 1;
                break;
            case 11:
            case 12:
            case 13:
                container = fopen("resources/flora/trawnshroom.txt", "rt");
                item_no = 2;
                break;
            case 14:
            case 15:
            case 16:
                container = fopen("resources/flora/lumen_flora.txt", "rt");
                item_no = 3;
                break;
            case 17:
            case 18:
            case 19:
                container = fopen("resources/flora/orbroot.txt", "rt");
                item_no = 4;
                break;
            case 20:
            case 21:
            case 22:
                container = fopen("resources/flora/dreamvine.txt", "rt");
                item_no = 5;
                break;
            case 23:
            case 24:
                container = fopen("resources/flora/skyroot.txt", "rt");
                item_no = 6;
                break;
            case 25:
                container = fopen("resources/flora/wyrmblossom.txt", "rt");
                item_no = 7;
                break;
            default:
                exit(0);
                break;
            }
        break;
    case 2:
        switch (encounter_rate) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                container = fopen("resources/bestiary/thorkin.txt", "rt");
                break;
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                container = fopen("resources/bestiary/mystra.txt", "rt");
                break;
            case 11:
            case 12:
            case 13:
            case 14:
                container = fopen("resources/bestiary/lorian_wolf.txt", "rt");
                break;
            case 15:
            case 16:
            case 17:
            case 18:
                container = fopen("resources/bestiary/bogwood.txt", "rt");
                break;
            case 19:
            case 20:
                container = fopen("resources/bestiary/bronhyde.txt", "rt");
                break;
            case 21:
            case 22:
                container = fopen("resources/bestiary/pyltzer.txt", "rt");
                break;
            case 23:
            case 24:
                container = fopen("resources/bestiary/bandit.txt", "rt");
                break;
            case 25:
                container = fopen("resources/bestiary/bandit_captain.txt", "rt");
                break;
            default:
                container = fopen("resources/bestiary/pyltzer.txt", "rt");
            }
        break;
    }

    for (int i = 0; i < 15; i++) {
        fgets(cont_array[i], sizeof(cont_array[i]), container);
    }

    switch (encounter_type) {
    case 1:
        system(CLEAR_SCREEN);

        strcpy(tmp_msg, "You have found a ");
        strcpy(tmp_msg2,"Would you like to take the ");
        strcat(tmp_msg, cont_array[0]);
        strcat(tmp_msg2, cont_array[0]);
        center_text(tmp_msg, 8);
        center_text(tmp_msg2, 9);

        switch (item_no) {
            case 0:
            case 1:
            case 2:
                printf(STARLIGHT);
                break;
            case 3:
            case 4:
            case 5:
                printf(NEBULA);
                break;
            case 7:
                printf(DEEP_CRIMSON);
                break;
        }

        printf("\033[12;51H  ▒▒▒▒▒▒▒            ██     ");
        printf("\033[13;51H▓▓▓▓██▒▒   ▒▒▒▒▒▒▒  ██      ");
        printf("\033[14;51H  ▓▓▓▓██  ▓▓▓██▒▒ ██        ");   
        printf("\033[15;51H        ██ ▓▓▓▓▓██          ");
        printf("\033[16;51H █████  ██  ██    ▒▒▒▒▒▒██  ");
        printf("\033[17;51H   ███████  ██  ▒▒▒▒██▓▓▓ ██");        
        printf("\033[18;51H        ████    ██▓▓▓▓▓     ");
        printf("\033[19;51H          ██  ██            ");
        printf("\033[20;51H            ██              ");
        printf("\033[21;51H            ██              ");
        printf(PARCHMENT);
        SLEEP(500);

        drawMenu(highlight, yesOrNo, 2);
        printf(PARCHMENT);
        while ((ch = getch()) != '\n') {
            switch (ch) {
                case KEY_UP:
                    highlight = (highlight - 1 + 2) % 2;

                    break;
                case KEY_DOWN:
                    highlight = (highlight + 1) % 2;

                    break;
                case ESC:
                    endwin();
            }
            drawMenu(highlight, yesOrNo, 2);
            printf(PARCHMENT);
        }

        system(CLEAR_SCREEN);
        if (highlight == 1) {
            center_text("Did not find anything else....", 10);
            SLEEP(1000);
            break;
        }

        player1->bag[item_no]+=1;
        player1->exp += rand() % 12 + 1;
        tier1_quest.plantID = item_no;

        strcpy(tmp_msg, "You took the ");
        strcat(tmp_msg, cont_array[0]);
        center_text(tmp_msg, 10);
        center_text("Press any key to continue...", 17);
        getch();
        check_quest(player1);
        tier1_quest.plantID = 0;
        break;
    case 2:
        cont_array[0][strcspn(cont_array[0], "\n")] = '\0';

        strcpy(tier1_beast.name, cont_array[0]);
        sscanf(cont_array[4], "%d", &temp);
        tier1_beast.hit_die = temp;
        sscanf(cont_array[5], "%d", &temp);
        tier1_beast.hp = temp;
        sscanf(cont_array[6], "%d", &temp);
        tier1_beast.dmg = temp;
        sscanf(cont_array[7], "%d", &temp);
        tier1_beast.skill_dmg = temp;
        sscanf(cont_array[8], "%d", &temp);
        tier1_beast.trig_die = temp;
        sscanf(cont_array[9], "%d", &temp);
        tier1_beast.def = temp;
        sscanf(cont_array[10], "%d", &temp);
        tier1_beast.wpn_type = temp;
        sscanf(cont_array[11], "%d", &temp);
        tier1_beast.agl = temp;
        sscanf(cont_array[12], "%d", &temp);
        tier1_beast.exp_drop = temp;
        sscanf(cont_array[13], "%d", &temp);
        tier1_beast.gold_drop = temp;
        sscanf(cont_array[14], "%d", &temp);
        tier1_beast.beast_ID = temp;

        battleAI(player1, place);
        break;
    default:
        exit(0);
        break;
    }
    SLEEP(1000);
    expanse(place, player1);
}

void drink_potion(struct player *player1) {
    FILE *container;
    char *potion_options[] = {
        "  Lesser Healing Potion   ",
        "      Healing Potion      ",
        "  Greater Healing Potion  ",
        "   Super Healing Potion   ",
        "          Return          "
    };
    
    int item_no;

    highlight = 0;
    system(CLEAR_SCREEN);
    center_text("Drink Potion", 10);
    drawMenu(highlight, potion_options, size_5);
    while ((ch = getch()) != '\n') {
        system(CLEAR_SCREEN);
        printf(PARCHMENT);
        center_text("Drink Potion", 10);
        switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + size_5) % size_5;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % size_5;
                break;
            case ESC:
                endwin();
        }
        printf(PARCHMENT);
        switch (highlight) {
        case 0:
            center_text("Regenerates 30 Health Points instantly", 14);
            break;
        case 1:
            center_text("Regenerates 45 Health Points instantly", 14);
            break;
        case 2:
            center_text("Regenerates 60 Health Points instantly", 14);
            break;
        case 3:
            center_text("Regenerates 80 Health Points instantly", 14);
            break;
        }
        drawMenu(highlight, potion_options, size_5);
    }

    switch (highlight) {
    case 0:
        item_no = 9;
        break;
    case 1:
        item_no = 10;
        break;
    case 2:
        item_no = 11;
        break;
    case 3:
        item_no = 12;
        break;
    default:
        return;
        break;
    }

    system(CLEAR_SCREEN);
    if (player1->bag[item_no] > 0) {
        switch (highlight) {
        case 0:
            player1->hp+=30;
            break;
        case 1:
            player1->hp+=45;
            break;
        case 2:
            player1->hp+=60;
            break;
        case 3:
            player1->hp+=80;
            break;
        }

        player1->bag[item_no]-=1;
        
        center_text("Drank potion", 10);
        center_text("Press any to continue...", 17);
        getch();
        save_game(player1);
    if (player1->hp >= max_hp) {
        player1->hp = max_hp;
    }
        
    } else {
        center_text("You do not have enough of this item", 10);
        center_text("Press any to continue...", 17);
        getch();
    }
    
}

void battleAI(struct player *player1, int place) {
    char tmp_msg[100];
    char tmp_roll[32];
    int temp_def = player1->def, btemp_def = tier1_beast.def;
    int temp_acc = tier1_beast.hit_die;
    int beast_skl_activ = 0;
    int beast_skl_dur = 0;
    int DoT;
    int DoT_beast;
    int gold_drop;
    int turn = player1->skill_cd;
    int skill_activ = 0;
    int duration = 0;
    int trig_skill, d12;
    system(CLEAR_SCREEN);

    switch (tier1_beast.wpn_type) {
    case 2:
        printf(GOLD);
        break;
    case 4:
        printf(NEBULA);
        break;
    case 7:
        printf(GREEN);
        break;
    case 8:
        printf(STARLIGHT);        
        break;
    case 9:
    case 10:
        printf(STARDUST);
        break;
    case 11:
        printf(DEEP_CRIMSON);
        break;
    default:
        printf(FORGE_FIRE);
        break;
    }
    printf("\033[12;56H    ██      ██    ");
    printf("\033[13;56H    █ █    █ █    ");
    printf("\033[14;56H     █ █  █ █     ");
    printf("\033[15;56H      █ ██ █      ");
    printf("\033[16;56H       █ ██       ");
    printf("\033[17;56H       ██ █       ");
    printf("\033[18;56H ██   █ ██ █   ██ ");
    printf("\033[19;56H  ██ █ █  █ █ ██  ");
    printf("\033[20;56H   ████    ███    ");
    printf("\033[21;56H  █  ██   ██  █   ");
    printf("\033[22;56H █  █  █ █  █  █  ");
    printf("\033[23;56H ███         ███  ");
    printf(RESET);
    SLEEP(500);

    show_health_bar(player1);
    printf(PARCHMENT);
    strcpy(tmp_msg, "You have encountered a ");
    strcat(tmp_msg, tier1_beast.name);
    center_text(tmp_msg, 26);
    center_text("Press any key to continue...", 28);
    getch();

    while (1) {
        system(CLEAR_SCREEN);
        trig_skill = (rand() % tier1_beast.trig_die + 1);
        SLEEP(1000);

        show_health_bar(player1);

        if (beast_skl_activ != 2) {
            attack_beast(player1, &turn, &skill_activ, &duration, place);
        }

        if (skill_activ != 3) {
            player1->def = temp_def;
        }
        if (skill_activ != 4) {
            tier1_beast.hit_die = temp_acc;
        }

        if (tier1_beast.hp <= 0) {
            break;
        }
        if (skill_activ != 2) {
            attack_player(player1, &trig_skill, &beast_skl_activ, &beast_skl_dur);
        }

        if (tier1_beast.hp <= 0 || player1->hp <= 0) {
            break;
        }

        if (beast_skl_activ != 3) {
            tier1_beast.def = btemp_def;
        }

        // for checking DoT
        if (skill_activ == 1) {
            system(CLEAR_SCREEN);
            DoT = (rand() % player1->trig_die) + 1;
            DoT += player1->dmg;
            tier1_beast.hp-=DoT;
            strcpy(tmp_msg, tier1_beast.name);
            strcat(tmp_msg, " took ");
            sprintf(tmp_roll, "%d", DoT);
            strcat(tmp_msg, tmp_roll);
            strcat(tmp_msg, " bleeding damage");
            printf(DEEP_CRIMSON);
            center_text(tmp_msg, 14);
            printf(PARCHMENT);
            center_text("Press any key to continue...", 17);
            getch();
        }

        if (beast_skl_activ == 1) {
            DoT_beast = (rand() % tier1_beast.trig_die) + 1;
            DoT_beast += tier1_beast.dmg;
            player1->hp-=DoT_beast;
            system(CLEAR_SCREEN);
            strcpy(tmp_msg, "You");
            strcat(tmp_msg, " took ");
            sprintf(tmp_roll, "%d", DoT_beast);
            strcat(tmp_msg, tmp_roll);
            strcat(tmp_msg, " bleeding damage");
            printf(DEEP_CRIMSON);
            center_text(tmp_msg, 14);
            printf(PARCHMENT);
            center_text("Press any key to continue...", 17);
            getch();
        }
        
        if (duration == 0) {
            skill_activ = 0;
        }

        if (beast_skl_dur == 0) {
            beast_skl_activ = 0;
        }

        if (tier1_beast.hp <= 0 || player1->hp <= 0) {
            break;
        }
        
        turn++;
    }

    if (tier1_beast.hp <= 0) {
        system(CLEAR_SCREEN);
        strcpy(tmp_msg, "You slayed the ");
        strcat(tmp_msg, tier1_beast.name);
        center_text(tmp_msg, 10);
        memset(tmp_msg, 0, sizeof(tmp_msg));

        strcpy(tmp_msg, "Gained ");
        sprintf(tmp_roll, "%d", tier1_beast.exp_drop);
        strcat(tmp_msg, tmp_roll);
        strcat(tmp_msg, " Exp");
        center_text(tmp_msg, 11);

        memset(tmp_msg, 0, sizeof(tmp_msg));
        gold_drop = rand() % tier1_beast.gold_drop + 1;
        strcpy(tmp_msg, "Found ");
        sprintf(tmp_roll, "%d", gold_drop);
        strcat(tmp_msg, tmp_roll);
        strcat(tmp_msg, " Gold");
        center_text(tmp_msg, 12);

        player1->gold+=gold_drop;
        player1->exp+=tier1_beast.exp_drop;
        

        tier1_quest.killID = tier1_beast.beast_ID;
        center_text("Press any key to continue...", 17);
        getch();
        system(CLEAR_SCREEN);
        check_quest(player1);
    }

    if (player1->hp <= 0) {
        d12 = rand() % 12 + 1;
        system(CLEAR_SCREEN);
        highlight = 0;
        center_text("Adventurer! good heavens you're awake!", 10);
        center_text("We found you outside the kingdom unconcious.", 11);
        player1->hp = 100;
        drawMenu(highlight, continueTo, 1);
        while ((ch = getch()) != '\n') {
            switch (ch) {
                case KEY_UP:
                    highlight = (highlight - 1 + 1) % 1;
                    break;
                case KEY_DOWN:
                    highlight = (highlight + 1) % 1;

                    break;
                case ESC:
                    endwin();
            }
            drawMenu(highlight, continueTo, 1);
        }
        player1->exp-=tier1_beast.exp_drop;
        player1->gold-=d12;
        if(player1->exp <= 0) {
            player1->exp = 0;
        }
        if(player1->gold <= 0) {
            player1->gold = 0;
        }
        system(CLEAR_SCREEN);
        spawn(player1, place);
    }

}

void attack_beast(struct player *player1, int *turn, int *skill_activ, int *duration, int place) {
    srand((unsigned int)time(NULL));
    int hit_die, hit_dmg, trig_die, atk_cnt, tmp_dmg = 0, multi_damage;
    char tmp_msg[100], tmp_roll[32];
    int pass = 0;
    int armor = tier1_beast.def;
    char *attack_menu[4];
    char skill[26];

    snprintf(skill, sizeof(skill), "%s", player1->skill);

    attack_menu[0] = "       Basic Attack      ";
    attack_menu[1] = skill;
    attack_menu[2] = "           Run           ";
    attack_menu[3] = "        Inventory        ";

    while(1) {
        show_health_bar(player1);
        highlight = 0;
        drawMenu(highlight, attack_menu, 4);
        while ((ch = getch()) != '\n') {
            switch (ch) {
                case KEY_UP:
                    highlight = (highlight - 1 + 4) % 4;
                    break;
                case KEY_DOWN:
                    highlight = (highlight + 1) % 4;

                    break;
                case ESC:
                    endwin();
            }
            drawMenu(highlight, attack_menu, 4);
        }

        hit_die = rand() % 20 + 1;
        hit_dmg = rand() % 10 + 1;
        trig_die = rand() % player1->trig_die + 1;

        system(CLEAR_SCREEN);
        pass = 0;
        switch (highlight) {
        case 0:
        case 1:
            strcpy(tmp_msg, "You rolled ");
            sprintf(tmp_roll, "%d", hit_die);
            strcat(tmp_msg, tmp_roll);
            center_text(tmp_msg, 10);
            switch (highlight) {
            case 0:
                if (hit_die == 20) {
                    hit_dmg = 12;
                    hit_dmg += player1->dmg;
                } else {
                    hit_dmg += player1->dmg;
                }
                break;
            case 1:
                if (*turn >= player1->skill_cd) {
                    if (hit_die == 20) {
                        hit_dmg = 15;
                        hit_dmg += player1->skill_dmg;
                        hit_dmg += player1->dmg;
                    } else {
                        hit_dmg += player1->skill_dmg;
                    }
                    *turn = 0;
                    break;
                } else {
                    system(CLEAR_SCREEN);
                    show_health_bar(player1);
                    printf(PARCHMENT);
                    center_text("Skill on cooldown!", 10);
                    center_text("Press any key to continue...", 17);
                    getch();
                    system(CLEAR_SCREEN);
                    pass = 1;
                    break;
                }
            }

            if (pass == 0) {
                if (hit_die < tier1_beast.agl) {
                    system(CLEAR_SCREEN);            
                    center_text(tmp_msg, 10);
                    SLEEP(100);
                    if (*duration > 0) {
                        strcpy(tmp_msg, "Skill duration: ");
                        sprintf(tmp_roll, "%d", *duration);
                        strcat(tmp_msg, tmp_roll);
                        center_text(tmp_msg, 13);
                        *duration-=1;
                    }
                    center_text("You missed!", 11);
                    
                    if (highlight == 1) {
                        *turn = 1;
                    }
                    center_text("Press any key to continue...", 17);
                    getch();
                    break;
                } else {
                    if (*skill_activ <= 0) {
                        SLEEP(150);
                        if (*turn == 0) {
                            switch (player1->wpn_type) {
                            case 2:
                                printf(GOLD);
                                center_text("Skill Activated!", 11);
                                center_text("---Enemy Immobilized---", 12);
                                *duration = (rand() % player1->trig_die) + 1;
                                *duration += 1;
                                *skill_activ = 2;
                                break;
                            case 3:
                                printf(CELESTIAL);
                                center_text("Skill Activated!", 11);
                                center_text("---Damage Multiplied---", 12);
                                strcpy(tmp_msg, "Damage + ");
                                sprintf(tmp_roll, "%d", hit_dmg);
                                strcat(tmp_msg, tmp_roll);
                                center_text(tmp_msg, 13);
                                hit_dmg += hit_dmg;
                                hit_dmg += tier1_beast.def;
                                break;
                            case 4:
                                printf(NEBULA);
                                center_text("Skill Activated!", 11);
                                center_text("---Multi attack---", 12);
                                multi_damage = 0;
                                trig_die = rand() % player1->trig_die + 1;
                                for (int i = 0; i < trig_die; i++) {
                                    tmp_dmg = rand() % player1->dmg + 1;
                                    multi_damage += tmp_dmg;
                                }
                                hit_dmg += multi_damage;
                                strcpy(tmp_msg, "Hit ");
                                sprintf(tmp_roll, "%d", trig_die);
                                strcat(tmp_msg, tmp_roll);
                                strcat(tmp_msg, " times");
                                center_text(tmp_msg, 13);
                                break;
                            case 5:
                                printf(COSMIC_BLUE);
                                center_text("Skill Activated!", 11);
                                center_text("---Enemy accuracy reduced---", 12);
                                tier1_beast.hit_die -= 2;
                                *duration = trig_die;
                                *duration += 1;
                                *skill_activ = 4;
                                strcpy(tmp_msg, "Enemy Accuracy Reduced by 10%");
                                center_text(tmp_msg, 13);
                                break;
                            case 6:
                                printf(MOUNTAIN_GRAY);
                                center_text("Skill Activated!", 11);
                                center_text("---Skill cooldown reduced---", 12);
                                turn++;
                                break;
                            case 7:
                                printf(GREEN);
                                center_text("Skill Activated!", 11);
                                center_text("---Lifesteal---", 12);
                                player1->hp+=trig_die;
                                if (player1->hp >= max_hp) {
                                    player1->hp = max_hp;
                                }
                                strcpy(tmp_msg, "Health points + ");
                                sprintf(tmp_roll, "%d", trig_die);
                                strcat(tmp_msg, tmp_roll);
                                center_text(tmp_msg, 13);
                                break;
                            case 8:
                                printf(STARLIGHT);
                                center_text("---True Damage---", 12);
                                center_text("Skill Activated!", 11);
                                hit_dmg += tier1_beast.def;
                                break;
                            case 9:
                            case 10:
                                printf(STARDUST);
                                center_text("Skill Activated!", 11);
                                center_text("---Conjure Shield---", 12);
                                player1->def += 2;
                                *duration = trig_die;
                                *duration += 1;
                                *skill_activ = 3;
                                strcpy(tmp_msg, "Armor +  ");
                                sprintf(tmp_roll, "%d", trig_die);
                                strcat(tmp_msg, tmp_roll);
                                center_text(tmp_msg, 13);
                                break;
                            case 11:
                                printf(DEEP_CRIMSON);
                                center_text("Skill Activated!", 11);
                                center_text("---Damage over Time---", 12);
                                *duration = trig_die;
                                *duration += 1;
                                *skill_activ = 1;
                                break;
                            case 19:
                                tier1_beast.hp -= 100;
                                break;
                            default:
                                break;
                            }
                        } else {}
                    
                    }

                    if (*duration > 0) {
                        strcpy(tmp_msg, "Skill duration: ");
                        sprintf(tmp_roll, "%d", *duration);
                        strcat(tmp_msg, tmp_roll);
                        center_text(tmp_msg, 14);
                        *duration-=1;
                    }

                    strcpy(tmp_msg, "You dealt ");

                    hit_dmg = armor - hit_dmg;
                    if (hit_dmg < 0) {
                        tier1_beast.hp -= abs(hit_dmg);
                    } else {
                        hit_dmg = 0;
                    }

                    sprintf(tmp_roll, "%d", hit_dmg);
                    strcat(tmp_msg, tmp_roll);
                    strcat(tmp_msg, " damage");
                    printf(FORGE_FIRE);
                    center_text(tmp_msg, 15);
                    printf(PARCHMENT);
                    center_text("Press any key to continue...", 17);
                    getch();
                    break;
                }
                break;
            }
            break;
        case 2:
            system(CLEAR_SCREEN);
            if (hit_die > tier1_beast.agl) {
                center_text("Successfully ran away", 10);
                center_text("Press any key to continue", 17);
                getch();
                expanse(place, player1);
            } else {
                center_text("Couldn't run away!", 10);
                center_text("Press any key to continue", 17);
                getch();
            }
            break;
        case 3:
            check_inventory(player1, 0);
            pass = 1;
        default:
            break;
        }
        if (pass == 0) {
            break;
        }
        system(CLEAR_SCREEN);
    }
    
}

void attack_player(struct player *player1, int *trig_skill, int *beast_skl_activ, int *beast_skl_dur) {
    srand((unsigned int)time(NULL));
    int hit_die = rand() % tier1_beast.hit_die + 1;
    int hit_dmg = rand() % 8 + 1;
    int tmp_trig = rand() % tier1_beast.trig_die + 1, tmp_dmg;
    char tmp_msg[50];
    char tmp_roll[32];
    int armor = player1->def;

    system(CLEAR_SCREEN);
    printf(PARCHMENT);
    strcpy(tmp_msg, tier1_beast.name);
    strcat(tmp_msg, " rolled ");
    sprintf(tmp_roll, "%d", hit_die);
    strcat(tmp_msg, tmp_roll);
    center_text(tmp_msg, 10);
    memset(tmp_msg, 0, sizeof(tmp_msg));
    SLEEP(250);
    if (hit_die < player1->agl) {
        strcpy(tmp_msg, tier1_beast.name);
        strcat(tmp_msg, " missed!");
        center_text(tmp_msg, 12);
        memset(tmp_msg, 0, sizeof(tmp_msg));
        if (*beast_skl_dur > 0) {
            strcpy(tmp_msg, "Skill Duration: ");
            sprintf(tmp_roll, "%d", *beast_skl_dur);
            strcat(tmp_msg, tmp_roll);
            center_text(tmp_msg, 13);
            memset(tmp_msg, 0, sizeof(tmp_msg));
            *beast_skl_dur -= 1;
        }
        center_text("Press any key to continue...", 17);
        getch();
    
    } else {
        if (hit_die == tier1_beast.hit_die) {
            hit_dmg = 10;
            hit_dmg += tier1_beast.skill_dmg;
        }
        if (*trig_skill >= tier1_beast.trig_die) {
            hit_dmg += tier1_beast.skill_dmg;

            if (*beast_skl_activ == 0) {
                switch (tier1_beast.wpn_type) {
                case 2:
                    printf(GOLD);
                    center_text("---You are Immobilized---", 12);
                    *beast_skl_dur = rand() % tier1_beast.trig_die + 1;
                    *beast_skl_dur += 1;
                    *beast_skl_activ = 2;
                    center_text("Skill Activated!", 11);
                    break;
                case 3:
                    center_text("---Damage Multiplied---", 12);
                    hit_dmg += hit_dmg;
                    center_text("Skill Activated!", 11);
                    break;
                case 4:
                    printf(NEBULA);
                    center_text("---Multi attack---", 12);
                    tmp_trig = rand() % tier1_beast.trig_die + 1;
                    for (int i = 1; i < tmp_trig; i++) {
                        tmp_dmg = rand() % 8 + 1;
                        tmp_dmg += tier1_beast.dmg;
                        hit_dmg += tmp_dmg;
                    }
                    strcpy(tmp_msg, "You got hit ");
                    sprintf(tmp_roll, "%d", tmp_trig);
                    strcat(tmp_msg, tmp_roll);
                    strcat(tmp_msg, " times");
                    center_text(tmp_msg, 13);
                    memset(tmp_msg, 0, sizeof(tmp_msg));
                    center_text("Skill Activated!", 11);
                    break;
                case 7:
                    printf(GREEN);
                    center_text("---Lifesteal---", 12);
                    tier1_beast.hp+=tmp_trig;
                    if (tier1_beast.hp >= 100) {
                        tier1_beast.hp = 100;
                    }
                    strcpy(tmp_msg, "Health points + ");
                    sprintf(tmp_roll, "%d", tmp_trig);
                    strcat(tmp_msg, tmp_roll);
                    center_text(tmp_msg, 13);
                    memset(tmp_msg, 0, sizeof(tmp_msg));
                    center_text("Skill Activated!", 11);
                    break;
                case 8:
                    
                    center_text("---True Damage---", 12);
                    hit_dmg += player1->def;
                    break;
                case 9:
                case 10:
                    printf(STARDUST);
                    center_text("---Conjure Shield---", 12);
                    tier1_beast.def += 2;
                    *beast_skl_dur = tmp_trig;
                    *beast_skl_activ = 3;
                    strcpy(tmp_msg, "Armor +  ");
                    sprintf(tmp_roll, "%d", tmp_trig);
                    strcat(tmp_msg, tmp_roll);
                    center_text(tmp_msg, 13);
                    memset(tmp_msg, 0, sizeof(tmp_msg));
                    center_text("Skill Activated!", 11);
                    break;
                case 11:
                    printf(FORGE_FIRE);
                    center_text("---Damage over Time---", 12);
                    *beast_skl_dur += (rand() % tier1_beast.trig_die) + 1;
                    *beast_skl_dur += 1;
                    *beast_skl_activ = 1;
                    center_text("Skill Activated!", 11);
                    break;
                default:
                    break;
            }
            
            }
        } else {
            hit_dmg += tier1_beast.dmg;
        }
        if (*beast_skl_dur > 0) {
            strcpy(tmp_msg, "Skill Duration: ");
            sprintf(tmp_roll, "%d", *beast_skl_dur);
            strcat(tmp_msg, tmp_roll);
            center_text(tmp_msg, 14);
            memset(tmp_msg, 0, sizeof(tmp_msg));
            *beast_skl_dur-=1;
        }
        strcpy(tmp_msg, tier1_beast.name);
        strcat(tmp_msg, " dealt ");

        hit_dmg = armor - hit_dmg;
        if (hit_dmg < 0) {
            player1->hp -= abs(hit_dmg);
        } else {
            hit_dmg = 0;
        }

        sprintf(tmp_roll, "%d", hit_dmg);
        strcat(tmp_msg, tmp_roll);
        strcat(tmp_msg, " damage");
        printf(FORGE_FIRE);
        center_text(tmp_msg, 15);
        printf(PARCHMENT);
        center_text("Press any key to continue...", 17);
        getch();
    
    }

}

void show_health_bar(struct player *player1) {
    int pcol = 13;
    int bcol = 93;
    int tempphp = player1->hp / hp_divisor;
    int tempbhp = tier1_beast.hp / 4;

    for (int i = 0; i < 25; i++) {
        printf("\033[4;%dH", pcol);
        printf(IRON"█"RESET);
        pcol++;
    }

    pcol = 13;

    for (int i = 0; i < tempphp; i++) {
        printf("\033[4;%dH", pcol);
        printf(GREEN"█"RESET);
        pcol++;
    }

    printf("\033[3;13H");
    printf("Adventurer");
    printf("\033[5;23H");
    printf(GREEN"%d%%"RESET, player1->hp);

    for (int i = 0; i < 25; i++) {
        printf("\033[4;%dH", bcol);
        printf(IRON"█"RESET);
        bcol++;
    }

    bcol = 93;

    for (int i = 0; i < tempbhp; i++) {
        printf("\033[4;%dH", bcol);
        printf(GREEN"█"RESET);
        bcol++;
    }

    printf("\033[3;93H");
    printf("%s", tier1_beast.name);
    printf("\033[5;104H");
    printf(GREEN"%d%%", tier1_beast.hp);
    printf(RESET);
}

void check_level(struct player *player1) {
    if (player1->level == 1) {
        max_exp = 100;
        max_hp = 100;
    }
    
    while (player1->exp >= max_exp) {
        if (player1->exp >= max_exp) {
            player1->exp -= max_exp;
            if (player1->exp <= 0) {
                player1->exp = 0;
            }
            player1->level += 1;
        }
    }
    
    switch (player1->level) {
        case 1:
            max_exp = 100;
            max_hp = 100;
            break;
        case 2:
            max_exp = 150;
            break;
        case 3:
            max_exp = 200;
            break;
        case 4:
            max_exp = 250;
            break;
        case 5:
            max_hp = 125;
            max_exp = 350;
            break;
        case 6:
            max_exp = 400;
            break;
        case 7:
            player1->agl += 1;
            max_exp = 450;
            break;
        case 8:
            max_exp = 500;
            break;
        case 9:
            max_exp = 650;
            break;
        case 10:
            max_hp = 150;
            player1->def += 1;
            max_exp = 750;
            break;
        case 11:
            max_exp = 825;
            break;
        case 12:
            max_exp = 900;
            break;
        case 13:
            max_exp = 975;
            break;
        case 14:
            max_exp = 1050;
            break;
        default:
            max_exp = 1125;
            break;
    }

    if (player1->level >= 2 && player1->dmg <= 6) {
        player1->dmg += 1;
        player1->skill_dmg += 1;
    }

    if (player1->level >= 3 && player1->dmg <= 8) {
        player1->dmg += 1;
        player1->skill_dmg += 1;
    }

    if (player1->level >= 6 && player1->dmg <= 10) {
        player1->dmg += 1;
        player1->skill_dmg += 1;
    }
    
    if (player1->level >= 7 && player1->dmg <= 12) {
        player1->dmg += 1;
        player1->skill_dmg += 1;
    }

    level_divisor = max_exp / 25;
    hp_divisor = max_hp / 25;
}

void storyline(struct player *player1, int type) {
    char storyline_container[10][150];
    FILE *storyline_file = NULL;
    
    system(CLEAR_SCREEN);
    switch (type) {
    case 0:
        storyline_file = fopen("resources/storylines/storyline_1.txt", "rt");
        for (int i = 0; i < 2; i++) {
            fgets(storyline_container[i], sizeof(storyline_container[i]), storyline_file);
        }
        printf(PARCHMENT);
        center_text("Press any key to continue", 17);
        printf(STARDUST);
        center_text(storyline_container[0], 10);
        getch();
        center_text(storyline_container[1], 11);
        getch();

        system(CLEAR_SCREEN);
        break;
    case 1:
        storyline_file = fopen("resources/storylines/gdial1.txt", "rt");
        for (int i = 0; i < 5; i++) {
            fgets(storyline_container[i], sizeof(storyline_container[i]), storyline_file);
        }
        printf(PARCHMENT);
        center_text("Press any key to continue", 21);
        printf(STARDUST);
        printf("\033[10;10H%s", storyline_container[0]);
        (void)getch();
        printf("\033[11;10H%s", storyline_container[1]);
        (void)getch();
        printf("\033[13;10H%s", storyline_container[2]);
        (void)getch();
        printf("\033[14;10H%s", storyline_container[3]);
        (void)getch();
        printf("\033[15;10H%s", storyline_container[4]);
        (void)getch();
        printf("\033[16;10H%s", storyline_container[5]);
        break;
    default:
        break;
    }
    
}

void quest_board(struct player *player1, int quest_rng1, int quest_rng2, int quest_rng3) {
    system(CLEAR_SCREEN);
    FILE *quest_file1, *quest_file2, *quest_file3;

    char quest_cont_1[10][60], quest_cont_2[10][60], quest_cont_3[10][60];
    int choice, quest_num_container[10];
    char temp_msg[32], quest_avlb[4][100];
   
    switch (quest_rng1) {
    case 1:
        quest_file1 = fopen("resources/quest_folder/quest1.txt", "rt");
        break;
    case 2:
        quest_file1 = fopen("resources/quest_folder/quest2.txt", "rt");
        break;
    case 3:
        quest_file1 = fopen("resources/quest_folder/quest3.txt", "rt");
        break;
    }
    switch (quest_rng2) {
    case 4:
        quest_file2 = fopen("resources/quest_folder/quest4.txt", "rt");
        break;
    case 5:
        quest_file2 = fopen("resources/quest_folder/quest5.txt", "rt");
        break;
    case 6:
        quest_file2 = fopen("resources/quest_folder/quest6.txt", "rt");
        break;
    }
    switch (quest_rng3) {
    case 7:
        quest_file3 = fopen("resources/quest_folder/quest7.txt", "rt");
        break;
    case 8:
        quest_file3 = fopen("resources/quest_folder/quest8.txt", "rt");
        break;
    case 9:
        quest_file3 = fopen("resources/quest_folder/quest9.txt", "rt");
        break;
    }

    for (int i = 0; i < 8; i++) {
        fgets(quest_cont_1[i], sizeof(quest_cont_1[i]), quest_file1);
        quest_cont_1[i][strcspn(quest_cont_1[i], "\n")] = '\0';
        fgets(quest_cont_2[i], sizeof(quest_cont_2[i]), quest_file2);
        quest_cont_2[i][strcspn(quest_cont_2[i], "\n")] = '\0';
        fgets(quest_cont_3[i], sizeof(quest_cont_3[i]), quest_file3);
        quest_cont_3[i][strcspn(quest_cont_3[i], "\n")] = '\0';
    }
    
    fclose(quest_file1);
    fclose(quest_file2);
    fclose(quest_file3);

    strcpy(quest_avlb[0], quest_cont_1[0]);
    strcpy(quest_avlb[1], quest_cont_2[0]);
    strcpy(quest_avlb[2], quest_cont_3[0]);
    strcpy(quest_avlb[3], "          Return          ");

    printf(CELESTIAL"\033[8;10H(Guildmaster Kimmentor): Here are the quests available today."PARCHMENT);
    drawMenu2d(highlight, quest_avlb, 4);
    while ((ch = getch()) != '\n') {
        system(CLEAR_SCREEN);
        printf(CELESTIAL"\033[8;10H(Guildmaster Kimmentor): Here are the quests available today."PARCHMENT);
        switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + 4) % 4;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 4;
                break;
            case ESC:
                endwin();
        }
        switch (highlight) {
        case 0:            
            center_text(quest_cont_1[1], 12);

            strcpy(temp_msg, "Exp: ");
            strcat(temp_msg, quest_cont_1[2]);
            center_text(temp_msg, 13);

            strcpy(temp_msg, "Gold: ");
            strcat(temp_msg, quest_cont_1[3]);
            center_text(temp_msg, 14);
            break;
        case 1:
            center_text(quest_cont_2[1], 12);

            strcpy(temp_msg, "Exp: ");
            strcat(temp_msg, quest_cont_2[2]);
            center_text(temp_msg, 13);

            strcpy(temp_msg, "Gold: ");
            strcat(temp_msg, quest_cont_2[3]);
            center_text(temp_msg, 14);
            break;
        case 2:
            center_text(quest_cont_3[1], 12);

            strcpy(temp_msg, "Exp: ");
            strcat(temp_msg, quest_cont_3[2]);
            center_text(temp_msg, 13);

            strcpy(temp_msg, "Gold: ");
            strcat(temp_msg, quest_cont_3[3]);
            center_text(temp_msg, 14);
            break;
        default:
            break;
        }
        drawMenu2d(highlight, quest_avlb, 4);
    }
    
    choice = highlight;
    if (highlight == 3) {
        return;
    }

    if (tier1_quest.activ == 1) {
        system(CLEAR_SCREEN);
        center_text("You cant have multiple quests at the same time!", 10);
        center_text("Press any key to continue", 17);
        getch();
        return;
    }
    
    system(CLEAR_SCREEN);
    printf(CELESTIAL"\033[8;10H(Guildmaster Kimmentor): Are you sure you can take this quest?"PARCHMENT);
    printf(CELESTIAL"\033[9;10H(Guildmaster Kimmentor): I ain't paying if you pass out."PARCHMENT);
    
    highlight = 0;
    drawMenu(highlight, yesOrNo, 2);
    while ((ch = getch()) != '\n') {
        switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + 2) % 2;

                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 2;

                break;
            case ESC:
                endwin();
        }
        drawMenu(highlight, yesOrNo, 2);
    }

        
    system(CLEAR_SCREEN);
    
    if (highlight == 0) {
        switch (choice) {
        case 0:            
            strcpy(tier1_quest.name, quest_cont_1[0]);
            strcpy(tier1_quest.dscrpt, quest_cont_1[1]);
            sscanf(quest_cont_1[2], "%d", &tier1_quest.exp);
            sscanf(quest_cont_1[3], "%d", &tier1_quest.gold);
            sscanf(quest_cont_1[4], "%d", &tier1_quest.req);
            sscanf(quest_cont_1[5], "%d", &tier1_quest.ID);
            break;
        case 1:
            strcpy(tier1_quest.name, quest_cont_2[0]);
            strcpy(tier1_quest.dscrpt, quest_cont_2[1]);
            sscanf(quest_cont_2[2], "%d", &tier1_quest.exp);
            sscanf(quest_cont_2[3], "%d", &tier1_quest.gold);
            sscanf(quest_cont_2[4], "%d", &tier1_quest.req);
            sscanf(quest_cont_2[5], "%d", &tier1_quest.ID);
            break;
        case 2:    
            strcpy(tier1_quest.name, quest_cont_3[0]);
            strcpy(tier1_quest.dscrpt, quest_cont_3[1]);
            sscanf(quest_cont_3[2], "%d", &tier1_quest.exp);
            sscanf(quest_cont_3[3], "%d", &tier1_quest.gold);
            sscanf(quest_cont_3[4], "%d", &tier1_quest.req);
            sscanf(quest_cont_3[5], "%d", &tier1_quest.ID);
            break;
        default:
            exit(1);
            break;
        }

        tier1_quest.activ = 1;
        system(CLEAR_SCREEN);
        center_text("Added to quest log", 10);
        center_text("Press any key to continue...", 17);
        getch();
        save_game(player1);
        return;
    }
}

void check_quest(struct player *player1) {
    system(CLEAR_SCREEN);
    int pass = 0;
    char tmp_msg[72];
    char tmp_num[8];

    if (tier1_quest.activ == 1) {
        if (tier1_quest.plantID == tier1_quest.ID) {
            tier1_quest.prog = player1->bag[tier1_quest.ID];
            pass = 1;
        }

        if (tier1_quest.killID == tier1_quest.ID) {
            tier1_quest.prog += 1;
            pass = 1;
        }
        
        if (pass == 1 && tier1_quest.prog >= tier1_quest.req) {
            if (tier1_quest.ID < 60000000) {
                if (player1->bag[tier1_quest.ID] >= tier1_quest.req) {
                    player1->bag[tier1_quest.ID] -= tier1_quest.req;
                }
            }
         
            printf(STARDUST);
            center_text("You have completed the quest: ", 10);
            center_text(tier1_quest.name, 11);

            strcpy(tmp_msg, "Received: ");
            sprintf(tmp_num, "%d", tier1_quest.exp);
            strcat(tmp_msg, tmp_num);
            strcat(tmp_msg, " Exp");
            center_text(tmp_msg, 13);
            memset(tmp_msg, 0, sizeof(tmp_msg));

            strcpy(tmp_msg, "Received: ");
            sprintf(tmp_num, "%d", tier1_quest.gold);
            strcat(tmp_msg, tmp_num);
            strcat(tmp_msg, " Gold");
            center_text(tmp_msg, 14);

            player1->exp += tier1_quest.exp;
            player1->gold += tier1_quest.gold;

            tier1_quest.activ = 0;

            center_text("Press any key to continue...", 17);
            system(CLEAR_SCREEN);
            getch();
            save_game(player1);
        }
    }
}

void rest_area(struct player *player1){
    system(CLEAR_SCREEN);
    printf(CELESTIAL"\033[10;10H(Guildmaster Kimmentor): Rest Here for 20 Gold? "PARCHMENT);

    highlight = 0;
    drawMenu(highlight, yesOrNo, 2);
        while ((ch = getch()) != '\n') {
        switch (ch) {
        case KEY_UP:
            highlight = (highlight - 1 + 2) % 2;

            break;
        case KEY_DOWN:
            highlight = (highlight + 1) % 2;

            break;
        case ESC:
            endwin();
        }
        drawMenu(highlight, yesOrNo, 2);
    }

    if (highlight == 1) {
        return;
    }

    if (highlight == 0 && player1->gold >= 20) {
        player1->hp += rand() % 50 + 1;
        player1->hp += 25;
        if (player1->hp > max_hp) {
            player1->hp = max_hp;
        }
        player1->gold -= 20;
        return;
    }

    if (player1->gold < 20) {
        system(CLEAR_SCREEN);
        printf(CELESTIAL"\033[10;10H(Guildmaster Kimmentor): Hey hey space ain't free."PARCHMENT);
        center_text("Press any key to continue", 17);
        getch();
    }
}

void expanse(int place, struct player *player1) {
    
    char *exit_avaloria[] = {
        "         Explore          ",
        "       Lorian Grove       ",
        "       Lorian Glade       ",
        "       To Avaloria        ",
        "        Inventory         "
    };
    char *exit_aetheris[] = {
        "         Explore          ",
        "       Serath Grove       ",
        "        Lushwoods         ",
        "       To Aetheris        ",
        "        Inventory         "
    };
    char *exit_thalnoria[] = {
        "         Explore          ",
        "       Lenoria Grove      ",
        "        Eldergrove        ",
        "       To Thalnoria       ",
        "        Inventory         "
    };
    char *exit_valerius[] = {
        "         Explore          ",
        "       Virian Ridge       ",
        "      Mountain Pass       ",
        "       To Valerius        ",
        "        Inventory         "
    };
    int temp_placeID = player1->placeID;
    char **expanse_menu = NULL;
    switch (place) {
    case 1:
        strcpy(player1->place, "Lorian Woods");
        expanse_menu = exit_avaloria;
        break;
    case 2:
        strcpy(player1->place, "Thorwyn Glade");
        expanse_menu = exit_aetheris;
        break;
    case 3:
        expanse_menu = exit_thalnoria;
        strcpy(player1->place, "Lenoria Glade");
        break;
    case 4:
        expanse_menu = exit_valerius;
        strcpy(player1->place, "Virian Hills");
        break;
    }

    while (1) {
        system(CLEAR_SCREEN);
        highlight = 0;
        center_text("Outside Kingdom", 8);
        center_text(player1->place, 9);
        player1->placeID = 0;
        drawMenu(highlight, expanse_menu, expanse_size);
        while ((ch = getch()) != '\n') {
        switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + expanse_size) % expanse_size;

                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % expanse_size;

                break;
            case ESC:
                endwin();
        }
        system(CLEAR_SCREEN);
        switch (highlight) {
        case 0:
            center_text("Explore outside the kingdom?", 13);
            center_text("Chance to find Dungeons: low", 14);
            break;
        case 1:
            center_text("Go deeper into the unknown?", 13);
            center_text("Chance to find Dungeons: Medium", 14);
            break;
    
        case 2:
            center_text("Go deeper into the unknown?", 13);
            center_text("Chance to find Dungeons: Medium", 14);
            break;

        case 3:
            center_text("Go to the Kingdom?", 13);
            break;

        case 4:
            center_text("Check inventory ?", 13);
        default:
            break;
        }
        center_text("Outside Kingdom", 8);
        center_text(player1->place, 9);
        drawMenu(highlight, expanse_menu, expanse_size);
        }

        system(CLEAR_SCREEN);
        switch (highlight) {
        case 0:
            low_encounter(player1, place);
            break;
    
        case 1:
            med_encounter(player1, place, 3);
            expanse1(place, player1);
            break;
        case 2:
            med_encounter(player1, place, 3);
            expanse2(place, player1);
            break;
        case 3:
            switch (place) {
                case 1:
                    player1->placeID = 50100000;
                    strcpy(player1->place, "Avaloria");
                    break;
                case 3:
                    player1->placeID = 50300000;
                    strcpy(player1->place, "Thalnoria");
                    break;
                
                case 4:
                    player1->placeID = 50400000;
                    strcpy(player1->place, "Valerius");
                    break;
                
                default:
                    player1->placeID = 50200000;
                    strcpy(player1->place, "Aetheris");
                    break;
                }
            spawn(player1, place);
            break;

        case 4:
            highlight = 0;
            check_inventory(player1, 0);
            break;
        
        default:
            break;
        }
    }

}

void guild_shop(struct player *player1) {
    char *flora_sell[] = {
        "         Veilrose         ",
        "       Yormen Flora       ",
        "        Trawnshroom       ",
        "        Lumen Flora       ",
        "          Orbroot         ",
        "         Dreamvine        ",
        "          Skyroot         ",
        "        Wyrmblossom       ",
        "          Return          "
    };
    while (1) {
        system(CLEAR_SCREEN);
        printf(CELESTIAL"\033[8;10H(Guildmaster Kimmentor): We buy herbs for a price"PARCHMENT);
        drawMenu(highlight, flora_sell, 9);
        while ((ch = getch()) != '\n') {
            switch (ch) {
                case KEY_UP:
                    highlight = (highlight - 1 + 9) % 9;

                    break;
                case KEY_DOWN:
                    highlight = (highlight + 1) % 9;

                    break;
                case ESC:
                    endwin();
            }
            drawMenu(highlight, flora_sell, 9);
        }

        system(CLEAR_SCREEN);
        if (highlight == 8) {
            break;
        } else if (player1->bag[highlight] == 0) {
            printf(CELESTIAL"\033[8;10H(Guildmaster Kimmentor): You can't sell what you don't have!"PARCHMENT);
            center_text("Press any key to continue", 17);
            getch();
        } else {
            switch (player1->bag[highlight]) {
            case 0:
            case 1:
            case 2:
            case 3:
                player1->gold += 5;
                center_text("Item sold for 5 gold", 10);
                break;
            case 4:
            case 5:
                player1->gold += 10;
                center_text("Item sold for 10 gold", 10);
                break;
            case 6:
                player1->gold += 5;
                center_text("Item sold for 5 gold", 10);
                break;
            case 7:
                player1->gold += 12;
                center_text("Item sold for 12 gold", 10);
                break;
            
            default:
                break;
            }
            player1->bag[highlight] -= 1;
            save_game(player1);
        }
    }
}

void expanse1(int place, struct player *player1) {
    char tmp_msg[32];
    int return_here = 1;
    char *exit_lorianwoods[] = {
        "         Explore          ",
        "       Virian Hills       ",
        "       Lorian Woods       ",
        "        Inventory         "
    };
    char *exit_serathgrove[] = {
        "         Explore          ",
        "       Lorian Woods       ",
        "       Serath Grove       ",
        "        Inventory         "
    };
    char *exit_lenoriaglade[] = {
        "         Explore          ",
        "       Virian Hills       ",
        "       Lenoria Glade      ",
        "        Inventory         "
    };
    char *exit_virianridge[] = {
        "         Explore          ",
        "       Lorian Woods       ",
        "       Virian Ridge       ",
        "        Inventory         "
    };
    char **expanse_menu = NULL;

    switch (place) {
        case 1:
            expanse_menu = exit_lorianwoods;
            strcpy(player1->place, "Lorian Grove");
            break;
        case 2:
            expanse_menu = exit_serathgrove;
            strcpy(player1->place, "Serath Grove");
            break;
        case 3:
            expanse_menu = exit_lenoriaglade;
            strcpy(player1->place, "Lenoria Grove");
            break;
        case 4:
            expanse_menu = exit_virianridge;
            strcpy(player1->place, "Mountain Pass");
            break;
        }
    while (1) {
        system(CLEAR_SCREEN);
        highlight = 0;
        center_text(player1->place, 9);
        player1->placeID = 0;
        drawMenu(highlight, expanse_menu, 4);
        while ((ch = getch()) != '\n') {
            switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + 4) % 4;

                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 4;

                break;
            case ESC:
                endwin();
            }
        system(CLEAR_SCREEN);
        switch (highlight) {
            case 0:
                center_text("Chance to encounter monsters: medium", 14);
                break;
            case 1:
                center_text("Go deeper into the unknown?", 13);
                center_text("Chance to encounter monsters: medium", 14);
                break;
        
            case 2:
                center_text("Go back?", 13);
                center_text("Chance to encounter monsters: low", 14);
                break;

            case 3:
                center_text("Check inventory ?", 13);
                break;
            }
        center_text(player1->place, 9);
        drawMenu(highlight, expanse_menu, 4);
        }

        system(CLEAR_SCREEN);
        switch (highlight) {
            case 0:
                med_encounter(player1, place, return_here);
                break;
            case 1:
                med_encounter(player1, place, 3);
                switch (place) {
                case 1:
                case 3:
                    expanse(4, player1);
                    break;
                case 2:
                case 4:
                    expanse(1, player1);
                    break;
                }
                break;
            case 2:
                med_encounter(player1, place, 3);
                expanse(place, player1);
                break;
            case 3:
                highlight = 0;
                check_inventory(player1, 0);
                break;
            }
    }

}

void expanse2(int place, struct player *player1) {
    char tmp_msg[32];
    int return_here = 2;
    char *exit_lorianglade[] = {
        "         Explore          ",
        "      Thorwyn Glade       ",
        "       Lorian Glade       ",
        "        Inventory         "
    };
    char *exit_lushwoods[] = {
        "         Explore          ",
        "      Lenoria Glade       ",
        "        Lushwoods         ",
        "        Inventory         "
    };
    char *exit_eldergrove[] = {
        "         Explore          ",
        "       Thorwyn Glade       ",
        "        Eldergrove        ",
        "        Inventory         "
    };
    char *exit_mountainpass[] = {
        "         Explore          ",
        "       Lenoria Glade      ",
        "       Mountain Pass      ",
        "        Inventory         "
    };
    char **expanse_menu = NULL;

    switch (place) {
        case 1:
            expanse_menu = exit_lorianglade;
            strcpy(player1->place, "Lorian Glade");
            break;
        case 2:
            expanse_menu = exit_lushwoods;
            strcpy(player1->place, "Lushwoods");
            break;
        case 3:
            expanse_menu = exit_eldergrove;
            strcpy(player1->place, "Eldergrove");
            break;
        case 4:
            expanse_menu = exit_mountainpass;
            strcpy(player1->place, "Mountain Pass");
            break;
        }
        
    while (1) {
        system(CLEAR_SCREEN);
        highlight = 0;
        center_text(player1->place, 9);
        player1->placeID = 0;
        drawMenu(highlight, expanse_menu, 4);
        while ((ch = getch()) != '\n') {
            switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + 4) % 4;

                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 4;

                break;
            case ESC:
                endwin();
            }
            system(CLEAR_SCREEN);
            switch (highlight) {
                case 0:
                    center_text("Chance to encounter monsters: medium", 14);
                    break;
                case 1:
                    center_text("Go deeper into the unknown?", 13);
                    center_text("Chance to encounter monsters: medium", 14);
                    break;
            
                case 2:
                    center_text("Go back?", 13);
                    center_text("Chance to encounter monsters: low", 14);
                    break;

                case 3:
                    center_text("Check inventory ?", 13);
                    break;
                }
            center_text(player1->place, 9);
            drawMenu(highlight, expanse_menu, 4);
        }

        system(CLEAR_SCREEN);
        switch (highlight) {
            case 0:
                med_encounter(player1, place, return_here);
                break;
            case 1:
                med_encounter(player1, place, 3);
                switch (place) {
                case 1:
                case 3:
                    expanse(2, player1);
                    break;
                case 2:
                case 4:
                    expanse(3, player1);
                    break;
                }
            case 2:
                med_encounter(player1, place, 3);
                expanse(place, player1);
                break;
            case 3:
                highlight = 0;
                check_inventory(player1, 0);
                break;
            }
    }

}

void med_encounter(struct player *player1, int place, int return_here) {
    system(CLEAR_SCREEN);

    srand((unsigned int)time(NULL));
    int encounter_rate = rand() % 25 + 1;
    int encounter_type = rand() % 3 + 1;
    char cont_array[15][150];
    char tmp_msg[100];
    char tmp_msg2[100];
    int item_no;
    FILE *container = NULL;
    int temp;

    switch (encounter_type) {
    case 1:
        switch (encounter_rate) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
                dungeon2(player1, place, return_here);
                break;
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
                dungeon4(player1, place, return_here);
                break;
            case 22:
            case 23:
                dungeon3(player1, place, return_here);
                break;
            case 24:
                dungeon1(player1, place, return_here);
                break;
            case 25:
                 dungeon5(player1, place, return_here);
                break;
            }

            return;
        break;
    case 2:
        switch (encounter_rate) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                container = fopen("resources/bestiary/mystra.txt", "rt");
                break;
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                container = fopen("resources/bestiary/qinoyin.txt", "rt");
                break;
            case 11:
            case 12:
            case 13:
            case 14:
                container = fopen("resources/bestiary/lorian_wolf.txt", "rt");
                break;
            case 15:
            case 16:
            case 17:
            case 18:
                container = fopen("resources/bestiary/bogwood.txt", "rt");
                break;
            case 19:
            case 20:
                container = fopen("resources/bestiary/mugger.txt", "rt");
                break;
            case 21:
            case 22:
                container = fopen("resources/bestiary/igorhorn.txt", "rt");
                break;
            case 23:
            case 24:
                container = fopen("resources/bestiary/bandit.txt", "rt");
                break;
            case 25:
                container = fopen("resources/bestiary/rrrxlmbs.txt", "rt");
                break;
            }
        break;
    case 3:
        switch (encounter_rate) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                container = fopen("resources/flora/veilrose.txt", "rt");
                item_no = 0;
                break;
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                container = fopen("resources/flora/yormen_flora.txt", "rt");
                item_no = 1;
                break;
            case 11:
            case 12:
            case 13:
                container = fopen("resources/flora/trawnshroom.txt", "rt");
                item_no = 2;
                break;
            case 14:
            case 15:
            case 16:
                container = fopen("resources/flora/lumen_flora.txt", "rt");
                item_no = 3;
                break;
            case 17:
            case 18:
            case 19:
                container = fopen("resources/flora/orbroot.txt", "rt");
                item_no = 4;
                break;
            case 20:
            case 21:
            case 22:
                container = fopen("resources/flora/dreamvine.txt", "rt");
                item_no = 5;
                break;
            case 23:
            case 24:
                container = fopen("resources/flora/skyroot.txt", "rt");
                item_no = 6;
                break;
            case 25:
                container = fopen("resources/flora/wyrmblossom.txt", "rt");
                item_no = 7;
                break;
            default:
                exit(0);
                break;
            }
        break;
    }

    for (int i = 0; i < 13; i++) {
        fgets(cont_array[i], sizeof(cont_array[i]), container);
    }

    switch (encounter_type) {
    case 1:
        break;
    case 2:
        cont_array[0][strcspn(cont_array[0], "\n")] = '\0';

        strcpy(tier1_beast.name, cont_array[0]);
        sscanf(cont_array[4], "%d", &temp);
        tier1_beast.hit_die = temp;
        sscanf(cont_array[5], "%d", &temp);
        tier1_beast.hp = temp;
        sscanf(cont_array[6], "%d", &temp);
        tier1_beast.dmg = temp;
        sscanf(cont_array[7], "%d", &temp);
        tier1_beast.skill_dmg = temp;
        sscanf(cont_array[8], "%d", &temp);
        tier1_beast.trig_die = temp;
        sscanf(cont_array[9], "%d", &temp);
        tier1_beast.def = temp;
        sscanf(cont_array[10], "%d", &temp);
        tier1_beast.wpn_type = temp;
        sscanf(cont_array[11], "%d", &temp);
        tier1_beast.agl = temp;
        sscanf(cont_array[12], "%d", &temp);
        tier1_beast.exp_drop = temp;
        sscanf(cont_array[13], "%d", &temp);
        tier1_beast.gold_drop = temp;
        sscanf(cont_array[14], "%d", &temp);
        tier1_beast.beast_ID = temp;

        battleAI(player1, place);
        break;
    
    case 3:
        system(CLEAR_SCREEN);

        strcpy(tmp_msg, "You have found a ");
        strcpy(tmp_msg2,"Would you like to take the ");
        strcat(tmp_msg, cont_array[0]);
        strcat(tmp_msg2, cont_array[0]);
        center_text(tmp_msg, 8);
        center_text(tmp_msg2, 9);

        switch (item_no) {
            case 0:
            case 1:
            case 2:
                printf(STARLIGHT);
                break;
            case 3:
            case 4:
            case 5:
                printf(NEBULA);
                break;
            case 7:
                printf(DEEP_CRIMSON);
                break;
        }

        printf("\033[12;51H  ▒▒▒▒▒▒▒            ██     ");
        printf("\033[13;51H▓▓▓▓██▒▒   ▒▒▒▒▒▒▒  ██      ");
        printf("\033[14;51H  ▓▓▓▓██  ▓▓▓██▒▒ ██        ");   
        printf("\033[15;51H        ██ ▓▓▓▓▓██          ");
        printf("\033[16;51H █████  ██  ██    ▒▒▒▒▒▒██  ");
        printf("\033[17;51H   ███████  ██  ▒▒▒▒██▓▓▓ ██");        
        printf("\033[18;51H        ████    ██▓▓▓▓▓     ");
        printf("\033[19;51H          ██  ██            ");
        printf("\033[20;51H            ██              ");
        printf("\033[21;51H            ██              ");
        printf(PARCHMENT);
        SLEEP(500);

        highlight = 0;
        drawMenu(highlight, yesOrNo, 2);
        printf(PARCHMENT);
        while ((ch = getch()) != '\n') {
        switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + 2) % 2;

                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 2;

                break;
            case ESC:
                endwin();
        }
        drawMenu(highlight, yesOrNo, 2);
        printf(PARCHMENT);
        }

        system(CLEAR_SCREEN);
        if (highlight == 1) {
            center_text("Did not find anything else....", 10);
            SLEEP(1000);
            break;
        }

        player1->bag[item_no]+=1;
        player1->exp += rand() % 12 + 1;
        tier1_quest.plantID = item_no;

        strcpy(tmp_msg, "You took the ");
        strcat(tmp_msg, cont_array[0]);
        center_text(tmp_msg, 10);
        center_text("Press any key to continue...", 17);
        getch();
        check_quest(player1);
        tier1_quest.plantID = 0;
        break;
    }
    SLEEP(1000);
}

void dungeon1(struct player *player1, int place, int return_here) {
    char *sandiwchnichris[] = {
        "      Egg Sandwich        ",
        "      Tuna Sandwich       ",
        "          Leave           "
    };
    
    printf(CELESTIAL);
    printf("\033[8;10H(Chris): Adventurer! Would you like to buy some of my sandwiches?");
    printf("\033[9;10H(Chris): 20 gold for an egg sandwich");
    printf("\033[10;10H(Chris): 29 gold for a tuna sandwich");
    printf(PARCHMENT);
    drawMenu(highlight, sandiwchnichris, 3);
    while ((ch = getch()) != '\n') {
        system(CLEAR_SCREEN);
        printf("\033[8;10H(Chris): Adventurer! Would you like to buy some of my sandwiches?");
        printf("\033[9;10H(Chris): 20 gold for an egg sandwich");
        printf("\033[10;10H(Chris): 25 gold for a tuna sandwich");
        switch (ch) {
        case KEY_UP:
            highlight = (highlight - 1 + 3) % 3;

            break;
        case KEY_DOWN:
            highlight = (highlight + 1) % 3;

            break;
        case ESC:
            endwin();
        }
        switch (highlight) {
            case 0:
                center_text("Heals 30 health points instantly", 12);
                break;
            case 1:
                center_text("Heals 40 health points instantly", 12);
                break;
            default:
                break;
            }
        drawMenu(highlight, sandiwchnichris, 3);
    }

    system(CLEAR_SCREEN);
    switch (highlight) {
    case 0:
    case 1:
        if (player1->gold < 20) {
            printf(CELESTIAL);
            printf("\033[8;10H(Chris): You dont have enough money adventurer!");
            printf("\033[9;10H(Chris): Its alright, i'll give you an egg sandwich for free");
            highlight = 0;
            printf(PARCHMENT);
            center_text("Press any key to continue", 17);
            getch();   
        }
        switch (highlight) {
        case 0:
            player1->hp += 30;
            player1->gold -= 20;
            break;
        
        case 1:
            player1->hp += 40;
            player1->gold -= 29;
            break;
        }
        if (player1->hp > max_hp) {
            player1->hp = max_hp;
        }
        
        printf(CELESTIAL);
        printf("\033[10;10H(Chris): Safe travels Adventurer!");
        printf(PARCHMENT);
        center_text("Press any key to continue", 17);
        getch();
        break;
    default:
        printf(CELESTIAL);
        printf("\033[8;10H(Chris): Safe travels Adventurer!");
        printf(PARCHMENT);
        center_text("Press any key to continue", 17);
        getch();
        break;
    }
}

void dungeon2(struct player *player1, int place, int return_here) {
    char *dungeon2_menu[] = {
        "     Fight the beasts     ",
        "          Leave           "
    };
    int encounter_type = rand() % 3 + 1;
    int encounter = 3, temp;
    char cont_array[15][150];
    FILE *container = NULL;
    printf(CELESTIAL);
    printf("\033[9;10H(Hunter Yormundir): Adventurer! Could you please help me?");
    printf("\033[10;10H(Hunter Yormundir): I was running away from some beasts inside the cave when i noticed my bag fell");
    printf("\033[11;10H(Hunter Yormundir): Could you help me get it? I'm not strong enough to fight them");
    printf("\033[12;10H(Hunter Yormundir): I'll share some of the loot i got!");
    printf(PARCHMENT);
    drawMenu(highlight, dungeon2_menu, 2);
    while ((ch = getch()) != '\n') {
        switch (ch) {
        case KEY_UP:
            highlight = (highlight - 1 + 2) % 2;

            break;
        case KEY_DOWN:
            highlight = (highlight + 1) % 2;

            break;
        case ESC:
            endwin();
        }
        drawMenu(highlight, dungeon2_menu, 2);
    }

    system(CLEAR_SCREEN);
    if (highlight == 1) {
        if (return_here == 1) {
        expanse1(place, player1);    
        } else {
        expanse2(place, player1);    
        }
    }

    switch (encounter_type) {
    case 1:
        container = fopen("resources/bestiary/lorian_wolf.txt", "rt");    
        break;
    case 2:
        container = fopen("resources/bestiary/bogwood.txt", "rt");    
        break;
    case 3:
        container = fopen("resources/bestiary/igorhorn.txt", "rt");    
        break;
    }

    for (int i = 0; i < 13; i++) {
        fgets(cont_array[i], sizeof(cont_array[i]), container);
    }

    fclose(container);

    cont_array[0][strcspn(cont_array[0], "\n")] = '\0';

    strcpy(tier1_beast.name, cont_array[0]);
    sscanf(cont_array[4], "%d", &temp);
    tier1_beast.hit_die = temp;
    sscanf(cont_array[5], "%d", &temp);
    tier1_beast.hp = temp;
    sscanf(cont_array[6], "%d", &temp);
    tier1_beast.dmg = temp;
    sscanf(cont_array[7], "%d", &temp);
    tier1_beast.skill_dmg = temp;
    sscanf(cont_array[8], "%d", &temp);
    tier1_beast.trig_die = temp;
    sscanf(cont_array[9], "%d", &temp);
    tier1_beast.def = temp;
    sscanf(cont_array[10], "%d", &temp);
    tier1_beast.wpn_type = temp;
    sscanf(cont_array[11], "%d", &temp);
    tier1_beast.agl = temp;
    sscanf(cont_array[12], "%d", &temp);
    tier1_beast.exp_drop = temp;
    sscanf(cont_array[13], "%d", &temp);
    tier1_beast.gold_drop = temp;
    sscanf(cont_array[14], "%d", &temp);
    tier1_beast.beast_ID = temp;

    while (encounter != 0) {
        battleAI(player1, place);
        tier1_beast.hp = 100;
        encounter--;
    }

    system(CLEAR_SCREEN);
    printf(CELESTIAL);
    printf("\033[9;10H(Hunter Yormundir): Thank you adventurer! Here, take these");
    center_text("Received 50 Gold", 11);
    center_text("Received 2 Lesser Healing Potion", 12);
    center_text("Received 2 Lumen Flora", 13);
    center_text("Gained 70 Exp", 14);
    player1->gold += 50;
    player1->bag[10] += 2;
    player1->bag[3] += 2;
    player1->exp += 70;
    printf(PARCHMENT);
    center_text("Press any key to continue...", 17);
    getch();

}

void dungeon3(struct player *player1, int place, int return_here) {
    char cont_array[15][150];
    int temp;
    FILE *container = NULL;
    printf(FORGE_FIRE);
    printf("\033[9;10H(RRXLM II Bandit): Commander Richard Rex Rene Lacdao Marbella II owns these lands!");
    printf(PARCHMENT);
    center_text("Press any key to continue...", 17);
    getch();
    printf(FORGE_FIRE);
    printf("\033[10;10H(RRXLM II Bandit): You, peasant, dare to walk here? Unacceptable!");
    printf(PARCHMENT);
    center_text("Press any key to continue...", 17);
    getch();
    printf(FORGE_FIRE);
    printf("\033[11;10H(RRXLM II Bandit): Prepare to die!");
    getch();

    container = fopen("resources/bestiary/rrrxlmbs.txt", "rt");

    for (int i = 0; i < 13; i++) {
        fgets(cont_array[i], sizeof(cont_array[i]), container);
    }

    fclose(container);

    cont_array[0][strcspn(cont_array[0], "\n")] = '\0';

    strcpy(tier1_beast.name, cont_array[0]);
    sscanf(cont_array[4], "%d", &temp);
    tier1_beast.hit_die = temp;
    sscanf(cont_array[5], "%d", &temp);
    tier1_beast.hp = temp;
    sscanf(cont_array[6], "%d", &temp);
    tier1_beast.dmg = temp;
    sscanf(cont_array[7], "%d", &temp);
    tier1_beast.skill_dmg = temp;
    sscanf(cont_array[8], "%d", &temp);
    tier1_beast.trig_die = temp;
    sscanf(cont_array[9], "%d", &temp);
    tier1_beast.def = temp;
    sscanf(cont_array[10], "%d", &temp);
    tier1_beast.wpn_type = temp;
    sscanf(cont_array[11], "%d", &temp);
    tier1_beast.agl = temp;
    sscanf(cont_array[12], "%d", &temp);
    tier1_beast.exp_drop = temp;
    sscanf(cont_array[13], "%d", &temp);
    tier1_beast.gold_drop = temp;
    sscanf(cont_array[14], "%d", &temp);
    tier1_beast.beast_ID = temp;

    battleAI(player1, place);
}

void dungeon4(struct player *player1, int place, int return_here) {
    char *dungeon2_menu[] = {
        "       Give 20 Gold       ",
        "          Leave           "
    };

    int encounter = 3, temp;
    char cont_array[15][150];
    FILE *container = fopen("resources/bestiary/mugger.txt", "rt");
    printf(CELESTIAL);
    printf("\033[9;10H(Mugger): Adventurer! Oi!");
    printf("\033[10;10H(Mugger): I'll let you pass without trouble for 20 gold");
    printf(PARCHMENT);
    center_text("Press any key to continue...", 17);
    getch();
    drawMenu(highlight, dungeon2_menu, 2);
    while ((ch = getch()) != '\n') {
        switch (ch) {
        case KEY_UP:
            highlight = (highlight - 1 + 2) % 2;

            break;
        case KEY_DOWN:
            highlight = (highlight + 1) % 2;

            break;
        case ESC:
            endwin();
        }
        drawMenu(highlight, dungeon2_menu, 2);
    }

    system(CLEAR_SCREEN);

    if (player1->gold < 20) {
        highlight = 1;
    }

    if (highlight == 0) {
        player1->gold -= 20;
        switch (return_here) {
        case 1:
            expanse1(place, player1);
            break;
        case 2:
            expanse2(place, player1);
            break;
        default:
            return;
            break;
        }
    }

    for (int i = 0; i < 13; i++) {
        fgets(cont_array[i], sizeof(cont_array[i]), container);
    }

    fclose(container);

    cont_array[0][strcspn(cont_array[0], "\n")] = '\0';

    strcpy(tier1_beast.name, cont_array[0]);
    sscanf(cont_array[4], "%d", &temp);
    tier1_beast.hit_die = temp;
    sscanf(cont_array[5], "%d", &temp);
    tier1_beast.hp = temp;
    sscanf(cont_array[6], "%d", &temp);
    tier1_beast.dmg = temp;
    sscanf(cont_array[7], "%d", &temp);
    tier1_beast.skill_dmg = temp;
    sscanf(cont_array[8], "%d", &temp);
    tier1_beast.trig_die = temp;
    sscanf(cont_array[9], "%d", &temp);
    tier1_beast.def = temp;
    sscanf(cont_array[10], "%d", &temp);
    tier1_beast.wpn_type = temp;
    sscanf(cont_array[11], "%d", &temp);
    tier1_beast.agl = temp;
    sscanf(cont_array[12], "%d", &temp);
    tier1_beast.exp_drop = temp;
    sscanf(cont_array[13], "%d", &temp);
    tier1_beast.gold_drop = temp;
    sscanf(cont_array[14], "%d", &temp);
    tier1_beast.beast_ID = temp;

    battleAI(player1, place);

    switch (return_here) {
    case 1:
        expanse1(place, player1);
        break;
    case 2:
        expanse2(place, player1);
        break;
    default:
        break;
    }
}

void dungeon5(struct player *player1, int place, int return_here) {
    char cont_array[15][150];
    int temp, encounter = 3;
    FILE *container = NULL;
    printf(PARCHMENT);
    center_text("Press any key to continue...", 17);
    printf(FORGE_FIRE);
    printf("\033[9;10H(Covenant): Which kingdom sent you?!");
    getch();
    printf("\033[10;10H(Covenant): No matter, The Rogue's Covenant won't let you go back");
    getch();
    printf("\033[11;10H(Covenant): Any last words?");
    getch();

    container = fopen("resources/bestiary/rrrxlmbs.txt", "rt");

    for (int i = 0; i < 13; i++) {
        fgets(cont_array[i], sizeof(cont_array[i]), container);
    }

    fclose(container);

    cont_array[0][strcspn(cont_array[0], "\n")] = '\0';

    strcpy(tier1_beast.name, cont_array[0]);
    sscanf(cont_array[4], "%d", &temp);
    tier1_beast.hit_die = temp;
    sscanf(cont_array[5], "%d", &temp);
    tier1_beast.hp = temp;
    sscanf(cont_array[6], "%d", &temp);
    tier1_beast.dmg = temp;
    sscanf(cont_array[7], "%d", &temp);
    tier1_beast.skill_dmg = temp;
    sscanf(cont_array[8], "%d", &temp);
    tier1_beast.trig_die = temp;
    sscanf(cont_array[9], "%d", &temp);
    tier1_beast.def = temp;
    sscanf(cont_array[10], "%d", &temp);
    tier1_beast.wpn_type = temp;
    sscanf(cont_array[11], "%d", &temp);
    tier1_beast.agl = temp;
    sscanf(cont_array[12], "%d", &temp);
    tier1_beast.exp_drop = temp;
    sscanf(cont_array[13], "%d", &temp);
    tier1_beast.gold_drop = temp;
    sscanf(cont_array[14], "%d", &temp);
    tier1_beast.beast_ID = temp;

    while (encounter != 0) {
        battleAI(player1, place);
        tier1_beast.hp = 100;
        encounter--;
    }
    system(CLEAR_SCREEN);
    printf(FORGE_FIRE);
    printf("\033[9;10H(Covenant): You.. Will.. Never.. Find us....");
    printf(PARCHMENT);
    center_text("Press any key to continue...", 17);
    getch();
}