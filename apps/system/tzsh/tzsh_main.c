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
 * system/tzsh/tzsh_main.c
 *
 *   Copyright (C) 2007-2013, 2017-2018 Gregory Nutt. All rights reserved.
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

#include <sys/stat.h>
#include <sys/boardctl.h>
#include <stdint.h>
#include <stdio.h>
#include <sched.h>
#include <errno.h>

#if defined(CONFIG_LIBC_EXECFUNCS)
#include <tinyara/binfmt/symtab.h>
#endif

#include "apps/platform/cxxinitialize.h"

#include "apps/tzshlib/tzshlib.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Kludge needed only for BINFS but should be harmless in other cases.  This
 * setups up an empty symbol table.  You will need to add logic to create
 * a "real" symbol table for your application elsewhere (see, for example
 * apps/system/symtab)
 */

#define HAVE_DUMMY_SYMTAB 1

/* Symbol table is not needed if loadable binary modules are not supported */

#if !defined(CONFIG_LIBC_EXECFUNCS)
#undef HAVE_DUMMY_SYMTAB
#undef CONFIG_SYSTEM_TZSH_SYMTAB
#endif

/* boardctl() support is also required  for application-space symbol table
 * support.
 */

#if !defined(CONFIG_LIB_BOARDCTL) || !defined(CONFIG_BOARDCTL_APP_SYMTAB)
#undef HAVE_DUMMY_SYMTAB
#undef CONFIG_SYSTEM_TZSH_SYMTAB
#endif

/* If a symbol table is provided by board-specific logic, then we do not
 * need to do anything from the application space.
 */

#ifdef CONFIG_EXECFUNCS_HAVE_SYMTAB
#undef HAVE_DUMMY_SYMTAB
#undef CONFIG_SYSTEM_TZSH_SYMTAB
#endif

/* If we are going to use the application-space symbol table, then suppress
 * the dummy symbol table.
 */

#if defined(CONFIG_SYSTEM_TZSH_SYMTAB)
#undef HAVE_DUMMY_SYMTAB
#endif

/* Check if we need to build in support for the system() and/or popen()
 * functions.  In the KERNEL build mode (only), TZSH is build as a ELF
 * program and must be capable of executing a single command provided
 * on the command line.
 */

#undef HAVE_TZSH_COMMAND
#if (defined(CONFIG_SYSTEM_SYSTEM) || defined(CONFIG_SYSTEM_POPEN)) && \
	defined(CONFIG_BUILD_LOADABLE)
#define HAVE_TZSH_COMMAND 1
#endif

/* Check if we have met the BINFS requirement either via a board-provided
 * symbol table, an application provided symbol table, or a dummy symbol
 * table
 */

#if defined(CONFIG_FS_BINFS) && !defined(HAVE_DUMMY_SYMTAB) && \
	!defined(CONFIG_SYSTEM_TZSH_SYMTAB) && \
!defined(CONFIG_EXECFUNCS_HAVE_SYMTAB)
#warning "Prequisites not met for BINFS symbol table"
#endif

/* C++ initialization requires CXX initializer support */

#if !defined(CONFIG_HAVE_CXX) || !defined(CONFIG_HAVE_CXXINITIALIZE)
#undef CONFIG_SYSTEM_TZSH_CXXINITIALIZE
#endif

/* The TZSH telnet console requires networking support (and TCP/IP) */

#ifndef CONFIG_NET
#undef CONFIG_TZSH_TELNET
#endif

/* If Telnet is used and both IPv6 and IPv4 are enabled, then we need to
 * pick one.
 */

#ifdef CONFIG_NET_IPv6
#define ADDR_FAMILY AF_INET6
#else
#define ADDR_FAMILY AF_INET
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

#if defined(HAVE_DUMMY_SYMTAB)
/* If posix_spawn() is enabled as required for CONFIG_TZSH_FILE_APPS, then
 * a symbol table is needed by the internals of posix_spawn().  The symbol
 * table is needed to support ELF and NXFLAT binaries to dynamically link to
 * the base code.  However, if only the BINFS file system is supported, then
 * no symbol table is needed.
 *
 * This will, of course, have to be replaced with a valid symbol table if
 * you want to support ELF or NXFLAT binaries!
 */

