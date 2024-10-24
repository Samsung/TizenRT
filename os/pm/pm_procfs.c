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
 * pm_procfs.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
#include <sys/statfs.h>
#include <sys/stat.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/procfs.h>
#include <tinyara/fs/dirent.h>
#include <tinyara/clock.h>

#include <tinyara/pm/pm.h>

#include "pm.h"

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && defined(CONFIG_FS_PROCFS)
#if defined(CONFIG_PM) && !defined(CONFIG_FS_PROCFS_EXCLUDE_POWER)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* This enumeration identifies all of the task/thread nodes that can be
 * accessed via the procfs file system.
 */

struct power_dir_s {
	struct procfs_dir_priv_s base; /* Base directory private data */
	uint8_t direntry;
	int domain_id;
	uint8_t dtype;
};

/* This structure describes one open "file" */
struct power_file_s {
	struct procfs_file_s base;     /* Base open file structure */
	struct power_dir_s dir;	       /* Reference to item being accessed */
	uint16_t offset;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* File system methods */
static int power_open(FAR struct file *filep, FAR const char *relpath, int oflags, mode_t mode);
static int power_close(FAR struct file *filep);
static ssize_t power_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t power_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);

static int power_dup(FAR const struct file *oldp, FAR struct file *newp);

static int power_opendir(const char *relpath, FAR struct fs_dirent_s *dir);
static int power_closedir(FAR struct fs_dirent_s *dir);
static int power_readdir(FAR struct fs_dirent_s *dir);
static int power_rewinddir(FAR struct fs_dirent_s *dir);

static int power_stat(const char *relpath, FAR struct stat *buf);

/****************************************************************************
 * Private Data
 ****************************************************************************/

#define POWER         "power"
#define POWER_DOMAINS "domains"
#define POWER_STATE   "state"
#define POWER_INFO    "info"

/*
 * Level 1 : /proc/power
 * Level 2 : /proc/power/domains
 * Level 3 : /proc/power/domains/{domain_name}
 * Level 4 : /proc/power/domains/{domain_name}/info
*/

#define POWER_LEVEL_1   1
#define POWER_LEVEL_2   2
#define POWER_LEVEL_3   3
#define POWER_LEVEL_4   4

static const char *pm_state_name[PM_COUNT] = {"PM_NORMAL", "PM_IDLE", "PM_STANDBY", "PM_SLEEP"};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* See include/tinyara/fs/procfs.h
 * We use the old-fashioned kind of initializers so that this will compile
 * with any compiler.
 */

