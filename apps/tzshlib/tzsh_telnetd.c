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
 * apps/tzshlib/tzsh_telnetd.c
 *
 *   Copyright (C) 2007-2013, 2016-2017 Gregory Nutt. All rights reserved.
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

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <debug.h>

#include <arpa/inet.h>

#include "netutils/telnetd.h"

#ifdef CONFIG_TELNET_CHARACTER_MODE
#ifdef CONFIG_TZSH_CLE
#include "system/cle.h"
#else
#include "apps/readline.h"
#endif
#endif

#include "tzsh.h"
#include "tzsh_console.h"

#ifdef CONFIG_TZSH_TELNET

/****************************************************************************
 * Private Types
 ****************************************************************************/

enum telnetd_state_e {
	TELNETD_NOTRUNNING = 0,
	TELNETD_STARTED,
	TELNETD_RUNNING
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_telnetmain
 ****************************************************************************/

static int tzsh_telnetmain(int argc, char *argv[])
{
	FAR struct console_stdio_s *pstate = tzsh_newconsole();
	FAR struct tzsh_vtbl_s *vtbl;
	int ret;

	DEBUGASSERT(pstate != NULL);
	vtbl = &pstate->cn_vtbl;

	_info("Session [%d] Started\n", getpid());

#ifdef CONFIG_TZSH_TELNET_LOGIN
	/* Login User and Password Check */

	if (tzsh_telnetlogin(pstate) != OK) {
		tzsh_exit(vtbl, 1);
		return -1;				/* tzsh_exit does not return */
	}
#endif							/* CONFIG_TZSH_TELNET_LOGIN */

	/* The following logic mostly the same as the login in tzsh_session.c.  It
	 * differs only in that gets() is called to get the command instead of
	 * readline().
	 */

	/* Present a greeting and possibly a Message of the Day (MOTD) */

	fputs(g_tzshgreeting, pstate->cn_outstream);

#ifdef CONFIG_TZSH_MOTD
#ifdef CONFIG_TZSH_PLATFORM_MOTD
	/* Output the platform message of the day */

	platform_motd(vtbl->iobuffer, IOBUFFERSIZE);
	fprintf(pstate->cn_outstream, "%s/n", vtbl->iobuffer);

#else
	/* Output the fixed message of the day */

	fprintf(pstate->cn_outstream, "%s/n", g_tzshmotd);
#endif
#endif

	fflush(pstate->cn_outstream);

	/* Execute the startup script.  If standard console is also defined, then
	 * we will not bother with the initscript here (although it is safe to
	 * call tzshinitscript multiple times).
	 */

#if defined(CONFIG_TZSH_ROMFSETC) && !defined(CONFIG_TZSH_CONSOLE)
	(void)tzsh_initscript(vtbl);
#endif

	/* Execute the login script */

#ifdef CONFIG_TZSH_ROMFSRC
	(void)tzsh_loginscript(vtbl);
#endif

	/* Then enter the command line parsing loop */

	for (;;) {
		/* Display the prompt string */

		fputs(g_tzshprompt, pstate->cn_outstream);
		fflush(pstate->cn_outstream);

		/* Get the next line of input from the Telnet client */
#ifdef CONFIG_TELNET_CHARACTER_MODE
#ifdef CONFIG_TZSH_CLE
		ret = cle(pstate->cn_line, CONFIG_TZSH_LINELEN, INSTREAM(pstate), OUTSTREAM(pstate));
#else
		ret = readline(pstate->cn_line, CONFIG_TZSH_LINELEN, INSTREAM(pstate), OUTSTREAM(pstate));
#endif
#else
		if (fgets(pstate->cn_line, CONFIG_TZSH_LINELEN, INSTREAM(pstate)) != NULL) {
			ret = strlen(pstate->cn_line);
		} else {
			ret = EOF;
		}
#endif

		if (ret != EOF) {
			/* Parse process the received Telnet command */

			(void)tzsh_parse(vtbl, pstate->cn_line);
			fflush(pstate->cn_outstream);
		} else {
			fprintf(pstate->cn_outstream, g_fmtcmdfailed, "tzsh_telnetmain", "cle/readline/fgets", TZSH_ERRNO);
			tzsh_exit(vtbl, 1);
		}
	}

	/* Clean up */

	tzsh_exit(vtbl, 0);

	/* We do not get here, but this is necessary to keep some compilers happy */

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_telnetstart
 *
 * Description:
 *   tzsh_telnetstart() starts the Telnet daemon that will allow multiple
 *   TZSH connections via Telnet.  This function returns immediately after
 *   the daemon has been started.
 *
 * Input Parameters:
 *   family - Provides the IP family to use by the server.  May be either
 *     AF_INET or AF_INET6.  This is needed because both both may be
 *     enabled in the configuration.
 *
 *   All of the other properties of the Telnet daemon are controlled by
 *   NuttX configuration settings.
 *
 * Returned Values:
 *   The task ID of the Telnet daemon was successfully started.  A negated
 *   errno value will be returned on failure.
 *
 ****************************************************************************/

int tzsh_telnetstart(sa_family_t family)
{
	static enum telnetd_state_e state = TELNETD_NOTRUNNING;
	int ret = OK;

	if (state == TELNETD_NOTRUNNING) {
		struct telnetd_config_s config;

		/* There is a tiny race condition here if two tasks were to try to
		 * start the Telnet daemon concurrently.
		 */

		state = TELNETD_STARTED;

		/* Initialize any USB tracing options that were requested.  If
		 * standard console is also defined, then we will defer this step to
		 * the standard console.
		 */

#if defined(CONFIG_TZSH_USBDEV_TRACE) && !defined(CONFIG_TZSH_CONSOLE)
		usbtrace_enable(TRACE_BITSET);
#endif

		/* Configure the telnet daemon */

		config.d_port = HTONS(CONFIG_TZSH_TELNETD_PORT);
		config.d_family = family;
		config.d_priority = CONFIG_TZSH_TELNETD_DAEMONPRIO;
		config.d_stacksize = CONFIG_TZSH_TELNETD_DAEMONSTACKSIZE;
		config.t_priority = CONFIG_TZSH_TELNETD_CLIENTPRIO;
		config.t_stacksize = CONFIG_TZSH_TELNETD_CLIENTSTACKSIZE;
		config.t_entry = tzsh_telnetmain;

		/* Start the telnet daemon */

		_info("Starting the Telnet daemon\n");

		ret = telnetd_start(&config);
		if (ret < 0) {
			_err("ERROR: Failed to tart the Telnet daemon: %d\n", ret);
			state = TELNETD_NOTRUNNING;
		} else {
			state = TELNETD_RUNNING;
		}
	}

	return ret;
}

/****************************************************************************
 * Name: cmd_telnetd
 *
 * Description:
 *   The Telnet daemon may be started either programmatically by calling
 *   tzsh_telnetstart() or it may be started from the TZSH command line using
 *   this telnetd command.
 *
 *   Normally this command would be suppressed with CONFIG_TZSH_DISABLE_TELNETD
 *   because the Telnet daemon is automatically started in tzsh_main.c.  The
 *   exception is when CONFIG_TZSH_NETLOCAL is selected.  IN that case, the
 *   network is not enabled at initialization but rather must be enabled from
 *   the TZSH command line or via other applications.
 *
 *   In that case, calling tzsh_telnetstart() before the the network is
 *   initialized will fail.
 *
 * Input Parameters:
 *   None.  All of the properties of the Telnet daemon are controlled by
 *   NuttX configuration setting.
 *
 * Returned Values:
 *   OK is returned on success; ERROR is return on failure.
 *
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_TELNETD
int cmd_telnetd(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	sa_family_t family;

	/* If both IPv6 nd IPv4 are enabled, then the address family must
	 * be specified on the command line.
	 */

#if defined(CONFIG_NET_IPv4) && defined(CONFIG_NET_IPv6)
	family = (strcmp(argv[1], "ipv6") == 0) ? AF_INET6 : AF_INET;
#elif defined(CONFIG_NET_IPv6)
	family = AF_INET6;
#else							/* if defined(CONFIG_NET_IPv4) */
	family = AF_INET;
#endif

	return tzsh_telnetstart(family) < 0 ? ERROR : OK;
}
#endif

#endif							/* CONFIG_TZSH_TELNET */
