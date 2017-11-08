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
 * fs/smartfs/smartfs.h
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

#ifndef __FS_SMARTFS_SMARTFS_H
#define __FS_SMARTFS_SMARTFS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>

#include <tinyara/fs/mtd.h>
#include <tinyara/fs/smart.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* SMART Definitions ********************************************************/
/* General SMART organization.  The following example assumes 4 logical
 * sectors per FLASH erase block.  The actual relationship is determined by
 * the FLASH geometry reported by the MTD driver.
 *
 * ERASE LOGICAL                   Sectors begin with a sector header.  Sectors may
 * BLOCK SECTOR      CONTENTS      be marked as "released," pending garbage collection
 *   n   4*n     --+---------------+
 *             Hdr |LLLLLLLLLLLLLLL| Logical sector number (2 bytes)
 *                 |QQQQQQQQQQQQQQQ| Sequence number (2 bytes)
 *                 |SSSSSSSSSSSSSSS| Status bits (1 byte)
 *                 +---------------+
 *                 |TTTTTTTTTTTTTTT| Sector Type (dir or file) (1 byte)
 *                 |NNNNNNNNNNNNNNN| Number of next logical sector in chain
 *                 |UUUUUUUUUUUUUUU| Number of bytes used in this sector
 *                 |               |
 *                 | (Sector Data) |
 *                 |               |
 *       4*n+1   --+---------------+
 *      Sector Hdr |LLLLLLLLLLLLLLL| Logical sector number (2 bytes)
 *                 |QQQQQQQQQQQQQQQ| Sequence number (2 bytes)
 *                 |SSSSSSSSSSSSSSS| Status bits (1 byte)
 *                 +---------------+
 *          FS Hdr |TTTTTTTTTTTTTTT| Sector Type (dir or file) (1 byte)
 *                 |NNNNNNNNNNNNNNN| Number of next logical sector in chain
 *                 |UUUUUUUUUUUUUUU| Number of bytes used in this sector
 *                 |               |
 *                 | (Sector Data) |
 *                 |               |
 *       4*n+2   --+---------------+
 *      Sector Hdr |LLLLLLLLLLLLLLL| Logical sector number (2 bytes)
 *                 |QQQQQQQQQQQQQQQ| Sequence number (2 bytes)
 *                 |SSSSSSSSSSSSSSS| Status bits (1 byte)
 *                 +---------------+
 *          FS Hdr |TTTTTTTTTTTTTTT| Sector Type (dir or file) (1 byte)
 *                 |NNNNNNNNNNNNNNN| Number of next logical sector in chain
 *                 |UUUUUUUUUUUUUUU| Number of bytes used in this sector
 *                 |               |
 *                 | (Sector Data) |
 *                 |               |
 *       4*n+3   --+---------------+
 *      Sector Hdr |LLLLLLLLLLLLLLL| Logical sector number (2 bytes)
 *                 |QQQQQQQQQQQQQQQ| Sequence number (2 bytes)
 *                 |SSSSSSSSSSSSSSS| Status bits (1 byte)
 *                 +---------------+
 *          FS Hdr |TTTTTTTTTTTTTTT| Sector Type (dir or file) (1 byte)
 *                 |NNNNNNNNNNNNNNN| Number of next logical sector in chain
 *                 |UUUUUUUUUUUUUUU| Number of bytes used in this sector
 *                 |               |
 *                 | (Sector Data) |
 *                 |               |
 *  n+1  4*(n+1) --+---------------+
 *      Sector Hdr |LLLLLLLLLLLLLLL| Logical sector number (2 bytes)
 *                 |QQQQQQQQQQQQQQQ| Sequence number (2 bytes)
 *                 |SSSSSSSSSSSSSSS| Status bits (1 byte)
 *                 +---------------+
 *          FS Hdr |TTTTTTTTTTTTTTT| Sector Type (dir or file) (1 byte)
 *                 |NNNNNNNNNNNNNNN| Number of next logical sector in chain
 *                 |UUUUUUUUUUUUUUU| Number of bytes used in this sector
 *                 |               |
 *                 |               |
 *                 |               |
 *               --+---------------+
 *
 * General operation:
 *   Physical sectors are allocated and assigned a logical sector number
 *   and a starting sequence number of zero.
 *
 * SECTOR HEADER:
 *   The sector header (first 5 bytes) tracks the state of each sector and
 *   is used by the SMART MTD block driver.  At the block level, there is
 *   no notion of sector chaining, only allocated sectors within erase
 *   blocks.
 *
 * FILE SYSTEM (FS) HEADER:
 *   The file system header (next 5 bytes) tracks file and directory entries
 *   and chains.
 *
 * SMART Limitations:
 * 1. SMART currently depends on the underlying MTD block driver supporting
 *    single-byte programming operation.  This is due to the method it
 *    uses for marking a sector as "released", committed, etc.
 * 2. Garbage collection can occur when a new sector is allocated or when
 *    existing sector data is overwritten with new data. Thus, occasionally,
 *    file writing may take longer than other times.
 * 3. The implementation curently does not track bad blocks on the device.
 * 4. There is no true wear-leveling implemented yet, though provesion have
 *    been made to reserve logical sectors to allow it to be added using
 *    a "sector aging" tracking mechanism.
 */

