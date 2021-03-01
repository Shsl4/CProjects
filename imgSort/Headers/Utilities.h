#pragma once

#ifndef _WIN32

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <unistd.h>

#else

#include <conio.h>
#include <stdarg.h>
#include <io.h>
#include <direct.h>

int asprintf(char** strPtr, const char* format, ...);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include "libbmp.h"

#define bool int
#define true 1
#define false 0

extern bool minimal;

extern const char* compNames[10];
extern int compMethod;
extern int numOuts;

extern const void* sortFuncs[4];
extern const char* funcNames[4];

float quickSort(Pixel* arr, int size);
float bubbleSort(Pixel* arr, int size);
float insertionSort(Pixel* arr, int size);
float selectionSort(Pixel* arr, int size);

int partPixel(Pixel* arr, int from, int to);
void quickSortPixel(Pixel* arr, int from, int to);
void swap(Pixel* a, Pixel* b);
bool compPixel(Pixel* a, Pixel* b);
bool checkSort(Pixel* arr, int size);
void processSort(const void* funcPtr, const char* funcName, Pixel* pixels, int size);
void copyAndProcess(const void* funcPtr, const char* funcName, Image* image, bool writeOutput);

bool doesDirectoryExist(const char* path);