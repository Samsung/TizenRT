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
#include <sys/types.h>
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
#include <errno.h>

#include "things_api.h"
#include "things_types.h"
#include "things_def.h"
#include "things_iotivity_lock.h"
#include "things_resource.h"

#include "framework/things_common.h"
#include "framework/things_server_builder.h"
#include "framework/things_security_manager.h"
#include "framework/things_req_handler.h"
#include "framework/things_data_manager.h"

#include "logging/things_logger.h"
#include "utils/things_malloc.h"
#include "utils/things_string.h"
#include "utils/things_network.h"
#include "utils/things_rtos_util.h"
#include "utils/things_network.h"

#include "easy-setup/easysetup_manager.h"

#include "cloud/cloud_manager.h"
#include "cloud/cloud_evtpend.h"

#ifdef CONFIG_ST_THINGS_FOTA
#include "fota/fmwup_api.h"
#include "deviceDef.h"
#define DEVICE_DEF_FILE_ROOT PATH_MNT
#else
#define DEVICE_DEF_FILE_ROOT PATH_ROM
#endif

#define TAG "[things_stack]"
#define SCAN_AP_INTERVAL 60
typedef void *(*pthread_func_type)(void *);
volatile static int is_things_module_initialized = 0;

typedef struct reset_args_s {
	things_resource_s *remote_owner;
	things_es_enrollee_reset_e resetType;
} reset_args_s;

static void *auto_scanning_loop(void);
static pthread_t h_thread_things_scan_ap;

things_server_builder_s *g_server_builder = NULL;
things_request_handler_s *g_req_handler = NULL;

things_reset_confirm_func_type g_confirm_reset_start = NULL;
things_sec_otm_state_func_type g_otm_event_handler = NULL;

static pthread_mutex_t g_things_stop_mutex = PTHREAD_MUTEX_INITIALIZER;

bool b_reset_continue_flag = false;
static bool b_thread_things_reset = false;
static pthread_t h_thread_things_reset = PTHREAD_ONCE_INIT;
static pthread_mutex_t m_thread_oic_reset = PTHREAD_MUTEX_INITIALIZER;
static volatile rst_state_e m_reset_bit_mask = RST_COMPLETE;

static void *t_things_reset_loop(reset_args_s *args);

int things_register_easysetup_state_func(things_get_easysetup_state_func_type func)
{
	int res = 0;

	if (func != NULL) {
		es_register_notify_easysetup_state((void *)func);
		res = 1;
	}

	return res;
}

