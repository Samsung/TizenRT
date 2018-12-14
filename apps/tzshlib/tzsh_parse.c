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
 * apps/tzshlib/tzsh_parse.c
 *
 *   Copyright (C) 2007-2013, 2014, 2017-2018 Gregory Nutt. All rights
 *     reserved.
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

#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#ifdef CONFIG_TZSH_CMDPARMS
#include <sys/stat.h>
#endif

#include <tinyara/version.h>
#include "apps/tzshlib/tzshlib.h"

#include "tzsh.h"
#include "tzsh_console.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* If CONFIG_TZSH_CMDPARMS or CONFIG_TZSH_ARGCAT is enabled, then we will need
 * retain a list of memory allocations to be freed at the completion of
 * command processing.
 */

#undef HAVE_MEMLIST
#if defined(CONFIG_TZSH_CMDPARMS) || defined(CONFIG_TZSH_ARGCAT)
#define HAVE_MEMLIST 1
#endif

#if defined(HAVE_MEMLIST) && !defined(CONFIG_TZSH_MAXALLOCS)
#ifdef CONFIG_TZSH_ARGCAT
#define CONFIG_TZSH_MAXALLOCS (2*CONFIG_TZSH_MAXARGUMENTS)
#else
#define CONFIG_TZSH_MAXALLOCS CONFIG_TZSH_MAXARGUMENTS
#endif
#endif

/* Allocation list helper macros */

#ifdef HAVE_MEMLIST
#define TZSH_MEMLIST_TYPE      struct tzsh_memlist_s
#define TZSH_MEMLIST_INIT(m)   memset(&(m), 0, sizeof(struct tzsh_memlist_s));
#define TZSH_MEMLIST_ADD(m, a)  tzsh_memlist_add(m, a)
#define TZSH_MEMLIST_FREE(m)   tzsh_memlist_free(m)
#else
#define TZSH_MEMLIST_TYPE      uint8_t
#define TZSH_MEMLIST_INIT(m)   do { (m) = 0; } while (0)
#define TZSH_MEMLIST_ADD(m, a)
#define TZSH_MEMLIST_FREE(m)
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* These structure describes the parsed command line */

#ifndef CONFIG_TZSH_DISABLEBG
struct cmdarg_s {
	FAR struct tzsh_vtbl_s *vtbl;	/* For front-end interaction */
	int fd;						/* FD for output redirection */
	int argc;					/* Number of arguments in argv */
	FAR char *argv[MAX_ARGV_ENTRIES];	/* Argument list */
};
#endif

/* This structure describes the allocation list */

#ifdef HAVE_MEMLIST
struct tzsh_memlist_s {
	int nallocs;				/* Number of allocations */
	FAR char *allocations[CONFIG_TZSH_MAXALLOCS];
};
#endif

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

#ifdef HAVE_MEMLIST
static void tzsh_memlist_add(FAR struct tzsh_memlist_s *memlist, FAR char *allocation);
static void tzsh_memlist_free(FAR struct tzsh_memlist_s *memlist);
#endif

#ifndef CONFIG_TZSH_DISABLEBG
static void tzsh_releaseargs(struct cmdarg_s *arg);
static pthread_addr_t tzsh_child(pthread_addr_t arg);
static struct cmdarg_s *tzsh_cloneargs(FAR struct tzsh_vtbl_s *vtbl, int fd, int argc, char *argv[]);
#endif

static int tzsh_saveresult(FAR struct tzsh_vtbl_s *vtbl, bool result);
static int tzsh_execute(FAR struct tzsh_vtbl_s *vtbl, int argc, FAR char *argv[], FAR const char *redirfile, int oflags);

#ifdef CONFIG_TZSH_CMDPARMS
static FAR char *tzsh_filecat(FAR struct tzsh_vtbl_s *vtbl, FAR char *s1, FAR const char *filename);
static FAR char *tzsh_cmdparm(FAR struct tzsh_vtbl_s *vtbl, FAR char *cmdline, FAR char **allocation);
#endif

#ifdef CONFIG_TZSH_ARGCAT
static FAR char *tzsh_strcat(FAR struct tzsh_vtbl_s *vtbl, FAR char *s1, FAR const char *s2);
#endif

#if defined(CONFIG_TZSH_QUOTE) && defined(CONFIG_TZSH_ARGCAT)
static FAR char *tzsh_strchr(FAR const char *str, int ch);
#else
#define tzsh_strchr(s, c) strchr(s, c)
#endif

#ifdef TZSH_HAVE_VARS
static FAR char *tzsh_envexpand(FAR struct tzsh_vtbl_s *vtbl, FAR char *varname);
#endif

#if defined(CONFIG_TZSH_QUOTE) && defined(CONFIG_TZSH_ARGCAT)
static void tzsh_dequote(FAR char *cmdline);
#else
#define tzsh_dequote(c)
#endif

static FAR char *tzsh_argexpand(FAR struct tzsh_vtbl_s *vtbl, FAR char *cmdline, FAR char **allocation);
static FAR char *tzsh_argument(FAR struct tzsh_vtbl_s *vtbl, char **saveptr, FAR TZSH_MEMLIST_TYPE *memlist);

#ifndef CONFIG_TZSH_DISABLESCRIPT
#ifndef CONFIG_TZSH_DISABLE_LOOPS
static bool tzsh_loop_enabled(FAR struct tzsh_vtbl_s *vtbl);
#endif
#ifndef CONFIG_TZSH_DISABLE_ITEF
static bool tzsh_itef_enabled(FAR struct tzsh_vtbl_s *vtbl);
#endif
static bool tzsh_cmdenabled(FAR struct tzsh_vtbl_s *vtbl);
#ifndef CONFIG_TZSH_DISABLE_LOOPS
static int tzsh_loop(FAR struct tzsh_vtbl_s *vtbl, FAR char **ppcmd, FAR char **saveptr, FAR TZSH_MEMLIST_TYPE *memlist);
#endif
#ifndef CONFIG_TZSH_DISABLE_ITEF
static int tzsh_itef(FAR struct tzsh_vtbl_s *vtbl, FAR char **ppcmd, FAR char **saveptr, FAR TZSH_MEMLIST_TYPE *memlist);
#endif
#endif

#ifndef CONFIG_TZSH_DISABLEBG
static int tzsh_nice(FAR struct tzsh_vtbl_s *vtbl, FAR char **ppcmd, FAR char **saveptr, FAR TZSH_MEMLIST_TYPE *memlist);
#endif

#ifdef CONFIG_TZSH_CMDPARMS
static int tzsh_parse_cmdparm(FAR struct tzsh_vtbl_s *vtbl, FAR char *cmdline, FAR const char *redirfile);
#endif

static int tzsh_parse_command(FAR struct tzsh_vtbl_s *vtbl, FAR char *cmdline);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const char g_token_separator[] = " \t\n";
#ifndef TZSH_DISABLE_SEMICOLON
static const char g_line_separator[] = "\"#;\n";
#endif
#ifdef CONFIG_TZSH_ARGCAT
static const char g_arg_separator[] = "`$";
#endif
static const char g_redirect1[] = ">";
static const char g_redirect2[] = ">>";
#ifdef TZSH_HAVE_VARS
static const char g_exitstatus[] = "?";
static const char g_success[] = "0";
static const char g_failure[] = "1";
#endif
static const char g_nullstring[] = "";

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* If NuttX versioning information is available, Include that information
 * in the TZSH greeting.
 */

#if CONFIG_VERSION_MAJOR != 0 || CONFIG_VERSION_MINOR != 0
const char g_tzshgreeting[] = "\nNuttShell (TZSH) NuttX-" CONFIG_VERSION_STRING "\n";
#else
const char g_tzshgreeting[] = "\nTinyAraShell (TASH)\n";
#endif

/* Fixed Message of the Day (MOTD) */

#if defined(CONFIG_TZSH_MOTD) && !defined(CONFIG_TZSH_PLATFORM_MOTD)
const char g_tzshmotd[] = CONFIG_TZSH_MOTD_STRING;
#endif

/* Telnet login prompts */

#ifdef CONFIG_TZSH_LOGIN
#if defined(CONFIG_TZSH_TELNET_LOGIN) && defined(CONFIG_TZSH_TELNET)
const char g_telnetgreeting[] = "\nWelcome to NuttShell(TZSH) Telnet Server...\n";
#endif
const char g_userprompt[] = "login: ";
const char g_passwordprompt[] = "password: ";
const char g_loginsuccess[] = "\nUser Logged-in!\n";
const char g_badcredentials[] = "\nInvalid username or password\n";
const char g_loginfailure[] = "Login failed!\n";
#endif

/* The TZSH prompt */

const char g_tzshprompt[] = CONFIG_TZSH_PROMPT_STRING;

/* Common, message formats */

