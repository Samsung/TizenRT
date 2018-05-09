/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <tinyara/clock.h>

int kdbg_uptime(int argc, char **args)
{
	clock_t ticktime;
#if defined(CONFIG_HAVE_DOUBLE) && defined(CONFIG_LIBC_FLOATINGPOINT)
	double now;
#else
#ifdef CONFIG_SYSTIME_TIME64
	uint64_t sec;
#else
	uint32_t sec;
#endif
	unsigned int csec;
#endif

	ticktime = clock();

#if defined(CONFIG_HAVE_DOUBLE) && defined(CONFIG_LIBC_FLOATINGPOINT)
	now = (double)ticktime / (double)CLOCKS_PER_SEC;
	printf("Uptime : %.2f\n", now);
#else
	/* Convert the system up time to seconds + hundredths of seconds */
	sec = ticktime / CLOCKS_PER_SEC;
	csec = (100 * (ticktime % CLOCKS_PER_SEC) + (CLOCKS_PER_SEC / 2)) / CLOCKS_PER_SEC;

	/* Make sure that rounding did not force the hundredths of a second above 99 */
	if (csec > 99) {
		sec++;
		csec -= 100;
	}
	printf("Uptime : %u.%u\n", sec, csec);
#endif
	return OK;
}