static void otm_event_cb(const char *addr, uint16_t port, const char *uuid, int event)
{
	switch (event) {
	case OIC_OTM_READY:
		THINGS_LOG_D(TAG, "OIC_OTM_READY");
		break;
	case OIC_OTM_STARTED:
		THINGS_LOG_D(TAG, "OIC_OTM_STARTED");
		THINGS_LOG_D(TAG, "[%s:%d][%s]", addr, port, uuid);
		break;
	case OIC_OTM_DONE:
		THINGS_LOG_D(TAG, "OIC_OTM_DONE");
		THINGS_LOG_D(TAG, "[%s:%d][%s]", addr, port, uuid);
		break;
	case OIC_OTM_ERROR:
		THINGS_LOG_E(TAG, "OIC_OTM_ERROR");
		break;
	default:
		THINGS_LOG_E(TAG, "Unknown");
		break;
	}
	if (g_otm_event_handler) {
		THINGS_LOG_D(TAG, "Call the registered funcation...");
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
	abs_json_path[len_of_path - 1] = '\0';
	return abs_json_path;
}

int things_initialize_stack(const char *json_path, bool *easysetup_completed)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	THINGS_LOG_D(TAG, "Make a file path to match device type.");
	char *abs_json_path = NULL;
	if (json_path == NULL) {
		THINGS_LOG_V(TAG, "json_path could not be null.");
		return 0;
	} else {
		abs_json_path = things_make_abs_device_def_path(json_path);
		THINGS_LOG_V(TAG, "Origin path(%s), converted path(%s).", json_path, abs_json_path);
	}
#ifdef CONFIG_ST_THINGS_FOTA
	THINGS_LOG_D(TAG, "Create a file regarding the device's resource");
	int ret = 0;
	int size_d = sizeof(deviceDef);
	FILE *fp = fopen(abs_json_path, "r");
	if (fp != NULL) {
		THINGS_LOG_V(TAG, "File is exist...");
		fclose(fp);
		cJSON *root = NULL;
		char *json_str = get_json_string_from_file(abs_json_path);
		cJSON *h_root = cJSON_Parse(deviceDef);
		if (json_str != NULL && strlen(json_str) > 0) {
			root = cJSON_Parse((const char *)json_str);
			if (root == NULL) {
				THINGS_LOG_D(TAG, "Failed to parse a device resource file");
			}
		}
		if (!cJSON_Compare(h_root, root, 1)) {
			THINGS_LOG_D(TAG, "Modify the device resource file");
			fp = fopen(abs_json_path, "w+");
			if (!fp) {
				THINGS_LOG_E(TAG, "fopen() failed");
				things_free(abs_json_path);
				return -1;
			}
			ret = fwrite(deviceDef, size_d, 1, fp);
			if (ret < 0) {
				fclose(fp);
				THINGS_LOG_E(TAG, "fwrite() failed");
				things_free(abs_json_path);
				return -1;
			}
			fclose(fp);
			THINGS_LOG_V(TAG, "Success to modify the device resource file");
		} else {
			THINGS_LOG_V(TAG, "A resource file alreasy exists");
		}
	} else {
		THINGS_LOG_V(TAG, "File did not exist...Create file");
		fp = fopen(abs_json_path, "w+");
		if (!fp) {
			THINGS_LOG_E(TAG, "fopen() failed");
			things_free(abs_json_path);
			return -1;
		}
		ret = fwrite(deviceDef, size_d, 1, fp);
		if (ret < 0) {
			fclose(fp);
			THINGS_LOG_E(TAG, "fwrite() failed");
			things_free(abs_json_path);
			return -1;
		}
		fclose(fp);
		THINGS_LOG_V(TAG, "Success to create the device resource file");
	}
#endif
	if (is_things_module_initialized) {
		THINGS_LOG_E(TAG, "Stack already initialized");
		things_free(abs_json_path);
		return 0;
	}

	init_iotivity_api_lock();

	if (!things_network_initialize()) {
		THINGS_LOG_E(TAG, "ERROR things_network initialize");
		things_free(abs_json_path);
		return 0;
	}

	things_log_init();
	things_log_set_version(ST_THINGS_STACK_VERSION);

	if (!dm_init_module(abs_json_path)) {
		THINGS_LOG_E(TAG, "dm_init_module() failed");
		things_free(abs_json_path);
		things_log_shutdown();
		return 0;
	}
	is_things_module_initialized = 1;

	int es_state = esm_read_easysetup_state();
	*easysetup_completed = ((es_state == ES_COMPLETE) ? true : false);

	if (es_state != ES_COMPLETE) {
		esm_save_easysetup_state(ES_NOT_COMPLETE);
		THINGS_LOG_D(TAG, "delete svrdb");
#ifdef CONFIG_SVR_DB_SECURESTORAGE
		secure_remove(NULL);
#else
		unlink(dm_get_svrdb_file_path());
#endif
	}

#ifdef CONFIG_ST_THINGS_FOTA
	if (fmwup_initialize() < 0) {
		THINGS_LOG_E(TAG, "fmwup_initizlize() failed");
	}
#endif
	things_free(abs_json_path);

	return 1;
}

int things_deinitialize_stack(void)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	dm_termiate_module();
	things_log_shutdown();
	deinit_iotivity_api_lock();

	is_things_module_initialized = 0;

	return 1;
}

int things_start_stack(void)
{
	THINGS_LOG_V(TAG, "ST_Things SDK version : %s", ST_THINGS_STACK_VERSION);

	if (!is_things_module_initialized) {
		THINGS_LOG_E(TAG, "Initialize failed. You must initialize it first.");
		return 0;
	}
	// Enable Security Features
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
		THINGS_LOG_E(TAG, "Failed to initialize OICSecurity features");
		return 0;
	}
	sm_set_otm_event_handler(otm_event_cb);
#endif
	g_req_handler = get_handler_instance();
	if (NULL == g_req_handler) {
		THINGS_LOG_E(TAG, "Failed to initialize Request Handler");
		return 0;
	}
	// Will be refactored to received callbacks for GET/PUT/POST/DELETE/OBSERVE ..
	g_req_handler->init_module();

	// Initiate Server Builder
	g_server_builder = get_builder_instance();
	if (NULL == g_server_builder) {
		THINGS_LOG_E(TAG, "Failed to initialize Resource Server");
		return 0;
	}

	g_server_builder->init_module(g_server_builder, g_req_handler->entity_handler);

#ifdef __SECURED__
	// Generate device UUID
	if (0 != sm_generate_device_id()) {
		THINGS_LOG_E(TAG, "Failed to generate device_id");
		return 0;
	}
