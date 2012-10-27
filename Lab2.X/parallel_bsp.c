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
#include <p18f45k20.h>

// Read Data Bus
// Reads data from the parallel port data pins and reconstructs the value as a BYTE
unsigned char test = 1;

BYTE ReadDataBus(){

    BYTE readResult = 0;
    //Grab data
    readResult |= (PortD3 << 3);
    readResult |= (PortD2 << 2);
    readResult |= (PortD1 << 1);
    readResult |= (PortD0);

    readResult &= 0x0F;

    return readResult;
}

//Read Command
//Purpose: Read 4 bits from the bus to conclude which command is being used.
//When Strobe is low

void ReadCommand() {

    if (STROBE) {
        //-----------------------------------------------------------STROBE HIGH
        BYTE command = 0;

        while (STROBE) continue;
        //-----------------------------------------------------------STROBE LOW
        //Read the command from the bus.
        TRISD |= 0xF; // Set the port as input
        command = ReadDataBus();
        while (!STROBE) continue;
        //-----------------------------------------------------------STROBE HIGH
        switch (command) {
            case MSG_PING:
                WriteData(MSG_ACK_PING);
                while (STROBE) continue; //<<---MESSAGE----We should try moving this into WriteData function
                break;
            case MSG_RESET:
                WriteData(MSG_ACK_RESET);
                //Reset Data or something.
                while (STROBE) continue;
                break;
            case MSG_GET:
                GetCMD();
                break;
            case MSG_NOTHING:
                TRUE; // Might need to add a while loop here too to prevent bugs
                break;
            default: // Might need to add a while loop here too to prevent bugs
                return FALSE;
        }
    }
    //-----------------------------------------------------------STROBE LOW
}

//WriteData
//Purpose: Write 4-bits of data to bus.

BYTE WriteData(BYTE data) {
    TRISD &= 0xF0; //Set as an output.

    PortD3 = ((data >> 3) & 1);
    PortD2 = ((data >> 2) & 1);
    PortD1 = ((data >> 1) & 1);
    PortD0 = ((data) & 1);

    return TRUE; //COME UP WITH A TEST FOR SUCCESS HERE
}

BYTE HighNibble(BYTE byte) {

    return (byte >> 4) & 0x0F;
}

BYTE LowNibble(BYTE byte) {

    return byte & 0x0F;
}

//GetCMD
//Purpose: Handles the Get command operationg

BYTE GetCMD() {

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

    //////////////////////////////////ACK////////////////////////////////////

    //-----------------------------------------------------------STROBE HIGH
    // Ack
    WriteData(MSG_ACK_GET);
    while(STROBE) continue;
    //-----------------------------------------------------------STROBE LOW

}

