
#include "tassimo.h"

void blinky(int pos);
char readbuf[100];
int bufpos;
void _start(void) {
	int ctr,bit,pos,reg;
	char inchar;
	char* pars;
	bufpos=0;
	ctr = 0x1000;
	while(ctr-- > 0); // let the bootloader finish...

	MEMMAP = 1; // User flash mode

	U0FCR |= 0x01; // enable UART 0 fifo
	//U0LCR |= 0x3; // set 8 bit uart
	//U0LCR |= 0x8; // enable parity (odd parity)
	
	IO0DIR = 0;
	blinky(0x09);
	blinky(0x0d);
	//blinky(0x0e);
	blinky(0x11);
	blinky(0x1a);


	/*
	 
	     red1    red2

	 yellow      green1
	     
	       green2
	                 [ txd0  vcc ]
			 [ rxd0  rst ]
			 [ gnd  boot ]
	 
	0x09  - yellow  on/off
	0x0d / 0x0e - high: green leds brighter
	0x11  -  green2   on off
	0x1a  -  red2
	0x1e  -  switches all leds (?)


		jtag connector
	[ trst tms rtck rst ??? dbgsel ]
	[ vcc  tdi ???  td0 ??? gnd    ]
	*/

	while(1){
		if (U0LSR & RDR) {
			inchar = U0RBR;
			if (inchar=='\n') {
				pos=htoi(readbuf+1);
				puthex(pos);
				bit=(1<<pos);
				switch (readbuf[0]) {
				case 'i':
					IO0DIR &= ~bit;
					break;
				case 'o':
					IO0DIR |= bit;
					break;
				case 's':
					IO0SET |= bit;
					break;
				case 'c':
					IO0CLR |= bit;
					break;
				case 'b':
					blinky(pos);
					break;
				}
				putchar('k');putchar('\n');
				bufpos=0;
			} else {
				readbuf[bufpos++] = inchar;
			}
		}
	}

}
void blinky(int pos) {
	int r,ctr; r=5;
	int bit;
	bit=(1<<pos);
	putchar('\n');
	puthex(pos);
	putchar(' ');
	puthex(bit);
	putchar('.');putchar('\n');

	while(r-- > 0) {
		IO0DIR |= bit;
		ctr=0x20000; while(ctr-- > 0);
		IO0SET |= bit;
		ctr=0x20000; while(ctr-- > 0);
		IO0CLR |= bit;
		ctr=0x20000; while(ctr-- > 0);
		IO0DIR &= ~bit;
		ctr=0x20000; while(ctr-- > 0);
	}

}

