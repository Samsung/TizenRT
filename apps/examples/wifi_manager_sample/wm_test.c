/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <wifi_manager/wifi_manager.h>

#define WM_TEST_COUNT  1

#define USAGE															\
	"\n usage: wm_test [options]\n"										\
	"\n run Wi-Fi Manager:\n"											\
	"	 wm_test start(default: station mode)\n"						\
	"	 wm_test stop\n"												\
	"\n softap mode options:\n"											\
	"	 wm_test softap [ssid] [password]\n"							\
	"\n station mode options:\n"										\
	"	 wm_test sta\n"													\
	"	 wm_test join [ssid] [security mode] [password]\n"				\
	"	 wm_test leave\n"												\
	"	 wm_test cancel\n"												\
	"\n run scan:\n"													\
	"	 wm_test scan\n"												\
	"\n get current state:\n"											\
	"	 wm_test mode\n\n"												\
	"\n repeat test:\n"													\
	"	 wm_test auto [softap ssid] [softap password] [ssid] [security mode] [password]\n\n\n"

typedef void (*test_func)(void *arg);

struct options {
	test_func func;
	uint16_t channel;
	char *ssid;
	char *password;
	wifi_manager_ap_auth_type_e    auth_type;
	wifi_manager_ap_crypto_type_e  crypto_type;
	char *softap_ssid;
	char *softap_password;
};

/**
 * Functions
 */
/*
 * Signal
 */
void wm_signal_deinit(void);
int wm_signal_init(void);
/*
 * callbacks
 */
static void wm_sta_connected(wifi_manager_result_e);
static void wm_sta_disconnected(void);
static void wm_softap_sta_join(void);
static void wm_softap_sta_leave(void);
static void wm_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res);

/*
 * handler
 */
static void wm_auto_test(void *arg); // it tests softap mode and stations mode repeatedly
static void wm_start(void *arg);
static void wm_stop(void *arg);
static void wm_softap_start(void *arg);
static void wm_connect(void *arg);
static void wm_disconnect(void *arg);
static void wm_cancel(void *arg); // stop reconnecting to wifi AP. it doesn't expect to receive a signal because AP is already disconnected
static void wm_scan(void *arg);
static void wm_display_state(void *arg);
static void wm_process(int argc, char *argv[]);

static int wm_parse_commands(struct options *opt, int argc, char *argv[]);

/*
 * Global
 */
static wifi_manager_cb_s wifi_callbacks = {
	wm_sta_connected,
	wm_sta_disconnected,
	wm_softap_sta_join,
	wm_softap_sta_leave,
	wm_scan_done,
};

static pthread_mutex_t g_wm_mutex = PTHREAD_MUTEX_INITIALIZER;;
static pthread_cond_t g_wm_cond;
static pthread_mutex_t g_wm_func_mutex = PTHREAD_MUTEX_INITIALIZER;;
static pthread_cond_t g_wm_func_cond;
static int g_mode = 0; // check program is running

#define SET_SOFTAP_OPTION(opt, func, ssid, pwd)	\
	do {										\
		opt->func = func;						\
		opt->ssid = ssid;						\
		opt->password = pwd;					\
	} while (0);

#define SET_STA_OPTION(opt, func, ssid, auth, crypto, pwd)	\
	do {													\
		opt->func = func;									\
		opt->ssid = ssid;									\
		opt->auth_type = auth;								\
		opt->crypto_type = crypto;							\
		opt->password = pwd;								\
	} while (0);

#define WM_TEST_TIME_DIFF												\
	(g_end_time.tv_sec - g_st_time.tv_sec)*1000 + (g_end_time.tv_usec - g_st_time.tv_usec)/1000

#define WM_TEST_SIGNAL										\
	do {													\
		pthread_mutex_lock(&g_wm_mutex);					\
		printf("T%d send signal\n", getpid());	\
		pthread_cond_signal(&g_wm_cond);					\
		pthread_mutex_unlock(&g_wm_mutex);					\
	} while (0)

#define WM_TEST_WAIT								\
	do {											\
		pthread_mutex_lock(&g_wm_mutex);			\
		printf(" T%d wait signal\n", getpid());		\
		pthread_cond_wait(&g_wm_cond, &g_wm_mutex);	\
		pthread_mutex_unlock(&g_wm_mutex);			\
	} while (0)

