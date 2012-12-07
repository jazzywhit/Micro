/*
 *File: ppadc.h
 *Purpose: pp0adc driver header file
 *Last Date Modified: 12/07/12
 */

#define DRIVER_NAME "ppadc"				//Driver Name

#define WAIT_TIME 10					//Delay Time

#define STRING_BUFFER_SIZE 0x40				//String Buffer Size

#define MIN(a,b) (a<b ? a : b)				//MIN Function Define

//Host Driver Command Messages

#define MSG_PING      		0x1			//Ping Message
#define MSG_ACK_PING  		0xE 

#define MSG_GET       		0x2			//Get Message
#define MSG_ACK_GET   		0xD 

#define MSG_RESET     		0x3			//Reset Message
#define MSG_ACK_RESET 		0xC

#define MSG_INTBETWEEN 		0x4			//Between Message
#define MSG_ACK_INTBETWEEN 	0xB

#define MSG_INTOUTSIDE 		0x5 			//Outside Message
#define MSG_ACK_INTOUTSIDE 	0xA

#define MSG_INTENABLE  		0x6 			//Interrupt Enable Message
#define MSG_ACK_INTENABLE 	0x9

#define MSG_INTDISABLE 		0x8 			//Interrupt Disable Message
#define MSG_ACK_INTDISABLE 	0xF

//Host Driver Command Strings

#define CMD_GET        		"get"			//Get Command String

#define CMD_RESET      		"reset" 		//Reset Command String

#define CMD_PING       		"ping"  		//Ping Command String

#define CMD_INTENABLE  		"enable"		//Enable Command String

#define CMD_INTDISABLE 		"disable"		//Disable Command String

#define CMD_BETWEEN    		"between"		//Between Command String

#define CMD_OUTSIDE    		"outside"		//Outside Command String

//Masks of Parallel Port Control Register

#define IMPED_STROB_MASK 	((1<<5) | (1<<0))	//Mask bits 5 and 0 of control port

#define IMPED_LO         	((0<<5) |    0  )	//Data port = output

#define IMPED_HI         	((1<<5) |    0  )	//Data port = input

#define STROB_LO         	(   0   | (1<<0))	//nStrobe = 1

#define STROB_HI         	(   0   | (0<<0))	//nStrobe = 0

#define NIBBLE_MASK    		0xF			//4-bit nibble mask

