#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

// Types
typedef struct
{
    // Can easily just truncate to integers
    float time;
    float height;
    float speed;
    float acceleration;
} DATA_POINT;

// PID.c
extern float PIDController(float, float, float, float);

// BMF055.c
extern int getCurrentHeight();
extern int getFakeCurrentHeightQuadratic(int, int, int);
int initialiseFile(char *, int, int, int, int);
DATA_POINT *getDataFromFile(float time);
