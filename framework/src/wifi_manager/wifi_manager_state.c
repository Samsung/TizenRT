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
#include <tinyara/config.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <wifi_manager/wifi_manager.h>
#include <tinyara/wifi/wifi_common.h>
#include "wifi_manager_utils.h"
#include "wifi_manager_profile.h"
#include "wifi_manager_dhcp.h"
#include "wifi_manager_stats.h"
#include "wifi_manager_event.h"
#include "wifi_manager_msghandler.h"
#include "wifi_manager_error.h"
#include "wifi_manager_cb.h"
#include "wifi_manager_state.h"
#include "wifi_manager_info.h"


enum _wifimgr_disconn_substate {
	WIFIMGR_DISCONN_NONE = -1,
	WIFIMGR_DISCONN_DEINIT, // turn off wi-fi manager while station connected to AP
	WIFIMGR_DISCONN_SOFTAP, // set softap mode while station connected to AP
	WIFIMGR_DISCONN_INTERNAL_ERROR, // internal error ex) dhcp fails
	WIFIMGR_DISCONN_MAX,
};
typedef enum _wifimgr_disconn_substate _wifimgr_disconn_substate_e;

static char *wifimgr_state_str[] = {
	"WIFIMGR_UNINITIALIZED",
	"WIFIMGR_STA_DISCONNECTED",
	"WIFIMGR_STA_DISCONNECTING",
	"WIFIMGR_STA_CONNECTING",
	"WIFIMGR_STA_CONNECTED",
	"WIFIMGR_STA_RECONNECT",
	"WIFIMGR_STA_RECONNECTING",
	"WIFIMGR_STA_CONNECT_CANCEL",
	"WIFIMGR_SOFTAP",
	"WIFIMGR_SCANNING",
	"WIFIMGR_NONE",
	"WIFIMGR_STATE_MAX",
};

/*  Setting MACRO */
static inline void WIFIMGR_SET_SSID(char *s)
{
	wifimgr_info_msg_s winfo;
	winfo.ssid = s;
	wifimgr_set_info(WIFIMGR_SSID, &winfo);
}


static inline void WIFIMGR_SET_SOFTAP_SSID(char *s)
{
	wifimgr_info_msg_s winfo;
	winfo.softap_ssid = s;
	wifimgr_set_info(WIFIMGR_SOFTAP_SSID, &winfo);
}


static inline void WIFIMGR_SET_MACADDR(char *s)
{
	wifimgr_info_msg_s winfo;
	winfo.mac_addr = s;
	wifimgr_set_info(WIFIMGR_MACADDR, &winfo);
}

/*  Copy MACRO */
#define WIFIMGR_COPY_SOFTAP_CONFIG(dest, src)							\
	do {																\
		(dest).channel = (src)->channel;								\
		strncpy((dest).ssid, (src)->ssid, WIFIMGR_SSID_LEN);			\
		(dest).ssid[WIFIMGR_SSID_LEN] = '\0';							\
		strncpy((dest).passphrase, (src)->passphrase, WIFIMGR_PASSPHRASE_LEN); \
		(dest).passphrase[WIFIMGR_PASSPHRASE_LEN] = '\0';				\
	} while (0)

#define WIFIMGR_COPY_AP_INFO(dest, src)									\
	do {																\
		(dest).ssid_length = (src).ssid_length;							\
		(dest).passphrase_length = (src).passphrase_length;				\
		strncpy((dest).ssid, (src).ssid, WIFIMGR_SSID_LEN);				\
		(dest).ssid[WIFIMGR_SSID_LEN] = '\0';							\
		strncpy((dest).passphrase, (src).passphrase, WIFIMGR_PASSPHRASE_LEN); \
		(dest).passphrase[WIFIMGR_PASSPHRASE_LEN] = '\0';				\
		(dest).ap_auth_type = (src).ap_auth_type;						\
		(dest).ap_crypto_type = (src).ap_crypto_type;					\
	} while (0)

#define WIFIMGR_COPY_RECONN_INFO(dest, src)			\
	do {											\
		(dest).type = (src).type;					\
		(dest).interval = (src).interval;			\
		(dest).max_interval = (src).max_interval;	\
	} while (0)

/*  Auto connect MACRO */
/* Check if external wifi driver supports auto(re)connect */
#ifndef CONFIG_DISABLE_EXTERNAL_AUTOCONNECT
#define WIFIDRIVER_SUPPORT_AUTOCONNECT 1
#else
#define WIFIDRIVER_SUPPORT_AUTOCONNECT 0
#endif

#define WIFIMGR_IPC_PORT 9098

