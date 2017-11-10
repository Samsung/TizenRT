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
/*
 * Copyright (c) 1997-2007  The Stanford SRP Authentication Project
 * All Rights Reserved.
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
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL STANFORD BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Redistributions in source or binary form must retain an intact copy
 * of this copyright notice.
 */

#include <stdio.h>
#include <tls/hap/config.h>
#include <tls/hap/t_read.h>

#define FSEPARATOR	':'

int t_nextfield(fp, s, max)
FILE *fp;
char *s;
unsigned max;
{
	int c, count = 0;

	while ((c = getc(fp)) != EOF) {
		if (c == '\r' || c == '\n') {
			ungetc(c, fp);
			break;
		} else if (c == FSEPARATOR) {
			break;
		}
		if (count < max - 1) {
			*s++ = c;
			++count;
		}
	}
	*s++ = '\0';
	return count;
}

int t_nextcstrfield(fp, s)
FILE *fp;
cstr *s;
{
	int c, count = 0;
	char cc;

	cstr_set_length(s, 0);
	while ((c = getc(fp)) != EOF) {
		if (c == '\r' || c == '\n') {
			ungetc(c, fp);
			break;
		} else if (c == FSEPARATOR) {
			break;
		}

		cc = c;
		cstr_appendn(s, &cc, 1);
		++count;
	}
	return count;
}

int t_nextline(fp)
FILE *fp;
{
	int c;

	while ((c = getc(fp)) != '\n')
		if (c == EOF) {
			return -1;
		}
	return 0;
}
