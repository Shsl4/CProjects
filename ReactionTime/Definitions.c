//
// Created by Pierre Juliot on 11/16/2020.
//

#include "Definitions.h"

#ifndef WIN32

int kbhit(void)
{
    int bytesWaiting;
    ioctl(0, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

void initTermAttr(void){

    // Get term attributes
    struct termios term;
    tcgetattr(0, &term);

    // Disable Canonical input and echoing
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);

    // Hides cursor
    printf("\e[?25l");

}

void destTermAttr(void){

    struct termios term;
    tcgetattr(0, &term);

    // Re-enabling Canonical input & echoing (Otherwise no input shows up in the terminal after the process exited)
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);

    // Shows cursor
    printf("\e[?25h");

}

#else

#ifdef WIN32

int asprintf(char** strPtr, const char* format, ...){

    if(strPtr == NULL) { return -1; }

    va_list list;
    va_start(list, format);

    int stringLength = _vscprintf(format, list);
    if(stringLength < 0) { return -1; }

    size_t size = (size_t)stringLength + 1;
    *strPtr = malloc(size);

    int errorCode = vsprintf_s(*strPtr, stringLength + 1, format, list);
    if(errorCode == -1) { free(*strPtr); return -1; }

    va_end(list);

    return errorCode;

}

#endif

#endif

int compteARebours (int duree){

    double waitTime = randDouble(1, duree);
    double elapsedTime = 0;
    clock_t timeStamp = clock();

    const double delay = 0.1;
    double nextDelay = delay;

    while (elapsedTime < waitTime){

        elapsedTime = timeDiff(timeStamp, clock());

        if(timerCheat){

            char* formatted;
            asprintf(&formatted, "%.2fs", waitTime - elapsedTime);
            printf("Time: %s\r", formatted);
            free(formatted);

        }
        else{

            if(nextDelay < elapsedTime){
                printf("_");
                fflush(stdout);
                nextDelay += delay;
            }

        }

        if(kbhit()){

            printf("%c[2K\r", 27);
            return -1;

        }

    }

    return 0;

}

double tempsAleatoire (int max){

    return randDouble(1, max);

}

double deroulerJeu (int duree, int max){

    return makeInstance(duree, max, 1, 1);

}

double makeInstance(int duree, int max, int currentAttempt, int maxAttempts) {

    clearScreen();

    if(maxAttempts > 1){
        printf("Attempt %d/%d\n", currentAttempt , maxAttempts);
    }

    printf("\n%s\n", getReadyTitle);

    clearInputBuffer();

    if(compteARebours(duree) == -1) { printf("Too early!\n"); return -2; }

    clock_t timeStamp = clock();

    return waitForReaction(max, currentAttempt, maxAttempts) == 0 ? timeDiff(timeStamp, clock()) : -1;
}

void clearInputBuffer(void) {
    while (kbhit()){
        getch();
    }
}

int waitForReaction(int max, int currentAttempt, int maxAttempts) {

    clearScreen();

    if(maxAttempts > 1){
        printf("Attempt %d/%d\n", currentAttempt , maxAttempts);
    }

    printf("\n%s\n", nowTitle);

    double waitTime = max;
    double elapsedTime = 0.0;

    const double delay = (max)/50.0;
    double nextDelay = delay;

    clock_t timeStamp = clock();

    while (elapsedTime < waitTime){

        elapsedTime = timeDiff(timeStamp, clock());

        if(nextDelay < elapsedTime){
            printf(".");
            fflush(stdout);
            nextDelay += delay;
        }

        if(kbhit()){
            printf("\n");
            clearInputBuffer();
            return 0;
        }

    }

    printf("\n");
    return -1;

}

char waitAndGetInput(void) {
    while(!kbhit()){
    }
    return getch();
}

void clearScreen(void) {
#ifdef WIN32
    system("cls");
#else
    system("clear");
#endif
}

char promptContinue(void) {
    return waitWithText("Press any key to continue...");
}

char waitWithText(string text) {
    clearInputBuffer();
    printf("\n%s\n", text);
    return waitAndGetInput();
}

void printMainMenu(void) {

    clearScreen();

    printf("%s", COLOR_YELLOW);
    printf("\n%s\n\n", reactionTimeTitle);
    printf("Welcome to ReactionTime! Test your reaction time through a rather short, but fun test!\n\n");
    printf("1) Single Attempt\n");
    printf("2) Multiple Attempts\n");
    printf("3) %s timer cheat\n", timerCheat ? "Disable" : "Enable");
    printf("4) Exit\n");

}

void printMultipleAttemptsMenu(void) {

    clearScreen();

    int amount = 5;

    printf("\n%s\n\n", multipleAttemptsTitle);

    printf("In multiple attempts mode, your reaction time will be tested multiple times in a row. \nYou will then get your average reaction time!\n\n");
    printf("1) Begin\n");
    printf("2) Decrease Attempts\n");
    printf("3) Increase attempts\n");
    printf("4) Cancel\n\n");

    printf("Attempt amount: %d \r", amount);
    fflush(stdout);

    while (true){

        switch (waitAndGetInput()) {

            case '1':
                clearScreen();
                makeMultipleAttempts(amount);
                return;
            case '2':
                if(amount > 2){
                    amount--;
                    printf("Attempt amount: %d  \r", amount);
                    fflush(stdout);
                }
                break;
            case '3':
                if(amount < 50){
                    amount++;
                    printf("Attempt amount: %d  \r", amount);
                    fflush(stdout);
                }
                break;
            case '4':
                return;
            case ESCAPE_KEY:
                return;
        }

        clearInputBuffer();

    }

}

void printResultsMenu(double time) {

    if(time == -1.0f) { printf("\n%s\n", badTimeAnswers[randInt(0, 8)]); promptContinue(); return; }
    else if(time == -2.0f) { promptContinue(); return; }

    clearScreen();

    printf("\n%s\n", resultsTitle);

    printf("\nYour reaction time: %.2fs. ", time);

    if(time > 1){
        printf("%s\n", badTimeAnswers[randInt(0, 8)]);
    }
    else if(time >= 0.25){
        printf("%s\n", okTimeAnswers[randInt(0, 8)]);
    }
    else{
        printf("%s\n", goodTimeAnswers[randInt(0, 8)]);
    }

    promptContinue();

}

void makeMultipleAttempts(int count) {

    double totalTime = 0.0f;

    for (int i = 0; i < count; ++i) {

        double result = makeInstance(tempsAleatoire(MAX_RAND_WAIT_TIME), MAX_ATTEMPT_TIME, i + 1, count);

        if(result <= -1){
            if(result == -1){
                printf("\nYou need to press a key before the time runs out.\n");
            }
            i--;
        }
        else{
            totalTime += result;
        }

        char charRes = waitWithText("Press any key to continue or Esc to exit...");

        if(charRes == ESCAPE_KEY){
            return;
        }

        clearScreen();


    }

    printResultsMenu(totalTime / count);

}


