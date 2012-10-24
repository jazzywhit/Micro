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
#define D0 PORTAbits.RA1
#define D1 PORTAbits.RA2
#define D2 PORTAbits.RA3
#define D3 PORTAbits.RA4
#define STROBE PORTEbits.RE0
#define ACKPIN PORTEbits.RE1
    
#define TRUE 1
#define FALSE 0
    
    //Parallel Port Commands
#define MSG_RESET 0x0
#define MSG_PING 0x1
#define MSG_GET 0x2
#define MSG_NOTHING 0xF
    
    //Parallel Reply Messages
#define MSG_ACK 0xE
    
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
    BYTE ReadCommand();
    BYTE SendAck( BYTE tyepOfAck ); //Send Acknowledgment Message
    BYTE WriteData(BYTE Data); //Write 4 bit data on the bus
    BYTE PingCMD(); //Handle PING
    BYTE GetCMD();//Handle GET
    BYTE ResetCMD(); //Handle RESET
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* PARALLEL_BSP_H */

