#include "air-brakes.h"

// #define KP 2
// #define KI 50
// #define KD 450
#define KP 50
#define KI 10
#define KD 300
#define OUTPUT_SCALING_DIVISOR 500

/**
 * PID controller to get the brake percentage required to slow the rocket down if needed
 * Crucial this runs for the whole flight (even if it doesn't have a physical effect for some of it)
 * so that the model learns about the flight over time.
 * 
 * Current height: in ft
 * Desired height: in ft
 * Current speed: in m/s
 * Current acceleration: in m/s/s
*/
float PIDController(float currentHeight, float desiredHeight, float currentSpeed, float currentAcceleration)
{
    // Retained upon the next call of the function
    static float R_old = 0.0;
    static float e_old = 0.0;
    static float e_old_old = 0.0;
    static bool firstComplete = false;
    static bool secondComplete = false;

    // Edge cases, but crucial
    if (desiredHeight < currentHeight)
    {
        // We are too high already
        return 100; // MAXIMUM
    }

    // Feet to metres
    currentHeight *= 0.3048;
    desiredHeight *= 0.3048;

    // Required a = (u^2) / (2s)
    // then subtract gravity because we already have that, and add what we have but need to counteract
    float requiredDeceleration = ((currentSpeed * currentSpeed) / (2 * (desiredHeight - currentHeight))) - 9.81 + currentAcceleration;

    float e = requiredDeceleration;
    float result = R_old + KP * (e - e_old) + KI * (e + e_old) / 2 + KD * (e - 2 * e_old + e_old_old);

    // Store current values to compare for next time
    R_old = result;
    e_old_old = e_old;
    e_old = e;

    if (firstComplete && secondComplete) // && ((desiredHeight - currentHeight) * 100) / desiredHeight < 20) // Last 20% of height
    {
        float returnValue = result / OUTPUT_SCALING_DIVISOR;
        if (returnValue > 100)
        {
            return 100.0;
        }
        else if (returnValue < 0)
        {
            return 0.0;
        }
        else
        {
            return returnValue;
        }
    }
    else
    {
        if (!firstComplete)
        {
            firstComplete = true;
        }
        else if (firstComplete && !secondComplete)
        {
            secondComplete = true;
        }
        return 0;
    }
}
