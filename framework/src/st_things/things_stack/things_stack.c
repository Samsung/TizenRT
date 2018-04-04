/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE				// for the usleep
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#ifdef CONFIG_ST_THINGS_FOTA
#include "octypes.h"
#include <json/cJSON.h>
#endif
#include "things_api.h"
#include "things_types.h"
#include "things_def.h"
#include "framework/things_common.h"
#include "logging/things_logger.h"
#include "utils/things_malloc.h"
#include "utils/things_string.h"

#include "things_resource.h"
#include "framework/things_server_builder.h"
#include "framework/things_security_manager.h"
#include "framework/things_req_handler.h"

#include "framework/things_data_manager.h"

#include "utils/things_network.h"
#include "easy-setup/easysetup_manager.h"
#include "cloud/cloud_manager.h"
#include "cloud/cloud_evtpend.h"

#include <wifi_manager/wifi_manager.h>

#include "utils/things_thread.h"
#ifdef __ST_THINGS_RTOS__
#include "utils/things_rtos_util.h"
#endif
#ifdef CONFIG_ST_THINGS_FOTA
#include "fota/fmwup_api.h"
#include "deviceDef.h"
#define DEVICE_DEF_FILE_ROOT "/mnt/"
#else
#define DEVICE_DEF_FILE_ROOT "/rom/"
#endif

#ifdef CONFIG_NETUTILS_NTPCLIENT
#include <protocols/ntpclient.h>
#endif

#define TAG "[things_stack]"

typedef void *(*pthread_func_type)(void *);

volatile static int g_quit_flag = 0;
volatile static int is_things_module_inited = 0;

typedef struct s_abort_s {
	pthread_t *h_thread;
	things_es_enrollee_abort_e level;
} s_abort_s;

typedef struct reset_args_s {
	things_resource_s *remote_owner;
	things_es_enrollee_reset_e resetType;
} reset_args_s;

things_abort_easysetup_func_type g_abort_easysetup = NULL;

things_server_builder_s *g_server_builder = NULL;
things_request_handler_s *g_req_handler = NULL;

things_reset_confirm_func_type g_confirm_reset_start = NULL;
things_sec_otm_state_func_type g_otm_event_handler = NULL;

static pthread_mutex_t g_app_cb_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t g_things_stop_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t g_wifi_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_wifi_cond;
static wifi_manager_scan_info_s *g_wifi_scan_info;
static int g_wifi_scan_count;

#define WIFI_SCAN_SIGNAL			\
do {										\
	pthread_mutex_lock(&g_wifi_mutex);		\
	pthread_cond_signal(&g_wifi_cond);		\
	pthread_mutex_unlock(&g_wifi_mutex);		\
} while (0)

#define WIFI_SCAN_WAIT						\
do {										\
	pthread_mutex_lock(&g_wifi_mutex);			\
	pthread_cond_wait(&g_wifi_cond, &g_wifi_mutex);	\
	pthread_mutex_unlock(&g_wifi_mutex);			\
} while (0)

bool b_reset_continue_flag = false;
static bool b_thread_things_reset = false;
static pthread_t h_thread_things_reset = NULL;
static pthread_t h_thread_things_wifi_join = NULL;
static pthread_mutex_t m_thread_oic_reset = PTHREAD_MUTEX_INITIALIZER;
static volatile rst_state_e m_reset_bit_mask = RST_COMPLETE;

static void *t_things_reset_loop(reset_args_s *args);
static void *t_things_abort_loop(s_abort_s *contents);

const char *things_get_device_id_string()
{
	return OCGetServerInstanceIDString();
}

int things_set_device_name(int dev_num, char *name)
{
	int res = 0;
	if (!is_things_module_inited) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack not initialized");
	} else {
		if (dev_num > 0) {
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Dev Num %d Not Supported Yet", dev_num);
		} else {
			if (NULL != name && strlen(name) > 0) {
				OCStackResult result = OCSetPropertyValue(PAYLOAD_TYPE_DEVICE,
									   OC_RSRVD_DEVICE_NAME,
									   name);
				if (result == OC_STACK_OK) {
					res = 1;
				} else {
					THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Set Device Name Failed : %d ", result);
				}
			} else {
				THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid Device Name : %s ", name);
			}
		}
	}

	return res;
}

// This logic will be refactored later to reflact whole easy-setup states..
int things_get_easysetup_state(void)
{
	bool isOwned = false;
	// 1. check the owned state
	if (OC_STACK_OK != OCGetDeviceOwnedState(&isOwned)) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Failed to get device owned state, Informing as UNOWNED~!!!!");
		isOwned = false;
	}
	// 2. check the resource publish state if necessary..

	// 3. anything else to check???

	return (isOwned == true);
}

int things_register_easysetup_state_func(things_get_easysetup_state_func_type func)
{
	int res = 0;

	if (func != NULL) {
		es_register_notify_easysetup_state((void *)func);
		res = 1;
	}

	return res;
}

