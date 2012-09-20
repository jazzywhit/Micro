/* UMass Lowell 16.480/552, Fall 2012
 *
 * Lab 1
 *
 * Lab1.c
 * 
 * License: GNU GPLv3.
 */

/* Group Name: Gangstas
 * Group Members:
 * - Jesse Whitworth
 * - Matthew Cook
 * - Denis Lemos
 * - Aadil Hassan
 * 
 * Date: 9-20-2012
 */

#include "p18f45K20.h"
#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF, LVP = OFF


//*** These are likely not needed as the new version of MPLABX has the precompilers already configured.
////////////////////////////////////////
/* Toolchain-specific directives
#ifdef SDCC
	// Small Device C Compiler
	#include <pic18f45k20.h>
	unsigned int at 0x2007 CONFIG =  _WDT_OFF & _PWRTE_ON & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOD_OFF & _IESO_OFF & _FCMEN_OFF & _INTOSCIO;
#else
	// HI-TECH C Compiler
	#include <htc.h>
	__CONFIG(WDTDIS & PWRTEN & MCLRDIS & UNPROTECT & UNPROTECT & BORDIS & IESODIS & FCMDIS & INTIO );
	#define GO_DONE GODONE
#endif

/*
 * DEFINITIONS ***** These should actually have no place in this program
 */

//Preprocessors
#define PPBit0 RA2  // PP Nibble bits
#define PPTRIS TRISA2=TRISC2=TRISC1=TRISC0 // PP Nibble tristate; allows setting all PP nibble bits as input or output simultaneously

//Structures
struct bounds{
	unsigned int low, high;
};

/*
 * FUNCTION DECLARATIONS
*/
void Init();
void InitADC();


/*
 * ------------------readAN0
// Purpose:    Reads the analog value of AN0 and stores it in "int adc"
// Parameters: None; But the ANCON and ANSEL registers should be setup before this method is called
// Returns:    The new value of "int adc"
*/
unsigned int readAN0(){

	return 0;
}



/*
 * ----------------------- I n i t () ---------------------------------------
 * Initialize the board
 * Functions called:
 *  InitADC()
 */
void Init(){
    // Comparitors off, CxIN pins are configured as digital I/O
    
    InitADC();
}


/*
 * ----------------------- I n i t A D C () ---------------------------------
 * Initialize any registers that the ADC needs for communication.
 */
void InitADC(){

    // From the template
    // We only need one analog pin: AN0
    // Setup ADCON0 and other ADC related registers

    // From the Programming guide p.45
    //    Looking at the schematic of the 44-Pin Demo Board in the Appendix, the potentiometer (RP1) output is connected to the RA0/AN0 pin of the PIC18F45K20.
    //    The basic steps needed to convert the ADC voltage on this pin are:
    //    1. Configure the RA0/AN0 pin as an analog input in ANSEL.
    //    2. Set the ADC voltage references in ADCON1.
    //    3. Set the result justification, ADC clock source, and acquisition time in ADCON2.
    //    4. Select the channel and turn on the ADC in ADCON0.
    //    5. Start the conversion in ADCON0.
}

/*
 * ----------------------- m a i n () ---------------------------------
 */
void main(){	

    //Light LED Example code (sanity)
    //TRISD = 0b01111111;// PORTD bit 7 to output (0); bits 6:0 are inputs (1)
    //LATDbits.LATD7 = 1;// Set LAT register bit 7 to turn on LED

    //Initialize the board and all necesary ports
    Init();

    // Obtain ADC result
    readAN0();

    while (1)
    ;
}
