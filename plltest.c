
#include "tassimo.h"

void setupPLL() {
	//F_OSC=16MHz, CCLK=48MHz   -->  M=3
	//P=2 ->  FCCO = FOSC × M × 2 × P = 16*3*2*2 = 192MHz -> OK  
	// PSEL= b01, MSEL = 00010
	PLLCFG = 0b00100010;
	PLLCON = PLLEN; //enable pll
	applyPllRegisters();

	//wait for PLL to lock
	while(! ( PLLSTAT & PLOCK ) );

	// connect pll to cpu clock source
	PLLCON |= PLLC;
	applyPllRegisters();
}

MAINFUNCTION void main(void) {
	int ctr;
	ctr = 0x1000;
	while(ctr-- > 0); // let the bootloader finish...

	setupPLL();

	// to measure speed: toggle AD0.0 / P0.22 as fast as possible
	IO0DIR |= (1<<22);
	while(1) {
		IO0SET = (1<<22);
		__asm("nop");
		IO0SET = (1<<22);
		__asm("nop");
	}
}
