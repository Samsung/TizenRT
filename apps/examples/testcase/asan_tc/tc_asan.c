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

/// @file tc_asan.c
/// @brief Test Case Example for Address Sanitizer
#include <stdio.h>
#include "tc_common.h"
#include <errno.h>

void __asan_load4(unsigned long addr, size_t size);

void tc_asan_load_call_test(void)
{
	int a[3];
	__asan_load4((ulong) & a, (size_t) & a[3]);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: Address Sanitizer test
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tc_asan_main(int argc, char *argv[])
#endif
{
	tc_asan_load_call_test();
	return 0;
}
