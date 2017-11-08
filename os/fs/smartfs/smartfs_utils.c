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
 * fs/smartfs/smartfs_utils.c
 *
 *   Copyright (C) 2013-2014 Ken Pettit. All rights reserved.
 *   Author: Ken Pettit <pettitkd@gmail.com>
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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <queue.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

#include "smartfs.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
#define SET1BITFROMMSB 128		//Binary: 0b10000000
#define SET2BITSFROMMSB 192		//Binary: 0b11000000
#define SET3BITSFROMMSB 224		//Binary: 0b11100000
#define SET4BITSFROMMSB 240		//Binary: 0b11110000
#define SET5BITSFROMMSB 248		//Binary: 0b11111000
#define SET6BITSFROMMSB 252		//Binary: 0b11111100
#define SET7BITSFROMMSB 254		//Binary: 0b11111110
#define SET8BITSFROMMSB 255		//Binary: 0b11111111
#define NEG8BIT(A) ((uint8_t)(~A & 0x000000FF))
#endif
/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

#if defined(CONFIG_SMARTFS_MULTI_ROOT_DIRS) || \
	(defined(CONFIG_FS_PROCFS) && !defined(CONFIG_FS_PROCFS_EXCLUDE_SMARTFS))
static struct smartfs_mountpt_s *g_mounthead = NULL;
#endif

#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
extern uint16_t chunk_shift;
extern uint16_t used_block_divident;
extern size_t smart_sect_header_size;
#define CHUNK_SIZE                              (CONFIG_MTD_SMART_SECTOR_SIZE / (USED_ARRAY_SIZE * (1 << 3)))
#endif

#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
#define ENTRY_VALID(e) ((smartfs_rdle16(&(e)->flags) & SMARTFS_DIRENT_EMPTY) != \
						(SMARTFS_ERASEDSTATE_16BIT & SMARTFS_DIRENT_EMPTY)) && \
						((smartfs_rdle16(&(e)->flags) & SMARTFS_DIRENT_ACTIVE) == \
						(SMARTFS_ERASEDSTATE_16BIT & SMARTFS_DIRENT_ACTIVE))

#else
#define ENTRY_VALID(e) (((e)->flags & SMARTFS_DIRENT_EMPTY) != \
						(SMARTFS_ERASEDSTATE_16BIT & SMARTFS_DIRENT_EMPTY)) && \
						(((e)->flags & SMARTFS_DIRENT_ACTIVE) == \
						(SMARTFS_ERASEDSTATE_16BIT & SMARTFS_DIRENT_ACTIVE))

#endif

#ifdef CONFIG_SMARTFS_SECTOR_RECOVERY
sq_queue_t g_recovery_queue;

struct sector_queue_s {
	struct sector_queue_s *flink;
	uint8_t type;
	uint16_t sector;
};
#endif

#ifdef CONFIG_SMARTFS_JOURNALING

#if (CONFIG_SMARTFS_ERASEDSTATE == 0xFF)
#define AREA_UNUSED 0xFF
#define AREA_USED   0xF0
#define AREA_OLD_UNERASED 0x00
#else
#define AREA_UNUSED 0x00
#define AREA_USED   0x0F
#define AREA_OLD_UNERASED 0xFF
#endif

#define NO_CHANGE 0
#define NEXT_SECTOR 1
#define MOVE_AREA 2

#define SZ_UINT8 (sizeof(uint8_t)*8)
#define IS_ACTIVE(x, s) ((x)[(s)/SZ_UINT8] & (1<<(SZ_UINT8-((s)%SZ_UINT8)-1)))
#define SET_ACTIVE(x, s) ((x)[(s)/SZ_UINT8] |= (1<<(SZ_UINT8-((s)%SZ_UINT8)-1)))
#define SET_INACTIVE(x, s) ((x)[(s)/SZ_UINT8] &= ~(1<<(SZ_UINT8-((s)%SZ_UINT8)-1)))


enum journal_action_e {
	JOURNAL_LOG,
	JOURNAL_RESTORE,
	JOURNAL_MODIFY_LOG
};

static int smartfs_get_journal_area(struct smartfs_mountpt_s *fs);
static int read_logging_entry(struct smartfs_mountpt_s *fs, struct journal_transaction_manager_s *j_mgr, uint16_t *sector, uint16_t *offset);
static int read_logging_data(struct smartfs_mountpt_s *fs, struct journal_transaction_manager_s *j_mgr, uint16_t *sector, uint16_t *offset);
static int smartfs_set_transaction(struct smartfs_mountpt_s *fs, uint16_t sect, uint16_t offt, uint8_t action);
static int smartfs_redo_create(struct smartfs_mountpt_s *fs, uint16_t type, uint16_t firstsector);
static int smartfs_redo_sync(struct smartfs_mountpt_s *fs, uint16_t used);
static int process_transaction(struct smartfs_mountpt_s *fs);
static int restore_write_transactions(struct smartfs_mountpt_s *fs);
static int add_to_list(struct smartfs_mountpt_s *fs, struct journal_transaction_manager_s *j_mgr, uint16_t j_sector, uint16_t j_offset, enum journal_action_e);
static int remove_from_list(struct journal_transaction_manager_s *j_mgr, uint16_t sector);
int clear_journal_sectors(struct smartfs_mountpt_s *fs, struct journal_transaction_manager_s *j_mgr);
static int set_area_id_bits(struct smartfs_mountpt_s *fs, uint8_t id_bits);
static int smartfs_write_transaction(struct smartfs_mountpt_s *fs, struct journal_transaction_manager_s
									 *j_mgr, uint16_t *sector, uint16_t *offset);
#ifdef CONFIG_SMARTFS_JOURNAL_VERIFY
static int smartfs_verify_transaction(struct smartfs_mountpt_s *fs, uint16_t sector, uint16_t offset);
#endif
static uint8_t smartfs_calc_crc_entry(struct journal_transaction_manager_s *j_mgr);
static uint8_t smartfs_calc_crc_data(struct journal_transaction_manager_s *j_mgr);
#endif
/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: smartfs_semtake
 ****************************************************************************/

void smartfs_semtake(struct smartfs_mountpt_s *fs)
{
	/* Take the semaphore (perhaps waiting) */

	while (sem_wait(fs->fs_sem) != 0) {
		/* The only case that an error should occur here is if
		 * the wait was awakened by a signal.
		 */

		ASSERT(*get_errno_ptr() == EINTR);
	}
}

/****************************************************************************
 * Name: smartfs_semgive
 ****************************************************************************/

void smartfs_semgive(struct smartfs_mountpt_s *fs)
{
	sem_post(fs->fs_sem);
}

/****************************************************************************
 * Name: smartfs_rdle16
 *
 * Description:
 *   Get a (possibly unaligned) 16-bit little endian value.
 *
 * Input Parameters:
 *   val - A pointer to the first byte of the little endian value.
 *
 * Returned Value:
 *   A uint16_t representing the whole 16-bit integer value
 *
 ****************************************************************************/

uint16_t smartfs_rdle16(FAR const void *val)
{
	return (uint16_t)((FAR const uint8_t *)val)[1] << 8 | (uint16_t)((FAR const uint8_t *)val)[0];
}

/****************************************************************************
 * Name: smartfs_wrle16
 *
 * Description:
 *   Put a (possibly unaligned) 16-bit little endian value.
 *
 * Input Parameters:
 *   dest - A pointer to the first byte to save the little endian value.
 *   val - The 16-bit value to be saved.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void smartfs_wrle16(FAR void *dest, uint16_t val)
{
	((FAR uint8_t *)dest)[0] = val & 0xff;	/* Little endian means LS byte first in byte stream */
	((FAR uint8_t *)dest)[1] = val >> 8;
}

/****************************************************************************
 * Name: smartfs_rdle32
 *
 * Description:
 *   Get a (possibly unaligned) 32-bit little endian value.
 *
 * Input Parameters:
 *   val - A pointer to the first byte of the little endian value.
 *
 * Returned Value:
 *   A uint32_t representing the whole 32-bit integer value
 *
 ****************************************************************************/

uint32_t smartfs_rdle32(FAR const void *val)
{
	/* Little endian means LS halfword first in byte stream */

	return (uint32_t)smartfs_rdle16(&((FAR const uint8_t *)val)[2]) << 16 | (uint32_t)smartfs_rdle16(val);
}

/****************************************************************************
 * Name: smartfs_wrle32
 *
 * Description:
 *   Put a (possibly unaligned) 32-bit little endian value.
 *
 * Input Parameters:
 *   dest - A pointer to the first byte to save the little endian value.
 *   val - The 32-bit value to be saved.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void smartfs_wrle32(uint8_t *dest, uint32_t val)
{
	/* Little endian means LS halfword first in byte stream */

	smartfs_wrle16(dest, (uint16_t)(val & 0xffff));
	smartfs_wrle16(dest + 2, (uint16_t)(val >> 16));
}

#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER

uint16_t get_used_byte_count(uint8_t *used)
{
	uint16_t retval = 0, i = 0;
	while ((i < USED_ARRAY_SIZE) && (used[i] == NEG8BIT(CONFIG_SMARTFS_ERASEDSTATE))) {
		i++;
	}
	retval = (i << (used_block_divident));
	if (i >= USED_ARRAY_SIZE) {
		goto out;
	}

#if (CONFIG_SMARTFS_ERASEDSTATE == 0x00)
	switch (used[i]) {
	case SET1BITFROMMSB:
		retval += (1 << chunk_shift);
		break;
	case SET2BITSFROMMSB:
		retval += (2 << chunk_shift);
		break;
	case SET3BITSFROMMSB:
		retval += (3 << chunk_shift);
		break;
	case SET4BITSFROMMSB:
		retval += (4 << chunk_shift);
		break;
	case SET5BITSFROMMSB:
		retval += (5 << chunk_shift);
		break;
	case SET6BITSFROMMSB:
		retval += (6 << chunk_shift);
		break;
	case SET7BITSFROMMSB:
		retval += (7 << chunk_shift);
		break;
	}
#elif (CONFIG_SMARTFS_ERASEDSTATE == 0xFF)
	switch (used[i]) {
	case NEG8BIT(SET1BITFROMMSB):
		retval += (1 << chunk_shift);
		break;
	case NEG8BIT(SET2BITSFROMMSB):
		retval += (2 << chunk_shift);
		break;
	case NEG8BIT(SET3BITSFROMMSB):
		retval += (3 << chunk_shift);
		break;
	case NEG8BIT(SET4BITSFROMMSB):
		retval += (4 << chunk_shift);
		break;
	case NEG8BIT(SET5BITSFROMMSB):
		retval += (5 << chunk_shift);
		break;
	case NEG8BIT(SET6BITSFROMMSB):
		retval += (6 << chunk_shift);
		break;
	case NEG8BIT(SET7BITSFROMMSB):
		retval += (7 << chunk_shift);
		break;
	}
#endif
out:
	return retval;
}

uint16_t get_leftover_used_byte_count(uint8_t *buffer, uint16_t base_index)
{
	uint16_t i, ending;
	int sector_data_length = (CONFIG_MTD_SMART_SECTOR_SIZE - sizeof(struct smartfs_chain_header_s) - smart_sect_header_size);
	if (base_index >= sector_data_length) {
		return sector_data_length;
	}

	base_index += sizeof(struct smartfs_chain_header_s);
	//We have to read from the last byte of the active chunk
	//But if the last byte is already written then
	//we will update corresponding bit of USED_BYTE in Chain Header
	//So, we can start from the byte before the last byte of the active chunk
	//And the index of that byte, suppose, nth byte, should be n-1
	if ((base_index + (CHUNK_SIZE - 1) - 1) < (CONFIG_MTD_SMART_SECTOR_SIZE - smart_sect_header_size - 1)) {
		ending = base_index + (CHUNK_SIZE - 1) - 1;
	} else {
		ending = CONFIG_MTD_SMART_SECTOR_SIZE - smart_sect_header_size - 1;
	}

	for (i = ending; i >= base_index && buffer[i] == CONFIG_SMARTFS_ERASEDSTATE; i--) ;
	i++;
	i -= sizeof(struct smartfs_chain_header_s);

	return i;
}

uint16_t get_used_byte_count_from_end(uint8_t *buffer)
{
	uint16_t i, ending = CONFIG_MTD_SMART_SECTOR_SIZE - smart_sect_header_size - 1;

	for (i = ending; i >= sizeof(struct smartfs_chain_header_s) && buffer[i] == (uint8_t)CONFIG_SMARTFS_ERASEDSTATE; i--) ;
	i++;
	i -= sizeof(struct smartfs_chain_header_s);

	return i;
}

int set_used_byte_count(uint8_t *used, uint16_t count)
{
	int i = 0, iter = (count >> (used_block_divident)), leftover_bits;
	leftover_bits = (count - (iter << (used_block_divident))) >> chunk_shift;

	if (count > (CONFIG_MTD_SMART_SECTOR_SIZE - sizeof(struct smartfs_chain_header_s)) || iter > USED_ARRAY_SIZE) {
		return ERROR;
	}
	while (i < iter) {
		used[i] = NEG8BIT(CONFIG_SMARTFS_ERASEDSTATE);
		i++;
	}

	if (i >= USED_ARRAY_SIZE) {
		goto out;
	}

#if (CONFIG_SMARTFS_ERASEDSTATE == 0x00)
	if (leftover_bits > 0) {
		switch (leftover_bits) {
		case 1:
			used[i] = SET1BITFROMMSB;
			break;
		case 2:
			used[i] = SET2BITSFROMMSB;
			break;
		case 3:
			used[i] = SET3BITSFROMMSB;
			break;
		case 4:
			used[i] = SET4BITSFROMMSB;
			break;
		case 5:
			used[i] = SET5BITSFROMMSB;
			break;
		case 6:
			used[i] = SET6BITSFROMMSB;
			break;
		case 7:
			used[i] = SET7BITSFROMMSB;
			break;
		}
	}
#elif (CONFIG_SMARTFS_ERASEDSTATE == 0xFF)
	if (leftover_bits > 0) {
		switch (leftover_bits) {
		case 1:
			used[i] = NEG8BIT(SET1BITFROMMSB);
			break;
		case 2:
			used[i] = NEG8BIT(SET2BITSFROMMSB);
			break;
		case 3:
			used[i] = NEG8BIT(SET3BITSFROMMSB);
			break;
		case 4:
			used[i] = NEG8BIT(SET4BITSFROMMSB);
			break;
		case 5:
			used[i] = NEG8BIT(SET5BITSFROMMSB);
			break;
		case 6:
			used[i] = NEG8BIT(SET6BITSFROMMSB);
			break;
		case 7:
			used[i] = NEG8BIT(SET7BITSFROMMSB);
			break;
		}
	}
#endif
out:
	return OK;
}
#endif
/****************************************************************************
 * Name: smartfs_mount
 *
 * Desciption: This function is called only when the mountpoint is first
 *   established.  It initializes the mountpoint structure and verifies
 *   that a valid SMART filesystem is provided by the block driver.
 *
 *   The caller should hold the mountpoint semaphore
 *
 ****************************************************************************/

