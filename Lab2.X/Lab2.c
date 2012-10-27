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

unsigned short adcRead = 0; // 2 bytes
unsigned char seconds = 0;
unsigned char minutes = 25;
unsigned char hours = 05;
unsigned char day = 1;
unsigned char date = 30;
unsigned char month = 9;
unsigned char year = 12;
unsigned char control;

void main()
{
    //unsigned char CMDRead;
    //P18f45k20Init(); //Initialize the board and all necesary ports.
    //InitHD44780();  //Initialize the LCD Board
    //SetupTimeDS1307(&seconds, &minutes, &hours, &day, &date, &month, &year);
    TRISD &= 0b11110000;     // PORTA bit RC0,RC1,RC2,RC5 -> Output

    while(1)
    {

        adcRead = ReadADC(); //Get the value from the ADC
        ProcessDigitalResult(&adcRead); //Send the value to turn LED on/off
        ReadTimeDS1307(&seconds, &minutes, &hours, &day, &date, &month, &year , &control); //Get data from RTC (DS1307).
        ReadCommand();
        //WriteData(0x06);

//        PORTDbits.RD0 = 0;
//        PORTDbits.RD1 = 1;
//        PORTDbits.RD2 = 1;
//        PORTDbits.RD3 = 1;
        //WriteData(MSG_ACK);
        //WriteData(0);

        // Set data bus as output
        //TRISC &= 0xFF;     // PORTA bit RC0,RC1,RC2,RC5 -> Output

//        //Output the ADC conversion data and RTC data to LCD screen.
    }
}
