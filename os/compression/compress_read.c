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

#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/fs/fs.h>
#include <tinyara/binfmt/compression/compress_read.h>

#if CONFIG_COMPRESSION_TYPE == 1
#include "lzma/LzmaLib.h"
#endif

/****************************************************************************
 * Private Declarations
 ****************************************************************************/

static struct s_header compression_header;

/* Number of blocks for caching */
static unsigned int number_blocks_caching;

/* Pointer to block_cache_t list to be used for holding uncompressed blocks */
static block_cache_t *blockcache;

/* Pointers for maintaining doubly linked list of blockcache */
static block_cache_t *head;		/* Pointer to least priority block for caching */
static block_cache_t *tail;		/* Pointer to highest priority block for caching */
static block_cache_t *most_accessed;	/* Pointer to most accessed block for quick access */

/* Number of requests for the most accessed block in blockcache list */
static unsigned int max_accessed_count;

/* Buffer to be used for reading compressed data from blockwise compressed binary */
static unsigned char *compressed_data_buffer;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: compress_decompress_block
 *
 * Description:
 *   Decompress block in 'compressed_data_buffer' of readsize into 'out_buffer' of writesize
 *
 * Returned Value:
 *   Non-negative value on Success.
 *   Negative value on Failure.
 ****************************************************************************/
static int compress_decompress_block(unsigned char *out_buffer, unsigned int *writesize, unsigned char *read_buf, unsigned int *size, int index)
{
	int ret = ERROR;

#if CONFIG_COMPRESSION_TYPE == 1
	if (compression_header.compression_format == COMPRESSION_TYPE_LZMA) {
		/* LZMA specific logic for decompression */
		*writesize = compression_header.blocksize;
		*size -= (LZMA_PROPS_SIZE);

		ret = LzmaUncompress(&out_buffer[0], writesize, &read_buf[LZMA_PROPS_SIZE], size, &read_buf[0], LZMA_PROPS_SIZE);

		if (ret == SZ_ERROR_FAIL) {
			bmdbg("Failure to decompress with LZMAUncompress API\n");
			ret = -ret;
		}
	}
#endif

	return ret;
}

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

	blocksize = compression_header.blocksize;

	*first_block = offset / blocksize;
	*last_block = (offset + readsize) / blocksize;
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
 *   ERROR (-1) on Failure
 ****************************************************************************/
static int compress_parse_header(int filfd, uint16_t offset)
{
	off_t rpos;					/* Position returned by lseek */
	int nbytes;					/* Number of bytes read  */

	/* Seek to the next read position */

	rpos = lseek(filfd, offset, SEEK_SET);
	if (rpos != offset) {
		int errval = get_errno();
		bmdbg("ERROR : leek to offset %lu failed: %d\n", (unsigned long)offset, errval);
		return -errval;
	}

	/* Read compression header data from the file data from offset */

	nbytes = read(filfd, &compression_header, sizeof(struct s_header));
	if (nbytes != sizeof(struct s_header)) {
		bmdbg("Read for compression header from offset %lu failed\n", offset);
		return ERROR;
	}

	bmvdbg("Compressed Binary Header info: size (%d), compression format (%d), blocksize (%d), No. sections (%d), Uncompressed binary size = %d\n", compression_header.size_header, compression_header.compression_format, compression_header.blocksize, compression_header.sections, compression_header.binary_size);

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
	off_t rpos;
	off_t position;
	off_t block_offset;
	int nbytes;

	/* Seek to offset in compression header that has offset value for 'block_number' block */
	position = binary_header_size + sizeof(struct s_header) + block_number * sizeof(int);

	rpos = lseek(filfd, position, SEEK_SET);
	if (rpos != position) {
		int errval = get_errno();
		bmdbg("Failed to seek to position %lu: %d\n", (unsigned long)position, errval);
		return -errval;
	}

	/* Read offset value into block_offset */
	nbytes = read(filfd, &block_offset, sizeof(off_t));
	if (nbytes != sizeof(off_t)) {
		bmdbg("Read for block offset for block no. %d failed\n", block_number);
		return ERROR;
	}

	return block_offset;
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
	off_t actual_offset;
	off_t block_offset;

	/* Calculate actual offset from start of file for start of 'block_number' block */
	block_offset = compress_offset_block(filfd, binary_header_size, block_number);
	if (block_offset < 0) {
		bmdbg("Incorrect offset for block number %d\n", block_number);
		return block_offset;
	}
	actual_offset = binary_header_size + compression_header.size_header + block_offset;;

	/* Seek to location of this block in compressed file */
	rpos = lseek(filfd, actual_offset, SEEK_SET);
	if (rpos != actual_offset) {
		int errval = get_errno();
		bmdbg("Failed to seek to position %lu: %d\n", (unsigned long)actual_offset, errval);
		return -errval;
	}

	return rpos;
}

