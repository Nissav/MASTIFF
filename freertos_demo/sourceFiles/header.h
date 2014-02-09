#ifndef HEADER_H
#define HEADER_H

//Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

//Kernel includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//Hardware library includes
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h" 
#include "driverlib/timer.h"
#include "drivers/buttons.h"
#include "priorities.h"
#include "driverlib/interrupt.h"
#include "inc/hw_gpio.h"
#include "driverlib/qei.h"

//Task Priorities
#define MOTOR_CONT_PRIORITY     5
#define SERVO_CONT_PRIORITY     4
#define SERVO_CONT_PRIORITY_PAN 4
#define SERVO_CONT_PRIORITY_TILT 4
#define DIRECTION_CONT_PRIORITY 1

//Task Stack Sizes
#define MOTOR_STACK_SIZE        100
#define SERVO_STACK_SIZE        100
#define SERVO_STACK_SIZE_PAN    100
#define SERVO_STACK_SIZE_TILT   100
#define DIRECTION_STACK_SIZE    100

//Motor Control
#define FULL_SPEED              1000
#define HALF_SPEED              500
#define SPEED_INC               100
#define STOP                    0

//Servo Control (in 100 us)
#define SERVO_INTERVAL          200
#define SERVO_0                 10
#define SERVO_18                11
#define SERVO_36                12
#define SERVO_54                13
#define SERVO_72                14
#define SERVO_90                15
#define SERVO_108               16
#define SERVO_126               17
#define SERVO_144               18
#define SERVO_162               19
#define SERVO_180               20

//Servo Control (number of pwm intervals required for 18 degrees of change)
#define SERVO_MOVE_PULSES       3

//Servo Control (number of pwm intervals required for ultrasound measurement)
#define SERVO_HOLD_PULSES       5

//Direction Control
#define STRAIGHT_THRESHOLD      10
#define TURN_THRESHOLD          10
#define UP_THRESHOLD            20
#define DOWN_THRESHOLD          45

//Define dataset for motorControl
typedef struct {
    unsigned int*       speed;
    unsigned int*       turnRad;
} motorContData;

//Define dataset for servoControl (sensor)
typedef struct {
    unsigned int*       servoCount;
    unsigned int*       servoCountOld;
    unsigned int*       servoPulse;
    unsigned int*       servoPosition;
} servoContData;

//Define dataset for servoControl (pan)
typedef struct {
    unsigned int*       servoCountPan;
    unsigned int*       servoCountOldPan;
    unsigned int*       servoPulsePan;
} servoContDataPan;

//Define dataset for servoControl (tilt)
typedef struct {
    unsigned int*       servoCountTilt;
    unsigned int*       servoCountOldTilt;
    unsigned int*       servoPulseTilt;
} servoContDataTilt;

//Define dataset for directionControl task
typedef struct {
    bool*               goRight;
    bool*               goLeft;
    bool*               stuckFlag;
    unsigned int*       turnRad;
    unsigned int*       speed;
    bool*               obstacleFlag;
    bool*               wallFlag;
    xTaskHandle*        xMotorControlHandle;
    unsigned int*       upDistance;
    unsigned int*       downDistance;
    bool*               overhangFlag;
    bool*               dropOffFlag;
    bool*               leftTurnFlag;
    bool*               rightTurnFlag;
    unsigned int*       distances;
    bool*               leftVeer;
    bool*               rightVeer;
    bool*               leftCorrectStraight;
    bool*               rightCorrectStraight;
    bool*               leftCorrectRight;
    bool*               rightCorrectLeft;
    bool*               leftCorrectStraightAgain;
    bool*               rightCorrectStraightAgain;
    unsigned long*      globalCount;
    unsigned long*      startCorrectionCount;
} directionContData;

//Define dataset for the softwareInit
typedef struct {
    xTaskHandle*        xMotorControlHandle;
    motorContData*      myMotorData;
    xTaskHandle*        xServoControlHandle;
    servoContData*      myServoData;
    xTaskHandle*        xServoControlHandlePan;
    servoContDataPan*   myServoDataPan;
    xTaskHandle*        xServoControlHandleTilt;
    servoContDataTilt*  myServoDataTilt;
    xTaskHandle*        xDirectionControlHandle;
    directionContData*  myDirectionControlData;
} softwareInitData;

//Function prototypes
void hardwareInit(void);
void softwareInit(softwareInitData* data);
int motorControlInit(xTaskHandle* xMotorControlHandle, motorContData* myMotorData);
int servoControlInit(xTaskHandle* xServoControlHandle, servoContData* myServoData);
int servoControlInitPan(xTaskHandle* xServoControlHandlePan, servoContDataPan* myServoDataPan);
int servoControlInitTilt(xTaskHandle* xServoControlHandleTilt, servoContDataTilt* myServoDataTilt);
int directionControlInit(xTaskHandle* xDirectionControlHandle, directionContData* myDirectionData);

#endif
