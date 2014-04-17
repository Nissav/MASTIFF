#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main (int argc, char** argv)
{
	int fd;
	fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY );
	if ( fd == -1 )
	{
		printf("Unable to open UART\n");
		return (-1);
	}

	fcntl(fd,F_SETFL,0);

	char buf[1];
	printf("Read function executing...\n");
	int n = read(fd, (void*)buf, 1);
	printf("Read function execution finished\n");
	if ( n < 0 )
		perror("Read failed - ");
	else if ( n == 0 )
		printf("No data on port\n");
	else
	{
		buf[n] = '\0';
		printf("%i bytes read : %s\n", n, buf);
	}
	/*
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B1200 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);

	int i;
	for ( i = 0 ; i < 100 ; i++ )
	{
		unsigned char rx_buffer[256];
		int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);
		if (rx_length < 0)
			printf("ERROR\n");
		else if (rx_length == 0)
			printf("No data waiting\n");
		else
		{
			rx_buffer[rx_length] = '\0';
			printf("%i bytes read : %s\n", rx_length, rx_buffer);
		}
	}
	*/
}
