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

#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE				// for the usleep

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/select.h>
#include <time.h>

#include "utils/things_network.h"
#include "utils/things_wait_handler.h"
#include "easysetup_manager.h"
#include "resource_handler.h"
#include "cloud/cloud_manager.h"
#include "easysetup.h"
#include "logging/things_logger.h"
#include "utils/things_malloc.h"

#include "utils/things_thread.h"
#include "utils/things_rtos_util.h"

#ifdef __SECURED__
#include "pinoxmcommon.h"
#include "oxmverifycommon.h"
#endif							// #ifdef __SECURED__

#define TAG "[ezsetup-mg]"
#define MAX_REFRESHCHECK_CNT    30	// 30 times

es_dev_conf_prov_data_s *g_dev_conf_prov_data = NULL;
es_wifi_prov_data_s *g_wifi_prov_data = NULL;
es_cloud_prov_data_s *g_cloud_prov_data = NULL;

static things_server_builder_s *g_server_builder = NULL;
static things_update_dev_prov_data_func_type g_update_dev_prov_data = NULL;
static pin_generated_func_type g_pin_generated_cb = NULL;
static pin_close_func_type g_pin_close_cb = NULL;
static user_confirm_result_func_type g_user_confirm_cb = NULL;

int esm_continue = 0;
#ifdef __ST_THINGS_RTOS__
int ci_token_expire_fds[2] = { -1, -1 };
#endif
static pthread_t gthread_id_network_status_check = 0;
static pthread_t gthread_id_cloud_refresh_check = 0;

static const int i_common_sleep_sec = CI_TOKEN_EXPIRECHECK_TIME;
static const int i_fail_sleep_sec = 60;	// 60 sec
static pthread_mutex_t g_es_mutex = PTHREAD_MUTEX_INITIALIZER;

static bool is_set_device_property = false;
static es_device_property device_property;
static const char *def_device_name = "ST_Things Device";

void wifi_prov_cb_in_app(es_wifi_prov_data_s *event_data);
void dev_conf_prov_cb_in_app(es_dev_conf_prov_data_s *event_data);
void cloud_data_prov_cb_in_app(es_cloud_prov_data_s *event_data);

typedef void (*callback_to_app)(es_cloud_prov_data_s *);
callback_to_app g_cloud_data_cb_to_app = NULL;

es_provisioning_callbacks_s g_callbacks = {
	.wifi_prov_cb = &wifi_prov_cb_in_app,
	.dev_conf_prov_cb = &dev_conf_prov_cb_in_app,
	.cloud_data_prov_cb = &cloud_data_prov_cb_in_app
};

static void *wifi_prov_set_loop(void *param);
static void *cloud_refresh_check_loop(void *param);
static void *wifi_prov_timeout_handler(timeout_s *param);

#ifdef __SECURED__
void generate_pin_cb(char *pin, size_t pin_size)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	if (NULL == pin || pin_size <= 0) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "INVALID PIN");
		return;
	}

	THINGS_LOG_V(THINGS_INFO, TAG, "PIN CODE : %s", pin);

	// TODO: print out pin number using display panel - ysy
	if (g_pin_generated_cb) {
		g_pin_generated_cb(pin, pin_size);
	} else {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "PIN GENERATED CB Not Registered~!!!");
	}
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
}

void pin_close_cb(void)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	// TODO: print out pin number using display panel - ysy
	if (g_pin_close_cb) {
		g_pin_close_cb();
	} else {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "PIN GENERATED CB Not Registered~!!!");
	}
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
}

OCStackResult get_user_confirmation(void *ctx)
{
	OCStackResult ret = OC_STACK_USER_DENIED_REQ;

	if (g_user_confirm_cb) {
		int result = g_user_confirm_cb();

		switch (result) {
		case 1: {
			THINGS_LOG_V(THINGS_INFO, TAG, "User Confirmed~!!");
			ret = OC_STACK_OK;
			break;
		}
		default: {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "User Denied~!! : [ %d ]", result);
			break;
		}
		}
	} else {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "MUTUAL AUTHENTICATION CONFIRM CB Not Registered~!!!");
	}

	THINGS_LOG_V(THINGS_INFO, TAG, "User Confirm Result : [ %d ]", ret);

	return ret;
}

#endif							//#ifdef __SECURED__

/*******************************************************************
 *
 * st_things API Function register
 *
 *******************************************************************/
