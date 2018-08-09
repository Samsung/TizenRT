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
 * libc/stdlib/lib_mkstemp.c
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
#include <tinyara/compiler.h>

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "lib_internal.h"

#ifdef CONFIG_FS_WRITABLE

/****************************************************************************
 * Pre-processor definitions
 ****************************************************************************/

#ifndef CONFIG_LIBC_TMPDIR
#define CONFIG_LIBC_TMPDIR "/tmp"
#endif

#define MAX_XS 6
#define BIG_XS 5

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mkstemp
 *
 * Description:
 *   The mkstemp() function replaces the contents of the string pointed to
 *   by path_template by a unique filename, and returns a file descriptor
 *   for the file open for reading and writing. The function thus prevents
 *   any possible race condition between testing whether the file exists and
 *   opening it for use. The string in path_template should look like a
 *   filename with six trailing 'X' s; mkstemp() replaces each 'X' with a
 *   character from the portable filename character set. The characters are
 *   chosen such that the resulting name does not duplicate the name of an
 *   existing file at the time of a call to mkstemp().
 *
 * Input Parameters:
 *   path_template - The base file name that will be modified to produce
 *     the unique file name.  This must be a full path beginning with /tmp.
 *     This function will modify only the first XXXXXX characters within
 *     that full path.
 *
 * Returned Value:
 *
 * Upon successful completion, mkstemp() returns an open file descriptor.
 * Otherwise, -1 is returned if no suitable file could be created.
 *
 ****************************************************************************/

int mkstemp(FAR char *path_template)
{
	uint32_t retries;
	FAR char *xptr;
	FAR char *ptr;
	int xlen;
	int fd;
	int i;

	if (!path_template) {
		set_errno(EINVAL);
		return ERROR;
	}

	/* Count the number of X's at the end of the template */

	xptr = strchr(path_template, 'X');
	if (!xptr) {
		/* No Xs?  There should always really be 6 */

		return open(path_template, O_RDWR | O_CREAT | O_EXCL, 0666);
	}

	/* There is at least one.. count all of them */

	for (xlen = 0, ptr = xptr; xlen < MAX_XS && *ptr == 'X'; xlen++, ptr++);

	/* If xlen is small, then we need to determine the maximum number of
	 * retries before the values will repeat.
	 */

	if (xlen >= BIG_XS) {
		retries = UINT32_MAX;
	} else {
		for (i = 1, retries = 62; i < xlen; i++, retries *= 62);
	}

	/* Then loop until we find a unique file name */

	do {
		/* Form the candidate file name */

		(void)__randname(xptr, xlen);

		/* Attempt to open the candidate file -- creating it exclusively
		 *
		 * REVISIT: This prohibits the use of this function to create unique
		 * directories
		 */

		fd = open(path_template, O_RDWR | O_CREAT | O_EXCL, 0666);
		if (fd >= 0) {
			/* We have it... return the file descriptor */

			return fd;
		}
	} while (--retries && get_errno() == EEXIST);

	/* We could not find an unique filename */

	return ERROR;
}

#endif							/* CONFIG_FS_WRITABLE */