#ifdef CONFIG_NETUTILS_NTPCLIENT
static void ntp_link_error(void)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "ntp_link_error() callback is called");
}

static void sync_time_from_ntp(int guarantee_secs)
{
	THINGS_LOG_D(THINGS_INFO, TAG, "T%d --> %s", getpid(), __FUNCTION__);

	if (ntpc_get_status() != NTP_RUNNING) {
		struct timespec pre_tp;
		clock_gettime(CLOCK_REALTIME, &pre_tp);

		struct timespec init_tp;
		init_tp.tv_sec = 0;
		init_tp.tv_nsec = 0;
		clock_settime(CLOCK_REALTIME, &init_tp);

		if (ntpc_start(NULL, 0, 0, ntp_link_error) < 0) {
			THINGS_LOG_ERROR(THINGS_ERROR, TAG, "ntpc_start() failed");
		} else {
			THINGS_LOG(THINGS_INFO, TAG, "ntpc_start() OK");
			int time_sync = 0;

			while (1) {
				struct timespec sync_tp;
				clock_gettime(CLOCK_REALTIME, &sync_tp);
				if ((init_tp.tv_sec + 1000) < sync_tp.tv_sec) {
					time_sync = 1;
					break;
				} else if ((init_tp.tv_sec + guarantee_secs) < sync_tp.tv_sec) {
					break;
				}
				usleep(100000);
			}
			if (time_sync) {
				THINGS_LOG(THINGS_INFO, TAG, "ntpc_time sync done");
			} else {
				THINGS_LOG(THINGS_ERROR, TAG, "ntpc_time sync fail");
				clock_settime(CLOCK_REALTIME, &pre_tp);
			}
		}
	} else {
		THINGS_LOG(THINGS_INFO, TAG, "ntpc already running");
	}
}
#endif

static void *__attribute__((optimize("O0"))) t_things_wifi_join_loop(void *args)
{
#ifdef CONFIG_NETUTILS_NTPCLIENT
	sync_time_from_ntp(10);
#else
	THINGS_LOG_D(THINGS_INFO, TAG, "CONFIG_NETUTILS_NTPCLIENT is not set");
#endif

	wifi_manager_info_s wifi_info;
	wifi_manager_get_info(&wifi_info);

	things_wifi_changed_call_func(1, wifi_info.ssid, wifi_info.ip4_address);

	return NULL;
}

void things_wifi_sta_connected(wifi_manager_result_e res)
{
	if (res == WIFI_MANAGER_FAIL) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to connect to the AP");
		return;
	}

	THINGS_LOG_D(THINGS_INFO, TAG, "T%d --> %s", getpid(), __FUNCTION__);

	pthread_create_rtos(&h_thread_things_wifi_join, NULL, (pthread_func_type) t_things_wifi_join_loop, NULL, THINGS_STACK_WIFI_JOIN_THREAD);
}

void things_wifi_sta_disconnected(void)
{
	THINGS_LOG_D(THINGS_INFO, TAG, "T%d --> %s", getpid(), __FUNCTION__);
	things_wifi_changed_call_func(0, NULL, NULL);
}

void things_wifi_soft_ap_sta_joined(void)
{
	THINGS_LOG_D(THINGS_INFO, TAG, "T%d --> %s", getpid(), __FUNCTION__);

}

void things_wifi_soft_ap_sta_left(void)
{
	THINGS_LOG_D(THINGS_INFO, TAG, "T%d --> %s", getpid(), __FUNCTION__);
}

void things_wifi_scan_done(wifi_manager_scan_info_s **scan_result, int res)
{
	THINGS_LOG_D(THINGS_INFO, TAG, "T%d --> %s", getpid(), __FUNCTION__);
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
//TO DO
}

static const wifi_manager_cb_s wifi_callbacks = {
	things_wifi_sta_connected,
	things_wifi_sta_disconnected,
	things_wifi_soft_ap_sta_joined,
	things_wifi_soft_ap_sta_left,
	things_wifi_scan_done,
};

typedef int8_t INT8;

bool try_connect_home_ap(wifi_manager_ap_config_s *connect_config)
{
	if (connect_config != NULL && connect_config->ssid != NULL) {
		THINGS_LOG_V(THINGS_INFO, TAG, "Try_connect_home_ap [ssid : %s]", connect_config->ssid);
	}

	wifi_manager_result_e result = WIFI_MANAGER_SUCCESS;
	int retry_count = 0;

	for (; retry_count < 3; retry_count++) {
		result = wifi_manager_connect_ap(connect_config);

		if (result == WIFI_MANAGER_SUCCESS) {
			break;
		} else {
			THINGS_LOG_V(THINGS_ERROR, TAG, "Failed to connect WiFi [Error Code : %d, Retry count : %d]", result, retry_count);
		}
	}

	if (retry_count == 3) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to connect WiFi 3 times");
		return false;
	}

	return true;
}

