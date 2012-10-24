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

BYTE readResult ;
unsigned char test = 1;

BYTE ReadDataBus(){

    readResult = 0;
    //Grab data
    readResult = (PortD3 << 3);
    readResult = (PortD2 << 2);
    readResult = (PortD1 << 1);
    readResult = (PortD0);

    readResult &= 0x0F;

    return readResult;
}

//Read Command
//Purpose: Read 4 bits from the bus to conclude which command is being used.
//When Strobe is low
void ReadCommand(){

    if(STROBE){
        BYTE command = 0;

        while(STROBE) continue;

        //Read the command from the bus.
        command = ReadDataBus();

        while(!STROBE) continue;

         switch ( command ) {
            case MSG_PING:
                SendAck(MSG_ACK_PING);
                while(STROBE) continue;
                break;
            case MSG_RESET:
                SendAck(MSG_ACK_RESET);
                //Reset Data or something.
                while(STROBE) continue;
                break;
            case MSG_GET:
                SendAck(MSG_ACK_GET);
                //GetCMD();
                while(STROBE) continue;
                break;
            case MSG_NOTHING:
                TRUE ;
                break;
            default:
                return FALSE ;
        }
    }

}

//SendACK
//Purpose: Send general acknowledgment
BYTE SendAck( BYTE msgType){
    //Select statemtn for the type of ACK.
//    D3 = ((typeOfAck >> 3) & 1);
//    D2 = ((typeOfAck >> 2) & 1);
//    D1 = ((typeOfAck >> 1) & 1);
//    D0 = ((typeOfAck) & 1);
    WriteData(msgType);
}

//WriteData
//Purpose: Write 4-bits of data to bus.
BYTE WriteData(BYTE data){

    //while(!STROBE);
    // Set data bus as output
    //TRISC = 0x0;
//    PORTDbits.RD0 = 0;;
//    PORTDbits.RD1 = 1;
//    PORTDbits.RD2 = 1;
//    PORTDbits.RD3 = 1;
    PortD3 = ((data >> 3) & 1);
    PortD2 = ((data >> 2) & 1);
    PortD1 = ((data >> 1) & 1);
    PortD0 = ((data) & 1);

    //while(STROBE);
    //Wait for falling edge
    return TRUE ; //COME UP WITH A TEST FOR SUCCESS HERE
}


BYTE HighNibble(BYTE byte){

    return (byte>>4) & 0x0F ;
}


BYTE LowNibble(BYTE byte){

    return  byte & 0x0F ;
}

//GetCMD
//Purpose: Handles the Get command operationg

BYTE GetCMD(){
//
//    unsigned adcRead = 0;
//
//    adcRead = ReadADC(); //Get the value from the ADC
//    ProcessDigitalResult(&adcRead); //Send the value to turn LED on/off // Might have to be modified for lab 2
////    ReadTimeDS1307(&RTCData.seconds,
//            &RTCData.minutes,
//            &RTCData.hours,
//            &RTCData.day,
//            &RTCData.date,
//            &RTCData.month,
//            &RTCData.year,
//            &RTCData.control); //Get data from RTC (DS1307).
//
//    // 3 ADC nibles-------------------------------------------ADC MUST BE RECONFIGURED TO 12 bit precision
//    // Manipulate unsigned into 3 nibbles
//    WriteData(0xFF);
//    WriteData(0xFF);
//    WriteData(0xFF);
//
//
//    // 8 RTC values
//     WriteData(HighNibble(RTCData.seconds));
//     WriteData(LowNibble(RTCData.seconds));
//
//     WriteData(HighNibble(RTCData.minutes));
//     WriteData(LowNibble(RTCData.minutes));
//
//     WriteData(LowNibble(RTCData.hours));
//     WriteData(LowNibble(RTCData.hours));
//
//     WriteData(LowNibble(RTCData.day));
//     WriteData(LowNibble(RTCData.day));
//
//     WriteData(LowNibble(RTCData.date));
//     WriteData(LowNibble(RTCData.date));
//
//     WriteData(LowNibble(RTCData.month));
//     WriteData(LowNibble(RTCData.month));
//
//     WriteData(LowNibble(RTCData.year));
//     WriteData(LowNibble(RTCData.year));
//
//     WriteData(LowNibble(RTCData.control));
//     WriteData(LowNibble(RTCData.control));
//
}

