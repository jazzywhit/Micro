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
    readResult = (D3 << 3);
    readResult = (D2 << 2);
    readResult = (D1 << 1);
    readResult = (D0);

    readResult &= 0x0F;

    return readResult;
}

//Read Command
//Purpose: Read 4 bits from the bus to conclude which command is being used.
//When Strobe is low
void ReadCommand(){

    // Set data bus as input
    //TRISD |= 0b00100111;     // PORTc bit 0,1,2,5 -> Input
    //TRISD &= 0b11110000;
    //TRISD &= 0b11110000;     // PORTA bit RC0,RC1,RC2,RC5 -> Output
    //WriteData(MSG_ACK);

    if(STROBE){
        BYTE command = 0;

        while(STROBE) continue;

        //Read the command from the bus.
        command = ReadDataBus();
        //TRISD &= 0b11110000;     // PORTA bit RC0,RC1,RC2,RC5 -> Output

        while(!STROBE) continue;

         switch ( command ) {
            case MSG_PING:
                //TRISD &= 0b11110000;     // PORTA bit RC0,RC1,RC2,RC5 -> Output
                while(STROBE) WriteData(MSG_ACK);
                //TRISD &= 0b11111111;     // PORTA bit RC0,RC1,RC2,RC5 -> Output
                break;
            case MSG_RESET:
                ResetCMD();
                break;
            case MSG_GET:
                GetCMD();
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
BYTE SendAck( BYTE typeOfAck){
   return WriteData(typeOfAck);
}

//WriteData
//Purpose: Write 4-bits of data to bus.
BYTE WriteData(BYTE Data){

    //while(!STROBE);
    // Set data bus as output
    //TRISC = 0x0;
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 1;
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 1;
//    D3 = ((Data >> 3) & 1);
//    D2 = ((Data >> 2) & 1);
//    D1 = ((Data >> 1) & 1);
//    D0 = ((Data) & 1);

    //while(STROBE);
    //Wait for falling edge
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
//
}

