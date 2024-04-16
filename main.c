#include "nnxt.h"

void turnLeft(int degree)
{
    Motor_Drive(Port_A, Motor_dir_forward, 75);
    Motor_Drive(Port_B, Motor_dir_backward, 75);
    Delay((int)(degree * 3.66));
    Motor_Stop(Port_A, Motor_stop_float);
    Motor_Stop(Port_B, Motor_stop_float);
}
void turnRight(int degree)
{
    Motor_Drive(Port_A, Motor_dir_backward, 75);
    Motor_Drive(Port_B, Motor_dir_forward, 75);
    Delay((int)(degree * 3.66));
    Motor_Stop(Port_A, Motor_stop_float);
    Motor_Stop(Port_B, Motor_stop_float);
}

void driveForward(int delay)
{
    Motor_Drive(Port_A, Motor_dir_forward, 75);
    Motor_Drive(Port_B, Motor_dir_forward, 75);
    Delay(delay);
    Motor_Stop(Port_A, Motor_stop_float);
    Motor_Stop(Port_B, Motor_stop_float);
}

int main()
{
    MotorPortInit(Port_A);
    MotorPortInit(Port_B);

    sensor_touch_clicked_t touch;
    sensor_touch_clicked_t last_state;
    char distStr[20];

    uint32_t last_press = 0;
    int avg_interval = 0;

    SensorConfig(Port_1, SensorTouch);

    while (1)
    {
        Touch_Clicked(Port_1, &touch);
        if (touch == SensorTouch_clicked)
        {
            NNXT_LCD_DisplayStringAtLine(0, "Taster gedrueckt");
            if (last_state == SensorTouch_released)
            {
                if (last_press == 0)
                {
                    last_press = GetSysTime();
                }
                else
                {
                    avg_interval = (int)(avg_interval * 0.9 + (GetSysTime() - last_press) * 0.1);
                    last_press = GetSysTime();
                }
            }
            last_state = touch;
        }
        else
        {
            NNXT_LCD_DisplayStringAtLine(0, "Taster losgelassen");
            last_state = touch;
        }

        sprintf(distStr, "Avg Interval: %d ms ", (int)avg_interval);
        NNXT_LCD_DisplayStringAtLine(1, distStr);

        sprintf(distStr, "Avg Freq: %d Hz ", (int)1000 / avg_interval);
        NNXT_LCD_DisplayStringAtLine(2, distStr);

        sprintf(distStr, "Last Press: %d ms ", (int)GetSysTime() - last_press);
        NNXT_LCD_DisplayStringAtLine(3, distStr);
        Delay(20);
    }

    return 0;
}
