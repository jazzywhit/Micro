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
//void WriteByte()
/*
 Purpose     : Writes a Byte to the RTC
 Parameters  : The Byte that needs to be written
 Output      : 1 if sucessful, 0 if not.
 */
////////////////////////////////////////////////////////////////////////////////
unsigned char WriteByte(unsigned char byte){
    //Send the address with the write bit set
    IdleI2C();
    WriteI2C(byte); //Bit 0 low for write
    while (SSPSTATbits.R_W){
        continue;
    }
    if (SSPCON2bits.ACKSTAT){
        SetupTimeDS1307Fail();
        return(0);
    }
    return(1);
}

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
unsigned char SetupTimeDS1307(RTCTime * readRTCTime) {
    
    //Send the start condition
    IdleI2C();
    StartI2C();
    
    //Really not convinced that this is necessary...
    while (SSPCON2bits.SEN)                     continue; //Bit indicating start is still in progress
    
    if(!WriteByte(DS1307_I2C_ADDRESS & 0xFE))   return (0);
    if(!WriteByte(0x00))                        return (0);
    
    
    //Check if the seconds are within range. If they are not, send the FAIL signal.
    //Not entirely sure this is necessary. Could just return (0).
    if (readRTCTime->seconds > 59) //Ensure value is in range
        SetupTimeDS1307Fail();                  return(0);
    
    //Write the seconds
    if(!WriteByte(GetBCD(readRTCTime->seconds , SECONDS )))                        return (0);
    
    
    
    //Check if the minutes are within range. If they are not, send the FAIL signal.
    //Same as seconds.. check if this is necessary.
    if (readRTCTime->minutes > 59) //Ensure value is in range
        SetupTimeDS1307Fail();                  return(0);
    //Write the minutes
    if(!WriteByte(GetBCD(readRTCTime->minutes, MINUTES)))                        return (0);
    
    
    
    //Check if the hours are within range. If they are not, send the FAIL signal.
    //Same as seconds.. check if this is necessary.
    if (readRTCTime->hours > 23) //Ensure value is in range
        SetupTimeDS1307Fail();                  return(0);
    
    //Write the Hours
    if(!WriteByte(GetBCD(readRTCTime->hours , HOURS )))                        return (0);
    
    
    
    //Write day
    if (readRTCTime->day > 7) //Ensure value is in range
        SetupTimeDS1307Fail();                  return(0);
    if (readRTCTime->day == 0)
        SetupTimeDS1307Fail();                  return(0);
    
    if(!WriteByte( GetBCD(readRTCTime->day , DAY )))                        return (0);
    
    
    
    //Write date
    if (readRTCTime->date > 31) //Ensure value is in range
        SetupTimeDS1307Fail();                  return(0);
    
    if(!WriteByte( GetBCD(readRTCTime->date, DATE )))                        return (0);
    
    
    
    //Write month
    if (readRTCTime->month > 12) //Ensure value is in range
        SetupTimeDS1307Fail();                  return(0);
    
    if(!WriteByte( GetBCD(readRTCTime->month, MONTH )))                        return (0);
    
    
    
    //Write year
    if (readRTCTime->year > 99) //Ensure value is in range
        SetupTimeDS1307Fail();                  return(0);
    
    if(!WriteByte( GetBCD(readRTCTime->year , YEAR )))                        return (0);
    
    
    //Write control
    //I don't recall this from the datagram. Check this against the datasheet.
    if(!WriteByte(0x00))                        return (0);
    
    //Finish up
    IdleI2C();
    StopI2C();
    while (SSPCON2bits.PEN)                     continue;
    
    return (1);
}
//----- I2C WRITE FAILED -----
void SetupTimeDS1307Fail(void){
    
    //Send Stop
    IdleI2C();
    StopI2C();
    while (SSPCON2bits.PEN){
        continue;
    }
}

