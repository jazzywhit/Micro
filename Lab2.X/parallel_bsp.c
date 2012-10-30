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
#include "Globals.h"

//Custom Parallel Port Bus Interface.
#define PortD0 PORTDbits.RD0
#define PortD1 PORTDbits.RD1
#define PortD2 PORTDbits.RD2
#define PortD3 PORTDbits.RD3
#define STROBE PORTDbits.RD5
//#define ACKPIN PORTCbits.RD7

//--------------------------- CheckParallel () ----------------------------------------
//Purpose: Read 4 bits from the bus to conclude which command is being used.
//When Strobe is low
void CheckParallel(timeStr *dateTime) {

    if (!STROBE){
        unsigned char command = 0;
        
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
                GetCommand(dateTime); //Run the GET command
                break;
            default:
                while (!STROBE) continue; //Remain here while the strobe remains high
                break;
        }
    }

}

/*--------------------------- ResetConnection () ------------------------------------------------------
 Purpose     : Reset the connection and all necessary variables.
 Parameters  : N/A
 Output      : N/A
*/
void ResetConnection(void){
    ADRESH = 0;
}

/*--------------------------- ReadData () ------------------------------------------------------
 Purpose     : Read data from the parallel port.
 Parameters  : N/A
 Output      : N/A
*/
void ReadData(unsigned char *readResult){
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
void WriteData(unsigned char data) {
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

    while (!STROBE) continue; //Remain here while the strobe remains high
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
void GetCommand(timeStr *dateTime){
    SendADC(); //Start with the ADC first.
    SendTime(dateTime); //Finish with the time.
}

/*--------------------------- SendADC () ------------------------------------------------------
 Purpose     : Write data to the parallel port.
 Parameters  : N/A
 Output      : N/A
*/
void SendADC(void){
    
    unsigned char adcRead = ReadADC();

    WriteData( (adcRead & 0x0300) >> 8 );

    WriteData( (adcRead & 0x00F0) >> 4 );

    WriteData( adcRead & 0x000F );
}

/*--------------------------- SendTime () ------------------------------------------------------
 Purpose     : Send the time data over the parallel port.
 Parameters  : N/A
 Output      : N/A
*/
void SendTime(timeStr *dateTime) {
    
     //Get time from RTC
    /////////////////////////////16 RTC WRITES//////////////////////////////
    // Seconds
    //-----------------------------------------------------------STROBE HIGH
    WriteData(HighNibble (dateTime->seconds));


     //-----------------------------------------------------------STROBE HIGH
    WriteData(LowNibble (dateTime->seconds));

    //-----------------------------------------------------------STROBE HIGH
    // Minutes
    WriteData(HighNibble(dateTime->minutes));

    //-----------------------------------------------------------STROBE HIGH
    WriteData(LowNibble(dateTime->minutes));

    //-----------------------------------------------------------STROBE HIGH
    // Hour
    WriteData(HighNibble(dateTime->hours));

    //-----------------------------------------------------------STROBE HIGH
    WriteData(LowNibble(dateTime->hours));

    //-----------------------------------------------------------STROBE HIGH
    // Day
    WriteData(HighNibble(dateTime->day));

    //-----------------------------------------------------------STROBE HIGH
    WriteData(LowNibble(dateTime->day));


    //-----------------------------------------------------------STROBE HIGH
    // Date
    WriteData(HighNibble(dateTime->date));

    //-----------------------------------------------------------STROBE HIGH
    WriteData(LowNibble(dateTime->date));


    //-----------------------------------------------------------STROBE HIGH
    // Month
    WriteData(HighNibble(dateTime->month));

    //-----------------------------------------------------------STROBE HIGH
    WriteData(LowNibble(dateTime->month));

    //-----------------------------------------------------------STROBE HIGH
    // Year
    WriteData(HighNibble(dateTime->year));

    //-----------------------------------------------------------STROBE HIGH
    WriteData(LowNibble(dateTime->year));

    //-----------------------------------------------------------STROBE HIGH
    // Date
    WriteData(HighNibble(dateTime->control));

    //-----------------------------------------------------------STROBE HIGH
    WriteData(LowNibble(dateTime->control));

}

