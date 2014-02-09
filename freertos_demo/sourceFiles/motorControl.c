#include "header.h"

void motorControl(void* data) {
    motorContData* myData = (motorContData*) data;

    //These three variables are local to this function:
    unsigned int leftSpeed = 0;
    unsigned int rightSpeed = 0;
    unsigned int wheelDir = 0;  //Represents a state of the wheels, see motorDirDecode.v
    int i;

    while(1) {
    if((*(myData->turnRad)) / 90 || (*(myData->turnRad) == 90)) { //Right turn
      leftSpeed = *(myData->speed);
      switch (((*(myData->turnRad)) - 90) / 15) {
          case 0:       rightSpeed = *(myData->speed);
                        wheelDir = 2;
                            break;
          case 1:       rightSpeed = 2 * (*(myData->speed)) / 3 ; // FIFTEEN_DEG_TURN;
                        wheelDir = 2;
                            break;
          case 2:       rightSpeed = ((*(myData->speed)) / 3); // THIRTY_DEG_TURN;
                        wheelDir = 2;               
                            break;
          case 3:       rightSpeed = STOP; //*(myData->speed) / 2;// FOURTYFIVE_DEG_TURN;
                        wheelDir = 2;
                            break;
          case 4:       rightSpeed = (*(myData->speed)) / 3;// SIXTY_DEG_TURN;
                        wheelDir = 1;
                            break;
          case 5:       rightSpeed = 2 * (*(myData->speed)) / 3;// SEVENTYFIVE_DEG_TURN;
                        wheelDir = 1;
                            break;
          case 6:       rightSpeed = (*(myData->speed)); // NINETY_DEG_TURN;
                        wheelDir = 1;
                            break;
          default:      rightSpeed = *(myData->speed);   
                        wheelDir = 2;
                            break;
      }
    } else {  //Left turn
          rightSpeed = *(myData->speed);
      switch ((90 - (*(myData->turnRad))) / 15) {
          case 0:       leftSpeed = *(myData->speed);
                        wheelDir = 2;
                            break;
          case 1:       leftSpeed = (2 * (*(myData->speed)) / 3) ; // FIFTEEN_DEG_TURN;
                        wheelDir = 2;
                            break;
          case 2:       leftSpeed = ((*(myData->speed)) / 3); // THIRTY_DEG_TURN;
                        wheelDir = 2;
                            break;
          case 3:       leftSpeed = STOP;//(*(myData->speed)) / 2;// FOURTYFIVE_DEG_TURN;
                        wheelDir = 2;
                            break;
          case 4:       leftSpeed = ((*(myData->speed)/ 3));// SIXTY_DEG_TURN;
                        wheelDir = 0;
                            break;
          case 5:       leftSpeed = (2 * (*(myData->speed)) / 3);// SEVENTYFIVE_DEG_TURN;
                        wheelDir = 0;
                            break;
          case 6:       leftSpeed = (*(myData->speed)); // NINETY_DEG_TURN;
                        wheelDir = 0;
                            break;
          default:      leftSpeed = *(myData->speed);   
                        wheelDir = 2;
                            break;
      }
    }
    //Load data MSB first
    for(i = 0; i < 10; i++) {
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, (leftSpeed & (0x200 >> i)) ? (0xFF) : (0x00));
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, (rightSpeed & (0x200 >> i)) ? (0xFF) : (0x00));
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0xFF);
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
    }
    
    //Load 2 dummy bits
    for(i = 0; i < 2; i++) {
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0xFF);
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
    }
    
    //Pulse the chip-select output
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0xFF);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x00);
    
    switch(wheelDir) {
        case 0:
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);
                    break;
        case 1:
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
                    break;
        case 2:
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0xFF);
                GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);
                    break;
        case 3:
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0xFF);
                GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
                    break;
        default:
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0xFF);
                GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);
                    break;
    }

    //Suspend task until bearings change again
    vTaskSuspend(NULL);
    }
}

//*****************************************************************************
//
// Initializes the motorControl task.
//
//*****************************************************************************
int
motorControlInit(xTaskHandle* xMotorControlHandle, motorContData* myMotorData)
{
    //
    // Create the motorControl task.
    //
    if(xTaskCreate(motorControl, (signed portCHAR *)"Motor Control", MOTOR_STACK_SIZE, 
                   myMotorData, MOTOR_CONT_PRIORITY, xMotorControlHandle) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}