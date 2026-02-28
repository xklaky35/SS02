# COMP710: Studio Session 02 – Exercise:

## EXERCISE NAME: C++ – Using Structures

Create a Visual Studio Solution named “SS02” and C++ Project named “Using Structures” for this exercise.

Declare a structure that represents a player in a 2D game environment. The player must have fields for their position, a value for their health, and a Boolean to represent whether or not they are alive.

The skeleton of this program is provided below:

```cpp
/* #include required headers here */

struct Player
{
    /* Insert your code here */
};

/* Insert Prototypes here */

int main()
{
    return 0;
}

/* Insert function definitions here */
```

Next, write a function named SetupPlayer() which takes in a reference to a Player structure, and populates the fields of the structure with the following values:

- position of (0, 0)
- health of 100
- alive state of true

Ensure you have a prototype and a definition for the SetupPlayer function.

Next, write a function called PrintPlayerDetails() which takes in a const reference to a Player structure. In this function print out the player’s details in the following format:

```
Player's Current State:
    Position: (0, 0)
    Health: 100
    Alive: Yes
```

Next, in the main function:

- Add two local structure variables named player1 and player2.
- Call PrintPlayerDetails with player1.
- Call PrintPlayerDetails with player2.
- Then call SetupPlayer with player1 and player2.
- Then call PrintPlayerDetails again.

Example structure of main:

```cpp
/* #include required headers here */

struct Player
{
    /* Insert your code here */
};

/* Insert Prototypes here */

int main()
{

    // Declare two local structure variables.

    // Call PrintPlayerDetails with player1.
    // Call PrintPlayerDetails with player2.

    // Call SetupPlayer with player1.
    // Call SetupPlayer with player2.

    // Call PrintPlayerDetails with player1.
    // Call PrintPlayerDetails with player2.

    return 0;
}

/* Insert function definitions here */
```

Add a comment to the source code that explains the difference between the details printed before the calls to SetupPlayer and the details printed afterwards.

Once complete, commit your program’s source code to your individual GitHub repository.