bool try_turn_on_soft_ap()
{
	wifi_manager_info_s info;
	wifi_manager_get_info(&info);

	wifi_manager_remove_config();

	if (info.mode != SOFTAP_MODE) {
		wifi_manager_softap_config_s *ap_config = dm_get_softap_wifi_config();
		if (wifi_manager_set_mode(SOFTAP_MODE, ap_config) != WIFI_MANAGER_SUCCESS) {
			THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to change to SOFTAP mode");
			return false;
		}
	}
	THINGS_LOG(THINGS_INFO, TAG, "In SOFTAP mode");
	return true;
}

bool things_handle_stop_soft_ap(wifi_manager_ap_config_s *connect_config)
{
	wifi_manager_result_e result = WIFI_MANAGER_SUCCESS;
	wifi_manager_info_s info;
	wifi_manager_get_info(&info);

	if (info.mode == SOFTAP_MODE) {
		if (wifi_manager_set_mode(STA_MODE, NULL) != WIFI_MANAGER_SUCCESS) {
			THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to change to STA mode)");
			return false;
		}
		usleep(100000);
	}

	int retry_count = 0;

	for (; retry_count < 3; retry_count++) {
		result = wifi_manager_connect_ap(connect_config);

		if (result == WIFI_MANAGER_SUCCESS) {
			break;
		} else {
			THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to connect WiFi");
		}
	}

	if (retry_count == 3) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to connect WiFi 3 times");
		return false;
	}

	return true;
}

INT8 things_wifi_connection_cb(access_point_info_s *p_info, char *p_cmd_id)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "T%d --> %s", getpid(), __FUNCTION__);

	if (p_info == NULL) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Invalid params");
		return -1;
	}

	wifi_manager_ap_config_s connect_config;

	strncpy(connect_config.ssid, p_info->e_ssid, sizeof(connect_config.ssid));
	connect_config.ssid_length = strlen(connect_config.ssid);
	strncpy(connect_config.passphrase, p_info->security_key, sizeof(connect_config.passphrase));

	connect_config.passphrase_length = strlen(connect_config.passphrase);

	THINGS_LOG_V(THINGS_INFO, TAG, "[%s] ssid : %s", __FUNCTION__, connect_config.ssid);

	// set auth type
	if (strncmp(p_info->auth_type, "WEP", strlen("WEP")) == 0) {
		connect_config.ap_auth_type = WIFI_MANAGER_AUTH_WEP_SHARED;
	} else if (strncmp(p_info->auth_type, "WPA-PSK", strlen("WPA-PSK")) == 0) {
		connect_config.ap_auth_type = WIFI_MANAGER_AUTH_WPA_PSK;
	} else if (strncmp(p_info->auth_type, "WPA2-PSK", strlen("WPA2-PSK")) == 0) {
		connect_config.ap_auth_type = WIFI_MANAGER_AUTH_WPA2_PSK;
	}
	// set encryption crypto type
	if (strncmp(p_info->enc_type, "WEP-64", strlen("WEP-64")) == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_WEP_64;
	} else if (strncmp(p_info->enc_type, "WEP-128", strlen("WEP-128")) == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_WEP_128;
	} else if (strncmp(p_info->enc_type, "TKIP", strlen("TKIP")) == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_TKIP;
	} else if (strncmp(p_info->enc_type, "AES", strlen("AES")) == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_AES;
	} else if (strncmp(p_info->enc_type, "TKIP_AES", strlen("TKIP_AES")) == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_TKIP_AND_AES;
	}

	wifi_manager_result_e res = wifi_manager_save_config(&connect_config);

	if (res != WIFI_MANAGER_SUCCESS) {
		THINGS_LOG_V(THINGS_ERROR, TAG, "Failed to save AP configuration : [%d]", res);
		return -1;
	} else {
		THINGS_LOG_V(THINGS_INFO, TAG, "Success to save AP configuration");
	}

	if (!things_handle_stop_soft_ap(&connect_config)) {
		return -1;
	}

	return 1;
}

static void otm_event_cb(const char *addr, uint16_t port, const char *uuid, int event)
{
	switch (event) {
	case OIC_OTM_READY:
		THINGS_LOG_D(THINGS_DEBUG, TAG, "OIC_OTM_READY");
		break;
	case OIC_OTM_STARTED:
		THINGS_LOG_D(THINGS_DEBUG, TAG, "OIC_OTM_STARTED");
		THINGS_LOG_D(THINGS_DEBUG, TAG, "[%s:%d][%s]", addr, port, uuid);
		break;
	case OIC_OTM_DONE:
		THINGS_LOG_D(THINGS_DEBUG, TAG, "OIC_OTM_DONE");
		THINGS_LOG_D(THINGS_DEBUG, TAG, "[%s:%d][%s]", addr, port, uuid);
		break;
	case OIC_OTM_ERROR:
		THINGS_LOG_D(THINGS_ERROR, TAG, "OIC_OTM_ERROR");
		break;
	default:
		THINGS_LOG_D(THINGS_ERROR, TAG, "Unknown");
		break;
	}
	if (g_otm_event_handler) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Call the registered funcation...");
		g_otm_event_handler(addr, port, uuid, event);
	}
}