/****************************************************************************
 * Name: compress_read_block
 *
 * Description:
 *   Read 'block_number' block from compressed blocks section into
 *   compressed_data_buffer.
 *
 * Returned Value:
 *   Number of bytes read into compressed_data_buffer on Success
 *   Negative value on Failure
 ****************************************************************************/
static off_t compress_read_block(int filfd, uint16_t binary_header_size, FAR uint8_t *buf, int block_number)
{
	off_t rpos;
	size_t readsize;
	ssize_t nbytes;
	off_t current_block_offset;
	off_t next_block_offset;

	/* Find out size of 'block_number' block in compressed file. Assign to readsize */
	next_block_offset = compress_offset_block(filfd, binary_header_size, block_number + 1);
	if (next_block_offset < 0) {
		bmdbg("Incorrect offset for block number %d\n", block_number + 1);
		return ERROR;
	}

	current_block_offset = compress_offset_block(filfd, binary_header_size, block_number);
	if (current_block_offset < 0) {
		bmdbg("Incorrect offset for block number %d\n", block_number);
		return ERROR;
	}

	readsize = next_block_offset - current_block_offset;
	if (readsize < 0) {
		bmdbg("Incorrect readsize %d for block, has to be positive\n", readsize);
		return readsize;
	}

	/* Seek to location of 'block_number' block in compressed file */
	rpos = compress_lseek_block(filfd, binary_header_size, block_number);
	if (rpos < 0) {
		bmdbg("Failed to seek to offset of block number %d\n", block_number);
		return rpos;
	}

	/* Read 'block_number' block into buf */
	nbytes = read(filfd, buf, readsize);
	if (nbytes != readsize) {
		bmdbg("Read for compressed block %d failed\n", block_number);
		return ERROR;
	}

	return nbytes;
}

/****************************************************************************
 * Name: compress_update_blockcache_list
 *
 * Description:
 *   Update blockcache list based on whether 'block_number' block in
 *   blockwise-compressed binary is already cached or not. Also, maintain
 *   head, tail and most_accessed pointers. Decompress block if not cached.
 *   tail = most recently cached block (highest priority block for keeping
 *          cached)
 *   head = oldest cached block (lowest priority for keeping it cached).
 *          If a new block needs to be cached, cache to this blockcache element.
 *   most_accessed = pointer to most accessed block in blockcache list.
 *          Needed for easy access to most accessed block from comp. binary.
 *
 * Returned Value:
 *   Index in blockcache list where 'block_number' block number from compressed
 *   binary is cached in uncompressed form. Return Negative value on failure.
 ****************************************************************************/
