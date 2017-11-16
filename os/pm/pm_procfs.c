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

#include <tinyara/pm/pm.h>

#include "pm.h"
#ifdef CONFIG_PM_METRICS
#include "pm_metrics.h"
#endif

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
	struct procfs_dir_priv_s base;	/* Base directory private data */
	uint8_t direntry;
	int domain;					/* Domain of registered device driver for PM */
};

/* This structure describes one open "file" */
struct power_file_s {
	struct procfs_file_s base;	/* Base open file structure */
	struct power_dir_s dir;		/* Reference to item being accessed */
	uint16_t offset;
};

struct power_procfs_entry_s {
	const char *name;			/* Name of the directory entry */
	size_t(*read)(FAR struct file *filep, FAR char *buffer, size_t buflen);
	uint8_t type;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* File system methods */
static int power_open(FAR struct file *filep, FAR const char *relpath, int oflags, mode_t mode);
static int power_close(FAR struct file *filep);
static ssize_t power_read(FAR struct file *filep, FAR char *buffer, size_t buflen);

static int power_dup(FAR const struct file *oldp, FAR struct file *newp);

static int power_opendir(const char *relpath, FAR struct fs_dirent_s *dir);
static int power_closedir(FAR struct fs_dirent_s *dir);
static int power_readdir(FAR struct fs_dirent_s *dir);
static int power_rewinddir(FAR struct fs_dirent_s *dir);

static int power_stat(const char *relpath, FAR struct stat *buf);

static size_t power_states_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static size_t power_curstate_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
#ifdef CONFIG_PM_METRICS
static size_t power_metrics_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
#endif
static size_t power_devices_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
/****************************************************************************
 * Private Data
 ****************************************************************************/

#define POWER_SUBDIRPATH_DOMAINS "power/domains"

static const struct power_procfs_entry_s g_power_direntry[] = {
	{"states", power_states_read, DTYPE_FILE},
	{"curstate", power_curstate_read, DTYPE_FILE},
#ifdef CONFIG_PM_METRICS
	{"metrics", power_metrics_read, DTYPE_FILE},
#endif
	{"devices", power_devices_read, DTYPE_FILE},
};

static const uint8_t g_power_direntrycount = sizeof(g_power_direntry) / sizeof(struct power_procfs_entry_s);

static const char *g_power_states[] = {
	"NORMAL",
	"IDLE",
	"STANDBY",
	"SLEEP",
};

static const uint8_t g_power_statescount = sizeof(g_power_states) / sizeof(g_power_states[0]);

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* See include/tinyara/fs/procfs.h
 * We use the old-fashioned kind of initializers so that this will compile
 * with any compiler.
 */

const struct procfs_operations power_procfsoperations = {
	power_open,					/* open */
	power_close,				/* close */
	power_read,					/* read */
	NULL,						/* write */

	power_dup,					/* dup */

	power_opendir,				/* opendir */
	power_closedir,				/* closedir */
	power_readdir,				/* readdir */
	power_rewinddir,			/* rewinddir */

	power_stat					/* stat */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

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
	uint16_t len;
	FAR char *str;
	int domain;
	int i;

	/* Skip the "power/domains" portion of relpath. We accept it only now */
	len = strlen(POWER_SUBDIRPATH_DOMAINS);
	if (strncmp(relpath, POWER_SUBDIRPATH_DOMAINS, len) != 0) {
		fdbg("Invalid Path : Failed to find path %s \n", relpath);
		return -ENOENT;
	}
	relpath += len;

	if (relpath[0] == '/') {
		relpath++;
	}

	if (relpath[0] == '\0') {
		/* Requesting directory listing of registered device drivers for PM */
		dir->base.level = 1;
		dir->base.index = 0;
		dir->base.nentries = CONFIG_PM_NDOMAINS;
		/* Search PM registry to the number of registered device driver */
		return OK;
	}

	str = NULL;
	domain = strtoul(relpath, &str, 10);

	if (!str) {
		fdbg("ERROR: Invalid path \"%s\"\n", relpath);
		return -ENOENT;
	}

	len = str - relpath;

	/* A valid domain would be in the range of 0-(CONFIG_PM_NDOMAINS-1) */
	if (domain < 0 || domain >= CONFIG_PM_NDOMAINS) {
		fdbg("ERROR: Invalid domain %ld\n", domain);
		return -ENOENT;
	}

	/* Save the domain and skip it in the relpath */
	dir->domain = domain;

	relpath += len;

	if (relpath[0] == '/') {
		relpath++;
	}

	if (relpath[0] == '\0') {
		/* Requesting directory listing of a specific device driver or entry */
		dir->base.level = 2;
		dir->base.index = 0;
		dir->base.nentries = g_power_direntrycount;
		dir->direntry = 0;
		return OK;
	}

	/* This is 3 level. Only files exist only in this level. */
	dir->base.level = 3;
	dir->base.nentries = 1;
	dir->base.index = 0;
	dir->direntry = 0;
	for (i = 0; i < g_power_direntrycount; i++) {
		if (strcmp(relpath, g_power_direntry[i].name) == 0) {
			dir->direntry = i;
			return OK;
		}
	}

	return -ENOENT;
}

/****************************************************************************
 * Name: power_states_read
 ****************************************************************************/
static size_t power_states_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	FAR struct power_file_s *priv;
	size_t copysize;
	size_t totalsize;
	int i;

