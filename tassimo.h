
#define MEMMAP (*(unsigned char*)0xe01Fc040)

// receive
#define U0RBR (*(unsigned char*)0xe000c000)

// send
#define U0THR (*(unsigned char*)0xe000c000)

// FIFO Control Register (0x01 fifo enable, 0x02 rx rst, 0x04 tx rst)
#define U0FCR (*(unsigned char*)0xe000c008)
// UART0 line control register
#define U0LCR (*(unsigned char*)0xe000c00C)
// UART0 line status register
#define U0LSR (*(unsigned char*)0xe000c014)
// Transmit Holding Register Empty
#define THRE (1<<5)
#define RDR (1<<0)

#define IO0SET (*(unsigned int*)0xe0028004)
#define IO0DIR (*(unsigned int*)0xe0028008)
#define IO0CLR (*(unsigned int*)0xe002800c)

#define LED_GREEN_1 (1<<0x11)
#define LED_YELLOW (1<<9)
#define LED_RED_2 (1<<0x1a)
#define LED_RED (1<<18)


void puthex(unsigned char x);
void putchar(unsigned char x);

int htoi(char s[]);

extern int cksum;

