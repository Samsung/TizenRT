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

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/types.h>

static void print_usage(void)
{
	printf("Usage: mem_leak [--heap=<name>] [--pid=<value>]\n");
	printf("\nOptions:\n");
	printf("  --heap=<name>    Check only the specified heap (default: all heaps)\n");
	printf("                    Valid names: kernel, app1, app2, ...\n");
	printf("  --pid=<value>    Filter detailed leak output by PID\n");
	printf("                    0  = summary only (default)\n");
	printf("                    -1 = show details for ALL PIDs\n");
	printf("                    N  = show details for PID N only\n");
	printf("\nExamples:\n");
	printf("  mem_leak                        Check all heaps, summary only\n");
	printf("  mem_leak --heap=kernel          Check kernel heap only\n");
	printf("  mem_leak --heap=app1 --pid=-1   Check app1, show all details\n");
}

int mem_leak_checker_v2_main(int argc, char **argv)
{
	int ret;
	int filter_pid = 0;          /* 0 = summary only (default) */
	const char *heap_name = NULL; /* NULL = all heaps (default) */
	int i;

	/* Parse command line arguments */
	for (i = 1; i < argc; i++) {
		if (strncmp(argv[i], "--pid=", 6) == 0) {
			filter_pid = atoi(argv[i] + 6);
		} else if (strncmp(argv[i], "--heap=", 7) == 0) {
			heap_name = argv[i] + 7;
		} else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
			print_usage();
			return OK;
		} else {
			printf("Unknown argument: %s\n\n", argv[i]);
			print_usage();
			return ERROR;
		}
	}

	/* Pass both filter_pid and heap_name to kernel.
	 * If heap_name is NULL, kernel checks all heaps.
	 * If heap_name is non-NULL, kernel checks only that heap.
	 */
	ret = prctl(PR_MEM_LEAK_CHECKER, getpid(), filter_pid, heap_name);
	if (ret < 0) {
		printf("Fail to launch MEMORY LEAK CHECKER.\n");
		return ERROR;
	}

	return OK;
}
