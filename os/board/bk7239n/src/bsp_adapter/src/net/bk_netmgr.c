/******************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/
#include <tinyara/config.h>
#include <pthread.h>
#include <stdlib.h>
#include <debug.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/mm/mm.h>
#include <include/modules/wifi.h>
#include <bk_netmgr.h>
#include <components/event.h>
#include <bk_wifi_adapter.h>
#include <bk_wifi.h>
#include <components/log.h>

/* Wi-Fi CONFIG ---------------------------------------------------------------*/
#define BK_OK          0                    /*!< BK_err_t value indicating success (no error) */
#define BK_FAIL        -1                   /*!< Generic BK_err_t code indicating failure */

#ifndef BK_ERR_NO_MEM
#define BK_ERR_NO_MEM               0x101   /*!< Out of memory */
#endif
#define BK_ERR_INVALID_ARG          0x102   /*!< Invalid argument */
#define BK_ERR_INVALID_STATE        0x103   /*!< Invalid state */
#define BK_ERR_INVALID_SIZE         0x104   /*!< Invalid size */
#ifndef BK_ERR_NOT_FOUND
#define BK_ERR_NOT_FOUND            0x105   /*!< Requested resource not found */
#endif
#define BK_ERR_NOT_SUPPORTED        0x106   /*!< Operation or feature not supported */
#ifndef BK_ERR_TIMEOUT
#define BK_ERR_TIMEOUT              0x107   /*!< Operation timed out */
#endif
#define BK_ERR_INVALID_RRTKONSE     0x108   /*!< Received rRTKonse was invalid */
#define BK_ERR_INVALID_CRC          0x109   /*!< CRC or checksum was invalid */
#define BK_ERR_INVALID_VERSION      0x10A   /*!< Version was invalid */
#ifndef BK_ERR_INVALID_MAC
#define BK_ERR_INVALID_MAC          0x10B   /*!< MAC address was invalid */
#endif

#ifndef BK_ERR_WIFI_BASE
#define BK_ERR_WIFI_BASE            0x3000  /*!< Starting number of WiFi error codes */
#endif

/* BEKEN Private define ------------------------------------------------------------*/

static bool g_bridge_on = false;
static bool g_netmgr_init = false;
static uint8_t g_sta_connecting = false;
static uint8_t g_sta_auto_reconnect = false;
static WiFi_InterFace_ID_t g_station_if = BK_WIFI_NONE;
static WiFi_InterFace_ID_t g_softap_if = BK_WIFI_NONE;

#define SCAN_RESULT_KEEP_TIMER_DURATION 180000

typedef void (*inform_sap_new_sta_join)(void);

int g_softap_channel = 11;
inform_sap_new_sta_join bk_trwifi_new_sta_join_func = NULL;
beken_mutex_t scanlistbusy;
beken_semaphore_t bk_trwifi_wlan_scan_sema;
static trwifi_ap_scan_info_s *saved_scan_list = NULL;
static uint32_t saved_scan_number = 0;
beken_timer_t scan_timer = {0};
static int g_scan_flag = 0;// 0: not scan, 1: scaning, 2: multi scaning

static trwifi_scan_list_s *g_scan_list;
static int g_scan_num;

#define AP_MULTI_SCAN_MAX_NUM  (6)

typedef struct {
	bool is_valid;
	unsigned int channel;									/**<  Radio channel that the AP beacon was received on		*/
	char ssid[TRWIFI_SSID_LEN + 1];							/**<  Service Set Identification (i.e. Name of Access Point) */
	unsigned int ssid_length;								/**<  The length of Service Set Identification				*/
	int scan_num;
	trwifi_ap_scan_info_s *saved_multi_scan_list;
} bkwifi_ap_multi_scan_info_s;
static bkwifi_ap_multi_scan_info_s *g_saved_multi_scan_list = NULL;


trwifi_result_e bk_wifi_netmgr_init(struct netdev *dev);
trwifi_result_e bk_wifi_netmgr_deinit(struct netdev *dev);
trwifi_result_e bk_wifi_netmgr_scan_ap(struct netdev *dev, trwifi_scan_config_s *config);
trwifi_result_e bk_wifi_netmgr_scan_multi_ap(struct netdev *dev, trwifi_scan_multi_configs_s *config);
trwifi_result_e bk_wifi_netmgr_connect_ap(struct netdev *dev, trwifi_ap_config_s *ap_connect_config, void *arg);
trwifi_result_e bk_wifi_netmgr_disconnect_ap(struct netdev *dev, void *arg);
trwifi_result_e bk_wifi_netmgr_get_info(struct netdev *dev, trwifi_info *wifi_info);
trwifi_result_e bk_wifi_netmgr_start_sta(struct netdev *dev);
trwifi_result_e bk_wifi_netmgr_start_softap(struct netdev *dev, trwifi_softap_config_s *softap_config);
trwifi_result_e bk_wifi_netmgr_stop_softap(struct netdev *dev);
trwifi_result_e bk_wifi_netmgr_set_autoconnect(struct netdev *dev, uint8_t check);
trwifi_result_e bk_wifi_netmgr_ioctl(struct netdev *dev, trwifi_msg_s *msg);
trwifi_result_e bk_wifi_netmgr_set_chplan(struct netdev *dev, uint8_t chplan);
trwifi_result_e bk_wifi_netmgr_get_signal_quality(struct netdev *dev, trwifi_signal_quality *signal_quality);
trwifi_result_e bk_wifi_netmgr_get_disconn_reason(struct netdev *dev, int *deauth_reason);
trwifi_result_e bk_wifi_netmgr_get_driver_info(struct netdev *dev, trwifi_driver_info *driver_info);
trwifi_result_e bk_wifi_netmgr_get_wpa_supplicant_state(struct netdev *dev, trwifi_wpa_states *wpa_supplicant_state);
trwifi_result_e bk_wifi_netmgr_set_bridge(struct netdev *dev, uint8_t control);

struct trwifi_ops g_trwifi_bk_ops = {
    bk_wifi_netmgr_init,                        /* init */
    bk_wifi_netmgr_deinit,                      /* deinit */
    bk_wifi_netmgr_scan_ap,                     /* scan_ap */
    bk_wifi_netmgr_connect_ap,                  /* connect_ap */
    bk_wifi_netmgr_disconnect_ap,               /* disconnect_ap */
    bk_wifi_netmgr_get_info,                    /* get_info */
    bk_wifi_netmgr_start_sta,                   /* start_sta */
    bk_wifi_netmgr_start_softap,                /* start_softap */
    bk_wifi_netmgr_stop_softap,                 /* stop_softap */
    bk_wifi_netmgr_set_autoconnect,             /* set_autoconnect */
    bk_wifi_netmgr_ioctl,                       /* drv_ioctl */
    bk_wifi_netmgr_scan_multi_ap,               /* scan_multi_ap */
    bk_wifi_netmgr_set_chplan,                  /* set_chplan */
    bk_wifi_netmgr_get_signal_quality,          /* get_signal_quality */
    bk_wifi_netmgr_get_disconn_reason,          /* get_deauth_reason */
    bk_wifi_netmgr_get_driver_info,             /* get_driver_info */
    bk_wifi_netmgr_get_wpa_supplicant_state,    /* get_wpa_supplicant_state */
    bk_wifi_netmgr_set_bridge,                  /* set_bridge */
};

extern struct netdev *armino_dev_wlan0;
extern struct netdev *armino_dev_wlan1;
static int auth_fail_cnt = 0;
static int valid_ch_list[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 149, 153, 157, 161, 165};
static int valid_ch_list_size = sizeof(valid_ch_list)/sizeof(valid_ch_list[0]);

bool g_sta_is_start = 0;
/*
 * Callback
*/

#if defined(CONFIG_ENABLE_HOMELYNK) && (CONFIG_ENABLE_HOMELYNK == 1)
typedef void *_mutex;
void rtw_mutex_init(_mutex *pmutex)
{
	if (*pmutex == NULL) {
		*pmutex = (_mutex)kmm_zalloc(sizeof(sem_t));
		if (*pmutex == NULL) {
			printf("Failed\n");
			return;
		}
	}
	sem_init(*pmutex, 0, 1);
	sem_setprotocol(*pmutex, SEM_PRIO_NONE);
}

void rtw_mutex_free(_mutex *pmutex)
{
	int i;
	if (*pmutex != NULL) {
		i = sem_destroy(*pmutex);
		if (i == 0) {
			kmm_free(*pmutex);
		} else {
			printf("Fail!!!\n");
		}
		*pmutex = NULL;
	}
}

void rtw_mutex_get(_mutex *pmutex)
{
	int temp;
	temp = sem_wait(*pmutex);
	if (temp != OK) {
		printf("Failed!\n");
	}
}