const char g_fmtsyntax[] = "tzsh: %s: syntax error\n";
const char g_fmtargrequired[] = "tzsh: %s: missing required argument(s)\n";
const char g_fmtnomatching[] = "tzsh: %s: no matching %s\n";
const char g_fmtarginvalid[] = "tzsh: %s: argument invalid\n";
const char g_fmtargrange[] = "tzsh: %s: value out of range\n";
const char g_fmtcmdnotfound[] = "tzsh: %s: command not found\n";
const char g_fmtnosuch[] = "tzsh: %s: no such %s: %s\n";
const char g_fmttoomanyargs[] = "tzsh: %s: too many arguments\n";
const char g_fmtdeepnesting[] = "tzsh: %s: nesting too deep\n";
const char g_fmtcontext[] = "tzsh: %s: not valid in this context\n";
#ifdef CONFIG_TZSH_STRERROR
const char g_fmtcmdfailed[] = "tzsh: %s: %s failed: %s\n";
#else
const char g_fmtcmdfailed[] = "tzsh: %s: %s failed: %d\n";
#endif
const char g_fmtcmdoutofmemory[] = "tzsh: %s: out of memory\n";
const char g_fmtinternalerror[] = "tzsh: %s: Internal error\n";
#ifndef CONFIG_DISABLE_SIGNALS
const char g_fmtsignalrecvd[] = "tzsh: %s: Interrupted by signal\n";
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_memlist_add
 ****************************************************************************/

#ifdef HAVE_MEMLIST
static void tzsh_memlist_add(FAR struct tzsh_memlist_s *memlist, FAR char *allocation)
{
	if (memlist && allocation) {
		int index = memlist->nallocs;
		if (index < CONFIG_TZSH_MAXALLOCS) {
			memlist->allocations[index] = allocation;
			memlist->nallocs = index + 1;
		}
	}
}
#endif

/****************************************************************************
 * Name: tzsh_memlist_free
 ****************************************************************************/

#ifdef HAVE_MEMLIST
static void tzsh_memlist_free(FAR struct tzsh_memlist_s *memlist)
{
	if (memlist) {
		int index;

		for (index = 0; index < memlist->nallocs; index++) {
			free(memlist->allocations[index]);
			memlist->allocations[index] = NULL;
		}

		memlist->nallocs = 0;
	}
}
#endif

/****************************************************************************
 * Name: tzsh_releaseargs
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLEBG
static void tzsh_releaseargs(struct cmdarg_s *arg)
{
	FAR struct tzsh_vtbl_s *vtbl = arg->vtbl;
	int i;

#if CONFIG_NFILE_STREAMS > 0
	/* If the output was redirected, then file descriptor should
	 * be closed.  The created task has its one, independent copy of
	 * the file descriptor
	 */

	if (vtbl->np.np_redirect) {
		(void)close(arg->fd);
	}
#endif

	/* Released the cloned vtbl instance */

	tzsh_release(vtbl);

	/* Release the cloned args */

	for (i = 0; i < arg->argc; i++) {
		free(arg->argv[i]);
	}

	free(arg);
}
#endif

/****************************************************************************
 * Name: tzsh_child
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLEBG
static pthread_addr_t tzsh_child(pthread_addr_t arg)
{
	struct cmdarg_s *carg = (struct cmdarg_s *)arg;
	int ret;

#ifdef CONFIG_DEBUG_FEATURES
	_info("BG %s\n", carg->argv[0]);
#endif

	/* Execute the specified command on the child thread */

	ret = tzsh_command(carg->vtbl, carg->argc, carg->argv);

	/* Released the cloned arguments */

#ifdef CONFIG_DEBUG_FEATURES
	_info("BG %s complete\n", carg->argv[0]);
#endif
	tzsh_releaseargs(carg);
	return (pthread_addr_t)((uintptr_t)ret);
}
#endif

/****************************************************************************
 * Name: tzsh_cloneargs
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLEBG
static struct cmdarg_s *tzsh_cloneargs(FAR struct tzsh_vtbl_s *vtbl, int fd, int argc, char *argv[])
{
	struct cmdarg_s *ret = (struct cmdarg_s *)zalloc(sizeof(struct cmdarg_s));
	int i;

	if (ret) {
		ret->vtbl = vtbl;
		ret->fd = fd;
		ret->argc = argc;

		for (i = 0; i < argc; i++) {
			ret->argv[i] = strdup(argv[i]);
		}
	}

	return ret;
}
#endif

/****************************************************************************
 * Name: tzsh_saveresult
 ****************************************************************************/

static int tzsh_saveresult(FAR struct tzsh_vtbl_s *vtbl, bool result)
{
	struct tzsh_parser_s *np = &vtbl->np;

#ifndef CONFIG_TZSH_DISABLESCRIPT
#ifndef CONFIG_TZSH_DISABLE_LOOPS
	/* Check if we are waiting for the condition associated with a while
	 * token.
	 *
	 *   while <test-cmd>; do <cmd-sequence>; done
	 *
	 *  Execute <cmd-sequence> as long as <test-cmd> has an exit status of
	 *  zero.
	 */

	if (np->np_lpstate[np->np_lpndx].lp_state == TZSH_LOOP_WHILE) {
		np->np_fail = false;
		np->np_lpstate[np->np_lpndx].lp_enable = (result == OK);
		return OK;
	}

	/* Check if we are waiting for the condition associated with an until
	 * token.
	 *
	 *   until <test-cmd>; do <cmd-sequence>; done
	 *
	 *  Execute <cmd-sequence> as long as <test-cmd> has a non-zero exit
	 * status.
	 */

	else if (np->np_lpstate[np->np_lpndx].lp_state == TZSH_LOOP_UNTIL) {
		np->np_fail = false;
		np->np_lpstate[np->np_lpndx].lp_enable = (result != OK);
		return OK;
	} else
#endif

#ifndef CONFIG_TZSH_DISABLE_ITEF
		/* Check if we are waiting for the condition associated with an if token */

		if (np->np_iestate[np->np_iendx].ie_state == TZSH_ITEF_IF) {
			np->np_fail = false;
			np->np_iestate[np->np_iendx].ie_ifcond = np->np_iestate[np->np_iendx].ie_inverted ^ result;
			return OK;
		} else
#endif
#endif
		{
			np->np_fail = result;
			return result ? ERROR : OK;
		}
}

/****************************************************************************
 * Name: tzsh_execute
 ****************************************************************************/

static int tzsh_execute(FAR struct tzsh_vtbl_s *vtbl, int argc, FAR char *argv[], FAR const char *redirfile, int oflags)
{
#if CONFIG_NFILE_STREAMS > 0 || !defined(CONFIG_TZSH_DISABLEBG)
	int fd = -1;
#endif
	int ret;

	/* Does this command correspond to an application filename?
	 * tzsh_fileapp() returns:
	 *
	 *   -1 (ERROR)  if the application task corresponding to 'argv[0]' could not
	 *               be started (possibly because it does not exist).
	 *    0 (OK)     if the application task corresponding to 'argv[0]' was
	 *               and successfully started.  If CONFIG_SCHED_WAITPID is
	 *               defined, this return value also indicates that the
	 *               application returned successful status (EXIT_SUCCESS)
	 *    1          If CONFIG_SCHED_WAITPID is defined, then this return value
	 *               indicates that the application task was spawned successfully
	 *               but returned failure exit status.
	 *
	 * Note the priority is not effected by nice-ness.
	 */

#ifdef CONFIG_TZSH_FILE_APPS
	ret = tzsh_fileapp(vtbl, argv[0], argv, redirfile, oflags);
	if (ret >= 0) {
		/* tzsh_fileapp() returned 0 or 1.  This means that the built-in
		 * command was successfully started (although it may not have ran
		 * successfully).  So certainly it is not an TZSH command.
		 */

		/* Save the result:  success if 0; failure if 1 */

		return tzsh_saveresult(vtbl, ret != OK);
	}

	/* No, not a file name command (or, at least, we were unable to start a
	 * program of that name).  Maybe it is a built-in application or an TZSH
	 * command.
	 */

#endif

	/* Does this command correspond to a built-in command?
	 * tzsh_builtin() returns:
	 *
	 *   -1 (ERROR)  if the application task corresponding to 'argv[0]' could not
	 *               be started (possibly because it doesn not exist).
	 *    0 (OK)     if the application task corresponding to 'argv[0]' was
	 *               and successfully started.  If CONFIG_SCHED_WAITPID is
	 *               defined, this return value also indicates that the
	 *               application returned successful status (EXIT_SUCCESS)
	 *    1          If CONFIG_SCHED_WAITPID is defined, then this return value
	 *               indicates that the application task was spawned successfully
	 *               but returned failure exit status.
	 *
	 * Note the priority if not effected by nice-ness.
	 */

#if defined(CONFIG_TZSH_BUILTIN_APPS) && (!defined(CONFIG_TZSH_FILE_APPS) || !defined(CONFIG_FS_BINFS))
#if CONFIG_NFILE_STREAMS > 0
	ret = tzsh_builtin(vtbl, argv[0], argv, redirfile, oflags);
#else
	ret = tzsh_builtin(vtbl, argv[0], argv, NULL, 0);
#endif
	if (ret >= 0) {
		/* tzsh_builtin() returned 0 or 1.  This means that the built-in
		 * command was successfully started (although it may not have ran
		 * successfully).  So certainly it is not an TZSH command.
		 */

		/* Save the result:  success if 0; failure if 1 */

		return tzsh_saveresult(vtbl, ret != OK);
	}

	/* No, not a built in command (or, at least, we were unable to start a
	 * built-in command of that name).  Treat it like an TZSH command.
	 */

#endif

#if CONFIG_NFILE_STREAMS > 0
	/* Redirected output? */

	if (vtbl->np.np_redirect) {
		/* Open the redirection file.  This file will eventually
		 * be closed by a call to either tzsh_release (if the command
		 * is executed in the background) or by tzsh_undirect if the
		 * command is executed in the foreground.
		 */

		fd = open(redirfile, oflags, 0666);
		if (fd < 0) {
			tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "open", TZSH_ERRNO);
			goto errout;
		}
	}
