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


//--------------------------------- Includes -------------------------------------

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>


//--------------------------------- Defines --------------------------------------

#define BYTE unsigned char

#define TRUE 1
#define FALSE 0
#define pport 0x378               //Base Address of Parallel Port.
#define WAIT_TIME 1             //Wait Time for Sleep Function.

//Parallel Port Setup
#define ppData pport              //Data Address is base address of Parallel Port.
#define ppStatus pport + 1        //Status Register Address of Parallel Port.
#define ppControl pport + 2       //Control Register Address of Parallel Port.
#define numPorts 3                //Number of addresses associated with the Parallel Port.

//Ping Command Setup
#define MSG_PING 0x1              // Ping Command.
#define MSG_ACK_PING 0x4          // PING Acknowledgment Command. 

//Get Command Setup
#define MSG_GET 0x2               // Get Command.
#define MSG_ACK_GET 0x5           // Get Acknowledgment Command. 

//Reset Command Setup
#define MSG_RESET 0x3             //Reset command.
#define MSG_ACK_RESET 0x6         // Reset Acknowledgment Command. 

//Nothing Command Setup
#define MSG_NOTHING 0xF           // No Operation Command.



//--------------------------- Function Declarations ------------------------------- 

int OpenPort(int portAdr);                           //Purpose: Open port with the designated port address.    
void ParPortWrite(BYTE byte);                        //Purpose: Execute a 4-bit write on the Parallel Port.
BYTE ParPortRead();                                  //Purpose: Execute a 4-bit read of the Parallel Port.
BYTE ReadByte();                
int Reset();
int Get();
int Ping();
void DisplayData(BYTE RTCData[], int adcResult ); 
void ClearTerminal();


//---------------------------- main ------------------------------------------------
//Purpose: Initial insertion into the program.

int main(int argc, char *argv[]){
  
  //Local variables.
  int cmd;                                                                              //Command read from the command line.
  int ignored;                                                                          //Ignored character buffer for the command line.
  int commandsuccess;                                                                   //Variable for checking Success of a command.

  //Open Parallel port.
  ClearTerminal();  
  printf("\n\nStatus: Opening port 0x378\n\n");
  commandsuccess = ioperm(0x378, numPorts, 1);                                          //Set up parallel port permissions. 

  //Check status of opening parallel port.
  ClearTerminal();
  if(!commandsuccess){
    printf("\nStatus: Port %x Opened\n\n",pport);
    sleep(3);
  } 
  else {
    printf("\n\nStatus: Port %x could not be opened. Ensure root access.\n\n", pport);
    sleep(3);
    ClearTerminal();
    return(-1);
  }

  //Set initial state of STROBE and Data Bus
  outb(0x01,ppControl); //Strobe low. 
  outb(0x00,ppData);	//Initial value on data bus of 0
  
  while(TRUE){

    //Prompt user with the program operation.
    printf("\n\nWhen entering a command only the first letter will be handled.");
    printf("\nAll other characters will be ignored."); 
    printf("\n\nCommands:\n\tR = Reset\n\tG = Get\n\tP = Ping\n\tQ = Quit\n");
   
    //Ask user for input
    printf("\n\nEnter a command: ");
    
    //Handle user input.
    cmd = getc(stdin);
    do {
      ignored = getc(stdin);
    } while ((ignored != '\n') && (ignored != EOF));
       
    //Handle command operation.
    switch(toupper(cmd)){
    case 'R':
      ClearTerminal();
      //commandsuccess = Reset();
      break;
    case 'G':
      ClearTerminal();
      //commandsuccess = Get();
      break;
    case 'P':
      ClearTerminal();
      commandsuccess = Ping();
      break;
    case 'Q':
      ClearTerminal();
      commandsuccess = TRUE;
      ioperm(pport, numPorts, 0); //Release Ports.
     return (0);		
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


//--------------------------------- OpenPort ------------------------------------------
//Purpose: Open port with the designated port address.

int OpenPort(int portAdr){
  
  ClearTerminal();

  printf("Status: Opening Port %X\n\n\n\n\n\n\n\n\n\n\n\n", portAdr);

  if(ioperm(portAdr,3,1)) return FALSE;

  sleep(WAIT_TIME);

  ClearTerminal();

  return TRUE;
}


//------------------------------- ParPortWrite -----------------------------------------
//Purpose: Execute a 4-bit write on the Parallel Port.

void ParPortWrite(BYTE byte){

  //Output command to data bus.
  outb(byte, ppData);                                      
  printf("\nParallel Port Wrote: %x",byte & 0x0F);
  sleep(WAIT_TIME);

  // Set the strobe to high.
  outb(0x00, ppControl);                                  
  sleep(WAIT_TIME);
  
  // Set the strobe to low.
  outb(0x01, ppControl); 
  sleep(WAIT_TIME);

} 


//------------------------------- ParPortRead -------------------------------------------
//Purpose: Execute a 4-bit read of the Parallel Port.

BYTE ParPortRead(){

  //Local varaibles.
  BYTE byte;

  //Set strobe high.
  outb(0x20, ppControl); 
  sleep(WAIT_TIME);

  //Read the data on the bus.
  byte = inb(ppData);    
  printf("\nParallel Port Read: %x",byte & 0x0F);
  sleep(WAIT_TIME);
  
  //Set strobe low.
  outb(0x21, ppControl);  
  sleep(WAIT_TIME);

  //Extract lower bits of read variable.
  byte &= 0x0F;

  //Put stobe and data back to low and low impedance mode
  //outb(0x01, ppControl);  

  //Return byte read.
  return byte;
}


//--------------------------------- Read Byte --------------------------------------------
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


//--------------------------------- Reset -------------------------------------------------
//Purpose: Execute the entire process of a 'Reset' command.

int Reset(){

  BYTE ackResult;

  ParPortWrite(MSG_RESET);
  
  ackResult = ParPortRead();	

  if( ackResult != MSG_ACK_RESET ) {
    printf("\nStatus: Reset command failed");
    return FALSE ;
  }

  return TRUE ; 
}


//------------------------------------ Get ------------------------------------------------
//Purpose: Execute the entire process of a 'Get' command. 

int Get(){

  BYTE ackResult;

  //Write GET Command to PIC.
  ParPortWrite(MSG_GET);
  
  //Place Get Code Here

  //Read the ACK from the GET on the data line.
  //ackResult = ParPortRead();

  if( ackResult != MSG_ACK_GET) {
    printf("\nStatus: Get command failed");
    return FALSE ;
  }

  return TRUE;
}


//----------------------------------- Ping ------------------------------------------------
//Purpose: Execute the entire process of a 'Ping' command.

int Ping(){

  BYTE ackResult;

  //Write PING Command to PIC.
  ParPortWrite(MSG_PING); 

  //Read the ACK from the PING on the data line.
  ackResult = ParPortRead();

  if( ackResult != MSG_ACK_PING ){
    printf("\nStatus: Ping command failed");
    return FALSE ;
  }
  
  return TRUE ;
}

//------------------------------- DisplayData --------------------------------------------
//Purpose: Diaplay the ADC and RTC data retreived from the PIC. 

void DisplayData(BYTE RTCData[], int adcResult ){


}

//------------------------------- ClearTerminal ------------------------------------------
//Purpose: Clear the terminal screen.

void ClearTerminal()
{
  //Not using system("clear") because it does not behave corretly.
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}



/*//Some Get Sudo Code
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

