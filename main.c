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

#define DRIVE_LENGTH 1000

typedef enum
{
    "L-3",
    "L-2",
    "L-1",
    "L0",
    "L1",
    "L2",

    "M0",

    "R-2",
    "R-1",
    "R0",
    "R1",
    "R2",
    "R3",
} state;

void restartTimer()
{
    clearEvent(TIMER_FINISHED);
    cancelTimer(0);
    startTimer(0);
}

void turnLeft()
{
    Motor_Drive(Port_A, Motor_dir_forward, 75);
    Motor_Drive(Port_B, Motor_dir_backward, 75);
    Delay(330);
    Motor_Stop(Port_A, Motor_stop_float);
    Motor_Stop(Port_B, Motor_stop_float);
}
void turnRight()
{
    Motor_Drive(Port_A, Motor_dir_backward, 75);
    Motor_Drive(Port_B, Motor_dir_forward, 75);
    Delay(330);
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
    while (1)
    {
        Delay(50);
        sensor_touch_clicked_t touch;
        Touch_Clicked(TOUCH_RIGHT, &touch);
        if (touch == SensorTouch_clicked)
        {
            setEvent(CLICK_RIGHT);
        }
    }
}

void drive()
{
    state currentState = "M0";
    setTimer(0, 5000, TIMER_FINISHED);
    char dispStr[20];
    while (1)
    {
        switch (currentState)
        {

        // Facing left
        case "L-3":
            if (eventIsSet(CLICK_LEFT))
            {
                clearEvent(CLICK_LEFT);
                restartTimer();
            }
            else if (eventIsSet(CLICK_RIGHT))
            {
                turnRight();
                turnRight();
                driveForward(DRIVE_LENGTH);
                currentState = "R-2";
                clearEvent(CLICK_RIGHT);
                restartTimer();
            }
            else if (eventIsSet(TIMER_FINISHED))
            {
                turnRight();
                turnRight();
                driveForward(DRIVE_LENGTH * 3);
                turnLeft();
                currentState = "M0";
                restartTimer();
                clearEvent(TIMER_FINISHED);
            }
            break;
        case "L-2":
            if (eventIsSet(CLICK_LEFT))
            {
                driveForward(DRIVE_LENGTH);
                currentState = "L-3";
                clearEvent(CLICK_LEFT);
                restartTimer();
            }
            else if (eventIsSet(CLICK_RIGHT))
            {
                turnRight();
                turnRight();
                driveForward(DRIVE_LENGTH);
                currentState = "R-1";
                clearEvent(CLICK_RIGHT);
                restartTimer();
            }
            else if (eventIsSet(TIMER_FINISHED))
            {
                turnRight();
                turnRight();
                driveForward(DRIVE_LENGTH * 2);
                turnLeft();
                currentState = "M0";
                restartTimer();
                clearEvent(TIMER_FINISHED);
            }
            break;
        case "L-1":
            if (eventIsSet(CLICK_LEFT))
            {
                driveForward(DRIVE_LENGTH);
                currentState = "L-2";
                clearEvent(CLICK_LEFT);
                restartTimer();
            }
            else if (eventIsSet(CLICK_RIGHT))
            {
                turnRight();
                turnRight();
                driveForward(DRIVE_LENGTH);
                currentState = "R0";
                clearEvent(CLICK_RIGHT);
                restartTimer();
            }
            else if (eventIsSet(TIMER_FINISHED))
            {
                turnRight();
                turnRight();
                driveForward(DRIVE_LENGTH);
                turnLeft();
                currentState = "M0";
                restartTimer();
                clearEvent(TIMER_FINISHED);
            }
            break;
        case "L0":
            if (eventIsSet(CLICK_LEFT))
            {
                driveForward(DRIVE_LENGTH);
                currentState = "L-1";
                clearEvent(CLICK_LEFT);
                restartTimer();
            }
            else if (eventIsSet(CLICK_RIGHT))
            {
                turnRight();
                turnRight();
                driveForward(DRIVE_LENGTH);
                currentState = "R1";
                clearEvent(CLICK_RIGHT);
                restartTimer();
            }
            else if (eventIsSet(TIMER_FINISHED))
            {
                turnRight();
                currentState = "M0";
                restartTimer();
                clearEvent(TIMER_FINISHED);
            }
            break;
        case "L1":
            if (eventIsSet(CLICK_LEFT))
            {
                driveForward(DRIVE_LENGTH);
                currentState = "L0";
                clearEvent(CLICK_LEFT);
                restartTimer();
            }
            else if (eventIsSet(CLICK_RIGHT))
            {
                turnRight();
                turnRight();
                driveForward(DRIVE_LENGTH);
                currentState = "R2";
                clearEvent(CLICK_RIGHT);
                restartTimer();
            }
            else if (eventIsSet(TIMER_FINISHED))
            {
                driveForward(DRIVE_LENGTH);
                turnRight();
                currentState = "M0";
                restartTimer();
                clearEvent(TIMER_FINISHED);
            }
            break;
        case "L2":
            if (eventIsSet(CLICK_LEFT))
            {
                driveForward(DRIVE_LENGTH);
                currentState = "L1";
                clearEvent(CLICK_LEFT);
                restartTimer();
            }
            else if (eventIsSet(CLICK_RIGHT))
            {
                turnRight();
                turnRight();
                driveForward(DRIVE_LENGTH);
                currentState = "R3";
                clearEvent(CLICK_RIGHT);
                restartTimer();
            }
            else if (eventIsSet(TIMER_FINISHED))
            {
                driveForward(DRIVE_LENGTH * 2);
                turnRight();
                currentState = "M0";
                restartTimer();
                clearEvent(TIMER_FINISHED);
            }
            break;

        // Facing up
        case "M0":
            if (eventIsSet(CLICK_LEFT))
            {
                turnLeft();
                driveForward(DRIVE_LENGTH);
                currentState = "L-1";
                clearEvent(CLICK_LEFT);
                restartTimer();
            }
            else if (eventIsSet(CLICK_RIGHT))
            {
                turnRight();
                driveForward(DRIVE_LENGTH);
                currentState = "R1";
                clearEvent(CLICK_RIGHT);
                restartTimer();
            }
            else if (eventIsSet(TIMER_FINISHED))
            {
                restartTimer();
                clearEvent(TIMER_FINISHED);
            }
            break;

        // Facing right
        case "R3":
            if (eventIsSet(CLICK_LEFT))
            {
                turnLeft();
                turnLeft();
                driveForward(DRIVE_LENGTH);
                currentState = "L2";
                clearEvent(CLICK_LEFT);
                restartTimer();
            }
            else if (eventIsSet(CLICK_RIGHT))
            {
                clearEvent(CLICK_RIGHT);
                restartTimer();
            }
            else if (eventIsSet(TIMER_FINISHED))
            {
                turnLeft();
                turnLeft();
                driveForward(DRIVE_LENGTH * 3);
                turnRight();
                currentState = "M0";
                restartTimer();
                clearEvent(TIMER_FINISHED);
            }
            break;
        case "R2":
            if (eventIsSet(CLICK_LEFT))
            {
                turnLeft();
                turnLeft();
                driveForward(DRIVE_LENGTH);
                currentState = "L1";
                clearEvent(CLICK_LEFT);
                restartTimer();
            }
            else if (eventIsSet(CLICK_RIGHT))
            {
                driveForward(DRIVE_LENGTH);
                currentState = "R3";
                clearEvent(CLICK_RIGHT);
                restartTimer();
            }
            else if (eventIsSet(TIMER_FINISHED))
            {
                turnLeft();
                turnLeft();
                driveForward(DRIVE_LENGTH * 2);
                turnRight();
                currentState = "M0";
                restartTimer();
                clearEvent(TIMER_FINISHED);
            }
            break;
        case "R1":
            if (eventIsSet(CLICK_LEFT))
            {
                turnLeft();
                turnLeft();
                driveForward(DRIVE_LENGTH);
                currentState = "L0";
                clearEvent(CLICK_LEFT);
                restartTimer();
            }
            else if (eventIsSet(CLICK_RIGHT))
            {
                driveForward(DRIVE_LENGTH);
                currentState = "R2";
                clearEvent(CLICK_RIGHT);
                restartTimer();
            }
            else if (eventIsSet(TIMER_FINISHED))
            {
                turnLeft();
                turnLeft();
                driveForward(DRIVE_LENGTH);
                turnRight();
                currentState = "M0";
                restartTimer();
                clearEvent(TIMER_FINISHED);
            }
            break;
        case "R0":
            if (eventIsSet(CLICK_LEFT))
            {
                turnLeft();
                turnLeft();
                driveForward(DRIVE_LENGTH);
                currentState = "L-1";
                clearEvent(CLICK_LEFT);
                restartTimer();
            }
            else if (eventIsSet(CLICK_RIGHT))
            {
                driveForward(DRIVE_LENGTH);
                currentState = "R1";
                clearEvent(CLICK_RIGHT);
                restartTimer();
            }
            else if (eventIsSet(TIMER_FINISHED))
            {
                turnLeft();
                currentState = "M0";
                restartTimer();
                clearEvent(TIMER_FINISHED);
            }
            break;
        case "R-1":
            if (eventIsSet(CLICK_LEFT))
            {
                turnLeft();
                turnLeft();
                driveForward(DRIVE_LENGTH);
                currentState = "L-2";
                clearEvent(CLICK_LEFT);
                restartTimer();
            }
            else if (eventIsSet(CLICK_RIGHT))
            {
                driveForward(DRIVE_LENGTH);
                currentState = "R0";
                clearEvent(CLICK_RIGHT);
                restartTimer();
            }
            else if (eventIsSet(TIMER_FINISHED))
            {
                driveForward(DRIVE_LENGTH);
                turnLeft();
                currentState = "M0";
                restartTimer();
                clearEvent(TIMER_FINISHED);
            }
            break;
        case "R-2":
            if (eventIsSet(CLICK_LEFT))
            {
                turnLeft();
                turnLeft();
                driveForward(DRIVE_LENGTH);
                currentState = "L-3";
                clearEvent(CLICK_LEFT);
                restartTimer();
            }
            else if (eventIsSet(CLICK_RIGHT))
            {
                driveForward(DRIVE_LENGTH);
                currentState = "R-1";
                clearEvent(CLICK_RIGHT);
                restartTimer();
            }
            else if (eventIsSet(TIMER_FINISHED))
            {
                driveForward(DRIVE_LENGTH * 2);
                turnLeft();
                currentState = "M0";
                restartTimer();
                clearEvent(TIMER_FINISHED);
            }
            break;

        default:
            return 0;
            break;
        }

        sprintf(dispStr, "%d  ", currentState);
        NNXT_LCD_DisplayStringAtLine(0, currentState);
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
