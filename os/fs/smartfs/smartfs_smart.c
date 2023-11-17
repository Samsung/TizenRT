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
 * fs/smartfs/smartfs_smart.c
 *
 *   Copyright (C) 2013 Ken Pettit. All rights reserved.
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
#include <sys/stat.h>
#include <sys/statfs.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/dirent.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/mtd.h>
#include <tinyara/fs/smart.h>

#include "smartfs.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int smartfs_open(FAR struct file *filep, const char *relpath, int oflags, mode_t mode);
static int smartfs_close(FAR struct file *filep);
static ssize_t smartfs_read(FAR struct file *filep, char *buffer, size_t buflen);
static ssize_t smartfs_write(FAR struct file *filep, const char *buffer, size_t buflen);
static off_t smartfs_seek(FAR struct file *filep, off_t offset, int whence);
static int smartfs_ioctl(FAR struct file *filep, int cmd, unsigned long arg);

static int smartfs_sync(FAR struct file *filep);
static int smartfs_dup(FAR const struct file *oldp, FAR struct file *newp);
static int smartfs_fstat(FAR const struct file *filep, FAR struct stat *buf);
static int smartfs_truncate(FAR struct file *filep, off_t length);
static int smartfs_opendir(struct inode *mountpt, const char *relpath, struct fs_dirent_s *dir);
static int smartfs_readdir(struct inode *mountpt, struct fs_dirent_s *dir);
static int smartfs_rewinddir(struct inode *mountpt, struct fs_dirent_s *dir);

static int smartfs_bind(FAR struct inode *blkdriver, const void *data, void **handle);
static int smartfs_unbind(void *handle, FAR struct inode **blkdriver);
static int smartfs_statfs(struct inode *mountpt, struct statfs *buf);

static int smartfs_unlink(struct inode *mountpt, const char *relpath);
static int smartfs_mkdir(struct inode *mountpt, const char *relpath, mode_t mode);
static int smartfs_rmdir(struct inode *mountpt, const char *relpath);
static int smartfs_rename(struct inode *mountpt, const char *oldrelpath, const char *newrelpath);
static void smartfs_stat_common(FAR struct smartfs_mountpt_s *fs, FAR struct smartfs_entry_s *entry, FAR struct stat *buf);
static int smartfs_stat(struct inode *mountpt, const char *relpath, struct stat *buf);

/****************************************************************************
 * Private Variables
 ****************************************************************************/

static uint8_t g_seminitialized = FALSE;
static sem_t g_sem;

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/* See fs_mount.c -- this structure is explicitly externed there.
 * We use the old-fashioned kind of initializers so that this will compile
 * with any compiler.
 */

