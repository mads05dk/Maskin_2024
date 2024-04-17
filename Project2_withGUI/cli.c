#include <stdio.h>
#include <string.h>
#include "cli.h"
#include "commands.h"
#include "yukon.h"
#include "cards.h"
#include <malloc.h>

int getCardName(Card *card, char *cardName, bool reveal) {
    if (card && (card->revealed || reveal)) {
        switch (card->number) {
            case 1:
                cardName[0] = 'A';
                break;
            case 10:
                cardName[0] = 'T';
                break;
            case 11:
                cardName[0] = 'J';
                break;
            case 12:
                cardName[0] = 'Q';
                break;
            case 13:
                cardName[0] = 'K';
                break;
            default:
                cardName[0] = card->number + '0';
                break;
        }
        switch (card->suit) {
            case CLUBS:
                cardName[1] = 'C';
                break;
            case DIAMONDS:
                cardName[1] = 'D';
                break;
            case HEARTS:
                cardName[1] = 'H';
                break;
            case SPADES:
                cardName[1] = 'S';
                break;
        }
    }
    return 200;
};

int getCardFromName(char *cardName, Card *card) {
    if (cardName == NULL) {
        return 400;
    }

//    printf("%s\n", cardName);
//    printf("cardName[0]: %c\n", cardName[0]);

    switch (cardName[0]) {
        case 'A':
            card->number = 1;
            break;
        case 'T':
            card->number = 10;
            break;
        case 'J':
            card->number = 11;
            break;
        case 'Q':
            card->number = 12;
            break;
        case 'K':
            card->number = 13;
            break;
        default:
            if (cardName[0] >= '0' && cardName[0] <= '9') {
                card->number = atoi(&cardName[0]);
            }
            else {
                return 400;
            }
            break;
    }
//    printf("cardName[1]: %c\n", cardName[1]);

    switch (cardName[1]) {
        case 'C':
            card->suit = CLUBS;
            break;
        case 'D':
            card->suit = DIAMONDS;
            break;
        case 'H':
            card->suit = HEARTS;
            break;
        case 'S':
            card->suit = SPADES;
            break;
        default:
            return 400;
    }

    return 200;
}

int drawCards(bool reveal) {
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n");
    printf("\n");
    bool activeColumns[7] = {true, true, true, true, true, true, true};

    CardNode *cardColumns[7];
    for (int i = 0; i < 7; i++) {
        cardColumns[i] = columns[i];
    }

    // keep looping until all columns are done printing
    int inactiveCount = 0;
    for (int i = 0; true; i++) {
        // loop through each column and foundation
        for (int j = 0; j < 7; j++) {
            // print the columns
            if (activeColumns[j]) {
                // check if the current item in the column is not null
                if (cardColumns[j] && cardColumns[j]->card) {
                    char cardName[] = "[]";
                    getCardName(cardColumns[j]->card, cardName, reveal);
                    printf("%s\t", cardName);
                    if (cardColumns[j]->next != NULL) {
                        cardColumns[j] = cardColumns[j]->next;
                    } else {
                        activeColumns[j] = false;
                        inactiveCount++;
                    }
                } else {
                    activeColumns[j] = false;
                    inactiveCount++;
                    printf("  \t");
                }
            } else {
                printf("  \t");
            }
        }
        // print the foundations
        if (i < 4) {
            char foundationCard[] = "[]";
            if (currentPhase != STARTUP && foundations[i] && foundations[i]->card) {
                getCardName(foundations[i]->card, foundationCard, reveal);
            }
            printf("\t%s\tF%d\n", foundationCard, i + 1);
        } else {
            printf("\n");
            if (inactiveCount >= 7) {
                break;
            }
        }

    }
    return 200;
};

