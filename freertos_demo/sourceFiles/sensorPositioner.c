#include "header.h"

void sensorPositioner(void* data) {
      sensorPositionerData* myData = (sensorPositionerData*) data;
      while(1){
        //Sweep
        *(myData->servoCount) = SERVO_0;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_10;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_20;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_30;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_40;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_50;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_60;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_70;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_80;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_90;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_100;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_110;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_120;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_130;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_140;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_150;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_160;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_170;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_180;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_170;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_160;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_150;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_140;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_130;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_120;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_110;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_100;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_90;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_80;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_70;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_60;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_50;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_40;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_30;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_20;
        vTaskSuspend(NULL);
        *(myData->servoCount) = SERVO_10;
        vTaskSuspend(NULL);
      }
}

//*****************************************************************************
//
// Initializes the sensorPositioner task.
//
//*****************************************************************************
int
sensorPositionerInit(xTaskHandle* xSensorPositionerHandle, sensorPositionerData* mySensorPositionerData)
{
    //
    // Create the sensorPositioner task.
    //
    if(xTaskCreate(sensorPositioner, (signed portCHAR *)"Sensor Positioner", SENSOR_POSITIONER_STACK_SIZE, 
                   mySensorPositionerData, SENSOR_POSITIONER_PRIORITY, xSensorPositionerHandle) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}