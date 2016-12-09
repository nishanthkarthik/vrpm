#define INFILE "engine.raw"
#define OUTFILE "engine.csv"
#define BFRLEN 10000

#include "stdint.h"
#include "stdio.h"
#include "inttypes.h"

int clean_exit(FILE* inf, FILE* outf)
{
	if (inf != NULL)
		fclose(inf);
	if (outf != NULL)
		fclose(outf);
	return 1;
}

int write_csv(FILE* out, uint32_t buf[], size_t len)
{
	for (int i = 0; i < len; ++i)
	{
		if (fprintf(out, "%" PRIi32 "\n", buf[i]) < 0)
			return 1;
	}
	return 0;
}

int main(int argc, char const *argv[])
{
	FILE* infile = NULL;
	FILE* outfile = NULL;
	uint32_t buffer[BFRLEN];

	if ((infile = fopen(INFILE, "rb")) == NULL)
	{
		fprintf(stderr, __FILE__" : fopen() : %s\n", "error opening " INFILE);
		return clean_exit(infile, outfile);
	}

	if ((outfile = fopen(OUTFILE, "w")) == NULL)
	{
		fprintf(stderr, __FILE__" : fopen() : %s\n", "error opening " OUTFILE);
		return clean_exit(infile, outfile);
	}

	int size = 0;
	long counter = 0;
	while((size = fread(buffer, sizeof(uint32_t), BFRLEN, infile)) != 0)
	{
		counter += size;
		write_csv(outfile, buffer, size);
		printf("counter at %ld with size %d\n", counter, size);
	}
	printf("counter %ld\n", counter);
	clean_exit(infile, outfile);
	return 0;
}