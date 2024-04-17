#ifndef MACHINE_ORIENTED_PROGRAMMING_PROJECT_2_COMMANDS_H
#define MACHINE_ORIENTED_PROGRAMMING_PROJECT_2_COMMANDS_H

#include <stdbool.h>

// Command enum
typedef enum {
    NONE,
    LD,
    SW,
    SI,
    SR,
    SD,
    QQ,
    P,
    Q,
    MOVE,
    U,
    R,
    S,
    L
} Command;

//Double linked for commands
typedef struct CommandNodeP CommandNode;
typedef struct CommandNodeP {
    CommandNode *prev;
    CommandNode *next;
    Command command;
    char *commandData;
    int status;
} CommandNode;

// the moves/executed commands can be stored in a linked list
// in order to be able to undo commands
// the commands that have been undone can be stored in a linked list
// in order to be able to redo commands
extern CommandNode *moveHistory;
extern CommandNode *lastCommand;

// loads a deck of cards from a file, if no file is given, we load a new deck of cards
int LDCommand(char *fileName);

// [SW] function to print/draw the terminal window / GUI/CLI
int SWCommand();

// SI, splits the card deck into to piles
int SICommand(int split);

// SR command, shuffles the card deck
int SRCommand();

// SD saves the current deck of cards to a file
int SDCommand(char *fileName);

// QQ command, quits the program
int QQCommand();

// P command starts the game using the current card deck
int PCommand();

// Q command quits the game and goes back to startup
int QCommand();

// game moves, where you can move the card from one pile to another
int MCommand(char *command, bool fromBottom, bool force);

// extra stuff if time is found
// U command, undo the last move
int UCommand();

// R command, redo the last move
int RCommand();

// S command saves the current game to a file
int SCommand(char *fileName);

// L command loads a game from a file
int LCommand(char *fileName);


#endif //MACHINE_ORIENTED_PROGRAMMING_PROJECT_2_COMMANDS_H

