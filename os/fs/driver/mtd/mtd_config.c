/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * drivers/mtd/mtd_config.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Copyright (C) 2013 Ken Pettit. All rights reserved.
 *   Author: Ken Pettit <pettitkd@gmail.com>
 *           With Updates from Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * dev_config provides an interface for saving and retrieving
 *            application configuration data to a standard MTD partition.
 *            It accepts an MTD pointer (presumable a partition) and
 *            publishes a /dev/config device file for accessing via
 *            defined ioctl calls to set and get config data.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <poll.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/fs/fs.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/mtd.h>
#include <tinyara/configdata.h>
#include <tinyara/common_logs/common_logs.h>

#ifdef CONFIG_MTD_CONFIG

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Define the current format version */

#define CONFIGDATA_FORMAT_VERSION       1
#define CONFIGDATA_BLOCK_HDR_SIZE       3

/* Define the erased state of the MTD device, if not already defined */

#ifndef CONFIG_MTD_CONFIG_ERASEDVALUE
#define CONFIG_MTD_CONFIG_ERASEDVALUE 0xff
#endif

#define MTD_ERASED_ID     ((CONFIG_MTD_CONFIG_ERASEDVALUE << 8) | \
							CONFIG_MTD_CONFIG_ERASEDVALUE)

#define MTD_ERASED_FLAGS  CONFIG_MTD_CONFIG_ERASEDVALUE

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct mtdconfig_struct_s {
	FAR struct mtd_dev_s *mtd;	/* Contained MTD interface */
	sem_t exclsem;				/* Supports mutual exclusion */
	uint32_t blocksize:14;		/* Size of blocks in contained MTD */
	uint32_t erasesize:18;		/* Size of erase block  in contained MTD */
	size_t nblocks;				/* Number of blocks available */
	size_t neraseblocks;		/* Number of erase blocks available */
	off_t readoff;				/* Read offset (for hexdump) */
	FAR uint8_t *buffer;		/* Temp block read buffer */
};

