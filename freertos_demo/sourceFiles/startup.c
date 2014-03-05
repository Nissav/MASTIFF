#include "header.h"

void hardwareInit( void ) {
    /*GENERAL*/
  
    //Set the clocking to run directly from the crystal
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    
    //Enable processor interrupts
    ROM_IntMasterEnable();
    
    /*GPIO*/
    
    //Enable the GPIO port A
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    
    //Enable the GPIO port B
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    
    //Setup port C (range finders)
    ROM_IntEnable(INT_GPIOC);
    
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7); // Trigger range finders
    ROM_GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_6);
    ROM_GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_BOTH_EDGES); // Detect sweep range finder echo
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_6);
    
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_5);
    ROM_GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_BOTH_EDGES); // Detect up range finder echo
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_5);
    
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);
    ROM_GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_BOTH_EDGES); // Detect down range finder echo
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4);
    
    //Setup port D (wheel turn counts)
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_6);
    ROM_GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_BOTH_EDGES); //Interrupt when magnet passes hall sensor left
    GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_6);
    
    //Unlock GPIOD7
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
    
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7);
    ROM_GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_BOTH_EDGES); //Interrupt when magnet passes hall sensor right
    GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_7);
    
    ROM_IntEnable(INT_GPIOD);
    
    //Enable the GPIO port E
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    
    //Enable the GPIO port F
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_3); 
    ROM_GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_RISING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_3);
    
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4); 
    ROM_GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);
    
    //Enable the GPIO pin F0 for output
    HWREG(BUTTONS_GPIO_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(BUTTONS_GPIO_BASE + GPIO_O_CR) |= 0x01;
    HWREG(BUTTONS_GPIO_BASE + GPIO_O_LOCK) = 0;
    
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    
    /*TIMER*/
    
    //Enable the peripherals used by this example
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    //Configure the 32-bit periodic timer (100 us timeout)
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet() / 10000);

    //Setup the interrupts for the timer timeouts
    ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //Enable the timers
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);
    
    /*I2C*/
    //Enable the I2C1 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    
    //Configure the pin muxing for I2C1
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);
    
    //Select the I2C function for these pins
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    
    // Set GPIO Pins for Open-Drain operation (I have two Rpulls=10K Ohm to 5V on the SCL and SDA lines)
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD);

    // Give control to the I2C1 Module
    GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_DIR_MODE_HW);
    GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_DIR_MODE_HW);
    
    // Enable the I2C1 slave Module
    I2CSlaveEnable(I2C1_BASE);
    
    // Set the slave address to SLAVE_ADDRESS
    I2CSlaveInit(I2C1_BASE, SLAVE_ADDRESS);
}

void softwareInit( softwareInitData* data ) {
    if(motorControlInit((data->xMotorControlHandle), (data->myMotorData)) != 0)
    {
        while(1)
        {
        }
    }
    
    if(servoControlInit((data->xServoControlHandle), (data->myServoData)) != 0)
    {
        while(1)
        {
        }
    }
    
    if(servoControlInitPan((data->xServoControlHandlePan), (data->myServoDataPan)) != 0)
    {
        while(1)
        {
        }
    }
    
    if(servoControlInitTilt((data->xServoControlHandleTilt), (data->myServoDataTilt)) != 0)
    {
        while(1)
        {
        }
    }
    
    if(sensorPositionerInit((data->xSensorPositionerHandle), (data->mySensorPositionerData)) != 0)
    {
        while(1)
        {
        }
    }
    
    if(cameraPositionerInit((data->xCameraPositionerHandle), (data->myCameraPositionerData)) != 0)
    {
        while(1)
        {
        }
    }
      
    if(directionControlInit((data->xDirectionControlHandle), (data->myDirectionControlData)) != 0)
    {
        while(1)
        {
        }
    }
    
    if(commInit((data->xCommHandle), (data->myCommData)) != 0)
    {
        while(1)
        {
        }
    }
}
