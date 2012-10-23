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

//Custom Parallel Port Bus Interface.
#define D0 PORTAbits.RA1
#define D1 PORTAbits.RA2
#define D2 PORTAbits.RA3
#define D3 PORTAbits.RA4
#define STROBE PORTEbits.RE0
#define ACKPIN PORTEbits.RE1

//Parallel Port Commands
#define MSG_RESET 0x0
#define MSG_PING 0x1
#define MSG_CMD 0x2
#define MSG_NOTHING 0xF

//Parallel Reply Messages
#define MSG_ACK 0xE

//Structure to define the DateTime in an easily accessible way.
typedef struct{
  unsigned char seconds;
  unsigned char minutes;
  unsigned char hours;
  unsigned char day;
  unsigned char date;
  unsigned char month;
  unsigned char year;
}RTCData;

//Functions
unsigned char ReadCommand();
void SendAck(); //Send Acknowledgment Message
void WriteData(unsigned char Data); //Write 4 bit data on the bus
void PingCMD(); //Handle PING
void GetCMD(unsigned char ADCResult, RTCData * RTCD); //Handle GET
void ResetCMD(); //Handle RESET


#ifdef	__cplusplus
}
#endif

#endif	/* PARALLEL_BSP_H */

