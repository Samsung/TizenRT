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
 * apps/tzshlib/tzsh_console.c
 *
 *   Copyright (C) 2007-2009, 2011-2013, 2015 Gregory Nutt. All rights reserved.
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
 * 3. Neither the name Gregory Nutt nor the names of its contributors may be
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include "tzsh.h"
#include "tzsh_console.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/

#if CONFIG_NFILE_STREAMS > 0
struct serialsave_s {
	int cn_outfd;				/* Re-directed output file descriptor */
	FILE *cn_outstream;			/* Re-directed output stream */
};
#endif

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLEBG
static FAR struct tzsh_vtbl_s *tzsh_consoleclone(FAR struct tzsh_vtbl_s *vtbl);
#endif
static void tzsh_consolerelease(FAR struct tzsh_vtbl_s *vtbl);
static ssize_t tzsh_consolewrite(FAR struct tzsh_vtbl_s *vtbl, FAR const void *buffer, size_t nbytes);
static int tzsh_consoleoutput(FAR struct tzsh_vtbl_s *vtbl, FAR const char *fmt, ...);
static FAR char *tzsh_consolelinebuffer(FAR struct tzsh_vtbl_s *vtbl);

#if CONFIG_NFILE_DESCRIPTORS > 0
static void tzsh_consoleredirect(FAR struct tzsh_vtbl_s *vtbl, int fd, FAR uint8_t *save);
static void tzsh_consoleundirect(FAR struct tzsh_vtbl_s *vtbl, FAR uint8_t *save);
#endif

static void tzsh_consoleexit(FAR struct tzsh_vtbl_s *vtbl, int exitstatus) noreturn_function;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_openifnotopen
 ****************************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0
static int tzsh_openifnotopen(struct console_stdio_s *pstate)
{
	/* The stream is open in a lazy fashion.  This is done because the file
	 * descriptor may be opened on a different task than the stream.
	 */

	if (!pstate->cn_outstream) {
		pstate->cn_outstream = fdopen(pstate->cn_outfd, "w");
		if (!pstate->cn_outstream) {
			return ERROR;
		}
	}

	return 0;
}
#endif

/****************************************************************************
 * Name: tzsh_closeifnotclosed
 *
 * Description:
 *   Close the output stream if it is not the standard output stream.
 *
 ****************************************************************************/

#if CONFIG_NFILE_STREAMS > 0
static void tzsh_closeifnotclosed(struct console_stdio_s *pstate)
{
	if (pstate->cn_outstream == OUTSTREAM(pstate)) {
		fflush(OUTSTREAM(pstate));
		pstate->cn_outfd = OUTFD(pstate);
	} else {
		if (pstate->cn_outstream) {
			fflush(pstate->cn_outstream);
			fclose(pstate->cn_outstream);
		} else if (pstate->cn_outfd >= 0 && pstate->cn_outfd != OUTFD(pstate)) {
			close(pstate->cn_outfd);
		}

		pstate->cn_outfd = -1;
		pstate->cn_outstream = NULL;
	}
}
#endif

/****************************************************************************
 * Name: tzsh_consolewrite
 *
 * Description:
 *   write a buffer to the remote shell window.
 *
 *   Currently only used by cat.
 *
 ****************************************************************************/

static ssize_t tzsh_consolewrite(FAR struct tzsh_vtbl_s *vtbl, FAR const void *buffer, size_t nbytes)
{
#if CONFIG_NFILE_DESCRIPTORS > 0
	FAR struct console_stdio_s *pstate = (FAR struct console_stdio_s *)vtbl;
	ssize_t ret;

	/* The stream is open in a lazy fashion.  This is done because the file
	 * descriptor may be opened on a different task than the stream.  The
	 * actual open will then occur with the first output from the new task.
	 */

	if (tzsh_openifnotopen(pstate) != 0) {
		return (ssize_t)ERROR;
	}

	/* Write the data to the output stream */

	ret = fwrite(buffer, 1, nbytes, pstate->cn_outstream);
#ifdef CONFIG_DEBUG_FEATURES
	if (ret < 0) {
		_err("ERROR: [%d] Failed to send buffer: %d\n", pstate->cn_outfd, errno);
	}
#endif
	return ret;
#else
	/* REVISIT: buffer may not be NUL-terminated */

	printf("%s", buffer);
	return nbytes;
#endif
}

