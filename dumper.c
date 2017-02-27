
#define MEMMAP (*(unsigned char*)0xe01Fc040)
#define U0THR (*(unsigned char*)0xe000c000)
// FIFO Control Register (0x01 fifo enable, 0x02 rx rst, 0x04 tx rst)
#define U0FCR (*(unsigned char*)0xe000c008)
// UART0 line control register
#define U0LCR (*(unsigned char*)0xe000c00C)
// UART0 line status register
#define U0LSR (*(unsigned char*)0xe000c014)
// Transmit Holding Register Empty
#define THRE (1<<5)


#define IO0SET (*(unsigned int*)0xe0028004)
#define IO0DIR (*(unsigned int*)0xe0028008)
#define IO0CLR (*(unsigned int*)0xe002800c)

#define LED_GREEN_1 (1<<17)
#define LED1 (1<<19)
#define LED2 (1<<18)
#define LED_RED (1<<18)

void puthex(unsigned char x);
void putchar(unsigned char x);
void ledoff();
void ledon();
int cksum;
void _start(void) {
	int ctr;
	ctr = 0x1000;
	while(ctr-- > 0); // let the bootloader finish...

	MEMMAP = 1; // User flash mode

	U0FCR |= 0x01; // enable UART 0 fifo
	//U0LCR |= 0x3; // set 8 bit uart
	//U0LCR |= 0x8; // enable parity (odd parity)
	
	IO0DIR = LED1 | LED2; // set led1 and led2 as output
	unsigned char *ptr = (unsigned char*)0x00000000;
	unsigned char *endptr = (unsigned char*)0x00008000;
	while(1){
		ctr = 0x100;
		ledon();
		while(ctr-- > 0) {
			putchar('-');
		}
		putchar('\n');
		ledoff();
		ctr=0x50000;
		while(ctr-- > 0) ;
		IO0SET=LED1;
		ptr = (unsigned char*)0x00000000;
		putchar('\n');
		cksum=0;
		putchar(':');puthex(0x02);puthex(0x00);puthex(0x00);puthex(0x04);
		puthex(((unsigned int)ptr>>24)&0xff);
		puthex(((unsigned int)ptr>>16)&0xff);
		while(ptr < endptr) {
			if(((unsigned int)ptr&0x0F) == 0){
				puthex(0x100-(cksum&0xff));
				putchar('\n');
				cksum=0;
				putchar(':');puthex(0x10);
				puthex(((unsigned int)ptr>>8)&0xff);
				puthex((unsigned int)ptr&0xff);
				puthex(0x00);
			}
			puthex(*ptr);
			ptr++;
			ctr++;
			if (ctr==0x100)ledon();
			if (ctr==0x200){ledoff();ctr=0;}
		}
		puthex(0x100-(cksum&0xff));
		putchar('\n');
		putchar('\n');
		IO0SET=LED2;
		putchar('e');putchar('n');putchar('d');putchar('\n');
		IO0CLR=LED1;
		ctr=0x2999999;
		while (ctr-- > 0);

	}

}

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
void ledoff(){
	IO0CLR = LED1 | LED2;
}
void ledon(){
	IO0SET = LED1 | LED2;
}


