/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * Author: Sangamanatha <sangam.swami@samsung.com>
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
#include <stdio.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define	STACK_NORMAL_SIZE		10
#define	STACK_OVERFLOW_SIZE		4096
/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int stack_prot_main(int argc, char *argv[])
#endif
{
	char ch;
	char *ptr;
	int i = 0;
	char array_normal[STACK_NORMAL_SIZE];

	printf("Stack Overflow Protection testcase!!\n");

	while (1) {
		printf("\nPress N - Test Stack access within range\n");
		printf("Press O - Test Stack access in overflow\n");
		printf("Press E - Exit the Test ..\n");
		ch = getchar();
		if (ch == 'N' || ch == 'n') {
			ptr = &array_normal[STACK_NORMAL_SIZE];
			for (i = 0; i < STACK_NORMAL_SIZE; i++) {
				*ptr = 'a';
				ptr = ptr - 1;
			}
			printf(" Test stack access within range - Passed !!\n");
		} else if (ch == 'O' || ch == 'o') {
			ptr = &array_normal[STACK_NORMAL_SIZE];
			for (i = 0; i < STACK_OVERFLOW_SIZE; i++) {
				*ptr = 'b';
				ptr = ptr - 1;
			}
			printf(" Test stack access in overflow - Failed !!\n");
		} else if (ch == 'E' || ch == 'e') {
			printf(" Exit the Test ..\n");
			return 0;
		}
	}

	return 0;
}
