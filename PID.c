#include "air-brakes.h"

// Metres
#define DESIRED_HEIGHT 10000

#define KP 1
#define KI 1
#define KD 1


// Example discrete PID controller
void PIDController()
{
    // Retained upon the next call of the function
    static int R_old = 0;
    static int e_old = 0;
    static int e_old_old = 0;

    int e = DESIRED_HEIGHT - getCurrentHeight();
    int result = R_old + KP * (e - e_old) + KI * (e + e_old) / 2 + KD * (e - 2 * e_old + e_old_old);

    // Store current values to compare for next time
    R_old = result;
    e_old_old = e_old;
    e_old = e;
}

