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
#pragma once

#include <tinyara/net/netlog.h>
#include "wifi_manager_error.h"

#ifndef __LINUX__
#define net_task_create task_create
#else
int net_task_create(FAR const char *name, int priority, int stack_size,
                    main_t entry, FAR char *const argv[]);
#endif
/*  Check Result MACRO */
#define WIFIMGR_SPC // to pass the code check ruls
#define WIFIMGR_CHECK_RESULT_CLEANUP(func, msg, ret, free_rsc)	\
	do {														\
		wifi_manager_result_e wmres = func;						\
		if (wmres != WIFI_MANAGER_SUCCESS) {					\
			NET_LOGE msg;										\
			free_rsc;											\
			return ret;											\
		}														\
	} while (0)

#define WIFIMGR_CHECK_RESULT(func, msg, ret) WIFIMGR_CHECK_RESULT_CLEANUP(func, msg, ret, WIFIMGR_SPC)

#define WIFIMGR_PASS_RESULT_CLEANUP(func, msg, free_rsc)	\
	do {													\
		wifi_manager_result_e wmres = func;					\
		if (wmres != WIFI_MANAGER_SUCCESS) {				\
			NET_LOGE msg;									\
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
			NET_LOGE msg;								\
		}												\
	} while (0)											\

#define WIFIMGR_CHECK_UTILRESULT(func, tag, msg)		\
	do {												\
		trwifi_result_e wmres = func;					\
		if (wmres != TRWIFI_SUCCESS) {					\
			NET_LOGE(TAG, msg " reason(%d)\n", wmres);	\
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_UTILS_FAIL);	\
			return wifimgr_convert2wifimgr_res(wmres);		\
		}												\
	} while (0)

/* errno EINVAL shouldn't happen
 * if it happens then the system could be corrupted */
#define WIFIMGR_WAIT_SIG(signal)		\
	do {                              \
		if (sem_wait(signal) == -1) { \
			if (errno == EINTR) {     \
				continue;             \
			}                         \
			assert(0);                \
		}                             \
	} while (0)

/*  Network Interface Card name definition */
#define WIFIMGR_SOFTAP_IFNAME CONFIG_WIFIMGR_SOFTAP_IFNAME
#define WIFIMGR_STA_IFNAME CONFIG_WIFIMGR_STA_IFNAME

wifi_manager_result_e wifimgr_convert2wifimgr_res(trwifi_result_e tres);
trwifi_ap_auth_type_e wifimgr_convert2trwifi_auth(wifi_manager_ap_auth_type_e atype);
wifi_manager_ap_auth_type_e wifimgr_convert2wifimgr_auth(trwifi_ap_auth_type_e atype);
trwifi_ap_crypto_type_e wifimgr_convert2trwifi_crypto(wifi_manager_ap_crypto_type_e ctype);
wifi_manager_ap_crypto_type_e wifimgr_convert2wifimgr_crypto(trwifi_ap_crypto_type_e ctype);
