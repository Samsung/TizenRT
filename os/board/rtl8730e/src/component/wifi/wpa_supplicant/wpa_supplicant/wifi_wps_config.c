#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "queue.h"
#include "utils/os.h"
#if CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
#endif
#include "wifi_conf.h"
#include "wifi_ind.h"
#include "wps/wps_defs.h"
#include "wps/wps_i.h"
#include <platform_stdlib.h>
#include "wifi_wps_config.h"

#include "platform_opts_bt.h"

// The maximum number of WPS credentials. The value should be in range of 1~10.
int wps_max_cred_count = 10;

/*
 * @brief  struct wps_credential - WPS Credential
 */
struct dev_credential {
	u8 ssid[32]; 			/**< SSID */
	size_t ssid_len;		/**< Length of SSID */
	u16 auth_type;		/**< Authentication Type (WPS_AUTH_OPEN, .. flags) */
	u16 encr_type;		/**< Encryption Type (WPS_ENCR_NONE, .. flags) */
	u8 key_idx;			/**< Key index */
	u8 key[65];			/**< Key */
	size_t key_len;		/**< Key length in octets */
	u8 mac_addr[6];		/**< MAC address of the Credential receiver */
	const u8 *cred_attr;	/**< Unparsed Credential attribute data (used only in cred_cb()).
							This may be NULL, if not used. */
	size_t cred_attr_len;	/**< Length of cred_attr in octets */
	u16 ap_channel;		/**< AP channel */
};

typedef struct {
	char *target_ssid;
	unsigned char *target_bssid;
	u16 config_method;
	_sema scan_sema;
	int isoverlap;
	int isoverlap_5G;
} internal_wps_scan_handler_arg_t;

#define WLAN0_NAME "wlan0"
#ifndef ENABLE
#define ENABLE	(1)
#endif
#ifndef DISABLE
#define	DISABLE (0)
#endif
#define STACKSIZE     512


//static xSemaphoreHandle wps_reconnect_semaphore;
//static struct _WIFI_NETWORK wifi_get_from_certificate = {0};

#define WPS_AUTH_TYPE_OPEN              (0x0001)
#define WPS_AUTH_TYPE_WPA_PERSONAL      (0x0002)
#define WPS_AUTH_TYPE_SHARED            (0x0004)
#define WPS_AUTH_TYPE_WPA_ENTERPRISE    (0x0008)
#define WPS_AUTH_TYPE_WPA2_PERSONAL     (0x0010)
#define WPS_AUTH_TYPE_WPA2_ENTERPRISE   (0x0020)

#define WPS_ENCR_TYPE_NONE              (0x0001)
#define WPS_ENCR_TYPE_WEP               (0x0002)
#define WPS_ENCR_TYPE_TKIP              (0x0004)
#define WPS_ENCR_TYPE_AES               (0x0008)

#define 	SCAN_BUFFER_LENGTH	(4096)

#if defined(CONFIG_ENABLE_P2P) && CONFIG_ENABLE_P2P
extern void _wifi_p2p_wps_success(const u8 *peer_addr, int registrar);
extern void _wifi_p2p_wps_failed(void);
#endif
extern void wpas_wsc_sta_wps_start_hdl(char *buf, int buf_len, int flags, void *userdata);
extern void wpas_wsc_wps_finish_hdl(char *buf, int buf_len, int flags, void *userdata);
extern void wpas_wsc_server_wps_finish_hdl(char *buf, int buf_len, int flags, void *userdata);
extern void wpas_wsc_eapol_recvd_hdl(char *buf, int buf_len, int flags, void *userdata);

void wifi_p2p_wps_success(const u8 *peer_addr, int registrar)
{
	/* To avoid gcc warnnings */
	(void) peer_addr;
	(void) registrar;
#if CONFIG_ENABLE_P2P
	_wifi_p2p_wps_success(peer_addr, registrar);
#endif
}

void wifi_p2p_wps_failed(void)
{
#if CONFIG_ENABLE_P2P
	_wifi_p2p_wps_failed();
#endif
}

void *wps_registrar_init(void *priv, void *pcfg)
{
	/* To avoid gcc warnnings */
	(void) priv;
	(void) pcfg;
	return NULL;
}

void wps_registrar_deinit(void *priv)
{
	/* To avoid gcc warnnings */
	(void) priv;
}

void *wps_registrar_alloc(void)
{
	return NULL;
}

u32 wps_registrar_process_msg(void *priv, enum wsc_op_code op_code, const void *pmsg)
{
	/* To avoid gcc warnnings */
	(void) priv;
	(void) op_code;
	(void) pmsg;

	return 0;
}

void *wps_registrar_get_msg(void *priv, enum wsc_op_code *op_code)
{
	/* To avoid gcc warnnings */
	(void) priv;
	(void) op_code;

	return NULL;
}


int wps_registrar_add_pin(void *priv, const u8 *addr,
						  const u8 *uuid, const u8 *pin, size_t pin_len,
						  int timeout)
{
	/* To avoid gcc warnnings */
	(void) priv;
	(void) addr;
	(void) uuid;
	(void) pin;
	(void) pin_len;
	(void) timeout;

	return 0;
}

int wps_registrar_button_pushed(void *priv,
								const u8 *p2p_dev_addr)
{
	/* To avoid gcc warnnings */
	(void) priv;
	(void) p2p_dev_addr;
	return 0;
}

int wps_registrar_wps_cancel(void *priv)
{
	/* To avoid gcc warnnings */
	(void) priv;

	return 0;
}

void wpas_wsc_ap_send_eap_reqidentity(void *priv, u8 *rx_buf)
{
	/* To avoid gcc warnnings */
	(void) priv;
	(void) rx_buf;
}

