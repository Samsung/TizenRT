/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <debug.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#include <tinyara/preference.h>

int preference_init(void)
{
	int ret;

	/* Make preference directory */
	ret = mkdir(PREF_PATH, 0777);
	if (ret < 0 && errno != EEXIST) {
		prefdbg("mkdir fail, %d\n", errno);
		return PREFERENCE_IO_ERROR;
	}
#if CONFIG_TASK_NAME_SIZE > 0

	/* Make private preference directory */
	ret = mkdir(PREF_PRIVATE_PATH, 0777);
	if (ret < 0 && errno != EEXIST) {
		prefdbg("mkdir fail, %d\n", errno);
		return PREFERENCE_IO_ERROR;
	}
#endif

	/* Make shared preference directory */
	ret = mkdir(PREF_SHARED_PATH, 0777);
	if (ret < 0 && errno != EEXIST) {
		prefdbg("mkdir fail, %d\n", errno);
		return PREFERENCE_IO_ERROR;
	}

	return OK;
}
