#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "inttypes.h"

#define STARTALLOC 15000
#define STEPALLOC 5000
#define INFILE "matlab/clip.csv"

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
    printf("%s\n", "enter");
    
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
            printf("realloc at %ld till %ld\n", count, n);
        }
    }
    
    fprintf(stdout, __FILE__" : readcsv() : %s %ld\n", "count is ", count);
    return count;
}

int main(int argc, char const *argv[])
{
    size_t initsize = sizeof(double) * STARTALLOC;
    fprintf(stdout, __FILE__" : main() : %s %ld\n", "initsize is ", initsize);

    double* arr = (double*)malloc(initsize);
    if (arr == NULL)
    {
        fprintf(stderr, __FILE__" : main() : %s\n", "malloc() is NULL");
        return 1;
    }

    size_t n = readcsv(INFILE, &arr, STARTALLOC, STEPALLOC);

    fprintf(stdout, __FILE__" : main() : %s %ld\n", "n is ", n);
    free(arr);
    return 0;
}