void wpas_wsc_ap_check_eap_rspidentity(void *priv, u8 *rx_buf)
{
	/* To avoid gcc warnnings */
	(void) priv;
	(void) rx_buf;
}

void wpas_wsc_registrar_send_eap_fail(void *priv)
{
	/* To avoid gcc warnnings */
	(void) priv;

}

void wpas_wsc_registrar_handle_recvd(void *priv, u8 *rx_buf)
{
	/* To avoid gcc warnnings */
	(void) priv;
	(void) rx_buf;

}

void *eap_wsc_server_process_hdl(void *priv, void *req, u8 id)
{
	/* To avoid gcc warnnings */
	(void) priv;
	(void) req;
	(void) id;

	return NULL;
}

void eap_wsc_server_reset(void *priv)
{
	/* To avoid gcc warnnings */
	(void) priv;
}

#if CONFIG_ENABLE_WPS
xqueue_handle_t queue_for_credential;
char wps_pin_code[33];
u16 config_method;
u8 wps_password_id;
static unsigned char wps_stop_notified = 0;

void wps_check_and_show_connection_info(void)
{
	rtw_wifi_setting_t setting;
#if CONFIG_LWIP_LAYER
	/* Start DHCP Client */
	LwIP_DHCP(0, DHCP_START);
#endif
	wifi_get_setting(STA_WLAN_INDEX, &setting);
	/*show setting*/
	printf("\n\r\nWIFI  %s Setting:", WLAN0_NAME);
	printf("\n\r==============================");

	switch (setting.mode) {
	case RTW_MODE_AP:
		printf("\n\r		MODE => AP");
		break;
	case RTW_MODE_STA:
		printf("\n\r		MODE => STATION");
		break;
	default:
		printf("\n\r		MODE => UNKNOWN");
	}
	printf("\n\r		SSID => %s", setting.ssid);
	printf("\n\r	 CHANNEL => %d", setting.channel);

	switch (setting.security_type) {
	case RTW_SECURITY_OPEN:
		printf("\n\r  SECURITY => OPEN");
		break;
	case RTW_SECURITY_WEP_PSK:
		printf("\n\r  SECURITY => WEP");
		printf("\n\r KEY INDEX => %d", setting.key_idx);
		break;
	case RTW_SECURITY_WPA_TKIP_PSK:
		printf("\n\r	SECURITY => WPA TKIP");
		break;
	case RTW_SECURITY_WPA_AES_PSK:
		printf("\n\r	SECURITY => WPA AES");
		break;
	case RTW_SECURITY_WPA_MIXED_PSK:
		printf("\n\r	SECURITY => WPA MIXED");
		break;
	case RTW_SECURITY_WPA2_AES_PSK:
		printf("\n\r	SECURITY => WPA2 AES");
		break;
	case RTW_SECURITY_WPA2_TKIP_PSK:
		printf("\n\r	SECURITY => WPA2 TKIP");
		break;
	case RTW_SECURITY_WPA2_MIXED_PSK:
		printf("\n\r	SECURITY => WPA2 MIXED");
		break;
	case RTW_SECURITY_WPA_WPA2_TKIP_PSK:
		printf("\n\r	SECURITY => WPA/WPA2 TKIP");
		break;
	case RTW_SECURITY_WPA_WPA2_AES_PSK:
		printf("\n\r	SECURITY => WPA/WPA2 AES");
		break;
	case RTW_SECURITY_WPA_WPA2_MIXED_PSK:
		printf("\n\r	SECURITY => WPA/WPA2 MIXED");
		break;
	case RTW_SECURITY_WPA_TKIP_ENTERPRISE:
		printf("\n\r	SECURITY => WPA TKIP ENTERPRISE");
		break;
	case RTW_SECURITY_WPA_AES_ENTERPRISE:
		printf("\n\r	SECURITY => WPA AES ENTERPRISE");
		break;
	case RTW_SECURITY_WPA_MIXED_ENTERPRISE:
		printf("\n\r	SECURITY => WPA MIXED ENTERPRISE");
		break;
	case RTW_SECURITY_WPA2_TKIP_ENTERPRISE:
		printf("\n\r	SECURITY => WPA2 TKIP ENTERPRISE");
		break;
	case RTW_SECURITY_WPA2_AES_ENTERPRISE:
		printf("\n\r	SECURITY => WPA2 AES ENTERPRISE");
		break;
	case RTW_SECURITY_WPA2_MIXED_ENTERPRISE:
		printf("\n\r	SECURITY => WPA2 MIXED ENTERPRISE");
		break;
	case RTW_SECURITY_WPA_WPA2_TKIP_ENTERPRISE:
		printf("\n\r	SECURITY => WPA/WPA2 TKIP ENTERPRISE");
		break;
	case RTW_SECURITY_WPA_WPA2_AES_ENTERPRISE:
		printf("\n\r	SECURITY => WPA/WPA2 AES ENTERPRISE");
		break;
	case RTW_SECURITY_WPA_WPA2_MIXED_ENTERPRISE:
		printf("\n\r	SECURITY => WPA/WPA2 MIXED ENTERPRISE");
		break;

#ifdef CONFIG_SAE_SUPPORT
	case RTW_SECURITY_WPA3_AES_PSK:
		printf("\n\r	SECURITY => WP3-SAE AES");
		break;
	case RTW_SECURITY_WPA2_WPA3_MIXED:
		printf("\n\r	SECURITY => WPA2/WPA3-SAE AES");
		break;
	case RTW_SECURITY_WPA3_ENTERPRISE:
		printf("\n\r	SECURITY => WPA3 ENTERPRISE");
		break;
#endif
	default:
		printf("\n\r  SECURITY => UNKNOWN");
	}

	printf("\n\r	PASSWORD => %s", setting.password);
	printf("\n\r");
}

