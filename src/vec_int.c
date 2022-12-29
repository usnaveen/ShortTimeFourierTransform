#include "vec_int.h"
#include <math.h>
extern float* voltages;
extern int counter;

__irq void ADC_ISR(void){
	int result = (AD0DR1 >> 6) & 0x3FF;
	voltages[counter++] = result/1023 * 3.3;
	
	if(counter == MAX_SIZE){
		/*
		 'n' - Hanning Window
		 'm' - Hamming Window
		 'b' - Blackman Window
		 'r' - Rectangular Window
		 */
		CMPLX* freq_components = STFT(voltages, 'n');
		
		float magnitude[MAX_SIZE];
		for(int i=0; i<MAX_SIZE; i++){
			magnitude[i] = sqrt(pow(creal(freq_components[i]), 2) + pow(cimag(freq_components[i]), 2));
		}
		// UART
		
		//
		counter = 0;
	}
}
