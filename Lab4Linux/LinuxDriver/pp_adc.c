/*
 *File: ppadc.c
 *Purpose: ppadc driver c file
 *Last Date Modified: 12/07/12
 */

//Includes

#include "ppadc.h"	   

#include <linux/module.h>	// Basic header for LKMs

#include <linux/mm.h>		// kmalloc

#include <linux/string.h>	// strcpy

#include <linux/list.h>		// linked list

#include <linux/sched.h>	// schedule()

#include <linux/ktime.h>	// ktime struct/methods

#include <linux/delay.h>	// mdelay/udelay

#include <linux/uaccess.h>	// copy_from_user, copy_to_user

#include <linux/cdev.h>		// Character device stuff 

#include <linux/poll.h>		// Constants for "poll" callback

#include <linux/parport.h>	// parport structures


//Defines 

#define delay mdelay(WAIT_TIME);

//Licencing

MODULE_LICENSE("GPL"); 

MODULE_DESCRIPTION("Some Desc");

// Module entry/exit points

static int __init     init(void); //Module Entry Point

static void __exit    exit(void); //Module Exit Point

// Callbacks for kernel to "attach" and "detach" driver to/from each parallel port

module_init(init); //Entry call back

module_exit(exit); //Exit call back
 
//Driver Function Declarations

LIST_HEAD(pp_adc_list); // Linked list of pp_adc structs to represent each parport on the machine (eg: pp0adc, pp1adc)

static void           pp_attach(struct parport *port);		//ppadc Register

static void           pp_detach(struct parport *port);		//ppadc Unregister

static struct pp_adc *create_pp_adc(struct parport *port);	//ppadc Create

static void           destroy_pp_adc(struct pp_adc *p);		//ppadc Destroy

static void           pp_adc_irq(void *handle);			//ppadc Interrupt Request

static int            hook_open(struct inode *inodep, struct file *filp); //ppadc Claim

static ssize_t        hook_write(struct file *filep, const char __user *buffer, size_t size, loff_t *offset); //ppadc Service Command

static ssize_t        hook_read(struct file *filp, char __user *buffer, size_t size, loff_t *offset); //ppadc Return Data

static int            hook_close(struct inode *inodep, struct file *filp); //ppadc Release Claim

static unsigned int   hook_poll(struct file *filp, poll_table *wait); //ppadc Poll file

//Command Service Function Declaration

static int            cmd_eval   (struct pp_adc *p, const char *cmdline);	//ppadc Evaulate Command

static int            cmd_reset  (struct pp_adc *p, int params[2]);		//ppadc Reset Command 

static int            cmd_ping   (struct pp_adc *p, int params[2]);		//ppadc Ping Command

static int            cmd_enable (struct pp_adc *p, int params[2]);		//ppadc Enable Command

static int            cmd_disable(struct pp_adc *p, int params[2]);		//ppadc Disable Command

static int            cmd_between(struct pp_adc *p, int params[2]);		//ppadc Between Command

static int            cmd_outside(struct pp_adc *p, int params[2]);		//ppadc Outside Command

static int            cmd_get    (struct pp_adc *p, int params[2]);		//ppadc Get Command


//Structures

static struct file_operations pp_adc_fops=			//Lists user space avaiable functions on file ppadc 
{ 
	.owner=THIS_MODULE,
	.open=hook_open,
	.read=hook_read,
	.write=hook_write,
	.poll=hook_poll,
	.release=hook_close,
};

struct pp_adc
{
	struct list_head list;         				// Points to previous and next pp_adc structs in linked list
	struct pardevice *pardev;      				// pardevice struct as provided from the kernel (see linux/parport.h)
	char name[STRING_BUFFER_SIZE];				// Name given to parport adc device (generally parport#_adc)
	int claimed;                   				// Boolean indicating that this pp_adc currently has control of the port
	volatile int irq_counter;      				// Increments every time an interrupt arrives on a claimed port
	wait_queue_head_t irq_wq;      				// "Woken up" upon every interrupt for deferred processing
	dev_t device_number;           				// Device Major/Minor number, as shown in /proc/devices
	struct cdev *cdev;             				// Character device structure to create a file node (typically in /dev)
};

