//
// Created by Pierre Juliot on 11/29/2020.
//

#include "libSl4sh.h"

#ifndef WIN32

/*
 * Returns the number of bytes waiting in the input buffer.
 */
int kbhit(void)
{
    int bytesWaiting;
    ioctl(0, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

/**
 * Setup linux terminal attributes when program launches.
 */
void initTermAttr(void) {

    // Get term attributes
    struct termios term;
    tcgetattr(0, &term);

    // Disable Canonical input and echoing
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);

}

/**
 * Restores linux terminal attributes when program exits.
 */
void destTermAttr(void) {

    struct termios term;
    tcgetattr(0, &term);

    // Re-enabling Canonical input & echoing (Otherwise no input shows up in the terminal after the process exits)
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);

}

#endif

/**
 * Clears the input buffer.
 */
void clearInputBuffer(void) {
    while (kbhit()) {
        getch();
    }
}

/**
 * Clears the screen using the right OS command.
 */
void clearScreen(void) {

#ifndef WIN32
    system("clear");
#else
    system("cls");
#endif

}

/**
 * Waits for an user keyboard input. This function stalls the program.
 * @return The pressed key
 */
char waitForInput(void) {

    while (!kbhit()) {
    }
    return getch();
}

/**
 * Show / hides the terminal's cursor depending on the OS
 * @param value Show Value
 */
void showCursor(bool value) {

#ifndef WIN32

    printf("\e[?25%c", value ? 'h' : 'l');

#else

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = {100, value};
    SetConsoleCursorInfo(handle, &info);

#endif

}

/**
 * Shows the user a text prompting them to press any key to continue executing the program
 */
void promptContinue(void){

    printf("Press any key to continue...");
    fflush(stdout);
    waitForInput();

}

/**
 * Dynamic menu allowing to modify an int value
 * @param val A pointer to the target integer
 * @param min The minimal allowed value
 * @param max The maximal allowed value
 * @param valueName The parameter's display name
 */
void increaseMenu(int* val, int min, int max, string valueName, string menuTitle){

    clearScreen();

    while (true){

        setCursorPosition(0, 0);

        printf("\n%s\n\n", menuTitle);

        printf("Editing %s value. Current: %d | (Min: %d, Max: %d)%s\n\n", valueName, *val, min, max, SPACER);

        printf("1) Decrease\n");
        printf("2) Increase\n");
        printf("3) Go Back\n");

        switch (waitForInput()) {

            case '1':
                if (*val > min) {
                    *val = *val-1;
                }
                break;

            case '2':

                if (*val < max) {
                    *val = *val+1;
                }
                break;

            case '3':
                return;

            case ESCAPE_KEY:
                return;

        }

        clearInputBuffer();

    }

}

void setCursorPosition(int line, int col) {

    printf( "\033[%d;%dH", line, col);

}

#ifdef LIBS4_TIME

/**
 * Creates a delay that hangs the current process for seconds. If an escape key is hit, the delay is cancelled.
 * @param seconds The time in seconds to delay
 * @return -1 if the delay was cancelled, otherwise 0
 */
int makeCancelableDelay(double seconds) {

    double waitTime = seconds;
    double elapsedTime = 0;
    clock_t timeStamp = clock();

    while (elapsedTime < waitTime) {

        elapsedTime = timeDiff(timeStamp, clock());

        if(kbhit() && getch() == ESCAPE_KEY){

            return -1;

        }

    }

    return 0;

}

/**
 * Creates a delay that hangs the current process for seconds. There is no way to stop the delay.
 * @param seconds The time in seconds to delay
 */
void makeDelay(double seconds) {

    double waitTime = seconds;
    double elapsedTime = 0;
    clock_t timeStamp = clock();

    while (elapsedTime < waitTime) {
        elapsedTime = timeDiff(timeStamp, clock());
    }

}

#endif