const struct mountpt_operations smartfs_operations = {
	smartfs_open,				/* open */
	smartfs_close,				/* close */
	smartfs_read,				/* read */
	smartfs_write,				/* write */
	smartfs_seek,				/* seek */
	smartfs_ioctl,				/* ioctl */

	smartfs_sync,				/* sync */
	smartfs_dup,				/* dup */
	smartfs_fstat,				/* fstat */
	smartfs_truncate,			/* truncate */

	smartfs_opendir,			/* opendir */
	NULL,						/* closedir */
	smartfs_readdir,			/* readdir */
	smartfs_rewinddir,			/* rewinddir */

	smartfs_bind,				/* bind */
	smartfs_unbind,				/* unbind */
	smartfs_statfs,				/* statfs */

	smartfs_unlink,				/* unlink */
	smartfs_mkdir,				/* mkdir */
	smartfs_rmdir,				/* rmdir */
	smartfs_rename,				/* rename */
	smartfs_stat				/* stat */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: smartfs_open
 ****************************************************************************/

static int smartfs_open(FAR struct file *filep, const char *relpath, int oflags, mode_t mode)
{
	struct inode *inode;
	struct smartfs_mountpt_s *fs;
	int ret;
	struct smartfs_ofile_s *sf;
#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
	struct smart_read_write_s readwrite;
#endif

	/* Sanity checks */

	DEBUGASSERT((filep->f_priv == NULL) && (filep->f_inode != NULL));

	/* Get the mountpoint inode reference from the file structure and the
	 * mountpoint private data from the inode structure
	 */

	inode = filep->f_inode;
	fs = inode->i_private;

	DEBUGASSERT(fs != NULL);

	/* Take the semaphore */

	smartfs_semtake(fs);

	/* Locate the directory entry for this path */

	sf = (struct smartfs_ofile_s *)kmm_zalloc(sizeof(struct smartfs_ofile_s));
	if (sf == NULL) {
		ret = -ENOMEM;
		goto errout_with_semaphore;
	}

	/* Allocate a sector buffer if CRC enabled in the MTD layer */

#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
	sf->buffer = (uint8_t *)kmm_malloc(fs->fs_llformat.availbytes);
	if (sf->buffer == NULL) {
		/* Error ... no memory */

		kmm_free(sf);
		ret = -ENOMEM;
		goto errout_with_semaphore;
	}

	sf->bflags = SMARTFS_BFLAG_UNMOD;
#endif							/* CONFIG_SMARTFS_USE_SECTOR_BUFFER */

	sf->entry.name = NULL;
	ret = smartfs_finddirentry(fs, &sf->entry, relpath);

	/* Three possibilities: (1) a node exists for the relpath and
	 * dirinfo describes the directory entry of the entity, (2) the
	 * node does not exist, or (3) some error occurred.
	 */

	if (ret == OK) {
		/* The name exists -- but is is a file or a directory ? */

		if (sf->entry.flags & SMARTFS_DIRENT_TYPE_DIR) {
			/* Can't open a dir as a file! */

			ret = -EISDIR;
			goto errout_with_buffer;
		}

		/* It would be an error if we are asked to create it exclusively */

		if ((oflags & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL)) {
			/* Already exists -- can't create it exclusively */

			ret = -EEXIST;
			goto errout_with_buffer;
		}

		/* If the file is being opened in a mode other than "READ ONLY", we will need the length of the file */
		if ((oflags & O_ACCMODE) != O_RDONLY) {
			ret = smartfs_get_datalen(fs, sf->entry.firstsector, &sf->entry.datalen);
			if (ret < 0) {
				fdbg("ERROR, Could not get the length of the file, ret : %d\n", ret);
				goto errout_with_buffer;
			}
		}

		/* The file exists already.
		 * If the file has been requested to be opened in TRUNCATE mode, then shrink the file.
		 * However, if APPEND mode has also been requested, do not shrink the file to 0.
		 */

		/* TODO: This method for checking the open modes is not in full accordance with POSIX standard.
		 *       POSIX standard truncates the file i.e. shrinks to 0 irrespective of other included flags.
		 */

		if (((oflags & O_TRUNC) != 0) && ((oflags & O_APPEND) == 0)) {
			/* Truncate the file to length 0 as part of the open */
			ret = smartfs_shrinkfile(fs, sf, 0);
			if (ret < 0) {
				goto errout_with_buffer;
			}
		}
	} else if (ret == -ENOENT) {
		/* The file does not exist.  Were we asked to create it? */
		if ((oflags & O_CREAT) == 0) {
			/* No.. then we fail with -ENOENT */

			ret = -ENOENT;
			goto errout_with_buffer;
		}

		/* Yes... test if the parent directory is valid */
		if (sf->entry.dsector != 0xFFFF) {
			/* We can create in the given parent directory */
			/* First we allocate a new data sector for the file */
			ret = smartfs_alloc_firstsector(fs, &sf->entry.firstsector, SMARTFS_DIRENT_TYPE_FILE, sf);
			if (ret != OK) {
				fdbg("Unable to allocate data sector for new entry, ret : %d\n", ret);
				goto errout_with_buffer;
			}

			/* First try to find an invalid or empty entry available in one of the chained parent sectors */
			ret = smartfs_find_availableentry(fs, &sf->entry);
			if (ret != OK) {
				/* find_availableentry encountered a problem with lower layer operations, return error */
				fdbg("Unable to find space for writing new entry, ret : %d\n", ret);
				goto errout_with_buffer;
			}

			sf->entry.flags = SMARTFS_ERASEDSTATE_16BIT;
#ifdef CONFIG_SMARTFS_ALIGNED_ACCESS
			smartfs_wrle16(&sf->entry.flags, (uint16_t)(mode & SMARTFS_DIRENT_MODE));
#else
			sf->entry.flags = (uint16_t)(mode & SMARTFS_DIRENT_MODE);
#endif
			/* At this point, either an available entry was found or a new one has been created */
			ret = smartfs_writeentry(fs, sf->entry, SMARTFS_DIRENT_TYPE_FILE, mode);
			if (ret != OK) {
				fdbg("Unable to write entry, ret : %d\n", ret);
				goto errout_with_buffer;
			}
		} else {
			/* Trying to create in a directory that doesn't exist */

			ret = -ENOENT;
			goto errout_with_buffer;
		}
	} else {
		goto errout_with_buffer;
	}

	/* Now perform the "open" on the file in direntry */

	sf->oflags = oflags;
	sf->crefs = 1;
	sf->filepos = 0;
	sf->curroffset = sizeof(struct smartfs_chain_header_s);
	sf->currsector = sf->entry.firstsector;
	sf->byteswritten = 0;

#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
	if ((sf->bflags & SMARTFS_BFLAG_DIRTY) == 0) {
		/* When using sector buffering, current sector with its header should
		 * always be present in sf->buffer. Otherwise data corruption may arise
		 * when writing.
		 */

		if (sf->currsector != SMARTFS_ERASEDSTATE_16BIT) {
			/* For truncate, we already read header and set data in sf->buffer */
			if (((oflags & (O_CREAT | O_TRUNC)) == 0) || (oflags & O_APPEND)) {
				smartfs_setbuffer(&readwrite, sf->currsector, 0, fs->fs_llformat.availbytes, (uint8_t *)sf->buffer);
				ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long) &readwrite);
				if (ret < 0) {
					fdbg("Error reading sector %d header, ret : %d\n", readwrite.logsector, ret);
					goto errout_with_buffer;
				}
			}
		}
	}
#endif

	/* Test if we opened for APPEND mode.  If we did, then seek to the
	 * end of the file.
	 */

	if (oflags & O_APPEND) {
		/* Perform the seek */

		smartfs_seek_internal(fs, sf, 0, SEEK_END);
	}

	/* Attach the private date to the struct file instance */

	filep->f_priv = sf;

	/* Then insert the new instance into the mountpoint structure.
	 * It needs to be there (1) to handle error conditions that effect
	 * all files, and (2) to inform the umount logic that we are busy
	 * (but a simple reference count could have done that).
	 */

	sf->fnext = fs->fs_head;
	fs->fs_head = sf;

	ret = OK;
	goto errout_with_semaphore;

errout_with_buffer:
#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
	if (sf->buffer != NULL) {
		kmm_free(sf->buffer);
		sf->buffer = NULL;
	}
#endif
	if (sf->entry.name != NULL) {
		/* Free the space for the name too */

		kmm_free(sf->entry.name);
		sf->entry.name = NULL;
	}

	kmm_free(sf);

errout_with_semaphore:
	smartfs_semgive(fs);
	if (ret == -EINVAL) {
		ret = -EIO;
	}

	return ret;
}

/****************************************************************************
 * Name: smartfs_close
 ****************************************************************************/

static int smartfs_close(FAR struct file *filep)
{
	struct inode *inode;
	struct smartfs_mountpt_s *fs;
	struct smartfs_ofile_s *sf;
	struct smartfs_ofile_s *nextfile;
	struct smartfs_ofile_s *prevfile;

	/* Sanity checks */

	DEBUGASSERT(filep->f_priv != NULL && filep->f_inode != NULL);

	/* Recover our private data from the struct file instance */

	inode = filep->f_inode;
	fs = inode->i_private;
	sf = filep->f_priv;

	/* Sync the file */

	smartfs_sync(filep);

	/* Take the semaphore */

	smartfs_semtake(fs);

	/* Check if we are the last one with a reference to the file and
	 * only close if we are. */

	if (sf->crefs > 1) {
		/* The file is opened more than once.  Just decrement the
		 * reference count and return. */

		sf->crefs--;
		goto okout;
	}

	/* Remove ourselves from the linked list */

	nextfile = fs->fs_head;
	prevfile = nextfile;
	while ((nextfile != sf) && (nextfile != NULL)) {
		/* Save the previous file pointer too */

		prevfile = nextfile;
		nextfile = nextfile->fnext;
	}

	if (nextfile != NULL) {
		/* Test if we were the first entry */

		if (nextfile == fs->fs_head) {
			/* Assign a new head */

			fs->fs_head = nextfile->fnext;
		} else {
			/* Take ourselves out of the list */

			prevfile->fnext = nextfile->fnext;
		}
	}

	/* Now free the pointer */

	filep->f_priv = NULL;;
	if (sf->entry.name != NULL) {
		/* Free the space for the name too */

		kmm_free(sf->entry.name);
		sf->entry.name = NULL;
	}
#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
	if (sf->buffer) {
		kmm_free(sf->buffer);
	}
#endif

	kmm_free(sf);
	filep->f_priv = NULL;

okout:
	smartfs_semgive(fs);
	return OK;
}

