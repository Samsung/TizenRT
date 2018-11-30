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
 * apps/tzshlib/tzsh_stdlogin.c
 *
 *   Copyright (C) 2016 Gregory Nutt. All rights reserved.
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
#include <ctype.h>

#include "fsutils/passwd.h"
#ifdef CONFIG_TZSH_CLE
#include "system/cle.h"
#else
#include "apps/readline.h"
#endif

#include "tzsh.h"
#include "tzsh_console.h"

#ifdef CONFIG_TZSH_CONSOLE_LOGIN

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_stdtoken
 ****************************************************************************/

static void tzsh_stdtoken(FAR struct console_stdio_s *pstate, FAR char *buffer, size_t buflen)
{
	FAR char *start;
	FAR char *endp1;
	bool quoted = false;

	/* Find the start of token.  Either (1) the first non-white space
	 * character on the command line or (2) the character immediately after
	 * a quotation mark.
	 */

	for (start = pstate->cn_line; *start; start++) {
		/* Does the token open with a quotation mark */

		if (*start == '"') {
			/* Yes.. break out with start set to the character after the
			 * quotation mark.
			 */

			quoted = true;
			start++;
			break;
		}

		/* No, then any non-whitespace is the first character of the token */

		else if (!isspace(*start)) {
			/* Break out with start set to the first character of the token */

			break;
		}
	}

	/* Find the terminating character after the token on the command line.  The
	 * terminating character is either (1) the matching quotation mark, or (2)
	 * any whitespace.
	 */

	for (endp1 = start; *endp1; endp1++) {
		/* Did the token begin with a quotation mark? */

		if (quoted) {
			/* Yes.. then only the matching quotation mark (or end of string)
			 * terminates
			 */

			if (*endp1 == '"') {
				/* Break out... endp1 points to closing quotation mark */

				break;
			}
		}

		/* No.. any whitespace (or end of string) terminates */

		else if (isspace(*endp1)) {
			/* Break out... endp1 points to first while space encountered */

			break;
		}
	}

	/* Replace terminating character with a NUL terminator */

	*endp1 = '\0';

	/* Copied the token into the buffer */

	strncpy(buffer, start, buflen);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_stdlogin
 *
 * Description:
 *   Prompt the user for a username and password.  Return a failure if valid
 *   credentials are not returned (after some retries.
 *
 ****************************************************************************/

int tzsh_stdlogin(FAR struct console_stdio_s *pstate)
{
	char username[16];
	char password[16];
	int ret;
	int i;

	/* Loop for the configured number of retries */

	for (i = 0; i < CONFIG_TZSH_LOGIN_FAILCOUNT; i++) {
		/* Ask for the login username */

		printf("%s", g_userprompt);

		/* Get the reponse, handling all possible cases */

#ifdef CONFIG_TZSH_CLE
		ret = cle(pstate->cn_line, CONFIG_TZSH_LINELEN, stdin, stdout);
#else
		ret = std_readline(pstate->cn_line, CONFIG_TZSH_LINELEN);
#endif

		username[0] = '\0';
		if (ret != EOF) {
			/* Parse out the username */

			tzsh_stdtoken(pstate, username, sizeof(username));
		}

		/* Ask for the login password */

		printf("%s", g_passwordprompt);

		password[0] = '\0';
		if (fgets(pstate->cn_line, CONFIG_TZSH_LINELEN, stdin) != NULL) {
			/* Parse out the password */

			tzsh_stdtoken(pstate, password, sizeof(password));

			/* Verify the username and password */

#if defined(CONFIG_TZSH_LOGIN_PASSWD)
			ret = passwd_verify(username, password);
			if (PASSWORD_VERIFY_MATCH(ret))
#elif defined(CONFIG_TZSH_LOGIN_PLATFORM)
			ret = platform_user_verify(username, password);
			if (PASSWORD_VERIFY_MATCH(ret))
#elif defined(CONFIG_TZSH_LOGIN_FIXED)
			if (strcmp(password, CONFIG_TZSH_LOGIN_PASSWORD) == 0 && strcmp(username, CONFIG_TZSH_LOGIN_USERNAME) == 0)
#else
#error No user verification method selected
#endif
			{
				printf("%s", g_loginsuccess);
				return OK;
			} else {
				printf("%s", g_badcredentials);
#if CONFIG_TZSH_LOGIN_FAILDELAY > 0
				usleep(CONFIG_TZSH_LOGIN_FAILDELAY * 1000L);
#endif
			}
		}
	}

	/* Too many failed login attempts */

	printf("%s", g_loginfailure);
	return -1;
}

#endif							/* CONFIG_TZSH_CONSOLE_LOGIN */