/* Values for SMART inode state.
 *
 * SMART_STATE_FILE    - The inode is a valid usuable, file
 * INODE_STATE_DELETED - The inode has been deleted.
 * Other values        - The inode is bad and has an invalid state.
 *
 * Care is taken so that the VALID to DELETED transition only involves burning
 * bits from the erased to non-erased state.
 */

#ifdef CONFIG_SMARTFS_SECURE
#define MAXBUFSIZE 256
#define CRYPTSIZE 16
#define KEYSIZE 128
#define CRYPT_KEY "12345678"
#endif

#define INODE_STATE_FILE          (CONFIG_NXFFS_ERASEDSTATE ^ 0x22)
#define INODE_STATE_DELETED       (CONFIG_NXFFS_ERASEDSTATE ^ 0xaa)

/* Directory entry flag definitions */

#define SMARTFS_DIRENT_EMPTY      0x8000	/* Set to non-erase state when entry used */
#define SMARTFS_DIRENT_ACTIVE     0x4000	/* Set to erase state when entry is active */
#define SMARTFS_DIRENT_TYPE       0x2000	/* Indicates the type of entry (file/dir) */
#define SMARTFS_DIRENT_DELETING   0x1000	/* Directory entry is being deleted */
#define SMARTFS_DIRENT_RESERVED   0x0E00	/* Reserved bits */
#define SMARTFS_DIRENT_MODE       0x01FF	/* Mode the file was created with */

#define SMARTFS_DIRENT_TYPE_DIR   0x2000
#define SMARTFS_DIRENT_TYPE_FILE  0x0000

/* Number of bytes in the SMART magic sequences */

#define SMART_MAGICSIZE           4

/* Quasi-standard definitions */

#ifndef MIN
#define MIN(a, b)                (a < b ? a : b)
#endif

#ifndef MAX
#define MAX(a, b)                (a > b ? a : b)
#endif

/* Underlying MTD Block driver access functions */

#define FS_BOPS(f)        (f)->fs_blkdriver->u.i_bops
#define FS_IOCTL(f, c, a) (FS_BOPS(f)->ioctl ? FS_BOPS(f)->ioctl((f)->fs_blkdriver, c, a) : (-ENOSYS))

/* The logical sector number of the root directory. */

#define SMARTFS_ROOT_DIR_SECTOR   3

/* Defines the sector types */

#define SMARTFS_SECTOR_TYPE_DIR   1
#define SMARTFS_SECTOR_TYPE_FILE  2

#ifndef CONFIG_SMARTFS_DIRDEPTH
#define CONFIG_SMARTFS_DIRDEPTH 8
#endif

