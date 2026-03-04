#include <ncurses.h>
#include <random>
#include <string>

#define CURSOR_WELCOME Vector2d(0,0)
#define CURSOR_ZUURP Vector2d(0,10)
#define MAX_GUESSES 9

enum UfoState {
    Level1,
    Level2,
    Level3,
    Level4,
    Level5,
    Level6,
    Level7,
    Level8,
    Level9,
    Level10
};

struct Vector2d {
    int x;
    int y;
};

// data controlled by the game
struct GameData {
    bool isRunning;
    bool isInGame;
    const std::string wordlist[8];
    std::string secretWord;
    int zuurpProgress;
    UfoState ufoState;
};

// data controlled by the player
struct PlayerData {
    int roundsWon;
    int roundsLost;
    int numCorrectGuessLocal;
    int numCorrectGuessGlobal;
    int numIncorrectGuessLocal;
    int numIncorrectGuessGlobal;
    std::string wrongLetters;
    std::string rightLetters;
};

void InitTerminal();
void PrintWelcome();
int GetRandomNumber(int,int);
void InitNewGameData(GameData&, PlayerData&);
void UpdateGamestate(PlayerData&, GameData&, char);
void UpdateScreen(const PlayerData&, const GameData&);
void PrintLoosingScreen(const PlayerData & player_data, const GameData& game_data);
void PrintFinalScreen(const PlayerData & player_data, const GameData& game_data);
void PrintWinningScreen(const PlayerData & player_data, const GameData& game_data);
void PrintOnScreen(Vector2d, std::string);
std::string GetUfo(UfoState);
std::string GetZuurp();
bool PlayAgain();
void WinningSequence(const PlayerData & playerData, GameData & gameData);
void LoosingSequence(const PlayerData & playerData, GameData & gameData);
void ZuurpStartsFromScratch();
void ZuurpReturns();


void PlaySpacemanZuurp() {
    PlayerData playerData = PlayerData(0,0,0,0,0,0, "", "");
    GameData gameData = GameData(
        true,
        true,
        {"Elephant","Kangaroo", "Antelope", "Flamingo", "Lion", "Chipmunk", "Dolphin", "Giraffe"},
        "",
        0,
        Level1
        );

    InitTerminal();
    PrintWelcome();

    // game loop
    while (gameData.isRunning)  {
        gameData.isInGame = true;
        InitNewGameData(gameData, playerData);

        // round loop
        while (gameData.isInGame) {
            // show initial question screen
            UpdateScreen(playerData, gameData);

            // get player input
            const char playerInput = getch();
            UpdateGamestate(playerData, gameData, playerInput);

            if (playerData.numCorrectGuessLocal == gameData.secretWord.length()) {
                // player guessed the word
                playerData.roundsWon++;
                WinningSequence(playerData, gameData);
            }
            else {
                // player did not guess the word

                if (playerData.numIncorrectGuessLocal >= MAX_GUESSES) {
                    // player has no guesses left
                    playerData.roundsLost++;
                    LoosingSequence(playerData, gameData);
                }
            }
        }
    }
    PrintFinalScreen(playerData, gameData);
}

// init ncurses terminal
void InitTerminal() {
    initscr();
    scrollok(stdscr, true);
}

// resets the gamestate
void InitNewGameData(GameData &gameData, PlayerData& playerData) {
    const int randomWordIndex = GetRandomNumber(0,7);

    gameData.secretWord = gameData.wordlist[randomWordIndex];
    gameData.ufoState = Level1;
    playerData.numIncorrectGuessLocal = 0;
    playerData.numCorrectGuessLocal = 0;

    playerData.wrongLetters = "";
    for (int i = 0; i < 9; i++) {
        playerData.wrongLetters.append("_", 1);
    }
    playerData.rightLetters = "";
    for (int i = 0; i < gameData.secretWord.length(); i++) {
        playerData.rightLetters.append("_", 1);
    }
}


