#include "header.h"

void cameraPositioner(void* data) {
  cameraPositionerData* myData = (cameraPositionerData*) data;
  while(1){
    
    vTaskSuspend(NULL);
  }
}

//*****************************************************************************
//
// Initializes the cameraPositioner task.
//
//*****************************************************************************
int
cameraPositionerInit(xTaskHandle* xCameraPositionerHandle, cameraPositionerData* myCameraPositionerData)
{
  //
  // Create the cameraPositioner task.
  //
  if(xTaskCreate(cameraPositioner, (signed portCHAR *)"Camera Positioner", CAMERA_POSITIONER_STACK_SIZE, 
                 myCameraPositionerData, CAMERA_POSITIONER_PRIORITY, xCameraPositionerHandle) != pdTRUE)
  {
    return(1);
  }
  
  //
  // Success.
  //
  return(0);
}