static struct parport_driver pp_driver=
{
	.name=DRIVER_NAME,
	.attach=pp_attach,
	.detach=pp_detach,
};

struct command
{
	const char *cmdstr;					//Command String
	int (*cmdfunc)(struct pp_adc *p, int params[2]);	//Associated Command Function
};


static struct command commands[]=
{				//List of commands available to user space 
	{CMD_RESET,      cmd_reset},
	{CMD_PING,       cmd_ping},
	{CMD_INTENABLE,  cmd_enable},
	{CMD_INTDISABLE, cmd_disable},
	{CMD_BETWEEN,    cmd_between},
	{CMD_OUTSIDE,    cmd_outside},
	{CMD_GET,        cmd_get},
	{0,0},
};

// Function Definitions 

/* init()
 * Purpose:    Called when kernel module is loaded. Registers the ppadc with the kernel.
 * Parameters: None
 * Returns:    0 for success
 */

static int __init init(void)
{
	printk(KERN_INFO "\n\n\n\n\n\n\n\n\n\n\n\n");		

	printk(KERN_INFO "%s: loading parport module\n", DRIVER_NAME);

	parport_register_driver(&pp_driver); //Registers ppadc

	printk(KERN_INFO "%s: module parport loaded\n", DRIVER_NAME);

	return 0;
}

/* exit()
 * Purpose:    Called upon the request to remove the module. Unregisters ppadc.
 * Parameters: None
 * Returns:    None
 */

static void __exit exit(void)
{
	printk(KERN_INFO "%s: unloading module\n", DRIVER_NAME);

	parport_unregister_driver(&pp_driver); //Unregisters ppadc

	printk(KERN_INFO "%s: module unloaded\n", DRIVER_NAME);
}

/* create_pp_adc
 * Purpose:    kmallocs a pp_adc structure and initializes its members
 * Parameters: Pointer to a "parport struct" that will be abstracted by the new pp_adc
 * Returns:    Pointer to newly created pp_adc struct on success or NULL on failure
 */

static struct pp_adc *create_pp_adc(struct parport *port)
{
	struct pp_adc *p=kmalloc(sizeof(struct pp_adc), GFP_KERNEL);	//Allocate memory
	
	//Check for memory allocation
	if(!p)
		return NULL; 						//Could not allocate memory

	strncpy(p->name, port->name, STRING_BUFFER_SIZE); 		//Store (port->name)+"_adc" in the name field of the pp_adc
	strncat(p->name, "_adc", STRING_BUFFER_SIZE);

	//Create and register parport_device structure
	p->pardev=parport_register_device 				
	(
		port,	
		p->name, 	// name
		NULL, 		// int (*preempt)(void *)
		NULL, 		// void (*wakeup)(void *)
		pp_adc_irq, 	// void (*irq)(void *)
		0, 		// Flags
		(void *)p 	// Handle
	);

	//Check parport_device structure registration
	if(!p->pardev)							
	{
		kfree(p); 	//Unsuccessful parport_device registration
		return NULL;
	}

	//Initialize data members
	INIT_LIST_HEAD(&p->list);
	p->claimed=0;
	p->irq_counter=0;
	init_waitqueue_head(&p->irq_wq);

	// Create/Register character device
	alloc_chrdev_region(&p->device_number, 0, 1, p->name);
	p->cdev=cdev_alloc();
	cdev_init(p->cdev, &pp_adc_fops);
	cdev_add(p->cdev, p->device_number, 1);

	printk(KERN_INFO "%s: Attached to %s.\n", DRIVER_NAME, port->name);
	printk(KERN_INFO "%s: %s(Major=%d, Minor=%d)", DRIVER_NAME, p->name, MAJOR(p->device_number), MINOR(p->device_number));

	return p;
}

