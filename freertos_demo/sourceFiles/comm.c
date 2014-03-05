#include "header.h"

void comm(void* data) {
      commData* myData = (commData*) data;
      uint32_t status = 0;
      uint32_t rx = 0;
      uint8_t tx = 0;
      while(1){
          status = I2CSlaveStatus(I2C1_BASE);
          //See pgs. 178-179 for status meaning
          if(status & I2C_SLAVE_ACT_TREQ){
            I2CSlaveDataPut(I2C1_BASE,tx);
          }
          if(status & I2C_SLAVE_ACT_RREQ){
            rx = I2CSlaveDataGet(I2C1_BASE);
            //TEST!!!
            tx = rx + 1;
          }
      }
}

//*****************************************************************************
//
// Initializes the comm task.
//
//*****************************************************************************
int
commInit(xTaskHandle* xCommHandle, commData* myCommData)
{
    //
    // Create the cameraPositioner task.
    //
    if(xTaskCreate(comm, (signed portCHAR *)"Communications", COMM_STACK_SIZE, 
                   myCommData, COMM_PRIORITY, xCommHandle) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}