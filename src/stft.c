#include "stft.h"
#include <stdlib.h>
#include <complex.h>
#include <cmath>

#define PI 3.14159

float* hanning_win(float* voltages, size_t size){
	for(int i=0; i<size; i++){
		voltages[i] = voltages[i]*(0.5 + 0.5*cos(PI*i/size));
	}
	return voltages;
}

float* hamming_win(float* voltages, size_t size){
	for(int i=0; i<size; i++){
		voltages[i] = voltages[i]*(0.54 + 0.46*cos(PI*i/size));
	}
	return voltages;
}

float* rectangular_win(float* voltages, size_t size){
	return voltages;
}

float* blackman_win(float* voltages, size_t size){
	for(int i=0; i<size; i++){
		voltages[i] = voltages[i]*(0.42 + .5*cos(PI*i/size) + 0.08*cos(2*PI*i/size));
	}
	return voltages;
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
		
		for(int i=0; i<n; i++){
			CMPLX w = cexp(-2*I*PI*i/size);
			input_sequence[i] = even_complex[i] + w*odd_complex[i];
			input_sequence[i+n] = even_complex[i] - w*odd_complex[i];
		}
	}
	
	return input_sequence;	
}

CMPLX** STFT(float voltages[BIN_SIZE][MAX_SIZE], char window_char){
	float* (*window)(float*, size_t);
	CMPLX** freq_components = malloc(sizeof(CMPLX)*BIN_SIZE*MAX_SIZE);
	
	switch(window_char){
		case 'n':
			window = hanning_win;
		case 'm':
			window = hamming_win;
		case 'b':
			window = blackman_win;
		case 'r':
			window = rectangular_win;
	}
	
	for(int i=0; i<BIN_SIZE; i++){
		CMPLX* windowed_voltages = (CMPLX*)window(voltages[i], BIN_SIZE);
		freq_components[i] = FFT(windowed_voltages, BIN_SIZE);
	}
	
	return freq_components;
}
