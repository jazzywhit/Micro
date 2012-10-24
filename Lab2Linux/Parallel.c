/*
Lab 2 : Interfacing with a Sensor Device on an Embedded Computer system.
Due Date : 10 - 24 - 12
Group Name : Shoulda Been Gangstas
Group Members:
   Jesse Whitworth
   Dennis Lemos
   Aadil Hassan
   Matthew Cook
*/


//Includes

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>



//Defines

#define BYTE unsigned char
#define TRUE 1
#define FALSE 0
#define pport 0x378             //Base Address of Parallel Port.
#define ppData pport            //Data Address is base address of Parallel Port.
#define ppStatus pport + 1      //Status Register Address of Parallel Port.
#define ppControl pport + 2      //Control Register Address of Parallel Port.
#define MSG_RESET 0x3           //Reset command.
#define MSG_PING 0x1            // Ping Command.
#define MSG_GET 0x2             // Get Command.
#define MSG_ACK_PING 0xE             // PING Acknowledgment Command. 
#define MSG_ACK_RESET 0xD       // Reset Acknowledgment Command. 
#define MSG_ACK_GET 0xC       // Get Acknowledgment Command. 
#define MSG_NOTHING 0xF         // No Operation Command.
#define WAIT_TIME 1             // Strobe duty cycle

//Function Declarations 

int OpenPort(int portAdr);     
BYTE ParPortWrite(BYTE byte);
BYTE ParPortRead();
BYTE ReadByte();                
int Reset();
int Get();
int Ping();
void DisplayData(BYTE RTCData[], int adcResult ); 
void ClearTerminal();


//main
//Purpose: Initial insertion into the program.

int main(int argc, char *argv[]){
  
  //Local variables.
  int scanfresult;
  int cmd;
  int choice;
  int ignored;
  int commandsuccess = TRUE;

  //int parportfd = open("/dev/parport0/", O_RDWR);
  //if (result != 0) result = ioctl(parportfd, PPCLAIM);
  commandsuccess = ioperm(0x378, 3, 1);

  if(!commandsuccess){
    printf("Status: Port %x Opened",pport);
  } 
  else {
    printf("Status: Port %x could not be opened. Ensure root access.", pport);
    sleep(5);
    return(-1);
  }


  //Set initial state of STROBE and Data Bus
  outb(0x01,ppControl); //Strobe low. 
  outb(0x00,ppData);	//Initial value of 0

  while(TRUE){
    printf("\n\nWhen entering a command only the first letter will be handled.");
    printf("\nAll other characters will be ignored."); 
    printf("\n\nCommands:\n\tR = Reset\n\tG = Get\n\tP = Ping\n\tQ = Quit\n");
   
    printf("\n\nEnter a command: ");
    
    cmd = getc(stdin);
    do {
      ignored = getc(stdin);
    } while ((ignored != '\n') && (ignored != EOF));
       
    switch(toupper(cmd)){
    case 'R':
      ClearTerminal();
      commandsuccess = Reset();
      break;
    case 'G':
      ClearTerminal();
      commandsuccess = Get();
      break;
    case 'P':
      ClearTerminal();
      commandsuccess = Ping();
      break;
    case 'Q':
      ClearTerminal();
      commandsuccess = TRUE;
      //result = ioctl(parportfd, PPRELEASE);
      //close(parportfd);
      return(0);
      break;
    default:
      ClearTerminal();
      printf("Command not valid. Enter(R, G, P, Q)");
      commandsuccess = FALSE;
      break;
    }
  }
  return(0);
}


//OpenPort
//Purpose: Open port with the designated port address.

int OpenPort(int portAdr){
  
  ClearTerminal();

  printf("Status: Opening Port %X\n\n\n\n\n\n\n\n\n\n\n\n", portAdr);

  if(ioperm(portAdr,3,1)) return FALSE;

  sleep(1);

  ClearTerminal();

  return TRUE;
}