/* destroy_pp_adc
 * Purpose:    Undoes the actions of create_pp_adc
 * Parameters: Pointer to pp_adc structure to be destroyed
 * Returns:    None
 */

static void destroy_pp_adc(struct pp_adc *p)
{
	//Check if device exists
	if(!p) // Do nothing if p==NULL
		return;

	// Unregister the character device
	cdev_del(p->cdev);
	unregister_chrdev_region(p->device_number, 1);

	//Unclaim Port
	if(p->claimed)		
	{
		p->claimed=0;
		parport_release(p->pardev);
	}

	parport_unregister_device(p->pardev);

	kfree(p); //Unclaim Port

	printk(KERN_INFO "%s: Detached from %s\n", DRIVER_NAME, p->pardev->port->name);
}

/* pp_attach
 * Purpose:    Callback function for kernel when registering each parallel port to this driver
 * Parameters: Pointer to parport struct to be attached to
 * Returns:    None
 */

static void pp_attach(struct parport *port)
{
	// Create pp_adc objects and store them in the array
	struct pp_adc *p;

	if((p=create_pp_adc(port)))
		list_add(&p->list, &pp_adc_list);
}

/* pp_detach
 * Purpose:    Callback function for kernel when kernel wants us to unregister ports
 * Parameters: Pointer to parport structure each pardevice was registered with
 * Returns:    None
 */

static void pp_detach(struct parport *port)
{
	struct pp_adc *p;

	// Search for port in linked list and delete it when found
	list_for_each_entry(p, &pp_adc_list, list)
	{
		if(port==p->pardev->port)
		{
			list_del(&p->list);
			destroy_pp_adc(p);
			break;
		}
	}
}


char getDay(int intDay)
{
    
	char day ;
    
	switch(intDay)
	{
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
			day = 'Z';
			break;
	}
    
	return day;
}

//------------------------------- DisplayData --------------------------------------------
//Purpose: Diaplay the ADC and RTC data retreived from the PIC.

void DisplayData(unsigned char *RTCData, unsigned char *adcResult){

    char day = getDay(RTCData[3]);
	
	printk(KERN_INFO "\nIn display data\n");
    printk(KERN_INFO "\nADC result: %u" , *adcResult);
    
	printk(KERN_INFO "Time: %02u:%02u:%02u %c %02u/%02u/20%02u",
           RTCData[2], // Hour
           RTCData[1], // Minutes
           RTCData[0], // Seconds
           day,
           RTCData[5], // Month
           RTCData[4], // Date
           RTCData[6] ); // Year
	
}


char ADCandRTCData[STRING_BUFFER_SIZE];
//------------------------------- formatData --------------------------------------------
//Purpose: Formats the ADC and RTC data retreived from the PIC.
static int formatResultString(unsigned char *RTCData, unsigned char *adcResult){
	

	int numberOfCharactersCopied = 0;
	char day = getDay(RTCData[3]);
	
	printk(KERN_INFO "\nIn format result string\n");

	// Copy get results to global string buffer so it can be later passed to user space.
	numberOfCharactersCopied = snprintf(ADCandRTCData, STRING_BUFFER_SIZE,
										"\nADCValue: %02u Time: %02u:%02u:%02u %c %02u/%02u/20%02u\n\n",
										*adcResult,
										RTCData[2], // Hour
										RTCData[1], // Minutes
										RTCData[0], // Seconds
										day,
										RTCData[5], // Month
										RTCData[4], // Date
										RTCData[6]);  // Year

	return numberOfCharactersCopied;
}

/* writePPNibble
 * Purpose:    Writes a nibble synchronously to the parallel port
 * Parameters: Pointer to a struct pp_adc representing the data to be written to the port
 * Returns:    None
 */

