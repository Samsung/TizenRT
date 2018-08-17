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
 * libc/stdio/lib_tempnam.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tempnam
 *
 * Description:
 *   The tempnam() function generates a pathname that may be used for a
 *   temporary file.
 *
 *   The tempnam() function allows the user to control the choice of a
 *   directory. The dir argument points to the name of the directory in which
 *   the file is to be created. If dir is a null pointer or points to a
 *   string which is not a name for an appropriate directory, the path prefix
 *   defined as P_tmpdir in the <stdio.h> header will be used. If that
 *   directory is not accessible, an implementation-defined directory may be
 *   used.
 *
 *   Many applications prefer their temporary files to have certain initial
 *   letter sequences in their names. The pfx argument should be used for
 *   this. This argument may be a null pointer or point to a string of up
 *   to five bytes to be used as the beginning of the filename.
 *
 * Returned Value:
 *   Upon successful completion, tempnam() will allocate space for a string
 *   put the generated pathname in that space, and return a pointer to it.
 *   The pointer will be suitable for use in a subsequent call to free().
 *   Otherwise, it will return a null pointer and set errno to indicate the
 *   error.
 *
 *   The tempnam() function will fail if:
 *     ENOMEM - Insufficient storage space is available.
 *
 ****************************************************************************/

FAR char *tempnam(FAR const char *dir, FAR const char *pfx)
{
	FAR char *path;
	char *ret;

	if (!dir) {
		dir = P_tmpdir;
	}

	if (!pfx) {
		asprintf(&path, "%s/XXXXXX", dir);
	} else {
		asprintf(&path, "%s/%.5sXXXXXX", dir, pfx);
	}

	if (path) {
		ret = mktemp(path);
		if (strncmp(ret, "", 1)) {
			return path;
		}

		free(path);
	}

	set_errno(ENOMEM);
	return NULL;
}
