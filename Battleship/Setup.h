#ifndef SS02_SETUP_H
#define SS02_SETUP_H

#include <vector>

#define BATTLESHIP_COUNT 5
#define BF_SIZE_X 10
#define BF_SIZE_Y 10
#define SONAR_OFFSET (BF_SIZE_Y + 1)

enum PlayerType {
    pt_ai,
    pt_human
};

enum Alignment {
    a_vertical,
    a_horizontal,
    a_unknown
};

enum Direction {
    dir_up,
    dir_down,
    dir_left,
    dir_right
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
    bool isDestroyed;
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
    Vector2d fieldOffset;
    bool isChoosingTarget;
    Vector2d playerCursor;
    PlayerType type;
    std::vector<Vector2d> hits;
    std::vector<Vector2d> misses;
    Battlefield* battlefield;
};

struct Game {
    bool isRunning;
    bool inProgress;
    std::vector<Player*> players;
    Player* currentPlayer;
    Player* targetedPlayer;
};



bool InitTerminalBattleShip();
void SetupGame(Game&);
Player* SetupPlayer(PlayerType, Vector2d);
void SetupBattlefieldOf(Player&);
void SetupShipsOf(Battlefield&);
void SetupColorsBattleship();
Battleship* CreateBattleship(int);
int GetRandomNumber(int, int);
int CalculatePosIndex(int i);
void Deconstruct(Game&);
Vector2d GetDirection(Direction);




#endif //SS02_SETUP_H