#endif

	/* Handle the case where the command is executed in background.
	 * However is app is to be started as built-in new process will
	 * be created anyway, so skip this step.
	 */

#ifndef CONFIG_TZSH_DISABLEBG
	if (vtbl->np.np_bg) {
		struct sched_param param;
		struct tzsh_vtbl_s *bkgvtbl;
		struct cmdarg_s *args;
		pthread_attr_t attr;
		pthread_t thread;

		/* Get a cloned copy of the vtbl with reference count=1.
		 * after the command has been processed, the tzsh_release() call
		 * at the end of tzsh_child() will destroy the clone.
		 */

		bkgvtbl = tzsh_clone(vtbl);
		if (!bkgvtbl) {
			goto errout_with_redirect;
		}

		/* Create a container for the command arguments */

		args = tzsh_cloneargs(bkgvtbl, fd, argc, argv);
		if (!args) {
			tzsh_release(bkgvtbl);
			goto errout_with_redirect;
		}
#if CONFIG_NFILE_STREAMS > 0
		/* Handle redirection of output via a file descriptor */

		if (vtbl->np.np_redirect) {
			(void)tzsh_redirect(bkgvtbl, fd, NULL);
		}
#endif

		/* Get the execution priority of this task */

		ret = sched_getparam(0, &param);
		if (ret != 0) {
			tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "sched_getparm", TZSH_ERRNO);
			tzsh_releaseargs(args);
			tzsh_release(bkgvtbl);
			goto errout;
		}

		/* Determine the priority to execute the command */

		if (vtbl->np.np_nice != 0) {
			int priority = param.sched_priority - vtbl->np.np_nice;
			if (vtbl->np.np_nice < 0) {
				int max_priority = sched_get_priority_max(SCHED_TZSH);
				if (priority > max_priority) {
					priority = max_priority;
				}
			} else {
				int min_priority = sched_get_priority_min(SCHED_TZSH);
				if (priority < min_priority) {
					priority = min_priority;
				}
			}

			param.sched_priority = priority;
		}

		/* Set up the thread attributes */

		(void)pthread_attr_init(&attr);
		(void)pthread_attr_setschedpolicy(&attr, SCHED_TZSH);
		(void)pthread_attr_setschedparam(&attr, &param);

		/* Execute the command as a separate thread at the appropriate priority */

		ret = pthread_create(&thread, &attr, tzsh_child, (pthread_addr_t)args);
		if (ret != 0) {
			tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "pthread_create", TZSH_ERRNO_OF(ret));
			tzsh_releaseargs(args);
			tzsh_release(bkgvtbl);
			goto errout;
		}

		/* Detach from the pthread since we are not going to join with it.
		 * Otherwise, we would have a memory leak.
		 */

		(void)pthread_detach(thread);

		tzsh_output(vtbl, "%s [%d:%d]\n", argv[0], thread, param.sched_priority);
	} else
#endif
	{
#if CONFIG_NFILE_STREAMS > 0
		uint8_t save[SAVE_SIZE];

		/* Handle redirection of output via a file descriptor */

		if (vtbl->np.np_redirect) {
			tzsh_redirect(vtbl, fd, save);
		}
#endif

		/* Then execute the command in "foreground" -- i.e., while the user waits
		 * for the next prompt.  tzsh_command will return:
		 *
		 * -1 (ERRROR) if the command was unsuccessful
		 *  0 (OK)     if the command was successful
		 */

		ret = tzsh_command(vtbl, argc, argv);

#if CONFIG_NFILE_STREAMS > 0
		/* Restore the original output.  Undirect will close the redirection
		 * file descriptor.
		 */

		if (vtbl->np.np_redirect) {
			tzsh_undirect(vtbl, save);
		}
#endif

		/* Mark errors so that it is possible to test for non-zero return values
		 * in tzsh scripts.
		 */

		if (ret < 0) {
			goto errout;
		}
	}

	/* Return success if the command succeeded (or at least, starting of the
	 * command task succeeded).
	 */

	return tzsh_saveresult(vtbl, false);

#ifndef CONFIG_TZSH_DISABLEBG
errout_with_redirect:
#if CONFIG_NFILE_STREAMS > 0
	if (vtbl->np.np_redirect) {
		close(fd);
	}
#endif
#endif

errout:
	return tzsh_saveresult(vtbl, true);
}

/****************************************************************************
 * Name: tzsh_filecat
 ****************************************************************************/

#ifdef CONFIG_TZSH_CMDPARMS
static FAR char *tzsh_filecat(FAR struct tzsh_vtbl_s *vtbl, FAR char *s1, FAR const char *filename)
{
	struct stat buf;
	size_t s1size = 0;
	size_t allocsize;
	ssize_t nbytesread;
	FAR char *argument;
	int index;
	int fd;
	int ret;

	/* Get the size of the string */

	if (s1) {
		s1size = (size_t)strlen(s1);
	}

	/* Get the size of file */

	ret = stat(filename, &buf);
	if (ret != 0) {
		tzsh_output(vtbl, g_fmtcmdfailed, "``", "stat", TZSH_ERRNO);
		return NULL;
	}

	/* Get the total allocation size */

	allocsize = s1size + (size_t)buf.st_size + 1;
	argument = (FAR char *)realloc(s1, allocsize);
	if (!argument) {
		tzsh_output(vtbl, g_fmtcmdoutofmemory, "``");
		return NULL;
	}

	/* Open the source file for reading */

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		tzsh_output(vtbl, g_fmtcmdfailed, "``", "open", TZSH_ERRNO);
		goto errout_with_alloc;
	}

	/* Now copy the file.  Loop until the entire file has been transferred to
	 * the allocated string (after the original contents of s1size bytes.
	 */

	for (index = s1size; index < allocsize - 1;) {
		/* Loop until we successfully read something , we encounter the
		 * end-of-file, or until a read error occurs
		 */

		do {
			nbytesread = read(fd, &argument[index], IOBUFFERSIZE);
			if (nbytesread == 0) {
				/* Unexpected end of file -- Break out of the loop */

				break;
			} else if (nbytesread < 0) {
				/* EINTR is not an error (but will still stop the copy) */

#ifndef CONFIG_DISABLE_SIGNALS
				if (errno == EINTR) {
					tzsh_output(vtbl, g_fmtsignalrecvd, "``");
				} else
#endif
				{
					/* Read error */

					tzsh_output(vtbl, g_fmtcmdfailed, "``", "read", TZSH_ERRNO);
				}

				goto errout_with_fd;
			}
		} while (nbytesread <= 0);

		/* Update the index based upon the number of bytes read */

		index += nbytesread;
	}

	/* Make sure that the new string is null terminated */

	argument[index] = '\0';

	/* Close the temporary file and return the concatenated value */

	close(fd);
	return argument;

errout_with_fd:
	close(fd);

errout_with_alloc:
	free(argument);
	return NULL;
}
#endif

/****************************************************************************
 * Name: tzsh_cmdparm
 ****************************************************************************/

#ifdef CONFIG_TZSH_CMDPARMS
static FAR char *tzsh_cmdparm(FAR struct tzsh_vtbl_s *vtbl, FAR char *cmdline, FAR char **allocation)
{
	FAR char *tmpfile;
	FAR char *argument;
	int ret;

	/* We cannot process the command argument if there is no allocation pointer */

	if (!allocation) {
		return (FAR char *)g_nullstring;
	}

	/* Create a unique file name using the task ID */

	tmpfile = NULL;
	ret = asprintf(&tmpfile, "%s/TMP%d.dat", CONFIG_LIBC_TMPDIR, getpid());
	if (ret < 0 || !tmpfile) {
		tzsh_output(vtbl, g_fmtcmdoutofmemory, "``");
		return (FAR char *)g_nullstring;
	}

	/* Execute the command that will re-direct the output of the command to
	 * the temporary file.  This is a simple command that can't handle most
	 * options.
	 */

	ret = tzsh_parse_cmdparm(vtbl, cmdline, tmpfile);
	if (ret != OK) {
		/* Report the failure */

		tzsh_output(vtbl, g_fmtcmdfailed, "``", "exec", TZSH_ERRNO);
		free(tmpfile);
		return (FAR char *)g_nullstring;
	}

	/* Concatenate the file contents with the current allocation */

	argument = tzsh_filecat(vtbl, *allocation, tmpfile);
	*allocation = argument;

	/* We can now unlink the tmpfile and free the tmpfile string */

	ret = unlink(tmpfile);
	if (ret < 0) {
		tzsh_output(vtbl, g_fmtcmdfailed, "``", "unlink", TZSH_ERRNO);
	}

	free(tmpfile);
	return argument;
}
#endif

