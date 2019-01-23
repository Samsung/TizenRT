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
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "logging/things_logger.h"
#include "utils/things_malloc.h"
#include "utils/things_string.h"
#include "utils/things_rtos_util.h"
#include "things_api.h"
#include "things_network.h"
#include "framework/things_data_manager.h"

#include "easy-setup/resource_handler.h"
#include "easy-setup/easysetup_manager.h"
#include "cloud/cloud_manager.h"
#include "things_ping.h"

#ifdef CONFIG_NETUTILS_NTPCLIENT
#include <protocols/ntpclient.h>
#endif

// Logging tag for module name.
#define TAG "[things_network]"

#define MAX_WIFI_CONNECT_RETRY_COUNT 3
unsigned short g_retry_connect_cnt = 0;

typedef void *(*pthread_func_type)(void *);

static char *app_ap_name = NULL;
static char *app_ip_addr = NULL;
volatile static bool is_connected_target_ap = false;
extern bool b_reset_continue_flag;

/* Initialize a static pthread mutex. */
static pthread_mutex_t g_ap_list_mutex = PTHREAD_MUTEX_INITIALIZER;
/* Global Varaible to hold home WiFi ap information. */
static wifi_manager_ap_config_s g_homeap_info;
/* Global variale to hold the wifi scan ap information. */
static access_point_info_s *g_wifi_scan_info;
/* Global variable to count the total number of wifi ap. */
static int g_wifi_count = 0;

static pthread_t h_thread_things_wifi_join = PTHREAD_ONCE_INIT;

#define IP_NULL_VAL     "0.0.0.0"

#ifdef CONFIG_NETUTILS_NTPCLIENT
static void ntp_link_error(void)
{
	THINGS_LOG_D(TAG, "ntp_link_error() callback is called");
}

static void sync_time_from_ntp(int guarantee_secs)
{
	THINGS_LOG_V(TAG, "T%d --> %s", getpid(), __FUNCTION__);

	if (ntpc_get_status() != NTP_RUNNING) {
		struct timespec pre_tp;
		clock_gettime(CLOCK_REALTIME, &pre_tp);

		struct timespec init_tp;
		init_tp.tv_sec = 0;
		init_tp.tv_nsec = 0;
		clock_settime(CLOCK_REALTIME, &init_tp);

		if (ntpc_start(NULL, 0, 0, ntp_link_error) < 0) {
			THINGS_LOG_E(TAG, "ntpc_start() failed");
		} else {
			THINGS_LOG_V(TAG, "ntpc_start() OK");
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
				THINGS_LOG_V(TAG, "ntpc_time sync done");
			} else {
				THINGS_LOG_E(TAG, "ntpc_time sync fail");
				clock_settime(CLOCK_REALTIME, &pre_tp);
			}
		}
	} else {
		THINGS_LOG_V(TAG, "ntpc already running");
	}
}
#endif

//--------------------------------------------------------------------------------
/**
 * Function definition for getting homeap configuration information.This function
 * calling the wifi manager api internally to get the configuration details.
 */
wifi_manager_ap_config_s *things_network_get_homeap_config(void)
{
	// Call the wifi manager api.
	wifi_manager_result_e res = wifi_manager_get_config(&g_homeap_info);

	if (res != WIFI_MANAGER_SUCCESS) {
		THINGS_LOG_E(TAG, "Get AP configuration failed [error code : %d]", res);
	} else {
		THINGS_LOG_D(TAG, "[WIFI] Saved SSID : %s", g_homeap_info.ssid);
	}
	return &g_homeap_info;
}

//--------------------------------------------------------------------------------
/**
 * Function definition for connect things network to home ap network.
 */
