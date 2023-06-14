/* timepng.c
 *
 * Copyright (c) 2013,2016 John Cunningham Bowler
 *
 * Last changed in libpng 1.6.22 [May 26, 2016]
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 *
 * Load an arbitrary number of PNG files (from the command line, or, if there
 * are no arguments on the command line, from stdin) then run a time test by
 * reading each file by row or by image (possibly with transforms in the latter
 * case).  The only output is a time as a floating point number of seconds with
 * 9 decimal digits.
 */
#define _POSIX_C_SOURCE 199309L /* for clock_gettime */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include <time.h>

#if defined(HAVE_CONFIG_H) && !defined(PNG_NO_CONFIG_H)
#  include <config.h>
#endif

/* Define the following to use this test against your installed libpng, rather
 * than the one being built here:
 */
#ifdef PNG_FREESTANDING_TESTS
#  include <png.h>
#else
#  include "../interfaces/include/png.h"
#endif

/* The following is to support direct compilation of this file as C++ */
#ifdef __cplusplus
#  define voidcast(type, value) static_cast<type>(value)
#else
#  define voidcast(type, value) (value)
#endif /* __cplusplus */

/* 'CLOCK_PROCESS_CPUTIME_ID' is one of the clock timers for clock_gettime.  It
 * need not be supported even when clock_gettime is available.  It returns the
 * 'CPU' time the process has consumed.  'CPU' time is assumed to include time
 * when the CPU is actually blocked by a pending cache fill but not time
 * waiting for page faults.  The attempt is to get a measure of the actual time
 * the implementation takes to read a PNG ignoring the potentially very large IO
 * overhead.
 */
typedef struct {
	unsigned char *data;
	int size;
	int offset;
} ImageSource;

static PNG_CALLBACK(void, no_warnings, (png_structp png_ptr, png_const_charp warning))
{
	(void)png_ptr;
	(void)warning;
}
static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
	ImageSource *isource = (ImageSource *)png_get_io_ptr(png_ptr);

	if (isource->offset + length <= isource->size) {
		memcpy(data, isource->data + isource->offset, length);
		isource->offset += length;
	} else {
		png_error(png_ptr, "pngReaderCallback failed");
	}
}


static int read_png(FILE *fp, png_int_32 transforms)
{
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, no_warnings);
	png_infop info_ptr = NULL;

	if (png_ptr == NULL) {
		return 0;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return 0;
	}

#  ifdef PNG_BENIGN_ERRORS_SUPPORTED
	png_set_benign_errors(png_ptr, 1/*allowed*/);
#  endif

	png_init_io(png_ptr, fp);
	info_ptr = png_create_info_struct(png_ptr);

	if (info_ptr == NULL) {
		png_error(png_ptr, "OOM allocating info structure");
	}
	png_read_png(png_ptr, info_ptr, transforms, NULL/*params*/);

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	return 1;
}

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

static int perform_test(char *name)
{
	struct timespec before, after;

	if (mytime(&before)) {
		{
			FILE *fp = fopen(name, "rb");
			if (NULL == fp) {
				return 0 ;
			}
			if (read_png(fp, PNG_TRANSFORM_EXPAND)) {
				if (ferror(fp)) {
					perror("temporary file");
					fprintf(stderr, "file %s: error reading PNG data\n", name);
					return 0;
				}
			} else {
				perror("temporary file");
				fprintf(stderr, "file %s: error from libpng\n", name);
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

static int read_png_frommem(void *text, const unsigned int dataSize, png_int_32 transforms)
{
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, no_warnings);
	png_infop info_ptr = NULL;

	if (png_ptr == NULL) {
		return 0;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return 0;
	}

#  ifdef PNG_BENIGN_ERRORS_SUPPORTED
	png_set_benign_errors(png_ptr, 1/*allowed*/);
#  endif

	ImageSource imgsource;
	imgsource.data = text;
	imgsource.size = dataSize;
	imgsource.offset = 0;
	png_set_read_fn(png_ptr, &imgsource, pngReadCallback);
	//png_init_io(png_ptr, fp);

	info_ptr = png_create_info_struct(png_ptr);

	if (info_ptr == NULL) {
		png_error(png_ptr, "OOM allocating info structure");
	}
	png_read_png(png_ptr, info_ptr, transforms, NULL/*params*/);

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
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
		if (read_png_frommem(text, size, PNG_TRANSFORM_EXPAND)) {
		} else {
			perror("temporary file");
			fprintf(stderr, "file %d: error from libpng\n", i);
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

int main(int argc, char **argv)
{
	int ok = 0;
	int i ;
	if (argc < 2) {
		printf("please input a valid file \n");
		return  0 ;
	}
	for (i = 1; i < argc; ++i) {
		perform_test(argv[i]);
		perform_one_test(argv[i]);
	}
	/* Exit code 0 on success. */
	return ok == 0;
}

