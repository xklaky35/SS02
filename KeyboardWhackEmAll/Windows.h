#ifndef WINDOWS_H
#define WINDOWS_H
#include <ncurses.h>

#define STD_OUTPUT_HANDLE stdout
#define COLOR_NUM 8
#define COLOR_BLACK   0
#define COLOR_RED     1
#define COLOR_GREEN   2
#define COLOR_YELLOW  3
#define COLOR_BLUE    4
#define COLOR_MAGENTA 5
#define COLOR_CYAN    6
#define COLOR_WHITE   7

typedef WINDOW* HANDLE;

struct COLOR {
    int foreground;
    int background;
};

struct COORD {
    short X;
    short Y;
};


HANDLE GetStdHandle(FILE*);
void SetConsoleCursorPosition(HANDLE, COORD);
void SetConsoleTextAttribute(HANDLE, COLOR);
void Exit();
void Print(const char*);
int _getch();


#endif