/*  Initialize MACRO */
#define WM_APINFO_INITIALIZER {{0,}, 0, {0,}, 0, WIFI_MANAGER_AUTH_UNKNOWN, WIFI_MANAGER_CRYPTO_UNKNOWN}
#define WM_RECONN_INITIALIZER {WIFI_RECONN_NONE, -1, -1}
#define WIFIMGR_SOTFAP_CONFIG {{0,}, {0,}, 1}

struct _wifimgr_state_handle {
	wifimgr_state_e state;
	wifimgr_state_e prev_state; // it is for returning to previous sta state after scanning is done
	// event
	wifimgr_evt_handler_s cbk;
	// substate
	_wifimgr_disconn_substate_e disconn_substate;
	sem_t *api_sig;
	wifi_manager_softap_config_s softap_config;

	// Reconnect
	pthread_t reconn_id; // thread id of reconn_worker: it is used for terminating _reconn_worker
	wifi_manager_ap_config_s connected_ap;
	wifi_manager_reconnect_config_s conn_config;
	int terminate;	// it is protected by g_reconn_mutex to sync between the callback task and reconn_worker
	int conn_tries; // to do: set  it by Kconfig
	int max_tries;
};
typedef struct _wifimgr_state_handle _wifimgr_state_handle_s;


/**
 * global variables
 */
static _wifimgr_state_handle_s g_manager_info = {
										 WIFIMGR_UNINITIALIZED, WIFIMGR_UNINITIALIZED, // state, prev_state
										 // event
										 WIFIMGR_EVENT_INITIALIZER,
										 // substate
										 WIFIMGR_DISCONN_NONE, // _wifimgr_disconn_substate_e
										 NULL,
										 WIFIMGR_SOTFAP_CONFIG,
										 // reconnect
										 0,
										 WM_APINFO_INITIALIZER,
										 WM_RECONN_INITIALIZER,
										 0, 0, 10};

/*  Auto connect variable */
#if WIFIDRIVER_SUPPORT_AUTOCONNECT == 0
static pthread_mutex_t g_reconn_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_reconn_signal = PTHREAD_COND_INITIALIZER;
#define LOCK_RECONN pthread_mutex_lock(&g_reconn_mutex);
#define UNLOCK_RECONN pthread_mutex_unlock(&g_reconn_mutex);
#define WIFIMGR_TRIGGER_RECONNECT_WORKER		\
	do {										\
		g_manager_info.conn_tries++;			\
		pthread_mutex_lock(&g_reconn_mutex);	\
		pthread_cond_signal(&g_reconn_signal);	\
		pthread_mutex_unlock(&g_reconn_mutex);	\
	} while (0)

#define WIFIMGR_WAIT_RECONNECT_EVENT							\
	do {														\
		pthread_mutex_lock(&g_reconn_mutex);					\
		pthread_cond_wait(&g_reconn_signal, &g_reconn_mutex);	\
		pthread_mutex_unlock(&g_reconn_mutex);					\
		WM_LOG_VERBOSE("[WM] T%d wake up\n", getpid());					\
	} while (0)

#define WIFIMGR_TERMINATE_RECONN_WORKER									\
	do {																\
		WM_LOG_VERBOSE("[WM] send terminate signal to worker(%d/%d)\n", g_manager_info.conn_tries, g_manager_info.max_tries); \
		pthread_mutex_lock(&g_reconn_mutex);							\
		g_manager_info.terminate = true;								\
		pthread_cond_signal(&g_reconn_signal);							\
		pthread_mutex_unlock(&g_reconn_mutex);							\
		WM_LOG_VERBOSE("[WM] wait worker to terminate\n");						\
		pthread_join(g_manager_info.reconn_id, NULL);					\
	} while (0)
#endif


/**
 * Internal functions
 */
static wifi_manager_result_e _wifimgr_deinit(void);
static wifi_manager_result_e _wifimgr_run_sta(void);
static wifi_manager_result_e _wifimgr_connect_ap(wifi_manager_ap_config_s *config);
static wifi_manager_result_e _wifimgr_save_connected_config(wifi_manager_ap_config_s *config);
static wifi_manager_result_e _wifimgr_disconnect_ap(void);
static wifi_manager_result_e _wifimgr_run_softap(wifi_manager_softap_config_s *config);
static wifi_manager_result_e _wifimgr_stop_softap(void);
static wifi_manager_result_e _wifimgr_scan(wifi_manager_ap_config_s *config);