/****************************************************************************
 * Name: tzsh_consoleoutput
 *
 * Description:
 *   Print a string to the currently selected stream.
 *
 ****************************************************************************/

static int tzsh_consoleoutput(FAR struct tzsh_vtbl_s *vtbl, FAR const char *fmt, ...)
{
#if CONFIG_NFILE_DESCRIPTORS > 0
	FAR struct console_stdio_s *pstate = (FAR struct console_stdio_s *)vtbl;
	va_list ap;
	int ret;

	/* The stream is open in a lazy fashion.  This is done because the file
	 * descriptor may be opened on a different task than the stream.  The
	 * actual open will then occur with the first output from the new task.
	 */

	if (tzsh_openifnotopen(pstate) != 0) {
		return ERROR;
	}

	va_start(ap, fmt);
	ret = vfprintf(pstate->cn_outstream, fmt, ap);
	va_end(ap);

	return ret;
#else
	va_list ap;
	char *str;

	/* Use vasprintf() to allocate a buffer and fill it with the formatted
	 * data
	 */

	va_start(ap, fmt);
	str = NULL;
	(void)vasprintf(&str, fmt, ap);

	/* Was a string allocated? */

	if (str) {
		/* Yes.. Print then free the allocated string */

		printf("%s", str);
		free(str);
	}

	va_end(ap);
	return 0;
#endif
}

/****************************************************************************
 * Name: tzsh_consolelinebuffer
 *
 * Description:
 *   Return a reference to the current line buffer
 *
 ****************************************************************************/

static FAR char *tzsh_consolelinebuffer(FAR struct tzsh_vtbl_s *vtbl)
{
	FAR struct console_stdio_s *pstate = (FAR struct console_stdio_s *)vtbl;
	return pstate->cn_line;
}

/****************************************************************************
 * Name: tzsh_consoleclone
 *
 * Description:
 *   Make an independent copy of the vtbl
 *
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLEBG
static FAR struct tzsh_vtbl_s *tzsh_consoleclone(FAR struct tzsh_vtbl_s *vtbl)
{
	FAR struct console_stdio_s *pclone = tzsh_newconsole();
	return &pclone->cn_vtbl;
}
#endif

/****************************************************************************
 * Name: tzsh_consolerelease
 *
 * Description:
 *   Release the cloned instance
 *
 ****************************************************************************/

static void tzsh_consolerelease(FAR struct tzsh_vtbl_s *vtbl)
{
	FAR struct console_stdio_s *pstate = (FAR struct console_stdio_s *)vtbl;

#if CONFIG_NFILE_DESCRIPTORS > 0
	/* Close the output stream */

	tzsh_closeifnotclosed(pstate);

	/* Close the console stream */

#ifdef CONFIG_TZSH_ALTCONDEV
	(void)fclose(pstate->cn_constream);
#endif
#endif

#ifdef CONFIG_TZSH_VARS
	/* Free any TZSH variables */

	if (pstate->varp != NULL) {
		free(pstate->varp);
	}
#endif

	/* Then release the vtable container */

	free(pstate);
}