const struct procfs_operations power_procfsoperations = {
	power_open,	     /* open */
	power_close,     /* close */
	power_read,	     /* read */
	power_write,     /* write */

	power_dup,       /* dup */

	power_opendir,	 /* opendir */
	power_closedir,	 /* closedir */
	power_readdir,	 /* readdir */
	power_rewinddir, /* rewinddir */

	power_stat       /* stat */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void power_read_domain_info(int domain_id, void (*readprint)(const char *, ...))
{
	readprint("%-15s : %d\n", "Domain ID", domain_id);
	readprint("%-15s : %s\n", "Domain Name", pm_domain_map[domain_id]);
	readprint("%-15s : %d\n", "Suspend Count", g_pmglobals.suspend_count[domain_id]);
}

static void power_read_domains(void (*readprint)(const char *, ...))
{
	int domain_id;
	readprint(" DOMAIN ID |            DOMAIN NAME            | SUSPEND COUNTS \n");
	readprint("-----------|-----------------------------------|----------------\n");
	for (domain_id = 0; domain_id < g_pmglobals.ndomains; domain_id++) {
		readprint(" %9d | %33s | %14d \n", domain_id, pm_domain_map[domain_id], g_pmglobals.suspend_count[domain_id]);
	}
}

static void power_read_state(void (*readprint)(const char *, ...))
{
	enum pm_state_e pm_state;
	for (pm_state = PM_NORMAL; pm_state < PM_COUNT; pm_state++) {
		readprint("%s %s\n", (pm_state == g_pmglobals.state) ? "*" : " ", pm_state_name[pm_state]);
	}
}
/****************************************************************************
 * Name: power_find_dirref
 *
 * Description:
 *   Analyse relpath to find the directory reference entry it represents,
 *   if any.
 *
 ****************************************************************************/

static int power_find_dirref(FAR const char *relpath, FAR struct power_dir_s *dir)
{
	char *str;
	int domain_id;
	/* Function to check path is starting with given name */
	int checkStart(char *name, bool isDirectory) {
		uint16_t length = strlen(name);
		if (strncmp(str, name, length) != 0) {
			return ERROR;
		}
		str += length;
		if (str[0] == '\0') {
			return OK;
		}
		if (isDirectory && (str[0] == '/')) {
			str++;
			return OK;
		}
		return ERROR;
	}
	str = (char *)relpath;
	dir->domain_id = -1;
	/* Check relpath has "power" mount point */
	if (checkStart(POWER, true) == OK) {
		if (str[0] == '\0') {
			dir->base.level = POWER_LEVEL_1;
			dir->base.index = 0;
			dir->base.nentries = 2;
			dir->dtype = DTYPE_DIRECTORY;
			return OK;
		}
		/* Check relpath has "domains" mount point */
		if (checkStart(POWER_DOMAINS, true) == OK) {
			if (str[0] == '\0') {
				dir->base.level = POWER_LEVEL_2;
				dir->base.index = 0;
				dir->base.nentries = g_pmglobals.ndomains + 1;
				dir->dtype = DTYPE_DIRECTORY;
				return OK;
			}
			/* Iterate over each domain_name till you find match */
			for (domain_id = 0; domain_id < g_pmglobals.ndomains; domain_id++) {
				if (checkStart(pm_domain_map[domain_id], true) == OK) {
					dir->domain_id = domain_id;
					if (str[0] == '\0') {
						dir->base.level = POWER_LEVEL_3;
						dir->base.index = 0;
						dir->base.nentries = 1;
						dir->dtype = DTYPE_DIRECTORY;	
						return OK;
					}
					break;
				}
			}
			/* Check relpath has "info" mount point */
			if (checkStart(POWER_INFO, false) == OK) {
				dir->base.level = (dir->domain_id != -1) ? POWER_LEVEL_4 : POWER_LEVEL_3;
				dir->base.index = 0;
				dir->base.nentries = 0;
				dir->dtype = DTYPE_FILE;
				return OK;
			}
		/* Check relpath has "state" mount point */
		} else if (checkStart(POWER_STATE, false) == OK) {
			dir->base.level = POWER_LEVEL_2;
			dir->base.index = 0;
			dir->base.nentries = 0;
			dir->dtype = DTYPE_FILE;
			return OK;
		}
	}
	fdbg("Invalid Path : Failed to find path %s \n", relpath);
	return -ENOENT;
}

/****************************************************************************
 * Name: power_open
 ****************************************************************************/

static int power_open(FAR struct file *filep, FAR const char *relpath, int oflags, mode_t mode)
{
	FAR struct power_file_s *priv;
	int ret;

	fvdbg("Open '%s'\n", relpath);

	/* Allocate a container to hold the task and attribute selection */
	priv = (FAR struct power_file_s *)kmm_malloc(sizeof(struct power_file_s));
	if (!priv) {
		fdbg("ERROR: Failed to allocate file attributes\n");
		return -ENOMEM;
	}

	/* Find the directory entry being opened */
	ret = power_find_dirref(relpath, &priv->dir);
	if (ret == -ENOENT) {
		/* Entry not found */

		kmm_free(priv);
		return ret;
	}

	priv->offset = 0;

	/* Save the index as the open-specific state in filep->f_priv */
	filep->f_priv = (FAR void *)priv;

	return OK;
}

/****************************************************************************
 * Name: power_close
 ****************************************************************************/

static int power_close(FAR struct file *filep)
{
	FAR struct power_file_s *powerfile;

	/* Recover our private data from the struct file instance */
	powerfile = (FAR struct power_file_s *)filep->f_priv;
	DEBUGASSERT(powerfile);

	/* Release the file container structure */

	kmm_free(powerfile);
	filep->f_priv = NULL;

	return OK;
}

/****************************************************************************
 * Name: power_read
 ****************************************************************************/

static ssize_t power_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	FAR struct power_file_s *priv;
	size_t totalsize;
	int domain_id;
	int last_read;
	/* Function to copy domain information into buffer */
	void readprint(const char *format, ...) {
		size_t copysize;
		va_list args;
		va_start(args, format);
		copysize = vsnprintf(buffer, buflen, format, args);
		va_end(args);
		/* Take min of buflen and copysize */
		if (buflen < copysize) {
			copysize = buflen - 1;
		}
		/* No need to copy information if we have already read information earlier */
		if (copysize <= last_read) {
			last_read -= copysize;
			return;
		}
		/* Overwrite the last read information */
		if (last_read) {
			copysize -= last_read;
			strncpy(buffer, buffer + last_read, copysize);
			last_read = 0;
		}
		/* Increment the buffer pointer */
		buflen -= copysize;
		buffer += copysize;
		totalsize += copysize;
		priv->offset += copysize;
	}

	priv = (FAR struct power_file_s *)filep->f_priv;
	domain_id = priv->dir.domain_id;
	totalsize = 0;
	last_read = priv->offset;
	/* Read the content of "{domain_name}/info" */
	if ((priv->dir.base.level == POWER_LEVEL_4) && (priv->dir.base.index == 0) && (domain_id >= 0)) {
		power_read_domain_info(domain_id, readprint);
	/* Read the content of "domains/info" */
	} else if ((priv->dir.base.level == POWER_LEVEL_3) && (priv->dir.base.index == 0)) {
		power_read_domains(readprint);
	/* Read the content of "power/state" */
	} else if ((priv->dir.base.level == POWER_LEVEL_2) && (priv->dir.base.index == 0)) {
		power_read_state(readprint);
	}
	/* Indicate we have already provided all the data */
	filep->f_pos += totalsize;
	return totalsize;
}

