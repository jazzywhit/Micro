#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <stdio.h>
#include <unistd.h>

#define uchar unsigned char
#define TRUE 1
#define FALSE 0

/* Parallel Port Constants */
#define pport 0x378 //Base Address
#define ppData pport 
#define ppStatus pport + 1 //Status Register
#define ppStrobe pport + 2 //Control Register
#define MSG_RESET 0x0
#define MSG_PING 0x1
#define MSG_GET 0x2
#define MSG_ACK 0xE
#define MSG_NOTHING 0xF

/* Function Declarations */
int OpenPort(int portAdr); //Open the port with the designated port number.
int WriteByte(uchar byte); //Send a byte by breaking it into 4bit parts.
int ParPortWrite(uchar byte);
int ReadByte(uchar *byte); //Send a byte by breaking it into 4bit parts.
int ParPortRead(uchar *byte);

int OpenPort(int portAdr){
	
	printf("Opening Port: %X\n", portAdr);
	if(ioperm(portAdr,3,1)) return FALSE;
	return TRUE;
}

int WriteByte(uchar byte){
	
	ParPortWrite(byte>>4);
	sleep(1); //Just so we can see it on the LCD.
	ParPortWrite(byte & 0x0F);
	
	return TRUE;
}

int ReadByte(uchar *byte){
	uchar a, b;
	while(ParPortRead(&a));
	//sleep(2);
	//while(ParPortRead(&b));
	
	*byte = a<<4 | b; //Reconstruct the two 4bit messages into a byte
	return TRUE;
}

int ParPortRead(uchar *byte){

	outb(0x20, ppStrobe); //Set strobe low and bit 5 high for input mode.
	outb(0x21, ppStrobe); //Set strobe high and input mode.
	usleep(10000);
	*byte = inb(ppData);
	outb(0x20, ppStrobe);
	return TRUE;
}

int ParPortWrite(uchar byte){
	outb(0x00, ppStrobe); //Set the strobe low
	//printf("%X\n", inb(ppStrobe));
	sleep(3);
	outb(0x01, ppStrobe); //Set the strobe high
	sleep(3);
	outb(0x00, ppStrobe); //Set the strobe low
	//printf("%X\n", inb(ppStrobe));
	sleep(3);
	outb(0x01, ppStrobe); //Set the strobe high
	sleep(3);
	//outb(0x01, ppStrobe); //Set the strobe low
	//outb(byte, ppData); //Send the data	
	//outb(0x01, ppStrobe); //Set the strobe high
	//printf("%X\n", inb(ppStrobe));
	//usleep(10000);
	//sleep(3);
	//outb(0x00, ppStrobe);
	//printf("%X\n", inb(ppStrobe));
	//Need to check for an ACK. This might require calling ParPortRead.
	return TRUE;
} 

int main(int argc, char *argv[]){

	int i = 0;
	uchar dataHex;

	if (argc < 2){
		printf("Data Hex Missing: Enter a two digit hex value (0F, FF, 01, etc..)\n");
		return 1;
	}
 
	dataHex = strtol(argv[1], NULL, 16);

	printf("DataHex: %X\n", dataHex);

	if(OpenPort(pport)){
		printf("Port Opened!\n");
	} else {
		printf("Port Could not be opened. Ensure root access.\n");
	}

	for(;i < 1; i++){
		uchar a, b, c; 
	ParPortWrite(dataHex); 	
	
		//a = inb(pport);	
		//b = inb(pport+1);	
		//c = inb(pport+2);	
		//printf("%X\t%X\t%X\n",a, b, c);
		//printf("%X\t\n",a);
		//usleep(1000000);
	}
}