static void wps_config_wifi_setting(rtw_network_info_t *wifi, struct dev_credential *dev_cred)
{
	printf("\r\nwps_config_wifi_setting\n");
	//memcpy((void *)wifi->ssid, (void *)dev_cred->ssid, dev_cred->ssid_len);
	strncpy((char *)wifi->ssid.val, (char *)&dev_cred->ssid[0], dev_cred->ssid_len);
	printf("\r\nwps_wifi.ssid = %s\n", wifi->ssid.val);
	wifi->ssid.len = dev_cred->ssid_len;
	printf("\r\nwps_wifi.ssid_len = %d\n", wifi->ssid.len);

	if (dev_cred->auth_type & (WPS_AUTH_TYPE_WPA2_PERSONAL | WPS_AUTH_TYPE_WPA2_ENTERPRISE)) {
		if ((dev_cred->encr_type & WPS_ENCR_TYPE_AES) && (dev_cred->encr_type & WPS_ENCR_TYPE_TKIP)) {
			printf("\r\nsecurity_type = RTW_SECURITY_WPA2_MIXED_PSK\n");
			wifi->security_type = RTW_SECURITY_WPA2_MIXED_PSK;
		} else if (dev_cred->encr_type & WPS_ENCR_TYPE_AES) {
			printf("\r\nsecurity_type = RTW_SECURITY_WPA2_AES_PSK\n");
			wifi->security_type = RTW_SECURITY_WPA2_AES_PSK;
		} else if (dev_cred->encr_type & WPS_ENCR_TYPE_TKIP) {
			printf("\r\nsecurity_type = RTW_SECURITY_WPA2_TKIP_PSK\n");
			wifi->security_type = RTW_SECURITY_WPA2_TKIP_PSK;
		}
	} else if (dev_cred->auth_type & (WPS_AUTH_TYPE_WPA_PERSONAL | WPS_AUTH_TYPE_WPA_ENTERPRISE)) {
		if (dev_cred->encr_type & WPS_ENCR_TYPE_AES) {
			printf("\r\nsecurity_type = RTW_SECURITY_WPA_AES_PSK\n");
			wifi->security_type = RTW_SECURITY_WPA_AES_PSK;
		} else if (dev_cred->encr_type & WPS_ENCR_TYPE_TKIP) {
			printf("\r\nsecurity_type = RTW_SECURITY_WPA_TKIP_PSK\n");
			wifi->security_type = RTW_SECURITY_WPA_TKIP_PSK;
		}
	} else if (dev_cred->auth_type & (WPS_AUTH_TYPE_OPEN | WPS_AUTH_TYPE_SHARED)) {
		if (dev_cred->encr_type & WPS_ENCR_TYPE_WEP) {
			printf("\r\nsecurity_type = RTW_SECURITY_WEP_PSK\n");
			wifi->security_type = RTW_SECURITY_WEP_PSK;
			wifi->key_id = dev_cred->key_idx - 1;
		} else {
			printf("\r\nsecurity_type = RTW_SECURITY_OPEN\n");
			wifi->security_type = RTW_SECURITY_OPEN;
		}
	}

	printf("\r\nwps_wifi.security_type = %d\n", (int)wifi->security_type);

	//memcpy(wifi->password, dev_cred->key, dev_cred->key_len);
	wifi->password = dev_cred->key;
	printf("\r\nwps_wifi.password = %s\n", wifi->password);
	wifi->password_len = dev_cred->key_len;
	printf("\r\nwps_wifi.password_len = %d", wifi->password_len);
	wifi->is_wps_trigger = _TRUE;
	//xSemaphoreGive(wps_reconnect_semaphore);
	//printf("\r\nrelease wps_reconnect_semaphore");
}

static int wps_connect_to_AP_by_certificate(rtw_network_info_t *wifi)
{
	int retry_count = WPS_CONNECT_RETRY_COUNT, ret;

	printf("\r\n=============== wifi_certificate_info ===============\n");
	printf("\r\nwps_wifi.ssid = %s\n", wifi->ssid.val);
	printf("\r\nsecurity_type = %d\n", (int)wifi->security_type);
	printf("\r\nwps_wifi.password = %s\n", wifi->password);
	printf("\r\nssid_len = %d\n", wifi->ssid.len);
	printf("\r\npassword_len = %d\n", wifi->password_len);
	printf("\r\nis_wps_trigger = %d\n", wifi->is_wps_trigger);
	while (1) {
		ret = wifi_connect(wifi, 1);
		if (ret == RTW_SUCCESS) {
			if (retry_count == WPS_CONNECT_RETRY_COUNT) {
				rtw_msleep_os(1000);    //When start wps with OPEN AP, AP will send a disassociate frame after STA connected, need reconnect here.
			}
			if (RTW_SUCCESS == wifi_is_connected_to_ap()) {
				//printf("\r\n[WPS]Ready to tranceive!!\n");
				wps_check_and_show_connection_info();
				break;
			}
		}
		if (retry_count == 0) {
			printf("\r\n[WPS]Join bss failed\n");
			ret = -1;
			break;
		}
		retry_count --;
		rtw_msleep_os(WPS_CONNECT_RETRY_INTERVAL);
	}
	return ret;
}

