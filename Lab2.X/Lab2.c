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
#include "p18f45k20_bsp.h" //Our own board support package
#include "ds1307_bsp.h"
#include "parallel_bsp.h"

////////////////////////////////////////////////////////////////////////////////
//PRAGMAS
////////////////////////////////////////////////////////////////////////////////
#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF, LVP = OFF

//unsigned char Sec=0,Min=0, Hrs=0, Mon=0, Year=0, Date=0, Day=0;


////////////////////////////////////////////////////////////////////////////////
//void main()
/*
 Purpose     : Main insertion into program.
 Parameters  : N/A
 Output      : N/A
 */
////////////////////////////////////////////////////////////////////////////////

unsigned char adcRead = 0;
unsigned char seconds = 0;
unsigned char minutes = 25;
unsigned char hours = 05;
unsigned char day = 1;
unsigned char date = 30;
unsigned char month = 9;
unsigned char year = 12;
unsigned char test = 1;

void main()
{
    //unsigned char CMDRead;
    P18f45k20Init(); //Initialize the board and all necesary ports.
    //InitHD44780();  //Initialize the LCD Board
    //SetupTimeDS1307(&seconds, &minutes, &hours, &day, &date, &month, &year);
    
// Set data bus as input
    TRISA &= 0b11100001;     // PORTA bit 1,2,3,4 -> Output

    PORTAbits.RA1 = 0;
    PORTAbits.RA2 = 1;
    PORTAbits.RA3 = 1;
    PORTAbits.RA4 = 1;
    while(1)
    {
 
        //adcRead = ReadADC(); //Get the value from the ADC
        //ProcessDigitalResult(&adcRead); //Send the value to turn LED on/off
        //ReadTimeDS1307(&seconds, &minutes, &hours, &day, &date, &month, &year , &control); //Get data from RTC (DS1307).

       // if( STROBE) ReadCommand();
        //Output the ADC conversion data and RTC data to LCD screen.
    }
}
