#include "globals.h"
#include "tasks.h"

/* Since this program depends on libcurl, you may need to 
 * install libcurl4-gnutls-dev.
 *
 * On Linux Mint, this can be done by running:
 * sudo apt-get install libcurl4-gnutls-dev
 */


int main(void){

    ClearTerminal();
    userInterface((void*)NULL);
    
    //    pointer = sensorControl(pointer);
    //    pointer = serverComunication(pointer);
    
	return 0;
}
