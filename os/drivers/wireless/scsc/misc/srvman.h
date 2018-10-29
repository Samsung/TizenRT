/*****************************************************************************
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

#ifndef _SRVMAN_H
#define _SRVMAN_H

#include <sys/types.h>

struct srvman;

void srvman_init(struct srvman *srvman, struct scsc_mx *mx);
void srvman_freeze_services(struct srvman *srvman);
void srvman_set_error(struct srvman *srvman);
void srvman_deinit(struct srvman *srvman);

struct srvman {
	struct scsc_mx *mx;
	struct slsi_dlist_head service_list;
	pthread_mutex_t service_list_mutex;
	pthread_mutex_t api_access_mutex;
	bool error;
};

#endif
