/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#ifndef _WIFI_MANAGER_UTILS_H_
#define _WIFI_MANAGER_UTILS_H_

#include "wifi_manager_log.h"
#include "wifi_manager_error.h"

/*  Check Result MACRO */
#define WIFIMGR_SPC // to pass the code check ruls
#define WIFIMGR_CHECK_RESULT_CLEANUP(func, msg, ret, free_rsc)	\
	do {														\
		wifi_manager_result_e wmres = func;						\
		if (wmres != WIFI_MANAGER_SUCCESS) {					\
			WM_LOG_ERROR(msg);											\
			free_rsc;											\
			return ret;											\
		}														\
	} while (0)

#define WIFIMGR_CHECK_RESULT(func, msg, ret) WIFIMGR_CHECK_RESULT_CLEANUP(func, msg, ret, WIFIMGR_SPC)

#define WIFIMGR_PASS_RESULT_CLEANUP(func, msg, free_rsc)	\
	do {													\
		wifi_manager_result_e wmres = func;					\
		if (wmres != WIFI_MANAGER_SUCCESS) {				\
			WM_LOG_ERROR(msg);										\
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_API_FAIL);       \
			free_rsc;										\
			return wmres;									\
		}													\
	} while (0)

#define WIFIMGR_PASS_RESULT(func, msg) WIFIMGR_PASS_RESULT_CLEANUP(func, msg, WIFIMGR_SPC)

#define WIFIMGR_CHECK_RESULT_NORET(func, msg)			\
	do {												\
		wifi_manager_result_e wmres = func;				\
		if (wmres != WIFI_MANAGER_SUCCESS) {			\
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_API_FAIL);	\
			WM_LOG_ERROR(msg);									\
		}												\
	} while (0)											\

#define WIFIMGR_CHECK_UTILRESULT(func, msg, ret)		\
	do {												\
		wifi_utils_result_e wmres = func;				\
		if (wmres != WIFI_UTILS_SUCCESS) {				\
			WM_LOG_ERROR(msg);									\
			WM_LOG_ERROR("error code(%d)\n", wmres);			\
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_UTILS_FAIL);	\
			return ret;									\
		}												\
	} while (0)



/*  Network Interface Card name definition */
#define WIFIMGR_SOFTAP_IFNAME CONFIG_WIFIMGR_SOFTAP_IFNAME
#define WIFIMGR_STA_IFNAME CONFIG_WIFIMGR_STA_IFNAME

struct _wifimgr_conn_info_msg {
	wifi_manager_ap_config_s *config;
	wifi_manager_reconnect_config_s *conn_config;
	int fd;
};
typedef struct _wifimgr_conn_info_msg _wifimgr_conn_info_msg_s;

#endif // _WIFI_MANAGER_UTILS_H_
