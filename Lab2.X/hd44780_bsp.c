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
#define CLEAR_DISPLAY 0x01
void InitHD44780(){
    OpenXLCD(FOUR_BIT & LINES_5X7); //Set 4bit communication with 5x7 characters on two lines.
    WriteCmdXLCD(DON & CURSOR_OFF & BLINK_OFF);
    WriteCmdXLCD(CLEAR_DISPLAY);
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
}

void DelayPORXLCD(void){
    //Delay1KTCYx(60);
    Delay100TCYx(0xA0);   // 100TCY * 160
    Delay100TCYx(0xA0);   // 100TCY * 160
    Delay100TCYx(0xA0);   // 100TCY * 160
    Delay100TCYx(0xA0);   // 100TCY * 160
}

void DelayXLCD(void){
    //Delay1KTCYx(20);
     Delay100TCYx(0x36);      // 100TCY * 54
     Delay100TCYx(0x36);      // 100TCY * 54
     Delay100TCYx(0x36);      // 100TCY * 54
     Delay100TCYx(0x36);      // 100TCY * 54
}


void TestDisplay(){

    //char data[]="LCD Test";
    //OSCCON = 0x70;
    //while(!OSCCONbits.IOFS);
    //ADCON1=0x0F;
    ///WriteCmdXLCD();
    //WriteCmdXLCD(0x06);
    //WriteCmdXLCD(0x01);
    
   //putsXLCD("HELLO WORLD");
    //WriteDataXLCD('Q');
}