static bool things_has_abs_device_def_path(const char *json_path)
{
	if (json_path != NULL) {
		if (json_path[0] == '/') {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}
static char *things_make_abs_device_def_path(const char *json_path)
{
	char *abs_json_path = NULL;
	int len_of_path = -1;
	if (!things_has_abs_device_def_path(json_path)) {
		len_of_path = strlen(DEVICE_DEF_FILE_ROOT) + strlen(json_path) + 1;
		abs_json_path = (char *)things_malloc(len_of_path * sizeof(char));
		strncpy(abs_json_path, DEVICE_DEF_FILE_ROOT, len_of_path);
		strncat(abs_json_path, json_path, len_of_path);
	} else {
		len_of_path = strlen(json_path) + 1;
		abs_json_path = (char *)things_malloc(len_of_path * sizeof(char));
		strncpy(abs_json_path, json_path, len_of_path);
	}
	abs_json_path[len_of_path] = NULL;
	return abs_json_path;
}
int things_initialize_stack(const char *json_path, bool *easysetup_completed)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Make a file path to match device type.");
	char *abs_json_path = NULL;
	if (json_path == NULL) {
		THINGS_LOG_D_DEBUG(THINGS_DEBUG, TAG, "json_path could not be null.");
		return 0;
	} else {
		abs_json_path = things_make_abs_device_def_path(json_path);
		THINGS_LOG_D_DEBUG(THINGS_DEBUG, TAG, "Origin path(%s), converted path(%s).", json_path, abs_json_path);
	}
#if CONFIG_ST_THINGS_FOTA
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Create a file regarding the device's resource");
	int ret = 0;
	int size_d = sizeof(deviceDef);
	FILE *fp;
	char *json_str = NULL;
	cJSON *root;
	cJSON *h_root;
	fp = fopen(abs_json_path, "r");
	if (fp) {
		THINGS_LOG_D(THINGS_INFO, TAG, "File is exist...");
		fclose(fp);
		json_str = get_json_string_from_file(abs_json_path);
		h_root = cJSON_Parse(deviceDef);
		if (json_str != NULL && strlen(json_str) > 0) {
			root = cJSON_Parse((const char *)json_str);
			if (root == NULL) {
				THINGS_LOG_D(THINGS_DEBUG, TAG, "Failed to parse a device resource file");
			}
		}
		if (!cJSON_Compare(h_root, root, 1)) {
			THINGS_LOG_D(THINGS_DEBUG, TAG, "Modify the device resource file");
			fp = fopen(abs_json_path, "w+");
			if (!fp) {
				THINGS_LOG_D(THINGS_ERROR, TAG, "fopen() failed");
				return -1;
			}
			ret = fwrite(deviceDef, size_d, 1, fp);
			if (ret < 0) {
				THINGS_LOG_D(THINGS_ERROR, TAG, "fwrite() failed");
				return -1;
			}
			fclose(fp);
			THINGS_LOG_D(THINGS_INFO, TAG, "Success to modify the device resource file");
		} else {
			THINGS_LOG_D(THINGS_INFO, TAG, "A resource file alreasy exists");
		}
	} else {
		fclose(fp);
		THINGS_LOG_D(THINGS_INFO, TAG, "File did not exist...Create file");
		fp = fopen(abs_json_path, "w+");
		if (!fp) {
			THINGS_LOG_D(THINGS_ERROR, TAG, "fopen() failed");
			return -1;
		}
		ret = fwrite(deviceDef, size_d, 1, fp);
		if (ret < 0) {
			THINGS_LOG_D(THINGS_ERROR, TAG, "fwrite() failed");
			return -1;
		}
		fclose(fp);
		THINGS_LOG_D(THINGS_INFO, TAG, "Success to create the device resource file");
	}
#endif
	if (is_things_module_inited) {
		THINGS_LOG(THINGS_ERROR, TAG, "Stack already initialized");
		return 0;
	}

	things_log_init();
	things_log_set_version(ST_THINGS_STACK_VERSION);

	if (!dm_init_module(abs_json_path)) {
		THINGS_LOG(THINGS_ERROR, TAG, "dm_init_module() failed");
		return 0;
	}
	is_things_module_inited = 1;

	*easysetup_completed = dm_is_there_things_cloud();

	things_register_set_ap_connection_func(things_wifi_connection_cb);

	if (wifi_manager_init(&wifi_callbacks) != WIFI_MANAGER_SUCCESS) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize WiFi manager");
		return 0;
	}
	
#ifdef CONFIG_ST_THINGS_FOTA
	if (fmwup_initialize() < 0) {
		THINGS_LOG(THINGS_ERROR, TAG, "fmwup_initizlize() failed");
	}
#endif
	things_free(abs_json_path);

	return 1;
}

