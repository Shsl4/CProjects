//
// Created by Pierre Juliot on 11/22/2020.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GlobalVariables.h"

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

void initTermAttr(void);
void destTermAttr(void);

void clearInputBuffer(void);
void clearScreen(void);
char waitAndGetInput(void);

void printMainMenu();
void printConfigureMenu();

void showCursor(bool value);

char** makeGrid();
int makeIterationDelay();

void initializeGrid(char** state);
void randomizeGrid(char** state);
void flushStateToGrid();
void randomizeGridVertical(char** state);
void randomizeGridHorizontal(char** state);
void increaseMenu(int* val, int min, int max, string valueName);

int initGame(void);
int loadGame(int pattern);
void drawGame(void);
void updateGame(void);

int isInRange(int x, int y);
int nbCasesAdj(int x, int y);

void printHorizontalLine();
void gameLogic();

bool updateCellState(int x, int y, char** state);
bool isCellAlive(int x, int y);

void resetCursorPosition();
