#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// PID.c
extern int PIDController(int);

// BMF055.c
extern int getCurrentHeight();
extern int getFakeCurrentHeightQuadratic(int, int, int);