/****************************************************************************
 * Name: smartfs_read
 ****************************************************************************/

static ssize_t smartfs_read(FAR struct file *filep, char *buffer, size_t buflen)
{
	struct inode *inode;
	struct smartfs_mountpt_s *fs;
	struct smartfs_ofile_s *sf;
	struct smart_read_write_s readwrite;
	struct smartfs_chain_header_s *header;
	int ret = OK;
	uint32_t bytesread;
	uint16_t bytestoread;
	uint16_t bytesinsector;

	/* Sanity checks */

	DEBUGASSERT(filep->f_priv != NULL && filep->f_inode != NULL);

	/* Recover our private data from the struct file instance */

	sf = filep->f_priv;
	inode = filep->f_inode;
	fs = inode->i_private;

	DEBUGASSERT(fs != NULL);

	/* Take the semaphore */

	smartfs_semtake(fs);

	/* Loop until all byte read or error */

	bytesread = 0;
	while (bytesread != buflen) {
		/* Test if we are at the end of data */

		if (sf->currsector == SMARTFS_ERASEDSTATE_16BIT) {
			/* Break and return the number of bytes we read (may be zero) */

			break;
		}

		/* Read the curent sector into our buffer */

		smartfs_setbuffer(&readwrite, sf->currsector, 0, fs->fs_llformat.availbytes, (uint8_t *)fs->fs_rwbuffer);
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
		if (ret < 0) {
			fdbg("Error reading sector %d data, ret : %d\n", readwrite.logsector, ret);
			goto errout_with_semaphore;
		}

		/* Point header to the read data to get used byte count */

		header = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;

		/* Get number of used bytes in this sector */
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
		bytesinsector = get_leftover_used_byte_count((uint8_t *)readwrite.buffer, get_used_byte_count((uint8_t *)header->used));
#else
		bytesinsector = SMARTFS_USED(header);

		if (bytesinsector == SMARTFS_ERASEDSTATE_16BIT) {
			/* No bytes to read from this sector */

			bytesinsector = 0;
		}
#endif
		/* Calculate the number of bytes to read into the buffer */

		bytestoread = bytesinsector - (sf->curroffset - (uint16_t)sizeof(struct smartfs_chain_header_s));
		if (bytestoread + bytesread > buflen) {
			/* Truncate bytesto read based on buffer len */

			bytestoread = buflen - bytesread;
		}

		/* Copy data to the read buffer */

		if (bytestoread > 0) {
			/* Do incremental copy from this sector */

			memcpy(&buffer[bytesread], &fs->fs_rwbuffer[sf->curroffset], bytestoread);
			bytesread += bytestoread;
			sf->filepos += bytestoread;
			sf->curroffset += bytestoread;
		}

		/* Test if we are at the end of the data in this sector */

		if ((bytestoread == 0) || (sf->curroffset == fs->fs_llformat.availbytes)) {
			/* Set the next sector as the current sector */

			sf->currsector = SMARTFS_NEXTSECTOR(header);
			sf->curroffset = sizeof(struct smartfs_chain_header_s);
		}
	}

	/* Return the number of bytes we read */

	ret = bytesread;

errout_with_semaphore:
	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_write
 ****************************************************************************/

static ssize_t smartfs_write(FAR struct file *filep, const char *buffer, size_t buflen)
{
	struct inode *inode;
	struct smartfs_mountpt_s *fs;
	struct smartfs_ofile_s *sf;
	struct smart_read_write_s readwrite;
	struct smartfs_chain_header_s *header;
	size_t byteswritten;
	uint16_t bytes;
	size_t size = sizeof(struct smartfs_chain_header_s);
	int ret;

	/* Sanity checks.  I have seen the following assertion misfire if
	 * CONFIG_DEBUG_MM is enabled while re-directing output to a
	 * file.  In this case, the debug output can get generated while
	 * the file is being opened,  FAT data structures are being allocated,
	 * and things are generally in a perverse state.
	 */

#ifdef CONFIG_DEBUG_MM
	if (filep->f_priv == NULL || filep->f_inode == NULL) {
		return -ENXIO;
	}
#else
	DEBUGASSERT(filep->f_priv != NULL && filep->f_inode != NULL);
#endif

	/* Recover our private data from the struct file instance */

	sf = filep->f_priv;
	inode = filep->f_inode;
	fs = inode->i_private;

	DEBUGASSERT(fs != NULL);

	/* Take the semaphore */

	smartfs_semtake(fs);

	/* Test the permissions. Only allow write if the file was opened with write flags */

	if ((sf->oflags & O_WROK) == 0) {
		ret = -EACCES;
		goto errout_with_semaphore;
	}

	header = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;
	byteswritten = 0;

	/* First, check if we are overwriting the existing file data or appending more
	 * As long as the current file pointer position for writing is less than
	 * the total data length in the file, we are doing an overwrite operation.
	 * When the file pointer's position surpasses the file's data length,
	 * it becomes an append operation which will be handled by a separate function
	 */
	while ((sf->filepos < sf->entry.datalen) && (buflen > 0)) {
		/* We will write data up to the current end-of-file and then break,
		 * allowing the append operation to write the additional data to the end of the file.
		 */

		fvdbg("Datalen : %d, sector : %d, offset : %d\nFilepos : %d, bytes written : %d, buflen : %d\n",\
			 sf->entry.datalen, sf->currsector, sf->curroffset, sf->filepos, byteswritten, buflen);
		bytes = fs->fs_llformat.availbytes - sf->curroffset;

		/* Limit the write based on available data to write */
		if (bytes > buflen) {
			bytes = buflen;
		}

		/* Limit the write based on current file length */
		if (bytes > sf->entry.datalen - sf->filepos) {
			bytes = sf->entry.datalen - sf->filepos;
		}

		/* Now perform the write. */
		if (bytes > 0) {

#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
			/* If sector buffer is enabled and it is full with data, overwrite right now.
			 * Otherwise, it will be written by sync_internal
			 */
			memcpy(&sf->buffer[sf->curroffset], &buffer[byteswritten], bytes);
			sf->bflags |= SMARTFS_BFLAG_DIRTY;

			if (sf->curroffset + bytes == fs->fs_llformat.availbytes)
#endif				
			{
#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
				smartfs_setbuffer(&readwrite, sf->currsector, size,\
					 fs->fs_llformat.availbytes - size, (uint8_t *)&sf->buffer[size]);
#else
				smartfs_setbuffer(&readwrite, sf->currsector, sf->curroffset,\
					 fs->fs_llformat.availbytes - sf->curroffset, (uint8_t *)&buffer[byteswritten]);
#endif
				/* Only the data bytes of the sector are written,
				 * we do not re-write/overwrite the header bytes.
				 */
				ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&readwrite);
				if (ret < 0) {
					fdbg("Error writing sector %d data, ret : %d\n", readwrite.logsector, ret);
					goto errout_with_semaphore;
				}
#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
				/* Clear the flags, no longer dirty if the buffer content is written to flash */
				sf->bflags = SMARTFS_BFLAG_UNMOD;
#endif
			}

			/* Update our control variables */
			sf->filepos += bytes;
			sf->curroffset += bytes;
			buflen -= bytes;
			byteswritten += bytes;
		}

		/* Test if we wrote to the end of the current sector */
		if (sf->curroffset == fs->fs_llformat.availbytes) {
			/* Wrote to the end of the sector. Update to point to the
			 * next sector for additional writes. Read header to get the sector chain info.
			 */
			smartfs_setbuffer(&readwrite, sf->currsector, 0, size, (uint8_t *)fs->fs_rwbuffer);
			ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
			if (ret < 0) {
				fdbg("Error reading sector %d header, ret : %d\n", readwrite.logsector, ret);
				goto errout_with_semaphore;
			}

			/* If file is modified and more data remains to be appended to file,
			 * but no next sector is available, do not update sf->currsector and curroffset.
			 * This will be handled when buffer data is synced.
			 */
			if (SMARTFS_NEXTSECTOR(header) != 0xFFFF) {
				/* Now get the chained sector info and reset the offset */
				sf->currsector = SMARTFS_NEXTSECTOR(header);
				sf->curroffset = size;

#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
				/* Read the next sector's data if we have reached the end of the current sector
				 * AND more data is left to be written.
				 * The header of the current sector will be read later if required.
				 */
				if (buflen > 0) {
					smartfs_setbuffer(&readwrite, sf->currsector, size,\
						 fs->fs_llformat.availbytes - size, (uint8_t *)&sf->buffer[size]);
					ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
					if (ret < 0) {
						fdbg("Error %d reading sector %d\n", ret, sf->currsector);
						goto errout_with_semaphore;
					}
				}
#endif
			}
		}
	}

#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
	/* If there is unsynced data in the sf->buffer and the control is now leaving the
	 * scope of this function, the correct header should also be present in sf->buffer
	 * for smartfs_append_data or smartfs_sync_internal. Hence we read the header now.
	 */
	if (sf->bflags & SMARTFS_BFLAG_DIRTY && byteswritten > 0) {
		smartfs_setbuffer(&readwrite, sf->currsector, 0, size, (uint8_t*)sf->buffer);
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
		if (ret < 0) {
			fdbg("Error %d reading sector %d header\n", ret, sf->currsector);
			goto errout_with_semaphore;
		}
	}
#endif
	/* Now append any remaining data to end of the file. */
	if (buflen > 0) {
		byteswritten = smartfs_append_data(fs, sf, buffer, byteswritten, buflen);
	}
	ret = byteswritten;

errout_with_semaphore:
	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_seek
 ****************************************************************************/

static off_t smartfs_seek(FAR struct file *filep, off_t offset, int whence)
{
	struct inode *inode;
	struct smartfs_mountpt_s *fs;
	struct smartfs_ofile_s *sf;
	int ret;

	/* Sanity checks */

	DEBUGASSERT(filep->f_priv != NULL && filep->f_inode != NULL);

	/* Recover our private data from the struct file instance */

	sf = filep->f_priv;
	inode = filep->f_inode;
	fs = inode->i_private;

	DEBUGASSERT(fs != NULL);

	/* Take the semaphore */

	smartfs_semtake(fs);

	/* Call our internal routine to perform the seek */

	ret = smartfs_seek_internal(fs, sf, offset, whence);

	if (ret >= 0) {
		filep->f_pos = ret;
	}

	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_ioctl
 ****************************************************************************/

static int smartfs_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	/* We don't use any ioctls */

	return -ENOSYS;
}

/****************************************************************************
 * Name: smartfs_sync
 *
 * Description: Synchronize the file state on disk to match internal, in-
 *   memory state.
 *
 ****************************************************************************/

static int smartfs_sync(FAR struct file *filep)
{
	struct inode *inode;
	struct smartfs_mountpt_s *fs;
	struct smartfs_ofile_s *sf;
	int ret;

	/* Sanity checks */

	DEBUGASSERT(filep->f_priv != NULL && filep->f_inode != NULL);

	/* Recover our private data from the struct file instance */

	sf = filep->f_priv;
	inode = filep->f_inode;
	fs = inode->i_private;

	DEBUGASSERT(fs != NULL);

	/* Take the semaphore */

	smartfs_semtake(fs);

	ret = smartfs_sync_internal(fs, sf);

	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smart_dup
 *
 * Description: Duplicate open file data in the new file structure.
 *
 ****************************************************************************/

static int smartfs_dup(FAR const struct file *oldp, FAR struct file *newp)
{
	struct smartfs_ofile_s *sf;

	fvdbg("Dup %p->%p\n", oldp, newp);

	/* Sanity checks */

	DEBUGASSERT(oldp->f_priv != NULL && newp->f_priv == NULL && newp->f_inode != NULL);

	/* Recover our private data from the struct file instance */

	sf = oldp->f_priv;

	DEBUGASSERT(sf != NULL);

	/* Just increment the reference count on the ofile */

	sf->crefs++;
	newp->f_priv = (FAR void *)sf;

	return OK;
}

/****************************************************************************
 * Name: smartfs_fstat
 *
 * Description:
 *   Obtain information about an open file associated with the file
 *   descriptor 'fd', and will write it to the area pointed to by 'buf'.
 *
 ****************************************************************************/

static int smartfs_fstat(FAR const struct file *filep, FAR struct stat *buf)
{
	FAR struct inode *inode;
	FAR struct smartfs_mountpt_s *fs;
	FAR struct smartfs_ofile_s *sf;

	DEBUGASSERT(filep != NULL);
	DEBUGASSERT(filep->f_priv != NULL && filep->f_inode != NULL);
	DEBUGASSERT(buf != NULL);

	/* Recover our private data from the struct file instance */
	sf = filep->f_priv;
	inode = filep->f_inode;

	fs = inode->i_private;

	/* Take the semaphore */
	smartfs_semtake(fs);

	/* Return information about the directory entry in the stat structure */
	smartfs_stat_common(fs, &sf->entry, buf);
	smartfs_semgive(fs);
	return OK;
}

/****************************************************************************
 * Name: smartfs_truncate
 *
 * Description:
 *   Set the length of the open, regular file associated with the file
 *   structure 'filep' to 'length'.
 *
 * NOTE:- This function deviates slightly from the POSIX standard as implemented in Linux.
 *        The file position pointer in Linux is not reset to the new data length. If a write
 *        operation is requested after truncate, new data is written starting at the old file
 *        position pointer. The extra space in between gets filled up with '0's.
 *        Our smartfs_shrinkfile function will reset the file pointer to the position of 'length'
 *        as requested in the truncate operation. New data is written starting at this position.
 *        Smartfs_extendfile will simply restore the file position pointer to its old location.
 *        Any conflicts/issues arising due to this deviation will be taken care of.
 *
 ****************************************************************************/

static int smartfs_truncate(FAR struct file *filep, off_t length)
{

	FAR struct inode *inode;
	FAR struct smartfs_mountpt_s *fs;
	FAR struct smartfs_ofile_s *sf;
	off_t oldsize;
	int ret;
	uint16_t oldfilepos = 0;

	DEBUGASSERT(filep->f_priv != NULL && filep->f_inode != NULL);

	/* Recover our private data from the struct file instance */

	sf    = filep->f_priv;
	inode = filep->f_inode;
	fs    = inode->i_private;

	DEBUGASSERT(fs != NULL);

	/* Take the semaphore */

	smartfs_semtake(fs);

	/* Test the permissions.  Only allow truncation if the file was opened with
	 * write flags.
	 */

	if ((sf->oflags & O_WROK) == 0) {
		ret = -EACCES;
		goto errout_with_semaphore;
	}

	/* Save old file position here */
	oldfilepos = sf->filepos;

	/* Are we shrinking the file?  Or extending it? */
	/* Seek till point 'length' of the file, file pointer lies at position of requested 'length' now */
	oldsize = sf->entry.datalen;
	if (oldsize == length) {
		/* Let's not and say we did */
		ret = OK;
	} else if (oldsize > length) {
		/* We are shrinking the file */
		ret = smartfs_shrinkfile(fs, sf, length);
		if (ret != OK) {
			goto errout_with_semaphore;
		}
	} else {
		/* Otherwise we are extending the file.  This is essentially the same
		 * as a write except that (1) we write NULL characters and (2) we don't
		 * update the file position.
		 */
		ret = smartfs_extendfile(fs, sf, length);
		if (ret != OK) {
			goto errout_with_semaphore;
		}
	}

	ret = OK;
errout_with_semaphore:
	/* TODO As we commented, this is violdation of POSIX, we'll see it..but fail case? */
	if (sf->entry.datalen < oldfilepos) {
		smartfs_seek_internal(fs, sf, 0, SEEK_END);
	} else {
		smartfs_seek_internal(fs, sf, oldfilepos, SEEK_SET);
	}

	/* Relinquish exclusive access */
	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_opendir
 *
 * Description: Open a directory for read access
 *
 ****************************************************************************/

static int smartfs_opendir(struct inode *mountpt, const char *relpath, struct fs_dirent_s *dir)
{
	struct smartfs_mountpt_s *fs;
	int ret;
	struct smartfs_entry_s entry;

	/* Sanity checks */

	DEBUGASSERT(mountpt != NULL && mountpt->i_private != NULL);

	/* Recover our private data from the inode instance */

	fs = mountpt->i_private;

	/* Take the semaphore */

	smartfs_semtake(fs);

	/* Search for the path on the volume */

	entry.name = NULL;
	ret = smartfs_finddirentry(fs, &entry, relpath);
	if (ret < 0) {
		goto errout_with_semaphore;
	}

	/* Populate our private data in the fs_dirent_s struct */

	dir->u.smartfs.fs_firstsector = entry.firstsector;
	dir->u.smartfs.fs_currsector = entry.firstsector;
	dir->u.smartfs.fs_curroffset = sizeof(struct smartfs_chain_header_s);

	ret = OK;

errout_with_semaphore:
	/* If space for the entry name was allocated, then free it */

	if (entry.name != NULL) {
		kmm_free(entry.name);
		entry.name = NULL;
	}

	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_readdir
 *
 * Description: Read the next directory entry
 *
 ****************************************************************************/

static int smartfs_readdir(struct inode *mountpt, struct fs_dirent_s *dir)
{
	struct smartfs_mountpt_s *fs;
	int ret;
	uint16_t entrysize;
	uint16_t namelen;
	struct smartfs_chain_header_s *header;
	struct smart_read_write_s readwrite;
	struct smartfs_entry_header_s *entry;

	/* Sanity checks */

	DEBUGASSERT(mountpt != NULL && mountpt->i_private != NULL);

	/* Recover our private data from the inode instance */

	fs = mountpt->i_private;

	/* Take the semaphore */

	smartfs_semtake(fs);

	/* Read sectors and search entries until one found or no more */

	entrysize = sizeof(struct smartfs_entry_header_s) + fs->fs_llformat.namesize;
	while (dir->u.smartfs.fs_currsector != SMARTFS_ERASEDSTATE_16BIT) {
		/* Read the logical sector */

		smartfs_setbuffer(&readwrite, dir->u.smartfs.fs_currsector, 0, fs->fs_llformat.availbytes, (uint8_t *)fs->fs_rwbuffer);
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
		if (ret < 0) {
			goto errout_with_semaphore;
		}

		/* Now search for entries, starting at curroffset */

		while (dir->u.smartfs.fs_curroffset < ret) {
			/* Point to next entry */

			entry = (struct smartfs_entry_header_s *)&fs->fs_rwbuffer[dir->u.smartfs.fs_curroffset];

			/* Test if this entry is valid and active */

			if (((entry->flags & SMARTFS_DIRENT_EMPTY) == (SMARTFS_ERASEDSTATE_16BIT & SMARTFS_DIRENT_EMPTY)) || ((entry->flags & SMARTFS_DIRENT_ACTIVE) != (SMARTFS_ERASEDSTATE_16BIT & SMARTFS_DIRENT_ACTIVE))) {
				/* This entry isn't valid, skip it */

				dir->u.smartfs.fs_curroffset += entrysize;
				continue;
			}

			/* Entry found!  Report it */

			if ((entry->flags & SMARTFS_DIRENT_TYPE) == SMARTFS_DIRENT_TYPE_DIR) {
				dir->fd_dir.d_type = DTYPE_DIRECTORY;
			} else {
				dir->fd_dir.d_type = DTYPE_FILE;
			}

			/* Copy the entry name to dirent */

			namelen = fs->fs_llformat.namesize;
			if (namelen > NAME_MAX) {
				namelen = NAME_MAX;
			}

			memset(dir->fd_dir.d_name, 0, namelen);
			strncpy(dir->fd_dir.d_name, entry->name, namelen);

			/* Now advance to the next entry */

			dir->u.smartfs.fs_curroffset += entrysize;
			if (dir->u.smartfs.fs_curroffset + entrysize >= fs->fs_llformat.availbytes) {
				/* We advanced past the end of the sector.  Go to next sector */

				dir->u.smartfs.fs_curroffset = sizeof(struct smartfs_chain_header_s);
				header = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;
				dir->u.smartfs.fs_currsector = SMARTFS_NEXTSECTOR(header);
			}

			/* Now exit */

			ret = OK;
			goto errout_with_semaphore;
		}

		/* No more entries in this sector.  Move on to next sector and
		 * continue the search.  If no more sectors, then we are all
		 * done and will report ENOENT.
		 */

		header = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;
		dir->u.smartfs.fs_curroffset = sizeof(struct smartfs_chain_header_s);
		dir->u.smartfs.fs_currsector = SMARTFS_NEXTSECTOR(header);
	}

	/* If we arrive here, then there are no more entries */

	ret = -ENOENT;

errout_with_semaphore:
	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_rewindir
 *
 * Description: Reset directory read to the first entry
 *
 ****************************************************************************/

static int smartfs_rewinddir(struct inode *mountpt, struct fs_dirent_s *dir)
{
	int ret = OK;

	/* Sanity checks */

	DEBUGASSERT(mountpt != NULL && mountpt->i_private != NULL);

	/* Reset the directory to the first entry */

	dir->u.smartfs.fs_currsector = dir->u.smartfs.fs_firstsector;
	dir->u.smartfs.fs_curroffset = sizeof(struct smartfs_chain_header_s);

	return ret;
}

/****************************************************************************
 * Name: smartfs_bind
 *
 * Description: This implements a portion of the mount operation. This
 *  function allocates and initializes the mountpoint private data and
 *  binds the blockdriver inode to the filesystem private data.  The final
 *  binding of the private data (containing the blockdriver) to the
 *  mountpoint is performed by mount().
 *
 ****************************************************************************/

static int smartfs_bind(FAR struct inode *blkdriver, const void *data, void **handle)
{
	struct smartfs_mountpt_s *fs;
	int ret;

	/* Open the block driver */

	if (!blkdriver || !blkdriver->u.i_bops) {
		return -ENODEV;
	}

	if (blkdriver->u.i_bops->open && blkdriver->u.i_bops->open(blkdriver) != OK) {
		return -ENODEV;
	}

	/* Create an instance of the mountpt state structure */

	fs = (struct smartfs_mountpt_s *)kmm_zalloc(sizeof(struct smartfs_mountpt_s));
	if (!fs) {
		return -ENOMEM;
	}

	/* If the global semaphore hasn't been initialized, then
	 * initialize it now. */

	fs->fs_sem = &g_sem;
	if (!g_seminitialized) {
		sem_init(&g_sem, 0, 0);	/* Initialize the semaphore that controls access */
		g_seminitialized = TRUE;
	} else {
		/* Take the semaphore for the mount */

		smartfs_semtake(fs);
	}

	/* Initialize the allocated mountpt state structure.  The filesystem is
	 * responsible for one reference on the blkdriver inode and does not
	 * have to addref() here (but does have to release in ubind().
	 */

	fs->fs_blkdriver = blkdriver;	/* Save the block driver reference */
	fs->fs_head = NULL;

	/* Now perform the mount.  */

	ret = smartfs_mount(fs, true);
	if (ret != 0) {
		goto error_with_semaphore;
	}

	*handle = (void *)fs;

#ifndef NXFUSE_HOST_BUILD
	ret = smartfs_sector_recovery(fs);
	if (ret != 0) {
		goto error_with_semaphore;
	}
#endif

	smartfs_semgive(fs);
	return ret;

error_with_semaphore:
	smartfs_semgive(fs);
	kmm_free(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_unbind
 *
 * Description: This implements the filesystem portion of the umount
 *   operation.
 *
 ****************************************************************************/

static int smartfs_unbind(void *handle, FAR struct inode **blkdriver)
{
	struct smartfs_mountpt_s *fs = (struct smartfs_mountpt_s *)handle;
	int ret;

	if (!fs) {
		return -EINVAL;
	}

	/* Check if there are sill any files opened on the filesystem. */

	ret = OK;					/* Assume success */
	smartfs_semtake(fs);
	if (fs->fs_head != NULL) {
		/* We cannot unmount now.. there are open files */

		smartfs_semgive(fs);
		return -EBUSY;
	}
	/* Unmount ... close the block driver */
	ret = smartfs_unmount(fs);
	smartfs_semgive(fs);
	kmm_free(fs);

	return ret;
}

/****************************************************************************
 * Name: smartfs_statfs
 *
 * Description: Return filesystem statistics
 *
 ****************************************************************************/

static int smartfs_statfs(struct inode *mountpt, struct statfs *buf)
{
	struct smartfs_mountpt_s *fs;
	int ret;

	/* Sanity checks */

	DEBUGASSERT(mountpt && mountpt->i_private);

	/* Get the mountpoint private data from the inode structure */

	fs = mountpt->i_private;

	smartfs_semtake(fs);

	/* Implement the logic!! */

	memset(buf, 0, sizeof(struct statfs));
	buf->f_type = SMARTFS_MAGIC;

	/* Re-request the low-level format info to update free blocks */

	ret = FS_IOCTL(fs, BIOC_GETFORMAT, (unsigned long)&fs->fs_llformat);

	buf->f_namelen = fs->fs_llformat.namesize;
	buf->f_bsize = fs->fs_llformat.sectorsize;
	buf->f_blocks = fs->fs_llformat.nsectors;
	if (buf->f_blocks == 65535) {
		buf->f_blocks++;
	}

	buf->f_bfree = fs->fs_llformat.nfreesectors;
	buf->f_bavail = fs->fs_llformat.nfreesectors;
	buf->f_files = 0;
	buf->f_ffree = fs->fs_llformat.nfreesectors;

	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_unlink
 *
 * Description: Remove a file
 *
 ****************************************************************************/

static int smartfs_unlink(struct inode *mountpt, const char *relpath)
{
	struct smartfs_mountpt_s *fs;
	int ret;
	struct smartfs_entry_s entry;

	/* Sanity checks */

	DEBUGASSERT(mountpt && mountpt->i_private);

	/* Get the mountpoint private data from the inode structure */

	fs = mountpt->i_private;

	smartfs_semtake(fs);

	/* Locate the directory entry for this path */

	entry.name = NULL;
	ret = smartfs_finddirentry(fs, &entry, relpath);
	if (ret == OK) {
		/* The name exists -- validate it is a file, not a dir */

		if ((entry.flags & SMARTFS_DIRENT_TYPE) == SMARTFS_DIRENT_TYPE_DIR) {
			ret = -EISDIR;
			goto errout_with_semaphore;
		}

		/* TODO:  Need to check permissions?  */

		/* Okay, we are clear to delete the file.  Use the deleteentry routine. */

		smartfs_deleteentry(fs, &entry);

	} else {
		/* Just report the error */

		goto errout_with_semaphore;
	}

	ret = OK;

errout_with_semaphore:
	if (entry.name != NULL) {
		kmm_free(entry.name);
	}
	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_mkdir
 *
 * Description: Create a directory
 *
 ****************************************************************************/

static int smartfs_mkdir(struct inode *mountpt, const char *relpath, mode_t mode)
{
	struct smartfs_mountpt_s *fs;
	int ret;
	struct smartfs_entry_s entry;

	/* Sanity checks */

	DEBUGASSERT(mountpt && mountpt->i_private);

	/* Get the mountpoint private data from the inode structure */

	fs = mountpt->i_private;

	smartfs_semtake(fs);

	/* Locate the directory entry for this path */

	entry.name = NULL;
	ret = smartfs_finddirentry(fs, &entry, relpath);
	/* Three possibililities: (1) a node exists for the relpath and
	 * dirinfo describes the directory entry of the entity, (2) the
	 * node does not exist, or (3) some error occurred.
	 */

	if (ret == OK) {
		/* The name exists -- can't create */

		ret = -EEXIST;
		goto errout_with_semaphore;
	} else if (ret == -ENOENT) {

		/* It doesn't exist ... we can create it, but only if we have
		 * the right permissions and if the previous parent is valid. */

		if (entry.dsector == 0xFFFF) {
			fdbg("Invalid parent directory sector\n");
			/* Invalid entry in the path (non-existant dir segment) */
			goto errout_with_semaphore;
		}
		/* Create the directory */
		/* Allocate new data sector for entry */
		ret = smartfs_alloc_firstsector(fs, &entry.firstsector, SMARTFS_DIRENT_TYPE_DIR, NULL);
		if (ret != OK) {
			fdbg("Unable to allocate data sector for new entry, ret : %d\n", ret);
			goto errout_with_semaphore;
		}
		/* Try to find empty/invalid entry available in one of he chained parent sectors */
		ret = smartfs_find_availableentry(fs, &entry);
		if (ret != OK) {
			fdbg("Unable to find space for writing new entry, ret : %d\n", ret);
			goto errout_with_semaphore;
		}
		/* Now we have an entry allocated for writing, write new entry to sector */
		ret = smartfs_writeentry(fs, entry, SMARTFS_DIRENT_TYPE_DIR, mode);
		if (ret != OK) {
			fdbg("Unable to write new entry, ret : %d\n", ret);
			goto errout_with_semaphore;
		}
	}

errout_with_semaphore:
	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_rmdir
 *
 * Description: Remove a directory
 *
 ****************************************************************************/

int smartfs_rmdir(struct inode *mountpt, const char *relpath)
{
	struct smartfs_mountpt_s *fs;
	int ret;
	struct smartfs_entry_s entry;

	/* Sanity checks */

	DEBUGASSERT(mountpt && mountpt->i_private);

	/* Get the mountpoint private data from the inode structure */

	fs = mountpt->i_private;

	/* Take the semaphore */

	smartfs_semtake(fs);

	/* Locate the directory entry for this path */

	entry.name = NULL;
	ret = smartfs_finddirentry(fs, &entry, relpath);
	if (ret == OK) {
		/* The name exists -- validate it is a dir, not a file */

		if ((entry.flags & SMARTFS_DIRENT_TYPE) == SMARTFS_DIRENT_TYPE_FILE) {
			ret = -ENOTDIR;
			goto errout_with_semaphore;
		}

		/* TODO:  Need to check permissions?  */

		/* Check if the directory is emtpy */

		ret = smartfs_countdirentries(fs, &entry);
		if (ret < 0) {
			goto errout_with_semaphore;
		}

		/* Only continue if there are zero entries in the directory */

		if (ret != 0) {
			ret = -ENOTEMPTY;
			goto errout_with_semaphore;
		}

		/* Okay, we are clear to delete the directory.  Use the deleteentry routine. */

		ret = smartfs_deleteentry(fs, &entry);
		if (ret < 0) {
			goto errout_with_semaphore;
		}
	} else {
		/* Just report the error */

		goto errout_with_semaphore;
	}

	ret = OK;

errout_with_semaphore:
	if (entry.name != NULL) {
		kmm_free(entry.name);
	}
	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_rename
 *
 * Description: Rename a file or directory
 *
 ****************************************************************************/

int smartfs_rename(struct inode *mountpt, const char *oldrelpath, const char *newrelpath)
{
	int ret;
	struct smartfs_mountpt_s *fs;
	struct smartfs_entry_s oldentry;
	struct smartfs_entry_s newentry;
	mode_t mode;
	uint16_t type;
#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
	struct smart_read_write_s readwrite;
#endif

	/* Sanity checks */

	DEBUGASSERT(mountpt && mountpt->i_private);

	/* Get the mountpoint private data from the inode structure */

	fs = mountpt->i_private;

	smartfs_semtake(fs);

	/* Search for old entry to validate it exists */

	oldentry.name = NULL;
	newentry.name = NULL;
	ret = smartfs_finddirentry(fs, &oldentry, oldrelpath);
	if (ret != OK) {
		fdbg("Old entry doesn't exist\n");
		goto errout_with_semaphore;
	}

	/* Search for the new entry and validate it DOESN'T exist */
	ret = smartfs_finddirentry(fs, &newentry, newrelpath);
	if (ret == OK) {
		/* The entry to be created after rename already exists, cannot complete rename request */
		ret = -EEXIST;
		goto errout_with_semaphore;
	}
	if (ret != -ENOENT) {
		fdbg("Cannot rename entry, ret : %d\n", ret);
		goto errout_with_semaphore;
	}

	/* Test if the new parent directory is valid */
	if (newentry.dsector != SMARTFS_ERASEDSTATE_16BIT) {
		/* We can move to the given parent directory */

		mode = oldentry.flags & SMARTFS_DIRENT_MODE;
		type = oldentry.flags & SMARTFS_DIRENT_TYPE;

#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
		if (oldentry.dfirst == newentry.dsector) {
			/* We will not use any new entry found, we will overwrite the existing entry but with a new name */
			smartfs_setbuffer(&readwrite, oldentry.dsector, oldentry.doffset + offsetof(struct smartfs_entry_header_s, name), fs->fs_llformat.namesize, (uint8_t *)newentry.name);
			ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&readwrite);
			if (ret != OK) {
				fdbg("Error writing new entry to sector %d, ret : %d\n", readwrite.logsector, ret);
			}
			/* Old entry doesn't have to be invalidated, directly go to end */
			goto errout_with_semaphore;
		}
#endif
		/* Find an available invalid/empty entry to write the new entry */
		ret = smartfs_find_availableentry(fs, &newentry);
		if (ret != OK) {
			fdbg("Unable to find space for writing new entry, ret : %d\n", ret);
			goto errout_with_semaphore;
		}

		newentry.firstsector = oldentry.firstsector;
		ret = smartfs_writeentry(fs, newentry, type, mode);
		if (ret != OK) {
			fdbg("Unable to write new entry, ret : %d\n", ret);
			goto errout_with_semaphore;
		}

		/* Now mark the old entry as inactive */
		ret = smartfs_invalidateentry(fs, oldentry.dsector, oldentry.doffset);
		if (ret != OK) {
			fdbg("Unable to invalidate old entry, ret : %d\n", ret);
			goto errout_with_semaphore;
		}
	} else {
		/* Trying to create in a directory that doesn't exist */

		ret = -ENOENT;
		goto errout_with_semaphore;
	}

	ret = OK;

errout_with_semaphore:
	if (oldentry.name != NULL) {
		kmm_free(oldentry.name);
		oldentry.name = NULL;
	}

	if (newentry.name != NULL) {
		kmm_free(newentry.name);
		newentry.name = NULL;
	}

	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_stat_common
 *
 * Description:
 *   Return information about a directory entry
 *
 ****************************************************************************/

static void smartfs_stat_common(FAR struct smartfs_mountpt_s *fs,
								FAR struct smartfs_entry_s *entry,
								FAR struct stat *buf)
{
	/* Initialize the stat structure */
	memset(buf, 0, sizeof(struct stat));
	if (entry->firstsector == fs->fs_rootsector) {
		/* It's directory name of the mount point */
		buf->st_mode = S_IFDIR | S_IROTH | S_IRGRP | S_IRUSR | S_IWOTH |
					   S_IWGRP | S_IWUSR;
	} else {
		buf->st_mode = entry->flags & 0xFFF;
		if ((entry->flags & SMARTFS_DIRENT_TYPE) == SMARTFS_DIRENT_TYPE_DIR) {
			buf->st_mode |= S_IFDIR;
		} else {
			buf->st_mode |= S_IFREG;
		}
	}
	buf->st_size = entry->datalen;
	buf->st_blksize = SMARTFS_AVAIL_DATABYTES(fs);
	buf->st_blocks = (buf->st_size + buf->st_blksize - 1) / buf->st_blksize;
	buf->st_atime = 0;
	buf->st_ctime = 0;
}

/****************************************************************************
 * Name: smartfs_stat
 *
 * Description: Return information about a file or directory
 *
 ****************************************************************************/

static int smartfs_stat(struct inode *mountpt, const char *relpath, struct stat *buf)
{
	struct smartfs_mountpt_s *fs;
	struct smartfs_entry_s entry;
	int ret;

	/* Sanity checks */

	DEBUGASSERT(mountpt && mountpt->i_private);
	DEBUGASSERT(buf != NULL);

	/* Get the mountpoint private data from the inode structure */

	fs = mountpt->i_private;

	smartfs_semtake(fs);

	/* Find the directory entry corresponding to relpath */

	entry.name = NULL;
	ret = smartfs_finddirentry(fs, &entry, relpath);
	if (ret < 0) {
		goto errout_with_semaphore;
	}

	/* We need to know the data length of the file too */
	ret = smartfs_get_datalen(fs, entry.firstsector, &entry.datalen);
	if (ret < 0) {
		fdbg("ERROR, Could not get the length of the file, ret : %d\n", ret);
		goto errout_with_semaphore;
	}

	/* Initialize the stat structure */
	smartfs_stat_common(fs, &entry, buf);

	ret = OK;

errout_with_semaphore:
	if (entry.name != NULL) {
		kmm_free(entry.name);
		entry.name = NULL;
	}

	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
