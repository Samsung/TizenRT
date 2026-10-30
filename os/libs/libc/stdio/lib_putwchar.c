/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
 * libs/libc/stdio/lib_putwchar.c
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2005-2014 Rich Felker, et al.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <limits.h>
#include <stdio.h>
#include <wchar.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: putwchar
 *
 * Description:
 *   Write wide character to stdout
 *
 * Input Parameters:
 *   c - the wide character to write
 *
 * Returned Value:
 *   Return the wide character that written in to the stdout on success,
 *   return WEOF on fail write to the stdout
 *
 ****************************************************************************/

wint_t putwchar_unlocked(wchar_t c)
{
#ifdef CONFIG_FILE_STREAM
	return fputwc_unlocked(c, stdout);
#else
	char mbc[MB_LEN_MAX];
	int l;

	l = wctomb(mbc, c);
	if (l < 0) {
		return WEOF;
	}

	return write(STDOUT_FILENO, mbc, l) == l ? c : WEOF;
#endif
}

wint_t putwchar(wchar_t c)
{
	wint_t w;

#ifdef CONFIG_FILE_STREAM
	flockfile(stdout);
#endif
	w = putwchar_unlocked(c);
#ifdef CONFIG_FILE_STREAM
	funlockfile(stdout);
#endif

	return w;
}
