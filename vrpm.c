#define APPNAME "vRPM"
#define STRMNAME "vRPM Listener"
#define BFRSIZE 1
#define OUTFILE "record.raw"

#include "stdio.h"
#include "errno.h"
#include "pulse/simple.h"
#include "pulse/error.h"

int clean_exit(pa_simple* s)
{
    if (s)
        pa_simple_free(s);
    return 0;
}

int file_write(char* filename, uint32_t data[], size_t size)
{
    printf("%s %zu\n", "size", size);
    FILE* fp = NULL;
    if ((fp = fopen(filename, "a")) == NULL)
        return 1;
    if (fwrite(data, sizeof(uint32_t), size, fp) != size)
        return 1;
    else
        fclose(fp);
    return 0;
}

int main(int argc, char const *argv[])
{
    int err;
    uint32_t buffer[BFRSIZE];
    char outfile[] = OUTFILE;
    long counter = 0;

    pa_simple* s = NULL;
    const pa_sample_spec samspec = {
        .format = PA_SAMPLE_S32LE,
        .rate = 48000,
        .channels = 2
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

        if (file_write(outfile, buffer, BFRSIZE))
        {
            fprintf(stderr, __FILE__" : file_write() : %s\n", "write error");
            clean_exit(s);
            return 1;
        }

        printf("%s %ld\n", "block", ++counter);
    }

    return 0;
}