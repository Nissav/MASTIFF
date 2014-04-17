#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char **argv)
{
	printf("INTO MAIN\n");

	int fd;
	char *fileName = "/dev/i2c-1";
	int address = 0x1E;
	unsigned char buf[16];
	

	if ((fd = open(fileName, O_RDWR)) < 0) 
	{
		printf("Failed to open i2c port\n");
		exit(1);
	}

	if (ioctl(fd,I2C_SLAVE, address) < 0)
	{
		printf("Unable to get bus access to talk to slave\n");
		exit(1);
	}

	//writeToDevice(fd,0x00,0x70);
	char buft[2];
	//buft[0] = 0x3C;
	buft[0] = 0x00;
	buft[1] = 0x70;
	write(fd,buft,2);

	//writeToDevice(fd,0x01,0xA0);
	buft[0] = 0x01;
	buft[1] = 0xA0;
	write(fd,buft,2);
	
	//writeToDevice(fd,0x02,0x01);
	buft[0] = 0x02;
	buft[1] = 0x00;
	write(fd,buft,2);
	
	usleep(6*1000);

	buf[0] = 0x03;	

	//write(fd,buf,1);
	read(fd,buf,6);
	

	//buf[0] = 0x3D;
	//buf[1] = 0x06;
	//read(fd,buf,6);

	short x = (buf[0] << 8) | buf[1];
	short z = (buf[2] << 8) | buf[3];
	short y = (buf[4] << 8) | buf[5];
	

	float angle = atan2((int)y, (int)x) * 180 / M_PI +16;

	printf("x=%d, y=%d, z=%d\n", x,y,z);
	printf("angle=%f\n", angle);

}
