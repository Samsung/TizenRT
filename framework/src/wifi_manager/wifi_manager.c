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
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <wifi_manager/wifi_manager.h>
#include <protocols/dhcpc.h>
#include <protocols/dhcpd.h>
#include <netutils/netlib.h>
#ifdef CONFIG_ENABLE_IOTIVITY
#include <mqueue.h>
#include <fcntl.h>
#endif
#include "wifi_utils.h"
#include "wifi_profile.h"

#ifdef CONFIG_WIFIMGR_ERROR_REPORT
#include <error_report/error_report.h>
#define WIFIADD_ERR_RECORD(reason_code)	ERR_DATA_CREATE(ERRMOD_WIFI_MANAGER, reason_code)
#else
#define WIFIADD_ERR_RECORD(reason_code)
#endif

#undef ndbg
#define ndbg printf
#undef nvdbg
#define nvdbg printf
#define WIFIMGR_NUM_CALLBACKS 3

enum _wifimgr_state {
	WIFIMGR_UNINITIALIZED,
	WIFIMGR_STA_DISCONNECTED,
	WIFIMGR_STA_DISCONNECTING,
	WIFIMGR_STA_CONNECTING,
	WIFIMGR_STA_CONNECTED,
	WIFIMGR_STA_RECONNECT, // 5
	WIFIMGR_STA_RECONNECTING,
	WIFIMGR_STA_CONNECT_CANCEL,
	WIFIMGR_SOFTAP_DISCONNECTING_STA,
	WIFIMGR_SOFTAP,
	WIFIMGR_SCANNING,
	WIFIMGR_NONE, // it is used for prev state only
	WIFIMGR_STATE_MAX,
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
	EVT_RECONNECT,		  // Command to reconnect to WiFi AP. If we use EVT_CONNECT, then we can't distinguish it's from application request
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
	CB_STA_RECONNECTED,
	CB_STA_JOINED,
	CB_STA_LEFT,
	CB_SCAN_DONE,
	CB_SOFTAP_DONE, /* This callback does not exist, but is used for stats management. */
	CB_MAX,
	CB_EVT_NONE = -1
};
typedef enum _wifimgr_usr_cb_type _wifimgr_usr_cb_type_e;

struct _wifimgr_conn_info_msg {
	wifi_manager_ap_config_s *config;
	wifi_manager_reconnect_config_s *conn_config;
	int fd;
};
typedef struct _wifimgr_conn_info_msg _wifimgr_conn_info_msg_s;
struct _wifimgr_msg {
	_wifimgr_evt_e event;
	void *param;
};
typedef struct _wifimgr_msg _wifimgr_msg_s;

struct _wifimgr_info {
	char ssid[WIFIMGR_SSID_LEN + 1];             // SSID of Connected AP if mode is a station
	char softap_ssid[WIFIMGR_SSID_LEN + 1];      // SoftAP SSID if mode is a soft ap
	char mac_address[WIFIMGR_MACADDR_LEN];	   // MAC address of wifi interface
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
	pthread_mutex_t softap_lock;
	pthread_cond_t softap_signal;
	wifi_manager_cb_s *cb[WIFIMGR_NUM_CALLBACKS];

	//
	// Reconnect
	//
	pthread_t reconn_id; // thread id of reconn_worker: it is used for terminating _reconn_worker
	wifi_manager_ap_config_s connected_ap;
	wifi_manager_reconnect_config_s conn_config;
	int terminate;	// it is protected by g_reconn_mutex to sync between the callback task and reconn_worker
	int conn_tries; // to do: set  it by Kconfig
	int max_tries;
	wifi_manager_softap_config_s softap_config;

	uint16_t stats[CB_MAX];
};
typedef struct _wifimgr_info _wifimgr_info_s;

#define WIFIMGR_SOFTAP_IFNAME CONFIG_WIFIMGR_SOFTAP_IFNAME
#define WIFIMGR_STA_IFNAME CONFIG_WIFIMGR_STA_IFNAME

#define WIFIMGR_MAX_CONN_RETRIES 10
#define WIFIMGR_IPC_PORT 9098

/* Check if external wifi driver supports auto(re)connect */
#ifndef CONFIG_DISABLE_EXTERNAL_AUTOCONNECT
#define WIFIDRIVER_SUPPORT_AUTOCONNECT 1
#else
#define WIFIDRIVER_SUPPORT_AUTOCONNECT 0
#endif

#define WIFIMGR_CHECK_STATE(s) ((s) != g_manager_info.state)
#define WIFIMGR_IS_STATE(s) ((s) == g_manager_info.state)
#define WIFIMGR_GET_STATE g_manager_info.state
#define WIFIMGR_SET_STATE(s)				\
	do {									\
		g_manager_info.state = s;			\
	} while (0)
#define WIFIMGR_STATS_INC(s)				\
	do {									\
		g_manager_info.stats[s]++;			\
	} while (0)
#define WIFIMGR_SET_NO_CBK (g_manager_info.chk_cbk = 1)
#define WIFIMGR_RESET_CBK_CHK (g_manager_info.chk_cbk = 0)
#define WIFIMGR_CHECK_CBK (g_manager_info.chk_cbk == 0)

#define WIFIMGR_SOFTAP_WAIT_CALLBACK                                                                    \
	do {                                                                                            \
		pthread_mutex_lock(&g_manager_info.softap_lock);					\
		pthread_cond_wait(&g_manager_info.softap_signal, &g_manager_info.softap_lock);          \
		pthread_mutex_unlock(&g_manager_info.softap_lock);	                                \
		nvdbg("[WM] T%d wait disconnect callback\n", getpid());					\
	} while (0)

#define WIFIMGR_SOFTAP_CALLBACK_RECEIVED					\
	do {									\
		pthread_mutex_lock(&g_manager_info.softap_lock);		\
		pthread_cond_signal(&g_manager_info.softap_signal);             \
		pthread_mutex_unlock(&g_manager_info.softap_lock);	        \
		nvdbg("[WM] T%d received disconnect callback\n", getpid());	\
	} while (0)

#define WIFIMGR_GET_PREVSTATE g_manager_info.prev_state
#define WIFIMGR_STORE_PREV_STATE (g_manager_info.prev_state = g_manager_info.state)
#define WIFIMGR_RESTORE_STATE								\
	do {										\
		g_manager_info.state = g_manager_info.prev_state;	                \
		g_manager_info.prev_state = WIFIMGR_NONE;			        \
	} while (0)

#define WIFIMGR_SET_SSID(s)					\
	do {							\
		strncpy(g_manager_info.ssid, s, WIFIMGR_SSID_LEN);	\
		g_manager_info.ssid[WIFIMGR_SSID_LEN] = '\0';          \
	} while (0)

#define WIFIMGR_SET_SOFTAP_SSID(s)					\
	do {							\
		strncpy(g_manager_info.softap_ssid, s, WIFIMGR_SSID_LEN);	\
		g_manager_info.softap_ssid[WIFIMGR_SSID_LEN] = '\0';          \
	} while (0)