int readCommand() {
    // mads please fix my retardedness
    // fixed, good old stackoverflow uWu https://stackoverflow.com/questions/32313150/array-type-char-is-not-assignable
    char lastCommandName[11];
    switch (lastCommand->command) {
        case LD:
            strcpy(lastCommandName, "LD");
            break;
        case SW:
            strcpy(lastCommandName, "SW");
            break;
        case SI:
            strcpy(lastCommandName, "SI");
            break;
        case SR:
            strcpy(lastCommandName, "SR");
            break;
        case SD:
            strcpy(lastCommandName, "SD");
            break;
        case QQ:
            strcpy(lastCommandName, "QQ");
            break;
        case P:
            strcpy(lastCommandName, "P");
            break;
        case Q:
            strcpy(lastCommandName, "Q");
            break;
        case MOVE:
            strcpy(lastCommandName, moveHistory->commandData);
            break;
        case U:
            strcpy(lastCommandName, "U");
            break;
        case R:
            strcpy(lastCommandName, "R");
            break;
        case S:
            strcpy(lastCommandName, "S");
            break;
        case L:
            strcpy(lastCommandName, "L");
            break;
        default:
            strcpy(lastCommandName, "");
            break;
    }

    if (lastCommand->command != SW) {
        drawCards(false);
    }
    printf("\n");

    printf("Last Command: %s\n", lastCommandName);
    char status[45];
    char unknownStatusFormat[] = "UNKNOWN ERROR (Code: %d)";
    switch (lastCommand->status) {
        case 200:
            strcpy(status, "OK");
            break;
        case 400:
            strcpy(status, "Bad Request");
            break;
        case 401:
            strcpy(status, "Invalid Target");
            break;
        case 402:
            strcpy(status, "Illegal Move");
            break;
        case 403:
            strcpy(status, "IO Error");
            break;
        case 404:
            strcpy(status, "File Syntax Error");
            break;
        case 405:
            strcpy(status, "Command only available in the STARTUP phase!");
            break;
        case 406:
            strcpy(status, "Command only available in the PLAY phase!");
            break;
        case 500:
            strcpy(status, "Internal Error");
            break;
        case 501:
            strcpy(status, "Not Implemented");
            break;
        default:
            sprintf(status, unknownStatusFormat, lastCommand->status);
            break;
    }
    printf("Message: %s\n", status);

    printf("INPUT > ");
    fflush(stdout);
    char command[110];
    fgets(command, 110, stdin);
    printf("\n");

    // remove trailing newline, if existing
    if ((strlen(command) > 0) && (command[strlen(command) - 1] == '\n')) {
        command[strlen(command) - 1] = '\0';
    }

    CommandNode commandNode = {.command = NONE};
    int statusCode = -1;
    if (strstr(command, "LD") != NULL) {
        commandNode.command = LD;
        if (currentPhase == STARTUP) {
            if (strstr(command, " ") != NULL) {
                char *arg = strtok(command, " ");
                char path[100];
                while (arg != NULL) {
                    strcpy(path, arg);
                    arg = strtok(NULL, " ");
                }
                statusCode = LDCommand(path);
            } else { statusCode = LDCommand(NULL); }
        } else {
            statusCode = 405;
        }

    } else if (strstr(command, "SW") != NULL) {
        commandNode.command = SW;
        statusCode = SWCommand();
    } else if (strstr(command, "SI") != NULL) {
        commandNode.command = SI;
        //TODO fix this at some point idk when, but please let anyone but me fix it :)
        if (currentPhase == STARTUP) {
            if (strstr(command, " ") != NULL) {
                char *arg = strtok(command, " ");
                int integer;
                while (arg != NULL) {
                    sscanf(arg, "%d", &integer);
                    arg = strtok(NULL, " ");
                }
                statusCode = SICommand(integer);
            } else { statusCode = SICommand(52/2); }

        } else {
            statusCode = 405;
        }
    } else if (strstr(command, "SR") != NULL) {
        commandNode.command = SR;
        if (currentPhase == STARTUP) {
            statusCode = SRCommand();
        } else {
            statusCode = 405;
        }
    } else if (strstr(command, "SD") != NULL) {
        commandNode.command = SD;
        if (currentPhase == STARTUP) {
            if (strstr(command, " ") != NULL) {
                char *arg = strtok(command, " ");
                char path[100];
                while (arg != NULL) {

                    strcpy(path, arg);
                    arg = strtok(NULL, " ");
                }
                statusCode = SDCommand(path);
            } else { statusCode = SDCommand(NULL); }
        } else {
            statusCode = 405;
        }
    } else if (strstr(command, "QQ") != NULL) {
        commandNode.command = QQ;
        if (currentPhase == STARTUP) {
            statusCode = QQCommand();
        } else {
            statusCode = 405;
        }
    } else if (strstr(command, "P") != NULL) {
        commandNode.command = P;
        if (currentPhase == STARTUP) {
            statusCode = PCommand();
        } else {
            statusCode = 405;
        }
    } else if (strstr(command, "Q") != NULL) {
        commandNode.command = Q;
        if (currentPhase == PLAY) {
            statusCode = QCommand();
        } else {
            statusCode = 406;
        }
    // Move command
    } else if (strchr(command, '-') != NULL && strchr(command, '>') != NULL) {
        commandNode.command = MOVE;
        if (currentPhase == PLAY) {
            bool force = false;
            if (strstr(command, "!!") != NULL) {
                force = true;
            }

            if (strchr(command, ':') != NULL) {
                statusCode = MCommand(command, false, force);
            }
            else {
                statusCode = MCommand(command, true, force);
            }

            commandNode.commandData = command;
            commandNode.prev = moveHistory;
            moveHistory->next = &commandNode;
            moveHistory = &commandNode;
        } else {
            statusCode = 406;
        }
    } else if (strstr(command, "U") != NULL) {
        commandNode.command = U;
        if (currentPhase == PLAY) {
            statusCode = UCommand();
        } else {
            statusCode = 406;
        }
    } else if (strstr(command, "R") != NULL) {
        commandNode.command = R;
        if (currentPhase == PLAY) {
            statusCode = RCommand();
        } else {
            statusCode = 406;
        }
    } else if (strstr(command, "S") != NULL) {
        commandNode.command = S;
        if (currentPhase == PLAY) {
            if (strstr(command, " ") != NULL) {
                char *arg = strtok(command, " ");
                char path[100];
                while (arg != NULL) {
                    strcpy(path, arg);
                    arg = strtok(NULL, " ");
                }
                statusCode = SCommand(path);
            } else { statusCode = SCommand(NULL); }
        } else {
            statusCode = 406;
        }
    } else if (strstr(command, "L") != NULL) {
        commandNode.command = L;
        if (currentPhase == PLAY) {
            if (strstr(command, " ") != NULL) {
                char *arg = strtok(command, " ");
                char path[100];
                while (arg != NULL) {
                    strcpy(path, arg);
                    arg = strtok(NULL, " ");
                }
                statusCode = LCommand(path);
            }
        } else {
            statusCode = 406;
        }
    } else if (strstr(command, "DEBUG") != NULL) {
        switchCardStorage();
    }
    else {
        statusCode = 400;
    }

    // Add command to command history
    if (commandNode.command != NONE) {
        commandNode.status = statusCode;
        lastCommand = &commandNode;
    }
    else {
        lastCommand->status = statusCode;
    }

    return statusCode;
};