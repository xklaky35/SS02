#include "Setup.h"
#include <cmath>
#include <ncurses.h>


bool InitTerminalBattleShip() {
    initscr();
    noecho();

    if (can_change_color() == false) {
        return false;
    }
    start_color();
    SetupColorsBattleship();
    return true;
}

void SetupColorsBattleship() {

    init_pair(1, COLOR_BLUE, COLOR_BLUE); // water
    init_pair(2, COLOR_BLACK, 250); // ship
    init_pair(3, COLOR_RED, 250); // your ship hit
    init_pair(4, COLOR_WHITE, 250); // enemy ship hit
    init_pair(5, COLOR_WHITE, COLOR_BLUE); // water hit
    init_pair(6, COLOR_GREEN, 235); // sonar
}

void SetupGame(Game& game) {
    game.currentPlayer = nullptr;
    game.targetedPlayer = nullptr;
    game.isRunning = true;
    game.inProgress = true;
    game.players.push_back(SetupPlayer(pt_human, Vector2d(0,0)));
    game.players.push_back(SetupPlayer(pt_ai, Vector2d(15,0)));
}

Player* SetupPlayer(const PlayerType t, Vector2d offset) {
    Player* p = new Player(0, offset,false, Vector2d(0,0), t);
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


Battleship* CreateBattleship(const int size) {
    const int radius = size/2;
    const auto alignment = static_cast<Alignment>(GetRandomNumber(0,1));
    Vector2d* position;
    // the ships can align with the border
    if (alignment == a_horizontal) {
        position = new Vector2d(GetRandomNumber(0 + radius, BF_SIZE_X - 1 - radius),GetRandomNumber(0, BF_SIZE_Y - 1));
    }
    else {
        position = new Vector2d(GetRandomNumber(0, BF_SIZE_X - 1),GetRandomNumber(0 + radius, BF_SIZE_Y - 1 - radius));
    }

    // place components on the field
    std::vector<ShipComponent*> components;
    for (int i = 0; i < size; i++) {

        int posIndex = CalculatePosIndex(i);
        if (alignment == a_horizontal) {
            components.push_back(new ShipComponent(Vector2d(position->x + posIndex, position->y), false)); // alternately add a ship component to the left and to the right of the position
        }
        if (alignment == a_vertical) {
            components.push_back(new ShipComponent(Vector2d(position->x, position->y + posIndex), false)); // alternately add a ship component to the left and to the right of the position
        }
    }

    return new Battleship(size, false, position, alignment, components);
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

// all uneven numbers return a negative value floor of (0,5*i)
// all even numbers return a positive value ceiling of (0,5*i)
int CalculatePosIndex(int i) {
    double posIndex = 1./2. * (std::pow(-1, i) * i);
    if (posIndex >= 0) {
        return static_cast<int>(ceil(posIndex));
    }
    else {
        return static_cast<int>(floor(posIndex));
    }
}


Vector2d GetDirection(const Direction dir) {
    switch (dir) {
        case dir_up: return Vector2d(0,-1);
        case dir_down: return Vector2d(0,1);
        case dir_left: return Vector2d(-1,0);
        case dir_right: return Vector2d(1,0);
        default: return Vector2d(0,0);
    }
}