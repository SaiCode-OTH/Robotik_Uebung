#include "nnxt.h"
#include "stdbool.h"

#define MAX_TIMERS 16
typedef uint8_t EventType;
typedef uint8_t TimerType;

typedef struct
{
    uint32_t interval;
    uint32_t t_end;
    uint8_t isRunning;
    EventType triggerEvent;
} timerObject;

volatile timerObject timerArr[MAX_TIMERS] = {0};

bool isRunningTimer(TimerType timer)
{
    return timerArr[timer].isRunning;
}

bool setTimer(TimerType timer, uint32_t interval, EventType ev)
{
    if (!isRunningTimer(timer))
    {
        timerArr[timer].interval = interval;
        timerArr[timer].t_end = NULL;
        timerArr[timer].isRunning = 0;
        timerArr[timer].triggerEvent = ev;
        return true;
    }
    return false;
}

bool startTimer(TimerType timer)
{
    if (!isRunningTimer(timer))
    {
        timerArr[timer].t_end = GetSysTime() + timerArr[timer].interval;
        timerArr[timer].isRunning = 1;
        return true;
    }
    return false;
}

void cancelTimer(TimerType timer)
{
    timerArr[timer].isRunning = 0;
}

void timerTask()
{
    while (1)
    {
        for (uint8_t i = 0; i < MAX_TIMER; i++)
        {
            if (isRunningTimer(i))
            {
                if (GetSysTime() >= timerArr[i].t_end)
                {
                    setEvent(timerArr[i].triggerEvent);
                    cancelTimer(i);
                }
            }
        }
    }
}