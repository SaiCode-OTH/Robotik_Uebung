#include "nnxt.h"
#define IR_SENSOR Port_3

const int16_t correctionPoints[][2] = {
    {78, 0},
    {85, 1},
    {106, 2},
    {137, 3},
    {180, 4},
    {270, 5},
    {615, 6}
};

const uint8_t correctionPointsLength = sizeof(correctionPoints) / sizeof(correctionPoints[0]);

uint16_t getDistance(){
    uint16_t distance;
    SensorADCWithFilter(IR_SENSOR, &distance);
    return distance;
}

uint16_t calcDistance(uint16_t distance) {
    // Find index where reference distance is just below actual distance
    uint8_t index = 0;
    while (index < correctionPointsLength && correctionPoints[index][0] < distance) {
        index++;
    }

    // If distance is below the first reference point, return the first reference point
    if (index == 0) {
        return correctionPoints[0][1];
    }
    
    // Calculate slope
    uint8_t lowerIndex = index - 1;
    int16_t slope = ((int32_t)(correctionPoints[index][1] - correctionPoints[lowerIndex][1]) * 1000) / (correctionPoints[index][0] - correctionPoints[lowerIndex][0]);

    // Linear interpolation
    return (correctionPoints[lowerIndex][1] * 1000 + slope * (distance - correctionPoints[lowerIndex][0])) / 1000;
}

int main(){
	uint16_t value;
    uint16_t distance;
	char msg[40];
	while(1)
	{
        value = getDistance();
        sprintf(msg, "%d   ", value);
        NNXT_LCD_DisplayStringAtLine(0, msg);
		distance = calcDistance(value);
		sprintf(msg, "%d  ", ( distance/ 1000));
        NNXT_LCD_DisplayStringAtLine(1, msg);
		NNXT_LCD_DisplayStringAtLine(4 , "WHY ARE WE STILL HERE?");
		Delay(250);
	}
	return 0;

}
