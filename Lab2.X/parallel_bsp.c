////////////////////////////////////////////////////////////////////////////////
/*
    UMass Lowell 16.480/552, Fall 2012

    Purpose : 16.480 Microprocessors II Lab 1
    File: parallel_bsp.c

    Group Name: Shoulda Been Gangstas
    Group Members:
        - Jesse Whitworth
        - Matthew Cook
        - Denis Lemos
        - Aadil Hassan

    License: GNU GPLv3.

    Date Created: 10-20-2012
*/
////////////////////////////////////////////////////////////////////////////////

#include "parallel_bsp.h"
#include "ds1307_bsp.h"
#include "p18f45k20_bsp.h"

//--------------------------- CheckParallel () ----------------------------------------
//Purpose: Read 4 bits from the bus to conclude which command is being used.
//When Strobe is low
void CheckParallel(void) {
        BYTE command = 0;

        while (!STROBE) continue; //Wait for STROBE HIGH
        ReadData(&command); //Read the command from the BUS
        while (STROBE) continue; //Wait for STROBE LOW

        while (!STROBE) continue; //Wait for STROBE HIGH
        while (STROBE) continue; //Wait for STROBE LOW

        switch (command) {
            case MSG_PING:
                WriteData(MSG_ACK_PING); //Send ack. (Only task)
                break;
            case MSG_RESET:
                WriteData(MSG_ACK_RESET); //Send ack.
                ResetConnection(); //Reset the connection.
                break;
            case MSG_GET:
                WriteData(MSG_ACK_GET); //Send ack.
                GetCommand(); //Run the GET command
                break;
            default:
                while (!STROBE) continue; //Remain here while the strobe remains high
                break;
        }
        while (!STROBE) continue; //Remain here while the strobe remains high
}

/*--------------------------- ResetConnection () ------------------------------------------------------
 Purpose     : Reset the connection and all necessary variables.
 Parameters  : N/A
 Output      : N/A
*/
void ResetConnection(void){

}


/*--------------------------- ReadData () ------------------------------------------------------
 Purpose     : Read data from the parallel port.
 Parameters  : N/A
 Output      : N/A
*/
BYTE ReadData(unsigned char * readResult){
    TRISD |= 0x0F; // Set the port as input

    //Grab data
    *readResult |= (PortD3 << 3);
    *readResult |= (PortD2 << 2);
    *readResult |= (PortD1 << 1);
    *readResult |= (PortD0);

    *readResult &= 0x0F;
}

/*--------------------------- WriteData () ------------------------------------------------------
 Purpose     : Write data to the parallel port.
 Parameters  : N/A
 Output      : N/A
*/
void WriteData(BYTE data) {
    TRISD &= 0xF0; //Set as an output.

    PortD3 = ((data >> 3) & 1);
    PortD2 = ((data >> 2) & 1);
    PortD1 = ((data >> 1) & 1);
    PortD0 = ((data) & 1);

    while(!STROBE) continue; //Linux Reading
    while(STROBE) continue; //Linux Done Reading

    PortD3 = 0;
    PortD2 = 0;
    PortD1 = 0;
    PortD0 = 0;
}

/*--------------------------- HighNibble () ------------------------------------------------------
 Purpose     : Make a high nibble
 Parameters  : N/A
 Output      : N/A
*/
BYTE HighNibble(BYTE byte) {

    return (byte >> 4) & 0x0F;
}

/*--------------------------- LowNibble () ------------------------------------------------------
 Purpose     : Make a low nibble
 Parameters  : N/A
 Output      : N/A
*/
BYTE LowNibble(BYTE byte) {

    return byte & 0x0F;

}



/*--------------------------- GetCommand () ------------------------------------------------------
 Purpose     : Send the ADC and then send the Time
 Parameters  : N/A
 Output      : N/A
*/
void GetCommand(void){
    SendADC(); //Start with the ADC first.
    //SendTime(); //Finish with the time.
}

/*--------------------------- SendADC () ------------------------------------------------------
 Purpose     : Write data to the parallel port.
 Parameters  : N/A
 Output      : N/A
*/
void SendADC(){
    /*
    unsigned char adcRead = ReadADC();
     //-----------------------------------------------------------STROBE HIGH
    /////////////////////////////3 ADC WRITES//////////////////////////////
    // High Niblle
    WriteData( (adcRead & 0x0300) >> 8 );
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;

    //-----------------------------------------------------------STROBE HIGH
   // Mid Nibble
    WriteData( (adcRead & 0x00F0) >> 4 );
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while (!STROBE) continue;

    //-----------------------------------------------------------STROBE HIGH
    // Low nible
    WriteData( adcRead & 0x000F );
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while (!STROBE) continue;
     */
}

/*--------------------------- SendTime () ------------------------------------------------------
 Purpose     : Send the time data over the parallel port.
 Parameters  : N/A
 Output      : N/A
*/
void SendTime() {
/*
    unsigned char seconds = 0;
    unsigned char minutes = 0;
    unsigned char hours = 0;
    unsigned char day = 0;
    unsigned char date = 0;
    unsigned char month = 0;
    unsigned char year = 0;
    unsigned char control = 0;
    
    ReadTimeDS1307(&seconds, &minutes, &hours, &day, &date, &month, &year , &control); //Get time from RTC

    /////////////////////////////16 RTC WRITES//////////////////////////////
    // Seconds
    //-----------------------------------------------------------STROBE HIGH  
    WriteData(HighNibble (seconds));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;
     //-----------------------------------------------------------STROBE HIGH  
    WriteData(LowNibble (seconds));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;

    //-----------------------------------------------------------STROBE HIGH  
    // Minutes
    WriteData(HighNibble(minutes));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;
    //-----------------------------------------------------------STROBE HIGH  
    WriteData(LowNibble(minutes));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while (!STROBE) continue;

    //-----------------------------------------------------------STROBE HIGH 
    // Hour
    WriteData(HighNibble(hours));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while (!STROBE) continue;
    //-----------------------------------------------------------STROBE HIGH  
    WriteData(LowNibble(hours));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;

    //-----------------------------------------------------------STROBE HIGH 
    // Day
    WriteData(HighNibble(day));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;
    //-----------------------------------------------------------STROBE HIGH  
    WriteData(LowNibble(day));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;

    //-----------------------------------------------------------STROBE HIGH  
    // Date
    WriteData(HighNibble(date));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;
    //-----------------------------------------------------------STROBE HIGH  
    WriteData(LowNibble(date));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;

    //-----------------------------------------------------------STROBE HIGH
    // Month
    WriteData(HighNibble(month));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;
    //-----------------------------------------------------------STROBE HIGH
    WriteData(LowNibble(month));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;
   
    //-----------------------------------------------------------STROBE HIGH
    // Year
    WriteData(HighNibble(year));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;
    //-----------------------------------------------------------STROBE HIGH
    WriteData(LowNibble(year));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;

    //-----------------------------------------------------------STROBE HIGH
    // Date
    WriteData(HighNibble(control));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while (!STROBE) continue;
    //-----------------------------------------------------------STROBE HIGH
    WriteData(LowNibble(control));
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW
    while(!STROBE) continue;
*/
 }