#define WIFIMGR_COPY_SOFTAP_CONFIG(dest, src)							\
	do {											\
		(dest).channel = (src)->channel;						\
		strncpy((dest).ssid, (src)->ssid, WIFIMGR_SSID_LEN);				\
		(dest).ssid[WIFIMGR_SSID_LEN] = '\0';                                        \
		strncpy((dest).passphrase, (src)->passphrase, WIFIMGR_PASSPHRASE_LEN);	\
		(dest).passphrase[WIFIMGR_PASSPHRASE_LEN] = '\0';                            \
	} while (0)

#define WIFIMGR_COPY_AP_INFO(dest, src)								\
	do {											\
		(dest).ssid_length = (src).ssid_length;						\
		(dest).passphrase_length = (src).passphrase_length;				\
		strncpy((dest).ssid, (src).ssid, WIFIMGR_SSID_LEN);		                \
		(dest).ssid[WIFIMGR_SSID_LEN] = '\0';                                          \
		strncpy((dest).passphrase, (src).passphrase, WIFIMGR_PASSPHRASE_LEN);          \
		(dest).passphrase[WIFIMGR_PASSPHRASE_LEN] = '\0';                              \
		(dest).ap_auth_type = (src).ap_auth_type;					\
		(dest).ap_crypto_type = (src).ap_crypto_type;					\
	} while (0)

#define WIFIMGR_COPY_RECONN_INFO(dest, src)			\
	do {							\
		(dest).type = (src).type;			\
		(dest).interval = (src).interval;		\
		(dest).max_interval = (src).max_interval;	\
	} while (0)

#define WIFIMGR_SET_IP4ADDR(intf, ip, netmask, gateway)	        \
	do {							\
		int res = -1;					\
		res = netlib_set_ipv4addr(intf, &ip);		\
		if (res == -1) {				\
			nvdbg("[WM] set ipv4 addr error\n");	\
		}						\
		res = netlib_set_ipv4netmask(intf, &netmask);	\
		if (res == -1) {				\
			nvdbg("[WM] set netmask addr error\n");	\
		}						\
		res = netlib_set_dripv4addr(intf, &gateway);	\
		if (res == -1) {				\
			nvdbg("[WM] set route addr error\n");	\
		}						\
		g_manager_info.ip4_address = ip.s_addr;		\
	} while (0)

#define WIFIMGR_INC_NUM_CLIENT						\
	do {								\
		pthread_mutex_lock(&g_manager_info.info_lock);		\
		g_manager_info.num_sta++;				\
		pthread_mutex_unlock(&g_manager_info.info_lock);	\
	} while (0)

#define WIFIMGR_DEC_NUM_CLIENT						\
	do {								\
		pthread_mutex_lock(&g_manager_info.info_lock);		\
		g_manager_info.num_sta--;				\
		pthread_mutex_unlock(&g_manager_info.info_lock);	\
	} while (0)

#define WIFIMGR_SPC // to pass the code check ruls
#define WIFIMGR_CHECK_RESULT_CLEANUP(func, msg, ret, free_rsc)	\
	do {							\
		wifi_manager_result_e wmres = func;		\
		if (wmres != WIFI_MANAGER_SUCCESS) {		\
			ndbg(msg);				\
			free_rsc;				\
			return ret;				\
		}						\
	} while (0)

#define WIFIMGR_CHECK_RESULT(func, msg, ret) WIFIMGR_CHECK_RESULT_CLEANUP(func, msg, ret, WIFIMGR_SPC)

#define WIFIMGR_PASS_RESULT_CLEANUP(func, msg, free_rsc)	\
	do {													\
		wifi_manager_result_e wmres = func;					\
		if (wmres != WIFI_MANAGER_SUCCESS) {				\
			ndbg(msg);										\
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_API_FAIL);       \
			free_rsc;										\
			return wmres;									\
		}													\
	} while (0)

#define WIFIMGR_PASS_RESULT(func, msg) WIFIMGR_PASS_RESULT_CLEANUP(func, msg, WIFIMGR_SPC)

#define WIFIMGR_CHECK_RESULT_NORET(func, msg)					\
	do {														\
		wifi_manager_result_e wmres = func;						\
		if (wmres != WIFI_MANAGER_SUCCESS) {					\
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_API_FAIL);       \
			ndbg(msg);											\
		}														\
	} while (0)													\

#define WIFIMGR_CHECK_UTILRESULT(func, msg, ret)	                \
	do {								\
		wifi_utils_result_e res = func;				\
		if (res != WIFI_UTILS_SUCCESS) {			\
			ndbg(msg);					\
			ndbg("error code(%d)\n", res);                  \
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_UTILS_FAIL);     \
			return ret;					\
		}							\
	} while (0)

#define WIFIMGR_CHECK_AP_CONFIG(config)									    \
	do {												    \
		if (config->ssid_length > WIFIMGR_SSID_LEN ||						    \
			config->passphrase_length > WIFIMGR_PASSPHRASE_LEN ||				    \
			strlen(config->ssid) > WIFIMGR_SSID_LEN ||					    \
			strlen(config->passphrase) > WIFIMGR_PASSPHRASE_LEN) {				    \
			ndbg("[WM] AP configuration fails: too long ssid or passphrase\n");	            \
			ndbg("[WM] Make sure that length of SSID < 33 and length of passphrase < 65\n");    \
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);				    \
			return WIFI_MANAGER_INVALID_ARGS;						    \
		}											    \
	} while (0)

#define LOCK_WIFIMGR pthread_mutex_lock(&g_manager_info.state_lock)
#define UNLOCK_WIFIMGR pthread_mutex_unlock(&g_manager_info.state_lock)

#define WIFIMGR_FREE_CONNMSG(msg)		\
	do {					\
		free(msg->config);		\
		msg->config = NULL;		\
		free(msg->conn_config);		\
		msg->conn_config = NULL;	\
		close(msg->fd);			\
		free(msg);			\
	} while (0)

/**
 * Debugging
 */
#define WM_LOG_START nvdbg("[WM] T%d\t%s:%d\n", getpid(), __FUNCTION__, __LINE__);
#define WM_LOG_HANDLER_START nvdbg("[WM] T%d %s:%d state(%d) evt(%d)\n", getpid(), __FUNCTION__, __LINE__, g_manager_info.state, msg->event);
#define WM_APINFO_INITIALIZER {{0,}, 0, {0,}, 0, WIFI_MANAGER_AUTH_UNKNOWN, WIFI_MANAGER_CRYPTO_UNKNOWN}
#define WM_RECONN_INITIALIZER {WIFI_RECONN_NONE, -1, -1}

#ifdef CONFIG_WIFIMGR_INTERNAL_AUTOCONNECT
#define WIFIMGR_DEFAULT_CONN_CONFIG {WIFI_RECONN_INTERVAL, 77, 128}
#else
#define WIFIMGR_DEFAULT_CONN_CONFIG {WIFI_RECONN_NONE, -1, -1}
#endif

#define WIFIMGR_SOTFAP_CONFIG {{0,}, {0,}, 1}

/**
 * global variables
 */
static _wifimgr_info_s g_manager_info = {{0}, {0}, {0}, 0, 0, 0,
										 WIFIMGR_UNINITIALIZED, WIFIMGR_UNINITIALIZED, 0,
										 PTHREAD_MUTEX_INITIALIZER,
										 PTHREAD_MUTEX_INITIALIZER,
										 PTHREAD_MUTEX_INITIALIZER,
										 PTHREAD_COND_INITIALIZER,
										 {NULL, NULL, NULL},
										 0,
										 WM_APINFO_INITIALIZER,
										 WM_RECONN_INITIALIZER,
										 0, 0, 10,
										 WIFIMGR_SOTFAP_CONFIG,
										 {0} };

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
		nvdbg("[WM] T%d wake up\n", getpid());						\
	} while (0)

