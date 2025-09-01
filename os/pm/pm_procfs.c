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
	FAR struct pm_domain_s *domain_ptr; /* Pointer to the domain structure */
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

static void power_read_domain_info(FAR struct pm_domain_s *domain, void (*readprint)(const char *, ...))
{
	if (domain) {
		readprint("%-15s : %s\n", "Domain Name", domain->name);
		readprint("%-15s : %d\n", "Suspend Count", domain->suspend_count);
	}
}

static void power_read_domains(void (*readprint)(const char *, ...))
{
	FAR struct pm_domain_s *domain;
	FAR dq_entry_t *entry;
	int index = 0;

	readprint(" %-33s | %14s \n", "DOMAIN NAME", "SUSPEND COUNTS");
	readprint("-----------------------------------|----------------\n");
	for (entry = dq_peek(&g_pmglobals.domains); entry != NULL; entry = dq_next(entry)) {
		domain = (FAR struct pm_domain_s *)entry;
		readprint(" %33s | %14d \n", domain->name, domain->suspend_count);
		index++;
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
	const char *str;
	FAR struct pm_domain_s *domain;
	FAR dq_entry_t *entry;
	irqstate_t flags;

	/* Function to check path is starting with given name */
	int checkStart(const char *name, bool isDirectory) {
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

	str = relpath;
	dir->domain_ptr = NULL;

	flags = enter_critical_section(); /* Protect domain list access */

	/* Check relpath has "power" mount point */
	if (checkStart(POWER, true) == OK) {
		if (str[0] == '\0') {
			dir->base.level = POWER_LEVEL_1;
			dir->base.index = 0;
			dir->base.nentries = 2;
			dir->dtype = DTYPE_DIRECTORY;
			leave_critical_section(flags);
			return OK;
		}
		/* Check relpath has "domains" mount point */
		if (checkStart(POWER_DOMAINS, true) == OK) {
			if (str[0] == '\0') {
				dir->base.level = POWER_LEVEL_2;
				dir->base.index = 0;
				dir->base.nentries = g_pmglobals.ndomains + 1; /* +1 for "info" file */
				dir->dtype = DTYPE_DIRECTORY;
				leave_critical_section(flags);
				return OK;
			}
			/* Iterate over each domain_name till you find match */
			for (entry = dq_peek(&g_pmglobals.domains); entry != NULL; entry = dq_next(entry)) {
				domain = (FAR struct pm_domain_s *)entry;
				if (checkStart(domain->name, true) == OK) {
					dir->domain_ptr = domain;
					if (str[0] == '\0') {
						dir->base.level = POWER_LEVEL_3;
						dir->base.index = 0;
						dir->base.nentries = 1; /* Only "info" file */
						dir->dtype = DTYPE_DIRECTORY;
						leave_critical_section(flags);
						return OK;
					}
					break; /* Found domain, break to check for "info" */
				}
			}
			/* Check relpath has "info" mount point (can be under domains or a specific domain) */
			if (checkStart(POWER_INFO, false) == OK) {
				if (dir->domain_ptr) {
					/* Info for a specific domain */
					dir->base.level = POWER_LEVEL_4;
				} else {
					/* General domains info */
					dir->base.level = POWER_LEVEL_3;
				}
				dir->base.index = 0;
				dir->base.nentries = 0;
				dir->dtype = DTYPE_FILE;
				leave_critical_section(flags);
				return OK;
			}
		/* Check relpath has "state" mount point */
		} else if (checkStart(POWER_STATE, false) == OK) {
			dir->base.level = POWER_LEVEL_2;
			dir->base.index = 0;
			dir->base.nentries = 0;
			dir->dtype = DTYPE_FILE;
			leave_critical_section(flags);
			return OK;
		}
	}
	leave_critical_section(flags);
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
	FAR struct pm_domain_s *domain_ptr;
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
	domain_ptr = priv->dir.domain_ptr;
	totalsize = 0;
	last_read = priv->offset;

	/* Read the content of "{domain_name}/info" */
	if ((priv->dir.base.level == POWER_LEVEL_4) && (priv->dir.base.index == 0) && (domain_ptr != NULL)) {
		power_read_domain_info(domain_ptr, readprint);
	/* Read the content of "domains/info" (general list of domains) */
	} else if ((priv->dir.base.level == POWER_LEVEL_3) && (priv->dir.base.index == 0) && (domain_ptr == NULL)) {
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
	FAR struct pm_domain_s *domain;
	FAR dq_entry_t *entry;
	int index;
	irqstate_t flags;

	DEBUGASSERT(dir && dir->u.procfs);
	powerdir = dir->u.procfs;

	/* Have we reached the end of the directory */
	index = powerdir->base.index;
	if (index >= powerdir->base.nentries) {
		return -ENOENT;
	}

	flags = enter_critical_section(); /* Protect domain list access */

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
		if (index == g_pmglobals.ndomains) { /* Last entry is the "info" file */
			dir->fd_dir.d_type = DTYPE_FILE;
			snprintf(dir->fd_dir.d_name, sizeof(dir->fd_dir.d_name), POWER_INFO);
		} else {
			/* Iterate to the current domain index */
			entry = dq_peek(&g_pmglobals.domains);
			for (int i = 0; i < index && entry != NULL; i++) {
				entry = dq_next(entry);
			}
			if (entry) {
				domain = (FAR struct pm_domain_s *)entry;
				dir->fd_dir.d_type = DTYPE_DIRECTORY;
				snprintf(dir->fd_dir.d_name, sizeof(dir->fd_dir.d_name), "%s", domain->name);
			} else {
				/* Should not happen if nentries is correct */
				leave_critical_section(flags);
				return -ENOENT;
			}
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
		leave_critical_section(flags);
		return -ENOENT;
	}
	leave_critical_section(flags);
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