void ReadByte(unsigned char *target, char type){
    IdleI2C();
    *target = GetBinary(ReadI2C(), type) ;
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
unsigned char ReadTimeDS1307(RTCTime * readRTCTime) {
    
    //Send the start condition
    IdleI2C();
    StartI2C();
    while (SSPCON2bits.SEN){
        continue; //Bit indicating start is still in progress
    }
    
    WriteByte(DS1307_I2C_ADDRESS & 0xFE);
    WriteByte(0x00);
    
    //Send restart condition
    IdleI2C();
    RestartI2C();
    while (SSPCON2bits.RSEN){
        continue; //Bit indicating re-start is still in
    }
    
    WriteByte(DS1307_I2C_ADDRESS | 0x01);
    
    if (SSPCON2bits.ACKSTAT){
        SetupTimeDS1307Fail();
        return 0;
    }
    
    //Read seconds
    ReadByte(readRTCTime->seconds, SECONDS);
    AckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN)                   continue;
    ReadByte(readRTCTime->minutes, MINUTES);
    AckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN)                   continue;
    ReadByte(readRTCTime->hours, HOURS);
    AckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN)                   continue;
    ReadByte(readRTCTime->day, DAY);
    AckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN)                   continue;
    ReadByte(readRTCTime->date, DATE);
    AckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN)                   continue;
    ReadByte(readRTCTime->month, MONTH);
    AckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN)                   continue;
    ReadByte(readRTCTime->year, YEAR);
    NotAckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN)                   continue;
    ReadByte(readRTCTime->year, CONTROL);
    NotAckI2C(); //Send Ack
    while (SSPCON2bits.ACKEN)                   continue;
    
    //Send Stop
    IdleI2C();
    StopI2C();
    //Bit indicating Stop is still in progress
    while (SSPCON2bits.PEN)                         continue;
    
    return (1);
}


unsigned char GetBinary(unsigned char bcd, char type) {
    
    unsigned char binary;
    
    switch (type) {
        case SECONDS:
            binary = (bcd & 0x0F) + (((bcd & 0x70) >> 4) * 10); //(Bit 7 is osc flag bit - dump)
            break;
            
        case MINUTES:
            binary = (bcd & 0x0f) + (((bcd & 0x70) >> 4) * 10);
            break;
            
        case HOURS:
            binary = (bcd & 0x0f) + (((bcd & 0x30) >> 4) * 10);
            break;
            
        case DAY:
            binary = bcd & 0x07;
            break;
            
        case DATE:
            binary = (bcd & 0x0f) + (((bcd & 0x30) >> 4) * 10);
            break;
            
        case MONTH:
            binary = (bcd & 0x0f) + (((bcd & 0x10) >> 4) * 10);
            break;
            
        case YEAR:
            binary = (bcd & 0x0f) + ((bcd >> 4) * 10);
            break;

        case CONTROL:
            binary = bcd ;
            break;
            
        default:
            return 0;
            break;
    }
    
    return binary;
    
}


unsigned char GetBCD(unsigned char binary, char type) {
    
    unsigned char bcd;
    unsigned char byte;
    
    switch (type) {
        case SECONDS:
            //Calculate the seconds.
            byte = (binary / 10);
            bcd = (binary - (byte * 10)) + (byte << 4);
            bcd &= 0x7f; //Bit7 = enable oscillator
            break;
            
        case MINUTES:
            //Calculate the minutes
            byte = (binary / 10);
            bcd = (binary - (byte * 10)) + (byte << 4);
            break;
            
        case HOURS:
            byte = (binary / 10);
            bcd = (binary - (byte * 10)) + (byte << 4);
            bcd &= 0x3F; //Bit6 low = set format to 24 hour
            break;
            
        case DAY:
            byte = (binary / 10);
            bcd = (binary - (byte * 10)) + (byte << 4);
            break;
            
        case DATE:
            byte = (binary / 10);
            bcd = (binary - (byte * 10)) + (byte << 4);
            break;
            
        case MONTH:
            byte = (binary / 10);
            bcd = (binary - (byte * 10)) + (byte << 4);
            break;
            
        case YEAR:
            byte = (binary / 10);
            bcd = (binary - (byte * 10)) + (byte << 4);
            break;
            
        default:
            return 0;
            break;
    }
    
    return bcd;
    
}