static void writePPNibble(struct pp_adc *p, unsigned char d)
{
	d&=NIBBLE_MASK; //  Mask out unnecessary bits

											// STROBE IS HIGH (START CONDITION)

	delay;

	parport_frob_control(p->pardev->port, IMPED_STROB_MASK, IMPED_LO|STROB_LO);	//  STROBE LOW / OUTPUT

	delay;

	parport_write_data(p->pardev->port, d);						//  OUTPUT DATA

	delay;
	
	parport_frob_control(p->pardev->port, IMPED_STROB_MASK, IMPED_LO|STROB_HI);	//  STROBE HIGH / OUTPUT

	delay;

	parport_frob_control(p->pardev->port, IMPED_STROB_MASK, IMPED_LO|STROB_LO);	//  STROBE LOW / OUTPUT

	delay;

	parport_frob_control(p->pardev->port, IMPED_STROB_MASK, IMPED_LO|STROB_HI);	// STROBE HIGH / OUTPUT



											// STROBE IS HIGH (END CONDITION)
}

/* readPPNibble
 * Purpose:    Reads a nibble synchronously to the parallel port
 * Parameters: Pointer to a struct pp_adc representing the port
 * Returns:    Data read from port (with NIBBLE_MASK applied)
 */

static unsigned char readPPNibble(struct pp_adc *p)
{
	unsigned char d;

												// STROBE IS HIGH (START CONDITION)

	delay;

	parport_frob_control(p->pardev->port, IMPED_STROB_MASK, IMPED_HI|STROB_LO);		// STROBE LOW / INPUT

	delay;

	parport_frob_control(p->pardev->port, IMPED_STROB_MASK, IMPED_HI|STROB_HI);		// STROBE HIGH / INPUT

	delay; 

	d=parport_read_data(p->pardev->port);							// READ DATA

	delay;

	parport_frob_control(p->pardev->port, IMPED_STROB_MASK, IMPED_HI|STROB_LO);		// STROBE LOW / OUTPUT

	delay;

	parport_frob_control(p->pardev->port, IMPED_STROB_MASK, IMPED_HI|STROB_HI);		// STROBE HIGH / OUTPUT


												// STROBE IS HIGH (END CONDITION)

	d&=NIBBLE_MASK; //  Mask out unnecessary bits

	return d;
}

/* writePP3Nibble
 * Purpose:     Writes the lowest 12-bits (3 nibbles) of an integer synchronously to the parallel port with big-endian ordering
 * Parameters:  Pointer to a struct pp_adc representing the port integer data to be written
 * Return: 	None
 */

static void writePP3Nibble(struct pp_adc *p, int n){
	unsigned char data;
	unsigned char ackResult;

	printk(KERN_INFO "\n---------------In 3 nibble");

	// Lower Nibble (0-3)
	data = (n & 0xF);
	writePPNibble(p, data);
	printk(KERN_INFO "\nData 1 : %X", data);

	ackResult=readPPNibble(p);
	printk(KERN_INFO "\nAckData 1 : %X", ackResult);

	// Upper Nibble (4-7)
	data = (n & 0xF0)>>4;
	writePPNibble(p, data);
	printk(KERN_INFO "\nData 2 : %X", data);

	ackResult=readPPNibble(p);
	printk(KERN_INFO "\nAckData 2 : %X", ackResult);

	// Most Significant Nibble (8-12)
	data = (n & 0xF00)>>8;
	writePPNibble(p, data);
	printk(KERN_INFO "\nData 3 : %X", data);

	ackResult=readPPNibble(p);
	printk(KERN_INFO "\nAckData 3 : %X", ackResult);
}

/* pp_adc_irq
 * Purpose:     Called every time an interrupt occurs on a claimed port. The coresponding pp_adc's irq_counter is incremented and it's 			
 *              irq_wq wait queue is woken up for defered processing.
 * Parameters:  Port's coresponding struct pp_adc cast to a (void*)
 * Return:	None
 */

static void pp_adc_irq(void *handle)
{
        struct pp_adc *p=(struct pp_adc *)(handle);

        printk(KERN_INFO "%s: %s IRQ\n", DRIVER_NAME, p->name);

        p->irq_counter++; //Increment interrupt request counter

        wake_up(&p->irq_wq);
}

