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
#include <pthread.h>
#include <wifi_manager/wifi_manager.h>
#include <protocols/dhcpc.h>
#include <protocols/dhcpd.h>
#ifdef CONFIG_ENABLE_IOTIVITY
#include <mqueue.h>
#include <fcntl.h>
#endif
#include "wifi_utils.h"
#include "wifi_profile.h"

#define NUM_OF_STATE_HANDLER 10 // the number of states
enum _wifimgr_state {
	WIFIMGR_UNINITIALIZED,
	WIFIMGR_STA_DISCONNECTED,
	WIFIMGR_STA_DISCONNECTING,
	WIFIMGR_STA_CONNECTING,
	WIFIMGR_STA_CONNECTED,
	WIFIMGR_SOFTAP, // 5
	WIFIMGR_SCANNING,
	WIFIMGR_NONE, // it is used for prev state only
};
typedef enum _wifimgr_state _wifimgr_state_e;

enum _wifimgr_evt {
	EVT_INIT,				// Command to initialize WiFi Manager
	EVT_DEINIT,				// Command to Deinit WiFi Manager
	EVT_SET_SOFTAP,			// Command to set SoftAP
	EVT_SET_STA,			// Command to set STA mode
	EVT_CONNECT,			// Command to connect to a WiFi AP
	EVT_DISCONNECT,		// Command to Disconnect from a connected WiFi AP
	EVT_SCAN,				// Command to perform WiFi Scanning over WLAN channels
	EVT_GETINFO,			// Command to get WiFi Manager information
	EVT_STA_CONNECTED,		// Event that STA is connected
	EVT_STA_CONNECT_FAILED,	// Event that STA connect failed
	EVT_STA_DISCONNECTED,	// Event that external STA disconnected from WiFi AP
	EVT_JOINED,				// Event that new STA joined softAP
	EVT_DHCPD_GET_IP,		// Event that SoftAP got IP address
	EVT_LEFT,				// Event that external STA device left softAP
	EVT_SCAN_DONE,			// Event that WiFi scanning over WLAN channels is done
	EVT_NONE,
};
typedef enum _wifimgr_evt _wifimgr_evt_e;

enum _wifimgr_usr_cb_type {
	CB_STA_CONNECTED,
	CB_STA_CONNECT_FAILED,
	CB_STA_DISCONNECTED,
	CB_STA_JOINED,
	CB_STA_LEFT,
	CB_SCAN_DONE,
	CB_EVT_NONE = -1
};
typedef enum _wifimgr_usr_cb_type _wifimgr_usr_cb_type_e;

struct _wifimgr_msg {
	_wifimgr_evt_e event;
	void *param;
};
typedef struct _wifimgr_msg _wifimgr_msg_s;

struct _wifimgr_info {
	char ssid[32];             // SSID of Connected AP if mode is a station, SoftAP SSID if mode is a soft ap
	char mac_address[6];	   // MAC address of wifi interface
	int rssi;                  // It is only used for a station mode
	int num_sta;               // It is only used for a softap mode, it shows the number of stations connected
	uint32_t ip4_address;
	_wifimgr_state_e state;
	_wifimgr_state_e prev_state; // it is for returning to previous state after scanning is done
	// check_callback
	// 0: pass callback to user
	// 1: skip callback, if an internal error is happen, you don't need to pass callback to user
	int chk_cbk;
	pthread_mutex_t state_lock;
	pthread_mutex_t info_lock;
	wifi_manager_cb_s cb;
};
typedef struct _wifimgr_info _wifimgr_info_s;

#define CTRL_IFNAME "wl1" // ToDo: interface name should be configurable.

#define WIFIMGR_CHECK_STATE(s) ((s) != g_manager_info.state)
#define WIFIMGR_IS_STATE(s) ((s) == g_manager_info.state)
#define WIFIMGR_GET_STATE g_manager_info.state
#define WIFIMGR_SET_STATE(s) (g_manager_info.state = s)
#define WIFIMGR_SET_NO_CBK (g_manager_info.chk_cbk = 1)
#define WIFIMGR_RESET_CBK_CHK (g_manager_info.chk_cbk = 0)
#define WIFIMGR_CHECK_CBK (g_manager_info.chk_cbk == 0)

#define WIFIMGR_GET_PREVSTATE g_manager_info.prev_state
#define WIFIMGR_STORE_PREV_STATE (g_manager_info.prev_state = g_manager_info.state)
#define WIFIMGR_RESTORE_STATE								\
	do {													\
		g_manager_info.state = g_manager_info.prev_state;	\
		g_manager_info.prev_state = WIFIMGR_NONE;			\
	} while (0)

