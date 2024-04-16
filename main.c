#include "nnxt.h"
volatile uint8_t left_pressed = 0;
volatile uint8_t right_pressed = 0;

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

void onLeftSensorClick()
{
    while (1)
    {
        Delay(100);
        sensor_touch_clicked_t touch;
        Touch_Clicked(Port_1, &touch);
        if (touch == SensorTouch_clicked)
        {
            left_pressed = 1;
        }
    }
}

void onRightSensorClick()
{
    uint8_t recently_right_pressed = 0;
    while (1)
    {
        Delay(100);
        sensor_touch_clicked_t touch;
        Touch_Clicked(Port_2, &touch);
        if (touch == SensorTouch_clicked && recently_right_pressed == 0)
        {
            right_pressed = 1;
            recently_right_pressed = 1;
        }
        else if (touch != SensorTouch_clicked && recently_right_pressed == 1)
        {
            recently_right_pressed = 0;
        }
    }
}

void drive()
{
    motor_dir_t current_direction = Motor_dir_forward;
    while (1)
    {

        if (current_direction == Motor_dir_backward)
        {
            NNXT_LCD_DisplayStringAtLine(0, "rueckwaerts ");
        }
        else
        {
            NNXT_LCD_DisplayStringAtLine(0, "vorwaerts     ");
        }

        if (right_pressed)
        {
            // invert direction
            if (current_direction == Motor_dir_forward)
            {
                current_direction = Motor_dir_backward;
            }
            else
            {
                current_direction = Motor_dir_forward;
            }
            right_pressed = 0;
        }

        if (left_pressed)
        {
            Motor_Drive(Port_B, current_direction, 35);
            Motor_Drive(Port_A, current_direction, 35);
            Delay(1000);
            Motor_Stop(Port_B, Motor_stop_float);
            Motor_Stop(Port_A, Motor_stop_float);
            left_pressed = 0;
        }
    }
}

int main()
{
    MotorPortInit(Port_A);
    MotorPortInit(Port_B);
    SensorConfig(Port_1, SensorTouch);
    SensorConfig(Port_2, SensorTouch);

    CreateAndStartTask(onLeftSensorClick);
    CreateAndStartTask(onRightSensorClick);
    CreateAndStartTask(drive);

    StartScheduler();

    return 0;
}