struct mtdconfig_header_s {
	uint8_t flags;				/* Entry control flags */
	uint8_t instance;			/* Instance of the item */
	uint16_t id;				/* ID of the config data item */
	uint16_t len;				/* Length of the data block */
} packed_struct;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int mtdconfig_open(FAR struct file *filep);
static int mtdconfig_close(FAR struct file *filep);
static ssize_t mtdconfig_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static int mtdconfig_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
#ifndef CONFIG_DISABLE_POLL
static int mtdconfig_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations mtdconfig_fops = {
	mtdconfig_open,				/* open */
	mtdconfig_close,			/* close */
	mtdconfig_read,				/* read */
	0,							/* write */
	0,							/* seek */
	mtdconfig_ioctl				/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, mtdconfig_poll		/* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mtdconfig_readbytes
 *
 *    Reads bytes from the contained MTD device.  This will either usee
 *    the read function or if that is not available, the bread with a copy.
 *
 ****************************************************************************/

static int mtdconfig_readbytes(FAR struct mtdconfig_struct_s *dev, int offset, FAR uint8_t *pdata, int readlen)
{
	off_t bytestoread = readlen;
	off_t bytesthisblock, firstbyte;
	off_t block, index;
	int ret = OK;
	size_t bytes;

	/* Test if read interface supported.  If it is, use it directly */

	if ((dev->mtd->read == NULL) && (readlen < dev->blocksize)) {
		/* Read interface available.  Read directly to buffer */

		bytes = MTD_READ(dev->mtd, offset, readlen, pdata);
		if (bytes != readlen) {
			/* Error reading data! */

			ret = -EIO;
		}
	} else {
		/* Read interface not available, do a block read into our buffer */

		block = offset / dev->blocksize;
		firstbyte = offset - (block * dev->blocksize);
		bytesthisblock = dev->blocksize - firstbyte;
		if (bytesthisblock > readlen) {
			bytesthisblock = readlen;
		}

		index = 0;

		while (bytestoread > 0) {
			if (bytesthisblock < dev->blocksize || bytestoread < dev->blocksize) {
				/* Copy to temp buffer first...don't need the whole block */

				bytes = MTD_BREAD(dev->mtd, block, 1, dev->buffer);
				if (bytes != 1) {
					/* Error reading data!  */

					ret = -EIO;
					goto errout;
				}

				/* Copy data to the output buffer */

				memcpy(&pdata[index], &dev->buffer[firstbyte], bytesthisblock);
			} else {
				/* We are reading a whole block.  Read directly to buffer */

				bytes = MTD_BREAD(dev->mtd, block, 1, &pdata[index]);
				if (bytes != 1) {
					/* Error reading data!  */

					ret = -EIO;
					goto errout;
				}
			}

			/* Update values for next block read */

			bytestoread -= bytesthisblock;
			index += bytesthisblock;
			bytesthisblock = dev->blocksize;
			if (bytesthisblock > bytestoread) {
				bytesthisblock = bytestoread;
			}

			firstbyte = 0;
			block++;
		}
	}

errout:
	return ret;
}

/****************************************************************************
 * Name: mtdconfig_writebytes
 *
 *    Writes bytes to the contained MTD device.  This will either usee
 *    the byte write function or if that is not available, the bwrite.
 *
 ****************************************************************************/

static int mtdconfig_writebytes(FAR struct mtdconfig_struct_s *dev, int offset, FAR const uint8_t *pdata, int writelen)
{
	int ret = OK;

#ifdef CONFIG_MTD_BYTE_WRITE

	/* Test if this MTD device supports byte write */

	if (dev->mtd->write != NULL) {
		ret = MTD_WRITE(dev->mtd, offset, writelen, pdata);
	} else
#endif

		/* Perform the write using the block write method of the MTD */

	{
		uint16_t block;
		uint16_t index;
		off_t bytes_this_block;
		off_t bytes_written = 0;

		while (writelen) {
			/* Read existing data from the block into the buffer */

			block = offset / dev->blocksize;
			ret = MTD_BREAD(dev->mtd, block, 1, dev->buffer);
			if (ret != 1) {
				ret = -EIO;
				goto errout;
			}

			index = offset - block * dev->blocksize;
			bytes_this_block = dev->blocksize - index;
			if (bytes_this_block > writelen) {
				bytes_this_block = writelen;
			}

			/* Now write data to the block */

			memcpy(&dev->buffer[index], pdata, bytes_this_block);
			ret = MTD_BWRITE(dev->mtd, block, 1, dev->buffer);
			if (ret != 1) {
				ret = -EIO;
				goto errout;
			}

			/* Update writelen, etc. */

			writelen -= bytes_this_block;
			pdata += bytes_this_block;
			offset += bytes_this_block;
			bytes_written += bytes_this_block;
		}

		/* Return the number of bytes written */

		ret = bytes_written;
	}

errout:
	return ret;
}

/****************************************************************************
 * Name: mtdconfig_findfirstentry
 *
 *    Locates the first config entry, even if it is empty.
 *
 * Returns:
 *     offset to the start of the entry.
 *
 ****************************************************************************/

static int mtdconfig_findfirstentry(FAR struct mtdconfig_struct_s *dev, FAR struct mtdconfig_header_s *phdr)
{
	off_t offset = CONFIGDATA_BLOCK_HDR_SIZE;
	uint8_t sig[CONFIGDATA_BLOCK_HDR_SIZE];
	bool found = false;
	int ret;
	uint16_t block;
	off_t bytes_left_in_block;
	uint16_t endblock;

	mtdconfig_readbytes(dev, 0, sig, sizeof(sig));	/* Read the signature bytes */
	if (sig[0] != 'C' || sig[1] != 'D' || sig[2] != CONFIGDATA_FORMAT_VERSION) {
		/* Config Data partition not formatted. */

		return 0;
	}
#ifdef CONFIG_MTD_CONFIG_RAM_CONSOLIDATE
	endblock = dev->neraseblocks;
#else
	if (dev->neraseblocks == 1) {
		endblock = 1;
	} else {
		endblock = dev->neraseblocks - 1;
	}
#endif

	/* Config is formatted.  Now loop until we find the first entry */

	while (!found) {
		/* Read headers until we find one that hasn't been released.
		 */

		ret = mtdconfig_readbytes(dev, offset, (uint8_t *)phdr, sizeof(struct mtdconfig_header_s));
		if (ret != OK) {
			return 0;
		}

		/* Test if this header has been released */

		if (phdr->flags != MTD_ERASED_FLAGS) {
			/* This entry has been released.  Advance to next entry */

			offset += sizeof(struct mtdconfig_header_s) + phdr->len;
			block = offset / dev->erasesize;
			bytes_left_in_block = (block + 1) * dev->erasesize - offset;
			if (bytes_left_in_block <= sizeof(*phdr)) {
				offset = (block + 1) * dev->erasesize + CONFIGDATA_BLOCK_HDR_SIZE;
				if (block + 1 >= endblock) {
					return 0;
				}
			}

			if (bytes_left_in_block == dev->erasesize) {
				/* Skip block hearder */

				offset += CONFIGDATA_BLOCK_HDR_SIZE;
			}
		} else {
			/* We found the first entry! */

			found = true;
		}
	}

	/* Return the offset of the first entry */

	return offset;
}

/****************************************************************************
 * Name: mtdconfig_findnextentry
 *
 *    Locates the next config entry starting at offset, even if it is empty.
 *
 * Returns:
 *     offset to the start of the next entry.
 *
 ****************************************************************************/

static int mtdconfig_findnextentry(FAR struct mtdconfig_struct_s *dev, off_t offset, FAR struct mtdconfig_header_s *phdr, uint16_t size)
{
	bool found = false;
	int ret;
	uint16_t block, bytes_left_in_block;
	uint16_t endblock;

#ifdef CONFIG_MTD_CONFIG_RAM_CONSOLIDATE
	endblock = dev->neraseblocks;
#else
	if (dev->neraseblocks == 1) {
		endblock = 1;
	} else {
		endblock = dev->neraseblocks - 1;
	}
#endif

	/* Loop until next entry found */

	while (!found) {
		/* Calculate offset of the next entry */

		offset += sizeof(struct mtdconfig_header_s) + phdr->len;
		if (offset % dev->erasesize == 0) {
			offset += CONFIGDATA_BLOCK_HDR_SIZE;
		}

		if (offset >= endblock * dev->erasesize) {
			return 0;
		}

		/* Test if too close to the end of the erase block */

		block = offset / dev->erasesize;
		bytes_left_in_block = (block + 1) * dev->erasesize - offset;
		if (bytes_left_in_block <= sizeof(*phdr)) {
			offset = (block + 1) * dev->erasesize + CONFIGDATA_BLOCK_HDR_SIZE;
			if (block + 1 >= endblock) {
				return 0;
			}
		}

		/* Read next header */

read_next:
		ret = mtdconfig_readbytes(dev, offset, (uint8_t *)phdr, sizeof(*phdr));
		if (ret != OK) {
			return 0;
		}

		/* Test if this header has is still active */

		if (phdr->flags == MTD_ERASED_FLAGS) {
			if (phdr->id == MTD_ERASED_ID) {
				/* If we are searching for free space, then check
				 * if this space is large enough.  If it is, then
				 * we are done.
				 */

				block = offset / dev->erasesize;
				bytes_left_in_block = (block + 1) * dev->erasesize - offset;
				if (size > 0 && bytes_left_in_block >= size + sizeof(*phdr)) {
					/* Free entry of large enough size found */

					break;
				}

				/* Advance to next erase block and continue search */

				offset = (block + 1) * dev->erasesize + CONFIGDATA_BLOCK_HDR_SIZE;
				if (block + 1 >= endblock) {
					return 0;
				}

				/* Don't return to top of loop since we manually adjusted
				 * the offset, and don't want to go to the next entry.
				 */

				goto read_next;
			} else {
				/* If we are searching for existing, active entries
				 * only, then we are done ... we found one.
				 */

				if (size == 0) {
					found = true;
				}
			}
		}
	}

	return offset;
}

/****************************************************************************
 * Name: mtdconfig_ramconsolidate
 *
 *    Attempts to perform a RAM based consolidation of released
 *    items.  This requires a large enough free RAM block.  This
 *    method of consolidation is used when only a single erase
 *    block is available in the partition.
 *
 * Returns:
 *     offset to the next available entry (after consolidation)..
 *
 ****************************************************************************/

static off_t mtdconfig_ramconsolidate(FAR struct mtdconfig_struct_s *dev)
{
	FAR uint8_t *pBuf;
	FAR struct mtdconfig_header_s *phdr;
	struct mtdconfig_header_s hdr;
	uint16_t src_block = 0, dst_block = 0, blkper;
	off_t dst_offset = CONFIGDATA_BLOCK_HDR_SIZE;
	off_t src_offset = CONFIGDATA_BLOCK_HDR_SIZE;
	off_t bytes_left_in_block;
	uint8_t sig[CONFIGDATA_BLOCK_HDR_SIZE];
	int ret;

	/* Allocate a consolidation buffer */

	pBuf = (uint8_t *)kmm_malloc(dev->erasesize);
	if (pBuf == NULL) {
		/* Unable to allocate buffer, can't consolidate! */

		return 0;
	}

	/* Loop for all blocks and consolidate them */

	blkper = dev->erasesize / dev->blocksize;
	while (src_block < dev->neraseblocks) {
		/* Point to beginning of pBuf and read the next erase block */

		ret = MTD_BREAD(dev->mtd, src_block * blkper, blkper, pBuf);
		if (ret < 0) {
			/* Error doing block read */

			dst_offset = 0;
			goto errout;
		}

		/* Now erase the block */

		ret = MTD_ERASE(dev->mtd, src_block, 1);
		if (ret < 0) {
			/* Error erasing the block */

			dst_offset = 0;
			goto errout;
		}

		/* If this is block zero, then write a format signature */

		if (src_block == 0) {
			sig[0] = 'C';
			sig[1] = 'D';
			sig[2] = CONFIGDATA_FORMAT_VERSION;
			mtdconfig_writebytes(dev, 0, sig, sizeof(sig));
		}

		/* Copy active items back to the MTD device */

		while (src_offset < dev->erasesize) {
			phdr = (FAR struct mtdconfig_header_s *)&pBuf[src_offset];
			if (phdr->id == MTD_ERASED_ID) {
				/* No more data in this erase block */

				src_offset = dev->erasesize;
				continue;
			}

			if (phdr->flags == MTD_ERASED_FLAGS) {
				/* This is an active entry.  Copy it.  Check if it
				 * fits in the current destination block.
				 */

				bytes_left_in_block = (dst_block + 1) * dev->erasesize - dst_offset;
				if (bytes_left_in_block < sizeof(*phdr) + phdr->len) {
					/* Item won't fit in the destination block.  Move to
					 * the next block
					 */

					dst_block++;
					dst_offset = dst_block * dev->erasesize + CONFIGDATA_BLOCK_HDR_SIZE;

					/* Test for program bug.  We shouldn't ever overflow
					 * even if no entries were inactive.
					 */

					DEBUGASSERT(dst_block != dev->neraseblocks);
				}

				/* Now Write the item to the current dst_offset location */

				ret = mtdconfig_writebytes(dev, dst_offset, (uint8_t *)phdr, sizeof(hdr));
				if (ret < 0) {
					dst_offset = 0;
					goto errout;
				}

				dst_offset += sizeof(hdr);
				ret = mtdconfig_writebytes(dev, dst_offset, &pBuf[src_offset + sizeof(hdr)], phdr->len);
				dst_offset += phdr->len;

				/* Test if enough space in dst block for another header */

				if (dst_offset + sizeof(hdr) >= (dst_block + 1) * dev->erasesize || dst_offset == (dst_block + 1) * dev->erasesize) {
					dst_block++;
					dst_offset = dst_block * dev->erasesize + CONFIGDATA_BLOCK_HDR_SIZE;
				}
			}

			/* Increment past the current source item */

			src_offset += sizeof(hdr) + phdr->len;
			if (src_offset + sizeof(hdr) > dev->erasesize) {
				src_offset = dev->erasesize;
			}

			DEBUGASSERT(src_offset <= dev->erasesize);
		}

		/* Increment to next source block */

		src_block++;
		src_offset = CONFIGDATA_BLOCK_HDR_SIZE;
	}

errout:
	kmm_free(pBuf);
	return dst_offset;
}

/****************************************************************************
 * Name: mtdconfig_consolidate
 *
 *    Performs consolidation by writing erase block zero to the
 *    reserved block at the end, erasing block zero, then
 *    walking through each item and copying them to the newly
 *    erased block.  It erases all blocks to the end of the
 *    partition as it goes.
 *
 * Returns:
 *     offset to the next available entry (after consolidation)..
 *
 ****************************************************************************/

#ifndef CONFIG_MTD_CONFIG_RAM_CONSOLIDATE
static off_t mtdconfig_consolidate(FAR struct mtdconfig_struct_s *dev)
{
	off_t src_block, dst_block;
	off_t src_offset, dst_offset;
	uint16_t blkper, x, bytes, bytes_left_in_block;
	struct mtdconfig_header_s hdr;
	int ret;
	uint8_t sig[CONFIGDATA_BLOCK_HDR_SIZE];
	uint8_t *pBuf;

	/* Prepare to copy block 0 to the last block (erase blocks) */

	src_block = 0;
	dst_block = dev->neraseblocks - 1;

	/* Ensure the last block is erased */

	MTD_ERASE(dev->mtd, dst_block, 1);
	blkper = dev->erasesize / dev->blocksize;
	dst_block *= blkper;		/* Convert to read/write blocks */

	/* Allocate a small buffer for moving data */

	pBuf = (uint8_t *)kmm_malloc(dev->blocksize);
	if (pBuf == NULL) {
		return 0;
	}

	/* Now copy block zero to last block */

	for (x = 0; x < blkper; x++) {
		ret = MTD_BREAD(dev->mtd, src_block++, 1, dev->buffer);
		if (ret < 0) {
			/* I/O Error! */

			goto errout;
		}

		ret = MTD_BWRITE(dev->mtd, dst_block++, 1, dev->buffer);
		if (ret < 0) {
			/* I/O Error! */

			goto errout;
		}
	}

	/* Erase block zero and write a format signature. */

	MTD_ERASE(dev->mtd, 0, 1);
	sig[0] = 'C';
	sig[1] = 'D';
	sig[2] = CONFIGDATA_FORMAT_VERSION;
	mtdconfig_writebytes(dev, 0, sig, sizeof(sig));

	/* Now consolidate entries */

	src_block = 1;
	dst_block = 0;
	src_offset = src_block * dev->erasesize + CONFIGDATA_BLOCK_HDR_SIZE;
	dst_offset = CONFIGDATA_BLOCK_HDR_SIZE;

	while (src_block < dev->neraseblocks) {
		/* Scan all headers and move them to the src_offset */

retry_relocate:
		MTD_READ(dev->mtd, src_offset, sizeof(hdr), (uint8_t *)&hdr);
		if (hdr.flags == MTD_ERASED_FLAGS) {
			/* Test if the source entry is active or if we are at the end
			 * of data for this erase block.
			 */

			if (hdr.id == MTD_ERASED_ID) {
				/* No more data in this erase block.  Advance to the
				 * next one.
				 */

				src_offset = (src_block + 1) * dev->erasesize + CONFIGDATA_BLOCK_HDR_SIZE;
			} else {
				/* Test if this entry will fit in the current destination block */

				bytes_left_in_block = (dst_block + 1) * dev->erasesize - dst_offset;
				if (hdr.len + sizeof(hdr) > bytes_left_in_block) {
					/* Item doesn't fit in the block.  Advance to the next one */

					/* Update control variables */

					dst_block++;
					dst_offset = dst_block * dev->erasesize + CONFIGDATA_BLOCK_HDR_SIZE;

					DEBUGASSERT(dst_block != src_block);

					/* Retry the relocate */

					goto retry_relocate;
				}

				/* Copy this entry to the destination */

				mtdconfig_writebytes(dev, dst_offset, (uint8_t *)&hdr, sizeof(hdr));
				src_offset += sizeof(hdr);
				dst_offset += sizeof(hdr);

				/* Now copy the data */

				while (hdr.len) {
					bytes = hdr.len;
					if (bytes > dev->blocksize) {
						bytes = dev->blocksize;
					}

					/* Move the data. */

					mtdconfig_readbytes(dev, src_offset, pBuf, bytes);
					mtdconfig_writebytes(dev, dst_offset, pBuf, bytes);

					/* Update control variables */

					hdr.len -= bytes;
					src_offset += bytes;
					dst_offset += bytes;
				}
			}
		} else {
			/* This item has been released.  Skip it! */

			src_offset += sizeof(hdr) + hdr.len;
			if (src_offset + sizeof(hdr) >= (src_block + 1) * dev->erasesize || src_offset == (src_block + 1) * dev->erasesize) {
				/* No room left at end of source block */

				src_offset = (src_block + 1) * dev->erasesize + CONFIGDATA_BLOCK_HDR_SIZE;
			}
		}

		/* Test if we are out of space in the src block */

		if (src_offset + sizeof(hdr) >= (src_block + 1) * dev->erasesize) {
			/* No room at end of src block for another header.  Go to next
			 * source block.
			 */

			src_offset = (src_block + 1) * dev->erasesize + CONFIGDATA_BLOCK_HDR_SIZE;
		}

		/* Test if we advanced to the next block.  If we did, then erase the
		 * old block.
		 */

		if (src_block != src_offset / dev->erasesize) {
			/* Erase the block ... we have emptied it */

			MTD_ERASE(dev->mtd, src_block, 1);
			src_block++;
		}

		/* Test if we are out of space in the dst block */

		if (dst_offset + sizeof(hdr) >= (dst_block + 1) * dev->erasesize) {
			/* No room at end of dst block for another header.  Go to next block. */

			dst_block++;
			dst_offset = dst_block * dev->erasesize + CONFIGDATA_BLOCK_HDR_SIZE;
			DEBUGASSERT(dst_block != src_block);
		}
	}

errout:
	kmm_free(pBuf);
	return 0;
}
#endif							/* CONFIG_MTD_CONFIG_RAM_CONSOLIDATE */

/****************************************************************************
 * Name: mtdconfig_open
 ****************************************************************************/

static int mtdconfig_open(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct mtdconfig_struct_s *dev = inode->i_private;
	int ret;

	/* Get exclusive access to the device */

	ret = sem_wait(&dev->exclsem);
	if (ret < 0) {
		ret = -errno;
		goto errout;
	}

	dev->readoff = 0;

errout:
	return ret;
}

/****************************************************************************
 * Name: mtdconfig_close
 ****************************************************************************/

static int mtdconfig_close(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct mtdconfig_struct_s *dev = inode->i_private;

	/* Release exclusive access to the device */

	sem_post(&dev->exclsem);
	return OK;
}

/****************************************************************************
 * Name: mtdconfig_read
 ****************************************************************************/

static ssize_t mtdconfig_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct mtdconfig_struct_s *dev = inode->i_private;
	size_t bytes;

	if (dev->readoff >= dev->neraseblocks * dev->erasesize) {
		return 0;
	}

	/* Read data from the file */

	bytes = MTD_READ(dev->mtd, dev->readoff, len, (uint8_t *)buffer);
	dev->readoff += bytes;
	return bytes;
}

