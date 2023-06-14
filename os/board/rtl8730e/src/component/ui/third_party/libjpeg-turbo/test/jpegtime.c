#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "jpeglib.h"

static int mytime(struct timespec *t)
{
	/* Do the timing using clock_gettime and the per-process timer. */
	if (!clock_gettime(CLOCK_PROCESS_CPUTIME_ID, t)) {
		return 1;
	}

	perror("CLOCK_PROCESS_CPUTIME_ID");
	fprintf(stderr, "timepng: could not get the time\n");
	return 0;
}

/*读JPEG文件相当于解压文件*/
static int decode_jpeg_file(FILE *input_file)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPARRAY buffer;
	int row_width;

	unsigned char *output_buffer;
	unsigned char *tmp = NULL;

	cinfo.err = jpeg_std_error(&jerr);

	// 1.Initialization of JPEG compression objects
	jpeg_create_decompress(&cinfo);

	/* 2.Specify data source for decompression */
	jpeg_stdio_src(&cinfo, input_file);

	/* 3.Read file header, set default decompression parameters */
	(void) jpeg_read_header(&cinfo, TRUE);

	/* 4.Start decompressor */
	(void) jpeg_start_decompress(&cinfo);

	row_width = cinfo.output_width * cinfo.output_components;

	/* 5.Make a one-row-high sample array that will go away when done with image  */
	buffer = (*cinfo.mem->alloc_sarray)
			 ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_width, 1);

	output_buffer = (unsigned char *)malloc(row_width * cinfo.output_height);
	memset(output_buffer, 0, row_width * cinfo.output_height);
	tmp = output_buffer;

	/* 6.Process data , start in leftTop, from top to bottom */
	while (cinfo.output_scanline < cinfo.output_height) {
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);

		memcpy(tmp, *buffer, row_width);
		tmp += row_width;
	}


	free(output_buffer);
	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	/* Close files, if we opened them */

	return 1;
}


static int jpeg_perform_test(char *name)
{
	struct timespec before, after;

	if (mytime(&before)) {
		{
			FILE *fp = fopen(name, "rb");
			if (NULL == fp) {
				return 0 ;
			}
			if (decode_jpeg_file(fp)) {
				if (ferror(fp)) {
					perror("temporary file");
					fprintf(stderr, "file %s: error reading jpg data\n", name);
					return 0;
				}
			} else {
				perror("temporary file");
				fprintf(stderr, "file %s: error from libjpg\n", name);
				return 0;
			}
			(void)fclose(fp);
		}
	} else {
		return 0;
	}

	if (mytime(&after)) {
		/* Work out the time difference and print it - this is the only output,
		 * so flush it immediately.
		 */
		unsigned long s = after.tv_sec - before.tv_sec;
		long ns = after.tv_nsec - before.tv_nsec;

		if (ns < 0) {
			--s;
			ns += 1000000000;

			if (ns < 0) {
				fprintf(stderr, "timepng: bad clock from kernel\n");
				return 0;
			}
		}

		printf("[%s]%lu.%.9ld(s)\n", name, s, ns);
		fflush(stdout);
		if (ferror(stdout)) {
			fprintf(stderr, "timepng: error writing output\n");
			return 0;
		}

		/* Successful return */
		return 1;
	}

	else {
		return 0;
	}
}
static int read_jpg_frommem(void *text, const unsigned int dataSize)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPARRAY buffer;
	int row_width;

	unsigned char *output_buffer;
	unsigned char *tmp = NULL;

	cinfo.err = jpeg_std_error(&jerr);

	// 1.Initialization of JPEG compression objects
	jpeg_create_decompress(&cinfo);

	/* 2.Specify data source for decompression */
	//jpeg_stdio_src(&cinfo, input_file);
	jpeg_mem_src(&cinfo, text, dataSize);

	/* 3.Read file header, set default decompression parameters */
	(void) jpeg_read_header(&cinfo, TRUE);

	/* 4.Start decompressor */
	(void) jpeg_start_decompress(&cinfo);

	row_width = cinfo.output_width * cinfo.output_components;

	/* 5.Make a one-row-high sample array that will go away when done with image  */
	buffer = (*cinfo.mem->alloc_sarray)
			 ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_width, 1);

	output_buffer = (unsigned char *)malloc(row_width * cinfo.output_height);
	memset(output_buffer, 0, row_width * cinfo.output_height);
	tmp = output_buffer;

	/* 6.Process data , start in leftTop, from top to bottom */
	while (cinfo.output_scanline < cinfo.output_height) {
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);

		memcpy(tmp, *buffer, row_width);
		tmp += row_width;
	}


	free(output_buffer);
	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	/* Close files, if we opened them */

	return 1;
}

static int perform_one_test(char *name)
{
	int i;
	struct timespec before, after;
	long size;
	char *text;

	FILE *fp = fopen(name, "rb");
	if (NULL == fp) {
		return 1;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	text = (char *)malloc(size + 1);
	rewind(fp);
	fread(text, sizeof(char), size, fp);
	text[size] = '\0';
	(void)fclose(fp);

	if (mytime(&before)) {
		if (read_jpg_frommem(text, size)) {
		} else {
			perror("temporary file");
			fprintf(stderr, "file %d: error from libjpeg\n", i);
			return 0;
		}
	} else {
		return 0;
	}

	if (mytime(&after)) {
		/* Work out the time difference and print it - this is the only output,
		 * so flush it immediately.
		 */
		unsigned long s = after.tv_sec - before.tv_sec;
		long ns = after.tv_nsec - before.tv_nsec;

		if (ns < 0) {
			--s;
			ns += 1000000000;

			if (ns < 0) {
				fprintf(stderr, "timepng: bad clock from kernel\n");
				return 0;
			}
		}

		printf("[%s]%lu.%.9ld(s)\n", name, s, ns);
		fflush(stdout);
		if (ferror(stdout)) {
			fprintf(stderr, "timepng: error writing output\n");
			return 0;
		}
		if (text) {
			free(text);
		}
		/* Successful return */
		return 1;
	}

	if (text) {
		free(text);
	}
	return 0;
}

int main(int argc, char *argv[])
{

	int i ;
	if (argc < 2) {
		printf("please input a valid file \n");
		return  0 ;
	}
	for (i = 1; i < argc; ++i) {
		jpeg_perform_test(argv[i]);
#if JPEG_LIB_VERSION >= 80 || defined(MEM_SRCDST_SUPPORTED)
		perform_one_test(argv[i]);
#endif
	}


	return 0;
}
