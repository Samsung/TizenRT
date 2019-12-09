/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
* Included Files
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdlib.h>
#include "../config.h"
#include "../compression.h"

#if CONFIG_COMPRESSION_TYPE == LZMA
#include "lzma/LzmaLib.h"
#elif CONFIG_COMPRESSION_TYPE == MINIZ
#include "miniz/miniz.h"
#endif

#define MAX_BLOCK_SIZE 8192

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void show_usage(const char *progname)
{
	fprintf(stderr, "USAGE: %s <block size> <compression format> <uncompressed file name> <compressed file name>\n", progname);
	exit(1);
}

static void compress_file(int block_size, int type, char *in_file, char *out_file)
{
	unsigned int sections;
	long unsigned int size;
	int in_fd;
	int out_fd;
	int index;
	int nbytes;
	int ret;
	struct stat buf;
	unsigned int rpos;
	unsigned long int readsize = block_size;
	unsigned long int writesize = block_size;
	unsigned char *read_buf = NULL;
	unsigned char *out_buf = NULL;
	unsigned char *tptr;
	struct s_header *phdr = NULL;
#if CONFIG_COMPRESSION_TYPE == LZMA
	long unsigned int propsSize = LZMA_PROPS_SIZE;
#endif

	ret = stat((char *)in_file, &buf);
	if (ret < 0) {
		goto error;
	}

	in_fd = open(in_file, O_RDONLY);
	if (in_fd < 0) {
		printf("Failed to open file %s\n", in_file);
		goto error;
	}

	out_fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (out_fd < 0) {
		printf("Failed to open file %s\n", out_file);
		goto error;
	}

	read_buf = (unsigned char *)malloc(block_size);
	if (!read_buf) {
		printf("Failed to allocate memory for read_buf\n");
		goto error;
	}

#if CONFIG_COMPRESSION_TYPE == LZMA
	out_buf = (unsigned char *)malloc(block_size + LZMA_PROPS_SIZE);
	if (!out_buf) {
		printf("Failed to allocate memory for out_buf\n");
		goto error;
	}
#elif CONFIG_COMPRESSION_TYPE == MINIZ
	out_buf = (unsigned char *)malloc(block_size);
	if (!out_buf) {
		printf("Failed to allocate memory for out_buf\n");
		goto error;
	}
#endif

	sections = buf.st_size / block_size;
	if (buf.st_size % block_size) {
		sections++;
	}
	sections++;

	printf("File Size %ld, No of Sections :%d \n", buf.st_size, sections);

	/* Calculating size of header */
	size = sections * sizeof(int) + sizeof(struct s_header);
	phdr = (struct s_header *)malloc(size);
	if (!phdr) {
		printf("Failed to allocate memory for header\n");
		goto error;
	}

	/* Assigning values to Compression Header members */
	phdr->sections = sections;
	phdr->compression_format = type;
	phdr->size_header = size;
	phdr->blocksize = block_size;
	phdr->binary_size = buf.st_size;
	phdr->secoff[0] = 0;

	rpos = lseek(out_fd, size, SEEK_SET);
	if (rpos != size) {
		printf("Failed to seek to position %lu\n", (unsigned long)size);
		goto error;
	}

	/* Compress all blocks and write to output file */
	for (index = 0; index < sections - 1; index++) {
		/* Read block into read_buf */
		tptr = read_buf;
		readsize = block_size;
		while (readsize > 0) {

			nbytes = read(in_fd, tptr, readsize);
			if (nbytes < 0) {
				/* EINTR just means that we received a signal */

				if (nbytes != -EINTR) {
					printf("Read failed: %d\n", (int)nbytes);
					goto error;
				}
			} else if (nbytes == 0) {
				printf("Unexpected end of file\n");
				break;
			} else {
				readsize -= nbytes;
				tptr += nbytes;
			}
		}
#if CONFIG_COMPRESSION_TYPE == LZMA
		/* LZMA Compression for data in read_buf into out_buf */
		writesize = block_size;
		ret = LzmaCompress(&out_buf[LZMA_PROPS_SIZE], &writesize, read_buf, (block_size - readsize), out_buf, &propsSize, 0, 1<<13 , -1, -1, -1, -1, 1);
		if (ret != SZ_OK) {
			printf("LZMA Compress failed, ret = %d\n", ret);
		}

		printf("==> lzma_compress %d writesize %lu\n", index, writesize);
#elif CONFIG_COMPRESSION_TYPE == MINIZ
		/* Miniz Compression for data in read_buf into out_buf */
		writesize = compressBound(block_size - readsize);
		ret = mz_compress(out_buf, &writesize, read_buf, (block_size - readsize));
		if (ret != Z_OK) {
			printf("Miniz Compress failed, ret = %d\n", ret);
		}
		printf("==> miniz_compress %d writesize %lu\n", index, writesize);
#else
		printf("Compression for type %d not supported\n", CONFIG_COMPRESSION_TYPE);
		printf("Set CONFIG_COMPRESSION_TYPE to %d, then generate mkcompressimg again for this type", CONFIG_COMPRESSION_TYPE);
		exit(1);
#endif
		phdr->secoff[index + 1] = phdr->secoff[index] + writesize;

		/* Write out_buf to output file */
		tptr = out_buf;
		while (writesize > 0) {
			nbytes = write(out_fd, tptr, writesize);
			if (nbytes < 0) {
				/* EINTR just means that we received a signal */

				if (nbytes != -EINTR) {
					printf("Write failed: %d\n", (int)nbytes);
					goto error;
				}
			} else if (nbytes == 0) {
				printf("Unexpected end of file\n");
				break;
			} else {
				writesize -= nbytes;
				tptr += nbytes;
			}
		}
	}

	rpos = lseek(out_fd, 0, SEEK_SET);
	if (rpos != 0) {
		printf("Failed to seek to position %lu\n", (unsigned long)size);
		goto error;
	}

	/* Write header into output file */
	writesize = size;
	tptr = (unsigned char *)phdr;
	while (writesize > 0) {
		nbytes = write(out_fd, tptr, writesize);
		if (nbytes < 0) {
			/* EINTR just means that we received a signal */

			if (nbytes != -EINTR) {
				printf("Write failed: %d\n", (int)nbytes);
				goto error;
			}
		} else if (nbytes == 0) {
			printf("Unexpected end of file\n");
			break;
		} else {
			writesize -= nbytes;
			tptr += nbytes;
		}
	}

error:
	if (phdr) {
		free(phdr);
	}
	if (read_buf) {
		free(read_buf);
	}
	if (out_buf) {
		free(out_buf);
	}
	if (in_fd > 0) {
		close(in_fd);
	}
	if (out_fd > 0) {
		close(out_fd);
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int main(int argc, char *argv[])	//Main defined
{
	int ret;
	int block_size;
	int comp_format;
	struct stat buf;

	if (argc != 5) {
		fprintf(stderr, "Unexpected number of arguments\n");
		show_usage(argv[0]);
	}

	block_size = atoi(argv[1]);

	if (block_size > MAX_BLOCK_SIZE) {
		fprintf(stderr, "Block size should be less than %d, Modify MAX_BLOCK_SIZE if needed\n", MAX_BLOCK_SIZE);
		exit(2);
	}

	comp_format = atoi(argv[2]);

	if (comp_format <= COMPRESSION_TYPE_NONE || comp_format > COMPRESSION_TYPE_MAX) {
		fprintf(stderr, "Compression Mode %d not supported\n", comp_format);
		exit(3);
	}

	ret = stat((char *)argv[3], &buf);
	if (ret < 0) {
		fprintf(stderr, "Input file not found\n");
		exit(4);
	}

	compress_file(block_size, comp_format, argv[3], argv[4]);

	return 0;
}
