#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>	//Used for UART
#include <fcntl.h>	//Used for UART
#include <termios.h>	//Used for UART


#define BAUD 38400	
#define PORT "/dev/ttyAMA0"

#define TIMEOUT 0.5
#define SERIALNUM 0

#define COMMANDSEND 0x56
#define COMMANDREPLY 0x76
#define COMMANDEND 0x00

#define CMD_GETVERSION 0x11
#define CMD_RESET 0x26
#define CMD_TAKEPHOTO 0x36
#define CMD_READBUFF 0x32
#define CMD_GETBUFFLEN 0x34

#define FBUF_CURRENTFRAME 0x00
#define FBUF_NEXTFRAME 0x01

#define FBUF_STOPCURRENTFRAME 0x00

int fd;
struct termios options;
char getversioncommand[] = {COMMANDSEND, SERIALNUM, CMD_GETVERSION, COMMANDEND};
char resetcommand[] = {COMMANDSEND, SERIALNUM, CMD_RESET, COMMANDEND};
char takephotocommand[] = {COMMANDSEND, SERIALNUM, CMD_TAKEPHOTO, 0x01, FBUF_STOPCURRENTFRAME};
char getbufflencommand[] = {COMMANDSEND, SERIALNUM, CMD_GETBUFFLEN, 0x01, FBUF_CURRENTFRAME};
char readphotocommand[] = {COMMANDSEND, SERIALNUM, CMD_READBUFF, 0x0c, FBUF_CURRENTFRAME, 0x0a};


int checkreply(unsigned char r[], char b){
	printf("r0 %x r1 %x r2 %x\n", r[0], r[1], r[2]);
	//printf("commandreply %x, serialnum %x, b %x\n", COMMANDREPLY,SERIALNUM, b);
	if( r[0] == COMMANDREPLY && r[1] == SERIALNUM && r[2] == b && r[3] == 0x00){
		return 1;
	}
	return 0;
}

int reset(){
	unsigned char reply[5];
	write(fd,resetcommand,4);
	usleep(10000);
	read(fd,(void*)reply,5);
	if (checkreply( reply, CMD_RESET)){
		return 1;
	}
	return 0;
}

int getversion(){
	unsigned char reply[16];
	int i = 5;
	write(fd,getversioncommand,4);
	usleep(10000);
	read(fd,(void*)reply,16);
	if (checkreply( reply, CMD_GETVERSION )){
		printf("Version: ");		
		while(i < 16){
			printf("%c",reply[i]);
			i++;	
		}
		printf("\n");
		return 1;
	}
	return 0;
}

int takephoto(){
	unsigned char reply[5];
	write(fd, takephotocommand,5);
	usleep(10000);
	read(fd,(void*)reply,5);
	if( checkreply( reply, CMD_TAKEPHOTO) && (reply[3] == 0x00) ){
		return 1;
	}
	return 0;
}

int getbufferlength(){
	unsigned char reply[9];
	write(fd,getbufflencommand, 5);
	usleep(10000);
	read(fd, reply, 9);
	
	if( checkreply( reply, CMD_GETBUFFLEN) && (reply[4] == 0x04) ){
		int l = reply[5];
		l <<= 8;
		l |= reply[6];
		l <<= 8;
		l |= reply[7];
		l <<= 8;
		l |= reply[8];
		return l;
	}
	return 0;
}


/*
int[] readbuffer(bytes){
	int addr = 0	# the initial offset into the frame buffer
	int photo = []

	# bytes to read each time (must be a mutiple of 4)
	inc = 8192
	#inc = 256

	while( addr < bytes ):
		# on the last read, we may need to read fewer bytes.
		chunk = min( bytes-addr, inc );

		# append 4 bytes that specify the offset into the frame buffer
		command = readphotocommand + [(addr >> 24) & 0xff,(addr>>16) & 0xff,(addr>>8 ) & 0xff,addr & 0xff]

		# append 4 bytes that specify the data length to read
		command += [(chunk >> 24) & 0xff,(chunk>>16) & 0xff,(chunk>>8 ) & 0xff,chunk & 0xff]

		# append the delay
		command += [1,0]

		# print map(hex, command)
		print "Reading", chunk, "bytes at", addr

		# make a string out of the command bytes.
		cmd = ''.join(map(chr, command))

		s.write(cmd)

		# the reply is a 5-byte header, followed by the image data
		#   followed by the 5-byte header again.
		reply = s.read(5+chunk+5)

		# convert the tuple reply into a list
		r = list(reply)
		if( len(r) != 5+chunk+5 ):
		# retry the read if we didn't get enough bytes back.
			print "Read", len(r), "Retrying."
			continue

		if( not checkreply(r, CMD_READBUFF)):
			print "ERROR READING PHOTO"
			break;

		# append the data between the header data to photo
		photo += r[5:chunk+5]

		# advance the offset into the frame buffer
		addr += chunk

	print addr, "Bytes written"
	return photo
}


int main (int argc, char** argv){
//fd = open(PORT,O_RDWR | O_NOCTTY | O_NDELAY); 
s = serial.Serial( PORT, baudrate=BAUD, timeout = TIMEOUT )

# forces camera to default settings, aka 38400 baud, etc
reset()

if( not getversion() ):
	print "Camera not found"
	exit(0)

print "VC0706 Camera found"

if takephoto():
	print "Snap!"

bytes = getbufferlength()

print bytes, "bytes to read"

photo = readbuffer( bytes )

f = open( "photo.jpg", 'w' )

photodata = ''.join( photo )

f.write( photodata )

f.close()
}*/

int main (int argc, char** argv){
	fd = open(PORT,O_RDWR | O_NOCTTY | O_NDELAY);
	tcgetattr(fd, &options);
	options.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);

	int i = reset();
	printf("reset:%d\n", i);
	usleep(10000);
	getversion();
 	//i = takephoto();
	//printf("Tp:%d\n", i);
	//i = getbufferlength();
	//printf("gbl:%d\n", i);
	return 0;
}