#define WIFIMGR_TERMINATE_RECONN_WORKER									\
	do {																\
		nvdbg("[WM] send terminate signal to worker(%d/%d)\n", g_manager_info.conn_tries, g_manager_info.max_tries); \
		pthread_mutex_lock(&g_reconn_mutex);							\
		g_manager_info.terminate = true;								\
		pthread_cond_signal(&g_reconn_signal);							\
		pthread_mutex_unlock(&g_reconn_mutex);							\
		nvdbg("[WM] wait worker to terminate\n");							\
		pthread_join(g_manager_info.reconn_id, NULL);					\
	} while (0)
#endif
#ifdef CONFIG_ENABLE_IOTIVITY
static mqd_t g_dw_nwevent_mqfd;
#endif

/**
 * Internal functions
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
static wifi_manager_result_e _start_dhcps(void);
static wifi_manager_result_e _stop_dhcps(void);
static wifi_manager_result_e _get_ipaddr_dhcpc(void);
static wifi_manager_result_e _wifimgr_deinit(void);
static wifi_manager_result_e _wifimgr_run_sta(void);
static wifi_manager_result_e _wifimgr_connect_ap(wifi_manager_ap_config_s *config);
static wifi_manager_result_e _wifimgr_save_connected_config(wifi_manager_ap_config_s *config);
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
static wifi_manager_result_e _handler_on_reconnecting_state(_wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_reconnect_state(_wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_connect_cancel_state(_wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_softap_disconnecting_state(_wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_softap_state(_wifimgr_msg_s *msg);
static wifi_manager_result_e _handler_on_scanning_state(_wifimgr_msg_s *msg);


typedef wifi_manager_result_e (*wifimgr_handler)(_wifimgr_msg_s *msg);
/*
 * g_handler should be matched to _wifimgr_state
 */
const wifimgr_handler g_handler[] = {
	_handler_on_uninitialized_state,
	_handler_on_disconnected_state,
	_handler_on_disconnecting_state,
	_handler_on_connecting_state,
	_handler_on_connected_state,
	_handler_on_reconnect_state,
	_handler_on_reconnecting_state,
	_handler_on_connect_cancel_state,
	_handler_on_softap_disconnecting_state,
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


#if WIFIDRIVER_SUPPORT_AUTOCONNECT == 0
wifi_manager_result_e _wifi_manager_reconnect_ap(wifi_manager_ap_config_s *config)
{
	_wifimgr_msg_s msg = {EVT_RECONNECT, config};
	wifi_manager_result_e res = _handle_request(&msg);
	return res;
}


static void *_reconn_worker(void *arg)
{
	_wifimgr_conn_info_msg_s *msg = (_wifimgr_conn_info_msg_s *)arg;
	wifi_manager_ap_config_s *apinfo = msg->config;
	wifi_manager_reconnect_config_s *conn_config = msg->conn_config;
	struct sockaddr_in servaddr;
	fd_set rfds;
	fd_set fds;

	int s = socket(PF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		perror("socket fail");
		WIFIMGR_FREE_CONNMSG(msg);
		return NULL;
	}
	memset(&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(WIFIMGR_IPC_PORT);

	int ret = bind(s, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in));
	if (ret < 0) { // critical error
		perror("bind fail\n");
		goto EXIT;
	}
	FD_ZERO(&fds);
	FD_SET(s, &fds);

	while (true) {
		LOCK_RECONN;
		if (g_manager_info.terminate) {		// check terminate
			UNLOCK_RECONN;
			nvdbg("[WM] T%d exit while loop\n", getpid());
			goto EXIT;
		}
		UNLOCK_RECONN;

		// get next interval
		uint32_t interval = conn_config->interval;

		// wait to next interval, if disconnect request is received then terminate the thread immediately
		struct timeval tv = {0, 0};
		tv.tv_sec = interval;
		rfds = fds;
		nvdbg("Wait interval(%d) fd(%d)\n", interval, s);
		ret = select(s + 1, &rfds, NULL, NULL, &tv);

		if (ret < 0) {
			ndbg("[WM] wait interval error(%d) (%d)\n", ret, errno);
			goto EXIT;
		}
		if (ret == 0) {
			nvdbg("[WM] timeout (%d)seconds\n", interval);
		} else if (FD_ISSET(s, &rfds)) {
			nvdbg("[WM] Terminate thread while waiting interval\n");
			goto EXIT;
		} else { // critical error
			ndbg("[WM] it should not happen\n");
		}

		wifi_manager_result_e res = _wifi_manager_reconnect_ap(apinfo);
		if (res == WIFI_MANAGER_SUCCESS) {
			WIFIMGR_WAIT_RECONNECT_EVENT;
		} else if (res == WIFI_MANAGER_ALREADY_CONNECTED) {
			ndbg("[WM] already connected(connected by driver)\n");
			goto EXIT;
		} else {
			ndbg("[WM] Calling connect API fails\n");
		}
	}
 EXIT:
	WIFIMGR_FREE_CONNMSG(msg);
	close(s);
	nvdbg("[WM] reconnect func terminated\n");
	return NULL;
}
#endif /* WIFIDRIVER_SUPPORT_AUTOCONNECT*/


wifi_manager_result_e
_convert_scan_info(wifi_manager_scan_info_s **wm_scan_list, wifi_utils_scan_list_s *wu_scan_list)
{
	wifi_manager_scan_info_s *cur = NULL, *prev = NULL;
	wifi_utils_scan_list_s *iter = wu_scan_list;
	while (iter) {
		cur = (wifi_manager_scan_info_s *)calloc(1, sizeof(wifi_manager_scan_info_s));
		if (!cur) {
			_free_scan_info(*wm_scan_list);
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_INTERNAL_FAIL);
			return WIFI_MANAGER_FAIL;
		}
		cur->rssi = iter->ap_info.rssi;
		cur->channel = iter->ap_info.channel;
		cur->phy_mode = iter->ap_info.phy_mode;
		cur->ap_auth_type = iter->ap_info.ap_auth_type;
		cur->ap_crypto_type = iter->ap_info.ap_crypto_type;
		strncpy(cur->ssid, (char *)iter->ap_info.ssid, WIFIMGR_SSID_LEN);
		cur->ssid[WIFIMGR_SSID_LEN] = '\0';
		strncpy(cur->bssid, (char *)iter->ap_info.bssid, WIFIMGR_MACADDR_STR_LEN);
		cur->bssid[WIFIMGR_MACADDR_STR_LEN] = '\0';

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
	case WIFIMGR_SOFTAP_DISCONNECTING_STA:
	case WIFIMGR_STA_CONNECT_CANCEL:
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
	case WIFIMGR_STA_RECONNECT:
	case WIFIMGR_STA_RECONNECTING:
		*mode = STA_MODE;
		*conn = AP_RECONNECTING;
		break;
	default:
		// CRITICAL ERROR
		ndbg("[WM] CRITICAL ERROR: BAD STATE (%d)\n", state);
		break;
	}
}


