#include "globals.h"
#include "tasks.h"

/* Since this program depends on libcurl, you may need to 
 * install libcurl4-gnutls-dev.
 *
 * On Linux Mint, this can be done by running:
 * sudo apt-get install libcurl4-gnutls-dev
 */


int main(void){
    
    int result = 0;
    
    // Thread Declarations
    pthread_t interfaceThread;
    pthread_t sensorControlThread;
    pthread_t servercommunicationThread;

    ClearTerminal();
    
    // Thread Definitions
    if ( (result = pthread_create(&sensorControlThread, NULL, sensorControl, NULL)) ){
        
        printf("\nERROR: Failed to create Sensor Control Thread, ERROR code: %d\n", result);
        return 1;
    }
	if ( (result = pthread_create(&servercommunicationThread, NULL, serverCommunication, NULL)) ){
        
        printf("\nERROR: Failed to create Server Communication Thread, ERROR code: %d\n", result);
        return 1;
    }
    
    if ( (result = pthread_create(&interfaceThread, NULL, userInterface, NULL)) ){
        
        printf("\nERROR: Failed to create User Interface Thread, ERROR code: %d\n", result);
        return 1;
        
    }

    // Wait for all threads to finish their jobs
    pthread_join(interfaceThread, NULL);
    pthread_join(sensorControlThread, NULL);
    pthread_join(servercommunicationThread, NULL);
    
    // Quit

	return 0;
}
