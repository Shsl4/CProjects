#pragma once

#include "libSl4sh.h"

const extern string enigmaTitle;
const extern string processTitle;
const extern string editTitle;
const extern string rotorTitle;
const extern string cableTitle;

extern char connexions[26];
extern char textRotor[3][26];
extern int rotor[3][26];
extern int invRotor[3][26];

extern char textReflector[26];
extern int reflector[26];

extern int shift[3];
extern char flippedChars[12];
extern int cableAmount;

extern char textInput[256];
extern int initShift[3];