void esm_register_update_dev_prov_data_func(things_update_dev_prov_data_func_type func)
{
	g_update_dev_prov_data = func;
}

int esm_set_device_property_by_app(char *name, const wifi_mode_e *mode, int ea_mode, const wifi_freq_e freq)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter.");

	int i = 0;

	if (mode == NULL || ea_mode < 1 || freq < WiFi_24G || freq >= WiFi_FREQ_EOF) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Input Arguments.(mode=0x%X, ea_mode=%d, freq=%d)", mode, ea_mode, freq);
		return 0;
	}

	if (name == NULL) {
		name = def_device_name;
	}

	if (ea_mode > NUM_WIFIMODE - 1) {
		ea_mode = NUM_WIFIMODE - 1;
	}

	things_strncpy(device_property.dev_conf_s.device_name, name, sizeof(device_property.dev_conf_s.device_name));
	device_property.WiFi.freq = freq;
	for (i = 0; i < ea_mode; i++) {
		device_property.WiFi.mode[i] = mode[i];
	}
	device_property.WiFi.mode[ea_mode] = WiFi_EOF;
	is_set_device_property = true;

	THINGS_LOG_D(THINGS_DEBUG, TAG, "device_name=%s, WiFi_frequence=%d, count of WiFi_mode=%d", device_property.dev_conf_s.device_name, device_property.WiFi.freq, ea_mode);
	for (i = 0; i <= ea_mode; i++) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "WiFi_mode[%d]=%d", i, device_property.WiFi.mode[i]);
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.(result=%d)", is_set_device_property);
	return ESM_OK;
}

esm_result_e esm_init_easysetup(int restart_flag, things_server_builder_s *server_builder)
{
	THINGS_LOG_D(THINGS_INFO, TAG, "Init EasySetup");

	wifi_mode_e wifi_mode[5] = { 0, };
	wifi_freq_e wifi_freq = dm_get_wifi_property_freq();
	int wifiIntf = dm_get_wifi_property_interface();
	int mode_count = 0;

	if (wifiIntf) {
		if (wifiIntf & 0x01) {
			wifi_mode[mode_count++] = WiFi_11A;
		}
		if (wifiIntf & 0x02) {
			wifi_mode[mode_count++] = WiFi_11B;
		}
		if (wifiIntf & 0x04) {
			wifi_mode[mode_count++] = WiFi_11G;
		}
		if (wifiIntf & 0x08) {
			wifi_mode[mode_count++] = WiFi_11N;
		}
		if (wifiIntf & 0x10) {
			wifi_mode[mode_count++] = WiFi_11AC;
		}
	}
	esm_set_device_property_by_app(NULL, wifi_mode, mode_count, wifi_freq);

	if (is_set_device_property == false) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "[Error] Please Check whether you call \"OICSetDeviceProperty\" API.(We need value of WiFi-property.)");
		return ESM_ERROR;
	}

	if (restart_flag == 1 || g_dev_conf_prov_data || g_wifi_prov_data || g_cloud_prov_data) {
		THINGS_LOG_D(THINGS_INFO, TAG, "Restart EasySetup");
		esm_continue = 0;
		if (gthread_id_cloud_refresh_check) {
			pthread_cancel(gthread_id_cloud_refresh_check);
			pthread_join(gthread_id_cloud_refresh_check, NULL);
			gthread_id_cloud_refresh_check = 0;
		}

		if (gthread_id_network_status_check) {
			pthread_cancel(gthread_id_network_status_check);
			pthread_join(gthread_id_network_status_check, NULL);
			gthread_id_network_status_check = 0;
		}

		if (es_terminate_enrollee() == ES_ERROR) {
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Init EasySetup Failed");
			return ESM_ERROR;
		}

		if (g_dev_conf_prov_data) {
			things_free(g_dev_conf_prov_data);
			g_dev_conf_prov_data = NULL;
		}

		if (g_wifi_prov_data) {
			things_free(g_wifi_prov_data);
			g_wifi_prov_data = NULL;
		}

		if (g_cloud_prov_data) {
			things_free(g_cloud_prov_data);
			g_cloud_prov_data = NULL;
		}
	}

	g_dev_conf_prov_data = (es_dev_conf_prov_data_s *) things_malloc(sizeof(es_dev_conf_prov_data_s));
	g_wifi_prov_data = (es_wifi_prov_data_s *) things_malloc(sizeof(es_wifi_prov_data_s));
	g_cloud_prov_data = (es_cloud_prov_data_s *) things_malloc(sizeof(es_cloud_prov_data_s));

	if (g_dev_conf_prov_data == NULL || g_wifi_prov_data == NULL || g_cloud_prov_data == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Easy Setup Memory allocation is failed.");
		return ESM_ERROR;
	}

	if (server_builder != NULL) {
		g_server_builder = server_builder;
	}

	if (g_server_builder == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] g_server_builder is NULL.");
		return ESM_ERROR;
	}
