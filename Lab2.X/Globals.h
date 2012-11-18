/* 
 * File:   Globals.h
 * Author: students
 *
 * Created on October 30, 2012, 5:04 PM
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

#ifdef	__cplusplus
extern "C" {
#endif

//Functions
typedef struct{
    unsigned seconds :8;
    unsigned minutes :8;
    unsigned hours :8;
    unsigned day :8;
    unsigned date :8;
    unsigned month :8;
    unsigned year :8;
    unsigned control:8;
}timeStr;

typedef union{
    unsigned int allbits;
    struct{
       unsigned adresl:8;
       unsigned adresh:2;
    }read;
    struct{
        unsigned lbits:4;
        unsigned mbits:4;
        unsigned hbits:2;
    }write;
}ADCData;

typedef union{
    struct{
        unsigned low:4;
        unsigned high:4;
    }parts;
    unsigned all:8;
} ByteParts;

typedef struct{
    unsigned char high;
    unsigned char low;
    unsigned char outside;
    unsigned char enable;
    ADCData adcData;
}ADCControl;

#define  INT_FLAG PORTDbits.RD6
#define BYTE unsigned char

//Parallel Port Commands ----------------------------
#define MSG_NOTHING 0x0F

#define MSG_PING 0x01
#define MSG_ACK_PING 0x0E

#define MSG_GET 0x02
#define MSG_ACK_GET 0x0D

#define MSG_RESET 0x03
#define MSG_ACK_RESET 0x0C

#define MSG_INTBETWEEN 0x4
#define MSG_ACK_INTBETWEEN 0xB

#define MSG_INTOUTSIDE 0x5
#define MSG_ACK_INTOUTSIDE 0xA

#define MSG_ENABLE 0x6
#define MSG_ACK_ENABLE 0x9

#define MSG_DISABLE 0x7
#define MSG_ACK_DISABLE 0x8
//--------------------------------------------------

#define TRUE 1
#define FALSE 0

#define SECONDS 's'
#define MINUTES 'm'
#define HOURS 'h'
#define DAY 'd'
#define DATE 'D'
#define MONTH 'M'
#define YEAR 'Y'
#define CONTROL 'C'

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALS_H */

