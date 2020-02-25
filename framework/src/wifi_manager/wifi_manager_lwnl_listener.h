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

#ifndef _WIFI_UTILS_EVENT_H__
#define _WIFI_UTILS_EVENT_H__

void lwnl_start_monitor(void);
void lwnl_stop_monitor(void);
int lwnl_join_monitor(wifi_utils_cb_s *cb);
int lwnl_leave_monitor(wifi_utils_cb_s *cb);

#endif