/* cmd_eval
 * Purpose:    Parses a command string and searches for the coresponding command in the "commands" array
 * Parameters: Pointer to a claimed pp_adc struct to operate on entire unparsed command line string
 * Returns:    ENODEV if device is no longer attached,
               ENOSYS if command is not recognized,
               Result if command is successful
 */

static int cmd_eval(struct pp_adc *p, const char *cmdline)
{
	char cmdstr[STRING_BUFFER_SIZE];
	int params[2];
	int i;

    	printk(KERN_INFO "\nCommand Evaluation\n");

	sscanf(cmdline, "%s %d %d", cmdstr, &params[0], &params[1]); //Parse the command string for a command and two integer arguments

	// Search through the commands array
	for(i=0; commands[i].cmdstr; i++)
		if(strcmp(cmdstr, commands[i].cmdstr)==0) //Command found, return its appropriate function.
			return commands[i].cmdfunc(p, params);
	
	//Command not found.
	printk(KERN_INFO "%s: Unknown command: %s\n", DRIVER_NAME, cmdstr);
	return -ENOSYS;
}

/* cmd_reset
 * Purpose:    Sends a reset command to device and resets the pp_adc's irq_counter to 0
 * Parameters: Pointer to a pp_adc struct
               Array of two integer parameters
               params[0]: Unused
               params[1]: Unused
 * Returns:    0 upon success, -ENODEV if device failed to acknowledge the command
 */

/* cmd_ping
 * Purpose:    Sends a ping command to the device
 * Parameters: Pointer to a pp_adc struct
               Array of two integer parameters
               params[0]: Unused
               params[1]: Unused
 * Returns:    0 upon success, -ENODEV if device failed to acknowledge the command
 */

static int cmd_ping(struct pp_adc *p, int params[2])
{
	unsigned char ackResult;

	printk(KERN_INFO "\nPing Command\n");

	//Write Ping Command to PIC.
  	writePPNibble(p,MSG_PING); 

	//Acknowledge Ping Command was received.
	ackResult = readPPNibble(p);

	if( ackResult != MSG_ACK_PING )
	{
		parport_write_data(p->pardev->port,0x0);
		printk(KERN_INFO "\nACK RESULT : %X", ackResult);
    		printk(KERN_INFO "\nStatus: Ping Command Failed");
		return -1;
    	}
	
	//Ping Command Successful
	printk(KERN_INFO "\nACK RESULT : %X", ackResult);
	printk(KERN_INFO "\nStatus: Ping Command Successful");

	return 0;

}

/* cmd_enable
 * Purpose:    Instructs the device to begin testing for the interrupt condition
 * Parameters: Pointer to a pp_adc struct
               Array of two integer parameters
               params[0]: Unused
               params[1]: Unused
 * Returns:    0 upon success, -ENODEV if device failed to acknowledge the command
 */

static int cmd_enable(struct pp_adc *p, int params[2])
{
	unsigned char ackResult;

	printk(KERN_INFO "\nInterrupt Enable Command\n");

  	//Write Interrupt Enable Command to PIC.
  	writePPNibble(p,MSG_INTENABLE); 
	
	//Acknowledge Interrupt Enable Command was received.
	ackResult=readPPNibble(p);

	if( ackResult != MSG_ACK_INTENABLE ){
		parport_write_data(p->pardev->port,0x0);
		printk(KERN_INFO "\nACK RESULT : %X", ackResult);
    		printk(KERN_INFO "\nStatus: Interrupt Enable Command Failed");
		return -1;
    	}
	
	//Interrupt Enable Command Successful
	printk(KERN_INFO "\nACK RESULT : %X", ackResult);
	printk(KERN_INFO "\nStatus: Interrupt Enable Command Success");

	return 0;
}

/* cmd_disable
 * Purpose:    Instructs the device to stop testing for the interrupt condition 
 * Parameters: Pointer to a pp_adc struct
               Array of two integer parameters
               params[0]: Unused
               params[1]: Unused
 * Returns:    0 upon success, -ENODEV if device failed to acknowledge the command
 */