int smartfs_mount(struct smartfs_mountpt_s *fs, bool writeable)
{
	FAR struct inode *inode;
	struct geometry geo;
	int ret = OK;
#if defined(CONFIG_SMARTFS_MULTI_ROOT_DIRS)
	struct smartfs_mountpt_s *nextfs;
#endif

	/* Assume that the mount is not successful */

	fs->fs_mounted = false;

	/* Check if there is media available */

	inode = fs->fs_blkdriver;
	if (!inode || !inode->u.i_bops || !inode->u.i_bops->geometry || inode->u.i_bops->geometry(inode, &geo) != OK || !geo.geo_available) {
		ret = -ENODEV;
		goto errout;
	}

	/* Make sure that that the media is write-able (if write access is needed) */

	if (writeable && !geo.geo_writeenabled) {
		ret = -EACCES;
		goto errout;
	}

	/* Get the SMART low-level format information to validate the device has been
	 * formatted and scan properly for logical to physical sector mapping.
	 */

	ret = FS_IOCTL(fs, BIOC_GETFORMAT, (unsigned long)&fs->fs_llformat);
	if (ret != OK) {
		fdbg("Error getting device low level format: %d\n", ret);
		goto errout;
	}

	/* Validate the low-level format is valid */

	if (!(fs->fs_llformat.flags & SMART_FMT_ISFORMATTED)) {
		fdbg("No low-level format found\n");
		ret = -ENODEV;
		goto errout;
	}

	/* Allocate a read/write buffer */

#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	/* Scan linked list of mounted file systems to find another FS with
	 * the same blockdriver.  We will reuse the buffers.
	 */

	nextfs = g_mounthead;
	while (nextfs != NULL) {
		/* Test if this FS uses the same block driver */

		if (nextfs->fs_blkdriver == fs->fs_blkdriver) {
			/* Yep, it's the same block driver.  Reuse the buffers.
			 * we can do this because we are protected by the same
			 * semaphore.
			 */

			fs->fs_rwbuffer = nextfs->fs_rwbuffer;
			fs->fs_workbuffer = nextfs->fs_workbuffer;
			break;
		}

		/* Advance to next FS */

		nextfs = nextfs->fs_next;
	}

	/* If we didn't find a FS above, then allocate some buffers */

	if (nextfs == NULL) {
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
		fs->fs_chunk_buffer = (char *)kmm_malloc(fs->fs_llformat.availbytes);
#endif
		fs->fs_rwbuffer = (char *)kmm_malloc(fs->fs_llformat.availbytes);
		fs->fs_workbuffer = (char *)kmm_malloc(256);
	}

	/* Now add ourselves to the linked list of SMART mounts */

	fs->fs_next = g_mounthead;
	g_mounthead = fs;

	/* Set our root directory sector based on the directory entry
	 * reported by the block driver (based on which device is
	 * associated with this mount.
	 */

	fs->fs_rootsector = SMARTFS_ROOT_DIR_SECTOR + fs->fs_llformat.rootdirnum;
#endif							/* CONFIG_SMARTFS_MULTI_ROOT_DIRS */

#if defined(CONFIG_FS_PROCFS) && !defined(CONFIG_FS_PROCFS_EXCLUDE_SMARTFS)
	/* Now add ourselves to the linked list of SMART mounts */

	fs->fs_next = g_mounthead;
	g_mounthead = fs;
#endif

#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
	fs->fs_chunk_buffer = (uint8_t *)kmm_malloc(fs->fs_llformat.availbytes);
#endif
	fs->fs_rwbuffer = (char *)kmm_malloc(fs->fs_llformat.availbytes);
	fs->fs_workbuffer = (char *)kmm_malloc(256);
	fs->fs_rootsector = SMARTFS_ROOT_DIR_SECTOR;

	/* We did it! */

	fs->fs_mounted = TRUE;

	fdbg("SMARTFS:\n");
	fdbg("\t    Sector size:     %d\n", fs->fs_llformat.sectorsize);
	fdbg("\t    Bytes/sector     %d\n", fs->fs_llformat.availbytes);
	fdbg("\t    Num sectors:     %d\n", fs->fs_llformat.nsectors);
	fdbg("\t    Free sectors:    %d\n", fs->fs_llformat.nfreesectors);
	fdbg("\t    Max filename:    %d\n", CONFIG_SMARTFS_MAXNAMLEN);
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	fdbg("\t    RootDirEntries:  %d\n", fs->fs_llformat.nrootdirentries);
#endif
	fdbg("\t    RootDirSector:   %d\n", fs->fs_rootsector);

errout:
	return ret;
}

/****************************************************************************
 * Name: smartfs_unmount
 *
 * Desciption: This function is called only when the mountpoint is being
 *   unbound.  If we are serving multiple directories, then we have to
 *   remove ourselves from the mount linked list, and potentially free
 *   the shared buffers.
 *
 *   The caller should hold the mountpoint semaphore
 *
 ****************************************************************************/

int smartfs_unmount(struct smartfs_mountpt_s *fs)
{
	int ret = OK;
	struct inode *inode;
#if defined(CONFIG_SMARTFS_MULTI_ROOT_DIRS) || \
	(defined(CONFIG_FS_PROCFS) && !defined(CONFIG_FS_PROCFS_EXCLUDE_SMARTFS))
	struct smartfs_mountpt_s *nextfs;
	struct smartfs_mountpt_s *prevfs;
	int count = 0;
	int found = FALSE;
#endif

#if defined(CONFIG_SMARTFS_MULTI_ROOT_DIRS) || \
	(defined(CONFIG_FS_PROCFS) && !defined(CONFIG_FS_PROCFS_EXCLUDE_SMARTFS))
	/* Start at the head of the mounts and search for our entry.  Also
	 * count the number of entries that match our blkdriver.
	 */

	nextfs = g_mounthead;
	prevfs = NULL;
	while (nextfs != NULL) {
		/* Test if this FS's blkdriver matches ours (it could be us) */

		if (nextfs->fs_blkdriver == fs->fs_blkdriver) {
			count++;
		}

		/* Test if this entry is our's */

		if (nextfs == fs) {
			found = TRUE;
		}

		/* Keep track of the previous entry until our's is found */

		if (!found) {
			/* Save this entry as the previous entry */

			prevfs = nextfs;
		}

		/* Advance to the next entry */

		nextfs = nextfs->fs_next;
	}

	/* Ensure we found our FS */

	if (!found) {
		/* Our entry not found!  Invalid unmount or bug somewhere */

		return -EINVAL;
	}

	/* If the count is only one, then we need to delete the shared
	 * buffers because we are the last ones.
	 */

	if (count == 1) {
		/* Close the block driver */

		if (fs->fs_blkdriver) {
			inode = fs->fs_blkdriver;
			if (inode) {
				if (inode->u.i_bops && inode->u.i_bops->close) {
					(void)inode->u.i_bops->close(inode);
				}
			}
		}

		/* Free the buffers */
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
		kmm_free(fs->fs_chunk_buffer);
#endif
		kmm_free(fs->fs_rwbuffer);
		kmm_free(fs->fs_workbuffer);

		/* Set the buffer's to invalid value to catch program bugs */

		fs->fs_rwbuffer = (char *)0xDEADBEEF;
		fs->fs_workbuffer = (char *)0xDEADBEEF;
	}

	/* Now removed ourselves from the linked list */

	if (fs == g_mounthead) {
		/* We were the first ones.  Set a new head */

		g_mounthead = fs->fs_next;
	} else {
		/* Remove from the middle of the list somewhere */

		prevfs->fs_next = fs->fs_next;
	}
#else
	if (fs->fs_blkdriver) {
		inode = fs->fs_blkdriver;
		if (inode) {
			if (inode->u.i_bops && inode->u.i_bops->close) {
				(void)inode->u.i_bops->close(inode);
			}
		}
	}

	/* Release the mountpoint private data */
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
	kmm_free(fs->fs_chunk_buffer);
#endif
	kmm_free(fs->fs_rwbuffer);
	kmm_free(fs->fs_workbuffer);
#endif

	return ret;
}

/****************************************************************************
 * Name: smartfs_finddirentry
 *
 * Description: Finds an entry in the filesystem as specified by relpath.
 *              If found, the direntry will be populated with information
 *              for accessing the entry.
 *
 *              If the final directory segment of relpath just before the
 *              last segment (the target file/dir) is valid, then the
 *              parentdirsector will indicate the logical sector number of
 *              the parent directory where a new entry should be created,
 *              and the filename pointer will point to the final segment
 *              (i.e. the "filename").
 *
 ****************************************************************************/

