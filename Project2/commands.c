#include <malloc.h>
#include <string.h>
#include "commands.h"
#include "cli.h"
#include "cards.h"
#include "yukon.h"
#include "io.h"

CommandNode *moveHistory;
CommandNode *lastCommand;

// Command enum



// loads a deck of cards from a file, if no file is given, we load a new deck of cards
int LDCommand(char *fileName) {
    // clear card storage, in case there is something
    clearCardStorage();
    // clear the deck as well
    clearDeck();

    bool using_default = false;
    if (fileName == NULL) {
        // load unshuffled deck of cards
        fileName = malloc(sizeof(char)*35);
        using_default = true;
        strcpy(fileName, "examples/decks/unshuffled.txt");
    }

    // load cards from file
    Card *cards = malloc(sizeof(Card)*52);
    int loadStatus;
    int addStatus = -1;

    loadStatus = loadCards(fileName, cards);
    if (loadStatus == 200) {
        // add cards to deck
        addStatus = addCardsToDeck(cards);
    }
    if (addStatus != 200) {
        char *fileName2 = malloc(sizeof(char)*200);
        strcpy(fileName2, "../");
        strcat(fileName2, fileName);
        loadStatus = loadCards(fileName2, cards);
        if (loadStatus == 200) {
            // add cards to deck
            addStatus = addCardsToDeck(cards);
        }
        if (addStatus != 200 && using_default) {
            printf("Warning: Could not find unshuffled.txt in 'examples/decks/' - loading unallocated unshuffled...\n");
            addStatus = addCardsToDeck(allCards);
        }
        free(fileName2);
    }

    if (using_default) {
        free(fileName);
    }

    if (addStatus != 200 && loadStatus != 200) {
        return loadStatus;
    }

    return addStatus;
}

// [SW] function to print/draw the terminal window / GUI/CLI
int SWCommand() {
    drawCards(true);
    return 200;
}

// SI, splits the card deck into to piles
int SICommand(int split) {
    // clear card storage, in case there is something
    clearCardStorage();

    CardNode *pile1 = malloc(sizeof(CardNode));
    int gotCards = getCardsFromDeckAsLinkedList(pile1);
    if (gotCards != 200) {
        return gotCards;
    }

    CardNode *pile2 = pile1;
    for (int i = 0; i < split; i++) {
        if (pile2->next) {
            pile2 = pile2->next;
        }
    }
    pile2->prev->next = NULL;
    pile2->prev = NULL;

    CardNode *currentSplittingCard = pile2;
    CardNode *nextPile1;
    for (int i = 0; i < split; i++) {
        nextPile1 = pile1->next;

        pile1->prev = currentSplittingCard->prev;
        if (pile1->prev) {
            pile1->prev->next = pile1;
        }
        pile1->next = currentSplittingCard;
        currentSplittingCard->prev = pile1;

        currentSplittingCard = currentSplittingCard->next;
        pile1 = nextPile1;
    }
    while (pile2->prev) {
        pile2 = pile2->prev;
    }

    clearDeck();
    addLinkedListCardsToDeck(pile2);

    return 200;
}

// SR command, shuffles the card deck
int SRCommand() {
    // clear card storage, in case there is something
    clearCardStorage();

    CardNode *deck = malloc(sizeof(CardNode));
    int gotCards = getCardsFromDeckAsLinkedList(deck);
    if (gotCards != 200) {
        return gotCards;
    }

    CardNode *randomizedDeck = NULL;

    // create the randomized deck
    int possiblePositions = 1;
    while (deck) {
        CardNode *nextDeck = deck->next;
        if (nextDeck) {
            nextDeck->prev = NULL;
        }

        if (randomizedDeck == NULL) {
            randomizedDeck = deck;
            randomizedDeck->next = NULL;
        }
        else {
            int position = rand() % possiblePositions;
            CardNode *insertionPoint = randomizedDeck;

            while (position > 0) {
                insertionPoint = insertionPoint->next;
                position--;
            }

            if (insertionPoint) {
                CardNode *prev = insertionPoint->prev;
                bool hasPrev = false;
                if (prev) {
                    hasPrev = true;
                    prev->next = deck;
                }
                deck->prev = prev;
                deck->next = insertionPoint;
                insertionPoint->prev = deck;
                if (!hasPrev) {
                    randomizedDeck = deck;
                }
            }
            else {
                insertionPoint = randomizedDeck;
                while (insertionPoint->next) {
                    insertionPoint = insertionPoint->next;
                }
                insertionPoint->next = deck;
                deck->next = NULL;
                deck->prev = insertionPoint;
            }
        }

        deck = nextDeck;
        possiblePositions++;
    }

    clearDeck();
    addLinkedListCardsToDeck(randomizedDeck);

//    // create the randomized deck - rand choice method
//    int numOfCards = 52;
//    while (numOfCards >= 0) {
//        // get random card
//        int randCard = rand() % numOfCards;
//
//        CardNode *card = deck;
//        while (randCard > 0) {
//            card = card->next;
//            randCard--;
//        }
//
//        // remove card from deck
//        if (card->prev) {
//            card->prev->next = card->next;
//        }
//        if (card->next) {
//            card->next->prev = card->prev;
//        }
//        card->prev = NULL;
//        card->next = NULL;
//
//        // Add card to random position in
//
//        numOfCards--;
//    }



    return 200;
}