#ifdef __SECURED__
	bool g_is_secured = true;
#else
	bool g_is_secured = false;
#endif

	es_resource_mask_e resourcem_mask = ES_WIFI_RESOURCE | ES_CLOUD_RESOURCE | ES_DEVCONF_RESOURCE;
	// Easy Setup Module Initiation

	if (es_init_enrollee(g_is_secured, resourcem_mask, g_callbacks) != ES_OK) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "StartEasySetup and onboarding Fail!!");
		return ESM_ERROR;
	}

	es_set_state(ES_STATE_INIT);
	es_set_error_code(ES_ERRCODE_NO_ERROR);

	if (es_set_device_property(&device_property) == ES_ERROR) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "es_set_device_property funtion Failed");
	}
#ifdef __SECURED__

	if (OC_STACK_OK != SetRandomPinPolicy(8, NUM_PIN)) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Set to Non-Numerical PIN Generation~!!!!!");
	} else {
		THINGS_LOG(THINGS_INFO, TAG, "Set to Numerical PIN Generation~!!!!!");
	}

	SetGeneratePinCB(&generate_pin_cb);
	SetClosePinDisplayCB(&pin_close_cb);
	SetUserConfirmCB(NULL, get_user_confirmation);
	SetVerifyOption(USER_CONFIRM);

#endif							//#ifdef __SECURED__
	if (gthread_id_cloud_refresh_check == 0) {
		esm_continue = 1;
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Create cloud_refresh_check_loop thread");
		pthread_create_rtos(&gthread_id_cloud_refresh_check, NULL, cloud_refresh_check_loop, (void *)&esm_continue, THINGS_STACK_CLOUD_TOKEN_CHECK_THREAD);
	}
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
	return ESM_OK;
}

esm_result_e esm_terminate_easysetup()
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Terminate EasySetup");
	esm_continue = 0;
	if (gthread_id_cloud_refresh_check) {
		pthread_cancel(gthread_id_cloud_refresh_check);
		pthread_join(gthread_id_cloud_refresh_check, NULL);
		gthread_id_cloud_refresh_check = 0;
	}

	if (gthread_id_network_status_check) {
		pthread_cancel(gthread_id_network_status_check);
		pthread_join(gthread_id_network_status_check, NULL);
		gthread_id_network_status_check = 0;
	}

	if (es_terminate_enrollee() != ES_OK) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Terminate EasySetup Error");
		return ESM_ERROR;
	}

	if (g_dev_conf_prov_data) {
		things_free(g_dev_conf_prov_data);
		g_dev_conf_prov_data = NULL;
	}

	if (g_wifi_prov_data) {
		things_free(g_wifi_prov_data);
		g_wifi_prov_data = NULL;
	}

	if (g_cloud_prov_data) {
		things_free(g_cloud_prov_data);
		g_cloud_prov_data = NULL;
	}

	g_server_builder = NULL;
	esm_continue = 0;

	return ESM_MODULE_TERMINATED;
}

static int checkRefresh(int *hour_cnt_24)
{
	OCStackResult ret = OC_STACK_ERROR;
	int sleep_sec = 0;

	if (*hour_cnt_24 >= CI_COMMON_REFRESH_TIME) {
		if ((ret = refresh_token_into_cloud()) != OC_STACK_OK) {
			THINGS_LOG_D(THINGS_DEBUG, TAG, "refresh_token_into_cloud is failed in cloud_refresh_check_loop");
		} else {
			*hour_cnt_24 = 0;
		}
	} else {
		// Find Resource & if accesstoken is expireded then refresh process is called.
		if ((ret = find_cloud_resource()) != OC_STACK_OK) {
			THINGS_LOG_D(THINGS_DEBUG, TAG, "find_cloud_resource is failed in cloud_refresh_check_loop");
		}
	}

	if (ret != OC_STACK_OK) {
		sleep_sec = i_fail_sleep_sec;
	} else {
		sleep_sec = i_common_sleep_sec;
		(*hour_cnt_24)++;
	}

	return sleep_sec;
}