static int wps_connect_to_AP_by_open_system(char *target_ssid, u8 channel)
{
	int retry_count = 3, ret;
	rtw_network_info_t connect_param = {0};

	if (target_ssid != NULL) {
		memcpy(connect_param.ssid.val, target_ssid, strlen(target_ssid));
		connect_param.ssid.len = strlen(target_ssid);
		connect_param.security_type = RTW_SECURITY_OPEN;
		if (channel) {
			connect_param.channel = channel;
			connect_param.pscan_option = PSCAN_FAST_SURVEY;
		}
		rtw_msleep_os(500);	//wait scan complete.
		while (1) {
			ret = wifi_connect(&connect_param, 1);
			if (ret == RTW_SUCCESS) {
				//wps_check_and_show_connection_info();
				break;
			}
			if (retry_count == 0) {
				printf("\r\n[WPS]Join bss failed\n");
				return -1;
			}
			retry_count --;
		}
		//
	} else {
		printf("\r\n[WPS]Target SSID is NULL\n");
	}

	return 0;
}

static int wps_connect_to_AP_by_open_system_with_bssid(char *target_ssid, unsigned char *target_bssid)
{
	int retry_count = 3, ret;
	rtw_network_info_t connect_param = {0};

	rtw_memset(&connect_param, 0, sizeof(rtw_network_info_t));

	if ((target_ssid != NULL) && (target_bssid != NULL)) {
		rtw_msleep_os(500);	//wait scan complete.
		while (1) {
			memcpy(connect_param.ssid.val, target_ssid, strlen(target_ssid));
			connect_param.ssid.len = strlen(target_ssid);
			memcpy(connect_param.bssid.octet, target_bssid, ETH_ALEN);
			connect_param.security_type = RTW_SECURITY_OPEN;

			ret = wifi_connect(&connect_param, 1);
			if (ret == RTW_SUCCESS) {
				//wps_check_and_show_connection_info();
				break;
			}
			if (retry_count == 0) {
				printf("\r\n[WPS]Join bss failed\n");
				return -1;
			}
			retry_count --;
		}
	} else {
		printf("\r\n[WPS]Target SSID or BSSID is NULL\n");
	}

	return 0;
}

static void process_wps_scan_result(rtw_scan_result_t *record, void *user_data)
{
	u8 zero_mac[ETH_ALEN] = {0};
	internal_wps_scan_handler_arg_t *wps_arg = (internal_wps_scan_handler_arg_t *)user_data;

	if ((record->wps_type != 0xff) && (record->channel != 0) &&
		(memcmp(&record->BSSID, zero_mac, 6) != 0) && (!(record->security & WEP_ENABLED))) {
		// ignore hidden ssid
		if (record->SSID.len == 0) {
			return;
		} else {
			int i;
			u8 is_hidden_ssid = 1;

			for (i = 0; i < record->SSID.len; i ++) {
				if (record->SSID.val[i] != 0) {
					is_hidden_ssid = 0;
					break;
				}
			}

			if (is_hidden_ssid) {
				return;
			}
		}

		if (wps_arg->config_method == WPS_CONFIG_PUSHBUTTON) {
			if (record->wps_type == 0x04) {
				wps_password_id = record->wps_type;
				if (record->channel > 14) {
					if (++wps_arg->isoverlap_5G == 0) {
						memcpy(&wps_arg->target_ssid[0], record->SSID.val, record->SSID.len);
						memcpy(wps_arg->target_bssid, record->BSSID.octet, ETH_ALEN);
						wps_arg->target_ssid[record->SSID.len] = '\0';
						printf("\r\n[pbc]Record first triger wps 5G AP = %s, %02x:%02x:%02x:%02x:%02x:%02x\n", \
							   wps_arg->target_ssid, wps_arg->target_bssid[0], wps_arg->target_bssid[1], wps_arg->target_bssid[2], \
							   wps_arg->target_bssid[3], wps_arg->target_bssid[4], wps_arg->target_bssid[5]);
					}
				} else {
					if ((++wps_arg->isoverlap == 0) && (wps_arg->isoverlap_5G == -1)) {
						memcpy(&wps_arg->target_ssid[0], record->SSID.val, record->SSID.len);
						memcpy(wps_arg->target_bssid, record->BSSID.octet, ETH_ALEN);
						wps_arg->target_ssid[record->SSID.len] = '\0';
						printf("\r\n[pbc]Record first triger wps AP = %s, %02x:%02x:%02x:%02x:%02x:%02x\n", \
							   wps_arg->target_ssid, wps_arg->target_bssid[0], wps_arg->target_bssid[1], wps_arg->target_bssid[2], \
							   wps_arg->target_bssid[3], wps_arg->target_bssid[4], wps_arg->target_bssid[5]);
					}
				}
			}
		} else if (wps_arg->config_method == WPS_CONFIG_DISPLAY || wps_arg->config_method == WPS_CONFIG_KEYPAD) {
			if (record->wps_type == 0x00) {
				wps_arg->isoverlap = 0;
				wps_password_id = record->wps_type;
				memcpy(&wps_arg->target_ssid[0], record->SSID.val, record->SSID.len);
				memcpy(wps_arg->target_bssid, record->BSSID.octet, ETH_ALEN);
				wps_arg->target_ssid[record->SSID.len] = '\0';
				printf("\r\n[pin]find out first triger wps AP = %s, %02x:%02x:%02x:%02x:%02x:%02x\n", \
					   wps_arg->target_ssid, wps_arg->target_bssid[0], wps_arg->target_bssid[1], wps_arg->target_bssid[2], \
					   wps_arg->target_bssid[3], wps_arg->target_bssid[4], wps_arg->target_bssid[5]);
			}
		}
	}
}

