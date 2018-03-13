/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <tinyara/mm/mm.h>
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************
 * asan_test_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int asan_test_main(int argc, char *argv[])
#endif
{
	int *a = (int *)malloc(3 * sizeof(int));
	int *b = (int *)malloc(4 * sizeof(int));
	int x, y;
	int c[3];
	srand(x);
	c[0] = 0xdeadbeef;
	char *xp = (char *)&x;
	xp[0] = 0x1;
	printf("c[0]@%08p: %x\n", &c[0], (int)c[0]);
	printf("c[3]@%08p: %x\n", &c[3], (int)c[3]);
	printf("a[0]@%08p: %x\n", &a[0], (int)a[1]);
	printf("a[2]@%08p: %x\n", &a[2], (int)a[2]);
	printf("a[3]@%08p: %x\n", &a[3], (int)a[3]);
	printf("b[3]@%08p: %x\n", &b[4], (int)b[4]);
	printf("c[3]@%08p: %x\n", &c[3], (int)c[3]);
	printf("======================================================\n\n");
	y = rand();
	int *d = a;
	for (x = 0; x < 5; ++x) {
		y = rand() % 4096;
		d = (int *)realloc(d, y * sizeof(int));
		printf("Reallocated %d ints (%d bytes) at %x\n", y, y * sizeof(int), d);
	}
	free(b);
	free(d);
	printf("\nLast line\n");
	return 0;
}
