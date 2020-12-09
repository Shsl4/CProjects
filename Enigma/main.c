#include "Definitions.h"

int main(int argc, char** argv) {

#ifndef WIN32

    initTermAttr();

#endif

    initProgram();

#ifndef WIN32

    destTermAttr();

#endif

    return 0;

}
