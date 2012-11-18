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
    
#include "Globals.h"
    
    //Functions
    void CheckParallel(timeStr *dateTime, ADCControl *adcControl);
    void ReadData(unsigned char *readResult);
    void WriteData(unsigned char data); //Write 4 bit data on the bus
    void WriteByte(BYTE byte);
    void GetCommand(timeStr dateTime);
    void SendADC(void);
    void SendTime(timeStr dateTime); //Send the Time to Linux
    void ResetConnection(void);
    void SetInBetween( ADCControl *adcControl);
    void SetOutside( ADCControl *adcControl);
    unsigned short ReadADCBoundValue();
    
#ifdef	__cplusplus
}
#endif

#endif	/* PARALLEL_BSP_H */