/****************************************************************************
 * Name: tzsh_strcat
 ****************************************************************************/

#ifdef CONFIG_TZSH_ARGCAT
static FAR char *tzsh_strcat(FAR struct tzsh_vtbl_s *vtbl, FAR char *s1, FAR const char *s2)
{
	FAR char *argument;
	int s1size = 0;
	int allocsize;

	/* Get the size of the first string... it might be NULL */

	if (s1) {
		s1size = strlen(s1);
	}

	/* Then reallocate the first string so that it is large enough to hold
	 * both (including the NUL terminator).
	 */

	allocsize = s1size + strlen(s2) + 1;
	argument = (FAR char *)realloc(s1, allocsize);
	if (!argument) {
		tzsh_output(vtbl, g_fmtcmdoutofmemory, "$");
		argument = s1;
	} else {
		argument[s1size] = '\0';	/* (In case s1 was NULL) */
		strcat(argument, s2);
	}

	return argument;
}
#endif

/****************************************************************************
 * Name: tzsh_strchr
 ****************************************************************************/

#if defined(CONFIG_TZSH_QUOTE) && defined(CONFIG_TZSH_ARGCAT)
static FAR char *tzsh_strchr(FAR const char *str, int ch)
{
	FAR const char *ptr;
	bool quoted = false;

	for (ptr = str;; ptr++) {
		if (*ptr == '\\' && !quoted) {
			quoted = true;
		} else if ((int)*ptr == ch && !quoted) {
			return (FAR char *)ptr;
		} else if (*ptr == '\0') {
			return NULL;
		} else {
			quoted = false;
		}
	}
}
#endif

/****************************************************************************
 * Name: tzsh_envexpand
 ****************************************************************************/

#ifdef TZSH_HAVE_VARS
static FAR char *tzsh_envexpand(FAR struct tzsh_vtbl_s *vtbl, FAR char *varname)
{
	/* Check for built-in variables */

	if (strcmp(varname, g_exitstatus) == 0) {
		if (vtbl->np.np_fail) {
			return (FAR char *)g_failure;
		} else {
			return (FAR char *)g_success;
		}
	} else {
		FAR char *value;

#ifdef CONFIG_TZSH_VARS
		/* Not a built-in? Return the value of the TZSH variable with this
		 * name.
		 */

		value = tzsh_getvar(vtbl, varname);
		if (value != NULL) {
			return value;
		}
#endif

#ifndef CONFIG_DISABLE_ENVIRON
		/* Not an TZSH variable? Return the value of the TZSH variable
		 * environment variable with this name.
		 */

		value = getenv(varname);
		if (value != NULL) {
			return value;
		}
#endif

		return (FAR char *)g_nullstring;
	}
}
#endif

/****************************************************************************
 * Name: tzsh_dequote
 ****************************************************************************/

#if defined(CONFIG_TZSH_QUOTE) && defined(CONFIG_TZSH_ARGCAT)
static void tzsh_dequote(FAR char *cmdline)
{
	FAR char *ptr;
	bool quoted;

	quoted = false;

	for (ptr = cmdline; *ptr != '\0';) {
		if (*ptr == '\\' && !quoted) {
			FAR char *dest = ptr;
			FAR const char *src = ptr + 1;
			char ch;

			/* Move the data to eliminate the quote from the command line */

			do {
				ch = *src++;
				*dest++ = ch;
			} while (ch != '\0');

			/* Remember that the next character is quote (in case it is
			 * another back-slash character).
			 */

			quoted = true;
			continue;
		} else {
			/* The next character is not quoted because either (1) it was not
			 * preceded by a back-slash, or (2) it was preceded by a quoted
			 * back-slash.
			 */

			quoted = false;
			ptr++;
		}
	}

	/* Make sure that the new, possibly shorted string is NUL terminated */

	*ptr = '\0';
}
#endif

/****************************************************************************
 * Name: tzsh_argexpand
 ****************************************************************************/

#if defined(CONFIG_TZSH_ARGCAT) && defined(HAVE_MEMLIST)
static FAR char *tzsh_argexpand(FAR struct tzsh_vtbl_s *vtbl, FAR char *cmdline, FAR char **allocation)
{
	FAR char *working = cmdline;
#ifdef CONFIG_TZSH_QUOTE
	FAR char *nextwork;
#endif
	FAR char *argument = NULL;
	FAR char *ptr;
	size_t len;

	/* Loop until all of the commands on the command line have been processed */

	for (;;) {
		/* Look for interesting things within the command string. */

		len = strcspn(working, g_arg_separator);
		ptr = working + len;
#ifdef CONFIG_TZSH_QUOTE
		nextwork = ptr + 1;

		/* But ignore these interesting things if they are quoted */

		while (len > 0 && *ptr != '\0') {
			FAR char *prev = working + len - 1;
			int bcount;
			bool quoted;

			/* Check if the current character is quoted */

			for (bcount = 0, quoted = false; bcount < len && *prev == '\\'; bcount++, prev--) {
				quoted ^= true;
			}

			if (quoted) {
				/* Yes.. skip over it */

				len += strcspn(ptr + 1, g_arg_separator) + 1;
				ptr = working + len;
				nextwork = ptr + 1;
			} else {
				/* Not quoted.. subject to normal processing */

				break;
			}
		}
#endif

		/* If ptr points to the NUL terminator, then there is nothing else
		 * interesting in the argument.
		 */

		if (*ptr == '\0') {
			/* Was anything previously concatenated? */

			if (argument) {
				/* Yes, then we probably need to add the last part of the argument
				 * beginning at the last working pointer to the concatenated
				 * argument.
				 *
				 * On failures to allocation memory, tzsh_strcat will just return
				 * old value of argument
				 */

				argument = tzsh_strcat(vtbl, argument, working);
				*allocation = argument;

				/* De-quote the returned string */

				tzsh_dequote(argument);
				return argument;
			} else {
				/* No.. just return the original string from the command line. */

				tzsh_dequote(cmdline);
				return cmdline;
			}
		} else
#ifdef CONFIG_TZSH_CMDPARMS
			/* Check for a back-quoted command embedded within the argument string. */

			if (*ptr == '`') {
				FAR char *tmpalloc = NULL;
				FAR char *result;
				FAR char *rptr;

				/* Replace the back-quote with a NUL terminator and add the
				 * intervening character to the concatenated string.
				 */

				*ptr++ = '\0';
				argument = tzsh_strcat(vtbl, argument, working);
				*allocation = argument;

				/* Find the closing back-quote (must be unquoted) */

				rptr = tzsh_strchr(ptr, '`');
				if (!rptr) {
					tzsh_output(vtbl, g_fmtnomatching, "`", "`");
					return (FAR char *)g_nullstring;
				}

				/* Replace the final back-quote with a NUL terminator */

				*rptr = '\0';

				/* Then execute the command to get the sub-string value.  On
				 * error, tzsh_cmdparm may return g_nullstring but never NULL.
				 */

				result = tzsh_cmdparm(vtbl, ptr, &tmpalloc);

				/* Concatenate the result of the operation with the accumulated
				 * string.  On failures to allocation memory, tzsh_strcat will
				 * just return old value of argument
				 */

				argument = tzsh_strcat(vtbl, argument, result);
				*allocation = argument;
				working = rptr + 1;

				/* And free any temporary allocations */

				if (tmpalloc) {
					free(tmpalloc);
				}
			} else
#endif

#ifdef TZSH_HAVE_VARS
				/* Check if we encountered a reference to an environment variable */

				if (*ptr == '$') {
					FAR const char *envstr;
					FAR char *rptr;

					/* Replace the dollar sign with a NUL terminator and add the
					 * intervening character to the concatenated string.
					 */

					*ptr++ = '\0';
					argument = tzsh_strcat(vtbl, argument, working);
					*allocation = argument;

					/* Find the end of the environment variable reference.  If the
					 * dollar sign ('$') is followed by a right bracket ('{') then the
					 * variable name is terminated with the left bracket character
					 * ('}').  Otherwise, the variable name goes to the end of the
					 * argument.
					 */

					if (*ptr == '{') {
						/* Skip over the left bracket */

						ptr++;

						/* Find the closing right bracket */

						rptr = tzsh_strchr(ptr, '}');
						if (!rptr) {
							tzsh_output(vtbl, g_fmtnomatching, "${", "}");
							return (FAR char *)g_nullstring;
						}

						/* Replace the right bracket with a NUL terminator and set the
						 * working pointer to the character after the bracket.
						 */

						*rptr = '\0';
						working = rptr + 1;
					} else {
						/* Set working to the NUL terminator at the end of the string.
						 *
						 * REVISIT:  Needs logic to get the size of the variable name
						 * based on parsing the name string which must be of the form
						 * [a-zA-Z_]+[a-zA-Z0-9_]*
						 */

						working = ptr + strlen(ptr);
					}

					/* Then get the value of the environment variable.  On errors,
					 * tzsh_envexpand will return the NULL string.
					 */

					envstr = tzsh_envexpand(vtbl, ptr);

#ifndef CONFIG_TZSH_DISABLESCRIPT
					if ((vtbl->np.np_flags & TZSH_PFLAG_SILENT) == 0)
#endif
					{
						tzsh_output(vtbl, "  %s=%s\n", ptr, envstr ? envstr : "(null)");
					}

					/* Concatenate the result of the operation with the accumulated
					 * string.  On failures to allocation memory, tzsh_strcat will
					 * just return old value of argument
					 */

					argument = tzsh_strcat(vtbl, argument, envstr);
					*allocation = argument;
				} else
#endif
				{
					/* Not a special character... skip to the next character in the
					 * cmdline.
					 */

#ifdef CONFIG_TZSH_QUOTE
					working = nextwork;
#else
					working++;
#endif
				}
	}
}

