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
 * apps/system/utils/fs_getpath.c
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <tinyara/fs/fs_utils.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

FAR const char *getwd(const char *wd)
{
	const char *path;
	path = getenv(wd);
	if (!path) {
		path = CONFIG_LIB_HOMEDIR;
	}
	return path;
}

FAR char *get_fullpath(FAR const char *relpath)
{
	const char *wd;

	/* Handle some special cases */
	if (!relpath || relpath[0] == '\0') {
		/* No relative path provided */
		return strdup(CONFIG_LIB_HOMEDIR);
	} else if (relpath[0] == '/') {
		return strdup(relpath);
	}

	/* Get the path to the current working directory */
	wd = getwd(PWD);

	/* Fake the '.' directory */
	if (strcmp(relpath, ".") == 0) {
		return strdup(wd);
	}

	/* Return the full path */
	return get_dirpath(wd, relpath);

}

char *get_dirpath(const char *dirpath, const char *relpath)
{
	char *alloc;
	int len;

	/* Handle the special case where the dirpath is simply "/" */

	if (strcmp(dirpath, "/") == 0) {
		len = strlen(relpath) + 2;
		alloc = (char *)malloc(len);
		if (alloc) {
			snprintf(alloc, len, "/%s", relpath);
		}
	} else {
		len = strlen(dirpath) + strlen(relpath) + 2;
		alloc = (char *)malloc(len);
		if (alloc) {
			snprintf(alloc, len, "%s/%s", dirpath, relpath);
		}
	}

	if (!alloc) {
		printf("\t out of memory\n");
	}
	return alloc;
}
