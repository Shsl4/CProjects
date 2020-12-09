//
// Created by Pierre Juliot on 11/22/2020.
//

#include <stdlib.h>
#include <stdbool.h>

typedef char* string;

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define GRID_WIDTH config.gridWidth
#define GRID_HEIGHT config.gridHeight

#define ESCAPE_KEY 27

#define DEAD_CELL ' '
#define LIVING_CELL '#'
#define SPACER "                                                                    "

#define randDouble(min, max) rand() / (double)RAND_MAX * ( (max) - (min) ) + min
#define randInt(min, max) ((rand() % ((max) - (min) + 1)) + (min))

#define timeDiff(first, last) ((double)((last) - (first)) / CLOCKS_PER_SEC)

extern int lastWindowX;
extern int lastWindowY;

enum ProgressMode{

    MANUAL,
    AUTOMATIC

};

struct GOLConfig
{

    int pattern;
    int gridHeight;
    int gridWidth;
    enum ProgressMode mode;
    int speed;
    int maxIterations;
    string color;

};

extern char** grid;
extern char** gridState;
extern bool isGameRunning;

const extern string appTitle;
const extern string configureTitle;
const extern string editTitle;

extern struct GOLConfig config;
extern int iterations;

string getPatternName(int pattern);
