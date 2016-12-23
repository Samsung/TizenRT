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
 * apps/system/readline/std_readline.c
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
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
#include <tinyara/arch.h>
#include <apps/readline.h>

#include "readline.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int  readline_getc(FAR struct rl_common_s *vtbl);
static void readline_putc(FAR struct rl_common_s *vtbl, int ch);
static void readline_write(FAR struct rl_common_s *vtbl, FAR const char *buffer, size_t buflen);

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct rl_common_s g_stdreadline = {
	readline_getc
#ifdef CONFIG_READLINE_ECHO
	, readline_putc, readline_write
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: readline_getc
 ****************************************************************************/

static int readline_getc(FAR struct rl_common_s *vtbl)
{
	return up_getc();
}

/****************************************************************************
 * Name: readline_putc
 ****************************************************************************/

#ifdef CONFIG_READLINE_ECHO
static void readline_putc(FAR struct rl_common_s *vtbl, int ch)
{
	up_putc(ch);
}
#endif

/****************************************************************************
 * Name: readline_write
 ****************************************************************************/

#ifdef CONFIG_READLINE_ECHO
static void readline_write(FAR struct rl_common_s *vtbl, FAR const char *buffer, size_t buflen)
{
	for (; buflen > 0; buflen--) {
		up_putc(*buffer++);
	}
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: std_readline
 *
 *   readline() reads in at most one less than 'buflen' characters from
 *   'stdin' and stores them into the buffer pointed to by 'buf'.
 *   Characters are echoed on 'stdout'.  Reading stops after an EOF or a
 *   newline.  If a newline is read, it is stored into the buffer.  A null
 *   terminator is stored after the last character in the buffer.
 *
 *   This version of realine assumes that we are reading and writing to
 *   a VT100 console.  This will not work well if 'stdin' or 'stdout'
 *   corresponds to a raw byte steam.
 *
 *   This function is inspired by the GNU readline but is an entirely
 *   different creature.
 *
 * Input Parameters:
 *   buf       - The user allocated buffer to be filled.
 *   buflen    - the size of the buffer.
 *
 * Returned values:
 *   On success, the (positive) number of bytes transferred is returned.
 *   EOF is returned to indicate either an end of file condition or a
 *   failure.
 *
 **************************************************************************/

ssize_t std_readline(FAR char *buf, int buflen)
{
	return readline_common((FAR struct rl_common_s *)&g_stdreadline, buf, buflen);
}