#else
static FAR char *tzsh_argexpand(FAR struct tzsh_vtbl_s *vtbl, FAR char *cmdline, FAR char **allocation)
{
	FAR char *argument = (FAR char *)g_nullstring;
#ifdef CONFIG_TZSH_QUOTE
	char ch = *cmdline;

	/* A single backslash at the beginning of the line is support, nothing
	 * more.
	 */

	tzsh_dequote(cmdline);
	if (ch == '\\') {
		argument = cmdline;
	} else
#endif

#ifdef CONFIG_TZSH_CMDPARMS
		/* Are we being asked to use the output from another command or program
		 * as an input parameters for this command?
		 */

		if (*cmdline == '`') {
			/* Verify that the final character is also a back-quote */

			FAR char *rptr = tzsh_strchr(cmdline + 1, '`');
			if (!rptr || rptr[1] != '\0') {
				tzsh_output(vtbl, g_fmtnomatching, "`", "`");
				return (FAR char *)g_nullstring;
			}

			/* Replace the final back-quote with a NUL terminator */

			*rptr = '\0';

			/* Then execute the command to get the parameter value */

			argument = tzsh_cmdparm(vtbl, cmdline + 1, allocation);
		} else
#endif

#ifdef TZSH_HAVE_VARS
			/* Check for references to environment variables */

			if (*cmdline == '$') {
				argument = tzsh_envexpand(vtbl, cmdline + 1);
			} else
#endif
			{
				/* The argument to be returned is simply the beginning of the
				 * delimited string.
				 */

				argument = cmdline;
			}

	return argument;
}
#endif

/****************************************************************************
 * Name: tzsh_argument
 ****************************************************************************/

static FAR char *tzsh_argument(FAR struct tzsh_vtbl_s *vtbl, FAR char **saveptr, FAR TZSH_MEMLIST_TYPE *memlist)
{
	FAR char *pbegin = *saveptr;
	FAR char *pend = NULL;
	FAR char *allocation = NULL;
	FAR char *argument = NULL;
	FAR const char *term;
#ifdef CONFIG_TZSH_QUOTE
	FAR char *prev;
	bool quoted;
#endif
#ifdef CONFIG_TZSH_CMDPARMS
	bool backquote;
#endif

	/* Find the beginning of the next token */

	for (; *pbegin && strchr(g_token_separator, *pbegin) != NULL; pbegin++) ;

	/* If we are at the end of the string with nothing but delimiters found,
	 * then return NULL, meaning that there are no further arguments on the line.
	 */

	if (!*pbegin) {
		return NULL;
	}

	/* Does the token begin with '>' -- redirection of output? */

	if (*pbegin == '>') {
		/* Yes.. does it begin with ">>"? */

		if (*(pbegin + 1) == '>') {
			*saveptr = pbegin + 2;
			argument = (FAR char *)g_redirect2;
		} else {
			*saveptr = pbegin + 1;
			argument = (FAR char *)g_redirect1;
		}
	}

	/* Does the token begin with '#' -- comment */

	else if (*pbegin == '#') {
		/* Return NULL meaning that we are at the end of the line */

		*saveptr = pbegin;
		argument = NULL;
	}

	/* Otherwise, it is a normal argument and we have to parse using the normal
	 * rules to find the end of the argument.
	 */

	else {
		/* However, the rules are a little different if the next argument is
		 * a quoted string.
		 */

		if (*pbegin == '"') {
			/* A quoted string can only be terminated with another quotation
			 * mark.  Set pbegin to point at the character after the opening
			 * quote mark.
			 */

			pbegin++;
			term = "\"";
		} else {
			/* No, then any of the usual separators will terminate the
			 * argument.  In this case, pbegin points for the first character
			 * of the token following the previous separator.
			 */

			term = g_token_separator;
		}

		/* Find the end of the string */

#ifdef CONFIG_TZSH_CMDPARMS
		/* Some special care must be exercised to make sure that we do not break up
		 * any back-quote delimited substrings.  NOTE that the absence of a closing
		 * back-quote is not detected;  That case should be detected later.
		 */

#ifdef CONFIG_TZSH_QUOTE
		quoted = false;
		backquote = false;

		for (prev = NULL, pend = pbegin; *pend != '\0'; prev = pend, pend++) {
			/* Check if the current character is quoted */

			if (prev != NULL && *prev == '\\' && !quoted) {
				/* Do no special checks on the quoted character */

				quoted = true;
				continue;
			}

			quoted = false;

			/* Check if the current character is an (unquoted) back-quote */

			if (*pend == '\\' && !quoted) {
				/* Yes.. Do no special processing on the backspace character */

				continue;
			}

			/* Toggle the back-quote flag when one is encountered? */

			if (*pend == '`') {
				backquote = !backquote;
			}

			/* Check for a delimiting character only if we are not in a
			 * back-quoted sub-string.
			 */

			else if (!backquote && tzsh_strchr(term, *pend) != NULL) {
				/* We found a delimiter outside of any back-quoted substring.
				 * Now we can break out of the loop.
				 */

				break;
			}
		}
#else
		backquote = false;

		for (pend = pbegin; *pend != '\0'; pend++) {
			/* Toggle the back-quote flag when one is encountered? */

			if (*pend == '`') {
				backquote = !backquote;
			}

			/* Check for a delimiting character only if we are not in a
			 * back-quoted sub-string.
			 */

			else if (!backquote && tzsh_strchr(term, *pend) != NULL) {
				/* We found a delimiter outside of any back-quoted substring.
				 * Now we can break out of the loop.
				 */

				break;
			}
		}

#endif							/* CONFIG_TZSH_QUOTE */
#else							/* CONFIG_TZSH_CMDPARMS */

		/* Search the next occurrence of a terminating character (or the end
		 * of the line).
		 */

#ifdef CONFIG_TZSH_QUOTE
		quoted = false;

		for (prev = NULL, pend = pbegin; *pend != '\0'; prev = pend, pend++) {
			/* Check if the current character is quoted */

			if (prev != NULL && *prev == '\\' && !quoted) {

				/* Do no special checks on the quoted character */

				quoted = true;
				continue;
			}

			quoted = false;

			/* Check if the current character is an (unquoted) back-quote */

			if (*pend == '\\' && !quoted) {
				/* Yes.. Do no special processing on the backspace character */

				continue;
			}

			/* Check for a delimiting character */

			if (tzsh_strchr(term, *pend) != NULL) {
				/* We found a delimiter. Now we can break out of the loop. */

				break;
			}
		}

#else

		for (pend = pbegin; *pend != '\0' && tzsh_strchr(term, *pend) == NULL; pend++) {
		}

#endif							/* CONFIG_TZSH_QUOTE */
#endif							/* CONFIG_TZSH_CMDPARMS */

		/* pend either points to the end of the string or to the first
		 * delimiter after the string.
		 */

		if (*pend) {
			/* Turn the delimiter into a NUL terminator */

			*pend++ = '\0';
		}

		/* Save the pointer where we left off */

		*saveptr = pend;

		/* Perform expansions as necessary for the argument */

		argument = tzsh_argexpand(vtbl, pbegin, &allocation);
	}

	/* If any memory was allocated for this argument, make sure that it is
	 * added to the list of memory to be freed at the end of commend
	 * processing.
	 */

	TZSH_MEMLIST_ADD(memlist, allocation);

	/* Return the parsed argument. */

	return argument;
}

/****************************************************************************
 * Name: tzsh_loop_enabled
 ****************************************************************************/

#if !defined(CONFIG_TZSH_DISABLESCRIPT) && !defined(CONFIG_TZSH_DISABLE_LOOPS)
static bool tzsh_loop_enabled(FAR struct tzsh_vtbl_s *vtbl)
{
	FAR struct tzsh_parser_s *np = &vtbl->np;

	/* If we are looping and the disable bit is set, then we are skipping
	 * all data until we next get to the 'done' token at the end of the
	 * loop.
	 */

	if (np->np_lpstate[np->np_lpndx].lp_state == TZSH_LOOP_DO) {
		/* We have parsed 'do', looking for 'done' */

		return (bool) np->np_lpstate[np->np_lpndx].lp_enable;
	}

	return true;
}
#else
#define tzsh_loop_enabled(vtbl) true
#endif

