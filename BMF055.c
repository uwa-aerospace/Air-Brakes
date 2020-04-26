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
