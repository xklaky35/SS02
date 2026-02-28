#include <iostream>
#include <ostream>
#include <random>

struct Vector2d {
    int x;
    int y;
};
struct NPC
{
    Vector2d position;
    int health;
    int strength;
    int tiredness;
    bool isAlive;
};

NPC* CreateNPC();
void PrintNPCDetails(const NPC* npc);
int getRandomNumber(int min, int max);

void UsePointersAndTheFreeStore()
{
    // Declare NPC pointer array.
    NPC** array = new NPC*[10];

    // add in the NPCs
    for (int i = 0; i < 10; i++) {
        array[i] = CreateNPC();
        PrintNPCDetails(array[i]);
    }

    // delete NPCs
    for (int i = 0; i < 10; i++) {
        delete array[i];
        array[i] = nullptr;
    }

    // delete initial pointer array
    delete[] array;
}
/* Insert function definitions here */

int getRandomNumber(const int min, const int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dist(min, max);

    return dist(gen);
}

// create new NPC
NPC* CreateNPC() {
    try {
        return new NPC {
            Vector2d(getRandomNumber(0,100), getRandomNumber(0,100)),
            getRandomNumber(50,200),
            getRandomNumber(3,20),
            getRandomNumber(1,15),
            true
        };
    }catch (std::bad_alloc& e) {
        std::cout << "Memory allocation failed: " << e.what() << std::endl;
        return nullptr;
    }

}

void PrintNPCDetails(const NPC* npc) {
    if (npc != nullptr) {
        std::cout << "NPC's current state:" << std::endl;
        std::cout << "\t- Position: (" << npc->position.x << ", " << npc->position.y << ")" << std::endl;
        std::cout << "\t- Health: " << npc->health << std::endl;
        std::cout << "\t- Strength: " << npc->strength << std::endl;
        std::cout << "\t- Tiredness: " << npc->tiredness << std::endl;
    }

}


