//
// Created by Pierre Juliot on 11/22/2020.
//

#include "Definitions.h"

#ifndef WIN32

/*
 * Returns the number of bytes waiting in the input buffer.
 */
int kbhit(void)
{
    int bytesWaiting;
    ioctl(0, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

/**
 * Setup linux terminal attributes when program launches.
 */
void initTermAttr(void) {

    // Get term attributes
    struct termios term;
    tcgetattr(0, &term);

    // Disable Canonical input and echoing
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);

}

/**
 * Restores linux terminal attributes when program exits.
 */
void destTermAttr(void) {

    struct termios term;
    tcgetattr(0, &term);

    // Re-enabling Canonical input & echoing (Otherwise no input shows up in the terminal after the process exits)
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);

}

#endif

/**
 * Clears the input buffer.
 */
void clearInputBuffer(void) {
    while (kbhit()) {
        getch();
    }
}

/**
 * Clears the screen using the right OS command.
 */
void clearScreen(void) {

#ifndef WIN32
    system("clear");
#else
    system("cls");
#endif

}

/**
 * Waits for an user keyboard input. This functions stalls the program.
 * @return The pressed key
 */
char waitAndGetInput(void) {

    while (!kbhit()) {
    }
    return getch();
}

/**
 * Manages the main menu. Function and program quits when isGameRunning is set to false.
 * @return 0 (no possible error code, potential exceptions will crash the program)
 */
int initGame(void) {

    bool runProgram = true;
    bool runProgram = true;

    printMainMenu();

    while (runProgram){

        isGameRunning = false;

        switch (waitAndGetInput()) {

            case '1':
                gameLogic();
                printMainMenu();
                break;
            case '2':
                printConfigureMenu();
                printMainMenu();
                break;
            case '3':
                runProgram = false;
                break;
            case ESCAPE_KEY:
                runProgram = false;
                break;
        }

    }

    printf("\nGoodbye!\n\n");

    clearInputBuffer();

    printf("%s", COLOR_RESET);

    return 0;
}

/**
 * Draws the grid and handles automatic update or user input depending on the configuration.
 */
void gameLogic() {

    iterations = 0;
    isGameRunning = true;

    clearScreen();
    resetCursorPosition(0);

    loadGame(config.pattern);
    drawGame();

    while (true){

        if(config.mode == AUTOMATIC && config.maxIterations != -1){
            printf("\nIterations: %d/%d\n", iterations, config.maxIterations);
        }
        else{
            printf("\nIterations: %d\n", iterations);
        }

        printf("Press Esc to go back to the main menu.");
        fflush(stdout);

        if(config.mode == AUTOMATIC){

            if(iterations < config.maxIterations || config.maxIterations == -1){

                if(makeIterationDelay() == -1){
                    return;
                }

                iterations++;
                updateGame();
                drawGame();

            }
            else {

                waitAndGetInput();
                return;

            }

        }
        else if(config.mode == MANUAL){

            printf(" Press any other key to iterate or hold a key to fast forward.");
            fflush(stdout);
            if(waitAndGetInput() == ESCAPE_KEY) { return; }

            iterations++;
            updateGame();
            drawGame();

        }

    }

}

/**
 * Called on game initialization. Dynamically allocates grids.
 * @param pattern The game's pattern
 * @return the selected pattern or -1 if it is invalid
 */
int loadGame(int pattern) {

    grid = makeGrid();
    initializeGrid(grid);

    gridState = makeGrid();
    initializeGrid(gridState);

    int x = GRID_WIDTH / 2 - 1;
    int y = GRID_HEIGHT / 2 - 1;

    switch (pattern) {

        case 0:
            //Line
            grid[y][x] = LIVING_CELL;
            grid[y][x+1] = LIVING_CELL;
            grid[y][x+2] = LIVING_CELL;
            return pattern;
        case 1:
            // U
            grid[y+2][x] = LIVING_CELL;
            grid[y+2][x+1] = LIVING_CELL;
            grid[y+2][x+2] = LIVING_CELL;
            grid[y+1][x] = LIVING_CELL;
            grid[y+1][x+2] = LIVING_CELL;
            grid[y][x] = LIVING_CELL;
            grid[y][x+2] = LIVING_CELL;
            return pattern;
        case 2:
            // Vessel
            grid[y][x] = LIVING_CELL;
            grid[y][x+1] = LIVING_CELL;
            grid[y][x+2] = LIVING_CELL;
            grid[y+1][x+2] = LIVING_CELL;
            grid[y+2][x+1] = LIVING_CELL;
            return pattern;
        case 3:
            randomizeGrid(grid);
            return pattern;
        case 4:
            randomizeGridVertical(grid);
            return pattern;
        case 5:
            randomizeGridHorizontal(grid);
            return pattern;
    }

    return -1;

}

