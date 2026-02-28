#include <iostream>

/* #include required headers here */
struct Vector2d {
    int x, y;
};
struct Player
{
    Vector2d position;
    int health;
    bool isAlive;
};

void SetupPlayer(Player&);
void PrintPlayerDetails(const Player&);

/* Insert Prototypes here */
void UseStructures()
{
    // Declare two local structure variables.
    auto p1 = Player();
    auto p2 = Player();


    // here the default initialization is used for the properties of the player
    // Call PrintPlayerDetails with player1.
    PrintPlayerDetails(p1);
    // Call PrintPlayerDetails with player2.
    PrintPlayerDetails(p2);

    // Call SetupPlayer with player1.
    SetupPlayer(p1);
    // Call SetupPlayer with player2.
    SetupPlayer(p2);

    // now the properties are properly initialized
    // Call PrintPlayerDetails with player1.
    PrintPlayerDetails(p1);
    // Call PrintPlayerDetails with player2.
    PrintPlayerDetails(p2);
}
/* Insert function definitions here */

void SetupPlayer(Player& p) {
    p.health = 100;
    p.position = Vector2d(0,0);
    p.isAlive = true;
}

void PrintPlayerDetails(const Player& p) {
    std::cout << "Player's Current State:" << std::endl;
    std::cout << "\t- Position: (" << p.position.x << ", " << p.position.y << ")" << std::endl;
    std::cout << "\t- Health: (" << p.health << ")" << std::endl;
    std::cout << "\t- Alive: " << (p.isAlive ? "yes": "no") <<  std::endl;
}
