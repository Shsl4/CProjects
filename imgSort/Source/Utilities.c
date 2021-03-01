#include "../Headers/Utilities.h"

bool minimal = false;

const char* compNames[10] = {"Y", "Cb", "Cr", "R", "G", "B", "H", "S", "V", "Special"};
int compMethod = 0;
int numOuts = 0;

// Store references to our existing sorting methods, with their nice display names
const void* sortFuncs[4] = {(void*)&quickSort, (void*)&insertionSort, (void*)&selectionSort, (void*)&bubbleSort};
const char* funcNames[4] = {"Quick Sort", "Insertion Sort", "Selection Sort", "Bubble Sort"};

#ifdef _WIN32

/**
 * \brief An asprintf function implementation for windows
 * \param strPtr A pointer to the memory location where the new string will be allocated
 * \param format A formatting string
 * \param ...
 * \return An error code
 */
int asprintf(char** strPtr, const char* format, ...)
{
    if (strPtr == NULL) { return -1; }

    va_list list;
    va_start(list, format);

    const int stringLength = _vscprintf(format, list);
    if (stringLength < 0) { return -1; }

    const size_t size = (size_t)stringLength + 1;
    *strPtr = (char*)malloc(size);

    const int errorCode = vsprintf_s(*strPtr, stringLength + 1, format, list);
    if (errorCode == -1)
    {
        free(*strPtr);
        return -1;
    }

    va_end(list);

    return errorCode;
}

#endif

/**
 * \brief Calls a quick sorting algorithm and return it's execution time
 * \param arr The array of pixels to be sorted
 * \param size The size of the pixels array
 * \return The execution time
 */
float quickSort(Pixel* arr, int size)
{
    const clock_t begin = clock();
    quickSortPixel(arr, 0, size - 1);
    return (clock() - begin) / (float)CLOCKS_PER_SEC;
}

/**
 * \brief Hoare's Partitioning algorithm
 * \param arr The array of pixels to be sorted
 * \param from Where to begin the sort
 * \param to Where to end the sort
 * \return The pivot index
 */
int partPixel(Pixel* arr, int from, int to)
{
    Pixel pivot = arr[(int)floor((from + to) / 2)];
    int i = from - 1;
    int j = to + 1;

    while (true)
    {
        do
        {
            ++i;
        }
        while (compPixel(&arr[i], &pivot));

        do
        {
            --j;
        }
        while (compPixel(&pivot, &arr[j]));

        if (i >= j) { return j; }
        swap(&arr[i], &arr[j]);
    }
}

/**
 * \brief Sorts a Pixel array using a quick sort algorithm, w/ hoare partitioning
 * \param arr The array of pixels to be sorted
 * \param from Where to begin the sort
 * \param to Where to end the sort
 */
void quickSortPixel(Pixel* arr, int from, int to)
{
    if (from < to)
    {
        const int pivot = partPixel(arr, from, to);
        quickSortPixel(arr, from, pivot);
        quickSortPixel(arr, pivot + 1, to);
    }
}


/**
 * \brief Compares two pixels based on different comparison methods
 * \param a A pointer to a pixel
 * \param b A pointer to another pixel
 * \return A < B
 */
bool compPixel(Pixel* a, Pixel* b)
{
    float valA = 0.0f;
    float valB = 0.0f;

    switch (compMethod)
    {
    case 0:
        valA = a->Y;
        valB = b->Y;
        break;
    case 1:
        valA = a->Cb;
        valB = b->Cb;
        break;
    case 2:
        valA = a->Cr;
        valB = b->Cr;
        break;
    case 3:
        valA = a->R;
        valB = b->R;
        break;
    case 4:
        valA = a->G;
        valB = b->G;
        break;
    case 5:
        valA = a->B;
        valB = b->B;
        break;
    case 6:
        valA = (float)a->H;
        valB = (float)b->H;
        break;
    case 7:
        valA = a->S;
        valB = b->S;
        break;
    case 8:
        valA = a->V;
        valB = b->V;
        break;
    case 9:
        valA = (float)a->R + (float)a->G + (float)a->B / (float)a->Y;
        valB = (float)b->R + (float)b->G + (float)b->B / (float)b->Y;
        break;
    default:
        break;
    }

    return valA < valB;
}

/**
 * \brief Checks if an array of pixel is sorted based on some comparison
 * \param arr The array to be checked
 * \param size The size of arr 
 * \return Whether the array is sorted or not
 */
bool checkSort(Pixel* arr, int size)
{
    for (int i = 0; i < size - 1; ++i)
    {
        if (compPixel(&arr[i + 1], &arr[i]))
        {
            return false;
        }
    }

    return true;
}

