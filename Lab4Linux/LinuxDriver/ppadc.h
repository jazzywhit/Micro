// ppadc.h

/*
 * Purpose: Provides the set of instructions available on the PIC16F684 ADC device
 *           as well as the commands available to the user on the host. 
 * 
 * part of the skeleton code for UMass Lowell 16.480/552 Fall 2011
 *
 * License: GPL
 */

//Delay
#define WAIT_TIME 10

#define MSG_PING       0x1
#define MSG_ACK_PING 0xE

#define MSG_GET        0x2
#define MSG_ACK_GET    0xD 

#define MSG_RESET      0x3 
#define MSG_ACK_RESET 0xC

#define MSG_INTBETWEEN 0x4
#define MSG_ACK_INTBETWEEN 0xB

#define MSG_INTOUTSIDE 0x5
#define MSG_ACK_INTOUTSIDE 0xA

#define MSG_INTENABLE  0x6
#define MSG_ACK_INTENABLE 0x9

#define MSG_INTDISABLE 0x8
#define MSG_ACK_INTDISABLE 0xF


// Host Driver Commands
#define CMD_GET        "get"
#define CMD_RESET      "reset"
#define CMD_PING       "ping"
#define CMD_INTENABLE  "enable"
#define CMD_INTDISABLE "disable"
#define CMD_BETWEEN    "between"
#define CMD_OUTSIDE    "outside"

// Useful Bits/Masks of Parallel Port Control Register
#define IMPED_STROB_MASK ((1<<5) | (1<<0)) // Mask bits 5 and 0 of control port
#define IMPED_LO         ((0<<5) |    0  ) // Data port = output
#define IMPED_HI         ((1<<5) |    0  ) // Data port = input
#define STROB_LO         (   0   | (1<<0)) // nStrobe = 1
#define STROB_HI         (   0   | (0<<0)) // nStrobe = 0

// Misc.
#define NIBBLE_MASK    0xF

