#include "vec_int.h"
#include <math.h>
#include <stdlib.h>

CMPLX voltages[MAX_SIZE];
int counter = 0;

__irq void TIMER_ISR(void){
	T0IR ^= 0x1;
	AD0CR |= (1 << 24) ; // begin conversion
	VICVectAddr = 0x00;
}

__irq void ADC_ISR(void){
	int result = (AD0DR1 >> 6) & 0x3FF;
	voltages[counter++] = result/1023 * 3.3;

	if(counter == MAX_SIZE){  
		CMPLX* freq_components = STFT(voltages, HANNING);

		counter = 0;
	}

	VICVectAddr = 0x00; // exiting the ISR
	
}