static const struct symtab_s g_dummy_symtab[1];	/* Wasted memory! */

#elif defined(CONFIG_SYSTEM_TZSH_SYMTAB)

extern const struct symtab_s CONFIG_SYSTEM_TZSH_SYMTAB_ARRAYNAME[];
extern const int CONFIG_SYSTEM_TZSH_SYMTAB_COUNTNAME;

#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_task
 *
 * Description:
 *   This is the main logic for the case of the TZSH task.  It will perform
 *   one-time TZSH initialization and start an interactive session on the
 *   current console device.
 *
 ****************************************************************************/

static int tzsh_task(void)
{
#if defined(HAVE_DUMMY_SYMTAB) || defined(CONFIG_SYSTEM_TZSH_SYMTAB)
	struct boardioc_symtab_s symdesc;
#endif
	int exitval = 0;
	int ret;

#if defined(CONFIG_SYSTEM_TZSH_CXXINITIALIZE)
	/* Call all C++ static constructors */

	up_cxxinitialize();
#endif

#if defined(HAVE_DUMMY_SYMTAB) || defined(CONFIG_SYSTEM_TZSH_SYMTAB)
#if defined(HAVE_DUMMY_SYMTAB)
	/* Make sure that we are using our symbol table */

	symdesc.symtab = (FAR struct symtab_s *)g_dummy_symtab;	/* Discard 'const' */
	symdesc.nsymbols = 0;

#else							/* if defined(CONFIG_SYSTEM_TZSH_SYMTAB) */
	symdesc.symtab = (FAR struct symtab_s *)CONFIG_SYSTEM_TZSH_SYMTAB_ARRAYNAME;	/* Discard 'const' */
	symdesc.nsymbols = CONFIG_SYSTEM_TZSH_SYMTAB_COUNTNAME;
#endif

	(void)boardctl(BOARDIOC_APP_SYMTAB, (uintptr_t) & symdesc);
#endif

	/* Initialize the TZSH library */

	tzsh_initialize();

#if defined(CONFIG_TZSH_TELNET) && !defined(CONFIG_TZSH_NETLOCAL)
	/* If the Telnet console is selected as a front-end, then start the
	 * Telnet daemon UNLESS network initialization is deferred via
	 * CONFIG_TZSH_NETLOCAL.  In that case, the telnet daemon must be
	 * started manually with the telnetd command after the network has
	 * been initialized
	 */

	ret = tzsh_telnetstart(ADDR_FAMILY);
	if (ret < 0) {
		/* The daemon is NOT running.  Report the error then fail...
		 * either with the serial console up or just exiting.
		 */

		fprintf(stderr, "ERROR: Failed to start TELNET daemon: %d\n", ret);
		exitval = 1;
	}
#endif

#ifdef CONFIG_TZSH_CONSOLE
	/* If the serial console front end is selected, then run it on this thread */

	ret = tzsh_consolemain(0, NULL);

	/* tzsh_consolemain() should not return.  So if we get here, something
	 * is wrong.
	 */

#if CONFIG_NFILE_DESCRIPTORS > 0
	fprintf(stderr, "ERROR: tzsh_consolemain() returned: %d\n", ret);
#else
	printf("ERROR: tzsh_consolemain() returned: %d\n", ret);
#endif

	exitval = 1;
#endif

	return exitval;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_main
 ****************************************************************************/

#ifdef BUILD_MODULE
int main(int argc, FAR char *argv[])
#else
int tzsh_main(int argc, char *argv[])
#endif
{
	/* There are two modes that TZSH can be executed in:
	 *
	 * 1) As a normal, interactive shell.  In this case, no arguments are
	 *    expected on the command line.  OR
	 * 2) As a single command processor.  In this case, the single command is
	 *    is provided in argv[1].
	 *
	 * NOTE:  The latter mode is only available if CONFIG_BUILD_LOADABLE=y.  In
	 * that cause, this main() function will be build as a process.  The process
	 * will be started with a command by the implementations of the system() and
	 * popen() interfaces.
	 */

#ifdef HAVE_TZSH_COMMAND
	if (argc > 1) {
		return tzsh_system(argc, argv);
	}
#endif
	{
		return tzsh_task();
	}
}