/****************************************************************************
 * Name: tzsh_consoleredirect
 *
 * Description:
 *   Set up for redirected output.  This function is called from tzsh_parse()
 *   in two different contexts:
 *
 *   1) Redirected background commands of the form:  command > xyz.text &
 *
 *      In this case:
 *      - vtbl: A newly allocated and initialized instance created by
 *        tzsh_consoleclone,
 *      - fd:- The file descriptor of the redirected output
 *      - save: NULL
 *
 *      tzsh_consolerelease() will perform the clean-up when the clone is
 *      destroyed.
 *
 *   2) Redirected foreground commands of the form:  command > xyz.txt
 *
 *      In this case:
 *      - vtbl: The current state structure,
 *      - fd: The file descriptor of the redirected output
 *      - save: Where to save the re-directed registers.
 *
 *      tzsh_consoleundirect() will perform the clean-up after the redirected
 *      command completes.
 *
 ****************************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0
static void tzsh_consoleredirect(FAR struct tzsh_vtbl_s *vtbl, int fd, FAR uint8_t *save)
{
	FAR struct console_stdio_s *pstate = (FAR struct console_stdio_s *)vtbl;
	FAR struct serialsave_s *ssave = (FAR struct serialsave_s *)save;

	/* Case 1: Redirected foreground commands */

	if (ssave) {
		/* pstate->cn_outstream and cn_outfd refer refer to the
		 * currently opened output stream.  If the output stream is open, flush
		 * any pending output.
		 */

		if (pstate->cn_outstream) {
			fflush(pstate->cn_outstream);
		}

		/* Save the current fd and stream values.  These will be restored
		 * when tzsh_consoleundirect() is called.
		 */

		ssave->cn_outfd = pstate->cn_outfd;
		ssave->cn_outstream = pstate->cn_outstream;
	} else {
		/* tzsh_consoleclone() set pstate->cn_outfd and cn_outstream to refer
		 * to standard out.  We just want to leave these alone and overwrite
		 * them with the fd for the re-directed stream.
		 */
	}

	/* In either case, set the fd of the new, re-directed output and nullify
	 * the output stream (it will be fdopen'ed if it is used).
	 */

	pstate->cn_outfd = fd;
	pstate->cn_outstream = NULL;
}
#endif

/****************************************************************************
 * Name: tzsh_consoleundirect
 *
 * Description:
 *   Set up for redirected output
 *
 ****************************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0
static void tzsh_consoleundirect(FAR struct tzsh_vtbl_s *vtbl, FAR uint8_t *save)
{
	FAR struct console_stdio_s *pstate = (FAR struct console_stdio_s *)vtbl;
	FAR struct serialsave_s *ssave = (FAR struct serialsave_s *)save;

	tzsh_closeifnotclosed(pstate);
	pstate->cn_outfd = ssave->cn_outfd;
	pstate->cn_outstream = ssave->cn_outstream;
}
#endif

/****************************************************************************
 * Name: tzsh_consoleexit
 *
 * Description:
 *   Exit the shell task
 *
 ****************************************************************************/

static void tzsh_consoleexit(FAR struct tzsh_vtbl_s *vtbl, int exitstatus)
{
	/* Destroy ourself then exit with the provided status */

	tzsh_consolerelease(vtbl);
	exit(exitstatus);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_newconsole
 ****************************************************************************/

FAR struct console_stdio_s *tzsh_newconsole(void)
{
	FAR struct console_stdio_s *pstate = (FAR struct console_stdio_s *)zalloc(sizeof(struct console_stdio_s));

	if (pstate) {
		/* Initialize the call table */

#ifndef CONFIG_TZSH_DISABLEBG
		pstate->cn_vtbl.clone = tzsh_consoleclone;
		pstate->cn_vtbl.release = tzsh_consolerelease;
#endif
		pstate->cn_vtbl.write = tzsh_consolewrite;
		pstate->cn_vtbl.output = tzsh_consoleoutput;
		pstate->cn_vtbl.linebuffer = tzsh_consolelinebuffer;
		pstate->cn_vtbl.exit = tzsh_consoleexit;

#ifndef CONFIG_TZSH_DISABLESCRIPT
		/* Set the initial option flags */

		pstate->cn_vtbl.np.np_flags = TZSH_NP_SET_OPTIONS_INIT;
#endif

#if CONFIG_NFILE_STREAMS > 0
		pstate->cn_vtbl.redirect = tzsh_consoleredirect;
		pstate->cn_vtbl.undirect = tzsh_consoleundirect;

		/* (Re-) open the console input device */

#ifdef CONFIG_TZSH_ALTCONDEV
		pstate->cn_confd = open(CONFIG_TZSH_CONDEV, O_RDWR);
		if (pstate->cn_confd < 0) {
			free(pstate);
			return NULL;
		}

		/* Create a standard C stream on the console device */

		pstate->cn_constream = fdopen(pstate->cn_confd, "r+");
		if (!pstate->cn_constream) {
			close(pstate->cn_confd);
			free(pstate);
			return NULL;
		}
#endif

		/* Initialize the output stream */

		pstate->cn_outfd = OUTFD(pstate);
		pstate->cn_outstream = OUTSTREAM(pstate);
#endif
	}

	return pstate;
}
