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
#include <stdint.h>

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
static int smartfs_stat(struct inode *mountpt, const char *relpath, struct stat *buf);

static off_t smartfs_seek_internal(struct smartfs_mountpt_s *fs, struct smartfs_ofile_s *sf, off_t offset, int whence);

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

	smartfs_opendir,			/* opendir */
	NULL,						/* closedir */
	smartfs_readdir,			/* readdir */
	smartfs_rewinddir,			/* rewinddir */

	smartfs_bind,				/* bind */
	smartfs_unbind,				/* unbind */
	smartfs_statfs,				/* statfs */

	smartfs_unlink,				/* unlinke */
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
	uint16_t parentdirsector;
	const char *filename;
	struct smartfs_ofile_s *sf;

#ifdef CONFIG_SMARTFS_JOURNALING
	int retj;
	uint16_t t_sector, t_offset;
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

	sf = (struct smartfs_ofile_s *)kmm_malloc(sizeof * sf);
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

	sf->bflags = 0;
#endif							/* CONFIG_SMARTFS_USE_SECTOR_BUFFER */

	sf->entry.name = NULL;
	ret = smartfs_finddirentry(fs, &sf->entry, relpath, &parentdirsector, &filename);

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

		/* TODO: Test open mode based on the file mode */

		/* The file exists.  Check if we are opening it for O_CREAT or
		 * O_TRUNC mode and delete the sector chain if we are. */

		if ((oflags & (O_CREAT | O_TRUNC)) != 0) {
			/* Don't truncate if open for APPEND */

			if (!(oflags & O_APPEND)) {
				/* Truncate the file as part of the open */

				ret = smartfs_truncatefile(fs, &sf->entry, sf);
				if (ret < 0) {
					goto errout_with_buffer;
				}
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

		if (parentdirsector != 0xFFFF) {
			/* We can create in the given parent directory */

#ifdef CONFIG_SMARTFS_JOURNALING
			ret = smartfs_create_journalentry(fs, T_CREATE, parentdirsector, 0, fs->fs_llformat.namesize, (uint16_t)mode, 0, (uint8_t *)filename, &t_sector, &t_offset);
			if (ret != OK) {
				fdbg("Journal entry creation failed.\n");
				goto errout_with_buffer;
			}
#endif
			ret = smartfs_createentry(fs, parentdirsector, filename, SMARTFS_DIRENT_TYPE_FILE, mode, &sf->entry, 0xFFFF, sf);
#ifdef CONFIG_SMARTFS_JOURNALING
			retj = smartfs_finish_journalentry(fs, 0, t_sector, t_offset, T_CREATE);
			if (retj != OK) {
				fdbg("Error finishing transaction\n");
				ret = retj;
				goto errout_with_buffer;
			}
#endif
			if (ret != OK) {
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

okout:
	smartfs_semgive(fs);
	return OK;
}

/****************************************************************************
 * Name: smartfs_read
 ****************************************************************************/

#ifdef CONFIG_SMARTFS_SECURE
static unsigned char output[MAXBUFSIZE];
#endif
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

		readwrite.logsector = sf->currsector;
		readwrite.offset = 0;
		readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
		readwrite.count = fs->fs_llformat.availbytes;
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
		if (ret < 0) {
			fdbg("Error %d reading sector %d data\n", ret, sf->currsector);
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

		bytestoread = bytesinsector - (sf->curroffset - sizeof(struct smartfs_chain_header_s));
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

			/* Test if at end of data */

			if (sf->currsector == SMARTFS_ERASEDSTATE_16BIT) {
				/* No more data!  Return what we have */

				break;
			}
		}
	}

#ifdef CONFIG_SMARTFS_SECURE
	char *final_output;
	ret = bytesread;
	final_output = (char *)kmm_zalloc(buflen + 1);
	/*Smartfs function for decryption*/

	smartfs_decrypt(buffer, ret, final_output);
	memset(buffer, '\0', MAXBUFSIZE);
	/*Copying the decrypted data into original buffer */

	memcpy(buffer, (const char *)final_output, ret);
	kmm_free(final_output);
#endif
	/* Return the number of bytes we read */
errout_with_semaphore:
	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_sync_internal
 *
 * Description: Synchronize the file state on disk to match internal, in-
 *   memory state.
 *
 ****************************************************************************/

static int smartfs_sync_internal(struct smartfs_mountpt_s *fs, struct smartfs_ofile_s *sf)
{
	struct smart_read_write_s readwrite;
	struct smartfs_chain_header_s *header;
	int ret = OK;
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
	int used_value;
#endif

#ifdef CONFIG_SMARTFS_JOURNALING
	int retj;
	uint16_t used_bytes;
	uint16_t t_sector, t_offset;
#endif

#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
	if (sf->bflags & SMARTFS_BFLAG_DIRTY) {
		/* Update the header with the number of bytes written */

		header = (struct smartfs_chain_header_s *)sf->buffer;
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
		used_value = get_leftover_used_byte_count((uint8_t *)sf.buffer, get_used_byte_count((uint8_t *)header->used));
		if (used_value == 0) {
			set_used_byte_count((uint8_t *)header->used, sf->byteswritten);
#else
		if (header->used[0] == CONFIG_SMARTFS_ERASEDSTATE) {
			*((uint16_t *)header->used) = sf->byteswritten;
#endif
		} else {
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
			set_used_byte_count((uint8_t *)header->used, used_value + sf->byteswritten);
#else
			*((uint16_t *)header->used) += sf->byteswritten;
#endif
		}

		/* Write the entire sector to FLASH */

		readwrite.logsector = sf->currsector;
		readwrite.offset = 0;
		readwrite.count = fs->fs_llformat.availbytes;
		readwrite.buffer = sf->buffer;

		ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&readwrite);
		if (ret < 0) {
			fdbg("Error %d writing used bytes for sector %d\n", ret, sf->currsector);
			goto errout;
		}

		sf->byteswritten = 0;
		sf->bflags = 0;
	}
#else							/* CONFIG_SMARTFS_USE_SECTOR_BUFFER */

	/* Test if we have written bytes to the current sector that
	 * need to be recorded in the chain header's used bytes field. */

	if (sf->byteswritten > 0) {
		fvdbg("Syncing sector %d\n", sf->currsector);

		/* Read the existing sector used bytes value */

		readwrite.logsector = sf->currsector;
		readwrite.offset = 0;
		header = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;
		readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
		readwrite.count = sizeof(struct smartfs_chain_header_s);
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
		if (ret < 0) {
			fdbg("Error %d reading sector %d data\n", ret, sf->currsector);
			goto errout;
		}
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
		used_value = sf->entry.datlen % (fs->fs_llformat.availbytes - sizeof(struct smartfs_chain_header_s));
		if (sf->entry.datlen > 0 && used_value == 0) {
			used_value = fs->fs_llformat.availbytes - sizeof(struct smartfs_chain_header_s);
		}

		set_used_byte_count((uint8_t *)header->used, used_value);
#else
		if (SMARTFS_USED(header) == SMARTFS_ERASEDSTATE_16BIT) {
			header->used[0] = (uint8_t)(sf->byteswritten & 0x00FF);
			header->used[1] = (uint8_t)(sf->byteswritten >> 8);
		} else {
			uint16_t tmp = SMARTFS_USED(header);
			tmp += sf->byteswritten;
			header->used[0] = (uint8_t)(tmp & 0x00FF);
			header->used[1] = (uint8_t)(tmp >> 8);
		}
#endif
		readwrite.offset = offsetof(struct smartfs_chain_header_s, used);
		readwrite.count = sizeof(uint16_t);
		readwrite.buffer = (uint8_t *)&fs->fs_rwbuffer[readwrite.offset];
#ifdef CONFIG_SMARTFS_JOURNALING
		used_bytes = ((header->used[0] & 0x00FF) | (header->used[1] & 0x00FF) << 8);

		ret = smartfs_create_journalentry(fs, T_SYNC, readwrite.logsector, readwrite.offset, 0, used_bytes, 1, NULL, &t_sector, &t_offset);
		if (ret != OK) {
			fdbg("Journal entry creation failed.\n");
			goto errout;
		}
#endif
		ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&readwrite);
#ifdef CONFIG_SMARTFS_JOURNALING
		retj = smartfs_finish_journalentry(fs, readwrite.logsector, t_sector, t_offset, T_SYNC);
		if (retj != OK) {
			fdbg("Error finishing transaction\n");
			ret = retj;
			goto errout;
		}
#endif
		if (ret < 0) {
			fdbg("Error %d writing used bytes for sector %d\n", ret, sf->currsector);
			goto errout;
		}

		sf->byteswritten = 0;
	}
#endif							/* CONFIG_SMARTFS_USE_SECTOR_BUFFER */

errout:
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
	int ret;
#ifdef CONFIG_SMARTFS_SECURE
	size_t writelength;
	size_t origlength;
#endif

#ifdef CONFIG_SMARTFS_JOURNALING
	int retj;
	uint16_t t_sector, t_offset;
#endif
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

	/* Test the permissions.  Only allow write if the file was opened with
	 * write flags.
	 */

	if ((sf->oflags & O_WROK) == 0) {
		ret = -EACCES;
		goto errout_with_semaphore;
	}

#ifdef CONFIG_SMARTFS_SECURE
	memset(output, '\0', sizeof(output));
	/*Smartfs function for encryption */

	smartfs_encrypt(buffer, buflen, output);
	/*Origlength variable contains the original length of data*/
	/*Writelength variable contains the length of encrypted data*/

	origlength = buflen;
	if (buflen % CRYPTSIZE != 0) {
		buflen = ((buflen / CRYPTSIZE) + 1) * CRYPTSIZE;
	}
	writelength = buflen;
#endif
	/* First test if we are overwriting an existing location or writing to
	 * a new one. */

	header = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;
	byteswritten = 0;
	while ((sf->filepos < sf->entry.datlen) && (buflen > 0)) {
		/* Overwriting data caused by a seek, etc.  In this case, we need
		 * to check if the write causes the file length to be extended
		 * or not and update it accordingly.  We will write data up to
		 * the current end-of-file and then break, allowing the next while
		 * loop below to write the additional data to the end of the file.
		 */

		readwrite.offset = sf->curroffset;
		readwrite.logsector = sf->currsector;
#ifdef CONFIG_SMARTFS_SECURE
		readwrite.buffer = (uint8_t *)&output[byteswritten];
#else
		readwrite.buffer = (uint8_t *)&buffer[byteswritten];
#endif	
		readwrite.count = fs->fs_llformat.availbytes - sf->curroffset;

		/* Limit the write based on available data to write */

		if (readwrite.count > buflen) {
			readwrite.count = buflen;
		}

		/* Limit the write based on current file length */

		if (readwrite.count > sf->entry.datlen - sf->filepos) {
			/* Limit the write length so we write to the current EOF. */

			readwrite.count = sf->entry.datlen - sf->filepos;
		}

		/* Now perform the write. */

		if (readwrite.count > 0) {
#ifdef CONFIG_SMARTFS_JOURNALING
			ret = smartfs_create_journalentry(fs, T_WRITE, readwrite.logsector, readwrite.offset, readwrite.count, 0, 0, readwrite.buffer, &t_sector, &t_offset);
			if (ret != OK) {
				fdbg("Journal entry creation failed.\n");
				goto errout_with_semaphore;
			}
#endif
			ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&readwrite);

#ifdef CONFIG_SMARTFS_JOURNALING
			retj = smartfs_finish_journalentry(fs, 0, t_sector, t_offset, T_WRITE);
			if (retj != OK) {
				fdbg("Error finishing transaction\n");
				ret = retj;
				goto errout_with_semaphore;
			}
#endif
			if (ret < 0) {
				fdbg("Error %d writing sector %d data\n", ret, sf->currsector);
				goto errout_with_semaphore;
			}

			/* Update our control variables */

			sf->filepos += readwrite.count;
			sf->curroffset += readwrite.count;
			buflen -= readwrite.count;
			byteswritten += readwrite.count;
		}

		/* Test if we wrote to the end of the current sector */

		if (sf->curroffset == fs->fs_llformat.availbytes) {
			/* Wrote to the end of the sector.  Update to point to the
			 * next sector for additional writes.  First read the sector
			 * header to get the sector chain info.
			 */

			readwrite.offset = 0;
			readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
			readwrite.count = sizeof(struct smartfs_chain_header_s);
			ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
			if (ret < 0) {
				fdbg("Error %d reading sector %d header\n", ret, sf->currsector);
				goto errout_with_semaphore;
			}

			/* Now get the chained sector info and reset the offset */

			sf->curroffset = sizeof(struct smartfs_chain_header_s);
			sf->currsector = SMARTFS_NEXTSECTOR(header);
		}
	}

	/* Now append data to end of the file. */

	while (buflen > 0) {
		/* We will fill up the current sector. Write data to
		 * the current sector first.
		 */

#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
		readwrite.count = fs->fs_llformat.availbytes - sf->curroffset;
		if (readwrite.count > buflen) {
			readwrite.count = buflen;
		}

#ifdef CONFIG_SMARTFS_SECURE
		memcpy(&sf->buffer[sf->curroffset], &output[byteswritten], readwrite.count);
#else
		memcpy(&sf->buffer[sf->curroffset], &buffer[byteswritten], readwrite.count);
#endif	
		sf->bflags |= SMARTFS_BFLAG_DIRTY;

#else							/* CONFIG_SMARTFS_USE_SECTOR_BUFFER */
		readwrite.offset = sf->curroffset;
		readwrite.logsector = sf->currsector;
#ifdef CONFIG_SMARTFS_SECURE
		readwrite.buffer = (uint8_t *)&output[byteswritten];
#else
		readwrite.buffer = (uint8_t *)&buffer[byteswritten];
#endif		
		readwrite.count = fs->fs_llformat.availbytes - sf->curroffset;
		if (readwrite.count > buflen) {
			/* Limit the write base on remaining bytes to write */

			readwrite.count = buflen;
		}

		/* Perform the write */

		if (readwrite.count > 0) {
#ifdef CONFIG_SMARTFS_JOURNALING
			ret = smartfs_create_journalentry(fs, T_WRITE, readwrite.logsector, readwrite.offset, readwrite.count, sf->curroffset + readwrite.count - sizeof(struct smartfs_chain_header_s), 1, readwrite.buffer, &t_sector, &t_offset);
			if (ret != OK) {
				fdbg("Journal entry creation failed.\n");
				goto errout_with_semaphore;
			}
#endif

			ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&readwrite);
			if (ret < 0) {
				fdbg("Error %d writing sector %d data\n", ret, sf->currsector);
				goto errout_with_semaphore;
			}
		}
#endif							/* CONFIG_SMARTFS_USE_SECTOR_BUFFER */

		/* Update our control variables */

		sf->entry.datlen += readwrite.count;
		sf->byteswritten += readwrite.count;
		sf->filepos += readwrite.count;
		sf->curroffset += readwrite.count;
		buflen -= readwrite.count;
		byteswritten += readwrite.count;

		/* Test if we wrote a full sector of data */

#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER
		if (sf->curroffset == fs->fs_llformat.availbytes && buflen) {
			/* First get a new chained sector */

			ret = FS_IOCTL(fs, BIOC_ALLOCSECT, 0xFFFF);
			if (ret < 0) {
				fdbg("Error %d allocating new sector\n", ret);
				goto errout_with_semaphore;
			}

			/* Copy the new sector to the old one and chain it */

			header = (struct smartfs_chain_header_s *)sf->buffer;
			*((uint16_t *)header->nextsector) = (uint16_t)ret;

			/* Now sync the file to write this sector out */

			ret = smartfs_sync_internal(fs, sf);
			if (ret != OK) {
				goto errout_with_semaphore;
			}

			/* Record the new sector in our tracking variables and
			 * reset the offset to "zero".
			 */

			if (sf->currsector == SMARTFS_NEXTSECTOR(header)) {
				/* Error allocating logical sector! */

				fdbg("Error - duplicate logical sector %d\n", sf->currsector);
			}

			sf->bflags = SMARTFS_BFLAG_DIRTY;
			sf->currsector = SMARTFS_NEXTSECTOR(header);
			sf->curroffset = sizeof(struct smartfs_chain_header_s);
			memset(sf->buffer, CONFIG_SMARTFS_ERASEDSTATE, fs->fs_llformat.availbytes);
			header->type = SMARTFS_DIRENT_TYPE_FILE;
		}
#else							/* CONFIG_SMARTFS_USE_SECTOR_BUFFER */

		if (sf->curroffset == fs->fs_llformat.availbytes) {
			/* Sync the file to write this sector out */

			ret = smartfs_sync_internal(fs, sf);
			if (ret != OK) {
				goto errout_with_semaphore;
			}

			/* Allocate a new sector if needed */

			if (buflen > 0) {
				/* Allocate a new sector */

				ret = FS_IOCTL(fs, BIOC_ALLOCSECT, 0xFFFF);
				if (ret < 0) {
					fdbg("Error %d allocating new sector\n", ret);
					goto errout_with_semaphore;
				}

				/* Copy the new sector to the old one and chain it */

				header = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;
				header->nextsector[0] = (uint8_t)(ret & 0x00FF);
				header->nextsector[1] = (uint8_t)((ret >> 8) & 0x00FF);

				readwrite.offset = offsetof(struct smartfs_chain_header_s, nextsector);
				readwrite.buffer = (uint8_t *)header->nextsector;
				readwrite.count = sizeof(uint16_t);
#ifdef CONFIG_SMARTFS_JOURNALING
				ret = smartfs_create_journalentry(fs, T_WRITE, readwrite.logsector,
												  readwrite.offset, readwrite.count, 0, 0, readwrite.buffer, &t_sector, &t_offset);
				if (ret != OK) {
					fdbg("Journal entry creation failed.\n");
					goto errout_with_semaphore;
				}
#endif
				ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&readwrite);
#ifdef CONFIG_SMARTFS_JOURNALING
				retj = smartfs_finish_journalentry(fs, 0, t_sector, t_offset, T_WRITE);
				if (retj != OK) {
					fdbg("Error finishing transaction\n");
					ret = retj;
					goto errout_with_semaphore;
				}
#endif
				if (ret < 0) {
					fdbg("Error %d writing next sector\n", ret);
					goto errout_with_semaphore;
				}

				/* Record the new sector in our tracking variables and
				 * reset the offset to "zero".
				 */

				if (sf->currsector == SMARTFS_NEXTSECTOR(header)) {
					/* Error allocating logical sector! */

					fdbg("Error - duplicate logical sector %d\n", sf->currsector);
				}

				sf->currsector = SMARTFS_NEXTSECTOR(header);
				sf->curroffset = sizeof(struct smartfs_chain_header_s);
			}
		}