/* Buffer flags (when CRC enabled) */

#define SMARTFS_BFLAG_DIRTY       0x01	/* Set if data changed in the sector */
#define SMARTFS_BFLAG_NEWALLOC    0x02	/* Set if sector not written since alloc */

#define SMARTFS_ERASEDSTATE_16BIT (uint16_t)((CONFIG_SMARTFS_ERASEDSTATE << 8) | \
								  CONFIG_SMARTFS_ERASEDSTATE)

#ifndef offsetof
#define offsetof(type, member)   ((size_t)&(((type *)0)->member))
#endif
#define UINT8_TO_UINT16(UINT8_ARRAY)                    ((uint16_t)(((uint16_t)UINT8_ARRAY[1] << 8) & 0xFF00) | UINT8_ARRAY[0])
#define SMARTFS_NEXTSECTOR(h)   (UINT8_TO_UINT16(h->nextsector))
#define SMARTFS_USED(h)                 (UINT8_TO_UINT16(h->used))
#ifdef CONFIG_MTD_SMART_ENABLE_CRC
#define CONFIG_SMARTFS_USE_SECTOR_BUFFER
#endif
#ifdef CONFIG_SMARTFS_BAD_SECTOR
#define SMARTFS_BSM_LOG_SECTOR_NUMBER   11
#endif

#define USED_ARRAY_SIZE                 2

#if !defined(CONFIG_SMARTFS_DYNAMIC_HEADER) || !defined(CONFIG_MTD_SMART_SECTOR_SIZE)
#undef  CONFIG_SMARTFS_DYNAMIC_HEADER
#endif

#ifdef CONFIG_SMARTFS_JOURNALING

/*
 * Journal entry info
 * 1st 4 bytes : Transaction Type
 * Last 4 bytes : Transaction status
 */

#define SMARTFS_LOGGING_SECTOR   12

#define TRANS_EXIST (1 << 0)
#define TRANS_STARTED (1 << 1)
#define TRANS_FINISHED (1 << 2)
#define TRANS_NEED_SYNC (1 << 3)	/* Note: If true, it means that this transaction needs sync only if it is not finished */

#define GET_TRANS_TYPE(t) ((t) >> 4)
#define SET_TRANS_TYPE(t, v) ((t) = ((t) & 0x0f) | ((v) << 4))

#define T_STATUS_RESET(t) ((t) = ((t) | (CONFIG_SMARTFS_ERASEDSTATE >> 4)))
#if CONFIG_SMARTFS_ERASEDSTATE == 0xFF
#define T_SET_TRANSACTION(t, s) ((t) = ((t) & ~(s)))
#else
#define T_SET_TRANSACTION(t, s) ((t) = ((t) | (s)))
#endif

#define T_FINISH_CHECK(t)  (((t) & TRANS_FINISHED) != (TRANS_FINISHED & CONFIG_SMARTFS_ERASEDSTATE))
#define T_START_CHECK(t)  (((t) & TRANS_STARTED) != (TRANS_STARTED & CONFIG_SMARTFS_ERASEDSTATE))
#define T_EXIST_CHECK(t)  (((t) & TRANS_EXIST) != (TRANS_EXIST & CONFIG_SMARTFS_ERASEDSTATE))
#define T_NEEDSYNC_CHECK(t) (((t) & TRANS_NEED_SYNC) != (TRANS_NEED_SYNC & CONFIG_SMARTFS_ERASEDSTATE))

#endif							/* CONFIG_SMARTFS_JOURNALING */
/****************************************************************************
 * Public Types
 ****************************************************************************/

/* This structure defines each packed block on the FLASH media */

/* This is an in-memory representation of the SMART inode as extracted from
 * FLASH and with additional state information.
 */

