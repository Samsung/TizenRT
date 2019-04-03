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

/// @file memory_fragmentation_test.c

/// @brief Intentionally allocate/free small and large memory segments in a mixed-up manner.

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* The number of memory sizes we are handling.
 * Here, we have 12 different memory sizes: 2^4, 2^5, ... , 2^(MAX_SIZE_EXPONENT + 3)
 */
#define MAX_SIZE_EXPONENT 12

/* We consider the first NUMOF_SMALL_MEMORY sizes as relatively small memory.
 * That is, the sizes of 2^4 ~ 2^8 are considered as small memory.
 */
#define NUMOF_SMALL_MEMORY 5

/* Seed for random number */
#define SEED 1

/* Data structure to store allocated memory segments */
struct alloc_list {
	char *data;
	struct alloc_list *next;
	struct alloc_list *prev;
};

static bool memory_allocation(struct alloc_list list[], int numof_size[], int num_alloc[])
{
	struct alloc_list *next[MAX_SIZE_EXPONENT];
	struct alloc_list *item;
	int max_interval;
	int i;
	int interval[MAX_SIZE_EXPONENT] = {0, };
	int cur_interval[MAX_SIZE_EXPONENT] = {0, };

	/* Calculate the maximum allocation count. */
	max_interval = numof_size[0];
	for (i = 1; i < MAX_SIZE_EXPONENT; ++i) {
		if (numof_size[i] > max_interval) {
			max_interval = numof_size[i];
		}
	}

	/* Move 'next' pointer to the end of list to add a new item from the tail */
	for (i = 0; i < MAX_SIZE_EXPONENT; ++i) {
		next[i] = &list[i];
		while (next[i]->next) {
			next[i] = next[i]->next;
		}
	}

	/* Calculate the interval of allocation per size. */
	for (i = 0; i < MAX_SIZE_EXPONENT; ++i) {
		if (numof_size[i] > 0) {
			interval[i] = cur_interval[i] = max_interval / numof_size[i];
		} else if (numof_size[i] == 0) {
			interval[i] = cur_interval[i] = max_interval + 1;
		} else {
			printf("Invalid number of memory allocation with size %d\n", (1 << (i + 4)));
			printf("It should be a positive number.\n");
			return false;
		}
	}

	/* Allocating each memory segment whenever its interval is zero */
	while (max_interval > 0) {
		for (i = 0; i < MAX_SIZE_EXPONENT; ++i) {
			if (cur_interval[i] == 0) {
				if (num_alloc[i] >= numof_size[i]) {
					cur_interval[i] = interval[i] - 1;
					continue;
				}
					
				item = (struct alloc_list *)malloc(sizeof(struct alloc_list));
				if (item) {
					item->data = (char *)malloc((1 << (i + 4) * sizeof(char)));
					/* add a new item at the tail */
					if (item->data) {
						item->next = NULL;
						item->prev = next[i];
						next[i]->next = item;
						next[i] = item;
						++num_alloc[i];
					} else {
						free(item);
						printf("Size %d's %d-th allocation failed.\n", 1 << (i + 4), num_alloc[i]);
						printf("Too many memory allocations were tried.\n");
						return false;
					}
				} else {
					printf("Size %d's %d-th allocation failed.\n", 1 << (i + 4), num_alloc[i]);
					printf("Too many memory allocations were tried.\n");
					return false;
				}
				cur_interval[i] = interval[i] - 1;
			} else {
				--cur_interval[i];
			}
		}
		--max_interval;
	}

	return true;
}

static bool memory_free(int num[], int num_alloc[], struct alloc_list list[])
{
	struct alloc_list *temp;
	int num_free[MAX_SIZE_EXPONENT];
	int i;

	for (i = 0; i < MAX_SIZE_EXPONENT; ++i) {
		num_free[i] = num[i];
	}

	for (i = 0; i < MAX_SIZE_EXPONENT; ++i) {
		while (num_free[i]) {
			temp = list[i].next;
			if (temp) {
				temp->prev->next = temp->next;
				if (temp->next) {
					temp->next->prev = temp->prev;
				} 
				free(temp->data);
				free(temp);
				--num_alloc[i];
			} else {
				return false;
			}
			--num_free[i];
		}
	}

	return true;
}

static void memory_cleanup(int num_alloc[], struct alloc_list list[])
{
	int i;
	int j;
	struct alloc_list *item;
	struct alloc_list *next[MAX_SIZE_EXPONENT];

	/* Free all the memory */
	for (i = 0; i < MAX_SIZE_EXPONENT; ++i) {
		next[i] = list[i].next;
	}
	for (i = 0; i < MAX_SIZE_EXPONENT; ++i) {
		while (next[i]) {
			item = next[i];
			free(item->data);
			next[i] = next[i]->next;
			free(item);
			--num_alloc[i];
		}
	}

	i = 0;
	for (j = 0; j < MAX_SIZE_EXPONENT; ++j) {
		i += num_alloc[j];
	}

	if (i == 0) {
		printf("All the memory is released.\n");
	} else {
		printf("All the memeory can't be released. # of allocation: %d\n", i);
		for (j = 0; j < MAX_SIZE_EXPONENT; ++j) {
			if (num_alloc[j] > 0) {
				printf("%d [Bytes] : %d\n", 1 << (j + 4), num_alloc[j]);
			}
		}
	}
}

