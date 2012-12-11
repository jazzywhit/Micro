#include "globals.h"
#include "tasks.h"
#include <semaphore.h>
#include <string.h>

//----------------------------GLOBAL VARIABLES-----------------------

// Locks
pthread_mutex_t stdoutMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sensorDataMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t commandMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t quitMutex = PTHREAD_MUTEX_INITIALIZER;

// Semaphores
sem_t sensorDataReady;

// Initialization of global variables
RTCData rtcDataStruct = { 1, 2, 3, 4, 5, 6 , 7, 8 };
Command commandStruct = { MSG_GET, 0 , 0 };
int sensorData;

int quit = 0;

//---------------------------------------------------------------------

/*--------------------userInterface()-------------------
 Manages a user interface that reads in commands from the
 stdout
 ------------------------------------------------------*/
void *userInterface(void *pointer){
    
    char cmd; // Command read from the command line.
    unsigned short low;
    unsigned short high;

    while(TRUE){

	usleep(WAIT_TIME);
        
        pthread_mutex_lock(&stdoutMutex); // <-----------------------LOCK STDOUT
        
        printf("\n-------------------------------IN USER INTERFACE FUNCTION-------------------------------\n");
        
        //Prompt user with the program operation.
        printf("\n\nWhen entering a command only the first letter will be handled.");
        printf("\nAll other characters will be ignored.");
        printf("\n\nCommands:\n\tE = Enable\n\tD = Disable\n\tB = Between\n\tO = Outside\n\tR = Reset\n\tG = Get\n\tP = Ping\n\tQ = Quit\n");
        
        //Ask user for input
        printf("\n\nEnter a command: ");
        
        // Handle user input.
        scanf("%c", &cmd);
		
		
		// If between or Outside read next 2 numbers
        if (toupper(cmd) == 'B'  || toupper(cmd) == 'O' ) {
			
            int scanfTest = scanf("%hu%hu" , &low, &high);
            if (scanfTest != 2) {
				
                printf("\ERROR: You must enter the lower and upper bounds with B and O.");
				while (getchar() != '\n');
				continue;
            }
        }
        
        // Empty stdin buffer
        while (getchar() != '\n');
        
        pthread_mutex_lock(&commandMutex); // <----------------------LOCK COMMAND

        // Handle command operation.
        switch(toupper(cmd)){
            case 'E':
                ClearTerminal();
                printf("Enable Command");
                commandStruct.command = MSG_INTENABLE;
                break;
            case 'D':
                ClearTerminal();
                printf("Disable Command");
                commandStruct.command = MSG_INTDISABLE;
                break;
            case 'B':
                ClearTerminal();
                commandStruct.lowBound = low;
                commandStruct.highBound = high;
                printf("Between Command, Low bound: %hu  High bound: %hu", commandStruct.lowBound, commandStruct.highBound);

                commandStruct.command = MSG_INTBETWEEN;
                break;
            case 'O':
                ClearTerminal();
                commandStruct.lowBound = low;
                commandStruct.highBound = high;
                printf("Outside Command, Low bound: %hu  High bound: %hu", commandStruct.lowBound, commandStruct.highBound);

                commandStruct.command = MSG_INTOUTSIDE;
                break;
            case 'R':
                ClearTerminal();
                printf("Reset Command");
                commandStruct.command = MSG_RESET;
                break;
            case 'G':
                ClearTerminal();
                printf("Get Command");
                commandStruct.command = MSG_GET;
                break;
            case 'P':
                ClearTerminal();
                printf("Ping Command");
                commandStruct.command = MSG_PING;
                break;
            case 'Q':
				ClearTerminal();
                printf("Quit Command\n");
				
				// Set quit to TRUE so other threads will exit
				pthread_mutex_lock(&quitMutex);  // <----------------------LOCK QUIT
				quit = 1 ;
				pthread_mutex_unlock(&quitMutex);  // <----------------------UNLOCK QUIT
				
				pthread_mutex_unlock(&commandMutex); // <----------------------UNLOCK COMMAND
				pthread_mutex_unlock(&stdoutMutex);// <-----------------------UNLOCK STDOUT
				
				// Quit
                pthread_exit(NULL);
                
                break;
            default:
                ClearTerminal();
                printf("Command not valid. Enter(R, G, P, Q)");
                break;
        }
        pthread_mutex_unlock(&commandMutex); // <----------------------UNLOCK COMMAND
       
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
	/* File pointer that represents the sensor device */
	FILE *charDevice;
	
	int quitEnabled = 0;
    
	/* Strings to send to the sensor device for various operations */
	char RESET_COMMAND[]     = "reset";
	char PING_COMMAND[]      = "ping";
	char ENABLE_COMMAND[]    = "enable";
	char DISABLE_COMMAND[]   = "disable";
	char OUTSIDE_COMMAND[20] = {0};
	char INSIDE_COMMAND[20]  = {0};
	char GET_COMMAND[] 		 = "get";
	char adcBuffer[BUFFER_SIZE];

	int test;	
    
	//Loop for Sensor Control
    while (1) {
		
		// Check if thread should exit
		pthread_mutex_lock(&quitMutex);  // <----------------------LOCK QUIT
		quitEnabled = quit;
		pthread_mutex_unlock(&quitMutex);  // <----------------------UNLOCK QUIT
		
		
		if (quitEnabled) {
			
			pthread_mutex_lock(&stdoutMutex); // <-----------------------LOCK STDOUT
			printf("\n-------------------------------QUITING SENSOR CONTROL FUNCTION-------------------------------\n");
			pthread_mutex_unlock(&stdoutMutex); // <-----------------------UNLOCK STDOUT
			pthread_exit(NULL);
		}
		
        charDevice = fopen("/dev/pp0adc", "r+");
		if ( !charDevice )
		{
			pthread_mutex_lock(&stdoutMutex); // <-----------------------LOCK STDOUT

			fprintf(stderr, "sensor thread: fopen\n");
			exit(1);
		
			pthread_mutex_unlock(&stdoutMutex); // <-----------------------UNLOCK STDOUT

		}

		
		pthread_mutex_lock(&stdoutMutex); // <-----------------------LOCK STDOUT
		
        printf("\n-------------------------------IN SENSOR CONTROL FUNCTION-------------------------------\n");
        if(commandStruct.command != MSG_NOTHING){
            switch(commandStruct.command)
            {
					
				case MSG_GET:
					test = fwrite( GET_COMMAND, sizeof(char), strlen(GET_COMMAND), charDevice);
					
					if(!test)
						fprintf(stderr, "fwrite error\n");
                    else
						test = fread(adcBuffer, sizeof(char), BUFFER_SIZE , charDevice);
					
					printf("%s", adcBuffer);
					
					pthread_mutex_lock(&sensorDataMutex); // <-----------------------LOCK SENSOR DATA
					
					// Update sensorData
					// Discard first token
					sensorData = atoi( strtok (adcBuffer," "));
					// Use second token
					sensorData = atoi(strtok (NULL, " "));
					// Post semaphore
					sem_post(&sensorDataReady);
					
					pthread_mutex_unlock(&sensorDataMutex); // <-----------------------UNLOCK SENSOR DATA
					break;
					
                case MSG_PING:
                    test = fwrite( PING_COMMAND, sizeof(char), strlen(PING_COMMAND), charDevice);
                    if(!test)
                        fprintf(stderr, "fwrite error\n");
                    else
                        printf("Ping Worked\n");
					
                    break;
                    
                case MSG_RESET:
                    test = fwrite( RESET_COMMAND, sizeof(char), strlen(RESET_COMMAND), charDevice);
                    if(!test)
                        fprintf(stderr, "fwrite error\n");
                    else
						printf("Reset Worked\n");
					
                    break;
                    
                case MSG_INTDISABLE:
                    test = fwrite( DISABLE_COMMAND, sizeof(char), strlen(DISABLE_COMMAND), charDevice);
                    if(!test)
                        fprintf(stderr, "fwrite error\n");
                    else
                        printf("Disable Worked\n");
                    
                    break;
                    
                case MSG_INTENABLE:
                    test = fwrite( ENABLE_COMMAND, sizeof(char), strlen(ENABLE_COMMAND), charDevice);
                    if(!test)
                        fprintf(stderr, "fwrite error\n");
                    else
                        printf("Enable Worked\n");
                    
                    break;
                case MSG_INTBETWEEN:
                    
                    // Format string command
                    sprintf( INSIDE_COMMAND, "between %d %d", commandStruct.lowBound, commandStruct.highBound);
                    
                    test = fwrite( INSIDE_COMMAND, sizeof(char), strlen(INSIDE_COMMAND), charDevice);
                    
                    if(!test)
                        fprintf(stderr, "fwrite error\n");
                    else
                        printf("Between Worked\n");
                    
                    break;
                    
                case MSG_INTOUTSIDE:
                    
                    // Format string command
                    sprintf( OUTSIDE_COMMAND, "outside %d %d", commandStruct.lowBound, commandStruct.highBound);
                    
                    test = fwrite( OUTSIDE_COMMAND, sizeof(char), strlen(INSIDE_COMMAND), charDevice);
                    if(!test)
                        fprintf(stderr, "fwrite error\n");
                    else
                        printf("Outside Worked\n");
                    
                    break;
                    
					
                default:
                    /* Unrecognized command type: this is bad */
                    fprintf(stderr, "ERROR:Unrecognized command in sensor control thread\n");
                    break;
                    
            }
			
        }
        
        pthread_mutex_unlock(&stdoutMutex); // <-----------------------UNLOCK STDOUT
		
        fclose(charDevice);
        
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

	int quitEnabled = 0;

    while (1) {
        
        pthread_mutex_lock(&stdoutMutex); // <-----------------------LOCK STDOUT
        printf("\n-------------------------------IN SERVER COMMUNICATION FUNCTION-------------------------------\n");
        pthread_mutex_unlock(&stdoutMutex); // <-----------------------UNLOCK STDOUT
        // Give up the CPU for a while
        usleep(WAIT_TIME);
		
		
		pthread_mutex_lock(&quitMutex);  // <----------------------LOCK QUIT
		quitEnabled = quit;
		pthread_mutex_unlock(&quitMutex);  // <----------------------UNLOCK QUIT
		
		if (quitEnabled) {
			
			pthread_mutex_lock(&stdoutMutex); // <-----------------------LOCK STDOUT
			printf("\n-------------------------------QUITING SERVER COMMUNICATION FUNCTION-------------------------------\n");
			pthread_mutex_unlock(&stdoutMutex); // <-----------------------UNLOCK STDOUT
			pthread_exit(NULL);
		}
		
		sem_wait(&sensorDataReady);

        const char* hostname="localhost";
        const int   port=8000;
        const int   id=1;
        const char* password="password";
        const char* name="ShouldaBeenGangstas";
        const char* status="Status:Running";
        
        char buf[1024];
        
		pthread_mutex_lock(&sensorDataMutex); // <-----------------------LOCK SENSOR DATA

        snprintf(buf, 1024, "http://%s:%d/update?id=%d&password=%s&name=%s&data=%d&status=%s",
                 hostname,
                 port,
                 id,
                 password,
                 name,
                 sensorData,
                 status);
		
		pthread_mutex_unlock(&sensorDataMutex); // <-----------------------UNLOCK SENSOR DATA

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

//------------------------------- ClearTerminal ------------------------------------------
//Purpose: Clear the terminal screen.

void ClearTerminal()
{
    if (CLEAR_TERMINAL_ENABLE) {
        
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("\n\n");
    }
}


