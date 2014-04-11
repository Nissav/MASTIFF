#include "header.h"

void servoContTilt(void* data) {
  servoContDataTilt* myData = (servoContDataTilt*) data;
  while(1){
    //When a request has been made to change the servo position then
    //move to the new position
    if(*(myData->servoCountTilt) != *(myData->servoCountOldTilt)){
      if(*(myData->servoCountTilt) > *(myData->servoCountOldTilt)){
        *(myData->servoPulseTilt) = SERVO_MOVE_PULSES * (*(myData->servoCountTilt) - *(myData->servoCountOldTilt));
      }else{
        *(myData->servoPulseTilt) = SERVO_MOVE_PULSES * (*(myData->servoCountOldTilt) - *(myData->servoCountTilt));
      }
      //Update the target position
      *(myData->servoCountOldTilt) = *(myData->servoCountTilt);
      //Suspend task until servo in position
      vTaskSuspend(NULL);
    }
    *(myData->servoPulseTilt) = SERVO_HOLD_PULSES;
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
servoControlInitTilt(xTaskHandle* xServoControlHandleTilt, servoContDataTilt* myServoDataTilt)
{
  //
  // Create the servoControl task.
  //
  if(xTaskCreate(servoContTilt, (signed portCHAR *)"Servo Control Tilt", SERVO_STACK_SIZE_TILT, 
                 myServoDataTilt, SERVO_CONT_PRIORITY_TILT, xServoControlHandleTilt) != pdTRUE)
  {
    return(1);
  }
  
  //
  // Success.
  //
  return(0);
}