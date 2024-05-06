/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#pragma once

trwifi_result_e wifi_utils_init(void);
trwifi_result_e wifi_utils_deinit(void);
trwifi_result_e wifi_utils_scan_ap(void *arg);
trwifi_result_e wifi_utils_connect_ap(trwifi_ap_config_s *ap_connect_config, void *arg);
trwifi_result_e wifi_utils_disconnect_ap(void *arg);
trwifi_result_e wifi_utils_get_info(trwifi_info *wifi_info);
trwifi_result_e wifi_utils_start_softap(trwifi_softap_config_s *softap_config);
trwifi_result_e wifi_utils_start_sta(void);
trwifi_result_e wifi_utils_stop_softap(void);
trwifi_result_e wifi_utils_set_autoconnect(uint8_t check);
trwifi_result_e wifi_utils_ioctl(trwifi_msg_s *dmsg);
trwifi_result_e wifi_utils_scan_multi_aps(void *arg);
