#ifndef SIGNAL_H
#define SIGNAL_H

double autocorr_unit(double* arr, long lag, long n);
void autocorr(double* ip, double* op, long n);

#endif