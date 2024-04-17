#include <stdio.h>
#include <malloc.h>
#include "yukon.h"
#include "cards.h"
#include "io.h"
#include "commands.h"

CardNode *columns[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
CardNode *columnStorage[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
CardNode *foundations[4] = { NULL, NULL, NULL, NULL };

Phase currentPhase = STARTUP;

// clear the deck
int clearDeck() {
    for (int i = 0; i < 4; i++) {
        CardNode *cardNode = foundations[i];
        while (cardNode) {
            CardNode *next = cardNode->next;
            free(cardNode->card);
            free(cardNode);
            cardNode = next;
        }
        foundations[i] = NULL;
    }
    for (int i = 0; i < 7; i++) {
        CardNode *cardNode = columns[i];
        while (cardNode) {
            CardNode *next = cardNode->next;
            // TODO: fix so this free of card doesn't break the code - requires not using AllCards as well
//            free(cardNode->card);
            free(cardNode);
            cardNode = next;
        }
        columns[i] = NULL;
    }
    return 200;
}

// Card struct - bool value for face up or down


// Game phase enum (startup, etc.)

int getCardsFromDeckAsLinkedList(CardNode *cards) {
    int cardNum = 0;
    int columnNums[7] = {0, 0, 0, 0, 0, 0, 0};
    CardNode *currentParent = cards;
    currentParent->prev = NULL;
    currentParent->next = NULL;
    currentParent->card = NULL;
    while (cardNum < 52) {
        for (int i = 0; i < 7; i++) {
            if (cardNum < 52) {
                CardNode *cardNode = columns[i];
                int columnNum = 0;
                while (cardNode->next && columnNum < columnNums[i]) {
                    cardNode = cardNode->next;
                    columnNum++;
                }
                if (columnNum == columnNums[i]) {
                    Card *card = malloc(sizeof(Card));
                    card->revealed = cardNode->card->revealed;
                    card->number = cardNode->card->number;
                    card->suit = cardNode->card->suit;

                    if (!currentParent->card) {
                        currentParent->card = card;
                    }
                    else {
                        CardNode *newNode = malloc(sizeof(CardNode));
                        newNode->prev = currentParent;
                        newNode->prev->next = newNode;
                        newNode->next = NULL;
                        newNode->card = card;
                        currentParent = newNode;
                    }

                    cardNum++;
                    columnNums[i]++;
                }
            }
        }
    }
    return 200;
}

int getCardsFromDeck(Card *cards) {
    int cardNum = 0;
    int columnNums[7] = {0, 0, 0, 0, 0, 0, 0};
    while (cardNum < 52) {
        for (int i = 0; i < 7; i++) {
            if (cardNum < 52) {
                CardNode *cardNode = columns[i];
                int columnNum = 0;
                while (cardNode->next && columnNum < columnNums[i]) {
                    cardNode = cardNode->next;
                    columnNum++;
                }
                if (columnNum == columnNums[i]) {
                    Card *card = malloc(sizeof(Card));
                    card->revealed = cardNode->card->revealed;
                    card->number = cardNode->card->number;
                    card->suit = cardNode->card->suit;
                    cards[cardNum] = *card;
                    cardNum++;
                    columnNums[i]++;
                }
            }
        }
    }
    return 200;
}

int addLinkedListCardsToDeck(CardNode *cards) {
    while (cards) {
        for (int i = 0; i < 7; i++) {
            if (cards) {
                if (!columns[i]) {
                    columns[i] = cards;
                    columns[i]->prev = NULL;
                }
                else {
                    CardNode *next = columns[i]->next;
                    CardNode *prev = columns[i];
                    while (next) {
                        prev = next;
                        next = next->next;
                    }
                    prev->next = cards;
                    cards->prev = prev;
                }

                if (cards->next) {
                    cards = cards->next;
                    cards->prev->next = NULL;
                    cards->prev = NULL;
                }
                else {
                    cards->next = NULL;
                    cards = NULL;
                }
            }
        }
    }
    return 200;
}

int addCardsToDeck(Card *cards) {
    int cardNum = 0;
    while (cardNum < 52) {
        for (int i = 0; i < 7; i++) {
            if (cardNum < 52) {
                CardNode *cardNode = malloc(sizeof(CardNode));
                if (cardNode == NULL) {
                    return -1;
                }
                cardNode->next = NULL;
                cardNode->prev = NULL;
                cardNode->card = &cards[cardNum];
                if (!columns[i]) {
                    columns[i] = cardNode;
                }
                else {
                    CardNode *next = columns[i]->next;
                    CardNode *prev = columns[i];
                    while (next) {
                        prev = next;
                        next = next->next;
                    }
                    prev->next = cardNode;
                    cardNode->prev = prev;
                }
//                printf("Added card to column %d!\n"
//                       "Cardnum: %d\n"
//                       "Head = Num: %d - Suit: %d\n"
//                       "Newcard = Num: %d - Suit: %d\n\n",
//                       i+1, cardNum+1,
//                       columns[i]->card->number, columns[i]->card->suit,
//                       cardNode->card->number, cardNode->card->suit);
                cardNum++;
            }
            else {
                break;
            }
        }
    }

    return 200;
}


// function to check if card from one column can move onto other column
// - get other column, get tail/head of linked list, check requirements in rules

int getUnshuffledDeck() {
    return LDCommand("examples/decks/unshuffled.txt");
}
