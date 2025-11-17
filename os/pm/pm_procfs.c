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

// Forward declaration
struct power_path_template_s;
struct power_file_s;

/* Template structure for a node in the procfs path */
struct power_path_template_s {
	const char *name;
	uint8_t dtype;
	void (*read)(FAR struct power_file_s *priv, void (*readprint)(const char *, ...));
	int (*readdir)(struct fs_dirent_s *dir);
	const struct power_path_template_s *children;
};

/* This enumeration identifies all of the task/thread nodes that can be
 * accessed via the procfs file system.
 */

struct power_path_priv_s {
	const struct power_path_template_s *template; /* Matched template node */
	struct pm_domain_s *domain_ptr;               /* Pointer to the domain structure if applicable */
};

struct power_dir_s {
	struct procfs_dir_priv_s base;
	struct power_path_priv_s path_priv;
	FAR dq_entry_t *domain_position;                /* For efficient domain dir reading */
};

/* This structure describes one open "file" */
struct power_file_s {
	struct procfs_file_s base;     /* Base open file structure */
	struct power_path_priv_s path_priv;
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

/* Directory-specific readdir implementations */
static int power_readdir_common(struct fs_dirent_s *dir);
static int power_readdir_domains(struct fs_dirent_s *dir);

/* File-specific read implementations */
static void power_read_domains_info(FAR struct power_file_s *priv, void (*readprint)(const char *, ...));
static void power_read_dynamic_domain_info(FAR struct power_file_s *priv, void (*readprint)(const char *, ...));
static void power_read_state(FAR struct power_file_s *priv, void (*readprint)(const char *, ...));

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Static Path Template Tree */
/* This data structure declaratively defines the procfs layout using full paths. */

static const struct power_path_template_s g_power_domain_dynamic_path_list[] = {
	{
		.name       = "info",      /* "/proc/power/domains/* /info" */
		.dtype      = DTYPE_FILE,
		.read       = power_read_dynamic_domain_info,
		.readdir    = NULL,
		.children   = NULL
	},
	{ .name = NULL } /* Sentinel */
};

static const struct power_path_template_s g_power_domains_path_list[] = {
	{
		.name       = "info",      /* "/proc/power/domains/info" */
		.dtype      = DTYPE_FILE,
		.read       = power_read_domains_info,
		.readdir    = NULL,
		.children   = NULL
	},
	{
		.name       = "*",        /* "/proc/power/domains/*" */
		.dtype      = DTYPE_DIRECTORY,
		.read       = NULL,
		.readdir    = power_readdir_common,
		.children   = g_power_domain_dynamic_path_list,
	},
	{ .name = NULL } /* Sentinel */
};

static const struct power_path_template_s g_power_path_list[] = {
	{
		.name       = "domains",   /* "/proc/power/domains" */
		.dtype      = DTYPE_DIRECTORY,
		.read       = NULL,
		.readdir    = power_readdir_domains,
		.children   = g_power_domains_path_list
	},
	{
		.name       = "state",    /* "/proc/power/state" */
		.dtype      = DTYPE_FILE,
		.read       = power_read_state,
		.readdir    = NULL,
		.children   = NULL,
	},
	{ .name = NULL } /* Sentinel */
};

const struct power_path_template_s g_power_path = {
	.name       = "power",        /* "/proc/power" */
	.dtype      = DTYPE_DIRECTORY,
	.read       = NULL,
	.readdir    = power_readdir_common,
	.children   = g_power_path_list,
};

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

static void power_read_dynamic_domain_info(FAR struct power_file_s *priv, void (*readprint)(const char *, ...))
{
	FAR struct pm_domain_s *domain = priv->path_priv.domain_ptr;

	if (domain) {
		readprint("%-15s : %s\n", "Domain Name", domain->name);
		readprint("%-15s : %d\n", "Suspend Count", domain->suspend_count);
	}
}

static void power_read_domains_info(FAR struct power_file_s *priv, void (*readprint)(const char *, ...))
{
	FAR struct pm_domain_s *domain;
	FAR dq_entry_t *entry;

	readprint(" %-33s | %14s \n", "DOMAIN NAME", "SUSPEND COUNTS");
	readprint("-----------------------------------|----------------\n");
	for (entry = dq_peek(&g_pmglobals.domains); entry != NULL; entry = dq_next(entry)) {
		domain = (FAR struct pm_domain_s *)entry;
		readprint(" %33s | %14d \n", domain->name, domain->suspend_count);
	}
}

static void power_read_state(FAR struct power_file_s *priv, void (*readprint)(const char *, ...))
{
	enum pm_state_e pm_state;
	for (pm_state = PM_NORMAL; pm_state < PM_COUNT; pm_state++) {
		readprint("%s %s\n", (pm_state == g_pmglobals.state) ? "*" : " ", pm_state_name[pm_state]);
	}
}
/****************************************************************************
 * Name: power_find_best_match
 *
 * Description:
 * Recursively traverses the template tree to find the most specific
 * template that matches the given relative path.
 *
 ****************************************************************************/

static struct power_path_template_s *power_find_best_match(const char *relpath, const struct power_path_template_s *templates, struct power_path_priv_s *path_priv, struct procfs_dir_priv_s *proc_dir)
{
	irqstate_t flags;
	char domain_name[CONFIG_PM_DOMAIN_NAME_SIZE];
	struct power_path_template_s *best_match = NULL;
	struct power_path_template_s *curr_level_candidate = NULL;
	struct power_path_template_s *child_matched_candidate = NULL;

