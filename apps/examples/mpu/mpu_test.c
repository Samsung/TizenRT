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
/// @file mpu_tc_main.c
/// @brief Main Function for MPU TestCase Example
#include <tinyara/config.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

#if !defined(CONFIG_MPUTEST_KERNEL_CODE_ADDR) || !defined(CONFIG_MPUTEST_KERNEL_DATA_ADDR) ||\
	(!defined(CONFIG_MPUTEST_APP_ADDR) && defined(CONFIG_APP_BINARY_SEPARATION))
#error "Address not defined for MPU TEST"
#endif

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int mpu_tc_main(int argc, char *argv[])
#endif
{
	volatile uint32_t *address;
	char ch;

	printf("\nPress R - For Read Test\n");
	printf("Press W - For Write Test\n");

	ch = getchar();
	if (ch == 'R' || ch == 'r') {
#ifdef CONFIG_MPUTEST_APP_ADDR
		printf("\nPress A for app MPU test\n");
#endif
		printf("Press K for kernel MPU test\n");

		ch = getchar();

#ifdef CONFIG_MPUTEST_APP_ADDR
		if (ch == 'A' || ch == 'a') {
			address = (uint32_t *)CONFIG_MPUTEST_APP_ADDR;

			uint32_t dest;

			printf("\n**************************************************\n");
			printf("**************************************************\n");
			printf("**************************************************\n");
			printf("* Test to verify protection of another app space *\n");
			printf("**************************************************\n");
			printf("**************************************************\n");
			printf("**************************************************\n");

			printf("INFO: Read another app space: 0x%x\n", address);
			sleep(1);
			dest = *address;

			printf("ERROR: This Task made invalid access to another app space\n");
		} else
#endif
		{

			printf("\nPress C - For Read Code Test\n");
			printf("Press D - For Read Data Test\n");

			ch = getchar();
			if (ch == 'C' || ch == 'c') {
				address = (uint32_t *)CONFIG_MPUTEST_KERNEL_CODE_ADDR;

				uint32_t dest;

				printf("\n************************************************\n");
				printf("* Test to verify protection of Kernel code     *\n");
				printf("* User Tasks should not be allowed to read     *\n");
				printf("* kernel code space. MPU shall raise exception *\n");
				printf("************************************************\n");
				printf("INFO: Read Code: 0x%x\n", address);

				sleep(1);
				dest = *address;

				printf("ERROR: User Task made invalid access to Kernel code space\n");
			} else if (ch == 'D' || ch == 'd') {
			       address = (uint32_t *)CONFIG_MPUTEST_KERNEL_DATA_ADDR;

				uint32_t dest;

				printf("\n************************************************\n");
				printf("* Test to verify protection of Kernel data     *\n");
				printf("* User Tasks should not be allowed to read     *\n");
				printf("* kernel data space. MPU shall raise exception *\n");
				printf("************************************************\n");
				printf("INFO: Read Data: 0x%x\n", address);

				sleep(1);
				dest = *address;


				printf("ERROR: User Task made invalid access to Kernel data space\n");
			}
		}
	} else if (ch == 'W' || ch == 'w') {
#ifdef CONFIG_MPUTEST_APP_ADDR
		printf("\nPress A for app MPU test\n");
#endif
		printf("Press K for kernel MPU test\n");

		ch = getchar();

#ifdef CONFIG_MPUTEST_APP_ADDR
		if (ch == 'A' || ch == 'a') {
			address = (uint32_t *)CONFIG_MPUTEST_APP_ADDR;

			uint32_t dest = 0xdeadbeef;

			printf("\n**************************************************\n");
			printf("**************************************************\n");
			printf("**************************************************\n");
			printf("* Test to verify protection of another app space *\n");
			printf("**************************************************\n");
			printf("**************************************************\n");
			printf("**************************************************\n");
			printf("INFO: Write another app space: 0x%x\n", address);

			sleep(1);
			*address = dest;

			printf("ERROR: This Task made invalid access to another app space\n");
		} else
#endif
		{

			printf("\nPress C - For Write Kernel Code Test\n");
			printf("Press D - For Write Kernel Data Test\n");

			ch = getchar();
			if (ch == 'C' || ch == 'c') {
				address = (uint32_t *)CONFIG_MPUTEST_KERNEL_CODE_ADDR;
				uint32_t dest = 0xdeadbeef;

				printf("\n************************************************\n");
				printf("* Test to verify protection of Kernel code     *\n");
				printf("* User Tasks should not be allowed to write    *\n");
				printf("* kernel code space. MPU shall raise exception *\n");
				printf("************************************************\n");
				printf("INFO: Write kernel space: 0x%x\n", address);

				sleep(1);
				*address = dest;

				printf("ERROR: User Task made invalid access to Kernel code space\n");
			} else if (ch == 'D' || ch == 'd') {
				address = (uint32_t *)CONFIG_MPUTEST_KERNEL_DATA_ADDR;
				uint32_t dest = 0xdeadbeef;

				printf("\n************************************************\n");
				printf("* Test to verify protection of Kernel data     *\n");
				printf("* User Tasks should not be allowed to write    *\n");
				printf("* kernel data space. MPU shall raise exception *\n");
				printf("************************************************\n");
				printf("INFO: Write kernel space: 0x%x\n", address);

				sleep(1);
				*address = dest;

				printf("ERROR: User Task made invalid access to Kernel data space\n");
			}
		}
	}

	return OK;
}
