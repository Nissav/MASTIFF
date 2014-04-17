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

int main (int argc, char** argv)
{
	int fd = open("/dev/i2c-1",O_RDWR); // Open I2C port1 with access to read/write
	int address = 0x3C; // Address of the tiva
	ioctl(fd, I2C_SLAVE, address); // Select from the port the device with the given address as a slave
	unsigned char dataR;
	unsigned char* xzyBufferR = &dataR; // Data storage
	char dataS = 0x44;
	char* craBufferW = &dataS; 
	int size=0;
	//size = write(fd,craBufferW,1); 
	//printf("    Sent Data: 0x%x, Size: %d byte\n", dataS, size);
	size = read(fd, xzyBufferR, 1); // Read 6 registers (8 bit ea.) starting at the address 0x03
	printf("Received Data: 0x%x, Size: %d byte\n",dataR, size);
	
	
}
