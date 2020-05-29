#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

// PID.c
extern int PIDController(int, int);

// BMF055.c
extern int getCurrentHeight();
extern int getFakeCurrentHeightQuadratic(int, int, int);
int initialiseFile(char *, int, int);
float getFakeCurrentHeightFromFile(float time);
