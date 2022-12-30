#include "vec_int.h"
#include <math.h>

float voltages[BIN_SIZE][MAX_SIZE];
int counter = 0;
int bin = 0;

__irq void TIMER_ISR(void){
	T0IR ^= 0x1;
	AD0CR |= (1 << 24) ;
}
__irq void UART_ISR(void){}

__irq void ADC_ISR(void){
	int result = (AD0DR1 >> 6) & 0x3FF;
	voltages[bin][counter++] = result/1023 * 3.3;

	if(counter*bin == MAX_SIZE){
		/*
		 'n' - Hanning Window
		 'm' - Hamming Window
		 'b' - Blackman Window
		 'r' - Rectangular Window
		 */
		CMPLX** freq_components = STFT(voltages, 'n');
		
		float magnitude[MAX_SIZE];
		for(int i=0, k=0; i<BIN_SIZE; i++){
			for(int j=0; j<BIN_SIZE; j++){
				CMPLX f_c = freq_components[i][j];
				magnitude[k++] = sqrt(pow(creal(f_c), 2) + pow(cimag(f_c), 2));
			}
		}
			

		counter = 0;
		bin = 0;
	}
	
	if(counter == 16){ 
		bin++; 
		counter = 0;
	}
	
	VICVectAddr = 0x00; // exiting the  
}
