/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

/// @file heap_stat_realtime.c

/// @brief Show the stat of heap for realtime support such as the number of misses of memory allocation within a constant time.

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/mm/mm.h>
#include <stdio.h>

static int heap_stat_realtime(int argc, char *argv[])
{
	char *addr;
	int ndx;
	int ret;
	size_t miss[MM_REALTIME_SUPPORT_NUMOF_SIZES] = { 0, };
	int num[MM_REALTIME_SUPPORT_NUMOF_SIZES] = { 0, };

	addr = (char *)malloc(sizeof(char));
	if (!addr) {
		printf("No more available memory to proceed.\n");
		return -1;
	}

	if (argc > 2) {
		if (strncmp(argv[2], "reset", 6) == 0) {
			ret = mm_heapstat_reset(addr);
			if (!ret) {
				printf("\nReset the stat of heap for realtime support.");
			} else {
				goto err;
			}
		} else {
			printf("Usage:	%s [reset]\n", argv[1]);
			goto close;
		}
	}

	ret = mm_heapstat_get(addr, miss, num);

	if (!ret) {
		printf("\nNumber of memory requests not allocated within a constant time: \n");
		for (ndx = 0; ndx < MM_REALTIME_SUPPORT_NUMOF_SIZES; ++ndx) {
			printf("Size %d bytes : %u times, given %d reserved nodes.\n", MM_REALTIME_SUPPORT_STEP_SIZE * (ndx + 1), miss[ndx], num[ndx]);
		}
		goto close;
	}

err:
	printf("\nCan't find the heap.\n");
close:
	free(addr);
	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int heapstat_main(int argc, char *argv[])
#endif
{
	printf("Show the stat of heap for realtime support!!\n");
	task_create("Heap stat for realtime support", 100, 1024, heap_stat_realtime, argv);

	sleep(1);

	return 0;
}
