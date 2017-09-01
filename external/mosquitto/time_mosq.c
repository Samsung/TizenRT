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
Copyright (c) 2013,2014 Roger Light <roger@atchoo.org>

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

#ifdef __APPLE__
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#ifdef WIN32
#	define _WIN32_WINNT _WIN32_WINNT_VISTA
#	include <windows.h>
#else
#	include <unistd.h>
#endif
#include <time.h>

#include "mosquitto.h"
#include "time_mosq.h"

#ifdef WIN32
static bool tick64 = false;

void _windows_time_version_check(void)
{
	OSVERSIONINFO vi;

	tick64 = false;

	memset(&vi, 0, sizeof(OSVERSIONINFO));
	vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&vi)) {
		if (vi.dwMajorVersion > 5) {
			tick64 = true;
		}
	}
}
#endif

time_t mosquitto_time(void)
{
#ifdef WIN32
	if (tick64) {
		return GetTickCount64() / 1000;
	} else {
		return GetTickCount() / 1000;	/* FIXME - need to deal with overflow. */
	}
#elif (_POSIX_TIMERS > 0) && defined(_POSIX_MONOTONIC_CLOCK)
	struct timespec tp;

	clock_gettime(CLOCK_MONOTONIC, &tp);
	return tp.tv_sec;
#elif defined(__APPLE__)
	static mach_timebase_info_data_t tb;
	uint64_t ticks;
	uint64_t sec;

	ticks = mach_absolute_time();

	if (tb.denom == 0) {
		mach_timebase_info(&tb);
	}
	sec = ticks * tb.numer / tb.denom / 1000000000;

	return (time_t)sec;
#else
	return time(NULL);
#endif
}