int smartfs_finddirentry(struct smartfs_mountpt_s *fs, struct smartfs_entry_s *direntry, const char *relpath, uint16_t *parentdirsector, const char **filename)
{
	int ret = -ENOENT;
	const char *segment;
	const char *ptr;
	uint16_t seglen;
	uint16_t depth = 0;
	uint16_t dirstack[CONFIG_SMARTFS_DIRDEPTH];
	uint16_t dirsector;
	uint16_t entrysize;
	uint16_t offset;
	struct smartfs_chain_header_s *header;
	struct smart_read_write_s readwrite;
	struct smartfs_entry_header_s *entry;
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
	int used_value;
#endif

	/* Initialize directory level zero as the root sector */

	dirstack[0] = fs->fs_rootsector;
	entrysize = sizeof(struct smartfs_entry_header_s) + fs->fs_llformat.namesize;

	/* Test if this is a request for the root directory */

	if (*relpath == '\0') {
		direntry->firstsector = fs->fs_rootsector;
		direntry->flags = SMARTFS_DIRENT_TYPE_DIR | 0777;
		direntry->utc = 0;
		direntry->dsector = 0;
		direntry->doffset = 0;
		direntry->dfirst = fs->fs_rootsector;
		direntry->name = NULL;
		direntry->datlen = 0;

		*parentdirsector = 0;	/* Our parent is the format sector I guess */
		return OK;
	}

	/* Parse through each segment of relpath */

	segment = relpath;
	while (segment != NULL && *segment != '\0') {
		/* Find the end of this segment.  It will be '/' or NULL. */

		ptr = segment;
		seglen = 0;
		while (*ptr != '/' && *ptr != '\0') {
			seglen++;
			ptr++;
		}

		strncpy(fs->fs_workbuffer, segment, seglen);
		fs->fs_workbuffer[seglen] = '\0';

		/* Search for "." and ".." as segment names */

		if (strcmp(fs->fs_workbuffer, ".") == 0) {
			/* Just ignore this segment.  Advance ptr if not on NULL */

			if (*ptr == '/') {
				ptr++;
			}

			segment = ptr;
			continue;
		} else if (strcmp(fs->fs_workbuffer, "..") == 0) {
			/* Up one level */

			if (depth == 0) {
				/* We went up one level past our mount point! */

				goto errout;
			}

			/* "Pop" to the previous directory level */

			depth--;
			if (*ptr == '/') {
				ptr++;
			}

			segment = ptr;
			continue;
		} else {
			/* Search for the entry in the current directory */

			dirsector = dirstack[depth];

			/* Read the directory */

			offset = 0xFFFF;

#if CONFIG_SMARTFS_ERASEDSTATE == 0xFF
			while (dirsector != 0xFFFF)
#else
			while (dirsector != 0)
#endif
			{
				/* Read the next directory in the chain */

				readwrite.logsector = dirsector;
				readwrite.count = fs->fs_llformat.availbytes;
				readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
				readwrite.offset = 0;
				ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
				if (ret < 0) {
					goto errout;
				}

				/* Point to next sector in chain */

				header = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;
				dirsector = SMARTFS_NEXTSECTOR(header);

				/* Search for the entry */

				offset = sizeof(struct smartfs_chain_header_s);
				entry = (struct smartfs_entry_header_s *)&fs->fs_rwbuffer[offset];
				while (offset < readwrite.count) {
					/* Test if this entry is valid and active */

					if (!(ENTRY_VALID(entry))) {
						/* This entry isn't valid, skip it */

						offset += entrysize;
						entry = (struct smartfs_entry_header_s *)
								&fs->fs_rwbuffer[offset];

						continue;
					}

					/* Test if the name matches */

					if (strncmp(entry->name, fs->fs_workbuffer, fs->fs_llformat.namesize) == 0) {
						/* We found it!  If this is the last segment entry,
						 * then report the entry.  If it isn't the last
						 * entry, then validate it is a directory entry and
						 * open it and continue searching.
						 */

						if (*ptr == '\0') {
							/* We are at the last segment.  Report the entry */

							/* Fill in the entry */

#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
							direntry->firstsector = smartfs_rdle16(&entry->firstsector);
							direntry->flags = smartfs_rdle16(&entry->flags);
							direntry->utc = smartfs_rdle32(&entry->utc);
#else
							direntry->firstsector = entry->firstsector;
							direntry->flags = entry->flags;
							direntry->utc = entry->utc;
#endif
							direntry->dsector = readwrite.logsector;
							direntry->doffset = offset;
							direntry->dfirst = dirstack[depth];
							if (direntry->name == NULL) {
								direntry->name = (char *)kmm_malloc(fs->fs_llformat.namesize + 1);
								if (direntry->name == NULL) {
									ret = ERROR;
									goto errout;
								}
							}

							memset(direntry->name, 0, fs->fs_llformat.namesize + 1);
							strncpy(direntry->name, entry->name, fs->fs_llformat.namesize);
							direntry->datlen = 0;

							/* Scan the file's sectors to calculate the length and perform
							 * a rudimentary check.
							 */

#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
							if ((smartfs_rdle16(&entry->flags) & SMARTFS_DIRENT_TYPE) == SMARTFS_DIRENT_TYPE_FILE) {
								dirsector = smartfs_rdle16(&entry->firstsector);
#else
							if ((entry->flags & SMARTFS_DIRENT_TYPE) == SMARTFS_DIRENT_TYPE_FILE) {
								dirsector = entry->firstsector;
#endif
								readwrite.count = sizeof(struct smartfs_chain_header_s);
								readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
								readwrite.offset = 0;

								while (dirsector != SMARTFS_ERASEDSTATE_16BIT) {
									/* Read the next sector of the file */

									readwrite.logsector = dirsector;
									ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
									if (ret < 0) {
										fdbg("Error in sector chain at %d!\n", dirsector);
										break;
									}
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
									if (SMARTFS_NEXTSECTOR(header) == SMARTFS_ERASEDSTATE_16BIT) {

										readwrite.count = fs->fs_llformat.availbytes;
										readwrite.buffer = (uint8_t *)fs->fs_chunk_buffer;

										ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
										if (ret < 0) {
											fdbg("Error %d reading sector %d header\n", ret, sf->currsector);
											break;
										}
										used_value = get_leftover_used_byte_count((uint8_t *)readwrite.buffer, get_used_byte_count((uint8_t *)header->used));
										direntry->datlen += used_value;
									} else {
										direntry->datlen += (fs->fs_llformat.availbytes - sizeof(struct smartfs_chain_header_s));
									}
									readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
#else
									/* Add used bytes to the total and point to next sector */
									if (SMARTFS_USED(header) != SMARTFS_ERASEDSTATE_16BIT) {
										direntry->datlen += SMARTFS_USED(header);
									}
#endif
									dirsector = SMARTFS_NEXTSECTOR(header);
								}
							}

							*parentdirsector = dirstack[depth];
							*filename = segment;
							ret = OK;
							goto errout;
						} else {
							/* Validate it's a directory */

#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
							if ((smartfs_rdle16(&entry->flags) & SMARTFS_DIRENT_TYPE) != SMARTFS_DIRENT_TYPE_DIR)
#else
							if ((entry->flags & SMARTFS_DIRENT_TYPE) != SMARTFS_DIRENT_TYPE_DIR)
#endif
							{
								/* Not a directory!  Report the error */

								ret = -ENOTDIR;
								goto errout;
							}

							/* "Push" the directory and continue searching */

							if (depth >= CONFIG_SMARTFS_DIRDEPTH - 1) {
								/* Directory depth too big */

								ret = -ENAMETOOLONG;
								goto errout;
							}
#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
							dirstack[++depth] = smartfs_rdle16(&entry->firstsector);
#else
							dirstack[++depth] = entry->firstsector;
#endif
							segment = ptr + 1;
							break;
						}
					}

					/* Not this entry.  Skip to the next one */

					offset += entrysize;
					entry = (struct smartfs_entry_header_s *)
							&fs->fs_rwbuffer[offset];
				}

				/* Test if a directory entry was found and break if it was */

				if (offset < readwrite.count) {
					break;
				}
			}

			/* If we found a dir entry, then continue searching */

			if (offset < readwrite.count) {
				/* Update the segment pointer */

				if (*ptr != '\0') {
					ptr++;
				}

				segment = ptr;
				continue;
			}

			/* Entry not found!  Report the error.  Also, if this is the last
			 * segment, then report the parent directory sector.
			 */

			if (*ptr == '\0') {
				*parentdirsector = dirstack[depth];
				*filename = segment;
			} else {
				*parentdirsector = 0xFFFF;
				*filename = NULL;
			}

			ret = -ENOENT;
			goto errout;
		}
	}

errout:
	if (direntry->name != NULL) {
		kmm_free(direntry->name);
		direntry->name = NULL;
	}
	return ret;
}

/****************************************************************************
 * Name: smartfs_createentry
 *
 * Description: Creates a new entry in the specified parent directory, using
 *              the specified type and name.  If the given sectorno is
 *              0xFFFF, then a new sector is allocated for the new entry,
 *              otherwise the supplied sectorno is used.
 *
 ****************************************************************************/

int smartfs_createentry(struct smartfs_mountpt_s *fs, uint16_t parentdirsector, const char *filename, uint16_t type, mode_t mode, struct smartfs_entry_s *direntry, uint16_t sectorno, FAR struct smartfs_ofile_s *sf)
{
	struct smart_read_write_s readwrite;
	int ret;
	uint16_t psector;
	uint16_t nextsector;
	uint16_t offset;
	uint16_t found;
	uint16_t entrysize;
	struct smartfs_entry_header_s *entry;
	struct smartfs_chain_header_s *chainheader;

	/* Start at the 1st sector in the parent directory */

	psector = parentdirsector;
	found = FALSE;
	entrysize = sizeof(struct smartfs_entry_header_s) + fs->fs_llformat.namesize;

	/* Validate the name isn't too long */

	if (strlen(filename) > fs->fs_llformat.namesize) {
		return -ENAMETOOLONG;
	}

	/* Read the parent directory sector and find a place to insert
	 * the new entry.
	 */

	while (1) {
		/* Read the next sector */

		readwrite.logsector = psector;
		readwrite.count = fs->fs_llformat.availbytes;
		readwrite.offset = 0;
		readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
		if (ret < 0) {
			goto errout;
		}

		/* Get the next chained sector */

		chainheader = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;
		nextsector = SMARTFS_NEXTSECTOR(chainheader);

		/* Search for an empty entry in this sector */

		offset = sizeof(struct smartfs_chain_header_s);
		entry = (struct smartfs_entry_header_s *)&fs->fs_rwbuffer[offset];
		while (offset + entrysize < readwrite.count) {
			/* Check if this entry is available */

#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
			if ((smartfs_rdle16(&entry->flags) == SMARTFS_ERASEDSTATE_16BIT) || ((smartfs_rdle16(&entry->flags) &
#else
			if ((entry->flags == SMARTFS_ERASEDSTATE_16BIT) || ((entry->flags &
#endif
					(SMARTFS_DIRENT_EMPTY | SMARTFS_DIRENT_ACTIVE)) == (~SMARTFS_ERASEDSTATE_16BIT & (SMARTFS_DIRENT_EMPTY | SMARTFS_DIRENT_ACTIVE)))) {
				/* We found an empty entry.  Use it. */

				found = TRUE;
				break;
			}

			/* Not available.  Skip to next entry */

			offset += entrysize;
			entry = (struct smartfs_entry_header_s *)&fs->fs_rwbuffer[offset];
		}

		/* If we found an entry, stop the search */

		if (found) {
			break;
		}

		/* If there are no more sectors, then we need to add one to make
		 * room for the new entry.
		 */

		if (nextsector == SMARTFS_ERASEDSTATE_16BIT) {
			/* Allocate a new sector and chain it to the last one */

			ret = FS_IOCTL(fs, BIOC_ALLOCSECT, 0xFFFF);
			if (ret < 0) {
				goto errout;
			}

			nextsector = (uint16_t)ret;

			/* Chain the next sector into this sector sector */

			chainheader->nextsector[0] = (uint8_t)(nextsector & 0x00FF);
			chainheader->nextsector[1] = (uint8_t)(nextsector >> 8);
			//*((uint16_t *)chainheader->nextsector) = nextsector;
			readwrite.offset = offsetof(struct smartfs_chain_header_s, nextsector);
			readwrite.count = sizeof(uint16_t);
			readwrite.buffer = chainheader->nextsector;
			ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&readwrite);
			if (ret < 0) {
				fdbg("Error chaining sector %d\n", nextsector);
				goto errout;
			}
		}

		/* Now update to the next sector */

		psector = nextsector;
	}

	/* We found an insertion point.  Create the entry at sector,offset */

#if CONFIG_SMARTFS_ERASEDSTATE == 0xFF
#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
	smartfs_wrle16(&entry->flags, (uint16_t)(SMARTFS_DIRENT_ACTIVE | SMARTFS_DIRENT_DELETING | SMARTFS_DIRENT_RESERVED | type | (mode & SMARTFS_DIRENT_MODE)));
#else
	entry->flags = (uint16_t)(SMARTFS_DIRENT_ACTIVE | SMARTFS_DIRENT_DELETING | SMARTFS_DIRENT_RESERVED | type | (mode & SMARTFS_DIRENT_MODE));
#endif
#else							/* CONFIG_SMARTFS_ERASEDSTATE == 0xFF */
#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
	smartfs_wrle16(&entry->flags, (uint16_t)(SMARTFS_DIRENT_EMPTY | type | (mode & SMARTFS_DIRENT_MODE)));
#else
	entry->flags = (uint16_t)(SMARTFS_DIRENT_EMPTY | type | (mode & SMARTFS_DIRENT_MODE));
#endif
#endif							/* CONFIG_SMARTFS_ERASEDSTATE == 0xFF */

	if (sectorno == 0xFFFF) {
		/* Allocate a new sector for the file / dir */

		ret = FS_IOCTL(fs, BIOC_ALLOCSECT, 0xFFFF);
		if (ret < 0) {
			goto errout;
		}

		nextsector = (uint16_t)ret;

		/* Set the newly allocated sector's type (file or dir) */

#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
		if (sf) {
			/* Using sector buffer and we have an open file context.  Just update
			 * the sector buffer in the open file context.
			 */

			memset(sf->buffer, CONFIG_SMARTFS_ERASEDSTATE, fs->fs_llformat.availbytes);
			chainheader = (struct smartfs_chain_header_s *)sf->buffer;
			chainheader->type = SMARTFS_SECTOR_TYPE_FILE;
			sf->bflags = SMARTFS_BFLAG_DIRTY | SMARTFS_BFLAG_NEWALLOC;
		} else
#endif
		{
			if ((type & SMARTFS_DIRENT_TYPE) == SMARTFS_DIRENT_TYPE_DIR) {
				chainheader->type = SMARTFS_SECTOR_TYPE_DIR;
			} else {
				chainheader->type = SMARTFS_SECTOR_TYPE_FILE;
			}

			readwrite.count = 1;
			readwrite.offset = offsetof(struct smartfs_chain_header_s, type);
			readwrite.buffer = (uint8_t *)&chainheader->type;
			readwrite.logsector = nextsector;
			ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&readwrite);
			if (ret < 0) {
				fdbg("Error %d setting new sector type for sector %d\n", ret, nextsector);
				goto errout;
			}
		}
	} else {
		/* Use the provided sector number */

		nextsector = sectorno;
	}

	/* Create the directory entry to be written in the parent's sector */

#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
	smartfs_wrle16(&entry->firstsector, nextsector);
	smartfs_wrle16(&entry->utc, time(NULL));
#else
	entry->firstsector = nextsector;
	entry->utc = time(NULL);
#endif
	memset(entry->name, 0, fs->fs_llformat.namesize);
	strncpy(entry->name, filename, fs->fs_llformat.namesize);

	/* Now write the new entry to the parent directory sector */

	readwrite.logsector = psector;
	readwrite.offset = offset;
	readwrite.count = entrysize;
	readwrite.buffer = (uint8_t *) &fs->fs_rwbuffer[offset];
	ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long) &readwrite);
	if (ret < 0) {
		fdbg("failed to write new entry to parent directory psector : %d\n", psector);
		goto errout;
	}

	/* Now fill in the entry */

	direntry->firstsector = nextsector;
	direntry->dsector = psector;
	direntry->doffset = offset;
#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
	direntry->flags = smartfs_rdle16(&entry->flags);
	direntry->utc = smartfs_rdle32(&entry->utc);
#else
	direntry->flags = entry->flags;
	direntry->utc = entry->utc;
#endif
	direntry->datlen = 0;
	if (direntry->name == NULL) {
		direntry->name = (FAR char *)kmm_malloc(fs->fs_llformat.namesize + 1);
		if (direntry->name == NULL) {
			ret = ERROR;
			goto errout;
		}
	}

	memset(direntry->name, 0, fs->fs_llformat.namesize + 1);
	strncpy(direntry->name, filename, fs->fs_llformat.namesize);

	ret = OK;

errout:
	return ret;
}

/****************************************************************************
 * Name: smartfs_deleteentry
 *
 * Description: Deletes an entry from the filesystem (file or dir) by
 *              freeing all the entry's sectors and then marking it inactive
 *              in it's parent's directory sector.  For a directory, it
 *              does not validate the directory is empty, nor does it do
 *              a recursive delete.
 *
 ****************************************************************************/

int smartfs_deleteentry(struct smartfs_mountpt_s *fs, struct smartfs_entry_s *entry)
{
	int ret;
	uint16_t nextsector;
	uint16_t sector;
	uint16_t count;
	uint16_t entrysize;
	uint16_t offset;
	struct smartfs_entry_header_s *direntry;
	struct smartfs_chain_header_s *header;
	struct smart_read_write_s readwrite;

	/* Okay, delete the file.  Loop through each sector and release them

	 * TODO:  We really should walk the list backward to avoid lost
	 *        sectors in the event we lose power. However this requires
	 *        allocating a buffer to build the sector list since we don't
	 *        store a doubly-linked list of sectors on the device.  We
	 *        could test if the sector data buffer is big enough and
	 *        just use that, and only allocate a new buffer if the
	 *        sector buffer isn't big enough.  Do do this, however, we
	 *        need to change the code below as it is using the a few
	 *        bytes of the buffer to read in header info.
	 */

	nextsector = entry->firstsector;
	header = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;
	readwrite.offset = 0;
	readwrite.count = sizeof(struct smartfs_chain_header_s);
	readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
	while (nextsector != SMARTFS_ERASEDSTATE_16BIT) {
		/* Read the next sector into our buffer */

		sector = nextsector;
		readwrite.logsector = sector;
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
		if (ret < 0) {
			fdbg("Error reading sector %d\n", nextsector);
			break;
		}

		/* Release this sector */

		nextsector = SMARTFS_NEXTSECTOR(header);
		ret = FS_IOCTL(fs, BIOC_FREESECT, sector);
		if (ret < 0) {
			fdbg("Error freeing sector %d\n", nextsector);
			break;
		}
	}

	/* Remove the entry from the directory tree */

	readwrite.logsector = entry->dsector;
	readwrite.offset = 0;
	readwrite.count = fs->fs_llformat.availbytes;
	readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
	ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
	if (ret < 0) {
		fdbg("Error reading directory info at sector %d\n", entry->dsector);
		goto errout;
	}

	/* Mark this entry as inactive */

	direntry = (struct smartfs_entry_header_s *)&fs->fs_rwbuffer[entry->doffset];
#if CONFIG_SMARTFS_ERASEDSTATE == 0xFF
#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
	smartfs_wrle16(&direntry->flags, smartfs_rdle16(&direntry->flags) & ~SMARTFS_DIRENT_ACTIVE);
#else
	direntry->flags &= ~SMARTFS_DIRENT_ACTIVE;
#endif
#else							/* CONFIG_SMARTFS_ERASEDSTATE == 0xFF */
#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
	smartfs_wrle16(&direntry->flags, smartfs_rdle16(&direntry->flags) | SMARTFS_DIRENT_ACTIVE);
#else
	direntry->flags |= SMARTFS_DIRENT_ACTIVE;
#endif
#endif							/* CONFIG_SMARTFS_ERASEDSTATE == 0xFF */

	/* Write the updated flags back to the sector */

	readwrite.offset = entry->doffset;
	readwrite.count = sizeof(uint16_t);
	readwrite.buffer = (uint8_t *)&direntry->flags;
	ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&readwrite);
	if (ret < 0) {
		fdbg("Error marking entry inactive at sector %d\n", entry->dsector);
		goto errout;
	}

	/* Test if any entries in this sector are being used */

	if ((entry->dsector != fs->fs_rootsector) && (entry->dsector != entry->dfirst)) {
		/* Scan the sector and count used entries */

		count = 0;
		offset = sizeof(struct smartfs_chain_header_s);
		entrysize = sizeof(struct smartfs_entry_header_s) + fs->fs_llformat.namesize;
		while (offset + entrysize < fs->fs_llformat.availbytes) {
			/* Test the next entry */

			direntry = (struct smartfs_entry_header_s *)&fs->fs_rwbuffer[offset];
			if (ENTRY_VALID(direntry)) {
				/* Count this entry */
				count++;
			}

			/* Advance to next entry */

			offset += entrysize;
		}

		/* Test if the count it zero.  If it is, then we will release the sector */

		if (count == 0) {
			/* Okay, to release the sector, we must find the sector that we
			 * are chained to and remove ourselves from the chain.  First
			 * save our nextsector value so we can "unchain" ourselves.
			 */

			nextsector = SMARTFS_NEXTSECTOR(header);

			/* Now loop through the dir sectors to find ourselves in the chain */

			sector = entry->dfirst;
			readwrite.offset = 0;
			readwrite.count = sizeof(struct smartfs_chain_header_s);
			readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
			while (sector != SMARTFS_ERASEDSTATE_16BIT) {
				/* Read the header for the next sector */

				readwrite.logsector = sector;
				ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
				if (ret < 0) {
					fdbg("Error reading sector %d\n", nextsector);
					break;
				}

				/* Test if this sector "points" to us */

				if (SMARTFS_NEXTSECTOR(header) == entry->dsector) {
					/* We found ourselves in the chain.  Update the chain. */

					header->nextsector[0] = (uint8_t)(nextsector & 0x00FF);
					header->nextsector[1] = (uint8_t)(nextsector >> 8);

					readwrite.offset = offsetof(struct smartfs_chain_header_s, nextsector);
					readwrite.count = sizeof(uint16_t);
					readwrite.buffer = header->nextsector;
					ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&readwrite);
					if (ret < 0) {
						fdbg("Error unchaining sector (%d)\n", nextsector);
						goto errout;
					}

					/* Now release our sector */

					ret = FS_IOCTL(fs, BIOC_FREESECT, (unsigned long)entry->dsector);
					if (ret < 0) {
						fdbg("Error freeing sector %d\n", entry->dsector);
						goto errout;
					}

					/* Break out of the loop, we are done! */

					break;
				}

				/* Chain to the next sector */

				sector = SMARTFS_NEXTSECTOR(header);
			}
		}
	}

	ret = OK;

