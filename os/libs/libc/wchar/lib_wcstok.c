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
 * libs/libc/wchar/lib_wcstok.c
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

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name:  wcstok
 *
 * Description:
 *    Split wide string into tokens
 *
 * Input Parameters:
 *   s - the source wide string to truncate
 *   sep - the wide string containing the separator wide characters
 *   p - the pointer of which using to store of the current tokenization
 *       sequence
 *
 * Returned Value:
 *   Return a pointer to the last token found in the wide string;
 *   Return null pointer if there are no tokens left to retrieve
 *
 ****************************************************************************/

FAR wchar_t *wcstok(FAR wchar_t *s, FAR const wchar_t *sep, FAR wchar_t **p)
{
	if (!s && !(s = *p)) {
		return NULL;
	}

	s += wcsspn(s, sep);
	if (! *s) {
		return *p = NULL;
	}

	*p = s + wcscspn(s, sep);

	if (**p) {
		*(*p)++ = 0;
	} else {
		*p = NULL;
	}

	return s;
}
