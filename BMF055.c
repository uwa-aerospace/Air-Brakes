#include "air-brakes.h"

int getCurrentHeight()
{
    // TODO: calculate the current height
    return 0;
}

int getFakeCurrentHeightQuadratic(int time, int maxTime, int maxHeight)
{
    // Quadratic: roots of 0 and maxTime, height of h
    return -(4 * time * (time - maxTime) * maxHeight) / (pow(maxTime, 2.0));
}

int[] initialiseFile(char *fileName)
{
    FILE *file;
    file = fopen(fileName, "r");

    // Modify file
    if (!file)
    {
        perror("Could not open file.");
    }

    //Get each line of the file
    char line[1000];
    while (fgets(line, sizeof line, file) != NULL)
    {
        char *ch = line;
        // TODO: read in the simulation data to use in place of a quadratic
    }
}

int getFakeCurrentHeightFromFile(int time, int)
{
    static bool initialised = false;
    static int[] heights;

    // TODO: initialiseFile() if required, read the appropriate data

    initialised = true;
}