#include "Definitions.h"
#include <string.h>
#include <stdio.h>

int main() {

    srand(time(NULL));
    rand();

#ifndef WIN32

    initTermAttr();

#endif

    bool runProgram = true;

    printMainMenu();

    while (runProgram){

        switch (waitAndGetInput()) {

            case '1':
                printResultsMenu(deroulerJeu(tempsAleatoire(MAX_RAND_WAIT_TIME), MAX_ATTEMPT_TIME));
                printMainMenu();
                break;
            case '2':
                printMultipleAttemptsMenu();
                printMainMenu();
                break;
            case '3':
                timerCheat = !timerCheat;
                printMainMenu();
                break;
            case '4':
                runProgram = false;
                break;
            case ESCAPE_KEY:
                runProgram = false;
                break;
        }

        clearInputBuffer();

    }

    printf("\nGoodbye!\n\n");

    clearInputBuffer();

    printf("%s", COLOR_RESET);

#ifndef WIN32

    destTermAttr();

#endif

    return 0;

}
