////////////////////////////////////////////////////////////////////////////////
/*
    UMass Lowell 16.480/552, Fall 2012

    Purpose : 16.480 Microprocessors II Lab 1
    File: hd44780_bsp.c

    Group Name: Shoulda Been Gangstas
    Group Members:
        - Jesse Whitworth
        - Matthew Cook
        - Denis Lemos
        - Aadil Hassan

    License: GNU GPLv3.

    Date Created: 9-30-2012
*/
////////////////////////////////////////////////////////////////////////////////

#include "hd44780_bsp.h"
void InitHD44780(){
   OpenXLCD(FOUR_BIT & LINES_5X7); //Set 4bit communication with 5x7 characters on two lines.
}

// 12 Nop to delay for 18 cycles.
void DelayFor18TCY(void){
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
}

void DelayPORXLCD(void){
    Delay1KTCYx(70);
}

void DelayXLCD(void){
    Delay1KTCYx(30);
}

void TestDisplay(){
    WriteDataXLCD('C');
}