/****************************************************************************
 * Name: power_write
 ****************************************************************************/

static ssize_t power_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	FAR struct power_file_s *priv;
	ssize_t ret;

	fvdbg("buffer=%p buflen=%d\n", buffer, (int)buflen);

	/* Recover our private data from the struct file instance */

	priv = (FAR struct power_file_s *)filep->f_priv;
	DEBUGASSERT(priv);

	/* pm_procfs does not have write operation, one must code write logic here */

	ret = 0;

	/* Update the file offset */
	if (ret > 0) {
		filep->f_pos += ret;
	}

	return ret;
}

/****************************************************************************
 * Name: power_dup
 *
 * Description:
 *   Duplicate open file data in the new file structure.
 *
 ****************************************************************************/

static int power_dup(FAR const struct file *oldp, FAR struct file *newp)
{
	FAR struct power_file_s *oldfile;
	FAR struct power_file_s *newfile;

	fvdbg("Dup %p->%p\n", oldp, newp);

	/* Recover our private data from the old struct file instance */

	oldfile = (FAR struct power_file_s *)oldp->f_priv;
	DEBUGASSERT(oldfile);

	/* Allocate a new container to hold the task and node selection */

	newfile = (FAR struct power_file_s *)kmm_malloc(sizeof(struct power_file_s));
	if (!newfile) {
		fdbg("ERROR: Failed to allocate file container\n");
		return -ENOMEM;
	}

	/* The copy the file information from the old container to the new */

	memcpy(newfile, oldfile, sizeof(struct power_file_s));

	/* Save the new container in the new file structure */

	newp->f_priv = (FAR void *)newfile;
	return OK;
}

/****************************************************************************
 * Name: power_opendir
 *
 * Description:
 *   Open a directory for read access
 *
 ****************************************************************************/

static int power_opendir(FAR const char *relpath, FAR struct fs_dirent_s *dir)
{
	FAR struct power_dir_s *powerdir;
	int ret;

	fvdbg("relpath: \"%s\"\n", relpath ? relpath : "NULL");
	DEBUGASSERT(relpath && dir && !dir->u.procfs);

	/* The path refers to the 1st level subdirectory.  Allocate the powerdir
	 * dirent structure.
	 */
	powerdir = (FAR struct power_dir_s *)kmm_malloc(sizeof(struct power_dir_s));

	if (!powerdir) {
		fdbg("ERROR: Failed to allocate the directory structure\n");
		return -ENOMEM;
	}

	/* Initialize base structure components */
	ret = power_find_dirref(relpath, powerdir);

	if (ret == OK) {
		dir->u.procfs = (FAR void *)powerdir;
	} else {
		kmm_free(powerdir);
	}

	return ret;
}

