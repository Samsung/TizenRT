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

#ifndef _IOTAPI_EVT_HANDLER_H__
#define _IOTAPI_EVT_HANDLER_H__

struct iotapi_elem_s {
	int fd;
	int idx;
	void *data;
	void (*func)(void *data);
};
typedef struct iotapi_elem_s iotapi_elem;

void iotapi_init(void);
int iotapi_insert(iotapi_elem *item);
int iotapi_remove(iotapi_elem *item);

#endif // #define _IOTAPI_EVT_HANDLER_H__
