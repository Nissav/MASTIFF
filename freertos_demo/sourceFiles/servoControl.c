#include "header.h"

void servoCont(void* data) {
      servoContData* myData = (servoContData*) data;
      while(1){
        //When a request has been made to change the servo position then
        //move to the new position
        if(*(myData->servoCount) != *(myData->servoCountOld)){
          if(*(myData->servoCount) > *(myData->servoCountOld)){
            *(myData->servoPulse) = SERVO_MOVE_PULSES * (*(myData->servoCount) - *(myData->servoCountOld));
          }else{
            *(myData->servoPulse) = SERVO_MOVE_PULSES * (*(myData->servoCountOld) - *(myData->servoCount));
          }
          //Update the target position
          *(myData->servoCountOld) = *(myData->servoCount);
          //Disallow range finder measurements while moving
          GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
          //Suspend task until servo in position
          vTaskSuspend(NULL);
        }
        *(myData->servoPulse) = SERVO_HOLD_PULSES;
        //Make the array index corresponding to the current position active
        switch (*(myData->servoCountOld)){
            case SERVO_0:     *(myData->servoPosition) = 0;
                              break;
            case SERVO_18:    *(myData->servoPosition) = 1;
                              break;
            case SERVO_36:    *(myData->servoPosition) = 2;
                              break;
            case SERVO_54:    *(myData->servoPosition) = 3;
                              break;
            case SERVO_72:    *(myData->servoPosition) = 4;
                              break;
            case SERVO_90:    *(myData->servoPosition) = 5;
                              break;
            case SERVO_108:   *(myData->servoPosition) = 6;
                              break;
            case SERVO_126:   *(myData->servoPosition) = 7;
                              break;
            case SERVO_144:   *(myData->servoPosition) = 8;
                              break;
            case SERVO_162:   *(myData->servoPosition) = 9;
                              break;
            case SERVO_180:   *(myData->servoPosition) = 10;
                              break;
            default:          *(myData->servoPosition) = 0;
                              break;
        }
        //Allow range finder measurements while still
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0xFF);
        //Suspend task until measurement collected
        vTaskSuspend(NULL);
      }
}

//*****************************************************************************
//
// Initializes the servoControl task.
//
//*****************************************************************************
int
servoControlInit(xTaskHandle* xServoControlHandle, servoContData* myServoData)
{
    //
    // Create the servoControl task.
    //
    if(xTaskCreate(servoCont, (signed portCHAR *)"Servo Control", SERVO_STACK_SIZE, 
                   myServoData, SERVO_CONT_PRIORITY, xServoControlHandle) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}