	priv = (FAR struct power_file_s *)filep->f_priv;

	totalsize = 0;

	if (priv->offset == 0) {
		/* PM does not support states for specific domain now. It will be updated. */
		for (i = 0; i < g_power_statescount; i++) {
			copysize = snprintf(buffer, buflen, "%s ", g_power_states[i]);
			buflen -= copysize;
			buffer += copysize;
			totalsize += copysize;
		}

		/* Indicate we have already provided all the data */
		priv->offset = 0xFF;
	}

	return totalsize;
}

/****************************************************************************
 * Name: power_curstate_read
 ****************************************************************************/
static size_t power_curstate_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	FAR struct power_file_s *priv;
	FAR struct pm_domain_s *pdom;
	size_t copysize;
	int domain;

	priv = (FAR struct power_file_s *)filep->f_priv;
	copysize = 0;

	if (priv->offset == 0) {
		domain = priv->dir.domain;

		if (domain >= 0 && domain < CONFIG_PM_NDOMAINS) {
			pdom = &g_pmglobals.domain[domain];

			if (pdom != NULL) {
				copysize = snprintf(buffer, buflen, "%s", g_power_states[pdom->state]);
			}
		}
		/* Indicate we have already provided all the data */
		priv->offset = 0xFF;
	}

	return copysize;
}

#ifdef CONFIG_PM_METRICS
/****************************************************************************
 * Name: power_metrics_read
 ****************************************************************************/
static size_t power_metrics_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	FAR struct power_file_s *priv;
	struct pm_time_in_each_s mtrics;
	size_t copysize;
	size_t totalsize;
	int domain;
	int index;

	priv = (FAR struct power_file_s *)filep->f_priv;

	index = 0;
	copysize = 0;
	totalsize = 0;

	if (priv->offset == 0) {
		domain = priv->dir.domain;

		if (domain >= 0 && domain < CONFIG_PM_NDOMAINS) {
			pm_get_domainmetrics(domain, &mtrics);

			/* Time in NORMAL state */
			copysize = snprintf(buffer, buflen, " Time in %s : %d\n", g_power_states[index++], mtrics.normal);
			buflen -= copysize;
			buffer += copysize;
			totalsize += copysize;

			/* Time in IDLE state */
			copysize = snprintf(buffer, buflen, " Time in %s : %d\n", g_power_states[index++], mtrics.idle);
			buflen -= copysize;
			buffer += copysize;
			totalsize += copysize;

			/* Time in STANDBY state */
			copysize = snprintf(buffer, buflen, " Time in %s : %d\n", g_power_states[index++], mtrics.standby);
			buflen -= copysize;
			buffer += copysize;
			totalsize += copysize;

			/* Time in SLEEP state */
			copysize = snprintf(buffer, buflen, " Time in %s : %d", g_power_states[index], mtrics.sleep);
			buflen -= copysize;
			buffer += copysize;
			totalsize += copysize;
		}
		/* Indicate we have already provided all the data */
		priv->offset = 0xFF;
	}
	return totalsize;
}
#endif

