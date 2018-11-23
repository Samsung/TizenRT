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

#ifndef _WIFI_PROFILE_H__
#define _WIFI_PROFILE_H__

wifi_utils_result_e wifi_profile_init(void);
wifi_utils_result_e wifi_profile_reset(int internal);
wifi_utils_result_e wifi_profile_write(wifi_manager_ap_config_s *config, int internal);
wifi_utils_result_e wifi_profile_read(wifi_manager_ap_config_s *config, int internal);

#endif //_WIFI_PROFILE_H__
