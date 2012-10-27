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


////////////////////////////////////////////////////////////////////////////////
//FUNCTION DECLARATIONS
////////////////////////////////////////////////////////////////////////////////
void P18f45k20Init();
void InitPorts();
void InitADC();
void InitI2C();
void ProcessADC(unsigned char compare);
unsigned char ReadADC();


#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