/****************************************************************************
 * Name: mtdconfig_findentry
 ****************************************************************************/

static int mtdconfig_findentry(FAR struct mtdconfig_struct_s *dev, off_t offset, FAR struct config_data_s *pdata, FAR struct mtdconfig_header_s *phdr)
{
	uint16_t endblock;

#ifdef CONFIG_MTD_CONFIG_RAM_CONSOLIDATE
	endblock = dev->neraseblocks;
#else
	if (dev->neraseblocks == 1) {
		endblock = 1;
	} else {
		endblock = dev->neraseblocks - 1;
	}
#endif

	while (offset > 0 && (pdata->id != phdr->id || pdata->instance != phdr->instance)) {
		if (phdr->id == MTD_ERASED_ID) {
			/* Advance to the next block and continue the search */

			offset = (offset + dev->erasesize) / dev->erasesize;
			offset = offset * dev->erasesize + CONFIGDATA_BLOCK_HDR_SIZE;
			if (offset >= endblock * dev->erasesize) {
				/* Entry doesn't exist on the device */

				offset = 0;
				break;
			}

			/* Read the 1st header from the next block */

			mtdconfig_readbytes(dev, offset, (uint8_t *)phdr, sizeof(*phdr));
			if (phdr->flags == MTD_ERASED_FLAGS) {
				continue;
			}
		}

		/* Nope, not the last header.  Get the next one */

		offset = mtdconfig_findnextentry(dev, offset, phdr, 0);
	}

	return offset;
}

