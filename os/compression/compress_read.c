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
#include <tinyara/config.h>
#include <tinyara/kmalloc.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/fs/fs.h>
#include <tinyara/binfmt/compression/compress_read.h>

#if CONFIG_COMPRESSION_TYPE == LZMA
#include <tinyara/lzma/LzmaLib.h>
#elif CONFIG_COMPRESSION_TYPE == MINIZ
#include <miniz/miniz.h>
#endif

/****************************************************************************
 * Private Declarations
 ****************************************************************************/

static struct s_header *compression_header;
static struct s_buffer buffers;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
 
/****************************************************************************
 * Name: compress_blocks_to_read
 *
 * Description:
 *   Assign values to first_block, last_block and number of blocks to be read
 *   from compressed file in this particular call to compress_read
 *
 * Returned Value:
 *   None
 ****************************************************************************/
static void compress_blocks_to_read(int *first_block, int *last_block, int *no_blocks, int offset, int readsize)
{
	int blocksize;

	blocksize = compression_header->blocksize;

	*first_block = offset / blocksize;
	*last_block = (offset + readsize - 1) / blocksize;
	*no_blocks = *last_block - *first_block + 1;
}

/****************************************************************************
 * Name: compress_parse_header
 *
 * Description:
 *   Parses the header containing compression related info present in the
 *   compressed file and assigns values to compression_header members
 *
 * Returned value:
 *   OK (0) is Success
 *   Negative value on Failure
 ****************************************************************************/
static int compress_parse_header(int filfd, uint16_t offset)
{
	off_t rpos;					/* Position returned by lseek */
	int nbytes;					/* Number of bytes read  */
	int compheader_size;				/* Total size of compression header */

	/* Seek to location of size of compression header */
	rpos = lseek(filfd, offset, SEEK_SET);
	if (rpos != offset) {
		int errval = get_errno();
		bcmpdbg("ERROR : lseek to offset %lu failed: %d\n", (unsigned long)offset, errval);
		return -errval;
	}

	/* Read compression header size from the file data */
	nbytes = read(filfd, &compheader_size, sizeof(compheader_size));
	if (nbytes != sizeof(compheader_size)) {
		bcmpdbg("Read for compression header size from offset %lu failed\n", offset);
		return ERROR;
	}

	/* Allocate memory for compression header now that we know it's size */
	compression_header = (struct s_header *)kmm_malloc(compheader_size);
	if (!compression_header) {
		bcmpdbg("Failed kmm_malloc for compression_header\n");
		return -ENOMEM;
	}

	/* Assign compresssion_header->size_header */
	compression_header->size_header = compheader_size;

	/* Read remaining compression header, including section offsets */
	nbytes = read(filfd, ((uint8_t *)compression_header + sizeof(compression_header->size_header)), compheader_size - sizeof(compression_header->size_header));
	if (nbytes != (compheader_size - sizeof(compression_header->size_header))) {
		bcmpdbg("Read for compression header from offset %lu failed\n", offset);
		return ERROR;
	}

	bcmpvdbg("Compressed Binary Header info: size (%d), compression format (%d), blocksize (%d), No. sections (%d), Uncompressed binary size = %d\n", compression_header->size_header, compression_header->compression_format, compression_header->blocksize, compression_header->sections, compression_header->binary_size);

	return OK;
}

/****************************************************************************
 * Name: compress_offset_block
 *
 * Description:
 *   Returns offset of 'block_number' block by reading it from compression header
 *
 * Returned Value:
 *   'block_offset' value (positive) on Success
 *   Negative value on Failure
 ****************************************************************************/
static off_t compress_offset_block(int filfd, uint16_t binary_header_size, int block_number)
{
	off_t position;

	/* Return position for 'block_number' block */
	position = binary_header_size + compression_header->size_header + compression_header->secoff[block_number];

	return position;
}

/****************************************************************************
 * Name: compress_lseek_block
 *
 * Description:
 *   Seek to start of "block_number" block in compressed blocks section
 *
 * Returned Value:
 *   Starting offset (positive) of 'block_number' block in binary on Success
 *   Negative value on Failure
 ****************************************************************************/
