#include <iostream>
#include <random>

#include "Windows.h"


void ShowWelcome();
void PresentAlphabet();
int GetRandomNumber(int,int);
void move_cursor_to(int column, int row);
void set_text_colour(int foreground, int background);
int get_random_color();

void PlayKeyboardWhackEmAll() {
    set_text_colour(COLOR_BLACK,COLOR_WHITE);
    Print("+-------------------------------------------------+\n");
    Print("|              Keyboard Whack-Em All              |\n");
    Print("+-------------------------------------------------+\n");
    Print("Press the missing letter's key! (Press '~' to quit)\n");

    do {
        set_text_colour(get_random_color(), COLOR_BLACK);
        Print("asdfasdf\n");
    }while (_getch() != 'q');

    Exit();
}

int get_random_color() {
    int rand;
    do {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution distribution(0, COLOR_NUM-1);
        rand = distribution(gen);
    } while (rand == COLOR_BLACK);


    return rand;
}

void set_text_colour(int foreground, int background)
{
    HANDLE console_window = GetStdHandle(STD_OUTPUT_HANDLE);
    COLOR colour = COLOR(foreground,background);
    SetConsoleTextAttribute(console_window, colour);
}

void move_cursor_to(int column, int row)
{
    HANDLE console_window = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position;
    position.X = column;
    position.Y = row;
    SetConsoleCursorPosition(console_window, position);
}
