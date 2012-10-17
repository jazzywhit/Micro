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
    SSPADD = 0x27; //Fosc = 16 MHz , Fcy = 4 MHz , Fscl = 100 kHz
    OSCCONbits.IRCF = 0b111; //Internal Oscillator Frequency Select bits 111 = 16 MHz (HFINTOSC drives clock directly)
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
    while(ADCON0bits.GO_DONE) {
        continue ; //While conversion is not completed. Loop.
    }
    return ADRESH;
}


void ParallelStatus(){
    /*To obtain the most recent ADC result:
    1. The user application (on Atom) sends MSG_GET to the sensor;
    2. The sensor will output three 4?bit nibbles for ADC value and 8?byte RTC
    value to the embedded system, trigger by the ?Strobe? signal from the
    parallel port; the user application (on x86) reads the three nibbles in
    sequence;
    3. The sensor responds with MSG_ACK to tell the user application that it
    finishes reporting a pair of 10?bit ADC and timestamp. */

    //We will be checking the port for the MSG_GET byte and if we get that we will start sending the ADC Value.
    //3 nibbles and 8 bites.
    return 0;
}

void ParallelRead(){
    /*Figure 4 illustrates the five steps involved in a read operation from the computer
    (although the timing diagram has the same pattern as Figure 3, there are differences
    in the steps):
    (1) The computer pulls the Strobe signal low to get ready for read operation

    3
    (2) The PIC microcontroller on the sensor device outputs a 4?bit value (either
    MSG_ACK or portion of ADC value) when it sees a low on the strobe line.
    (3) The computer raises the Strobe signal and starting reading the value from
    the data bus. The PIC checks the Strobe signal and learns that the computer
    has started reading the value.
    (4) The computer pulls the Strobe signal low again to indicate that the value has
    been read.
    (5) The PIC microcontroller sees the Strobe pulled low and stops outputting the
    4?bit value.
    */
    return;
}

void ParallelWrite(){

    /*Figure 3. Write operation on the bus
 
    The steps (in Figure 3) involved in the write operation are as follows.
    (1) The computer pulls the Strobe signal low. The PIC microcontroller gets ready
    to read 4?bit command message.
    (2) The computer outputs a command on the data bus
    (3) The computer raises the Strobe signal to indicate the command is ready on
    the bus. The PIC microcontroller starts reading the value (i.e. a command)
    from the bus. The computer maintains the value for at least 10ms.
    (4) The computer ends the write operation by pulling the Strobe signal to low
    again. By this time, the PIC should have already finished reading the value.
    (5) The computer stops putting the command on the bus. The write operation
    concludes.
    */
    return;
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
    TRISA = 0b11111111;     // PORTA bit 0 set to 1 so AN0 can be the analog input voltage from light sensor
    TRISC = 0x11111111; //turn on tri-state register. The pins should be set to input for the I2C bus.

    //Setup PortD to attach to the LED
    TRISD = 0b01111111;// PORTD bit 7 to output (0); bits 6:0 are inputs (1) (a.k.a. RD7 or pin 30)
    TRISE = 0b11111111; //PORTE set as input. We are going to be using this port for part of our parallel port com.

}