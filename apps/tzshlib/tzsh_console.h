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
 * apps/tzshlib/tzsh_console.h
 *
 *   Copyright (C) 2007-2013 Gregory Nutt. All rights reserved.
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

#ifndef __APPS_TZSHLIB_TZSH_CONSOLE_H
#define __APPS_TZSHLIB_TZSH_CONSOLE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Method access macros */

#define tzsh_clone(v)           (v)->clone(v)
#define tzsh_release(v)         (v)->release(v)
#define tzsh_write(v, b, n)       (v)->write(v, b, n)
#define tzsh_linebuffer(v)      (v)->linebuffer(v)
#define tzsh_redirect(v, f, s)    (v)->redirect(v, f, s)
#define tzsh_undirect(v, s)      (v)->undirect(v, s)
#define tzsh_exit(v, s)          (v)->exit(v, s)

#ifdef CONFIG_CPP_HAVE_VARARGS
#define tzsh_output(v, ...)    (v)->output(v, ##__VA_ARGS__)
#else
#define tzsh_output            vtbl->output
#endif

/* Size of info to be saved in call to tzsh_redirect */

#define SAVE_SIZE (sizeof(int) + sizeof(FILE*) + sizeof(bool))

/* Are we using the NuttX console for I/O?  Or some other character device? */

#if CONFIG_NFILE_STREAMS > 0
#ifdef CONFIG_TZSH_ALTCONDEV

#ifndef CONFIG_TZSH_CONDEV
#error CONFIG_TZSH_ALTCONDEV selected but CONFIG_TZSH_CONDEV not provided
#endif

#define INFD(p)      ((p)->cn_confd)
#define INSTREAM(p)  ((p)->cn_constream)
#define OUTFD(p)     ((p)->cn_confd)
#define OUTSTREAM(p) ((p)->cn_constream)

#else

#define INFD(p)      0
#define INSTREAM(p)  stdin
#define OUTFD(p)     1
#define OUTSTREAM(p) stdout

#endif
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/
/* This describes a generic console front-end */

struct tzsh_vtbl_s {
	/* This function pointers are "hooks" into the front end logic to
	 * handle things like output of command results, redirection, etc.
	 * -- all of which must be done in a way that is unique to the nature
	 * of the front end.
	 */

#ifndef CONFIG_TZSH_DISABLEBG
	FAR struct tzsh_vtbl_s *(*clone)(FAR struct tzsh_vtbl_s *vtbl);
	void (*addref)(FAR struct tzsh_vtbl_s *vtbl);
	void (*release)(FAR struct tzsh_vtbl_s *vtbl);
#endif
	ssize_t(*write)(FAR struct tzsh_vtbl_s *vtbl, FAR const void *buffer, size_t nbytes);
	int (*output)(FAR struct tzsh_vtbl_s *vtbl, FAR const char *fmt, ...);
	FAR char *(*linebuffer)(FAR struct tzsh_vtbl_s *vtbl);
#if CONFIG_NFILE_DESCRIPTORS > 0
	void (*redirect)(FAR struct tzsh_vtbl_s *vtbl, int fd, FAR uint8_t *save);
	void (*undirect)(FAR struct tzsh_vtbl_s *vtbl, FAR uint8_t *save);
#endif
	void (*exit)(FAR struct tzsh_vtbl_s *vtbl, int exitstatus)noreturn_function;

#ifdef TZSH_HAVE_IOBUFFER
	/* Common buffer for file I/O. */

	char iobuffer[IOBUFFERSIZE];
#endif

	/* Parser state data */

	struct tzsh_parser_s np;
};

/* This structure describes a console front-end that is based on stdin and
 * stdout (which is all of the supported console types at the time being).
 */

struct console_stdio_s {
	/* TZSH front-end call table */

	struct tzsh_vtbl_s cn_vtbl;

	/* TZSH input/output streams */

#if CONFIG_NFILE_STREAMS > 0
#ifdef CONFIG_TZSH_ALTCONDEV
	int cn_confd;				/* Console I/O file descriptor */
#endif
	int cn_outfd;				/* Output file descriptor (possibly redirected) */
#ifdef CONFIG_TZSH_ALTCONDEV
	FILE *cn_constream;			/* Console I/O stream (possibly redirected) */
#endif
	FILE *cn_outstream;			/* Output stream */
#endif

#ifdef CONFIG_TZSH_VARS
	/* Allocation and size of TZSH variables */

	FAR char *varp;
	size_t varsz;
#endif

	/* Line input buffer */

	char cn_line[CONFIG_TZSH_LINELEN];
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* Defined in tzsh_console.c *************************************************/

FAR struct console_stdio_s *tzsh_newconsole(void);

#endif							/* __APPS_TZSHLIB_TZSH_CONSOLE_H */