void rtw_mutex_put(_mutex *pmutex)
{
	sem_post(*pmutex);
}
#endif

void beken_report_wrong_password( void )
{
    auth_fail_cnt++;
    //set80211_sta_error_flag(CLP_STA_WRONG_PASSWORD);
    if (auth_fail_cnt%2 == 1) {
        trwifi_post_event(armino_dev_wlan0, LWNL_EVT_STA_DISCONNECTED, NULL, 0);
    } else {
        /* If only auth-fail is produced, application stops to connect ap*/
        trwifi_post_event(armino_dev_wlan0, LWNL_EVT_STA_CONNECT_FAILED, NULL, 0);
    }
}

int beken_is_wpa3_pwd_wrong(int status_code)
{
    int security = bk_sta_cipher_type();
    if ((security == WIFI_SECURITY_WPA3_SAE) ||
        (security == WIFI_SECURITY_WPA3_WPA2_MIXED)) {
        if ((status_code == 1) || (status_code == 15) || (status_code == 65529) ) {
            beken_report_wrong_password();
            return 1;
        }
    }
    return 0;
}

trwifi_ap_auth_type_e wifi_security_to_auth_type(wifi_security_t security)
{
    switch (security) {
        case WIFI_SECURITY_NONE:
            return TRWIFI_AUTH_OPEN;
        case WIFI_SECURITY_WEP:
            return TRWIFI_AUTH_WEP_SHARED;
        case WIFI_SECURITY_WPA_TKIP:
        case WIFI_SECURITY_WPA_AES:
        case WIFI_SECURITY_WPA_MIXED:
            return TRWIFI_AUTH_WPA_PSK;
        case WIFI_SECURITY_WPA2_TKIP:
        case WIFI_SECURITY_WPA2_AES:
        case WIFI_SECURITY_WPA2_MIXED:
            return TRWIFI_AUTH_WPA2_PSK;
        case WIFI_SECURITY_WPA3_SAE:
            return TRWIFI_AUTH_WPA3_PSK;
        case WIFI_SECURITY_WPA3_WPA2_MIXED:
            return TRWIFI_AUTH_WPA2_AND_WPA3_PSK;
        case WIFI_SECURITY_EAP:
            return TRWIFI_AUTH_WPA_PSK_ENT;
        default:
            return TRWIFI_AUTH_UNKNOWN;
    }
}

trwifi_ap_crypto_type_e wifi_security_to_crypto(wifi_security_t security)
{
    switch (security) {
        case WIFI_SECURITY_NONE:
            return TRWIFI_CRYPTO_NONE;
        case WIFI_SECURITY_WEP:
            return TRWIFI_CRYPTO_WEP_128;
        case WIFI_SECURITY_WPA_TKIP:
            return TRWIFI_CRYPTO_TKIP;
        case WIFI_SECURITY_WPA_AES:
            return TRWIFI_CRYPTO_AES;
        case WIFI_SECURITY_WPA_MIXED:
            return TRWIFI_CRYPTO_TKIP_AND_AES;
        case WIFI_SECURITY_WPA2_TKIP:
            return TRWIFI_CRYPTO_TKIP;
        case WIFI_SECURITY_WPA2_AES:
            return TRWIFI_CRYPTO_AES;
        case WIFI_SECURITY_WPA2_MIXED:
            return TRWIFI_CRYPTO_TKIP_AND_AES;
        case WIFI_SECURITY_WPA3_SAE:
        case WIFI_SECURITY_WPA3_WPA2_MIXED:
            return TRWIFI_CRYPTO_AES;
        case WIFI_SECURITY_EAP:
            return TRWIFI_CRYPTO_AES_ENT;
        default:
            return TRWIFI_CRYPTO_UNKNOWN;
    }
}

wifi_security_t wifi_auth_type_crypto_to_security(trwifi_ap_auth_type_e auth_type,trwifi_ap_crypto_type_e crypto)
{
    wifi_security_t security = WIFI_SECURITY_NONE;
    switch (auth_type) {
        case TRWIFI_AUTH_OPEN:
            security = WIFI_SECURITY_NONE;  /**< Open system. */
            break;
        case TRWIFI_AUTH_WEP_SHARED:
            security = WIFI_SECURITY_WEP;   /**< WEP security, **it's unsafe security, please don't use it** */
            break;
        case TRWIFI_AUTH_WPA_PSK:
            if(crypto == TRWIFI_CRYPTO_TKIP)
                security = WIFI_SECURITY_WPA_TKIP;  /**< WPA TKIP */
            else if(crypto == TRWIFI_CRYPTO_AES)
                security = WIFI_SECURITY_WPA_AES;  /**< WPA AES */
            else if(crypto == TRWIFI_CRYPTO_TKIP_AND_AES)
                security = WIFI_SECURITY_WPA_MIXED;  /**< WPA AES or TKIP */
            else
                security = WIFI_SECURITY_NONE;
            break;
        case TRWIFI_AUTH_WPA2_PSK:
            if(crypto == TRWIFI_CRYPTO_TKIP)
                security = WIFI_SECURITY_WPA2_TKIP;  /**< WPA2 TKIP */
            else if(crypto == TRWIFI_CRYPTO_AES)
                security = WIFI_SECURITY_WPA2_AES;  /**< WPA2 AES */
            else if(crypto == TRWIFI_CRYPTO_TKIP_AND_AES)
                security = WIFI_SECURITY_WPA2_MIXED;  /**< WPA2 AES or TKIP */
            else
                security = WIFI_SECURITY_NONE;
            break;
        case TRWIFI_AUTH_WPA3_PSK:
            security = WIFI_SECURITY_WPA3_SAE;  /**< WPA3 SAE */
            break;
        case TRWIFI_AUTH_WPA2_AND_WPA3_PSK:
            security = WIFI_SECURITY_WPA3_WPA2_MIXED;   /**< WPA3 SAE or WPA2 AES */
            break;
        case TRWIFI_AUTH_WPA_PSK_ENT:
            security = WIFI_SECURITY_EAP;   /**< EAP */
            break;
        #if 0
        case :
            security = WIFI_SECURITY_OWE;   /**< OWE */
            break;
        case :
            security = WIFI_SECURITY_AUTO;  /**< WiFi automatically detect the security type */
            break;
        case :
            security = WIFI_SECURITY_TYPE_WAPI_PSK;
            break;
        case :
            security = WIFI_SECURITY_TYPE_WAPI_CERT;
            break;
        #endif
        default:
            security = WIFI_SECURITY_NONE;
            break;
    }
    return security;
}

/* Clear persistent cached scan results used for fast connect (saved_scan_list/saved_scan_number). */
static void bk_trwifi_clear_saved_scan_cache(void)
{
	if (saved_scan_list != NULL) {
		os_free(saved_scan_list);
		saved_scan_list = NULL;
		saved_scan_number = 0;
	}
}

/* Clear temporary scan result chain used only for one TRWIFI_POST_SCANEVENT (g_scan_list/g_scan_num). */
static void bk_trwifi_clear_scan_chain_list(void)
{
	if (g_scan_list != NULL) {
		os_free(g_scan_list);
		g_scan_list = NULL;
		g_scan_num = 0;
	}
}

/* Clear per-SSID multi-scan cached results (g_saved_multi_scan_list). */
static void bk_trwifi_clear_multi_scan_cache(void)
{
	if (g_saved_multi_scan_list == NULL) {
		return;
	}

	for (int i = 0; i < AP_MULTI_SCAN_MAX_NUM; i++) {
		if(g_saved_multi_scan_list[i].saved_multi_scan_list != NULL) {
			os_free(g_saved_multi_scan_list[i].saved_multi_scan_list);
			g_saved_multi_scan_list[i].saved_multi_scan_list = NULL;
		}
		g_saved_multi_scan_list[i].is_valid = false;
		g_saved_multi_scan_list[i].scan_num = 0;
		g_saved_multi_scan_list[i].ssid_length = 0;
		memset(g_saved_multi_scan_list[i].ssid, 0, (TRWIFI_SSID_LEN + 1));
	}

	/* Free the array itself */
	os_free(g_saved_multi_scan_list);
	g_saved_multi_scan_list = NULL;
}

static void bk_trwifi_stop_scan_timer(void)
{
	if(rtos_is_timer_init(&scan_timer)) {
		/* Best-effort stop. It's OK if the timer is already stopped/expired. */
		int ret = rtos_stop_timer(&scan_timer);
		if(ret != 0) {
			nvdbg("[BK] scan stop timer returned %d\r\n", ret);
		}
	}
}

