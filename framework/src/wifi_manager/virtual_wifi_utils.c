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

#include <tinyara/config.h>
#include <sys/types.h>
#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <tinyara/wifi/wifi_utils.h>


wifi_utils_result_e wifi_utils_init(void)
{
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_deinit(void)
{
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_scan_ap(void *arg)
{
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_connect_ap(wifi_utils_ap_config_s *ap_connect_config, void *arg)
{
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_disconnect_ap(void *arg)
{
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_get_info(wifi_utils_info_s *wifi_info)
{
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_register_callback(wifi_utils_cb_s *cbk)
{
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_start_softap(wifi_utils_softap_config_s *softap_config)
{
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_start_sta(void)
{
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_stop_softap(void)
{
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_set_autoconnect(uint8_t check)
{
	return WIFI_UTILS_SUCCESS;
}
