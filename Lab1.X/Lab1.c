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


#include <i2c.h>
#include <p18F45K20.h>


// PP Nibble tristate; allows setting all PP nibble
//  bits as input or output simultaneously
#define PPTRIS TRISA2=TRISC2=TRISC1=TRISC0

////////////////////////////////////////////////////////////////////////////////
//PRAGMAS
////////////////////////////////////////////////////////////////////////////////

#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF, LVP = OFF

////////////////////////////////////////////////////////////////////////////////
//DEFINES
////////////////////////////////////////////////////////////////////////////////

#define DS1307
#define PPBit0 RA2  // PP Nibble bits
#define PPTRIS TRISA2=TRISC2=TRISC1=TRISC0 // PP Nibble tristate; allows setting all PP nibble bits as input or output simultaneously

////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////

unsigned int Digital_Result = 0;
unsigned char Sec=0,Min=0, Hrs=0, Mon=0, Year=0, Date=0, Day=0;

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
void InitDS1307();
void Setup_Time_DS1307();
void Start_Conversion();
void Process_Digital_Result();
void Read_Time_DS1307();

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
    InitDS1307();
    Setup_Time_DS1307();
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

    TRISC = 0x00; //turn on tri-state register and
    //make all output pins
    PORTC = 0x00; //make all output pins LOW

    //Setup PortD to attach to the LED
    TRISD = 0b01111111;// PORTD bit 7 to output (0); bits 6:0 are inputs (1) (a.k.a. RD7 or pin 30)

}

 void InitDS1307(){
     unsigned char seconds = 0;

     StartI2C();
     WriteI2C(0xD0);
     WriteI2C(0x00);
     WriteI2C(0x03); //Write 3 to the 

     StartI2C();
     WriteI2C(0xD0);
     WriteI2C(0x07);
     WriteI2C(0x80);
     StopI2C();

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
        //Functions we can use
        SSPADD = 0x09;
        OSCCONbits.IRCF = 0b101;
        OpenI2C(MASTER, SLEW_OFF);
        //OSCCONbits.SCS = 0b00;

        //while(1){
        //    StartI2C();
        //    WriteI2C(0xD0);
        //    ReadI2C();
        //}


        //StartI2C(); //Begin the I2C Connection
        //IdleI2C();

        //WriteI2C( 0xD0 ); //Send an address to the clock
        //IdleI2C();
        
        //while(1){
        //AckI2C();
        //IdleI2C();
       // }
  
        //StopI2C();
        //IdleI2C();
//        I2C1_SCL = 1;               // Set SCL1 (PORTC,3) pin to input
//        I2C1_SDA = 1;               // Set SDA1 (PORTC,4) pin to input
        //DS1307
            /*
                The address byte contains
                the 7 bit DS1307 address, which is 1101000 (0x0),
                followed  by the direction bit (R/W ) which,
                for a write, is a 0.
             */
        //Registers Needed: SSPADD (Data Sheet page 153)
        //Pull up resistors: (2-10)k ohms
        //I2C Functions - http://wiki.ulcape.org/tutorials:pic:pic18_i2c
        //IdleI2C()
            /*
                IdleI2C() is placed in between as protection
                to make sure that the PIC doesn?t get ahead of
                itself in code before the devices are ready.
                Either way, it?s a safe guard and it doesn?t hurt.
             */

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
    if(Digital_Result < 60) //If it is dark the voltage will be around (.3-.4)V
        PORTDbits.RD7 = 1; // LED Will turn Off/On depending on comparision.
    else
	PORTDbits.RD7 = 0;
}

