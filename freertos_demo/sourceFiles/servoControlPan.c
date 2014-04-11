#include "header.h"

void servoContPan(void* data) {
  servoContDataPan* myData = (servoContDataPan*) data;
  while(1){
    //When a request has been made to change the servo position then
    //move to the new position
    if(*(myData->servoCountPan) != *(myData->servoCountOldPan)){
      if(*(myData->servoCountPan) > *(myData->servoCountOldPan)){
        *(myData->servoPulsePan) = SERVO_MOVE_PULSES * (*(myData->servoCountPan) - *(myData->servoCountOldPan));
      }else{
        *(myData->servoPulsePan) = SERVO_MOVE_PULSES * (*(myData->servoCountOldPan) - *(myData->servoCountPan));
      }
      //Update the target position
      *(myData->servoCountOldPan) = *(myData->servoCountPan);
      //Suspend task until servo in position
      vTaskSuspend(NULL);
    }
    *(myData->servoPulsePan) = SERVO_HOLD_PULSES;
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
servoControlInitPan(xTaskHandle* xServoControlHandlePan, servoContDataPan* myServoDataPan)
{
  //
  // Create the servoControl task.
  //
  if(xTaskCreate(servoContPan, (signed portCHAR *)"Servo Control Pan", SERVO_STACK_SIZE_PAN, 
                 myServoDataPan, SERVO_CONT_PRIORITY_PAN, xServoControlHandlePan) != pdTRUE)
  {
    return(1);
  }
  
  //
  // Success.
  //
  return(0);
}