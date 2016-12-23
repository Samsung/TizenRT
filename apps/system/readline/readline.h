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
 * apps/system/readline/readline.h
 *
 *   Copyright (C) 2013, 2015 Gregory Nutt. All rights reserved.
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

#ifndef __APPS_SYSTEM_READLINE_READLINE_H
#define __APPS_SYSTEM_READLINE_READLINE_H 1

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Some environments may return CR as end-of-line, others LF, and others
 * both.  If not specified, the logic here assumes either (but not both) as
 * the default.
 */

#if defined(CONFIG_EOL_IS_CR)
#undef  CONFIG_EOL_IS_LF
#undef  CONFIG_EOL_IS_BOTH_CRLF
#undef  CONFIG_EOL_IS_EITHER_CRLF
#elif defined(CONFIG_EOL_IS_LF)
#undef  CONFIG_EOL_IS_CR
#undef  CONFIG_EOL_IS_BOTH_CRLF
#undef  CONFIG_EOL_IS_EITHER_CRLF
#elif defined(CONFIG_EOL_IS_BOTH_CRLF)
#undef  CONFIG_EOL_IS_CR
#undef  CONFIG_EOL_IS_LF
#undef  CONFIG_EOL_IS_EITHER_CRLF
#elif defined(CONFIG_EOL_IS_EITHER_CRLF)
#undef  CONFIG_EOL_IS_CR
#undef  CONFIG_EOL_IS_LF
#undef  CONFIG_EOL_IS_BOTH_CRLF
#else
#undef  CONFIG_EOL_IS_CR
#undef  CONFIG_EOL_IS_LF
#undef  CONFIG_EOL_IS_BOTH_CRLF
#define CONFIG_EOL_IS_EITHER_CRLF 1
#endif

/* Helper macros */

#define RL_GETC(v)      ((v)->rl_getc(v))
#define RL_PUTC(v, ch)   ((v)->rl_putc(v, ch))
#define RL_WRITE(v, b, s) ((v)->rl_write(v, b, s))

/****************************************************************************
 * Public Type Declarations
 ****************************************************************************/

struct rl_common_s {
	int (*rl_getc)(FAR struct rl_common_s *vtbl);
#ifdef CONFIG_READLINE_ECHO
	void (*rl_putc)(FAR struct rl_common_s *vtbl, int ch);
	void (*rl_write)(FAR struct rl_common_s *vtbl, FAR const char *buffer, size_t buflen);
#endif
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: readline_common
 *
 *   Common logic shared by readline and std_readline().
 *
 * Input Parameters:
 *   buf       - The user allocated buffer to be filled.
 *   buflen    - the size of the buffer.
 *   instream  - The stream to read characters from
 *   outstream - The stream to each characters to.
 *
 * Returned values:
 *   On success, the (positive) number of bytes transferred is returned.
 *   EOF is returned to indicate either an end of file condition or a
 *   failure.
 *
 **************************************************************************/

ssize_t readline_common(FAR struct rl_common_s *vtbl, FAR char *buf, int buflen);

#endif							/* __APPS_SYSTEM_READLINE_READLINE_H */
