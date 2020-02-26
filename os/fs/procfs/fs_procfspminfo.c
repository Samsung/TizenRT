/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#include <sys/stat.h>
#include <sys/types.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pm.h>
#include <pm_metrics.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/procfs.h>
#include <tinyara/pm/pm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define PMINFO_LINELEN 256

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct pminfo_file_s {
	struct procfs_file_s base;	/* Base open file structure */
	unsigned int linesize;		/* Number of valid characters in line[] */
	char line[PMINFO_LINELEN];	/* Pre-allocated buffer for formatted lines */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* File system methods */

static int pminfo_open(FAR struct file *filep, FAR const char *relpath, int oflags, mode_t mode);
static int pminfo_close(FAR struct file *filep);
static ssize_t pminfo_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static int pminfo_dup(FAR const struct file *oldp, FAR struct file *newp);
static int pminfo_stat(FAR const char *relpath, FAR struct stat *buf);

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/* See fs_mount.c -- this structure is explicitly externed there.
 * We use the old-fashioned kind of initializers so that this will compile
 * with any compiler.
 */

const struct procfs_operations pminfo_operations = {
	pminfo_open,				/* open */
	pminfo_close,				/* close */
	pminfo_read,				/* read */
	NULL,						/* write */
	pminfo_dup,						/* dup */
	NULL,						/* opendir */
	NULL,						/* closedir */
	NULL,						/* readdir */
	NULL,						/* rewinddir */
	pminfo_stat						/* stat */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pminfo_open
 ****************************************************************************/

static int pminfo_open(FAR struct file *filep, FAR const char *relpath, int oflags, mode_t mode)
{
	FAR struct pminfo_file_s *attr;

	pmvdbg("Open '%s'\n", relpath);

	/* PROCFS is read-only.  Any attempt to open with any kind of write
	 * access is not permitted.
	 *
	 * REVISIT:  Write-able proc files could be quite useful.
	 */

	if ((oflags & O_WRONLY) != 0 || (oflags & O_RDONLY) == 0) {
		pmdbg("ERROR: Only O_RDONLY supported\n");
		return -EACCES;
	}

	/* "pminfo" is the only acceptable value for the relpath */

	if (strncmp(relpath, "pminfo", strlen("pminfo") + 1) != 0) {
		pmdbg("ERROR: relpath is '%s'\n", relpath);
		return -ENOENT;
	}

	/* Allocate a container to hold the file attributes */

	attr = (FAR struct pminfo_file_s *)kmm_zalloc(sizeof(struct pminfo_file_s));
	if (!attr) {
		pmdbg("ERROR: Failed to allocate file attributes\n");
		return -ENOMEM;
	}

	/* Save the attributes as the open-specific state in filep->f_priv */

	filep->f_priv = (FAR void *)attr;
	return OK;
}

/****************************************************************************
 * Name: pminfo_close
 ****************************************************************************/

static int pminfo_close(FAR struct file *filep)
{
	FAR struct pminfo_file_s *attr;

	/* Recover our private data from the struct file instance */

	attr = (FAR struct pminfo_file_s *)filep->f_priv;
	DEBUGASSERT(attr);

	/* Release the file attributes structure */

	kmm_free(attr);
	filep->f_priv = NULL;
	return OK;
}

/****************************************************************************
 * Name: pminfo_read
 ****************************************************************************/

static ssize_t pminfo_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	FAR struct pminfo_file_s *attr;
	FAR struct pm_domain_s *pdom = &g_pmglobals.domain[PM_IDLE_DOMAIN];
	struct pm_statechange_s *node = NULL;
	struct pm_statechange_s *next = NULL;
	struct pm_accumchange_s *ac = NULL;
	size_t remaining;
	size_t linesize = 0;
	size_t copysize = 0;
	size_t totalsize;
	off_t offset = filep->f_pos;

	pmvdbg("buffer=%p buflen=%d\n", buffer, (int)buflen);

	/* Recover our private data from the struct file instance */

	attr = (FAR struct pminfo_file_s *)filep->f_priv;
	DEBUGASSERT(attr);

	remaining = buflen;
	totalsize = 0;

	linesize = snprintf(attr->line, PMINFO_LINELEN, "%d %d %d %d %d ", pdom->state_stay[0], pdom->state_stay[1], pdom->state_stay[2], pdom->state_stay[3], pdom->state);
	offset = filep->f_pos;
	copysize = procfs_memcpy(attr->line, linesize, buffer, remaining, &offset);
	totalsize += copysize;
	buffer += copysize;
	remaining -= copysize;

	for (node = (struct pm_statechange_s *)sq_peek(&pdom->history); node->entry.flink; node = (struct pm_statechange_s *)(node->entry.flink)) {
		next = (struct pm_statechange_s *)(node->entry.flink);

		int driver_cnt = 0;
		for (ac = (struct pm_accumchange_s *)sq_peek(&node->accum_history); ac; ac = (struct pm_accumchange_s *)(ac->entry.flink)) {
			driver_cnt++;
		}

		linesize = snprintf(attr->line, PMINFO_LINELEN, "%d %d %d %s ", driver_cnt, node->state, next->timestamp - node->timestamp, node->wakeup_reason);
		for (ac = (struct pm_accumchange_s *)sq_peek(&node->accum_history); ac; ac = (struct pm_accumchange_s *)(ac->entry.flink)) {
			linesize += snprintf(attr->line + linesize, PMINFO_LINELEN, "%s %d ", ac->name, ac->accum);
		}

		offset = filep->f_pos;
		copysize = procfs_memcpy(attr->line, linesize, buffer, remaining, &offset);
		totalsize += copysize;
		buffer += copysize;
		remaining -= copysize;
	}

	linesize = snprintf(attr->line, PMINFO_LINELEN, "eof");
	offset = filep->f_pos;
	copysize = procfs_memcpy(attr->line, linesize, buffer, remaining, &offset);
	totalsize += copysize;

	return totalsize;
}

/****************************************************************************
 * Name: pminfo_dup
 *
 * Description:
 *   Duplicate open file data in the new file structure.
 *
 ****************************************************************************/

static int pminfo_dup(FAR const struct file *oldp, FAR struct file *newp)
{
	FAR struct pminfo_file_s *oldattr;
	FAR struct pminfo_file_s *newattr;

	pmvdbg("Dup %p->%p\n", oldp, newp);

	/* Recover our private data from the old struct file instance */

	oldattr = (FAR struct pminfo_file_s *)oldp->f_priv;
	DEBUGASSERT(oldattr);

	/* Allocate a new container to hold the task and attribute selection */

	newattr = (FAR struct pminfo_file_s *)kmm_malloc(sizeof(struct pminfo_file_s));
	if (!newattr) {
		pmdbg("ERROR: Failed to allocate file attributes\n");
		return -ENOMEM;
	}

	/* The copy the file attributes from the old attributes to the new */

	memcpy(newattr, oldattr, sizeof(struct pminfo_file_s));

	/* Save the new attributes in the new file structure */

	newp->f_priv = (FAR void *)newattr;
	return OK;
}

/****************************************************************************
 * Name: pminfo_stat
 *
 * Description: Return information about a file or directory
 *
 ****************************************************************************/

static int pminfo_stat(const char *relpath, struct stat *buf)
{
	/* "pminfo" is the only acceptable value for the relpath */

	if (strcmp(relpath, "pminfo") != 0) {
		pmdbg("ERROR: relpath is '%s'\n", relpath);
		return -ENOENT;
	}

	/* "pminfo" is the name for a read-only file */

	buf->st_mode = S_IFREG | S_IROTH | S_IRGRP | S_IRUSR;
	buf->st_size = 0;
	buf->st_blksize = 0;
	buf->st_blocks = 0;
	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