wifi_manager_result_e _start_dhcps(void)
{
	struct in_addr ip = {.s_addr = 0x012fa8c0 };
	struct in_addr netmask = {.s_addr = 0x00ffffff};
	struct in_addr gw = {.s_addr = 0x012fa8c0};
	WIFIMGR_SET_IP4ADDR(WIFIMGR_SOFTAP_IFNAME, ip, netmask, gw);
	wifi_manager_result_e wret = WIFI_MANAGER_FAIL;
	if (dhcp_server_start(WIFIMGR_SOFTAP_IFNAME, _wifi_dhcpd_event) == OK) {
		ndbg("[WM] DHCP Server - started successfully\n");
#ifdef CONFIG_ENABLE_IOTIVITY
		__tizenrt_manual_linkset("gen");
#endif
		wret = WIFI_MANAGER_SUCCESS;
	} else {
		ndbg("[WM] DHCP Server - started fail\n");
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_SOFTAP_DHCPD_FAIL);
	}
	return wret;
}


wifi_manager_result_e _stop_dhcps(void)
{
	struct in_addr in = { .s_addr = INADDR_NONE };
	WIFIMGR_SET_IP4ADDR(WIFIMGR_SOFTAP_IFNAME, in, in, in);
	wifi_manager_result_e wret = WIFI_MANAGER_FAIL;
	if (dhcp_server_stop(WIFIMGR_SOFTAP_IFNAME) == OK) {
		ndbg("[WM] DHCP Server - stopped successfully\n");
		wret = WIFI_MANAGER_SUCCESS;
#ifdef CONFIG_ENABLE_IOTIVITY
		__tizenrt_manual_linkset("del");
#endif
	}
	return wret;
}


wifi_manager_result_e _get_ipaddr_dhcpc(void)
{
	wifi_manager_result_e wret = WIFI_MANAGER_FAIL;
	int ret;
	struct in_addr ip_check;

	ret = dhcp_client_start(WIFIMGR_STA_IFNAME);
	if (ret != OK) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_CONNECT_DHCPC_FAIL);
		ndbg("[WM] get IP address fail\n");
		return wret;
	}

	ret = netlib_get_ipv4addr(WIFIMGR_STA_IFNAME, &ip_check);
	if (ret != OK) {
		ndbg("[WM] get IP address fail\n");
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_CONNECT_DHCPC_FAIL);
		return wret;
	}

	ndbg("[WM] get IP address %s\n", inet_ntoa(ip_check));
	g_manager_info.ip4_address = ip_check.s_addr;
	wret = WIFI_MANAGER_SUCCESS;
	return wret;
}

void _close_ipaddr_dhcpc(void)
{
	int ret;
	struct in_addr ip_check;	
	dhcp_client_stop(WIFIMGR_STA_IFNAME);

	ret = netlib_get_ipv4addr(WIFIMGR_STA_IFNAME, &ip_check);
	if (ret != OK) {
		ndbg("[WM] dhcp_stop - get IP address fail\n");
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_CONNECT_DHCPC_FAIL);
		return;
	}

	ndbg("[WM] dhcp_stop - release IP address %s\n", inet_ntoa(ip_check));
	g_manager_info.ip4_address = ip_check.s_addr;

	return;
}

wifi_manager_result_e _wifimgr_deinit(void)
{
	WM_LOG_START;
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_deinit(), "[WM] wifi_utils_deinit fail", WIFI_MANAGER_FAIL);
#ifdef CONFIG_ENABLE_IOTIVITY
	int ret = mq_close(g_dw_nwevent_mqfd);
	if (ret < 0) {
		ndbg("[WM] close message queue fail\n");
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_MSGQ_FAIL);
		return WIFI_MANAGER_FAIL;
	}
#endif
	int i = 0;
	for (i = 0; i < WIFIMGR_NUM_CALLBACKS; i++) {
		if (g_manager_info.cb[i] != NULL) {
			g_manager_info.cb[i] = NULL;
		}
	}
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_run_sta(void)
{
	WM_LOG_START;
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
	WM_LOG_START;
	wifi_utils_result_e ret = wifi_profile_write(config, 1);
	if (ret != WIFI_UTILS_SUCCESS) {
		ndbg("[WM] Failed to save the connected AP configuration in file system\n");
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_connect_ap(wifi_manager_ap_config_s *config)
{
	WM_LOG_START;
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
	WM_LOG_START;

	WIFIMGR_CHECK_UTILRESULT(wifi_utils_disconnect_ap(NULL), "[WM] disconnect to ap fail\n", WIFI_MANAGER_FAIL);
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_run_softap(wifi_manager_softap_config_s *config)
{
	WM_LOG_START;

	if (strlen(config->ssid) > WIFIMGR_SSID_LEN || strlen(config->passphrase) > WIFIMGR_PASSPHRASE_LEN) {
		ndbg("[WM] SSID or PASSPHRASE length invalid\n");
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

	WIFIMGR_CHECK_UTILRESULT(wifi_utils_start_softap(&softap_config), "[WM] Starting softap mode failed.", WIFI_MANAGER_FAIL);
	WIFIMGR_CHECK_RESULT(_start_dhcps(), "[WM] Starting DHCP server failed.\n", WIFI_MANAGER_FAIL);

	/* update wifi_manager_info */
	WIFIMGR_SET_SOFTAP_SSID(config->ssid);
	g_manager_info.num_sta = 0;

	if (g_manager_info.state == WIFIMGR_SOFTAP_DISCONNECTING_STA) {
		WIFIMGR_SOFTAP_CALLBACK_RECEIVED;
	}
	/* For tracking softap stats, the LAST value is used */
	WIFIMGR_STATS_INC(CB_SOFTAP_DONE);
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_stop_softap(void)
{
	WM_LOG_START;
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_stop_softap(), "[WM] Stoping softap failed", WIFI_MANAGER_FAIL);
	WIFIMGR_CHECK_RESULT(_stop_dhcps(), "[WM] Stoping softap DHCP server failed.", WIFI_MANAGER_FAIL);
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _wifimgr_scan(void)
{
	WM_LOG_START;
	wifi_manager_cb_s *cbk = g_manager_info.cb[0];
	wifi_manager_result_e wret = WIFI_MANAGER_FAIL;
	if (!cbk->scan_ap_done) {
		return WIFI_MANAGER_CALLBACK_NOT_REGISTERED;
	}
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_scan_ap(NULL), "[WM] request scan to wifi utils is fail\n", WIFI_MANAGER_FAIL);
	wret = WIFI_MANAGER_SUCCESS;
	return wret;
}


void _wifi_utils_connect_event(wifi_utils_result_e result, void *arg)
{
	WM_LOG_START;
	_wifimgr_msg_s msg = {EVT_STA_CONNECTED, NULL};
	if (result == WIFI_UTILS_FAIL) {
		msg.event = EVT_STA_CONNECT_FAILED;
	}
	WIFIMGR_CHECK_RESULT_NORET(_handle_request(&msg), "[WM] handle connect_event fail\n");
}


void _wifi_utils_disconnect_event(void *arg)
{
	WM_LOG_START;
	_wifimgr_msg_s msg = {EVT_STA_DISCONNECTED, NULL};
	WIFIMGR_CHECK_RESULT_NORET(_handle_request(&msg), "[WM] handle disconnect event fail\n");
}


void _wifi_utils_join_event(void *arg)
{
	WM_LOG_START;
	_wifimgr_msg_s msg = {EVT_JOINED, NULL};
	WIFIMGR_CHECK_RESULT_NORET(_handle_request(&msg), "[WM] handle join event fail\n");
}


void _wifi_utils_leave_event(void * arg)
{
	WM_LOG_START;
	_wifimgr_msg_s msg = {EVT_LEFT, NULL};
	WIFIMGR_CHECK_RESULT_NORET(_handle_request(&msg), "[WM] handle leave event fail\n");
}


void _wifi_utils_scan_done(wifi_utils_result_e result, wifi_utils_scan_list_s *slist, void *arg)
{
	WM_LOG_START;
	_wifimgr_msg_s msg = {EVT_SCAN_DONE, (void *)slist};
	WIFIMGR_CHECK_RESULT_NORET(_handle_request(&msg), "[WM] handle scan done event fail\n");
}

void _wifi_dhcpd_event(void)
{
	WM_LOG_START;
	_wifimgr_msg_s msg = {EVT_DHCPD_GET_IP, NULL};
	WIFIMGR_CHECK_RESULT_NORET(_handle_request(&msg), "[WM] handle dhcpd event fail\n");
	return;
}

/*
 * State Functions
 */
wifi_manager_result_e _handler_on_uninitialized_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	_wifimgr_evt_e evt = msg->event;
	if (evt != EVT_INIT) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
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
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_MSGQ_FAIL);
		return WIFI_MANAGER_FAIL;
	}
#endif
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_init(), "[WM] wifi_utils_init fail\n", WIFI_MANAGER_FAIL);
#if WIFIDRIVER_SUPPORT_AUTOCONNECT
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_set_autoconnect(1), "[WM] Set Autoconnect failed", WIFI_MANAGER_FAIL);
#else
	WIFIMGR_CHECK_UTILRESULT(wifi_utils_set_autoconnect(0), "[WM] Set Autoconnect failed", WIFI_MANAGER_FAIL);