/****************************************************************************
 * Name: tzsh_itef_enabled
 ****************************************************************************/

#if !defined(CONFIG_TZSH_DISABLESCRIPT) && !defined(CONFIG_TZSH_DISABLE_ITEF)
static bool tzsh_itef_enabled(FAR struct tzsh_vtbl_s *vtbl)
{
	FAR struct tzsh_parser_s *np = &vtbl->np;
	bool ret = !np->np_iestate[np->np_iendx].ie_disabled;
	if (ret) {
		switch (np->np_iestate[np->np_iendx].ie_state) {
		case TZSH_ITEF_NORMAL:
		case TZSH_ITEF_IF:
		default:
			break;

		case TZSH_ITEF_THEN:
			ret = !np->np_iestate[np->np_iendx].ie_ifcond;
			break;

		case TZSH_ITEF_ELSE:
			ret = np->np_iestate[np->np_iendx].ie_ifcond;
			break;
		}
	}

	return ret;
}
#else
#define tzsh_itef_enabled(vtbl) true
#endif

/****************************************************************************
 * Name: tzsh_cmdenabled
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLESCRIPT
static bool tzsh_cmdenabled(FAR struct tzsh_vtbl_s *vtbl)
{
	/* Return true if command processing is enabled on this pass through the
	 * loop AND if command processing is enabled in this part of the if-then-
	 * else-fi sequence.
	 */

	return (tzsh_loop_enabled(vtbl) && tzsh_itef_enabled(vtbl));
}
#endif

/****************************************************************************
 * Name: tzsh_loop
 ****************************************************************************/

#if !defined(CONFIG_TZSH_DISABLESCRIPT) && !defined(CONFIG_TZSH_DISABLE_LOOPS)
static int tzsh_loop(FAR struct tzsh_vtbl_s *vtbl, FAR char **ppcmd, FAR char **saveptr, FAR TZSH_MEMLIST_TYPE *memlist)
{
	FAR struct tzsh_parser_s *np = &vtbl->np;
	FAR char *cmd = *ppcmd;
	long offset;
	bool whilematch;
	bool untilmatch;
	bool enable;
	int ret;

	if (cmd != NULL) {
		/* Check if the command is preceded by "while" or "until" */

		whilematch = strcmp(cmd, "while") == 0;
		untilmatch = strcmp(cmd, "until") == 0;

		if (whilematch || untilmatch) {
			uint8_t state;

			/* Get the cmd following the "while" or "until" */

			*ppcmd = tzsh_argument(vtbl, saveptr, memlist);
			if (*ppcmd == NULL || **ppcmd == '\0') {
				tzsh_output(vtbl, g_fmtarginvalid, cmd);
				goto errout;
			}

			/* Verify that "while" or "until" is valid in this context */

			if (
#ifndef CONFIG_TZSH_DISABLE_ITEF
				np->np_iestate[np->np_iendx].ie_state == TZSH_ITEF_IF ||
#endif
				np->np_lpstate[np->np_lpndx].lp_state == TZSH_LOOP_WHILE || np->np_lpstate[np->np_lpndx].lp_state == TZSH_LOOP_UNTIL || np->np_stream == NULL || np->np_foffs < 0) {
				tzsh_output(vtbl, g_fmtcontext, cmd);
				goto errout;
			}

			/* Check if we have exceeded the maximum depth of nesting */

			if (np->np_lpndx >= CONFIG_TZSH_NESTDEPTH - 1) {
				tzsh_output(vtbl, g_fmtdeepnesting, cmd);
				goto errout;
			}

			/* "Push" the old state and set the new state */

			state = whilematch ? TZSH_LOOP_WHILE : TZSH_LOOP_UNTIL;
			enable = tzsh_cmdenabled(vtbl);
#ifdef TZSH_DISABLE_SEMICOLON
			offset = np->np_foffs;
#else
			offset = np->np_foffs + np->np_loffs;
#endif

#ifndef TZSH_DISABLE_SEMICOLON
			np->np_jump = false;
#endif
			np->np_lpndx++;
			np->np_lpstate[np->np_lpndx].lp_state = state;
			np->np_lpstate[np->np_lpndx].lp_enable = enable;
#ifndef CONFIG_TZSH_DISABLE_ITEF
			np->np_lpstate[np->np_lpndx].lp_iendx = np->np_iendx;
#endif
			np->np_lpstate[np->np_lpndx].lp_topoffs = offset;
		}

		/* Check if the token is "do" */

		else if (strcmp(cmd, "do") == 0) {
			/* Get the cmd following the "do" -- there may or may not be one */

			*ppcmd = tzsh_argument(vtbl, saveptr, memlist);

			/* Verify that "do" is valid in this context */

			if (np->np_lpstate[np->np_lpndx].lp_state != TZSH_LOOP_WHILE && np->np_lpstate[np->np_lpndx].lp_state != TZSH_LOOP_UNTIL) {
				tzsh_output(vtbl, g_fmtcontext, "do");
				goto errout;
			}

			np->np_lpstate[np->np_lpndx].lp_state = TZSH_LOOP_DO;
		}

		/* Check if the token is "done" */

		else if (strcmp(cmd, "done") == 0) {
			/* Get the cmd following the "done" -- there should be one */

			*ppcmd = tzsh_argument(vtbl, saveptr, memlist);
			if (*ppcmd) {
				tzsh_output(vtbl, g_fmtarginvalid, "done");
				goto errout;
			}

			/* Verify that "done" is valid in this context */

			if (np->np_lpstate[np->np_lpndx].lp_state != TZSH_LOOP_DO) {
				tzsh_output(vtbl, g_fmtcontext, "done");
				goto errout;
			}

			if (np->np_lpndx < 1) {	/* Shouldn't happen */
				tzsh_output(vtbl, g_fmtinternalerror, "done");
				goto errout;
			}

			/* Now what do we do?  We either:  Do go back to the top of the
			 * loop (if lp_enable == true) or continue past the end of the
			 * loop (if lp_enable == false)
			 */

			if (np->np_lpstate[np->np_lpndx].lp_enable) {
				/* Set the new file position to the top of the loop offset */

				ret = fseek(np->np_stream, np->np_lpstate[np->np_lpndx].lp_topoffs, SEEK_SET);
				if (ret < 0) {
					tzsh_output(vtbl, g_fmtcmdfailed, "done", "fseek", TZSH_ERRNO);
				}
#ifndef TZSH_DISABLE_SEMICOLON
				/* Signal tzsh_parse that we need to stop processing the
				 * current line and jump back to the top of the loop.
				 */

				np->np_jump = true;
#endif
			} else {
				np->np_lpstate[np->np_lpndx].lp_enable = true;
			}

			/* "Pop" the previous state.  We do this no matter what we
			 * decided to do
			 */

			np->np_lpstate[np->np_lpndx].lp_state = TZSH_LOOP_NORMAL;
			np->np_lpndx--;
		}

		/* If we just parsed "while" or "until", then nothing is acceptable
		 * other than "do"
		 */

		else if (np->np_lpstate[np->np_lpndx].lp_state == TZSH_LOOP_WHILE || np->np_lpstate[np->np_lpndx].lp_state == TZSH_LOOP_UNTIL) {
			tzsh_output(vtbl, g_fmtcontext, cmd);
			goto errout;
		}
	}

	return OK;

errout:
#ifndef TZSH_DISABLE_SEMICOLON
	np->np_jump = false;
#endif
	np->np_lpndx = 0;
	np->np_lpstate[0].lp_state = TZSH_LOOP_NORMAL;
	np->np_lpstate[0].lp_enable = true;
	np->np_lpstate[0].lp_topoffs = 0;
	return ERROR;
}
#endif

/****************************************************************************
 * Name: tzsh_itef
 ****************************************************************************/