static off_t compress_lseek_block(int filfd, uint16_t binary_header_size, int block_number)
{
	off_t rpos;
	off_t block_offset;

	/* Calculate block offset from start of file for start of 'block_number' block */
	block_offset = compress_offset_block(filfd, binary_header_size, block_number);
	if (block_offset < 0) {
		bcmpdbg("Incorrect offset for block number %d\n", block_number);
		return block_offset;
	}

	/* Seek to location of this block in compressed file */
	rpos = lseek(filfd, block_offset, SEEK_SET);
	if (rpos != block_offset) {
		int errval = get_errno();
		bcmpdbg("Failed to seek to position %lu: %d\n", (unsigned long)block_offset, errval);
		return -errval;
	}

	return rpos;
}

/****************************************************************************
 * Name: compress_read_block
 *
 * Description:
 *   Read 'block_number' block from compressed blocks section into read_buffer
 *
 * Returned Value:
 *   Number of bytes read into read_buffer on Success
 *   Negative value on Failure
 ****************************************************************************/
static off_t compress_read_block(int filfd, uint16_t binary_header_size, FAR uint8_t *buf, int block_number)
{
	off_t rpos;
	off_t readsize;
	ssize_t nbytes;
	off_t current_block_offset;
	off_t next_block_offset;

	/* Find out size of 'block_number' block in compressed file. Assign to readsize */
	next_block_offset = compress_offset_block(filfd, binary_header_size, block_number + 1);
	if (next_block_offset < 0) {
		bcmpdbg("Incorrect offset for block number %d\n", block_number + 1);
		return ERROR;
	}

	current_block_offset = compress_offset_block(filfd, binary_header_size, block_number);
	if (current_block_offset < 0) {
		bcmpdbg("Incorrect offset for block number %d\n", block_number);
		return ERROR;
	}

	readsize = next_block_offset - current_block_offset;
	if (readsize < 0) {
		bcmpdbg("Incorrect readsize %d for block, has to be positive\n", readsize);
		return readsize;
	}

	/* Seek to location of 'block_number' block in compressed file */
	rpos = compress_lseek_block(filfd, binary_header_size, block_number);
	if (rpos < 0) {
		bcmpdbg("Failed to seek to offset of block number %d\n", block_number);
		return rpos;
	}

	/* Read 'block_number' block into buf */
	nbytes = read(filfd, buf, readsize);
	if (nbytes != readsize) {
		bcmpdbg("Read for compressed block %d failed\n", block_number);
		return ERROR;
	}

	return nbytes;
}

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
int compress_read(int filfd, uint16_t binary_header_size, FAR uint8_t *buffer, size_t readsize, off_t offset)
{
	int first_block;
	int last_block;
	int no_blocks;
	int index;
	int ret;
	int actual_offset;			/* Offset from start of uncompressed file */
	int block_size_to_write;	/* Size to write into buffer from decompressed block */
	int buffer_index;
	int blocksize;
	int block_readsize;
#if CONFIG_COMPRESSION_TYPE == LZMA
	unsigned int writesize;
	unsigned int size;
#elif CONFIG_COMPRESSION_TYPE == MINIZ
	long unsigned int writesize;
	long unsigned int size;
#endif

	/* Setting first block, end block and number of blocks to read and decompressed */
	blocksize = compression_header->blocksize;
	compress_blocks_to_read(&first_block, &last_block, &no_blocks, offset, readsize);
	if (first_block < 0 || no_blocks < 0) {
		bcmpdbg("Incorrect first_block, no_blocks info\n");
		buffer_index = ERROR;
		goto error_compress_read;
	}

	index = first_block;
	buffer_index = 0;
	/* Actual Offset in uncompressed file is same as Offset passed to this function */
	actual_offset = offset;

	/* Reading and decompressing blocks from first_block to last_block. Then writing to buffer. */
	for (; index < first_block + no_blocks; index++) {
		/* Read compressed 'index' block into read_buffer */
		block_readsize = compress_read_block(filfd, binary_header_size, buffers.read_buffer, index);
		if (block_readsize < 0) {
			bcmpdbg("Read for compressed block %d failed\n", index);
			buffer_index = block_readsize;
			goto error_compress_read;
		}

#if CONFIG_COMPRESSION_TYPE == LZMA
		size = (unsigned int)block_readsize;
#elif CONFIG_COMPRESSION_TYPE == MINIZ
		size = (long unsigned int)block_readsize;
#endif
		writesize = compression_header->blocksize; 
		/* Decompress block in read_buffer to out_buffer */
		ret = decompress_block(buffers.out_buffer, &writesize, buffers.read_buffer, &size);
		if (ret == ERROR) {
			bcmpdbg("Failed to decompress %d block of this binary\n", index);
			buffer_index = ret;
			goto error_compress_read;
		}

		if (index == first_block) {
			/*
			 * Read appropriate number of bytes to buffer from this block using readsize and offset info.
			 * If start_offset + readsize < end_offset, write from start_offset to start_offset + readsize.
			 * Otherwise, write from start_offset to end_offset into buffer.
			 */
			block_size_to_write = ((index + 1) * blocksize - 1 > actual_offset + readsize - 1 ? readsize : (index + 1) * blocksize - actual_offset);
			memcpy(&buffer[buffer_index], &buffers.out_buffer[actual_offset - (index * blocksize)], block_size_to_write);
			buffer_index += block_size_to_write;
		} else if (index == last_block) {
			/*
			 * Read appropriate number of bytes to buffer from this block using readsize and offset info.
			 * Calculate end_offset (Offset of last byte to write from this block).
			 * Write from start_offset to end_offset from this block into buffer.
			 */
			block_size_to_write = actual_offset + readsize - (index * blocksize);
			memcpy(&buffer[buffer_index], &buffers.out_buffer[0], block_size_to_write);
			buffer_index += block_size_to_write;
		} else {
			/*
			 * This is not the first or last block. This is intermediary block.
			 * So, write entire block into buffer.
			 */
			block_size_to_write = blocksize;
			memcpy(&buffer[buffer_index], &buffers.out_buffer[0], block_size_to_write);
			buffer_index += block_size_to_write;
		}
	}

error_compress_read:
	return buffer_index;
}

