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
Copyright (c) 2009-2014 Roger Light <roger@atchoo.org>

All rights reserved. This program and the accompanying materials
are made available under the terms of the Eclipse Public License v1.0
and Eclipse Distribution License v1.0 which accompany this distribution.

The Eclipse Public License is available at
   http://www.eclipse.org/legal/epl-v10.html
and the Eclipse Distribution License is available at
  http://www.eclipse.org/org/documents/edl-v10.php.

Contributors:
   Roger Light - initial implementation and documentation.
*/

#include <config.h>

#include <stdlib.h>
#include <string.h>

#include <memory_mosq.h>

#ifdef REAL_WITH_MEMORY_TRACKING
#	if defined(__APPLE__)
#		include <malloc/malloc.h>
#		define malloc_usable_size malloc_size
#	elif defined(__FreeBSD__)
#		include <malloc_np.h>
#	else
#		include <malloc.h>
#	endif
#endif

#ifdef REAL_WITH_MEMORY_TRACKING
static unsigned long memcount = 0;
static unsigned long max_memcount = 0;
#endif

void *_mosquitto_calloc(size_t nmemb, size_t size)
{
	void *mem = calloc(nmemb, size);

#ifdef REAL_WITH_MEMORY_TRACKING
	memcount += malloc_usable_size(mem);
	if (memcount > max_memcount) {
		max_memcount = memcount;
	}
#endif

	return mem;
}

void _mosquitto_free(void *mem)
{
#ifdef REAL_WITH_MEMORY_TRACKING
	if (!mem) {
		return;
	}
	memcount -= malloc_usable_size(mem);
#endif
	free(mem);
}

void *_mosquitto_malloc(size_t size)
{
	void *mem = malloc(size);

#ifdef REAL_WITH_MEMORY_TRACKING
	memcount += malloc_usable_size(mem);
	if (memcount > max_memcount) {
		max_memcount = memcount;
	}
#endif

	return mem;
}

#ifdef REAL_WITH_MEMORY_TRACKING
unsigned long _mosquitto_memory_used(void)
{
	return memcount;
}

unsigned long _mosquitto_max_memory_used(void)
{
	return max_memcount;
}
#endif

void *_mosquitto_realloc(void *ptr, size_t size)
{
	void *mem;
#ifdef REAL_WITH_MEMORY_TRACKING
	if (ptr) {
		memcount -= malloc_usable_size(ptr);
	}
#endif
	mem = realloc(ptr, size);

#ifdef REAL_WITH_MEMORY_TRACKING
	memcount += malloc_usable_size(mem);
	if (memcount > max_memcount) {
		max_memcount = memcount;
	}
#endif

	return mem;
}

char *_mosquitto_strdup(const char *s)
{
	char *str = strdup(s);

#ifdef REAL_WITH_MEMORY_TRACKING
	memcount += malloc_usable_size(str);
	if (memcount > max_memcount) {
		max_memcount = memcount;
	}
#endif

	return str;
}