#endif							/* CONFIG_SMARTFS_USE_SECTOR_BUFFER */
	}

#ifdef CONFIG_SMARTFS_SECURE
	if (byteswritten == writelength) {
		ret = origlength;
	} else {
		ret = byteswritten;
	}
#else
	ret = byteswritten;
#endif

errout_with_semaphore:
	smartfs_semgive(fs);
	return ret;
}

/****************************************************************************
 * Name: smartfs_seek_internal
 *
 * Description: Performs the logic of the seek function.  This is an internal
 *              function because it does not provide semaphore protection and
 *              therefore must be called from one of the other public
 *              interface routines (open, seek, etc.).
 *
 ****************************************************************************/

static off_t smartfs_seek_internal(struct smartfs_mountpt_s *fs, struct smartfs_ofile_s *sf, off_t offset, int whence)
{
	struct smart_read_write_s readwrite;
	struct smartfs_chain_header_s *header;
	int ret;
	off_t newpos;
	off_t sectorstartpos;
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
	int sector_used = 0;
#endif
	/* Test if this is a seek to get the current file pos */

	if ((whence == SEEK_CUR) && (offset == 0)) {
		return sf->filepos;
	}

	/* Test if we need to sync the file */

	if (sf->byteswritten > 0) {
		/* Perform a sync */

		smartfs_sync_internal(fs, sf);
	}

	/* Calculate the file position to seek to based on current position */

	switch (whence) {
	case SEEK_SET:
	default:
		newpos = offset;
		break;

	case SEEK_CUR:
		newpos = sf->filepos + offset;
		break;

	case SEEK_END:
		newpos = sf->entry.datlen + offset;
		break;
	}

	/* Ensure newpos is in range */

	if (newpos < 0) {
		newpos = 0;
	}

	if (newpos > sf->entry.datlen) {
		newpos = sf->entry.datlen;
	}

	/* Now perform the seek.  Test if we are seeking within the current
	 * sector and can skip the search to save time.
	 */

	sectorstartpos = sf->filepos - (sf->curroffset - sizeof(struct smartfs_chain_header_s));
	if (newpos >= sectorstartpos && newpos < sectorstartpos + fs->fs_llformat.availbytes - sizeof(struct smartfs_chain_header_s)) {
		/* Seeking within the current sector.  Just update the offset */

		sf->curroffset = sizeof(struct smartfs_chain_header_s) + newpos - sectorstartpos;
		sf->filepos = newpos;

		return newpos;
	}

	/* Nope, we have to search for the sector and offset.  If the new pos is greater
	 * than the current pos, then we can start from the beginning of the current
	 * sector, otherwise we have to start from the beginning of the file.
	 */

	if (newpos > sf->filepos) {
		sf->filepos = sectorstartpos;
	} else {
		sf->currsector = sf->entry.firstsector;
		sf->filepos = 0;
	}

	header = (struct smartfs_chain_header_s *)fs->fs_rwbuffer;
	while ((sf->currsector != SMARTFS_ERASEDSTATE_16BIT) && (sf->filepos + fs->fs_llformat.availbytes - sizeof(struct smartfs_chain_header_s) < newpos)) {
		/* Read the sector's header */

		readwrite.logsector = sf->currsector;
		readwrite.offset = 0;
		readwrite.count = sizeof(struct smartfs_chain_header_s);
		readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
		if (ret < 0) {
			fdbg("Error %d reading sector %d header\n", ret, sf->currsector);
			goto errout;
		}
#ifdef CONFIG_SMARTFS_DYNAMIC_HEADER
		if (SMARTFS_NEXTSECTOR(header) == SMARTFS_ERASEDSTATE_16BIT) {
			sf->filepos += (sf->entry.datlen - (sector_used * (fs->fs_llformat.availbytes - sizeof(struct smartfs_chain_header_s))));
		} else {
			sf->filepos += (fs->fs_llformat.availbytes - sizeof(struct smartfs_chain_header_s));
		}
		sector_used++;
#else
		sf->filepos += SMARTFS_USED(header);
#endif
		sf->currsector = SMARTFS_NEXTSECTOR(header);
	}

#ifdef CONFIG_SMARTFS_USE_SECTOR_BUFFER

	/* When using sector buffering, we must read in the last buffer to our
	 * sf->buffer in case any changes are made.
	 */

	if (sf->currsector != SMARTFS_ERASEDSTATE_16BIT) {
		readwrite.logsector = sf->currsector;
		readwrite.offset = 0;
		readwrite.count = fs->fs_llformat.availbytes;
		readwrite.buffer = (uint8_t *)sf->buffer;
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
		if (ret < 0) {
			fdbg("Error %d reading sector %d header\n", ret, sf->currsector);
			goto errout;
		}
	}
#endif

	/* Now calculate the offset */

	sf->curroffset = sizeof(struct smartfs_chain_header_s) + newpos - sf->filepos;
	sf->filepos = newpos;
	return newpos;

errout:
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
	uint16_t parentdirsector;
	const char *filename;

	/* Sanity checks */

	DEBUGASSERT(mountpt != NULL && mountpt->i_private != NULL);

	/* Recover our private data from the inode instance */

	fs = mountpt->i_private;

	/* Take the semaphore */

	smartfs_semtake(fs);

	/* Search for the path on the volume */

	entry.name = NULL;
	ret = smartfs_finddirentry(fs, &entry, relpath, &parentdirsector, &filename);
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

		readwrite.logsector = dir->u.smartfs.fs_currsector;
		readwrite.count = fs->fs_llformat.availbytes;
		readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
		readwrite.offset = 0;
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
			if (dir->u.smartfs.fs_curroffset >= fs->fs_llformat.availbytes) {
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
	 * initialized it now. */

	fs->fs_sem = &g_sem;
	if (!g_seminitialized) {
		sem_init(&g_sem, 0, 0);	/* Initialize the semaphore that controls access */
		g_seminitialized = TRUE;
	} else {
		/* Take the semaphore for the mount */

		smartfs_semtake(fs);
	}

	/* Initialize the allocated mountpt state structure.  The filesystem is
	 * responsible for one reference ont the blkdriver inode and does not
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
#ifdef CONFIG_SMARTFS_JOURNALING
	ret = smartfs_journal_init(fs);
	if (ret != 0) {
		fdbg("init failed!!\n");
		goto error_with_semaphore;
	}
#endif

#ifdef CONFIG_SMARTFS_SECTOR_RECOVERY
	ret = smartfs_recover(fs);
	if (ret != 0) {
		fdbg("recovery failed!!\n");
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
#ifdef CONFIG_SMARTFS_JOURNALING
	if (fs->journal) {
		kmm_free(fs->journal);
	}
#endif
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
	const char *filename;
	uint16_t parentdirsector;
#ifdef CONFIG_SMARTFS_JOURNALING
	uint16_t t_sector, t_offset;
#endif

	/* Sanity checks */

	DEBUGASSERT(mountpt && mountpt->i_private);

	/* Get the mountpoint private data from the inode structure */

	fs = mountpt->i_private;

	smartfs_semtake(fs);

	/* Locate the directory entry for this path */

	entry.name = NULL;
	ret = smartfs_finddirentry(fs, &entry, relpath, &parentdirsector, &filename);

	if (ret == OK) {
		/* The name exists -- validate it is a file, not a dir */

		if ((entry.flags & SMARTFS_DIRENT_TYPE) == SMARTFS_DIRENT_TYPE_DIR) {
			ret = -EISDIR;
			goto errout_with_semaphore;
		}

		/* TODO:  Need to check permissions?  */

		/* Okay, we are clear to delete the file.  Use the deleteentry routine. */

#ifdef CONFIG_SMARTFS_JOURNALING
		ret = smartfs_create_journalentry(fs, T_DELETE, entry.dsector, entry.doffset, entry.dfirst, entry.firstsector, 0, NULL, &t_sector, &t_offset);
		if (ret != OK) {
			fdbg("Journal entry creation failed.\n");
			goto errout_with_semaphore;
		}
#endif

		smartfs_deleteentry(fs, &entry);
#ifdef CONFIG_SMARTFS_JOURNALING
		ret = smartfs_finish_journalentry(fs, 0, t_sector, t_offset, T_DELETE);
		if (ret != OK) {
			fdbg("Error finishing transaction\n");
			goto errout_with_semaphore;
		}
#endif

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
	uint16_t parentdirsector;
	const char *filename;
#ifdef CONFIG_SMARTFS_JOURNALING
	int retj;
	uint16_t t_sector, t_offset;
#endif

	/* Sanity checks */

	DEBUGASSERT(mountpt && mountpt->i_private);

	/* Get the mountpoint private data from the inode structure */

	fs = mountpt->i_private;

	smartfs_semtake(fs);

	/* Locate the directory entry for this path */

	entry.name = NULL;
	ret = smartfs_finddirentry(fs, &entry, relpath, &parentdirsector, &filename);

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
		 * the right permissions and if the parentdirsector is valid. */

		if (parentdirsector == 0xFFFF) {
			/* Invalid entry in the path (non-existant dir segment) */

			goto errout_with_semaphore;
		}

		/* Check mode */

		/* Create the directory */

#ifdef CONFIG_SMARTFS_JOURNALING
		ret = smartfs_create_journalentry(fs, T_MKDIR, parentdirsector, 0, fs->fs_llformat.namesize, (uint16_t)mode, 0, (uint8_t *)filename, &t_sector, &t_offset);
		if (ret != OK) {
			fdbg("Journal entry creation failed.\n");
			goto errout_with_semaphore;
		}
#endif

		ret = smartfs_createentry(fs, parentdirsector, filename,
								  SMARTFS_DIRENT_TYPE_DIR, mode, &entry, 0xFFFF, NULL);
#ifdef CONFIG_SMARTFS_JOURNALING
		retj = smartfs_finish_journalentry(fs, 0, t_sector, t_offset, T_MKDIR);
		if (retj != OK) {
			fdbg("Error finishing transaction\n");
			ret = retj;
			goto errout_with_semaphore;
		}
#endif
		if (ret != OK) {
			goto errout_with_semaphore;
		}

		ret = OK;
	}

errout_with_semaphore:
	if (entry.name != NULL) {
		/* Free the filename space allocation */

		kmm_free(entry.name);
		entry.name = NULL;
	}

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
	const char *filename;
	uint16_t parentdirsector;
#ifdef CONFIG_SMARTFS_JOURNALING
	int retj;
	uint16_t t_sector, t_offset;
#endif

	/* Sanity checks */

	DEBUGASSERT(mountpt && mountpt->i_private);

	/* Get the mountpoint private data from the inode structure */

	fs = mountpt->i_private;

	/* Take the semaphore */

	smartfs_semtake(fs);

	/* Locate the directory entry for this path */

	entry.name = NULL;
	ret = smartfs_finddirentry(fs, &entry, relpath, &parentdirsector, &filename);

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
#ifdef CONFIG_SMARTFS_JOURNALING
		ret = smartfs_create_journalentry(fs, T_DELETE, entry.dsector, entry.doffset, entry.dfirst, 0, 0, NULL, &t_sector, &t_offset);
		if (ret != OK) {
			fdbg("Journal entry creation failed.\n");
			goto errout_with_semaphore;
		}
#endif
		/* Okay, we are clear to delete the directory.  Use the deleteentry routine. */

		ret = smartfs_deleteentry(fs, &entry);
#ifdef CONFIG_SMARTFS_JOURNALING
		retj = smartfs_finish_journalentry(fs, 0, t_sector, t_offset, T_DELETE);
		if (retj != OK) {
			fdbg("Error finishing transaction\n");
			ret = retj;
			goto errout_with_semaphore;
		}
#endif
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
	struct smartfs_mountpt_s *fs;
	int ret;
	struct smartfs_entry_s oldentry;
	uint16_t oldparentdirsector;
	const char *oldfilename;
	struct smartfs_entry_s newentry;
	uint16_t newparentdirsector;
	const char *newfilename;
	mode_t mode;
	uint16_t type;
	uint16_t sector;
	uint16_t offset;
	uint16_t entrysize;
	struct smartfs_entry_header_s *direntry;
	struct smart_read_write_s readwrite;

#ifdef CONFIG_SMARTFS_JOURNALING
	int retj;
	uint16_t t_sector, t_offset;
#endif

	uint8_t *tmp_pntr = NULL;
	uint16_t tmp_flag = 0;

	/* Sanity checks */

	DEBUGASSERT(mountpt && mountpt->i_private);

	/* Get the mountpoint private data from the inode structure */

	fs = mountpt->i_private;

	smartfs_semtake(fs);

	/* Search for old entry to validate it exists */

	oldentry.name = NULL;
	newentry.name = NULL;
	ret = smartfs_finddirentry(fs, &oldentry, oldrelpath, &oldparentdirsector, &oldfilename);
	if (ret < 0) {
		goto errout_with_semaphore;
	}

	/* Search for the new entry and validate it DOESN'T exist, unless we
	 * are copying to a directory and keeping the same filename, such as:
	 *
	 *    mv /mntpoint/somefile.txt /mntpoint/somedir
	 *
	 * in which case, we need to validate the /mntpoint/somedir/somefile.txt
	 * doens't exsit.
	 */

	ret = smartfs_finddirentry(fs, &newentry, newrelpath, &newparentdirsector, &newfilename);
	if (ret == OK) {
		/* Test if it's a file.  If it is, then it's an error */

		if ((newentry.flags & SMARTFS_DIRENT_TYPE) == SMARTFS_DIRENT_TYPE_FILE) {
			ret = -EEXIST;
			goto errout_with_semaphore;
		}

		/* Nope, it's a directory.  Now search the directory for oldfilename */

		sector = newentry.firstsector;
		while (sector != SMARTFS_ERASEDSTATE_16BIT) {
			/* Read the next sector of diretory entries */

			readwrite.logsector = sector;
			readwrite.offset = 0;
			readwrite.count = fs->fs_llformat.availbytes;
			readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
			ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
			if (ret < 0) {
				fdbg("Error %d reading sector %d data\n", ret, oldentry.dsector);
				goto errout_with_semaphore;
			}

			/* Search for newfilename in this sector */

			offset = sizeof(struct smartfs_chain_header_s);
			entrysize = sizeof(struct smartfs_entry_header_s) + fs->fs_llformat.namesize;
			while (offset + entrysize < fs->fs_llformat.availbytes) {
				/* Test the next entry */

				direntry = (struct smartfs_entry_header_s *)&fs->fs_rwbuffer[offset];
				if (((direntry->flags & SMARTFS_DIRENT_EMPTY) == (SMARTFS_ERASEDSTATE_16BIT & SMARTFS_DIRENT_EMPTY)) || ((direntry->flags & SMARTFS_DIRENT_ACTIVE) != (SMARTFS_ERASEDSTATE_16BIT & SMARTFS_DIRENT_ACTIVE))) {
					/* This entry isn't valid, skip it */

					offset += entrysize;
					continue;
				}

				/* Test if this entry matches newfilename */

				if (strcmp(newfilename, direntry->name) == 0) {
					/* Uh-oh, looks like the entry already exists */

					ret = -EEXIST;
					goto errout_with_semaphore;
				}

				offset += entrysize;
			}

			/* Chain to next sector */

			sector = SMARTFS_NEXTSECTOR(((struct smartfs_chain_header_s *)fs->fs_rwbuffer));
		}

		/* Okay, we will create newfilename in the newentry directory */

		newparentdirsector = newentry.firstsector;
		newfilename = oldfilename;
	}

	/* Test if the new parent directory is valid */

	if (newparentdirsector != 0xFFFF) {
		/* We can move to the given parent directory */

		mode = oldentry.flags & SMARTFS_DIRENT_MODE;
		type = oldentry.flags & SMARTFS_DIRENT_TYPE;
#ifdef CONFIG_SMARTFS_JOURNALING
		ret = smartfs_create_journalentry(fs, T_RENAME, newparentdirsector, oldentry.doffset, fs->fs_llformat.namesize, oldentry.dsector, 0, (uint8_t *)newfilename, &t_sector, &t_offset);
		if (ret != OK) {
			fdbg("Journal entry creation failed.\n");
			goto errout_with_semaphore;
		}
#endif
		ret = smartfs_createentry(fs, newparentdirsector, newfilename, type, mode, &newentry, oldentry.firstsector, NULL);
		if (ret != OK) {
			goto errout_with_semaphore;
		}

		/* Now mark the old entry as inactive */

		readwrite.logsector = oldentry.dsector;
		readwrite.offset = 0;
		readwrite.count = fs->fs_llformat.availbytes;
		readwrite.buffer = (uint8_t *)fs->fs_rwbuffer;
		ret = FS_IOCTL(fs, BIOC_READSECT, (unsigned long)&readwrite);
		if (ret < 0) {
			fdbg("Error %d reading sector %d data\n", ret, oldentry.dsector);
			goto errout_with_semaphore;
		}
		// bug fix for rename
		tmp_pntr = (uint8_t *)&fs->fs_rwbuffer[oldentry.doffset];
		tmp_flag = tmp_pntr[0];
		tmp_flag |= (tmp_pntr[1] << 8);

#if CONFIG_SMARTFS_ERASEDSTATE == 0xFF
		tmp_flag &= ~SMARTFS_DIRENT_ACTIVE;
#else
		tmp_flag |= SMARTFS_DIRENT_ACTIVE;
#endif
		tmp_pntr[0] = (uint8_t)(tmp_flag & 0x00FF);
		tmp_pntr[1] = (uint8_t)((tmp_flag >> 8) & 0x00FF);

		/* Now write the updated flags back to the device */

		readwrite.offset = oldentry.doffset;
		readwrite.count = sizeof(uint16_t);
		readwrite.buffer = (uint8_t *)tmp_pntr;
		ret = FS_IOCTL(fs, BIOC_WRITESECT, (unsigned long)&readwrite);
#ifdef CONFIG_SMARTFS_JOURNALING
		retj = smartfs_finish_journalentry(fs, 0, t_sector, t_offset, T_RENAME);
		if (retj != OK) {
			fdbg("Error finishing transaction\n");
			ret = retj;
			goto errout_with_semaphore;
		}
#endif
		if (ret < 0) {
			fdbg("Error %d writing flag bytes for sector %d\n", ret, readwrite.logsector);
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
	uint16_t parentdirsector;
	const char *filename;

	/* Sanity checks */

	DEBUGASSERT(mountpt && mountpt->i_private);

	/* Get the mountpoint private data from the inode structure */

	fs = mountpt->i_private;

	smartfs_semtake(fs);

	/* Find the directory entry corresponding to relpath */

	entry.name = NULL;
	ret = smartfs_finddirentry(fs, &entry, relpath, &parentdirsector, &filename);
	if (ret < 0) {
		goto errout_with_semaphore;
	}

	/* Initialize the stat structure */

	memset(buf, 0, sizeof(struct stat));
	if (entry.firstsector == fs->fs_rootsector) {
		/* It's directory name of the mount point */

		buf->st_mode = S_IFDIR | S_IROTH | S_IRGRP | S_IRUSR | S_IWOTH | S_IWGRP | S_IWUSR;
	} else {

		buf->st_mode = entry.flags & 0xFFF;
		if ((entry.flags & SMARTFS_DIRENT_TYPE) == SMARTFS_DIRENT_TYPE_DIR) {
			buf->st_mode |= S_IFDIR;
		} else {
			buf->st_mode |= S_IFREG;
		}
	}
	buf->st_size = entry.datlen;
	buf->st_blksize = fs->fs_llformat.availbytes;
	buf->st_blocks = (buf->st_size + buf->st_blksize - 1) / buf->st_blksize;
	buf->st_atime = 0;
	buf->st_ctime = 0;

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