/**
 * \brief Swaps the values of two pixels
 * \param a A pinter to a pixel
 * \param b A pointer to another pixel
 */
void swap(Pixel* a, Pixel* b)
{
    const Pixel temp = *b;
    *b = *a;
    *a = temp;
}

/**
* \brief Sorts a Pixel array using a bubble sort algorithm
* \param arr The array of pixels to be sorted
* \param size The size of the pixels array
* \return The execution time
*/
float bubbleSort(Pixel* arr, int size)
{
    const clock_t begin = clock();

    while (true)
    {
        bool reordered = false;

        for (int i = 0; i < size - 1; ++i)
        {
            if (compPixel(&arr[i + 1], &arr[i]))
            {
                swap(&arr[i], &arr[i + 1]);
                reordered = true;
            }
        }

        if (!reordered) { break; }
    }

    return (clock() - begin) / (float)CLOCKS_PER_SEC;
}

/**
* \brief Sorts a Pixel array using an insertion sort algorithm
* \param arr The array of pixels to be sorted
* \param size The size of the pixels array
* \return The execution time
*/
float insertionSort(Pixel* arr, int size)
{
    const clock_t begin = clock();

    for (int i = 1; i < size; i++)
    {
        Pixel val = arr[i];
        int j;

        for (j = i - 1; j >= 0; j--)
        {
            if (compPixel(&val, &arr[j]))
            {
                arr[j + 1] = arr[j];
            }
            else
            {
                break;
            }
        }

        arr[j + 1] = val;
    }

    return (clock() - begin) / (float)CLOCKS_PER_SEC;
}

/**
* \brief Sorts a Pixel array using a selection sort algorithm
* \param arr The array of pixels to be sorted
* \param size The size of the pixels array
* \return The execution time
*/
float selectionSort(Pixel* arr, int size)
{
    const clock_t begin = clock();

    for (int i = 0; i < size; ++i)
    {
        int lowest = i;

        for (int j = i; j < size; ++j)
        {
            if (compPixel(&arr[j], &arr[lowest]))
            {
                lowest = j;
            }
        }

        if (lowest == i) { continue; }

        swap(&arr[i], &arr[lowest]);
    }

    return (clock() - begin) / (float)CLOCKS_PER_SEC;
}

/**
* \brief Calls sorting function on a pixel array while printing results and execution time
* \param funcPtr A pointer pointing to a sorting function of prototype : float f(Pixel*, int)
* \param funcName The displayed function name when processing
* \param pixels An array of pixels to be sorted
* \param size The size of the pixels array
*/
void processSort(const void* funcPtr, const char* funcName, Pixel* pixels, int size)
{

    if(!minimal)
    {
        printf("Sorting using %s...\n", funcName);
    }
    
    // Cast our function pointer to the expected function prototype
    float (*sortFunc)(Pixel*, int) = (float(*)(Pixel*, int))funcPtr;

    const float execTime = sortFunc(pixels, size);

    if(!minimal)
    {
        printf("Sorting result: %s\n", checkSort(pixels, size) ? "Sorted" : "Unsorted");
        printf("Execution Time: %.4f s.\n\n", execTime); 
    }
    
}

/**
 * \brief Copies the pixels of the input image and sorts them. This allows the original image to be unmodified.
 * \param funcPtr A pointer pointing to a sorting function of prototype : float f(Pixel*, int)
 * \param funcName The displayed function name when processing
 * \param image A pointer to an image 
 * \param writeOutput Should write the sorted image
 */
void copyAndProcess(const void* funcPtr, const char* funcName, Image* image, bool writeOutput)
{

    // Allocates a new array of pixels and copies the image's pixels to the newly allocated memory space
    // We don't directly sort the image's pixels because the sorting algorithms executing after the first
    // one would sort an already sorted array
    Pixel* pixels = calloc(image->size, sizeof(Pixel));
    
    // Using this copy method as memcpy sometimes has weird behaviors >:(
    for (int j = 0; j < image->size; ++j)
    {
        pixels[j] = image->pix[j];
    }

    processSort(funcPtr, funcName, pixels, image->size);

    // Writes the pixels if requested and frees the allocated memory 
    if (writeOutput)
    {
        char* buf;
        asprintf(&buf, "Output/%s - %s.bmp", compNames[compMethod], funcName);
        writePixs(pixels, image, buf);
        free(buf);
        ++numOuts;
    }

    free(pixels);
    
}


/**
* \brief Checks if a directory exists by path
* \param path The path of the directory
* \return if the directory exists or not
*/
bool doesDirectoryExist(const char* path)
{
    struct stat status;
    stat(path, &status);
    return (status.st_mode & S_IFDIR) != 0;
}
