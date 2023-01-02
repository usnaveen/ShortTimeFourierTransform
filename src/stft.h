#ifndef __STFT__
#define __STFT__

#include <complex.h>
#include <stdlib.h>

#define MAX_SIZE 16

typedef complex float CMPLX;

typedef enum {
	RECTANGULAR, HANNING, HAMMING, BLACKMAN
}WINDOW;

CMPLX* STFT(CMPLX voltages[MAX_SIZE], WINDOW window_char);

#endif
