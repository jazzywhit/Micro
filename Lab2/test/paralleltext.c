#include <sys/io.h>
#include <stdio.h>
#include <unistd.h>

int main(){

	if(ioperm(0x378,3,1)) return(1);
	int i = 0;
	for(;i < 1; i++){
		char a, b, c; 

		outb(0x01,0x378); //Send out a recognizable signature
		outb(0x01,0x378+2); //Strobe
		a = inb(0x378);	
		b = inb(0x378+1);	
		c = inb(0x378+2);	
		printf("%X\t%X\t%X\n",a, b, c);
		usleep(1000000); //One second pause
	 	
	}
}
