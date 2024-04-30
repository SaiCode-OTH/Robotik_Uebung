#include "nnxt.h"
#include "nnxt_pcf8574lb.h"
#include "stdbool.h"
#include "event.h"
#include "timer.h"

#define I2C_PORT Port_3

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

void baguette() {
    uint32_t freq = 1000;
    char dispMsg[100];
    bool ledOn = false;
    setTimer(0, freq, TIMER_FINISHED);
    startTimer(0);
    while(1){
        if(eventIsSet(TIMER_FINISHED)){
            sprintf(dispMsg, "Freqency : %d Hz   ", freq/1000);
            NNXT_LCD_DisplayStringAtLine(0, dispMsg);
            ledOn = !ledOn;
            if(ledOn){
                WritePort(I2C_PORT, 0);
            } else {
                DeletePort(I2C_PORT, 0);
            }
            cancelTimer(0);
            startTimer(0);
            clearEvent(TIMER_FINISHED);
        }
        if(eventIsSet(LEFT_CLICK)){
            freq /= 2;
            sprintf(dispMsg, "Sub: %d     ", freq);
            NNXT_LCD_DisplayStringAtLine(1, dispMsg);
            cancelTimer(0);
            setTimer(0, freq, TIMER_FINISHED);
            clearEvent(TIMER_FINISHED);
            startTimer(0);
            clearEvent(LEFT_CLICK);
        }
        if(eventIsSet(RIGHT_CLICK)){
            freq *= 2;
            sprintf(dispMsg, "Add: %d     ", freq);
            NNXT_LCD_DisplayStringAtLine(1, dispMsg);
            cancelTimer(0);
            setTimer(0, freq, TIMER_FINISHED);
            clearEvent(TIMER_FINISHED);
            startTimer(0);
            clearEvent(RIGHT_CLICK);
        }
    }

}


int main()
{
    SensorConfig(TOUCH_LEFT, SensorTouch);
    SensorConfig(TOUCH_RIGHT, SensorTouch);

    CreateAndStartTask(timerTask);

    CreateAndStartTask(onLeftSensorClick);
    CreateAndStartTask(onRightSensorClick);
    CreateAndStartTask(baguette);

    StartScheduler();

    return 0;
}