static void *cloud_refresh_check_loop(void *param)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "cloud_refresh_check_loop Close Loop Enter.");

	int i_failed_cnt = 0;
	int sleep_sec = i_common_sleep_sec;
	int *i_continue = (int *)param;
	int hour_cnt_24 = 0;

	THINGS_LOG_D(THINGS_DEBUG, TAG, "device ID = %s", OCGetServerInstanceIDString());

	// Close Loop Thread.
	while (*i_continue) {
		if (es_get_state() != ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD) {
			hour_cnt_24++;
			sleep_sec = i_common_sleep_sec;
			sleep(sleep_sec);
			continue;
		}

		if ((sleep_sec = checkRefresh(&hour_cnt_24)) == i_fail_sleep_sec) {
			i_failed_cnt++;
		} else {
			i_failed_cnt = 0;
		}

		if (i_failed_cnt >= MAX_REFRESHCHECK_CNT) {
			THINGS_LOG_D(THINGS_INFO, TAG, "Refresh checking routin is Failed. Return cloud state to INIT.");
			i_failed_cnt = 0;
			esm_get_network_status();	// State return
		}

		sleep(sleep_sec);
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "cloud_refresh_check_loop Close Loop Exit.");

	return NULL;
}

static int things_update_dev_prov_data(es_dev_conf_prov_data_s *dev_prov_data)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter.");

	if (dev_prov_data == NULL) {
		return -1;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Country=%s, DateTime=%s, Language=%s", dev_prov_data->country, dev_prov_data->datetime, dev_prov_data->language);

	if (g_update_dev_prov_data == NULL) {
		THINGS_LOG_D(THINGS_INFO, TAG, "st_things API is not registered.(gUpdateDateTime)");
		return -1;
	}

	return g_update_dev_prov_data(dev_prov_data);
}

static void *wifi_prov_set_loop(void *param)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Open Loop Enter.");

	static int timeout_handle_num = 100;
	timeout_s timeout;

	es_set_state(ES_STATE_CONNECTING_TO_ENROLLER);
	set_wifi_prov_state(WIFI_READY);

	while (get_wifi_prov_state() == WIFI_READY) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "waiting... WIFI_RESPONSE-Done for wifi provisioning.");
		usleep(50000);
	}

	if (get_wifi_prov_state() != WIFI_RESPONSE) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "WiFi Response is failed.");
		set_wifi_prov_state(WIFI_INIT);
		things_free((access_point_info_s *) param);
		return 0;
	}
	// Notify DevConfProvData to THINGS_App.
	things_update_dev_prov_data(g_dev_conf_prov_data);

	usleep(50000);

	int res = things_set_ap_connection((access_point_info_s *) param);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Set AP Connection Result : %d", res);

	if (res != 1) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Wifi Provisioning is Failed");
		es_set_state(ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER);
		things_free((access_point_info_s *) param);
		set_wifi_prov_state(WIFI_INIT);
		return 0;
	}

	things_wifi_state_changed_cb_init();
	set_wifi_prov_state(WIFI_SET);
	things_free((access_point_info_s *) param);

	if (add_request_handle((OCDoHandle) timeout_handle_num) != NULL) {
		timeout.cur_num = 300;	// 5 min    because, st_things-App wait AP-Connecting for 5 min.
		timeout.cur_counter = timeout.cur_num;
		create_time_out_process((OCDoHandle) timeout_handle_num++, wifi_prov_timeout_handler, &timeout);
	}

	gthread_id_network_status_check = 0;
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Open Loop Exit.");

	return NULL;
}

static void *wifi_prov_timeout_handler(timeout_s *param)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "WiFi Setting of Provisioning data was Time-Out.");

	es_set_state(ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER);
	es_set_error_code(ES_ERRCODE_NO_INTERNETCONNECTION);

	THINGS_LOG_ERROR(THINGS_ERROR, TAG, "es_set_state ES_STATE_CONNECTED_FAIL_TO_ENROLLER");

	if (ESM_OK != esm_init_easysetup(1, NULL)) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Easy-Setup Module Initialization failed");
		return NULL;
	}

	PROFILING_TIME("WiFi Provisioning End.");

	set_wifi_prov_state(WIFI_INIT);

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.");

	return NULL;
}

