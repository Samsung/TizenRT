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
 * apps/tzshlib/tzsh_fsutils.c
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
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
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <dirent.h>

#include "tzsh.h"
#include "tzsh_console.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_catfile
 *
 * Description:
 *   Dump the contents of a file to the current TZSH terminal.
 *
 * Input Paratemets:
 *   vtbl     - session vtbl
 *   cmd      - TZSH command name to use in error reporting
 *   filepath - The full path to the file to be dumped
 *
 * Returned Value:
 *   Zero (OK) on success; -1 (ERROR) on failure.
 *
 ****************************************************************************/

#ifdef TZSH_HAVE_CATFILE
int tzsh_catfile(FAR struct tzsh_vtbl_s *vtbl, FAR const char *cmd, FAR const char *filepath)
{
	FAR char *buffer;
	int fd;
	int ret = OK;

	/* Open the file for reading */

	fd = open(filepath, O_RDONLY);
	if (fd < 0) {
		tzsh_output(vtbl, g_fmtcmdfailed, cmd, "open", TZSH_ERRNO);
		return ERROR;
	}

	buffer = (FAR char *)malloc(IOBUFFERSIZE);
	if (buffer == NULL) {
		(void)close(fd);
		tzsh_output(vtbl, g_fmtcmdfailed, cmd, "malloc", TZSH_ERRNO);
		return ERROR;
	}

	/* And just dump it byte for byte into stdout */

	for (;;) {
		int nbytesread = read(fd, buffer, IOBUFFERSIZE);

		/* Check for read errors */

		if (nbytesread < 0) {
			int errval = errno;

			/* EINTR is not an error (but will stop stop the cat) */

#ifndef CONFIG_DISABLE_SIGNALS
			if (errval == EINTR) {
				tzsh_output(vtbl, g_fmtsignalrecvd, cmd);
			} else
#endif
			{
				tzsh_output(vtbl, g_fmtcmdfailed, cmd, "read", TZSH_ERRNO_OF(errval));
			}

			ret = ERROR;
			break;
		}

		/* Check for data successfully read */

		else if (nbytesread > 0) {
			int nbyteswritten = 0;

			while (nbyteswritten < nbytesread) {
				ssize_t n = tzsh_write(vtbl, buffer, nbytesread);
				if (n < 0) {
					int errcode = errno;

					/* EINTR is not an error (but will stop stop the cat) */

#ifndef CONFIG_DISABLE_SIGNALS
					if (errcode == EINTR) {
						tzsh_output(vtbl, g_fmtsignalrecvd, cmd);
					} else
#endif
					{
						tzsh_output(vtbl, g_fmtcmdfailed, cmd, "write", TZSH_ERRNO_OF(errcode));
					}

					ret = ERROR;
					break;
				} else {
					nbyteswritten += n;
				}
			}
		}

		/* Otherwise, it is the end of file */

		else {
			break;
		}
	}

	/* Make sure that the following TZSH prompt appears on a new line.  If the
	 * file ends in a newline, then this will print an extra blank line
	 * before the prompt, but that is preferable to the case where there is
	 * no newline and the TZSH prompt appears on the same line as the cat'ed
	 * file.
	 */

	tzsh_output(vtbl, "\n");

	/* Close the input file and return the result */

	(void)close(fd);
	free(buffer);
	return ret;
}
#endif

/****************************************************************************
 * Name: tzsh_readfile
 *
 * Description:
 *   Read a small file into a user-provided buffer.  The data is assumed to
 *   be a string and is guaranteed to be NUL-termined.  An error occurs if
 *   the file content (+terminator)  will not fit into the provided 'buffer'.
 *
 * Input Paramters:
 *   vtbl     - The console vtable
 *   filepath - The full path to the file to be read
 *   buffer   - The user-provided buffer into which the file is read.
 *   buflen   - The size of the user provided buffer
 *
 * Returned Value:
 *   Zero (OK) is returned on success; -1 (ERROR) is returned on any
 *   failure to read the fil into the buffer.
 *
 ****************************************************************************/

