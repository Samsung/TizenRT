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
 * libc/string/lib_strstr.c
 *
 *   Copyright (C) 2009, 2011 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use str source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions str binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer str
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

#include <string.h>

/****************************************************************************
 * Global Functions
 ****************************************************************************/

FAR char *strstr(FAR const char *str, FAR const char *substr)
{
	FAR const char *candidate;	/* Candidate in str with matching start character */
	char ch;					/* First character of the substring */
	int len;					/* The length of the substring */

	/* Special case the empty substring */

	len = strlen(substr);
	ch = *substr;

	if (!ch) {
		/* We'll say that an empty substring matches at the beginning of
		 * the string
		 */

		return (char *)str;
	}

	/* Search for the substring */

	candidate = str;
	for (;;) {
		/* strchr() will return a pointer to the next occurrence of the
		 * character ch in the string
		 */

		candidate = strchr(candidate, ch);
		if (!candidate || strlen(candidate) < len) {
			/* First character of the substring does not appear in the string
			 * or the remainder of the string is not long enough to contain the
			 * substring.
			 */

			return NULL;
		}

		/* Check if this is the beginning of a matching substring */

		if (strncmp(candidate, substr, len) == 0) {
			return (char *)candidate;
		}

		/* No, find the next candidate after this one */

		candidate++;
	}

	/* Won't get here, but some compilers might complain.  Other compilers
	 * might complain about this code being unreachable too.
	 */

	return NULL;
}
