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
#include <stdio.h>
#include <semaphore.h>

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int mpu_tc_main(int argc, char *argv[])
#endif
{
	char ch;
	printf("\nPress R - For Read Test\n");
	printf("Press W - For Write Test\n");

	ch = getchar();
	if (ch == 'R' || ch == 'r') {
		printf("\nPress C - For Read Kernel Code Test\n");
		printf("Press D - For Read Kernel Data Test\n");

		ch = getchar();
		if (ch == 'C' || ch == 'c') {
#ifdef CONFIG_TC_MPU_READ_CODE
			read_code_main();
#else
			printf("\nCan't Run Test: CONFIG_TC_MPU_READ_CODE is not set\n");
#endif
		} else if (ch == 'D' || ch == 'd') {
#ifdef CONFIG_TC_MPU_READ_DATA
			read_data_main();
#else
			printf("\nCan't Run Test: CONFIG_TC_MPU_READ_DATA is not set\n");
#endif
		} else {
			goto out;
		}
	} else if (ch == 'W' || ch == 'w') {
		printf("\nPress C - For Write Kernel Code Test\n");
		printf("Press D - For Write Kernel Data Test\n");

		ch = getchar();
		if (ch == 'C' || ch == 'c') {
#ifdef CONFIG_TC_MPU_WRITE_CODE
			write_code_main();
#else
			printf("\nCan't Run Test: CONFIG_TC_MPU_WRITE_CODE is not set\n");
#endif
		} else if (ch == 'D' || ch == 'd') {
#ifdef CONFIG_TC_MPU_WRITE_DATA
			write_data_main();
#else
			printf("\nCan't Run Test: CONFIG_TC_MPU_WRITE_DATA is not set\n");
#endif
		} else {
			goto out;
		}
	} else {
		goto out;
	}

	return 0;

out:
	printf("\nEntered An Invalid Key!!\nExiting...\n");
	return ERROR;
}
