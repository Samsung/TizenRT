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
#include "libelf.h"

#ifdef CONFIG_COMPRESSED_BINARY
#include <tinyara/binfmt/compression/compress_read.h>
#endif
/****************************************************************************
 * Private Declarations
 ****************************************************************************/

/* Number of blocks to be caching */
static unsigned int number_blocks_caching;

/* Size of each blocks for caching */
static unsigned int cache_blocks_size;

/* Length of file w/o binary header */
static unsigned int file_len;

/* Number of blocks for caching */
static unsigned int number_of_blocks;

/* Pointer to block_cache_t list to be used for holding ELF blocks */
static block_cache_t *blockcache;

/* Pointers for maintaining doubly linked list of blockcache */
static block_cache_t *head;		/* Pointer to least priority block for caching */
static block_cache_t *tail;		/* Pointer to highest priority block for caching */
static block_cache_t *most_accessed;	/* Pointer to most accessed block for quick access */

/* Number of requests for the most accessed block in blockcache list */
static unsigned int max_accessed_count;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: elf_cache_blocks_to_read
 *
 * Description:
 *   Assign values to first_block, last_block and number of blocks to be read
 *   from elf file in this particular call to elf_cache_read
 *
 * Returned Value:
 *   None
 ****************************************************************************/
static void elf_cache_blocks_to_read(int *first_block, int *last_block, int *no_blocks, int offset, int readsize)
{
	int blocksize;

	blocksize = cache_blocks_size;

	*first_block = offset / blocksize;
	*last_block = (offset + readsize) / blocksize;
	*no_blocks = *last_block - *first_block + 1;
}

/****************************************************************************
 * Name: elf_cache_lseek_block
 *
 * Description:
 *   Seek to start of "block_number" block in elf blocks section
 *
 * Returned Value:
 *   Starting offset (positive) of 'block_number' block in binary on Success
 *   Negative value on Failure
 ****************************************************************************/
static off_t elf_cache_lseek_block(int filfd, uint16_t binary_header_size, int block_number)
{
	off_t rpos;
	off_t actual_offset;

	actual_offset = binary_header_size + block_number * cache_blocks_size;

	/* Seek to location of this block in actual ELF file */
	rpos = lseek(filfd, actual_offset, SEEK_SET);

	binfo("filfd: %d actual_offset: %d rpos: %d\n", filfd, actual_offset, rpos);

	if (rpos != actual_offset) {
		int errval = get_errno();
		berr("Failed to seek to position %lu: %d\n", (unsigned long)actual_offset, errval);
		return -errval;
	}

	return rpos;
}

/****************************************************************************
 * Name: elf_cache_read_block
 *
 * Description:
 *   Read 'block_number' block from elf blocks section into
 *   provided respective block numbers 'out_buffer' variable.
 *
 * Returned Value:
 *   Number of bytes read into out_buffer on Success
 *   Negative value on Failure
 ****************************************************************************/
static off_t elf_cache_read_block(int filfd, uint16_t binary_header_size, FAR uint8_t *buf, int block_number)
{
	off_t rpos;
	size_t readsize;
	ssize_t nbytes;

	binfo("filfd: %d block_number: %d binary_header_size: %d\n", filfd, block_number, binary_header_size);

	/* Seek to location of 'block_number' block in elf file for uncompressed elf */
#ifdef CONFIG_COMPRESSED_BINARY
	rpos = binary_header_size + (block_number * cache_blocks_size);
#else
	rpos = elf_cache_lseek_block(filfd, binary_header_size, block_number);
#endif


	if (rpos < 0) {
		berr("Failed to seek to offset of block number %d\n", block_number);
		return rpos;
	}

	/* Last unaligned blocks to be read with its actual size and not with blocksize;*/
	if (block_number == number_of_blocks - 1) {
		readsize = file_len % cache_blocks_size;
	} else {
		readsize = cache_blocks_size;
	}

#ifdef CONFIG_COMPRESSED_BINARY
	/* Read readsize bytes from offset from uncompressed file into user buffer */
	nbytes = compress_read(filfd, binary_header_size, buf, readsize, rpos - binary_header_size);
#else
	/* Read actual data to 'block_number's buf */
	nbytes = read(filfd, buf, readsize);
#endif

	binfo("readsize: %d nbytes: %d rpos: %d\n", readsize, nbytes, rpos);

	if (nbytes != readsize) {
		int errval = get_errno();
		berr("Read failed for size (%d) errno(%d)\n", readsize, errval);
		return -errval;
	}

	return nbytes;
}