//------------------------------- ParPortWrite -----------------------------------------
//Purpose: Execute a 4-bit write on the Parallel Port.
BYTE ParPortWrite(BYTE byte){

  outb(0x00, ppControl); // Set the strobe to high.(4
  sleep(1);
  
  outb(byte, ppData); //Output command to data bus.(2)
  printf("\nParPortWrite: %x",byte & 0x0F);
  sleep(1);

  outb(0x01, ppControl); // Set the strobe to low.(4
  sleep(1);

  return byte;

} 


//------------------------------- ParPortRead ---------------------------------------------
//Purpose: Execute a 4-bit read of the Parallel Port.

BYTE ParPortRead(){

  //Local varaibles.
  BYTE byte;
  
  outb(0x20, ppControl); //Set strobe high.(3)
  sleep(1);

  byte = inb(ppData);    //Read the data on the bus.
  printf("\nParPortRead: %x",byte & 0x0F);
  sleep(1);
  
  outb(0x21, ppControl);  //Set strobe low.(4)
  sleep(1);

  return byte;
}


//Read Byte
//Purpose: Read an entire byte by breaking it up into 4-bit parts.

BYTE ReadByte(){
  
  BYTE temp;
  BYTE byte;
  
  //Reconstruct the two 4bit messages into a byte
  temp = ParPortRead();
  byte = temp<<4 & 0xF0; 
  temp = ParPortRead();
  byte |= temp;
  
  return byte;  
}


//Reset
//Purpose: Execute the entire process of a 'Reset' command.

int Reset(){

  BYTE ackResult;

  ParPortWrite(MSG_RESET);
  
  ackResult = ParPortRead();
  ackResult &= 0x0F;	

  if( ackResult != MSG_ACK_RESET ) {
    printf("\nStatus: Reset command failed");
    return FALSE ;
  }

  return TRUE ; 
}


//Get
//Purpose: Execute the entire process of a 'Get' command. 

int Get(){

  BYTE ackResult;

  ParPortWrite(MSG_GET);
  
  ackResult = ParPortRead();
  ackResult &= 0x0F;	

  if( ackResult != MSG_ACK_GET) {
    printf("\nStatus: Get command failed");
    return FALSE ;
  }

  return TRUE;
}


//Ping
//Purpose: Execute the entire process of a 'Ping' command.

int Ping(){
  
  //outb(0x00, ppControl); // Set the strobe to high.(4

  BYTE ackResult;

  //Tell the PIC you are doing a PING.
  ParPortWrite(MSG_PING); //3 seconds sleep

  //Read the ACK from the PING on the data line.
  ackResult = ParPortRead(); // 3 seconds sleep
  ackResult &= 0x0F;

  if( ackResult != MSG_ACK_PING ){
    printf("\nStatus: Ping command failed");
    return FALSE ;
  }
  
  return TRUE ;
}

//DisplayData
//Purpose: Diaplay the ADC and RTC data retreived from the PIC. 

void DisplayData(BYTE RTCData[], int adcResult ){


}

//ClearTerminal
//Purpose: Clear the terminal screen.

void ClearTerminal()
{
  //Not using system("clear") because it does not behave corretly.
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}



/*
  BYTE RTCData[8];
  BYTE adcHighNibble; 
  BYTE adcLowByte;
  int adcResult;
  BYTE ackResult;
  
  // Send Command
  ParPortWrite(MSG_GET);
  

  // Read 3 Nible from ADC 
  adcHighNibble = ParPortRead();
  adcLowByte = ReadByte();
  adcResult = ((adcHighNibble&0x0F) * 16 * 16) + adcLowByte ;
  
  // Read RTC data for all 8 Bytes of information.
  int i ;
  for(i = 0 ; i < 8 ; i++) {
    RTCData[i] = ReadByte() ;
  }

  ackResult = ParPortRead();

  // Get Acknowledgement 
  if( ackResult != MSG_ACK ) {
    printf("\nStatus: Get command failed");   
    return FALSE ;
  }

  // Display Data 
  DisplayData( RTCData , adcResult );
  */