int esm_wifi_prov_check_cb(int enabled, char *ssid, char *addr)
{
	int ret = -1;

	if (get_wifi_prov_state() != WIFI_SET) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "WiFi Provisioning State is Not WIFI_SET step.(%d)", get_wifi_prov_state());
		return 0;
	}

	if (enabled != 1 || ssid == NULL || addr == NULL) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "enabled=%d, ssid=%s, addr=%s", enabled, ssid, addr);
		return 0;
	}

	pthread_mutex_lock(&g_es_mutex);
	if (strncmp(ssid, g_wifi_prov_data->ssid, strlen(g_wifi_prov_data->ssid)) == 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "CONNECTED TO TARGET AP  %s ", ssid);
		THINGS_LOG_D(THINGS_DEBUG, TAG, "GET IP ADDR  : %s ", addr);
		ret = 1;
	} else {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "SSID is not Targeted One : (%s), (%s)", ssid, g_wifi_prov_data->ssid);
		ret = 0;
	}
	pthread_mutex_unlock(&g_es_mutex);

	if (ret == 1) {
		es_set_state(ES_STATE_CONNECTED_TO_ENROLLER);
		set_wifi_prov_state(WIFI_DONE);
		THINGS_LOG(THINGS_DEBUG, TAG, "es_set_state ES_STATE_CONNECTED_TO_ENROLLER");

#ifndef __ST_THINGS_RTOS__
		if (g_server_builder->broadcast_presence(g_server_builder, 20) == 1) {
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Broadcast Presence Failed.");
			ret = 0;
		}
#endif

		del_all_request_handle();	// clear time-out thread.
		PROFILING_TIME("WiFi Provisioning End.");

		set_wifi_prov_state(WIFI_INIT);
	}

	return ret;
}

