#include "nnxt.h"
#include "stdbool.h"
#include "math.h"
#include "event.h"
#include "timer.h"

#define DEG_TO_RPS 166.6667/60
#define WHEEL_DIAMETER 4.3
#define PI 3.14159

#define TARGET_SPEED 9.0
#define KP 10
#define KI 1.5
#define KD 0.75
#define DELAY 50

#define MOTOR_LEFT Port_B
#define MOTOR_RIGHT Port_A

uint32_t speed_last_deg_measure, speed_last_time_measure, last_i_time, last_d_delta;
double rps, cummulativeDelta;


void getCurrentSpeed(double *speedInCmPerS)
{
    uint32_t deg, dt;
    dt = GetSysTime() - speed_last_time_measure;
    Motor_Tacho_GetCounter(Port_A, &deg);
    deg = deg - speed_last_deg_measure;
    rps = ((deg * 1.33) * DEG_TO_RPS) / (double)dt;
    Motor_Tacho_GetCounter(Port_A, &speed_last_deg_measure);
    speed_last_time_measure = GetSysTime();
    *speedInCmPerS = PI * WHEEL_DIAMETER * rps;
}

double pRegulator(double diff)
{
    return KP * diff;
}

double iRegulator(double diff)
{
    cummulativeDelta += diff;
    int8_t ret = KI * cummulativeDelta * DELAY;
    last_i_time = GetSysTime();
    return ret;
}

double dRegulator(double diff)
{
    int8_t ret = KD * (diff - last_d_delta) / DELAY;
    last_d_delta = diff;
    return ret;
}


int main()
{
    int8_t motorForce;
    double currentSpeed, ds;
    char dispMsg[20];

    Motor_Tacho_GetCounter(MOTOR_RIGHT, &speed_last_deg_measure);
    last_i_time = speed_last_time_measure = GetSysTime();
    last_d_delta = speed_last_deg_measure = motorForce = cummulativeDelta = 0;
    while (1)
    {
        // calculate speed difference
        getCurrentSpeed(&currentSpeed);
        sprintf(dispMsg, "%f    ", currentSpeed);
        NNXT_LCD_DisplayStringAtLine(0, dispMsg);
        ds = TARGET_SPEED - currentSpeed;
        // call regulators
        motorForce = (pRegulator(ds) + iRegulator(ds) + dRegulator(ds));
        // limit motor force between 0 and 80
        motorForce = (motorForce < 0) ? 0 : (motorForce > 80) ? 80
                                                              : motorForce;
        sprintf(dispMsg, "%d    ", motorForce);
        NNXT_LCD_DisplayStringAtLine(1, dispMsg);
        // power motor
        Motor_Drive(Port_A, Motor_dir_forward, motorForce);
        Delay(DELAY);
    }
    return 0;
}