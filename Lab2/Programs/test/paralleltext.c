#include <sys/io.h>
#include <stdio.h>
int main(){

char a; 

 if(ioperm(0x378,3,1)) return(1);      

 outb(0x01,0x378);

 a = inb(0x378+1);

 return(0);
}
