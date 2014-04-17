#include <wiringPiI2C.h>
#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <errno.h>


#define COMPASS 0x1e
#define TIVA 0x3C

int setup(void);
int increment(int);

int main()
{
	printf("INTO MAIN\n");
	int ret = setup();
	if(ret)
	{
		return 1;
	}
	int dec = increment(0);
	int time = 0;
	while (time < 15)
	{
		//wiringPiI2CWrite(TIVA,dec);
		int read = wiringPiI2CRead (COMPASS);
		int error = errno;

		printf("error:%d\n",error);
		printf("Read data:%d\n", read);	
		time++;
		sleep(2);
		
	}
		
	// int wiringPiI2CSetup(int devID)
	// int wiringPiI2CRead (int fd)
	// int wiringPiI2CWrite (int fd, int data)
	// int wiringPiI2CWriteReg8 (int fd, int reg, int data) or Reg16
	// int wiringPiI2CReadReg8 (int fd, int reg) or Reg16
		

	return 0;
}


int setup()
{
	int check = wiringPiSetupSys();
	int checkCompass = wiringPiI2CSetup(COMPASS);
	int checkTiva = wiringPiI2CSetup(TIVA);

	if ( check == -1 || checkCompass == -1 || checkTiva == -1 ) 
	{
		printf("Setup Failed");
		return 1;
	}
	return 0;
}

int increment(int i)
{
	return i+1;
}
