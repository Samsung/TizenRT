/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <tinyara/sched.h>
#include <tinyara/mm/mm.h>

/************************************************************************
 * Name: heap_dbg
 *
 * Description: Dump heap info debug logs without adding function name to start of log.
 ************************************************************************/
int heap_dbg(const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
#if defined(CONFIG_BUILD_FLAT) || defined(__KERNEL__)
	extern bool abort_mode;

	if (abort_mode) {
		ret = lowvsyslog(LOG_ERR, fmt, ap);
	} else {
		ret = vsyslog(LOG_ERR, fmt, ap);
	}
#else
	ret = vsyslog(LOG_ERR, fmt, ap);
#endif
	va_end(ap);

	return ret;
}

/************************************************************************
 * Name: mm_dump_heap_region
 *
 * Description: Print the hex value contents of heap from start to end address.
 ************************************************************************/
void mm_dump_heap_region(uint32_t start, uint32_t end)
{
	heap_dbg("#########################################################################################\n");
	heap_dbg("Dump heap: 0x%08x - 0x%08x\n", start, end);
	heap_dbg("#########################################################################################\n");
	for (; start < end; start += 32) {
		uint32_t *ptr = (uint32_t *)start;
		heap_dbg("%08x: %08x %08x %08x %08x %08x %08x %08x %08x\n",
			   start, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
	}
	heap_dbg("#########################################################################################\n");
}

