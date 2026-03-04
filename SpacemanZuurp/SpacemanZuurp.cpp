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

struct GameData {
    bool isRunning;
    bool isInGame;
    const std::string wordlist[8];
    std::string secretWord;
    int zuurpProgress;
    UfoState ufoState;
};

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
void PrintWinningScreen(const PlayerData & player_data, const GameData& game_data);
void PrintOnScreen(Vector2d, std::string);
std::string GetUfo(UfoState);
std::string GetZuurp();
bool PlayAgain();

// programs main function
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

    while (gameData.isRunning) {
        gameData.isInGame = true;
        InitNewGameData(gameData, playerData);

        while (gameData.isInGame) {
            // show initial question screen
            UpdateScreen(playerData, gameData);
            const char playerInput = getch();
            UpdateGamestate(playerData, gameData, playerInput);

            if (playerData.numCorrectGuessLocal == MAX_GUESSES) {
                // player has guessed the word
                // win

                PrintWinningScreen(playerData, gameData);
                if (!PlayAgain()) {
                    gameData.isRunning = false;
                }

                gameData.isInGame = false;
                break;
            }


            // player did not guess the word
            // check try count
            if (playerData.numIncorrectGuessLocal < MAX_GUESSES) {
                // player can guess again
                continue;
            }
            else {
                // loose
                UpdateScreen(playerData, gameData);
                PrintLoosingScreen(playerData, gameData);
                if (!PlayAgain()) {
                    gameData.isRunning = false;
                }
                gameData.isInGame = false;
                break;
            }
        }
    }
}

void InitTerminal() {
    // init ncurses terminal
    initscr();
    scrollok(stdscr, true);
}

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

void UpdateGamestate(PlayerData &player_data, GameData &game_data, char player_input) {
    const char caseInsensInput = static_cast<char>(toupper(player_input));
    const char caseInsensLetter = static_cast<char>(toupper(game_data.secretWord[player_data.numCorrectGuessLocal]));

    if (isalpha(caseInsensInput) == false) {
        printw("No letter");
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

void PrintWelcome() {
    printw("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printw("Welcome to SPACEMAN, a word-guessing game!\n");
    printw("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printw("Zuurp, an alien, has crashed on planet Earth! Zuurp mistakes you\n");
    printw("for Earth's leader and demands you\n");
    printw("guess their secret word, one letter at a time. Zuurp's people have\n");
    printw("been monitoring Earth for years, and hence know the English\n");
    printw("language quite well!\n");
    printw("If you don't guess the word quickly enough, Zuurp will\n");
    printw("rebuild their spaceship and leave, but if you do manage to}\n");
    printw("guess Zuurp's secret word then they will stay to play again!}\n");
    printw("Beware, Zuurp likes numbers, and will be recording your data as you guess!\n");
    printw("Press a key to start...");
    getch();
    noecho();
}

int GetRandomNumber(const int min, const int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dist(min, max);
    return dist(gen);
}


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
            "     Zuurp counts %d incorrect guesses... [ %s ] Zuurp's   secret word: %s Earth leader         \n",
        playerData.numIncorrectGuessLocal, playerData.wrongLetters.c_str(), playerData.rightLetters.c_str());

    PrintOnScreen(Vector2d(3,3), GetUfo(gameData.ufoState));
    if (gameData.ufoState != Level10) {
        PrintOnScreen(Vector2d(3,50), GetZuurp());
    }
}

void PrintLoosingScreen(const PlayerData &player_data, const GameData& game_data) {
    clear();
    printw("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n"
           "Zuurp left planet Earth for good this time!\n"
           "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-}\n"
           "%d words fully revealedl.\n"
           "%d word not fully revealed.\n"
           "%d letters correctly guessed.\n"
           "%d letters incorrectly guessed.\n",
           player_data.roundsWon,
           player_data.roundsLost,
           player_data.numCorrectGuessGlobal,
           player_data.numIncorrectGuessGlobal);
}

void PrintWinningScreen(const PlayerData & player_data, const GameData& game_data) {
    clear();
    printw(
        "     Zuurp says, \"My secret word was...\" %s    "
        " -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- "
        "Xzoopz!! That means 'well done' on Zuurp's world!"
        " -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- "
        "                     \\_/                        "
        "                      (\")                       "
        "                    ---+---                      "
        "                      /+\\                       ", game_data.secretWord.c_str());
}

void PrintOnScreen(Vector2d position, std::string s) {
    int x = getcurx(stdscr);
    int y = getcury(stdscr);
    auto oldPos = Vector2d(x,y);

    int lineLength = 0;
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
                " ____/____('')____\\____\n"
                "/_O___O____O____O___O_\\\n"
                "  \\_|_|_|_|_|_|_|_|_/  \n";
        default:
            return "";
    }
}

std::string GetZuurp() {
    return "  \\_/  \n"
              "  (\"\")\n"
              "---+--- \n"
              " _/ \\_ \n ";
}


bool PlayAgain() {
    char playerInput;
    is_echo();
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
