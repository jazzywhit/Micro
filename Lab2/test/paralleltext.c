#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#define BYTE unsigned char
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
int WriteByte(BYTE byte); //Send a byte by breaking it into 4bit parts.
int ParPortWrite(BYTE byte);
BYTE ReadByte(); //Send a byte by breaking it into 4bit parts.
BYTE ParPortRead();
int Reset();
int Get();
int Ping();
void DisplayData( BYTE RTCData[], int adcResult ) ; 

void DisplayData( BYTE RTCData[], int adcResult ) {
  printf("\n\n\nMSG_GET Results\n---------------------------\n");

}

    BYTE ReadByte(){

    BYTE temp;
    BYTE byte;
    
    temp = ParPortRead();
    byte = temp<<4 & 0xF0; //Reconstruct the two 4bit messages into a byte
    temp = ParPortRead();
    byte |= temp;
    
    return byte;
  
  }


int Get(){
 
  printf("I am fucking getting");

  BYTE RTCData[8];
  BYTE adcHighNible; 
  BYTE adcLowByte;
  int adcResult;

  // Send Command
  ParPortWrite(MSG_GET);

   // Read 3 Nible from ADC 
  adcHighNible = ParPortRead();
  adcLowByte = ReadByte();
  adcResult = ((adcHighNible&0x0F) * 16 * 16) + adcLowByte ;

  // Read RTC data
  int i ;
  for(i = 0 ; i < 8 ; i++) {
    RTCData[i] = ReadByte() ;
  }

  // Get Acknowledgement 
  if(!Ping()) return FALSE ;

  // Display Data 
  DisplayData( RTCData , adcResult );
 
  return TRUE;
}

int Reset(){

  BYTE result ;

  printf("\nI am fucking resetting");
  ParPortWrite(MSG_RESET);
  result = ParPortRead();

  if( result != MSG_RESET ) return FALSE ;

  return TRUE ; 

}


int Ping(){

  printf("\nI and fucking pinging");
  BYTE result ;

  ParPortWrite(MSG_PING);
  result = ParPortRead();

  if( result != MSG_ACK ) return FALSE ;

  return TRUE ;

}


int OpenPort(int portAdr){
  
  printf("Opening Port: %X\n", portAdr);
  if(ioperm(portAdr,3,1)) return FALSE;
  return TRUE;
}


BYTE ParPortRead(){
  BYTE byte;
  outb(0x21, ppStrobe); //Set strobe low and bit 5 high for input mode.
  outb(0x20, ppStrobe); //Set strobe high and input mode.
  usleep(10000);
  byte = inb(ppData);
  outb(0x21, ppStrobe);  //Set strobe low and bit 5 high for input mode.
  return byte;
}

int ParPortWrite(BYTE byte){
  outb(0x01, ppStrobe); //Set the strobe low
  outb(byte, ppData); //Output command to data bus.
  outb(0x00, ppStrobe); //Set the strobe high
  sleep(10000);
  outb(0x01, ppStrobe); // Set the strobe to low.
  return TRUE;
} 

int main(int argc, char *argv[]){
  int scanfresult;
  char cmd;
  int Yay = 1;
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //Don't you fucking dare take this out.
  if(OpenPort(pport)){
    printf("Port Opened!\n-------------------------\n");
  } else {
    printf("Port Could not be opened. Ensure root access.\n-------------------------\n");
    return(-1);
  }
  printf("\nCommands:\n\tR = Reset\n\tG = Get\n\tP = Ping\n\tQ = Quit\n");
  while(1){
    
    printf("\nEnter a command: ");
    
    scanfresult = scanf("%c",&cmd);
    while (getchar() != '\n');
    switch(toupper(cmd)){
    case 'R':
      Yay = Reset();
      break;
    case 'G':
      Yay = Get();
      break;
    case 'P':
      printf("before");
      Yay = Ping();
      printf("after");
      break;
    case 'Q':
      return(0);
    default:
      Yay=FALSE;
      break;
    }
    if(!Yay)
      {
	printf("Enter another command. (R,G,P,Q)");
      }
  }
  return(0);
}

int WriteByte(BYTE byte){
	
	ParPortWrite(byte>>4);
	sleep(1); //Just so we can see it on the LCD.
	ParPortWrite(byte & 0x0F);
	
	return TRUE;
}

  /*
	int i = 0;
	char dataHex;

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

	//for(;i < 1; i++){
	//	byte a, b, c; 
	ParPortWrite(dataHex); 	
	
		//a = inb(pport);	
		//b = inb(pport+1);	
		//c = inb(pport+2);	
		//printf("%X\t%X\t%X\n",a, b, c);
		//printf("%X\t\n",a);
		//usleep(1000000);
	//}
	*/
