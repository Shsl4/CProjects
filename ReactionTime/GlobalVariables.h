//
// Created by Pierre Juliot on 11/21/2020.
//

#include <stdbool.h>

typedef char* string;

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define randDouble(min, max) rand() / (double)RAND_MAX * ( (max) - (min) ) + min
#define randInt(min, max) ((rand() % ((max) - (min) + 1)) + (min))

#define timeDiff(first, last) ((double)((last) - (first)) / CLOCKS_PER_SEC)

#define MAX_ATTEMPT_TIME 1
#define MAX_RAND_WAIT_TIME 7
#define ESCAPE_KEY 27

const extern string badTimeAnswers[];
const extern string okTimeAnswers[];
const extern string goodTimeAnswers[];
const extern string reactionTimeTitle;
const extern string getReadyTitle;
const extern string nowTitle;
const extern string resultsTitle;
const extern string multipleAttemptsTitle;

extern bool timerCheat;


