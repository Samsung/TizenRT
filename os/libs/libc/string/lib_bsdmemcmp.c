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
 * libs/libc/string/lib_bsdmemcmp.c
 *
 * SPDX-License-Identifier: BSD
 * SPDX-FileCopyrightText: 1994-2009  Red Hat, Inc. All rights reserved
 *
 * Copyright (c) 1994-2009  Red Hat, Inc. All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the BSD License.   This program is distributed in the hope that
 * it will be useful, but WITHOUT ANY WARRANTY expressed or implied,
 * including the implied warranties of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  A copy of this license is available at
 * http://www.opensource.org/licenses. Any Red Hat trademarks that are
 * incorporated in the source code or documentation are not subject to
 * the BSD License and may only be used or replicated with the express
 * permission of Red Hat, Inc.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <string.h>

#include "libc.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Nonzero if either x or y is not aligned on a "long" boundary. */

#define UNALIGNED(x, y) \
  (((long)(uintptr_t)(x) & (sizeof(long) - 1)) | ((long)(uintptr_t)(y) & (sizeof(long) - 1)))

/* How many bytes are copied each iteration of the word copy loop. */

#define LBLOCKSIZE (sizeof(long))

/* Threshold for punting to the byte copier. */

#define TOO_SMALL(len) ((len) < LBLOCKSIZE)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#if !defined(CONFIG_LIBC_ARCH_MEMCMP) && defined(LIBC_BUILD_MEMCMP)
#undef memcmp
no_builtin("memcmp")
int memcmp(FAR const void *s1, FAR const void *s2, size_t n)
{
	FAR unsigned char *p1 = (FAR unsigned char *)s1;
	FAR unsigned char *p2 = (FAR unsigned char *)s2;
	FAR unsigned long *a1;
	FAR unsigned long *a2;

	/* If the size is too small, or either pointer is unaligned,
	 * then we punt to the byte compare loop.  Hopefully this will
	 * not turn up in inner loops.
	 */

	if (!TOO_SMALL(n) && !UNALIGNED(p1, p2)) {
		/* Otherwise, load and compare the blocks of memory one
		 * word at a time.
		 */

		a1 = (FAR unsigned long *)p1;
		a2 = (FAR unsigned long *)p2;
		while (n >= LBLOCKSIZE) {
			if (*a1 != *a2) {
				break;
			}

			a1++;
			a2++;
			n -= LBLOCKSIZE;
		}

		/* check s mod LBLOCKSIZE remaining characters */

		p1 = (FAR unsigned char *)a1;
		p2 = (FAR unsigned char *)a2;
	}

	while (n--) {
		if (*p1 != *p2) {
			return *p1 - *p2;
		}

		p1++;
		p2++;
	}

	return 0;
}
#endif