struct smartfs_entry_s {
	uint16_t firstsector;		/* Sector number of the name */
	uint16_t dsector;			/* Sector number of the directory entry */
	uint16_t doffset;			/* Offset of the directory entry */
	uint16_t dfirst;			/* 1st sector number of the directory entry */
	uint16_t flags;				/* Flags, including mode */
	FAR char *name;				/* inode name */
	uint32_t utc;				/* Time stamp */
	uint32_t datlen;			/* Length of inode data */
};

/* This is an on-device representation of the SMART inode it esists on
 * the FLASH.
 */

struct smartfs_entry_header_s {
	uint16_t flags;				/* Flags, including permissions:
								   15:   Empty entry
								   14:   Active entry
								   12-0: Permissions bits */
	int16_t firstsector;		/* Sector number of the name */
	uint32_t utc;				/* Time stamp */
	char name[0];				/* inode name */
};

/* This structure describes the smartfs header at the start of each
 * sector.  It manages the sector chain and used bytes in the sector.
 */

#if defined(CONFIG_MTD_SMART_ENABLE_CRC) && defined(CONFIG_SMART_CRC_32)
struct smartfs_chain_header_s {
	uint8_t nextsector[4];		/* Next logical sector in the chain */
	uint8_t used[4];			/* Number of bytes used in this sector */
	uint8_t type;				/* Type of sector entry (file or dir) */
};
#elif defined(CONFIG_MTD_SMART_ENABLE_CRC) && defined(CONFIG_SMART_CRC_16)
struct smartfs_chain_header_s {
	uint8_t type;				/* Type of sector entry (file or dir) */
	uint8_t nextsector[2];		/* Next logical sector in the chain */
	uint8_t used[2];			/* Number of bytes used in this sector */
};
#else
struct smartfs_chain_header_s {
	uint8_t type;				/* Type of sector entry (file or dir) */
	uint8_t nextsector[2];		/* Next logical sector in the chain */
	uint8_t used[2];			/* Number of bytes used in this sector */
};
#endif

/* This structure describes the state of one open file.  This structure
 * is protected by the volume semaphore.
 */

struct smartfs_ofile_s {
	struct smartfs_ofile_s *fnext;	/* Supports a singly linked list */
#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
	uint8_t *buffer;			/* Sector buffer to reduce writes */
	uint8_t bflags;				/* Buffer flags */
#endif
	int16_t crefs;				/* Reference count */
	mode_t oflags;				/* Open mode */
	struct smartfs_entry_s entry;	/* Describes the SMARTFS inode entry */
	size_t filepos;				/* Current file position */
	uint16_t currsector;		/* Current sector of filepos */
	uint16_t curroffset;		/* Current offset in sector */
	uint16_t byteswritten;		/* Count of bytes written to currsector
								 * that have not been recorded in the
								 * sector yet.  We delay updating the
								 * used field until the file is closed,
								 * a seek, or more data is written that
								 * causes the sector to change. */
};

/* This structure represents the overall mountpoint state.  An instance of this
 * structure is retained as inode private data on each mountpoint that is
 * mounted with a smartfs filesystem.
 */

struct smartfs_mountpt_s {
#if defined(CONFIG_SMARTFS_MULTI_ROOT_DIRS) || defined(CONFIG_FS_PROCFS)
	struct smartfs_mountpt_s *fs_next;	/* Pointer to next SMART filesystem */
#endif
	FAR struct inode *fs_blkdriver;	/* Our underlying block device */
	sem_t *fs_sem;			/* Used to assure thread-safe access */
	FAR struct smartfs_ofile_s
			*fs_head;					/* A singly-linked list of open files */
	bool fs_mounted;			/* true: The file system is ready */
	struct smart_format_s fs_llformat;	/* Low level device format info */
	char *fs_rwbuffer;			/* Read/Write working buffer */
	char *fs_workbuffer;		/* Working buffer */
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
	uint8_t *fs_chunk_buffer;
#endif
#ifdef CONFIG_SMARTFS_JOURNALING
	struct journal_transaction_manager_s *journal;
#endif
	uint8_t fs_rootsector;		/* Root directory sector num */
};

#ifdef CONFIG_SMARTFS_JOURNALING

