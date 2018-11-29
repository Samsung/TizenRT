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
 * apps/tzshlib/tzsh_session.c
 *
 *   Copyright (C) 2007-2009, 2011-2014, 2016 Gregory Nutt. All rights reserved.
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
#include <stdlib.h>

#ifdef CONFIG_TZSH_CLE
#include "system/cle.h"
#else
#include "apps/readline.h"
#endif

#include "tzsh.h"
#include "tzsh_console.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_session
 *
 * Description:
 *   This is the common session login on any TZSH session.  This function
 *   returns when an error reading from the input stream occurs, presumably
 *   signaling the end of the session.
 *
 *   This function:
 *   - Performs the login sequence if so configured
 *   - Executes the TZSH login script
 *   - Presents a greeting
 *   - Then provides a prompt then gets and processes the command line.
 *   - This continues until an error occurs, then the session returns.
 *
 * Input Parameters:
 *   pstate - Abstracts the underlying session.
 *
 * Returned Values:
 *   EXIT_SUCCESS or EXIT_FAILURE is returned.
 *
 ****************************************************************************/

int tzsh_session(FAR struct console_stdio_s *pstate)
{
	FAR struct tzsh_vtbl_s *vtbl;
	int ret;

	DEBUGASSERT(pstate);
	vtbl = &pstate->cn_vtbl;

#ifdef CONFIG_TZSH_CONSOLE_LOGIN
	/* Login User and Password Check */

	if (tzsh_login(pstate) != OK) {
		tzsh_exit(vtbl, 1);
		return -1;				/* tzsh_exit does not return */
	}
#endif							/* CONFIG_TZSH_CONSOLE_LOGIN */

	/* Present a greeting and possibly a Message of the Day (MOTD) */

	fputs(g_tzshgreeting, pstate->cn_outstream);

#ifdef CONFIG_TZSH_MOTD
#ifdef CONFIG_TZSH_PLATFORM_MOTD
	/* Output the platform message of the day */

	platform_motd(vtbl->iobuffer, IOBUFFERSIZE);
	fprintf(pstate->cn_outstream, "%s\n", vtbl->iobuffer);

#else
	/* Output the fixed message of the day */

	fprintf(pstate->cn_outstream, "%s\n", g_tzshmotd);
#endif
#endif

	fflush(pstate->cn_outstream);

	/* Execute the login script */

#ifdef CONFIG_TZSH_ROMFSRC
	(void)tzsh_loginscript(vtbl);
#endif

	/* Then enter the command line parsing loop */

	for (;;) {
		/* For the case of debugging the USB console... dump collected USB trace data */

#ifdef CONFIG_TZSH_USBDEV_TRACE
		tzsh_usbtrace();
#endif

		/* Display the prompt string */

		fputs(g_tzshprompt, pstate->cn_outstream);
		fflush(pstate->cn_outstream);

		/* Get the next line of input. readline() returns EOF on end-of-file
		 * or any read failure.
		 */

#ifdef CONFIG_TZSH_CLE
		ret = cle(pstate->cn_line, CONFIG_TZSH_LINELEN, INSTREAM(pstate), OUTSTREAM(pstate));
#else
		ret = readline(pstate->cn_line, CONFIG_TZSH_LINELEN, INSTREAM(pstate), OUTSTREAM(pstate));
#endif
		if (ret != EOF) {
			/* Parse process the command */

			(void)tzsh_parse(vtbl, pstate->cn_line);
			fflush(pstate->cn_outstream);
		}

		/* Readline normally returns the number of characters read,
		 * but will return EOF on end of file or if an error occurs.
		 * EOF will cause the session to terminate.
		 */

		else {
			fprintf(pstate->cn_outstream, g_fmtcmdfailed, "tzsh_session", "readline", TZSH_ERRNO_OF(-ret));
			return ret == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}

	/* We do not get here, but this is necessary to keep some compilers happy.
	 * But others will complain that this code is not reachable.
	 */

	return EXIT_SUCCESS;
}
