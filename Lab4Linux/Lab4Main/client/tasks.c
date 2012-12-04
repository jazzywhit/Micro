#include "globals.h"
#include "tasks.h"

//----------------------------GLOBAL VARIABLES-----------------------

// Locks
pthread_mutex_t stdoutMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sensorDataMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t comandMutex = PTHREAD_MUTEX_INITIALIZER;

// Initialization only needed in single threaded application
RTCData rtcDataStruct = { 1, 2, 3, 4, 5, 6 , 7, 8 };
Command comandStruct = { MSG_GET, 0 , 0 };
ADCData adcData = 0;


// Used in file descriptor set implementation
char *ping = "ping";
char *reset = "reset";
char *get = "get";
char *enable = "enable";
char *disable = "disable";
char *between = "between";
char *outside = "outside";

//---------------------------------------------------------------------

/*--------------------userInterface()-------------------
 Manages a user interface that reads in commands from the
 stdout
 ------------------------------------------------------*/
void *userInterface(void *pointer){
    
    
    char cmd; // Command read from the command line.
    unsigned short low;
    unsigned short high;
    
    int ignored; // Ignored character buffer for the command line.
    
    while(TRUE){
        
        pthread_mutex_lock(&stdoutMutex); // <-----------------------LOCK STDOUT
        
        printf("\n-------------------------------IN USER INTERFACE FUNCTION-------------------------------\n");
        
        //Prompt user with the program operation.
        printf("\n\nWhen entering a command only the first letter will be handled.");
        printf("\nAll other characters will be ignored.");
        printf("\n\nCommands:\n\tE = Enable\n\tD = Disable\n\tB = Between\n\tO = Outiside\n\tR = Reset\n\tG = Get\n\tP = Ping\n\tQ = Quit\n");
        
        //Ask user for input
        printf("\n\nEnter a command: ");
        
        // Handle user input.
        scanf("%c", &cmd);
       
        // If between or Outside read next 2 numbers
        if (toupper(cmd) == 'B'  || toupper(cmd) == 'O' ) scanf("%hu%hu" , &low, &high);
        
        // Empty stdin buffer
        while (getchar() != '\n');
        
        pthread_mutex_lock(&comandMutex); // <----------------------LOCK COMMAND

        // Handle command operation.
        switch(toupper(cmd)){
            case 'E':
                ClearTerminal();
                printf("Enable Command");
                comandStruct.command = MSG_INTENABLE;
                break;
            case 'D':
                ClearTerminal();
                printf("Disable Command");
                comandStruct.command = MSG_INTDISABLE;
                break;
            case 'B':
                ClearTerminal();
                comandStruct.lowBound = low;
                comandStruct.highBound = high;
                printf("Between Command, Low bound: %hu  High bound: %hu", comandStruct.lowBound, comandStruct.highBound);

                comandStruct.command = MSG_INTBETWEEN;
                break;
            case 'O':
                ClearTerminal();
                comandStruct.lowBound = low;
                comandStruct.highBound = high;
                printf("Outside Command, Low bound: %hu  High bound: %hu", comandStruct.lowBound, comandStruct.highBound);

                comandStruct.command = MSG_INTOUTSIDE;
                break;
            case 'R':
                ClearTerminal();
                printf("Reset Command");
                comandStruct.command = MSG_RESET;
                break;
            case 'G':
                ClearTerminal();
                printf("Get Command");
                comandStruct.command = MSG_GET;
                break;
            case 'P':
                ClearTerminal();
                printf("Ping Command");
                comandStruct.command = MSG_PING;
                break;
            case 'Q':
                ClearTerminal();
                printf("Quit Command\n");
                pthread_exit(NULL);
                break;
            default:
                ClearTerminal();
                printf("Command not valid. Enter(R, G, P, Q)");
                break;
        }
        pthread_mutex_unlock(&comandMutex); // <----------------------UNLOCK COMMAND
       
        DisplayData();
        usleep(WAIT_TIME);
            
        pthread_mutex_unlock(&stdoutMutex);// <-----------------------UNLOCK STDOUT
        // Give up the CPU for a while
        usleep(WAIT_TIME);


    }
    
	 pthread_exit(NULL);
}

/*--------------------sensorControl()-------------------
 Communicates with the sensor and updates the ADC value
 ------------------------------------------------------*/
void *sensorControl(void *pointer)
{
    while (1) {
        
        
        pthread_mutex_lock(&stdoutMutex); // <-----------------------LOCK STDOUT
        printf("\n-------------------------------IN SENSOR CONTROL FUNCTION-------------------------------\n");
        pthread_mutex_unlock(&stdoutMutex); // <-----------------------UNLOCK STDOUT

        // Give up the CPU for a while
        usleep(WAIT_TIME);
    }
    pthread_exit(NULL);
}

/*--------------------serverCommunication()------------
Performs the communication with the server
------------------------------------------------------*/
void *serverCommunication(void *pointer)
{

    while (1) {
        
        pthread_mutex_lock(&stdoutMutex); // <-----------------------LOCK STDOUT
        printf("\n-------------------------------IN SERVER COMMUNICATION FUNCTION-------------------------------\n");
        pthread_mutex_unlock(&stdoutMutex); // <-----------------------UNLOCK STDOUT
        // Give up the CPU for a while
        usleep(WAIT_TIME);

        const char* hostname="localhost";
        const int   port=8000;
        const int   id=1;
        const char* password="password";
        const char* name="ShouldaBeenGangstas";
        const int   adcval=123;
        const char* status="Status:Running";
        
        char buf[1024];
        
        snprintf(buf, 1024, "http://%s:%d/update?id=%d&password=%s&name=%s&data=%d&status=%s",
                 hostname,
                 port,
                 id,
                 password,
                 name,
                 adcval,
                 status);
        
        HTTP_GET(buf);
        
    }
    pthread_exit(NULL);
}

/*-------------------------HTTP_GET()----------------------
Sends a command to the web server using curl libraries.
----------------------------------------------------------*/
void HTTP_GET(const char* url){
    
	CURL *curl;
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
    
}

//--------------------- DisplayData -------------------------
//Purpose: Diaplay the ADC and RTC data retreived from the PIC.

void DisplayData(void){
    char day ;
    
    switch ( rtcDataStruct.day ) {
        case 2:
            day = 'M';
            break;
        case 3:
            day = 'T';
            break;
        case 4:
            day = 'W';
            break;
        case 5:
            day = 'R';
            break;
        case 6:
            day = 'F';
            break;
        case 7:
            day = 'S';
            break;
        case 1:
            day = 'U';
            break;
        default:
            break;
    }
    
    
    printf("\nADC result: %u" , adcData) ;
    
    printf("\nTime: %02u:%02u:%02u %c %02u/%02u/20%02u" ,
           rtcDataStruct.hour,
           rtcDataStruct.minutes,
           rtcDataStruct.seconds,
           day,
           rtcDataStruct.month,
           rtcDataStruct.date,
           rtcDataStruct.year );  
    
}

//------------------------------- ClearTerminal ------------------------------------------
//Purpose: Clear the terminal screen.

void ClearTerminal()
{
    
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n");
}



