#include "globals.h"
#include "tasks.h"

/*
Interface thread which continuously blocks and waits for standard input (keyboard).
If data is valid, the matching command code is given to the sensor thread.
select(), similar to poll(), is used to block and wait for input from the keyboard file descriptor.
*/

void *interface(void *ptr)
{

/*
	fd_set interface_fds, test_fds;
	int fd;
	char input[INPUT_SIZE];
	char *ping = "ping";
	char *reset = "reset";
	char *get = "get";
	char *enable = "enable";
	char *disable = "disable";
	char *between = "between";
	char *outside = "outside";
	char delims[] = " ";
	char newline[] = "\n";
	char *result = NULL;

    
    
    // Four macros are provided to manipulate the sets. 
     // FD_ZERO() clears a set. 
   // FD_SET() and FD_CLR() respectively add and remove a given file descriptor from a set. 
     // FD_ISSET() tests to see if a file descriptor is part of the set; this is useful after select() returns.
     
	FD_ZERO(&interface_fds); // Initialize file descriptor set
	FD_SET(KEYBOARD, &interface_fds); // Adds standard input file descriptor (always 0) to file descriptor set

	while (1)
	{
		test_fds = interface_fds;
		
        pthread_mutex_lock(&command_mutex); // Grab the mutex to pause other threads while printing the interface
		printf("\nEnter Command (ping, reset, get, enable, disable, between [PARAM1] [PARAM2], outside [PARAM1] [PARAM2]) or press Ctrl-c to quit: ");
        fflush(stdout); // Flush the printf statement inside the standard output buffer in case it's stuck
		pthread_mutex_unlock(&command_mutex); // Interface is printed, now release the mutex
		
        
        
         // select() and pselect() allow a program to monitor multiple file descriptors, waiting 
        // until one or more of the file descriptors become "ready" for some class of I/O operation (e.g., input possible). 
         // A file descriptor is considered ready if it is possible to perform the corresponding I/O operation (e.g., read(2)) without blocking.
         
         //int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
         
        select(FD_SETSIZE, &test_fds, NULL, NULL, NULL); // Block and wait for activity from the file descriptor set

		for (fd = 0; fd < FD_SETSIZE; fd++)
		{
			if (FD_ISSET(fd, &test_fds)) // Check for activity
			{
				if (fd == KEYBOARD) // Did we sense activity from the keyboard?
				{
					pthread_mutex_lock(&command_mutex); // Grab mutex to access global data
					fgets(input, INPUT_SIZE, stdin);
					result = strtok(input, newline);

					if (!strcmp(ping, result))
						command = MSG_PING;

					else if (!strcmp(reset, result))
						command = MSG_RESET;

					else if (!strcmp(get, result))
						command = MSG_GET;

					else if (!strcmp(enable, result))
						command = MSG_INTENABLE;

					else if (!strcmp(disable, result))
						command = MSG_INTDISABLE;

					else if ((result = strtok(input, delims))) // Is the input between or outside?
					{
						if (!strcmp(between, result))
						{
							command = MSG_INTBETWEEN;
							if ((result = strtok(NULL, delims))) // Get the upper and lower boundaries
							{
								low_bound = (unsigned char)atoi(result);
								if ((result = strtok(NULL, newline)))
									high_bound = (unsigned char)atoi(result);
							}
						}
						else if (!strcmp(outside, result))
						{
							command = MSG_INTOUTSIDE;
							if ((result = strtok(NULL, delims))) // Get the upper and lower boundaries
							{
								low_bound = (unsigned char)atoi(result);
								if ((result = strtok(NULL, newline)))
									high_bound = (unsigned char)atoi(result);
							}
						}
						else
							command = MSG_NOTHING;
					}
					else
						command = MSG_NOTHING;

					pthread_mutex_unlock(&command_mutex); // Release the mutex
				}
			}
		}

		sleep(1); // Let the other threads work long enough so that any output is printed BEFORE the next interface print
	}

	return (void *)NULL;
*/
}