#if !defined(CONFIG_TZSH_DISABLESCRIPT) && !defined(CONFIG_TZSH_DISABLE_ITEF)
static int tzsh_itef(FAR struct tzsh_vtbl_s *vtbl, FAR char **ppcmd, FAR char **saveptr, FAR TZSH_MEMLIST_TYPE *memlist)
{
	FAR struct tzsh_parser_s *np = &vtbl->np;
	FAR char *cmd = *ppcmd;
	bool disabled;
	bool inverted = false;

	if (cmd != NULL) {
		/* Check if the command is preceded by "if" */

		if (strcmp(cmd, "if") == 0) {
			/* Get the cmd following the if */

			*ppcmd = tzsh_argument(vtbl, saveptr, memlist);
			if (*ppcmd == NULL || **ppcmd == '\0') {
				tzsh_output(vtbl, g_fmtarginvalid, "if");
				goto errout;
			}

			/* Check for inverted logic */

			if (strcmp(*ppcmd, "!") == 0) {
				inverted = true;

				/* Get the next cmd */

				*ppcmd = tzsh_argument(vtbl, saveptr, memlist);
				if (*ppcmd == NULL || **ppcmd == '\0') {
					tzsh_output(vtbl, g_fmtarginvalid, "if");
					goto errout;
				}
			}

			/* Verify that "if" is valid in this context */

			if (np->np_iestate[np->np_iendx].ie_state == TZSH_ITEF_IF) {
				tzsh_output(vtbl, g_fmtcontext, "if");
				goto errout;
			}

			/* Check if we have exceeded the maximum depth of nesting */

			if (np->np_iendx >= CONFIG_TZSH_NESTDEPTH - 1) {
				tzsh_output(vtbl, g_fmtdeepnesting, "if");
				goto errout;
			}

			/* "Push" the old state and set the new state */

			disabled = !tzsh_cmdenabled(vtbl);
			np->np_iendx++;
			np->np_iestate[np->np_iendx].ie_state = TZSH_ITEF_IF;
			np->np_iestate[np->np_iendx].ie_disabled = disabled;
			np->np_iestate[np->np_iendx].ie_ifcond = false;
			np->np_iestate[np->np_iendx].ie_inverted = inverted;
		}

		/* Check if the token is "then" */

		else if (strcmp(cmd, "then") == 0) {
			/* Get the cmd following the "then" -- there may or may not be one */

			*ppcmd = tzsh_argument(vtbl, saveptr, memlist);

			/* Verify that "then" is valid in this context */

			if (np->np_iestate[np->np_iendx].ie_state != TZSH_ITEF_IF) {
				tzsh_output(vtbl, g_fmtcontext, "then");
				goto errout;
			}

			np->np_iestate[np->np_iendx].ie_state = TZSH_ITEF_THEN;
		}

		/* Check if the token is "else" */

		else if (strcmp(cmd, "else") == 0) {
			/* Get the cmd following the "else" -- there may or may not be one */

			*ppcmd = tzsh_argument(vtbl, saveptr, memlist);

			/* Verify that "else" is valid in this context */

			if (np->np_iestate[np->np_iendx].ie_state != TZSH_ITEF_THEN) {
				tzsh_output(vtbl, g_fmtcontext, "else");
				goto errout;
			}

			np->np_iestate[np->np_iendx].ie_state = TZSH_ITEF_ELSE;
		}

		/* Check if the token is "fi" */

		else if (strcmp(cmd, "fi") == 0) {
			/* Get the cmd following the fi -- there should be one */

			*ppcmd = tzsh_argument(vtbl, saveptr, memlist);
			if (*ppcmd) {
				tzsh_output(vtbl, g_fmtarginvalid, "fi");
				goto errout;
			}

			/* Verify that "fi" is valid in this context */

			if (np->np_iestate[np->np_iendx].ie_state != TZSH_ITEF_THEN && np->np_iestate[np->np_iendx].ie_state != TZSH_ITEF_ELSE) {
				tzsh_output(vtbl, g_fmtcontext, "fi");
				goto errout;
			}

			if (np->np_iendx < 1) {	/* Shouldn't happen */
				tzsh_output(vtbl, g_fmtinternalerror, "if");
				goto errout;
			}

			/* "Pop" the previous state */

			np->np_iendx--;
		}

		/* If we just parsed "if", then nothing is acceptable other than "then" */

		else if (np->np_iestate[np->np_iendx].ie_state == TZSH_ITEF_IF) {
			tzsh_output(vtbl, g_fmtcontext, cmd);
			goto errout;
		}
	}

	return OK;

errout:
	np->np_iendx = 0;
	np->np_iestate[0].ie_state = TZSH_ITEF_NORMAL;
	np->np_iestate[0].ie_disabled = false;
	np->np_iestate[0].ie_ifcond = false;
	np->np_iestate[0].ie_inverted = false;
	return ERROR;
}
#endif

/****************************************************************************
 * Name: tzsh_nice
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLEBG
static int tzsh_nice(FAR struct tzsh_vtbl_s *vtbl, FAR char **ppcmd, FAR char **saveptr, FAR TZSH_MEMLIST_TYPE *memlist)
{
	FAR char *cmd = *ppcmd;

	vtbl->np.np_nice = 0;
	if (cmd) {
		/* Check if the command is preceded by "nice" */

		if (strcmp(cmd, "nice") == 0) {
			/* Nicenesses range from -20 (most favorable scheduling) to 19
			 * (least  favorable).  Default is 10.
			 */

			vtbl->np.np_nice = 10;

			/* Get the cmd (or -d option of nice command) */

			cmd = tzsh_argument(vtbl, saveptr, memlist);
			if (cmd && strcmp(cmd, "-d") == 0) {
				FAR char *val = tzsh_argument(vtbl, saveptr, memlist);
				if (val) {
					char *endptr;
					vtbl->np.np_nice = (int)strtol(val, &endptr, 0);
					if (vtbl->np.np_nice > 19 || vtbl->np.np_nice < -20 || endptr == val || *endptr != '\0') {
						tzsh_output(vtbl, g_fmtarginvalid, "nice");
						return ERROR;
					}
					cmd = tzsh_argument(vtbl, saveptr, memlist);
				}
			}

			/* Return the real command name */

			*ppcmd = cmd;
		}
	}

	return OK;
}
#endif

/****************************************************************************
 * Name: tzsh_parse_cmdparm
 *
 * Description:
 *   This function parses and executes a simple TZSH command.  Output is
 *   always redirected.  This function supports command parameters like
 *
 *     set FOO `hello`
 *
 *   which would set the environment variable FOO to the output from
 *   the hello program
 *
 ****************************************************************************/

#ifdef CONFIG_TZSH_CMDPARMS
static int tzsh_parse_cmdparm(FAR struct tzsh_vtbl_s *vtbl, FAR char *cmdline, FAR const char *redirfile)
{
	TZSH_MEMLIST_TYPE memlist;
	FAR char *argv[MAX_ARGV_ENTRIES];
	FAR char *saveptr;
	FAR char *cmd;
#ifndef CONFIG_TZSH_DISABLEBG
	bool bgsave;
#endif
	bool redirsave;
	int argc;
	int ret;

	/* Initialize parser state */

	memset(argv, 0, MAX_ARGV_ENTRIES * sizeof(FAR char *));
	TZSH_MEMLIST_INIT(memlist);

	/* If any options like nice, redirection, or backgrounding are attempted,
	 * these will not be recognized and will just be passed through as
	 * normal, invalid commands or parameters.
	 */

#ifndef CONFIG_TZSH_DISABLEBG
	/* The command is never backgrounded .  Remember the current backgrounding
	 * state
	 */

	bgsave = vtbl->np.np_bg;
	vtbl->np.np_bg = false;
#endif

	/* Output is always redirected.  Remember the current redirection state */

	redirsave = vtbl->np.np_redirect;
	vtbl->np.np_redirect = true;

	/* Parse out the command at the beginning of the line */

	saveptr = cmdline;
	cmd = tzsh_argument(vtbl, &saveptr, &memlist);

	/* Check if any command was provided -OR- if command processing is
	 * currently disabled.
	 */

#ifndef CONFIG_TZSH_DISABLESCRIPT
	if (!cmd || !tzsh_cmdenabled(vtbl))
#else
	if (!cmd)
#endif
	{
		/* An empty line is not an error and an unprocessed command cannot
		 * generate an error, but neither should it change the last command
		 * status.
		 */

		TZSH_MEMLIST_FREE(&memlist);
		return OK;
	}

	/* Parse all of the arguments following the command name.  The form
	 * of argv is:
	 *
	 *   argv[0]:      The command name.
	 *   argv[1]:      The beginning of argument (up to CONFIG_TZSH_MAXARGUMENTS)
	 *   argv[argc]:   NULL terminating pointer
	 *
	 * Maximum size is CONFIG_TZSH_MAXARGUMENTS+1
	 */

	argv[0] = cmd;
	for (argc = 1; argc < MAX_ARGV_ENTRIES - 1; argc++) {
		argv[argc] = tzsh_argument(vtbl, &saveptr, &memlist);
		if (!argv[argc]) {
			break;
		}
	}

	argv[argc] = NULL;

	/* Check if the maximum number of arguments was exceeded */

	if (argc > CONFIG_TZSH_MAXARGUMENTS) {
		tzsh_output(vtbl, g_fmttoomanyargs, cmd);
	}

	/* Then execute the command */

	ret = tzsh_execute(vtbl, argc, argv, redirfile, O_WRONLY | O_CREAT | O_TRUNC);

	/* Restore the backgrounding and redirection state */

#ifndef CONFIG_TZSH_DISABLEBG
	vtbl->np.np_bg = bgsave;
#endif
	vtbl->np.np_redirect = redirsave;

	TZSH_MEMLIST_FREE(&memlist);
	return ret;
}
#endif

/****************************************************************************
 * Name: tzsh_parse_command
 *
 * Description:
 *   This function parses and executes one TZSH command from the command line.
 *
 ****************************************************************************/