//
// functions managing a state machine
//
static wifi_manager_result_e _handle_request(wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_uninitialized_state(wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_disconnected_state(wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_disconnecting_state(wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_connecting_state(wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_connected_state(wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_reconnecting_state(wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_reconnect_state(wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_connect_cancel_state(wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_softap_state(wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_scanning_state(wifimgr_msg_s *msg);

typedef wifi_manager_result_e (*wifimgr_handler)(wifimgr_msg_s *msg);

/*
 * g_handler should be matched to _wifimgr_state
 */
static const wifimgr_handler g_handler[] = {
	_handler_on_uninitialized_state,
	_handler_on_disconnected_state,
	_handler_on_disconnecting_state,
	_handler_on_connecting_state,
	_handler_on_connected_state,
	_handler_on_reconnect_state,
	_handler_on_reconnecting_state,
	_handler_on_connect_cancel_state,
	_handler_on_softap_state,
	_handler_on_scanning_state,
};

/*  State MACRO */
#define WIFIMGR_CHECK_STATE(s) ((s) != g_manager_info.state)
#define WIFIMGR_IS_STATE(s) ((s) == g_manager_info.state)
#define WIFIMGR_GET_STATE g_manager_info.state
#define WIFIMGR_GET_PREVSTATE g_manager_info.prev_state
#define WIFIMGR_STORE_PREV_STATE (g_manager_info.prev_state = g_manager_info.state)
#define WIFIMGR_RESTORE_STATE								\
	do {													\
		g_manager_info.state = g_manager_info.prev_state;	\
		g_manager_info.prev_state = WIFIMGR_NONE;			\
	} while (0)

static inline void WIFIMGR_SET_STATE(wifimgr_state_e s)
{
	g_manager_info.state = s;
	wifimgr_info_msg_s wmsg;
	wmsg.state = s;
	wifimgr_set_info(WIFIMGR_STATE, &wmsg);
}


static inline char *wifimgr_get_state_str(int state)
{
	return wifimgr_state_str[state];
}


/*  Substate MACRO */
static inline void WIFIMGR_SET_SUBSTATE(_wifimgr_disconn_substate_e state, sem_t *signal)
{
	g_manager_info.disconn_substate = state;
	g_manager_info.api_sig = signal;
}

static inline void WIFIMGR_RESET_SUBSTATE(void)
{
	g_manager_info.disconn_substate = WIFIMGR_DISCONN_NONE;
	g_manager_info.api_sig = NULL;
}

static inline void WIFIMGR_SEND_API_SIGNAL(sem_t *api_sig)
{
	/*	send signal to wifi_manager_api to notify request is done*/
	if (api_sig) {
		sem_post(api_sig);
	}
}

/*
 * Internal functions
 */
wifi_manager_result_e _wifimgr_deinit(void)
{
	WM_ENTER;
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_deinit(), "[WM] wifi_utils_deinit fail", WIFI_MANAGER_FAIL);
	wifimgr_unregister_all();

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_run_sta(void)
{
	WM_ENTER;
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_start_sta(), "[WM] Starting STA failed.", WIFI_MANAGER_FAIL);
#if WIFIDRIVER_SUPPORT_AUTOCONNECT
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_set_autoconnect(1), "[WM] Set Autoconnect failed", WIFI_MANAGER_FAIL);
#else
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_set_autoconnect(0), "[WM] Set Autoconnect failed", WIFI_MANAGER_FAIL);
#endif
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_save_connected_config(wifi_manager_ap_config_s *config)
{
#ifdef CONFIG_WIFI_MANAGER_SAVE_CONFIG
	WM_ENTER;
	wifi_utils_result_e ret = wifi_profile_write(config, 1);
	if (ret != WIFI_UTILS_SUCCESS) {
		WM_LOG_ERROR("[WM] Failed to save the connected AP configuration in file system\n");
		return WIFI_MANAGER_FAIL;
	}
#endif
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_connect_ap(wifi_manager_ap_config_s *config)
{
	WM_ENTER;
	wifi_utils_ap_config_s util_config;
	strncpy(util_config.ssid, config->ssid, WIFIMGR_SSID_LEN);
	util_config.ssid[WIFIMGR_SSID_LEN] = '\0';
	util_config.ssid_length = config->ssid_length;
	strncpy(util_config.passphrase, config->passphrase, WIFIMGR_PASSPHRASE_LEN);
	util_config.passphrase[WIFIMGR_PASSPHRASE_LEN] = '\0';
	util_config.passphrase_length = config->passphrase_length;
	util_config.ap_auth_type = config->ap_auth_type;
	util_config.ap_crypto_type = config->ap_crypto_type;

	wifi_utils_result_e wres = wifi_utils_connect_ap(&util_config, NULL);
	if (wres == WIFI_UTILS_ALREADY_CONNECTED) {
		return WIFI_MANAGER_ALREADY_CONNECTED;
	} else if (wres != WIFI_UTILS_SUCCESS) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_CONNECT_FAIL);
		return WIFI_MANAGER_FAIL;
	}
	WIFIMGR_SET_SSID(config->ssid);
	wifi_manager_result_e wret = _wifimgr_save_connected_config(config);
	if (wret != WIFI_MANAGER_SUCCESS) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INTERNAL_FAIL);
		return wret;
	}

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_disconnect_ap(void)
{
	WM_ENTER;
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_disconnect_ap(NULL), "[WM] disconnect to ap fail\n", WIFI_MANAGER_FAIL);
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_run_softap(wifi_manager_softap_config_s *config)
{
	WM_ENTER;
	if (strlen(config->ssid) > WIFIMGR_SSID_LEN || strlen(config->passphrase) > WIFIMGR_PASSPHRASE_LEN) {
		WM_LOG_ERROR("[WM] SSID or PASSPHRASE length invalid\n");
		return WIFI_MANAGER_FAIL;
	}
	wifi_utils_softap_config_s softap_config;

	softap_config.channel = config->channel;
	softap_config.ap_crypto_type = WIFI_UTILS_CRYPTO_AES;
	softap_config.ap_auth_type = WIFI_UTILS_AUTH_WPA2_PSK;
	softap_config.ssid_length = strlen(config->ssid);
	softap_config.passphrase_length = strlen(config->passphrase);
	strncpy(softap_config.ssid, config->ssid, WIFIMGR_SSID_LEN);
	softap_config.ssid[WIFIMGR_SSID_LEN] = '\0';
	strncpy(softap_config.passphrase, config->passphrase, WIFIMGR_PASSPHRASE_LEN);
	softap_config.passphrase[WIFIMGR_PASSPHRASE_LEN] = '\0';

	WIFIMGR_CHECK_UTILRESULT(wifi_utils_start_softap(&softap_config),
							 "[WM] Starting softap mode failed.", WIFI_MANAGER_FAIL);
#ifndef CONFIG_WIFIMGR_DISABLE_DHCPS
	WIFIMGR_CHECK_RESULT(wm_dhcps_start((dhcp_sta_joined_cb)g_manager_info.cbk.dhcps_sta_joined),
						 "[WM] Starting DHCP server failed.\n", WIFI_MANAGER_FAIL);
#endif
	/* update wifi_manager_info */
	WIFIMGR_SET_SOFTAP_SSID(config->ssid);
	dhcps_reset_num();

	/* For tracking softap stats, the LAST value is used */
	WIFIMGR_STATS_INC(CB_SOFTAP_DONE);
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_stop_softap(void)
{
	WM_ENTER;
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_stop_softap(), "[WM] Stoping softap failed", WIFI_MANAGER_FAIL);
#ifndef CONFIG_WIFIMGR_DISABLE_DHCPS
	WIFIMGR_CHECK_RESULT(wm_dhcps_stop(), "[WM] Stoping softap DHCP server failed.", WIFI_MANAGER_FAIL);
#endif
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_scan(wifi_manager_ap_config_s *config)
{
	WM_ENTER;
	wifi_utils_ap_config_s uconf;

	if (!config) {
		WIFIMGR_CHECK_UTILRESULT(wifi_utils_scan_ap(NULL),
								 "[WM] request scan to wifi utils is fail\n", WIFI_MANAGER_FAIL);
		return WIFI_MANAGER_SUCCESS;
	}

	strncpy(uconf.ssid, config->ssid, WIFIMGR_SSID_LEN);
	uconf.ssid[WIFIMGR_SSID_LEN] = '\0';
	uconf.ssid_length = config->ssid_length;
	if (config->passphrase_length > 0) {
		strncpy(uconf.passphrase, config->passphrase, WIFIMGR_PASSPHRASE_LEN);
		uconf.passphrase[WIFIMGR_PASSPHRASE_LEN] = '\0';
		uconf.passphrase_length = config->passphrase_length;
	}
	uconf.ap_auth_type = config->ap_auth_type;
	uconf.ap_crypto_type = config->ap_crypto_type;
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_scan_ap((void *)&uconf),
							 "[WM] request scan to wifi utils is fail\n", WIFI_MANAGER_FAIL);

	return WIFI_MANAGER_SUCCESS;
}


/*
 * State Functions
 */
wifi_manager_result_e _handler_on_uninitialized_state(wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	wifimgr_evt_e evt = msg->event;
	if (evt != EVT_INIT_CMD) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_FAIL;
	}

	WIFIMGR_CHECK_UTILRESULT(wifi_utils_init(), "[WM] wifi_utils_init fail\n", WIFI_MANAGER_FAIL);
#if WIFIDRIVER_SUPPORT_AUTOCONNECT
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_set_autoconnect(1), "[WM] Set Autoconnect failed", WIFI_MANAGER_FAIL);
#else
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_set_autoconnect(0), "[WM] Set Autoconnect failed", WIFI_MANAGER_FAIL);
#endif
#ifdef CONFIG_WIFI_MANAGER_SAVE_CONFIG
	WIFIMGR_CHECK_UTILRESULT(wifi_profile_init(), "[WM] wifi_profile init fail\n", WIFI_MANAGER_FAIL);
#endif
	/*  get event function pointers from event handler */
	int res = wifimgr_get_evthandler(&g_manager_info.cbk);
	if (res < 0) {
		WM_LOG_ERROR("[WM][ERR] WIFIMGR GET EVENT\n");
	}
	wifi_utils_register_callback(&g_manager_info.cbk.wifi_evt);

	/*  register default callback to callback handler */
	wifi_manager_cb_s *cb = (wifi_manager_cb_s *)msg->param;
	res = wifimgr_register_cb(cb);
	if (res < 0) {
		WM_LOG_ERROR("[WM][ERR] WIFIMGR REGISTER CB\n");
	}

	/* Get information from porting layer to set MAC addr  */
	wifi_utils_info_s info;
	wifi_utils_result_e wres = wifi_utils_get_info(&info);
	if (wres != WIFI_UTILS_SUCCESS) {
		WM_LOG_ERROR("[WM] T%d wifi_utils_get_info fail\n", getpid());
		WIFIMGR_CHECK_UTILRESULT(wifi_utils_deinit(), "critical error\n", WIFI_MANAGER_FAIL);
	}

	WIFIMGR_SET_MACADDR((char *)info.mac_address);

	WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_disconnected_state(wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;

	if (msg->event == EVT_CONNECT_CMD) {
		_wifimgr_conn_info_msg_s *conn_msg = (_wifimgr_conn_info_msg_s *)msg->param;
		wifi_manager_ap_config_s *apinfo = conn_msg->config;
		wifi_manager_reconnect_config_s *conn_config = conn_msg->conn_config;
		/* store wifi ap info to reconnect when the device is disconnected abnormaly*/
		WIFIMGR_COPY_AP_INFO(g_manager_info.connected_ap, *apinfo);
		WIFIMGR_COPY_RECONN_INFO(g_manager_info.conn_config, *conn_config);
		WIFIMGR_CHECK_RESULT(_wifimgr_connect_ap(apinfo), "connect ap fail\n", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECTING);
	} else if (msg->event == EVT_DEINIT_CMD) {
		WIFIMGR_CHECK_RESULT(_wifimgr_deinit(), "critical error", WIFI_MANAGER_FAIL);
		WIFIMGR_SEND_API_SIGNAL(msg->signal);
		WIFIMGR_SET_STATE(WIFIMGR_UNINITIALIZED);
	} else if (msg->event == EVT_SET_SOFTAP_CMD) {
		WIFIMGR_CHECK_RESULT(_wifimgr_run_softap((wifi_manager_softap_config_s *)msg->param),
							 "run_softap fail", WIFI_MANAGER_FAIL);
		WIFIMGR_SEND_API_SIGNAL(msg->signal);
		WIFIMGR_SET_STATE(WIFIMGR_SOFTAP);
	} else if (msg->event == EVT_SCAN_CMD) {
		WIFIMGR_CHECK_RESULT(_wifimgr_scan((wifi_manager_ap_config_s *)msg->param), "fail scan", WIFI_MANAGER_FAIL);
		WIFIMGR_STORE_PREV_STATE;
		WIFIMGR_SET_STATE(WIFIMGR_SCANNING);
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_disconnecting_state(wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	if (msg->event != EVT_STA_DISCONNECTED) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_BUSY;
	}

	/* it handles disconnecting state differently by substate
	 * for example, if it enters disconnecting state because dhcpc fails
	 * then it should not call disconnect callback to applications */
	switch (g_manager_info.disconn_substate) {
	case WIFIMGR_DISCONN_DEINIT:
		WIFIMGR_SEND_API_SIGNAL(g_manager_info.api_sig);
		WIFIMGR_SET_STATE(WIFIMGR_UNINITIALIZED);
		break;
	case WIFIMGR_DISCONN_SOFTAP:
		WIFIMGR_CHECK_RESULT(_wifimgr_run_softap(
								 (wifi_manager_softap_config_s *)&g_manager_info.softap_config),
								 "run_softap fail", WIFI_MANAGER_FAIL);
		WIFIMGR_SEND_API_SIGNAL(g_manager_info.api_sig);
		WIFIMGR_SET_STATE(WIFIMGR_SOFTAP);
		break;
	case WIFIMGR_DISCONN_INTERNAL_ERROR:
		wifimgr_call_cb(CB_STA_CONNECT_FAILED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
		break;
	case WIFIMGR_DISCONN_NONE:
		wifimgr_call_cb(CB_STA_DISCONNECTED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
		break;
	default:
		WM_LOG_ERROR("[WM] invalid argument\n");
		break;
	}

	WIFIMGR_RESET_SUBSTATE();

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_connecting_state(wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	if (msg->event == EVT_STA_CONNECTED) {
#ifndef CONFIG_WIFIMGR_DISABLE_DHCPC
		wifi_manager_result_e wret;
		wret = dhcpc_get_ipaddr();
		if (wret != WIFI_MANAGER_SUCCESS) {
			WIFIMGR_CHECK_RESULT(_wifimgr_disconnect_ap(), "[WM] critical error: DHCP failure\n", WIFI_MANAGER_FAIL);
			WIFIMGR_SET_SUBSTATE(WIFIMGR_DISCONN_INTERNAL_ERROR, NULL);
			WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTING);
			return wret;
		}
#endif
		wifimgr_call_cb(CB_STA_CONNECTED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECTED);
	} else if (msg->event == EVT_STA_CONNECT_FAILED) {
		wifimgr_call_cb(CB_STA_CONNECT_FAILED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_BUSY;
	}

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_connected_state(wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	if (msg->event == EVT_DISCONNECT_CMD) {
		WIFIMGR_CHECK_RESULT(_wifimgr_disconnect_ap(), "critical error", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTING);
	} else if (msg->event == EVT_STA_DISCONNECTED) {
#if WIFIDRIVER_SUPPORT_AUTOCONNECT == 0
		if (g_manager_info.conn_config.type == WIFI_RECONN_NONE) {
			wifimgr_call_cb(CB_STA_DISCONNECTED, NULL);
			dhcpc_close_ipaddr();
			WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
		} else {
			wifimgr_call_cb(CB_STA_RECONNECTED, NULL);
			_wifimgr_conn_info_msg_s *rmsg = (_wifimgr_conn_info_msg_s *)malloc(sizeof(_wifimgr_conn_info_msg_s));
			if (!rmsg) {				// critical error
				WM_LOG_ERROR("[WM] allocate memory for reconn msg fail\n");
				WIFIADD_ERR_RECORD(ERR_WIFIMGR_INTERNAL_FAIL);
				return WIFI_MANAGER_FAIL;
			}
			rmsg->config = (wifi_manager_ap_config_s *)malloc(sizeof(wifi_manager_ap_config_s));
			if (!rmsg->config) {				// critical error
				free(rmsg);
				WM_LOG_ERROR("[WM] allocate memory for reconnmsg apconfig is fail\n");
				WIFIADD_ERR_RECORD(ERR_WIFIMGR_INTERNAL_FAIL);
				return WIFI_MANAGER_FAIL;
			}

			rmsg->conn_config = (wifi_manager_reconnect_config_s *)malloc(sizeof(wifi_manager_reconnect_config_s));
			if (!rmsg->conn_config) {				// critical error
				free(rmsg->config);
				free(rmsg);
				WM_LOG_ERROR("[WM] allocate memory for reonnmsg conn_config is fail\n");
				WIFIADD_ERR_RECORD(ERR_WIFIMGR_INTERNAL_FAIL);
				return WIFI_MANAGER_FAIL;
			}
			WIFIMGR_COPY_AP_INFO(*(rmsg->config), g_manager_info.connected_ap);
			WIFIMGR_COPY_RECONN_INFO(*(rmsg->conn_config), g_manager_info.conn_config);

			g_manager_info.conn_tries = 0;
			g_manager_info.terminate = false;

			// todo : post message to reconnect

			WIFIMGR_SET_STATE(WIFIMGR_STA_RECONNECT);
		}
#else /* WIFIDRIVER_SUPPORT_AUTOCONNECT */
		WM_LOG_VERBOSE("[WM] External AUTOCONNECT: go to RECONNECT state\n");
		wifimgr_call_cb(CB_STA_RECONNECTED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_RECONNECT);
#endif /* WIFIDRIVER_SUPPORT_AUTOCONNECT */
	} else if (msg->event == EVT_SET_SOFTAP_CMD) {
		dhcpc_close_ipaddr();
		WIFIMGR_COPY_SOFTAP_CONFIG(g_manager_info.softap_config, (wifi_manager_softap_config_s *)msg->param);
		WIFIMGR_CHECK_RESULT(_wifimgr_disconnect_ap(), "critical error", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_SUBSTATE(WIFIMGR_DISCONN_SOFTAP, msg->signal);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTING);
	} else if (msg->event == EVT_DEINIT_CMD) {
		dhcpc_close_ipaddr();
		WIFIMGR_CHECK_RESULT(_wifimgr_disconnect_ap(), "critical error", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_SUBSTATE(WIFIMGR_DISCONN_DEINIT, msg->signal);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTING);
	} else if (msg->event == EVT_SCAN_CMD) {
		WIFIMGR_CHECK_RESULT(_wifimgr_scan((wifi_manager_ap_config_s *)msg->param), "fail scan", WIFI_MANAGER_FAIL);
		WIFIMGR_STORE_PREV_STATE;
		WIFIMGR_SET_STATE(WIFIMGR_SCANNING);
	} else if (msg->event == EVT_CONNECT_CMD) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_ALREADY_CONNECTED;
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_reconnect_state(wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
#if WIFIDRIVER_SUPPORT_AUTOCONNECT == 0
	WM_LOG_VERBOSE("[WM] INTERNAL AUTOCONNECT event status : %d\n", msg->event);
	if (msg->event == EVT_DISCONNECT_CMD) {
		pthread_mutex_lock(&g_reconn_mutex);
		g_manager_info.terminate = true;
		pthread_mutex_unlock(&g_reconn_mutex);

		struct sockaddr_in serveraddr;
		int sd = socket(PF_INET, SOCK_DGRAM, 0);
		DEBUGASSERT(sd >= 0);

		bzero(&serveraddr, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		serveraddr.sin_port = htons(WIFIMGR_IPC_PORT);

		int res = sendto(sd, "term", 5, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
		if (res < 0) { // critical error
			WM_LOG_ERROR("send signal result(%d) (%d)\n", res, errno);
		}
		close(sd);
		pthread_join(g_manager_info.reconn_id, NULL);
		dhcpc_close_ipaddr();
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	} else if (msg->event == EVT_RECONNECT_CMD) {
		wifi_manager_ap_config_s *apinfo = (wifi_manager_ap_config_s *)msg->param;
		wifi_manager_result_e wres = _wifimgr_connect_ap(apinfo);
		if (wres == WIFI_MANAGER_ALREADY_CONNECTED) {
			/* some wifi drivers try to reconnect by itself.*/
			WM_LOG_VERBOSE("[WM] reconnect state already connected\n");
			wifimgr_call_cb(CB_STA_CONNECTED, NULL);
			WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECTED);
			return WIFI_MANAGER_ALREADY_CONNECTED;
		}
		if (wres != WIFI_MANAGER_SUCCESS) {
			WM_LOG_ERROR("[WM] Reconnect AP fail disconnect AP for initializing it(%d)\n", wres);
			_wifimgr_disconnect_ap();
			return wres;
		}
		WIFIMGR_SET_STATE(WIFIMGR_STA_RECONNECTING);
	} else if (msg->event == EVT_DEINIT_CMD) {
		WIFIMGR_CHECK_RESULT(_wifimgr_deinit(), "critical error\n", WIFI_MANAGER_FAIL);
		WIFIMGR_SEND_API_SIGNAL(msg->signal);
		WIFIMGR_SET_STATE(WIFIMGR_UNINITIALIZED);
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_FAIL;
	}
#else /* WIFIDRIVER_SUPPORT_AUTOCONNECT*/
	WM_LOG_VERBOSE("[WM] EXTERNAL AUTOCONNECT event status : %d\n", msg->event);
	if (msg->event == EVT_DISCONNECT_CMD) {
		dhcpc_close_ipaddr();
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	} else if (msg->event == EVT_STA_CONNECT_FAILED) {
		//nothing to do but to wait
	} else if (msg->event == EVT_STA_CONNECTED) {
		wifimgr_call_cb(CB_STA_CONNECTED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECTED);
	} else if (msg->event == EVT_DEINIT_CMD) {
		WIFIMGR_CHECK_RESULT(_wifimgr_deinit(), "critical error\n", WIFI_MANAGER_FAIL);
		WIFIMGR_SEND_API_SIGNAL(msg->signal);
		WIFIMGR_SET_STATE(WIFIMGR_UNINITIALIZED);
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_FAIL;
	}
#endif /* WIFIDRIVER_SUPPORT_AUTOCONNECT*/
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_reconnecting_state(wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
#if WIFIDRIVER_SUPPORT_AUTOCONNECT == 0
	if (msg->event == EVT_STA_CONNECT_FAILED) {
		WM_LOG_VERBOSE("[WM] reconnect fail\n");
		if (g_manager_info.conn_tries == g_manager_info.max_tries) {
			WM_LOG_ERROR("[WM] Stop to reconnect because of reaching to max tries\n");
			WIFIMGR_TERMINATE_RECONN_WORKER;
		} else {
			WIFIMGR_TRIGGER_RECONNECT_WORKER;
		}
		WIFIMGR_SET_STATE(WIFIMGR_STA_RECONNECT);
	} else if (msg->event == EVT_DISCONNECT_CMD) {
		WM_LOG_VERBOSE("[WM] disconnect\n");
		WIFIMGR_TERMINATE_RECONN_WORKER;
		WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECT_CANCEL);
	} else if (msg->event == EVT_STA_CONNECTED) {
		WIFIMGR_TERMINATE_RECONN_WORKER;
		wifimgr_call_cb(CB_STA_CONNECTED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECTED);
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_FAIL;
	}
#endif /* WIFIDRIVER_SUPPORT_AUTOCONNECT*/
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_connect_cancel_state(wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
#if WIFIDRIVER_SUPPORT_AUTOCONNECT == 0
	if (msg->event == EVT_STA_CONNECTED) {
		WIFIMGR_CHECK_RESULT(_wifimgr_disconnect_ap(), "critical error", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTING);
	} else if (msg->event == EVT_STA_CONNECT_FAILED) {
		dhcpc_close_ipaddr();
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	}
#endif /* WIFIDRIVER_SUPPORT_AUTOCONNECT*/
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_softap_state(wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	if (msg->event == EVT_SET_STA_CMD) {
		WIFIMGR_CHECK_RESULT(_wifimgr_stop_softap(), "critical error\n", WIFI_MANAGER_FAIL);
		WIFIMGR_CHECK_RESULT(_wifimgr_run_sta(), "critical error\n", WIFI_MANAGER_FAIL);
		WIFIMGR_SEND_API_SIGNAL(msg->signal);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	} else if (msg->event == EVT_SCAN_CMD) {
		WIFIMGR_CHECK_RESULT(_wifimgr_scan((wifi_manager_ap_config_s *)msg->param), "fail scan", WIFI_MANAGER_FAIL);
		WIFIMGR_STORE_PREV_STATE;
		WIFIMGR_SET_STATE(WIFIMGR_SCANNING);
#ifdef CONFIG_WIFIMGR_DISABLE_DHCPS
	} else if (msg->event == EVT_JOINED) {
#else
	/* wifi manager passes the callback after the dhcp server gives a station an IP address*/
	} else if (msg->event == EVT_DHCPS_ASSIGN_IP) {
		if (dhcps_add_node((dhcp_node_s *)msg->param) == DHCP_EXIST) {
			return WIFI_MANAGER_SUCCESS;
		}
#endif
		dhcps_inc_num();
		wifimgr_call_cb(CB_STA_JOINED, NULL);
	} else if (msg->event == EVT_LEFT) {
#ifndef CONFIG_WIFIMGR_DISABLE_DHCPS
		dhcps_del_node();
#endif
		dhcps_dec_num();
		wifimgr_call_cb(CB_STA_LEFT, NULL);
	} else if (msg->event == EVT_DEINIT_CMD) {
		WIFIMGR_CHECK_RESULT(_wifimgr_stop_softap(), "critical error\n", WIFI_MANAGER_FAIL);
		WIFIMGR_CHECK_RESULT(_wifimgr_deinit(), "critical error\n", WIFI_MANAGER_FAIL);
		WIFIMGR_SEND_API_SIGNAL(msg->signal);
		WIFIMGR_SET_STATE(WIFIMGR_UNINITIALIZED);
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_FAIL;
	}

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_scanning_state(wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	wifi_manager_result_e wret = WIFI_MANAGER_FAIL;
	if (msg->event == EVT_SCAN_DONE) {
		wifimgr_call_cb(CB_SCAN_DONE, msg->param);
		WIFIMGR_RESTORE_STATE;
		wret = WIFI_MANAGER_SUCCESS;
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
	}
	return wret;
}


/*
 * public
 */
wifi_manager_result_e wifimgr_handle_request(wifimgr_msg_s *msg)
{
	wifi_manager_result_e res = WIFI_MANAGER_FAIL;

	WM_LOG_HANDLER_START;
	res = g_handler[WIFIMGR_GET_STATE](msg);
#ifdef CONFIG_WIFIMGR_ERROR_REPORT
	_set_error_code(res);
#endif
	WM_LOG_VERBOSE("[WM] T%d <-- _handle_request\n", getpid());
	return res;
}
