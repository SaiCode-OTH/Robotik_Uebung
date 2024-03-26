#include "nnxt.h"
#include <stdio.h>

#define DEG_TO_RPM 166.6667

int main()
{
    sensor_touch_clicked_t touch;
    SensorConfig(Port_3, SensorTouch);



    char rpmMsg[10];
    uint32_t prev_deg_A, deg_A,prev_deg_B, deg_B;
    uint32_t dt, prev_tick = GetSysTime();
    double rpm;

    MotorPortInit(Port_A);
    MotorPortInit(Port_B);

    Motor_Tacho_GetCounter(Port_A, &prev_deg_A);
    Motor_Tacho_GetCounter(Port_A, &prev_deg_B);

    NNXT_LCD_DisplayStringAtLine(0, "Hello There");

    while(!touch) {
            !Touch_Clicked(Port_3, &touch)

    }

    while (1)
    {
        Motor_Drive(Port_A, Motor_dir_forward, 75);
        Motor_Drive(Port_B, Motor_dir_backward, 75);
        Delay(450);
        Motor_Stop(Port_A, Motor_stop_float);
        Motor_Stop(Port_B, Motor_stop_float);
        Delay(1000);
        Motor_Drive(Port_A, Motor_dir_backward, 75);
        Motor_Drive(Port_B, Motor_dir_forward, 75);
        Delay(450);
        Motor_Stop(Port_A, Motor_stop_float);
        Motor_Stop(Port_B, Motor_stop_float);
        Delay(1000);
    }
    return 0;
}
