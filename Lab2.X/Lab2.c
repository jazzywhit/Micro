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
#include "parallel_bsp.h"
#include "hd44780_bsp.h"

//PRAGMAS
#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF, LVP = OFF


/*--------------------------- main() ------------------------------------------------------
 Purpose     : Main insertion into program.
 Parameters  : N/A
 Output      : N/A
*/

void main()
{
    P18f45k20Init(); //Initialize the board and all necesary ports.
    InitHD44780();  //Initialize the LCD Board
    //SetupTimeDS1307(&seconds, &minutes, &hours, &day, &date, &month, &year); ****ONLY NEED TO DO THIS ONCE****

    while(1)
    {
        ProcessADC(ReadADC()); //Send the value to turn LED on/off
        //TestDisplay(); //Output the ADC conversion data and RTC data to LCD screen.
        CheckParallel(); //Check the Parallel Port for Communications.
    }
}