void wifi_prov_cb_in_app(es_wifi_prov_data_s *event_data)
{
	THINGS_LOG_V(THINGS_DEBUG, TAG, "Enter");
	access_point_info_s *p_info = NULL;

	if (event_data == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "es_wifi_prov_data_s Data Is NULL");
		return;
	}

	if (things_is_net_initialize() != 1) {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Network API Is not ready.");
		return;
	}

	p_info = (access_point_info_s *) things_malloc(sizeof(access_point_info_s));
	if (p_info == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "access_point_info_s memory allocation is failed.");
		return;
	}

	pthread_mutex_lock(&g_es_mutex);
	memcpy(g_wifi_prov_data, event_data, sizeof(es_wifi_prov_data_s));
	pthread_mutex_unlock(&g_es_mutex);

	// Attention : when 64bit linux Build, runtime-error occur. (memset () at ../sysdeps/x86_64/memset.S: Not Found File or Directory)
	// It's because of p_info.(64 bit malloc internal code flow is not support that character array data-type of access_point_info_s)
	// TODO : Fix Bug for 64bit support.
	memset(p_info->e_ssid, 0, MAX_ESSID);
	memset(p_info->security_key, 0, MAX_SECUIRTYKEY);
	memset(p_info->enc_type, 0, MAX_TYPE_ENC);
	memset(p_info->auth_type, 0, MAX_TYPE_AUTH);
	memset(p_info->channel, 0, MAX_CHANNEL);
	memset(p_info->bss_id, 0, MAX_BSSID);
	memset(p_info->signal_level, 0, MAX_LEVEL_SIGNAL);

	if (strlen(event_data->ssid) > 0) {
		things_strncpy(p_info->e_ssid, event_data->ssid, strlen(event_data->ssid));
	}

	if (strlen(event_data->pwd) > 0) {
		things_strncpy(p_info->security_key, event_data->pwd, strlen(event_data->pwd));
	}

	if (event_data->authtype >= NONE_AUTH && event_data->authtype <= WPA2_PSK) {
		switch (event_data->authtype) {
		case WEP:
			things_strncpy(p_info->auth_type, "WEP", strlen("WEP"));
			break;
		case WPA_PSK:
			things_strncpy(p_info->auth_type, "WPA-PSK", strlen("WPA-PSK"));
			break;
		case WPA2_PSK:
			things_strncpy(p_info->auth_type, "WPA2-PSK", strlen("WPA2-PSK"));
			break;
		case NONE_AUTH:
		default:
			things_strncpy(p_info->auth_type, "NONE", strlen("NONE"));
			break;
		}
	} else {
		things_strncpy(p_info->auth_type, "NONE", strlen("NONE"));
	}

	if (event_data->enctype >= NONE_ENC && event_data->enctype <= TKIP_AES) {
		switch (event_data->enctype) {
		case WEP_64:
			things_strncpy(p_info->enc_type, "WEP_64", strlen("WEP_64"));
			break;
		case WEP_128:
			things_strncpy(p_info->enc_type, "WEP_128", strlen("WEP_128"));
			break;
		case TKIP:
			things_strncpy(p_info->enc_type, "TKIP", strlen("TKIP"));
			break;
		case AES:
			things_strncpy(p_info->enc_type, "AES", strlen("AES"));
			break;
		case TKIP_AES:
			things_strncpy(p_info->enc_type, "TKIP_AES", strlen("TKIP_AES"));
			break;
		default:
			things_strncpy(p_info->enc_type, "NONE", strlen("NONE"));
			break;
		}
	} else {
		things_strncpy(p_info->enc_type, "NONE", strlen("NONE"));
	}

	if (event_data->discovery_channel != -1) {
		snprintf(p_info->channel, MAX_CHANNEL, "%d", event_data->discovery_channel);
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "e_ssid : %s", p_info->e_ssid);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "security_key : %s", p_info->security_key);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "auth_type : %s", p_info->auth_type);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "enc_type : %s", p_info->enc_type);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "channel : %s", p_info->channel);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Copied ssid = %s", g_wifi_prov_data->ssid);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Copied pwd = %s", g_wifi_prov_data->pwd);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Copied enctype = %d", g_wifi_prov_data->enctype);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Copied discovery_channel = %d", g_wifi_prov_data->discovery_channel);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Copied authtype = %d", g_wifi_prov_data->authtype);

	// Connect to AP
	if (gthread_id_network_status_check == 0) {
		THINGS_LOG(THINGS_DEBUG, TAG, "Create NetworkStatusCheck thread");
		del_all_request_handle();	// clear time-out thread.
		set_wifi_prov_state(WIFI_INIT);

		pthread_create_rtos(&gthread_id_network_status_check, NULL, wifi_prov_set_loop, (void *)p_info, THINGS_STACK_AP_INFO_SET_THREAD);
		set_wifi_prov_state(WIFI_READY);
	} else {
		things_free(p_info);
	}

	THINGS_LOG_V(THINGS_DEBUG, TAG, "Exit");
}

void dev_conf_prov_cb_in_app(es_dev_conf_prov_data_s *event_data)
{
	if (event_data == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "es_dev_conf_prov_data_s Is NULL");
		return;
	}

	pthread_mutex_lock(&g_es_mutex);
	memcpy(g_dev_conf_prov_data, event_data, sizeof(es_dev_conf_prov_data_s));
	pthread_mutex_unlock(&g_es_mutex);

	if (strlen(event_data->language) > 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Language : %s", event_data->language);
	}

	if (strlen(event_data->country) > 0) {
		// check payload
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Country : %s", event_data->country);
	}

	if (strlen(event_data->datetime) > 0) {
		// check payload
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Datetime : %s", event_data->datetime);
	}

	//Added by THINGS to set time in appliance yyyy-mm-ddThh-mm-ss
	char ch_year[5];
	char ch_month[3];
	char ch_day[3];
	char ch_hour[3];
	char ch_min[3];
	char ch_sec[3];
	struct tm st_time;
	memset(&st_time, 0, sizeof(st_time));

	int itr = 0;
	for (; itr < 4; itr++) {
		ch_year[itr] = event_data->datetime[itr];
	}
	ch_year[itr] = '\0';
	st_time.tm_year = (atoi(ch_year) - 1900);

	ch_month[0] = event_data->datetime[++itr];
	ch_month[1] = event_data->datetime[++itr];
	ch_month[2] = '\0';
	st_time.tm_mon = (atoi(ch_month) - 1);

	itr++;
	ch_day[0] = event_data->datetime[++itr];
	ch_day[1] = event_data->datetime[++itr];
	ch_day[2] = '\0';
	st_time.tm_mday = atoi(ch_day);

	itr++;
	ch_hour[0] = event_data->datetime[++itr];
	ch_hour[1] = event_data->datetime[++itr];
	ch_hour[2] = '\0';
	st_time.tm_hour = atoi(ch_hour);

	itr++;
	ch_min[0] = event_data->datetime[++itr];
	ch_min[1] = event_data->datetime[++itr];
	ch_min[2] = '\0';
	st_time.tm_min = atoi(ch_min);

	itr++;
	ch_sec[0] = event_data->datetime[++itr];
	ch_sec[1] = event_data->datetime[++itr];
	ch_sec[2] = '\0';
	st_time.tm_sec = atoi(ch_sec);

	unsigned int ep_time = (unsigned int)mktime(&st_time);

	struct timespec current_time;

	current_time.tv_sec = ep_time;
	current_time.tv_nsec = 0;

	if (clock_settime(CLOCK_REALTIME, &current_time) != 0) {
		THINGS_LOG_V(THINGS_ERROR, TAG, "Failed to clock_settime");
	}
}

