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

BYTE ReadDataBus(){

    BYTE readresult ;

    // Set data bus as input
    TRISA |= 0b00011110;     // PORTA bit 1,2,3,4 -> Input

    //Grab data
    readresult |= 0x00 ;
    readresult |= (D3 << 3);
    readresult |= (D2 << 2);
    readresult |= (D1 << 1);
    readresult |= (D0);

    return readresult;
}

//Read Command
//Purpose: Read 4 bits from the bus to conclude which command is being used.

BYTE ReadCommand(){

    BYTE command;
    BYTE success = FALSE;
  
    // Read Command
    command = ReadDataBus();

    switch ( command ) {
        case MSG_PING:
            success = PingCMD();
            break;
        case MSG_RESET:
            success = ResetCMD();
            break;
        case MSG_GET:
            success = GetCMD();
            break;
        case MSG_NOTHING:
            success = TRUE ;
            break;
        default:
            return FALSE ;

    }
 
    //Wait for falling edge
   while(STROBE) continue ;

    return success;
}
//SendACK
//Purpose: Send general acknowledgment

BYTE SendAck( BYTE typeOfAck){
   return WriteData(typeOfAck);
}

//WriteData
//Purpose: Write 4-bits of data to bus.

BYTE WriteData(BYTE Data){


    // Set data bus as input
    TRISA &= 0b11100001;     // PORTA bit 1,2,3,4 -> Output

    PORTAbits.RA1 = 0;
    PORTAbits.RA2 = 1;
    PORTAbits.RA3 = 1;
    PORTAbits.RA4 = 1;
//    // Meanwhile Linux brings strobe high
//    D3 = (Data >> 3) & 0x01;
//    D2 = (Data >> 2) & 0x01;
//    D1 = (Data >> 1) & 0x01;
//    D0 = (Data) & 0x01;



    //Wait for falling edge
    while(STROBE) continue ;

    return TRUE ; //COME UP WITH A TEST FOR SUCCESS HERE
}

//PingCMD
//Purpose: Handles the Ping command operation

BYTE PingCMD(){
    return SendAck(MSG_PING);
}

//ResetCMD
//Purpose: Handles the Reseet command operation

BYTE ResetCMD(){
   return SendAck(MSG_RESET);
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

}

