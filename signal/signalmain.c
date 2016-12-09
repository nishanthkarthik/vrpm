#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "inttypes.h"

#define STARTALLOC 15000
#define STEPALLOC 5000
#define INFILE "../matlab/clip.csv"
#define OUTFILE "../matlab/clipc.csv"

int main(int argc, char const *argv[])
{
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

    autocorr(inarr, cliparr, n);
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
    fprintf(stdout, __FILE__" : main() : %s\n", "clipping done");

    free(expandarr);
    free(cliparr);
    free(inarr);
    return 0;
}