#define WM_TEST_FUNC_SIGNAL								\
	do {												\
		pthread_mutex_lock(&g_wm_func_mutex);			\
		printf(" T%d send func signal\n", getpid());	\
		pthread_cond_signal(&g_wm_func_cond);			\
		pthread_mutex_unlock(&g_wm_func_mutex);			\
	} while (0)

#define WM_TEST_FUNC_WAIT										\
	do {														\
		pthread_mutex_lock(&g_wm_func_mutex);					\
		printf(" T%d wait func signal\n", getpid());			\
		pthread_cond_wait(&g_wm_func_cond, &g_wm_func_mutex);	\
		pthread_mutex_unlock(&g_wm_func_mutex);					\
	} while (0)

#define WM_TEST_LOG_START						\
	do {										\
		printf("-->%s\n", __FUNCTION__);		\
	} while (0)


#define WM_TEST_LOG_END							\
	do {										\
		printf("<--%s\n", __FUNCTION__);		\
	} while (0)

/*
 * supported security method
 */
static const char *wifi_test_auth_method[] = {
	"open",
	"wep_shared",
	"wpa_aes",
	"wpa2_aes",
	"wpa2_mixed",
};


static const wifi_manager_ap_auth_type_e auth_type_table[] = {
	WIFI_MANAGER_AUTH_OPEN,                    /**<  open mode                      */
	WIFI_MANAGER_AUTH_WEP_SHARED,              /**<  use shared key (wep key)       */
	WIFI_MANAGER_AUTH_WPA_PSK,                 /**<  WPA_PSK mode                   */
	WIFI_MANAGER_AUTH_WPA2_PSK,                /**<  WPA2_PSK mode                  */
	WIFI_MANAGER_AUTH_WPA_AND_WPA2_PSK,        /**<  WPA_PSK and WPA_PSK mixed mode */
	WIFI_MANAGER_AUTH_UNKNOWN,                 /**<  unknown type                   */
};


static const wifi_manager_ap_crypto_type_e crypto_type_table[] = {
	WIFI_MANAGER_CRYPTO_NONE,                  /**<  none encryption                */
	WIFI_MANAGER_CRYPTO_UNKNOWN,               /**<  to be fixed            */
	WIFI_MANAGER_CRYPTO_AES,                   /**<  AES encryption                 */
	WIFI_MANAGER_CRYPTO_AES,                   /**<  AES encryption                 */
	WIFI_MANAGER_CRYPTO_AES,                   /**<  AES encryption                 */
	WIFI_MANAGER_CRYPTO_UNKNOWN,               /**<  unknown encryption             */
};

void print_wifi_ap_profile(wifi_manager_ap_config_s *config, char *title)
{
	printf("====================================\n");
	if (title) {
		printf("%s\n", title);
	}
	printf("------------------------------------\n");
	printf("SSID: %s\n", config->ssid);
	printf("SECURITY TYPE: %s\n", wifi_test_auth_method[config->ap_auth_type]);
	/*if (config->ap_auth_type != WIFI_MANAGER_AUTH_OPEN) {
		printf("PASSWORD: %s\n", config->passphrase);
		}*/
	printf("====================================\n");
}

void print_wifi_softap_profile(wifi_manager_softap_config_s *config, char *title)
{
	printf("====================================\n");
	if (title) {
		printf("%s\n", title);
	}
	printf("------------------------------------\n");
	printf("SSID: %s\n", config->ssid);
	printf("channel: %d\n", config->channel);
	/*printf("PASSWORD: %s\n", config->passphrase);*/
	printf("====================================\n");
}


wifi_manager_ap_auth_type_e get_auth_type(const char *method)
{
	int i = 0;
	int list_size = sizeof(wifi_test_auth_method)/sizeof(wifi_test_auth_method[0]);
	for (; i < list_size; i++) {
		if (strcmp(method, wifi_test_auth_method[i]) == 0) {
			return auth_type_table[i];
		}
	}
	return WIFI_MANAGER_AUTH_UNKNOWN;
}


wifi_manager_ap_crypto_type_e get_crypto_type(const char *method)
{
	int i = 0;
	int list_size = sizeof(wifi_test_auth_method)/sizeof(wifi_test_auth_method[0]);
	for (; i < list_size; i++) {
		if (strcmp(method, wifi_test_auth_method[i]) == 0) {
			return crypto_type_table[i];
		}
	}
	return WIFI_MANAGER_CRYPTO_UNKNOWN;
}