////////////////////////////////////////////////////////////////////////////////
//void Setup_Time_DS1307()
/*
        Purpose     : Setup Time in DS 1307
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////

void Setup_Time_DS1307()
{
    //Date and Time Variables
    //******** NEED TO BE CHANGED ******************
    unsigned char sec, min, hr, month, year, date, day; //These may be better as global variables for LCD output.
    min = 0b00000011;
    hr  = 0b01100111;   // Set to 12 hour format.
    sec = 0b00000000;   // 
    day = 0b00000100;   // Set 1 = Sunday.
    date= 0b00010110;   // Date set to 1st.
    month = 0b00001001;   // Month set to January.
    year= 0b00010010;   // Year set to 2008.

    //Transmit Time and Date
    StartI2C();
    //IdleI2C();
    //if (!DataRdyI2C()){
     WriteI2C(0xD0); //address of DS1307. bit 0 indicates write(0) or read(1)
       //IdleI2C();
    //}
     //if (!DataRdyI2C())
     //    IdleI2C();
      WriteI2C(0x00); // Position the address pointer to 0.

    //IdleI2C();
    // if (!DataRdyI2C())
      WriteI2C(sec);

    //IdleI2C();
    // if (!DataRdyI2C())
      WriteI2C(min);

    //IdleI2C();
    // if (!DataRdyI2C())
      WriteI2C(hr);

    //IdleI2C();
   // if (!DataRdyI2C())
      WriteI2C(day);

   //IdleI2C();
   // if (!DataRdyI2C())
      WriteI2C(date);

    //IdleI2C();

   // if (!DataRdyI2C())
       WriteI2C(month);

    //IdleI2C();

   // if (!DataRdyI2C())
       WriteI2C(year);

    //IdleI2C();
       WriteI2C(0x80); // Disable Square Wave
    //}

    //IdleI2C();
    StopI2C();
}

////////////////////////////////////////////////////////////////////////////////
//void Read_Time_DS1307()
/*
        Purpose     : Read Time from DS1307.
        Parameters  : N/A
        Output      : N/A
*/
////////////////////////////////////////////////////////////////////////////////

void Read_Time_DS1307()
{
    
     StartI2C();
     //IdleI2C();

     //if (!DataRdyI2C()){
        WriteI2C(0xD0); //address of DS1307.
    // }

        WriteI2C(0x00); // Position the address pointer to 0.

        StartI2C();
        WriteI2C(0xD1); //address of DS1307.

     //IdleI2C();

     //if (!DataRdyI2C()){
        //WriteI2C(0x00); // Position the address pointer to 0.
     //   AckI2C();
     //}

     //IdleI2C();

     //if (!DataRdyI2C()){
     //    WriteI2C(0b11010000 | 1); // Direction bit set to read.
     //    AckI2C();
     //}

     //IdleI2C();
    //Receive Time Varaibles from DS1307

     //if (DataRdyI2C()){
         Sec = ReadI2C();
         //AckI2C();
     //}

     //IdleI2C();
     //AckI2C();
     //IdleI2C();

     //if (DataRdyI2C()){
        Min = ReadI2C();
        //AckI2C();
     //}

     //IdleI2C();
     //AckI2C();
     //IdleI2C();

     //if (DataRdyI2C()){
        Hrs = ReadI2C();
     //    AckI2C();
     //}

     //IdleI2C();
     //AckI2C();
     //IdleI2C();

     //if (DataRdyI2C()){
         //Day = ReadI2C();
      //   AckI2C();
     //}

     //dleI2C();
     //AckI2C();
     //IdleI2C();

     //if (DataRdyI2C()){
         //Date = ReadI2C();
     //    AckI2C();
     //}

     //IdleI2C();
     //AckI2C();
     //IdleI2C();

     //if (DataRdyI2C()){
         //Mon = ReadI2C();
     //    AckI2C();
     //}

     //IdleI2C();
     // AckI2C();
     //IdleI2C();

     //if (DataRdyI2C()){
          //Year = ReadI2C();
     //     AckI2C();
     //}

     StopI2C();
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
	Read_Time_DS1307(); //Get data from RTC (DS1307).
	//Output the ADC conversion data and RTC data to LCD screen.
    }
}
