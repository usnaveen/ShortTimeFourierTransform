#include <lpc214x.h>
#include "vec_int.h"

#define VPB_CLK_FREQ 15e6

int main(){
	// Fosc = 12MHz
	// CCLK = 60MHz
	// PCLK = 15MHz 
	// Sampling time period = ms
	// ADC clock frequency ~ 4.5MHz
	// Algorithm Computation time ~ 3ms
	// Baud rate = bps
	
	PLL0CFG = 0x24  ; // CCLK = 60MHz, CCO = 240MHz
	PLL0CON = 0x1;    // turn on PLL 
	PLL0FEED = 0xAA;  // feed sequence for changes to take effect
	PLL0FEED = 0x55;
	
	while(!(PLL0STAT & (1 << 10)));
	
	PLL0CON |= 1 << 1; // connect PLL to processor
	PLL0FEED = 0xAA;   // feed sequence for changes to take effect
	PLL0FEED = 0x55;
	
	VPBDIV = 0; // VPB = CCLK/4 = 15MHz;

	T0TCR = 1 << 2; // reset TC and PC
	T0CTCR = 0; 	// timer mode
	T0MCR = 0x3; 	// interrupt and reset when it matches MR0
	T0PR = 0;
	//T0MR0 = 15e6 - 1; // 5 sec delay
	
	PINSEL1 = 1 << 24; // AD0.1
	int CLKDIV = VPB_CLK_FREQ/(4.5*1e6) - 1; // ADC clock frequency ~ 4.5MHz
	AD0CR = (1 << 2) | (CLKDIV << 8) | (1 << 21); // ADC operational,  10-bit accuracy
	AD0INTEN = 1 << 2; // generate interrupt after conversion, ADC channel 1
	
	VICIntEnable = (1 << 18) | (1 << 4); // ADC0, TIMER0
	VICIntSelect = (0 << 18) | (0 << 4); // IRQ
	
	VICVectCntl0 = (1 << 5) | 4;  // TIMER0
	VICVectCntl1 = (1 << 5) | 18; // ADC0
	
	VICVectAddr0 = (unsigned) TIMER_ISR;
	VICVectAddr1 = (unsigned) ADC_ISR;
	
	T0TCR = 0x1;
	
	while(1);
	
	/*
	CMPLX arr[16];
	for(int i=0; i<16; i++){ arr[i] = i; }
	IO0DIR = 1;
	IO0SET = 1;
	CMPLX* freq_components = STFT(arr, BLACKMAN);
	IO0CLR = 1;
	float bbb;
	for(int i=0; i<16; i++){
		CMPLX f_c = arr[i];
		bbb = creal(f_c);
		bbb = cimag(f_c);
	}
	*/
}
