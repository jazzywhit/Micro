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
    unsigned char minutes = 0;
    unsigned char hours = 0;
    unsigned char day = 0;
    unsigned char date = 0;
    unsigned char month = 0;
    unsigned char year = 0;
      unsigned char test = 1;
void main()
{

        P18f45k20Init(); //Initialize the board and all necesary ports.



   test =  SetupTimeDS1307(&seconds, &minutes, &hours, &day, &date, &month, &year);
   if (!test) return 0;
   
//-----------------------------------------------------------------NOTE THAT READ AND WRITE FUNCTIONS RETURN AN unsigned char, FOR ERROR CHECKING,
//-----------------------------------------------------------------We could change the Program loop to acount for that
    while(1) //Program loop.
    {
        adcRead = ReadADC(); //Get the value from the ADC
        ProcessDigitalResult(&adcRead); //Send the value to turn LED on/off
	ReadTimeDS1307(&seconds, &minutes, &hours, &day, &date, &month, &year); //Get data from RTC (DS1307).
	//Output the ADC conversion data and RTC data to LCD screen.
    }
}
