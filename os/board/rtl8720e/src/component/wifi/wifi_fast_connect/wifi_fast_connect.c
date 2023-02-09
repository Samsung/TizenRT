/******************************************************************************
 *
 * Copyright(c) 2007 - 2015 Realtek Corporation. All rights reserved.
 *
 *
 ******************************************************************************/

/** @file

This example demonstrate how to implement wifi fast reconnection
**/
#include <platform_opts.h>
#include <wifi_fast_connect.h>
#include <wifi_conf.h>

#include "FreeRTOS.h"
#include <task.h>
#include <platform_stdlib.h>
#include "osdep_service.h"
#include "flash_api.h"
#include "device_lock.h"
#if CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
#include "system_data_api.h"
extern struct netif xnetif[NET_IF_NUM];
#endif

#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
uint32_t offer_ip = 0;
uint32_t server_ip = 0;
#endif
#define WIFI_RETRYCOUNT 5
#define WIFI_FAST_CON_PARTION_SIZE 0x400
#define TOTAL_PARTION_SIZE 0x1000
int wifi_check_fast_connect_data(struct wlan_fast_reconnect *data);

/*
* Usage:
*       wifi connection indication trigger this function to save current
*       wifi profile in flash
*/

int write_fast_connect_data_to_flash(unsigned int offer_ip, unsigned int server_ip)
{
	/* To avoid gcc warnings */
#if(!defined(CONFIG_FAST_DHCP) || (!CONFIG_FAST_DHCP))
	(void) offer_ip;
	(void) server_ip;
#endif
	struct wlan_fast_reconnect read_data = {0};
	struct wlan_fast_reconnect wifi_data_to_flash = {0};
	rtw_wifi_setting_t setting;
	struct psk_info PSK_info;
	u32 channel = 0;

	/* STEP1: get current connect info from wifi driver*/
	if (wifi_get_setting(STA_WLAN_INDEX, &setting) || setting.mode == RTW_MODE_AP) {
		RTW_API_INFO("\r\n %s():wifi_get_setting fail or ap mode", __func__);
		return RTW_ERROR;
	}

	rtw_memset(&wifi_data_to_flash, 0, sizeof(struct wlan_fast_reconnect));
	channel = (u32)setting.channel;

	switch (setting.security_type) {
	case RTW_SECURITY_OPEN:
		rtw_memcpy(wifi_data_to_flash.psk_essid, setting.ssid, strlen((const char *)setting.ssid));
		wifi_data_to_flash.security_type = RTW_SECURITY_OPEN;
		break;
	case RTW_SECURITY_WEP_PSK:
		rtw_memcpy(wifi_data_to_flash.psk_essid, setting.ssid, strlen((const char *)setting.ssid));
		rtw_memcpy(wifi_data_to_flash.psk_passphrase, setting.password, strlen((const char *)setting.password));
		channel |= (setting.key_idx) << 28;
		wifi_data_to_flash.security_type = RTW_SECURITY_WEP_PSK;
		break;
	case RTW_SECURITY_WPA_TKIP_PSK:
	case RTW_SECURITY_WPA_AES_PSK:
	case RTW_SECURITY_WPA2_AES_PSK:
	case RTW_SECURITY_WPA2_TKIP_PSK:
#ifdef CONFIG_SAE_SUPPORT
	case RTW_SECURITY_WPA3_AES_PSK:
#endif
		rtw_memset(&PSK_info, 0, sizeof(struct psk_info));
		wifi_psk_info_get(&PSK_info);
		rtw_memcpy(wifi_data_to_flash.psk_essid, PSK_info.psk_essid, sizeof(wifi_data_to_flash.psk_essid));
		rtw_memcpy(wifi_data_to_flash.psk_passphrase, PSK_info.psk_passphrase, sizeof(wifi_data_to_flash.psk_passphrase));
		rtw_memcpy(wifi_data_to_flash.wpa_global_PSK, PSK_info.wpa_global_PSK, sizeof(wifi_data_to_flash.wpa_global_PSK));
		wifi_data_to_flash.security_type = setting.security_type;
		break;

	default:
		break;
	}
	rtw_memcpy(&(wifi_data_to_flash.channel), &channel, 4);
#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
	wifi_data_to_flash.offer_ip = offer_ip;
	wifi_data_to_flash.server_ip = server_ip;
#endif

	/* STEP2: get last time fast connect info from flash*/
	memset(&read_data, 0xff, sizeof(struct wlan_fast_reconnect));
	sys_read_wlan_data_from_flash((uint8_t *) &read_data,  sizeof(struct wlan_fast_reconnect));

#if ATCMD_VER == ATVER_2
	struct wlan_fast_reconnect *copy_data = (struct wlan_fast_reconnect *) &wifi_data_to_flash;
	copy_data->enable = read_data.enable;
#endif

	/* STEP3: wirte new connect info to flash if different content: SSID, Passphrase, Channel, Security type*/
	if (memcmp((u8 *) &wifi_data_to_flash, (u8 *) &read_data, sizeof(struct wlan_fast_reconnect)) != 0) {
#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
		printf("\r\n %s():not the same ssid/passphrase/channel/offer_ip, write new profile to flash \n", __func__);
#else
		printf("\r\n %s():not the same ssid/passphrase/channel, write new profile to flash \n", __func__);
#endif
		sys_write_wlan_data_to_flash((uint8_t *)&wifi_data_to_flash, sizeof(struct wlan_fast_reconnect));
	}

	return RTW_SUCCESS;
}