#endif

	// Register Device-ID & Resources
	if (dm_register_device_id() == false) {
		THINGS_LOG_E(TAG, "Failed to register Device ID");
		return 0;
	}

	if (!dm_register_resource(g_server_builder)) {
		THINGS_LOG_E(TAG, "Failed to register Resource");
		return 0;
	}
	// Initiate Easy-Setup & Login to Cloud
	if (ESM_OK != esm_init_easysetup(0, g_server_builder)) {
		THINGS_LOG_E(TAG, "Failed to initialize Easy-Setup Module");
		return 0;
	}
	// Register Callback
	esm_register_cloud_cb(cloud_data_cb_esm);

	if (es_cloud_init(g_server_builder) == NULL) {
		THINGS_LOG_E(TAG, "Failed to initialize Cloud");
		return 0;
	}
	if (dm_get_easysetup_connectivity_type() == es_conn_type_softap) {
		if (dm_is_es_complete() == false) {
			if (!things_network_turn_on_soft_ap()) {
				return 0;
			}
#ifndef CONFIG_WIFI_SCAN_IN_SOFT_AP_DISABLED
			//call wifi scan ap.
			if (!things_start_scanning_ap()) {
				return 0;
			}
#endif
		} else if (!things_network_connect_home_ap()) {
			return 0;
		}
	} else if (dm_get_easysetup_connectivity_type() == es_conn_type_ble) {
		//TO DO
	} else {
		THINGS_LOG_E(TAG, "es_conn_type_Unknown");
		return 0;
	}
	THINGS_LOG_V(TAG, "Stack Start Success");
	return 1;
}

int things_start_scanning_ap(void)
{
	if (pthread_create_rtos(&h_thread_things_scan_ap, NULL, (pthread_func_type)auto_scanning_loop, NULL, THINGS_STACK_AP_SCAN_THREAD) != 0) {
		THINGS_LOG_E(TAG, "Failed to create thread");
		return 0;
	}
	return 1;
}

static void *__attribute__((optimize("O0"))) auto_scanning_loop(void)
{
	while (!things_is_connected_ap()) {
		if (!things_wifi_scan_ap()) {
			THINGS_LOG_E(TAG, "Things failed to call wifi_scan_ap");
		}
		// Wifi scan is doing in every 60sec.
		sleep(SCAN_AP_INTERVAL);
	}
	return NULL;
}

int things_reset(void *remote_owner, things_es_enrollee_reset_e resetType)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (resetType == RST_AUTO_RESET) {
		things_network_turn_on_soft_ap();
	}

	int res = -1;

	if (b_thread_things_reset) {
		return 0;
	}

	if (resetType >= RST_ENUM_EOF) {
		THINGS_LOG_V(TAG, "Not support things_es_enrollee_reset_e value(%d). So, Set value with RST_NEED_CONFIRM", resetType);
		resetType = RST_NEED_CONFIRM;
	}

	if (g_confirm_reset_start == NULL) {
		THINGS_LOG_E(TAG, "Reset-Start confirmation Function is not registered. It's mandatory");
		return res;
	}

	pthread_mutex_lock(&m_thread_oic_reset);
	if (b_thread_things_reset == false) {
		reset_args_s *args = (reset_args_s *) things_malloc(sizeof(reset_args_s));
		if (args == NULL) {
			THINGS_LOG_E(TAG, "Failed to allocate reset_args_s memory");
			res = -1;
			goto GOTO_OUT;
		}
		args->remote_owner = (things_resource_s *) remote_owner;
		args->resetType = resetType;

		b_thread_things_reset = true;
		b_reset_continue_flag = true;

		if (pthread_create_rtos(&h_thread_things_reset, NULL, (pthread_func_type) t_things_reset_loop, args, THINGS_STACK_RESETLOOP_THREAD) != 0) {
			THINGS_LOG_E(TAG, "Failed to create thread");
			h_thread_things_reset = 0;
			things_free(args);
			b_thread_things_reset = false;
			b_reset_continue_flag = false;
			res = -1;
			goto GOTO_OUT;
		}
		pthread_detach(h_thread_things_reset);
		THINGS_LOG_D(TAG, "Reset Thread handler = 0x%X", h_thread_things_reset);
		res = 1;
	} else {
		THINGS_LOG_V(TAG, "Already run Reset-Process.");
		res = 0;
	}

GOTO_OUT:
	pthread_mutex_unlock(&m_thread_oic_reset);

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return res;
}

