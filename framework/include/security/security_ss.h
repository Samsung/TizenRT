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

#ifndef _SECURITY_API_SS_H__
#define _SECURITY_API_SS_H__

#include "security_common.h"

typedef struct security_storage_file {
	char name[20];
	unsigned int attr;
} security_storage_file;

typedef security_storage_file *security_storage_list;

/**
 * Secure Storage
 */
security_error ss_read_secure_storage(security_handle hnd, const char *name, unsigned int offset, security_data *data);
security_error ss_write_secure_storage(security_handle hnd, const char *name, unsigned int offsest, security_data *data);
security_error ss_delete_secure_storage(security_handle hnd, const char *name);
security_error ss_get_size_secure_storage(security_handle hnd, const char *name, unsigned int *size);
security_error ss_get_list_secure_storage(security_handle hnd, security_storage_list *list);

#endif // _SECURITY_API_SS_H__
