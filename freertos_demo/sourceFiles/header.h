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
#define SENSOR_POSITIONER_PRIORITY 3
#define CAMERA_POSITIONER_PRIORITY 2

//Task Stack Sizes
#define MOTOR_STACK_SIZE        100
#define SERVO_STACK_SIZE        100
#define SERVO_STACK_SIZE_PAN    100
#define SERVO_STACK_SIZE_TILT   100
#define DIRECTION_STACK_SIZE    100
#define SENSOR_POSITIONER_STACK_SIZE 100
#define CAMERA_POSITIONER_STACK_SIZE 100

//Motor Control
#define FULL_SPEED              1000
#define HALF_SPEED              500
#define SPEED_INC               100
#define STOP                    0

#define UPDATE_MOTOR_CONTROL_INTERVAL 1000

//Servo Control (in 100 us)
#define SERVO_INTERVAL          200

#define SERVO_0                 6
#define SERVO_10                7
#define SERVO_20                8
#define SERVO_30                9
#define SERVO_40                10
#define SERVO_50                11
#define SERVO_60                12
#define SERVO_70                13
#define SERVO_80                14
#define SERVO_90                15
#define SERVO_100               16
#define SERVO_110               17
#define SERVO_120               18
#define SERVO_130               19
#define SERVO_140               20
#define SERVO_150               21
#define SERVO_160               22
#define SERVO_170               23
#define SERVO_180               24

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
    xTaskHandle*        xSensorPositionerHandle;
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
    bool*               updateMotorControl;
} directionContData;

//Define dataset for sensorPositioner task
typedef struct {
    unsigned int*       servoCount;
    unsigned int*       servoPosition;
} sensorPositionerData;

//Define dataset for cameraPositioner task
typedef struct {
    unsigned int*       servoCountPan;
    unsigned int*       servoCountTilt;
} cameraPositionerData;

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
    
    xTaskHandle*        xSensorPositionerHandle;
    sensorPositionerData* mySensorPositionerData;
    
    xTaskHandle*        xCameraPositionerHandle;
    cameraPositionerData* myCameraPositionerData;
} softwareInitData;

//Function prototypes
void hardwareInit(void);
void softwareInit(softwareInitData* data);
int motorControlInit(xTaskHandle* xMotorControlHandle, motorContData* myMotorData);
int servoControlInit(xTaskHandle* xServoControlHandle, servoContData* myServoData);
int servoControlInitPan(xTaskHandle* xServoControlHandlePan, servoContDataPan* myServoDataPan);
int servoControlInitTilt(xTaskHandle* xServoControlHandleTilt, servoContDataTilt* myServoDataTilt);
int directionControlInit(xTaskHandle* xDirectionControlHandle, directionContData* myDirectionData);
int sensorPositionerInit(xTaskHandle* xSensorPositionerHandle, sensorPositionerData* mySensorPositionerData);
int cameraPositionerInit(xTaskHandle* xCameraPositionerHandle, cameraPositionerData* myCameraPositionerData);

#endif
