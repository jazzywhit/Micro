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
#define	DS1307_I2C_ADDRESS 0xd0

////////////////////////////////////////////////////////////////////////////////
//void InitDS1307()
/*
        Purpose     : Setup Time in DS 1307
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////
//void InitDS1307(){
//
//     StartI2C();
//     WriteI2C(0xD0);
//     WriteI2C(0x00);
//     WriteI2C(0x03); //Write 3 to the
//
//     StartI2C();
//     WriteI2C(0xD0);
//     WriteI2C(0x07);
//     WriteI2C(0x80);
//     StopI2C();
//
// }



////////////////////////////////////////////////////////////////////////////////
//void SetupTimeDS1307()
/*
        Purpose     : Setup Time in DS 1307
                    : The fields are converted from binary to BCD before being sent to the RTC
                    : All fields are 0 - #, except day, date and month, which are 1 - #
                    : Pointers are used for comanilty with rtc_get_time
                    : Uses 24 hour clock, not 12 hour
        Parameters  : N/A
        Output      : 1 if sucessful, 0 if not.
*/
////////////////////////////////////////////////////////////////////////////////
unsigned char SetupTimeDS1307(unsigned char *seconds,
                              unsigned char *minutes,
                              unsigned char *hours,
                              unsigned char *day,
                              unsigned char *date,
                              unsigned char *month,
                              unsigned char *year) {

    unsigned char temp;
    unsigned char temp1;

    //Send the start condition
    IdleI2C();
    StartI2C();
    while (SSPCON2bits.SEN) continue; //Bit indicating start is still in progress

    //Send the address with the write bit set
    IdleI2C();
    WriteI2C(DS1307_I2C_ADDRESS & 0xFE); //Bit 0 low for write
    while (SSPSTATbits.R_W){
        continue;
    }
    if (SSPCON2bits.ACKSTAT)
        SetupTimeDS1307Fail();         return(0);

    //Send the register address
    IdleI2C();
    WriteI2C(0x00);
    while (SSPSTATbits.R_W) {
        continue;
    }
    if (SSPCON2bits.ACKSTAT)
        SetupTimeDS1307Fail();         return(0);

    //Write seconds
    if (*seconds > 59) //Ensure value is in range
        SetupTimeDS1307Fail();         return(0);
    temp1 = (*seconds / 10);
    temp = (*seconds - (temp1 * 10)) + (temp1 << 4);
    temp &= 0x7f; //Bit7 = enable oscillator
    IdleI2C();
    WriteI2C(temp);
    while (SSPSTATbits.R_W){
        continue;
    }
    if (SSPCON2bits.ACKSTAT)
        SetupTimeDS1307Fail();         return(0);

    //Write minutes
    if (*minutes > 59) //Ensure value is in range
        SetupTimeDS1307Fail();         return(0);

    temp1 = (*minutes / 10);
    temp = (*minutes - (temp1 * 10)) + (temp1 << 4);
    IdleI2C();
    WriteI2C(temp);
    while (SSPSTATbits.R_W){
        continue;
    }
    if (SSPCON2bits.ACKSTAT)
        SetupTimeDS1307Fail();         return(0);

    //Write hours
    if (*hours > 23) //Ensure value is in range
        SetupTimeDS1307Fail();         return(0);

    temp1 = (*hours / 10);
    temp = (*hours - (temp1 * 10)) + (temp1 << 4);
    temp &= 0x3F; //Bit6 low = set format to 24 hour
    IdleI2C();
    WriteI2C(temp);
    while (SSPSTATbits.R_W){
        continue;
    }

    if (SSPCON2bits.ACKSTAT)
        SetupTimeDS1307Fail();         return(0);

    //Write day
    if (*day > 7) //Ensure value is in range
        SetupTimeDS1307Fail();         return(0);
    if (*day == 0)
        SetupTimeDS1307Fail();         return(0);

    temp1 = (*day / 10);
    temp = (*day - (temp1 * 10)) + (temp1 << 4);
    IdleI2C();
    WriteI2C(temp);
    while (SSPSTATbits.R_W){
        continue;
    }

    if (SSPCON2bits.ACKSTAT)
        SetupTimeDS1307Fail();         return(0);

    //Write date
    if (*date > 31) //Ensure value is in range
        SetupTimeDS1307Fail();         return(0);

    temp1 = (*date / 10);
    temp = (*date - (temp1 * 10)) + (temp1 << 4);
    IdleI2C();
    WriteI2C(temp);
    while (SSPSTATbits.R_W){
        continue;
    }

    if (SSPCON2bits.ACKSTAT)
        SetupTimeDS1307Fail();         return(0);

    //Write month
    if (*month > 12) //Ensure value is in range
        SetupTimeDS1307Fail();         return(0);

    temp1 = (*month / 10);
    temp = (*month - (temp1 * 10)) + (temp1 << 4);
    IdleI2C();
    WriteI2C(temp);
    while (SSPSTATbits.R_W){
        continue;
    }

    if (SSPCON2bits.ACKSTAT)
        SetupTimeDS1307Fail();         return(0);

    //Write year
    if (*year > 99) //Ensure value is in range
        SetupTimeDS1307Fail();         return(0);

    temp1 = (*year / 10);
    temp = (*year - (temp1 * 10)) + (temp1 << 4);
    IdleI2C();
    WriteI2C(temp);
    while (SSPSTATbits.R_W){
        continue;
    }

    if (SSPCON2bits.ACKSTAT)
        SetupTimeDS1307Fail();         return(0);

    //Write control
    IdleI2C();
    WriteI2C(0x00); //0x00 = square wave output off
    while (SSPSTATbits.R_W){
        continue;
    }

    if (SSPCON2bits.ACKSTAT){
        SetupTimeDS1307Fail();          return(0);
    }

    //Send Stop
    IdleI2C();
    StopI2C();
    while (SSPCON2bits.PEN){
        continue;
    }

    return (1);
}
    //----- I2C WRITE FAILED -----
