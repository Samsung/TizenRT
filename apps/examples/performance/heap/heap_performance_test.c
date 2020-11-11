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

/// @file heap_performance_test.c

/// @brief Measure the elapsed time while simply repeating memory allocation and release.

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ALLOC 100

static int heap_performance_test(int argc, char *argv[])
{
	struct timespec ts1, ts2;
	int repeat = NUM_ALLOC;
	int size = 32;
	int i, j, k;
	char *data[100];
	int test_repeat = 11;
	uint32_t elapsed = 0;
	uint32_t total_elapsed = 0;
	int sizes[11] = {16, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};
	int interval = 1;

	for (j = 0; j < NUM_ALLOC; ++j) {
		data[j] = NULL;
	}

	if (argc == 4) {
		int in = strtol(argv[2], (char **)NULL, 10);
		if (in > 0) {
			interval = in;
		}
		in = strtol(argv[3], (char **)NULL, 10);
		if (in > 0) {
			repeat = in;
		}
	} else {
		printf("Usage:	%s param1 param2\n", argv[1]);
		printf("	param1 is the interval between tests each of which handles a different size.\n");
		printf("	param2 is the number of repetition of one experiment.\n");
		printf("	In one experiment, param1-sized memory is allocated a hundred times and then released.\n\n");
		printf("At this time, %s will be performed with default values.\n\n", argv[1]);
	}

	printf("\nTest with interval %d, repetition %d.\n", interval, repeat);
	printf("Elapsed time doing a cycle of malloc() and free() %u times:\n", NUM_ALLOC * repeat);

	for (k = 0; k < test_repeat; ++k) {
		size = sizes[k];
		if (clock_gettime(CLOCK_REALTIME, &ts1) == -1) {
			printf("gettime error occured.\n");
			return 0;
		}

		for (i = 0; i < repeat; ++i) {
			for (j = 0; j < NUM_ALLOC; ++j) {
				data[j] = (char *)malloc(size);
				if (data[j] == NULL) {
					printf("With size %d, %d-th, Test failed due to malloc failure.\n", size, j);
					for (i = 0; i < j; ++i) {
						free(data[i]);
					}
					return 0;
				}
			}
			for (j = 0; j < NUM_ALLOC; ++j) {
				free(data[j]);
			}
		}
		if (clock_gettime(CLOCK_REALTIME, &ts2) == -1) {
			printf("gettime error occured.\n");
			return 0;
		}

		if (k > 0) {
			elapsed = ((ts2.tv_sec - ts1.tv_sec) * 1000 + (ts2.tv_nsec - ts1.tv_nsec) / 1000000);
			total_elapsed += elapsed;
			printf("Size %u bytes	: %u mseconds.\n", size, elapsed);
		}

		sleep(interval);
	}

	printf("Total elapsed time : %u mseconds\n", total_elapsed);

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int heaptest_main(int argc, char *argv[])
#endif
{
	printf("Heap Performance Test!!\n");
	task_create("Heap performance test", 100, 6144, heap_performance_test, argv);

	sleep(1);

	return 0;
}
