#include "nnxt.h"

void turnLeft (int degree) {
    Motor_Drive(Port_A, Motor_dir_forward, 75);
    Motor_Drive(Port_B, Motor_dir_backward, 75);
    Delay((int)(degree*3.66));
    Motor_Stop(Port_A, Motor_stop_float);
    Motor_Stop(Port_B, Motor_stop_float);
}
void turnRight (int degree) {
    Motor_Drive(Port_A, Motor_dir_backward, 75);
    Motor_Drive(Port_B, Motor_dir_forward, 75);
    Delay((int)(degree*3.66));
    Motor_Stop(Port_A, Motor_stop_float);
    Motor_Stop(Port_B, Motor_stop_float);
}

void driveForward (int delay) {
    Motor_Drive(Port_A, Motor_dir_forward, 75);
    Motor_Drive(Port_B, Motor_dir_forward, 75);
    Delay(delay);
    Motor_Stop(Port_A, Motor_stop_float);
    Motor_Stop(Port_B, Motor_stop_float);
}

int detektiereObjekt(){
    sensor_error_t err;
    uint8_t dist;
    char distStr[20];

    err = US_GetDistance(Port_3,&dist);
    if (err!=sensor_error_NoError){
        NNXT_LCD_DisplayStringAtLine(0, "Sensor Error!!!!   ");
        return -1;
    } else {
        sprintf(distStr, "Entfernung: %d ",(int)dist);
        NNXT_LCD_DisplayStringAtLine(0, distStr);
        return (int)dist;
    }
}


int main()
{
    MotorPortInit(Port_A);
    MotorPortInit(Port_B);
    SensorConfig(Port_3, SensorUS);

    while (1) {
        int dist = detektiereObjekt();
        if( dist == -1 ) {
            return 0;
        }
        else if(dist < 15){
            turnRight(15);
        } else {
            driveForward(50);
        }
    }
    return 0;
}



