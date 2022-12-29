#include <lpc214x.h>
#include <stdio.h>
#include "stft.h"
#include "vec_int.h"
#include <cmath>

void init_timer();
void init_adc();
void init_uart();
__irq void ADC_ISR(void);

extern float voltages[MAX_SIZE];
extern int counter;

int main(){
	init_timer();
	init_adc(); 
	
	counter = 0;
	
	VICIntEnable = 1 << 18;
	VICIntSelect = 0 << 18;
	VICVectCntl0 = (1 << 5) | 18;
	VICVectAddr0 = (unsigned) ADC_ISR;
	
	T0TCR = 0x1;
	AD0CR |= (1 << 24) ;
	
	while(!(T0IR & 0x01));
}

void init_timer(){
	T0TCR = 1 << 2; // resetting TC and PC
	T0CTCR = 0; // timer mode
	T0MCR = 0x7; // interrupt, reset and stop TC when it matches MR0
	T0PR = 50;
	T0MR0 = 5999999; // 5 sec delay
} 

void init_adc(){
	PINSEL1 = 1 << 24; // AD0.1
	AD0CR = (1 << 2) | (1 << 21); // ADC operational, 10-bit accuracy
	AD0INTEN = 1 << 2; // generate interrupt after conversion, ADC channel 1
}

void init_uart(){}