static void bk_trwifi_deinit_scan_timer(void)
{
	if (rtos_is_timer_init(&scan_timer)) {
		/* Stop first (best-effort), then deinit. */
		bk_trwifi_stop_scan_timer();
		int ret = rtos_deinit_timer(&scan_timer);
		if (ret != 0) {
			ndbg("[BK] scan deinit timer failed, ret=%d\r\n", ret);
		}
	}
}

/* Forward declaration for timer reset helper */
static void bk_trwifi_scan_timer_handler(void *FunctionContext);

static int bk_trwifi_reset_scan_timer(void)
{
	int ret;

	/*
	 * Reuse scan_timer if possible:
	 * - if timer is not initialized, init it once.
	 * - if timer is initialized, stop + start to reset (reuses init-time duration/callback).
	 * - if stop fails due to timer state, fallback to deinit+init.
	 */
	if (!rtos_is_timer_init(&scan_timer)) {
		ret = rtos_init_timer(&scan_timer, SCAN_RESULT_KEEP_TIMER_DURATION,
							  bk_trwifi_scan_timer_handler, NULL);
		if (ret != 0) {
			ndbg("[BK] scan init timer failed\r\n");
			return ret;
		}
	} else {
		/* Stop + start resets timer using original duration/callback from init */
		ret = rtos_stop_timer(&scan_timer);
		if (ret != 0) {
			nvdbg("[BK] scan stop timer returned %d, fallback recreate\r\n", ret);
			/* Stop failed, need to deinit and reinit */
			bk_trwifi_deinit_scan_timer();
			ret = rtos_init_timer(&scan_timer, SCAN_RESULT_KEEP_TIMER_DURATION,
								  bk_trwifi_scan_timer_handler, NULL);
			if (ret != 0) {
				ndbg("[BK] scan init timer failed\r\n");
				return ret;
			}
		}
	}

	ret = rtos_start_timer(&scan_timer);
	if (ret != 0) {
		ndbg("[BK] scan start timer failed, ret=%d\r\n", ret);
		/* Best-effort stop to avoid leaving it running */
		bk_trwifi_stop_scan_timer();
		return ret;
	}

	nvdbg("[BK] Start scan timer\r\n");
	return 0;
}

static void bk_trwifi_scan_timer_handler(void *FunctionContext)
{
	nvdbg("[BK] scan Timer expired : release saved scan list\r\n");
	nvdbg("[BK] scan Timer expired : sizeof(trwifi_scan_list_s) =%d, scan_number=%d \r\n", 
	      sizeof(trwifi_scan_list_s), saved_scan_number);
	rtos_lock_mutex(&scanlistbusy);
	// Check if scan list still exists (might have been freed by new scan)
	// bk_trwifi_clear_saved_scan_cache() internally checks saved_scan_list, so no need to check saved_scan_number
	bk_trwifi_clear_saved_scan_cache();
	/* Keep timer initialized for reuse; no need to deinit here. */
	rtos_unlock_mutex(&scanlistbusy);
}

bk_err_t bk_trwifi_scan_dump_result(trwifi_scan_list_s *scan_list)
{
	if (scan_list == NULL) {
		ndbg("[BK] scan doesn't found AP\r\n");
		return BK_OK;
	}

	nvdbg("%32s %17s   %4s %4s %s\r\n", "              SSID              ",
			   "      BSSID      ", "RSSI", "chan", "auth_type", "crypto_type");
	nvdbg("%32s %17s   %4s %4s %s\r\n", "--------------------------------",
			   "-----------------", "----", "----",  "----", "------\n");
	trwifi_scan_list_s *scan_out =  scan_list;
	while(scan_out) {
		nvdbg("%32s  " BK_MAC_FORMAT "   %4d %4d %d %d\r\n", scan_out->ap_info.ssid,
			   BK_MAC_STR(scan_out->ap_info.bssid), scan_out->ap_info.rssi, scan_out->ap_info.channel,
			   scan_out->ap_info.ap_auth_type,scan_out->ap_info.ap_crypto_type);
		scan_out = scan_out->next;
		rtos_delay_milliseconds(10);
	}
	return BK_OK;
}

void bk_trwifi_scan_result_record(trwifi_ap_scan_info_s *scan_ptr,wifi_scan_ap_info_t *ap)
{
	if((scan_ptr == NULL) || (ap == NULL)) {
		return;
	}
	int ssid_len = strlen(ap->ssid);
	char mac_str[TRWIFI_MACADDR_STR_LEN + 1];
	os_memset(scan_ptr->ssid, '\0', TRWIFI_SSID_LEN + 1);
	os_memcpy(scan_ptr->ssid, ap->ssid, ssid_len);
	scan_ptr->ssid_length = strlen(ap->ssid);
	snprintf(mac_str, TRWIFI_MACADDR_STR_LEN + 1, BK_MAC_FORMAT, BK_MAC_STR(ap->bssid));
	os_memcpy(scan_ptr->bssid, mac_str, TRWIFI_MACADDR_STR_LEN);//TRWIFI_MACADDR_STR_LEN 17
	scan_ptr->channel = ap->channel;
	scan_ptr->rssi = ap->rssi;
	scan_ptr->ap_auth_type = wifi_security_to_auth_type(ap->security);
	scan_ptr->ap_crypto_type = wifi_security_to_crypto(ap->security);
	scan_ptr->phy_mode = 0x00000004;
	scan_ptr->max_rate = 0;
}