// SD saves the current deck of cards to a file
int SDCommand(char *fileName) {
    // belive that filename is allocated by caller, but is set to null
    bool malloced = false;
    if (fileName == NULL) {
        malloced = true;
        fileName = malloc(sizeof(char)*10);
        strcpy(fileName, "cards.txt");
    }

    // printf("saving to %s", fileName);
    saveCards(fileName);

    if (malloced) {
        free(fileName);
    }

    return 200;
}

// QQ command, quits the program
int QQCommand() {
    return 200;
}

// P command starts the game using the current card deck
int PCommand() {
    currentPhase = PLAY;
    // check if we're just continuing a game that we just paused
    bool hasStorage = false;
    for (int i = 0; i < 7; i++) {
        if (columnStorage[i] != NULL) {
            hasStorage = true;
            break;
        }
    }

    if (hasStorage) {
        switchCardStorage();
    }
    else {
        convertStartupToPlay();
    }
    return 200;
}

// Q command quits the game and goes back to startup
int QCommand() {
    currentPhase = STARTUP;
    switchCardStorage();
    return 200;
}

// game moves, where you can move the card from one pile to another
int MCommand(char *command, bool fromBottom, bool force) {
    char pile = command[0];
    int column = atol(&command[1]);
    if (((column > 7 && pile == 'C') || (column > 4 && pile == 'F')) || column < 1 || (pile != 'C' && pile != 'F')) {
        return 401;
    }
    char toPile;
    int toColumn;
    char cardName[2];

    Card *card;

    if (fromBottom) {
        toPile = command[4];
        toColumn = atol(&command[5]);

        switch (pile) {
            case 'C':
            {
                CardNode *bottomNode = columns[column - 1];
                while (bottomNode->next) {
                    bottomNode = bottomNode->next;
                }
                card = bottomNode->card;
                break;
            }
            case 'F':
                card = foundations[column-1]->card;
                break;
        }
    }
    else if (pile == 'F') {
        return 402;
    }
    else {
        toPile = command[7];
        toColumn = atol(&command[8]);
        cardName[0] = command[3];
        cardName[1] = command[4];

        // husk at free din bisse
        card = malloc(sizeof(Card));
        getCardFromName(cardName, card);
    }

    if (((toColumn > 7 && toPile == 'C') || (toColumn > 4 && toPile == 'F')) || toColumn < 1 || (toPile != 'C' && toPile != 'F')) {
        if (!fromBottom) free(card);
        return 401;
    }

    CardNode *currentCard;
    switch (pile) {
        case 'C':
            currentCard = columns[column - 1];

            while (currentCard->card->number != card->number || currentCard->card->suit != card->suit) {
                if (!currentCard->next) {
                    if (!fromBottom) free(card);
                    return 402;
                }
                currentCard = currentCard->next;
            }
            if (!currentCard->card->revealed) {
                if (!fromBottom) free(card);
                return 402;
            }

            break;
        case 'F':
            currentCard = foundations[column - 1];
            if (currentCard->card->number != card->number || currentCard->card->suit != card->suit) {
                if (!fromBottom) free(card);
                return 402;
            }
            break;
    }

    CardNode *toCard;
    switch (toPile) {
        case 'C':
            toCard = columns[toColumn - 1];

            if (toCard == NULL) {
                if (currentCard->card->number == 13 || force) {
                    if (currentCard->prev) {
                        currentCard->prev->next = NULL;
                        currentCard->prev->card->revealed = true;
                    }
                    else {
                        switch (pile) {
                            case 'C':
                                columns[column - 1] = NULL;
                                break;
                            case 'F':
                                foundations[column - 1] = NULL;
                                break;
                        }
                    }
                    columns[toColumn - 1] = currentCard;
                    currentCard->prev = NULL;
                } else {
                    if (!fromBottom) free(card);
                    return 402;
                }
            } else {
                while (toCard->next) {
                    toCard = toCard->next;
                }
                if ((toCard->card->suit != currentCard->card->suit &&
                    toCard->card->number - 1 == currentCard->card->number) || force) {
                    if (currentCard->prev) {
                        currentCard->prev->next = NULL;
                        currentCard->prev->card->revealed = true;
                    }
                    else {
                        switch (pile) {
                            case 'C':
                                columns[column - 1] = NULL;
                                break;
                            case 'F':
                                foundations[column - 1] = NULL;
                                break;
                        }
                    }
                    currentCard->prev = toCard;
                    toCard->next = currentCard;

                } else {
                    if (!fromBottom) free(card);
                    return 402;
                }
            }

            break;
        case 'F':
            toCard = foundations[toColumn - 1];

            if (toCard == NULL) {
                if ((currentCard->card->number == 1 && currentCard->next == NULL) || force) {
                    if (currentCard->prev) {
                        currentCard->prev->next = NULL;
                        currentCard->prev->card->revealed = true;
                    }
                    else {
                        switch (pile) {
                            case 'C':
                                columns[column - 1] = NULL;
                                break;
                            case 'F':
                                foundations[column - 1] = NULL;
                                break;
                        }
                    }
                    foundations[toColumn - 1] = currentCard;
                    currentCard->prev = NULL;
                } else {
                    if (!fromBottom) free(card);
                    return 402;
                }
            } else {
                if ((currentCard->card->suit == toCard->card->suit &&
                    currentCard->card->number - 1 == toCard->card->number &&
                    currentCard->next == NULL) || force) {
                    if (currentCard->prev) {
                        currentCard->prev->next = NULL;
                        currentCard->prev->card->revealed = true;
                    }
                    else {
                        switch (pile) {
                            case 'C':
                                columns[column - 1] = NULL;
                                break;
                            case 'F':
                                foundations[column - 1] = NULL;
                                break;
                        }
                    }
                    toCard->prev = currentCard;
                    currentCard->next = toCard;
                    foundations[toColumn - 1] = currentCard;

                    if (currentCard->card->number == 13) {
                        bool not_all_kings = false;
                        for (int i = 0; i < 4; i++){
                            CardNode *cardNode = foundations[i];

                            if (cardNode == NULL || cardNode->card->number != 13){
                                not_all_kings = true;
                                break;
                            }
                        }
                        if (not_all_kings == false){
                            printf("You have won the Yukon Solitaire \n");
                            printf("Please write the command: 'Q' to quit and play again!\n");
                        }
                    }

                } else {
                    if (!fromBottom) free(card);
                    return 402;
                }
            }

            break;
    }

    if (!fromBottom) {
        // gør læreren glad :)
        free(card); 
    }

    return 200;
}

// extra stuff if time is found
// U command, undo the last move
int UCommand() {
    // TODO: remove return when function done
    return 501;

    return 200;
}

// R command, redo the last move
int RCommand() {
    // TODO: remove return when function done
    return 501;

    return 200;
}

// S command saves the current game to a file
int SCommand(char *fileName) {
    // TODO: remove return when function done
    return 501;

    // belive that filename is allocated by caller, but is set to null
    if (fileName == NULL) {
        fileName = malloc(sizeof(char)*14);
        strcpy(fileName, "gamestate.txt");
    }

    return saveState(fileName);
}

// L command loads a game from a file
int LCommand(char *fileName) {
    // TODO: remove return when function done
    return 501;

    return loadState(fileName);
}
