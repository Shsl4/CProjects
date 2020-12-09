#include "Definitions.h"

void shiftRotor(void)
{

    if(shift[0] < 25){
        shift[0]++;
    }
    else{
        shift[0] = 0;
        shift[1]++;
    }
    if(shift[1] >= 26){
        shift[1] = 0;
        shift[2]++;
    }
    if(shift[2] >= 26){
        shift[0] = 0;
        shift[1] = 0;
        shift[2] = 0;
    }

}

char encrypt(char arg)
{

    const int stage1 = alphaPos(connexions[alphaPos(arg)]);

    const int stage2 = shiftedPos(stage1, 0, false);
    const int stage3 = shiftedPos(stage2, 1, false);
    const int stage4 = shiftedPos(stage3, 2, false);

    const int stage5 = reflector[stage4];

    const int stage6 = shiftedPos(stage5, 2, true);
    const int stage7 = shiftedPos(stage6, 1, true);
    const int stage8 = shiftedPos(stage7, 0, true);

    const int stage9 = connexions[stage8];

    shiftRotor();

    return stage9;
    
}

void initConnexions(void)
{
    for (int i = 0; i < 26; i++)
    {
        connexions[i] = 'A' + i;
    }
}

int connectCables(void)
{

    printf("Select a number of cables to connect (0 - 6): ");
    scanf("%d", &cableAmount);
    printf("\n");

    while(getchar() != '\n'){}

    if(cableAmount <= 0) { return 0; }
    if(cableAmount > 6) { cableAmount = 6; printf("Amount reduced to 6.\n\n"); }

    int lastPutIndex = 0;

    for (int i = 0; i < cableAmount; i++)
    {

        char first;
        char second;

        printf("Pick two letters to flip (%d/%d) (A B): ", i+1, cableAmount);
        scanf("%c %c", &first, &second);
        printf("\n");

        while(getchar() != '\n'){}

        first = toupper(first);
        second = toupper(second);
               
        if(first == second || containsChar(flippedChars, 12, first) || containsChar(flippedChars, 12, second)) { return -1; }

        flippedChars[lastPutIndex] = first;
        lastPutIndex++;
        flippedChars[lastPutIndex] = second;
        lastPutIndex++;

        flip(connexions, first, second);

    }
    
    return 0;
    
}

int shiftedPos(int c, int r, bool b){

    if (!b){

        return (rotor[r][( c + 26 - shift[r] ) % 26] + shift[r]) % 26;

    }
    
    return (invRotor[r][( c + 26 - shift[r] ) % 26] + shift[r]) % 26;
    
}

bool containsChar(char* ptr, int size, char test)
{

    for (int i = 0; i < size; ++i) {

        if(ptr[i] == test){
            return true;
        }

    }

    return false;

}

void Text2Value(void)
{
    for (int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 26; j++)
        {
            rotor[i][j] = alphaPos(textRotor[i][j]);
        }
    }

    for (int i = 0; i < 26; i++)
    {
        reflector[i] = alphaPos(textReflector[i]);
    }
}

void flip(char* dest, char a, char b)
{
    dest[alphaPos(a)] = b;
    dest[alphaPos(b)] = a;
}

void GenerateInvPerm(void)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            invRotor[i][rotor[i][j]] = j;
        }
    }

}

void initProgram(void) {

    initConnexions();

    bool runProgram = true;

    while (runProgram){

        printMainMenu();

        switch (waitForInput()) {

            case '1':
                encryptMenu();
                break;
            case '2':
                rotorMenu();
                break;
            case '3':
                cableMenu();
                break;
            case '4':
                runProgram = false;
                break;
            case ESCAPE_KEY:
                runProgram = false;
                break;
            default:
                break;
        }

        clearInputBuffer();

    }

    printf("\nGoodbye!\n\n");
    printf(COLOR_RESET);
    showCursor(true);


}

void printMainMenu(void) {

    showCursor(false);

    clearScreen();

    printf("\n%s%s\n\n", COLOR_RED, enigmaTitle);

    printf("Welcome to ENIGMA, a simulation of the famous machine used by the germans to encrypt messages during WWII!\n");
    printf("You can encrypt or decrypt your own messages using this small app. The rotors and reflector are predefined and static.\n\n");

    printf("Active Rotor shifts: %d %d %d | %c %c %c\n", initShift[0], initShift[1], initShift[2], initShift[0] + 65, initShift[1] + 65, initShift[2] + 65);
    printf("Active cables: ");

    if(cableAmount == 0){

        printf("None\n\n");

    }
    else{

        int lastIndex = 0;

        for (int i = 0; i < cableAmount; ++i) {

            lastIndex += 2;
            printf("{%c ; %c} ", flippedChars[lastIndex - 2], flippedChars[lastIndex - 1]);

        }

        printf("\n\n");

    }


    printf("1) Encrypt / Decrypt a string\n");
    printf("2) Set initial rotor position\n");
    printf("3) (Re)Set cables\n");
    printf("4) Exit\n");

}

void encryptMenu(void) {

    clearScreen();

    printf("\n%s\n\n", processTitle);

    showCursor(true);

#ifndef WIN32

    destTermAttr();

#endif

    printf("Please enter a string to encrypt / decrypt (256 chars max): ");
    scanf("%256s", textInput);
    printf("\n");

#ifndef WIN32

    initTermAttr();

#endif

    showCursor(false);

    while(getchar() != '\n'){}

    printf("Your original string is: %s\n", textInput);
    char* str = process(textInput);
    printf("Your processed string is: %s\n\n", str);

    promptContinue();

    free(str);

}

char* process(char *str) {

    Text2Value();
    GenerateInvPerm();
    resetShift();

    char* newStr = malloc(strlen(str) * sizeof(char));

    for (int j = 0; j < strlen(str); j++)
    {
        newStr[j] = encrypt(str[j]);
    }

    return newStr;

}

void resetShift(){

    shift[0] = initShift[0];
    shift[1] = initShift[1];
    shift[2] = initShift[2];

}

void cableMenu(void){

    clearScreen();
    clearInputBuffer();
    initConnexions();

    printf("\n%s\n\n", cableTitle);

#ifndef WIN32

    destTermAttr();

#endif

    showCursor(true);

    if(connectCables() == -1){

        printf("You may not flip a letter several times.\n\n");
        cableAmount = 0;
        for (int i = 0; i < 12; ++i) {
            flippedChars[i] = ' ';
        }
        initConnexions();

    }

#ifndef WIN32

    initTermAttr();

#endif

    showCursor(false);

    promptContinue();

}

void rotorMenu(void){

    while (true){

        clearScreen();

        printf("\n%s\n\n", rotorTitle);

        printf("You can edit the rotors initial shifts here.\n\n");

        printf("1) Edit RotorI Shift\n");
        printf("2) Edit RotorII Shift\n");
        printf("3) Edit RotorIII Shift\n");
        printf("4) Exit\n");

        switch (waitForInput()) {

            case '1':
                increaseMenu(&initShift[0], 0, 25, "RotorI Shift", editTitle);
                break;
            case '2':
                increaseMenu(&initShift[1], 0, 25, "RotorII Shift", editTitle);
                break;
            case'3':
                increaseMenu(&initShift[2], 0, 25, "RotorIII Shift", editTitle);
                break;
            case '4':
                return;
            case ESCAPE_KEY:
                return;
            default:
                break;
        }

        clearInputBuffer();

    }

}