int things_send_push_message(const char *push_uri, void *payload)
{
	if (payload == NULL) {
		THINGS_LOG(THINGS_ERROR, TAG, "payload is NULL");
		return 0;
	}
	if (!push_notification_to_cloud(push_uri, (OCRepPayload *) payload)) {
		THINGS_LOG_D(THINGS_ERROR, TAG, "push_notification_to_cloud() failed");
		return 0;
	}
	return 1;
}

int things_deinitialize_stack()
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	dm_termiate_module();

	is_things_module_inited = 0;

	return 1;
}

int things_start_stack()
{
	THINGS_LOG_D(THINGS_INFO, TAG, "ST_Things SDK version : %s", ST_THINGS_STACK_VERSION);

	if (dm_get_easysetup_connectivity_type() == es_conn_type_softap) {
		if (dm_is_there_things_cloud() == false) {
			if (!try_turn_on_soft_ap()) {
				return 0;
			}
		} else {				// Alread Owned.
			wifi_manager_ap_config_s *ap_config = dm_get_homeap_wifi_config();
			if (!try_connect_home_ap(ap_config)) {
				return 0;
			}
		}
	} else if (dm_get_easysetup_connectivity_type() == es_conn_type_ble) {
		//TO DO
	} else {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "es_conn_type_Unknown");
		return 0;
	}

	// 2. Enable Security Features
#ifdef __SECURED__
	int auty_type = AUTH_UNKNOW;
	switch (dm_get_ownership_transfer_method()) {
	case 1:					// PIN
		auty_type = AUTH_RANDOM_PIN;
		break;
	case 2:					// USER_CONFIRM
		auty_type = AUTH_CERTIFICATE_CONFIRM;
		break;
	case 3:					// BOTH
		auty_type = AUTH_RANDOM_PIN + AUTH_CERTIFICATE_CONFIRM;
		break;
	}

	if (0 != sm_init_things_security(auty_type, dm_get_svrdb_file_path())) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize OICSecurity features");
		return 0;
	}
	sm_set_otm_event_handler(otm_event_cb);
#endif
	g_req_handler = get_handler_instance();
	if (NULL == g_req_handler) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize Request Handler");
		return 0;
	}
	// Will be refactored to received callbacks for GET/PUT/POST/DELETE/OBSERVE ..
	g_req_handler->init_module();

	// 4. Initiate Server Builder
	g_server_builder = get_builder_instance();
	if (NULL == g_server_builder) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize Resource Server");
		return 0;
	}

	g_server_builder->init_module(g_server_builder, g_req_handler->entity_handler);

#ifdef __SECURED__
	//5. Generate device UUID
	if (0 != sm_generate_device_id()) {
		THINGS_LOG(THINGS_WARNING, TAG, "Failed to generate device_id");
		return 0;
	}
#endif

	// 6. Register Device-ID & Resources
	if (dm_register_device_id() == false) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to register Device ID");
		return 0;
	}

	if (!dm_register_resource(g_server_builder)) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to register Resource");
		return 0;
	}
	// 7. Initiate Easy-Setup & Login to Cloud
	if (ESM_OK != esm_init_easysetup(0, g_server_builder)) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize Easy-Setup Module");
		return 0;
	}
	// Register Callback
	esm_register_cloud_cb(cloud_data_cb_esm);

	if (es_cloud_init(g_server_builder) == NULL) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize Cloud");
		return 0;
	}

	is_things_module_inited = 1;

	g_quit_flag = 0;

	THINGS_LOG_D(THINGS_INFO, TAG, "Stack Initialization Success");
	return 1;
}

int things_reset(void *remote_owner, things_es_enrollee_reset_e resetType)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	if (resetType == RST_AUTO_RESET) {
		try_turn_on_soft_ap();
	}

	int res = -1;

	if (b_thread_things_reset) {
		return 0;
	}

	if (resetType >= RST_ENUM_EOF) {
		THINGS_LOG_D(THINGS_INFO, TAG, "Not support things_es_enrollee_reset_e value(%d). So, Set value with RST_NEED_CONFIRM", resetType);
		resetType = RST_NEED_CONFIRM;
	}

	if (g_confirm_reset_start == NULL) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Reset-Start confirmation Function is not registered. It's mandatory");
		return res;
	}

	pthread_mutex_lock(&m_thread_oic_reset);
	if (b_thread_things_reset == false) {
		b_thread_things_reset = true;
		reset_args_s *args = (reset_args_s *) things_malloc(sizeof(reset_args_s));
		if (args == NULL) {
			THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to allocate reset_args_s memory");
			b_thread_things_reset = false;
			res = -1;
			goto GOTO_OUT;
		}
		args->remote_owner = (things_resource_s *) remote_owner;
		args->resetType = resetType;

		b_reset_continue_flag = true;

		if (pthread_create_rtos(&h_thread_things_reset, NULL, (pthread_func_type) t_things_reset_loop, args, THINGS_STACK_RESETLOOP_THREAD) != 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Failed to create thread");
			h_thread_things_reset = 0;
			things_free(args);
			b_thread_things_reset = false;
			res = -1;
			goto GOTO_OUT;
		}
		pthread_detach(h_thread_things_reset);
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Reset Thread handler = 0x%X", h_thread_things_reset);
		res = 1;
	} else {
		THINGS_LOG(THINGS_INFO, TAG, "Already run Reset-Process.");
		res = 0;
	}

