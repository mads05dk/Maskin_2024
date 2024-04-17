//
// IO Operations - saving and loading
//

#include <sys/stat.h>
#include <malloc.h>
#include "io.h"
#include "cli.h"

// validate whether the file is open
// file is the output of function
int validateFile(char *fileName, char *fileMode, FileType fileType, FILE** file) {
    if ((*file = fopen(fileName, fileMode)) == NULL) {
        // printf("cannot open file with filename '%s'!\n", fileName);
        return 403;
    }

    struct stat sb;
    if (stat(fileName, &sb) == -1) {
        return 403;
    }

    // validate the file if the mode is not write
    if (!strcmp(fileMode, "w")) {
        char *file_contents = (char *) malloc(sb.st_size);
        switch (fileType) {
            case CARDS:
                while (fgets(file_contents, sizeof(file_contents), *file) != NULL) {
                    Card *card = NULL;
                    // validate if the line is a card
                    int gotCard = getCardFromName(file_contents, card);

                    if (gotCard != 200) {
                        free(file_contents);
                        if (gotCard == 400) {
                            return 404;
                        }
                        return gotCard;
                    }
                }
                break;
            case STATE:
                printf("NO VALIDATION FOR STATE");
                break;
            default:
                printf("what the fuck is this filetype??\n");
                return -1;
        }
        free(file_contents);
    }
    rewind(*file);

    return 200;
}

int saveCards(char *fileName) {
    // get and validate file
    FILE *file;
    int fileValid = validateFile(fileName, "w", CARDS, &file);
    if (fileValid != 200) {
        return fileValid;
    }

    // --------- Add all cards from the deck to the file -------- //
    // thanks to my own drawCards function :)
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
                    char cardName[] = "  ";
                    getCardName(cardColumns[j]->card, cardName, true);
                    fprintf(file, "%s\n", cardName);
                    if (cardColumns[j]->next != NULL) {
                        cardColumns[j] = cardColumns[j]->next;
                    } else {
                        activeColumns[j] = false;
                        inactiveCount++;
                    }
                } else {
                    activeColumns[j] = false;
                    inactiveCount++;
                }
            }
        }
        if (inactiveCount >= 7) {
            break;
        }
    }

    fclose(file);
    return 200;
}

int loadCards(char *fileName, Card *cards) {
    // get and validate file
    FILE *file;
    int fileValid = validateFile(fileName, "r", CARDS, &file);

    if (fileValid != 200) {
        fclose(file);
        return fileValid;
    }

    // load cards from file into cards array
    struct stat sb;
    stat(fileName, &sb);
//    char file_contents[100];
    char *file_contents = malloc(sb.st_size);

    for (int i = 0; i < 52;) {
        // (fscanf(file, "%[^\n] ", &file_contents) == EOF)
        if (fgets(file_contents, sizeof(file_contents), file) == NULL) {
            free(file_contents);
            fclose(file);
            return -1;
        }

        // if not newline
        if (file_contents[0] != '\n') {
            // validate if the line is a card
            int gotCard = getCardFromName(file_contents, &cards[i]);

            if (gotCard != 200) {
                if (gotCard == 400) {
                    free(file_contents);
                    fclose(file);
                    return 404;
                }
                else {
                    printf("DEBUG: Error reading card - code: %d\n", gotCard);
                }
            }
            else {
                cards[i].revealed = false;
                i++;
            }
        }
    }

    free(file_contents);

    fclose(file);
    return 200;
}

int saveState(char *fileName) {
    // get and validate file
    FILE *file = NULL;
    int fileValid = validateFile(fileName, "w", STATE, &file);
    if (fileValid != 200) {
        return fileValid;
    }

    // TODO: save the state

    fclose(file);
    return 200;
}

int loadState(char *fileName) {
    // get and validate file
    FILE *file = NULL;
    int fileValid = validateFile(fileName, "r", STATE, &file);
    if (fileValid != 200) {
        return fileValid;
    }

    // TODO: load the state

    // load state from state file into the columns and foundations
    fclose(file);
    return 200;
}