static int cmd_disable(struct pp_adc *p, int params[2])
{
	unsigned char ackResult;

	printk(KERN_INFO "\nInterrupt Disable Command\n");

  	//Write Interrupt Disable Command to PIC.
  	writePPNibble(p,MSG_INTDISABLE); 
	
	//Acknowledge Interrupt Disable Command was received.
	ackResult=readPPNibble(p);

	if( ackResult != MSG_ACK_INTDISABLE ){
		parport_write_data(p->pardev->port,0x0);
		printk(KERN_INFO "\nACK RESULT : %X", ackResult);
    		printk(KERN_INFO "\nStatus: Interrupt Disable Command Failed");
		return -1;
    	}

	//Interrupt Disable Command Successful
	printk(KERN_INFO "\nACK RESULT : %X", ackResult);
	printk(KERN_INFO "\nStatus: Interrupt Disable Command Success");

	return 0;
}

/* cmd_between
 * Purpose:    Sets the bounds on the device to interrupt if ( lo_bound <= ADC <= hi_bound )
 *             Does NOT enable interrupts!
 * Parameters: Pointer to a pp_adc struct
               Array of two integer parameters
               params[0]: Lower Bound
               params[1]: Upper Bound
 * Returns:    0 upon success, -ENODEV if device failed to acknowledge the command
 */

static int cmd_between(struct pp_adc *p, int params[2])
{
	unsigned char ackResult;

	printk(KERN_INFO "\nBetween Command\n");

  	//Write Between Command to PIC.
  	writePPNibble(p,MSG_INTBETWEEN); 
	
	//Acknowledge Between Command was received.
	ackResult=readPPNibble(p);

	if( ackResult != MSG_ACK_INTBETWEEN ){
		parport_write_data(p->pardev->port,0x0);
		printk(KERN_INFO "\nACK RESULT : %X", ackResult);
    		printk(KERN_INFO "\nStatus: Between Command Failed");
		return -1;
    	}

	//Write between parameters to device
	writePP3Nibble(p, params[0]); //Write first between command parameter.
	writePP3Nibble(p, params[1]); //Write second between command parameter.

	//Between Command Successful
	printk(KERN_INFO "\nACK RESULT : %X", ackResult);
	printk(KERN_INFO "\nStatus: Between Command Success");

	return 0;
}

/* cmd_outside
 * Purpose:    Sets the bounds on the device to interrupt if ( ADC < lo_bound  OR  hi_bound < ADC )
 *             Does NOT enable interrupts!
 * Parameters: Pointer to a pp_adc struct
               Array of two integer parameters
               params[0]: Lower Bound
               params[1]: Upper Bound
 * Returns:    0 upon success, -ENODEV if device failed to acknowledge the command
 */

static int cmd_outside(struct pp_adc *p, int params[2])
{
	unsigned char ackResult;

	printk(KERN_INFO "\nOutside Command\n");

  	//Write Outside Command to PIC.
  	writePPNibble(p,MSG_INTOUTSIDE); 
	
	//Acknowledge Outside Command was received.
	ackResult=readPPNibble(p);

	if( ackResult != MSG_ACK_INTOUTSIDE ){
		parport_write_data(p->pardev->port,0x0);
		printk(KERN_INFO "\nACK RESULT : %X", ackResult);
    		printk(KERN_INFO "\nStatus: Outside Command Failed");
		return -1;
    	}

	//Write between parameters to device
	writePP3Nibble(p, params[0]); //Write first between command parameter.
	writePP3Nibble(p, params[1]); //Write second between command parameter.

	//Outside Command Successful
	printk(KERN_INFO "\nACK RESULT : %X", ackResult);
	printk(KERN_INFO "\nStatus: Outside Command Success");

	return 0;
}

/* ReadByte
 * Purpose:    Read an entire byte by breaking it up into 4-bit parts.
 * Parameters: Pointer to a pp_adc struct
 * Returns:    byte read from device
 */

