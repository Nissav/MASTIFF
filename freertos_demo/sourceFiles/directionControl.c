#include "header.h"

void directionCont(void* data) {
    directionContData* myData = (directionContData*) data;
    unsigned int oldSpeed;
    
    while (1) {
      if(!(*(myData->goLeft) || *(myData->goRight))) {
          //If overhang then stop
          if((*(myData->upDistance)) < UP_THRESHOLD) {
              *(myData->speed) = STOP;
              *(myData->overhangFlag) = true;
              vTaskResume(*(myData->xMotorControlHandle));
          }
          //If drop then stop
          if((*(myData->downDistance)) > DOWN_THRESHOLD) {
              *(myData->speed) = STOP;
              *(myData->dropOffFlag) = true;
              vTaskResume(*(myData->xMotorControlHandle));
          }
          //If obstacle then stop
          if(((myData->distances[5]) < STRAIGHT_THRESHOLD) && !(*myData->stuckFlag)) {
              oldSpeed = *(myData->speed);
              *(myData->speed) = STOP;
              *(myData->stuckFlag) = true;
              vTaskResume(*(myData->xMotorControlHandle));
          }
          
          
          //Detects if wall or obstacle is upcoming
          if((*(myData->leftVeer) || *(myData->rightVeer) || *(myData->leftCorrectStraight) || *(myData->rightCorrectStraight) || *(myData->leftCorrectRight) || *(myData->rightCorrectLeft) || *(myData->leftCorrectStraightAgain) || *(myData->rightCorrectStraightAgain)) || (myData->distances[5]) < STRAIGHT_THRESHOLD * 2.0) {
              //Room to maneuver around obstacle to left
             if((!(*(myData->leftVeer) || *(myData->rightVeer) || *(myData->leftCorrectStraight) || *(myData->rightCorrectStraight) || *(myData->leftCorrectRight) || *(myData->rightCorrectLeft) || *(myData->leftCorrectStraightAgain) || *(myData->rightCorrectStraightAgain))) && ((myData->distances[2]) > STRAIGHT_THRESHOLD / 1.4)) {
                  *(myData->turnRad) = 45;
                  *(myData->obstacleFlag) = true;
                  *(myData->leftVeer) = true;
                  *(myData->startCorrectionCount) = *(myData->globalCount);
                  vTaskResume(*(myData->xMotorControlHandle));
              } 
              //Room to maneuver around obstacle to right
              else if((!(*(myData->leftVeer) || *(myData->rightVeer) || *(myData->leftCorrectStraight) || *(myData->rightCorrectStraight) || *(myData->leftCorrectRight) || *(myData->rightCorrectLeft) || *(myData->leftCorrectStraightAgain) || *(myData->rightCorrectStraightAgain))) && ((myData->distances[8]) > STRAIGHT_THRESHOLD / 1.4)) {
                  *(myData->turnRad) = 135;
                  *(myData->obstacleFlag) = true;
                  *(myData->rightVeer) = true;
                  *(myData->startCorrectionCount) = *(myData->globalCount);
                  vTaskResume(*(myData->xMotorControlHandle));
              } 
              else if(*(myData->leftVeer)){
                if(*(myData->globalCount) - *(myData->startCorrectionCount) > 20000){
                  *(myData->turnRad) = 135;
                  *(myData->leftVeer) = false;
                  *(myData->leftCorrectStraight) = true;
                  *(myData->startCorrectionCount) = *(myData->globalCount);
                  vTaskResume(*(myData->xMotorControlHandle));
                }
              }
              else if(*(myData->rightVeer)){
                if(*(myData->globalCount) - *(myData->startCorrectionCount) > 20000){
                  *(myData->turnRad) = 45;
                  *(myData->rightVeer) = false;
                  *(myData->rightCorrectStraight) = true;
                  *(myData->startCorrectionCount) = *(myData->globalCount);
                  vTaskResume(*(myData->xMotorControlHandle));
                }
              }
              else if(*(myData->leftCorrectStraight)){
                if(*(myData->globalCount) - *(myData->startCorrectionCount) > 20000){
                  *(myData->turnRad) = 135;
                  *(myData->leftCorrectStraight) = false;
                  *(myData->leftCorrectRight) = true;
                  *(myData->startCorrectionCount) = *(myData->globalCount);
                  vTaskResume(*(myData->xMotorControlHandle));
                }
              }
              else if(*(myData->rightCorrectStraight)){
                if(*(myData->globalCount) - *(myData->startCorrectionCount) > 20000){
                  *(myData->turnRad) = 45;
                  *(myData->rightCorrectStraight) = false;
                  *(myData->rightCorrectLeft) = true;
                  *(myData->startCorrectionCount) = *(myData->globalCount);
                  vTaskResume(*(myData->xMotorControlHandle));
                }
              }
              else if(*(myData->leftCorrectRight)){
                if(*(myData->globalCount) - *(myData->startCorrectionCount) > 20000){
                  *(myData->turnRad) = 45;
                  *(myData->leftCorrectRight) = false;
                  *(myData->leftCorrectStraightAgain) = true;
                  *(myData->startCorrectionCount) = *(myData->globalCount);
                  vTaskResume(*(myData->xMotorControlHandle));
                }
              }
              else if(*(myData->rightCorrectLeft)){
                if(*(myData->globalCount) - *(myData->startCorrectionCount) > 20000){
                  *(myData->turnRad) = 135;
                  *(myData->rightCorrectLeft) = false;
                  *(myData->rightCorrectStraightAgain) = true;
                  *(myData->startCorrectionCount) = *(myData->globalCount);
                  vTaskResume(*(myData->xMotorControlHandle));
                }
              }
              else if(*(myData->leftCorrectStraightAgain)){
                if(*(myData->globalCount) - *(myData->startCorrectionCount) > 20000){
                  *(myData->leftCorrectStraightAgain) = false;
                  *(myData->startCorrectionCount) = 0;
                  vTaskResume(*(myData->xMotorControlHandle));
                }
              }
              else if(*(myData->rightCorrectStraightAgain)){
                if(*(myData->globalCount) - *(myData->startCorrectionCount) > 20000){
                  *(myData->rightCorrectStraightAgain) = false;
                  *(myData->startCorrectionCount) = 0;
                  vTaskResume(*(myData->xMotorControlHandle));
                }
              }
              //obstacle is unavoidable
              else {
                  *(myData->wallFlag) = true;
              }
          }
        } 
      if(*(myData->goLeft)) {
            //If left turn available then turn left
            if((myData->distances[0]) > TURN_THRESHOLD) {
                *(myData->turnRad) = 0;
                *(myData->leftTurnFlag) = true;
                *(myData->goLeft) = false;
                if(*(myData->stuckFlag)){
                  *(myData->speed) = oldSpeed;
                }
                *(myData->stuckFlag) = false;
                vTaskResume(*(myData->xMotorControlHandle));
            }
        }
      if(*(myData->goRight)) {
            //If right turn available then turn right
            if((myData->distances[10]) > TURN_THRESHOLD) {
                *(myData->turnRad) = 180;
                *(myData->rightTurnFlag) = true;
                *(myData->goRight) = false;
                if(*(myData->stuckFlag)){
                  *(myData->speed) = oldSpeed;
                }
                *(myData->stuckFlag) = false;
                vTaskResume(*(myData->xMotorControlHandle));
            }
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