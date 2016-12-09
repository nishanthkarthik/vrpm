#include "signal.h"
#include "omp.h"
#include "stdio.h"

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
    for (size_t i = 0; i < n-1; ++i)
        op[i] = autocorr_unit(ip, i, n);
}

size_t readcsv(char* filename, double** arr, size_t n, size_t step)
{
    FILE* infile;
    infile = fopen(filename, "r");
    if (infile == NULL)
    {
        fprintf(stderr, __FILE__" : readcsv() : %s\n", "fopen() is NULL");
        return 1;
    }

    size_t count = 0;
    double temp = 0;
    
    while (fscanf(infile, "%lf", &temp) == 1)
    {
        (*arr)[count] = temp;
        // printf("%ld -> %lf\n", count, (*arr)[count]);
        ++count;

        if (count == n-5)
        {
            *arr = (double*)realloc(*arr, sizeof(double) * (n+step));
            if (arr == NULL)
            {
                fprintf(stderr, __FILE__" : readcsv() : %s\n", "realloc() is NULL");
                return 0;
            }
            n += step;
            // printf("realloc at %ld till %ld\n", count, n);
        }
    }
    
    fprintf(stdout, __FILE__" : readcsv() : %s %ld\n", "count is ", count);
    return count;
}