/****************************************************************************
 * Name: mtdconfig_setconfig
 ****************************************************************************/

static int mtdconfig_setconfig(FAR struct mtdconfig_struct_s *dev, FAR struct config_data_s *pdata)
{
	uint8_t sig[CONFIGDATA_BLOCK_HDR_SIZE];	/* Format signature bytes ("CD") */
	char retrycount = 0;
	int ret = -ENOSYS;
	off_t offset, bytes_left_in_block, bytes;
	uint16_t block;
	struct mtdconfig_header_s hdr;
	uint8_t ram_consolidate;

	/* Allocate a temp block buffer */

	dev->buffer = (FAR uint8_t *)kmm_malloc(dev->blocksize);

	/* Read and vaidate the signature bytes */

retry:
	offset = mtdconfig_findfirstentry(dev, &hdr);
	if (offset == 0) {
		/* Config Data partition not formatted. */

		if (retrycount) {
			ret = -ENOSYS;
			goto errout;
		}

		/* Try to format the config partition */

		ret = MTD_IOCTL(dev->mtd, MTDIOC_BULKERASE, 0);
		if (ret < 0) {
			goto errout;
		}

		/* Write a format signature */

		sig[0] = 'C';
		sig[1] = 'D';
		sig[2] = CONFIGDATA_FORMAT_VERSION;
		mtdconfig_writebytes(dev, 0, sig, sizeof(sig));

		/* Now go try to read the signature again (as verification) */

		retrycount++;
		goto retry;
	}

	/* Okay, the Config Data partition is formatted.  Check if the
	 * config item being written is already in the database.  If it
	 * is, we must mark it as obsolete before creating a new entry.
	 */

	offset = mtdconfig_findentry(dev, offset, pdata, &hdr);

	/* Test if the header was found. */

	if (offset > 0 && pdata->id == hdr.id && pdata->instance == hdr.instance) {
		/* Mark this entry as released */

		hdr.flags = (uint8_t)~MTD_ERASED_FLAGS;
		mtdconfig_writebytes(dev, offset, &hdr.flags, sizeof(hdr.flags));
	}

	/* Test if the new length is zero.  If it is, then we are
	 * deleting the entry.
	 */

	if (pdata->len == 0) {
		ret = OK;
		goto errout;
	}

	/* Now find a new entry for this config data */

	retrycount = 0;
retry_find:
	offset = mtdconfig_findfirstentry(dev, &hdr);
	if (offset > 0 && hdr.id == MTD_ERASED_ID) {
		block = offset / dev->erasesize;
		bytes_left_in_block = (block + 1) * dev->erasesize - offset;
		if (bytes_left_in_block < sizeof(hdr) + pdata->len) {
			/* Simulate an active block to search for the next one
			 * in the code below.
			 */

			hdr.id = 1;
		}
	}

	if (hdr.id != MTD_ERASED_ID) {
		/* Read the next entry */

		offset = mtdconfig_findnextentry(dev, offset, &hdr, pdata->len);
		if (offset == 0) {
			/* No free entries left on device! */

#ifdef CONFIG_MTD_CONFIG_RAM_CONSOLIDATE
			ram_consolidate = 1;
#else
			ram_consolidate = dev->neraseblocks == 1;
#endif
			if (ram_consolidate) {
				/* If we only have 1 erase block, then we must do a RAM
				 * assisted consolidation of released entries.
				 */

				if (retrycount) {
					/* Out of space! */

					ret = -ENOMEM;
					goto errout;
				}

				mtdconfig_ramconsolidate(dev);
				retrycount++;
				goto retry_find;
			}
#ifndef CONFIG_MTD_CONFIG_RAM_CONSOLIDATE
			else {
				if (retrycount) {
					/* Out of space! */

					ret = -ENOMEM;
					goto errout;
				}

				mtdconfig_consolidate(dev);
				retrycount++;
				goto retry_find;
			}
#endif
		}
	}

	/* Test if a new entry was found */

	if (offset > 0) {
		/* Save the data at this entry */

		hdr.id = pdata->id;
		hdr.instance = pdata->instance;
		hdr.len = pdata->len;
		hdr.flags = MTD_ERASED_FLAGS;

		mtdconfig_writebytes(dev, offset, (uint8_t *)&hdr, sizeof(hdr));
		bytes = mtdconfig_writebytes(dev, offset + sizeof(hdr), pdata->configdata, pdata->len);
		if (bytes != pdata->len) {
			/* Error writing data! */

			hdr.flags = MTD_ERASED_FLAGS;
			mtdconfig_writebytes(dev, offset, (uint8_t *)&hdr, sizeof(hdr.flags));
			ret = -EIO;
			goto errout;
		}

		ret = OK;
	}

errout:

	/* Free the buffer */

	kmm_free(dev->buffer);
	return ret;
}

