#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include <random>
#include "Setup.h"


void PlayTurn(Game&);
void ChooseTarget(Player&);
void LetPlayerChooseTarget(Player&);
void LetAiChooseTarget(Player&);
void Move(Player& player, Vector2d targetPos);
void UpdateScreen(const Game & game);
void UpdateFieldOf(const Player&);
void UpdateSonarOf(const Player&);
bool Shoot(Player& currentPlayer, const Player& target, const Game & game);
bool HasHit(const std::vector<Battleship*>&, Vector2d);
bool AllShipsDestroyedOf(const Player&);
bool IsInBounds(Vector2d);
Vector2d GetRandomN4NeighbourAt(const Vector2d&, const Player&);
Vector2d GetRandomPosFor(const Player&);
bool Contains(const std::vector<Vector2d>&, Vector2d);


void PlayBattleships() {
    if (InitTerminalBattleShip() == false) {
        exit(1);
    }
    Game game;

    SetupGame(game);
    UpdateScreen(game);

    while (game.isRunning) {

        game.currentPlayer = game.players[0];
        game.targetedPlayer = game.players[1];
        PlayTurn(game);


        game.currentPlayer = game.players[1];
        game.targetedPlayer = game.players[0];
        PlayTurn(game);
    }

    Deconstruct(game);
    endwin();
}


void PlayTurn(Game& game) {
    while (game.inProgress) {

        ChooseTarget(*game.currentPlayer);

        if (Shoot(*game.currentPlayer, *game.targetedPlayer, game) == false) {
            break;
        }

        if (AllShipsDestroyedOf(*game.targetedPlayer)) {
            game.inProgress = false;
            game.isRunning = false;
        }
    }
}

void ChooseTarget(Player& player) {
    if (player.type == pt_human) {
        LetPlayerChooseTarget(player);
    }
    else if (player.type == pt_ai) {
        LetAiChooseTarget(player);
    }
}


void LetPlayerChooseTarget(Player& player) {
    player.isChoosingTarget = true;

    while (player.isChoosingTarget) {
        switch (getch()) {
            case 'h': {
                Move(player, Vector2d(player.playerCursor.x - 1, player.playerCursor.y));
                break;
            }
            case 'j': {
                Move(player, Vector2d(player.playerCursor.x, player.playerCursor.y + 1));
                break;
            }
            case 'k': {
                Move(player, Vector2d(player.playerCursor.x, player.playerCursor.y - 1));
                break;
            }
            case 'l': {
                Move(player, Vector2d(player.playerCursor.x + 1, player.playerCursor.y));
                break;
            }
            case 'x': {
                player.isChoosingTarget = false;
                break;
            }
            default: break;
        }
    }
}

void Move(Player& player, const Vector2d targetPos) {
    // move current cursor
    if (IsInBounds(targetPos)) {

        player.playerCursor = targetPos;
        move(player.playerCursor.y + player.fieldOffset.y, player.playerCursor.x + player.fieldOffset.x);
    }
}

bool IsInBounds(const Vector2d pos) {
    if (pos.x >= 0 && pos.x < BF_SIZE_X
        && pos.y >= 0 && pos.y < BF_SIZE_Y) {
        return true;
    }
    return false;
}

