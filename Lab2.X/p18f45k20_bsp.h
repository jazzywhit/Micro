////////////////////////////////////////////////////////////////////////////////
/*
    UMass Lowell 16.480/552, Fall 2012

    Purpose : 16.480 Microprocessors II Lab 1
    File: p18f45k20_bsp.h

    Group Name: Shoulda Been Gangstas
    Group Members:
        - Jesse Whitworth
        - Matthew Cook
        - Denis Lemos
        - Aadil Hassan

    License: GNU GPLv3.
    
    Date Created: 9-20-2012
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef P18F45K20_BSP_H
#define	P18F45K20_BSP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <p18f45k20.h>
#include <i2c.h>

////////////////////////////////////////////////////////////////////////////////
//DEFINES
////////////////////////////////////////////////////////////////////////////////
#define DS1307
#define PPBit0 RA2  // PP Nibble bits
#define PPTRIS TRISA2=TRISC2=TRISC1=TRISC0 // PP Nibble tristate; allows setting all PP nibble bits as input or output simultaneously
#define ADC_COMPARE_VALUE 100
    
    
/* user commands -- Likely issued from the Atom */
#define MSG_RESET 0x0 /* reset the sensor to initial state */
#define MSG_PING 0x1 /* check if the sensor is working */


//Defined by the lab handout.
#define MSG_ACK 0xE /* Sensor Device Responses */
#define MSG_NOTHING 0xF /* acknowledgement to the commands */
/* reserved */


////////////////////////////////////////////////////////////////////////////////
//FUNCTION DECLARATIONS
////////////////////////////////////////////////////////////////////////////////
void P18f45k20Init();
void InitPorts();
void InitADC();
void InitI2C();
void ProcessDigitalResult(unsigned char *compare);
void ParallelStatus();
void ParallelRead();
void ParallelWrite();
unsigned char ReadADC();


#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