int things_stop_stack(void)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	pthread_mutex_lock(&g_things_stop_mutex);
	pthread_mutex_lock(&m_thread_oic_reset);

	if (b_thread_things_reset == true) {
		b_reset_continue_flag = false;

		pthread_join(h_thread_things_reset, NULL);
		pthread_detach(h_thread_things_reset);
		h_thread_things_reset = 0;
		b_thread_things_reset = false;
	}

	pthread_mutex_unlock(&m_thread_oic_reset);

	THINGS_LOG_D(TAG, "Terminate Cloud Session Managing");
	es_cloud_terminate();

	THINGS_LOG_D(TAG, "Terminate EasySetup");
	esm_terminate_easysetup();

#ifdef __SECURED__
	THINGS_LOG_D(TAG, "Terminate OIC Security Manager");
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
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

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
		THINGS_LOG_E(TAG, "g_req_handler is NULL");
		return 0;
	}
	return g_req_handler->notify_things_observers(uri);
}

int things_register_pin_generated_func(things_pin_generated_func_type func)
{
	if (NULL != func) {
#ifdef __SECURED__
		return esm_register_pin_generated_cb(func);
#else
		THINGS_LOG_E(TAG, "Stack is in UNSECURED Mode");
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
		return esm_register_pin_close_cb((pin_close_func_type)func);
#else
		THINGS_LOG_E(TAG, "Stack is in UNSECURED Mode");
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
		THINGS_LOG_E(TAG, "Stack is in UNSECURED Mode");
		return 0;
#endif
	} else {
		return 0;
	}
}

