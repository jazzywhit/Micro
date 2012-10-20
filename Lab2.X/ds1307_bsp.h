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

#define SECONDS 's'
#define MINUTES 'm'
#define HOURS 'h'
#define DAY 'd'
#define DATE 'D'
#define MONTH 'M'
#define YEAR 'Y'

//Structure to define the DateTime in an easily accessible way.
struct DateTime{
  unsigned char seconds;
  unsigned char minutes;
  unsigned char hours;
  unsigned char day;
  unsigned char date;
  unsigned char month;
  unsigned char year;

};

//Functions
void InitDS1307();
unsigned char SetupTimeDS1307();
void SetupTimeDS1307Fail();
unsigned char ReadTimeDS1307(unsigned char *seconds,
                             unsigned char *minutes,
                             unsigned char *hours,
                             unsigned char *day,
                             unsigned char *date,
                             unsigned char *month,
                             unsigned char *year);
//unsigned char WriteByte(unsigned char byte);
void ReadByte(unsigned char *target, char type);
unsigned char GetBCD( unsigned char binary , char type );
unsigned char GetBinary( unsigned char binary , char type );

#ifdef	__cplusplus
}
#endif

#endif	/* DS1307_BSP_H */