/**
 * Draws the grid on screen with formatting
 */
void drawGame(void) {

    printHorizontalLine();

    for (int y = 0; y < GRID_HEIGHT; y++) {

        printf("|%.*s|\n", GRID_WIDTH, grid[y]);

    }

    printHorizontalLine();

    fflush(stdout);

}

/**
 * Updates the grid to the next state.
 */
void updateGame(void) {

    resetCursorPosition(0);

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {

            // Save modifications on a separate grid
            updateCellState(x, y, gridState);

        }
    }

    // Then flush the modified state into the main grid
    flushStateToGrid();

}

/**
 * Copies the modified grid state back into the main grid
 */
void flushStateToGrid() {

    for (int y = 0; y < GRID_HEIGHT; y++) {

        for (int x = 0; x < GRID_WIDTH; x++) {

            grid[y][x] = gridState[y][x];

        }

    }

}

/**
 * Prints an horizontal --- line based on the grid's width. Used for grid drawing.
 */
void printHorizontalLine() {

    for (int x = 0; x < GRID_WIDTH + 2; x++) {
        printf("-");
    }

    printf("\n");

}

/**
 * Checks whether the given coordinates represents a cell.
 * @param x The cell's x position
 * @param y The cell's y position
 * @return 1 if the cell exists, otherwise 0
 */
int isInRange(int x, int y) {

    return !(x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT);

}

/**
 * Returns an integer value representing the number of living cells neighboring the cell at the specified coordinates.
 * @param x The cell's x position
 * @param y The cell's y position
 * @return The number of living adjacent cells
 */
int nbCasesAdj(int x, int y) {

    int count = 0;
    int testVals[8][2] = {{x - 1, y},
                          {x - 1, y - 1},
                          {x - 1, y + 1},
                          {x + 1, y},
                          {x + 1, y - 1},
                          {x + 1, y + 1},
                          {x, y + 1},
                          {x, y - 1}};

    for (int i = 0; i < 8; ++i) {

        if(isCellAlive(testVals[i][0], testVals[i][1]))
            count++;

    }

    return count;

}

/**
 * Check whether a cell is alive
 * @param x The cell's x position
 * @param y The cell's y position
 * @return Whether the cell exists and is alive
 */
bool isCellAlive(int x, int y) {

    return isInRange(x, y) && grid[y][x] == LIVING_CELL;

}

/**
 * This function is used clear the screen each time the grid is drawn. Calls ClearScreen if the window is too small, or when the window's size changed.
 * This avoids visual artifacts like text ending up a bit everywhere on the screen when the window is resized. Otherwise, draw the grid
 * on top of the previous one, preventing a black flash from appearing on the screen (calling clear causes black flashes)
 */
void resetCursorPosition() {

#ifndef WIN32

    //Get terminal screen size
    struct winsize size;
    ioctl(0, TIOCGWINSZ, &size);

    if(lastWindowX != size.ws_col || lastWindowY != size.ws_row || (isGameRunning && (size.ws_row < GRID_HEIGHT + 5 || size.ws_col < GRID_WIDTH + 2))){

        clearScreen();

    }
    else{

        printf( "\033[0H");

    };

    lastWindowX = size.ws_col;
    lastWindowY = size.ws_row;

#else

    struct _CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

    if(lastWindowX != info.dwSize.X || lastWindowY != info.dwSize.Y || (isGameRunning && (info.dwSize.Y <= GRID_HEIGHT + 5 || info.dwSize.X <= GRID_WIDTH + 2))){

        clearScreen();

    }
    else{

        printf( "\033[0H");

    }

    lastWindowX = info.dwSize.X;
    lastWindowY = info.dwSize.Y;

#endif

}

/**
 * Dynamically allocates a new grid depending on the configuration
 * @return The new grid
 */
