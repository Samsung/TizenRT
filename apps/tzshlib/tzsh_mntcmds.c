/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
 * apps/tzshlib/tzsh_mntcmds.c
 *
 *   Copyright (C) 2007-2009, 2011-2013 Gregory Nutt. All rights reserved.
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
#include <sys/mount.h>
#include <sys/statfs.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <debug.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "tzsh.h"
#include "tzsh_console.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cmd_df
 ****************************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0 && !defined(CONFIG_DISABLE_MOUNTPOINT) && \
	defined(CONFIG_FS_READABLE) && !defined(CONFIG_TZSH_DISABLE_DF)
#ifdef TZSH_HAVE_CATFILE
int cmd_df(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
#if defined(HAVE_DF_HUMANREADBLE) && defined(HAVE_DF_BLOCKOUTPUT)
	if (argc > 1 && strcmp(argv[1], "-h") == 0)
#endif
#ifdef HAVE_DF_HUMANREADBLE
	{
		return tzsh_catfile(vtbl, argv[0], CONFIG_TZSH_PROC_MOUNTPOINT "/fs/usage");
	}
#endif
#if defined(HAVE_DF_HUMANREADBLE) && defined(HAVE_DF_BLOCKOUTPUT)
	else
#endif
#ifdef HAVE_DF_BLOCKOUTPUT
	{
		return tzsh_catfile(vtbl, argv[0], CONFIG_TZSH_PROC_MOUNTPOINT "/fs/blocks");
	}
#endif
}
#endif
#endif

/****************************************************************************
 * Name: cmd_mount
 ****************************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0 && !defined(CONFIG_DISABLE_MOUNTPOINT) && \
	defined(CONFIG_FS_READABLE) && !defined(CONFIG_TZSH_DISABLE_MOUNT)
int cmd_mount(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	FAR const char *source;
	FAR char *fullsource;
	FAR const char *target;
	FAR char *fulltarget;
	FAR const char *filesystem = NULL;
	FAR const char *options = NULL;
	bool badarg = false;
	int option;
	int ret;

	/* The mount command behaves differently if no parameters are provided. */

#if defined(TZSH_HAVE_CATFILE) && defined(HAVE_MOUNT_LIST)
	if (argc < 2) {
		return tzsh_catfile(vtbl, argv[0], CONFIG_TZSH_PROC_MOUNTPOINT "/fs/mount");
	}
#endif

	/* Get the mount options.  NOTE: getopt() is not thread safe nor re-
	 * entrant.  To keep its state proper for the next usage, it is necessary
	 * to parse to the end of the line even if an error occurs.  If an error
	 * occurs, this logic just sets 'badarg' and continues.
	 */

	while ((option = getopt(argc, argv, ":o:t:")) != ERROR) {
		switch (option) {
		case 't':
			filesystem = optarg;
			break;

		case 'o':
			options = optarg;
			break;

		case ':':
			tzsh_output(vtbl, g_fmtargrequired, argv[0]);
			badarg = true;
			break;

		case '?':
		default:
			tzsh_output(vtbl, g_fmtarginvalid, argv[0]);
			badarg = true;
			break;
		}
	}

	/* If a bad argument was encountered, then return without processing the
	 * command.
	 */

	if (badarg) {
		return ERROR;
	}

	/* There may be one or two required arguments after the options: the source
	 * and target paths.  Some file systems do not require the source parameter
	 * so if there is only one parameter left, it must be the target.
	 */

	if (optind >= argc) {
		tzsh_output(vtbl, g_fmtargrequired, argv[0]);
		return ERROR;
	}

	source = NULL;
	target = argv[optind];
	optind++;

	if (optind < argc) {
		source = target;
		target = argv[optind];
		optind++;

		if (optind < argc) {
			tzsh_output(vtbl, g_fmttoomanyargs, argv[0]);
			return ERROR;
		}
	}

	/* While the above parsing for the -t argument looks nice, the -t argument
	 * not really optional.
	 */

	if (!filesystem) {
		tzsh_output(vtbl, g_fmtargrequired, argv[0]);
		return ERROR;
	}

	/* The source and target paths might be relative to the current
	 * working directory.
	 */

	fullsource = NULL;
	fulltarget = NULL;

	if (source) {
		fullsource = tzsh_getfullpath(vtbl, source);
		if (!fullsource) {
			return ERROR;
		}
	}

	fulltarget = tzsh_getfullpath(vtbl, target);
	if (!fulltarget) {
		ret = ERROR;
		goto errout;
	}

	/* Perform the mount */

	ret = mount(fullsource, fulltarget, filesystem, 0, options);
	if (ret < 0) {
		tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "mount", TZSH_ERRNO);
	}

errout:
	if (fullsource) {
		tzsh_freefullpath(fullsource);
	}

	if (fulltarget) {
		tzsh_freefullpath(fulltarget);
	}

	return ret;
}
#endif

/****************************************************************************
 * Name: cmd_umount
 ****************************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0 && !defined(CONFIG_DISABLE_MOUNTPOINT) && \
	defined(CONFIG_FS_READABLE) && !defined(CONFIG_TZSH_DISABLE_UMOUNT)
int cmd_umount(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	char *fullpath = tzsh_getfullpath(vtbl, argv[1]);
	int ret = ERROR;

	if (fullpath) {
		/* Perform the umount */

		ret = umount(fullpath);
		if (ret < 0) {
			tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "umount", TZSH_ERRNO);
		}

		tzsh_freefullpath(fullpath);
	}

	return ret;
}
#endif