errout:
	return ret;
}

/****************************************************************************
 * Name: smartfs_countdirentries
 *
 * Description: Counts the number of items in the specified directory entry.
 *              This routine assumes you have validated the entry you are
 *              passing is in fact a directory sector, though it checks
 *              just in case you were stupid :-)
 *
 ****************************************************************************/

int smartfs_countdirentries(struct smartfs_mountpt_s *fs, struct smartfs_entry_s *entry)
{
	int ret;
	uint16_t nextsector;
	uint16_t offset;
	uint16_t entrysize;
	int count;
	struct smartfs_entry_header_s *direntry;
	struct smartfs_chain_header_s *header;
	struct smart_read_write_s readwrite;

	/* Walk through the directory's sectors and count entries */

	count = 0;
	nextsector = entry->firstsector;
	while (nextsector != SMARTFS_ERASEDSTATE_16BIT) {
		/* Read the next sector into our buffer */

		readwrite.logsector = nextsector;
		readwrite.offset = 0;
		readwrite.count = fs->fs_llformat.availbytes;
		readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
		if (ret < 0) {
			fdbg("Error reading sector %d\n", nextsector);
			break;
		}

		/* Validate this is a directory type sector */

		header = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;
		if (header->type != SMARTFS_SECTOR_TYPE_DIR) {
			fdbg("Sector %d is not a DIR sector!\n", nextsector);
			goto errout;
		}

		/* Loop for all entries in this sector and count them */

		offset = sizeof(struct smartfs_chain_header_s);
		entrysize = sizeof(struct smartfs_entry_header_s) + fs->fs_llformat.namesize;
		direntry = (struct smartfs_entry_header_s *)&fs->fs_rwbuffer[offset];
		while (offset + entrysize < readwrite.count) {
			if (ENTRY_VALID(direntry)) {
				/* Count this entry */
				count++;
			}

			offset += entrysize;
			direntry = (struct smartfs_entry_header_s *)&fs->fs_rwbuffer[offset];
		}

		/* Get the next sector from the header */

		nextsector = SMARTFS_NEXTSECTOR(header);
	}

	ret = count;

errout:
	return ret;
}

/****************************************************************************
 * Name: smartfs_truncatefile
 *
 * Description: Truncates an existing file on the device so that it occupies
 *              zero bytes and can be completely re-written.
 *
 ****************************************************************************/

int smartfs_truncatefile(struct smartfs_mountpt_s *fs, struct smartfs_entry_s *entry, FAR struct smartfs_ofile_s *sf)
{
	int ret;
	uint16_t nextsector;
	uint16_t sector;
	struct smartfs_chain_header_s *header;
	struct smart_read_write_s readwrite;

	/* Walk through the directory's sectors and count entries */

	nextsector = entry->firstsector;
	header = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;

	while (nextsector != SMARTFS_ERASEDSTATE_16BIT) {
		/* Read the next sector's header into our buffer */

		readwrite.logsector = nextsector;
		readwrite.offset = 0;
		readwrite.count = sizeof(struct smartfs_chain_header_s);
		readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
		if (ret < 0) {
			fdbg("Error reading sector %d header\n", nextsector);
			goto errout;
		}

		/* Get the next chained sector */

		sector = SMARTFS_NEXTSECTOR(header);

		/* If this is the 1st sector of the file, then just overwrite
		 * the sector data with the erased state value.  The underlying
		 * SMART block driver will detect this and release the old
		 * sector and create a new one with the new (blank) data.
		 */

		if (nextsector == entry->firstsector) {
#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER

			/* When we have a sector buffer in use, simply skip the first sector */

			nextsector = sector;
			continue;

#else

			/* Fill our buffer with erased data */

			memset(fs->fs_rwbuffer, CONFIG_SMARTFS_ERASEDSTATE, fs->fs_llformat.availbytes);
			header->type = SMARTFS_SECTOR_TYPE_FILE;

			/* Now write the new sector data */

			readwrite.count = fs->fs_llformat.availbytes;
			ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&readwrite);
			if (ret < 0) {
				fdbg("Error blanking 1st sector (%d) of file\n", nextsector);
				goto errout;
			}

			/* Set the entry's data length to zero ... we just truncated */

			entry->datlen = 0;
#endif							/* CONFIG_SMARTFS_USE_SECTOR_BUFFER */
		} else {
			/* Not the 1st sector -- release it */

			ret = FS_IOCTL(fs, BIOC_FREESECT, (unsigned long)nextsector);
			if (ret < 0) {
				fdbg("Error freeing sector %d\n", nextsector);
				goto errout;
			}
		}

		/* Now move on to the next sector */

		nextsector = sector;
	}

	/* Now deal with the first sector in the event we are using a sector buffer
	   like we would be if CRC is enabled.
	 */

#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
	if (sf) {
		/* Using sector buffer and we have an open file context.  Just update
		 * the sector buffer in the open file context.
		 */

		readwrite.logsector = entry->firstsector;
		readwrite.offset = 0;
		readwrite.count = sizeof(struct smartfs_chain_header_s);
		readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);

		memset(sf->buffer, CONFIG_SMARTFS_ERASEDSTATE, fs->fs_llformat.availbytes);
		header = (struct smartfs_chain_header_s *)sf->buffer;
		header->type = SMARTFS_SECTOR_TYPE_FILE;
		sf->bflags = SMARTFS_BFLAG_DIRTY;
		entry->datlen = 0;
	}
#endif

	ret = OK;

errout:
	return ret;
}

/****************************************************************************
 * Name: smartfs_get_first_mount
 *
 * Description: Returns a pointer to the first mounted smartfs volume.
 *
 ****************************************************************************/

#if defined(CONFIG_FS_PROCFS) && !defined(CONFIG_FS_PROCFS_EXCLUDE_SMARTFS)
struct smartfs_mountpt_s *smartfs_get_first_mount(void)
{
	return g_mounthead;
}
#endif

#ifdef CONFIG_SMARTFS_SECTOR_RECOVERY
/****************************************************************************
 * Name: smartfs_examine_sector
 *
 * Description: Recursively read sectors, find out its children and mark the
 *              sectors valid.
 *
 ****************************************************************************/
int smartfs_examine_sector(struct smartfs_mountpt_s *fs, char *validsectors, int *nusedsectors)
{
	int ret;
	uint16_t offset;
	uint16_t entrysize;
	uint16_t logsector;
	uint16_t nextsector;
	uint16_t firstsector;
	uint8_t *sectordata;
	uint8_t type;
	uint8_t entrytype;
	struct sector_queue_s *node;
	struct smart_read_write_s readwrite;
	struct smartfs_entry_header_s *entry;
	struct smartfs_chain_header_s *header;

	ret = OK;

	DEBUGASSERT(fs != NULL);

	entrysize = sizeof(struct smartfs_entry_header_s) + fs->fs_llformat.namesize;
	sectordata = (uint8_t *)kmm_malloc(fs->fs_llformat.availbytes);
	if (!sectordata) {
		ret = -ENOMEM;
		goto errout;
	}

	while (!sq_empty(&g_recovery_queue)) {
		node = (struct sector_queue_s *)sq_remfirst(&g_recovery_queue);
		if (!node) {
			ret = ERROR;
			goto errout;
		}
		logsector = node->sector;
		type = node->type;
		kmm_free(node);
		while (logsector != SMARTFS_ERASEDSTATE_16BIT) {

			/* Read the curent sector into our buffer */
			readwrite.logsector = logsector;
			readwrite.offset = 0;
			readwrite.buffer = sectordata;
			readwrite.count = fs->fs_llformat.availbytes;
			ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
			if (ret < 0) {
				ret = -EINVAL_SECTOR;
				fdbg("Error %d reading sector %d data\n", ret, logsector);
				goto errout;
			}

			/* Point header to the read data to get used byte count */
			header = (struct smartfs_chain_header_s *)sectordata;
			nextsector = SMARTFS_NEXTSECTOR(header);

			if (type == SMARTFS_SECTOR_TYPE_DIR) {
				if (smart_validatesector(fs->fs_blkdriver, logsector, validsectors) == OK) {
					(*nusedsectors)++;
				}

				offset = sizeof(struct smartfs_chain_header_s);

				/* Test if this entry is valid and active */
				while (offset < readwrite.count) {
					entry = (struct smartfs_entry_header_s *)&sectordata[offset];
					if (!(ENTRY_VALID(entry))) {
						/* This entry isn't valid, skip it */

						offset += entrysize;
						continue;
					}
#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
					firstsector = smartfs_rdle16(&entry->firstsector);
					if ((smartfs_rdle16(&entry->flags) & SMARTFS_DIRENT_TYPE) == SMARTFS_DIRENT_TYPE_FILE)
#else
					firstsector = entry->firstsector;
					if ((entry->flags & SMARTFS_DIRENT_TYPE) == SMARTFS_DIRENT_TYPE_FILE)
#endif
					{
						entrytype = SMARTFS_SECTOR_TYPE_FILE;
					} else {
						entrytype = SMARTFS_SECTOR_TYPE_DIR;
					}

					node = (struct sector_queue_s *)kmm_malloc(sizeof(struct sector_queue_s));
					if (!node) {
						ret = -ENOMEM;
						goto errout;
					}
					node->sector = firstsector;
					node->type = entrytype;
					sq_addlast((FAR sq_entry_t *)node, &g_recovery_queue);
					offset += entrysize;
				}
			} else if (type == SMARTFS_SECTOR_TYPE_FILE) {
				if (smart_validatesector(fs->fs_blkdriver, logsector, validsectors) == OK) {
					(*nusedsectors)++;
				}
			}
			logsector = nextsector;
		}
	}
	ret = OK;
errout:
	if (sectordata) {
		kmm_free(sectordata);
	}
	return ret;
}

/****************************************************************************
 * Name: smartfs_recover
 *
 * Description: Recovery after  a power failure
 *
 ****************************************************************************/
int smartfs_recover(struct smartfs_mountpt_s *fs)
{
	int i, ret;
	uint8_t rootsector;
	uint16_t nsectors;
	char *validsectors;
	int nusedsectors;
	int nobsolete;
	int nrecovered;
	struct sector_queue_s *node;

	nsectors = fs->fs_llformat.nsectors;
	rootsector = fs->fs_rootsector;

	validsectors = (char *)kmm_malloc(nsectors / 8 + 1);
	if (!validsectors) {
		ret = -ENOMEM;
		goto errout_with_semaphore;
	}
	for (i = 0; i < (nsectors / 8 + 1); i++) {
		validsectors[i] = 0;
	}

	sq_init(&g_recovery_queue);
	node = (struct sector_queue_s *)kmm_malloc(sizeof(struct sector_queue_s));
	if (!node) {
		ret = -ENOMEM;
		goto errout_with_semaphore;
	}
	node->sector = rootsector;
	node->type = SMARTFS_SECTOR_TYPE_DIR;
	sq_addlast((FAR sq_entry_t *)node, &g_recovery_queue);

	nusedsectors = 0;
	ret = smartfs_examine_sector(fs, validsectors, &nusedsectors);
	if (ret != OK) {
		goto errout_with_semaphore;
	}

	nobsolete = 0;
	nrecovered = 0;
	ret = smart_recoversectors(fs->fs_blkdriver, validsectors, &nobsolete, &nrecovered);
	if (ret != OK) {
		goto errout_with_semaphore;
	}
	fdbg("###############################\n");
	fdbg("#      FS Recovery Report     #\n");
	fdbg("###############################\n");
	fdbg("Total sectors : %d\n", fs->fs_llformat.nsectors);
	fdbg("Bytes in each sector : %d\n", fs->fs_llformat.availbytes - sizeof(struct smartfs_chain_header_s));
	fdbg("Used Sectors : %d\n", nusedsectors);
	fdbg("Obsoleted Sectors : %d\n", nobsolete);
	fdbg("Recovered Sectors : %d\n\n", nrecovered);

errout_with_semaphore:
	if (validsectors) {
		kmm_free(validsectors);
	}
	return ret;
}
#endif

#ifdef CONFIG_SMARTFS_JOURNALING
/****************************************************************************
 * Name: smartfs_journal_init
 *
 * Description: Initialize jornal manager
 *              Read logs to restore any previously failed transaction
 *
 ****************************************************************************/
