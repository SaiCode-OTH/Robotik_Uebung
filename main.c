#include "nnxt.h"
#include "stdbool.h"
#include "event.h"
#include "timer.h"

#define MOTOR_LEFT Port_B
#define MOTOR_RIGHT Port_A
#define TOUCH_LEFT Port_0
#define TOUCH_RIGHT Port_1

#define CLICK_LEFT 0
#define CLICK_RIGHT 1
#define TIMER_FINISHED 2

void onLeftSensorClick()
{
    while (1)
    {
        Delay(50);
        sensor_touch_clicked_t touch;
        Touch_Clicked(TOUCH_LEFT, &touch);
        if (touch == SensorTouch_clicked)
        {
            setEvent(CLICK_LEFT);
        }
    }
}

void onRightSensorClick()
{
    bool recently_right_pressed = false;
    while (1)
    {
        Delay(50);
        sensor_touch_clicked_t touch;
        Touch_Clicked(TOUCH_RIGHT, &touch);
        if (touch == SensorTouch_clicked && !recently_right_pressed)
        {
            recently_right_pressed = true;
            setEvent(CLICK_RIGHT);
        }
        else if (touch == SensorTouch_released && recently_right_pressed)
        {
            recently_right_pressed = false;
        }
    }
}

void drive()
{
    motor_dir_t motor_direction = Motor_dir_forward;
    while (1)
    {
        Delay(10);
        if (eventIsSet(CLICK_RIGHT))
        {
            // invert direction
            if (motor_direction == Motor_dir_forward)
            {
                motor_direction = Motor_dir_backward;
            }
            else
            {
                motor_direction = Motor_dir_forward;
            }
            clearEvent(CLICK_RIGHT);
        }

        if (eventIsSet(CLICK_LEFT))
        {
            // drive forward or backward for 1 second
            Motor_Drive(MOTOR_LEFT, motor_direction, 35);
            Motor_Drive(MOTOR_RIGHT, motor_direction, 35);
            clearEvent(CLICK_LEFT);
            setTimer(0, 1000, TIMER_FINISHED);
            startTimer(0);
        }

        if (eventIsSet(TIMER_FINISHED))
        {
            Motor_Stop(MOTOR_LEFT, Motor_stop_float);
            Motor_Stop(MOTOR_RIGHT, Motor_stop_float);
            clearEvent(TIMER_FINISHED);
        }

        if (motor_direction == Motor_dir_backward)
        {
            NNXT_LCD_DisplayStringAtLine(0, "rueckwaerts ");
        }
        else
        {
            NNXT_LCD_DisplayStringAtLine(0, "vorwaerts     ");
        }
    }
}

int main()
{
    MotorPortInit(MOTOR_LEFT);
    MotorPortInit(MOTOR_RIGHT);
    SensorConfig(TOUCH_LEFT, SensorTouch);
    SensorConfig(TOUCH_RIGHT, SensorTouch);

    CreateAndStartTask(timerTask);

    CreateAndStartTask(onLeftSensorClick);
    CreateAndStartTask(onRightSensorClick);
    CreateAndStartTask(drive);

    StartScheduler();

    return 0;
}
