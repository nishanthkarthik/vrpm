#include "signal.h"
#include "omp.h"
#include "stdio.h"
#include "fftw3.h"

double autocorr_unit(double* arr, long lag, size_t n)
{
    double sum = 0;

    for (size_t i = 0; i < n-1-lag; ++i)
        sum += arr[i] * arr[i+lag];

    return sum;
}

void autocorr(double* ip, double* op, size_t n)
{
    #pragma omp parallel for
    for (size_t i = 0; i < n-1; ++i)
        op[i] = autocorr_unit(ip, i, n);
}

void autocorrft(double* ip, double* op, size_t n)
{
    fftw_plan fwdplan;
    fftw_complex* fwd = malloc(n*sizeof(fftw_complex));
    fwdplan = fftw_plan_dft_r2c_1d(n, ip, fwd, FFTW_ESTIMATE);
    fftw_execute(fwdplan);

    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
    {
        fwd[i][0] = fwd[i][0]*fwd[i][0] + fwd[i][1]*fwd[i][1];
        fwd[i][1] = 0;
    }

    fftw_plan invplan;
    invplan = fftw_plan_dft_c2r_1d(n, fwd, op, FFTW_ESTIMATE);
    fftw_execute(invplan);

    fftw_destroy_plan(fwdplan);
    fftw_destroy_plan(invplan);
    fftw_free(fwd);
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
                fclose(infile);
                return 0;
            }
            n += step;
            // printf("realloc at %ld till %ld\n", count, n);
        }
    }
    
    fprintf(stdout, __FILE__" : readcsv() : %s %ld\n", "count is ", count);
    fclose(infile);
    return count;
}

void clip(double* arr, size_t n)
{
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
    {
        if (arr[i] < 0)
            arr[i] = 0.0;
    }
}

void expand2(double* in, size_t n, double* out)
{
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
        out[2*i] = in[i];

    #pragma omp parallel for
    for (size_t i = 0; i < n-1; ++i)
        out[2*i + 1] = (in[i] + in[i+1])/2.0;
}

void subtract(double* in, double* out, size_t n)
{
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
        in[i] = in[i] - out[i];
}

int writecsv(char* filename, double* in, size_t n)
{
    FILE* f;
    f = fopen(filename, "w");
    if (f == NULL)
    {
        fprintf(stderr, __FILE__" : writecsv() : %s\n", "fopen() is NULL");
        return 1;
    }
    for (size_t i = 0; i < n; ++i)
    {
        fprintf(f, "%lf\n", in[i]);
    }
    fclose(f);
    return 0;
}

size_t maxidx(double* arr, size_t n)
{
    size_t maxidx = 0;
    double maxval = 0;

    for (size_t i = 0; i < n; ++i)
    {
        if (arr[i] > maxval)
        {
            maxidx = i;
            maxval = arr[i];
        }
    }

    return maxidx;
}

size_t maxidxft(double* arr, size_t n)
{
    size_t maxidx = 0;
    double maxval = 0;

    for (size_t i = LOWLIMRPM; i < MIN(n,HIGHLIMRPM); ++i)
    {
        if (arr[i] > maxval)
        {
            maxidx = i;
            maxval = arr[i];
        }
    }

    return maxidx;
}