int smartfs_journal_init(struct smartfs_mountpt_s *fs)
{
	int ret;
	uint16_t mapsize;
	uint16_t readoffset;
	uint16_t readsect;
	uint16_t startsector;
	struct smart_format_s fmt;
	struct smartfs_logging_entry_s *entry;
	struct journal_transaction_manager_s *journal;

	journal = (struct journal_transaction_manager_s *)kmm_malloc(sizeof(struct journal_transaction_manager_s));
	if (!journal) {
		goto err_out;
	}

	fs->journal = journal;
	journal->jarea = smartfs_get_journal_area(fs);
	journal->list = NULL;

	ret = FS_IOCTL(fs, BIOC_GETFORMAT, (unsigned long)&fmt);
	if (ret != OK) {
		goto err_out;
	}

	if (fmt.nsectors < CONFIG_SMARTFS_JOURNALING_THRESHOLD) {
		journal->enabled = false;
		fdbg("Journal Manager Not enabled due to size constraints. %d %d\n", fmt.nsectors, CONFIG_SMARTFS_JOURNALING_THRESHOLD);
		return 0;
	}

	journal->enabled = true;
	journal->availbytes = fmt.availbytes;

	journal->buffer = (uint8_t *)kmm_malloc(sizeof(struct smartfs_logging_entry_s) + journal->availbytes);
	if (!(journal->buffer)) {
		goto err_out;
	}

	/* Allocate a bitmap to mark currently active sectors (sectors which are
	 * written and need sync) */
	mapsize = fmt.nsectors / SZ_UINT8 + 1;
	journal->active_sectors = (uint8_t *)kmm_malloc(mapsize);
	if (!(journal->active_sectors)) {
		goto err_out;
	}
	memset(journal->active_sectors, 0, mapsize);

	if (journal->jarea != -1) {
#ifdef CONFIG_DEBUG_FS
		print_journal_sectors(fs);
#endif
		startsector = SMARTFS_LOGGING_SECTOR + journal->jarea * CONFIG_SMARTFS_NLOGGING_SECTORS;
		journal->sector = startsector;
		readsect = startsector;
		readoffset = 1;

		while (readsect < startsector + CONFIG_SMARTFS_NLOGGING_SECTORS) {
			/* Read transactions in journal sector */
			fvdbg("readsector : %d offset : %d\n", readsect, readoffset);
			ret = read_logging_entry(fs, journal, &readsect, &readoffset);
			if (ret != OK) {
				break;
			}
			entry = (struct smartfs_logging_entry_s *)journal->buffer;
			if (entry->crc16[0] != smartfs_calc_crc_entry(journal)) {
				fdbg("Journal entry header crc mismatch! sector : %d type : %d entry crc : %d calc-crc : %d\n", journal->sector, GET_TRANS_TYPE(entry->trans_info), entry->crc16[0], smartfs_calc_crc_entry(journal));
				break;
			}
			/* Check whether this transaction exists, and logging of transaction has been completed */
			if (T_EXIST_CHECK(entry->trans_info) && T_START_CHECK(entry->trans_info)) {
				journal->sector = readsect;
				journal->offset = readoffset - sizeof(struct smartfs_logging_entry_s);

				/* If this entry has additional data, read additional data
				   Skip reading additional data if transaction os of type DELETE, where we have reused datalen field
				 */
				if (entry->datalen > 0 && GET_TRANS_TYPE(entry->trans_info) != T_DELETE) {
					/* We skip reading additional data if transaction needs sync,
					 * because these type of transactions are restored from list later.
					 * So, we only increment the offset here */
					/* If not a T_WRITE transaction which needs sync, read the additional data */
					ret = read_logging_data(fs, journal, &readsect, &readoffset);
					if (ret != OK) {
						fdbg("Cannot read entry data.\n");
						break;
					}
					if (entry->crc16[1] != smartfs_calc_crc_data(journal)) {
						fdbg("Journal entry data crc mismatch! sector : %d type : %d entry crc : %d calc-crc : %d\n", journal->sector, GET_TRANS_TYPE(entry->trans_info), entry->crc16[1], smartfs_calc_crc_data(journal));
						break;
					}
				}
				/* Restore the transaction. (T_WRITE with sync type transaction will not be
				 * restored yet */
				ret = process_transaction(fs);
				if (ret != OK) {
					fdbg("process_transaction failed, but clean journal area\n");
					break;
				}
			} else {
				/* If a valid transaction does not exist here, stop checking further */
				break;
			}
		}

		if (journal->list) {
			/* Now restore write transactions from the list */
			ret = restore_write_transactions(fs);
			if (ret != OK) {
				fdbg("restore_write_transactions failed, but clean journal area\n");
			}
		}
	}

	/* Clear all the logging sectors */
	journal->jarea = 1;
	ret = clear_journal_sectors(fs, journal);
	if (ret != OK) {
		goto err_out;
	}
	journal->jarea = 0;
	ret = clear_journal_sectors(fs, journal);
	if (ret != OK) {
		goto err_out;
	}
	journal->sector = SMARTFS_LOGGING_SECTOR;
	journal->offset = 1;
	ret = set_area_id_bits(fs, AREA_USED);
	if (ret != OK) {
		fdbg("set_area_id_bits failed... bits : %d\n", AREA_USED);
	}
	return ret;

err_out:
	/* Free in case of failure */
	if (journal) {
		if (journal->active_sectors) {
			kmm_free(journal->active_sectors);
		}
		if (journal->buffer) {
			kmm_free(journal->buffer);
		}
		kmm_free(journal);
		journal = NULL;
	}
	return ERROR;
}

static int set_area_id_bits(struct smartfs_mountpt_s *fs, uint8_t id_bits)
{
	struct smart_read_write_s req;

	req.logsector = SMARTFS_LOGGING_SECTOR + fs->journal->jarea * CONFIG_SMARTFS_NLOGGING_SECTORS;
	req.offset = 0;
	req.count = 1;
	req.buffer = &id_bits;
	return FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&req);
}

/****************************************************************************
 * Name: smartfs_get_journal_area
 *
 * Description: Reads identification bits of two journal areas and chooses
 *              which journal area has to be used for restoration.
 *              Return value: 0: area 0 has to be used for restore
 *                            1: area 1 has to be used for restore
 *                           -1: no area is fit to use without erase.
 *
 ****************************************************************************/
static int smartfs_get_journal_area(struct smartfs_mountpt_s *fs)
{
	int i, read_from_A, read_from_B;
	uint8_t area[2], index[2];
	struct smart_read_write_s req;

	/* Result to be returned for different combination of status
	 * bits of 2 areas.
	 *
	 * Row: index of status of area 0
	 * Column: index of status of area 1
	 *
	 * index for status AREA_UNUSED is 0, for AREA_USED is 1,
	 * AREA_OLD_UNERASED is 2, undefined is 3
	 * e.g if area 0 has status AREA_USED (index[0]=1) and area 1 has
	 * status AREA_UNUSED (index[1]=0), we return
	 * result[index[0]][index[1]] = result[1][0] = 0
	 * The function returns area 0.
	 */
	int8_t result[][4] = { { -1, 1, 1, -1},
		{0, 0, 0, 0},
		{0, 1, 0, 0},
		{ -1, 1, 1, -1}
	};

	req.logsector = SMARTFS_LOGGING_SECTOR;
	req.offset = 0;
	req.count = 1;
	req.buffer = &area[0];

	read_from_A = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&req);

	req.logsector += CONFIG_SMARTFS_NLOGGING_SECTORS;
	req.buffer = &area[1];

	read_from_B = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&req);

	if (read_from_A != req.count && read_from_B != req.count) {
		return -1;
	} else if (read_from_A != req.count && (area[1] == AREA_USED || area[1] == AREA_OLD_UNERASED)) {
		return 1;
	} else if (read_from_B != req.count && (area[0] == AREA_USED || area[0] == AREA_OLD_UNERASED)) {
		return 0;
	}

	for (i = 0; i <= 1; i++) {
		if (area[i] == AREA_UNUSED) {
			index[i] = 0;
		} else if (area[i] == AREA_USED) {
			index[i] = 1;
		} else if (area[i] == AREA_OLD_UNERASED) {
			index[i] = 2;
		} else {
			index[i] = 3;
		}
	}

	return result[index[0]][index[1]];
}

#ifdef CONFIG_DEBUG_FS
int print_journal_sectors(struct smartfs_mountpt_s *fs)
{
	int ret;
	uint16_t i;
	uint16_t readsect;
	uint16_t readoffset;
	uint16_t startsector;
	struct journal_transaction_manager_s *j_mgr;
	struct smartfs_logging_entry_s *entry;

	fdbg("Print all Journal sectors\n");
	j_mgr = fs->journal;

	if (!(j_mgr->enabled)) {
		fdbg("Journal Manager Not enabled due to size constraints.\n");
		return OK;
	}

	if (j_mgr->jarea != -1) {
		startsector = SMARTFS_LOGGING_SECTOR + j_mgr->jarea * CONFIG_SMARTFS_NLOGGING_SECTORS;
		readsect = startsector;
		readoffset = 1;
		while (readsect < startsector + CONFIG_SMARTFS_NLOGGING_SECTORS) {
			ret = read_logging_entry(fs, j_mgr, &readsect, &readoffset);
			if (ret != OK) {
				break;
			}
			entry = (struct smartfs_logging_entry_s *)j_mgr->buffer;
			if (T_EXIST_CHECK(entry->trans_info) && T_START_CHECK(entry->trans_info)) {
				if (entry->datalen > 0 && GET_TRANS_TYPE(entry->trans_info) != T_DELETE) {
					ret = read_logging_data(fs, j_mgr, &readsect, &readoffset);
					if (ret != OK) {
						fdbg("Cannot read entry data.\n");
						break;
					}
				}
				if (!T_FINISH_CHECK(entry->trans_info)) {
					fdbg("*****Entry*****\n");
					fdbg("Transaction Type: %u\n", GET_TRANS_TYPE(entry->trans_info));
					fdbg("Transaction Started: %s\n", T_START_CHECK(entry->trans_info) ? "yes" : "no");
					fdbg("Transaction Finished: %s\n", T_FINISH_CHECK(entry->trans_info) ? "yes" : "no");
					fdbg("Transaction required a sync: %s\n", T_NEEDSYNC_CHECK(entry->trans_info) ? "yes" : "no");
					fdbg("sector: %u\n", entry->curr_sector);
					fdbg("offset: %u\n", entry->offset);
					fdbg("seq_no: %u\n", entry->seq_no);
					fdbg("datalen: %u\n", entry->datalen);
					fdbg("generic_1 %u\n", entry->generic_1);
#ifdef CONFIG_DEBUG_FS
#ifndef DEBUG_VERBOSE
					if (entry->datalen > 0 && GET_TRANS_TYPE(entry->trans_info) != T_DELETE)
#endif
					{
						fdbg("Data: ");
						for (i = 0; i < entry->datalen; i++) {
							fsdbg("%02x ", j_mgr->buffer[sizeof(struct smartfs_logging_entry_s) + i]);
						}
						fsdbg("\n");
					}
#endif
				}
			} else {
				break;
			}
		}
	}
	return OK;
}
#endif

/****************************************************************************
 * Name: clear_journal_sectors
 *
 * Description: clean all the journal sectors
 *              if any journal logical sector is unallocated, allocate it
 *
 ****************************************************************************/
int clear_journal_sectors(struct smartfs_mountpt_s *fs, struct journal_transaction_manager_s *j_mgr)
{
	int ret;
	bool allocate;
	uint16_t i;
	uint16_t j;
	uint16_t readsect;
	struct smart_read_write_s req;

	if (!(j_mgr->enabled)) {
		fdbg("Journal Manager Not enabled due to size constraints.\n");
		return OK;
	}

	/* Clear all the journal sectors */
	for (i = 0; i < CONFIG_SMARTFS_NLOGGING_SECTORS; i++) {
		allocate = false;
		readsect = SMARTFS_LOGGING_SECTOR + (j_mgr->jarea * CONFIG_SMARTFS_NLOGGING_SECTORS) + i;

		req.logsector = readsect;
		req.count = j_mgr->availbytes;
		req.buffer = j_mgr->buffer;
		req.offset = 0;

		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&req);
		if (ret < 0) {
			if (ret != -EINVAL) {
				/* Read failed due to reason other than log sector not allocated */
				return ret;
			}
			/* need to allocate this logical sector */
			allocate = true;
		} else {
			ret = OK;
			/* Check whether the sector needs relocation */
			for (j = 0; j < req.count; j++) {
				if (req.buffer[j] != CONFIG_SMARTFS_ERASEDSTATE) {
					/* Free sector and set to allocate */
					FS_IOCTL(fs, BIOC_FREESECT, (unsigned long)req.logsector);
					allocate = true;
					break;
				}
			}
		}
		if (allocate) {
			ret = FS_IOCTL(fs, BIOC_ALLOCSECT, (unsigned long)readsect);
			if (ret < 0) {
				fdbg("alloc failed!!\n");
				break;
			}
		}
		/* if returned logical sector is bigger than 0, change ret to OK */
		if (ret > OK) {
			ret = OK;
		}
	}
	return ret;
}

/****************************************************************************
 * Name: read_logging_entry
 *
 * Description: read the entry located in sector 'sector' and offset 'offset'
 *              It also updates the offset and sector for next read.
 *
 ****************************************************************************/
static int read_logging_entry(struct smartfs_mountpt_s *fs, struct
							  journal_transaction_manager_s *j_mgr, uint16_t *sector, uint16_t *offset)
{
	int ret;
	struct smart_read_write_s req;
	uint16_t startsector;

	startsector = SMARTFS_LOGGING_SECTOR + j_mgr->jarea * CONFIG_SMARTFS_NLOGGING_SECTORS;
	if (*offset + sizeof(struct smartfs_logging_entry_s) > j_mgr->availbytes) {
		/* If this sector doesnt have enough space for the entry, we would not have
		 * written the entry here. Skip to the next sector. */
		(*sector)++;
		*offset = 0;
		if (*sector >= startsector + CONFIG_SMARTFS_NLOGGING_SECTORS) {
			/* If no more sectors exist, do not try to read anything and return */
			return ERROR;
		}
	}

	req.logsector = *sector;
	req.offset = *offset;
	req.count = sizeof(struct smartfs_logging_entry_s);
	req.buffer = (uint8_t *)j_mgr->buffer;

	ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&req);
	if (ret < 0) {
		fdbg("Reading failed\n");
		return ERROR;
	}
	*offset += req.count;
	return OK;
}

/****************************************************************************
 * Name: read_logging_data
 *
 * Description: read the additional data at sector and offset after an entry
 *              It also updates the offset and sector for next read.
 *
 ****************************************************************************/