#endif

	WIFIMGR_CHECK_UTILRESULT(wifi_profile_init(), "[WM] wifi_profile init fail\n", WIFI_MANAGER_FAIL);
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
	wifi_utils_result_e wres = wifi_utils_get_info(&info);
	if (wres != WIFI_UTILS_SUCCESS) {
		ndbg("[WM] wifi_utils_get_info fail");
		WIFIMGR_CHECK_UTILRESULT(wifi_utils_deinit(), "critical error\n", WIFI_MANAGER_FAIL);
	}

	g_manager_info.cb[0] = cb;
	memcpy(g_manager_info.mac_address, info.mac_address, WIFIMGR_MACADDR_LEN);

	WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_disconnected_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;

	if (msg->event == EVT_CONNECT) {
		_wifimgr_conn_info_msg_s *conn_msg = (_wifimgr_conn_info_msg_s *)msg->param;
		wifi_manager_ap_config_s *apinfo = conn_msg->config;
		wifi_manager_reconnect_config_s *conn_config = conn_msg->conn_config;
		/* store wifi ap info to reconnect when the device is disconnected abnormaly*/
		WIFIMGR_COPY_AP_INFO(g_manager_info.connected_ap, *apinfo);
		WIFIMGR_COPY_RECONN_INFO(g_manager_info.conn_config, *conn_config);
		WIFIMGR_CHECK_RESULT(_wifimgr_connect_ap(apinfo), "connect ap fail\n", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECTING);
	} else if (msg->event == EVT_DEINIT) {
		WIFIMGR_CHECK_RESULT(_wifimgr_deinit(), "critical error", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_UNINITIALIZED);
	} else if (msg->event == EVT_SET_SOFTAP) {
		WIFIMGR_CHECK_RESULT(_wifimgr_run_softap((wifi_manager_softap_config_s *)msg->param), "run_softap fail", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_SOFTAP);
	} else if (msg->event == EVT_SCAN) {
		WIFIMGR_CHECK_RESULT(_wifimgr_scan(), "fail scan", WIFI_MANAGER_FAIL);
		WIFIMGR_STORE_PREV_STATE;
		WIFIMGR_SET_STATE(WIFIMGR_SCANNING);
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
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
		_close_ipaddr_dhcpc();
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}



wifi_manager_result_e _handler_on_softap_disconnecting_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	if (msg->event == EVT_STA_DISCONNECTED) {
		if (WIFIMGR_CHECK_CBK) {
			_handle_user_cb(CB_STA_DISCONNECTED, NULL);
		} else {
			WIFIMGR_RESET_CBK_CHK;
		}
		_close_ipaddr_dhcpc();
		WIFIMGR_CHECK_RESULT(_wifimgr_run_softap(&g_manager_info.softap_config), "run_softap fail", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_SOFTAP);
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}

wifi_manager_result_e _handler_on_connecting_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	if (msg->event == EVT_STA_CONNECTED) {
#ifndef CONFIG_WIFIMGR_DISABLE_AUTO_GET_IP
		wifi_manager_result_e wret;
		wret = _get_ipaddr_dhcpc();
		if (wret != WIFI_MANAGER_SUCCESS) {
			_handle_user_cb(CB_STA_CONNECT_FAILED, NULL);
			WIFIMGR_CHECK_RESULT(_wifimgr_disconnect_ap(), "[WM] critical error: DHCP failure\n", WIFI_MANAGER_FAIL);
			WIFIMGR_SET_NO_CBK;
			WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTING);
			return wret;
		}