/****************************************************************************
 * Name: mtdconfig_getconfig
 ****************************************************************************/

static int mtdconfig_getconfig(FAR struct mtdconfig_struct_s *dev, FAR struct config_data_s *pdata)
{
	int ret = -ENOSYS;
	off_t offset, bytes_to_read;
	struct mtdconfig_header_s hdr;

	/* Allocate a temp block buffer */

	dev->buffer = (FAR uint8_t *)kmm_malloc(dev->blocksize);
	if (dev->buffer == NULL) {
		return -ENOMEM;
	}

	/* Get the offset of the first entry.  This will also check
	 * the format signature bytes.
	 */

	offset = mtdconfig_findfirstentry(dev, &hdr);
	offset = mtdconfig_findentry(dev, offset, pdata, &hdr);

	/* Test if the header was found. */

	if (offset > 0 && (pdata->id == hdr.id && pdata->instance == hdr.instance)) {
		/* Entry found.  Read the data */

		bytes_to_read = hdr.len;
		if (bytes_to_read > pdata->len) {
			bytes_to_read = pdata->len;
		}

		/* Perform the read */

		ret = mtdconfig_readbytes(dev, offset + sizeof(hdr), pdata->configdata, bytes_to_read);
		if (ret != OK) {
			/* Error reading the data */

			ret = -EIO;
			goto errout;
		}

		ret = OK;
	}

errout:
	/* Free the buffer */

	kmm_free(dev->buffer);
	return ret;
}

