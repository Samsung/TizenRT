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

#ifndef CLOUD_EVENTDATA_PENDING_PROCESS
#define CLOUD_EVENTDATA_PENDING_PROCESS

#include "easy-setup/es_common.h"

typedef struct es_cloud_pending_event_s {
	bool exist;
	bool is_there_ci_prov;
	int using_count;
	es_cloud_prov_data_s event;
} es_cloud_pending_event_s;

void ci_cp_init_event_pend(void);

es_cloud_prov_data_s *ci_cp_get_pended_data(void);

bool ci_cp_get_is_there_cp(void);

bool ci_cp_cas_is_there_cp_if_false(void);

void ci_cp_del_pended_data(void);

void ci_cp_del_is_there_cp(void);

int ci_cp_pend_event_data(es_cloud_prov_data_s *event_data);

int ci_cp_enter_pend_data_block(void);

int ci_cp_release_penddata_block(void);

#endif							// CLOUD_EVENTDATA_PENDING_PROCESS