#endif
		_handle_user_cb(CB_STA_CONNECTED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECTED);
	} else if (msg->event == EVT_STA_CONNECT_FAILED) {
		_handle_user_cb(CB_STA_CONNECT_FAILED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_connected_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	if (msg->event == EVT_DISCONNECT) {
		WIFIMGR_CHECK_RESULT(_wifimgr_disconnect_ap(), "critical error", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTING);
	} else if (msg->event == EVT_STA_DISCONNECTED) {
#if WIFIDRIVER_SUPPORT_AUTOCONNECT == 0
		if (g_manager_info.conn_config.type == WIFI_RECONN_NONE) {
			_handle_user_cb(CB_STA_DISCONNECTED, NULL);
			_close_ipaddr_dhcpc();
			WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
		} else {
			_handle_user_cb(CB_STA_RECONNECTED, NULL);
			_wifimgr_conn_info_msg_s *rmsg = (_wifimgr_conn_info_msg_s *)malloc(sizeof(_wifimgr_conn_info_msg_s));
			if (!rmsg) {				// critical error
				ndbg("[WM] allocate memory for reconn msg fail\n");
				WIFIADD_ERR_RECORD(ERR_WIFIMGR_INTERNAL_FAIL);
				return WIFI_MANAGER_FAIL;
			}
			rmsg->config = (wifi_manager_ap_config_s *)malloc(sizeof(wifi_manager_ap_config_s));
			if (!rmsg->config) {				// critical error
				free(rmsg);
				ndbg("[WM] allocate memory for reconnmsg apconfig is fail\n");
				WIFIADD_ERR_RECORD(ERR_WIFIMGR_INTERNAL_FAIL);
				return WIFI_MANAGER_FAIL;
			}

			rmsg->conn_config = (wifi_manager_reconnect_config_s *)malloc(sizeof(wifi_manager_reconnect_config_s));
			if (!rmsg->conn_config) {				// critical error
				free(rmsg->config);
				free(rmsg);
				ndbg("[WM] allocate memory for reonnmsg conn_config is fail\n");
				WIFIADD_ERR_RECORD(ERR_WIFIMGR_INTERNAL_FAIL);
				return WIFI_MANAGER_FAIL;
			}
			WIFIMGR_COPY_AP_INFO(*(rmsg->config), g_manager_info.connected_ap);
			WIFIMGR_COPY_RECONN_INFO(*(rmsg->conn_config), g_manager_info.conn_config);

			g_manager_info.conn_tries = 0;
			g_manager_info.terminate = false;
			int ret = pthread_create(&g_manager_info.reconn_id, NULL, _reconn_worker, (pthread_addr_t)rmsg);
			if (ret != 0) {				// critical error
				ndbg("[WM] [error] pthread_create fail\n");
				WIFIADD_ERR_RECORD(ERR_WIFIMGR_INTERNAL_FAIL);
			} else {
				ndbg("[WM] Internal AUTOCONNECT: go to RECONNECT state\n");
			}
			WIFIMGR_SET_STATE(WIFIMGR_STA_RECONNECT);
		}
#else /* WIFIDRIVER_SUPPORT_AUTOCONNECT */
		ndbg("[WM] External AUTOCONNECT: go to RECONNECT state\n");
		_handle_user_cb(CB_STA_RECONNECTED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_RECONNECT);
#endif /* WIFIDRIVER_SUPPORT_AUTOCONNECT */
	} else if (msg->event == EVT_SET_SOFTAP) {
		WIFIMGR_CHECK_RESULT(_wifimgr_disconnect_ap(), "critical error", WIFI_MANAGER_FAIL);
		WIFIMGR_COPY_SOFTAP_CONFIG(g_manager_info.softap_config, (wifi_manager_softap_config_s *)msg->param);
		WIFIMGR_SET_STATE(WIFIMGR_SOFTAP_DISCONNECTING_STA);
	} else if (msg->event == EVT_DEINIT) {
		// Todo: do I need to disconnect?
		WIFIMGR_CHECK_RESULT(_wifimgr_deinit(), "critical error\n", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_UNINITIALIZED);
	} else if (msg->event == EVT_SCAN) {
		WIFIMGR_CHECK_RESULT(_wifimgr_scan(), "fail scan\n", WIFI_MANAGER_FAIL);
		WIFIMGR_STORE_PREV_STATE;
		WIFIMGR_SET_STATE(WIFIMGR_SCANNING);
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_FAIL;
	}
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_reconnect_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
#if WIFIDRIVER_SUPPORT_AUTOCONNECT == 0
	if (msg->event == EVT_DISCONNECT) {
		nvdbg("[WM] disconnect\n");
		pthread_mutex_lock(&g_reconn_mutex);
		g_manager_info.terminate = true;
		pthread_mutex_unlock(&g_reconn_mutex);

		struct sockaddr_in serveraddr;
		int sd = socket(PF_INET, SOCK_DGRAM, 0);
		if (sd < 0) {
			ndbg("[WM] socket create fail\n");
		}
		bzero(&serveraddr, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		serveraddr.sin_port = htons(WIFIMGR_IPC_PORT);

		int res = sendto(sd, "term", 5, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
		if (res < 0) { // critical error
			ndbg("send signal result(%d) (%d)\n", res, errno);
		}
		close(sd);
		pthread_join(g_manager_info.reconn_id, NULL);
		_close_ipaddr_dhcpc();
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	} else if (msg->event == EVT_RECONNECT) {
		wifi_manager_ap_config_s *apinfo = (wifi_manager_ap_config_s *)msg->param;
		wifi_manager_result_e wres = _wifimgr_connect_ap(apinfo);
		if (wres == WIFI_MANAGER_ALREADY_CONNECTED) {
			/* some wifi drivers try to reconnect by itself.*/
			ndbg("[WM] reconnect state already connected\n");
			_handle_user_cb(CB_STA_CONNECTED, NULL);
			WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECTED);
			return WIFI_MANAGER_ALREADY_CONNECTED;
		}
		if (wres != WIFI_MANAGER_SUCCESS) {
			ndbg("[WM] Reconnect AP fail disconnect AP for initializing it(%d)\n", wres);
			_wifimgr_disconnect_ap();
			return wres;
		}
		WIFIMGR_SET_STATE(WIFIMGR_STA_RECONNECTING);
	} else if (msg->event == EVT_DEINIT) {
		nvdbg("[WM] deinit\n");
		WIFIMGR_CHECK_RESULT(_wifimgr_deinit(), "critical error\n", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_UNINITIALIZED);
	}
#else /* WIFIDRIVER_SUPPORT_AUTOCONNECT*/
	if (msg->event == EVT_DISCONNECT) {
		ndbg("[WM] AUTOCONNECT fail: go to DISCONNECTED\n");
		_close_ipaddr_dhcpc();
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	} else if (msg->event == EVT_STA_CONNECT_FAILED) {
		ndbg("[WM] AUTOCONNECT wait\n");
	} else if (msg->event == EVT_STA_CONNECTED) {
		ndbg("[WM] AUTOCONNECT done: go to CONNECTED\n");
		_handle_user_cb(CB_STA_CONNECTED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECTED);
	} else if (msg->event == EVT_DEINIT) {
		ndbg("[WM] AUTOCONNECT fail: go to DEINIT\n");
		WIFIMGR_CHECK_RESULT(_wifimgr_deinit(), "critical error\n", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_UNINITIALIZED);
	}
#endif /* WIFIDRIVER_SUPPORT_AUTOCONNECT*/
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_reconnecting_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
#if WIFIDRIVER_SUPPORT_AUTOCONNECT == 0
	if (msg->event == EVT_STA_CONNECT_FAILED) {
		nvdbg("[WM] reconnect fail\n");
		if (g_manager_info.conn_tries == g_manager_info.max_tries) {
			ndbg("[WM] Stop to reconnect because of reaching to max tries\n");
			WIFIMGR_TERMINATE_RECONN_WORKER;
		} else {
			WIFIMGR_TRIGGER_RECONNECT_WORKER;
		}
		WIFIMGR_SET_STATE(WIFIMGR_STA_RECONNECT);
	} else if (msg->event == EVT_DISCONNECT) {
		nvdbg("[WM] disconnect\n");
		WIFIMGR_TERMINATE_RECONN_WORKER;
		WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECT_CANCEL);
	} else if (msg->event == EVT_STA_CONNECTED) {
		WIFIMGR_TERMINATE_RECONN_WORKER;
		_handle_user_cb(CB_STA_CONNECTED, NULL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_CONNECTED);
	}
#endif /* WIFIDRIVER_SUPPORT_AUTOCONNECT*/
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_connect_cancel_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
#if WIFIDRIVER_SUPPORT_AUTOCONNECT == 0
	if (msg->event == EVT_STA_CONNECTED) {
		WIFIMGR_CHECK_RESULT(_wifimgr_disconnect_ap(), "critical error", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTING);
	} else if (msg->event == EVT_STA_CONNECT_FAILED) {
		_close_ipaddr_dhcpc();
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	}
#endif /* WIFIDRIVER_SUPPORT_AUTOCONNECT*/
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_softap_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	if (msg->event == EVT_SET_STA) {
		WIFIMGR_CHECK_RESULT(_wifimgr_stop_softap(), "critical error\n", WIFI_MANAGER_FAIL);
		WIFIMGR_CHECK_RESULT(_wifimgr_run_sta(), "critical error\n", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_STA_DISCONNECTED);
	} else if (msg->event == EVT_SCAN) {
		WIFIMGR_CHECK_RESULT(_wifimgr_scan(), "fail scan\n", WIFI_MANAGER_FAIL);
		WIFIMGR_STORE_PREV_STATE;
		WIFIMGR_SET_STATE(WIFIMGR_SCANNING);
	} else if (msg->event == EVT_JOINED) {
		/* wifi manager passes the callback after the dhcp server gives a station an IP address*/
	} else if (msg->event == EVT_DHCPD_GET_IP) {
		WIFIMGR_INC_NUM_CLIENT;
		_handle_user_cb(CB_STA_JOINED, NULL);
	} else if (msg->event == EVT_LEFT) {
		WIFIMGR_DEC_NUM_CLIENT;
		_handle_user_cb(CB_STA_LEFT, NULL);
	} else if (msg->event == EVT_DEINIT) {
		WIFIMGR_CHECK_RESULT(_wifimgr_stop_softap(), "critical error\n", WIFI_MANAGER_FAIL);
		WIFIMGR_CHECK_RESULT(_wifimgr_deinit(), "critical error\n", WIFI_MANAGER_FAIL);
		WIFIMGR_SET_STATE(WIFIMGR_UNINITIALIZED);
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
		return WIFI_MANAGER_FAIL;
	}

	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e _handler_on_scanning_state(_wifimgr_msg_s *msg)
{
	WM_LOG_HANDLER_START;
	wifi_manager_result_e wret = WIFI_MANAGER_FAIL;
	if (msg->event == EVT_SCAN_DONE) {
		_handle_user_cb(CB_SCAN_DONE, msg->param);
		WIFIMGR_RESTORE_STATE;
		wret = WIFI_MANAGER_SUCCESS;
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
	}
	return wret;
}


void _handle_user_cb(_wifimgr_usr_cb_type_e evt, void *arg)
{
	WM_LOG_START;
	int i = 0;
	for (; i < WIFIMGR_NUM_CALLBACKS; i++) {
		wifi_manager_cb_s *cbk = g_manager_info.cb[i];
		if (!cbk) {
			continue;
		}
		switch (evt) {
		case CB_STA_CONNECTED:
			nvdbg("[WM] call sta connect success event\n");
#ifdef CONFIG_ENABLE_IOTIVITY
			__tizenrt_manual_linkset("gen");
#endif
			cbk->sta_connected(WIFI_MANAGER_SUCCESS);
			break;
		case CB_STA_CONNECT_FAILED:
			nvdbg("[WM] call sta connect fail event\n");
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_CONNECT_FAIL);
			cbk->sta_connected(WIFI_MANAGER_FAIL);
			break;
		case CB_STA_DISCONNECTED:
			nvdbg("[WM] call sta disconnect event\n");
#ifdef CONFIG_ENABLE_IOTIVITY
			__tizenrt_manual_linkset("del");
#endif
			cbk->sta_disconnected(WIFI_MANAGER_DISCONNECT);
			break;
		case CB_STA_RECONNECTED:
			nvdbg("[WM] call sta disconnect event\n");
#ifdef CONFIG_ENABLE_IOTIVITY
			__tizenrt_manual_linkset("del");
#endif
			cbk->sta_disconnected(WIFI_MANAGER_RECONNECT);
			break;
		case CB_STA_JOINED:
			nvdbg("[WM] call sta join event\n");
			cbk->softap_sta_joined();
			break;
		case CB_STA_LEFT:
			nvdbg("[WM] call sta leave event\n");
			cbk->softap_sta_left();
			break;
		case CB_SCAN_DONE:
			nvdbg("[WM] call sta scan event\n");
			/* convert scan data.*/
			wifi_manager_scan_info_s *info = NULL;
			wifi_utils_scan_list_s *list = (wifi_utils_scan_list_s *)arg;
			if (list) {
				WIFIMGR_CHECK_RESULT_CLEANUP(_convert_scan_info(&info, (wifi_utils_scan_list_s *)arg), "parse error", , cbk->scan_ap_done(NULL, WIFI_SCAN_FAIL));
				cbk->scan_ap_done(&info, WIFI_SCAN_SUCCESS);
				_free_scan_info(info);
			} else {
				WIFIADD_ERR_RECORD(ERR_WIFIMGR_SCAN_FAIL);
				cbk->scan_ap_done(NULL, WIFI_SCAN_FAIL);
			}
			break;
		default:
			WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_EVENT);
			ndbg("[WM] Invalid State\n");
			return;
		}
	}
	WIFIMGR_STATS_INC(evt);
}


