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
#define WAIT_TIME 10000             //Wait Time for Sleep Function. 50000 = 0.1 seconds

//Parallel Port Setup
#define ppData pport              //Data Address is base address of Parallel Port.
#define ppStatus pport + 1        //Status Register Address of Parallel Port.
#define ppControl pport + 2       //Control Register Address of Parallel Port.
#define numPorts 3                //Number of addresses associated with the Parallel Port.

//Bus Clear Setup
#define MSG_CLEAR 0x0

//Ping Command Setup
#define MSG_PING 0x1              // Ping Command.
#define MSG_ACK_PING 0xE          // PING Acknowledgment Command. 

//Get Command Setup
#define MSG_GET 0x2               // Get Command.
#define MSG_ACK_GET 0xC           // Get Acknowledgment Command. 

//Reset Command Setup
#define MSG_RESET 0x3             //Reset command.
#define MSG_ACK_RESET 0xD         // Reset Acknowledgment Command. 

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
void DisplayData(); 
void ClearTerminal();

//--------------------------- Global Variables -------------------------------
//Structure to define the DateTime in an easily accessible way.
//struct RTCData{
//    unsigned char seconds = 0;
//    unsigned char minutes = 0;
//    unsigned char hours = 0;
//    unsigned char day = 0;
//    unsigned char date = 0;
//    unsigned char month = 0;
//    unsigned char year = 0;
//    unsigned char control = 0;
//};

BYTE RTCData[8]; // Decide whether we want to use a struct or Array for this
unsigned short adcResult = 0;


BYTE RTCData[8]; // Decide whether we want to use a struct or Array for this
unsigned short adcResult = 0;

//---------------------------- main ------------------------------------------------
//Purpose: Initial insertion into the program.