#ifndef CONFIG_ENABLE_WPS_DISCOVERY
#define CONFIG_ENABLE_WPS_DISCOVERY 0
#endif
#if CONFIG_ENABLE_WPS_DISCOVERY
#define DISCOVERED_SSIDS_NUM 10
static char *discovered_ssids[DISCOVERED_SSIDS_NUM];
static char discovery_ssid[64];

static void reset_discovery_phase(void)
{
	memset(discovered_ssids, 0, sizeof(discovered_ssids));
	memset(discovery_ssid, 0, sizeof(discovery_ssid));
}

static void reset_discovery_ssid(void)
{
	memset(discovery_ssid, 0, sizeof(discovery_ssid));
}

static void clean_discovered_ssids(void)
{
	int i;

	for (i = 0; i < DISCOVERED_SSIDS_NUM; i ++) {
		if (discovered_ssids[i]) {
			free(discovered_ssids[i]);
			discovered_ssids[i] = NULL;
		}
	}
}

static void update_discovered_ssids(char *ssid)
{
	int i;

	for (i = 0; i < DISCOVERED_SSIDS_NUM; i ++) {
		if (discovered_ssids[i] != NULL) {
			if (strcmp(discovered_ssids[i], ssid) == 0) {
				break;
			}
		} else {
			if (strlen(discovery_ssid) == 0) {
				discovered_ssids[i] = malloc(strlen(ssid) + 1);
				strcpy(discovered_ssids[i], ssid);
				strncpy(discovery_ssid, ssid, sizeof(discovery_ssid));
				break;
			}
		}
	}
}

static int start_discovery_phase(u16 wps_config)
{
	struct dev_credential *dev_cred;
	rtw_network_info_t wifi = {0};
	int ret = 0;


	if (strlen(discovery_ssid) == 0) {
		//clean_discovered_ssids();
		return -1;
	}

	printf("\ndiscovery_ssid=%s\n", discovery_ssid);

	if (queue_for_credential != NULL) {
		os_xqueue_delete(queue_for_credential);
		queue_for_credential = NULL;
	}
	queue_for_credential = os_xqueue_create(wps_max_cred_count, sizeof(struct dev_credential));
	if (!queue_for_credential) {
		return -1;
	}

	wifi_reg_event_handler(WIFI_EVENT_WPA_STA_WPS_START, wpas_wsc_sta_wps_start_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_WPA_WPS_FINISH, wpas_wsc_wps_finish_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_WPA_EAPOL_RECVD, wpas_wsc_eapol_recvd_hdl, NULL);

	wpas_wps_enrollee_init_probe_ie(wps_config);
	wpas_wps_enrollee_init_assoc_ie();
	wifi_set_wps_phase(ENABLE);

	if (wps_stop_notified) {
		ret = -1;
		goto exit;
	}

	ret = wps_connect_to_AP_by_open_system(discovery_ssid, 0);
	if (ret < 0) {
		goto exit;
	}

	dev_cred = (struct dev_credential *)os_zalloc(sizeof(struct dev_credential) * wps_max_cred_count);
	if (!dev_cred) {
		printf("\n\rWPS: dev_credential allocate fail\n");
		goto exit;
	}

	for (int i = 0; i < wps_max_cred_count; i++) {
		os_xqueue_receive(queue_for_credential, &dev_cred[i], 20);

		// ssid == 0: no more credential
		if (dev_cred[i].ssid[0] == 0) {
			break;
		}
	}

	// choose first credential as default
	if (dev_cred[0].ssid[0] != 0 && dev_cred[0].ssid_len <= 32) {
		wps_config_wifi_setting(&wifi, &dev_cred[0]);
		wifi_set_wps_phase(DISABLE);
		ret = wps_connect_to_AP_by_certificate(&wifi);
		os_free(dev_cred, 0);
		goto exit1;
	} else {
		ret = -1;
	}

	os_free(dev_cred, 0);
exit:
	wifi_set_wps_phase(DISABLE);
exit1:
	if (queue_for_credential != NULL) {
		os_xqueue_delete(queue_for_credential);
		queue_for_credential = NULL;
	}

	wifi_unreg_event_handler(WIFI_EVENT_WPA_STA_WPS_START, wpas_wsc_sta_wps_start_hdl);
	wifi_unreg_event_handler(WIFI_EVENT_WPA_WPS_FINISH, wpas_wsc_wps_finish_hdl);
	wifi_unreg_event_handler(WIFI_EVENT_WPA_EAPOL_RECVD, wpas_wsc_eapol_recvd_hdl);

	wpas_wps_deinit();
	vTaskDelay(10);
	return ret;
}
#endif /* CONFIG_ENABLE_WPS_DISCOVERY */

static rtw_result_t wps_scan_result_handler(unsigned int scanned_AP_num, void *user_data)
{
	internal_wps_scan_handler_arg_t *wps_arg = (internal_wps_scan_handler_arg_t *)user_data;
	rtw_scan_result_t *scaned_ap_info;
	char *scan_buf = NULL;
	int ret = RTW_SUCCESS;
	unsigned int i = 0;

	if (scanned_AP_num == 0) {
		ret = RTW_ERROR;
		goto EXIT;
	}

	scan_buf = (char *)rtw_zmalloc(scanned_AP_num * sizeof(rtw_scan_result_t));
	if (scan_buf == NULL) {
		printf("malloc scan buf fail for wps\n");
		ret = RTW_ERROR;
		goto EXIT;
	}

	if (wifi_get_scan_records(&scanned_AP_num, scan_buf) < 0) {
		ret = RTW_ERROR;
		goto EXIT;
	}

	for (i = 0; i < scanned_AP_num; i++) {
		scaned_ap_info = (rtw_scan_result_t *)(scan_buf + i * sizeof(rtw_scan_result_t));
		scaned_ap_info->SSID.val[scaned_ap_info->SSID.len] = 0; /* Ensure the SSID is null terminated */

		process_wps_scan_result(scaned_ap_info, (void *)wps_arg);

#if CONFIG_ENABLE_WPS_DISCOVERY
		if (((wps_arg->config_method == WPS_CONFIG_DISPLAY) || (wps_arg->config_method == WPS_CONFIG_KEYPAD))
			&& (scaned_ap_info->wps_type == 0x07)) {

			update_discovered_ssids((char *)scaned_ap_info->SSID.val);
		}
#endif
	}
EXIT:
	if (scan_buf) {
		rtw_mfree((u8 *)scan_buf, 0);
	}
	printf("\r\nWPS scan done!\r\n");
	rtw_up_sema(&wps_arg->scan_sema);
	return (rtw_result_t)ret;
}