GOTO_OUT:
	pthread_mutex_unlock(&m_thread_oic_reset);

	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

	return res;
}

int things_stop(void)
{
	pthread_mutex_lock(&g_things_stop_mutex);

	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
	g_quit_flag = 1;
	is_things_module_inited = 0;

	pthread_mutex_lock(&m_thread_oic_reset);
	if (b_thread_things_reset == true) {
		b_reset_continue_flag = false;

		pthread_join(h_thread_things_reset, NULL);
		pthread_detach(h_thread_things_reset);
		h_thread_things_reset = 0;
		b_thread_things_reset = false;
	}

	pthread_mutex_unlock(&m_thread_oic_reset);

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Terminate Cloud Session Managing");
	es_cloud_terminate();

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Terminate EasySetup");
	esm_terminate_easysetup();

#ifdef __SECURED__
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Terminate OIC Security Manager");
#endif

	if (g_server_builder != NULL) {
		release_builder_instance(g_server_builder);
		g_server_builder = NULL;
	}

	if (g_req_handler != NULL) {
		g_req_handler->deinit_module();
		release_handler_instance(g_req_handler);
		g_req_handler = NULL;
	}

	dm_termiate_module();

	// [Jay] Need to add memory release for the Queue..
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

	pthread_mutex_unlock(&g_things_stop_mutex);
	return 1;
}

int things_register_confirm_reset_start_func(things_reset_confirm_func_type func)
{
	if (func != NULL) {
		g_confirm_reset_start = func;
		return 1;
	} else {
		return 0;
	}
}

int things_notify_observers(const char *uri)
{
	if (g_req_handler == NULL) {
		THINGS_LOG(THINGS_ERROR, TAG, "g_req_handler is NULL");
		return 0;
	}
	return g_req_handler->notify_things_observers(uri);
}

int things_register_update_dev_prov_data_func(things_update_dev_prov_data_func_type func)
{
	if (func != NULL) {
		esm_register_update_dev_prov_data_func(func);
		return 1;
	} else {
		return 0;
	}
}

int things_register_set_ap_connection_func(things_set_ap_connection_func_type func)
{
	// Stack (Easy-setup) can request device to access specific AP
	// with this registered callback
	if (func != NULL) {
		things_register_set_ap_conn_func(func);
		return 1;
	} else {
		return 0;
	}
}

int things_wifi_changed_call_func(int state, char *ap_name, char *ip_addr)
{
	int ret = 0;

	if (is_things_module_inited == 0) {
		return ret;
	}

	pthread_mutex_lock(&g_app_cb_mutex);
#ifdef __ST_THINGS_RTOS__
	pthread_mutex_lock(&m_thread_oic_reset);
	ret = things_wifi_state_changed_cb(state, ap_name, ip_addr);
	pthread_mutex_unlock(&m_thread_oic_reset);
#else
	ret = things_wifi_state_changed_cb(state, ap_name, ip_addr);
#endif
	pthread_mutex_unlock(&g_app_cb_mutex);

	return ret;
}

int things_register_stop_soft_ap_func(things_stop_soft_ap_func_type func)
{
	if (NULL != func) {
		return register_stop_softap_cb(func);
	} else {
		return 0;
	}
}

int things_register_pin_generated_func(things_pin_generated_func_type func)
{
	if (NULL != func) {
#ifdef __SECURED__
		return esm_register_pin_generated_cb(func);
#else
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
		return 0;
#endif
	} else {
		return 0;
	}
}

int things_register_pin_display_close_func(things_pin_display_close_func_type func)
{
	if (NULL != func) {
#ifdef __SECURED__
		return esm_register_pin_close_cb(func);
#else
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
		return 0;
#endif
	} else {
		return 0;
	}
}

int things_register_user_confirm_func(things_get_user_confirm_func_type func)
{
	if (NULL != func) {
#ifdef __SECURED__
		return esm_register_user_confirm_cb(func);
#else
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
		return 0;
#endif
	} else {
		return 0;
	}
}

int things_register_handle_request_func(things_handle_request_func_type get_func, things_handle_request_func_type set_func)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
	if ((NULL == get_func) || (NULL == set_func)) {
		THINGS_LOG(THINGS_ERROR, TAG, "Callback is NULL");
		return 0;
	}
	register_handle_request_func(get_func, set_func);
	return 1;
}

