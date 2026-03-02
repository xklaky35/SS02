#include <iostream>
#include <random>
#include <ncurses.h>

#define COLOR_NUM 8
#define COLOR_BLACK   0
#define COLOR_RED     1
#define COLOR_GREEN   2
#define COLOR_YELLOW  3
#define COLOR_BLUE    4
#define COLOR_MAGENTA 5
#define COLOR_CYAN    6
#define COLOR_WHITE   7
#define ALPHABET "abcdefghijklmnopqrstuvwxyz";

struct Stats {
    int correct_count;
    int incorrect_count;
};


int get_random_color();
int get_random_number(int, int);
void init_colors();
void print_welcome();
char alter_alphabet(std::string& string);
void print_with_color(const char*, int);
void print_stats(const Stats& stats);




void PlayKeyboardWhackEmAll() {

    bool is_running = true;
    auto player_stats = Stats(0,0);

    // sets up ncurses screen to write to it (stdout)
    WINDOW* win= initscr();
    noecho(); // hides the player input character
    scrollok(win, true);

    // checks if the terminal is capable of changing color
    if (can_change_color() == false) {
        endwin();
        std::cout << "You terminal does not support colors!" << std::endl;
        return;
    }
    start_color(); // inits ncurses usage of colors

    // setup colors used in the game
    init_colors();
    print_welcome();

    // main game loop
    while (is_running) {

        // first change a random letter in the alphabet
        std::string alphabet = ALPHABET;
        const char searched_char = alter_alphabet(alphabet);

        // present the alphabet to the player with a random color
        const int rand = get_random_color();
        print_with_color(alphabet.append("\n").c_str(), rand);

        char input = getch();

        // takes action depending on user input
        if (input == '~') {
            is_running = false;
            print_stats(player_stats);
        }
        else if (input == searched_char) {
            player_stats.correct_count++;
        }
        else {
            player_stats.incorrect_count++;
        }
    }

    printw("Press any key to exit...");
    getch();
    endwin();
}

int get_random_color() {
    int rand;
    do {
        rand = get_random_number(0, COLOR_NUM-1); // 8 colors index starting at 0
    } while (rand == COLOR_BLACK); // avoids black text on black background
    return rand;
}

int get_random_number(const int min, const int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution distribution(min, max);
    return distribution(gen);
}

void print_stats(const Stats& stats) {
    attron(COLOR_PAIR(11));
    printw("\nCorrect count: %d\n", stats.correct_count);
    printw("Incorrect count: %d\n", stats.incorrect_count);
    attroff(COLOR_PAIR(11));
    refresh();
}


void init_colors() {
    // colors for game itself
    // init_pair(<customm identifier>, <foreground>, <background>)
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_CYAN, COLOR_BLACK);

    // misc corlors colors for
    init_pair(11, COLOR_BLACK, COLOR_WHITE); // color for heading
}

void print_welcome() {
    print_with_color("+-------------------------------------------------+\n", 11);
    print_with_color("|              Keyboard Whack-Em All              |\n", 11);
    print_with_color("+-------------------------------------------------+\n",11);
    print_with_color("Press the missing letter's key! (Press '~' to quit)\n\n", 11);
    refresh();
}

void print_with_color(const char* msg, const int color_pair_num) {
    attron(COLOR_PAIR(color_pair_num));
    printw("%s", msg);
    attroff(COLOR_PAIR(color_pair_num));
}

// swaps out a charater of the alphabet with '_'
char alter_alphabet(std::string &string) {
    if (string.size() < 26) {
        print_with_color("Error reading alphabet", COLOR_PAIR(1));
        exit(1);
    }
    const int rand = get_random_number(0,25);
    const char char_to_replace = string[rand];
    string[rand] = '_';
    return char_to_replace;
}