	/* Validate input parameters */
	if (!relpath || !templates || !path_priv) {
		return NULL;
	}

	size_t relpath_len = strlen(relpath);

	/* Iterate through all templates at the current level */
	for (struct power_path_template_s *current = templates; current && current->name; current++) {
		FAR const struct power_path_template_s *candidate = NULL;
		size_t domain_len;

		/* Check for a match with the current template */
		const char *wildcard = strstr(current->name, "*");
		if (wildcard) {

			/* Dynamic path: extract the domain name before the first '/' */
			const char *slash_pos = strchr(relpath, '/');
			/* Calculate the length of the domain name */
			domain_len = slash_pos ? slash_pos - relpath : relpath_len;

			strncpy(domain_name, relpath, domain_len);
			domain_name[domain_len] = '\0'; /* Ensure null termination */

			/* Validate domain name length */
			if (domain_len > 0 && domain_len < CONFIG_PM_DOMAIN_NAME_SIZE) {
				flags = enter_critical_section();
				path_priv->domain_ptr = NULL;
				for (dq_entry_t *entry = dq_peek(&g_pmglobals.domains); entry != NULL; entry = dq_next(entry)) {
					struct pm_domain_s *domain = (struct pm_domain_s *)entry;
					if (strcmp(domain->name, domain_name) == 0) {
						path_priv->domain_ptr = domain;
						break;
					}
				}
				leave_critical_section(flags);

				/* If the domain name from path is not found, it's not a valid match */
				if (path_priv->domain_ptr) {
					candidate = current;
				}
			}

		} else {
			uint16_t length = strlen(current->name);
			/* Static path: use strcmp for an exact match */
			if (strncmp(relpath, current->name, length) == 0) {
				candidate = current;
			}
		}

		/* Is this the best match we've found so far? (Longest pattern wins) */
		if (candidate && (!curr_level_candidate || strlen(candidate->name) > strlen(curr_level_candidate->name))) {

			curr_level_candidate = candidate;

			/* We have a match. Check if a more specific match exists in its children. */
			const char *slash_pos = strchr(relpath, '/');
			if (candidate->children && slash_pos && slash_pos[0] != '\0') {
				child_matched_candidate = power_find_best_match(slash_pos + 1, candidate->children, path_priv, proc_dir);
				candidate = child_matched_candidate;
			}

			best_match = candidate;
		}
	}

	if (best_match && proc_dir) {
		/* Increse path depth level */
		proc_dir->level++;

		if (!child_matched_candidate) {
			/* If this level is highest, Set the nentries */
			proc_dir->nentries = 0;

			for (struct power_path_template_s *current = best_match->children; current && current->name; current++) {
				const char *wildcard = strstr(current->name, "%s");
				if (wildcard) {
					proc_dir->nentries += g_pmglobals.ndomains;
				} else {
					proc_dir->nentries++;
				}
			}
		}
	}