//////////////////////////////////////////////////////////////////////////
// Functions Definition for Security feature
//////////////////////////////////////////////////////////////////////////
int things_register_otm_event_handler(things_sec_otm_state_func_type otmEventCallback)
{
	THINGS_LOG(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
#ifdef __SECURED__
	if (otmEventCallback == NULL) {
		THINGS_LOG(THINGS_ERROR, TAG, "otmEventCallback is NULL");
		return 0;
	}
	g_otm_event_handler = otmEventCallback;
#else
	THINGS_LOG(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
	return 0;
#endif
	return 1;
}

int things_save_acl(const char *uuid, const char *resourceUri, const char *resourceType, const char *interfaceName, uint16_t permission)
{
	int ret = -1;
	THINGS_LOG(THINGS_DEBUG, TAG, "In things_save_acl");
#ifdef __SECURED__
	ret = sm_save_acl(uuid, resourceUri, resourceType, interfaceName, permission);
#else
	THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
#endif
	THINGS_LOG(THINGS_DEBUG, TAG, "Out things_save_acl");
	return ret;
}

int things_set_mot_status(bool enable)
{
	int ret = -1;
	THINGS_LOG(THINGS_DEBUG, TAG, "In things_set_mot_status");
#ifdef __SECURED__
	ret = sm_set_mot_status(enable);
#else
	THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
#endif
	THINGS_LOG(THINGS_DEBUG, TAG, "Out things_set_mot_status");
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// Functions Definition for Reset Handling.
//////////////////////////////////////////////////////////////////////////
static void things_set_reset_mask(rst_state_e value)
{
	if (value != RST_COMPLETE) {
		m_reset_bit_mask |= value;
	} else {
		m_reset_bit_mask = value;
	}
}

bool things_get_reset_mask(rst_state_e value)
{
	return (int)(m_reset_bit_mask & value) != 0 ? true : false;
}

int things_return_user_opinion_for_reset(int b_reset_start)	// b_reset_start : User opinion.
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	int result = 0;

	if (b_reset_start == true && b_reset_continue_flag == true) {	// User allow Reset-Starting.
		things_set_reset_mask(RST_USER_CONFIRM_COMPLETED);
		result = 1;
	} else {					// User not allow Reset.
		b_reset_continue_flag = false;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "User Opinion : %d", result);

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return result;
}

static void *__attribute__((optimize("O0"))) t_things_reset_loop(reset_args_s *args)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter. Handler=0x%X", h_thread_things_reset);

	int result = 0;
	things_reset_result_func_type things_cb_func_for_reset_result = NULL;

	if (args == NULL) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "args is NULL. It's mandatory.");
		goto GOTO_OUT;
	}

	if (g_confirm_reset_start == NULL) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Reset-Start confirmation Function is not registered. It's mandatory.");
		goto GOTO_OUT;
	}

	things_set_reset_mask(RST_COMPLETE);
	THINGS_LOG(THINGS_DEBUG, TAG, "Start Reset Processing.");

	// 0. Trigger User-Confirm GUI and Wait for getting User-Opinion with regard to Reset.
	THINGS_LOG(THINGS_DEBUG, TAG, "Trigger Reset-GUI.");
	if (g_confirm_reset_start(&things_cb_func_for_reset_result, args->resetType) == 0) {
		THINGS_LOG(THINGS_DEBUG, TAG, "User don't want to reset. So reset canceled.");
		goto GOTO_OUT;
	}

	THINGS_LOG(THINGS_DEBUG, TAG, "Waiting User-Opinion for Reset-Start.");
	while (b_reset_continue_flag == true && things_get_reset_mask(RST_USER_CONFIRM_COMPLETED) == false) {	// It's Blocking until receive user-opinion.
		usleep(300000);			// 0.3 seconds wait.
	}

	THINGS_LOG(THINGS_DEBUG, TAG, "Break-Stop Check.");
	if (b_reset_continue_flag == false) {
		THINGS_LOG(THINGS_DEBUG, TAG, "Called Reset break-STOP.");
		goto GOTO_OUT;
	}
	// 1. Easy Setup : Provisioning Disable.
	// 2. Controlling Module : Disable pushing request to Queue.
	THINGS_LOG(THINGS_DEBUG, TAG, "Disable Modules.(EasySetup/Controlling Module)");
	things_set_reset_mask(RST_EASY_SETUP_DISABLE | RST_CONTROL_MODULE_DISABLE);

	THINGS_LOG(THINGS_DEBUG, TAG, "Break-Stop Check.");
	if (b_reset_continue_flag == false) {
		THINGS_LOG(THINGS_DEBUG, TAG, "Called Reset break-STOP.");
		goto GOTO_OUT;
	}
	// 3. Notification Push Disable.
	THINGS_LOG(THINGS_DEBUG, TAG, "Disable Notification Module.");
	things_set_reset_mask(RST_NOTI_MODULE_DISABLE);

	OCClearObserverlist();		// delete All Observer. (for remote Client)

	// 4. Cloud Manager : Terminate
	THINGS_LOG(THINGS_DEBUG, TAG, "Terminate Cloud Module.");
	es_cloud_terminate();

	// 5. Easy Setup : Terminate
	THINGS_LOG(THINGS_DEBUG, TAG, "Terminate Easy-Setup Module.");
	if (esm_terminate_easysetup() != ESM_MODULE_TERMINATED) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Easy-Setup Module termination failed");
		goto GOTO_OUT;
	}
	// 6. Delete Cloud data in "provisioning.json"
	THINGS_LOG(THINGS_DEBUG, TAG, "Delete Cloud-Data in provisioning.json file.");
	if (dm_del_things_cloud_data() == 0) {
		THINGS_LOG(THINGS_DEBUG, TAG, "It's failed to delete information about Cloud-Data in \"ocf_infoX.json\" file.");
		goto GOTO_OUT;
	}
	THINGS_LOG(THINGS_INFO, TAG, "Reset done: cloud provisioning data");

	OCClearCallBackList();		// delete All Client Call-Back List. (for SET Self-Request)

	// 7. Security Reset.
