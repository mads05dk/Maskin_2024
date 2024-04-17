#include <malloc.h>
#include "cards.h"
#include "yukon.h"

Card allCards[] = {
        {.number = 1, .suit = CLUBS, .revealed = false},
        {.number = 2, .suit = CLUBS, .revealed = false},
        {.number = 3, .suit = CLUBS, .revealed = false},
        {.number = 4, .suit = CLUBS, .revealed = false},
        {.number = 5, .suit = CLUBS, .revealed = false},
        {.number = 6, .suit = CLUBS, .revealed = false},
        {.number = 7, .suit = CLUBS, .revealed = false},
        {.number = 8, .suit = CLUBS, .revealed = false},
        {.number = 9, .suit = CLUBS, .revealed = false},
        {.number = 10, .suit = CLUBS, .revealed = false},
        {.number = 11, .suit = CLUBS, .revealed = false},
        {.number = 12, .suit = CLUBS, .revealed = false},
        {.number = 13, .suit = CLUBS, .revealed = false},
        {.number = 1, .suit = DIAMONDS, .revealed = false},
        {.number = 2, .suit = DIAMONDS, .revealed = false},
        {.number = 3, .suit = DIAMONDS, .revealed = false},
        {.number = 4, .suit = DIAMONDS, .revealed = false},
        {.number = 5, .suit = DIAMONDS, .revealed = false},
        {.number = 6, .suit = DIAMONDS, .revealed = false},
        {.number = 7, .suit = DIAMONDS, .revealed = false},
        {.number = 8, .suit = DIAMONDS, .revealed = false},
        {.number = 9, .suit = DIAMONDS, .revealed = false},
        {.number = 10, .suit = DIAMONDS, .revealed = false},
        {.number = 11, .suit = DIAMONDS, .revealed = false},
        {.number = 12, .suit = DIAMONDS, .revealed = false},
        {.number = 13, .suit = DIAMONDS, .revealed = false},
        {.number = 1, .suit = HEARTS, .revealed = false},
        {.number = 2, .suit = HEARTS, .revealed = false},
        {.number = 3, .suit = HEARTS, .revealed = false},
        {.number = 4, .suit = HEARTS, .revealed = false},
        {.number = 5, .suit = HEARTS, .revealed = false},
        {.number = 6, .suit = HEARTS, .revealed = false},
        {.number = 7, .suit = HEARTS, .revealed = false},
        {.number = 8, .suit = HEARTS, .revealed = false},
        {.number = 9, .suit = HEARTS, .revealed = false},
        {.number = 10, .suit = HEARTS, .revealed = false},
        {.number = 11, .suit = HEARTS, .revealed = false},
        {.number = 12, .suit = HEARTS, .revealed = false},
        {.number = 13, .suit = HEARTS, .revealed = false},
        {.number = 1, .suit = SPADES, .revealed = false},
        {.number = 2, .suit = SPADES, .revealed = false},
        {.number = 3, .suit = SPADES, .revealed = false},
        {.number = 4, .suit = SPADES, .revealed = false},
        {.number = 5, .suit = SPADES, .revealed = false},
        {.number = 6, .suit = SPADES, .revealed = false},
        {.number = 7, .suit = SPADES, .revealed = false},
        {.number = 8, .suit = SPADES, .revealed = false},
        {.number = 9, .suit = SPADES, .revealed = false},
        {.number = 10, .suit = SPADES, .revealed = false},
        {.number = 11, .suit = SPADES, .revealed = false},
        {.number = 12, .suit = SPADES, .revealed = false},
        {.number = 13, .suit = SPADES, .revealed = false}
};