enum logging_transaction_type_e {
	T_WRITE = 0,				/* for smartfs_write */
	T_CREATE,					/* Used when new entry for file is created */
	T_RENAME,					/* for smartfs_rename */
	T_MKDIR,					/* for smartfs_mkdir */
	T_DELETE,					/* for smartfs_rmdir and smartfs_unlink */
	T_SYNC						/* for smartfs_sync_internal */
};

/* This structure is a node in the list of currently
   active (not yet finished) write transactions.
*/
struct active_write_node_s {
	uint8_t trans_info;			/* Transaction type and status */
	uint16_t sector;			/* Sector on which write has to be made */
	uint16_t used_bytes;		/* Number of bytes used in the sector after finishing write */
	uint8_t seq_no;				/* Sequence number of writes on same sector */
	uint16_t journal_sector;	/* Sector in which transaction is written */
	uint16_t journal_offset;	/* Offset in journal_sector where transaction is written */
	struct active_write_node_s *next;	/* Next active write transaction */
};

/* This structure is the logging entry that is written in journal */

struct smartfs_logging_entry_s {
	uint8_t trans_info;			/*      First 4 bits reserved for transaction Type
								 *      Last 4 bits reserved for transaction status.
								 */

	uint8_t seq_no;				/*      Sequence number of writes which need a sync on a sector.
								 *      We are limiting sequence number only to 0 and 1. This is because
								 *      if we receive a new write on a sector which has been written
								 *      before, it means that previous write was complete. So we can finish
								 *      the previous transaction. Thus, there can be at most 2 unfinished
								 *      transactions for a sector in case of power failure.
								 */

	uint8_t crc16[2];			/*      For CRC value to check validation of journal entry
							     *      First 8bits for entry, next 8bits for entry + data
							     */

	uint16_t curr_sector;		/*      Transaction type : Use
								 *        1) T_WRITE  : sector number of sector to be written.
								 *        2) T_CREATE : sector number of parent sector of new entry.
								 *        3) T_RENAME : sector number of parent sector of new entry.
								 *        4) T_MKDIR  : sector number of parent sector where entry has to be created.
								 *        5) T_DELETE  : sector number of parent sector of entry to be deleted.
								 *        6) T_SYNC   : sector number of sector to be synced
								 */

	uint16_t offset;			/*      Transaction type : Use
								 *        1) T_WRITE  : offset in sector where data has to be written.
								 *        2) T_CREATE : Unused
								 *        3) T_RENAME : offset in parent sector of the old directory entry.
								 *        4) T_MKDIR  : Unused.
								 *        5) T_DELETE : offset in parent sector of the old directory entry.
								 *        6) T_SYNC   : Unused
								 */

	uint16_t datalen;			/*     Transaction type : Use
								 *       1) T_WRITE  : Length of data that has to be written.
								 *       2) T_CREATE : Length of filename
								 *       3) T_RENAME : Length of new name of file/directory.
								 *       4) T_MKDIR  : Length of new directory name.
								 *       5) T_DELETE : We are reusing this field to store the sector which is chaining
								 *                     to the sector containing the entry to be deleted
								 *       6) T_SYNC   : Unused
								 */

	uint16_t generic_1;			/*      Generic field. Different meaning in different transaction types.
								 *      Transaction type : Use
								 *        1) T_WRITE  : sector size needed to be updated after write to the sector.
								 *        2) T_CREATE : Mode of file create.
								 *        3) T_RENAME : sector number of parent sector of old entry.
								 *        4) T_MKDIR  : Unused.
								 *        5) T_DELETE : first sector pointed by the entry to delete
								 *        6) T_SYNC   : Sector size that is needed to update.
								 */

};

