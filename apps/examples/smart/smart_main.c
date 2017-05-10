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
 * examples/smart/smart_main.c
 *
 *   Copyright (C) 2011, 2013 Gregory Nutt. All rights reserved.
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

#include <sys/mount.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <crc32.h>
#include <debug.h>

#include <tinyara/fs/mtd.h>
#include <tinyara/fs/smart.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/mksmartfs.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* The default is to use the RAM MTD device at drivers/mtd/rammtd.c.  But
 * an architecture-specific MTD driver can be used instead by defining
 * CONFIG_EXAMPLES_SMART_ARCHINIT.  In this case, the initialization logic
 * will call smart_archinitialize() to obtain the MTD driver instance.
 */

#ifndef CONFIG_EXAMPLES_SMART_ARCHINIT

/* This must exactly match the default configuration in drivers/mtd/rammtd.c */

#ifndef CONFIG_RAMMTD_ERASESIZE
#define CONFIG_RAMMTD_ERASESIZE 4096
#endif

#ifndef CONFIG_EXAMPLES_SMART_NEBLOCKS
#define CONFIG_EXAMPLES_SMART_NEBLOCKS (256)
#endif

#define EXAMPLES_SMART_BUFSIZE \
	(CONFIG_RAMMTD_ERASESIZE * CONFIG_EXAMPLES_SMART_NEBLOCKS)
#endif

#ifndef CONFIG_EXAMPLES_SMART_MAXNAME
#define CONFIG_EXAMPLES_SMART_MAXNAME 128
#endif

#if CONFIG_EXAMPLES_SMART_MAXNAME > 255
#undef CONFIG_EXAMPLES_SMART_MAXNAME
#define CONFIG_EXAMPLES_SMART_MAXNAME 255
#endif

#ifndef CONFIG_EXAMPLES_SMART_MAXFILE
#define CONFIG_EXAMPLES_SMART_MAXFILE 8192
#endif

#ifndef CONFIG_EXAMPLES_SMART_MAXIO
#define CONFIG_EXAMPLES_SMART_MAXIO 347
#endif

#ifndef CONFIG_EXAMPLES_SMART_MAXOPEN
#define CONFIG_EXAMPLES_SMART_MAXOPEN 512
#endif

#ifndef CONFIG_EXAMPLES_SMART_MOUNTPT
#define CONFIG_EXAMPLES_SMART_MOUNTPT "/mnt"
#endif

#ifndef CONFIG_EXAMPLES_SMART_NLOOPS
#define CONFIG_EXAMPLES_SMART_NLOOPS 100
#endif