int bk_wifi_scan_result_handle(const wifi_scan_result_t *scan_result)
{
	int ret;
	if (!scan_result) {
		nwdbg("[BK] scan doesn't found AP\r\n");
		goto scan_res_fail;
	}

	if ((scan_result->ap_num > 0) && (!scan_result->aps)) {
		ndbg("[BK] scan number is %d, but AP info is NULL\r\n", scan_result->ap_num);
		goto scan_res_fail;
	}

	rtos_lock_mutex(&scanlistbusy);
	// Note: Previous scan resources should be cleaned up in bk_trwlan_scan_start()
	// before starting the new scan. However, to prevent race conditions (e.g., timer
	// callback or rapid consecutive scans), we also check and free here as a safety measure.

	// Step 1: Stop timer first to prevent timer callback from interfering
	// This ensures that timer callback won't try to free memory while we're allocating new one
	bk_trwifi_stop_scan_timer();

	// Step 2: Handle case when no APs found (early return)
	if(scan_result->ap_num == 0) {
		// Still need to clear any existing scan cache
		bk_trwifi_clear_saved_scan_cache();
		TRWIFI_POST_SCANEVENT(armino_dev_wlan0, LWNL_EVT_SCAN_DONE, NULL);
		rtos_unlock_mutex(&scanlistbusy);
		return BK_OK;
	}

	// Step 3: Free old saved_scan_list first to prevent memory leak
	// This is a safety check in case cleanup in bk_trwlan_scan_start() didn't happen
	// or timer callback cleared it but new scan result arrived before cleanup completed
	bk_trwifi_clear_saved_scan_cache();

	// Step 4: Allocate all required memory first
	saved_scan_list = (trwifi_ap_scan_info_s *)os_malloc(sizeof(trwifi_ap_scan_info_s) * scan_result->ap_num);
	if (saved_scan_list == NULL) {
		ndbg("[BK] Fail to malloc saved_scan_list, size=%d bytes\r\n", 
		     sizeof(trwifi_ap_scan_info_s) * scan_result->ap_num);
		goto scan_res_fail;
	}
	g_scan_list = (trwifi_scan_list_s *)os_malloc(sizeof(trwifi_scan_list_s) * scan_result->ap_num);
	g_scan_num = scan_result->ap_num;
	if (g_scan_list == NULL) {
		ndbg("[BK] Fail to malloc g_scan_list\r\n");
		bk_trwifi_clear_saved_scan_cache();
		goto scan_res_fail;
	}

	// Step 5: Fill scan results
	wifi_scan_ap_info_t *ap;
	for (int i = 0; i < scan_result->ap_num; i++) {
		ap = (wifi_scan_ap_info_t *)(&scan_result->aps[i]);
		bk_trwifi_scan_result_record(&saved_scan_list[i], ap);
		os_memcpy(&g_scan_list[i].ap_info, &saved_scan_list[i], sizeof(trwifi_ap_scan_info_s));
		if(i > 0) {
			g_scan_list[i-1].next = &g_scan_list[i];
		}
		g_scan_list[i].next = NULL;
	}
	saved_scan_number = scan_result->ap_num;

	// Step 6: Initialize and start timer only after everything is ready
	// Timer will automatically free saved_scan_list after SCAN_RESULT_KEEP_TIMER_DURATION
	ret = bk_trwifi_reset_scan_timer();
	if (ret != 0) {
		ndbg("[BK] Failed to start scan timer, cleaning up\r\n");
		bk_trwifi_clear_saved_scan_cache();
		bk_trwifi_clear_scan_chain_list();
		goto scan_res_fail;
	}

	// Step 7: Post event
	TRWIFI_POST_SCANEVENT(armino_dev_wlan0, LWNL_EVT_SCAN_DONE, (void *)g_scan_list);

	// Step 8: Cleanup temporary g_scan_list
	bk_trwifi_clear_scan_chain_list();
	rtos_unlock_mutex(&scanlistbusy);
	return BK_OK;

scan_res_fail:
	TRWIFI_POST_SCANEVENT(armino_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
	rtos_unlock_mutex(&scanlistbusy);
	return BK_FAIL;
}

int bk_wifi_multi_scan_result_handle(const wifi_scan_result_t *scan_result,wifi_scan_config_t *scan_config)
{
	trwifi_ap_scan_info_s result_ap;
	wifi_scan_ap_info_t *ap;
	int ret = BK_FAIL;

	//int ret;
	if (!scan_result) {
		nwdbg("[BK] scan multi doesn't found AP\n");
		goto scan_res_fail;
	}

	if ((scan_result->ap_num > 0) && (!scan_result->aps)) {
		nvdbg("[BK] scan number is %d, but AP info is NULL\n", scan_result->ap_num);
		goto scan_res_fail;
	}

	nvdbg("[BK]:scan multi config: ssid_cnt %d, chan_cnt %d\r\n",scan_config->ssid_cnt, scan_config->chan_cnt);

	//Scan specific aps and filter the results
	if (scan_config->ssid_cnt) {
		// Get result ap number for per specific ssid
		int ap_num_per_ssid;
		if (g_saved_multi_scan_list == NULL) {
			ndbg("[BK] g_saved_multi_scan_list is NULL\r\n");
			goto scan_res_fail;
		}
		for(int j = 0; j < AP_MULTI_SCAN_MAX_NUM; j++) {
			if (!g_saved_multi_scan_list[j].is_valid)
				continue;

			ap_num_per_ssid = 0;
			for (int i = 0; i < scan_result->ap_num; i++) {
				ap = (wifi_scan_ap_info_t *)(&scan_result->aps[i]);
				if((os_memcmp(ap->ssid, g_saved_multi_scan_list[j].ssid, WIFI_SSID_STR_LEN) == 0) &&
				((g_saved_multi_scan_list[j].channel == 0) || (ap->channel == g_saved_multi_scan_list[j].channel))) {
					ap_num_per_ssid ++;
				}
			}

			if (ap_num_per_ssid == 0)
				continue;

			if (g_saved_multi_scan_list[j].is_valid)
				os_printf("bk_wifi_multi_scan_result_handle: ssid %s, ap_num %d \n", g_saved_multi_scan_list[j].ssid, ap_num_per_ssid);
			else
				os_printf("bk_wifi_multi_scan_result_handle: ap_num %d \n", ap_num_per_ssid);

			// Free old memory first to prevent memory leak
			if (g_saved_multi_scan_list[j].saved_multi_scan_list != NULL) {
				os_free(g_saved_multi_scan_list[j].saved_multi_scan_list);
				g_saved_multi_scan_list[j].saved_multi_scan_list = NULL;
			}

			trwifi_ap_scan_info_s * scan_list = (trwifi_ap_scan_info_s *)os_malloc(sizeof(trwifi_ap_scan_info_s)*ap_num_per_ssid);
			if (scan_list == NULL) {
				ndbg("[BK] Fail to malloc scan_list\r\n");
				goto scan_res_fail;
			}
			memset(scan_list, 0, (sizeof(trwifi_ap_scan_info_s)*ap_num_per_ssid));
			int ap_cnt_per_ssid = 0;
			for (int i = 0; i < scan_result->ap_num; i++) {
				ap = (wifi_scan_ap_info_t *)(&scan_result->aps[i]);
				if((os_memcmp(ap->ssid, g_saved_multi_scan_list[j].ssid, WIFI_SSID_STR_LEN) == 0) &&
				((g_saved_multi_scan_list[j].channel == 0) || (ap->channel == g_saved_multi_scan_list[j].channel))) {
					bk_trwifi_scan_result_record(&scan_list[ap_cnt_per_ssid],ap);
					ap_cnt_per_ssid ++;
					if (ap_cnt_per_ssid == ap_num_per_ssid) {
						break;
					}
				}
			}
			g_saved_multi_scan_list[j].saved_multi_scan_list = scan_list;
			g_saved_multi_scan_list[j].scan_num = ap_num_per_ssid;
		}
		return BK_OK;
	} else {
		int scan_result_sum = 0;
		if (g_saved_multi_scan_list != NULL) {
			for(int j = 0; j < AP_MULTI_SCAN_MAX_NUM; j++) {
				if(g_saved_multi_scan_list[j].is_valid) {
					scan_result_sum += g_saved_multi_scan_list[j].scan_num;
				}
			}
		}

		g_scan_num = scan_result_sum + scan_result->ap_num;
		if (g_scan_num == 0) {
			ndbg("[BK] scan multi doesn't found AP\r\n");
			goto scan_res_fail;
		}

		g_scan_list = (trwifi_scan_list_s *)os_malloc(sizeof(trwifi_scan_list_s)*g_scan_num);
		if (g_scan_list == NULL) {
			ndbg("[BK] Fail to malloc g_scan_list\r\n");
			goto scan_res_fail;
		}

		int add_list_idx = 0;
		if(scan_result_sum > 0 && g_saved_multi_scan_list != NULL){
			for(int j = 0; j < AP_MULTI_SCAN_MAX_NUM; j++) {
				if(g_saved_multi_scan_list[j].is_valid) {
					for(int m = 0; m<g_saved_multi_scan_list[j].scan_num;m++) {
						os_memcpy(&g_scan_list[add_list_idx].ap_info, &g_saved_multi_scan_list[j].saved_multi_scan_list[m], sizeof(trwifi_ap_scan_info_s));
						if(add_list_idx > 0) {
							g_scan_list[add_list_idx-1].next = &g_scan_list[add_list_idx];
						}
						g_scan_list[add_list_idx].next = NULL;
						add_list_idx++;
					}
				}
			}
		}

		int scan_ssid_result_num = add_list_idx;
		wifi_scan_ap_info_t *ap_info;
		for (int i = 0; i < scan_result->ap_num; i++) {
			ap_info = (wifi_scan_ap_info_t *)(&scan_result->aps[i]);
			bk_trwifi_scan_result_record(&result_ap, ap_info);
			// Skip if ap is already existent
			bool skip = false;
			for (int n= 0; n < scan_ssid_result_num; n ++) {
				if (!os_memcmp(g_scan_list[n].ap_info.bssid, result_ap.bssid, TRWIFI_MACADDR_STR_LEN)) {
					skip = true;
					break;
				}
			}
			if (skip)
				continue;

			os_memcpy(&g_scan_list[add_list_idx].ap_info, &result_ap, sizeof(trwifi_ap_scan_info_s));
			if(add_list_idx > 0) {
				g_scan_list[add_list_idx-1].next = &g_scan_list[add_list_idx];
			}
			g_scan_list[add_list_idx].next = NULL;
			add_list_idx++;
		}

		bk_trwifi_scan_dump_result(g_scan_list);
		TRWIFI_POST_SCANEVENT(armino_dev_wlan0, LWNL_EVT_SCAN_DONE, (void *)g_scan_list);
		os_free(g_scan_list);
		ret = BK_OK;
	}

scan_res_fail:
	if (ret != BK_OK) {
		TRWIFI_POST_SCANEVENT(armino_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
	}
	// clear saved multi scan list
	bk_trwifi_clear_multi_scan_cache();
	return BK_FAIL;
}

static int bk_trwlan_scan_start(wifi_scan_config_t *scan_config)
{
	int err;
	wifi_scan_result_t scan_result = {0};

	// Release previous cached scan result before starting new scan
	rtos_lock_mutex(&scanlistbusy);
	// Stop and cleanup timer first to prevent timer callback from executing
	bk_trwifi_stop_scan_timer();
	// Print memory status before freeing previous scan result
	bool had_previous_scan = (saved_scan_number != 0);
	// Free scan list after stopping timer to avoid race condition
	// bk_trwifi_clear_saved_scan_cache() internally checks saved_scan_list, so no need to check saved_scan_number
	bk_trwifi_clear_saved_scan_cache();
	if (had_previous_scan) {
		nwdbg("[BK] Release previous scan result before new scan\r\n");
	}
	// Also check and free g_scan_list if it exists (shouldn't happen in normal flow, but for safety)
	bk_trwifi_clear_scan_chain_list();
	// Also free g_saved_multi_scan_list if it exists (from previous multi scan)
	bk_trwifi_clear_multi_scan_cache();
	rtos_unlock_mutex(&scanlistbusy);

	if(bk_trwifi_wlan_scan_sema == NULL ) {
		err = rtos_init_semaphore(&bk_trwifi_wlan_scan_sema,1);
		if(err != kNoErr){
			ndbg("[BK] scan semaphore init failed!\r\n");
			goto error;
		}
	}

	if(scan_config)
	    nvdbg("[BK] scan ssid %s,type %d ,dur %d ,cnt %d\r\n",scan_config->ssid,scan_config->scan_type,scan_config->duration,scan_config->chan_cnt);

	BK_LOG_ON_ERR(bk_wifi_scan_start(scan_config));

	if(bk_trwifi_wlan_scan_sema != NULL) {
		err = rtos_get_semaphore(&bk_trwifi_wlan_scan_sema, 10000);
		if(err != kNoErr) {
			ndbg("[BK] scan wait semaphore timeout!\r\n");
			goto error;
		} else {
			/*check special scan requirement*/
			nvdbg("[BK] scan get semaphore !\r\n");
			bk_wifi_scan_get_result(&scan_result);
			if(g_scan_flag == 1) {
				bk_wifi_scan_result_handle(&scan_result);
			} else if(g_scan_flag == 2) {
				bk_wifi_multi_scan_result_handle(&scan_result,scan_config);
			}
			//BK_LOG_ON_ERR(bk_wifi_scan_dump_result(&scan_result));
    		bk_wifi_scan_free_result(&scan_result);
		}
	}
	else {
		ndbg("[BK] scan_sema is NULL!!\r\n");
		goto error;
	}
	//atsvr_event_sender("wifi",AT_WLAN_STA_SCAN_DONE,0,NULL,false);
	rtos_deinit_semaphore(&bk_trwifi_wlan_scan_sema);
	bk_trwifi_wlan_scan_sema = NULL;
	return 0;
error:
	if(bk_trwifi_wlan_scan_sema != NULL)
		rtos_deinit_semaphore(&bk_trwifi_wlan_scan_sema);
	bk_trwifi_wlan_scan_sema = NULL;
	return -1;
}

bool bk_wifi_check_channel(int ch)
{
    for (int i = 0; i < valid_ch_list_size; i++) {
        if (valid_ch_list[i] == ch) {
            return true;
        }
    }
    return false;
}

int bk_wifi_softap_init(trwifi_softap_config_s *softap_config)
{
	wifi_ap_config_t ap_config = WIFI_DEFAULT_AP_CONFIG();

	if (TRWIFI_SSID_LEN < softap_config->ssid_length) {
		ndbg("[BK] ssid name more than 32 Bytes\r\n");
		return BK_FAIL;
	}
	if (0 == softap_config->ssid_length) {
		ndbg("[BK] ssid name must not be null\r\n");
		return BK_FAIL;
	}

	if (8 > softap_config->passphrase_length)
		ndbg("[BK] key less than 8 Bytes, the security will be set NONE\r\n");

	if (64 < softap_config->passphrase_length) {
		ndbg("[BK] key more than 64 Bytes\r\n");
		return BK_FAIL;
	}

	if (softap_config->channel != 0) {
		ap_config.channel = softap_config->channel;
	}
	else{
		ap_config.channel = g_softap_channel;
	}

	if (!bk_wifi_check_channel(ap_config.channel)) {
		ndbg("[BK] AP channel is wrong: %d\r\n", ap_config.channel);
		return TRWIFI_INVALID_ARGS;
	}

	wifi_security_t security_type;
	switch (softap_config->ap_auth_type) {
		case TRWIFI_AUTH_OPEN:
			security_type = WIFI_SECURITY_NONE;
			break;
		case TRWIFI_AUTH_WPA2_PSK:
			if(softap_config->ap_crypto_type == TRWIFI_CRYPTO_AES) {
				security_type = WIFI_SECURITY_WPA2_AES;
				os_memcpy(ap_config.password, softap_config->passphrase, softap_config->passphrase_length);
				break;
			}
		case TRWIFI_AUTH_WPA2_AND_WPA3_PSK:
			if(softap_config->ap_crypto_type == TRWIFI_CRYPTO_AES) {
				security_type = WIFI_SECURITY_WPA3_WPA2_MIXED;
				os_memcpy(ap_config.password, softap_config->passphrase, softap_config->passphrase_length);
				break;
			}
		case TRWIFI_AUTH_UNKNOWN:
			ndbg("[BK] AP AUTH type is unknown %d\r\n", softap_config->ap_auth_type);
		default:
			ndbg("[BK] AP AUTH type or CRYPTO type is not support (%d, %d)\r\n", softap_config->ap_auth_type, softap_config->ap_crypto_type);
			return TRWIFI_INVALID_ARGS;
	}

	ap_config.security = security_type;

	if (softap_config->ssid_length < 1) {
		ndbg("[BK] AP ssid_length is wrong: %s,	%d\r\n", softap_config->ssid, softap_config->ssid_length);
		return TRWIFI_INVALID_ARGS;
	}

	if (softap_config->ap_auth_type != TRWIFI_AUTH_OPEN && softap_config->passphrase_length < 1) {
		ndbg("[BK] AP passphrase_length is wrong: %s,  %d\r\n", softap_config->passphrase, softap_config->passphrase_length);
		return TRWIFI_INVALID_ARGS;
	}

	os_memcpy(ap_config.ssid, softap_config->ssid, softap_config->ssid_length);
	ap_config.ssid[softap_config->ssid_length] = '\0';

	nvdbg("[BK] ssid:%s  key:%s\r\n", ap_config.ssid, ap_config.password);
	BK_RETURN_ON_ERR(bk_wifi_ap_set_config(&ap_config));
	return bk_wifi_ap_start();
}
int beken_wifi_event_cb(void *arg, event_module_t event_module,
                                int event_id, void *event_data)
{
    (void) arg;
    wifi_event_sta_disconnected_t *sta_disconnected;
    wifi_event_sta_connected_t *sta_connected;
    wifi_event_ap_disconnected_t *ap_disconnected;
    wifi_event_ap_connected_t *ap_connected;

    switch (event_id) {
    case EVENT_WIFI_STA_ASSOCIATED:

        break;
    case EVENT_WIFI_STA_CONNECTED:
        sta_connected = (wifi_event_sta_connected_t *)event_data;
        BK_WIFI_LOGI("BEKEN_WIFI_EVENT", "BK STA connected %s\n", sta_connected->ssid);
        // set ap info start
		g_sta_is_start = 1;
#if 0
        os_memset(&ap_info, 0, sizeof(struct wlm_connected_apinfo));
        os_memcpy(ap_info.bssid, sta_connected->bssid, WIFI_BSSID_LEN);
        os_strncpy(ap_info.ssid, sta_connected->ssid, WIFI_SSID_STR_LEN);
        ap_info.channel = bk_wlan_sta_get_channel();
        { // temporal code to update bssid
            wifi_link_status_t link_status = {0};
            bk_wifi_sta_get_link_status(&link_status);
            os_memcpy(ap_info.bssid, link_status.bssid, WIFI_BSSID_LEN);
        }
        //set80211_sta_error_flag(CLP_STA_NO_ERROR);
        // set ap info end
#endif
        trwifi_cbk_msg_s msg = {TRWIFI_REASON_UNKNOWN, {0,}, NULL};
        os_memcpy(msg.bssid, sta_connected->bssid, WIFI_BSSID_LEN);
        g_sta_connecting = false;
        trwifi_post_event(armino_dev_wlan0, LWNL_EVT_STA_CONNECTED, &msg, sizeof(trwifi_cbk_msg_s));
        break;

    case EVENT_WIFI_STA_DISCONNECTED:
        sta_disconnected = (wifi_event_sta_disconnected_t *)event_data;
		g_sta_is_start = 0;
        ndbg("BK STA disconnected, reason(%d)%s\n", sta_disconnected->disconnect_reason,
            sta_disconnected->local_generated ? ", local_generated" : "");
        if (g_sta_connecting) {
            trwifi_post_event(armino_dev_wlan0, LWNL_EVT_STA_CONNECT_FAILED, NULL, 0);
        } else {
            trwifi_post_event(armino_dev_wlan0, LWNL_EVT_STA_DISCONNECTED, NULL, 0);
        }
        g_sta_connecting = false;
        break;

    case EVENT_WIFI_NETWORK_FOUND:
        break;

    case EVENT_WIFI_SCAN_DONE:
        //ToDo:store scan results
		if (bk_trwifi_wlan_scan_sema) {
			rtos_set_semaphore(&bk_trwifi_wlan_scan_sema);
		}
        break;

    case EVENT_WIFI_AP_CONNECTED:
        ap_connected = (wifi_event_ap_connected_t *)event_data;
        ndbg(BK_MAC_FORMAT" connected to BK AP\n", BK_MAC_STR(ap_connected->mac));
		if (bk_trwifi_new_sta_join_func) {
			(*bk_trwifi_new_sta_join_func)();
		}
        trwifi_post_event(armino_dev_wlan1, LWNL_EVT_SOFTAP_STA_JOINED, NULL, 0);
        break;

    case EVENT_WIFI_AP_DISCONNECTED:
        ap_disconnected = (wifi_event_ap_disconnected_t *)event_data;
        ndbg(BK_MAC_FORMAT" disconnected from BK AP\n", BK_MAC_STR(ap_disconnected->mac));
        trwifi_post_event(armino_dev_wlan1, LWNL_EVT_SOFTAP_STA_LEFT, NULL, 0);
        break;

    case EVENT_WIFI_REGDOMAIN_CHANGED:
        /* fixme: handle regulatory domain changed event */
        break;

    default:
        BK_WIFI_LOGI("BEKEN_WIFI_EVENT", "rx event <%d %d>\n", event_module, event_id);
        trwifi_post_event(armino_dev_wlan0, LWNL_EVT_UNKNOWN, NULL, 0);
        break;
    }

    return BK_OK;
}

/* APIs Interface */
trwifi_result_e bk_wifi_netmgr_init(struct netdev *dev)
{
    trwifi_result_e wnret = TRWIFI_FAIL;
    BK_WIFI_LOGI("BK_NETMGR_INIT", "\n[BK] Init netmgr with dev %s\n",dev->ifname);
    if (!(memcmp(dev->ifname, "wlan0", 5))) {
        /* init bk event */
        BK_LOG_ON_ERR(bk_event_init());
        /* register beken callback to receive event*/
        bk_err_t err = bk_event_register_cb(EVENT_MOD_WIFI, EVENT_ID_ALL, beken_wifi_event_cb, NULL);
        if (err != BK_OK) {
            ndbg("[BK] Failed to register event callback\n");
            return TRWIFI_FAIL;
        }

        if (!g_netmgr_init) {
            wifi_init_config_t wifi_config = WIFI_DEFAULT_INIT_CONFIG();
            wnret = bk_wifi_init(&wifi_config);
            if (wnret != TRWIFI_SUCCESS) {
                ndbg("[BK] Failed to init BK7239\n");
                /* unregister beken event callback*/
                bk_event_unregister_cb(EVENT_MOD_WIFI, EVENT_ID_ALL, beken_wifi_event_cb);
                return TRWIFI_FAIL;
            }
        }
        g_station_if = BK_WIFI_STATION_IF;
        rtos_init_mutex(&scanlistbusy);
    } else if (!(memcmp(dev->ifname, "wlan1", 5))) {
        BK_WIFI_LOGI("BK_NETMGR_INIT", "[BK] Init netmgr with softAP mode\n");
    } else {
        ndbg("[BK] Unknown interface name\n");
        return TRWIFI_FAIL;
    }

    #if CONFIG_BK_WIFI_CSI_ADAPTER
    extern int bk_wifi_csi_init(int minor);
    bk_wifi_csi_init(0);
    #endif

    g_netmgr_init = true;
    return TRWIFI_SUCCESS;
}

trwifi_result_e bk_wifi_netmgr_deinit(struct netdev *dev)
{
    ndbg("\n[BK] Deinit netmgr with dev %s\n",dev->ifname);

    if (g_station_if == BK_WIFI_STATION_IF) {
        bk_err_t err = bk_wifi_sta_stop();
        if (err != BK_OK) {
            ndbg("[BK] Failed to stop STA mode\n");
            return TRWIFI_FAIL;
        }
    } else if (g_softap_if == BK_WIFI_SOFTAP_IF) {
        bk_err_t err = bk_wifi_ap_stop();
        if (err != BK_OK) {
            ndbg("[BK] Failed to stop BK7239\n");
            return TRWIFI_FAIL;
        }
    }

    g_station_if = BK_WIFI_NONE;
    g_softap_if = BK_WIFI_NONE;

    // Clean up scan related resources
    rtos_lock_mutex(&scanlistbusy);
    // Stop and deinit scan timer
    bk_trwifi_deinit_scan_timer();
    // Free scan lists
    bk_trwifi_clear_saved_scan_cache();
    bk_trwifi_clear_scan_chain_list();
    bk_trwifi_clear_multi_scan_cache();
    rtos_unlock_mutex(&scanlistbusy);

    /* unregister beken event callback*/
    bk_event_unregister_cb(EVENT_MOD_WIFI, EVENT_ID_ALL, beken_wifi_event_cb);

    return TRWIFI_SUCCESS;
}

trwifi_result_e bk_wifi_netmgr_scan_ap(struct netdev *dev, trwifi_scan_config_s *config)
{
	wifi_scan_config_t scan_config = {0};
	os_memset(&scan_config,0,sizeof(wifi_scan_config_t));

	if (config) {
		if (bk_wifi_check_channel(config->channel)) {
			scan_config.chan_cnt = 1;
			scan_config.chan_nb[0] = config->channel;
		}

		if (config->ssid_length > 0) {
			os_memcpy(&scan_config.ssid[0],config->ssid,config->ssid_length);
			scan_config.ssid_cnt = 1;
		}
	}
	if(	g_scan_flag != 0) {
		nwdbg("[BK] WIFi is Scanning\n");
		return -1;
	}
	g_scan_flag = 1;
	if(bk_trwlan_scan_start(&scan_config) != BK_OK){
		ndbg("[BK] WIFi Scan fail\r\n");
		g_scan_flag = 0;
		return -1;
	}
	g_scan_flag = 0;
	nvdbg("[BK] WIFi Scan success\r\n");
	return TRWIFI_SUCCESS;
}

trwifi_result_e bk_wifi_netmgr_scan_multi_ap(struct netdev *dev, trwifi_scan_multi_configs_s *config)
{
	int i = 0;
	bool invalid_chan_exist = false;
	trwifi_result_e ret = TRWIFI_FAIL;

	if(	g_scan_flag != 0) {
		nwdbg("[BK] WIFi is Scanning\r\n");
		return TRWIFI_FAIL;
	}

	wifi_scan_config_t scan_config = {0};
	os_memset(&scan_config,0,sizeof(wifi_scan_config_t));

	/* Allocate memory for multi-scan list */
	if (g_saved_multi_scan_list == NULL) {
		g_saved_multi_scan_list = (bkwifi_ap_multi_scan_info_s *)os_malloc(sizeof(bkwifi_ap_multi_scan_info_s) * AP_MULTI_SCAN_MAX_NUM);
		if (g_saved_multi_scan_list == NULL) {
			ndbg("[BK] Fail to malloc g_saved_multi_scan_list\r\n");
			ret = TRWIFI_FAIL;
			goto multi_scan_fail;
		}
		memset(g_saved_multi_scan_list, 0, sizeof(bkwifi_ap_multi_scan_info_s) * AP_MULTI_SCAN_MAX_NUM);
	}

	if (config) {
		if ((config->scan_ap_config_count == 0) && !config->scan_all) {
			ndbg("[BK] ERROR: scan config param is invalid\r\n");
			ret = TRWIFI_INVALID_ARGS;
			goto multi_scan_fail;
		} else if (config->scan_ap_config_count > AP_MULTI_SCAN_MAX_NUM) {
			ndbg("[BK] ERROR: SSID count exceeded, maximum allowed:%d given:%d\r\n",AP_MULTI_SCAN_MAX_NUM,config->scan_ap_config_count);
			ret = TRWIFI_INVALID_ARGS;
			goto multi_scan_fail;
		}

		for (i = 0; i < config->scan_ap_config_count; i++) {
			g_saved_multi_scan_list[i].is_valid = true;
			if ((config->scan_ap_config[i].ssid_length == 0) || !config->scan_ap_config[i].ssid) {
				continue;
			}

			if (bk_wifi_check_channel(config->scan_ap_config[i].channel))
				g_saved_multi_scan_list[i].channel = config->scan_ap_config[i].channel;
			else {
				invalid_chan_exist = true;
				g_saved_multi_scan_list[i].channel = 0;
			}
			os_memcpy(g_saved_multi_scan_list[i].ssid,config->scan_ap_config[i].ssid,config->scan_ap_config[i].ssid_length);
			g_saved_multi_scan_list[i].ssid_length = config->scan_ap_config[i].ssid_length;
		}

		/* Scan SSID with valid channnel */
		if (!invalid_chan_exist) {
			for (i = 0; i < config->scan_ap_config_count; i++) {
				os_memset(&scan_config,0,sizeof(wifi_scan_config_t));
				os_memcpy(&scan_config.ssid[0],config->scan_ap_config[i].ssid,config->scan_ap_config[i].ssid_length);
				scan_config.ssid_cnt = 1;
				scan_config.chan_cnt = 1;
				scan_config.chan_nb[0] = config->scan_ap_config[i].channel;

				g_scan_flag = 2;
				if(bk_trwlan_scan_start(&scan_config) != BK_OK) {
					ndbg("[BK] WIFi Scan fail\r\n");
					goto multi_scan_fail;
				}
			}
		} else {
			/* Scan all channels if any channel in scan config is invalid */
			os_memset(&scan_config,0,sizeof(wifi_scan_config_t));
			for (i = 0; i < config->scan_ap_config_count; i++) {
				os_memcpy(&scan_config.ssid[scan_config.ssid_cnt],config->scan_ap_config[i].ssid,config->scan_ap_config[i].ssid_length);
				scan_config.ssid_cnt ++;
			}

			if (scan_config.ssid_cnt) {
				g_scan_flag = 2;
				if(bk_trwlan_scan_start(&scan_config) != BK_OK){
					ndbg("[BK] WIFi Scan fail\r\n");
					goto multi_scan_fail;
				}
			}
		}

		os_memset(&scan_config,0,sizeof(wifi_scan_config_t));
		if(config->scan_all) {
			g_scan_flag = 2;
			if(bk_trwlan_scan_start(&scan_config) != BK_OK){
				ndbg("[BK] WIFi Scan fail\r\n");
				goto multi_scan_fail;
			}
		} else {
			int scan_result_sum = 0;
			if (g_saved_multi_scan_list != NULL) {
				for(int j = 0; j < AP_MULTI_SCAN_MAX_NUM; j++) {
					if(g_saved_multi_scan_list[j].is_valid) {
						scan_result_sum += g_saved_multi_scan_list[j].scan_num;
					}
				}
			}

			if (scan_result_sum == 0) {
				ndbg("[BK] scan multi doesn't found AP\r\n");
				goto multi_scan_fail;
			}
			if(scan_result_sum > 0){
				g_scan_list	= (trwifi_scan_list_s *)os_malloc(sizeof(trwifi_scan_list_s)*scan_result_sum);
				if (g_scan_list == NULL) {
					ndbg("[BK] Fail to malloc g_scan_list\r\n");
					goto multi_scan_fail;
				}
				int add_list_idx = 0;
				if (g_saved_multi_scan_list != NULL) {
					for(int j = 0; j < AP_MULTI_SCAN_MAX_NUM; j++) {
						if(g_saved_multi_scan_list[j].is_valid) {
							for(int m = 0; m<g_saved_multi_scan_list[j].scan_num;m++) {
								os_memcpy(&g_scan_list[add_list_idx].ap_info, &g_saved_multi_scan_list[j].saved_multi_scan_list[m], sizeof(trwifi_ap_scan_info_s));
							if(add_list_idx > 0) {
								g_scan_list[add_list_idx-1].next = &g_scan_list[add_list_idx];
							}
							g_scan_list[add_list_idx].next = NULL;
							add_list_idx++;
						}
					}
				}
				bk_trwifi_scan_dump_result(g_scan_list);
				TRWIFI_POST_SCANEVENT(armino_dev_wlan0, LWNL_EVT_SCAN_DONE, (void *)g_scan_list);
				bk_trwifi_clear_scan_chain_list();
			}
		}
	}

	nvdbg("[BK] WIFi Scan success\r\n");
	ret = TRWIFI_SUCCESS;

multi_scan_fail:
	if (ret != TRWIFI_SUCCESS) {
		TRWIFI_POST_SCANEVENT(armino_dev_wlan0, LWNL_EVT_SCAN_FAILED, NULL);
	}
	g_scan_flag = 0;
	// clear saved multi scan list
	bk_trwifi_clear_multi_scan_cache();
	return ret;
}

/* Fix missing closing brace introduced by recent refactor */
}

trwifi_result_e bk_wifi_netmgr_connect_ap(struct netdev *dev, trwifi_ap_config_s *ap_connect_config, void *arg)
{
	if (!ap_connect_config) {
		ndbg("[BK] no ap_connect_config\r\n");
		return TRWIFI_INVALID_ARGS;
	}

	wifi_sta_config_t sta_config = {0};
	os_memset(&sta_config,0,sizeof(wifi_sta_config_t));

	if (ap_connect_config->ssid_length != 0)
		os_memcpy(sta_config.ssid, ap_connect_config->ssid,ap_connect_config->ssid_length);

	if (ap_connect_config->passphrase_length != 0)
		os_memcpy(sta_config.password, ap_connect_config->passphrase,ap_connect_config->passphrase_length);

	//check scan result
	rtos_lock_mutex(&scanlistbusy);
	if (saved_scan_number) {
		for (int i = 0 ; i < saved_scan_number ; i++) {
			if((strncmp(saved_scan_list[i].ssid, ap_connect_config->ssid, ap_connect_config->ssid_length) == 0)
				&& (ap_connect_config->ap_auth_type == saved_scan_list[i].ap_auth_type)
				&& (ap_connect_config->ap_crypto_type == saved_scan_list[i].ap_crypto_type)) {
				sta_config.channel = saved_scan_list[i].channel;
				nvdbg("[BK] Scanned AP info : ssid = %s, auth = %d, crypt = %d\r\n", ap_connect_config->ssid, ap_connect_config->ap_auth_type, ap_connect_config->ap_crypto_type);
				break;
			}
		}
	}
	rtos_unlock_mutex(&scanlistbusy);

	int ret = 0;

	// security type
	sta_config.security = wifi_auth_type_crypto_to_security(ap_connect_config->ap_auth_type,ap_connect_config->ap_crypto_type);

	if (!g_sta_auto_reconnect) {
		sta_config.auto_reconnect_count = 1;
		sta_config.disable_auto_reconnect_after_disconnect = true;
	}

	ret = bk_wifi_sta_set_config(&sta_config);
	if (ret != BK_OK) {
		ndbg("[BK] WiFiNetworkJoin failed, %s\r\n", ap_connect_config->ssid);
		return TRWIFI_FAIL;
	}
	g_sta_connecting = true;
	ret = bk_wifi_sta_start();
	if (ret != BK_OK) {
		ndbg("[BK] WiFiNetworkJoin failed, %s\r\n", ap_connect_config->ssid);
		return TRWIFI_FAIL;
	} else {
		nvdbg("[BK] Successfully joined the network: %s(%d)\r\n", ap_connect_config->ssid,
			  ap_connect_config->ssid_length);
	}
	return TRWIFI_SUCCESS;
}

trwifi_result_e bk_wifi_netmgr_disconnect_ap(struct netdev *dev, void *arg)
{
	trwifi_result_e ret = TRWIFI_FAIL;
	int res = bk_wifi_sta_stop();
	if (res == BK_OK) {
		nvdbg("[BK] WiFiNetworkLeave success\n");
		ret = TRWIFI_SUCCESS;
	} else {
		ndbg("[BK] WiFiNetworkLeave fail because of %d\r\n", ret);
	}
	return ret;
}

trwifi_result_e bk_wifi_netmgr_get_signal_quality(struct netdev *dev, trwifi_signal_quality *signal_quality)
{
	trwifi_result_e wuret = TRWIFI_INVALID_ARGS;

	if (!dev) {
		ndbg("[BK] invalid dev argument \n");
		return wuret;
	}

	if (!signal_quality) {
		ndbg("[BK] invalid signal_quality argument \n");
		return wuret;
	}

	memset(signal_quality, 0, sizeof(trwifi_signal_quality));
	wuret = TRWIFI_FAIL;

	if (g_station_if == BK_WIFI_NONE || g_softap_if == BK_WIFI_NONE) {
		ndbg("[BK] Failed to get signal quality, wifi not initialized \n");
		return wuret;
	}

	u8 mode = (dev == armino_dev_wlan0 ? NETIF_IF_STA:NETIF_IF_AP);
	struct tx_stats_t trx_stats ={0};

	if(bk_wifi_get_tx_stats(mode,&trx_stats) != TRWIFI_SUCCESS)
	{
		ndbg("[BK] Failed to get signal quality \n");
		return wuret;
	}
	signal_quality->tx_drop       = trx_stats.tx_drop;
	signal_quality->rx_drop       = trx_stats.rx_drop;
	signal_quality->tx_retry      = trx_stats.tx_retry;
	signal_quality->max_rate      = trx_stats.max_rate;
	signal_quality->network_bw    = trx_stats.bandwidth;
	signal_quality->channel       = trx_stats.chan;
	signal_quality->snr           = trx_stats.snr;

	wuret = TRWIFI_SUCCESS;

	return wuret;
}

trwifi_result_e bk_wifi_netmgr_get_info(struct netdev *dev, trwifi_info *wifi_info)
{
	trwifi_result_e ret = TRWIFI_INVALID_ARGS;
	if (wifi_info) {
		ret = TRWIFI_FAIL;
		if (g_station_if != BK_WIFI_NONE || g_softap_if != BK_WIFI_NONE) {
			wifi_info->rssi = (int)0;
			if (g_softap_if == BK_WIFI_SOFTAP_IF) {
				wifi_info->wifi_status = TRWIFI_SOFTAP_MODE;
			} else if (g_station_if == BK_WIFI_STATION_IF) {
				// check is station up
				if (g_sta_is_start == 1) {
					wifi_info->wifi_status = TRWIFI_CONNECTED;
					wifi_link_status_t link_status = {0};
					os_memset(&link_status, 0x0, sizeof(link_status));
					if(bk_wifi_sta_get_link_status(&link_status) == kNoErr) {
						wifi_info->rssi = link_status.rssi;
					}
				} else {
					wifi_info->wifi_status = TRWIFI_DISCONNECTED;
				}
			}
#ifdef CONFIG_ENABLE_HOMELYNK
			if (g_bridge_on) {
				wifi_info->wifi_status = TRWIFI_BRIDGE_MODE;
			}
#endif //#ifdef CONFIG_ENABLE_HOMELYNK
			ret = TRWIFI_SUCCESS;
		} else {
			ndbg("[BK] need to init... get info fail\r\n");
		}
	}
	return ret;
}

trwifi_result_e bk_wifi_netmgr_get_wpa_supplicant_state(struct netdev *dev, trwifi_wpa_states *wpa_supplicant_state)
{
    wifi_linkstate_reason_t wpas_state = mhdr_get_station_status();
    
    switch (wpas_state.state) {
        case WIFI_LINKSTATE_STA_IDLE:
        wpa_supplicant_state->wpa_supplicant_state = WPA_INACTIVE;
        break;
        case WIFI_LINKSTATE_STA_CONNECTING:
        wpa_supplicant_state->wpa_supplicant_state = WPA_ASSOCIATING;
        break;
        case WIFI_LINKSTATE_STA_CONNECTED:
        wpa_supplicant_state->wpa_supplicant_state = WPA_COMPLETED;
        break;
        case WIFI_LINKSTATE_STA_CONNECT_FAILED:
        case WIFI_LINKSTATE_STA_DISCONNECTED:
        wpa_supplicant_state->wpa_supplicant_state = WPA_DISCONNECTED;
        break;
        default:
            break;
    }
    //Todo More

    return TRWIFI_SUCCESS;
}

trwifi_result_e bk_wifi_netmgr_get_driver_info(struct netdev *dev, trwifi_driver_info *driver_info)
{
    return 0;
}

trwifi_result_e bk_wifi_netmgr_start_softap(struct netdev *dev, trwifi_softap_config_s *softap_config)
{
	if (!softap_config) {
		return TRWIFI_INVALID_ARGS;
	}

	if (g_softap_if == BK_WIFI_SOFTAP_IF)
	    nwdbg("[BK] softap is already running!\n");

	trwifi_result_e ret = TRWIFI_FAIL;

	//bk_trwifi_new_sta_join_func = softap_config->inform_new_sta_join;

	ret = bk_wifi_softap_init(softap_config);
	if (ret != BK_OK) {
		ndbg("[BK] Failed to start AP mode(%d)\r\n", ret);
		return ret;
	}
	g_softap_if = BK_WIFI_SOFTAP_IF;
	nvdbg("[BK] SoftAP with SSID: %s has successfully started!\r\n", softap_config->ssid);

	ret = TRWIFI_SUCCESS;

	return ret;
}

trwifi_result_e bk_wifi_netmgr_start_sta(struct netdev *dev)
{
	trwifi_result_e ret = TRWIFI_FAIL;

	g_station_if = BK_WIFI_STATION_IF;
	ret = TRWIFI_SUCCESS;

	return ret;
}

trwifi_result_e bk_wifi_netmgr_stop_softap(struct netdev *dev)
{
	trwifi_result_e ret = TRWIFI_FAIL;

	int res = bk_wifi_ap_stop();
	if (res == BK_OK) {
		g_softap_if = BK_WIFI_NONE;
		ret = TRWIFI_SUCCESS;
		nvdbg("[BK] Stop AP mode successfully\r\n");
	} else {
		ndbg("[BK] Stop AP mode fail\r\n");
	}

	return ret;
}

trwifi_result_e bk_wifi_netmgr_set_autoconnect(struct netdev *dev, uint8_t check)
{
#if !CONFIG_STA_AUTO_RECONNECT
    ndbg("Auto reconnect feature is not enabled\n");
    return TRWIFI_FAIL;
#else
    wlan_auto_reconnect_t ar_config = {0};
    // check 0 disable auto reconnect
    if (check == 0) {
        nvdbg("Auto reconnect disabled\n");
        g_sta_auto_reconnect = false;
        ar_config.disable_reconnect_when_disconnect = true;
        wlan_sta_set_autoreconnect(&ar_config);
        return TRWIFI_SUCCESS;
    } else {
        nvdbg("Auto reconnect enabled\n");
        g_sta_auto_reconnect = true;
        ar_config.disable_reconnect_when_disconnect = false;
        wlan_sta_set_autoreconnect(&ar_config);
        return TRWIFI_SUCCESS;
    }
#endif
}

trwifi_result_e bk_wifi_netmgr_set_bridge(struct netdev *dev, uint8_t control)
{
    if (control) {
        g_bridge_on = true;
    } else {
        g_bridge_on = false;
    }
    nvdbg("[BK] Set Bridge mode to %d\n", control);
    return TRWIFI_SUCCESS;
}

trwifi_result_e bk_wifi_netmgr_ioctl(struct netdev *dev, trwifi_msg_s *msg)
{
	if (msg->cmd == TRWIFI_MSG_SET_POWERMODE) {
		int *mode = (int *)msg->data;
		if (*mode == TRWIFI_POWERMODE_ON) {
			ndbg("[BK] set power mode on\n");
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
			bk_wlan_ps_enable();
#pragma GCC diagnostic pop
			return TRWIFI_SUCCESS;
		} else if (*mode == TRWIFI_POWERMODE_OFF) {
			ndbg("[BK] set power mode off\n");
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
			bk_wlan_ps_disable();
#pragma GCC diagnostic pop
			return TRWIFI_SUCCESS;
		}
	} else if (msg->cmd == TRWIFI_MSG_GET_STATS) {
		ndbg("[BK] don't support TRWIFI_MSG_GET_STATS\n");
		return TRWIFI_SUCCESS;
	}
	ndbg("[BK] unsupport ioctl cmd:%d\r\n", msg->cmd);
	return TRWIFI_NOT_SUPPORTED;
}

trwifi_result_e bk_wifi_netmgr_set_chplan(struct netdev *dev, uint8_t chplan)
{
 	trwifi_result_e wuret = TRWIFI_SUCCESS;

 	if (!bk_wifi_check_channel(chplan)) {
 		ndbg("[BK] invalid chplan argument \r\n");
		wuret = TRWIFI_INVALID_ARGS;
		goto exit_set_chp;
 	}

 	if ((g_sta_is_start == 1) || (g_softap_if == BK_WIFI_SOFTAP_IF)) {
 		ndbg("[BK] Failed to set channel plan, disconnect from AP or stop SoftAP before setting\r\n");
 		wuret = TRWIFI_FAIL;
		goto exit_set_chp;
 	}

	if (g_softap_channel != chplan) {
		g_softap_channel = chplan;
	}

	bk_wifi_set_ap_channel(chplan);

exit_set_chp:
 	return wuret;
}

trwifi_result_e bk_wifi_netmgr_get_disconn_reason(struct netdev *dev, int *deauth_reason)
{
    // Get Wi-Fi Disconnect Reason
    wifi_linkstate_reason_t state = mhdr_get_station_status();
    *deauth_reason = state.reason_code;

    nvdbg("[BK] Disconnection reason code: %d\n", *deauth_reason);
    return TRWIFI_SUCCESS;
}