static int wps_find_out_triger_wps_AP(char *target_ssid, unsigned char *target_bssid, u16 config_method)
{
	int isoverlap = -1;
	internal_wps_scan_handler_arg_t wps_arg = {0};
	rtw_scan_param_t scan_param;

	wps_password_id = 0xFF;

	wps_arg.isoverlap = -1;
	wps_arg.isoverlap_5G = -1;
	wps_arg.config_method = config_method;
	wps_arg.target_ssid = target_ssid;
	wps_arg.target_bssid = target_bssid;

	rtw_init_sema(&wps_arg.scan_sema, 0);
	if (wps_arg.scan_sema == NULL) {
		return RTW_ERROR;
	}

	rtw_memset(&scan_param, 0, sizeof(rtw_scan_param_t));
	scan_param.scan_user_data = &wps_arg;
	scan_param.scan_user_callback = wps_scan_result_handler;

	if (wifi_scan_networks(&scan_param, 0) != RTW_SUCCESS) {
		printf("\n\rERROR: wifi scan failed");
		goto exit;
	}
	if (rtw_down_timeout_sema(&wps_arg.scan_sema, SCAN_LONGEST_WAIT_TIME) == _FAIL) {
		printf("\r\nWPS scan done early!\r\n");
	}

exit:
	rtw_free_sema(&wps_arg.scan_sema);

	if ((wps_arg.isoverlap >= 0) && (wps_arg.isoverlap_5G >= 0)) {
		isoverlap = wps_arg.isoverlap + wps_arg.isoverlap_5G;
	} else if (wps_arg.isoverlap_5G >= 0) {
		isoverlap = wps_arg.isoverlap_5G;
	} else if (wps_arg.isoverlap >= 0) {
		isoverlap = wps_arg.isoverlap;
	}

	return isoverlap;
}

static u8 wps_scan_cred_ssid(struct dev_credential *dev_cred)
{
	u8 ssid_found = 0;
	char *scan_buf = NULL;
	rtw_scan_param_t scan_param;
	rtw_scan_result_t *scanned_ap_info;
	int scanned_ap_num, i = 0;

	//set scan_param for scan
	rtw_memset(&scan_param, 0, sizeof(rtw_scan_param_t));
	scan_param.ssid = (char *)(dev_cred->ssid);

	if ((scanned_ap_num = wifi_scan_networks(&scan_param, 1)) <= 0) {
		printf("\n\rERROR: wifi scan failed");
	} else {
		scan_buf = (char *)rtw_zmalloc(scanned_ap_num * sizeof(rtw_scan_result_t));
		if (scan_buf == NULL) {
			// if cannot scan, suppose it can be found
			ssid_found = 1;
			return ssid_found;
		}
		if (wifi_get_scan_records((unsigned int *)(&scanned_ap_num), scan_buf) < 0) {
			rtw_mfree((u8 *)scan_buf, 0);
			return ssid_found;
		}

		for (i = 0; i < scanned_ap_num; i++) {
			scanned_ap_info = (rtw_scan_result_t *)(scan_buf + i * sizeof(rtw_scan_result_t));
			if ((scanned_ap_info->SSID.len == dev_cred->ssid_len) && (memcmp(scanned_ap_info->SSID.val, dev_cred->ssid, dev_cred->ssid_len) == 0)) {
				ssid_found = 1;
				break;
			}
		}

		rtw_mfree((u8 *)scan_buf, 0);
	}

	return ssid_found;
}

static void wps_filter_cred_by_scan(struct dev_credential *dev_cred, int cred_cnt)
{
	u8 ssid_found_count = 0;
	u8 *ssid_found_flags = (u8 *) malloc(cred_cnt);

	if (ssid_found_flags) {
		int i, times;

		for (times = 0; times < (WPS_CONNECT_RETRY_COUNT + 1); times ++) {
			memset(ssid_found_flags, 0, cred_cnt);
			ssid_found_count = 0;

			for (i = 0; i < cred_cnt; i ++) {
				ssid_found_flags[i] = wps_scan_cred_ssid(&dev_cred[i]);
				ssid_found_count += ssid_found_flags[i];
			}

			if (ssid_found_count) {
				break;
			} else {
				rtw_msleep_os(WPS_CONNECT_RETRY_INTERVAL);
			}
		}

		for (i = 0; i < cred_cnt; i ++) {
			if (ssid_found_flags[i] == 0) {
				memset(&dev_cred[i], 0, sizeof(struct dev_credential));
			}
		}

		free(ssid_found_flags);
	}
}

