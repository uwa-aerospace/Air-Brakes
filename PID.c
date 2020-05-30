#include "air-brakes.h"

#define KP 2
#define KI 50
#define KD 450
// #define KP 50
// #define KI 20
// #define KD 1
#define OUTPUT_SCALING_DIVISOR 200000

bool first = true;

// Example discrete PID controller
int PIDController(int currentHeight, int desiredHeight)
{
    // Retained upon the next call of the function
    static int R_old = 0.0;
    static int e_old = 0.0;
    static int e_old_old = 0.0;

    int e = desiredHeight - currentHeight;
    int result = R_old + KP * (e - e_old) + KI * (e + e_old) / 2 + KD * (e - 2 * e_old + e_old_old);

    // Store current values to compare for next time
    R_old = result;
    e_old_old = e_old;
    e_old = e;
    if (!first && ((desiredHeight - currentHeight) * 100)/desiredHeight < 20) { // Last 20% of flight
        return result / OUTPUT_SCALING_DIVISOR;
    }
    else
    {
        first = false;
        return 0;
    }
}
