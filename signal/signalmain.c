#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "inttypes.h"
#include "omp.h"

#define FFT_YES

#define STARTALLOC 15000
#define STEPALLOC 5000
#define SAMPRATE 48000
// #define INFILE "../matlab/clip.csv"
// #define OUTFILE "../matlab/clipc.csv"
#define INFILE "../audio/ignition.csv"
#define OUTFILE "../audio/ignitionc.csv"

int main(int argc, char const *argv[])
{
    // omp_set_num_threads(4);

    double* inarr = (double*)malloc(sizeof(double) * STARTALLOC);
    if (inarr == NULL)
    {
        fprintf(stderr, __FILE__" : main() : %s\n", "malloc() is NULL");
        return 1;
    }

    size_t n = readcsv(INFILE, &inarr, STARTALLOC, STEPALLOC);
    fprintf(stdout, __FILE__" : main() : %s %ld\n", "n is ", n);
    fprintf(stdout, __FILE__" : main() : %s\n", "parsing done");

    double* cliparr = (double*)malloc(sizeof(double)*n);
    if (cliparr == NULL)
    {
        fprintf(stderr, __FILE__" : main() : %s\n", "malloc() is NULL");
        return 1;
    }    

    #ifdef FFT_YES
    autocorrft(inarr, cliparr, n);
    #else
    autocorr(inarr, cliparr, n);
    #endif

    fprintf(stdout, __FILE__" : main() : %s\n", "autocorrelation done");
    
    clip(cliparr, n);
    fprintf(stdout, __FILE__" : main() : %s\n", "clipping done");

    double* expandarr = (double*)malloc(sizeof(double)*(2*n+1));
    if (expandarr == NULL)
    {
        fprintf(stderr, __FILE__" : main() : %s\n", "malloc() is NULL");
        return 1;
    }

    expand2(cliparr, n, expandarr);
    fprintf(stdout, __FILE__" : main() : %s\n", "expansion done");

    subtract(cliparr, expandarr, n);
    fprintf(stdout, __FILE__" : main() : %s\n", "array difference done");
    
    clip(cliparr, n);
    fprintf(stdout, __FILE__" : main() : %s\n", "clipping done");

    writecsv(OUTFILE,cliparr, n);
    fprintf(stdout, __FILE__" : main() : %s\n", "writecsv done");
    
    #ifdef FFT_YES
    size_t i_max = maxidxft(cliparr, n);
    #else
    size_t i_max = maxidx(cliparr, n);
    #endif

    double f_fun = (double)SAMPRATE / i_max;
    fprintf(stdout, __FILE__" : main() : %s\n", "maxindex done");
    fprintf(stdout, __FILE__" : main() : %s %lf Hz, %lf RPM\n", "fundamental frequency at", f_fun, 60*f_fun);

    free(expandarr);
    free(cliparr);
    free(inarr);
    return 0;
}