static int tzsh_parse_command(FAR struct tzsh_vtbl_s *vtbl, FAR char *cmdline)
{
	TZSH_MEMLIST_TYPE memlist;
	FAR char *argv[MAX_ARGV_ENTRIES];
	FAR char *saveptr;
	FAR char *cmd;
	FAR char *redirfile = NULL;
	int oflags = 0;
	int argc;
	int ret;

	/* Initialize parser state */

	memset(argv, 0, MAX_ARGV_ENTRIES * sizeof(FAR char *));
	TZSH_MEMLIST_INIT(memlist);

#ifndef CONFIG_TZSH_DISABLEBG
	vtbl->np.np_bg = false;
#endif

#if CONFIG_NFILE_STREAMS > 0
	vtbl->np.np_redirect = false;
#endif

	/* Parse out the command at the beginning of the line */

	saveptr = cmdline;
	cmd = tzsh_argument(vtbl, &saveptr, &memlist);

#ifndef CONFIG_TZSH_DISABLESCRIPT
#ifndef CONFIG_TZSH_DISABLE_LOOPS
	/* Handle while-do-done and until-do-done loops */

	if (tzsh_loop(vtbl, &cmd, &saveptr, &memlist) != 0) {
		TZSH_MEMLIST_FREE(&memlist);
		return tzsh_saveresult(vtbl, true);
	}
#endif

#ifndef CONFIG_TZSH_DISABLE_ITEF
	/* Handle if-then-else-fi */

	if (tzsh_itef(vtbl, &cmd, &saveptr, &memlist) != 0) {
		TZSH_MEMLIST_FREE(&memlist);
		return tzsh_saveresult(vtbl, true);
	}
#endif
#endif

	/* Handle nice */

#ifndef CONFIG_TZSH_DISABLEBG
	if (tzsh_nice(vtbl, &cmd, &saveptr, &memlist) != 0) {
		TZSH_MEMLIST_FREE(&memlist);
		return tzsh_saveresult(vtbl, true);
	}
#endif

	/* Check if any command was provided -OR- if command processing is
	 * currently disabled.
	 */

#ifndef CONFIG_TZSH_DISABLESCRIPT
	if (!cmd || !tzsh_cmdenabled(vtbl))
#else
	if (!cmd)
#endif
	{
		/* An empty line is not an error and an unprocessed command cannot
		 * generate an error, but neither should it change the last command
		 * status.
		 */

		TZSH_MEMLIST_FREE(&memlist);
		return OK;
	}

	/* Parse all of the arguments following the command name.  The form
	 * of argv is:
	 *
	 *   argv[0]:      The command name.
	 *   argv[1]:      The beginning of argument (up to CONFIG_TZSH_MAXARGUMENTS)
	 *   argv[argc-3]: Possibly '>' or '>>'
	 *   argv[argc-2]: Possibly <file>
	 *   argv[argc-1]: Possibly '&'
	 *   argv[argc]:   NULL terminating pointer
	 *
	 * Maximum size is CONFIG_TZSH_MAXARGUMENTS+5
	 */

	argv[0] = cmd;
	for (argc = 1; argc < MAX_ARGV_ENTRIES - 1; argc++) {
		argv[argc] = tzsh_argument(vtbl, &saveptr, &memlist);
		if (!argv[argc]) {
			break;
		}
	}

	argv[argc] = NULL;

	/* Check if the command should run in background */

#ifndef CONFIG_TZSH_DISABLEBG
	if (argc > 1 && strcmp(argv[argc - 1], "&") == 0) {
		vtbl->np.np_bg = true;
		argv[argc - 1] = NULL;
		argc--;
	}
#endif

#if CONFIG_NFILE_STREAMS > 0
	/* Check if the output was re-directed using > or >> */

	if (argc > 2) {
		/* Check for redirection to a new file */

		if (strcmp(argv[argc - 2], g_redirect1) == 0) {
			vtbl->np.np_redirect = true;
			oflags = O_WRONLY | O_CREAT | O_TRUNC;
			redirfile = tzsh_getfullpath(vtbl, argv[argc - 1]);
			argc -= 2;
		}

		/* Check for redirection by appending to an existing file */

		else if (strcmp(argv[argc - 2], g_redirect2) == 0) {
			vtbl->np.np_redirect = true;
			oflags = O_WRONLY | O_CREAT | O_APPEND;
			redirfile = tzsh_getfullpath(vtbl, argv[argc - 1]);
			argc -= 2;
		}
	}
#endif

	/* Check if the maximum number of arguments was exceeded */

	if (argc > CONFIG_TZSH_MAXARGUMENTS) {
		tzsh_output(vtbl, g_fmttoomanyargs, cmd);
	}

	/* Then execute the command */

	ret = tzsh_execute(vtbl, argc, argv, redirfile, oflags);

	/* Free any allocated resources */

#if CONFIG_NFILE_STREAMS > 0
	/* Free the redirected output file path */

	if (redirfile) {
		tzsh_freefullpath(redirfile);
	}
#endif

	TZSH_MEMLIST_FREE(&memlist);
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_parse
 *
 * Description:
 *   This function parses and executes the line of text received from the
 *   user.  This may consist of one or more TZSH commands.  Multiple TZSH
 *   commands are separated by semi-colons.
 *
 ****************************************************************************/

int tzsh_parse(FAR struct tzsh_vtbl_s *vtbl, FAR char *cmdline)
{
#ifdef TZSH_DISABLE_SEMICOLON
	return tzsh_parse_command(vtbl, cmdline);

#else
#if !defined(CONFIG_TZSH_DISABLESCRIPT) && !defined(CONFIG_TZSH_DISABLE_LOOPS)
	FAR struct tzsh_parser_s *np = &vtbl->np;
#endif
	FAR char *start = cmdline;
	FAR char *working = cmdline;
	FAR char *ptr;
	size_t len;
	int ret;

	/* Loop until all of the commands on the command line have been processed OR
	 * until the end-of-loop has been recountered and we need to reload the line
	 * at the top of the loop.
	 */

#if !defined(CONFIG_TZSH_DISABLESCRIPT) && !defined(CONFIG_TZSH_DISABLE_LOOPS)
	for (np->np_jump = false; !np->np_jump;)
#else
	for (;;)
#endif
	{
#if !defined(CONFIG_TZSH_DISABLESCRIPT) && !defined(CONFIG_TZSH_DISABLE_LOOPS)
		/* Save the offset on the line to the start of the command */

		np->np_loffs = (uint16_t)(working - cmdline);
#endif
		/* A command may be terminated with a newline character, the end of the
		 * line, a semicolon, or a '#' character.  NOTE that the set of
		 * delimiting characters includes the quotation mark.  We need to
		 * handle quotation marks here because any other delimiter within a
		 * quoted string must be treated as normal text.
		 */

		len = strcspn(working, g_line_separator);
		ptr = working + len;

		/* Check for the last command on the line.  This means that the none
		 * of the delimiting characters was found or that the newline or '#'
		 * character was found.  Anything after the newline or '#' character
		 * is ignored (there should not be anything after a newline, of
		 * course).
		 */

		if (*ptr == '\0' || *ptr == '\n' || *ptr == '#') {
			/* Parse the last command on the line */

			return tzsh_parse_command(vtbl, start);
		}

		/* Check for a command terminated with ';'.  There is probably another
		 * command on the command line after this one.
		 */

		else if (*ptr == ';') {
			/* Terminate the line */

			*ptr++ = '\0';

			/* Parse this command */

			ret = tzsh_parse_command(vtbl, start);
			if (ret != OK) {
				/* tzsh_parse_command may return (1) -1 (ERROR) meaning that the
				 * command failed or we failed to start the command application
				 * or (2) 1 meaning that the application task was spawned
				 * successfully but returned failure exit status.
				 */

				return ret;
			}

			/* Then set the start of the next command on the command line */

			start = ptr;
			working = ptr;
		}

		/* Check if we encountered a quoted string */

		else {					/* if (*ptr == '"') */

			/* Find the closing quotation mark */

			FAR char *tmp = tzsh_strchr(ptr + 1, '"');
			if (!tmp) {
				/* No closing quotation mark! */

				tzsh_output(vtbl, g_fmtnomatching, "\"", "\"");
				return ERROR;
			}

			/* Otherwise, continue parsing after the closing quotation mark */

			working = ++tmp;
		}
	}

#ifndef CONFIG_TZSH_DISABLESCRIPT
	return OK;
#endif
#endif
}

/****************************************************************************
 * Name: cmd_break
 ****************************************************************************/

#if !defined(CONFIG_TZSH_DISABLESCRIPT) && !defined(CONFIG_TZSH_DISABLE_LOOPS)
int cmd_break(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	FAR struct tzsh_parser_s *np = &vtbl->np;

	/* Break outside of a loop is ignored */

	if (np->np_lpstate[np->np_lpndx].lp_state == TZSH_LOOP_DO) {
#ifndef CONFIG_TZSH_DISABLE_ITEF
		/* Yes... pop the original if-then-else-if state */

		np->np_iendx = np->np_lpstate[np->np_lpndx].lp_iendx;
#endif
		/* Disable all command processing until 'done' is encountered.  */

		np->np_lpstate[np->np_lpndx].lp_enable = false;
	}

	/* No syntax errors are detected(?).  Break is a nop everywhere except
	 * the supported context.
	 */

	return OK;
}
#endif