/*
sensor()
This is the function that will be executed by the sensor thread.
No arguments will be passed in as *ptr
*/
void *sensor(void *ptr)
{
/*
	// File pointer that represents the sensor device 
	FILE *sensor_device;
	// Strings to send to the sensor device for various operations 
	char STRING_RESET[]     = "reset";
	char STRING_PING[]      = "ping";
	char STRING_BETWEEN[20] = {0};  //We'll have to sprintf into these later.
	char STRING_OUTSIDE[20] = {0};
	char STRING_ENABLE[]    = "enable";
	char STRING_DISABLE[]   = "disable";
	// The value produced by the sensor's 8-bit ADC 
	unsigned char sensor_value[2] = {MSG_NOTHING, MSG_NOTHING};
	// Used to verify device driver read and writes 
	size_t ret; //Short for "return"
	int sensor_poll = 0;
	struct pollfd mypollfd;


	// Open the sensor with file operations. 
	// This uses our device driver from Lab 3. 


	// Main Loop 
	while (1)
	{

		sensor_device = fopen("/dev/pp_adc", "w");
		if ( !sensor_device )
		{
			fprintf(stderr, "sensor thread: fopen\n");
			exit(1);
		}

		// Hold onto command mutex until it's done processing commands, if any 
		pthread_mutex_lock(&command_mutex);

		if(command != MSG_NOTHING)
		{

			switch (command)
			{
			case MSG_RESET:
				ret = fwrite( STRING_RESET, sizeof(char), strlen(STRING_RESET), sensor_device);
				if (!ret) fprintf(stderr, "fwrite error\n");
				if (sensor_poll == 2) { sensor_poll = 1; }
				break;

			case MSG_PING:
				ret = fwrite( STRING_PING, sizeof(char), strlen(STRING_PING), sensor_device);
				if (!ret) fprintf(stderr, "fwrite error\n");
				break;

			case MSG_GET:
				// Read two bytes from the device 
				fclose(sensor_device);
				sensor_device = fopen("/dev/pp_adc", "r");
				if ( !sensor_device )
				{
					fprintf(stderr, "sensor thread: fopen\n");
					exit(1);
				}
				ret = fread( sensor_value, sizeof(unsigned char), 2, sensor_device );
				if (!ret) fprintf(stderr, "fread error\n");
				printf("Sensor Value: %d, sensor_value\n", (int)sensor_value[0]);
				fflush(stdout);
				break;

			case MSG_INTBETWEEN:
				//Format the command string 
				sprintf( STRING_BETWEEN, "between %d %d", low_bound, high_bound);
				// Write the command string to the device 
				ret = fwrite( STRING_BETWEEN, sizeof(char), strlen(STRING_BETWEEN), sensor_device);
				if (!ret) fprintf(stderr, "fwrite error\n");
				break;

			case MSG_INTOUTSIDE:
				// Format the command string 
				sprintf( STRING_OUTSIDE, "outside %d %d", low_bound, high_bound);
				// Write the command string to the device 
				ret = fwrite( STRING_OUTSIDE, sizeof(char), strlen(STRING_OUTSIDE), sensor_device);
				break;

			case MSG_INTENABLE:
				ret = fwrite( STRING_ENABLE, sizeof(char), strlen(STRING_ENABLE), sensor_device);
				if (!ret){ fprintf(stderr, "fwrite error\n"); }
				sensor_poll = 1;
				break;

			case MSG_INTDISABLE:
				ret = fwrite( STRING_DISABLE, sizeof(char), strlen(STRING_DISABLE), sensor_device);
				if (!ret){ fprintf(stderr, "fwrite error\n"); }
				sensor_poll = 0;
				break;

			default:
				// Unrecognized command type: this is bad 
				fprintf(stderr, "sensor thread: unrecognized command type\n");
				break;
			}

			// Clear the command because we have already serviced it. 
			command = MSG_NOTHING;

		}

		// Done servicing the command 
		pthread_mutex_unlock( &command_mutex );
		fclose(sensor_device);

		// Now fill out the Url_Data so the communication thread 
		// can send the info to Luo's server. 
		pthread_mutex_lock( &communication_mutex );  //Acquire rights to change the Url_Data.

		sensor_device = fopen("/dev/pp_adc", "r");
		if ( !sensor_device )
		{
			fprintf(stderr, "sensor thread: fopen\n");
			exit(1);
		}

		if (sensor_poll == 1)
		{
			mypollfd.fd=fileno(sensor_device);
			mypollfd.events=POLLRDBAND;
			mypollfd.revents=0;

			int result=poll(&mypollfd, 1, TIMEOUT);

			if(result){
				printf("\n\nInterrupt! Enter reset\n\n");
				fflush(stdout);
				sensor_poll = 2;
			}
		}

		ret = fread( sensor_value, sizeof(unsigned char), 2, sensor_device );
		if (!ret)
		{
			sprintf(Url_Data.status, "read failure");
		} else {
			sprintf(Url_Data.status, "up"); //TODO this might not be best
		}

		Url_Data.data = ( unsigned int ) sensor_value[0]; //Update the sensor value.
		pthread_mutex_unlock( &communication_mutex );  //Release the mutex.
		fclose(sensor_device);

		// Give up the CPU 
		usleep(0);
	}

	// For errors: 
	fprintf(stderr, "wut");
	exit(1);
*/
}

/*--------------------communicate()-------------------
Performs the communication with the server
------------------------------------------------------*/
void *communicate(void *ptr)
{
    
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
    
/*	
const char* hostname="cans.uml.edu";
	const int   port=8080;
	const int   id=2;
	const char* password="banana";
	const char* name="Shamrock";
	const int data=150;

	char buf[1024];

	while (1)
	{
		pthread_mutex_lock( &communication_mutex );

		// Format the URL string
		snprintf(buf, 1024, "http://%s:%d/update?id=%d&password=%s&name=%s&data=%u&status=%s",
			hostname,
			port,
			id,
			password,
			name,
			Url_Data.data,
			Url_Data.status);

		// Send the URL to the server
		HTTP_GET(buf);

		pthread_mutex_unlock( &communication_mutex );

		// Give up the CPU
		usleep(0);
	}

	return (void *) NULL;
*/
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


/*
size_t dummy(void *nothing1, size_t nothing2, size_t nothing3, void *nothing4)
{
	return 0;
}
*/
