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
	int address = 0x1E; // Address of the compass
	ioctl(fd, I2C_SLAVE, address); // Select from the port the device with the given address as a slave
	
	// Buffer(W) = {Register address(0x00-0x02), Value to write to the register}
	char craBufferW[2] = {0x00, 0x70}; // 0x70: 8 samples averaged, 15 Hz data output rate, normal measurement
	char crbBufferW[2] = {0x01, 0xA0}; // 0xA0: Gain of +/-4.7 (Preventing data overflow/saturation)
	char cmBufferW[2] = {0x02, 0x00}; // 0x00: Continuous measurement mode
	
	// Fake buffer, which, when written, doesn't change the value of the register at the address, but moves 
	// the pointer within the register list to the address 0x03 for convenient reading of data x, y, and z
	char pointerBuffer[1] = {0x03};  

	unsigned char xzyBufferR[6]; // Data storage
	int ack=0;
	// Configuration of settings
	ack = write(fd, craBufferW, 2); // Write the value 0x70 to the register at the address 0x00
	printf("Ack= %d\n",ack);
	write(fd, crbBufferW, 2); // Write the value 0xA0 to the register at the address 0x01
	write(fd, cmBufferW, 2); // Write the value 0x00 to the register at the address 0x02
	printf("config\n");
	usleep(6000); // Wait for 6 ms
	printf("sleep\n");
	write(fd,pointerBuffer,1); // Move the pointer to the address 0x03
	read(fd, xzyBufferR, 6); // Read 6 registers (8 bit ea.) starting at the address 0x03	
	
	printf("read\n");
	// 16 bits of data per axis: although each byte of data is stored as an unsigned char, 
	// the combined result of 16-bit data represents 2's complement => Range: [32767, -32768]

	// Combining two separate 8-bit data to form a 16-bit data for each axis
	// EX). xzyBufferR[0] = 10110010, xzyBufferR[0] << 8 = 	1011001000000000 (Bit-wise left shift)
	//	xzyBufferR[1] = 00100111,   or equivalently,   	0000000000100111 
	// 	(xzyBufferR[0] << 8) | xzyBufferR[1]	     =	1011001000100111 (Bit-wise OR operation)
	//						     = 	-19929 (in decimal)
	short x = (xzyBufferR[0] << 8) | xzyBufferR[1];
	short z = (xzyBufferR[2] << 8) | xzyBufferR[3];
	short y = (xzyBufferR[4] << 8) | xzyBufferR[5];

	int declinationAngle = 16;
	float angle = atan2(y,x) * 180 / M_PI + declinationAngle; 
	
	printf("x = %d\ny = %d\nz = %d\nangle = %f\n", x, y, z, angle);
}
