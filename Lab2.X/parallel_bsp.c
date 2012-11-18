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

#define WaitForStrobeLow() while (STROBE) continue; //Wait for STROBE LOW
#define WaitForStrobeHigh() while (!STROBE) continue; //Wait for STROBE HIGH

//--------------------------- CheckParallel () ----------------------------------------
//Purpose: Read 4 bits from the bus to conclude which command is being used.
//When Strobe is low

void CheckParallel(timeStr *dateTime, ADCControl *adcControl) {

    if (!STROBE) { // If STROBE is LOW
        unsigned char command = 0;
        ReadData(&command); //Read the command from the BUS

        switch (command) {
            case MSG_PING:
                WriteData(MSG_ACK_PING); //Send ack. (Only task)
                break;
            case MSG_RESET:
                WriteData(MSG_ACK_RESET); //Send ack.
                //ResetConnection(); //Reset the connection.
                break;
            case MSG_GET:
                WriteData(MSG_ACK_GET);
                GetCommand(*dateTime, adcControl); //Run the GET command
                break;
            case MSG_INTBETWEEN:
                WriteData(MSG_ACK_INTBETWEEN);
                SetInBetween(adcControl);
                break;
            case MSG_INTOUTSIDE:
                WriteData(MSG_ACK_INTOUTSIDE);
                SetOutside(adcControl);
                break;
            case MSG_ENABLE:
                WriteData(MSG_ACK_ENABLE);
                SetEnable(adcControl);
                break;
            case MSG_DISABLE:
                WriteData(MSG_ACK_DISABLE);
                SetDisble(adcControl);
                break;
            default:
                WaitForStrobeHigh()
                WaitForStrobeLow()
                break;
        }
    }
}

void SetInBetween(ADCControl *adcControl) {

    unsigned char low;
    unsigned char high;

    // Sets for in between
    adcControl->outside = 0;

    ReadData(&low);
    ReadData(&high);

    adcControl->low = low;
    adcControl->high = high;
}

void SetDisble(ADCControl *adcControl) {

    // Sets for outside
    adcControl->enable = 0;
}

void SetEnable(ADCControl *adcControl) {

    // Sets for outside
    adcControl->enable = 1;
}

void SetOutside(ADCControl *adcControl) {

    unsigned char low;
    unsigned char high;

    // Sets for in between
    adcControl->outside = 1;

    ReadData(&low);
    ReadData(&high);

    adcControl->low = low;
    adcControl->high = high;
}

/*--------------------------- ResetConnection () ------------------------------------------------------
 Purpose     : Reset the connection and all necessary variables.
 Parameters  : N/A
 Output      : N/A
 */
void ResetConnection(void) {
    ///ADRESH = 0;
}

/*--------------------------- ReadData () ------------------------------------------------------
 Purpose     : Read data from the parallel port.
 Parameters  : N/A
 Output      : N/A
 */
void ReadData(unsigned char *readResult) {
    WaitForStrobeHigh() //Wait for STROBE HIGH

    TRISD |= 0x0F; // Set the port as input

    //Grab data
    *readResult |= (PortD3 << 3);
    *readResult |= (PortD2 << 2);
    *readResult |= (PortD1 << 1);
    *readResult |= (PortD0);

    *readResult &= 0x0F;

    WaitForStrobeLow() //Wait for STROBE LOW
    WaitForStrobeHigh() //Wait for STROBE HIGH

}

/*--------------------------- WriteData () ------------------------------------------------------
 Purpose     : Write data to the parallel port.
 Parameters  : N/A
 Output      : N/A
 */
void WriteData(unsigned char data) {
    WaitForStrobeLow() //Wait for STROBE LOW

    TRISD &= 0xF0; //Set as an output.

    PortD3 = ((data >> 3) & 1);
    PortD2 = ((data >> 2) & 1);
    PortD1 = ((data >> 1) & 1);
    PortD0 = ((data) & 1);

    WaitForStrobeHigh() //Linux Reading
    WaitForStrobeLow() //Linux Done Reading

    PortD3 = 0;
    PortD2 = 0;
    PortD1 = 0;
    PortD0 = 0;

    WaitForStrobeHigh()
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
void GetCommand(timeStr dateTime, ADCControl *adcControl) {
    SendADC(adcControl); //Start with the ADC first.
    SendTime(dateTime); //Finish with the time.
}

/*--------------------------- SendADC () ------------------------------------------------------
 Purpose     : Write data to the parallel port.
 Parameters  : N/A
 Output      : N/A
 */
void SendADC(ADCControl *adcControl) {
    WriteData(adcControl->adcData.write.hbits);
    WriteData(adcControl->adcData.write.mbits);
    WriteData(adcControl->adcData.write.lbits);
}

void WriteByte(BYTE byte) {

    WriteData(HighNibble(byte));
    WriteData(LowNibble(byte));
}

/*--------------------------- SendTime () ------------------------------------------------------
 Purpose     : Send the time data over the parallel port.
 Parameters  : N/A
 Output      : N/A
 */
void SendTime(timeStr dateTime) {

    WriteByte(dateTime.seconds);
    WriteByte(dateTime.minutes);
    WriteByte(dateTime.hours);
    WriteByte(dateTime.day);
    WriteByte(dateTime.date);
    WriteByte(dateTime.month);
    WriteByte(dateTime.year);
    WriteByte(dateTime.control);
    WriteData(MSG_ACK_GET); //Send a final ack to make sure it knows we are finished.
    
}