int things_register_handle_request_func(things_handle_request_func_type get_func, things_handle_request_func_type set_func)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	if ((NULL == get_func) || (NULL == set_func)) {
		THINGS_LOG_E(TAG, "Callback is NULL");
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
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
#ifdef __SECURED__
	if (otmEventCallback == NULL) {
		THINGS_LOG_E(TAG, "otmEventCallback is NULL");
		return 0;
	}
	g_otm_event_handler = otmEventCallback;
#else
	THINGS_LOG_E(TAG, "Stack is in UNSECURED Mode");
	return 0;
#endif
	return 1;
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

int things_return_user_opinion_for_reset(int reset)
{
	THINGS_LOG_D(TAG, "Enter.");

	int result = 0;

	if (reset == 1 && b_reset_continue_flag == true) {	// User allow Reset-Starting.
		things_set_reset_mask(RST_USER_CONFIRM_COMPLETED);
		result = 1;
	} else {					// User not allow Reset.
		pthread_mutex_lock(&m_thread_oic_reset);
		b_reset_continue_flag = false;
		pthread_mutex_unlock(&m_thread_oic_reset);
	}

	THINGS_LOG_D(TAG, "User Opinion : %d", result);

	THINGS_LOG_D(TAG, "Exit.");
	return result;
}

static void *__attribute__((optimize("O0"))) t_things_reset_loop(reset_args_s *args)
{
	THINGS_LOG_D(TAG, "Enter. Handler=0x%X", h_thread_things_reset);

	int result = 0;
	things_reset_result_func_type things_cb_func_for_reset_result = NULL;

	if (args == NULL) {
		THINGS_LOG_E(TAG, "args is NULL. It's mandatory.");
		goto GOTO_OUT;
	}

	if (g_confirm_reset_start == NULL) {
		THINGS_LOG_E(TAG, "Reset-Start confirmation Function is not registered. It's mandatory.");
		goto GOTO_OUT;
	}

	things_set_reset_mask(RST_COMPLETE);
	THINGS_LOG_D(TAG, "Start Reset Processing.");

	// 0. Trigger User-Confirm GUI and Wait for getting User-Opinion with regard to Reset.
	THINGS_LOG_D(TAG, "Trigger Reset-GUI.");
	if (g_confirm_reset_start(&things_cb_func_for_reset_result, args->resetType) == 0) {
		THINGS_LOG_D(TAG, "User don't want to reset. So reset canceled.");
		goto GOTO_OUT;
	}

	THINGS_LOG_D(TAG, "Waiting User-Opinion for Reset-Start.");
	while (b_reset_continue_flag == true && things_get_reset_mask(RST_USER_CONFIRM_COMPLETED) == false) {	// It's Blocking until receive user-opinion.
		usleep(300000);			// 0.3 seconds wait.
	}

	THINGS_LOG_D(TAG, "Break-Stop Check.");
	if (b_reset_continue_flag == false) {
		THINGS_LOG_D(TAG, "Called Reset break-STOP.");
		goto GOTO_OUT;
	}
	// 1. Easy Setup : Provisioning Disable.
	// 2. Controlling Module : Disable pushing request to Queue.
	THINGS_LOG_D(TAG, "Disable Modules.(EasySetup/Controlling Module)");
	things_set_reset_mask(RST_EASY_SETUP_DISABLE | RST_CONTROL_MODULE_DISABLE);

	THINGS_LOG_D(TAG, "Break-Stop Check.");
	if (b_reset_continue_flag == false) {
		THINGS_LOG_D(TAG, "Called Reset break-STOP.");
		goto GOTO_OUT;
	}
	// 3. Notification Push Disable.
	THINGS_LOG_D(TAG, "Disable Notification Module.");
	things_set_reset_mask(RST_NOTI_MODULE_DISABLE);

	iotivity_api_lock();
	OCClearObserverlist();		// delete All Observer. (for remote Client)
	iotivity_api_unlock();

	// 4. Cloud Manager : Terminate
	THINGS_LOG_D(TAG, "Terminate Cloud Module.");
	es_cloud_terminate();

	// 5. Easy Setup : Terminate
	THINGS_LOG_D(TAG, "Terminate Easy-Setup Module.");
	if (esm_terminate_easysetup() != ESM_MODULE_TERMINATED) {
		THINGS_LOG_E(TAG, "Easy-Setup Module termination failed");
		goto GOTO_OUT;
	}
	// 6. Delete Cloud data in "provisioning.json"
	THINGS_LOG_D(TAG, "Delete Cloud-Data in provisioning.json file.");
	if (dm_del_things_cloud_data() == 0) {
		THINGS_LOG_D(TAG, "It's failed to delete information about Cloud-Data in \"ocf_infoX.json\" file.");
		goto GOTO_OUT;
	}
	THINGS_LOG_V(TAG, "Reset done: cloud provisioning data");

	iotivity_api_lock();
	OCClearCallBackList();		// delete All Client Call-Back List. (for SET Self-Request)
	iotivity_api_unlock();

	// 7. Security Reset.
#ifdef __SECURED__
	if (0 != sm_reset_svrdb()) {
		THINGS_LOG_E(TAG, "Failed to initialize OICSecurity features.");
		goto GOTO_OUT;
	}
	THINGS_LOG_V(TAG, "Reset done: security resources");

	if (0 != sm_generate_device_id()) {
		THINGS_LOG_E(TAG, "Failed to generate device_id.");
		goto GOTO_OUT;
	}
#else
	THINGS_LOG_E(TAG, "NOT SECURED MODE!!!!! ");
#endif

	// 8. Easy Setup : Initialization.
	THINGS_LOG_D(TAG, "Initial Easy-Setup Module.");
	if (ESM_OK != esm_init_easysetup(0, g_server_builder)) {
		THINGS_LOG_E(TAG, "Easy-Setup Module initialization failed");
		goto GOTO_OUT;
	}
	esm_register_cloud_cb(cloud_data_cb_esm);

	// 9. Cloud Manager : Initialization.
	THINGS_LOG_D(TAG, "Initial Cloud Module.");
	if (es_cloud_init(g_server_builder) == NULL) {
		THINGS_LOG_D(TAG, "It's failed Cloud_Module initialization.");
		goto GOTO_OUT;
	}

	THINGS_LOG_D(TAG, "Reset Success.");
	result = 1;
#ifndef CONFIG_WIFI_SCAN_IN_SOFT_AP_DISABLED
	// After completed reset of device doing wifi scan
	things_start_scanning_ap();
#endif

GOTO_OUT:
	// 10. All Module Enable.
	things_set_reset_mask(RST_COMPLETE);

	/*SVACE warning fix */
	if (args) {
		// 11. If reset-processing is triggered by remote-owner, then Notify result of Reset.
		THINGS_LOG_D(TAG, "Notify result of reset to remote-client.(mobile)");
		notify_result_of_reset(args->remote_owner, result);
		things_free(args);
	}
	// 12. If there is Reset Result Call-Back Function, then Notify result of Reset to st_things-Application.
	if (things_cb_func_for_reset_result) {	// Notify result of reset to st_things.
		THINGS_LOG_D(TAG, "Notify result of reset to st_things.");
		things_cb_func_for_reset_result(result);
	}

	pthread_mutex_lock(&m_thread_oic_reset);
	h_thread_things_reset = 0;
	b_thread_things_reset = false;
	b_reset_continue_flag = false;
	pthread_mutex_unlock(&m_thread_oic_reset);
	THINGS_LOG_D(TAG, "Exit.");
	return NULL;
}

int things_is_things_module_initialized(void)
{
	return is_things_module_initialized;
}
