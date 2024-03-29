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

//INCLUDES
#include "p18f45k20_bsp.h" //Our own board support package
#include "ds1307_bsp.h"
#include "hd44780_bsp.h"
#include "Globals.h"

//PRAGMAS
#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF, LVP = OFF

timeStr dateTime;

ADCControl adcControl;

/*--------------------------- main() ------------------------------------------------------
 Purpose     : Main insertion into program.
 Parameters  : N/A
 Output      : N/A
*/
void main()
{
    //setup dateTime
    //dateTime.seconds = 0;

    P18f45k20Init(); //Initialize the board and all necesary ports.

    //InitHD44780();  //Initialize the LCD Board
    //SetupTimeDS1307(&dateTime);// ****ONLY NEED TO DO THIS ONCE****
    //ReadTimeDS1307(&dateTime); //Send the date time construct.

    adcControl.high.allbits = ADC_COMPARE_VALUE; //Default to the ADC Compare value
    adcControl.low.allbits = 0;
    adcControl.outside = 0; //Between mode by default
    adcControl.enable = 0; //Disbaled by default
    adcControl.adcData.allbits = 0;

    while(1)
    {
        ReadADC(&adcControl);
        ProcessADC(&adcControl); //Send the value to turn LED on/off
        ReadTimeDS1307(&dateTime); //Send the date time construct.
        CheckParallel(&dateTime, &adcControl); //Check the Parallel Port for Communications.
        continue;
    }
}