// checks if the players input was correct or not and updates the UI accordignly
void UpdateGamestate(PlayerData &player_data, GameData &game_data, char player_input) {
    const char caseInsensInput = static_cast<char>(toupper(player_input));
    const char caseInsensLetter = static_cast<char>(toupper(game_data.secretWord[player_data.numCorrectGuessLocal]));

    if (isalpha(caseInsensInput) == false) {
        printw("Zuurp says, \"Qzwopp! %c is not a letter!\" Earth leader?", player_input);
        return;
    }

    // if he guesses wrong
    if (caseInsensInput != caseInsensLetter) {
        player_data.wrongLetters[player_data.numIncorrectGuessLocal] = caseInsensInput;
        player_data.numIncorrectGuessLocal++;
        player_data.numIncorrectGuessGlobal++;
        game_data.ufoState = static_cast<UfoState>(game_data.ufoState + 1);
    }
    else {
        // if he guesses right
        player_data.rightLetters[player_data.numCorrectGuessLocal] = caseInsensInput;
        player_data.numCorrectGuessLocal++;
        player_data.numCorrectGuessGlobal++;
    }

    UpdateScreen(player_data, game_data);
}


// redraw the screen with the games current data
void UpdateScreen(const PlayerData& playerData, const GameData& gameData) {
    clear();
    printw(
            "+----------------------------------------------------------------------------------------------+\n"
            "                                                                                                \n"
            "                                                                                                \n"
            "                                                                                                \n"
            "                                                                                                \n"
            "                                                                                                \n"
            "                                                                                                \n"
            "                                                                                                \n"
            "                                                                                                \n"
            "+----------------------------------------------------------------------------------------------+\n"
            "     Zuurp counts %d incorrect guesses... [ %s ] Zuurp's   secret word: %s                      \n",
        playerData.numIncorrectGuessLocal, playerData.wrongLetters.c_str(), playerData.rightLetters.c_str());

    PrintOnScreen(Vector2d(3,3), GetUfo(gameData.ufoState));
    if (gameData.ufoState != Level10) {
        PrintOnScreen(Vector2d(3,50), GetZuurp());
    }
}

// screen if player chooses to end the game
void PrintFinalScreen(const PlayerData &player_data, const GameData& game_data) {
    clear();
    printw("      -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-       \n"
           "        Zuurp left planet Earth for good this time!         \n"
           "      -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-       \n"
           "                 %d words fully revealedl.                  \n"
           "                 %d word not fully revealed.                \n"
           "                 %d letters correctly guessed.              \n"
           "                 %d letters incorrectly guessed.            \n",
           player_data.roundsWon,
           player_data.roundsLost,
           player_data.numCorrectGuessGlobal,
           player_data.numIncorrectGuessGlobal);
    getch();
}

// screen if player has not guessed the word
void PrintLoosingScreen(const PlayerData &player_data, const GameData& game_data) {
    move(15,0);
    printw("Zuurp counts %d incorrect guesses... [ %s ] Zuurp says,              \n"
           "         \"My secret word was .....................\"                \n"
           "                                                                     \n"
           "                           %s                                        \n"
           "                                                                     \n"
           "        -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-              \n"
           "                 Zuurp is ready to go home                           \n",
           player_data.numIncorrectGuessLocal, player_data.wrongLetters.c_str(), game_data.secretWord.c_str());
}


// screen if player won the game
// screen if player won the game
void PrintWinningScreen(const PlayerData & player_data, const GameData& game_data) {
    clear();
    printw(
        "     Zuurp says, \"My secret word was...\" %s    \n"
        "                                                 \n"
        "                                                 \n"
        " -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- \n"
        "Xzoopz!! That means 'well done' on Zuurp's world!\n"
        " -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- \n"
        "                                                 \n"
        "                                                 \n"
        "                      \\_/                       \n"
        "                      (\")                       \n"
        "                    ---+---                      \n"
        "                      /+\\                       \n"
        "                                                 \n", game_data.secretWord.c_str());
}

// screen when player enters the game
void PrintWelcome() {
    PrintOnScreen(Vector2d(0,0),
        "        -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-            \n"
            "           Welcome to SPACEMAN, a word-guessing game!              \n"
            "        -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-            \n"
            "Zuurp, an alien, has crashed on planet Earth! Zuurp mistakes you   \n"
            "                for Earth's leader and demands you                 \n"
            "guess their secret word, one letter at a time. Zuurp's people have \n"
            "    been monitoring Earth for years, and hence know the English    \n"
            "                      language quite well!                         \n"
            "       If you don't guess the word quickly enough, Zuurp will      \n"
            "     rebuild their spaceship and leave, but if you do manage to}   \n"
            "   guess Zuurp's secret word then they will stay to play again!}   \n"
            "Beware, Zuurp likes numbers, and will be recording your data as you guess! \n"
            "Press a key to start...");
    getch();
    noecho(); // needed so the enter key is detected
}