	return best_match;
}

/****************************************************************************
 * Name: power_find_dirref
 *
 * Description:
 * Analyse relpath to find the directory reference entry it represents.
 *
 ****************************************************************************/
static int power_find_dirref(const char *relpath, struct power_path_priv_s *path_priv, struct procfs_dir_priv_s *proc_dir)
{
	FAR const struct power_path_template_s *match = NULL;

	path_priv->template = NULL;
	path_priv->domain_ptr = NULL;

	if (proc_dir) {
		/* init dir info value to default root */
		proc_dir->level = 1;
		proc_dir->index = 0;
		proc_dir->nentries = 2;
	}

	/* Handle root path separately */
	if (strcmp(relpath, "power") == 0 || strcmp(relpath, "power/") == 0) {
		match = &g_power_path;
	} else {
		/* Search the tree for the best matching path template */
		match = power_find_best_match(relpath + strlen("power/"), g_power_path.children, path_priv, proc_dir);
	}

	if (match) {
		path_priv->template = match;
		return OK;
	}

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
	ret = power_find_dirref(relpath, &priv->path_priv, NULL);
	if (ret == -ENOENT) {
		/* Entry not found */
		kmm_free(priv);
		return ret;
	}

	/* We only support opening files */
	if (priv->path_priv.template->dtype != DTYPE_FILE) {
		kmm_free(priv);
		return -EISDIR;
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
	/* Check if a read method is available for this file type */
	if (!priv || !priv->path_priv.template || !priv->path_priv.template->read) {
		return -EINVAL;
	}

	totalsize = 0;

	last_read = priv->offset;

	/* Generate the full content into a temporary buffer */
	priv->path_priv.template->read(priv, readprint);

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
	ret = power_find_dirref(relpath, &powerdir->path_priv, &powerdir->base);
	if (ret == OK) {
		if (powerdir->path_priv.template->dtype == DTYPE_DIRECTORY) {
			dir->u.procfs = (FAR void *)powerdir;
		} else {
			/* Not a directory */
			kmm_free(powerdir);
			ret = -ENOTDIR;
		}
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
 * Name: power_readdir_common
 ****************************************************************************/
static int power_readdir_common(struct fs_dirent_s *dir)
{
	FAR struct power_dir_s *powerdir = dir->u.procfs;
	const struct power_path_template_s *child;
	
	if (powerdir && powerdir->path_priv.template) {
		child = &powerdir->path_priv.template->children[powerdir->base.index];

		if (child && child->name) {
			/* Extract basename from full path for directory entry */
			const char *name_start = strrchr(child->name, '/');
			name_start = (name_start) ? name_start + 1 : child->name;
			
			dir->fd_dir.d_type = child->dtype;
			snprintf(dir->fd_dir.d_name, sizeof(dir->fd_dir.d_name), "%s", name_start);

			powerdir->base.index++;
			return OK;
		}
	}

	return -ENOENT; /* End of directory */
}

/****************************************************************************
 * Name: power_readdir_domains
 ****************************************************************************/
static int power_readdir_domains(struct fs_dirent_s *dir)
{
	FAR struct power_dir_s *powerdir = dir->u.procfs;
	irqstate_t flags;

	/* First, list static entries from the template. */
	if (powerdir->base.index == 0) {
		dir->fd_dir.d_type = DTYPE_FILE;
		snprintf(dir->fd_dir.d_name, sizeof(dir->fd_dir.d_name), "info");
		powerdir->base.index++;
		return OK;
	}

	flags = enter_critical_section();

	/* On first dynamic read, initialize the entry pointer */
	if (powerdir->base.index == 1 && powerdir->domain_position == NULL) {
		powerdir->domain_position = dq_peek(&g_pmglobals.domains);
	}

	/* Read through dynamic domain entries */
	if (powerdir->domain_position) {
		FAR struct pm_domain_s *domain = (FAR struct pm_domain_s *)powerdir->domain_position;
		dir->fd_dir.d_type = DTYPE_DIRECTORY;
		snprintf(dir->fd_dir.d_name, sizeof(dir->fd_dir.d_name), "%s", domain->name);
		
		powerdir->domain_position = dq_next(powerdir->domain_position);
		powerdir->base.index++;
		leave_critical_section(flags);
		return OK;
	}
	
	leave_critical_section(flags);
	return -ENOENT; /* End of directory */
}


/****************************************************************************
 * Name: power_readdir
 ****************************************************************************/
static int power_readdir(struct fs_dirent_s *dir)
{
	FAR struct power_dir_s *powerdir;

	DEBUGASSERT(dir && dir->u.procfs);
	powerdir = dir->u.procfs;

	if (powerdir && powerdir->path_priv.template && powerdir->path_priv.template->readdir) {
		return powerdir->path_priv.template->readdir(dir);
	}

	return -ENOSYS; /* Should not happen, indicates a template misconfiguration */
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
	priv->path_priv.domain_ptr = NULL; /* Reset the domain entry pointer */
	priv->domain_position = NULL;     /* Reset the domain position pointer */

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
	ret = power_find_dirref(relpath, &dir.path_priv, &dir.base);

	if (ret == OK) {
		if (dir.path_priv.template->dtype == DTYPE_DIRECTORY) {
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