void LetAiChooseTarget(Player& aiPlayer) {

    // has already hit?
    for (int i = 0; i < aiPlayer.hits.size(); i++) {

        // check if horizontal
        const auto h1 = Vector2d(aiPlayer.hits[i].x + 1, aiPlayer.hits[i].y);
        const auto h2 = Vector2d(aiPlayer.hits[i].x - 1, aiPlayer.hits[i].y);
        if (Contains(aiPlayer.hits, h1) || Contains(aiPlayer.hits, h2)) {
            bool singleSideLocked = false;


            for (int j = 0; j < BF_SIZE_X ; j++) {

                // used with increasing values we can alter between the sides of a coordiante and increase the distance
                // this way we can increasingly check both sides of a coordinate
                int posIndex = CalculatePosIndex(j);
                auto posToCheck = Vector2d(aiPlayer.hits[i].x + posIndex, aiPlayer.hits[i].y);


                // if a spot is hit where the ai missed before
                if (singleSideLocked == true || Contains(aiPlayer.misses, posToCheck)) {

                    // flag to access this code block
                    singleSideLocked = true;

                    // this locks the negative or positive iteration of numbers
                    // now only one direction is checked
                    posIndex = CalculatePosIndex(j+1);
                    posToCheck.x += posIndex;
                }

                // if we encounter a  field the ai missed before while one side is ignored, we hit the other end of the ship
                // in this case we just go on the the next hit point
                if (singleSideLocked == true && Contains(aiPlayer.misses, posToCheck)) {
                    break;
                }

                // if we did not encounter the point before we shoot it
                if (!Contains(aiPlayer.hits, posToCheck) && !Contains(aiPlayer.misses, posToCheck)) {
                    Move(aiPlayer, Vector2d(aiPlayer.hits[i].x + posIndex, aiPlayer.hits[i].y));
                    return;
                }
            }
            continue;
        }

        // check if vertical
        const auto v1 = Vector2d(aiPlayer.hits[i].x, aiPlayer.hits[i].y + 1);
        const auto v2 = Vector2d(aiPlayer.hits[i].x, aiPlayer.hits[i].y - 1);
        if (Contains(aiPlayer.hits, v1) || Contains(aiPlayer.hits, v2)) {
            bool singleSideLocked = false;

            for (int j = 0; j < BF_SIZE_Y ; j++) {
                int posIndex = CalculatePosIndex(j);
                auto posToCheck = Vector2d(aiPlayer.hits[i].x, aiPlayer.hits[i].y + posIndex);

                if (singleSideLocked == true && Contains(aiPlayer.misses, posToCheck)) {
                    break;
                }

                if (singleSideLocked == true || Contains(aiPlayer.misses, posToCheck)) {
                    singleSideLocked = true;

                    // this locks the negative or positive iteration of numbers
                    posIndex = CalculatePosIndex(j+1);
                    posToCheck.y += posIndex;
                }

                if (!Contains(aiPlayer.hits, posToCheck) && !Contains(aiPlayer.misses, posToCheck)) {
                    Move(aiPlayer, Vector2d(aiPlayer.hits[i].x, aiPlayer.hits[i].y + posIndex));
                    return;
                }
            }
            continue;
        }


        // if it is not possible to tell if the ship is vertical or horizontal, the n4 neighbors are checked
        if (Vector2d newPos = GetRandomN4NeighbourAt(aiPlayer.hits[i], aiPlayer); newPos.x != -1) {
            return Move(aiPlayer, newPos);
        }
    }

    // if no n4 neighbors are available or there are no hits yet, we choose a random location
    Vector2d newPos = GetRandomPosFor(aiPlayer);
    Move(aiPlayer, newPos);
}

// current player shoots at the current cursor location
bool Shoot(Player& currentPlayer, const Player& target, const Game &game) {
    // checks if the current players cursor has targeted a ship from the opponent
    if (HasHit(target.battlefield->battleships, currentPlayer.playerCursor)) {
        // if hit add to list of hit targets
        currentPlayer.hits.push_back(currentPlayer.playerCursor);
        UpdateScreen(game);
        return true;
    }

    // if no hit add to list of misses
    currentPlayer.misses.push_back(currentPlayer.playerCursor);
    UpdateScreen(game);
    return false;
}

// check if the shot hit a enemy ship and marks it if so
bool HasHit(const std::vector<Battleship*>& shipsToCheck, const Vector2d targetedLoc) {
    // checks if the current players cursor has targeted a ship from the opponent
    for (int i = 0; i < shipsToCheck.size(); i++) {
        for (int j = 0; j < shipsToCheck[i]->components.size(); j++) {
            ShipComponent* shipPart = shipsToCheck[i]->components[j];
            if (targetedLoc.x == shipPart->pos.x && targetedLoc.y == shipPart->pos.y) {
                // hit a part of a ship
                shipPart->isHit = true;
                return true;
            }
        }
    }
    return false;
}