// print the povided string on the screen at a specific location
void PrintOnScreen(Vector2d position, std::string s) {
    const int x = getcurx(stdscr);
    const int y = getcury(stdscr);
    auto oldPos = Vector2d(x,y);

    int lineLength = 0;

    // loops through the string to translate linebreaks
    // this way multiline images can be generated
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '\n') {
            ++position.x;
            position.y -= lineLength;
            lineLength = 0;
            move(position.x, position.y);
            continue;
        }
        ++lineLength;
        ++position.y;
        move(position.x, position.y);
        printw("%c", s[i]);
    }
    move(oldPos.x, oldPos.y);
}

// asks if the player wants to play again
bool PlayAgain() {
    char playerInput;
    printw("Play again? ");

    do {
        playerInput = getch();
        if (playerInput == 'n' || playerInput == 'N') {
            return false;
        }
        if (playerInput == 'y' || playerInput == 'Y') {
            return true;
        }

    } while (!isalpha(playerInput)) ;

    return false;
}


// starts the winning sequence
// Prints winning screen and asks the player to play again
void WinningSequence(const PlayerData &playerData, GameData &gameData) {

    PrintWinningScreen(playerData, gameData);

    // check if player wants to play again
    if (PlayAgain()) {
        gameData.isInGame = false;
        return;
    }
    gameData.isInGame = false;
    gameData.isRunning = false;
}

// starts the loosing sequence
// Prints loosing screen and asks the player to play again
void LoosingSequence(const PlayerData &playerData, GameData &gameData) {
    PrintLoosingScreen(playerData, gameData);
    if (PlayAgain()) {
        ZuurpReturns();
        gameData.isInGame = false;
        return;
    }
    gameData.isInGame = false;
    gameData.isRunning = false;
}

void ZuurpReturns() {
    clear();
    PrintOnScreen(Vector2d(0,0),
    "     -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-      \n"
         "   Something went wrong with Zuurp's spaceship... Zuurp   \n"
         "        returns to planet Earth to rebuild again...       \n"
         "     -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-      \n");
    getch();
}

// generates a random number between the given thresholds
int GetRandomNumber(const int min, const int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dist(min, max);
    return dist(gen);
}

std::string GetUfo(UfoState state) {
    switch (state) {
        case 0:
            return
               "";
        case 1:
            return
            " _____________________  \n"
               "  \\________________/   \n";
        case 2:
            return
            " _____________________  \n"
               "/_____________________\\\n"
               "  \\_________________/  \n";
        case 3:
            return
             " _____________________  \n"
                "/__________O__________\\\n"
                "  \\_________________/  \n";
        case 4:
            return
             " _____________________  \n"
                "/_O___O____O__________\\\n"
                "  \\_________________/  \n";
        case 5:
            return
             " _____________________  \n"
                "/_O___O____O____O___O_\\\n"
                "  \\_________________/  \n";
        case 6:
            return
             " _____/_________\\_____ \n"
                "/_O___O____O____O___O_\\\n"
                "  \\_________________/  \n";
        case 7:
            return
             " _____/_________\\_____\n"
                "/_O___O____O____O___O_\\\n"
                "  \\_|_|_|_|_|_|_|_|_/ \n";
        case 8:
            return
             "          ....         \n"
                "       ..      ..      \n"
                " ____/____________\\____\n"
                "/_O___O____O____O___O_\\\n"
                "  \\_|_|_|_|_|_|_|_|_/ \n";
        case 9:
            return
             "          ....          \n"
                "       .. \\__/ ..      \n"
                " ____/____(\")_____\\____\n"
                "/_O___O____O____O___O_\\\n"
                "  \\_|_|_|_|_|_|_|_|_/  \n";
        default:
            return "";
    }
}

std::string GetZuurp() {
    return "  \\_/  \n"
              "  (\")  \n"
              "---+--- \n"
              " _/ \\_ \n ";
}