static unsigned int compress_update_blockcache_list(int block_number, int filfd, uint16_t binary_header_size)
{
	unsigned int blockcache_index;	/* Which blockcache element has needed uncompressed data for read */
	block_cache_t *ptr;		/* Pointer to element in blockcache list which will be updated */
	block_cache_t *temp;		/* Used when updating blockcache list */
	bool flag_for_caching;		/* 1 = Block needs to be cached, 0 = Block already cached */
	bool update_most_accessed;	/* 1 = Most accessed block is being removed from blockcache.
					 *     Need to update most accessed pointer from scratch */
	unsigned int writesize;
	int size;
	int ret;

	/* Initialize flags */
	flag_for_caching = true;
	update_most_accessed = false;

	/* First check is with block_number at most_accessed pointer */
	if (most_accessed->block_number == block_number) {
		flag_for_caching = false;
		blockcache_index = most_accessed->index_block_cache;
	}

	/* Start checking for if block is cached from the tail */
	if (flag_for_caching == true) {
		ptr = tail;
		while (ptr != NULL) {
			if (ptr->block_number == block_number) {
				flag_for_caching = false;
				blockcache_index = ptr->index_block_cache;
				break;
			} else {
				ptr = ptr->prev;
			}
		}
	}

	/* If block_number block from compressed binary needs to be cached */
	if (flag_for_caching == true) {

		/* If most_accessed == head, need to update most_accessed */
		if (head == most_accessed)
			update_most_accessed = true;

		/* Detach head, Reassign head to head->next */
		ptr = head;
		head = ptr->next;
		head->prev = ptr->prev;

		/* Read compressed 'block_number' block into compressed_data_buffer */
		size = compress_read_block(filfd, binary_header_size, compressed_data_buffer, block_number);
		if (size < 0) {
			bmdbg("Read for compressed block %d failed\n", block_number);
			return ERROR;
		}

		/* Decompress block in compressed_data_buffer to chosen blockcache element */
		ret = compress_decompress_block(ptr->out_buffer, &writesize, compressed_data_buffer, &size, block_number);
		if (ret == ERROR) {
			bmdbg("Failed to decompress %d block of this binary\n", block_number);
			return ret;
		}

		/* Update block_number, Number of requests */
		ptr->block_number = block_number;
		ptr->no_requests_for_block = 1;

		/* Updating most_accessed pointer */
		if (ptr->no_requests_for_block >= max_accessed_count) {
			max_accessed_count = ptr->no_requests_for_block;
			most_accessed = ptr;
		}

		/* Always attach ptr at tail */
		temp = tail;
		temp->next = ptr;
		ptr->prev = temp;
		ptr->next = NULL;
		tail = ptr;

		/* Update most_accessed from scratch, if needed */
		if (update_most_accessed == true) {
			max_accessed_count = 0;
			temp = tail;
			while (temp != NULL) {
				if (temp->no_requests_for_block > max_accessed_count) {
					max_accessed_count = temp->no_requests_for_block;
					most_accessed = temp;
				}
				temp = temp->prev;
			}
		}

		/* Update blockcache_index into which block is uncompressed */
		blockcache_index = ptr->index_block_cache;

	} else { /* If block is already cached */

		/* If block is cached at head location */
		if (head == &blockcache[blockcache_index]) {
			/* Detach head, Reassign head to head->next */
			ptr = head;
			head = ptr->next;
			head->prev = ptr->prev;

			ptr->no_requests_for_block += 1;

			/* Update most_accessed pointer */
			if (ptr->no_requests_for_block >= max_accessed_count) {
				max_accessed_count = ptr->no_requests_for_block;
				most_accessed = ptr;
			}

			/* Always attach ptr at tail */
			temp = tail;
			temp->next = ptr;
			ptr->prev = temp;
			ptr->next = NULL;
			tail = ptr;

		} else if (tail == &blockcache[blockcache_index]) { /* Block is cached at tail location */
			blockcache[blockcache_index].no_requests_for_block += 1;

			/* Update most_accessed pointer */
			ptr = tail;
			if (ptr->no_requests_for_block >= max_accessed_count) {
				max_accessed_count = ptr->no_requests_for_block;
				most_accessed = ptr;
			}
		} else { /* Block is cached at some other index in blockcache list */

			/* Detach blockcache element at blockcache_index */
			ptr = &blockcache[blockcache_index];
			ptr->prev->next = ptr->next;
			ptr->next->prev = ptr->prev;

			/* Update Number of requests for block */
			ptr->no_requests_for_block += 1;

			/* Update most accessed pointer */
			if (ptr->no_requests_for_block >= max_accessed_count) {
				max_accessed_count = ptr->no_requests_for_block;
				most_accessed = ptr;
			}

			/* Always attach ptr at tail */
			temp = tail;
			temp->next = ptr;
			ptr->prev = temp;
			ptr->next = NULL;
			tail = ptr;
		}
	}

	return blockcache_index;
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
	int block_number;		/* Block number in compressed file */
	int actual_offset;		/* Offset from start of uncompressed file */
	int block_size_to_write;	/* Size to write into buffer from decompressed block */
	int buffer_pos;			/* Position in buffer to start writing from */
	int blocksize;			/* Blocksize used for compressing the binary */
	unsigned int blockcache_index;	/* Which blockcache element has needed uncompressed data for read */

	/* Setting first block, end block and number of blocks to read and decompressed */
	blocksize = compression_header.blocksize;
	compress_blocks_to_read(&first_block, &last_block, &no_blocks, offset, readsize);
	if (first_block < 0 || no_blocks < 0) {
		bmdbg("Incorrect first_block, no_blocks info\n");
		buffer_pos = ERROR;
		goto error_compress_read;
	}

	block_number = first_block;
	buffer_pos = 0;
	/* Actual Offset in uncompressed file is same as Offset passed to this function */
	actual_offset = offset;

	/* Reading and decompressing blocks from first_block to last_block. Then writing to buffer. */
	for (; block_number < first_block + no_blocks; block_number++) {

		/* Update blockcache list and get uncompressed data into one of the blockcache elements */
		blockcache_index = compress_update_blockcache_list(block_number, filfd, binary_header_size);
		if (blockcache_index >= number_blocks_caching) {
			buffer_pos = ERROR;
			goto error_compress_read;
		}

		if (block_number == first_block) {
			/*
			 * Read appropriate number of bytes to buffer from this block using readsize and offset info.
			 * If start_offset + readsize < end_offset, write from start_offset to start_offset + readsize.
			 * Otherwise, write from start_offset to end_offset into buffer.
			 */
			block_size_to_write = ((block_number + 1) * blocksize - 1 > actual_offset + readsize - 1 ? readsize : (block_number + 1) * blocksize - actual_offset);
			memcpy(&buffer[buffer_pos], &blockcache[blockcache_index].out_buffer[actual_offset - (block_number * blocksize)], block_size_to_write);
			buffer_pos += block_size_to_write;
		} else if (block_number == last_block) {
			/*
			 * Read appropriate number of bytes to buffer from this block using readsize and offset info.
			 * Calculate end_offset (Offset of last byte to write from this block).
			 * Write from start_offset to end_offset from this block into buffer.
			 */
			block_size_to_write = actual_offset + readsize - (block_number * blocksize);
			memcpy(&buffer[buffer_pos], &blockcache[blockcache_index].out_buffer[0], block_size_to_write);
			buffer_pos += block_size_to_write;
		} else {
			/*
			 * This is not the first or last block. This is intermediary block.
			 * So, write entire block into buffer.
			 */
			block_size_to_write = blocksize;
			memcpy(&buffer[buffer_pos], &blockcache[blockcache_index].out_buffer[0], block_size_to_write);
			buffer_pos += block_size_to_write;
		}
	}

error_compress_read:
	return buffer_pos;
}

