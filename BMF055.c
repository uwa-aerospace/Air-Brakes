#include "air-brakes.h"

bool fileMode = false;
bool fileInitalised = false;

DATA_POINT *heights;
int numberOfHeights = 0;

int getCurrentHeight()
{
    // TODO: calculate the current height using real sensor
    return 0;
}

int getFakeCurrentHeightQuadratic(int time, int maxTime, int maxHeight)
{
    // Quadratic: roots of 0 and maxTime, height of h
    return -(4 * time * (time - maxTime) * maxHeight) / (pow(maxTime, 2.0));
}

const char *getfield(char *line, int num)
{
    const char *tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

int initialiseFile(char *fileName, int timeColumn, int altColumn, int speedColumn, int accelerationColumn)
{
    fileMode = true;

    FILE *stream = fopen(fileName, "r");
    char line[3000];
    heights = malloc(0);
    float apogeeTime = 0.0;
    float previousHeight = 0.0;
    bool foundApogee = false;
    while (fgets(line, 3000, stream))
    {
        if (startsWith("#", line)) // Comment line in the CSV
        {
            continue;
        }
        numberOfHeights++;

        char *tmp = strdup(line);
        float height = atof(getfield(tmp, altColumn));
        free(tmp);

        tmp = strdup(line);
        float time = atof(getfield(tmp, timeColumn));
        free(tmp);

        tmp = strdup(line);
        float speed = atof(getfield(tmp, speedColumn));
        free(tmp);

        tmp = strdup(line);
        float acceleration = atof(getfield(tmp, accelerationColumn));
        free(tmp);

        // printf("At time %f, altitude: %f\n", time, height);
        heights = realloc(heights, numberOfHeights * sizeof(DATA_POINT));
        heights[numberOfHeights - 1].time = time;
        heights[numberOfHeights - 1].height = height;
        heights[numberOfHeights - 1].speed = speed;
        heights[numberOfHeights - 1].acceleration = acceleration;

        if (!foundApogee && numberOfHeights > 2)
        {
            if (previousHeight > height && height > 5.0)
            {
                apogeeTime = time;
                foundApogee = true;
            }
        }
        previousHeight = height;
    }

    fileInitalised = true;
    return (int)apogeeTime;
}

DATA_POINT *getDataFromFile(float time)
{
    if (!fileInitalised)
    {
        return NULL;
    }
    float previous = 0.0;
    DATA_POINT *correct = NULL;
    for (size_t i = 0; i < numberOfHeights; i++)
    {
        if (time < heights[i].time)
        {
            return correct;
        }
        previous = heights[i].height;
        correct = &heights[i];
    }
    return correct;
}
