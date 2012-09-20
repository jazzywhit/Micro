/* UMass Lowell 16.480/552, Fall 2011
 *
 * Lab 1
 *
 * pic_adc_template.c
 * Template for PIC based ADC. 
 * 
 * License: GNU GPLv3.
 */

/* Group Name: 
 * Group Members:
 * Date:
 */


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



////////////////////////////////////////
// Constants

// PP Nibble bits
#define PPBit0 RA2

// PP Nibble tristate; allows setting all PP nibble
//  bits as input or output simultaneously
#define PPTRIS TRISA2=TRISC2=TRISC1=TRISC0

////////////////////////////////////////
// Structure Definitions
struct bounds{
	unsigned int low, high;
};

////////////////////////////////////////
// Function Declarations
unsigned int  readAN0();

////////////////////////////////////////
// Global Variables
unsigned int adc=0;                        // Holds the value of the ADC after calling readAN0()

////////////////////////////////////////
// Function Definitions

// readAN0
// Purpose:    Reads the analog value of AN0 and stores it in "int adc"
// Parameters: None; But the ANCON and ANSEL registers should be setup before this method is called
// Returns:    The new value of "int adc"

unsigned int readAN0(){
	
	return adc;
}
*/


#include "p18f45K20.h"
#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF, LVP = OFF
/* main
 * Purpose: Initializes PIC (setting configuration / pin tristates) and then
 *           passes control to listenForCommands()
 */
void main(){	

	TRISD = 0b01111111;// PORTD bit 7 to output (0); bits 6:0 are inputs (1)
	LATDbits.LATD7 = 1;// Set LAT register bit 7 to turn on LED
	while (1)
	;

	// Comparitors off, CxIN pins are configured as digital I/O
	
	// We only need one analog pin: AN0
	
	// Setup ADCON0 and other ADC related registers
	
	// Obtain ADC result
       // readAN0();
}
