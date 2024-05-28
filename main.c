#include "nnxt.h"
#define IR_SENSOR Port_3

const int16_t correctionPoints[][2] = {
    {1000, 20000},
    {1100, 8000},
    {1250, 5000},
    {1350, 4000},
    {1400, 3000},
    {1600, 2500},
    {1800, 2000},
    {2000, 1500},
    {2600, 1000},
    {4000, 500},
    {4100, 0}
    };

const uint8_t correctionPointsLength = sizeof(correctionPoints) / sizeof(correctionPoints[0]);

uint16_t calcDistance(uint16_t distance)
{
    char msg[80];
    // Find index where reference distance is just below actual distance
    uint8_t index = 0;
    while (index < correctionPointsLength && correctionPoints[index][0] < distance)
    {
        index++;
    }

    // If distance is below the first reference point, return the first reference point
    if (index == 0)
    {
        return correctionPoints[0][1];
    }

    // Calculate slope
    uint8_t lowerIndex = index - 1;
    int16_t slope = ((int32_t)(correctionPoints[index][1] - correctionPoints[lowerIndex][1]) * 1000) / (correctionPoints[index][0] - correctionPoints[lowerIndex][0]);

    sprintf(msg, " index: %d | slope: %d  -> res: %d   \n", index, slope, (correctionPoints[lowerIndex][1] * 1000 + slope * (distance - correctionPoints[lowerIndex][0])) / 1000);
    printf(msg);

    sprintf(msg, " i: %d", index);
    NNXT_LCD_DisplayStringAtLine(4, msg);

    sprintf(msg, " s: %d", slope);
    NNXT_LCD_DisplayStringAtLine(5, msg);

    // Linear interpolation
    return (correctionPoints[lowerIndex][1] * 1000 + slope * (distance - correctionPoints[lowerIndex][0])) / 1000;
}

int main()
{
    uint16_t value;
    uint16_t distance;
    char msg[40];
    while (1)
    {
        SensorADCWithFilter(IR_SENSOR, &value);
        sprintf(msg, "%d   ", value);
        NNXT_LCD_DisplayStringAtLine(0, msg);
        distance = calcDistance(value);
        sprintf(msg, "%d  ", distance);
        NNXT_LCD_DisplayStringAtLine(1, msg);
        Delay(250);
    }
    return 0;
}