#define WIFIMGR_SET_SSID(s)							\
	do {											\
		strncpy(g_manager_info.ssid, s, strlen(s));	\
	} while (0)

#define WIFIMGR_SET_IP4ADDR(intf, ip, netmask, gateway)	\
	do {												\
		netlib_set_ipv4addr(intf, &ip);					\
		netlib_set_ipv4netmask(intf, &netmask);			\
		netlib_set_dripv4addr(intf, &gateway);			\
		g_manager_info.ip4_address = ip.s_addr;			\
	} while (0)

#define WIFIMGR_INC_NUM_CLIENT								\
	do {													\
		pthread_mutex_lock(&g_manager_info.info_lock);		\
		g_manager_info.num_sta++;							\
		pthread_mutex_unlock(&g_manager_info.info_lock);	\
	} while (0)

#define WIFIMGR_DEC_NUM_CLIENT								\
	do {													\
		pthread_mutex_lock(&g_manager_info.info_lock);		\
		g_manager_info.num_sta--;							\
		pthread_mutex_unlock(&g_manager_info.info_lock);	\
	} while (0)

#define LOCK_WIFIMGR pthread_mutex_lock(&g_manager_info.state_lock)
#define UNLOCK_WIFIMGR pthread_mutex_unlock(&g_manager_info.state_lock)

//
// Debugging
//
#define WM_LOG_START nvdbg("[WM] T%d\t%s:%d\n", getpid(), __FUNCTION__, __LINE__);
#define WM_LOG_HANDLER_START nvdbg("[WM] T%d %s:%d state(%d) evt(%d)\n", getpid(), __FUNCTION__, __LINE__, g_manager_info.state, msg->event);

/*
 * global variables
 */
static _wifimgr_info_s g_manager_info = {{0}, {0}, 0, 0, 0,
										 WIFIMGR_UNINITIALIZED, WIFIMGR_UNINITIALIZED, 0,
										 PTHREAD_MUTEX_INITIALIZER,
										 PTHREAD_MUTEX_INITIALIZER,
										 {NULL, NULL, NULL, NULL, NULL} };
#ifdef CONFIG_ENABLE_IOTIVITY
static mqd_t g_dw_nwevent_mqfd;
#endif

/*
 * internal functions
 */
//
// wifi utils callbacks registered
//
static void _wifi_utils_connect_event(wifi_utils_result_e result, void *arg);
static void _wifi_utils_disconnect_event(void *arg);
static void _wifi_utils_join_event(void *arg); // it needs arg which contains sta infomation joined
static void _wifi_utils_leave_event(void *arg); // it needs arg which contains a sta info left
static void _wifi_utils_scan_done(wifi_utils_result_e result, wifi_utils_scan_list_s *slist, void *arg);
static void _wifi_dhcpd_event(void); // join event from dhcpd

//
// internal APIs
//
static wifi_manager_result_e _convert_scan_info(wifi_manager_scan_info_s **wm_scan_list, wifi_utils_scan_list_s *wu_scan_list);
static void _free_scan_info(wifi_manager_scan_info_s *wm_scan_list);
static void _convert_state_to_info(connect_status_e *conn, wifi_manager_mode_e *mode, _wifimgr_state_e state);
static wifi_manager_result_e _start_dhcpd(void);
static wifi_manager_result_e _stop_dhcpd(void);
static wifi_manager_result_e _get_ipaddr_dhcp(void);
static wifi_manager_result_e _wifimgr_deinit(void);
static wifi_manager_result_e _wifimgr_run_sta(void);
static wifi_manager_result_e _wifimgr_connect_ap(wifi_manager_ap_config_s *config);
static wifi_manager_result_e _wifimgr_disconnect_ap(void);
static wifi_manager_result_e _wifimgr_run_softap(wifi_manager_softap_config_s *config);
static wifi_manager_result_e _wifimgr_stop_softap(void);
static wifi_manager_result_e _wifimgr_scan(void);

//
// user callback handler
//
static void _handle_user_cb(_wifimgr_usr_cb_type_e evt, void *arg);

