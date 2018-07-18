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

#ifndef __PREFERENCE_INTERNAL_H__
#define __PREFERENCE_INTERNAL_H__

#ifndef CONFIG_MOUNT_POINT
#define MOUNT_PATH "/mnt"
#else
#define MOUNT_PATH CONFIG_MOUNT_POINT
#endif

#define PREF_PATH MOUNT_PATH"/pref"

#define PREFERENCE_TYPE_INT    0
#define PREFERENCE_TYPE_DOUBLE 1
#define PREFERENCE_TYPE_BOOL   2
#define PREFERENCE_TYPE_STRING 3

typedef struct preference_data_s {
	int type;
	int len;
	union {
		int i;
		double d;
		char *s;
	} value;
} preference_data_t;

int preference_get_dirpath(char **path);
int preference_get_keypath(char *key, char **path);
#endif
