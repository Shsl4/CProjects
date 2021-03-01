#include "Headers/Utilities.h"

void printHelp()
{
    printf("Usage: imgSort [OPTION]... [FILE]... [OUTPUT]...\n");
    printf("Sorts the FILE BMP image using a pixel comparison method and writes it at OUTPUT.\n\n");

    printf(" -s [NUMBER]    Use a different sorting method\n");

    printf("                0: Quick Sort (default, fastest)\n");
    printf("                1: Insertion Sort\n");
    printf("                2: Selection Sort\n");
    printf("                3: Bubble Sort\n\n");

    printf(" -c [NUMBER]    Uses a different pixel comparison method\n");
    printf("                0: Y (default)\n");
    printf("                1: Cb\n");
    printf("                2: Cr\n");
    printf("                3: R\n");
    printf("                4: G\n");
    printf("                5: B\n");
    printf("                6: H\n");
    printf("                7: S\n");
    printf("                8: V\n");
    printf("                9: Special (RGB / Y)\n\n");

    printf(" --minimal      Reduces the amount of text printing\n\n");
    printf(" --noOutput     Only evaluate the sorting execution time, does not write to the output\n\n");
    printf(" --allSorts     Evaluates every sorting method and writes images named [COMP] [SORT].bmp if output is enabled (may take a long time)\n\n");
    printf(" --allComps     Evaluates every comparison method and writes images named [COMP] [SORT].bmp if output is enabled (may take a long time)\n\n");
    printf(" --help         Prints this help menu\n\n");
    printf("The --allSorts and --allComps options ignores the OUTPUT argument. They generate a folder named 'Output' (if it does not already exist) and store all of the generated images in it.\n\n");
}

int main(int argc, char** argv)
{
    // Prints help if less than two arguments are provided (requires at least two arguments, the executable name counts as one)
    if (argc < 3)
    {
        printHelp();
        return 0;
    }

    // Cool variables
    bool writeOutput = true;
    bool allSorts = false;
    bool allComps = false;
    int sortMethod = 0;
    int nextArg = 1;
    char* inPath = NULL;
    char* outPath = NULL;

    // Handle and processes arguments
    while (nextArg < argc)
    {
        const char* arg = argv[nextArg];

        // Changes the sorting method
        if (!strcmp(arg, "-s"))
        {
            sortMethod = atoi(argv[nextArg + 1]);

            if (sortMethod < 0 || sortMethod > 3)
            {
                printf("Unrecognized sorting method: %d. Use --help for more info.\n", sortMethod);
                return -1;
            }

            ++nextArg;
        }
        // Disables output
        else if (!strcmp(arg, "--noOutput"))
        {
            writeOutput = false;
        }
        // Enables all sorting methods
        else if (!strcmp(arg, "--allSorts"))
        {
            allSorts = true;
        }
        // Enables all comparison methods
        else if (!strcmp(arg, "--allComps"))
        {
            allComps = true;
        }
        // Changes the pixel comparison method
        else if (!strcmp(arg, "-c"))
        {
           
            compMethod = atoi(argv[nextArg + 1]);

            if (compMethod < 0 || compMethod > 9)
            {
                printf("Unrecognized comparison method: %d. Use --help for more info.\n", compMethod);
                return -2;
            }

            ++nextArg;
        }
        // Prints the help menu
        else if (!strcmp(arg, "--help"))
        {
            printHelp();
            return 0;
        }
        // Enables minimal output
        else if (!strcmp(arg, "--minimal"))
        {
            minimal = true;
        }
        // If the argument is not recognized, consider that it is our bmp image input file
        else if (!inPath)
        {
            inPath = argv[nextArg];
        }
        // If the argument is not recognized a second time, consider that it is our bmp image output file
        else if (!outPath)
        {
            outPath = argv[nextArg];
        }

        // Increments the variable so the next argument can be processed
        ++nextArg;
    }

    // Check if an input path is provided
    if (!inPath)
    {
        printf("No input path provided.\n");
        return -3;
    }

    // Check if the input file is a bmp image (file extension is .bmp)
    if (strcmp(".bmp", &inPath[strlen(inPath) - 4]) != 0)
    {
        printf("Input path (%s) does not point to a bitmap image.\n", inPath);
        return -4;
    }

    if (writeOutput && !allSorts && !allComps)
    {
        // Check if an output path is provided if we need to write an output
        if (!outPath)
        {
            printf("No output path provided.\n");
            return -5;
        }

        // Check if the output file is a bmp image (file extension is .bmp)
        if (strcmp(".bmp", &outPath[strlen(outPath) - 4]) != 0)
        {
            printf("Output path (%s) is not a bitmap image.\n", outPath);
            return -6;
        }
    }

    // Tries to read the file pointed by the input path, returns an 
    // error code if the file couldn't be opened / read
    Image* image = readImage(inPath);
    if (!image) { return -7; }

    if (allComps) { compMethod = 0; }

    if (allComps || allSorts)
    {
        if (!doesDirectoryExist("./Output"))
        {           
#ifdef _WIN32

            if (_mkdir("Output") == -1)
            {
                printf("Failed to create output directory. Please create the ./Output directory manually.\n");
                return -8;
            }

#else

            if(mkdir("Output", 0700) == -1)
            {
                printf("Failed to create output directory. Please create the ./Output directory manually.\n");
                return -8;
            }
            
#endif
            
            printf("Created ./Output directory.\n");
        }
    }

    do
    {
        printf("Sorting using %s comparison...\n", compNames[compMethod]);

        if (allSorts)
        {
            for (int i = 0; i < 4; ++i)
            {
                copyAndProcess(sortFuncs[i], funcNames[i], image, writeOutput);
            }
        }
        else if(allComps)
        {
            copyAndProcess(sortFuncs[sortMethod], funcNames[sortMethod], image, writeOutput);
        }
        else
        {

            processSort(sortFuncs[sortMethod], funcNames[sortMethod], image->pix, image->size);
                
            // Writes the pixels if requested
            if (writeOutput)
            {
                writeImage(image, outPath);
                ++numOuts;
            }
               
        }

        ++compMethod;
    }
    while (allComps && compMethod < 10);

    free(image->pix);
    free(image);

    printf("Done! Created a total of %d files.\n\n", numOuts);
    
    return 0;
}
