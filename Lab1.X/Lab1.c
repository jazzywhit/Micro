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
void main()
{
    unsigned char adcRead; 
    unsigned char seconds;
    unsigned char minutes;
    unsigned char hours;
    unsigned char day;
    unsigned char date;
    unsigned char month;
    unsigned char year;

    //Setup_Time_DS1307(&seconds, &minutes, &hours, &day, &date, &month, &year);

    P18f45k20Init(); //Initialize the board and all necesary ports.
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
