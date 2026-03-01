#include "Windows.h"
#include <ncurses.h>
#include <string>

/*
 * Simulation of Windows.h library for usage on linux using ncurses (not seriously lol)
 */

HANDLE GetStdHandle(FILE*) {
    WINDOW* win = initscr();
    if (can_change_color() == false) {
        endwin();
        return nullptr;
    }
    start_color();
    return win;
}
void SetConsoleCursorPosition(const HANDLE out, const COORD cord) {
    wmove(out, cord.X, cord.Y);
}
void SetConsoleTextAttribute(const HANDLE out, COLOR color) {
    init_pair(1, static_cast<short>(color.foreground), static_cast<short>(color.background));
    wattron(out, COLOR_PAIR(1));
}

void Print(const char* s) {
    printw(s);
}

void Exit() {
    endwin();
}

int _getch() {
    return getch();
}
