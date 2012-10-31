////////////////////////////////////////////////////////////////////////////////
/*
 UMass Lowell 16.480/552, Fall 2012
 
 Purpose : 16.480 Microprocessors II Lab 1
 File: ds1307_bsp.h
 
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

#ifndef DS1307_BSP_H
#define	DS1307_BSP_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <i2c.h>
#include "Globals.h"   
    
void SetupTimeDS1307Fail(void);
unsigned char Write(unsigned char byte);
void Read(unsigned char *target, char type);
unsigned char GetBCD( unsigned char binary , char type );
unsigned char GetBinary( unsigned char binary , char type );

void SetupTimeDS1307(timeStr *dateTime);
void ReadTimeDS1307(timeStr *dateTime);

#ifdef	__cplusplus
}
#endif

#endif	/* DS1307_BSP_H */

