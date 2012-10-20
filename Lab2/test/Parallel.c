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
#define ppStrobe pport + 2      //Control Register Address of Parallel Port.
#define MSG_RESET 0x0           //Reset command.
#define MSG_PING 0x1            // Ping Command.
#define MSG_GET 0x2             // Get Command.
#define MSG_ACK 0xE             // General Acknowledgment Command. 
#define MSG_ACKRESET 0XD        // Reset Acknowledgment Command.
#define MSG_NOTHING 0xF         // No Operation Command.


//Function Declarations 

int OpenPort(int portAdr);     
int ParPortWrite(BYTE byte);
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
  int commandsuccess = 1;

  commandsuccess = OpenPort(pport);

  if(commandsuccess){
    printf("Status: Port %x Opened",pport);
  } 
  else {
    printf("Status: Port %x could not be opened. Ensure root access.", pport);
    sleep(5);
    return(-1);
  }
  
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


//ParPortWrite
//Purpose: Execute a 4-bit write on the Parallel Port.

int ParPortWrite(BYTE byte){

  outb(0x01, ppStrobe); //Set the strobe low.

  outb(byte, ppData); //Output command to data bus.

  outb(0x00, ppStrobe); //Set the strobe high.

  usleep(100);

  outb(0x01, ppStrobe); // Set the strobe to low.

  return TRUE;
} 


//ParPortRead
//Purpose: Execute a 4-bit read of the Parallel Port.

BYTE ParPortRead(){

  //Local varaibles.
  BYTE byte;

  outb(0x21, ppStrobe); //Set strobe low and bit 5 high for input mode.

  outb(0x20, ppStrobe); //Set strobe high and input mode.

  usleep(100);

  byte = inb(ppData);

  outb(0x21, ppStrobe);  //Set strobe low and bit 5 high for input mode.

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

  BYTE result;

  ParPortWrite(MSG_RESET);
  result = ParPortRead();

  if( result != MSG_ACKRESET ) {
    printf("\nStatus: Reset command failed");
    return FALSE ;
  }

  printf("\nStatus: Reset command executed");
  return TRUE ; 

}


//Get
//Purpose: Execute the entire process of a 'Get' command. 

int Get(){

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
  
  // Read RTC data for all 8 Bytes of information.
  int i ;
  for(i = 0 ; i < 8 ; i++) {
    RTCData[i] = ReadByte() ;
  }
  
  // Get Acknowledgement 
  if(!Ping()){
    printf("\nStatus: Get command failed");   
    return FALSE ;
  }

  // Display Data 
  DisplayData( RTCData , adcResult );
  
  printf("\nStatus: Get command executed");
  return TRUE;
}


//Ping
//Purpose: Execute the entire process of a 'Ping' command.

int Ping(){
  
  BYTE result ;

  ParPortWrite(MSG_PING);
  result = ParPortRead();

  if( result != MSG_ACK ){
    printf("\nStatus: Ping command failed");
    return FALSE ;
  }

  printf("\nStatus: Ping command executed");
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
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