void UpdateScreen(const Game& game) {
    // redraw field and sonar
    clear();
    const int oldCursorX = getcurx(stdscr);
    const int oldCursorY = getcury(stdscr);

    for (int i = 0; i < game.players.size(); i++) {
        // dont draw ai field
        if (game.players[i]->type == pt_ai) {
            continue;
        }

        // draw sonar field
        attron(COLOR_PAIR(6));
        for (int j = game.players[i]->fieldOffset.x; j < BF_SIZE_X + game.players[i]->fieldOffset.x; j++) {
            for (int k = game.players[i]->fieldOffset.y; k < BF_SIZE_Y + game.players[i]->fieldOffset.y; k++) {
                move(k,j);
                printw(" ");
            }
        }
        attroff(COLOR_PAIR(6));

        // draw ship field
        attron(COLOR_PAIR(1));
        for (int j = game.players[i]->fieldOffset.x; j < BF_SIZE_X + game.players[i]->fieldOffset.x; j++) {
            for (int k = game.players[i]->fieldOffset.y; k < BF_SIZE_Y + game.players[i]->fieldOffset.y; k++) {
                move(k + SONAR_OFFSET,j);
                printw(" ");
            }
        }
        attroff(COLOR_PAIR(1));

        UpdateSonarOf(*game.players[i]);
        UpdateFieldOf(*game.players[i]);
    }

    move(oldCursorY, oldCursorX);
}

void UpdateFieldOf(const Player& player) {
    // mark your ships the enemy has hit
    for (int j = 0; j < player.battlefield->battleships.size(); j++) {
        for (int k = 0; k < player.battlefield->battleships[j]->components.size(); k++) {
            ShipComponent* component = player.battlefield->battleships[j]->components[k];
            move(component->pos.y + player.fieldOffset.y + SONAR_OFFSET, component->pos.x + player.fieldOffset.x);
            if (component->isHit) {
                attron(COLOR_PAIR(3));
                printw("X");
                attroff(COLOR_PAIR(3));
            } else {
                attron(COLOR_PAIR(2));
                printw(" ");
                attroff(COLOR_PAIR(2));
            }
        }
    }
}

void UpdateSonarOf(const Player &player) {
    attron(COLOR_PAIR(6));
    // mark missed shots
    for (int i = 0; i < player.misses.size(); i++) {
        move(player.misses[i].y + player.fieldOffset.y, player.misses[i].x + player.fieldOffset.x);
        printw("O");
    }
    attroff(COLOR_PAIR(6));

    attron(COLOR_PAIR(6));
    // mark hit shots
    for (int i = 0; i < player.hits.size(); i++) {
        move(player.hits[i].y + player.fieldOffset.y, player.hits[i].x + player.fieldOffset.x);
        printw("X");
    }
    attroff(COLOR_PAIR(6));
}



bool AllShipsDestroyedOf(const Player& p) {
    for (int i = 0; i < p.battlefield->battleships.size(); i++) {
        for (int j = 0; j < p.battlefield->battleships[i]->components.size(); j++) {
            if (p.battlefield->battleships[i]->components[j]->isHit == false) {
                return false;
            }
        }
    }
    return true;
}



Vector2d GetRandomPosFor(const Player& aiPlayer) {
    // get random fields until all fields are occupied (very bad performance!)
    while (aiPlayer.hits.size() + aiPlayer.misses.size() != BF_SIZE_X * BF_SIZE_Y) {
        const auto randomPos = Vector2d(GetRandomNumber(0, BF_SIZE_X - 1), GetRandomNumber(0, BF_SIZE_Y - 1));
        if (!Contains(aiPlayer.hits, randomPos) && !Contains(aiPlayer.misses, randomPos)) {
            return randomPos;
        }
    }
    // if this is reached all fields are hit but no winner yet
    // should be impossible
    exit(1);
}

Vector2d GetRandomN4NeighbourAt(const Vector2d& pos, const Player& aiPlayer) {

    // check all n4 neighbors
    for (int i = 0; i < 4; i++) {
        const Vector2d dir = GetDirection(static_cast<Direction>(i));
        const auto newPos = Vector2d(pos.x + dir.x, pos.y + dir.y);
        if (IsInBounds(newPos) && !Contains(aiPlayer.hits, newPos) && !Contains(aiPlayer.misses, newPos)) {
            return newPos;
        }
    }
    // indicates that no n4 neighbors are left
    return Vector2d(-1,-1);
}

bool Contains(const std::vector<Vector2d>& playerHits, const Vector2d hit) {
    for (int i = 0; i < playerHits.size(); i++) {
        // check if horizontal
        if (playerHits[i].x == hit.x && playerHits[i].y == hit.y) {
            return true;
        }
    }
    return false;
}
