#include "signal.h"
#include "omp.h"

double autocorr_unit(double* arr, long lag, long n)
{
    double sum = 0;

    for (long i = 0; i < n-1-lag; ++i)
        sum += arr[i] * arr[i+lag];

    return sum;
}


void autocorr(double* ip, double* op, long n)
{
    #pragma omp parallel for
    for (long i = 0; i < n-1; ++i)
        op[i] = autocorr_unit(ip, i, n);

}