// #define DEBUG
// #define ENABLE_FILE_OUTPUT
#define ENABLE_REALTIME
#define ENABLE_FFT

#define APPNAME "vRPM"
#define STRMNAME "vRPM Listener"
#define BFRSIZE 48000
#define OUTFILE "record.raw"

#include "signal.h"
#include "process.h"
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "pulse/simple.h"
#include "pulse/error.h"
#include "inttypes.h"

void print_setup()
{
    #ifdef DEBUG
    fprintf(stdout, __FILE__" : print_setup() : %s\n", "debug on");
    #endif
    #ifdef ENABLE_FILE_OUTPUT
    fprintf(stdout, __FILE__" : print_setup() : %s\n", "audio file writing on");
    #endif
    #ifdef ENABLE_REALTIME
    fprintf(stdout, __FILE__" : print_setup() : %s\n", "realtime frequency detection on");
    #endif
    #ifdef ENABLE_FFT
    fprintf(stdout, __FILE__" : print_setup() : %s\n", "fft autocorrelation on");
    #endif
}

int main(int argc, char const *argv[])
{
    print_setup();

    uint32_t buffer[BFRSIZE];
    double bufferd[BFRSIZE];

    int err;
    size_t n = BFRSIZE;

    #ifdef ENABLE_REALTIME
    double f_freq = 0;
    #endif
    
    #ifdef ENABLE_FILE_OUTPUT
    char outfile[] = OUTFILE;
    #endif

    #ifdef DEBUG
    long counter = 0;
    #endif

    pa_simple* s = NULL;
    const pa_sample_spec samspec = {
        .format = PA_SAMPLE_S32LE,
        .rate = SAMPLINGRATE,
        .channels = 1
    };
    
    s = pa_simple_new(NULL, APPNAME, PA_STREAM_RECORD, NULL, STRMNAME, &samspec, NULL, NULL, &err);

    if (s == NULL)
    {
        fprintf(stderr, __FILE__" : pa_simple_new() : %s\n", pa_strerror(err));
        clean_exit(s);
        return 1;
    }

    while(1)
    {
        if (pa_simple_read(s, &buffer, sizeof(buffer), &err) < 0)
        {
            fprintf(stderr, __FILE__" : pa_simple_read() : %s\n", pa_strerror(err));
            clean_exit(s);
            return 1;
        }

        #ifdef ENABLE_REALTIME
        uint32p_to_doublep(buffer, bufferd, n);
        f_freq = detect_freq(bufferd, n);
        fprintf(stdout, __FILE__" : main() : %s %lf Hz, %lf RPM\n", "fundamental frequency at", f_freq, 60*f_freq);
        #endif

        #ifdef ENABLE_FILE_OUTPUT
        if (file_write(outfile, buffer, BFRSIZE))
        {
            fprintf(stderr, __FILE__" : file_write() : %s\n", "write error");
            clean_exit(s);
            return 1;
        }
        #endif

        FPRINTFD((stdout, __FILE__" : main() : block %ld\n", ++counter));
    }

    return 0;
}