void cloud_data_prov_cb_in_app(es_cloud_prov_data_s *event_data)
{
	if (event_data == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "es_cloud_prov_data_s Data Is NULL");
		return;
	}

	pthread_mutex_lock(&g_es_mutex);
	memcpy(g_cloud_prov_data, event_data, sizeof(es_cloud_prov_data_s));
	pthread_mutex_unlock(&g_es_mutex);

	if (strlen(event_data->auth_code) > 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "AuthCode : %s", g_cloud_prov_data->auth_code);
	}

	if (strlen(event_data->accesstoken) > 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Accesstoken : %s", g_cloud_prov_data->accesstoken);
	}

	if (strlen(event_data->refreshtoken) > 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Refreshtoken : %s", g_cloud_prov_data->refreshtoken);
	}

	if (strlen(event_data->uid) > 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Uid : %s", g_cloud_prov_data->uid);
	}

	if (strlen(event_data->auth_provider) > 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "AuthProvider : %s", g_cloud_prov_data->auth_provider);
	}

	if (strlen(event_data->host_name) > 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "CIHostName : %s", g_cloud_prov_data->host_name);
	}

	if (strlen(event_data->ip) > 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "CIip : %s", g_cloud_prov_data->ip);
	}

	if (strlen(event_data->port) > 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "CIPort : %s", g_cloud_prov_data->port);
	}

	if (strlen(event_data->ci_server) > 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "CIServer : %s", g_cloud_prov_data->ci_server);
	}

	if (strlen(event_data->server_id) > 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Server ID : %s", g_cloud_prov_data->server_id);
	}

	g_cloud_data_cb_to_app(g_cloud_prov_data);
	// CB to things_stack.c
	// Need to check the running state of the thread...
}

bool esm_get_network_status(void)
{
	bool is_ok = false;

	if (things_is_net_initialize() != 1) {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Network API is not initialized.");
		return false;
	}

	if (things_is_connected_ap() == true) {
		THINGS_LOG_V(THINGS_INFO, TAG, "Connected to AP");
		es_set_state(ES_STATE_CONNECTED_TO_ENROLLER);

		is_ok = true;
	} else {
		THINGS_LOG_V(THINGS_INFO, TAG, "Disconnected to AP");
		es_set_state(ES_STATE_INIT);
	}

	return is_ok;
}

void *esm_register_cloud_cb(void *func)
{
	g_cloud_data_cb_to_app = func;
	return NULL;
}

esm_result_e esm_set_wifi_conn_err(void)
{
	es_set_state(ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER);
	es_set_error_code(ES_ERRCODE_UNKNOWN);
	return ESM_OK;
}

int esm_register_pin_generated_cb(pin_generated_func_type func)
{
	if (func) {
		g_pin_generated_cb = func;
		return 1;
	} else {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Invalid PINGeneratedCB function");
		return 0;
	}
}

int esm_register_pin_close_cb(pin_close_func_type func)
{
	if (func) {
		g_pin_close_cb = func;
		return 1;
	} else {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Invalid PINGeneratedCB function");
		return 0;
	}
}

int esm_register_user_confirm_cb(user_confirm_result_func_type func)
{
	if (func) {
		g_user_confirm_cb = func;
		return 1;
	} else {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Invalid UserConformCB function");
		return 0;
	}
}