/*
  This structure provides the transaction manager for all journaling operations
*/
struct journal_transaction_manager_s {
	bool enabled;               /* State value to check journaling enabled or not */
	int8_t jarea;				/* 0 or 1. Specifies which journal area is usable */
	uint16_t sector;			/* Sector number where next logging entry has to be written */
	uint16_t offset;			/* Offset in the sector above */
	uint16_t availbytes;		/* Total space in a logging sector to write entries */
	uint8_t *buffer;			/* buffer to hold logging entry header and data */
	uint8_t *active_sectors;	/* map to mark sectors which are written but not yet synced */
	struct active_write_node_s *list;	/* linked list to hold information about writes which need sync */
};
#endif
/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Internal function prototypes
 ****************************************************************************/

/* Semaphore access for internal use */

void smartfs_semtake(struct smartfs_mountpt_s *fs);
void smartfs_semgive(struct smartfs_mountpt_s *fs);

/* Forward references for utility functions */

struct smartfs_mountpt_s;

/* Utility functions */

int smartfs_mount(struct smartfs_mountpt_s *fs, bool writeable);

int smartfs_unmount(struct smartfs_mountpt_s *fs);

int smartfs_finddirentry(struct smartfs_mountpt_s *fs, struct smartfs_entry_s *direntry, const char *relpath, uint16_t *parentdirsector, const char **filename);

int smartfs_createentry(struct smartfs_mountpt_s *fs, uint16_t parentdirsector, const char *filename, uint16_t type, mode_t mode, struct smartfs_entry_s *direntry, uint16_t sectorno, FAR struct smartfs_ofile_s *sf);

int smartfs_deleteentry(struct smartfs_mountpt_s *fs, struct smartfs_entry_s *entry);

int smartfs_countdirentries(struct smartfs_mountpt_s *fs, struct smartfs_entry_s *entry);

int smartfs_truncatefile(struct smartfs_mountpt_s *fs, struct smartfs_entry_s *entry, FAR struct smartfs_ofile_s *sf);

uint16_t smartfs_rdle16(FAR const void *val);

void smartfs_wrle16(void *dest, uint16_t val);

uint32_t smartfs_rdle32(FAR const void *val);

void smartfs_wrle32(uint8_t *dest, uint32_t val);

#if defined(CONFIG_FS_PROCFS) && !defined(CONFIG_FS_PROCFS_EXCLUDE_SMARTFS)
struct smartfs_mountpt_s *smartfs_get_first_mount(void);
#endif

#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
uint16_t get_leftover_used_byte_count(uint8_t *buffer, uint16_t base_index);
uint16_t get_used_byte_count_from_end(uint8_t *buffer);
int set_used_byte_count(uint8_t *used, uint16_t count);
uint16_t get_used_byte_count(uint8_t *used);
#endif
#ifdef CONFIG_SMARTFS_SECTOR_RECOVERY
int smartfs_recover(struct smartfs_mountpt_s *fs);
int smart_validatesector(FAR struct inode *inode, uint16_t logsector, char *validsectors);
int smart_recoversectors(FAR struct inode *inode, char *validsectors, int *nobsolete, int *nrecovered);

#endif
struct file;					/* Forward references */
struct inode;
struct fs_dirent_s;
struct statfs;
struct stat;

#ifdef CONFIG_SMARTFS_JOURNALING
int smartfs_journal_init(struct smartfs_mountpt_s *fs);
int smartfs_create_journalentry(struct smartfs_mountpt_s *fs, enum logging_transaction_type_e type, uint16_t curr_sector, uint16_t offset, uint16_t datalen, uint16_t genericdata, uint8_t needsync, const uint8_t *data, uint16_t *t_sector, uint16_t *t_offset);
int smartfs_finish_journalentry(struct smartfs_mountpt_s *fs, uint16_t curr_sector, uint16_t sector, uint16_t offset, enum logging_transaction_type_e type);
#endif

#ifdef CONFIG_SMARTFS_SECURE
int smartfs_encrypt(const char *buffer, size_t buflen, unsigned char *output);
int smartfs_decrypt(char *buffer, int ret, char *final_output);
#endif

#endif							/* __FS_SMARTFS_SMARTFS_H */
