#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include "bma2x2.h"

// Types
typedef struct
{
    float time;
    float height;
    float speed;
    float vertical_acceleration;
    s8 temperature;
} DATA_POINT;

// PID.c
extern float PIDController(float, DATA_POINT *);

// BMF055.c
extern DATA_POINT getCurrentBMX055Data();
extern void initialiseBMX055();
// Simulations
extern int getFakeCurrentHeightQuadratic(int, int, int);
int initialiseFile(char *, int, int, int, int);
DATA_POINT *getDataFromFile(float time);