int wps_start(u16 wps_config, char *pin, u8 channel, char *ssid)
{
	struct dev_credential *dev_cred;
	rtw_network_info_t wifi = {0};
	char target_ssid[64];
	unsigned char target_bssid[ETH_ALEN];
	int is_overlap = -1;
	u32 start_time = rtw_get_current_time();
	int ret = 0;
	// for multiple credentials
	int cred_cnt = 0;
	int select_index = 0;
	u32 select_security = 0;

	if (wps_max_cred_count < 1 || wps_max_cred_count > 10) {
		printf("\n\rWPS: wps_max_cred_count should be in range 1~10\n");
		return -1;
	}

	memset(target_ssid, 0, 64);
	memset(target_bssid, 0, ETH_ALEN);
	wps_stop_notified = 0;

	if ((wps_config != WPS_CONFIG_PUSHBUTTON)
		&& (wps_config != WPS_CONFIG_DISPLAY)
		&& (wps_config != WPS_CONFIG_KEYPAD)) {
		printf("\n\rWPS: Wps method(%d) is wrong. Not triger WPS.\n", wps_config);
		return -1;
	}
	config_method = wps_config;

	if (wps_config == WPS_CONFIG_DISPLAY
		|| wps_config == WPS_CONFIG_KEYPAD) {
		if (pin) {
			strncpy(wps_pin_code, pin, sizeof(wps_pin_code) - 1);
		} else {
			printf("\n\rWPS: PIN is NULL. Not triger WPS.\n");
			return -1;
		}
	}

	if (!ssid)	{
#if CONFIG_ENABLE_WPS_DISCOVERY
		reset_discovery_phase();
#endif
		while (1) {
			unsigned int current_time = rtw_get_current_time();
			if ((rtw_systime_to_sec(current_time - start_time) < 120) && !wps_stop_notified) {
#if CONFIG_ENABLE_WPS_DISCOVERY
				reset_discovery_ssid();
#endif
				wpas_wps_enrollee_init_probe_ie(wps_config);
				is_overlap = wps_find_out_triger_wps_AP(&target_ssid[0], &target_bssid[0], wps_config);
				if ((is_overlap == 0) || (is_overlap > 0)) {
					break;
				}
#if CONFIG_ENABLE_WPS_DISCOVERY
				if ((wps_config == WPS_CONFIG_DISPLAY) || (wps_config == WPS_CONFIG_KEYPAD)) {
					if (start_discovery_phase(wps_config) == 0) {
						clean_discovered_ssids();
						return 0;
					}
				}
#endif
			} else {
				printf("\r\nWPS: WPS Walking Time Out\n");
				return -2;
			}
		}
#if CONFIG_ENABLE_WPS_DISCOVERY
		clean_discovered_ssids();
#endif
		if (is_overlap > 0) {
			printf("\r\nWPS: WPS session overlap. Not triger WPS.\n");
			return -2;
		}
	} else {
		memcpy(target_ssid, ssid, strlen(ssid));
	}

	if (queue_for_credential != NULL) {
		os_xqueue_delete(queue_for_credential);
		queue_for_credential = NULL;
	}
	queue_for_credential = os_xqueue_create(wps_max_cred_count, sizeof(struct dev_credential));
	if (!queue_for_credential) {
		return -1;
	}

	wifi_reg_event_handler(WIFI_EVENT_WPA_STA_WPS_START, wpas_wsc_sta_wps_start_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_WPA_WPS_FINISH, wpas_wsc_wps_finish_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_WPA_EAPOL_RECVD, wpas_wsc_eapol_recvd_hdl, NULL);

	wpas_wps_enrollee_init_probe_ie(wps_config);
	wpas_wps_enrollee_init_assoc_ie();
	wifi_set_wps_phase(ENABLE);
	if (ssid)
		/*for the customers who scan themself use wps_start to connect directly*/
	{
		ret = wps_connect_to_AP_by_open_system(target_ssid, channel);
	} else
		/*for wifi mesh AP, should use bssid to choose the right AP*/
	{
		ret = wps_connect_to_AP_by_open_system_with_bssid(target_ssid, target_bssid);
	}
	if (ret < 0) {
		printf("\n\rWPS: WPS Fail!!\n");
		goto exit;
	}

	dev_cred = (struct dev_credential *)os_zalloc(sizeof(struct dev_credential) * wps_max_cred_count);
	if (!dev_cred) {
		printf("\n\rWPS: dev_credential allocate fail\n");
		goto exit;
	}

	for (int i = 0; i < wps_max_cred_count; i++) {
		os_xqueue_receive(queue_for_credential, &dev_cred[i], 120);

		// ssid == 0: no more credential
		if (dev_cred[i].ssid[0] == 0) {
			break;
		} else {
			cred_cnt++;
		}
	}

	wifi_disconnect();

	// filter 5G rf_band cred
	if (cred_cnt > 1) {
		wps_filter_cred_by_scan(dev_cred, cred_cnt);
	}

	// check got credentials and select the most secure one to connect
	for (int cur_index = 0; cur_index < cred_cnt; cur_index++) {
		u32 cur_security = 0;
		//printf("\r\nWPS: check %d th cred\n", cur_index+1);
		if (dev_cred[cur_index].ssid_len == 0) {
			continue;
		}

		if (dev_cred[cur_index].auth_type & (WPS_AUTH_TYPE_WPA2_PERSONAL | WPS_AUTH_TYPE_WPA2_ENTERPRISE)) {
			if ((dev_cred[cur_index].encr_type & WPS_ENCR_TYPE_AES) && (dev_cred[cur_index].encr_type & WPS_ENCR_TYPE_TKIP)) {
				cur_security = RTW_SECURITY_WPA2_MIXED_PSK;
			} else if (dev_cred[cur_index].encr_type & WPS_ENCR_TYPE_AES) {
				cur_security = RTW_SECURITY_WPA2_AES_PSK;
			} else if (dev_cred[cur_index].encr_type & WPS_ENCR_TYPE_TKIP) {
				cur_security = RTW_SECURITY_WPA2_TKIP_PSK;
			}
		} else if (dev_cred[cur_index].auth_type & (WPS_AUTH_TYPE_WPA_PERSONAL | WPS_AUTH_TYPE_WPA_ENTERPRISE)) {
			if (dev_cred[cur_index].encr_type & WPS_ENCR_TYPE_AES) {
				cur_security = RTW_SECURITY_WPA_AES_PSK;
			} else if (dev_cred[cur_index].encr_type & WPS_ENCR_TYPE_TKIP) {
				cur_security = RTW_SECURITY_WPA_TKIP_PSK;
			}
		} else if (dev_cred[cur_index].auth_type & (WPS_AUTH_TYPE_OPEN | WPS_AUTH_TYPE_SHARED)) {
			if (dev_cred[cur_index].encr_type & WPS_ENCR_TYPE_WEP) {
				cur_security = RTW_SECURITY_WEP_PSK;
			} else {
				cur_security = RTW_SECURITY_OPEN;
			}
		}
		//printf("\r\nWPS: cur_security: %d\n", cur_security);
		if (cur_security >= select_security) {
			//printf("\r\nWPS: update index to %d of security type %d\n", cur_index, cur_security);
			select_security = cur_security;
			select_index = cur_index;
		}
	}

	// choose first credential as default
	if (dev_cred[select_index].ssid[0] != 0 && dev_cred[select_index].ssid_len <= 32) {
		wps_config_wifi_setting(&wifi, &dev_cred[select_index]);
		wifi_set_wps_phase(DISABLE);
		ret = wps_connect_to_AP_by_certificate(&wifi);
		os_free(dev_cred, 0);
		goto exit1;
	} else {
		printf("\n\rWPS: WPS FAIL!!!\n");
		printf("\n\rWPS: WPS FAIL!!!\n");
		printf("\n\rWPS: WPS FAIL!!!\n");
		ret = -1;
	}
	os_free(dev_cred, 0);
exit:
	wifi_set_wps_phase(DISABLE);
exit1:
	if (queue_for_credential != NULL) {
		os_xqueue_delete(queue_for_credential);
		queue_for_credential = NULL;
	}

	wifi_unreg_event_handler(WIFI_EVENT_WPA_STA_WPS_START, wpas_wsc_sta_wps_start_hdl);
	wifi_unreg_event_handler(WIFI_EVENT_WPA_WPS_FINISH, wpas_wsc_wps_finish_hdl);
	wifi_unreg_event_handler(WIFI_EVENT_WPA_EAPOL_RECVD, wpas_wsc_eapol_recvd_hdl);

	wpas_wps_deinit();
	return ret;
}

