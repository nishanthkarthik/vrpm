#include "process.h"

int clean_exit(pa_simple* s)
{
	if (s)
        pa_simple_free(s);
    return 0;
}

int file_write(char* filename, uint32_t data[], size_t size)
{
	#ifdef DEBUG
    printf("%s %zu\n", "size", size);
    #endif

    FILE* fp = NULL;
    if ((fp = fopen(filename, "ab")) == NULL)
        return 1;
    if (fwrite(data, sizeof(uint32_t), size, fp) != size)
        return 1;
    else
        fclose(fp);
    return 0;
}

void uint32p_to_doublep(uint32_t* in, double* out, size_t n)
{
	#pragma omp parallel for
	for (size_t i = 0; i < n; ++i)
		out[i] = in[i];
}

double detect_freq(double* inarr, size_t n)
{
	double* cliparr = (double*)malloc(sizeof(double)*n);
    if (cliparr == NULL)
    {
        fprintf(stderr, __FILE__" : main() : %s\n", "malloc() is NULL");
        return -1;
    }

    #ifdef FFT_YES
    autocorrft(inarr, cliparr, n);
    #else
    autocorr(inarr, cliparr, n);
    #endif

    FPRINTFD((stdout, __FILE__" : main() : %s\n", "autocorrelation done"));
    
    clip(cliparr, n);
    FPRINTFD((stdout, __FILE__" : main() : %s\n", "clipping done"));

    double* expandarr = (double*)malloc(sizeof(double)*(2*n+1));
    if (expandarr == NULL)
    {
        fprintf(stderr, __FILE__" : main() : %s\n", "malloc() is NULL");
        return -1;
    }

    expand2(cliparr, n, expandarr);
    FPRINTFD((stdout, __FILE__" : main() : %s\n", "expansion done"));

    subtract(cliparr, expandarr, n);
    FPRINTFD((stdout, __FILE__" : main() : %s\n", "array difference done"));
    
    clip(cliparr, n);
    FPRINTFD((stdout, __FILE__" : main() : %s\n", "clipping done"));
    
    #ifdef FFT_YES
    size_t i_max = maxidxft(cliparr, n);
    #else
    size_t i_max = maxidx(cliparr, n);
    #endif

    double f_fun = (double)SAMPLINGRATE / i_max;
    FPRINTFD((stdout, __FILE__" : main() : %s\n", "maxindex done"));
    fprintf(stdout, __FILE__" : main() : %s %lf Hz, %lf RPM\n", "fundamental frequency at", f_fun, 60*f_fun);

    free(expandarr);
    free(cliparr);

    return f_fun;
}