/****************************************************************************
 * Name: compress_init
 *
 * Description:
 *   Initialize the compression_header of type'struct s_header' for this file
 *
 * Returned value:
 *   OK (0) on Success
 *   ERROR (-1) on Failure
 ****************************************************************************/
int compress_init(int filfd, uint16_t offset, off_t *filelen)
{
	int ret;

	/* Parsing compression header for compressed file */
	ret = compress_parse_header(filfd, offset);
	if (ret != OK) {
		bcmpdbg("Failed to parse compression header from file\n");
		goto error_compress_init;
	}

	/* Assign file length as that of uncompressed file */
	*filelen = compression_header->binary_size;

#if CONFIG_COMPRESSION_TYPE == LZMA
	/* Allocating memory for read and out buffer to be used for LZMA decompression */
	if (compression_header->compression_format == COMPRESSION_TYPE_LZMA) {
		buffers.read_buffer = (unsigned char *)kmm_malloc(compression_header->blocksize + LZMA_PROPS_SIZE);
		if (buffers.read_buffer == NULL) {
			return -ENOMEM;
		}
		buffers.out_buffer = (unsigned char *)kmm_malloc(compression_header->blocksize);
		if (buffers.out_buffer == NULL) {
			kmm_free(buffers.read_buffer);
			return -ENOMEM;
		}
	}
#elif CONFIG_COMPRESSION_TYPE == MINIZ
	/* Allocating memory for read and out buffer to be used for Miniz decompression */
	if (compression_header->compression_format == COMPRESSION_TYPE_MINIZ) {
		buffers.read_buffer = (unsigned char *)kmm_malloc(compressBound(compression_header->blocksize));
		if (buffers.read_buffer == NULL) {
			return -ENOMEM;
		}
		buffers.out_buffer = (unsigned char *)kmm_malloc(compression_header->blocksize);
		if (buffers.out_buffer == NULL) {
			kmm_free(buffers.read_buffer);
			return -ENOMEM;
		}
	}
#endif

error_compress_init:
	return ret;
}

/****************************************************************************
 * Name: compress_uninit
 *
 * Description:
 *   Release buffers initialized during compress_init
 *
 * Returned Value:
 *   None
 ****************************************************************************/
void compress_uninit(void)
{
#if CONFIG_COMPRESSION_TYPE == LZMA || CONFIG_COMPRESSION_TYPE == MINIZ
	/* Freeing memory allocated to read_buffer and out_buffer for file decompression */
	if (compression_header->compression_format == COMPRESSION_TYPE_LZMA || compression_header->compression_format == COMPRESSION_TYPE_MINIZ) {
		if (buffers.read_buffer) {
			kmm_free(buffers.read_buffer);
			buffers.read_buffer = NULL;
		}
		if (buffers.out_buffer) {
			kmm_free(buffers.out_buffer);
			buffers.out_buffer = NULL;
		}
	}
#endif

	kmm_free(compression_header);
	compression_header = NULL;
}

struct s_header *get_compression_header(void)
{
	return compression_header;
}
