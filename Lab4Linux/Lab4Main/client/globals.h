#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h> // Object files must be compiled with -pthread flag (gcc)
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <curl/curl.h>
#include <poll.h>
#include <ctype.h>

#define WAIT_TIME 10        //Delay

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

#define TRUE 1
#define FALSE 0


// Typedefs
typedef struct{
    unsigned char seconds;
    unsigned char minutes;
    unsigned char hour;
    unsigned char day;
    unsigned char date;
    unsigned char month;
    unsigned char year;
    unsigned char control;
}RTCData;

typedef unsigned short ADCData;

typedef struct{
    unsigned char command;
    ADCData highBound ;
    ADCData lowBound;
}Command;

    
/*
// Other Macros
#define KEYBOARD 0
#define INPUT_SIZE 20
#define TIMEOUT 1

// URL data to send
typedef struct {

	unsigned int id;
	char passwd[50];
	char name[9];     // "Shamrock" is 8 characters plus null terminator
	char status[100];
	unsigned int data;

} url_data;

// Global data
extern unsigned char command;             // Command shared between interface and sensor threads
extern unsigned char low_bound;
extern unsigned char high_bound;
extern url_data Url_Data;                 // URL data shared between sensor and communicate threads
extern pthread_mutex_t command_mutex;     // Mutex shared between interface and sensor threads
extern pthread_mutex_t communication_mutex; // Mutex shared between sensor and communicate threads
extern size_t dummy(void *nothing1, size_t nothing2, size_t nothing3, void *nothing4);
*/

#endif
