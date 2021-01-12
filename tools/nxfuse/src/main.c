/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * SmartFUSE - A FUSE filesystem for mounting NuttX FS natively under Linux.
 *
 * tools/nxfuse/main.c
 *
 *   Copyright (C) 2016 Ken Pettit. All rights reserved.
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

#define NXFUSE_VERSION  "1.2"

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <debug.h>
#include <string.h>

#define FUSE_USE_VERSION 26

#include <fuse.h>

#include "nxfuse.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Extern data references
 ****************************************************************************/

extern struct fuse_operations nxfuse_oper;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name:  main
 *
 * Main entry point for the FUSE based filesystem.
 *
 * Invocation Format:
 *
 *     nxfuse [-e erasesize] [-s sectorsize] mount_point filename
 *
 ****************************************************************************/

int main(int argc, char *argv[])
{
	int opt, ret;
	uint32_t erasesize = 0;
	uint16_t sectsize = 0;
	int pagesize = 0;
	int fuse_argc;
	int confirm = 0;
	char *generic = "";
	int opt_mkfs = 0;
	int no_mount = 0;
	char **fuse_argv = NULL;
	const char *filename;
	char *mount_point;
	struct inode *pinode;
	struct nxfuse_state *nxfuse_data = NULL;
	const char *fs_type = "smartfs";
	char fsname[128];

	/* We will recreate the argv array for fuse_main with only those
	 * options that FUSE understands, pulling out our additions.
	 */

	fuse_argc = 1;
	fuse_argv = malloc(sizeof(char *));
	if (fuse_argv == NULL) {
		printf("Unable to allocate memory for \'fuse_argv\' pointer\n");
		return -1;
	}
	fuse_argv[0] = argv[0];

	/* Parse the options.  This includes our own custom options as well
	 * as the standard FUSE -d -f -h -s -o and -V options.
	 */

	while ((opt = getopt(argc, argv, "cde:fg:ho:l:mp:st:Vv")) != -1) {
		switch (opt) {
		case 'd':
			/* Add this arg to the fuse_main args */

			fuse_argc++;
			fuse_argv = realloc(fuse_argv, sizeof(char *) * fuse_argc);
			if (fuse_argv == NULL) {
				printf("Unable to reallocate memory for \'fuse_argv\' pointer\n");
				return -1;
			}
			fuse_argv[fuse_argc - 1] = "-d";
			break;

		case 'h':
			/* Add this arg to the fuse_main args */

			no_mount = 1;
			fuse_argc++;
			fuse_argv = realloc(fuse_argv, sizeof(char *) * fuse_argc);
			if (fuse_argv == NULL) {
				printf("Unable to reallocate memory for \'fuse_argv\' pointer\n");
				return -1;
			}
			fuse_argv[fuse_argc - 1] = "-h";
			break;

		case 'V':
			/* Add this arg to the fuse_main args */

			no_mount = 1;
			fuse_argc++;
			fuse_argv = realloc(fuse_argv, sizeof(char *) * fuse_argc);
			if (fuse_argv == NULL) {
				printf("Unable to reallocate memory for \'fuse_argv\' pointer\n");
				return -1;
			}
			fuse_argv[fuse_argc - 1] = "-V";
			break;

		case 's':
			/* Add this arg to the fuse_main args */

			fuse_argc++;
			fuse_argv = realloc(fuse_argv, sizeof(char *) * fuse_argc);
			if (fuse_argv == NULL) {
				printf("Unable to reallocate memory for \'fuse_argv\' pointer\n");
				return -1;
			}
			fuse_argv[fuse_argc - 1] = "-s";
			break;

		case 'f':
			/* Add this arg to the fuse_main args */

			fuse_argc++;
			fuse_argv = realloc(fuse_argv, sizeof(char *) * fuse_argc);
			if (fuse_argv == NULL) {
				printf("Unable to reallocate memory for \'fuse_argv\' pointer\n");
				return -1;
			}
			fuse_argv[fuse_argc - 1] = "-f";
			break;

		case 'o':
			/* Add this arg to the fuse_main args */

			fuse_argc += 2;
			fuse_argv = realloc(fuse_argv, sizeof(char *) * fuse_argc);
			if (fuse_argv == NULL) {
				printf("Unable to reallocate memory for \'fuse_argv\' pointer\n");
				return -1;
			}
			fuse_argv[fuse_argc - 2] = "-o";
			fuse_argv[fuse_argc - 1] = optarg;
			break;

			/* MTD Page size option */

		case 'p':
			pagesize = atoi(optarg);
			break;

			/* Erase size option */

		case 'e':
			erasesize = atoi(optarg);
			break;

			/* Logical sector size option */

		case 'l':
			sectsize = atoi(optarg);
			break;

			/* FS type option */

		case 't':
			/* Save the FS type */

			fs_type = optarg;
			break;

		case 'c':
			confirm = 1;
			break;

		case 'm':
			opt_mkfs = 1;
			no_mount = 1;
			break;

		case 'g':
			generic = optarg;
			break;

		case 'v':
			printf("nxfuse version %s\n", NXFUSE_VERSION);
			printf("Copyright (C) 2016 Ken Pettit.  All rights reserved.\n");
			return 1;

		}
	}

	/* If mkfs or help option provided, then we only expect 1 arg */

	if (no_mount) {
		/* We should have 1 args left */

		if (argc - optind != 1) {
			printf("Usage: %s [-e erasesize] [-l logical_sectorsize] [-t fstype] [fuse options] mount_point datasource\n", argv[0]);
			return -1;
		}

		if (opt_mkfs) {
			filename = argv[optind];
		} else {
			mount_point = argv[optind];
		}
	} else {
		/* We should have 2 args left */

		if (argc - optind != 2) {
			printf("Usage: %s [-e erasesize] [-l logical_sectorsize] [-t fstype] [fuse options] mount_point datasource\n", argv[0]);
			return -1;
		}

		mount_point = argv[optind];
		filename = argv[optind + 1];
	}

	/* Test for mkfs option */

	if (opt_mkfs) {
		mkfs(filename, fs_type, erasesize, sectsize, pagesize, generic, confirm);
		return 0;
	}

	/* If no in help mode, mount the NuttX FS */

	if (!no_mount) {
		/* Try to virtually mount the NuttX Filesystem */

		pinode = vmount(filename, mount_point, fs_type, erasesize, sectsize, pagesize, generic);
		if (pinode == NULL) {
			/* Error mounting the NuttX filesystem */

			printf("Unable to mount filesytem on %s of type %s\n", mount_point, fs_type);
			return -1;
		}

		/* Setup the FUSE environment */

		nxfuse_data = malloc(sizeof(struct nxfuse_state));
		nxfuse_data->rootdir = mount_point;
		nxfuse_data->pinode = pinode;
	}

	/* Add the fs_type to the fuse arguments */

	fuse_argc += 2;
	fuse_argv = realloc(fuse_argv, sizeof(char *) * fuse_argc);
	if (fuse_argv == NULL) {
		printf("Unable to reallocate memory for \'fuse_argv\' pointer\n");
		return -1;
	}
	fuse_argv[fuse_argc - 2] = "-o";
	sprintf(fsname, "subtype=%s", fs_type);
	fuse_argv[fuse_argc - 1] = fsname;

	/* Add the mount_point to the fuse arguments */

	fuse_argc++;
	fuse_argv = realloc(fuse_argv, sizeof(char *) * fuse_argc);
	if (fuse_argv == NULL) {
		printf("Unable to reallocate memory for \'fuse_argv\' pointer\n");
		return -1;
	}
	fuse_argv[fuse_argc - 1] = mount_point;

	/* Call fuse_main to start the FS */

	ret = fuse_main(fuse_argc, fuse_argv, &nxfuse_oper, nxfuse_data);

	return ret;
}