int wm_signal_init(void)
{
	if (g_mode != 0) {
		printf("Program is already running\n");
		return -1;
	}
	g_mode = 1;
	int res = pthread_mutex_init(&g_wm_func_mutex, NULL);
	if (res != 0) {
		printf(" Pthread mutex func init fail(%d) (%d)\n", res, errno);
		return -1;
	}
	res = pthread_cond_init(&g_wm_func_cond, NULL);
	if (res != 0) {
		printf(" Conditional mutex func init fail(%d) (%d)\n", res, errno);
		return -1;
	}

	res = pthread_mutex_init(&g_wm_mutex, NULL);
	if (res != 0) {
		printf(" Pthread mutex init fail(%d) (%d)\n", res, errno);
		return -1;
	}

	res = pthread_cond_init(&g_wm_cond, NULL);
	if (res != 0) {
		printf(" Conditional mutex init fail(%d) (%d)\n", res, errno);
		return -1;
	}

	return 0;
}


void wm_signal_deinit(void)
{
	pthread_mutex_destroy(&g_wm_func_mutex);
	pthread_cond_destroy(&g_wm_func_cond);
	pthread_mutex_destroy(&g_wm_mutex);
	pthread_cond_destroy(&g_wm_cond);
	g_mode = 0;
}


/*
 * Callback
 */
void wm_sta_connected(wifi_manager_result_e res)
{
	printf(" T%d --> %s res(%d)\n", getpid(), __FUNCTION__, res);
	WM_TEST_SIGNAL;
}


void wm_sta_disconnected(void)
{
	sleep(2);
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
	WM_TEST_SIGNAL;
}


void wm_softap_sta_join(void)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
	WM_TEST_SIGNAL;
}


void wm_softap_sta_leave(void)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
	WM_TEST_SIGNAL;
}


void wm_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (scan_result == NULL) {
		WM_TEST_SIGNAL;
		return;
	}
	wifi_manager_scan_info_s *wifi_scan_iter = *scan_result;
	while (wifi_scan_iter != NULL) {
		printf("WiFi AP SSID: %-20s, WiFi AP BSSID: %-20s, WiFi Rssi: %d\n",
			   wifi_scan_iter->ssid, wifi_scan_iter->bssid, wifi_scan_iter->rssi);
		wifi_scan_iter = wifi_scan_iter->next;
	}
	WM_TEST_SIGNAL;
}


/*
 * Control Functions
 */
void wm_reset_info(void *arg)
{
	WM_TEST_LOG_START;
	wifi_manager_result_e res = wifi_manager_remove_config();
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("Get AP configuration failed\n");
		return;
	}

	WM_TEST_LOG_END;
}


void wm_get_info(void *arg)
{
	WM_TEST_LOG_START;
	wifi_manager_ap_config_s apconfig;
	wifi_manager_result_e res = wifi_manager_get_config(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("Get AP configuration failed\n");
		return;
	}
	print_wifi_ap_profile(&apconfig, "Stored Wi-Fi Infomation");

	WM_TEST_LOG_END;
}

void wm_set_info(void *arg)
{
	WM_TEST_LOG_START;
	struct options *ap_info = (struct options *)arg;
	wifi_manager_ap_config_s apconfig;
	strncpy(apconfig.ssid, ap_info->ssid, 33);
	apconfig.ssid_length = strlen(ap_info->ssid);
	apconfig.ap_auth_type = ap_info->auth_type;
	if (apconfig.ap_auth_type != WIFI_MANAGER_AUTH_OPEN) {
		strncpy(apconfig.passphrase, ap_info->password, 64);
		apconfig.passphrase_length = strlen(ap_info->password);
		apconfig.ap_crypto_type = ap_info->crypto_type;
	}

	print_wifi_ap_profile(&apconfig, "Set AP Info");

	wifi_manager_result_e res = wifi_manager_save_config(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("Save AP configuration failed\n");
		return;
	}
	WM_TEST_LOG_END;
}

void wm_start(void *arg)
{
	WM_TEST_LOG_START;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	res = wifi_manager_init(&wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" wifi_manager_init fail\n");
	}
	WM_TEST_LOG_END;
}


void wm_stop(void *arg)
{
	WM_TEST_LOG_START;
	wifi_manager_result_e res = wifi_manager_deinit();
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" WiFi Manager failed to stop\n");
	}
	WM_TEST_LOG_END;
}


