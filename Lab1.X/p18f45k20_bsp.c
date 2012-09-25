////////////////////////////////////////////////////////////////////////////////
/*
    UMass Lowell 16.480/552, Fall 2012

    Purpose : 16.480 Microprocessors II Lab 1
    File: p18f45k20_bsp.c

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

#include "p18f45k20_bsp.h"


////////////////////////////////////////////////////////////////////////////////
//void Init()
/*
        Purpose     : Initiate PIC Components
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////
void P18f45k20Init()
{
    InitPorts();
    InitADC();
    InitI2C();
    InitDS1307();
    //Setup_Time_DS1307();
}

////////////////////////////////////////////////////////////////////////////////
//void InitI2C()
/*
        Purpose     : Initialize any registers that the I2C needs for communication.
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////
void InitI2C()
{
    //Functions we can use
    SSPADD = 0x09;
    OSCCONbits.IRCF = 0b101;
    OpenI2C(MASTER, SLEW_OFF);

}

////////////////////////////////////////////////////////////////////////////////
//void Read_ADC()
/*
        Purpose     : Reads the value from ADC
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////
unsigned char ReadADC()
{
    ADCON0bits.GO_DONE = 1; //Start Conversion.
    while(ADCON0bits.GO_DONE) //While conversion is not completed. Loop.
    {
     Nop();  //Implement a delay structure.
    }
    return ADRESH;
}

////////////////////////////////////////////////////////////////////////////////
//void Process_Digital_Result()
/*
        Purpose     : Handle result of ADC conversion.
        Parameters  : N/A
        Output      : N/A
 *      Info        : The ADC_COMPARE_VALUE value will change based on the room conditions.
*/
////////////////////////////////////////////////////////////////////////////////
void ProcessDigitalResult(unsigned char *compare)
{
    if(*compare < ADC_COMPARE_VALUE) //If it is dark the voltage will be around (.3-.4)V
        PORTDbits.RD7 = 1; // LED Will turn Off/On depending on comparision.
    else
	PORTDbits.RD7 = 0;
}

////////////////////////////////////////////////////////////////////////////////
//void InitADC()
/*
        Purpose     : Initialize any registers that the ADC needs for communication.
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////
void InitADC()
{
    //1. Configure the RA0/AN0 pin as an analog input in ANSEL.
            ANSELbits.ANS0 = 1;     // Disable Digital Input Buffer for Analog select control bit. (Data Sheet PAGE )

    //2. Set the ADC voltage references in ADCON1.
            ADCON1bits.VCFG0 = 0;  //Positive Voltage Reference select bit supplied by VDD. (Data Sheet PAGE 272)
            ADCON1bits.VCFG1 = 0;  //Negative Voltage Reference select bit supplied by VSS. (Data Sheet PAGE 272)

    //3. Set the result justification, ADC clock source, and acquisition time in ADCON2.
            ADCON2 = 0b00111000; // Left Justified (7), Not Used (6), Aquisition Time 20 TAD (3-5),Conversion Clock Time FOSC/2 (0-2) (Data Sheet PAGE 273)

    //4. Select the channel and turn on the ADC in ADCON0.
            ADCON0 = 0b00000001; // Not Used(7-6), Analog Channel select AN0 (5-2), A/D Conversion Status Bit (1), ADC Enable (0) (Data Sheet PAGE 271)
}

////////////////////////////////////////////////////////////////////////////////
//void InitPorts()
/*
        Purpose     : Initialize PIC PORTS for I/O Pins
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////
void InitPorts()
{
    //Setup Port A to communicate with the ADC
    // ANO (PIN 2) Input pin for ADC conversion.
    TRISA = 0b11111111;     // PORTA bit 0 set 0 as output so AN0 can be the analog input voltage from photoresitor

    TRISC = 0x00; //turn on tri-state register and
    //make all output pins
    PORTC = 0x00; //make all output pins LOW

    //Setup PortD to attach to the LED
    TRISD = 0b01111111;// PORTD bit 7 to output (0); bits 6:0 are inputs (1) (a.k.a. RD7 or pin 30)

}