/****************************************************************************
 * Name: power_devices_read
 ****************************************************************************/
static size_t power_devices_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	FAR struct power_file_s *priv;
	FAR struct pm_callback_s *callback;
	FAR sq_entry_t *entry;
	size_t copysize;
	size_t totalsize;
	int domain;

	priv = (FAR struct power_file_s *)filep->f_priv;
	copysize = 0;
	totalsize = 0;

	if (priv->offset == 0) {
		domain = priv->dir.domain;

		if (domain >= 0 && domain < CONFIG_PM_NDOMAINS) {
			entry = sq_peek(&g_pmglobals.domain[domain].registry);
			while (entry) {
				callback = (FAR struct pm_callback_s *)entry;
				copysize = snprintf(buffer, buflen, "%s ", callback->name);
				buflen -= copysize;
				buffer += copysize;
				totalsize += copysize;
				entry = sq_next(entry);
			}
		}

		/* Indicate we have already provided all the data */
		priv->offset = 0xFF;
	}

	return totalsize;
}

/****************************************************************************
 * Name: power_open
 ****************************************************************************/

static int power_open(FAR struct file *filep, FAR const char *relpath, int oflags, mode_t mode)
{
	FAR struct power_file_s *priv;
	int ret;

	fvdbg("Open '%s'\n", relpath);

	/* PROCFS is read-only.  Any attempt to open with any kind of write
	 * access is not permitted.
	 *
	 * REVISIT:  Write-able proc files could be quite useful.
	 */

	if ((oflags & O_WRONLY) != 0 || (oflags & O_RDONLY) == 0) {
		fdbg("ERROR: Only O_RDONLY supported\n");
		return -EACCES;
	}

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
	ssize_t ret;

	fvdbg("buffer=%p buflen=%d\n", buffer, (int)buflen);

	/* Recover our private data from the struct file instance */

	priv = (FAR struct power_file_s *)filep->f_priv;
	DEBUGASSERT(priv);

	/* Perform the read based on the directory entry */

	ret = 0;

	if (priv->dir.base.level == 3 && priv->dir.direntry < g_power_direntrycount) {
		ret = g_power_direntry[priv->dir.direntry].read(filep, buffer, buflen);
	}

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
		fdbg("Entry %d: End of directory\n", index);
		return -ENOENT;
	}

	if (powerdir->base.level < 1 || powerdir->base.level > 3) {
		fdbg("Invalid directory level\n");
		return -ENOENT;
	}

	if (powerdir->base.level == 1) {
		/* Listing the contents of domains */
		if (index >= 0 && index < CONFIG_PM_NDOMAINS) {
			dir->fd_dir.d_type = DTYPE_DIRECTORY;
			snprintf(dir->fd_dir.d_name, sizeof(dir->fd_dir.d_name), "%d", index);
			powerdir->base.index++;
			return OK;
		}
	} else if (powerdir->base.level == 2) {
		/* Listing the contents of a specific domain */
		dir->fd_dir.d_type = g_power_direntry[powerdir->base.index].type;
		strncpy(dir->fd_dir.d_name, g_power_direntry[powerdir->base.index].name, NAME_MAX + 1);
		powerdir->base.index++;
		return OK;
	} else if (powerdir->base.level == 3) {
		fdbg("%s : Not a directory\n", g_power_direntry[index].name);
		return -ENOTDIR;
	}

	return -ENOENT;
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
		if (dir.base.level < 3) {
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
#endif							/* !CONFIG_DISABLE_MOUNTPOINT && CONFIG_FS_PROCFS */
#endif							/* CONFIG_PM && !CONFIG_FS_PROCFS_EXCLUDE_POWER */
