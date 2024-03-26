#include "nnxt.h"

int main()
{
    sensor_touch_clicked_t touch;
    SensorConfig(Port_1, SensorTouch);


    MotorPortInit(Port_A);
    MotorPortInit(Port_B);

    NNXT_LCD_DisplayStringAtLine(0, "Linken sensor drücken");

// wait for touch sensor to be clicked
    while (1)
    {
        Touch_Clicked(Port_1, &touch);
        if (touch == SensorTouch_clicked)
        {
            break;
        }
        Delay(100);
    }

    while (1)
    {
        Motor_Drive(Port_A, Motor_dir_forward, 75);
        Motor_Drive(Port_B, Motor_dir_backward, 75);
        Delay(500);
        Motor_Stop(Port_A, Motor_stop_float);
        Motor_Stop(Port_B, Motor_stop_float);
        Delay(1000);
        Motor_Drive(Port_A, Motor_dir_backward, 75);
        Motor_Drive(Port_B, Motor_dir_forward, 75);
        Delay(500);
        Motor_Stop(Port_A, Motor_stop_float);
        Motor_Stop(Port_B, Motor_stop_float);
        Delay(1000);
    }
    return 0;
}
