#pragma once

#include "GlobalVariables.h"
#include <ctype.h>

/*
 * Gives the position of a char in the alphabet
 */
#define alphaPos(c) toupper(c) - 65

int shiftedPos(int c, int r, bool b);

char encrypt(char arg);

void shiftRotor(void);

void initConnexions(void);

int connectCables(void);

bool containsChar(char* ptr, int size, char test);

void Text2Value(void);

void GenerateInvPerm(void);

void flip(char* dest, char a, char b);

void resetShift();

void initProgram(void);

void printMainMenu(void);

void encryptMenu(void);

void cableMenu(void);

void rotorMenu(void);

char* process(char* str);

