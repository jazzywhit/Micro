////////////////////////////////////////////////////////////////////////////////
/*
 UMass Lowell 16.480/552, Fall 2012
 
 Purpose : 16.480 Microprocessors II Lab 1
 File: parallel_bsp.h
 
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

#ifndef PARALLEL_BSP_H
#define	PARALLEL_BSP_H

#ifdef	__cplusplus
extern "C" {
#endif
    
// Define data type
typedef unsigned char BYTE ;
    
//Custom Parallel Port Bus Interface.
#define D0 PORTDbits.RD0
#define D1 PORTDbits.RD1
#define D2 PORTDbits.RD2
#define D3 PORTDbits.RD3
#define STROBE PORTDbits.RD5
//#define ACKPIN PORTCbits.RD7
    
#define TRUE 1
#define FALSE 0
    
    //Parallel Port Commands
#define MSG_RESET 0x00
#define MSG_PING 0x01
#define MSG_GET 0x02
#define MSG_NOTHING 0x0F

    //Parallel Reply Messages
#define MSG_ACK 0x0E
    
    //Structure to define the DateTime in an easily accessible way.
    typedef struct{
        BYTE seconds;
        BYTE minutes;
        BYTE hours;
        BYTE day;
        BYTE date;
        BYTE month;
        BYTE year;
        BYTE control;
    }RTCData;
    
    //Functions
    void ReadCommand();
    BYTE SendAck( BYTE tyepOfAck ); //Send Acknowledgment Message
    BYTE WriteData(BYTE Data); //Write 4 bit data on the bus
    BYTE PingCMD(); //Handle PING
    BYTE GetCMD();//Handle GET
    BYTE ResetCMD(); //Handle RESET


    
#ifdef	__cplusplus
}
#endif

#endif	/* PARALLEL_BSP_H */