void SetupTimeDS1307Fail(){

    //Send Stop
    IdleI2C();
    StopI2C();
    while (SSPCON2bits.PEN){
        continue;
    }
}

////////////////////////////////////////////////////////////////////////////////
//void ReadTimeDS1307()
/*
        Purpose     : Read Time from DS1307.
                    : All fields are converted to binary values.
                    : All fields are 0 - #, except day, date and month, which are 1 - #
                    : Uses 24 hour clock, not 12 hour.
        Parameters  : N/A
        Output      : 1 if sucessful, 0 if not.

*/
////////////////////////////////////////////////////////////////////////////////
unsigned char ReadTimeDS1307(unsigned char *seconds,
                             unsigned char *minutes,
                             unsigned char *hours,
                             unsigned char *day,
                             unsigned char *date,
                             unsigned char *month,
                             unsigned char *year) {

    unsigned char temp;


    //Send the start condition
    IdleI2C();
    StartI2C();
    while (SSPCON2bits.SEN){
        continue; //Bit indicating start is still in progress
    }

    //Send the address with the write bit set
    IdleI2C();
    WriteI2C(DS1307_I2C_ADDRESS & 0xFE); //Bit 0 low for write
    while (SSPSTATbits.R_W){
        continue; //Bit indicating transmit unsigned char is still in
    }

    if (SSPCON2bits.ACKSTAT) //Bit that is high when ACK was not received
        goto ReadTimeDS1307_FAIL;

    //Send the register address
    IdleI2C();
    WriteI2C(0x00);
    while (SSPSTATbits.R_W){
        continue;
    }

    if (SSPCON2bits.ACKSTAT)
        goto ReadTimeDS1307_FAIL;

    //Send restart condition
    IdleI2C();
    RestartI2C();
    while (SSPCON2bits.RSEN){
        continue; //Bit indicating re-start is still in
    }

    //Send the address with the read bit set
    IdleI2C();
    WriteI2C(DS1307_I2C_ADDRESS | 0x01); //Bit 1 high for read
    while (SSPSTATbits.R_W){
        continue;
    }

    if (SSPCON2bits.ACKSTAT)
        goto ReadTimeDS1307_FAIL;

    //Read seconds
    IdleI2C();
    temp = ReadI2C();
    *seconds = (temp & 0x0F) + (((temp & 0x70) >> 4) * 10); //(Bit 7 is osc flag bit - dump)
    AckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN){
        continue;
    }

    //Read minutes
    IdleI2C();
    temp = ReadI2C();
    *minutes = (temp & 0x0f) + (((temp & 0x70) >> 4) * 10);
    AckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN){
        continue;
    }

    //Read hours
    IdleI2C();
    temp = ReadI2C();
    *hours = (temp & 0x0f) + (((temp & 0x30) >> 4) * 10);
    AckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN){
        continue;
    }

    //Read day
    IdleI2C();
    temp = ReadI2C();
    *day = (temp & 0x07);
    AckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN){
        continue;
    }

    //Read date
    IdleI2C();
    temp = ReadI2C();
    *date = (temp & 0x0f) + (((temp & 0x30) >> 4) * 10);
    AckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN){
        continue;
    }

    //Read month
    IdleI2C();
    //  There was something here and on every other read that I have deleted but I think we don't need. I can get it back if necessary
    temp = ReadI2C();
    *month = (temp & 0x0f) + (((temp & 0x10) >> 4) * 10);
    AckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN){
        continue;
    }

    //Read year
    IdleI2C();
    temp = ReadI2C();
    *year = (temp & 0x0f) + ((temp >> 4) * 10);
    //AckI2C();		    //Send Ack           // Progression if read was to continue like
    //while (SSPCON2bits.ACKEN)                  // in steps above, but we have read all
    //	;                                        // the data we needed so skip these.

    //Send NAK
    IdleI2C();                                   // Reading is done, send NACK and stop
    NotAckI2C();
    while (SSPCON2bits.ACKEN){
        continue;
    }

    //Send Stop
    IdleI2C();
    StopI2C();
    //Bit indicating Stop is still in progress
    while (SSPCON2bits.PEN){
      
        continue;
    }

    return (1);

    //----- I2C READ FAILED -----
ReadTimeDS1307_FAIL:

    *seconds = 0;
    *minutes = 0;
    *hours = 0;
    *day = 0;
    *date = 1;
    *month = 1;
    *year = 0;

    //Send Stop
    IdleI2C();
    StopI2C();
    while (SSPCON2bits.PEN){
        continue;
    }

    return (0);

}