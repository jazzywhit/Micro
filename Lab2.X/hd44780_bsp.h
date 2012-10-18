////////////////////////////////////////////////////////////////////////////////
/*
    UMass Lowell 16.480/552, Fall 2012

    Purpose : 16.480 Microprocessors II Lab 1
    File: hd44780_bsp.h

    Group Name: Shoulda Been Gangstas
    Group Members:
        - Jesse Whitworth
        - Matthew Cook
        - Denis Lemos
        - Aadil Hassan

    License: GNU GPLv3.

    Date Created: 9-30-2012
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef HD44780_BSP_H
#define	HD44780_BSP_H

#ifdef	__cplusplus
extern "C" {
#endif

//#include <p18F45K20.h>
#include <xlcd.h>
#include <delays.h>

void TestDisplay();
void DelayFor18TCY(void);                   //Delay of 2ms
void DelayPORXLCD(void);  //Delay of 15ms //Cycles = (TimeDelay * Fosc) / 4
void DelayXLCD(void);    //Delay of 5ms


#ifdef	__cplusplus
}
#endif

#endif	/* HD44780_BSP_H */