/****************************************************************************
 * Name: elf_cache_update_blockcache_list
 *
 * Description:
 *   Update blockcache list based on whether 'block_number' block in
 *   blockwise-elf binary is already cached or not. Also, maintain
 *   head, tail and most_accessed pointers.
 *   tail = most recently cached block (highest priority block for keeping
 *          cached)
 *   head = oldest cached block (lowest priority for keeping it cached).
 *          If a new block needs to be cached, cache to this blockcache element.
 *   most_accessed = pointer to most accessed block in blockcache list.
 *          Needed for easy access to most accessed block from comp. binary.
 *
 * Returned Value:
 *   Index in blockcache list where 'block_number' from elf
 *   binary is cached state. Return Negative value on failure.
 ****************************************************************************/
static unsigned int elf_cache_update_blockcache_list(int block_number, int filfd, uint16_t binary_header_size)
{
	unsigned int blockcache_index;		/* Which blockcache element has needed ELF data for read */
	block_cache_t *ptr;					/* Pointer to element in blockcache list which will be updated */
	block_cache_t *temp;				/* Used when updating blockcache list */
	bool flag_for_caching;				/* 1 = Block needs to be cached, 0 = Block already cached */
	bool update_most_accessed;			/* 1 = Most accessed block is being removed from blockcache.
										 *     Need to update most accessed pointer from scratch */
	int size;

	binfo("filfd: %d block_number: %d\n", filfd, block_number);

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

	/* If block_number block from elf binary needs to be cached */
	if (flag_for_caching == true) {

		/* If most_accessed == head, need to update most_accessed */
		if (head == most_accessed)
			update_most_accessed = true;

		/* Detach head, Reassign head to head->next */
		ptr = head;
		head = ptr->next;
		head->prev = ptr->prev;

		/* Read elf 'block_number' block into respective 'out_buffer' */
		size = elf_cache_read_block(filfd, binary_header_size, ptr->out_buffer, block_number);
		if (size < 0) {
			berr("Read for block %d failed\n", block_number);
			return ERROR;
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

		/* Update blockcache_index into which block is cached */
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
 * Name: elf_cache_read
 *
 * Description:
 *   Read bytes from the elf file using 'offset' and 'readsize' info
 *   provided for cached buffer.  The data is read into 'buffer'. Offset
 *   value here is offset from start of elf binary (excluding binary
 *   header).
 *
 * Returned Value:
 *   Number of bytes read into buffer on Success
 *   Negative value on failure
 ****************************************************************************/
int elf_cache_read(int filfd, uint16_t binary_header_size, FAR uint8_t *buffer, size_t readsize, off_t offset)
{
	int first_block;
	int last_block;
	int no_blocks;
	int block_number;		/* Block number in an ELF file */
	int actual_offset;		/* Offset from start of ELF file */
	int block_size_to_write;	/* Size to write into buffer from cached block */
	int buffer_pos;			/* Position in buffer to start writing from */
	int blocksize;			/* Blocksize used by the binary */
	unsigned int blockcache_index;	/* Which blockcache element has needed ELF data for read */

	binfo("filfd: %d readsize: %d offset: %d\n", filfd, readsize, offset);

	/* Setting first block, end block and number of blocks to read */
	blocksize = cache_blocks_size;
	elf_cache_blocks_to_read(&first_block, &last_block, &no_blocks, offset, readsize);
	if (first_block < 0 || no_blocks < 0) {
		berr("Incorrect first_block, no_blocks info\n");
		buffer_pos = ERROR;
		goto error_cache_read;
	}

	block_number = first_block;
	buffer_pos = 0;
	/* Actual Offset in ELF file is same as Offset passed to this function */
	actual_offset = offset;

	/* Reading from first_block to last_block. Then writing to buffer. */
	for (; block_number < first_block + no_blocks; block_number++) {

		/* Update blockcache list and get data into one of the blockcache elements */
		blockcache_index = elf_cache_update_blockcache_list(block_number, filfd, binary_header_size);
		if (blockcache_index >= number_blocks_caching) {
			buffer_pos = ERROR;
			goto error_cache_read;
		}

		if (block_number == first_block) {
			/*
			 * Read appropriate number of bytes to buffer from this block using readsize and offset info.
			 * If start_offset + readsize < end_offset, write from start_offset to start_offset + readsize.
			 * Otherwise, write from start_offset to end_offset into buffer.
			 */
			block_size_to_write = (((block_number + 1) * blocksize - 1) > (actual_offset + readsize - 1)) ? readsize : ((block_number + 1) * blocksize - actual_offset);
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

error_cache_read:
	return buffer_pos;
}

/****************************************************************************
 * Name: elf_cache_init
 *
 * Description:
 *   IInitialize the cache blocks
 *
 * Returned value:
 *   OK (0) on Success
 *   Negative value on Failure
 ****************************************************************************/
int elf_cache_init(int filfd, uint16_t offset, off_t filelen)
{
	int ret = OK;

	binfo("filfd: %d offset: %u filelen: %d\n", filfd, offset, filelen);

	/* Initialize the ELF params */
	number_blocks_caching = CONFIG_ELF_CACHE_BLOCKS_COUNT;
	cache_blocks_size = CONFIG_ELF_CACHE_BLOCK_SIZE;
	file_len = filelen;
	number_of_blocks = file_len / cache_blocks_size;

	/* Set number of blocks to use for caching */
	if (CONFIG_ELF_CACHE_BLOCKS_COUNT > (CUTOFF_RATIO_CACHE_BLOCKS) * (number_of_blocks)) {
		number_blocks_caching = (CUTOFF_RATIO_CACHE_BLOCKS) * (number_of_blocks);
	}

	/* Extra unaligned data apart from blocksize */
	if (file_len % cache_blocks_size) {
		number_of_blocks++;
		number_blocks_caching++;
	}

	/* Minimum 2 blocks needed for caching logic to work */
	if (number_blocks_caching < 2) {
		number_blocks_caching = 2;
	}

	/* Initialize max_accesed_count to 0 */
	max_accessed_count = 0;

	blockcache = (block_cache_t *)kmm_malloc(number_blocks_caching * sizeof(block_cache_t));
	if (!blockcache) {
		berr("Failed kmm_malloc for blockcache\n");
		elf_cache_uninit();
		return -ENOMEM;
	}

	/* Initialize blockcache list */
	for (int i = 0; i < number_blocks_caching; i++) {
		blockcache[i].out_buffer = (unsigned char *)kmm_malloc(cache_blocks_size);

		if (!blockcache[i].out_buffer) {
			berr("Failed kmm_malloc for blockcache's out_buffer\n");
			elf_cache_uninit();
			return -ENOMEM;
		}

		blockcache[i].block_number = -1;
		blockcache[i].no_requests_for_block = 0;
		blockcache[i].index_block_cache = i;
		blockcache[i].next = &blockcache[(i + 1) % number_blocks_caching];
		blockcache[i].prev = &blockcache[(i - 1) % number_blocks_caching];
	}

	/* Assign head, tail and most_accessed pointers */
	head = &blockcache[0];
	tail = &blockcache[number_blocks_caching - 1];
	most_accessed = tail;

	head->prev = NULL;
	tail->next = NULL;

	return ret;
}

/****************************************************************************
 * Name: elf_cache_uninit
 *
 * Description:
 *   Release buffers initialized during elf_cache_init
 *
 * Returned Value:
 *   None
 ****************************************************************************/
void elf_cache_uninit(void)
{
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
