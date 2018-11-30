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
 * apps/tzshlib/tzsh_ddcmd.c
 *
 *   Copyright (C) 2008-2009, 2012, 2015 Gregory Nutt. All rights reserved.
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
#include <sys/stat.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <debug.h>
#include <errno.h>
#include <time.h>

#include "tzsh.h"
#include "tzsh_console.h"

#if CONFIG_NFILE_DESCRIPTORS > 0 && !defined(CONFIG_TZSH_DISABLE_DD)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* If no sector size is specified with BS=, then the following default value
 * is used.
 */

#define DEFAULT_SECTSIZE 512

/* At present, piping of input and output are not support, i.e., both of=
 * and if= arguments are required.
 */

#undef CAN_PIPE_FROM_STD

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct dd_s {
	FAR struct tzsh_vtbl_s *vtbl;

	int infd;					/* File descriptor of the input device */
	int outfd;					/* File descriptor of the output device */
	uint32_t nsectors;			/* Number of sectors to transfer */
	uint32_t sector;			/* The current sector number */
	uint32_t skip;				/* The number of sectors skipped on input */
	bool eof;					/* true: The end of the input or output file has been hit */
	uint16_t sectsize;			/* Size of one sector */
	uint16_t nbytes;			/* Number of valid bytes in the buffer */
	uint8_t *buffer;			/* Buffer of data to write to the output file */
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const char g_dd[] = "dd";

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: dd_write
 ****************************************************************************/

static int dd_write(struct dd_s *dd)
{
	uint8_t *buffer = dd->buffer;
	uint16_t written;
	ssize_t nbytes;

	/* Is the out buffer full (or is this the last one)? */

	written = 0;
	do {
		nbytes = write(dd->outfd, buffer, dd->sectsize - written);
		if (nbytes < 0) {
			FAR struct tzsh_vtbl_s *vtbl = dd->vtbl;
			tzsh_output(vtbl, g_fmtcmdfailed, g_dd, "write", TZSH_ERRNO_OF(-nbytes));
			return ERROR;
		}

		written += nbytes;
		buffer += nbytes;
	} while (written < dd->sectsize);

	return OK;
}

/****************************************************************************
 * Name: dd_read
 ****************************************************************************/

static int dd_read(struct dd_s *dd)
{
	uint8_t *buffer = dd->buffer;
	ssize_t nbytes;

	dd->nbytes = 0;
	do {
		nbytes = read(dd->infd, buffer, dd->sectsize - dd->nbytes);
		if (nbytes < 0) {
			FAR struct tzsh_vtbl_s *vtbl = dd->vtbl;
			tzsh_output(vtbl, g_fmtcmdfailed, g_dd, "read", TZSH_ERRNO_OF(-nbytes));
			return ERROR;
		}

		dd->nbytes += nbytes;
		buffer += nbytes;
	} while (dd->nbytes < dd->sectsize && nbytes > 0);

	dd->eof |= (dd->nbytes == 0);
	return OK;
}

/****************************************************************************
 * Name: dd_infopen
 ****************************************************************************/

static inline int dd_infopen(const char *name, struct dd_s *dd)
{
	dd->infd = open(name, O_RDONLY);
	if (dd->infd < 0) {
		FAR struct tzsh_vtbl_s *vtbl = dd->vtbl;
		tzsh_output(vtbl, g_fmtcmdfailed, g_dd, "open", TZSH_ERRNO);
		return ERROR;
	}

	return OK;
}

/****************************************************************************
 * Name: dd_outfopen
 ****************************************************************************/

static inline int dd_outfopen(const char *name, struct dd_s *dd)
{
	dd->outfd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (dd->outfd < 0) {
		tzsh_output(dd->vtbl, g_fmtcmdfailed, g_dd, "open", TZSH_ERRNO);
		return ERROR;
	}

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cmd_dd
 ****************************************************************************/

int cmd_dd(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	struct dd_s dd;
	char *infile = NULL;
	char *outfile = NULL;
#ifdef CONFIG_TZSH_CMDOPT_DD_STATS
	struct timespec ts0;
	struct timespec ts1;
	uint64_t elapsed;
	uint64_t total;
#endif
	int ret = ERROR;
	int i;

	/* Initialize the dd structure */

	memset(&dd, 0, sizeof(struct dd_s));
	dd.vtbl = vtbl;				/* For tzsh_output */
	dd.sectsize = DEFAULT_SECTSIZE;	/* Sector size if 'bs=' not provided */
	dd.nsectors = 0xffffffff;	/* MAX_UINT32 */

	/* If no IF= option is provided on the command line, then read
	 * from stdin.
	 */

#ifdef CAN_PIPE_FROM_STD
	dd->infd = 0;				/* stdin */
#endif

	/* If no OF= option is provided on the command line, then write
	 * to stdout.
	 */

#ifdef CAN_PIPE_FROM_STD
	dd->outfd = 1;				/* stdout */
#endif

	/* Parse command line parameters */

	for (i = 1; i < argc; i++) {
		if (strncmp(argv[i], "if=", 3) == 0) {
			if (infile != NULL) {
				free(infile);
			}

			infile = tzsh_getfullpath(vtbl, &argv[i][3]);
		} else if (strncmp(argv[i], "of=", 3) == 0) {
			if (outfile != NULL) {
				free(outfile);
			}

			outfile = tzsh_getfullpath(vtbl, &argv[i][3]);
		} else if (strncmp(argv[i], "bs=", 3) == 0) {
			dd.sectsize = atoi(&argv[i][3]);
		} else if (strncmp(argv[i], "count=", 6) == 0) {
			dd.nsectors = atoi(&argv[i][6]);
		} else if (strncmp(argv[i], "skip=", 5) == 0) {
			dd.skip = atoi(&argv[i][5]);
		}
	}

#ifndef CAN_PIPE_FROM_STD
	if (infile == NULL || outfile == NULL) {
		tzsh_output(vtbl, g_fmtargrequired, g_dd);
		goto errout_with_paths;
	}
#endif

	/* Allocate the I/O buffer */

	dd.buffer = malloc(dd.sectsize);
	if (!dd.buffer) {
		tzsh_output(vtbl, g_fmtcmdoutofmemory, g_dd);
		goto errout_with_paths;
	}

	/* Open the input file */

	ret = dd_infopen(infile, &dd);
	if (ret < 0) {
		goto errout_with_paths;
	}

	/* Open the output file */

	ret = dd_outfopen(outfile, &dd);
	if (ret < 0) {
		goto errout_with_inf;
	}

	/* Then perform the data transfer */

#ifdef CONFIG_TZSH_CMDOPT_DD_STATS
#ifdef CONFIG_CLOCK_MONOTONIC
	(void)clock_gettime(CLOCK_MONOTONIC, &ts0);
#else
	(void)clock_gettime(CLOCK_REALTIME, &ts0);
#endif
#endif

	dd.sector = 0;
	while (!dd.eof && dd.nsectors > 0) {
		/* Read one sector from from the input */

		ret = dd_read(&dd);
		if (ret < 0) {
			goto errout_with_outf;
		}

		/* Has the incoming data stream ended? */

		if (!dd.eof) {
			/* Pad with zero if necessary (at the end of file only) */

			for (i = dd.nbytes; i < dd.sectsize; i++) {
				dd.buffer[i] = 0;
			}

			/* Write one sector to the output file */

			if (dd.sector >= dd.skip) {
				ret = dd_write(&dd);
				if (ret < 0) {
					goto errout_with_outf;
				}

				/* Decrement to show that a sector was written */

				dd.nsectors--;
			}

			/* Increment the sector number */

			dd.sector++;
		}
	}

	ret = OK;

#ifdef CONFIG_TZSH_CMDOPT_DD_STATS
#ifdef CONFIG_CLOCK_MONOTONIC
	(void)clock_gettime(CLOCK_MONOTONIC, &ts1);
#else
	(void)clock_gettime(CLOCK_REALTIME, &ts1);
#endif

	elapsed = (((uint64_t)ts1.tv_sec * NSEC_PER_SEC) + ts1.tv_nsec);
	elapsed -= (((uint64_t)ts0.tv_sec * NSEC_PER_SEC) + ts0.tv_nsec);
	elapsed /= NSEC_PER_MSEC;	/* msec */

	total = ((uint64_t)dd.sector * (uint64_t)dd.sectsize);

	tzsh_output(vtbl, "%llu bytes copied, %u msec, ", total, (unsigned int)elapsed);
	tzsh_output(vtbl, "%u KB/s\n", (unsigned int)((double)total / (double)elapsed));
#endif

errout_with_outf:
	(void)close(dd.outfd);

errout_with_inf:
	(void)close(dd.infd);
	free(dd.buffer);

errout_with_paths:
	if (infile) {
		tzsh_freefullpath(infile);
	}

	if (outfile) {
		tzsh_freefullpath(outfile);
	}

	return ret;
}

#endif							/* CONFIG_NFILE_DESCRIPTORS && !CONFIG_TZSH_DISABLE_DD */