static int read_logging_data(struct smartfs_mountpt_s *fs, struct
							 journal_transaction_manager_s *j_mgr, uint16_t *sector, uint16_t *offset)
{
	int ret;
	uint16_t startsector;
	struct smart_read_write_s req;
	struct smartfs_logging_entry_s *entry;

	startsector = SMARTFS_LOGGING_SECTOR + j_mgr->jarea * CONFIG_SMARTFS_NLOGGING_SECTORS;
	entry = (struct smartfs_logging_entry_s *)j_mgr->buffer;

	req.buffer = (uint8_t *)j_mgr->buffer + sizeof(struct smartfs_logging_entry_s);
	req.logsector = *sector;
	req.offset = *offset;
	req.count = entry->datalen;

	if (req.offset + entry->datalen > j_mgr->availbytes) {
		/* If entire data is not in current sector, read only what is present */
		req.count = j_mgr->availbytes - req.offset;
	}

	if (req.count > 0) {
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&req);
		if (ret < 0) {
			return ERROR;
		}
		req.buffer += req.count;
		*offset += req.count;
	}
	if (req.count < entry->datalen) {
		/* Read next sector if more data is present to read.  */
		(*sector)++;
		if (*sector >= startsector + CONFIG_SMARTFS_NLOGGING_SECTORS) {
			return ERROR;
		}
		*offset = 0;
		req.logsector = *sector;
		req.offset = *offset;
		req.count = entry->datalen - req.count;
		if (req.offset + req.count > j_mgr->availbytes) {
			fdbg("Data should never be more than available bytes in a sector offset : %d count : %d availbytes : %d\n", req.offset, req.count, j_mgr->availbytes);
			return ERROR;
		}
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&req);
		if (ret < 0) {
			return ERROR;
		}
		*offset += req.count;
	}
	return OK;
}

/****************************************************************************
 * Name: restore_write_transactions
 *
 * Description: restore write and sync transactions from the list
 *
 ****************************************************************************/
static int restore_write_transactions(struct smartfs_mountpt_s *fs)
{
	int ret;
	uint16_t readsect;
	uint16_t readoffset;
	struct smart_read_write_s req;
	struct active_write_node_s *curr;
	struct smartfs_logging_entry_s *entry;
	struct journal_transaction_manager_s *j_mgr;

	j_mgr = fs->journal;
	curr = j_mgr->list;
	/* Loop through the list */
	while (curr) {
		/* read the logging entry in the list */
		readsect = curr->journal_sector;
		readoffset = curr->journal_offset;
		ret = read_logging_entry(fs, j_mgr, &readsect, &readoffset);
		if (ret != OK) {
			fdbg("Error reading entry\n");
			return ERROR;
		}
		entry = (struct smartfs_logging_entry_s *)j_mgr->buffer;
		if (entry->datalen > 0) {
			/* Read logging data from journal here */
			ret = read_logging_data(fs, j_mgr, &readsect, &readoffset);
			if (ret != OK) {
				fdbg("Cannot read entry data.\n");
				return ERROR;
			}

			req.logsector = entry->curr_sector;
			req.offset = entry->offset;
			req.count = entry->datalen;
			req.buffer = (uint8_t *)j_mgr->buffer + sizeof(struct smartfs_logging_entry_s);

			ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&req);
			if (ret != OK) {
				return ERROR;
			}
		}

		/* write the sector size from used_bytes */
		ret = smartfs_redo_sync(fs, curr->used_bytes);
		if (ret != OK) {
			return ERROR;
		}
		/* Transaction restored. Now set its status to Finished */
		ret = smartfs_set_transaction(fs, curr->journal_sector, curr->journal_offset, TRANS_FINISHED);
		if (ret != OK) {
			return ERROR;
		}
		/* unmark the sector from map of sectors needing sync */
		SET_INACTIVE(j_mgr->active_sectors, entry->curr_sector);
		curr = curr->next;
	}

	/* now clear the list */
	while (j_mgr->list) {
		curr = j_mgr->list->next;
		kmm_free(j_mgr->list);
		j_mgr->list = curr;
	}

	return OK;
}

/****************************************************************************
 * Name: smartfs_redo_sync
 *
 * Description: redo the sync to update used bytes filed in chainheader
 *
 ****************************************************************************/
static int smartfs_redo_sync(struct smartfs_mountpt_s *fs, uint16_t used_val)
{
	struct smart_read_write_s req;
	struct smartfs_logging_entry_s *entry;
	struct smartfs_chain_header_s *header;

	entry = (struct smartfs_logging_entry_s *)fs->journal->buffer;
	header = (struct smartfs_chain_header_s *)(fs->journal->buffer + sizeof(struct smartfs_logging_entry_s));

#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
	set_used_byte_count((uint8_t *)header->used_val, used);
#else
	header->used[0] = (uint8_t)(used_val & 0x00FF);
	header->used[1] = (uint8_t)(used_val >> 8);
#endif
	req.logsector = entry->curr_sector;
	req.offset = offsetof(struct smartfs_chain_header_s, used);

	req.buffer = (uint8_t *)header + req.offset;
	req.count = sizeof(uint16_t);
	if (FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&req) != OK) {
		return ERROR;
	}
	return OK;
}

/****************************************************************************
 * Name: smartfs_redo_rename
 *
 * Description: redo the file rename operation
 *
 ****************************************************************************/
static int smartfs_redo_rename(struct smartfs_mountpt_s *fs)
{
	int ret;
	char *filename;
	uint16_t oldflags;
	uint16_t oldoffset;
	uint16_t firstsector;
	mode_t mode;
	uint16_t type;
	struct smart_read_write_s req;
	struct smartfs_logging_entry_s *entry;
	struct smartfs_entry_header_s *direntry;
	struct journal_transaction_manager_s *j_mgr;

	j_mgr = fs->journal;
	entry = (struct smartfs_logging_entry_s *)(j_mgr->buffer);
	/* Allocate a buffer to temporarily store the filename */
	filename = (char *)kmm_malloc(fs->fs_llformat.namesize);
	if (!filename) {
		ret = -ENOMEM;
		goto errout;
	}
	oldoffset = entry->offset;
	memcpy(filename, j_mgr->buffer + sizeof(struct smartfs_logging_entry_s), entry->datalen);

	/* Read the old entry to exract mode, type and firstsector */
	req.logsector = entry->generic_1;
	req.offset = 0;
	req.count = j_mgr->availbytes;
	req.buffer = j_mgr->buffer + sizeof(struct smartfs_logging_entry_s);
	ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&req);
	if (ret < 0) {
		goto errout;
	}
	direntry = (struct smartfs_entry_header_s *)&req.buffer[oldoffset];

	if (ENTRY_VALID(direntry)) {
		oldflags = direntry->flags;
#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
		type = (smartfs_rdle16(&oldflags) & SMARTFS_DIRENT_TYPE);
		mode = (smartfs_rdle16(&oldflags) & SMARTFS_DIRENT_MODE);
#else
		type = oldflags & SMARTFS_DIRENT_TYPE;
		mode = oldflags & SMARTFS_DIRENT_MODE;
#endif

		firstsector = (uint16_t)direntry->firstsector;
		/* Now create new entry at new location */
		entry->generic_1 = (uint16_t)mode;
		memcpy(j_mgr->buffer + sizeof(struct smartfs_logging_entry_s), filename, entry->datalen);
		ret = smartfs_redo_create(fs, type, firstsector);
		if (ret != OK) {
			goto errout;
		}

		/* Set old entry to inactive */
#if CONFIG_SMARTFS_ERASEDSTATE == 0xFF
		oldflags &= ~SMARTFS_DIRENT_ACTIVE;
#else
		oldflags |= SMARTFS_DIRENT_ACTIVE;
#endif
		direntry->flags = oldflags;

		req.offset = oldoffset + offsetof(struct smartfs_entry_header_s, flags);
		req.count = sizeof(direntry->flags);
		req.buffer = (uint8_t *)&direntry->flags;
		ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&req);
		if (ret != OK) {
			fdbg("Inactive old entry failed... offset : %d\n", req.offset);
		}
	} else {
		ret = OK;
	}
errout:
	if (filename) {
		kmm_free(filename);
	}
	return ret;
}

/****************************************************************************
 * Name: smartfs_redo_create
 *
 * Description: redo file/directory create
 *              type: file or directory
 *              firstsector: first sector of the new entry
 *
 ****************************************************************************/
static int smartfs_redo_create(struct smartfs_mountpt_s *fs, uint16_t type, uint16_t firstsector)
{
	int ret;
	char *name;
	uint16_t nextsector;
	uint16_t offset;
	uint16_t direntrysize;
	mode_t mode;
	struct smart_read_write_s req;
	struct smartfs_logging_entry_s *entry;
	struct smartfs_chain_header_s *chainheader;
	struct smartfs_entry_header_s *direntry;
	struct smartfs_entry_s dummyentry;
	struct journal_transaction_manager_s *j_mgr;

	ret = OK;
	j_mgr = fs->journal;

	entry = (struct smartfs_logging_entry_s *)(j_mgr->buffer);
	/* Get the name from data followint the entry header in buffer */
	name = (char *)entry + sizeof(struct smartfs_logging_entry_s);

	nextsector = entry->curr_sector;
	mode = (mode_t)entry->generic_1;
	direntrysize = sizeof(struct smartfs_entry_header_s) + fs->fs_llformat.namesize;
	dummyentry.name = (FAR char *)kmm_malloc(fs->fs_llformat.namesize + 1);
	if (!dummyentry.name) {
		ret = -ENOSPC;
		goto err_out;
	}

	/* Search the parentsector for already written entry with the same name
	 * and deactivate it.
	 * It might have been created just before power failure.
	 */
	while (nextsector != SMARTFS_ERASEDSTATE_16BIT) {
		req.logsector = nextsector;
		req.count = fs->fs_llformat.availbytes;
		req.offset = 0;
		req.buffer = (uint8_t *)fs->fs_rwbuffer;
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&req);
		if (ret < 0) {
			goto err_out;
		}
		chainheader = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;
		nextsector = SMARTFS_NEXTSECTOR(chainheader);
		offset = sizeof(struct smartfs_chain_header_s);
		direntry = (struct smartfs_entry_header_s *)&(req.buffer[offset]);

		while (offset + direntrysize < fs->fs_llformat.availbytes) {
			if (ENTRY_VALID(direntry) && strncmp(direntry->name, name, fs->fs_llformat.namesize) == 0) {
				/* Entry found, Deactivate it */
#if CONFIG_SMARTFS_ERASEDSTATE == 0xFF
#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
				smartfs_wrle16(&direntry->flags, smartfs_rdle16(&direntry->flags) & ~SMARTFS_DIRENT_ACTIVE);
#else
				direntry->flags &= ~SMARTFS_DIRENT_ACTIVE;
#endif
#else							/* CONFIG_SMARTFS_ERASEDSTATE == 0xFF */
#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
				smartfs_wrle16(&direntry->flags, smartfs_rdle16(&direntry->flags) | SMARTFS_DIRENT_ACTIVE);
#else
				direntry->flags |= SMARTFS_DIRENT_ACTIVE;
#endif
#endif							/* CONFIG_SMARTFS_ERASEDSTATE == 0xFF */
				req.offset = offset;
				req.count = sizeof(uint16_t);
				req.buffer = (uint8_t *)&direntry->flags;
				ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&req);
				if (ret < 0) {
					fdbg("Error marking entry inactive at sector %d\n", req.logsector);
					goto err_out;
				}
				break;
			}
			offset += direntrysize;
			direntry = (struct smartfs_entry_header_s *)&(fs->fs_rwbuffer[offset]);
		}
	}
	/* Now create a fresh entry in the parent sector */
	ret = smartfs_createentry(fs, entry->curr_sector, name, type, mode, &dummyentry, firstsector, NULL);
err_out:
	if (dummyentry.name) {
		kmm_free(dummyentry.name);
	}
	if (ret != OK) {
		return ERROR;
	}
	return OK;
}

/****************************************************************************
 * Name: smartfs_redo_write
 *
 * Description: redo the file write
 *
 ****************************************************************************/
static int smartfs_redo_write(struct smartfs_mountpt_s *fs)
{
	int ret;
	struct smart_read_write_s req;
	struct smartfs_logging_entry_s *entry;
	struct journal_transaction_manager_s *j_mgr;

	j_mgr = fs->journal;
	entry = (struct smartfs_logging_entry_s *)(j_mgr->buffer);

	if (entry->datalen > 0) {
		req.logsector = entry->curr_sector;
		req.offset = entry->offset;
		req.count = entry->datalen;
		req.buffer = (uint8_t *)j_mgr->buffer + sizeof(struct smartfs_logging_entry_s);
		ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&req);
		if (ret != OK) {
			return ERROR;
		}
	}
	return OK;
}

/****************************************************************************
 * Name: smartfs_redo_delete
 *
 * Description: redo the delete operation for file/directory
 *
 ****************************************************************************/
static int smartfs_redo_delete(struct smartfs_mountpt_s *fs)
{
	int ret;
	struct smartfs_entry_s direntry;
	struct smartfs_logging_entry_s *entry;
	struct smart_read_write_s readwrite;
	struct smartfs_entry_header_s entryread;

	entry = (struct smartfs_logging_entry_s *)(fs->journal->buffer);

	readwrite.logsector = entry->curr_sector;
	readwrite.offset = entry->offset;
	readwrite.count = sizeof(struct smartfs_entry_header_s);
	readwrite.buffer = (uint8_t *)&entryread;
	ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
	if (ret < 0) {
		return OK;
	}
	if (ENTRY_VALID(&entryread)) {
		direntry.dsector = entry->curr_sector;
		direntry.doffset = entry->offset;
		direntry.dfirst = entry->datalen;
		direntry.firstsector = entry->generic_1;

		smartfs_deleteentry(fs, &direntry);
	} else {
		fdbg("No redo delete required\n");
	}
	return OK;
}

/****************************************************************************
 * Name: process_transactions
 *
 * Description: process the transactions which started but didnt finish.
 *              processing is done based on transaction type.
 *
 ****************************************************************************/
