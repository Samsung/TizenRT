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

/// @file itc_libc_misc.c
/// @brief Scenario Test Case for Libc Misc API

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include "tc_internal.h"

#define COUNT 10

/**
 * @fn                  :itc_libc_misc_init_give_take_semaphore_p_multitime
 * @brief               :Initilizes file stream semaphore, Takes(locks) the file stream, and gives(unlocks) the file stream
 * @scenario            :Initialize file stream semaphore, and take the stream and give it multiple times
 * @API's covered       :lib_sem_initialize, lib_take_semaphore, lib_give_semaphore
 * @Preconditions       :None
 * @Postconditions      :None
 */
static void itc_libc_misc_init_give_take_semaphore_p_multitime(void)
{
	struct file_struct stream;
	pid_t pid = getpid();
	int i;

	lib_sem_initialize(&stream);
	TC_ASSERT_EQ("lib_sem_initialize", stream.fs_holder, -1);
	TC_ASSERT_EQ("lib_sem_initialize", stream.fs_counts, 0);

	for (i = 1; i <= COUNT; i++) {
		lib_take_semaphore(&stream);
		TC_ASSERT_EQ("lib_take_semaphore", stream.fs_holder, pid);
		TC_ASSERT_EQ("lib_take_semaphore", stream.fs_counts, 1);

		lib_give_semaphore(&stream);
		TC_ASSERT_EQ("lib_give_semaphore", stream.fs_holder, -1);
		TC_ASSERT_EQ("lib_give_semaphore", stream.fs_counts, 0);
	}

	for (i = 1; i <= COUNT; i++) {
		lib_take_semaphore(&stream);
		TC_ASSERT_EQ("lib_take_semaphore", stream.fs_holder, pid);
		TC_ASSERT_EQ("lib_take_semaphore", stream.fs_counts, i);
	}

	for (i = COUNT - 1; i > 0; i--) {
		lib_give_semaphore(&stream);
		TC_ASSERT_EQ("lib_give_semaphore", stream.fs_holder, pid);
		TC_ASSERT_EQ("lib_give_semaphore", stream.fs_counts, i);
	}

	lib_give_semaphore(&stream);
	TC_ASSERT_EQ("lib_give_semaphore", stream.fs_holder, -1);
	TC_ASSERT_EQ("lib_give_semaphore", stream.fs_counts, 0);

	TC_SUCCESS_RESULT();
}

int itc_libc_misc_main(void)
{
	itc_libc_misc_init_give_take_semaphore_p_multitime();

	return 0;
}
