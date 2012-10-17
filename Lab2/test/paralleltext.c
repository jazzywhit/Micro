#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <stdio.h>
#include <unistd.h>

#define pport 0x378

int main(int argc, char *argv[]){

	//int parportfd = open("/dev/parport0", 0_RDWR);	
	//if (result != 0) result = ioctl(parportfd,PPCLAIM);
	if (argc < 2){
		printf("Enter a two digit hex value (0F, FF, 01, etc..)\n");
		return 1;
	}
	char hexChar;
	hexChar = strtol(argv[1], NULL, 16);
	//hex_value[1] = strtol(argv[1], NULL, 16)*16;
	printf("You entered: %X\n", hexChar);
	//return 1;
	if(ioperm(pport,3,1)) return(1);
	int i = 0;
	for(;i < 10000; i++){
		char a, b, c; 

		outb(0x01,pport);
		outb(0x00,pport+2); //Strobe
		a = inb(pport);	
		b = inb(pport+1);	
		c = inb(pport+2);	
		printf("%X\t%X\t%X\n",a, b, c);
		//printf("%X\t\n",a);
		usleep(1000000);
	 	
	}

	//result = ioctl(parportfd,PPRELEASE);
	//close(parportfd);
}
