#include "stft.h"
#include <stdlib.h>
#include <cmath>

#undef I
#define J _Imaginary_I
#define K 1000
#define PI 3.14159

const CMPLX Wn[16] = { 
	1000, 924-383*J, 707-707*J, 383-924*J, -1000*J, -383-924*J, -707-707*J, -924-383*J, -1000, 
	-924 + 383*J, -707+707*J, -383+924*J, 1000*J, 383+924*J, 707+707*J, 924+383*J
};

const int hanning_values[MAX_SIZE] = {
	1000, 990, 962, 916, 854, 778, 691, 598, 500, 402, 308, 222, 146, 84, 38, 10
};

const int hamming_values[MAX_SIZE] = {
	1000, 991, 965, 922, 865, 796, 716, 630, 540, 450, 364, 284, 215, 158, 115, 89
};

const int blackman_values[MAX_SIZE] = {
	1000, 984, 939, 866, 774, 667, 555, 444, 340, 249, 172, 112, 66, 35, 15, 4
};

void hanning_win(CMPLX voltages[MAX_SIZE]){
	for(int i=0; i<MAX_SIZE; i++){
		voltages[i] = voltages[i]*hanning_values[i];
	}
}

void hamming_win(CMPLX voltages[MAX_SIZE]){
	for(int i=0; i<MAX_SIZE; i++){
		voltages[i] = voltages[i]*hamming_values[i];
	}
}

void rectangular_win(CMPLX voltages[MAX_SIZE]){ return; }

void blackman_win(CMPLX voltages[MAX_SIZE]){
	for(int i=0; i<MAX_SIZE; i++){
		voltages[i] = voltages[i]*blackman_values[i];
	}
}

CMPLX* FFT(CMPLX* input_sequence, size_t size){
	if(size != 1){ 
		int n = size/2;
		CMPLX even_split[n], odd_split[n];
		
		for(int i=0; i<n; i++){
			even_split[i] = input_sequence[i*2];
			odd_split[i] = input_sequence[i*2+1];
		}
		
		CMPLX* even_complex = FFT(even_split, n);
		CMPLX* odd_complex = FFT(odd_split, n);
		
		int d = 1;	
		for(int i=0; i<n; i++){
			if(size != 2){ d = K; }
			input_sequence[i] = (even_complex[i]*K + Wn[i*MAX_SIZE/size]*odd_complex[i])/d;
			input_sequence[i+n] = (even_complex[i]*K - Wn[i*MAX_SIZE/size]*odd_complex[i])/d;
		}
	}
	
	return input_sequence;	
}

CMPLX* STFT(CMPLX voltages[MAX_SIZE], WINDOW window_char){
	void (*window)(CMPLX[MAX_SIZE]);
	
	switch(window_char){
		case HANNING:
			window = hanning_win;
			break;
		case HAMMING:
			window = hamming_win;
			break;
		case BLACKMAN:
			window = blackman_win;
			break;
		case RECTANGULAR:
			window = rectangular_win;
			break;
	}

	window(voltages);
	FFT(voltages, MAX_SIZE);
	
	return voltages;
}