void wps_stop(void)
{
	wps_stop_notified = 1;
	wpas_wsc_wps_finish_hdl(NULL, 0, 0, NULL);
}

int wps_judge_staion_disconnect(void)
{
	rtw_wifi_setting_t setting = {RTW_MODE_NONE, {0}, {0}, 0, RTW_SECURITY_OPEN, {0}, 0, 0, 0, 0, 0};

	if (wifi_get_setting(STA_WLAN_INDEX, &setting) != 0) {
		return -1;
	}

	if (os_strlen((char *)setting.ssid) > 0) {
		wifi_disconnect();
	}

	return 0;
}

void cmd_wps(int argc, char **argv)
{
	int ret = -1;
	(void) ret;
	rtw_join_status_t join_status = RTW_JOINSTATUS_UNKNOWN;

	if (wps_judge_staion_disconnect() != 0) {
		return;
	}

	// check if STA is conencting
	join_status = wifi_get_join_status();
	if ((join_status > RTW_JOINSTATUS_UNKNOWN) && (join_status < RTW_JOINSTATUS_SUCCESS)) {
		RTW_API_INFO("\nthere is ongoing wifi connect!");
		return;
	}

	if ((argc == 2 || argc == 3) && (argv[1] != NULL)) {
		if (strcmp(argv[1], "pin") == 0) {
			unsigned int pin_val = 0;
			/* start pin */
			if (argc == 2) {
				char device_pin[10];
				pin_val = wps_generate_pin();
				snprintf(device_pin, sizeof(device_pin), "%08d", pin_val);
				/* Display PIN 3 times to prevent to be overwritten by logs from other tasks */
				printf("\n\rWPS: Start WPS PIN Display. PIN: [%s]\n\r", device_pin);
				printf("\n\rWPS: Start WPS PIN Display. PIN: [%s]\n\r", device_pin);
				printf("\n\rWPS: Start WPS PIN Display. PIN: [%s]\n\r", device_pin);
				ret = wps_start(WPS_CONFIG_DISPLAY, (char *)device_pin, 0, NULL);
			} else {
				pin_val = atoi(argv[2]);
				if (!wps_pin_valid(pin_val)) {
					printf("\n\rWPS: Device pin code is invalid. Not triger WPS.\n");
					goto exit;
				}
				printf("\n\rWPS: Start WPS PIN Keypad.\n\r");
				ret = wps_start(WPS_CONFIG_KEYPAD, argv[2], 0, NULL);
			}
		} else if (strcmp(argv[1], "pbc") == 0) {
			/* start pbc */
			printf("\n\rWPS: Start WPS PBC.\n\r");
			ret = wps_start(WPS_CONFIG_PUSHBUTTON, NULL, 0, NULL);
		} else {
			printf("\n\rWPS: Wps Method is wrong. Not triger WPS.\n");
			goto exit;
		}
	}
exit:
	return;
}

#endif //CONFIG_ENABLE_WPS
