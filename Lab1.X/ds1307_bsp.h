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

void InitDS1307();
unsigned char SetupTimeDS1307();
unsigned char ReadTimeDS1307(unsigned char *seconds,
                             unsigned char *minutes,
                             unsigned char *hours,
                             unsigned char *day,
                             unsigned char *date,
                             unsigned char *month,
                             unsigned char *year);

#ifdef	__cplusplus
}
#endif

#endif	/* DS1307_BSP_H */