char **makeGrid() {

    char** ptr = calloc(GRID_HEIGHT, sizeof(char*));

    for (int i = 0; i < GRID_HEIGHT; ++i) {
        ptr[i] = calloc(GRID_WIDTH, sizeof(char));
    }

    return ptr;

}

/**
 * Fills a grid with empty data (dead cells)
 * @param state The grid to initialize
 */
void initializeGrid(char **state) {

    for (int y = 0; y < GRID_HEIGHT; y++) {

        for (int x = 0; x < GRID_WIDTH; x++) {

            state[y][x] = DEAD_CELL;

        }

    }

}

/**
 * Updates an individual cell's state
 * @param x The cell's x position
 * @param y The cell's y position
 * @param state The cell's grid
 * @return Whether the updated cell is alive or not
 */
bool updateCellState(int x, int y, char **state) {

    if (!isInRange(x, y)) { return false; }

    bool alive = isCellAlive(x, y);
    int adjCells = nbCasesAdj(x, y);

    if ((alive && (adjCells == 2 || adjCells == 3)) || (!alive && adjCells == 3)) {

        state[y][x] = LIVING_CELL;

    }
    else {

        state[y][x] = DEAD_CELL;

    }

    return isCellAlive(x, y);

}

/**
 * Randomizes a grid's cell states
 * @param state The grid to randomize
 */
void randomizeGrid(char **state) {

    for (int y = 0; y < GRID_HEIGHT; y++) {

        for (int x = 0; x < GRID_WIDTH; x++) {

            state[y][x] = randInt(0, 1) == 0 ? DEAD_CELL : LIVING_CELL;

        }

    }

}

/**
 * Randomizes a grid's cell states with vertical symmetry
 * @param state The grid to randomize
 */
void randomizeGridVertical(char **state) {

    for (int y = 0; y < GRID_HEIGHT; y++) {

        for (int x = 0; (GRID_WIDTH % 2 == 0 ? x < GRID_WIDTH / 2 : x <= GRID_WIDTH / 2); x++) {

            state[y][x] = randInt(0, 1) == 0 ? DEAD_CELL : LIVING_CELL;

        }

        for (int x = 0; x <= GRID_WIDTH / 2; x++) {

            state[y][x + (GRID_WIDTH / 2)] = state[y][(GRID_WIDTH / 2) - x - (GRID_WIDTH % 2 == 0 ? 1 : 0)];

        }

    }

}

/**
 * Randomizes a grid's cell states with horizontal symmetry
 * @param state The grid to randomize
 */
void randomizeGridHorizontal(char **state) {

    for (int y = 0; (GRID_HEIGHT % 2 == 0 ? y < GRID_HEIGHT / 2 : y <= GRID_HEIGHT / 2); y++) {

        for (int x = 0; x < GRID_WIDTH; x++) {

            state[y][x] = randInt(0, 1) == 0 ? DEAD_CELL : LIVING_CELL;

        }

    }

    for (int y = 0; y <= GRID_HEIGHT / 2; y++) {

        state[y + GRID_HEIGHT / 2] = state[(GRID_HEIGHT / 2) - y - (GRID_HEIGHT % 2 == 0 ? 1 : 0)];

    }

}

/**
 * Prints the main menu
 */
void printMainMenu() {

    clearScreen();

    printf("%s", config.color);
    printf("\n%s\n", appTitle);

    printf("Welcome to the game of life! Simulate the famous game through customizable parameters!\n\n");

    printf("Pattern: %s\n", getPatternName(config.pattern));
    printf("Mode: %s\n", config.mode == AUTOMATIC ? "Automatic" : "Manual");

    if(config.mode == AUTOMATIC){

        if(config.maxIterations == -1){
            printf("Iterations: Unlimited%s\n", SPACER);
        }
        else{
            printf("Iterations: %d%s\n", config.maxIterations, SPACER);
        }
        printf("Speed: %d\n", config.speed);

    }

    printf("Grid Size: %dx%d\n\n", config.gridWidth, config.gridHeight);

    printf("1) Launch!\n");
    printf("2) Configure\n");
    printf("3) Exit\n");

}

/**
 * Manages the configure menu
 */
