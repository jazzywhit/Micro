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


//Read Command
//Purpose: Read 4 bits from the bus to conclude which command is being used.

unsigned char ReadCommand(){

    unsigned char readresult;
    
    //See STROBE go H-->L-->H.
    while(STROBE);
    while(!STROBE);
    
    //Grab Data -- Not sure if this is correct.
    readresult |= 0x00 ;
    readresult |= (D3 << 3);
    readresult |= (D2 << 2);
    readresult |= (D1 << 1);
    readresult |= (D0);

    //See STROBE go L.
    while(STROBE);

    return readresult;
}

//SendACK
//Purpose: Send general acknowledgment

void SendAck(){
    WriteData(MSG_ACK);
}

//WriteData
//Purpose: Write 4-bits of data to bus.

void WriteData(unsigned char Data){
    
    //See STROBE go H-->L-->H.
    while(STROBE);
    while(!STROBE);
    
    D3 = (Data >> 3) & 0x01;
    D2 = (Data >> 2) & 0x01;
    D1 = (Data >> 1) & 0x01;
    D0 = (Data) & 0x01;
    
    //See STROBE go L.
    while(STROBE);
}

//PingCMD
//Purpose: Handles the Ping command operation

void PingCMD(){
    SendAck();
}

//GetCMD
//Purpose: Handles the Get command operationg

void GetCMD(unsigned char ADCResult, RTCData * RTCD){

}


void ResetCMD(){
    P18f45k20Init();
    SendAck();
}