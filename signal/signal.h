#ifndef SIGNAL_H
#define SIGNAL_H

#include "stdlib.h"

double autocorr_unit(double* arr, long lag, size_t n);
void autocorr(double* ip, double* op, size_t n);
size_t readcsv(char* filename, double** arr, size_t n, size_t step);
void clip(double* arr, size_t n);
void expand2(double* in, size_t n, double* out);
void subtract(double* in, double* out, size_t n);
int writecsv(char* filename, double* in, size_t n);
size_t maxidx(double* arr, size_t n);

#endif