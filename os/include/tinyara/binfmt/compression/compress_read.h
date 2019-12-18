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

#ifndef __INCLUDE_COMPRESS_READ_H
#define __INCLUDE_COMPRESS_READ_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/binfmt/compression/compression.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Struct for buffers to be used for read/decompression */
struct s_buffer {
	unsigned char *read_buffer;
	unsigned char *out_buffer;
};

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: compress_uninit
 *
 * Description:
 *   Release buffers initialized during compress_init
 *
 * Returned Value:
 *   None
 ****************************************************************************/
void compress_uninit(void);

/****************************************************************************
 * Name: compress_init
 *
 * Description:
 *   Initialize the header 's_header' for this binary
 *
 * Returned value:
 *   OK (0) on Success
 *   ERROR (-1) on Failure
 ****************************************************************************/
int compress_init(int filfd, uint16_t offset, off_t *filelen);

/****************************************************************************
 * Name: compress_read
 *
 * Description:
 *   Read bytes from the compressed file using 'offset' and 'readsize' info
 *   provided for uncompressed file.  The data is read into 'buffer'. Offset
 *   value here is offset from start of uncompressed binary (excluding binary
 *   header).
 *
 * Returned Value:
 *   Number of bytes read into buffer on Success
 *   Negative value on failure
 ****************************************************************************/
int compress_read(int filfd, uint16_t binary_header_size, FAR uint8_t *buffer, size_t readsize, off_t offset);

/****************************************************************************
 * Name: get_compression_header
 *
 * Returned Value:
 *   Address of the compression_header
 ****************************************************************************/
struct s_header *get_compression_header(void);

#endif							/* __INCLUDE_COMPRESS_READ_H */