#ifdef TZSH_HAVE_READFILE
int tzsh_readfile(FAR struct tzsh_vtbl_s *vtbl, FAR const char *cmd, FAR const char *filepath, FAR char *buffer, size_t buflen)
{
	FAR char *bufptr;
	size_t remaining;
	ssize_t nread;
	ssize_t ntotal;
	int fd;
	int ret;

	/* Open the file */

	fd = open(filepath, O_RDONLY);
	if (fd < 0) {
		tzsh_output(vtbl, g_fmtcmdfailed, cmd, "open", TZSH_ERRNO);
		return ERROR;
	}

	/* Read until we hit the end of the file, until we have exhausted the
	 * buffer space, or until some irrecoverable error occurs
	 */

	ntotal = 0;					/* No bytes read yet */
	*buffer = '\0';				/* NUL terminate the empty buffer */
	bufptr = buffer;			/* Working pointer */
	remaining = buflen - 1;		/* Reserve one byte for a NUL terminator */
	ret = ERROR;				/* Assume failure */

	do {
		nread = read(fd, bufptr, remaining);
		if (nread < 0) {
			/* Read error */

			int errcode = errno;
			DEBUGASSERT(errcode > 0);

			/* EINTR is not a read error.  It simply means that a signal was
			 * received while waiting for the read to complete.
			 */

			if (errcode != EINTR) {
				/* Fatal error */

				tzsh_output(vtbl, g_fmtcmdfailed, cmd, "read", TZSH_ERRNO);
				break;
			}
		} else if (nread == 0) {
			/* End of file */

			ret = OK;
			break;
		} else {
			/* Successful read.  Make sure that the buffer is null terminated */

			DEBUGASSERT(nread <= remaining);
			ntotal += nread;
			buffer[ntotal] = '\0';

			/* Bump up the read count and continuing reading to the end of
			 * file.
			 */

			bufptr += nread;
			remaining -= nread;
		}
	} while (buflen > 0);

	/* Close the file and return. */

	close(fd);
	return ret;
}
#endif

/****************************************************************************
 * Name: tzsh_foreach_direntry
 *
 * Description:
 *    Call the provided 'handler' for each entry found in the directory at
 *    'dirpath'.
 *
 * Input Parameters
 *   vtbl     - The console vtable
 *   cmd      - TZSH command name to use in error reporting
 *   dirpath  - The full path to the directory to be traversed
 *   handler  - The handler to be called for each entry of the directory
 *   pvarg    - User provided argument to be passed to the 'handler'
 *
 * Returned Value:
 *   Zero (OK) returned on success; -1 (ERROR) returned on failure.
 *
 ****************************************************************************/

#ifdef TZSH_HAVE_FOREACH_DIRENTRY
int tzsh_foreach_direntry(FAR struct tzsh_vtbl_s *vtbl, FAR const char *cmd, FAR const char *dirpath, tzsh_direntry_handler_t handler, void *pvarg)
{
	DIR *dirp;
	int ret = OK;

	/* Open the directory */

	dirp = opendir(dirpath);
	if (dirp == NULL) {
		/* Failed to open the directory */

		tzsh_output(vtbl, g_fmtnosuch, cmd, "directory", dirpath);
		return ERROR;
	}

	/* Read each directory entry */

	for (;;) {
		FAR struct dirent *entryp = readdir(dirp);
		if (entryp == NULL) {
			/* Finished with this directory */

			break;
		}

		/* Call the handler with this directory entry */

		if (handler(vtbl, dirpath, entryp, pvarg) < 0) {
			/* The handler reported a problem */

			ret = ERROR;
			break;
		}
	}

	closedir(dirp);
	return ret;
}
#endif

/****************************************************************************
 * Name: tzsh_trimdir
 *
 * Description:
 *   Skip any trailing '/' characters (unless it is also the leading '/')
 *
 * Input Parmeters:
 *   dirpath - The directory path to be trimmed.  May be modified!
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/

#ifdef TZSH_HAVE_TRIMDIR
void tzsh_trimdir(FAR char *dirpath)
{
	/* Skip any trailing '/' characters (unless it is also the leading '/') */

	int len = strlen(dirpath) - 1;
	while (len > 0 && dirpath[len] == '/') {
		dirpath[len] = '\0';
		len--;
	}
}
#endif

/****************************************************************************
 * Name: tzsh_trimspaces
 *
 * Description:
 *   Trim any leading or trailing spaces from a string.
 *
 * Input Parmeters:
 *   str - The sring to be trimmed.  May be modified!
 *
 * Returned value:
 *   The new string pointer.
 *
 ****************************************************************************/

#ifdef TZSH_HAVE_TRIMSPACES
FAR char *tzsh_trimspaces(FAR char *str)
{
	FAR char *trimmed;
	int ndx;

	/* Strip leading whitespace from the value */

	for (trimmed = str; *trimmed != '\0' && isspace(*trimmed); trimmed++) ;

	/* Strip trailing whitespace from the value */

	for (ndx = strlen(trimmed) - 1; ndx >= 0 && isspace(trimmed[ndx]); ndx--) {
		trimmed[ndx] = '\0';
	}

	return trimmed;
}
#endif
