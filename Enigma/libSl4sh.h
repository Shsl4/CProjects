//
// Created by Pierre Juliot on 11/29/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef LIBS4_TIME

#include <time.h>

#endif

/**
 * J’ai bien rédigé moi-même l'intégralité des fonctions présentes ici.
 */

typedef char* string;

#ifndef WIN32

#include <termios.h>
#include <sys/ioctl.h>

int kbhit(void);
#define getch() getchar()

void initTermAttr(void);
void destTermAttr(void);

#else

#include <conio.h>
#include <windows.h>
#define kbhit() _kbhit()
#define getch() _getch()

#endif

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define randDouble(min, max) rand() / (double)RAND_MAX * ( (max) - (min) ) + min
#define randFloat(min, max) rand() / (float)RAND_MAX * ( (max) - (min) ) + min
#define randInt(min, max) ((rand() % ((max) - (min) + 1)) + (min))

#define ESCAPE_KEY 27
#define SPACER "                                                                    "

void clearInputBuffer(void);

void clearScreen(void);

char waitForInput(void);

void showCursor(bool value);

void promptContinue(void);

void increaseMenu(int* val, int min, int max, string valueName, string menuTitle);

void setCursorPosition(int line, int col);

#ifdef LIBS4_TIME

#define timeDiff(first, last) ((double)((last) - (first)) / CLOCKS_PER_SEC)

void makeDelay(double seconds);

int makeCancelableDelay(double seconds);

#endif


