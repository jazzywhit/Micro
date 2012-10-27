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
    typedef unsigned char BYTE;

    //Custom Parallel Port Bus Interface.
#define PortD0 PORTDbits.RD0
#define PortD1 PORTDbits.RD1
#define PortD2 PORTDbits.RD2
#define PortD3 PORTDbits.RD3
#define STROBE PORTDbits.RD5
//#define ACKPIN PORTCbits.RD7

#define TRUE 1
#define FALSE 0

    //Parallel Port Commands
#define MSG_RESET 0x03
#define MSG_ACK_RESET 0x0D
#define MSG_PING 0x01
#define MSG_ACK_PING 0x0E
#define MSG_GET 0x02
#define MSG_ACK_GET 0x0C
#define MSG_NOTHING 0x0F

//Parallel Reply Messages
#define MSG_ACK 0x0E

//Structure to define the DateTime in an easily accessible way.

typedef struct {
    BYTE seconds;
    BYTE minutes;
    BYTE hours;
    BYTE day;
    BYTE date;
    BYTE month;
    BYTE year;
    BYTE control;
} RTCData;
 
//Functions
void CheckParallel();
BYTE ReadData();
void WriteData(BYTE Data); //Write 4 bit data on the bus
void GetCommand();
void SendADC();
void SendTime(); //Send the Time to Linux
void ResetConnection();

#ifdef	__cplusplus
}
#endif

#endif	/* PARALLEL_BSP_H */