/*
* Usage:
*       This function read previous saved wlan profile in flash and execute connection.
*
*/
int wifi_do_fast_connect(void)
{
	struct wlan_fast_reconnect *data;
	uint32_t	channel;
	uint32_t    security_type;
	u8 key_id;
	int ret;
	uint32_t wifi_retry_connect = WIFI_RETRYCOUNT;//For fast wifi connect retry
	rtw_network_info_t wifi = {0};
	struct psk_info PSK_INFO;

#if CONFIG_LWIP_LAYER
	netifapi_netif_set_up(&xnetif[0]);
#endif

#if CONFIG_AUTO_RECONNECT
	//setup reconnection flag
	if (wifi_config_autoreconnect(1) < 0) {
		return -1;
	}
#endif
	data = (struct wlan_fast_reconnect *)malloc(sizeof(struct wlan_fast_reconnect));
	if (data) {
		memset(data, 0xff, sizeof(struct wlan_fast_reconnect));
		sys_read_wlan_data_from_flash((uint8_t *)data, sizeof(struct wlan_fast_reconnect));

		/* Check whether stored flash profile is empty */
		struct wlan_fast_reconnect *empty_data;
		empty_data = (struct wlan_fast_reconnect *)malloc(sizeof(struct wlan_fast_reconnect));
		if (empty_data) {
			memset(empty_data, 0xff, sizeof(struct wlan_fast_reconnect));
			if (memcmp(empty_data, data, sizeof(struct wlan_fast_reconnect)) == 0) {
				printf("[FAST_CONNECT] Fast connect profile is empty, abort fast connection\n");
				free(data);
				free(empty_data);
				return 0;
			}
			free(empty_data);
		}

		memset(&PSK_INFO, 0, sizeof(struct psk_info));
		rtw_memcpy(PSK_INFO.psk_essid, data->psk_essid, sizeof(data->psk_essid));
		rtw_memcpy(PSK_INFO.psk_passphrase, data->psk_passphrase, sizeof(data->psk_passphrase));
		rtw_memcpy(PSK_INFO.wpa_global_PSK, data->wpa_global_PSK, sizeof(data->wpa_global_PSK));

		if (wifi_check_fast_connect_data(data) < 0) {
			printf("[FAST_CONNECT] Fast connect profile is wrong, abort fast connection\n");
			free(data);
			return -1;
		}

		wifi_psk_info_set(&PSK_INFO);

		channel = data->channel;
		key_id = channel >> 28;
		channel &= 0xff;
		security_type = data->security_type;
		//set partial scan for entering to listen beacon quickly
WIFI_RETRY_LOOP:
		wifi.channel = channel;
		wifi.pscan_option = PSCAN_FAST_SURVEY;
		wifi.security_type = security_type;
		//SSID
		strcpy((char *)wifi.ssid.val, (char *)(data->psk_essid));
		wifi.ssid.len = strlen((char *)(data->psk_essid));

		switch (security_type) {
		case RTW_SECURITY_WEP_PSK:
			wifi.password = (unsigned char *)(data->psk_passphrase);
			wifi.password_len = strlen((char *)(data->psk_passphrase));
			wifi.key_id = key_id;
			break;
		case RTW_SECURITY_WPA_TKIP_PSK:
		case RTW_SECURITY_WPA_AES_PSK:
		case RTW_SECURITY_WPA2_AES_PSK:
		case RTW_SECURITY_WPA2_TKIP_PSK:
#ifdef CONFIG_SAE_SUPPORT
		case RTW_SECURITY_WPA3_AES_PSK:
#endif
			wifi.password = (unsigned char *)(data->psk_passphrase);
			wifi.password_len = strlen((char *)(data->psk_passphrase));
			break;
		default:
			break;
		}

#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
		offer_ip = data->offer_ip;
		server_ip = data->server_ip;
#endif

		ret = wifi_connect(&wifi, 1);
		if (ret != RTW_SUCCESS) {
			wifi_retry_connect--;
			if (wifi_retry_connect > 0) {
				/* Add the delay to wait for the _rtw_join_timeout_handler
				 * If there is no this delay, there are some error when rhe AP
				 * send the disassociation frame. It will cause the connection
				 * to be failed at first time after resetting. So keep 300ms delay
				 * here. For the detail about this error, please refer to
				 * [RSWLANDIOT-1954].
				 */
				vTaskDelay(300);
				printf("wifi retry\r\n");
				goto WIFI_RETRY_LOOP;
			}
		}
#if CONFIG_LWIP_LAYER
		if (ret == RTW_SUCCESS) {
			LwIP_DHCP(0, DHCP_START);
		}
#endif
		free(data);
	}

	return 0;
}

