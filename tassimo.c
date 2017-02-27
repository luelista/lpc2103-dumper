#include "tassimo.h"

int cksum;


#define hexdigit(d) ( ((d)>0x9) ? ('A'-0xA+(d)) : ('0'+(d)) )
void puthex(unsigned char x) {
	putchar(hexdigit((x&0xF0) >>4));
	putchar(hexdigit(x& 0x0F));
	cksum += x;
}
void putchar(unsigned char x) {
	while(!(U0LSR & THRE)) ;
	U0THR = x;
}


int htoi(char s[])
{
    int hexdigit,i,inhex,n;
    i = 0;
    if( s[i] == '0')
    {
        ++i;
        if(s[i] == 'x' || s[i] == 'X')
            ++i;
    }
    
    n = 0;
    inhex = 1;

    for(;inhex==1;++i)
    {
        if(s[i] >='0' && s[i] <='9')
            hexdigit= s[i] - '0';
        else if(s[i] >='a' && s[i] <='f')
            hexdigit= s[i] -'a' + 10;
        else if(s[i] >='A' && s[i] <='F')
            hexdigit= s[i] -'A' + 10;
        else
            inhex = 0;
        
        if(inhex == 1)
            n = 16 * n + hexdigit;
    }
    return n;
}