/****************************************************************************
 * Name: power_closedir
 *
 * Description: Close the directory listing
 *
 ****************************************************************************/

static int power_closedir(FAR struct fs_dirent_s *dir)
{
	FAR struct power_dir_s *priv;

	DEBUGASSERT(dir && dir->u.procfs);
	priv = dir->u.procfs;

	if (priv) {
		kmm_free(priv);
	}

	dir->u.procfs = NULL;

	return OK;
}

/****************************************************************************
 * Name: power_readdir
 *
 * Description: Read the next directory entry
 *
 ****************************************************************************/

static int power_readdir(struct fs_dirent_s *dir)
{
	FAR struct power_dir_s *powerdir;
	int index;

	DEBUGASSERT(dir && dir->u.procfs);
	powerdir = dir->u.procfs;

	/* Have we reached the end of the directory */
	index = powerdir->base.index;
	if (index >= powerdir->base.nentries) {
		return -ENOENT;
	}
	switch (powerdir->base.level) {
		/* List the content of "power" directory */
	case POWER_LEVEL_1:
		switch (index) {
		case 0:
			dir->fd_dir.d_type = DTYPE_DIRECTORY;
			snprintf(dir->fd_dir.d_name, sizeof(dir->fd_dir.d_name), POWER_DOMAINS);
			powerdir->base.index++;
			break;
		case 1:
			dir->fd_dir.d_type = DTYPE_FILE;
			snprintf(dir->fd_dir.d_name, sizeof(dir->fd_dir.d_name), POWER_STATE);
			powerdir->base.index++;
			break;
		}
		break;
	/* List the content of "domains" directory */
	case POWER_LEVEL_2:
		if (index == g_pmglobals.ndomains) {
			dir->fd_dir.d_type = DTYPE_FILE;
			snprintf(dir->fd_dir.d_name, sizeof(dir->fd_dir.d_name), POWER_INFO);
		} else {
			dir->fd_dir.d_type = DTYPE_DIRECTORY;
			snprintf(dir->fd_dir.d_name, sizeof(dir->fd_dir.d_name), pm_domain_map[index]);
		}
		powerdir->base.index++;
		break;
	/* List the content of "{domain_name}" directory*/
	case POWER_LEVEL_3:
		dir->fd_dir.d_type = DTYPE_FILE;
		snprintf(dir->fd_dir.d_name, sizeof(dir->fd_dir.d_name), POWER_INFO);
		powerdir->base.index++;
		break;
	default:
		fdbg("Invalid directory level\n");
		return -ENOENT;
	}
	return OK;
}

/****************************************************************************
 * Name: power_rewindir
 *
 * Description: Reset directory read to the first entry
 *
 ****************************************************************************/

static int power_rewinddir(struct fs_dirent_s *dir)
{
	FAR struct power_dir_s *priv;

	DEBUGASSERT(dir && dir->u.procfs);
	priv = dir->u.procfs;

	priv->base.index = 0;
	return OK;
}

/****************************************************************************
 * Name: power_stat
 *
 * Description: Return information about a file or directory
 *
 ****************************************************************************/

static int power_stat(const char *relpath, struct stat *buf)
{
	int ret;
	struct power_dir_s dir;

	/* Decide if the relpath is valid and if it is a file
	 *      or a directory and set it's permissions.
	 */

	ret = power_find_dirref(relpath, &dir);

	if (ret == OK) {
		if (dir.dtype == DTYPE_DIRECTORY) {
			/* This is a directory */
			buf->st_mode = S_IFDIR | S_IROTH | S_IRGRP | S_IRUSR;
		} else {
			/* This is a file */
			buf->st_mode = S_IFREG | S_IROTH | S_IRGRP | S_IRUSR;
		}
	}

	/* File/directory size, access block size */

	buf->st_size = 0;
	buf->st_blksize = 0;
	buf->st_blocks = 0;

	return ret;
}
#endif /* !CONFIG_DISABLE_MOUNTPOINT && CONFIG_FS_PROCFS */
#endif /* CONFIG_PM && !CONFIG_FS_PROCFS_EXCLUDE_POWER */
