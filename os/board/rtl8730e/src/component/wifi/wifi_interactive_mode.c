/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <debug.h>
#include <lwip/netif.h>

#include "os_wrapper.h"
#include "platform_opts.h"
#include "wifi_conf.h"
#include "wifi_intf_drv_to_upper.h"
#include "rtw_wifi_constants.h"
#include "lwip/ip_addr.h"
#include "lwip/ip4_addr.h"
#include "lwip/tcpip.h"
#include "rtk_wifi_utils.h"
#include <tinyara/net/if/wifi.h>
#include <netutils/netlib.h>
#include <net/if.h>

/* Give default value if not defined */
#ifndef NET_IF_NUM
#ifdef CONFIG_CONCURRENT_MODE
#define NET_IF_NUM 2
#else
#define NET_IF_NUM 1
#endif
#endif

/*Static IP ADDRESS*/
#ifndef IP_ADDR0
#define IP_ADDR0 192
#define IP_ADDR1 168
#define IP_ADDR2 1
#define IP_ADDR3 80
#endif

/*NETMASK*/
#ifndef NETMASK_ADDR0
#define NETMASK_ADDR0 255
#define NETMASK_ADDR1 255
#define NETMASK_ADDR2 255
#define NETMASK_ADDR3 0
#endif

/*Gateway Address*/
#ifndef GW_ADDR0
#define GW_ADDR0 192
#define GW_ADDR1 168
#define GW_ADDR2 1
#define GW_ADDR3 1
#endif

/*Static IP ADDRESS*/
#ifndef AP_IP_ADDR0
#define AP_IP_ADDR0 192
#define AP_IP_ADDR1 168
#define AP_IP_ADDR2 43
#define AP_IP_ADDR3 1
#endif

/*NETMASK*/
#ifndef AP_NETMASK_ADDR0
#define AP_NETMASK_ADDR0 255
#define AP_NETMASK_ADDR1 255
#define AP_NETMASK_ADDR2 255
#define AP_NETMASK_ADDR3 0
#endif

/*Gateway Address*/
#ifndef AP_GW_ADDR0
#define AP_GW_ADDR0 192
#define AP_GW_ADDR1 168
#define AP_GW_ADDR2 43
#define AP_GW_ADDR3 1
#endif

#if CONFIG_WLAN
int8_t cmd_wifi_on(WiFi_InterFace_ID_t interface_id);
int8_t cmd_wifi_off(void);
int8_t cmd_wifi_disconnect(void);

static int _find_ap_from_scan_buf(char *target_ssid, void *user_data, int ap_num)
{
	rtw_wifi_setting_t *pwifi = (rtw_wifi_setting_t *)user_data;
	rtw_scan_result_t *scanned_ap_info;
	int i = 0;
	char *scan_buf = NULL;

	scan_buf = (char *)rtos_mem_zmalloc(ap_num * sizeof(rtw_scan_result_t));
	if (scan_buf == NULL) {
		return -1;
	}

	if (wifi_get_scan_records((unsigned int *)&ap_num, scan_buf) < 0) {
		rtos_mem_free((void *)scan_buf);
		return -1;
	}

	for (i = 0; i < ap_num; i++) {
		scanned_ap_info = (rtw_scan_result_t *)(scan_buf + i * sizeof(rtw_scan_result_t));
		if ((scanned_ap_info->SSID.len == strlen(target_ssid)) && (!memcmp(scanned_ap_info->SSID.val, target_ssid, scanned_ap_info->SSID.len))) {
			strncpy((char *)pwifi->ssid, target_ssid, sizeof(pwifi->ssid) - 1);
			pwifi->ssid[sizeof(pwifi->ssid) - 1] = '\0';
			pwifi->channel = scanned_ap_info->channel;
			pwifi->security_type = scanned_ap_info->security;
		}
	}
	rtos_mem_free((void *)scan_buf);
	return 0;

}

