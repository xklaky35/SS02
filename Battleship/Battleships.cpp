#include <cstdlib>
#include <ncurses.h>
#include <list>
#include <random>

#define BATTLESHIP_COUNT 5
#define BF_SIZE_X 10
#define BF_SIZE_Y 10

enum PlayerType {
    pt_ai,
    pt_human
};

enum Alignment {
    a_vertical,
    a_horizontal
};

struct Vector2d {
    int x;
    int y;
};

struct ShipComponent {
    Vector2d pos;
    bool isHit;
};

struct Battleship {
    int size;
    Vector2d* position;
    Alignment alignment;
    std::vector<ShipComponent*> components;
};

struct Battlefield {
    int size_x;
    int size_y;
    std::vector<Battleship*> battleships;
};

struct Player {
    int points;
    PlayerType type;
    Battlefield* battlefield;
};

struct Game {
    bool isRunning;
    bool inProgress;
    std::vector<Player*> players;
    Player* currentPlayer;
};

bool InitTerminalBattleShip();
void SetupGame(Game&);
Player* SetupPlayer(PlayerType);
void SetupBattlefieldOf(Player&);
void SetupShipsOf(Battlefield&);
void SetupColors();
Battleship* CreateBattleship(int);
int GetRandomNumber(int, int);
void Deconstruct(Game&);
void Shoot(const Player* target, Vector2d vector2d, Game & game);


void PlayBattleships() {
    if (InitTerminalBattleShip() == false) {
        exit(1);
    }
    Game game;
    SetupGame(game);

    while (game.isRunning) {

        game.currentPlayer = game.players[0];
        while (game.inProgress) {

            Shoot(game.players[1], Vector2d(3,3), game);


            break;
        }

        game.currentPlayer = game.players[1];
        while (game.inProgress) {
            break;
        }
        break;
    }



    Deconstruct(game);
    endwin();
}


void Shoot(const Player *target, Vector2d pos, Game &game) {
    for (int i = 0; i < target->battlefield->battleships.size(); i++) {
        for (int j = 0; j < target->battlefield->battleships[i]->components.size(); j++) {

            ShipComponent* shipPart = target->battlefield->battleships[i]->components[j];
            if (pos.x == shipPart->pos.x && pos.y == shipPart->pos.y) {
                // hit a part of a ship
                shipPart->isHit = true;
            }
        }
    }
}




bool InitTerminalBattleShip() {
    initscr();
    noecho();

    if (can_change_color() == false) {
        return false;
    }
    start_color();
    SetupColors();
    return true;
}

void SetupColorsBattleship() {
    init_pair(1, COLOR_BLUE, COLOR_BLUE); // water 1
    init_pair(2, COLOR_CYAN, COLOR_CYAN); // water 2
    init_pair(3, COLOR_BLACK, COLOR_BLUE); // ship
}







void SetupGame(Game& game) {
    game.isRunning = true;
    game.inProgress = true;
    game.players.push_back(SetupPlayer(pt_human));
    game.players.push_back(SetupPlayer(pt_ai));
}

Player* SetupPlayer(const PlayerType t) {
    Player* p = new Player(0, t);
    SetupBattlefieldOf(*p);
    return  p;
}

void SetupBattlefieldOf(Player& p) {
     p.battlefield = new Battlefield(BF_SIZE_X, BF_SIZE_Y);
    SetupShipsOf(*p.battlefield);
}

void SetupShipsOf(Battlefield& b) {
    for (int i = 0; i < BATTLESHIP_COUNT; i++) {
        b.battleships.insert(b.battleships.end(), CreateBattleship(i+1));
    }
}



/*
* 1. Die Schiffe dürfen nicht aneinander stoßen.
* 2. Die Schiffe dürfen nicht über Eck gebaut sein oder Ausbuchtungen besitzen.
* 3. Die Schiffe dürfen auch am Rand liegen.
* 4. Die Schiffe dürfen nicht diagonal aufgestellt werden.
 */





Battleship* CreateBattleship(const int size) {
    const int radius = size/2;
    const auto alignment = static_cast<Alignment>(GetRandomNumber(0,1));
    Vector2d* position;
    // the ships can align with the border
    if (alignment == a_horizontal) {
        position = new Vector2d(GetRandomNumber(0 + radius, (BF_SIZE_X - 1) - radius),GetRandomNumber(0,BF_SIZE_Y - 1));
    }
    else {
        position = new Vector2d(GetRandomNumber(0, BF_SIZE_X - 1),GetRandomNumber(0,BF_SIZE_Y - 1));
    }

    // place components on the field
    std::vector<ShipComponent*> components;
    for (int i = 0; i < size; i++) {
        const int inv_toggle = static_cast<int>(pow(-1, i));
        if (alignment == a_horizontal) {
            components.push_back(new ShipComponent(Vector2d(position->x + inv_toggle*i, position->y), false)); // alternately add a ship component to the left and to the right of the position
        }
        if (alignment == a_vertical) {
            components.push_back(new ShipComponent(Vector2d(position->x, position->y + inv_toggle*i), false)); // alternately add a ship component to the left and to the right of the position
        }
    }

    return new Battleship(size, position, alignment, components);
}


void Deconstruct(Game &game) {
    const int playerCount = static_cast<int>(game.players.size());

    for (int i = 0; i < playerCount; i++) {

        const int shipCount = static_cast<int>(game.players[i]->battlefield->battleships.size());
        for (int j = 0; j < shipCount; j++) {
            delete game.players[i]->battlefield->battleships[j]->position;
            game.players[i]->battlefield->battleships[j]->position = nullptr;

            delete game.players[i]->battlefield->battleships[j];
            game.players[i]->battlefield->battleships[j] = nullptr;
        }

        delete game.players[i]->battlefield;
        game.players[i]->battlefield = nullptr;

        delete game.players[i];
        game.players[i] = nullptr;
    }
}


