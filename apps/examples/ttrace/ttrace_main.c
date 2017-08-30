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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/ttrace.h>
#include <stdio.h>
#include <unistd.h>

#define PRINT_INTERVAL_SECS 1000000

/****************************************************************************
 * ttrace_main
 ****************************************************************************/

void core_lock_func(void);
void inner_lock_func(void);
void outer_lock_func(void);

void core_ipc_func(void);
void inner_ipc_func(void);
void outer_ipc_func(void);

void core_libs_func(void);
void inner_libs_func(void);
void outer_libs_func(void);

void core_apps_func(void);
void inner_apps_func(void);
void outer_apps_func(void);

void core_lock_func()
{
	trace_begin(TTRACE_TAG_LOCK, "tag lock, core\r\n");
	usleep(PRINT_INTERVAL_SECS);
	trace_end(TTRACE_TAG_LOCK);
}

void inner_lock_func()
{
	trace_begin(TTRACE_TAG_LOCK, "tag lock, inner\r\n");
	usleep(PRINT_INTERVAL_SECS);
	core_lock_func();
	trace_end(TTRACE_TAG_LOCK);
}

void outer_lock_func()
{
	trace_begin(TTRACE_TAG_LOCK, "tag lock, outer\r\n");
	usleep(PRINT_INTERVAL_SECS);
	inner_lock_func();
	trace_end(TTRACE_TAG_LOCK);
}

void core_ipc_func()
{
	trace_begin(TTRACE_TAG_IPC, "tag ipc, core\r\n");
	usleep(PRINT_INTERVAL_SECS);
	trace_end(TTRACE_TAG_IPC);
}

void inner_ipc_func()
{
	trace_begin(TTRACE_TAG_IPC, "tag ipc, inner\r\n");
	usleep(PRINT_INTERVAL_SECS);
	core_ipc_func();
	trace_end(TTRACE_TAG_IPC);
}

void outer_ipc_func()
{
	trace_begin(TTRACE_TAG_IPC, "tag ipc, outer\r\n");
	usleep(PRINT_INTERVAL_SECS);
	inner_ipc_func();
	trace_end(TTRACE_TAG_IPC);
}

void core_libs_func()
{
	trace_begin(TTRACE_TAG_LIBS, "tag libs, core\r\n");
	usleep(PRINT_INTERVAL_SECS);
	trace_end(TTRACE_TAG_LIBS);
}

void inner_libs_func()
{
	trace_begin(TTRACE_TAG_LIBS, "tag libs, inner\r\n");
	usleep(PRINT_INTERVAL_SECS);
	core_libs_func();
	trace_end(TTRACE_TAG_LIBS);
}

void outer_libs_func()
{
	trace_begin(TTRACE_TAG_LIBS, "tag libs, outer\r\n");
	usleep(PRINT_INTERVAL_SECS);
	inner_libs_func();
	trace_end(TTRACE_TAG_LIBS);
}

void core_apps_func()
{
	trace_begin(TTRACE_TAG_APPS, "tag apps, core\r\n");
	usleep(PRINT_INTERVAL_SECS);
	trace_end(TTRACE_TAG_APPS);
}

void inner_apps_func()
{
	trace_begin(TTRACE_TAG_APPS, "tag apps, inner\r\n");
	usleep(PRINT_INTERVAL_SECS);
	core_apps_func();
	trace_end(TTRACE_TAG_APPS);
}

void outer_apps_func()
{
	trace_begin(TTRACE_TAG_APPS, "tag apps, outer\r\n");
	usleep(PRINT_INTERVAL_SECS);
	inner_apps_func();
	trace_end(TTRACE_TAG_APPS);
}


#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int ttrace_main(int argc, char *argv[])
#endif
{
	while (1) {
		trace_begin_uid(TTRACE_TAG_APPS, 1);
		trace_begin_uid(TTRACE_TAG_LIBS, 2);
		trace_begin_uid(TTRACE_TAG_IPC, 5);

		outer_apps_func();
		outer_libs_func();
		outer_lock_func();

		trace_end_uid(TTRACE_TAG_IPC);
		trace_end_uid(TTRACE_TAG_LIBS);
		trace_end_uid(TTRACE_TAG_APPS);
	}
	return 0;
}