static int _get_ap_security_mode(IN char *ssid, OUT rtw_security_t *security_mode, OUT u8 *channel)
{
	rtw_wifi_setting_t wifi;
	rtw_scan_param_t scan_param;
	int scanned_ap_num = 0;
	int ret = 0;

	memset(&wifi, 0, sizeof(wifi));

	memset(&scan_param, 0, sizeof(rtw_scan_param_t));
	scan_param.ssid[0].ssid = ssid;
	scanned_ap_num = wifi_scan_networks(&scan_param, 1);

	if (scanned_ap_num <= 0) {
		printf("Wifi scan failed!\n");
		ret = 0;
	} else {
		_find_ap_from_scan_buf(ssid, (void *)&wifi, scanned_ap_num);

		if (strcmp((const char *)wifi.ssid, ssid) == 0) {
			*security_mode = wifi.security_type;
			*channel = wifi.channel;
			ret = wifi.security_type;
		}
	}

	return ret;
}

extern struct netif xnetif[NET_IF_NUM]; /* network interface structure */
static int _netlib_setmacaddr(const char *ifname, const uint8_t *macaddr)
{
	int ret = ERROR;
	if (ifname && macaddr) {
		/* Get a socket (only so that we get access to the INET subsystem) */

		int sockfd = socket(2, NETLIB_SOCK_IOCTL, 0);
		if (sockfd >= 0) {
			struct ifreq req;

			/* Put the driver name into the request */
			strncpy(req.ifr_name, ifname, IFNAMSIZ - 1);
			req.ifr_name[IFNAMSIZ - 1] = '\0';

			/* Put the new MAC address into the request */
			req.ifr_hwaddr.sa_family = 2;
			memcpy(&req.ifr_hwaddr.sa_data, macaddr, 6);

			/* Perform the ioctl to set the MAC address */
			ret = netdev_ifrioctl(NULL, 1813, &req);
			close(sockfd);
		}
	}
	return ret;
}