int main(int argc, char *argv[]){
  
  //Local variables.
  int cmd; //Command read from the command line.
  int ignored; //Ignored character buffer for the command line.
  int commandsuccess; //Variable for checking Success of a command.

  //Open Parallel port.
  ClearTerminal();  
  printf("\n\nStatus: Opening port 0x378\n\n");
  commandsuccess = ioperm(0x378, numPorts, 1); //Set up parallel port permissions. 

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
  outb(0x00,ppControl); // STROBE HIGH / OUTPUT
  outb(MSG_CLEAR,ppData); //Initial value on data bus of 0
  
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

//------------------------------- ParPortRead -------------------------------------------
//Purpose: Execute a 4-bit read of the Parallel Port.

BYTE ParPortRead(){
    
    BYTE byte;
    usleep(WAIT_TIME); //PIC will see low and output to the BUS

    outb(0x21, ppControl); //STROBE LOW / INPUT
    
    usleep(WAIT_TIME); //PIC will see low and output to the BUS

    outb(0x20, ppControl); //STROBE HIGH / INPUT
    
    //Read the data on the bus.
    byte = inb(ppData);   
    
    //printf("\nParallel Port Read: %X",byte & 0x0F);
    
    //Extract lower bits of read variable.
    byte &= 0x0F;
    
    usleep(WAIT_TIME); //PIC will see low and output to the BUS

    outb(0x01, ppControl); //STROBE LOW / OUTPUT

    usleep(WAIT_TIME); //PIC will see low and output to the BUS

    //outb(MSG_CLEAR,ppData); //Clear the Data Bus
    outb(0x00, ppControl); //STROBE HIGH / OUTPUT
    
    return byte;
}

//------------------------------- ParPortWrite -----------------------------------------
//Purpose: Execute a 4-bit write on the Parallel Port.

void ParPortWrite(BYTE byte){

  usleep(WAIT_TIME); //PIC will see low and output to the BUS
  outb(0x01, ppControl); //STROBE LOW / OUTPUT
  
  //Output command to data bus.
  usleep(WAIT_TIME); //PIC will see low and output to the BUS
  outb(byte, ppData);
  //printf("\nParallel Port Wrote: %X",byte & 0x0F);
  
  usleep(WAIT_TIME); //PIC will see low and output to the BUS
  outb(0x00, ppControl); //STROBE HIGH / OUTPUT
  
  usleep(WAIT_TIME); // Wait for PIC to read data on BUS.  
  
  outb(0x01, ppControl); //STROBE LOW / OUTPUT

  usleep(WAIT_TIME); //PIC will see low and output to the BUS

  outb(0x00, ppControl); //STROBE HIGH / OUTPUT
    
}

//--------------------------------- Read Byte --------------------------------------------
//Purpose: Read an entire byte by breaking it up into 4-bit parts.

BYTE ReadByte(){
 
  BYTE byte;
  
  //Reconstruct the two 4bit messages into a byte
  byte = (ParPortRead()<<4) & 0xF0;

  byte |= ParPortRead();
  
  return byte;  
}


//--------------------------------- Reset -------------------------------------------------
//Purpose: Execute the entire process of a 'Reset' command.

int Reset(){

  BYTE ackResult;

  //Write PING Command to PIC.
  ParPortWrite(MSG_RESET); 

  ackResult=ParPortRead();

  if( ackResult != MSG_ACK_RESET ){
    printf("\nStatus: Reset command failed");
    return FALSE ;
  }
  
  printf("\nStatus: Reset command success");
  return TRUE ;

}


//------------------------------------ Get ------------------------------------------------
//Purpose: Execute the entire process of a 'Get' command. 

int Get(){

    BYTE ackResult;
    
    //Write GET Command to PIC.
    ParPortWrite(MSG_GET);
    /*
    int x=0;
    while(x<19)
      {
	ParPortRead();
	x++;     
      }
    */
   //Read the ACK from the GET on the data line.
    ackResult = ParPortRead();
    
    if( ackResult != MSG_ACK_GET) {
      printf("\nStatus: Get command failed");
        return FALSE ;
    }
    
    //Read 3 ADC Nibles and assemble them
    adcResult = ( ParPortRead() << 8 ) & 0x0300; // bit 8 and 9.
    adcResult |= ReadByte(); // ReadByte reads low Byte and
    
    // Read RTC data for all 8 Bytes of information.
    int i ;
    for(i = 0 ; i < 8 ; i++) {
      RTCData[i] = ReadByte() ;
    }
    
    //Read the ACK from the GET on the data line.
    ackResult = ParPortRead();
    
    if( ackResult != MSG_ACK_GET) {
      printf("\nStatus: Get command failed");
        return FALSE ;
    }
    
      // Display Data
      DisplayData();
    
    return TRUE;
}


//----------------------------------- Ping ------------------------------------------------
//Purpose: Execute the entire process of a 'Ping' command.

int Ping(){

  BYTE ackResult;

  //Write PING Command to PIC.
  ParPortWrite(MSG_PING); 

  ackResult=ParPortRead();

  if( ackResult != MSG_ACK_PING ){
    printf("\nStatus: Ping command failed");
    return FALSE ;
  }
  
  printf("\nStatus: Ping command success");
  return TRUE ;
}

//------------------------------- DisplayData --------------------------------------------
//Purpose: Diaplay the ADC and RTC data retreived from the PIC. 

void DisplayData(){
 char day ;
    
    switch ( RTCData[3] ) {
        case 2:
            day = 'M';
            break;
        case 3:
            day = 'T';
            break;
        case 4:
            day = 'W';
            break;
        case 5:
            day = 'R';
            break;
        case 6:
            day = 'F';
            break;
        case 7:
            day = 'S';
            break;
        case 1:
            day = 'U';
            break;
        default:
            break;
    }
   
    printf("\nADC result: %u" , adcResult) ;
    
    //                                                   Hour         Minutes     Seconds          Month        date          year
    printf("\nTime: %02u:%02u:%02u %c %02u/%02u/20%02u" , RTCData[2], RTCData[1], RTCData[0], day, RTCData[5], RTCData[4], RTCData[6] );

}

//------------------------------- ClearTerminal ------------------------------------------
//Purpose: Clear the terminal screen.

void ClearTerminal()
{
  //Not using system("clear") because it does not behave corretly.
  //printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  //printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  printf("\n\n");
  //sleep(1);
}



