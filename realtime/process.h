#ifndef PROCESS_H
#define PROCESS_H

#include "signal.h"
#define SAMPLINGRATE 48000

#ifdef DEBUG
#define FPRINTFD(x) fprintf x;
#else
#define FPRINTFD(x) ((void)0)
#endif

#include "pulse/simple.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"

int clean_exit(pa_simple* s);
int file_write(char* filename, uint32_t data[], size_t size);
void uint32p_to_doublep(uint32_t* in, double* out, size_t n);
double detect_freq(double* src, size_t n);

#endif