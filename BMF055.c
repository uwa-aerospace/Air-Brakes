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

int initialiseFile(char *fileName)
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
        printf("%s\n", ch);
        // TODO: read in the simulation data to use in place of a quadratic
    }
    return 0;
}

int getFakeCurrentHeightFromFile(int time)
{
    static bool initialised = false;
    // static int heights[1024];

    printf("%i\n", time);

    initialised = true;
    return 0;
}
