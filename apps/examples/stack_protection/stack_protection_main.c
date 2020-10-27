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
#define	STACK_NORMAL_SIZE		300
#define	STACK_OVERFLOW_SIZE		4096
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stack_prot_memory_usage
 ****************************************************************************/

#if defined(CONFIG_REG_STACK_OVERFLOW_PROTECTION)
static void stack_overflow_array(void)
{
	volatile char array_normal[STACK_OVERFLOW_SIZE];

	array_normal[0] = 'a';
}

static void stack_prot_inlimit(void)
{
	char array_normal[STACK_NORMAL_SIZE];

	array_normal[0] = 'a';
}

static void stack_overflow_func(int *val)
{
	char *ptr;
	int i = 0;
	volatile char array_normal[STACK_NORMAL_SIZE];

	ptr = &array_normal[STACK_NORMAL_SIZE];
	for (i = 0; i < STACK_NORMAL_SIZE; i++) {
		*ptr = 'a';
		ptr = ptr - 1;
	}

	*val = *val + 1;
	if (*val == (STACK_OVERFLOW_SIZE / STACK_NORMAL_SIZE)) {
		return;
	}
	/* recursive call to create stack overflow condition */
	stack_overflow_func(val);
}
#endif
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int stack_prot_main(int argc, char *argv[])
#endif
{
	char ch;
	int i = 0;
#if defined(CONFIG_MPU_STACK_OVERFLOW_PROTECTION)
	char *ptr;
	char array_normal[STACK_NORMAL_SIZE];
#endif

	printf("Stack Overflow Protection testcase!!\n");

	while (1) {
#if defined(CONFIG_MPU_STACK_OVERFLOW_PROTECTION)
		printf("\nPress M - Test Stack protection with MPU\n");
#elif defined(CONFIG_REG_STACK_OVERFLOW_PROTECTION)
		printf("Press R - Test Stack protection with CPU register\n");
#endif
		printf("Press E - Exit the Test ..\n");
		ch = getchar();

#if defined(CONFIG_MPU_STACK_OVERFLOW_PROTECTION)
		if (ch == 'M' || ch == 'm') {
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
#elif defined(CONFIG_REG_STACK_OVERFLOW_PROTECTION)
		if (ch == 'R' || ch == 'r') {
			printf("\nPress N - Test Stack access within range\n");
			printf("Press O - Test Stack access in overflow\n");
			printf("Press E - Exit the Test ..\n");
			ch = getchar();
			i = 0;
			if (ch == 'N' || ch == 'n') {
				stack_prot_inlimit();
				printf(" Test stack access within range using array size - Passed !!\n");
			} else if (ch == 'O' || ch == 'o') {
				printf("\nPress A - Test Overflow using array declaration\n");
				printf("Press F - Test Overflow using recursive function\n");
				ch = getchar();
				if (ch == 'A' || ch == 'a') {
					stack_overflow_array();
					printf(" Test stack access in overflow - Failed !!\n");
				} else if (ch == 'F' || ch == 'f') {
					stack_overflow_func(&i);
					printf(" Test stack access in overflow - Failed !!\n");
				}
			} else if (ch == 'E' || ch == 'e') {
				printf(" Exit the Test ..\n");
				return 0;
			}
#endif
		} else {
			printf(" Exit the Test ..\n");
			return 0;
		}
	}
	return 0;
}
