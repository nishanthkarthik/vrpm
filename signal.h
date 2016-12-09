#ifndef SIGNAL_H
#define SIGNAL_H

#include "stdlib.h"

double autocorr_unit(double* arr, long lag, long n);
void autocorr(double* ip, double* op, long n);
size_t readcsv(char* filename, double** arr, size_t n, size_t step);

#endif