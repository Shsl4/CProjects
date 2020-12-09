//
// Created by Pierre Juliot on 11/16/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GlobalVariables.h"

#ifndef WIN32

#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

int kbhit(void);
#define getch() getchar()

void initTermAttr(void);
void destTermAttr(void);

#else

#include <conio.h>
#include <stdarg.h>
#define kbhit() _kbhit()
#define getch() _getch()

int asprintf(char** strPtr, const char* format, ...);

#endif

void clearInputBuffer(void);
void clearScreen(void);

char waitWithText(string text);
char promptContinue(void);

double makeInstance(int duree, int max, int currentAttempt, int maxAttempts);
int waitForReaction(int max, int currentAttempt, int maxAttempts);
char waitAndGetInput(void);
void makeMultipleAttempts(int count);

void printMultipleAttemptsMenu(void);
void printResultsMenu(double time);
void printMainMenu(void);

int compteARebours (int duree);
double tempsAleatoire (int max);
double deroulerJeu (int duree, int max);