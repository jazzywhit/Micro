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
#define MSG_RESET 0x0           //Reset command.
#define MSG_PING 0x1            // Ping Command.
#define MSG_GET 0x2             // Get Command.
#define MSG_ACK 0xE             // General Acknowledgment Command. 
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
  int commandsuccess = TRUE;

  commandsuccess = OpenPort(pport);

  if(commandsuccess){
    printf("Status: Port %x Opened",pport);
  } 
  else {
    printf("Status: Port %x could not be opened. Ensure root access.", pport);
    sleep(5);
    return(-1);
  }

  //Set initial state of STROBE and Data Bus
  outb(0x00,ppControl); //Strobe high.
  outb(0xF,ppData);
  
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
  
  outb(0x01, ppControl); //Set the strobe low.(1)
  
  outb(byte, ppData); //Output command to data bus.(2)

  outb(0x00, ppControl); //Set the strobe high.(3)
  
  sleep(.015);// Maintain signal for 15ms(3)
  
  outb(0x01, ppControl); // Set the strobe to low.(4)
  
  outb(MSG_NOTHING, ppData); //Stop writing data to data port.(5)

  return TRUE;
} 


//ParPortRead
//Purpose: Execute a 4-bit read of the Parallel Port.

BYTE ParPortRead(){

  //Local varaibles.
  BYTE byte;
  
  outb(0x21, ppControl); //Set strobe low and bit 5 high for input mode.(1)
  
  sleep(0.015); //Sleep 15ms while pic puts data on bus.(2)

  outb(0x20, ppControl); //Set strobe high and input mode.(3)

  byte = inb(ppData); //read the value from the bus.(3)

  outb(0x21, ppControl);  //Set strobe low and bit 5 high for input mode. (4)

  sleep(0.015); //Sleep 15ms while pic puts data on bus. LINUX done reading.(5)

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
  
  sleep(0.015); //Sleep until PIC finishes reset.

  ackResult = ParPortRead();

  if( ackResult != MSG_RESET ) {
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
  
  printf("\nStatus: Get command executed");
  return TRUE;
}


//Ping
//Purpose: Execute the entire process of a 'Ping' command.

int Ping(){
  
  BYTE ackResult ;

  ParPortWrite(MSG_PING);
  
    ackResult = ParPortRead();
  
  if( ackResult != MSG_ACK ){
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
  //Not using system("clear") because it does not behave corretly.
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