void wm_scan(void *arg)
{
	WM_TEST_LOG_START;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	res = wifi_manager_scan_ap();
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" scan Fail\n");
		return ;
	}
	WM_TEST_WAIT; // wait the scan result
	WM_TEST_LOG_END;
}


void wm_display_state(void *arg)
{
	WM_TEST_LOG_START;
	wifi_manager_info_s info;
	char mac_str[20];
	char mac_char[6];
	wifi_manager_result_e res = wifi_manager_get_info(&info);
	if (res != WIFI_MANAGER_SUCCESS) {
		goto exit;
	}
	if (info.mode == SOFTAP_MODE) {
		if (info.status == CLIENT_CONNECTED) {
			printf("MODE: softap (client connected)\n");
		} else if (info.status == CLIENT_DISCONNECTED) {
			printf("MODE: softap (no client)\n");
		}
		printf("IP: %s\n", info.ip4_address);
		printf("SSID: %s\n", info.ssid);
		res = wifi_manager_mac_addr_to_mac_str(info.mac_address, mac_str);
		if (res != WIFI_MANAGER_SUCCESS) {
			goto exit;
		}
		printf("MAC: %s\n", mac_str);
		res = wifi_manager_mac_str_to_mac_addr(mac_str, mac_char);
		if (res != WIFI_MANAGER_SUCCESS) {
			goto exit;
		}
	} else if (info.mode == STA_MODE) {
		if (info.status == AP_CONNECTED) {
			printf("MODE: station (connected)\n");
			printf("IP: %s\n", info.ip4_address);
			printf("SSID: %s\n", info.ssid);
			printf("rssi: %d\n", info.rssi);
		} else if (info.status == AP_DISCONNECTED) {
			printf("MODE: station (disconnected)\n");
		}
		res = wifi_manager_mac_addr_to_mac_str(info.mac_address, mac_str);	
		if (res != WIFI_MANAGER_SUCCESS) {
			goto exit;
		}
		printf("MAC: %s\n", mac_str);
		res = wifi_manager_mac_str_to_mac_addr(mac_str, mac_char);
		if (res != WIFI_MANAGER_SUCCESS) {
			goto exit;
		}
	} else {
		printf("STATE: NONE\n");
	}
exit:
	WM_TEST_LOG_END;
	return ;
}


void wm_sta_start(void *arg)
{
	WM_TEST_LOG_START;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	res = wifi_manager_set_mode(STA_MODE, NULL);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" Set STA mode Fail\n");
		return ;
	}
	printf(" Connecting to AP\n");
	WM_TEST_LOG_END;
}


void wm_connect(void *arg)
{
	WM_TEST_LOG_START;
	struct options *ap_info = (struct options *)arg;
	wifi_manager_ap_config_s apconfig;
	strncpy(apconfig.ssid, ap_info->ssid, 33);
	apconfig.ssid_length = strlen(ap_info->ssid);
	apconfig.ap_auth_type = ap_info->auth_type;
	if (ap_info->auth_type != WIFI_MANAGER_AUTH_OPEN) {
		strncpy(apconfig.passphrase, ap_info->password, 64);
		apconfig.passphrase_length = strlen(ap_info->password);
		apconfig.ap_crypto_type = ap_info->crypto_type;
	}

	print_wifi_ap_profile(&apconfig, "Connecting AP Info");

	wifi_manager_result_e res = wifi_manager_connect_ap(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" AP connect failed\n");
		return;
	}
	/* Wait for DHCP connection */
	printf(" wait join done\n");
	WM_TEST_WAIT;

	WM_TEST_LOG_END;
}


void wm_disconnect(void *arg)
{
	WM_TEST_LOG_START;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	/* Disconnect AP */
	res = wifi_manager_disconnect_ap();
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("disconnect fail (%d)\n", res);
		return;
	}
	WM_TEST_WAIT;
	WM_TEST_LOG_END;;
}


void wm_cancel(void *arg)
{
	WM_TEST_LOG_START;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	/* Disconnect AP */
	res = wifi_manager_disconnect_ap();
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("disconnect fail (%d)\n", res);
		return;
	}
	WM_TEST_LOG_END;
}


