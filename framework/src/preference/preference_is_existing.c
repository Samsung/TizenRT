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
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <debug.h>
#include <preference/preference.h>

#include "preference_internal.h"

int preference_is_existing(char *key, bool *existing)
{
	int fd;
	int ret;
	char *path;

	*existing = false;

	if (key == NULL) {
		prefdbg("Invalid parameter\n");
		return PREFERENCE_INVALID_PARAMETER;
	}

	ret = preference_get_keypath(key, &path);
	if (ret < 0) {
		prefdbg("Failed to get preference path\n");
		return ret;
	}

	fd = open(path, O_RDONLY, 0666);
	free(path);

	if (fd >= 0) {
		close(fd);
		*existing = true;
	} else if (errno != ENOENT) {
		prefdbg("Failed to get preference path\n");
		return PREFERENCE_IO_ERROR;
	}

	return OK;
}
