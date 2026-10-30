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
 * libs/libc/stdio/lib_fgetwc.c
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

#include <wchar.h>
#include <errno.h>
#include <string.h>

#ifdef CONFIG_FILE_STREAM

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fgetwc_unlocked
 *
 * Description:
 *   Get wide character from stream without lock the stream
 *
 * Input Parameters:
 *   f - Pointer to a FILE object that identifies an input stream, during
 *   the read operation, the stream will not be locked
 *
 * Returned Value:
 *   Return the character read is returned,
 *   Return WEOF is the sequence of bytes that read cannot be interpreted as
 *   a valid wide character, and sets the errno to EILSEQ
 *
 ****************************************************************************/

wint_t fgetwc_unlocked(FAR FILE *f)
{
	mbstate_t st;
	wchar_t wc;
	int c;
	char b;
	size_t l = -2;

	memset(&st, 0, sizeof(mbstate_t));

	/* Convert character byte-by-byte */

	while (l == -2) {
		c = getc_unlocked(f);
		b = (char)c;
		if (c < 0) {
			if (!mbsinit(&st)) {
				set_errno(EILSEQ);
			}

			f->fs_flags |= __FS_FLAG_EOF;
			return WEOF;
		}

		l = mbrtowc(&wc, &b, 1, &st);
		if (l == -1) {
			f->fs_flags |= __FS_FLAG_EOF;
			return WEOF;
		}
	}

	return wc;
}

/****************************************************************************
 * Name: fgetwc
 *
 * Description:
 *   Get wide character from stream
 *
 * Input Parameters:
 *   f - Pointer to a FILE object that identifies an input stream, the stream
 *   will be locked during the read operation
 *
 * Returned Value:
 *   Return the character read is returned,
 *   Return WEOF is the sequence of bytes that read cannot be interpreted as
 *   a valid wide character, and sets the errno to EILSEQ
 *
 ****************************************************************************/

wint_t fgetwc(FAR FILE *f)
{
	wint_t c;
	flockfile(f);
	c = fgetwc_unlocked(f);
	funlockfile(f);
	return c;
}

#endif /* CONFIG_FILE_STREAM */