//
// functions managing a state machine
//
static wifi_manager_result_e _handle_request(_wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_uninitialized_state(_wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_disconnected_state(_wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_disconnecting_state(_wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_connecting_state(_wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_connected_state(_wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_softap_state(_wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_scanning_state(_wifimgr_msg_s *msg);


typedef wifi_manager_result_e (*wifimgr_handler)(_wifimgr_msg_s *msg);
/*
 * g_handler should be matched to _wifimgr_state
 */
wifimgr_handler g_handler[NUM_OF_STATE_HANDLER] = {
	_handler_on_uninitialized_state,
	_handler_on_disconnected_state,
	_handler_on_disconnecting_state,
	_handler_on_connecting_state,
	_handler_on_connected_state,
	_handler_on_softap_state,
	_handler_on_scanning_state,
};


#ifdef CONFIG_ENABLE_IOTIVITY
void __tizenrt_manual_linkset(const char *msg)
{
	int ret = mq_send(g_dw_nwevent_mqfd, msg, 3, 42);
	if (ret < 0) {
		ndbg("[WM] send message fail\n");
		return;
	}
}
#endif


wifi_manager_result_e
_convert_scan_info(wifi_manager_scan_info_s **wm_scan_list, wifi_utils_scan_list_s *wu_scan_list)
{
	wifi_manager_scan_info_s *cur = NULL, *prev = NULL;
	wifi_utils_scan_list_s *iter = wu_scan_list;
	while (iter) {
		cur = (wifi_manager_scan_info_s *)calloc(1, sizeof(wifi_manager_scan_info_s));
		if (!cur) {
			_free_scan_info(*wm_scan_list);
			return WIFI_MANAGER_FAIL;
		}
		cur->rssi = iter->ap_info.rssi;
		cur->channel = iter->ap_info.channel;
		cur->phy_mode = iter->ap_info.phy_mode;
		strncpy(cur->ssid, (char *)iter->ap_info.ssid, 32);
		strncpy(cur->bssid, (char *)iter->ap_info.bssid, 17);

		if (!prev) {
			*wm_scan_list = cur;
		} else {
			prev->next = cur;
		}
		prev = cur;
		iter = iter->next;
	}
	return WIFI_MANAGER_SUCCESS;
}


void _free_scan_info(wifi_manager_scan_info_s *scan_list)
{
	wifi_manager_scan_info_s *iter = scan_list, *prev = NULL;
	while (iter) {
		prev = iter;
		iter = iter->next;
		free(prev);
	}
}


void _convert_state_to_info(connect_status_e *conn, wifi_manager_mode_e *mode, _wifimgr_state_e state)
{
	switch (state) {
	case WIFIMGR_UNINITIALIZED:
		*mode = WIFI_NONE;
		*conn = STATUS_UNKNOWN;
		break;
	case WIFIMGR_STA_DISCONNECTED:
	case WIFIMGR_STA_DISCONNECTING:
	case WIFIMGR_STA_CONNECTING:
		*mode = STA_MODE;
		*conn = AP_DISCONNECTED;
		break;
	case WIFIMGR_STA_CONNECTED:
		*mode = STA_MODE;
		*conn = AP_CONNECTED;
		break;
	case WIFIMGR_SOFTAP:
		*mode = SOFTAP_MODE;
		if (g_manager_info.num_sta > 0) {
			*conn = CLIENT_CONNECTED;
		} else {
			*conn = CLIENT_DISCONNECTED;
		}
		break;
	default:
		// CRITICAL ERROR
		break;
	}
}


wifi_manager_result_e _start_dhcpd(void)
{
	struct in_addr ip = {.s_addr = 0x012fa8c0 };
	struct in_addr netmask = {.s_addr = 0x00ffffff};
	struct in_addr gw = {.s_addr = 0x012fa8c0};
	WIFIMGR_SET_IP4ADDR(CTRL_IFNAME, ip, netmask, gw);

	if (dhcpd_start(CTRL_IFNAME, _wifi_dhcpd_event) < 0) {
		ndbg("[WM] DHCP Server - started fail\n");
		return WIFI_MANAGER_FAIL;
	}
#ifdef CONFIG_ENABLE_IOTIVITY
	__tizenrt_manual_linkset("gen");
#endif
	nvdbg("[WM] DHCP Server - started success\n");
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _stop_dhcpd(void)
{
	struct in_addr in = { .s_addr = INADDR_NONE };
	WIFIMGR_SET_IP4ADDR(CTRL_IFNAME, in, in, in);

	dhcpd_stop();

#ifdef CONFIG_ENABLE_IOTIVITY
	__tizenrt_manual_linkset("del");
#endif
	return WIFI_UTILS_SUCCESS;
}


wifi_manager_result_e _get_ipaddr_dhcp(void)
{
	struct dhcpc_state state;
	int ret;
	void *dhcp_hnd = NULL;

	dhcp_hnd = dhcpc_open(CTRL_IFNAME);
	if (dhcp_hnd == NULL) {
		ndbg("[WM] Invalid dhcp handle\n");
		return WIFI_MANAGER_FAIL;
	}
	ret = dhcpc_request(dhcp_hnd, &state);
	if (ret != OK) {
		ndbg("[WM] get IP address fail\n");
		dhcpc_close(dhcp_hnd);
		return WIFI_MANAGER_FAIL;
	}
	WIFIMGR_SET_IP4ADDR(CTRL_IFNAME, state.ipaddr, state.netmask, state.default_router);
#ifdef CONFIG_ENABLE_IOTIVITY
	__tizenrt_manual_linkset("gen");
#endif
	nvdbg("[WM] IP address : %s ----\n", inet_ntoa(state.ipaddr));
	dhcpc_close(dhcp_hnd);

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_deinit(void)
{
	WM_LOG_START;
	wifi_utils_result_e res = wifi_utils_deinit();
	if (res != WIFI_UTILS_SUCCESS) {
		ndbg("[WM] wifi_utils_deinit fail");
		return WIFI_MANAGER_FAIL;
	}
#ifdef CONFIG_ENABLE_IOTIVITY
	int ret = mq_close(g_dw_nwevent_mqfd);
	if (ret < 0) {
		ndbg("[WM] close message queue fail\n");
		return WIFI_MANAGER_FAIL;
	}
#endif
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_run_sta(void)
{
	WM_LOG_START;
	wifi_utils_result_e res = wifi_utils_start_sta();
	if (res != WIFI_UTILS_SUCCESS) {
		ndbg("[WM] Starting STA failed.");
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_connect_ap(wifi_manager_ap_config_s *config)
{
	WM_LOG_START;
	wifi_utils_ap_config_s util_config;
	strncpy(util_config.ssid, config->ssid, config->ssid_length + 1);
	util_config.ssid_length = config->ssid_length;
	strncpy(util_config.passphrase, config->passphrase, config->passphrase_length + 1);
	util_config.passphrase_length = config->passphrase_length;
	util_config.ap_auth_type = config->ap_auth_type;
	util_config.ap_crypto_type = config->ap_crypto_type;

	wifi_utils_result_e result = wifi_utils_connect_ap(&util_config, NULL);
	if (result != WIFI_UTILS_SUCCESS) {
		ndbg("[WM] Wifi AP connect fail");
		return WIFI_MANAGER_FAIL;
	}

	strncpy(g_manager_info.ssid, config->ssid, config->ssid_length + 1);

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_disconnect_ap(void)
{
	WM_LOG_START;
	wifi_utils_result_e res = wifi_utils_disconnect_ap(NULL);
	if (res != WIFI_UTILS_SUCCESS) {
		ndbg("[WM] disconnect to ap fail\n");
		return WIFI_MANAGER_FAIL;
	}
#ifdef CONFIG_ENABLE_IOTIVITY
	__tizenrt_manual_linkset("del");
#endif
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_run_softap(wifi_manager_softap_config_s *config)
{
	WM_LOG_START;
	wifi_utils_softap_config_s softap_config;

	softap_config.channel = config->channel;
	softap_config.ap_crypto_type = WIFI_UTILS_CRYPTO_AES;
	softap_config.ap_auth_type = WIFI_UTILS_AUTH_WPA2_PSK;
	softap_config.ssid_length = strlen(config->ssid);
	softap_config.passphrase_length = strlen(config->passphrase);
	strncpy(softap_config.ssid, config->ssid, softap_config.ssid_length + 1);
	strncpy(softap_config.passphrase, config->passphrase, softap_config.passphrase_length + 1);

	wifi_utils_result_e res = wifi_utils_start_softap(&softap_config);
	if (res != WIFI_UTILS_SUCCESS) {
		ndbg("[WM] Starting softap mode failed.");
		return WIFI_MANAGER_FAIL;
	}

	if (_start_dhcpd() != WIFI_MANAGER_SUCCESS) {
		ndbg("[WM] Starting DHCP server failed.\n");
		return WIFI_MANAGER_FAIL;
	}

	/* update wifi_manager_info */
	strncpy(g_manager_info.ssid, config->ssid, softap_config.ssid_length + 1);
	g_manager_info.num_sta = 0;

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_stop_softap(void)
{
	WM_LOG_START;
	wifi_utils_result_e res = wifi_utils_stop_softap();
	if (res != WIFI_UTILS_SUCCESS) {
		ndbg("[WM] Stoping softap failed.");
		return WIFI_MANAGER_FAIL;
	}

	wifi_manager_result_e mres = _stop_dhcpd();
	if (mres != WIFI_MANAGER_SUCCESS) {
		ndbg("[WM] Stoping softap DHCP server failed.");
		return WIFI_MANAGER_FAIL;
	}

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_scan(void)
{
	WM_LOG_START;
	wifi_utils_result_e res = wifi_utils_scan_ap(NULL);
	if (res != WIFI_UTILS_SUCCESS) {
		ndbg("[WM] request scan to wifi utils is fail\n");
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}


void _wifi_utils_connect_event(wifi_utils_result_e result, void *arg)
{
	WM_LOG_START;
	_wifimgr_msg_s msg = {EVT_STA_CONNECTED, NULL};
	if (result == WIFI_UTILS_FAIL) {
		msg.event = EVT_STA_CONNECT_FAILED;
	}
	wifi_manager_result_e res = _handle_request(&msg);
	if (res != WIFI_MANAGER_SUCCESS) {
		ndbg("[WM] handle connect_event fail\n");
	}
}


void _wifi_utils_disconnect_event(void *arg)
{
	WM_LOG_START;
	_wifimgr_msg_s msg = {EVT_STA_DISCONNECTED, NULL};
	wifi_manager_result_e res = _handle_request(&msg);
	if (res != WIFI_MANAGER_SUCCESS) {
		ndbg("[WM] handle connect_event fail\n");
	}
}


void _wifi_utils_join_event(void *arg)
{
	WM_LOG_START;
	_wifimgr_msg_s msg = {EVT_JOINED, NULL};
	wifi_manager_result_e res = _handle_request(&msg);
	if (res != WIFI_MANAGER_SUCCESS) {
		ndbg("[WM] handle connect_event fail\n");
	}
}


void _wifi_utils_leave_event(void * arg)
{
	WM_LOG_START;
	_wifimgr_msg_s msg = {EVT_LEFT, NULL};
	wifi_manager_result_e res = _handle_request(&msg);
	if (res != WIFI_MANAGER_SUCCESS) {
		ndbg("[WM] handle connect_event fail\n");
	}
}


void _wifi_utils_scan_done(wifi_utils_result_e result, wifi_utils_scan_list_s *slist, void *arg)
{
	WM_LOG_START;
	_wifimgr_msg_s msg = {EVT_SCAN_DONE, (void *)slist};
	wifi_manager_result_e res = _handle_request(&msg);
	if (res != WIFI_MANAGER_SUCCESS) {
		ndbg("[WM] handle scan done event fail\n");
	}
}

void _wifi_dhcpd_event(void)
{
	WM_LOG_START;
	_wifimgr_msg_s msg = {EVT_DHCPD_GET_IP, NULL};
	wifi_manager_result_e res = _handle_request(&msg);
	if (res != WIFI_MANAGER_SUCCESS) {
		ndbg("[WM] handle dhcpd_event fail\n");
	}

	return ;
}

/*
 * State Functions
 */
wifi_manager_result_e _handler_on_uninitialized_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	_wifimgr_evt_e evt = msg->event;
	if (evt != EVT_INIT) {
		return WIFI_MANAGER_FAIL;
	}

#ifdef CONFIG_ENABLE_IOTIVITY
	struct mq_attr lq_attr;
	lq_attr.mq_maxmsg = 10;
	lq_attr.mq_msgsize = 4;
	lq_attr.mq_flags = 0;
	g_dw_nwevent_mqfd = mq_open("netlink_evtq", O_WRONLY | O_CREAT, 0666, &lq_attr);

	if (g_dw_nwevent_mqfd == (mqd_t)ERROR) {
		ndbg("[WM] iotivity connect event message queue init fail");
		return WIFI_MANAGER_FAIL;
	}
#endif
	wifi_utils_result_e wres = wifi_utils_init();
	if (wres != WIFI_UTILS_SUCCESS) {
		ndbg("[WM] wifi_utils_init fail(%d)", wres);
		return WIFI_MANAGER_FAIL;
	}

	wres = wifi_profile_init();
	if (wres != WIFI_UTILS_SUCCESS) {
		ndbg("[WM] wifi_profile init fail(%d)", wres);
		return WIFI_MANAGER_FAIL;
	}

	wifi_manager_cb_s *cb = (wifi_manager_cb_s *)msg->param;
	wifi_utils_cb_s util_cb = {
		_wifi_utils_connect_event,
		_wifi_utils_disconnect_event,
		_wifi_utils_join_event,
		_wifi_utils_leave_event,
		_wifi_utils_scan_done,
	};

	wifi_utils_register_callback(&util_cb);
	wifi_utils_info info;
	wres = wifi_utils_get_info(&info);
	if (wres != WIFI_UTILS_SUCCESS) {
		ndbg("[WM] wifi_utils_get_info fail");
		wres = wifi_utils_deinit();
		if (wres != WIFI_UTILS_SUCCESS) {
			// CRITICAL ERROR
		}
		return WIFI_MANAGER_FAIL;
	}

	g_manager_info.cb = *cb;
	memcpy(g_manager_info.mac_address, info.mac_address, 6);

	WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_disconnected_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;

	wifi_manager_result_e res = WIFI_MANAGER_FAIL;
	if (msg->event == EVT_CONNECT) {
		res = _wifimgr_connect_ap((wifi_manager_ap_config_s *)msg->param);
		if (res != WIFI_MANAGER_SUCCESS) {
			return WIFI_MANAGER_FAIL;
		}
		WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECTING);
	} else if (msg->event == EVT_DEINIT) {
		res = _wifimgr_deinit();
		if (res != WIFI_MANAGER_SUCCESS) {
			return WIFI_MANAGER_FAIL;
		}
		WIFIMGR_SET_STATE(WIFIMGR_UNINITIALIZED);
	} else if (msg->event == EVT_SET_SOFTAP) {
		res = _wifimgr_run_softap((wifi_manager_softap_config_s *)msg->param);
		if (res != WIFI_MANAGER_SUCCESS) {
			return WIFI_MANAGER_FAIL;
		}
		WIFIMGR_SET_STATE(WIFIMGR_SOFTAP);
	} else if (msg->event == EVT_SCAN) {
		res = _wifimgr_scan();
		if (res != WIFI_MANAGER_SUCCESS) {
			ndbg("fail scan\n");
			return WIFI_MANAGER_FAIL;
		}
		WIFIMGR_STORE_PREV_STATE;
		WIFIMGR_SET_STATE(WIFIMGR_SCANNING);
	} else {
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_disconnecting_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	if (msg->event == EVT_STA_DISCONNECTED) {
		if (WIFIMGR_CHECK_CBK) {
			_handle_user_cb(CB_STA_DISCONNECTED, NULL);
		} else {
			WIFIMGR_RESET_CBK_CHK;
		}
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	} else {
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_connecting_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	wifi_manager_result_e res;
	if (msg->event == EVT_STA_CONNECTED) {
		res = _get_ipaddr_dhcp();
		if (res != WIFI_MANAGER_SUCCESS) {
			_handle_user_cb(CB_STA_CONNECT_FAILED, NULL);
			res = _wifimgr_disconnect_ap(); // without callback
			if (res != WIFI_MANAGER_SUCCESS) {
				// this should succeed
			}
			WIFIMGR_SET_NO_CBK;
			WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTING);
			return WIFI_MANAGER_FAIL;
		}
		_handle_user_cb(CB_STA_CONNECTED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECTED);
	} else if (msg->event == EVT_STA_CONNECT_FAILED) {
		_handle_user_cb(CB_STA_CONNECT_FAILED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	} else {
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_connected_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	wifi_manager_result_e res;
	if (msg->event == EVT_DISCONNECT) {
		// call disconnect_ap
		res = _wifimgr_disconnect_ap();
		if (res != WIFI_MANAGER_SUCCESS) {
			// this should succeed
		}
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTING);
	} else if (msg->event == EVT_STA_DISCONNECTED) {
		// ap is disconnected from remote
		_handle_user_cb(CB_STA_DISCONNECTED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	} else if (msg->event == EVT_SET_SOFTAP) {
		res = _wifimgr_run_softap((wifi_manager_softap_config_s *)msg->param);
		if (res != WIFI_MANAGER_SUCCESS) {
			res = _wifimgr_disconnect_ap();
			if (res != WIFI_MANAGER_SUCCESS) {
				// CRITICAL ERROR
			}
			WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTING);
		}
		WIFIMGR_SET_STATE(WIFIMGR_SOFTAP);
	} else if (msg->event == EVT_DEINIT) {
		// do I need to disconnect?
		res = _wifimgr_deinit();
		if (res != WIFI_MANAGER_SUCCESS) {
			// this should succeed
		}
		WIFIMGR_SET_STATE(WIFIMGR_UNINITIALIZED);
	} else if (msg->event == EVT_SCAN) {
		res = _wifimgr_scan();
		if (res != WIFI_MANAGER_SUCCESS) {
			ndbg("fail scan\n");
			return WIFI_MANAGER_FAIL;
		}
		WIFIMGR_STORE_PREV_STATE;
		WIFIMGR_SET_STATE(WIFIMGR_SCANNING);
	} else {
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_softap_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	wifi_manager_result_e res = WIFI_MANAGER_FAIL;
	if (msg->event == EVT_SET_STA) {
		res = _wifimgr_stop_softap();
		if (res != WIFI_MANAGER_SUCCESS) {
			// CRITICAL ERROR(recovery fail)
		}
		//sleep(5);
		res = _wifimgr_run_sta();
		if (res != WIFI_MANAGER_SUCCESS) {
			// CRITICAL ERROR(recovery fail)
		}
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	} else if (msg->event == EVT_SCAN) {
		res = _wifimgr_scan();
		if (res != WIFI_MANAGER_SUCCESS) {
			ndbg("fail scan\n");
			return WIFI_MANAGER_FAIL;
		}
		WIFIMGR_STORE_PREV_STATE;
		WIFIMGR_SET_STATE(WIFIMGR_SCANNING);
	} else if (msg->event == EVT_JOINED) {
		// wifi manager passes the callback after the dhcp server gives a station an IP address
	} else if (msg->event == EVT_DHCPD_GET_IP) {
		WIFIMGR_INC_NUM_CLIENT;
		_handle_user_cb(CB_STA_JOINED, NULL);
	} else if (msg->event == EVT_LEFT) {
		WIFIMGR_DEC_NUM_CLIENT;
		_handle_user_cb(CB_STA_LEFT, NULL);
	} else if (msg->event == EVT_DEINIT) {
		res = _wifimgr_stop_softap();
		if (res != WIFI_MANAGER_SUCCESS) {
			// CRITICAL ERROR
		}
		res = _wifimgr_deinit();
		if (res != WIFI_MANAGER_SUCCESS) {
			// CRITICAL ERROR
		}
		WIFIMGR_SET_STATE(WIFIMGR_UNINITIALIZED);
	} else {
		return WIFI_MANAGER_FAIL;
	}

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_scanning_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	if (msg->event == EVT_SCAN_DONE) {
		_handle_user_cb(CB_SCAN_DONE, msg->param);
		WIFIMGR_RESTORE_STATE;
		return WIFI_MANAGER_SUCCESS;
	}
	return WIFI_MANAGER_FAIL;
}


void _handle_user_cb(_wifimgr_usr_cb_type_e evt, void *arg)
{
	WM_LOG_START;
	wifi_manager_cb_s *cbk = &g_manager_info.cb;
	switch (evt) {
	case CB_STA_CONNECTED:
		nvdbg("call sta connect success event\n");
		cbk->sta_connected(WIFI_MANAGER_SUCCESS);
		break;
	case CB_STA_CONNECT_FAILED:
		nvdbg("call sta connect fail event\n");
		cbk->sta_connected(WIFI_MANAGER_FAIL);
		break;
	case CB_STA_DISCONNECTED:
		nvdbg("call sta disconnect event\n");
		cbk->sta_disconnected();
		break;
	case CB_STA_JOINED:
		nvdbg("call sta join event\n");
		cbk->softap_sta_joined();
		break;
	case CB_STA_LEFT:
		nvdbg("call sta leave event\n");
		cbk->softap_sta_left();
		break;
	case CB_SCAN_DONE:
		nvdbg("call sta scan event\n");
		// convert scan data.
		wifi_manager_scan_info_s *info = NULL;
		wifi_utils_scan_list_s *list = (wifi_utils_scan_list_s *)arg;
		if (list) {
			wifi_manager_result_e res = _convert_scan_info(&info, (wifi_utils_scan_list_s *)arg);
			if (res != WIFI_MANAGER_SUCCESS) {
				ndbg("parsing error\n");
			}
			cbk->scan_ap_done(&info, WIFI_SCAN_SUCCESS);
			_free_scan_info(info);
		} else {
			cbk->scan_ap_done(NULL, WIFI_SCAN_FAIL);
		}
		break;
	default:
		ndbg("[WM] Invalid State\n");
		break;
	}
}


wifi_manager_result_e _handle_request(_wifimgr_msg_s *msg)
{
	wifi_manager_result_e res = WIFI_MANAGER_FAIL;

	LOCK_WIFIMGR;
	WM_LOG_HANDLER_START;
	res = g_handler[WIFIMGR_GET_STATE](msg);
	if (res != WIFI_MANAGER_SUCCESS) {
		ndbg("[WM] Wi-Fi manager fail: state error\n");
	}
	UNLOCK_WIFIMGR;
	nvdbg("[WM] T%d <-- _handle_request\n", getpid());
	return res;
}


/**
 * public APIs
 */
wifi_manager_result_e wifi_manager_init(wifi_manager_cb_s *wmcb)
{
	if (!wmcb) {
		return WIFI_MANAGER_INVALID_ARGS;
	}
	_wifimgr_msg_s msg = {EVT_INIT, (void *)wmcb};
	wifi_manager_result_e res = _handle_request(&msg);
	return res;
}


wifi_manager_result_e wifi_manager_deinit(void)
{
	_wifimgr_msg_s msg = {EVT_DEINIT, NULL};
	wifi_manager_result_e res = _handle_request(&msg);
	return res;
}


wifi_manager_result_e wifi_manager_set_mode(wifi_manager_mode_e mode, wifi_manager_softap_config_s *config)
{
	if (mode != STA_MODE && mode != SOFTAP_MODE) {
		return WIFI_MANAGER_INVALID_ARGS;
	}
	if (mode == SOFTAP_MODE && !config) {
		return WIFI_MANAGER_INVALID_ARGS;
	}

	_wifimgr_msg_s msg = {EVT_SET_STA, NULL};
	if (mode == SOFTAP_MODE) {
		msg.event = EVT_SET_SOFTAP;
		msg.param = (void *)config;
	}
	wifi_manager_result_e res = _handle_request(&msg);
	return res;
}


wifi_manager_result_e wifi_manager_get_info(wifi_manager_info_s *info)
{
	LOCK_WIFIMGR;
	uint8_t *ip = (uint8_t *)&g_manager_info.ip4_address;
	snprintf(info->ip4_address, 18, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	memcpy(info->ssid, g_manager_info.ssid, 32);
	memcpy(info->mac_address, g_manager_info.mac_address, 6);
	info->rssi = g_manager_info.rssi;
	if (WIFIMGR_IS_STATE(WIFIMGR_SCANNING)) {
		_convert_state_to_info(&info->status, &info->mode, WIFIMGR_GET_PREVSTATE);
	} else {
		_convert_state_to_info(&info->status, &info->mode, WIFIMGR_GET_STATE);
	}
	UNLOCK_WIFIMGR;
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e wifi_manager_connect_ap(wifi_manager_ap_config_s *config)
{
	if (!config) {
		return WIFI_MANAGER_INVALID_ARGS;
	}

	if ((config->ssid_length > 31) || (config->passphrase_length > 63)) {
		ndbg("[WM] AP configuration fails: too long ssid or passphrase\n");
		ndbg("[WM] Make sure that length of SSID < 32 and length of passphrase < 64\n");
		return WIFI_MANAGER_INVALID_ARGS;
	}
	_wifimgr_msg_s msg = {EVT_CONNECT, (void *)config};
	wifi_manager_result_e res = _handle_request(&msg);
	return res;
}


wifi_manager_result_e wifi_manager_disconnect_ap(void)
{
	_wifimgr_msg_s msg = {EVT_DISCONNECT, NULL};
	wifi_manager_result_e res = _handle_request(&msg);
	return res;
}


wifi_manager_result_e wifi_manager_scan_ap(void)
{
	_wifimgr_msg_s msg = {EVT_SCAN, NULL};
	wifi_manager_result_e res = _handle_request(&msg);
	return res;
}


wifi_manager_result_e wifi_manager_save_config(wifi_manager_ap_config_s *config)
{
	wifi_utils_result_e res = wifi_profile_write(config);
	if (res != WIFI_UTILS_SUCCESS) {
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_get_config(wifi_manager_ap_config_s *config)
{
	wifi_utils_result_e res = wifi_profile_read(config);
	if (res != WIFI_UTILS_SUCCESS) {
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e wifi_manager_remove_config(void)
{
	wifi_utils_result_e res = wifi_profile_reset();
	if (res != WIFI_UTILS_SUCCESS) {
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}