// convert the startup deck to a playing deck
// this puts the current deck into the deck storage!
int convertStartupToPlay() {
    switchCardStorage();
    CardNode *tempColumnStorage[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    for (int i = 0; i < 7; i++) {
        tempColumnStorage[i] = columns[i];
        columns[i] = NULL;
    }
    int rowCounts[7] = { 0, 0, 0, 0, 0, 0, 0 };
    int emptyCount = 0;
    int tempColCounter = 0;
    while (emptyCount < 7) {
        emptyCount = 0;
        for (int j = 0; j < 7; j++) {
            if (tempColumnStorage[j] == NULL || tempColumnStorage[j]->card == NULL) {
                emptyCount++;
            }
        }
        if (emptyCount >= 7) {
            break;
        }

        // loop through columns
        for (int i = 0; i < 7; i++) {
            // add to the first column that accepts more stuff
            if ((i == 0 && rowCounts[i] < 1) || (i > 0 && rowCounts[i] < 5+i)) {
                emptyCount = 0;
                while (true) {
                    int j = tempColCounter%7;
                    tempColCounter++;
                    if (tempColumnStorage[j] == NULL || tempColumnStorage[j]->card == NULL) {
                        emptyCount++;
                        if (emptyCount >= 7) {
                            break;
                        }
                    }
                    else {
                        // append to column
                        if (columns[i] && columns[i]->card != NULL) {
//                            printf("add from tempcol %d to column %d\n", j, i);
//                            printf("card = N:%d S:%d\n", tempColumnStorage[j]->card->number, tempColumnStorage[j]->card->suit);
                            CardNode *parent = columns[i];
                            while (parent->next) {
                                parent = parent->next;
                            }
                            parent->next = tempColumnStorage[j];
                            tempColumnStorage[j]->prev = parent;
                        }
                        // start a new column
                        else {
//                            printf("add from tempcol %d to column %d [NEW]\n", j, i);
//                            printf("card = N:%d S:%d\n", tempColumnStorage[j]->card->number, tempColumnStorage[j]->card->suit);
                            columns[i] = tempColumnStorage[j];
                            columns[i]->prev = NULL;
                        }
                        rowCounts[i]++;

                        // move temp col one down
                        tempColumnStorage[j] = tempColumnStorage[j]->next;
                        if (tempColumnStorage[j]) {
                            if (tempColumnStorage[j]->prev) {
                                tempColumnStorage[j]->prev->next = NULL;
                            }
                            tempColumnStorage[j]->prev = NULL;
                        }
                        break;
                    }
                }
            }
        }
    }

    // reveal top 5 cards in each column
    for (int i = 0; i < 7; i++) {
        CardNode *tail = columns[i];
        if (tail) {
            while (tail->next) {
                tail = tail->next;
            }
            for (int j = 0; j < 5; j++) {
                tail->card->revealed = true;
                if (tail->prev) {
                    tail = tail->prev;
                }
            }
        }
    }

    return 200;
}

// clears the card storage, if the user loads a new deck or smthn
int clearCardStorage() {
    for (int i = 0; i < 7; i++) {
        // free all the memory! - ignore warning, it's harmless, I think
        while (columnStorage[i] != NULL) {
            if (columnStorage[i]->next) {
                columnStorage[i] = columnStorage[i]->next;
                free(columnStorage[i]->prev->card);
                free(columnStorage[i]->prev);
            }
            else {
                CardNode *cardNode = columnStorage[i];
                columnStorage[i] = NULL;
                free(cardNode->card);
                free(cardNode);
            }
        }
    }
    return 200;
}

// save cards from current game to memory
// takes stored cards and restores them
// create a copy of the columns into the columnStorage
int switchCardStorage() {
    for (int i = 0; i < 7; i++) {
        CardNode *tempColumnPointer = columnStorage[i];
        columnStorage[i] = columns[i];
        columns[i] = NULL;
        if (tempColumnPointer == NULL) {
            CardNode *currentCard = columnStorage[i];
            while (currentCard != NULL) {
                CardNode *newCardNode = malloc(sizeof(CardNode));
                newCardNode->next = NULL;
                newCardNode->prev = NULL;

                Card *newCard = malloc(sizeof(Card));
                newCard->number = currentCard->card->number;
                newCard->suit = currentCard->card->suit;
                newCard->revealed = currentCard->card->revealed;
                newCardNode->card = newCard;

                if (columns[i] == NULL) {
                    columns[i] = newCardNode;
                }
                else {
                    CardNode *parentCard = columns[i];
                    while (parentCard->next) {
                        parentCard = parentCard->next;
                    }
                    parentCard->next = newCardNode;
                    newCardNode->prev = parentCard;
                }

                currentCard = currentCard->next;
            }
        }
        else {
            columns[i] = tempColumnPointer;
        }
    }
    return 200;
}