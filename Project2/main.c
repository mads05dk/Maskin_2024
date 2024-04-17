#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "commands.h"  // Import the commands
#include "cli.h"  // The command-line interface
#include "yukon.h"  // The game

int main() {
    // Initialize random with time as seed
    srand(time(NULL));

    CommandNode startCommandNode;
    startCommandNode.command = NONE;
    startCommandNode.status = 200;
    moveHistory = &startCommandNode;
    lastCommand = &startCommandNode;

    while (true) {
        int status = readCommand();
        // QQ break
        if (status == 200 && lastCommand->command == QQ) {
            break;
        }
    }

    return 0;
}