void wm_softap_start(void *arg)
{
	WM_TEST_LOG_START;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	struct options *ap_info = (struct options *)arg;
	wifi_manager_softap_config_s ap_config;
	strcpy(ap_config.ssid, ap_info->ssid);
	ap_config.ssid[strlen(ap_info->ssid)] = '\0';
	strcpy(ap_config.passphrase, ap_info->password);
	ap_config.passphrase[strlen(ap_info->password)] = '\0';
	ap_config.channel = 1;

	print_wifi_softap_profile(&ap_config, "AP INFO");

	res = wifi_manager_set_mode(SOFTAP_MODE, &ap_config);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" Run SoftAP Fail\n");
	}
	WM_TEST_LOG_END;
}



/****************************************************************************
 * ocf_wifi_test
 ****************************************************************************/

void wm_auto_test(void *arg)
{
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	struct options *info = (struct options *)arg;

	/* Set SoftAP Configuration */
	wifi_manager_softap_config_s softap_config;
	strncpy(softap_config.ssid, info->softap_ssid, 33);
	softap_config.ssid[strlen(info->ssid)] = '\0';
	strncpy(softap_config.passphrase, info->softap_password, 64);
	softap_config.passphrase[strlen(info->password)] = '\0';
	softap_config.channel = 1;

	/* Set AP Configuration */
	wifi_manager_ap_config_s ap_config;
	strncpy(ap_config.ssid, info->ssid, 33);
	ap_config.ssid_length = strlen(info->ssid);
	strncpy(ap_config.passphrase, info->password, 64);
	ap_config.passphrase_length = strlen(info->password);
	ap_config.ap_auth_type = info->auth_type;
	ap_config.ap_crypto_type = info->crypto_type;

	/* Set FAKE AP Configuration */
	char fake_passphrase[] = "fakePasswd1#";
	char fake_long_ssid[] = "fakefakefakefakefakefakefakeNewAP";
	char fake_ssid[] = "fakeNewAP";
	wifi_manager_ap_config_s fake_config;
	fake_config.ssid_length = strlen(fake_long_ssid);
	fake_config.passphrase_length = strlen(fake_passphrase);
	strncpy(fake_config.ssid, fake_long_ssid, fake_config.ssid_length + 1);
	strncpy(fake_config.passphrase, fake_passphrase, fake_config.passphrase_length + 1);
	fake_config.ap_auth_type = 2;
	fake_config.ap_crypto_type = 3;
	
	printf("Init WiFi (default STA mode)\n");
	res = wifi_manager_init(NULL);
	res = wifi_manager_init(&wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("wifi_manager_init fail\n");
		return ;
	}
	/* Print current status */
	wm_display_state(NULL);

	printf("====================================\n");
	printf("Repeated Test\n");
	printf("Total: %d\n", WM_TEST_COUNT);
	printf("====================================\n");
	print_wifi_ap_profile(&ap_config, "");
	print_wifi_softap_profile(&softap_config, "SoftAP Info");

	int cnt = 0;
	while (cnt++ < WM_TEST_COUNT) {
		printf(" T%d Starting round %d\n", getpid(), cnt);
		/* Print current status */
		wm_display_state(NULL);
		printf("Connecting to NULL\n");
		/* Connect to fake AP */
		res = wifi_manager_connect_ap(NULL);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("AP connect failed due to AP NULL\n");
			res = WIFI_MANAGER_FAIL;
		}
		printf("Connecting to AP with too long SSID\n");
		/* Connect to fake AP */
		res = wifi_manager_connect_ap(&fake_config);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("AP connect failed due to bad configuration: long SSID\n");
			res = WIFI_MANAGER_FAIL;
			fake_config.ssid_length = strlen(fake_ssid);
			strncpy(fake_config.ssid, fake_ssid, fake_config.ssid_length + 1);
		}
		printf("Connecting to fake AP (Auth/Crypto type 2/3)\n");
		/* Connect to fake AP */
		res = wifi_manager_connect_ap(&fake_config);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("AP connect failed due to bad configuration: auth/crypto type 2/3\n");
			res = WIFI_MANAGER_FAIL;
			fake_config.ap_auth_type = 1;
			fake_config.ap_crypto_type = info->crypto_type;
		}
		WM_TEST_WAIT;

		printf("Connecting to AP\n");
		/* Connect to AP */
		res = wifi_manager_connect_ap(&ap_config);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("AP connect failed in round %d\n", cnt);
			return ;
		}
		/* Print current status */
		wm_display_state(NULL);
		WM_TEST_WAIT; // wait dhcp
		/* Print current status */
		wm_display_state(NULL);

		printf("Connecting to AP, but already connected\n");
		/* Connect to AP */
		res = wifi_manager_connect_ap(&ap_config);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("AP connect failed in round %d\n", cnt);
		}

		/* Start SoftAP mode */
		printf("Start SoftAP mode\n");
		res = wifi_manager_set_mode(SOFTAP_MODE, &softap_config);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf(" Set AP mode Fail\n");
			return ;
		}
		/* Print current status */
		wm_display_state(NULL);
		/* Scanning */
		printf("Start scanning\n");
		res = wifi_manager_scan_ap();
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("scan Fail\n");
			return ;
		}
		WM_TEST_WAIT; // wait the scan result
		/* Print current status */
		wm_display_state(NULL);
		//TODO how to call callback functions such as join, leave, dhcpd, etc.
		/* Print current status */
		wm_display_state(NULL);

		/* Start STA mode */	
		printf("Start STA mode\n");
		res = wifi_manager_set_mode(STA_MODE, NULL);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf(" Set STA mode Fail\n");
			return ;
		}
		/* Scanning */
		printf("Start scanning\n");
		res = wifi_manager_scan_ap();
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("scan Fail\n");
			return ;
		}
		WM_TEST_WAIT; // wait the scan result


		printf("Connecting to fake AP (Auth/Crypto type 1/3)\n");
		/* Connect to fake AP */
		res = wifi_manager_connect_ap(&fake_config);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("AP connect failed due to bad configuration: auth/crypto type 1/3\n");
			res = WIFI_MANAGER_FAIL;
			fake_config.ap_auth_type = 4;
		}
		WM_TEST_WAIT;

		printf("Connecting to AP\n");
		/* Connect to AP */
		res = wifi_manager_connect_ap(&ap_config);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("AP connect failed in round %d\n", cnt);
			return ;
		}
		WM_TEST_WAIT; // wait dhcp
		/* File system call */
		printf("Save AP info.\n");
		res = wifi_manager_save_config(&ap_config);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("Save AP configuration failed\n");
			return;
		}
		printf("Get AP info.\n");
		wifi_manager_ap_config_s new_config;
		res = wifi_manager_get_config(&new_config);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("Get AP configuration failed\n");
			return;
		}
		print_wifi_ap_profile(&new_config, "Stored WiFi Information");
		printf("Reset AP info.\n");
		res = wifi_manager_remove_config();
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("Reset AP configuration failed\n");
			return;
		}
		/* Print current status */
		wm_display_state(NULL);
		//TODO how to call callback functions to change the state into RECONNECT

		printf("Disconnecting AP\n");
		/* Disconnect AP */
		res = wifi_manager_disconnect_ap();
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("disconnect fail (%d)\n", res);
			return;
		}
		/* Print current status */
		wm_display_state(NULL);
		WM_TEST_WAIT;
		/* Print current status */
		wm_display_state(NULL);
		printf("Deinit TEST in disconnected state\n");
		res = wifi_manager_deinit();
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("WiFi Manager failed to stop\n");
			return ;
		}	
		printf("Re-init WiFi (default STA mode)\n");
		res = wifi_manager_init(&wifi_callbacks);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("wifi_manager_init fail\n");
			return ;
		}
		printf("Connecting to fake AP (Auth/Crypto type 4/3)\n");
		/* Connect to fake AP */
		res = wifi_manager_connect_ap(&fake_config);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("AP connect failed due to bad configuration: auth/crypto type 4/3\n");
			res = WIFI_MANAGER_FAIL;
		}
		WM_TEST_WAIT;

		printf("Connecting to AP\n");
		/* Connect to AP */
		res = wifi_manager_connect_ap(&ap_config);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("AP connect failed in round %d\n", cnt);
			return ;
		}
		WM_TEST_WAIT; // wait dhcp
		/* Scanning */
		printf("Start scanning in connected state\n");
		res = wifi_manager_scan_ap();
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("scan Fail\n");
			return ;
		}
		WM_TEST_WAIT; // wait the scan result

		printf("Deinit TEST in connected state\n");
		res = wifi_manager_deinit();
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("WiFi Manager failed to stop\n");
			return ;
		}
		printf("Re-init WiFi (default STA mode)\n");
		res = wifi_manager_init(&wifi_callbacks);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("wifi_manager_init fail\n");
			return ;
		}
		printf("Re-start SoftAP mode\n");
		res = wifi_manager_set_mode(SOFTAP_MODE, &softap_config);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf(" Set AP mode Fail\n");
			return ;
		}
		printf("Deinit TEST in SoftAP state\n");
		res = wifi_manager_deinit();
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("WiFi Manager failed to stop\n");
			return ;
		}
		printf("Cycle finished [Round %d]\n", cnt);
	}
	printf("Exit WiFi Manager Stress Test..\n");

	return ;
}


