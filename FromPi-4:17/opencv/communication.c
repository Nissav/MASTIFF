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
#include <stdint.h>
#include <stdbool.h>



#define SLAVE_ADDRESS 0x3C
#define CHAR_VALUE 48


//Communication Protocal Command Characters
#define READ 			'r'
#define WRITE 			'w'
#define PAN_RIGHT 		'p'
#define PAN_LEFT 		'l'
#define TILT_UP 		'u'
#define TILT_DOWN		'd'

void closeCom(int fd);
int  setupCom(void);
int readData(char bufferIn[32], int fd);
//void writeData (char command, char * bufferIn, int fd);
int constructCommand(char bufferW[32], char command, char * data);
int sendCommand(char bufferW[32], int fd, int size);
void commSwitch (char command, char * bufferWrite, char bufferReceive[32], int fd);


int main (int argc, char** argv)
{
	char bufferWrite[32];
	int i = 0;
	for(i; i < 33; i++){
		bufferWrite[i] = ' ';
	}
	char bufferReceive[32];
	//Open communication channel
	int fd = setupCom();
	commSwitch(WRITE,bufferWrite, bufferRecieve,fd);
	sleep(2);
	commSwitch(PAN_LEFT, "P", bufferReceive, fd);

	sleep(5);
	commSwitch(PAN_LEFT, "P", bufferReceive, fd);
	//commSwitch(WRITE, "This might not work", bufferReceive, fd);
	//commSwitch(READ,"",bufferReceive, fd);
	closeCom(fd);
}

void commSwitch (char command, char * bufferData, char bufferReceive[32], int fd){//, char WriteRead){
	int size = 0;
	int ack = -1;
	char bufferWrite[32];
	char bufferRead[1];
	bool endRead = false;
	printf("Running Command: %c\n",command);

	switch ( command ){

		case WRITE  :  //For standard write
			size = constructCommand(bufferWrite,command,bufferData); 	//Create the command structure {command, data, ':'}  
			sendCommand(bufferWrite,fd,size);						//Send command structure over one char at a time			
			break;

		case READ	:
			size = 0;
			while(!endRead){			//While we are not at the special end transmission char continue reading
				ack = read(fd, bufferRead, 1); 	//Read 1 register
				bufferReceive[size] = bufferRead[0];	//Constuct Read buffer
				printf("Received Data: '%c', Size: %d byte\n", bufferRead[0], size++);
				if ( bufferRead[0] == ':' )
					endRead = true;
			}
			break;

		case PAN_LEFT:
			size = constructCommand(bufferWrite,command,bufferData); 	//Create the command structure {command, data, ':'} 
			sendCommand(bufferWrite,fd,size);
			//Wait for go camera to move
			//while(bufferReceive != "k");
			//	commSwitch(READ,bufferData, bufferReceive, fd);
			//}
			break;

		case PAN_RIGHT:
			size = constructCommand(bufferWrite,command,bufferData); 	//Create the command structure {command, data, ':'}  
			sendCommand(bufferWrite,fd,size);
			//Wait for go camera to move
			//while(bufferReceive != "k");
			//	commSwitch(READ,bufferData, bufferReceive, fd);
			//}
			break;

		case TILT_UP:
			size = constructCommand(bufferWrite,command,bufferData); 	//Create the command structure {command, data, ':'}  
			sendCommand(bufferWrite,fd,size);
			//Wait for go camera to move
			//while(bufferReceive != "k");
			//	commSwitch(READ,bufferData, bufferReceive, fd);
			//}
			break;

		case TILT_DOWN:
			size = constructCommand(bufferWrite,command,bufferData); 	//Create the command structure {command, data, ':'}  
			sendCommand(bufferWrite,fd,size);
			//while(bufferReceive != "k");
			//Wait for go camera to mov e
			//	commSwitch(READ,bufferData, bufferReceive, fd);
			//}
			break;

		default 	:
			printf("No such command: %s", command);
			break;
	}	
}

int setupCom(){
	int fd = open("/dev/i2c-1",O_RDWR);
	ioctl(fd, I2C_SLAVE, SLAVE_ADDRESS);
	printf("Setup Complete\n");
	return fd;
}

void closeCom(int fd){
	close(fd);
}

