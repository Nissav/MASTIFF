#include "header.h"

void directionCont(void* data) {
  directionContData* myData = (directionContData*) data;
  static unsigned int oldSpeed = 0;
  static unsigned int testCount = 0; //TEST
  static unsigned int testStage = 0; //TEST
  while (1) { 
    /*
    //Map routine
    if(*(myData->routine) == MAP_R){
      //If overhang then stop
      if((*(myData->upDistance)) < UP_THRESHOLD && !(*(myData->overhangFlag))) {
        oldSpeed = *(myData->speed);
        *(myData->speed) = STOP;
        *(myData->overhangFlag) = true;
        vTaskResume(*(myData->xMotorControlHandle));
      }
      //If drop then stop
      if((*(myData->downDistance)) > DOWN_THRESHOLD && !(*(myData->dropOffFlag))) {
        oldSpeed = *(myData->speed);
        *(myData->speed) = STOP;
        *(myData->dropOffFlag) = true;
        vTaskResume(*(myData->xMotorControlHandle));
      }
      //If obstacle then stop
      if(((myData->distances[9]) < STRAIGHT_THRESHOLD) && !(*(myData->stuckFlag))) {
        oldSpeed = *(myData->speed);
        *(myData->speed) = STOP;
        *(myData->stuckFlag) = true;
        vTaskResume(*(myData->xMotorControlHandle));
      }
      //If left turn requested then turn left
      if(*(myData->goLeft)) {
        if((myData->distances[0]) > TURN_THRESHOLD) {
          *(myData->turnRad) = 0;
          *(myData->leftTurnFlag) = true;
          *(myData->goLeft) = false;
          if((*(myData->stuckFlag)) || (*(myData->dropOffFlag)) || (*(myData->overhangFlag))){
            *(myData->speed) = oldSpeed;
          }
          vTaskResume(*(myData->xMotorControlHandle));
        }
      }
      //If right turn requested then turn right
      if(*(myData->goRight)) {
        if((myData->distances[18]) > TURN_THRESHOLD) {
          *(myData->turnRad) = 180;
          *(myData->rightTurnFlag) = true;
          *(myData->goRight) = false;
          if((*(myData->stuckFlag)) || (*(myData->dropOffFlag)) || (*(myData->overhangFlag))){
            *(myData->speed) = oldSpeed;
          }
          vTaskResume(*(myData->xMotorControlHandle));
        }
      }
      
      //When an overhang, drop, or obstacle is encountered turn left (if no corrective action is already being taken)
      //Take every available right turn when going straight (if no corrective action is already being taken)
      if(((*(myData->stuckFlag)) || (*(myData->dropOffFlag)) || (*(myData->overhangFlag))) && !(*(myData->rightTurnFlag)) && !(*(myData->leftTurnFlag))){
        *(myData->goLeft) = true;
      }else if(((myData->distances[18]) > TURN_THRESHOLD) && !(*(myData->rightTurnFlag)) && !(*(myData->leftTurnFlag))){
        *(myData->goRight) = true;
      }
    }
    //Search routine
    else if(*(myData->routine) == SEARCH_R){
      
    }
    //Dock routine
    else if(*(myData->routine) == DOCK_R){
      
    }
    //Idle routine
    else if(*(myData->routine) == IDLE_R){
      
    }
    //Default routine
    else{
      
    }
    */
    //TEST
    *(myData->speed) = FULL_SPEED;
    if(*(myData->updateMotorControl)){
      testCount++;
      if(testCount < 20){
        if(testStage != 1){
          testStage = 1;
          *(myData->turnRad) = 90;
        }
      }else if(testCount < 40){
        if(testStage != 2){
          testStage = 2;
          *(myData->turnRad) = 180;
        }
      }else if(testCount < 60){
        if(testStage != 3){
          testStage = 3;
          *(myData->turnRad) = 90;
        }
      }else if(testCount < 80){
        if(testStage != 4){
          testStage = 4;
          *(myData->turnRad) = 0;
        }
      }else{
        testCount = 0;
      }
      vTaskResume(*(myData->xMotorControlHandle));
      *(myData->updateMotorControl) = false;
    }
  }
}

//*****************************************************************************
//
// Initializes the directionControl task.
//
//*****************************************************************************
int
directionControlInit(xTaskHandle* xDirectionControlHandle, directionContData* myDirectionData)
{
  //
  // Create the directionControl task.
  //
  if(xTaskCreate(directionCont, (signed portCHAR *)"Direction Control", DIRECTION_STACK_SIZE, 
                 myDirectionData, DIRECTION_CONT_PRIORITY, xDirectionControlHandle) != pdTRUE)
  {
    return(1);
  }
  
  //
  // Success.
  //
  return(0);
}