/*
* Usage:
*       After wifi connected, check if it is the same AP which previously stored in flash.
*       Return 0 means not the same AP or FASH DHCP not enabled, return 1 means the same AP.
*/
int check_is_the_same_ap()
{
	int ret = 0;
#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
	if (p_store_fast_connect_info != NULL) {
		struct wlan_fast_reconnect data;
		rtw_wifi_setting_t setting;

		if (wifi_get_setting(STA_WLAN_INDEX, &setting) || setting.mode == RTW_MODE_AP) {
			RTW_API_INFO("\r\n %s():wifi_get_setting fail or ap mode", __func__);
			return 0;
		}

		memset(&data, 0xff, sizeof(struct wlan_fast_reconnect));
		sys_read_wlan_data_from_flash((uint8_t *)&data, sizeof(struct wlan_fast_reconnect));

		if (strncmp((const char *)data.psk_essid, (const char *)setting.ssid, strlen((char const *)setting.ssid)) == 0) {
			ret = 1;
		} else {
			ret = 0;
		}
	}
#endif
	return ret;
}

int Erase_Fastconnect_data(void)
{
#ifndef CONFIG_USE_FLASHCFG
	flash_t flash;

	if (p_wifi_do_fast_connect != NULL) {
		device_mutex_lock(RT_DEV_LOCK_FLASH);
		flash_erase_sector(&flash, FAST_RECONNECT_DATA);
		device_mutex_unlock(RT_DEV_LOCK_FLASH);
	}
#endif
	return 0;
}

/*
* Usage:
*       config fast connect related function.
*       input param: enable, set to 1 means enable fast connect, set to 0 means disable fast connect
*/
void wifi_fast_connect_enable(u8 enable)
{
	if (enable == 0) {
		p_wifi_do_fast_connect = NULL;
		p_store_fast_connect_info = NULL;
	} else {
#if ATCMD_VER == ATVER_2
		struct wlan_fast_reconnect read_data = {0};
		memset(&read_data, 0xff, sizeof(struct wlan_fast_reconnect));
		sys_read_wlan_data_from_flash((uint8_t *)&read_data, sizeof(struct wlan_fast_reconnect));

		if (read_data.enable == 1)
#endif
		{
			// this function will be called after wlan init done to read info from flash and do fast connect
			p_wifi_do_fast_connect = wifi_do_fast_connect;

			// this function will be called after wifi_connection success to store connect info to flash
			p_store_fast_connect_info = write_fast_connect_data_to_flash;
		}
	}
}

/*
* Usage:
*       This function load previous saved ip in flash for fast dhcp.
*
*/
void wifi_fast_connect_load_fast_dhcp(void)
{
#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
	struct wlan_fast_reconnect *data;
	data = (struct wlan_fast_reconnect *)malloc(sizeof(struct wlan_fast_reconnect));
	if (data) {
		memset(data, 0xff, sizeof(struct wlan_fast_reconnect));
		sys_read_wlan_data_from_flash((uint8_t *)data, sizeof(struct wlan_fast_reconnect));

		/* Check whether stored flash profile is empty */
		struct wlan_fast_reconnect *empty_data;
		empty_data = (struct wlan_fast_reconnect *)malloc(sizeof(struct wlan_fast_reconnect));
		if (empty_data) {
			memset(empty_data, 0xff, sizeof(struct wlan_fast_reconnect));
			if (memcmp(empty_data, data, sizeof(struct wlan_fast_reconnect)) == 0) {
				free(data);
				free(empty_data);
				return;
			}
			free(empty_data);
		}

		offer_ip = data->offer_ip;
		server_ip = data->server_ip;
		free(data);
	}
#endif
}

int wifi_check_fast_connect_data(struct wlan_fast_reconnect *data)
{
	//Check SSID
	int ret = 0;
	if ((strlen((const char *)data->psk_essid) > 32) || (strlen((const char *)data->psk_essid) == 0)) {
		ret = -1;
		return ret;
	}

	//Check Password
	if (strlen((const char *)data->psk_passphrase) > 64) {
		ret = -1;
		return ret;
	}

	return ret;
}
