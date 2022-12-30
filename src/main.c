#include <lpc214x.h>
#include "vec_int.h"

#define VPB_CLK_FREQ 15e6
#define SYS_CLK_FREQ 60e6

int main(){
	// Fosc = 12MHz
	// CCLK = 60MHz
	// PCLK = 15MHz
	// Sampling time period = 3ms
	// ADC clock frequency = 4.5MHz
	// Baud rate = bps
	
	PLL0CFG = 0x24  ; // CCLK = 60MHz, CCO = 240MHz
	PLL0CON = 0x1; // turn on PLL 
	PLL0FEED = 0xAA; // feed sequence for changes to take effect
	PLL0FEED = 0x55;
	
	while(!(PLL0STAT & (1 << 10)));
	
	PLL0CON |= 1 << 2;
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	
	VPBDIV = 0; // VPB = CCLK/4 = 15MHz;
	
	T0TCR = 1 << 2; // reset TC and PC
	T0CTCR = 0; // timer mode
	T0MCR = 0x7; // interrupt, reset and stop TC when it matches MR0
	T0PR = 0;
	//T0MR0 = 15e6 - 1; // 5 sec delay
	
	PINSEL1 = 1 << 24; // AD0.1
	int CLKDIV = VPB_CLK_FREQ/(4.5*10e6) - 1;
	AD0CR = (1 << 2) | (CLKDIV << 8) | (1 << 21); // ADC operational,  10-bit accuracy
	AD0INTEN = 1 << 2; // generate interrupt after conversion, ADC channel 1
	
	VICIntEnable = (1 << 18) | (1 << 4) | (1 << 6); // ADC0, TIMER0, UART0
	VICIntSelect = (0 << 18) | (0 << 4) | (0 << 6); // IRQ
	
	VICVectCntl0 = (1 << 5) | 4;
	VICVectCntl1 = (1 << 5) | 18;
	VICVectCntl2 = (1 << 5) | 6;
	
	VICVectAddr0 = (unsigned) TIMER_ISR;
	VICVectAddr1 = (unsigned) ADC_ISR;
	VICVectAddr2 = (unsigned) UART_ISR;
	
	T0TCR = 0x1;
	
	while(1);
}