/****************************************************************************
 * Name: compress_init
 *
 * Description:
 *   Initialize the compression_header of type'struct s_header' for this file
 *
 * Returned value:
 *   OK (0) on Success
 *   Negative value on Failure
 ****************************************************************************/
int compress_init(int filfd, uint16_t offset, off_t *filelen)
{
	int ret;

	/* Parsing compression header for compressed file */
	ret = compress_parse_header(filfd, offset);
	if (ret != OK) {
		bmdbg("Failed to parse compression header from file\n");
		goto error_compress_init;
	}

	/* Assign file length as that of uncompressed file */
	*filelen = compression_header.binary_size;

	/* Set number of blocks to use for caching */
	if (CONFIG_DECOMPRESSION_CACHE_BLOCKS < (CUTOFF_RATIO_CACHE_BLOCKS) * (compression_header.sections - 1))
		number_blocks_caching = CONFIG_DECOMPRESSION_CACHE_BLOCKS;
	else
		number_blocks_caching = ((CUTOFF_RATIO_CACHE_BLOCKS) * (compression_header.sections - 1));

	/* Initialize max_accesed_count to 0 */
	max_accessed_count = 0;

#if CONFIG_COMPRESSION_TYPE == 1
	/* Allocating memory for compressed_data_buffer and blockcache list to be used for LZMA decompression */
	if (compression_header.compression_format == COMPRESSION_TYPE_LZMA) {

		compressed_data_buffer = (unsigned char *)kmm_malloc(compression_header.blocksize + 5);
		if (!compressed_data_buffer) {
			bmdbg("Failed kmm_malloc for read buffer\n");
			return -ENOMEM;
		}

		blockcache = (block_cache_t *)kmm_malloc(number_blocks_caching * sizeof(block_cache_t));
		if (!blockcache) {
			bmdbg("Failed kmm_malloc for blockcache\n");
			compress_uninit();
			return -ENOMEM;
		}

		/* Initialize blockcache list */
		for (int i = 0; i < number_blocks_caching; i++) {
			blockcache[i].out_buffer = (unsigned char *)kmm_malloc(compression_header.blocksize);
			if (!blockcache[i].out_buffer) {
				bmdbg("Failed kmm_malloc for blockcache's out_buffer\n");
				compress_uninit();
				return -ENOMEM;
			}

			blockcache[i].block_number = -1;
			blockcache[i].no_requests_for_block = 0;
			blockcache[i].index_block_cache = i;
			blockcache[i].next = &blockcache[(i + 1) % number_blocks_caching];
			blockcache[i].prev = &blockcache[(i - 1) % number_blocks_caching];
		}
	}
#endif

	/* Assign head, tail and most_accessed pointers */
	head = &blockcache[0];
	tail = &blockcache[number_blocks_caching - 1];
	most_accessed = tail;

	head->prev = NULL;
	tail->next = NULL;

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
	/* Freeing memory allocated to compressed_data_buffer and blockcache for file decompression */
	if (compressed_data_buffer) {
		kmm_free(compressed_data_buffer);
		compressed_data_buffer = NULL;
	}

	for (int i = 0; i < number_blocks_caching; i++) {
		if (blockcache[i].out_buffer) {
			kmm_free(blockcache[i].out_buffer);
			blockcache[i].out_buffer = NULL;
		}
	}

	if (blockcache) {
		kmm_free(blockcache);
		blockcache = NULL;
	}
}
