/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
/// @file sg_tc_main.c
/// @brief Main Function for TestCase Example for stack guard
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

/****************************************************************************
 * Name: recursion_main
 ****************************************************************************/
/// @brief Test Case Example to test stack guard during recurrsive call
#ifdef CONFIG_TC_SG_RECURSION_TEST
int recursion_main(void)
{
	static uint32_t count = 1;

	printf("%s:%d\n", __func__, count++);
	recursion_main();
	printf("ERR: Failed to guard the task stack\n");
	return 0;
}
#endif

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int sg_tc_main(int argc, char *argv[])
#endif
{
	printf("=== TINYARA STACKGUARD TC START! ===\n");

#ifdef CONFIG_TC_SG_RECURSION_TEST
	recursion_main();
#endif

	return 0;
}