wifi_manager_result_e _handle_request(_wifimgr_msg_s *msg)
{
	wifi_manager_result_e res = WIFI_MANAGER_FAIL;

	LOCK_WIFIMGR;
	WM_LOG_HANDLER_START;
	res = g_handler[WIFIMGR_GET_STATE](msg);
#ifdef CONFIG_WIFIMGR_ERROR_REPORT
	if (res != WIFI_MANAGER_SUCCESS) {
		ndbg("[WM] Wi-Fi manager fail: state error\n");
		error_code_wifi_manager_t wifi_err_code;
		switch (msg->event) {
		case EVT_INIT:
			wifi_err_code = ERR_WIFIMGR_INIT_FAIL;
			break;
		case EVT_DEINIT:
			wifi_err_code = ERR_WIFIMGR_DEINIT_FAIL;
			break;
		case EVT_SET_SOFTAP:
			wifi_err_code = ERR_WIFIMGR_SOFTAP_FAIL;
			break;
		case EVT_SET_STA:
			wifi_err_code = ERR_WIFIMGR_STA_FAIL;
			break;
		case EVT_CONNECT:
			wifi_err_code = ERR_WIFIMGR_CONNECT_FAIL;
			break;
		case EVT_DISCONNECT:
			wifi_err_code = ERR_WIFIMGR_DISCONNECT_FAIL;
			break;
		case EVT_SCAN:
			wifi_err_code = ERR_WIFIMGR_SCAN_FAIL;
			break;
		default:
			wifi_err_code = ERR_WIFIMGR_UTILS_FAIL;
			break;
		}
		WIFIADD_ERR_RECORD(wifi_err_code);
	}
#endif
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
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
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
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}
	if (mode == SOFTAP_MODE && !config) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}

	_wifimgr_msg_s msg = {EVT_SET_STA, NULL};
	if (mode == SOFTAP_MODE) {
		msg.event = EVT_SET_SOFTAP;
		msg.param = (void *)config;
	}
	wifi_manager_result_e res = _handle_request(&msg);
	LOCK_WIFIMGR;
	if (g_manager_info.state == WIFIMGR_SOFTAP_DISCONNECTING_STA) {
		UNLOCK_WIFIMGR;
		WIFIMGR_SOFTAP_WAIT_CALLBACK;
	} else {
		UNLOCK_WIFIMGR;
	}

	return res;
}


