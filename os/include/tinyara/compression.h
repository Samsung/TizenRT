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

#ifndef __INCLUDE_COMPRESSION_H
#define __INCLUDE_COMPRESSION_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define COMP_SUCCESS            0
#define COMP_NOT_SUPPORTED      1
#define COMP_FAILURE            2
#define COMP_INVALID_INPUT      3
#define COMP_USAGE_ERROR        4
#define COMP_DRVPATH		"/dev/compress"

#define LZMA_TYPE	        1
#define LZMA_NAME               "LZMA"

#define MINIZ_TYPE		2
#define MINIZ_NAME              "MINIZ"
#define COMP_NAME_SIZE          6

/****************************************************************************
 * Public Types
 ****************************************************************************/

enum compression_formats {
	COMPRESSION_TYPE_NONE = 0,
	COMPRESSION_TYPE_LZMA,
	COMPRESSION_TYPE_MINIZ,
	COMPRESSION_TYPE_MAX = COMPRESSION_TYPE_MINIZ,
};

/* Compression header struct */
struct s_header {
	int size_header;			/* Size of Compression header */
	int compression_format;			/* Compression Format */
	int blocksize;				/* Block Size used for file compression */
	int sections;				/* No. of compressed blocks for this file */
	int binary_size;			/* Uncompressed file size */
	int secoff[];				/* Offsets to Compressed blocks */
};

/* compress ioctl argument data struct */
struct compress_header {
	long unsigned int input_size;
	unsigned char *input_buffer;
	long unsigned int output_size;
	unsigned char *output_buffer;
};

/****************************************************************************
 * Name: compress_register
 *
 * Description:
 *   Register compress driver path, MMINFO_DRVPATH
 *
 ****************************************************************************/
void compress_register(void);

/****************************************************************************
 * Name: compress_block
 *
 * Description:
 *   compresses block in 'read_buffer' of readsize into 'out_buffer' of writesize
 *
 * Returned Value:
 *   Non-negative value on Success.
 *   Negative value on Failure.
 ****************************************************************************/
int compress_block(unsigned char *out_buffer, long unsigned int *writesize, unsigned char *read_buffer, long unsigned int size);

/****************************************************************************
 * Name: decompress_block
 *
 * Description:
 *   Decompress block in 'read_buffer' of readsize into 'out_buffer' of writesize
 *
 * Returned Value:
 *   Non-negative value on Success.
 *   Negative value on Failure.
 ****************************************************************************/
int decompress_block(unsigned char *out_buffer, long unsigned int *writesize, unsigned char *read_buffer, long unsigned int *size);

#endif						/* __INCLUDE_COMPRESSION_H */