static unsigned char ReadByte(struct pp_adc *p){
 
  unsigned char byte;
  
  byte = (readPPNibble(p)<<4) & 0xF0; //Reconstruct the two 4bit messages into a byte

  byte |= readPPNibble(p);
  
  return byte;  
}



/* cmd_get
 * Purpose:    Request's the device's ADC value and resets the pp_adc's irq_counter to 0
 * Parameters: Pointer to a pp_adc struct
 *             Array of two integer parameters
 *              params[0]: Unused
 *              params[1]: Unused
 * Returns:    0-1023 upon success, -ENODEV if device failed to acknowledge the command
 */
static int cmd_get(struct pp_adc *p, int params[2]){

	unsigned char ackResult;
	unsigned char adcResult;
	unsigned char RTCData[8];
	int i ;
	
	printk(KERN_INFO "In Get\n");
	
	//Write GET Command to PIC.
	writePPNibble(p, MSG_GET);
	
	//Read the ACK from the GET on the data line.
	ackResult=readPPNibble(p);
	
	if(ackResult != MSG_ACK_GET) {
		printk(KERN_INFO "\nStatus: Get command failed");
		return 0 ;
	}
	
	//Read 3 ADC Nibles and assemble them
	adcResult = ( readPPNibble(p) << 8 ) & 0x0300; // bit 8 and 9.
	adcResult |= ReadByte(p); // ReadByte reads low Byte and
	
	
	// Read RTC data for all 8 Bytes of information.
	
	for(i = 0 ; i < 8 ; i++) {
		RTCData[i] = ReadByte(p) ;
	}
	
	//Read the ACK from the GET on the data line.
	ackResult = readPPNibble(p);
	
	if( ackResult != MSG_ACK_GET) {
		printk(KERN_INFO "\nStatus: Get command failed");
		return 0 ;
	}
	
	// Display Data
	DisplayData(RTCData, &adcResult);
	
	// Copy result to global buffer
	i = formatResultString(RTCData, &adcResult); // we can test i if needed
	
	//Reset the IRQ Counter
	p->irq_counter = 0;
	
	return 1;
	
}


///////////////////////////////////////////////////////
// Character device file operations

/* hook_open
 * Purpose: Attempts to claim the parallel port. Then pings the ADC device.
 *          Stores a pointer to the associated pp_adc in the user pointer of
 *          the "struct file".
 * Returns: -EBUSY if the port is already in use
 *          -ENODEV if the device does not respond to pings
 *          0 if the port was successfully claimed and the device responded
 */
static int hook_open(struct inode *inodep, struct file *filp){
	struct pp_adc *p;
	int i;
    
    printk(KERN_INFO "In Hook Open\n" );

	// Search for the pp_adc in pp_adc_list that this file represents
	list_for_each_entry(p, &pp_adc_list, list)
		if(p->device_number==inodep->i_rdev)
			break;
	
	// Store a pointer in filp->private_data for later use in hook_read/write/close
	filp->private_data=(void *)p;

	// If we already claimed the port, the file is being opened in multiple places. 
	if(p->claimed)
		return -EBUSY;

	// Try to claim the port. If we cannot, some other driver is using the port
	if(parport_claim(p->pardev))
		return -EBUSY;

	// If we made it here, then the port has been claimed!
	p->claimed=1;
	printk(KERN_INFO "%s: %s claimed\n", DRIVER_NAME, p->name);

	// Try to communicate with the device
	for(i=0; i<4; i++)
		if(cmd_ping(p, NULL)>=0)
			return 0; // Device responded

	// Device did not respond
	parport_release(p->pardev);
	p->claimed=0;
	printk(KERN_INFO "%s: %s released\n", DRIVER_NAME, p->name);
	return -ENODEV;
}

/* hook_close
 * Purpose: Releases the previously claimed parallel port
 * Returns: 0: Success
 */
static int hook_close(struct inode *inodep, struct file *filp){
	struct pp_adc *p=(struct pp_adc *)filp->private_data;
	// If something is calling "close", then they were able to "open" successfully
	printk(KERN_INFO "In Hook Close\n" );
	
    parport_release(p->pardev);
	p->claimed=0;
	printk(KERN_INFO "%s: %s released\n", DRIVER_NAME, p->name);

	return 0;
}