/****************************************************************************
 * Name: mtdconfig_ioctl
 ****************************************************************************/

static int mtdconfig_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct mtdconfig_struct_s *dev = inode->i_private;
	FAR struct config_data_s *pdata;
	int ret = -ENOSYS;
	if (!arg) {
		return -EINVAL;
	}

	switch (cmd) {
	case CFGDIOC_SETCONFIG:

		/* Set the config item */

		pdata = (FAR struct config_data_s *)arg;
		ret = mtdconfig_setconfig(dev, pdata);
		break;

	case CFGDIOC_GETCONFIG:

		/* Get the config item */

		pdata = (FAR struct config_data_s *)arg;
		ret = mtdconfig_getconfig(dev, pdata);
		break;
	}

	return ret;
}

/****************************************************************************
 * Name: mtdconfig_poll
 ****************************************************************************/

#ifndef CONFIG_DISABLE_POLL
static int mtdconfig_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup)
{
	if (setup) {
		fds->revents |= (fds->events & (POLLIN | POLLOUT));
		if (fds->revents != 0) {
			sem_post(fds->sem);
		}
	}

	return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mtdconfig_register
 *
 * Description:
 *   Register a /dev/config device backed by an MTD
 *
 ****************************************************************************/

int mtdconfig_register(FAR struct mtd_dev_s *mtd)
{
	int ret = OK;
	struct mtdconfig_struct_s *dev;
	struct mtd_geometry_s geo;	/* Device geometry */

	dev = (struct mtdconfig_struct_s *)kmm_malloc(sizeof(struct mtdconfig_struct_s));
	if (dev) {
		/* Initialize the mtdconfig device structure */

		dev->mtd = mtd;
		sem_init(&dev->exclsem, 0, 1);

		/* Get the device geometry. (casting to uintptr_t first eliminates
		 * complaints on some architectures where the sizeof long is different
		 * from the size of a pointer).
		 */

		ret = MTD_IOCTL(mtd, MTDIOC_GEOMETRY, (unsigned long)((uintptr_t)&geo));
		if (ret < 0) {
			fdbg("%s : %d\n", clog_message_str[CMN_LOG_FILE_IOCTL_ERROR], ret);
			kmm_free(dev);
			goto errout;
		}

		dev->blocksize = geo.blocksize;
		dev->neraseblocks = geo.neraseblocks;
		dev->erasesize = geo.erasesize;
		dev->nblocks = geo.neraseblocks * geo.erasesize / geo.blocksize;

		(void)register_driver("/dev/config", &mtdconfig_fops, 0666, dev);
	} else {
		ret = -ENOMEM;
	}

errout:
	return ret;
}
#endif							/* CONFIG_MTD_CONFIG */