#ifdef __SECURED__
	if (0 != sm_reset_svrdb()) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize OICSecurity features.");
		goto GOTO_OUT;
	}
	THINGS_LOG(THINGS_INFO, TAG, "Reset done: security resources");

	if (0 != sm_generate_device_id()) {
		THINGS_LOG(THINGS_WARNING, TAG, "Failed to generate device_id.");
		goto GOTO_OUT;
	}
#else
	THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "NOT SECURED MODE!!!!! ");
#endif

	// 8. Easy Setup : Initialization.
	THINGS_LOG(THINGS_DEBUG, TAG, "Initial Easy-Setup Module.");
	if (ESM_OK != esm_init_easysetup(0, g_server_builder)) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Easy-Setup Module initialization failed");
		goto GOTO_OUT;
	}
	esm_register_cloud_cb(cloud_data_cb_esm);

	// 9. Cloud Manager : Initialization.
	THINGS_LOG(THINGS_DEBUG, TAG, "Initial Cloud Module.");
	if (es_cloud_init(g_server_builder) == NULL) {
		THINGS_LOG(THINGS_DEBUG, TAG, "It's failed Cloud_Module initialization.");
		goto GOTO_OUT;
	}

	THINGS_LOG(THINGS_DEBUG, TAG, "Reset Success.");
	result = 1;

GOTO_OUT:
	// 10. All Module Enable.
	things_set_reset_mask(RST_COMPLETE);

	/*SVACE warning fix */
	if (args) {
		// 11. If reset-processing is triggered by remote-owner, then Notify result of Reset.
		THINGS_LOG(THINGS_DEBUG, TAG, "Notify result of reset to remote-client.(mobile)");
		notify_result_of_reset(args->remote_owner, result);
	}
	// 12. If there is Reset Result Call-Back Function, then Notify result of Reset to st_things-Application.
	if (things_cb_func_for_reset_result) {	// Notify result of reset to st_things.
		THINGS_LOG(THINGS_DEBUG, TAG, "Notify result of reset to st_things.");
		things_cb_func_for_reset_result(result);
	}

	if (args) {
		things_free(args);
		args = NULL;
	}
	pthread_mutex_lock(&m_thread_oic_reset);
	h_thread_things_reset = 0;
	b_thread_things_reset = false;
	pthread_mutex_unlock(&m_thread_oic_reset);
	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return NULL;
}

OCEntityHandlerResult things_abort(pthread_t *h_thread_abort, things_es_enrollee_abort_e level)
{
	s_abort_s *ARGs = NULL;
	OCEntityHandlerResult eh_result = OC_EH_ERROR;

	if (h_thread_abort == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "h_thread_abort is NULL.");
		return eh_result;
	}

	if ((*h_thread_abort) == 0) {
		if ((ARGs = (s_abort_s *) things_malloc(sizeof(s_abort_s))) == NULL) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Memory Allocation is failed.(for abort Thread)");
			return OC_EH_ERROR;
		}

		ARGs->h_thread = h_thread_abort;
		ARGs->level = level;

		eh_result = OC_EH_OK;

		if (pthread_create_rtos(h_thread_abort, NULL, (pthread_func_type) t_things_abort_loop, ARGs, THINGS_STACK_ABORT_THREAD) != 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Create thread is failed.(for abort Thread)");
			*h_thread_abort = 0;
			things_free(ARGs);
			pthread_detach(h_thread_abort);
			eh_result = OC_EH_ERROR;
		}
	} else {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Already called Thread. So, OC_EH_NOT_ACCEPTABLE");
		eh_result = OC_EH_NOT_ACCEPTABLE;
	}

	return eh_result;
}

static void *__attribute__((optimize("O0"))) t_things_abort_loop(s_abort_s *contents)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	if (contents == NULL) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "contents is NULL.");
		goto GOTO_OUT;
	}

	if (g_abort_easysetup != NULL) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Registered Abort Function. So, Call Function st_things API.");
		g_abort_easysetup(contents->level);
	}

GOTO_OUT:
	if (contents) {
		*(contents->h_thread) = 0;
		things_free(contents);
		contents = NULL;
	}

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return NULL;
}
