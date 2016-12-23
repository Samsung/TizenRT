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
	printf("=== TINYARA MPU TC START! ===\n");
	printf("Press R - For Read Test\n");
	printf("Press W - For Write Test\n");

	ch = getchar();
	if (ch == 'R' || ch == 'r')
#ifdef CONFIG_TC_MPU_READ
		read_main();
#endif
	else if (ch == 'W' || ch == 'w')
#ifdef CONFIG_TC_MPU_WRITE
		write_main();
#endif

	printf("\n=== TINYARA MPU TC COMPLETE ===\n");

	return 0;
}