int constructCommand(char * bufferW, char command, char * data){
	int slot = 0;
	bufferW[slot++] = command;
	
	while(data[slot-1] != '\0'){
		bufferW[slot] = data[slot-1];
		slot++;
	}
	bufferW[slot] = ':'; //end of string indicator
	bufferW[slot +1] = '\0';
	return ++slot;
}

int sendCommand(char bufferW[32], int fd, int size){
	int i=0;
	//int ack=1;
	printf("In Send: %s\n",bufferW);
	for(i; i < size;i++){
		//while(ack == 1){
			int ack = write(fd, &bufferW[i], 1); 
			if ( ack == 1 )
				printf("    Sent Data: '%c', Size: %d byte\n", bufferW[i], ack);
			else
			{
				printf(" FAILED: write(fd, %c, 1)\n", bufferW[i]);
				return 0;
			}
		//}
			
	}
	return 1;
}



/////////////////////////////Old main and commSwitch////////////////////////////////////////
/*
void commSwitch (char command, char dataI, int fd);

int main (int argc, char** argv)
{
	
	char command = 'w';

	//Open communication channel
	int fd = setupCom();

	
	//Simple Read and Write test
	char data = 'w';
	commSwitch (command, data, fd);
	sleep(1);
	data = 'J';
	commSwitch (command, data, fd);
	sleep(1);
	data = 'U';
	commSwitch (command, data, fd);
	sleep(1);
	data = 'N';
	commSwitch (command, data, fd);
	sleep(1);
	data = ' ';
	commSwitch (command, data, fd);
	sleep(1);
	data = 'R';
	commSwitch (command, data, fd);
	sleep(1);
	data = 'O';
	commSwitch (command, data, fd);
	sleep(1);
	data = 'C';
	commSwitch (command, data, fd);
	sleep(1);
	data = 'K';
	commSwitch (command, data, fd);
	sleep(1);
	data = 'S';
	commSwitch (command, data, fd);
	sleep(1);
	data = '!';
	commSwitch (command, data, fd);
	sleep(1);
	data = ':';
	commSwitch (command, data, fd);
	
	sleep(3);
	
	command = 'r';
	int lp;
	for ( lp = 0 ; lp < 11 ; lp++ )
	{
		sleep(1);
		commSwitch (command, data, fd);
	}
	

	
	closeCom(fd);
}

//dataI = Data In	dataS = Data Send	dataR = Data Received
void commSwitch (char command, char dataI, int fd){
	unsigned char dataR;
	unsigned char dataS;
	unsigned char bufferR[1];// = &dataR; // Data storage
	char bufferW[1] = {dataI};//&dataS; 
	char bufferA[32];
	int size = 0;
	int new = 0;
	char temp[1];
	int ack = -1;

	printf("Running Command: %c\n",command);

	switch ( command ){

		case WRITE : 
			//constructCommand(bufferW,command,dataS);
			size = write(fd, bufferW, sizeof(bufferW)); 
			printf("    Sent Data: '%c', Size: %d byte\n", bufferW[0], size);
			break;
		case READ : 
			size = read(fd, bufferR, sizeof(bufferR)); //Read 1 register
			printf("Received Data: '%c', Size: %d byte\n", bufferR[0], size);
			break;
		case ADD :
			new = (int)dataI - CHAR_VALUE + 1;
			sprintf(temp,"%d",new);
			constructCommand(bufferA, command, temp);
			size = write(fd,bufferA,sizeof(bufferA)); 
			printf("Added 1 to %d and sent %d", (int) dataS, new);
			sleep(1);
			read(fd, bufferR, sizeof(bufferR));
			printf("Added 1 to %d and sent %d", (int) dataS, new);
			break;
		default :
			printf("No such command: %s", command);
			break;
	}

}*/

/*int readData(char bufferIn[32], int fd){
	int size = 0;
	int ack = -1;
	char bufferRead[1];
	while(bufferRead[0] != ':'){			//While we are not at the special end transmission char continue reading
		while(ack == -1){					//While we have unsuccessfull reads
			ack = read(fd, bufferRead, 1); 	//Read 1 register
		}
		bufferIn[size] = bufferRead[0];	//Constuct Read buffer
		printf("Received Data: '%c', Size: %d byte\n", bufferRead[0], size);
		size++;
	}
	return size;
}*/
/*int readData(char bufferIn[32], int fd){
	
	return size;
}*/