wifi_manager_result_e wifi_manager_get_info(wifi_manager_info_s *info)
{
	if (info == NULL) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
		return WIFI_MANAGER_INVALID_ARGS;
	}
	LOCK_WIFIMGR;
	uint8_t *ip = (uint8_t *)&g_manager_info.ip4_address;
	snprintf(info->ip4_address, 18, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	memcpy(info->mac_address, g_manager_info.mac_address, WIFIMGR_MACADDR_LEN);
	/* Get RSSI */
	wifi_utils_info info_utils;
	wifi_utils_result_e wres = wifi_utils_get_info(&info_utils);
	if (wres != WIFI_UTILS_FAIL) {
		g_manager_info.rssi = info_utils.rssi;
	}
	info->rssi = g_manager_info.rssi;
	if (WIFIMGR_IS_STATE(WIFIMGR_SCANNING)) {
		_convert_state_to_info(&info->status, &info->mode, WIFIMGR_GET_PREVSTATE);
	} else {
		_convert_state_to_info(&info->status, &info->mode, WIFIMGR_GET_STATE);
	}
	if (info->mode == SOFTAP_MODE) {
		memcpy(info->ssid, g_manager_info.softap_ssid, WIFIMGR_SSID_LEN + 1);
	} else {
		memcpy(info->ssid, g_manager_info.ssid, WIFIMGR_SSID_LEN + 1);
	}
	UNLOCK_WIFIMGR;
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e wifi_manager_connect_ap_config(wifi_manager_ap_config_s *config,
													 wifi_manager_reconnect_config_s *conn_config)
{
	wifi_manager_result_e wret = WIFI_MANAGER_INVALID_ARGS;
	if (config && conn_config) {
		WIFIMGR_CHECK_AP_CONFIG(config);
		_wifimgr_conn_info_msg_s conninfo = {config, conn_config};
		_wifimgr_msg_s msg = {EVT_CONNECT, &conninfo};
		wret = _handle_request(&msg);
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
	}
	return wret;
}


wifi_manager_result_e wifi_manager_connect_ap(wifi_manager_ap_config_s *config)
{
	wifi_manager_result_e wret = WIFI_MANAGER_INVALID_ARGS;
	if (config) {
		wifi_manager_reconnect_config_s conn_config = WIFIMGR_DEFAULT_CONN_CONFIG;
		wret = wifi_manager_connect_ap_config(config, &conn_config);
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
	}
	return wret;
}


wifi_manager_result_e wifi_manager_disconnect_ap(void)
{
	_wifimgr_msg_s msg = {EVT_DISCONNECT, NULL};
	wifi_manager_result_e wret = _handle_request(&msg);
	return wret;
}


wifi_manager_result_e wifi_manager_scan_ap(void)
{
	_wifimgr_msg_s msg = {EVT_SCAN, NULL};
	wifi_manager_result_e wret = _handle_request(&msg);
	return wret;
}

wifi_manager_result_e wifi_manager_save_config(wifi_manager_ap_config_s *config)
{
	wifi_manager_result_e wret = WIFI_MANAGER_INVALID_ARGS;
	if (config) {
		WIFIMGR_CHECK_AP_CONFIG(config);
		WIFIMGR_CHECK_UTILRESULT(wifi_profile_write(config, 0), "wifimgr save config fail\n", WIFI_MANAGER_FAIL);
		wret = WIFI_MANAGER_SUCCESS;
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
	}
	return wret;
}


wifi_manager_result_e wifi_manager_get_config(wifi_manager_ap_config_s *config)
{
	wifi_manager_result_e wret = WIFI_MANAGER_INVALID_ARGS;
	if (config) {
		WIFIMGR_CHECK_UTILRESULT(wifi_profile_read(config, 0), "wifimgr get config fail\n", WIFI_MANAGER_FAIL);
		wret = WIFI_MANAGER_SUCCESS;
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
	}
	return wret;
}


wifi_manager_result_e wifi_manager_remove_config(void)
{
	WIFIMGR_CHECK_UTILRESULT(wifi_profile_reset(0), "wifimgr remove config fail\n", WIFI_MANAGER_FAIL);
	return WIFI_MANAGER_SUCCESS;
}


wifi_manager_result_e wifi_manager_get_connected_config(wifi_manager_ap_config_s *config)
{
	wifi_manager_result_e wret = WIFI_MANAGER_INVALID_ARGS;
	if (config) {
		WIFIMGR_CHECK_UTILRESULT(wifi_profile_read(config, 1), "wifimgr get config fail\n", WIFI_MANAGER_FAIL);
		wret = WIFI_MANAGER_SUCCESS;
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
	}
	return wret;
}


wifi_manager_result_e wifi_manager_get_stats(wifi_manager_stats_s *stats)
{
	wifi_manager_result_e wret = WIFI_MANAGER_INVALID_ARGS;
	if (stats) {
		LOCK_WIFIMGR;
		stats->connect = g_manager_info.stats[CB_STA_CONNECTED];
		stats->connectfail = g_manager_info.stats[CB_STA_CONNECT_FAILED];
		stats->disconnect = g_manager_info.stats[CB_STA_DISCONNECTED];
		stats->reconnect = g_manager_info.stats[CB_STA_RECONNECTED];
		stats->joined = g_manager_info.stats[CB_STA_JOINED];
		stats->left = g_manager_info.stats[CB_STA_LEFT];
		stats->scan = g_manager_info.stats[CB_SCAN_DONE];
		/* SoftAP mode has no callback */
		stats->softap = g_manager_info.stats[CB_SOFTAP_DONE];
		wret = WIFI_MANAGER_SUCCESS;
		UNLOCK_WIFIMGR;
	} else {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_INVALID_ARGUMENTS);
	}
	return wret;
}

wifi_manager_result_e wifi_manager_register_cb(wifi_manager_cb_s *wmcb)
{
	wifi_manager_result_e res = WIFI_MANAGER_FAIL;
	int i;

	LOCK_WIFIMGR;
	// g_manager_info.cb[0] is assigned to the callback which is registered by wifi_manager_init
	if (g_manager_info.cb[0] == NULL) {
		UNLOCK_WIFIMGR;
		return WIFI_MANAGER_DEINITIALIZED;
	}
	for (i = 1; i < WIFIMGR_NUM_CALLBACKS; i++) {
		if (g_manager_info.cb[i] == NULL) {
			g_manager_info.cb[i] = wmcb;
			res = WIFI_MANAGER_SUCCESS;
			break;
		}
	}
	UNLOCK_WIFIMGR;
	return res;
}

wifi_manager_result_e wifi_manager_unregister_cb(wifi_manager_cb_s *wmcb)
{
	wifi_manager_result_e res = WIFI_MANAGER_FAIL;
	int i;
	
	LOCK_WIFIMGR;
	// g_manager_info.cb[0] is assigned to the callback which is registered by wifi_manager_init
	for (i = 1; i < WIFIMGR_NUM_CALLBACKS; i++) {
		if (g_manager_info.cb[i] == wmcb) {
			g_manager_info.cb[i] = NULL;
			res = WIFI_MANAGER_SUCCESS;
			break;
		}
	}
	UNLOCK_WIFIMGR;
	return res;
}
