#include "Definitions.h"

int main() {

    srand(time(NULL));
    rand();

#ifndef WIN32

    initTermAttr();

#endif

    showCursor(false);

    int returnValue = initGame();

#ifndef WIN32

    destTermAttr();

#endif

    showCursor(true);

    return returnValue;

}