/* hook_read
 * Purpose: Called when a user-space application tries to read from the character device.
 *          This method responds with the value of the device's ADC (in decimal)
 * Returns: Number of bytes sent back to user-space (0 if device failed to respond)
 */
static ssize_t hook_read(struct file *filp, char __user *buffer, size_t size, loff_t *offset){
	//struct pp_adc *p=(struct pp_adc *)filp->private_data;
	//int value=0;
	//int count;
	
	if(*offset!=0)
        return 0;
	// read ADC value from sensor
	
	// copy data to user space
	
	return size;

}

/* hook_write
 * Purpose: Called when a user-space application writes to the character device.
 *          This method will attempt to parse commands written to the device.
 * Returns: Number of bytes read from user-space (Always the number sent down)
 */
static ssize_t hook_write(struct file *filp, const char __user *buffer, size_t size, loff_t *offset){
	struct pp_adc *p=(struct pp_adc *)filp->private_data;
	char kbuf[STRING_BUFFER_SIZE+1]={0};
    
	printk(KERN_INFO "In Hook Write\n" );
	
	// get commands from user space
	copy_from_user(kbuf, buffer, MIN(STRING_BUFFER_SIZE, size));
	printk(KERN_INFO "Size of string buffer: %d", STRING_BUFFER_SIZE);

	//if(parport_claim(p->pardev))
	//	return -EBUSY;
	p->claimed=1;
	printk(KERN_INFO "%s: %s claimed\n",DRIVER_NAME, p->name);
	
	// pass commands to the sensor
	cmd_eval(p, kbuf);

	printk(KERN_INFO "Size: %d", size);
	copy_to_user((void *)buffer, (void*)ADCandRTCData, sizeof(ADCandRTCData));
	
	printk(KERN_INFO "BUFFER: %s",buffer);
	
	//parport_release(p->pardev);
	p->claimed=0;

	return size;
}

/* hook_poll
 * Purpose: Called when a user-space application attempts to monitor a file using the "poll" call
 *          This method will add the user-space poll_table to a wait queue which is woken up every
 *          interrupt. The userspace "poll" call will block until the wait queue is woken up or times out.
 * Returns: (POLLIN | POLLRDNORM) to indicate that this character device is always readable
 *          If the pp_adc's irq_counter is non-zero (one or more interrupts have occured since the last reset or get)
 *           then POLLRDBAND is also OR'ed in to indicate an event. (The irq_counter member must be reset to zero eventually
 *           for this method to be useful for another interrupt)
 */
static unsigned int hook_poll(struct file *filp, poll_table *wait){
    struct pp_adc *p=(struct pp_adc *) filp->private_data;
    printk(KERN_INFO "In Hook Poll\n" );
    
    poll_wait(filp, &p->irq_wq, wait);
    return (POLLIN | POLLRDNORM) | (p->irq_counter ? POLLRDBAND : 0);
}

static int cmd_reset(struct pp_adc *p, int params[2]){
	
	unsigned char ackResult;
	
	printk(KERN_INFO "\nReset Command\n");
	
	//Write Reset Command to PIC.
	writePPNibble(p,MSG_RESET);
	
	//Acknowledge Reset Command was received.
	ackResult=readPPNibble(p);
	
	if( ackResult != MSG_ACK_RESET )
	{
		parport_write_data(p->pardev->port,0x0);
		printk(KERN_INFO "\nACK RESULT : %X", ackResult);
		printk(KERN_INFO "\nStatus: Reset Command failed");
		return -1;
	}
	
	//Reset Command Successful
	printk(KERN_INFO "\nACK RESULT : %X", ackResult);
	printk(KERN_INFO "\nStatus: Reset Command Successful");
	parport_write_data(p->pardev->port,0x0);
	
	p->irq_counter = 0;
	
	return 0;
}



