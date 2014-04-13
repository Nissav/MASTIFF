//*****************************************************************************
//
//      PIN RESERVATIONS/DEFINITIONS
//
//      PORT A:
//      PA2:    Motor Control, Left Wheels Speed
//      PA3:    Motor Control, Right Wheels Speed
//      PA4:    Motor Control, DAC load bit (clock)
//      PA5:    Motor Control, DAC output data (chip select)
//
//      PORT B:
//      PB2:    motorDirDecode LSB (in0 input for GAL)
//      PB3:    
//
//      PORT C:
//      PC4:    Range Finder (down) interrupt
//      PC5:    Range Finder (up) interrupt
//      PC6:    Range Finder (sweep) interrupt
//      PC7:    Range Finder (sweep) trigger 
//
//      PORT D:
//      PD6:    Wheel Turn Count, Left
//      PD7:    Wheel Turn Count, Right
//
//      PORT E:
//      PE0:    Servo Control
//      PE4:    motorDirDecode MSB (in1 input for GAL)
//
//      PORT F:
//      PF0:    Pan Servo Control
//      PF2:    Tilt Servo Control
//      PF3:    
//      PF4:    
//
//*****************************************************************************

#include "header.h"

//*****************************************************************************
//
//      INITIALIZATION OF VARIABLES AND DATA
//
//*****************************************************************************

//Task Handles
static xTaskHandle      xMotorControlHandle;
static xTaskHandle      xServoControlHandle;
static xTaskHandle      xServoControlHandlePan;
static xTaskHandle      xServoControlHandleTilt;
static xTaskHandle      xDirectionControlHandle;
static xTaskHandle      xSensorPositionerHandle;
static xTaskHandle      xCameraPositionerHandle;
static xTaskHandle      xCommHandle;

//Global Variables
//Time (100 us per count)
static unsigned long    globalCount     = 0;

//Motor control
static unsigned int     speed           = FULL_SPEED;
static unsigned int     turnRad         = 90;
static unsigned int     updateMotorControlCount = 0;
static bool             updateMotorControl = true;

//Sensor servo control
static unsigned int     servoCount      = SERVO_90;
static unsigned int     servoCountOld   = SERVO_90;
static unsigned int     servoCurCount   = 0;
static unsigned int     servoPulse      = SERVO_MOVE_PULSES;
static unsigned int     servoCurPulse   = 0;