#ifndef CONFIG_EXAMPLES_SMART_VERBOSE
#define CONFIG_EXAMPLES_SMART_VERBOSE 0
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct smart_filedesc_s {
	FAR char *name;
	bool deleted;
	size_t len;
	uint32_t crc;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* Pre-allocated simulated flash */

#if 0
#ifndef CONFIG_EXAMPLES_SMART_ARCHINIT
#ifdef CONFIG_RAMMTD
static uint8_t g_simflash[EXAMPLES_SMART_BUFSIZE];
#endif
#endif
#endif

static uint8_t g_fileimage[CONFIG_EXAMPLES_SMART_MAXFILE];
static struct smart_filedesc_s g_files[CONFIG_EXAMPLES_SMART_MAXOPEN];
static const char g_mountdir[] = CONFIG_EXAMPLES_SMART_MOUNTPT "/";
static int g_nfiles;
static int g_ndeleted;

static struct mallinfo g_mmbefore;
static struct mallinfo g_mmprevious;
static struct mallinfo g_mmafter;

/****************************************************************************
 * External Functions
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_SMART_ARCHINIT
extern FAR struct mtd_dev_s *smart_archinitialize(void);
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: smart_memusage
 ****************************************************************************/

static void smart_showmemusage(struct mallinfo *mmbefore, struct mallinfo *mmafter)
{
	printf("VARIABLE  BEFORE   AFTER\n");
	printf("======== ======== ========\n");
	printf("arena    %8x %8x\n", mmbefore->arena, mmafter->arena);
	printf("ordblks  %8d %8d\n", mmbefore->ordblks, mmafter->ordblks);
	printf("mxordblk %8x %8x\n", mmbefore->mxordblk, mmafter->mxordblk);
	printf("uordblks %8x %8x\n", mmbefore->uordblks, mmafter->uordblks);
	printf("fordblks %8x %8x\n", mmbefore->fordblks, mmafter->fordblks);
}

/****************************************************************************
 * Name: smart_loopmemusage
 ****************************************************************************/

static void smart_loopmemusage(void)
{
	/* Get the current memory usage */

#ifdef CONFIG_CAN_PASS_STRUCTS
	g_mmafter = mallinfo();
#else
	(void)mallinfo(&g_mmafter);
#endif

	/* Show the change from the previous loop */

	printf("\nEnd of loop memory usage:\n");
	smart_showmemusage(&g_mmprevious, &g_mmafter);

	/* Set up for the next test */

#ifdef CONFIG_CAN_PASS_STRUCTS
	g_mmprevious = g_mmafter;
#else
	memcpy(&g_mmprevious, &g_mmafter, sizeof(struct mallinfo));
#endif
}

/****************************************************************************
 * Name: smart_endmemusage
 ****************************************************************************/

static void smart_endmemusage(void)
{
#ifdef CONFIG_CAN_PASS_STRUCTS
	g_mmafter = mallinfo();
#else
	(void)mallinfo(&g_mmafter);
#endif
	printf("\nFinal memory usage:\n");
	smart_showmemusage(&g_mmbefore, &g_mmafter);
}

/****************************************************************************
 * Name: smart_randchar
 ****************************************************************************/

static inline char smart_randchar(void)
{
	int value = rand() % 63;
	if (value == 0) {
		return '0';
	} else if (value <= 10) {
		return value + '0' - 1;
	} else if (value <= 36) {
		return value + 'a' - 11;
	} else {					/* if (value <= 62) */

		return value + 'A' - 37;
	}
}

/****************************************************************************
 * Name: smart_randname
 ****************************************************************************/

static inline void smart_randname(FAR struct smart_filedesc_s *file)
{
	int dirlen;
	int maxname;
	int namelen;
	int alloclen;
	int i;

	dirlen = strlen(g_mountdir);
	maxname = CONFIG_EXAMPLES_SMART_MAXNAME - dirlen;
	namelen = (rand() % maxname) + 1;
	alloclen = namelen + dirlen;

	file->name = (FAR char *)malloc(alloclen + 1);
	if (!file->name) {
		printf("ERROR: Failed to allocate name, length=%d\n", namelen);
		fflush(stdout);
		exit(5);
	}

	memcpy(file->name, g_mountdir, dirlen);
	for (i = dirlen; i < alloclen; i++) {
		file->name[i] = smart_randchar();
	}

	file->name[alloclen] = '\0';
}

/****************************************************************************
 * Name: smart_randfile
 ****************************************************************************/

static inline void smart_randfile(FAR struct smart_filedesc_s *file)
{
	int i;

	file->len = (rand() % CONFIG_EXAMPLES_SMART_MAXFILE) + 1;
	for (i = 0; i < file->len; i++) {
		g_fileimage[i] = smart_randchar();
	}

	file->crc = crc32(g_fileimage, file->len);
}

/****************************************************************************
 * Name: smart_freefile
 ****************************************************************************/

static void smart_freefile(FAR struct smart_filedesc_s *file)
{
	if (file->name) {
		free(file->name);
	}

	memset(file, 0, sizeof(struct smart_filedesc_s));
}

/****************************************************************************
 * Name: smart_wrfile
 ****************************************************************************/

static inline int smart_wrfile(FAR struct smart_filedesc_s *file)
{
	size_t offset;
	int fd;
	int ret;

	/* Create a random file */

	smart_randname(file);
	smart_randfile(file);
	fd = open(file->name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	if (fd < 0) {
		/* If it failed because there is no space on the device, then don't
		 * complain.
		 */

		if (errno != ENOSPC) {
			printf("ERROR: Failed to open file for writing: %d\n", errno);
			printf("  File name: %s\n", file->name);
			printf("  File size: %d\n", file->len);
		}

		smart_freefile(file);
		return ERROR;
	}

	/* Write a random amount of data to the file */

	for (offset = 0; offset < file->len;) {
		size_t maxio = (rand() % CONFIG_EXAMPLES_SMART_MAXIO) + 1;
		size_t nbytestowrite = file->len - offset;
		ssize_t nbyteswritten;

		if (nbytestowrite > maxio) {
			nbytestowrite = maxio;
		}

		nbyteswritten = write(fd, &g_fileimage[offset], nbytestowrite);
		if (nbyteswritten < 0) {
			int err = errno;

			/* If the write failed because there is no space on the device,
			 * then don't complain.
			 */

			if (err != ENOSPC) {
				printf("ERROR: Failed to write file: %d\n", err);
				printf("  File name:    %s\n", file->name);
				printf("  File size:    %d\n", file->len);
				printf("  Write offset: %ld\n", (long)offset);
				printf("  Write size:   %ld\n", (long)nbytestowrite);
				ret = ERROR;
			}
			close(fd);

			/* Remove any garbage file that might have been left behind */

			ret = unlink(file->name);
			if (ret < 0) {
				printf("  Failed to remove partial file\n");
			} else {
#if CONFIG_EXAMPLES_SMART_VERBOSE != 0
				printf("  Successfully removed partial file\n");
#endif
			}

			smart_freefile(file);
			return ERROR;
		} else if (nbyteswritten != nbytestowrite) {
			printf("ERROR: Partial write:\n");
			printf("  File name:    %s\n", file->name);
			printf("  File size:    %d\n", file->len);
			printf("  Write offset: %ld\n", (long)offset);
			printf("  Write size:   %ld\n", (long)nbytestowrite);
			printf("  Written:      %ld\n", (long)nbyteswritten);
		}

		offset += nbyteswritten;
	}

	close(fd);
	return OK;
}

/****************************************************************************
 * Name: smart_fillfs
 ****************************************************************************/

static int smart_fillfs(void)
{
	FAR struct smart_filedesc_s *file;
	int ret;
	int i;

	/* Create a file for each unused file structure */

	for (i = 0; i < CONFIG_EXAMPLES_SMART_MAXOPEN; i++) {
		file = &g_files[i];
		if (file->name == NULL) {
			ret = smart_wrfile(file);
			if (ret < 0) {
#if CONFIG_EXAMPLES_SMART_VERBOSE != 0
				printf("ERROR: Failed to write file %d\n", i);
#endif
				return ERROR;
			}
#if CONFIG_EXAMPLES_SMART_VERBOSE != 0
			printf("  Created file %s\n", file->name);
#endif
			g_nfiles++;
		}
	}

	return OK;
}

/****************************************************************************
 * Name: smart_rdblock
 ****************************************************************************/

static ssize_t smart_rdblock(int fd, FAR struct smart_filedesc_s *file, size_t offset, size_t len)
{
	size_t maxio = (rand() % CONFIG_EXAMPLES_SMART_MAXIO) + 1;
	ssize_t nbytesread;

	if (len > maxio) {
		len = maxio;
	}

	nbytesread = read(fd, &g_fileimage[offset], len);
	if (nbytesread < 0) {
		printf("ERROR: Failed to read file: %d\n", errno);
		printf("  File name:    %s\n", file->name);
		printf("  File size:    %d\n", file->len);
		printf("  Read offset:  %ld\n", (long)offset);
		printf("  Read size:    %ld\n", (long)len);
		return ERROR;
	} else if (nbytesread == 0) {
#if 0							/* No... we do this on purpose sometimes */
		printf("ERROR: Unexpected end-of-file:\n");
		printf("  File name:    %s\n", file->name);
		printf("  File size:    %d\n", file->len);
		printf("  Read offset:  %ld\n", (long)offset);
		printf("  Read size:    %ld\n", (long)len);
#endif
		return ERROR;
	} else if (nbytesread != len) {
		printf("ERROR: Partial read:\n");
		printf("  File name:    %s\n", file->name);
		printf("  File size:    %d\n", file->len);
		printf("  Read offset:  %ld\n", (long)offset);
		printf("  Read size:    %ld\n", (long)len);
		printf("  Bytes read:   %ld\n", (long)nbytesread);
	}

	return nbytesread;
}

/****************************************************************************
 * Name: smart_rdfile
 ****************************************************************************/

static inline int smart_rdfile(FAR struct smart_filedesc_s *file)
{
	size_t ntotalread;
	ssize_t nbytesread;
	uint32_t crc;
	int fd;

	/* Open the file for reading */

	fd = open(file->name, O_RDONLY);
	if (fd < 0) {
		if (!file->deleted) {
			printf("ERROR: Failed to open file for reading: %d\n", errno);
			printf("  File name: %s\n", file->name);
			printf("  File size: %d\n", file->len);
		}

		return ERROR;
	}

	/* Read all of the data info the fileimage buffer using random read sizes */

	for (ntotalread = 0; ntotalread < file->len;) {
		nbytesread = smart_rdblock(fd, file, ntotalread, file->len - ntotalread);
		if (nbytesread < 0) {
			close(fd);
			return ERROR;
		}

		ntotalread += nbytesread;
	}

	/* Verify the file image CRC */

	crc = crc32(g_fileimage, file->len);
	if (crc != file->crc) {
		printf("ERROR: Bad CRC: %d vs %d\n", crc, file->crc);
		printf("  File name: %s\n", file->name);
		printf("  File size: %d\n", file->len);
		close(fd);
		return ERROR;
	}

	/* Try reading past the end of the file */

	nbytesread = smart_rdblock(fd, file, ntotalread, 1024);
	if (nbytesread > 0) {
		printf("ERROR: Read past the end of file\n");
		printf("  File name:  %s\n", file->name);
		printf("  File size:  %d\n", file->len);
		printf("  Bytes read: %ld\n", (long)nbytesread);
		close(fd);
		return ERROR;
	}

	close(fd);
	return OK;
}

/****************************************************************************
 * Name: smart_verifyfs
 ****************************************************************************/

static int smart_verifyfs(void)
{
	FAR struct smart_filedesc_s *file;
	int ret;
	int i;

	/* Create a file for each unused file structure */

	for (i = 0; i < CONFIG_EXAMPLES_SMART_MAXOPEN; i++) {
		file = &g_files[i];
		if (file->name != NULL) {
			ret = smart_rdfile(file);
			if (ret < 0) {
				if (file->deleted) {
#if CONFIG_EXAMPLES_SMART_VERBOSE != 0
					printf("Deleted file %d OK\n", i);
#endif
					smart_freefile(file);
					g_ndeleted--;
					g_nfiles--;
				} else {
					printf("ERROR: Failed to read a file: %d\n", i);
					printf("  File name: %s\n", file->name);
					printf("  File size: %d\n", file->len);
					return ERROR;
				}
			} else {
				if (file->deleted) {
#if CONFIG_EXAMPLES_SMART_VERBOSE != 0
					printf("Succesffully read a deleted file\n");
					printf("  File name: %s\n", file->name);
					printf("  File size: %d\n", file->len);
#endif
					smart_freefile(file);
					g_ndeleted--;
					g_nfiles--;
					return ERROR;
				} else {
#if CONFIG_EXAMPLES_SMART_VERBOSE != 0
					printf("  Verifed file %s\n", file->name);
#endif
				}
			}
		}
	}

	return OK;
}

/****************************************************************************
 * Name: smart_delfiles
 ****************************************************************************/

static int smart_delfiles(void)
{
	FAR struct smart_filedesc_s *file;
	int ndel;
	int ret;
	int i;
	int j;

	/* Are there any files to be deleted? */

	int nfiles = g_nfiles - g_ndeleted;
	if (nfiles < 1) {
		return 0;
	}

	/* Yes... How many files should we delete? */

	ndel = (rand() % nfiles) + 1;

	/* Now pick which files to delete */

	for (i = 0; i < ndel; i++) {
		/* Guess a file index */

		int ndx = (rand() % (g_nfiles - g_ndeleted));

		/* And delete the next undeleted file after that random index.  NOTE
		 * that the entry at ndx is not checked.
		 */

		for (j = ndx + 1; j != ndx; j++) {
			/* Test for wrap-around */

			if (j >= CONFIG_EXAMPLES_SMART_MAXOPEN) {
				j = 0;
			}

			file = &g_files[j];
			if (file->name && !file->deleted) {
				ret = unlink(file->name);
				if (ret < 0) {
					printf("ERROR: Unlink %d failed: %d\n", i + 1, errno);
					printf("  File name:  %s\n", file->name);
					printf("  File size:  %d\n", file->len);
					printf("  File index: %d\n", j);
				} else {
#if CONFIG_EXAMPLES_SMART_VERBOSE != 0
					printf("  Deleted file %s\n", file->name);
#endif
					file->deleted = true;
					g_ndeleted++;
					break;
				}
			}
		}
	}

	return OK;
}

/****************************************************************************
 * Name: smart_delallfiles
 ****************************************************************************/

static int smart_delallfiles(void)
{
	FAR struct smart_filedesc_s *file;
	int ret;
	int i;

	for (i = 0; i < CONFIG_EXAMPLES_SMART_MAXOPEN; i++) {
		file = &g_files[i];
		if (file->name) {
			ret = unlink(file->name);
			if (ret < 0) {
				printf("ERROR: Unlink %d failed: %d\n", i + 1, errno);
				printf("  File name:  %s\n", file->name);
				printf("  File size:  %d\n", file->len);
				printf("  File index: %d\n", i);
			} else {
#if CONFIG_EXAMPLES_SMART_VERBOSE != 0
				printf("  Deleted file %s\n", file->name);
#endif
				smart_freefile(file);
			}
		}
	}

	g_nfiles = 0;
	g_ndeleted = 0;
	return OK;
}

/****************************************************************************
 * Name: smart_directory
 ****************************************************************************/

static int smart_directory(void)
{
	DIR *dirp;
	FAR struct dirent *entryp;
	int number;

	/* Open the directory */

	dirp = opendir(CONFIG_EXAMPLES_SMART_MOUNTPT);

	if (!dirp) {
		/* Failed to open the directory */

		printf("ERROR: Failed to open directory '%s': %d\n", CONFIG_EXAMPLES_SMART_MOUNTPT, errno);
		return ERROR;
	}

	/* Read each directory entry */

	printf("Directory:\n");
	number = 1;
	do {
		entryp = readdir(dirp);
		if (entryp) {
			printf("%2d. Type[%d]: %s Name: %s\n", number, entryp->d_type, entryp->d_type == DTYPE_FILE ? "File " : "Error", entryp->d_name);
		}

		number++;
	} while (entryp != NULL);

	closedir(dirp);
	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: smart_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int smart_main(int argc, char *argv[])
#endif
{
#if 0
	FAR struct mtd_dev_s *mtd;
#endif
	unsigned int i;
	int ret;

	/* Seed the random number generated */

	srand(0x93846);

#if CONFIG_EXAMPLES_SMART_NLOOPS == 0
	for (i = 0;; i++)
#else
	for (i = 1; i <= CONFIG_EXAMPLES_SMART_NLOOPS; i++)
#endif
	{
		/* Write a files to the SMART file system until either (1) all of the
		 * open file structures are utilized or until (2) SMART reports an error
		 * (hopefully that the file system is full)
		 */

		printf("\n=== FILLING %u =============================\n", i);
		(void)smart_fillfs();
		printf("Filled file system\n");
		printf("  Number of files: %d\n", g_nfiles);
		printf("  Number deleted:  %d\n", g_ndeleted);

		/* Directory listing */

		smart_directory();

		/* Verify all files written to FLASH */

		ret = smart_verifyfs();
		if (ret < 0) {
			printf("ERROR: Failed to verify files\n");
			printf("  Number of files: %d\n", g_nfiles);
			printf("  Number deleted:  %d\n", g_ndeleted);
		} else {
#if CONFIG_EXAMPLES_SMART_VERBOSE != 0
			printf("Verified!\n");
			printf("  Number of files: %d\n", g_nfiles);
			printf("  Number deleted:  %d\n", g_ndeleted);
#endif
		}

		/* Delete some files */

		printf("\n=== DELETING %u ============================\n", i);
		ret = smart_delfiles();
		if (ret < 0) {
			printf("ERROR: Failed to delete files\n");
			printf("  Number of files: %d\n", g_nfiles);
			printf("  Number deleted:  %d\n", g_ndeleted);
		} else {
			printf("Deleted some files\n");
			printf("  Number of files: %d\n", g_nfiles);
			printf("  Number deleted:  %d\n", g_ndeleted);
		}

		/* Directory listing */

		smart_directory();

		/* Verify all files written to FLASH */

		ret = smart_verifyfs();
		if (ret < 0) {
			printf("ERROR: Failed to verify files\n");
			printf("  Number of files: %d\n", g_nfiles);
			printf("  Number deleted:  %d\n", g_ndeleted);
		} else {
#if CONFIG_EXAMPLES_SMART_VERBOSE != 0
			printf("Verified!\n");
			printf("  Number of files: %d\n", g_nfiles);
			printf("  Number deleted:  %d\n", g_ndeleted);
#endif
		}

		/* Show memory usage */

		smart_loopmemusage();
		fflush(stdout);
	}

	/* Delete all files then show memory usage again */

	smart_delallfiles();
	smart_endmemusage();
	fflush(stdout);
	return 0;
}