static int process_transaction(struct smartfs_mountpt_s *fs)
{
	int ret;
	struct smartfs_logging_entry_s *entry;
	struct journal_transaction_manager_s *j_mgr;

	ret = OK;
	j_mgr = fs->journal;
	entry = (struct smartfs_logging_entry_s *)(j_mgr->buffer);

	/* Check whether the transaction qualifies for redo */
	if (T_START_CHECK(entry->trans_info) && !T_FINISH_CHECK(entry->trans_info)) {
		/* Choose redo routine based on transaction type */
		fdbg("type : %d sector : %d generic_1 : %d\n", GET_TRANS_TYPE(entry->trans_info), entry->curr_sector, entry->generic_1);
		switch (GET_TRANS_TYPE(entry->trans_info)) {
		case T_SYNC:
		case T_WRITE:
			if (!T_NEEDSYNC_CHECK(entry->trans_info)) {
				/* If write doesnt need a sync, redo it immediately */
				ret = smartfs_redo_write(fs);
				break;
			}
			/* If write needs sync, add this information to the list containing info
			 * of sectors which need sync. These will be restored later
			 */
			return add_to_list(fs, j_mgr, j_mgr->sector, j_mgr->offset, JOURNAL_RESTORE);
		case T_CREATE:
			/* Redo create transactions for files so that a new sector is allocated as firstsector */
			ret = smartfs_redo_create(fs, SMARTFS_DIRENT_TYPE_FILE, 0xFFFF);
			break;
		case T_RENAME:
			ret = smartfs_redo_rename(fs);
			break;
		case T_DELETE:
			ret = smartfs_redo_delete(fs);
			break;
		case T_MKDIR:
			/* Redo create transactions for directory so that a new sector is allocated as firstsector */
			ret = smartfs_redo_create(fs, SMARTFS_DIRENT_TYPE_DIR, 0xFFFF);
			break;
		default:
			ret = ERROR;
		}
		if (ret != OK) {
			return ret;
		}
		/* Then set the transaction as finished */
		ret = smartfs_set_transaction(fs, j_mgr->sector, j_mgr->offset, TRANS_FINISHED);
	}
	return ret;
}

/****************************************************************************
 * Name: move_journal_area
 *
 * Description: When one journal area is filled, move the unfinished transac-
 *              tions to other journal area.
 *
 ****************************************************************************/
static int move_journal_area(struct smartfs_mountpt_s *fs)
{
	int ret;
	uint8_t read_area;
	uint8_t write_area;
	uint16_t readsect;
	uint16_t startsector;
	uint16_t readoffset;
	uint16_t newsector, newoffset;
	struct smartfs_logging_entry_s *entry;
	struct journal_transaction_manager_s *j_mgr;
	struct journal_transaction_manager_s temp_mgr;

	ret = OK;
	j_mgr = fs->journal;
	read_area = j_mgr->jarea;
	write_area = (read_area == 0) ? 1 : 0;

	temp_mgr.enabled = j_mgr->enabled;
	temp_mgr.jarea = read_area;
	temp_mgr.sector = SMARTFS_LOGGING_SECTOR + write_area * CONFIG_SMARTFS_NLOGGING_SECTORS;
	temp_mgr.offset = 1;
	temp_mgr.availbytes = j_mgr->availbytes;
	temp_mgr.active_sectors = j_mgr->active_sectors;
	temp_mgr.list = j_mgr->list;
	temp_mgr.buffer = (uint8_t *)kmm_malloc(sizeof(struct smartfs_logging_entry_s) + j_mgr->availbytes);
	if (!(temp_mgr.buffer)) {
		fdbg("out of memory\n");
		return -ENOMEM;
	}

	ret = set_area_id_bits(fs, AREA_OLD_UNERASED);
	if (ret != OK) {
		fdbg("set_area_id_bits failed bits : %d\n", AREA_OLD_UNERASED);
		goto errout;
	}
	entry = (struct smartfs_logging_entry_s *)temp_mgr.buffer;
	startsector = SMARTFS_LOGGING_SECTOR + read_area * CONFIG_SMARTFS_NLOGGING_SECTORS;
	readsect = startsector;
	readoffset = 1;

	while (readsect < startsector + CONFIG_SMARTFS_NLOGGING_SECTORS) {
		/* Read transactions in journal sector */

		if (read_logging_entry(fs, &temp_mgr, &readsect, &readoffset) != OK) {
			/* Failure here means no more entries exist/readable */
			ret = OK;
			break;
		}
		/* Check whether this transaction exists, and logging of transaction has been completed */
		if (T_EXIST_CHECK(entry->trans_info) && T_START_CHECK(entry->trans_info)) {
			/* If this entry has additional data, read additional data
			   Skip reading additional data if transaction os of type DELETE, as we have reused datalen field
			 */
			if (entry->datalen > 0 && GET_TRANS_TYPE(entry->trans_info) != T_DELETE) {
				/* If not a T_WRITE transaction which needs sync, read the additional data */
				ret = read_logging_data(fs, &temp_mgr, &readsect, &readoffset);
				if (ret != OK) {
					fdbg("Cannot read entry data.\n");
					goto errout;
				}
			}
			/* If not finished, copy this transaction */
			if (!T_FINISH_CHECK(entry->trans_info)) {
				/* Change the area to write */
				temp_mgr.jarea = write_area;
				ret = smartfs_write_transaction(fs, &temp_mgr, &newsector, &newoffset);
				if (ret != OK) {
					goto errout;
				}
				if (IS_ACTIVE(temp_mgr.active_sectors, entry->curr_sector)) {
					/* change the journal address of this transaction in the list */
					ret = add_to_list(fs, &temp_mgr, newsector, newoffset, JOURNAL_MODIFY_LOG);
					if (ret != OK) {
						goto errout;
					}
				}
				/* Change back the area to read */
				temp_mgr.jarea = read_area;
			}
		} else {
			break;
		}
	}

	j_mgr->jarea = write_area;
	ret = set_area_id_bits(fs, AREA_USED);
	if (ret != OK) {
		fdbg("set_area_id_bits failed bits : %d\n", AREA_USED);
		goto errout;
	}

	temp_mgr.jarea = read_area;
	ret = clear_journal_sectors(fs, &temp_mgr);
	if (ret != OK) {
		fdbg("clear_journal_sectors failed... ret : %d\n", ret);
		goto errout;
	}
	j_mgr->sector = temp_mgr.sector;
	j_mgr->offset = temp_mgr.offset;

errout:
	if (temp_mgr.buffer) {
		kmm_free(temp_mgr.buffer);
	}
	return ret;
}

/****************************************************************************
 * Name: smartfs_set_transaction
 *
 * Description: Mark a transaction located at offset 'offset' in logging sector
 *              'sector' to 'action'.
 *
 ****************************************************************************/
static int smartfs_set_transaction(struct smartfs_mountpt_s *fs, uint16_t sector, uint16_t offset, uint8_t action)
{
	int ret;
	struct smart_read_write_s req;
	struct smartfs_logging_entry_s *entry;
	struct journal_transaction_manager_s *j_mgr;

	j_mgr = fs->journal;
	entry = (struct smartfs_logging_entry_s *)(j_mgr->buffer);

	req.logsector = sector;
	req.offset = offset + offsetof(struct smartfs_logging_entry_s, trans_info);

	req.count = sizeof(entry->trans_info);
	req.buffer = (uint8_t *)&(entry->trans_info);
	if (req.offset + req.count <= j_mgr->availbytes) {
		/* Read current current transaction info */
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&req);
		if (ret < 0) {
			return ERROR;
		}

		T_SET_TRANSACTION(entry->trans_info, action);

		/* Write back the transaction info */
		ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&req);
		if (ret != OK) {
			fdbg("Writing failed %u %u\n", req.logsector, req.offset);
			return ERROR;
		}
	} else {
		/* Should never happen. Entry header should never be broken in 2 sectors. */
		fdbg("Entry header should never be broken in 2 sectors. offset : %d count : %d availbytes : %d\n", req.offset, req.count, j_mgr->availbytes);
		return ERROR;
	}
	return OK;
}

/****************************************************************************
 * Name: get_next_sector_info
 *
 * Description: Calculate next sector and offset from given condition.
 *              If current area filled with data, it returns value to change area.
 *
 ****************************************************************************/
static int get_next_sector_info(struct journal_transaction_manager_s *j_mgr)
{
	int ret;
	uint16_t startsector;
	uint16_t newoffset, newsector;
	struct smartfs_logging_entry_s *entry;

	ret = NO_CHANGE;
	newsector = j_mgr->sector;
	newoffset = j_mgr->offset;
	entry = (struct smartfs_logging_entry_s *)(j_mgr->buffer);
	startsector = SMARTFS_LOGGING_SECTOR + j_mgr->jarea * CONFIG_SMARTFS_NLOGGING_SECTORS;
	if ((j_mgr->offset + sizeof(struct smartfs_logging_entry_s)) > j_mgr->availbytes) {
		newsector++;
		newoffset = 0;
		ret = NEXT_SECTOR;
	}
	if (newsector >= startsector + CONFIG_SMARTFS_NLOGGING_SECTORS) {
		return MOVE_AREA;
	}

	newoffset += sizeof(struct smartfs_logging_entry_s) + (GET_TRANS_TYPE(entry->trans_info) != T_DELETE ? entry->datalen : 0);

	if (newoffset > j_mgr->availbytes) {
		newsector++;
	}
	if (newsector >= startsector + CONFIG_SMARTFS_NLOGGING_SECTORS) {
		ret = MOVE_AREA;
	}
	return ret;
}
/****************************************************************************
 * Name: smartfs_write_transaction
 *
 * Description: Write a transaction entry to Journal log at position 'sector'
 *              and 'offset' of journal manager. Updates the 'sector'
 *              and 'offset' of journal manager to logging sector and offset
 *              where next transaction can be written.
 *
 ****************************************************************************/

static int smartfs_write_transaction(struct smartfs_mountpt_s *fs, struct journal_transaction_manager_s *j_mgr,
									 uint16_t *sector, uint16_t *offset)
{
	int ret;
	int info;
	uint16_t startsector;
	struct smart_read_write_s req;
	struct smartfs_logging_entry_s *entry;

	startsector = SMARTFS_LOGGING_SECTOR + j_mgr->jarea * CONFIG_SMARTFS_NLOGGING_SECTORS;
	entry = (struct smartfs_logging_entry_s *)(j_mgr->buffer);

	info = get_next_sector_info(j_mgr);
	if (info == NEXT_SECTOR) {
		j_mgr->sector++;
		j_mgr->offset = 0;
	} else if (info == MOVE_AREA) {
		fvdbg("entry : %d\n", entry->datalen);
		ret = move_journal_area(fs);
		if (ret != OK) {
			fdbg("move_journal_area failed ret : %d\n", ret);
			return ret;
		}
		/* change startsector again here because area changed */
		startsector = SMARTFS_LOGGING_SECTOR + j_mgr->jarea * CONFIG_SMARTFS_NLOGGING_SECTORS;
	}

	*sector = j_mgr->sector;
	*offset = j_mgr->offset;
	req.logsector = *sector;
	req.offset = *offset;
	req.count = sizeof(struct smartfs_logging_entry_s);
	req.buffer = j_mgr->buffer;
	/* Write the entry */
	ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&req);
	if (ret != OK) {
		fdbg("write entry failed ret : %d\n", ret);
		return ret;
	}
	/* Increment the journal manager offset */
	j_mgr->offset += req.count;
	req.offset += req.count;
	req.buffer += req.count;
	if (entry->datalen > 0 && GET_TRANS_TYPE(entry->trans_info) != T_DELETE) {
		/* Write data if present. We also check transaction type because we are
		 * reusing datalen field of logging entry in case of T_DELETE
		 */
		req.count = entry->datalen;

		if (req.offset + entry->datalen > j_mgr->availbytes) {
			/* Not enough space to write full data in the same sector.
			 * Write only what can be accomodated
			 */
			req.count = j_mgr->availbytes - req.offset;
		}

		if (req.count > 0) {
			/* Write the data */
			ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&req);
			if (ret != OK) {
				fdbg("write data failed ret : %d\n", ret);
				return ret;
			}
			req.buffer += req.count;
			j_mgr->offset += req.count;
		}
		if (req.count < entry->datalen) {
			/* If any data is left to write, go to next sector */
			req.logsector++;
			if (req.logsector >= startsector + CONFIG_SMARTFS_NLOGGING_SECTORS) {
				/* This case should have been handled above. */
				fdbg("logical sector is too big!! %d\n", req.logsector);
				return -ENOSPC;
			}
			req.offset = 0;
			req.count = entry->datalen - req.count;

			/* Write remaining data */
			ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&req);
			if (ret != OK) {
				fdbg("write remained data failed ret : %d\n", ret);
				return ret;
			}
			/* Update journal manager sector and offset */
			j_mgr->sector = req.logsector;
			j_mgr->offset = req.count;
		}
	}

	/* Mark the transaction as STARTED */
	ret = smartfs_set_transaction(fs, *sector, *offset, TRANS_STARTED);
	if (ret != OK) {
		fdbg("setting status failed : %d\n", ret);
	}

	return ret;
}

/****************************************************************************
 * Name: smartfs_set_sequence
 *
 * Description: Set the sequence number of a logged write/sync transaction to seq.
 *
 ****************************************************************************/
static int smartfs_set_sequence(struct smartfs_mountpt_s *fs, uint16_t sector, uint16_t offset, uint8_t seq)
{
	struct smart_read_write_s req;
	struct smartfs_logging_entry_s *entry;
	struct journal_transaction_manager_s *j_mgr;
	int ret;

	j_mgr = fs->journal;
	entry = (struct smartfs_logging_entry_s *)(j_mgr->buffer);
	entry->seq_no = seq;

	req.logsector = sector;
	req.offset = offset + offsetof(struct smartfs_logging_entry_s, seq_no);

	req.count = sizeof(entry->seq_no);
	req.buffer = (uint8_t *)&(entry->seq_no);
	if (req.offset + req.count <= j_mgr->availbytes)  {
		ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&req);
		if (ret != OK) {
			fdbg("set sequence failed.. seq no : %d logicsector : %d\n", seq, sector);
		}
		return ret;
	}
	return ERROR;
}

/****************************************************************************
 * Name: add_to_list
 *
 * Description: Add a T_WRITE transaction located at (j_sector, j_offset)
 *              to journal_transaction_manager_s's write transactions list.
 *              If a transaction for same sector is already present,
 *              finish that transaction and remove it from the list.
 *
 ****************************************************************************/
