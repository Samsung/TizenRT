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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <semaphore.h>

/****************************************************************************
 * stack_test_main: Test application to verify Stack Protection feature
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int stack_test_main(int argc, char *argv[])
#endif
{
	char ch[8];

	printf("\n*************************************************\n"
	"* Test to verify User Stack protetion feature,  *\n"
	"* User task should not be allowed to            *\n"
	"* access out of it's stack boundary.            *\n"
	"* MPU shall raise exception.                    *\n"
	"*************************************************\n");

	printf("\nTrying to access out of %s task's stack boundary...\n\n", __func__);

	// Access Out of this task's stack boundary
	printf("%x\n", ch[4097]);

	return 0;
}
