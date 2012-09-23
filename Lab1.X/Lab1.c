////////////////////////////////////////////////////////////////////////////////
/*
    UMass Lowell 16.480/552, Fall 2012

    Purpose : 16.480 Microprocessors II Lab 1
    File: Lab1.c

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


////////////////////////////////////////////////////////////////////////////////
//INCLUDES
////////////////////////////////////////////////////////////////////////////////

#include "p18f45K20.h"
#include "i2c.h"


////////////////////////////////////////////////////////////////////////////////
//PRAGMAS
////////////////////////////////////////////////////////////////////////////////

#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF, LVP = OFF

////////////////////////////////////////////////////////////////////////////////
//DEFINES
////////////////////////////////////////////////////////////////////////////////

#define PPBit0 RA2  // PP Nibble bits
#define PPTRIS TRISA2=TRISC2=TRISC1=TRISC0 // PP Nibble tristate; allows setting all PP nibble bits as input or output simultaneously

////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////

unsigned int Digital_Result=0;

////////////////////////////////////////////////////////////////////////////////
//STRUCTURES
////////////////////////////////////////////////////////////////////////////////

struct bounds
{
	unsigned int low, high;
};

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DECLARATIONS
////////////////////////////////////////////////////////////////////////////////

void Init();
void InitPorts();
void InitADC();
void InitI2C();
void Start_Conversion();
void Process_Digital_Result();

////////////////////////////////////////////////////////////////////////////////
//void Init()
/*
        Purpose     : Initiate PIC Components
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////

void Init()
{
    InitPorts();
    InitADC();
    InitI2C();
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

    //Setup PortD to attach to the LED
    TRISD = 0b01111111;// PORTD bit 7 to output (0); bits 6:0 are inputs (1) (a.k.a. RD7 or pin 30)
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
            ADCON1bits.VCFG0 = 0 ;  //Positive Voltage Reference select bit supplied by VDD. (Data Sheet PAGE 272)
            ADCON1bits.VCFG1 = 0 ;  //Negative Voltage Reference select bit supplied by VSS. (Data Sheet PAGE 272)

    //3. Set the result justification, ADC clock source, and acquisition time in ADCON2.
            ADCON2 = 0b00111000; // Left Justified (7), Not Used (6), Aquisition Time 20 TAD (3-5),Conversion Clock Time FOSC/2 (0-2) (Data Sheet PAGE 273)

    //4. Select the channel and turn on the ADC in ADCON0.
            ADCON0 = 0b00000001; // Not Used(7-6), Analog Channel select AN0 (5-2), A/D Conversion Status Bit (1), ADC Enable (0) (Data Sheet PAGE 271)
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

        //The address byte contains
        //the 7 bit DS1307 address, which is 1101000, followed by the *direction bit (R/W ) which, for a write,
        //is a 0.
}

////////////////////////////////////////////////////////////////////////////////
//void Start_Conversion()
/*
        Purpose     : Start an ADC conversion.
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////

void Start_Conversion()
{
    ADCON0bits.GO_DONE = 1; //Start Conversion.
    while(ADCON0bits.GO_DONE) //While conversion is not completed. Loop.
    {
     Nop();  //Implement a delay structure.
    }
    Digital_Result = ADRESH;
    Process_Digital_Result();
}

////////////////////////////////////////////////////////////////////////////////
//void Process_Digital_Result()
/*
        Purpose     : Handle result of ADC conversion.
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////

void Process_Digital_Result()
{
    // Compared Digital_Result to an Value we choose.
    // LED Will turn Off/On depending on comparision.
}

////////////////////////////////////////////////////////////////////////////////
//void main()
/*
        Purpose     : Main insertion into program.
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////

void main()
{	
    Init(); //Initialize the board and all necesary ports.
    while(1) //Program loop.
    {
        Start_Conversion(); //Start a conversion.
	//Get data from RTC (DS1307).
	//Output the ADC conversion data and RTC data to LCD screen.
    }
}
