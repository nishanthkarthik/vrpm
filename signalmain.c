#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "inttypes.h"

#define STARTALLOC 15000
#define STEPALLOC 5000
#define INFILE "matlab/clip.csv"

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

    double* xarr = (double*)malloc(sizeof(double)*n);
    if (xarr == NULL)
    {
        fprintf(stderr, __FILE__" : main() : %s\n", "malloc() is NULL");
        return 1;
    }    
    autocorr(inarr, xarr, n);
    printf("%s\n", "done");
    free(inarr);
    free(xarr);
    return 0;
}