static unsigned int     servoPosition   = 9;
static unsigned int     distances[19]   = {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
static unsigned int     upDistance      = 100;
static unsigned int     downDistance    = 0;

//Camera pan servo control
static unsigned int     servoCountPan      = SERVO_90;
static unsigned int     servoCountOldPan   = SERVO_90;
static unsigned int     servoCurCountPan   = 0;
static unsigned int     servoPulsePan      = SERVO_MOVE_PULSES;
static unsigned int     servoCurPulsePan   = 0;

//Camera tilt servo control
static unsigned int     servoCountTilt      = SERVO_90;
static unsigned int     servoCountOldTilt   = SERVO_90;
static unsigned int     servoCurCountTilt   = 0;
static unsigned int     servoPulseTilt      = SERVO_MOVE_PULSES;
static unsigned int     servoCurPulseTilt   = 0;

//Navigation commands
static bool             goRight         = false;
static bool             goLeft          = false;
static unsigned int     routine         = MAP_R;

//Navigation states
static bool             obstacleFlag    = false;
static bool             wallFlag        = false;
static bool             overhangFlag    = false;
static bool             dropOffFlag     = false;
static bool             leftTurnFlag    = false;
static bool             rightTurnFlag   = false;
static bool             stuckFlag       = false;

//Range finder (sweep) expected edge and measured start time and stop time
static unsigned int     lowToHighSweep = 1; //Rising edge of sensor expected if 1, otherwise 0
static unsigned long    pulseStartSweep = 0;
static unsigned long    pulseStopSweep = 0;

//Range finder (up) expected edge and measured start time and stop time
static unsigned int     lowToHighUp = 1; //Rising edge of sensor expected if 1, otherwise 0
static unsigned long    pulseStartUp = 0;
static unsigned long    pulseStopUp = 0;

//Range finder (down) expected edge and measured start time and stop time
static unsigned int     lowToHighDown = 1; //Rising edge of sensor expected if 1, otherwise 0
static unsigned long    pulseStartDown = 0;
static unsigned long    pulseStopDown = 0;

//Count the number of 1/5 turns of the wheels on each side
static signed int       wheelTurnCountLeft = 0;
static signed int       wheelTurnCountRight = 0;

//Rising edge of sensor expected if 1, otherwise 0
static unsigned int     lowToHighLeft = 0;
static unsigned int     lowToHighRight = 0;

//Used to determine completion of a turn
static unsigned int     curTurnRad = 0;
static signed int       wheelTurnCountLeftStart = 0;
static signed int       wheelTurnCountRightStart = 0;
static unsigned int     turnInProgress = 0;

//Used to determine completion of an obstacle avoidance maneuver
static bool               leftVeer = false;
static bool               rightVeer = false;
static bool               leftCorrectStraight = false;
static bool               rightCorrectStraight = false;
static bool               leftCorrectRight = false;
static bool               rightCorrectLeft = false;
static bool               leftCorrectStraightAgain = false;
static bool               rightCorrectStraightAgain = false;
static unsigned long      startCorrectionCount = 0;

//I2C
static unsigned int       commIn = 0;

//Data Set Initializations
static motorContData    myMotorData = {&speed, &turnRad};
static servoContData    myServoData = {&servoCount, &servoCountOld, &servoPulse, &servoPosition, &xSensorPositionerHandle};
static servoContDataPan myServoDataPan = {&servoCountPan, &servoCountOldPan, &servoPulsePan};
static servoContDataTilt myServoDataTilt = {&servoCountTilt, &servoCountOldTilt, &servoPulseTilt};
static directionContData myDirectionControlData  =  {&goRight, &goLeft, &stuckFlag, 
&turnRad, &speed, &obstacleFlag, &wallFlag, &xMotorControlHandle, 
&upDistance, &downDistance, &overhangFlag, &dropOffFlag,
&leftTurnFlag, &rightTurnFlag, distances, &leftVeer, &rightVeer, &leftCorrectStraight, &rightCorrectStraight,
&leftCorrectRight, &rightCorrectLeft, &leftCorrectStraightAgain, &rightCorrectStraightAgain,
&globalCount, &startCorrectionCount, &updateMotorControl, &routine};
static sensorPositionerData mySensorPositionerData = {&servoCount, &servoPosition};
static cameraPositionerData myCameraPositionerData = {&servoCountPan, &servoCountTilt};
static commData myCommData = {&commIn};
static softwareInitData mySoftInitData = {&xMotorControlHandle, &myMotorData, 
&xServoControlHandle, &myServoData, 
&xServoControlHandlePan, &myServoDataPan, 
&xServoControlHandleTilt, &myServoDataTilt, 
&xDirectionControlHandle, &myDirectionControlData,
&xSensorPositionerHandle, &mySensorPositionerData,
&xCameraPositionerHandle, &myCameraPositionerData,
&xCommHandle, &myCommData};

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif

//*****************************************************************************
//
// Initialize FreeRTOS and start the initial set of tasks.
//
//*****************************************************************************
int
main(void)
{
  hardwareInit();
  softwareInit(&mySoftInitData);
  
  //
  // Start the scheduler.  This should not return.
  //
  vTaskStartScheduler();
  
  //
  // In case the scheduler returns for some reason, print an error and loop
  // forever.
  //
  while(1)
  {
  }
}

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
  //
  // This function can not return, so loop forever.  Interrupts are disabled
  // on entry to this function, so no processor interrupts will interrupt
  // this loop.
  //
  while(1)
  {
  }
}

/*
This interrupt establishes a global time base using the variable
globalCount. globalCount increments at a frequency of 10 kHz
(increments every 100 us)
*/
void Timer0IntHandler(void){
  //Clear the timer interrupt
  TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  
  //Increase the global time base
  globalCount++;
  
  //Servo pwm generation (sensor)
  servoCurCount++;
  if(servoCurCount > SERVO_INTERVAL){
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0xFF);
    servoCurCount = 0;
    servoCurPulse++;
  }
  if(servoCurCount > servoCountOld){
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0x00);
  }
  if(servoCurPulse > servoPulse){
    servoCurPulse = 0;
    vTaskResume(xServoControlHandle);
  }
  
  //Servo pwm generation (pan)
  servoCurCountPan++;
  if(servoCurCountPan > SERVO_INTERVAL){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0xFF);
    servoCurCountPan = 0;
    servoCurPulsePan++;
  }
  if(servoCurCountPan > servoCountOldPan){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x00);
  }
  if(servoCurPulsePan > servoPulsePan){
    servoCurPulsePan = 0;
    vTaskResume(xServoControlHandlePan);
  }
  
  //Servo pwm generation (tilt)
  servoCurCountTilt++;
  if(servoCurCountTilt > SERVO_INTERVAL){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0xFF);
    servoCurCountTilt = 0;
    servoCurPulseTilt++;
  }
  if(servoCurCountTilt > servoCountOldTilt){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
  }
  if(servoCurPulseTilt > servoPulseTilt){
    servoCurPulseTilt = 0;
    vTaskResume(xServoControlHandleTilt);
  }
  
  //Motor control update
  updateMotorControlCount++;
  if(updateMotorControlCount > UPDATE_MOTOR_CONTROL_INTERVAL){
    updateMotorControlCount = 0;
    updateMotorControl = true;
  }
}

