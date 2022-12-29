#ifndef __STFT__
#define __STFT__

#include <complex.h>

#define MAX_SIZE 128

typedef float complex CMPLX;

CMPLX* STFT(float voltages[MAX_SIZE], char window);

#endif
