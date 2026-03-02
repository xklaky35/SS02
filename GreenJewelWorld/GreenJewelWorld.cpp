#include <cstdlib>
#include <ncurses.h>
#include <random>

#define JEWEL_COUNT 4
#define FIELD_COL_NUM 50
#define FIELD_ROW_NUM 10

#define MOVE_UP 'w'
#define MOVE_DOWN 's'
#define MOVE_LEFT 'a'
#define MOVE_RIGHT 'd'


struct Vector2D {
    int x;
    int y;
};

struct PlayerData {
    int turns;
    int currentJewels;
    Vector2D position;
};



void SetupTerminal();
void SetupField(char**&);
void SetupColors();
bool HandleMoveCommand(PlayerData&, char);
bool Move(Vector2D& pos);
void PrintField(char**& field);
void PrintLoosingScreen(const PlayerData&);
void PrintWinningScreen(const PlayerData&);
void DeleteField(char**&);
int GenerateRandomNumber(int,int);
void PlaceJewels(char**&);
void CheckForJewels(char**&,PlayerData&);

void PlayGreenJewelWorld() {
    SetupTerminal();

    // setup game
    bool isRunning = true;
    auto playerData = PlayerData(
            0, // turns
            0, // current jewels
            Vector2D(0,0) // position
        );
    char** playerMap = nullptr;

    SetupField(playerMap);
    PrintField(playerMap);

    // reset cursor to start pos
    move(playerData.position.x, playerData.position.y);

    // main game loop
    while (isRunning) {

        // wait for input
        const char input = getch();
        if (input == 'q') {
            PrintLoosingScreen(playerData);
            break;
        }
        if (input != MOVE_UP && input != MOVE_DOWN && input != MOVE_LEFT && input != MOVE_RIGHT) {
            continue;
        }
        if (!HandleMoveCommand(playerData, input)) {
            continue;
        }

        CheckForJewels(playerMap, playerData);
        playerData.turns++;

        if (playerData.currentJewels == JEWEL_COUNT) {
            PrintWinningScreen(playerData);
            break;
        }
    }

    DeleteField(playerMap);
    printw("Press any key to exit...");
    getch();
    endwin();
}


// randomly place jewels on the map
void PlaceJewels(char**& map) {
    for (int i = 0; i < JEWEL_COUNT; i++) {
        auto randPos = Vector2D(GenerateRandomNumber(0, FIELD_COL_NUM-1), GenerateRandomNumber(0, FIELD_ROW_NUM-1));
        map[randPos.y][randPos.x] = static_cast<char>(i+65);
    }
}

// check if there is a jewel on the players position
void CheckForJewels(char**& map, PlayerData &player_data) {
    if (map[player_data.position.y][player_data.position.x] != '-') {
        map[player_data.position.y][player_data.position.x] = '-';
        refresh();
        player_data.currentJewels++;
    }
    PrintField(map);
}

// initialize ncurses and colors on the terminal
void SetupTerminal() {
    //setup terminal
    WINDOW* win = initscr();
    scrollok(win, true);
    noecho();
    if (can_change_color() == false) {
        printf("Terminal does not support colors!");
        endwin();
        exit(1);
    }
    start_color();
    SetupColors();
}

// allocation of field
void SetupField(char**& field) {
    field = new char*[FIELD_COL_NUM];
    for (int i = 0; i < FIELD_ROW_NUM; i++) {
        field[i] = new char[FIELD_COL_NUM];
        for (int j = 0; j < FIELD_COL_NUM; j++) {
            field[i][j] = '-';
        }
    }

    PlaceJewels(field);
}

void PrintField(char**& field) {
    auto oldPos = Vector2D(getcurx(stdscr), getcury(stdscr));

    move(0,0);
    attron(COLOR_PAIR(1));
    for (int i = 0; i < FIELD_ROW_NUM; i++) {
        for (int j = 0; j < FIELD_COL_NUM; j++) {
            printw("%c", field[i][j]);
        }
        printw("\n");
    }
    refresh();
    attroff(COLOR_PAIR(1));

    move(oldPos.y,oldPos.x);
}

void PrintLoosingScreen(const PlayerData &data) {
    move(FIELD_ROW_NUM + 2, 0);
    printw("        Oh no! You quit!!         \n");
    printw("[] [] [] [] Game over! [] [] [] []\n");
    refresh();
}

void PrintWinningScreen(const PlayerData & data) {
    move(FIELD_ROW_NUM + 2, 0);
    printw("[] [] [] [] Well done! [] [] [] []\n");
    printw("  You collected all four jewels!  \n");
    printw("       It took you %d moves!      \n", data.turns);
    refresh();
}

// setup colors
void SetupColors() {
    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    init_pair(2, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(3, COLOR_WHITE, COLOR_CYAN);
    init_pair(4, COLOR_WHITE, COLOR_YELLOW);
    init_pair(5, COLOR_WHITE, COLOR_BLUE);
}


// decides where the player move depending on input
bool HandleMoveCommand(PlayerData& data, const char dir) {
    switch (dir) {
        case MOVE_UP:
            --data.position.y;
            return Move(data.position);
        case MOVE_DOWN:
            ++data.position.y;
            return Move(data.position);
        case MOVE_LEFT:
            --data.position.x;
            return Move(data.position);
        case MOVE_RIGHT:
            ++data.position.x;
            return Move(data.position);
        default:
            return false;
    }
}

// move moves the cursor to the specified position
// caps the position on the max/min values of the map
bool Move(Vector2D& pos) {
    if (pos.y >= FIELD_ROW_NUM) {
        pos.y = FIELD_ROW_NUM-1;
        return false;
    }
    if (pos.y < 0) {
        pos.y = 0;
        return false;
    }
    if (pos.x >= FIELD_COL_NUM) {
        pos.x = FIELD_COL_NUM-1;
        return false;
    }
    if (pos.x < 0) {
        pos.x = 0;
        return false;
    }
    move(pos.y, pos.x);
    return true;
}


// deallocation of field
void DeleteField(char**& field) {
    for (int i = 0; i < FIELD_ROW_NUM; i++) {
        delete[] field[i];
        field[i] = nullptr;
    }
    delete[] field;
    field = nullptr;
}


// returns a random number between max and min
int GenerateRandomNumber(int max, int min) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dist(max, min);
    return dist(gen);
}


