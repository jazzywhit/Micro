////////////////////////////////////////////////////////////////////////////////
/*
    UMass Lowell 16.480/552, Fall 2012

    Purpose : 16.480 Microprocessors II Lab 1
    File: ds1307_bsp.c

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
#include "ds1307_bsp.h"

////////////////////////////////////////////////////////////////////////////////
//void InitDS1307()
/*
        Purpose     : Setup Time in DS 1307
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////
void InitDS1307(){

     StartI2C();
     WriteI2C(0xD0);
     WriteI2C(0x00);
     WriteI2C(0x03); //Write 3 to the

     StartI2C();
     WriteI2C(0xD0);
     WriteI2C(0x07);
     WriteI2C(0x80);
     StopI2C();

 }

////////////////////////////////////////////////////////////////////////////////
//void SetupTimeDS1307()
/*
        Purpose     : Setup Time in DS 1307
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////
void SetupTimeDS1307()
{
    //****************MOVE THIS OUT OF THE FUNCTION ONCE TESTING IS DONE.***************
    unsigned char sec, min, hr, month, year, date, day; //These may be better as global variables for LCD output.
    min = 0b00000011;
    hr  = 0b01100111;   // Set to 12 hour format.
    sec = 0b00000000;   //
    day = 0b00000100;   // Set 1 = Sunday.
    date= 0b00010110;   // Date set to 1st.
    month = 0b00001001;   // Month set to January.
    year= 0b00010010;   // Year set to 2008.

    //Connect and reset stack pointer
    StartI2C();
    WriteI2C(0xD0); //address of DS1307. bit 0 indicates write(0) or read(1)
    WriteI2C(0x00); // Position the address pointer to 0.

    //Write Date Data to the DSE1307
    WriteI2C(sec);
    WriteI2C(min);
    WriteI2C(hr);
    WriteI2C(day);
    WriteI2C(date);
    WriteI2C(month);
    WriteI2C(year);
    WriteI2C(0x80); // Disable Square Wave

    StopI2C();
}

////////////////////////////////////////////////////////////////////////////////
//void ReadTimeDS1307()
/*
        Purpose     : Read Time from DS1307.
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////
void ReadTimeDS1307(unsigned char *seconds, 
        unsigned char *minutes,
        unsigned char *hours,
        unsigned char *day,
        unsigned char *date,
        unsigned char *month,
        unsigned char *year)
{
    //Connect and reset stack pointer
    StartI2C();
    WriteI2C(0xD0); //address of DS1307.
    WriteI2C(0x00); // Position the address pointer to 0.
    StartI2C();
    WriteI2C(0xD1); //address of DS1307.

    //Read Date Data from the DS1307
    *seconds = ReadI2C();
    *minutes = ReadI2C();
    *hours = ReadI2C();
    *day = ReadI2C();
    *date = ReadI2C();
    *month = ReadI2C();
    *year = ReadI2C();

    StopI2C();
}