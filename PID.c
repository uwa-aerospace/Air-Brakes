#include "air-brakes.h"

#define KP 5
#define KI 30
#define KD 7
#define OUTPUT_SCALING_DIVISOR 5000
#define BRAKES_MAX_DECELERATION 10

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
float PIDController(float desiredHeight, DATA_POINT *currentData)
{
    // Retained upon the next call of the function
    static float R_old = 0.0;
    static float e_old = 0.0;
    static float e_old_old = 0.0;
    static bool firstComplete = false;
    static bool secondComplete = false;
    static float lastTime = 0.0;

    // Edge cases, but crucial
    if (desiredHeight < currentData->height)
    {
        // We are too high already
        return 100.0; // MAXIMUM
    }

    // Feet to metres
    float currentHeight = currentData->height * 0.3048;
    desiredHeight *= 0.3048;

    // Required a = (u^2) / (2s)
    // then subtract gravity because we already have that, and add what we have but need to counteract
    float requiredDeceleration = ((currentData->speed * currentData->speed) / (2 * (desiredHeight - currentHeight))) - 9.81 + currentData->vertical_acceleration;

    float e = requiredDeceleration;// - currentData->vertical_acceleration; //desiredHeight - currentHeight;
    float result = R_old + (KP * (e - e_old) + KI * (e + e_old) / 2 + KD * (e - 2 * e_old + e_old_old)) * (1.0 + (currentData->time - lastTime)*20);

    // Store current values to compare for next time
    R_old = result;
    e_old_old = e_old;
    e_old = e;
    lastTime = currentData->time;

    if (firstComplete && secondComplete) // && ((desiredHeight - currentHeight) * 100) / desiredHeight < 50) // Last 20% of height
    {
        // float returnValue = ((currentData->speed * currentData->speed) / (2 * (result))) - 9.81 + currentData->vertical_acceleration;
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