bool things_network_connect_home_ap(void)
{
	// Initialize the WiFi manager result with success.
	wifi_manager_result_e result = WIFI_MANAGER_SUCCESS;

	// Get home access point information.
	wifi_manager_ap_config_s *connect_config = things_network_get_homeap_config();

	if (connect_config == NULL || connect_config->ssid == NULL) {
		return false;
	}

	// Print the home ap ssid.
	THINGS_LOG_V(TAG, "Try_connect_home_ap [ssid : %s]", connect_config->ssid);

	// Call wifi manager api to connect home ap.
	result = wifi_manager_connect_ap(connect_config);
	if (result != WIFI_MANAGER_SUCCESS) {
		THINGS_LOG_E(TAG, "Failed to connect WiFi [Error Code : %d]", result);
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------
/**
 * Function definition for turn on soft ap.
 */
bool things_network_turn_on_soft_ap(void)
{
	wifi_manager_info_s info;	// Keep Wi-Fi Manager information
	// Get the Wi-Fi Manager information(ip4 address,mac address,rssi,mode).
	wifi_manager_get_info(&info);
	wifi_manager_remove_config();

	/**
	 * If info mode is not in soft ap mode than data manager get the soft ap configuration details.
	 * and after that wifi manager set the state mode in soft ap mode.
	 */
	if (info.mode != SOFTAP_MODE) {
		wifi_manager_softap_config_s *ap_config = dm_get_softap_wifi_config();
		if (wifi_manager_set_mode(SOFTAP_MODE, ap_config) != WIFI_MANAGER_SUCCESS) {
			THINGS_LOG_E(TAG, "Failed to change to SOFTAP mode");
			return false;
		}
	}
	pthread_mutex_init(&g_ap_list_mutex, NULL);
	THINGS_LOG_V(TAG, "In SOFTAP mode");
	return true;
}

//--------------------------------------------------------------------------------
/**
 * Function definition for handle stop soft ap.
 */
bool things_handle_stop_soft_ap(wifi_manager_ap_config_s *connect_config)
{
	// Initialize the WiFi manager result with success.
	wifi_manager_result_e result = WIFI_MANAGER_SUCCESS;
	// Keep Wi-Fi Manager information.
	wifi_manager_info_s info;
	// Get the Wi-Fi Manager information(ip4 address,mac address,rssi,mode).
	wifi_manager_get_info(&info);

	if (info.mode == SOFTAP_MODE) {
		// Set wifi manager state mode in station mode.
		if (wifi_manager_set_mode(STA_MODE, NULL) != WIFI_MANAGER_SUCCESS) {
			THINGS_LOG_E(TAG, "Failed to change to STA mode)");
			return false;
		}
		usleep(100000);
	}
	// Set the WiFi connect retry count to 0.
	g_retry_connect_cnt = 0;
	// Connect to Access point.
	result = wifi_manager_connect_ap(connect_config);
	if (result != WIFI_MANAGER_SUCCESS) {
		THINGS_LOG_E(TAG, "Failed to connect WiFi");
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------
/**
 * This function set the Wi-Fi manager ap configuration from access point.
 */
int things_set_ap_connection(access_point_info_s *p_info)
{
	THINGS_LOG_D(TAG, "T%d --> %s", getpid(), __FUNCTION__);

	if (p_info == NULL) {
		THINGS_LOG_E(TAG, "Invalid params");
		return -1;
	}
	// Keep Wi-Fi Manager access point configuration information.
	wifi_manager_ap_config_s connect_config;

	strncpy(connect_config.ssid, p_info->e_ssid, sizeof(connect_config.ssid));	// Set the ssid.
	connect_config.ssid_length = strlen(connect_config.ssid);
	strncpy(connect_config.passphrase, p_info->security_key, sizeof(connect_config.passphrase));	// Set the security key

	connect_config.passphrase_length = strlen(connect_config.passphrase);

	THINGS_LOG_V(TAG, "[%s] ssid : %s", __FUNCTION__, connect_config.ssid);

	// Set sec type
	if (strncmp(p_info->sec_type, SEC_TYPE_WEP, strlen(SEC_TYPE_WEP)) == 0) {
		connect_config.ap_auth_type = WIFI_MANAGER_AUTH_WEP_SHARED;
	} else if (strncmp(p_info->sec_type, SEC_TYPE_WPA_PSK, strlen(SEC_TYPE_WPA_PSK)) == 0) {
		connect_config.ap_auth_type = WIFI_MANAGER_AUTH_WPA_PSK;
	} else if (strncmp(p_info->sec_type, SEC_TYPE_WPA2_PSK, strlen(SEC_TYPE_WPA2_PSK)) == 0) {
		connect_config.ap_auth_type = WIFI_MANAGER_AUTH_WPA2_PSK;
	}
	// Set encryption crypto type
	if (strncmp(p_info->enc_type, ENC_TYPE_WEP_64, strlen(ENC_TYPE_WEP_64)) == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_WEP_64;
	} else if (strncmp(p_info->enc_type, ENC_TYPE_WEP_128, strlen(ENC_TYPE_WEP_128)) == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_WEP_128;
	} else if (strncmp(p_info->enc_type, ENC_TYPE_TKIP, strlen(ENC_TYPE_TKIP)) == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_TKIP;
	} else if (strncmp(p_info->enc_type, ENC_TYPE_AES, strlen(ENC_TYPE_AES)) == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_AES;
	} else if (strncmp(p_info->enc_type, ENC_TYPE_TKIP_AES, strlen(ENC_TYPE_TKIP_AES)) == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_TKIP_AND_AES;
	}

	// Call WiFi Manager to save AP configuration.
	wifi_manager_result_e res = wifi_manager_save_config(&connect_config);

	if (res != WIFI_MANAGER_SUCCESS) {
		THINGS_LOG_E(TAG, "Failed to save AP configuration : [%d]", res);
		return -1;
	} else {
		THINGS_LOG_V(TAG, "Success to save AP configuration");
	}

	if (!things_handle_stop_soft_ap(&connect_config)) {
		return -1;
	}

	return 1;
}

//--------------------------------------------------------------------------------
/**
 * Function definition for get ap list from globally stored scan list.
 * When things want to get the ap scan results that time this api is call and send
 * the result to the things.
 */
int things_get_ap_list(access_point_info_s** p_info, int* p_count)
{
	if (p_info == NULL || p_count == NULL) {
		THINGS_LOG_E(TAG, "Can't Call GetAPSearchList(p_info=0x%X, p_count=0x%X).", p_info, p_count);
		return 0;
	}
	pthread_mutex_lock(&g_ap_list_mutex);
	// Copy the total wifi count list from global stored value.
	*p_count = g_wifi_count;

	// Copy the starting location of wifi scan list from global stored list.
	access_point_info_s *wifi_scan_iter = g_wifi_scan_info;
	access_point_info_s *pinfo = NULL;
	access_point_info_s *p_last_info = NULL;

	// Copy the globally scan result into p_info list.
	while (wifi_scan_iter != NULL) {
		if (wifi_scan_iter->e_ssid != NULL) {
			// Create dynamic memory for pinfo list.
			pinfo = (access_point_info_s*)things_malloc(sizeof(access_point_info_s));
			pinfo->next = NULL;
			snprintf(pinfo->e_ssid, WIFIMGR_SSID_LEN, "%s", wifi_scan_iter->e_ssid);	// Set the ssid.
			snprintf(pinfo->bss_id, WIFIMGR_MACADDR_STR_LEN, "%s", wifi_scan_iter->bss_id);	// Set the mac address.
			snprintf(pinfo->signal_level, MAX_LEVEL_SIGNAL, "%d", wifi_scan_iter->signal_level);	// Set the signal level.
			snprintf(pinfo->sec_type, MAX_TYPE_SEC, "%s", wifi_scan_iter->sec_type);		// Set the sec type.
			snprintf(pinfo->enc_type, MAX_TYPE_ENC, "%s", wifi_scan_iter->enc_type);		// Set the enc type.
			if (*p_info == NULL) {
				*p_info = pinfo;
			} else {
				p_last_info->next = pinfo;
			}
			p_last_info = pinfo;
		}
		// Move to the 2nd element of the list.
		wifi_scan_iter = wifi_scan_iter->next;
	}
	pthread_mutex_unlock(&g_ap_list_mutex);
	return 1;
}

//--------------------------------------------------------------------------------
/**
 * Callback definition for changed the WiFi state(0 to 1 or 1 to 0).
 * State : 1 -> Connected to WiFi AP.
 *        0 -> Disconnected from WiFi AP.
 */
int things_wifi_changed_call_func(int state, char *ap_name, char *ip_addr)
{
	return things_wifi_state_changed_cb(state, ap_name, ip_addr);;
}

//--------------------------------------------------------------------------------
static void *__attribute__((optimize("O0"))) t_things_wifi_join_loop(void *args)
{
	bool* is_wifi_retry_connect = (bool*)args;

	if ((*is_wifi_retry_connect) == true) {
		THINGS_LOG_E(TAG, "Retry to connect to home ap.");

		if (things_network_connect_home_ap() == false) {
			THINGS_LOG_E(TAG, "Failed to connect home ap.");
		}
	} else {
#ifdef CONFIG_NETUTILS_NTPCLIENT
		sync_time_from_ntp(10);
#else
		THINGS_LOG_V(TAG, "CONFIG_NETUTILS_NTPCLIENT is not set");
#endif
		wifi_manager_info_s wifi_info;
		wifi_manager_get_info(&wifi_info);

		/**
		 * Call the callback to change the WiFi state. Here change the WiFi state
		 * to connected AP.
		 */
		things_wifi_changed_call_func(1, wifi_info.ssid, wifi_info.ip4_address);
	}

	return NULL;
}

//--------------------------------------------------------------------------------
/**
 * Function definiton for callback of wifi connected to AP in station mode.
 */
void things_wifi_sta_connected(wifi_manager_result_e res)
{
	/* If WiFi is failed to connect to AP.Then it will retry to connect to home AP */
	bool is_wifi_retry_connect = false;
	if (res == WIFI_MANAGER_FAIL) {
		THINGS_LOG_E(TAG, "Failed to connect to the AP");

		if (!dm_is_es_complete()) {
			g_retry_connect_cnt++;
			// Retry to connect to AP during easy-setup.
			if (g_retry_connect_cnt <= MAX_WIFI_CONNECT_RETRY_COUNT) {
				/**
				 * Set this variable to true for calling the api for reconnect to home AP.
				 */
				is_wifi_retry_connect = true;
				THINGS_LOG_E(TAG, "(%d) Retry to connect ap during easy-setup.", g_retry_connect_cnt);
			} else {
				return;
			}
		} else {
			// Retry to connect home AP during normal time.
			is_wifi_retry_connect = true;
			THINGS_LOG_V(TAG, "Retry to connect ap during normal time.");
		}
	} else {
		/* If WiFi is connected to home AP.No need to retry to connect to home AP */
		is_wifi_retry_connect = false;
		THINGS_LOG_V(TAG, "Stop retry to connect ap.");
	}

	THINGS_LOG_V(TAG, "T%d --> %s", getpid(), __FUNCTION__);

	// If things stack is initialzed.Then create a thread to wifi join.
	if (things_is_things_module_initialized() == 1) {
		pthread_create_rtos(&h_thread_things_wifi_join, NULL, (pthread_func_type)t_things_wifi_join_loop, (void *)&is_wifi_retry_connect, THINGS_STACK_WIFI_JOIN_THREAD);
	} else {
		THINGS_LOG_E(TAG, "things module was not initialized.");
	}
}

//--------------------------------------------------------------------------------
/**
 * Function definiton for callback of wifi disconnected to AP in station mode.
 */
void things_wifi_sta_disconnected(wifi_manager_disconnect_e disconn)
{
	THINGS_LOG_V(TAG, "T%d --> %s", getpid(), __FUNCTION__);
	things_wifi_changed_call_func(0, NULL, NULL);
}

//--------------------------------------------------------------------------------
/**
 * Function definiton for callback of wifi, joined station in soft ap mode .
 */
void things_wifi_soft_ap_sta_joined(void)
{
	THINGS_LOG_V(TAG, "T%d --> %s", getpid(), __FUNCTION__);

}

//--------------------------------------------------------------------------------
/**
 * Function definiton for callback of wifi, left station in soft ap mode.
 */
void things_wifi_soft_ap_sta_left(void)
{
	THINGS_LOG_V(TAG, "T%d --> %s", getpid(), __FUNCTION__);
}

//--------------------------------------------------------------------------------
/**
 * Function definition for clean the global scan list.
 */
void things_wifi_list_cleanup(void)
{
	if (g_wifi_scan_info != NULL) {
		// Free old wifi list
		access_point_info_s* piter;
		access_point_info_s* pdel;
		// Copy the starting location of list.
		piter = pdel = g_wifi_scan_info;
		while (piter != NULL) {
			pdel = piter;
			piter = piter->next;	// Move to the next node.
			things_free(pdel);	// Dlete the current node.
		}
		g_wifi_scan_info = NULL;	// Make the global scan list NULL
	}
	g_wifi_count = 0;		// Set the count of wifi scan list to 0.
}

//--------------------------------------------------------------------------------
/**
 * Function definiton for callback of wifi, scanning ap is done.
 */
void things_wifi_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res)
{
	THINGS_LOG_V(TAG, "T%d --> %s", getpid(), __FUNCTION__);
	/**
	 *  Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (scan_result == NULL) {
		return;
	}
	pthread_mutex_lock(&g_ap_list_mutex);

	/**
	 *  First it clean the existing global scan list.After that it start
	 * doing wifi scan.
	 */
	things_wifi_list_cleanup();
	wifi_manager_scan_info_s *wifi_scan_iter = *scan_result;
	access_point_info_s *pinfo = NULL;
	access_point_info_s *p_last_info = NULL;
	/**
	 * Stored the wifi scan result into globally defined list g_wifi_scan_info. and
	 * also update the globally wifi list count variable.
	 */
	while (wifi_scan_iter != NULL) {
		if ((wifi_scan_iter->ssid != NULL) && (strlen(wifi_scan_iter->ssid)) != 0) {
			// Create dynamic memory for pinfo list.
			pinfo = (access_point_info_s*)things_malloc(sizeof(access_point_info_s));
			pinfo->next = NULL;
			// Set the ssid.
			snprintf(pinfo->e_ssid, WIFIMGR_SSID_LEN, "%s", wifi_scan_iter->ssid);
			// Set the bssid(mac address).
			snprintf(pinfo->bss_id, WIFIMGR_MACADDR_STR_LEN, "%s", wifi_scan_iter->bssid);
			// Set the signal level.
			snprintf(pinfo->signal_level, MAX_LEVEL_SIGNAL, "%d", wifi_scan_iter->rssi);
			// Set auth type
			const char *sec_type;
			if (wifi_scan_iter->ap_auth_type == WIFI_MANAGER_AUTH_WEP_SHARED) {
				sec_type = SEC_TYPE_WEP;					// Use Shared key(WEP key)
			} else if (wifi_scan_iter->ap_auth_type == WIFI_MANAGER_AUTH_WPA_PSK) {
				sec_type = SEC_TYPE_WPA_PSK;					// WPA_PSK mode
			} else if (wifi_scan_iter->ap_auth_type == WIFI_MANAGER_AUTH_WPA2_PSK) {
				sec_type = SEC_TYPE_WPA2_PSK;					// WPA2_PSK mode
			} else {
				sec_type = SEC_TYPE_NONE;
			}
			snprintf(pinfo->sec_type, MAX_TYPE_SEC, "%s", sec_type);

			// Set encryption type
			const char *enc_type;
			if (wifi_scan_iter->ap_crypto_type == WIFI_MANAGER_CRYPTO_WEP_64) {
				enc_type = ENC_TYPE_WEP_64;					// WEP encryption wep-40
			} else if (wifi_scan_iter->ap_crypto_type == WIFI_MANAGER_CRYPTO_WEP_128) {
				enc_type = ENC_TYPE_WEP_128;					// WEP encryption wep-128
			} else if (wifi_scan_iter->ap_crypto_type == WIFI_MANAGER_CRYPTO_TKIP) {
				enc_type = ENC_TYPE_TKIP;					// TKIP encryption
			} else if (wifi_scan_iter->ap_crypto_type == WIFI_MANAGER_CRYPTO_AES) {
				enc_type = ENC_TYPE_AES;					// AES encryption
			} else if (wifi_scan_iter->ap_crypto_type == WIFI_MANAGER_CRYPTO_TKIP_AND_AES) {
				enc_type = ENC_TYPE_TKIP_AES;					// TKIP and AES mixed encryption
			} else {
				enc_type = ENC_TYPE_NONE;
			}
			snprintf(pinfo->enc_type, MAX_TYPE_ENC, "%s", enc_type);
			if (g_wifi_scan_info == NULL) {
				g_wifi_scan_info = pinfo;
			} else {
				p_last_info->next = pinfo;
			}
			p_last_info = pinfo;
			// Increment global scan list count
			g_wifi_count++;
		}
		wifi_scan_iter = wifi_scan_iter->next;
	}		/* while (wifi_scan_iter != NULL) to endicate end of while */
	pinfo = g_wifi_scan_info;
	// Print the all near by Wifi scan access point.
	while (pinfo != NULL) {
		THINGS_LOG_V(TAG, "WiFi AP - SSID: %-20s, WiFi AP BSSID: %-20s, WiFi Rssi: %d",
				   pinfo->e_ssid, pinfo->bss_id, pinfo->signal_level);
		pinfo = pinfo->next;
	}
	THINGS_LOG_V(TAG, "WiFi List Count is (%d)", g_wifi_count);
	pthread_mutex_unlock(&g_ap_list_mutex);
	return;
}

//--------------------------------------------------------------------------------
/**
 * Function definition for calling the wifi manager scan api.
 */
int things_wifi_scan_ap(void)
{
	// Call the WiFi manager scan API.
	if (wifi_manager_scan_ap() != WIFI_MANAGER_SUCCESS) {
		THINGS_LOG_E(TAG, "Wifi manager scan ap failed");
		return 0;
	}
	return 1;
}

//--------------------------------------------------------------------------------
/**
 * Define the WiFi manager callbacks which are asynchronously called afer WiFi
 * Manager are called..
 */
static const wifi_manager_cb_s wifi_callbacks = {
	things_wifi_sta_connected,		// In station mode, connected to AP.
	things_wifi_sta_disconnected,		// In station mode, disconnected from AP.
	things_wifi_soft_ap_sta_joined,		// In soft AP mode, a station joined.
	things_wifi_soft_ap_sta_left,		// In soft AP mode, a station left.
	things_wifi_scan_done,			// Scanning AP is done.
};

//--------------------------------------------------------------------------------
/**
 * Function definition for things_network_initialize.It initialize the WiFi manager
 *callbacks.
 */
int things_network_initialize(void)
{
	// Call the WiFi manager api to initialize the wifi callbacks.
	if (wifi_manager_init((wifi_manager_cb_s *) &wifi_callbacks) != WIFI_MANAGER_SUCCESS) {
		THINGS_LOG_E(TAG, "Failed to initialize WiFi manager");
		return 0;
	}
	return 1;
}

//--------------------------------------------------------------------------------
/**
 * It's called from st_things Application when WiFi state changed.
 * state : 1 -> Connected to WiFi AP.
 *         0 -> Disconnected from WiFi AP.
 * ap_name : AP SSID.
 * ip_addr : IP address.
 */
int things_wifi_state_changed_cb(int state, char *ap_name, char *ip_addr)
{
	bool Retry1Stop0 = false;

	THINGS_LOG_D(TAG, "state=%d, ap_name=%s, ip_name=%s", state, ap_name, ip_addr);

	// Process.
	/**
	 * [Info] Always, when Soft-AP enabling, Call-Back State maintain disconnected.
	 * So, Soft-AP Check is not need.
	 */
	if (state == 1) {
		THINGS_LOG_D(TAG, "AP CONNECTED.");
		is_connected_target_ap = true;

		set_ssid_in_wifi_resource(app_ap_name);
		Retry1Stop0 = true;		// Re-Try with Last failed Access info.
		esm_wifi_prov_check_cb(state, app_ap_name, app_ip_addr);
	} else {
		THINGS_LOG_D(TAG, "AP DISCONNECTED.");
		is_connected_target_ap = false;
		set_ssid_in_wifi_resource(NULL);
	}

	// Call Retry or forceStop Connection
	ci_retry_stop_by_wifi_cb(Retry1Stop0);

	return 1;
}

//--------------------------------------------------------------------------------
/**
 * It's called from Iotivity Framework when Adaptor state changed.
 * adapter : Adapter Type.
 * enabled  : true -> connected.
 *            false-> disconnected.
 */
void things_adapter_state_cb(CATransportAdapter_t adapter, bool enabled)
{
	(void)adapter;

	THINGS_LOG_D(TAG, "IN");

	if (enabled) {
		THINGS_LOG_D(TAG, "CONNECTED but Not Guarantee to be Communication-Available");
	} else {
		THINGS_LOG_D(TAG, "AP DISCONNECTED.");
		is_connected_target_ap = false;
		set_ssid_in_wifi_resource(NULL);
		ci_retry_stop_by_wifi_cb(false);
	}

	THINGS_LOG_D(TAG, "OUT");
}

//-------------------------------------------------------------------------------
/**
 * It's called from Iotivity Framework when TCP session state changed.
 * info : information about remote-peer.
 * connected  : true -> connected
 *              false-> disconnected
 */
void things_tcp_session_state_cb(const CAEndpoint_t *info, bool connected)
{
	THINGS_LOG_D(TAG, "Enter.");

	if (b_reset_continue_flag == true) {
		return;
	}

	// If does not have any endpoint information regarding connectivity.
	if (info == NULL) {
		THINGS_LOG_E(TAG, "[IoTivity Error] invalid call-back parameter.(info is null.)");
		return;
	}

	// If endpoint does not support any TCP adapter.
	if (info->adapter != CA_ADAPTER_TCP) {
		THINGS_LOG_D(TAG, "Don't support adapter type.(%d)", info->adapter);
		return;
	}

	// If endpoint does not have IP address and port number.
	if (strlen(info->addr) == 0 || info->port == 0) {
		THINGS_LOG_E(TAG, "[IoTivity Error] invalid call-back parameter.(addrIP=%s, port=%d)", info->addr, info->port);
		return;
	}

	// Print the IP address and port number.
	THINGS_LOG_D(TAG, "IP = %s, port = %d", info->addr, info->port);

	// If TCP session connected to remote-peer. Then set ping bit mask state.
	if (connected == true) {
		THINGS_LOG_D(TAG, "CONNECTED");
		things_ping_set_mask(info->addr, info->port, PING_ST_TCPCONNECT);
		return;
	}

	// If TCP session disconnected from remote-peer. Then unset ping bit mask state.
	THINGS_LOG_D(TAG, "DISCONNECTED");
	things_ping_unset_mask(info->addr, PING_ST_SIGNIN | PING_ST_TCPCONNECT);

	if (ci_retry_stop_by_tcp_cb(info->addr, info->port) == -1) {
		THINGS_LOG_E(TAG, "[Error] System Error.");
		return;
	}

	THINGS_LOG_D(TAG, "Exit.");
}


//--------------------------------------------------------------------------------
/**
 * Ready Check All of Network APIs.
 * return : 0 --> Not Ready.
 *          1 --> Initialize Success.
 *         -1 --> Initialize Failed.
 */
bool things_is_connected_ap(void)
{
	return is_connected_target_ap;
}