/*
Wheel turn count interrupt
*/
void PortDIntHandler(void){
  //When the left wheels rotate 1/5 of a turn...
  if((lowToHighLeft == 0) && !GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6)){
    //increment if slight left, straight, right
    if(turnRad >= 45){
      wheelTurnCountLeft++;
    //decrement if sharp left since wheels will be reversing
    }else{
      wheelTurnCountLeft--;
    }
    lowToHighLeft = 1;
  }else if((lowToHighLeft == 1) && GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6)){
    lowToHighLeft = 0;
  }
  //When the right wheels rotate 1/5 of a turn...
  if((lowToHighRight == 0) && !GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7)){
    //increment if slight right, straight, left
    if(turnRad <= 135){
      wheelTurnCountRight++;
    //decrement if sharp right since wheels will be reversing
    }else{
      wheelTurnCountRight--;
    }
    lowToHighRight = 1;
  }else if((lowToHighRight == 1) && GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7)){
    lowToHighRight = 0;
  }
  //If the turnRad has been changed from 90 degrees (not going straight) start monitoring the turn
  if((turnRad != 90) && !turnInProgress){
    turnInProgress = 1;
    wheelTurnCountRightStart = wheelTurnCountRight;
    wheelTurnCountLeftStart = wheelTurnCountLeft;
  }
  //Calculate the current turn angle (left)
  if((turnRad < 90) && turnInProgress){
    if(turnRad  <= 45){
      curTurnRad = 90 - 15 * ((wheelTurnCountRight - wheelTurnCountRightStart) - (wheelTurnCountLeft - wheelTurnCountLeftStart));
    }else{
      curTurnRad = 90 - 15 * ((wheelTurnCountRight - wheelTurnCountRightStart) - (wheelTurnCountLeft - wheelTurnCountLeftStart));
    }
  }
  //Calculate the current turn angle (right)
  if((turnRad > 90) && turnInProgress){
    if(turnRad >= 135){
      curTurnRad = 90 + 15 * ((wheelTurnCountLeft - wheelTurnCountLeftStart) - (wheelTurnCountRight - wheelTurnCountRightStart));
    }else{
      curTurnRad = 90 + 15 * ((wheelTurnCountLeft - wheelTurnCountLeftStart) - (wheelTurnCountRight - wheelTurnCountRightStart));
    }  
  }
  //Once the turn is complete reset all of the turn variables and go straight
  if(((turnRad < 90) && turnInProgress && (curTurnRad <= turnRad)) || ((turnRad > 90) && turnInProgress && (curTurnRad >= turnRad))){
    turnRad = 90;
    curTurnRad = 90;
    turnInProgress = 0;
    rightTurnFlag = false;
    leftTurnFlag = false;
    stuckFlag = false;
    dropOffFlag = false;
    overhangFlag = false;
    vTaskResume(xMotorControlHandle);
  }
  GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
}

/*
Range finder interrupt (sweep, up, and down)
*/
void PortCIntHandler(void){
  // Sweep range finder
  if(lowToHighSweep == 0 && !GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6)){
    lowToHighSweep = 1; //stop time
    pulseStopSweep = globalCount;
    distances[servoPosition] = (pulseStopSweep - pulseStartSweep) * 100 / 58;
  }else if(lowToHighSweep == 1 && GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6)){
    lowToHighSweep = 0; //start time
    pulseStartSweep = globalCount;
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00); //one pulse done
  }
  
  // Up range finder
  if(lowToHighUp == 0 && !GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5)){
    lowToHighUp = 1; //stop time
    pulseStopUp = globalCount;
    upDistance = (pulseStopUp - pulseStartUp) * 100 / 58;
  }else if(lowToHighUp == 1 && GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5)){
    lowToHighUp = 0; //start time
    pulseStartUp = globalCount;
  }
  
  //Down range finder
  if(lowToHighDown == 0 && !GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4)){
    lowToHighDown = 1; //stop time
    pulseStopDown = globalCount;
    downDistance = (pulseStopDown - pulseStartDown) * 100 / 58;
  }else if(lowToHighDown == 1 && GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4)){
    lowToHighDown = 0; //start time
    pulseStartDown = globalCount;
  }
  GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
}