int8_t cmd_wifi_ap(trwifi_softap_config_s *softap_config)
{
	int ret = 0;
	rtw_security_t security_type;
	char *password;
	rtw_softap_info_t rtw_AP_config = {0};
#if CONFIG_LWIP_LAYER
	struct netif *pnetif = &xnetif[STA_WLAN_INDEX];
	u32 ip_addr;
	u32 netmask;
	u32 gw;
#endif
	nvdbg("\n\rStarting AP ...");

	switch (softap_config->ap_auth_type) {
	case TRWIFI_AUTH_OPEN:
		security_type = RTW_SECURITY_OPEN;
		password = NULL;
		break;
	case TRWIFI_AUTH_WPA2_PSK:
		security_type = RTW_SECURITY_WPA2_AES_PSK;
		password = softap_config->passphrase;
		break;
	case TRWIFI_AUTH_WPA3_PSK:
		security_type = RTW_SECURITY_WPA3_AES_PSK;
		password = softap_config->passphrase;
		break;
	case TRWIFI_AUTH_UNKNOWN:
	default:
		ndbg("\r\nAP AUTH type is unknown %d;\n", softap_config->ap_auth_type);
		return TRWIFI_INVALID_ARGS;
	}

	if (softap_config->ssid_length < 1) {
		ndbg("\r\nAP ssid_length is wrong: %s,	%d;\n", softap_config->ssid, softap_config->ssid_length);
		return TRWIFI_INVALID_ARGS;
	}

	if (softap_config->ap_auth_type != TRWIFI_AUTH_OPEN && softap_config->passphrase_length < 1) {
		ndbg("\r\nAP passphrase_length is wrong: %s,  %d;\n", softap_config->passphrase, softap_config->passphrase_length);
		return TRWIFI_INVALID_ARGS;
	}

	//Populate softAP values
	rtw_AP_config.ssid.len = softap_config->ssid_length;
	memcpy(rtw_AP_config.ssid.val, (unsigned char *)softap_config->ssid, rtw_AP_config.ssid.len);
	rtw_AP_config.security_type = security_type;
	rtw_AP_config.password = (unsigned char*)password;
	rtw_AP_config.password_len = softap_config->passphrase_length;
	rtw_AP_config.channel = softap_config->channel;


#if CONFIG_LWIP_LAYER
	dhcps_stop(pnetif);
	ip_addr = WIFI_MAKEU32(GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
	netmask = WIFI_MAKEU32(NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
	gw = WIFI_MAKEU32(GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
	LwIP_SetIP(STA_WLAN_INDEX, ip_addr, netmask, gw);
#endif

	wifi_stop_ap();
	if (wifi_start_ap(&rtw_AP_config) != RTW_SUCCESS) {
		ndbg("\n\rERROR: Operation failed!");
		return -1;
	}

	nvdbg("\r\nap start");
#if CONFIG_LWIP_LAYER
		uint8_t *mac = (uint8_t *)LwIP_GetMAC(1);
		nvdbg("\n\r  MAC => %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		_netlib_setmacaddr(CONFIG_WIFIMGR_SOFTAP_IFNAME, mac);
#endif

#if CONFIG_LWIP_LAYER
	ip_addr = WIFI_MAKEU32(AP_IP_ADDR0, AP_IP_ADDR1, AP_IP_ADDR2, AP_IP_ADDR3);
	netmask = WIFI_MAKEU32(AP_NETMASK_ADDR0, AP_NETMASK_ADDR1, AP_NETMASK_ADDR2, AP_NETMASK_ADDR3);
	gw = WIFI_MAKEU32(AP_GW_ADDR0, AP_GW_ADDR1, AP_GW_ADDR2, AP_GW_ADDR3);
	LwIP_SetIP(STA_WLAN_INDEX, ip_addr, netmask, gw);
	dhcps_start(pnetif);
#endif

	return ret;
}

int8_t cmd_wifi_connect(trwifi_ap_config_s *ap_connect_config, void *arg, uint32_t ap_channel)
{
	int ret;

	char *ssid;
	rtw_security_t security_type = -1;
	char *password;
	int ssid_len;
	int password_len;
	int key_id = 0;
	void *semaphore;
	int security_retry_count = 0;
	uint8_t pscan_config = 0;
	rtw_network_info_t wifi_info = {0};

	trwifi_ap_auth_type_e auth = ap_connect_config->ap_auth_type;
	ssid = ap_connect_config->ssid;
	switch (auth) {
	case TRWIFI_AUTH_OPEN:
		security_type = RTW_SECURITY_OPEN;
		password = NULL;
		ssid_len = strlen((const char *)ssid);
		password_len = 0;
		semaphore = NULL;
		break;
	case TRWIFI_AUTH_WEP_SHARED:
		security_type = RTW_SECURITY_WEP_PSK;
		password = ap_connect_config->passphrase;
		ssid_len = strlen((const char *)ssid);
		password_len = ap_connect_config->passphrase_length;
		if ((password_len != 5) && (password_len != 13) && (password_len != 10) && (password_len != 26)) {
			ndbg("\n\rWrong WEP key length. Must be 5 or 13 ASCII characters, or 10 or 26 for HEX password.");
			return -1;
		}
		semaphore = NULL;
		break;
	case TRWIFI_AUTH_WPA2_PSK:
		security_type = RTW_SECURITY_WPA2_AES_PSK;
		password = ap_connect_config->passphrase;
		ssid_len = strlen((const char *)ssid);
		password_len = ap_connect_config->passphrase_length;
		semaphore = NULL;
		break;
	case TRWIFI_AUTH_WPA3_PSK:
	case TRWIFI_AUTH_WPA2_AND_WPA3_PSK:
		security_type = RTW_SECURITY_WPA3_AES_PSK;
		password = ap_connect_config->passphrase;
		ssid_len = strlen((const char *)ssid);
		password_len = ap_connect_config->passphrase_length;
		semaphore = NULL;
		break;
	case TRWIFI_AUTH_WPA_PSK:
		if (ap_connect_config->ap_crypto_type == TRWIFI_CRYPTO_AES) {
			security_type = RTW_SECURITY_WPA_AES_PSK;
		} else {
			security_type = RTW_SECURITY_WPA_TKIP_PSK;
		}
		password = ap_connect_config->passphrase;
		ssid_len = strlen((const char *)ssid);
		password_len = ap_connect_config->passphrase_length;
		semaphore = NULL;
		break;
	case TRWIFI_AUTH_WPA_AND_WPA2_PSK:
		if (ap_connect_config->ap_crypto_type == TRWIFI_CRYPTO_TKIP_AND_AES) {
			security_type = RTW_SECURITY_WPA2_MIXED_PSK;
		} else if (ap_connect_config->ap_crypto_type == TRWIFI_CRYPTO_AES) {
			security_type = RTW_SECURITY_WPA2_AES_PSK;
		} else {
			security_type = RTW_SECURITY_WPA2_TKIP_PSK;
		}
		password = ap_connect_config->passphrase;
		ssid_len = strlen((const char *)ssid);
		password_len = ap_connect_config->passphrase_length;
		semaphore = NULL;
		break;
	default:
		while (1) {
			security_type = _get_ap_security_mode((char *)ssid, &security_type, (u8 *) &ap_channel);
			if (security_type >= 0)
				break;

			security_retry_count++;
			if (security_retry_count >= 3) {
				ndbg("Can't get AP security mode.\n");
				break;
			}
		}
		password = ap_connect_config->passphrase;
		ssid_len = strlen((const char *)ssid);
		password_len = ap_connect_config->passphrase_length;
		semaphore = NULL;
		break;
	}

	if ((ap_channel >= 1) && (ap_channel <= 13)) {
		pscan_config = PSCAN_FAST_SURVEY; //justin temporary take off PSCAN_ENABLE flag
	}

	wifi_info.channel = ap_channel;
	wifi_info.pscan_option = pscan_config; //0 for normal partial scan, PSCAN_FAST_SURVEY for fast survey
	wifi_info.security_type = security_type;
	strncpy((char *)wifi_info.ssid.val, (char *)(ssid), sizeof(wifi_info.ssid.val) - 1);
	wifi_info.ssid.val[sizeof(wifi_info.ssid.val) - 1] = '\0';
	wifi_info.ssid.len = ssid_len;
	wifi_info.password = (unsigned char *)(password);
	wifi_info.password_len = password_len;
	wifi_info.key_id = key_id;

	ret = wifi_connect(&wifi_info, 1);

	if (ret) {
		ndbg("\n\rrtk_wifi_connect failed\n");
		return -1;
	}

	return 0;
}

int8_t cmd_wifi_connect_bssid(int argc, char **argv)
{
	int ret = RTW_ERROR;
	rtw_wifi_setting_t wlan_setting;
	int mode = 0;
	unsigned char bssid[ETH_ALEN];
	char *ssid = NULL;
	rtw_security_t security_type;
	char *password;
	int bssid_len;
	int ssid_len = 0;
	int password_len;
	int key_id;
	void *semaphore;
	u32 mac[ETH_ALEN];
	u32 i;
	u32 index = 0;

	if ((argc != 3) && (argc != 4) && (argc != 5) && (argc != 6)) {
		ndbg("\n\rUsage: wifi_connect_bssid 0/1 [SSID] BSSID / xx:xx:xx:xx:xx:xx [WPA PASSWORD / (5 or 13) ASCII WEP KEY] [WEP KEY ID 0/1/2/3]");
		return -1;
	}

	//Check if in AP mode
	wifi_get_setting(WLAN0_IDX, &wlan_setting);
	mode = wlan_setting.mode;

	//check ssid
	if (memcmp(argv[1], "0", 1)) {
		index = 1;
		ssid_len = strlen((const char *)argv[2]);
		if ((ssid_len <= 0) || (ssid_len > 32)) {
			ndbg("\n\rWrong ssid. Length must be less than 32.");
			return -1;
		}
		ssid = argv[2];
	}
	sscanf(argv[2 + index], MAC_FMT, mac, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5);
	for (i = 0; i < ETH_ALEN; i++)
		bssid[i] = (u8)mac[i] & 0xFF;

	if (argc == 3 + index) {
		security_type = RTW_SECURITY_OPEN;
		password = NULL;
		bssid_len = ETH_ALEN;
		password_len = 0;
		key_id = 0;
		semaphore = NULL;
	} else if (argc == 4 + index) {
		security_type = RTW_SECURITY_WPA2_AES_PSK;
		password = argv[3 + index];
		bssid_len = ETH_ALEN;
		password_len = strlen((const char *)argv[3 + index]);
		key_id = 0;
		semaphore = NULL;
	} else {
		security_type = RTW_SECURITY_WEP_PSK;
		password = argv[3 + index];
		bssid_len = ETH_ALEN;
		password_len = strlen((const char *)argv[3 + index]);
		key_id = atoi(argv[4 + index]);
		if ((password_len != 5) && (password_len != 13)) {
			ndbg("\n\rWrong WEP key length. Must be 5 or 13 ASCII characters.");
			return -1;
		}
		if ((key_id < 0) || (key_id > 3)) {
			ndbg("\n\rWrong WEP key id. Must be one of 0,1,2, or 3.");
			return -1;
		}
		semaphore = NULL;
	}

	ret = wifi_connect_bssid(bssid,
							 ssid,
							 security_type,
							 password,
							 bssid_len,
							 ssid_len,
							 password_len,
							 key_id,
							 semaphore);

	if (ret != RTW_SUCCESS) {
		ndbg("\n\rERROR: Operation failed!");
		return -1;
	} else {
		nvdbg("\r\nConnected\n");
	}

	return 0;
}

int8_t cmd_wifi_disconnect(void)
{
	int timeout = 20;
	rtw_join_status_t curr_join_status = wifi_get_join_status();

	nvdbg("\n\rDeassociating AP ...");

	if (curr_join_status == RTW_JOINSTATUS_DISCONNECT) {
		nvdbg("\n\rWIFI disconnected");
		return 0;
	}

	if (wifi_disconnect() < 0) {
		ndbg("\n\rERROR: Operation failed!");
		return -1;
	}

	while (1) {
		curr_join_status = wifi_get_join_status();
		if (curr_join_status == RTW_JOINSTATUS_DISCONNECT) {
			nvdbg("\n\rWIFI disconnected");
			return 0;
		}

		if (timeout == 0) {
			ndbg("\n\rERROR: Deassoc timeout!");
			return -1;
		}

		rtos_time_delay_ms(1);
		timeout--;
	}
}

extern int netdev_ifrioctl(FAR struct socket *sock, int cmd, FAR struct ifreq *req);

int8_t cmd_wifi_on(WiFi_InterFace_ID_t interface_id)
{
	rtw_wifi_setting_t setting;
	wifi_get_setting(WLAN0_IDX, &setting);

#if CONFIG_LWIP_LAYER
	uint8_t *mac = (uint8_t *)LwIP_GetMAC(0);
	nvdbg("\n\r  MAC => %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	_netlib_setmacaddr(CONFIG_WIFIMGR_STA_IFNAME, mac);
#endif
	nvdbg("\r\n===============>>Finish wifi_on!!\r\n");
	return RTK_STATUS_SUCCESS;
}

int8_t cmd_wifi_off(void)
{
/* 8730E doesn't support wifi_off() */
	return 0;
}

int8_t cmd_wifi_stop_ap(void)
{
	if (!wifi_stop_ap())
		return 0;
	return -1;
}
#endif //#if CONFIG_WLAN