void printConfigureMenu(){

    clearScreen();

    while (true){

        resetCursorPosition(0);

        printf("\n%s\n", configureTitle);

        printf("Pattern: %s%s\n", getPatternName(config.pattern), SPACER);
        printf("Mode : %s%s\n", config.mode == AUTOMATIC ? "Automatic" : "Manual", SPACER);

        if(config.mode == AUTOMATIC){

            if(config.maxIterations == -1){
                printf("Iterations: Unlimited%s\n", SPACER);
            }
            else{
                printf("Iterations: %d%s\n", config.maxIterations, SPACER);
            }

            printf("Speed: %d%s\n", config.speed, SPACER);

        }

        printf("Grid Size: %dx%d%s\n\n", config.gridWidth, config.gridHeight, SPACER);

        printf("1) Previous Pattern\n");
        printf("2) Next Pattern\n");
        printf("3) Edit grid width\n");
        printf("4) Edit grid height\n");

        if(config.mode == AUTOMATIC){

            printf("5) Enable manual mode\n");

            if(config.maxIterations == -1){

                printf("6) Enable limited Iterations\n");
                printf("7) Edit Speed\n");
                printf("8) Go Back\n");

            }
            else{

                printf("6) Enable unlimited Iterations\n");
                printf("7) Edit Speed\n");
                printf("8) Edit Max Iterations\n");
                printf("9) Go Back\n");

            }

        }
        else{

            printf("5) Enable automatic mode\n");
            printf("6) Back\n");

        }

        switch (waitAndGetInput()) {

            case '1':

                if (config.pattern > 0) {
                    config.pattern--;
                }
                break;

            case '2':

                if (config.pattern < 5) {
                    config.pattern++;
                }
                break;

            case '3':
                increaseMenu(&config.gridWidth, 10, 200, "Grid's Width");
                break;

            case '4':
                increaseMenu(&config.gridHeight, 10, 45, "Grid's Height");
                break;

            case '5':
                config.mode = config.mode == AUTOMATIC ? MANUAL : AUTOMATIC;
                clearScreen();
                break;

            case '6':

                if(config.mode == AUTOMATIC){
                    if(config.maxIterations == -1){
                        config.maxIterations = 5;
                    }
                    else{
                        config.maxIterations = -1;
                    }
                    clearScreen();
                    break;
                }
                return;

            case '7':
                increaseMenu(&config.speed, 1, 25, "Speed");
                break;

            case '8':

                if(config.mode == AUTOMATIC) {

                    if(config.maxIterations == -1){
                        return;
                    }

                    increaseMenu(&config.maxIterations, 2, 100, "Max Iterations");
                }
                break;

            case '9':

                if(config.mode == AUTOMATIC && config.maxIterations != -1) {
                    return;
                }
                break;

            case ESCAPE_KEY:
                return;

        }

    }

}

/**
 * Dynamic menu allowing to modify an int value
 * @param val A pointer to the target integer
 * @param min The minimal allowed value
 * @param max The maximal allowed value
 * @param valueName The parameter's display name
 */
void increaseMenu(int* val, int min, int max, string valueName){

    clearScreen();

    while (true){

        resetCursorPosition();

        printf("\n%s\n", editTitle);

        printf("Editing %s value. Current: %d | (Min: %d, Max: %d)%s\n\n", valueName, *val, min, max, SPACER);

        printf("1) Decrease\n");
        printf("2) Increase\n");
        printf("3) Go Back\n");

        switch (waitAndGetInput()) {

            case '1':
                if (*val > min) {
                    *val = *val-1;
                }
                break;

            case '2':

                if (*val < max) {
                    *val = *val+1;
                }
                break;

            case '3':
                return;

            case ESCAPE_KEY:
                return;

        }

    }

}

/**
 * Creates a delay between iterations when using automatic mode
 * @return -1 if escape was pressed while the delay timer was running
 */
int makeIterationDelay() {

    double waitTime = 1.0 / config.speed;
    double elapsedTime = 0;
    clock_t timeStamp = clock();

    while (elapsedTime < waitTime) {

        elapsedTime = timeDiff(timeStamp, clock());

        if(kbhit() && getch() == ESCAPE_KEY){

            return -1;

        }

    }

    return 0;

}

/**
 * Show / hides the terminal's cursor depending on th OS
 * @param value Show Value
 */
void showCursor(bool value) {

#ifndef WIN32

    printf("\e[?25%c", value ? 'h' : 'l');

#else

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = {100, value};
    SetConsoleCursorInfo(handle, &info);

#endif

}
