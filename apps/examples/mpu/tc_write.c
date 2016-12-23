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
/// @file tc_write.c
/// @brief Test Case Example for write to kernel space from user space

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

extern uint32_t __ksram_segment_start__[];
/****************************************************************************
 * Name: read_main
 ****************************************************************************/

int write_main(void)
{
	uint32_t *address = (uint32_t *)(__ksram_segment_start__);
	uint32_t dest = 0xdeadbeef;

	printf("************************************************\n");
	printf("* Test to verify protection of Kernel data     *\n");
	printf("* User Tasks should not be allowed to write     *\n");
	printf("* kernel data space. MPU shall raise exception *\n");
	printf("************************************************\n");

	sleep(3);
	*address = dest;

	printf("ERR: User Task successfully accessed Kernel space\n");
	return 0;
}
