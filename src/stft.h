#ifndef __STFT__
#define __STFT__

#include <complex.h>

#define MAX_SIZE 128
#define BIN_SIZE 16

typedef float complex CMPLX;

CMPLX** STFT(float voltages[BIN_SIZE][MAX_SIZE], char window_char);

#endif
