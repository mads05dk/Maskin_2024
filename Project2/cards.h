#ifndef MACHINE_ORIENTED_PROGRAMMING_PROJECT_2_CARDS_H
#define MACHINE_ORIENTED_PROGRAMMING_PROJECT_2_CARDS_H

#include "yukon.h"

extern Card allCards[53];

int shuffleCards();

// convert the startup deck to a playing deck
int convertStartupToPlay();

int clearCardStorage();

// save cards from current game to memory
// takes stored cards and restores them
int switchCardStorage();

#endif //MACHINE_ORIENTED_PROGRAMMING_PROJECT_2_CARDS_H