static void print_usage(void)
{
	printf("Usage: memfrag param1 param2 repeat\n");
	printf("       'param1' and 'param2' should range [1-9].\n");
	printf("       'param1/10' and 'param2/10' portions of initial small and large memory allocations, respectively,\n");
	printf("       are released and allocated repeatedly 'repeat' times.\n");
	printf("       Initial numbers of memory allocations are given inside the source code.\n");
	printf("       If you want to change these values, modify the values of the array called 'numof_size'.\n");
	exit(-1);
}

static int memory_fragmentation_test(int argc, char *argv[])
{
	int i;
	int j;
	int f1;
	int f2;
	int r;
	int num_alloc[MAX_SIZE_EXPONENT] = {0, };
	int num_alloc_tmp[MAX_SIZE_EXPONENT] = {0, };
	int num_free[MAX_SIZE_EXPONENT] = {0, };
	struct alloc_list list[MAX_SIZE_EXPONENT];

	/* The number of memory segments whose size is 2^(i+4) where i is the index
	 * of the arrary of numof_size[].
	 * The following data was derived from the memory usage of the DA easy-setup
	 * when MAX_SIZE_EXPONENT is 12.
	 */
	// DA easy-setup memory usage 
	//int numof_size[MAX_SIZE_EXPONENT] = {57, 809, 561, 74, 57, 90, 17, 8, 11, 15, 4, 9};
	// Three small memory sizes and two large memory sizes
	int numof_size[MAX_SIZE_EXPONENT] = {700, 300, 200, 0, 0, 0, 0, 0, 70, 50, 0, 0}; 

	for (i = 0; i < MAX_SIZE_EXPONENT; ++i) {
		list[i].data = NULL;
		list[i].next = NULL;
		list[i].prev = NULL;
	}

	if (argc < 4) {
		print_usage();
	}

	f1 = strtol(argv[2], (char **)NULL, 10);
	if ((f1 < 1) || (f1 > 9)) {
		print_usage();
	}

	f2 = strtol(argv[3], (char **)NULL, 10);
	if ((f2 < 1) || (f2 > 9)) {
		print_usage();
	}

	r = strtol(argv[4], (char **)NULL, 10);
	if (r < 0) {
		print_usage();
	}

	srand(SEED);

	/* Allocate memory according to 'numof_size' */
	if (memory_allocation(list, numof_size, num_alloc) == false) {
		printf("memory_allocation failed!\n");
		memory_cleanup(num_alloc, list);
		return 0;
	}

	/* Free the number of memory allocation according to 'num_free' 
	 * with a first-come first-free manner.
	 */
	printf("Test Configuration:\n");
	printf("Size [bytes]	# of init alloc		# of free\n");
	for (i = 0; i < NUMOF_SMALL_MEMORY; ++i) {
		num_free[i] = num_alloc[i] * f1 / 10;
		if (num_alloc[i] > 0) {
			printf("%d		%d			%d\n", 1 << (i + 4), num_alloc[i], num_free[i]);
		}
	}
	for (i = NUMOF_SMALL_MEMORY; i < MAX_SIZE_EXPONENT; ++i) {
		num_free[i] = num_alloc[i] * f2 / 10;
		if (num_alloc[i] > 0) {
			printf("%d		%d			%d\n", 1 << (i + 4), num_alloc[i], num_free[i]);
		}
	}
	if (memory_free(num_free, num_alloc, list) == false) {
		printf("memory_free failed!\n");
		memory_cleanup(num_alloc, list);
		return 0;
	}

	/* Repeat a cycle of allocation and free a given number of times */
	for (i = 0; i < r; ++i) {
		/* memory allocation according to 'num_free' */
		for (j = 0; j < MAX_SIZE_EXPONENT; ++j) {
			num_alloc_tmp[j] = 0;
		}
		if (memory_allocation(list, num_free, num_alloc_tmp) == false) {
			printf("memory_allocation failed!\n");
			memory_cleanup(num_alloc, list);
			return 0;
		}
		for (j = 0; j < MAX_SIZE_EXPONENT; ++j) {
			num_alloc[j] += num_alloc_tmp[j];
		}
		
		/* free allocation according to 'num_free' */
		if (memory_free(num_alloc_tmp, num_alloc, list) == false) {
			printf("memory_free failed!\n");
			memory_cleanup(num_alloc, list);
			return 0;
		}
	}

	printf("\nMemory allocation & free repeated at %d times.\n", r);

	/* wait for 10 seconds before free-ing all the memory */
	printf("\nAll memory test has been done.\n");
	printf("\nMemory allocation status: \n");
	printf("Size [bytes]	# of alloc/free(repeat)	# of remaining alloc\n");
	for (i = 0; i < MAX_SIZE_EXPONENT; ++i) {
		if (num_alloc[i] > 0) {
			printf("%d		%d			%d\n", 1 << (i + 4), num_free[i], num_alloc[i]);
		}
	}
	printf("\nPlease, use 'heapinfo' to see how the heap memory is fragmented in detail.\n");

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int memfrag_main(int argc, char *argv[])
#endif
{
	printf("Memory fragmentation test!!\n");
	task_create("memory fragmentation test", 100, 2048, memory_fragmentation_test, argv);

	sleep(1);

	return 0;
}