int wm_parse_commands(struct options *opt, int argc, char *argv[])
{
	if (argc < 3) {
		return -1;
	}
	if (strcmp(argv[2], "start") == 0) {
		opt->func = wm_start;
	} else if (strcmp(argv[2], "stop") == 0) {
		opt->func = wm_stop;
	} else if (strcmp(argv[2], "softap") == 0) {
		/* wpa2 aes is a default security mode. */
		opt->func = wm_softap_start;
		opt->ssid = argv[3];
		opt->password = argv[4];
	} else if (strcmp(argv[2], "sta") == 0) {
		opt->func = wm_sta_start;
	} else if (strcmp(argv[2], "join") == 0) {
		if (argc < 5) {
			return -1;
		}
		opt->func = wm_connect;
		opt->ssid = argv[3];
		opt->auth_type = get_auth_type(argv[4]);
		if (opt->auth_type == WIFI_MANAGER_AUTH_UNKNOWN) {
			return -1;
		}
		if (opt->auth_type == WIFI_MANAGER_AUTH_OPEN) {
			return 0;
		}
		opt->crypto_type = get_crypto_type(argv[4]);
		opt->password = argv[5];
	} else if (strcmp(argv[2], "set") == 0) {
		if (argc < 5) {
			return -1;
		}
		opt->func = wm_set_info;
		opt->ssid = argv[3];
		opt->auth_type = get_auth_type(argv[4]);
		if (opt->auth_type == WIFI_MANAGER_AUTH_UNKNOWN) {
			return -1;
		}
		if (opt->auth_type == WIFI_MANAGER_AUTH_OPEN) {
			return 0;
		}
		opt->crypto_type = get_crypto_type(argv[4]);
		opt->password = argv[5];
	} else if (strcmp(argv[2], "get") == 0) {
		opt->func = wm_get_info;
	} else if (strcmp(argv[2], "reset") == 0) {
		opt->func = wm_reset_info;
	} else if (strcmp(argv[2], "leave") == 0) {
		opt->func = wm_disconnect;
	} else if (strcmp(argv[2], "cancel") == 0) {
		opt->func = wm_cancel;
	} else if (strcmp(argv[2], "mode") == 0) {
		opt->func = wm_display_state;
	} else if (strcmp(argv[2], "scan") == 0) {
		opt->func = wm_scan;
	} else if (strcmp(argv[2], "auto") == 0) {
		opt->func = wm_auto_test;
		if (argc < 7) {
			return -1;
		}
		opt->softap_ssid = argv[3];
		opt->softap_password = argv[4];
		opt->ssid = argv[5];
		opt->auth_type = get_auth_type(argv[6]);
		if (opt->auth_type == WIFI_MANAGER_AUTH_UNKNOWN) {
			return -1;
		}
		if (opt->auth_type == WIFI_MANAGER_AUTH_OPEN) {
			return 0;
		}
		opt->crypto_type = get_crypto_type(argv[6]);
		opt->password = argv[7];
	} else {
		return -1;
	}
	return 0;
}


void wm_process(int argc, char *argv[])
{
	struct options opt;
	int res = wm_parse_commands(&opt, argc, argv);
	if (res < 0) {
		printf("%s", USAGE);
		goto exit;
	}
	opt.func((void *)&opt);
exit:
	WM_TEST_FUNC_SIGNAL;
}


#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int wm_test_main(int argc, char *argv[])
#endif
{
	printf("wifi manager test!!\n");
	int res = wm_signal_init();
	if (res < 0) {
		return -1;
	}
	task_create("wifi test sample", 100, 1024 * 10, (main_t)wm_process, argv);

	WM_TEST_FUNC_WAIT;

	wm_signal_deinit();

	return 0;
}