static int add_to_list(struct smartfs_mountpt_s *fs, struct journal_transaction_manager_s *j_mgr,
					   uint16_t j_sector, uint16_t j_offset, enum journal_action_e action)
{
	int ret;
	struct active_write_node_s *node;
	struct smartfs_logging_entry_s *entry;

	entry = (struct smartfs_logging_entry_s *)(j_mgr->buffer);

	/* If another write transaction for same sector which needs sync
	   does not exist, create a new list node */
	if (!IS_ACTIVE(j_mgr->active_sectors, entry->curr_sector)) {
		node = (struct active_write_node_s *)kmm_malloc(sizeof(struct active_write_node_s));
		if (!node) {
			return -ENOSPC;
		}
		node->next = j_mgr->list;
		j_mgr->list = node;
	} else {
		/* If it exists, search the old node for this sector */
		for (node = j_mgr->list; node != NULL; node = node->next) {
			if (node->sector == entry->curr_sector) {
				if (action == JOURNAL_LOG || action == JOURNAL_MODIFY_LOG) {
					/* if it is a logging operation, break */
					break;
				}
				if (action == JOURNAL_RESTORE) {
					/* if it is a restore operation, break only if previous transaction os older */
					if (node->seq_no < entry->seq_no) {
						break;
					}
					/* Existing transaction in the list is newer than the newly read
					 * transaction. No need to add this transaction to list, because
					 * only the sector size after the newer write transaction will be synced
					 */
					return OK;

				}
			}
		}
	}

	/* Now update the transaction info on the node */
	if (!node) {
		return ERROR;
	}
	node->trans_info = entry->trans_info;
	node->sector = entry->curr_sector;
	node->used_bytes = entry->generic_1;
	node->seq_no = entry->seq_no;

	/* If transaction for this sector already existed, finish the previous transaction.
	   Skip if action is to modify existing log */

	if (IS_ACTIVE(j_mgr->active_sectors, node->sector) && action != JOURNAL_MODIFY_LOG) {
		ret = smartfs_set_transaction(fs, node->journal_sector, node->journal_offset, TRANS_FINISHED);
		if (ret != OK) {
			return ERROR;
		}

		if (action == JOURNAL_LOG) {
			/* As the older transaction for same sector has been marked finished, set the
			 * sequence number of new transaction to 0. There is no need to do this during
			 * restore, because more than 2 transactions for same sector cannot exist.
			 */
			ret = smartfs_set_sequence(fs, j_sector, j_offset, 0);
			if (ret != OK) {
				return ERROR;
			}
		}
	}

	node->journal_sector = j_sector;
	node->journal_offset = j_offset;
	/* mark the sector as active */
	SET_ACTIVE(j_mgr->active_sectors, node->sector);

	return OK;
}

/****************************************************************************
 * Name: remove_from_list
 *
 * Description: remove a transaction from the active transactions list
 *
 ****************************************************************************/
static int remove_from_list(struct journal_transaction_manager_s *j_mgr, uint16_t sector)
{
	int ret;
	struct active_write_node_s *temp;
	struct active_write_node_s *prev;

	ret = ERROR;
	if (!(j_mgr->list)) {
		/* List doesnt exist or Sector doesnt exist in the list */
		goto errout;
	}
	temp = j_mgr->list;
	if (temp->sector == sector) {
		j_mgr->list = temp->next;
		kmm_free(temp);
		ret = OK;
		SET_INACTIVE(j_mgr->active_sectors, sector);
		goto errout;
	}
	prev = temp;
	temp = temp->next;
	while (temp != NULL) {
		if (temp->sector == sector) {
			prev->next = temp->next;
			kmm_free(temp);
			ret = OK;
			break;
		}
		prev = temp;
		temp = temp->next;
	}
	SET_INACTIVE(j_mgr->active_sectors, sector);
errout:
	return ret;
}

/****************************************************************************
 * Name: smartfs_create_journalentry
 *
 * Description: creates a journal entry based on transaction type.
 *
 ****************************************************************************/
int smartfs_create_journalentry(struct smartfs_mountpt_s *fs, enum logging_transaction_type_e type, uint16_t curr_sector, uint16_t offset, uint16_t datalen, uint16_t genericdata, uint8_t needsync, const uint8_t *data, uint16_t *t_sector, uint16_t *t_offset)
{
	int ret;
	struct smartfs_logging_entry_s *entry;
	struct journal_transaction_manager_s *j_mgr;

	j_mgr = fs->journal;
	if (!j_mgr) {
		fdbg("Journal manager not available\n");
		return ERROR;
	}
	if (!(j_mgr->enabled)) {
		fdbg("Journal Manager Not enabled due to size constraints.\n");
		return OK;
	}

	entry = (struct smartfs_logging_entry_s *)(j_mgr->buffer);

	T_STATUS_RESET(entry->trans_info);
	SET_TRANS_TYPE(entry->trans_info, type);
	T_SET_TRANSACTION(entry->trans_info, TRANS_EXIST);

	if (needsync) {
		T_SET_TRANSACTION(entry->trans_info, TRANS_NEED_SYNC);
	}
	entry->curr_sector = curr_sector;
	entry->offset = offset;
	entry->datalen = datalen;
	entry->generic_1 = genericdata;
	entry->seq_no = 0;
	entry->crc16[0] = smartfs_calc_crc_entry(j_mgr);
	if (datalen && type != T_DELETE) {
		memcpy(j_mgr->buffer + sizeof(struct smartfs_logging_entry_s), data, datalen);
		entry->crc16[1] = smartfs_calc_crc_data(j_mgr);
	} else {
		entry->crc16[1] = CONFIG_SMARTFS_ERASEDSTATE;
	}

	switch (type) {
	case T_SYNC:
	case T_WRITE:
		/* Store the position of writing new transaction, as journal manager
		 * will be updated. */
		if (IS_ACTIVE(j_mgr->active_sectors, entry->curr_sector)) {
			/* If another write transaction with sync exists for this sector,
			   set the sequence numbre to 1 */
			entry->seq_no = 1;
		} else {
			entry->seq_no = 0;
		}
		/* Now write the transaction on disk */
		ret = smartfs_write_transaction(fs, j_mgr, t_sector, t_offset);
		if (ret != OK) {
			fdbg("smartfs_write_transaction failed type : %d ret : %d\n", type, ret);
			return ret;
		}

		if (T_NEEDSYNC_CHECK(entry->trans_info)) {
			/* If the write requires a sync (when written data is appended), we add
			* it to list */
			ret = add_to_list(fs, j_mgr, *t_sector, *t_offset, JOURNAL_LOG);
			if (ret != OK) {
				fdbg("add_to_list failed ret : %d\n", ret);
				return ret;
			}
		}
		break;

	default:
		ret = smartfs_write_transaction(fs, j_mgr, t_sector, t_offset);
		if (ret != OK) {
			fdbg("smartfs_write_transaction failed type : %d ret : %d\n", type, ret);
			return ret;
		}
#ifdef CONFIG_SMARTFS_JOURNAL_VERIFY
		if (type != T_DELETE) {
			fdbg("write transaction %d %d %d \n", type, *t_sector, *t_offset);
			ret = smartfs_verify_transaction(fs, *t_sector, *t_offset);
			if (ret != OK) {
				fdbg("smartfs_verify_transaction failed: %d ret : %d\n", type, ret);
				return ret;
			}
		}
#endif
		break;
	}

	fvdbg("Create journalentry : %d %d\n", *t_sector, *t_offset);
	return OK;
}

#ifdef CONFIG_SMARTFS_JOURNAL_VERIFY
/****************************************************************************
 * Name: smartfs_verify_transaction
 *
 * Description: verify after write transaction
 *
 ****************************************************************************/
static int smartfs_verify_transaction(struct smartfs_mountpt_s *fs, uint16_t sector, uint16_t offset)
{
	struct smart_read_write_s req;
	struct journal_transaction_manager_s *j_mgr;
	struct smartfs_logging_entry_s *entry;
	uint8_t *buff;
	int i;
	int ret;

	j_mgr = fs->journal;
	entry = (struct smartfs_logging_entry_s *)(j_mgr->buffer);

	buff = (uint8_t *)malloc(entry->datalen + sizeof(struct smartfs_logging_entry_s));

	if (!buff) {
		fdbg("mem alloc fail\n");
		return ERROR;
	}

	req.logsector = sector;
	req.offset = offset;
	req.count = sizeof(struct smartfs_logging_entry_s);
	req.buffer = buff;
	/* read entry first */
	fvdbg("read entry :: sector : %d offset :%d cnt: %d\n", req.logsector, req.offset, req.count);
	ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&req);
	if (ret < 0) {
		fdbg("read fail %d\n", ret);
		goto error_with_ret;
	}


	req.buffer += req.count;
	req.offset += req.count;
	req.count = entry->datalen;
	if (req.offset + entry->datalen > j_mgr->availbytes) {
		req.count = j_mgr->availbytes - req.offset;
	}
	if (req.count > 0) {
		fvdbg("read data sector :: %d offset :%d cnt: %d\n", req.logsector, req.offset, req.count);
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&req);
		if (ret < 0) {
			fdbg("read fail %d\n", ret);
			goto error_with_ret;
		}
		req.buffer += req.count;
		req.offset += req.count;
	}

	if (req.count < entry->datalen) {
		req.logsector++;
		req.offset = 0;
		req.count = entry->datalen - req.count;
		fvdbg("read remain data:: sector : %d offset :%d cnt: %d\n", req.logsector, req.offset, req.count);
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&req);
		if (ret < 0) {
			fdbg("read fail %d\n", ret);
			goto error_with_ret;
		}
	}

	for (i = 0; i < entry->datalen + sizeof(struct smartfs_logging_entry_s); i++) {
		if (buff[i] != j_mgr->buffer[i]) {
			fdbg("journal data write fail :: offset : %d write : 0x%x read : 0x%x\n", i, j_mgr->buffer[i], buff[i]);
			goto error_with_dumpret;
		}
	}

	free(buff);
	return OK;


error_with_dumpret:
	for (i = 0; i < entry->datalen + sizeof(struct smartfs_logging_entry_s); i++) {
		fdbg("offset : %d write : 0x%x read : 0x%x\n", i, j_mgr->buffer[i], buff[i]);
	}
error_with_ret:
	free(buff);
	return ERROR;
}
#endif

/****************************************************************************
 * Name: smartfs_calc_crc_entry
 *
 * Description: Calculates crc value for journal entry
 *
 ****************************************************************************/
static uint8_t smartfs_calc_crc_entry(struct journal_transaction_manager_s *j_mgr)
{
	uint8_t crc = 0;
	uint16_t offset;
	struct smartfs_logging_entry_s *entry;

	offset = offsetof(struct smartfs_logging_entry_s, crc16) + sizeof(entry->crc16);
	entry = (struct smartfs_logging_entry_s *)j_mgr->buffer;

	crc = crc8((uint8_t *)&j_mgr->buffer[offset], sizeof(struct smartfs_logging_entry_s) - offset);
	return crc;
}

/****************************************************************************
 * Name: smartfs_calc_crc_data
 *
 * Description: Calculates crc value for journal entry data
 *
 ****************************************************************************/
static uint8_t smartfs_calc_crc_data(struct journal_transaction_manager_s *j_mgr)
{
	uint8_t crc = 0;
	uint16_t offset;
	struct smartfs_logging_entry_s *entry;

	offset = offsetof(struct smartfs_logging_entry_s, crc16) + sizeof(entry->crc16);
	entry = (struct smartfs_logging_entry_s *)j_mgr->buffer;

	crc = crc8((uint8_t *)&j_mgr->buffer[offset], entry->datalen + sizeof(struct smartfs_logging_entry_s) - offset);
	return crc;
}




/****************************************************************************
 * Name: smartfs_finish_journalentry
 *
 * Description: sets a journal entry state to finish.
 *              curr_sector : sector for which the entry belongs
 *              sector: journal sector of location of entry
 *              offset: journal sector offset of entry
 *
 ****************************************************************************/
int smartfs_finish_journalentry(struct smartfs_mountpt_s *fs, uint16_t curr_sector, uint16_t sector, uint16_t offset, enum logging_transaction_type_e type)
{
	struct journal_transaction_manager_s *j_mgr;

	j_mgr = fs->journal;
	if (!j_mgr) {
		fdbg("Journal manager not available\n");
		return ERROR;
	}
	if (!(j_mgr->enabled)) {
		fdbg("Journal Manager Not enabled due to size constraints.\n");
		return OK;
	}
	/* If sector is in the list and action is FINISH, remove sector from list */
	if (IS_ACTIVE(j_mgr->active_sectors, curr_sector) && type == T_SYNC) {
		remove_from_list(j_mgr, curr_sector);
	}
	return smartfs_set_transaction(fs, sector, offset, TRANS_FINISHED);
}
#endif /* END OF CONFIG_SMARTFS_JOURNALING */

#ifdef CONFIG_SMARTFS_SECURE
/****************************************************************************
 * Name: smartfs_decrypt
 *
 * Description: Decrypts the given buffer using AES128 decryption technique
 *		and stores the result in final_output variable.
 ****************************************************************************/
int smartfs_decrypt(char *buffer, int ret, char *final_output)
{
	int byteread;
	int bytepointer;
	int bufferpointer;
	int loopcounter;
	char temp[CRYPTSIZE];

	/* Generating round keys for decryption */

	byteread = 0;
	bytepointer = 0;
	bufferpointer = 0;
	/*Looping for entire buffer length */
	/*Ret variable contains the number of bytes read from the given file */

	while (byteread < ret) {
		strncpy((char *)temp, (const char *)buffer + bytepointer, CRYPTSIZE);
		bytepointer += CRYPTSIZE;
		loopcounter = 0;
		if (temp[0] == '\0') {
			break;
		}
		/* Decrypting the buffer data */

		crypto_aes_decrypt((uint8_t *)temp, (const uint8_t *)CRYPT_KEY);
		/*Copying the decrypted data into the Output buffer*/

		while (loopcounter < CRYPTSIZE && temp[loopcounter] != '\0') {
			final_output[bufferpointer] = temp[loopcounter];
			loopcounter++;
			bufferpointer++;
		}
		byteread += CRYPTSIZE;
	}
	return 0;
}

/****************************************************************************
 * Name: smartfs_encrypt
 *
 * Description: Encrypts the given buffer using AES-128 encryption and
 *              stores it in output variable
 *
 ****************************************************************************/
int smartfs_encrypt(const char *buffer, size_t buflen, unsigned char output[])
{
	int bytesread;
	int bytepointer;
	int nbytes;
	int loopcounter;
	char temp[CRYPTSIZE];

	bytesread = 0;
	nbytes = buflen;
	bytepointer = 0;

	/*Encryption loop for entire input data*/

	while (bytesread < nbytes) {
		memset(temp, '\0', CRYPTSIZE);
		/* Considering only 16 bytes of the input data*/

		if (nbytes - bytesread <= CRYPTSIZE) {
			strncpy((char *)temp, (const char *)buffer + bytepointer, nbytes - bytesread);
		} else {
			strncpy((char *)temp, (const char *)buffer + bytepointer, CRYPTSIZE);
			bytepointer += CRYPTSIZE;
		}
		loopcounter = 0;
		/*Encrypting the data */

		crypto_aes_encrypt((uint8_t *)temp, (const uint8_t *)CRYPT_KEY);
		/*Copying the encrypted data into output buffer */

		while (loopcounter < CRYPTSIZE && temp[loopcounter] != '\0') {
			output[loopcounter + bytesread] = temp[loopcounter];
			loopcounter++;
		}
		bytesread += CRYPTSIZE;
	}
	return 0;
}
#endif /*END OF CONFIG_SMARTFS_SECURE*/
