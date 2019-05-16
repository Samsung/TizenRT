/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include "rtk_wifi_utils.h"
#include "wifi_common.h"
#include "rtk_lwip_netconf.h"
#include "ip_addr.h"
#include "ip4_addr.h"

//RTK
#define CONFIG_INIT_NET                 1
/* WLAN CONFIG ---------------------------------------------------------------*/
#define CONFIG_WLAN	1
#define RTK_OK          0		/*!< RTK_err_t value indicating success (no error) */
#define RTK_FAIL        -1		/*!< Generic RTK_err_t code indicating failure */

#define RTK_ERR_NO_MEM          0x101	/*!< Out of memory */
#define RTK_ERR_INVALID_ARG     0x102	/*!< Invalid argument */
#define RTK_ERR_INVALID_STATE   0x103	/*!< Invalid state */
#define RTK_ERR_INVALID_SIZE    0x104	/*!< Invalid size */
#define RTK_ERR_NOT_FOUND       0x105	/*!< Requested resource not found */
#define RTK_ERR_NOT_SUPPORTED   0x106	/*!< Operation or feature not supported */
#define RTK_ERR_TIMEOUT         0x107	/*!< Operation timed out */
#define RTK_ERR_INVALID_RRTKONSE    0x108	/*!< Received rRTKonse was invalid */
#define RTK_ERR_INVALID_CRC     0x109	/*!< CRC or checksum was invalid */
#define RTK_ERR_INVALID_VERSION     0x10A	/*!< Version was invalid */
#define RTK_ERR_INVALID_MAC     0x10B	/*!< MAC address was invalid */

#define RTK_ERR_WIFI_BASE       0x3000	/*!< Starting number of WiFi error codes */
#define RTK_ERR_MESH_BASE       0x4000	/*!< Starting number of MESH error codes */

#define RTW_LOGE  printf
#define vTaskDelay(t)   usleep(t)

/*Static IP ADDRESS*/
#ifndef IP_ADDR0
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   1
#define IP_ADDR3   80
#endif

/*NETMASK*/
#ifndef NETMASK_ADDR0
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0
#endif

/*Gateway Address*/
#ifndef GW_ADDR0
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   1
#define GW_ADDR3   1
#endif

/*Static IP ADDRESS*/
#ifndef AP_IP_ADDR0
#define AP_IP_ADDR0   192
#define AP_IP_ADDR1   168
#define AP_IP_ADDR2   43
#define AP_IP_ADDR3   1
#endif
   
/*NETMASK*/
#ifndef AP_NETMASK_ADDR0
#define AP_NETMASK_ADDR0   255
#define AP_NETMASK_ADDR1   255
#define AP_NETMASK_ADDR2   255
#define AP_NETMASK_ADDR3   0
#endif

/*Gateway Address*/
#ifndef AP_GW_ADDR0
#define AP_GW_ADDR0   192
#define AP_GW_ADDR1   168
#define AP_GW_ADDR2   43
#define AP_GW_ADDR3   1  
#endif

/*Static IP ADDRESS FOR ETHERNET*/
#ifndef ETH_IP_ADDR0
#define ETH_IP_ADDR0 192
#define ETH_IP_ADDR1 168
#define ETH_IP_ADDR2 0
#define ETH_IP_ADDR3 80
#endif

/*NETMASK FOR ETHERNET*/
#ifndef ETH_NETMASK_ADDR0
#define ETH_NETMASK_ADDR0 255
#define ETH_NETMASK_ADDR1 255
#define ETH_NETMASK_ADDR2 255
#define ETH_NETMASK_ADDR3 0
#endif

/*Gateway address for ethernet*/
#ifndef ETH_GW_ADDR0
#define ETH_GW_ADDR0 192
#define ETH_GW_ADDR1 168
#define ETH_GW_ADDR2 0
#define ETH_GW_ADDR3 1
#endif

/* Private define ------------------------------------------------------------*/
#define MAX_DHCP_TRIES 5

#ifndef FALSE		
    #define FALSE   0
#endif

#ifndef false
	#define false 0
#endif

#ifndef TRUE
    #define TRUE    (!FALSE)
#endif

#ifndef true
	#define true (!false)
#endif

extern struct netif xnetif[NET_IF_NUM]; 

wifi_utils_cb_s g_cbk = { NULL, NULL, NULL, NULL, NULL };

typedef enum WiFi_InterFace_ID {
	WIFI_NONE,					// default
	WIFI_STATION_IF,			// Station mode (turns on wpa_supplicant)
	WIFI_SOFT_AP_IF,			// Soft AP mode (turns on hostapd)
	WIFI_P2P_IF					// P2P mode (turns on wpa_supplicant)
} WiFi_InterFace_ID_t;

//typedef unsigned char    bool;


static WiFi_InterFace_ID_t g_mode = WIFI_STATION_IF;
static volatile int isStaConnected = 0;
static bool wifi_init = false;

wifi_utils_result_e wifi_utils_init(void)
{
	int ret;
    
#if RTW_LWIP_LAYER
	/* Initilaize the LwIP stack */
	LwIP_Init();
#endif
	/* Kill init thread after all init tasks done */
	ret = wifi_on(RTW_MODE_STA);
    if (ret != RTK_OK) {
		RTW_LOGE("\n\rrtk_wifi_start failed\n");
		return WIFI_UTILS_FAIL;
	}
	printf("\r\n===============>>wifi_on success!!\r\n");
    wifi_init = true;
#if RTW_AUTO_RECONNECT
		//setup reconnection flag
	wifi_set_autoreconnect(1);
#endif

	{
		rtw_wifi_setting_t setting;

#if RTW_LWIP_LAYER 	
		uint8_t *mac = LwIP_GetMAC(&xnetif[0]);
		uint8_t *ip = LwIP_GetIP(&xnetif[0]);
#endif

		wifi_get_setting(WLAN0_NAME,&setting);
		wifi_show_setting(WLAN0_NAME,&setting);

#if RTW_LWIP_LAYER 
		printf("\n\r  MAC => %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]) ;
		printf("\n\r  IP  => %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
#endif
    }

	g_mode = WIFI_STATION_IF;
	isStaConnected = 0;

	printf("\r\n===============>>Finish wifi_utils_init!!\r\n");

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_deinit(void)
{
	int ret;
    printf("\n\rwifi_utils_deinit\n");
    
	ret = wifi_off();
	if (ret != RTK_OK) {
		RTW_LOGE("\n\rrtk_wifi_deinit failed\n");
		return WIFI_UTILS_FAIL;
	}
	wifi_init = false;

	g_mode = WIFI_NONE;
	isStaConnected = 0;
    
	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_scan_ap(void *arg)
{
	if(wifi_scan_networks(app_scan_result_handler, NULL ) != RTW_SUCCESS){
		printf("\n\rERROR: wifi scan failed");
		return WIFI_UTILS_FAIL;
	}
    return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_register_callback(wifi_utils_cb_s *cbk)
{
	if (!cbk) {
		return WIFI_UTILS_INVALID_ARGS;
	}
	g_cbk = *cbk;
	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_connect_ap(wifi_utils_ap_config_s *ap_connect_config, void *arg)
{
	int ret;

    char 				*ssid;
	rtw_security_t	security_type;
	char 				*password;
	int 				ssid_len;
	int 				password_len;
	int 				key_id;
	void				*semaphore;
    
	wifi_utils_ap_auth_type_e auth = ap_connect_config->ap_auth_type;
	wifi_utils_ap_crypto_type_e crypto = ap_connect_config->ap_crypto_type;
	/* WIFI_MANAGER_CRYPTO_AES: 3 */
	if (crypto == 3 && auth != WIFI_UTILS_AUTH_WPA2_PSK) {
		RTW_LOGE("\r\nInvalid crypto/auth match\n");
		return WIFI_UTILS_FAIL;
	}
	ssid = ap_connect_config->ssid;	

    switch(auth){
        case WIFI_UTILS_AUTH_OPEN:
			security_type = RTW_SECURITY_OPEN;
            password = NULL;
            ssid_len = strlen((const char *)ssid);
            password_len = 0;
		    key_id = 0;
		    semaphore = NULL;
			break;
		case WIFI_UTILS_AUTH_WEP_SHARED:
			security_type = RTW_SECURITY_WEP_PSK;
    		password = ap_connect_config->passphrase;
    		ssid_len = strlen((const char *)ssid);
    		password_len = ap_connect_config->passphrase_length;
    		key_id = 1;//Foucus
    		if(( password_len != 5) && (password_len != 13)) {
    			printf("\n\rWrong WEP key length. Must be 5 or 13 ASCII characters.");
    			return;
    		}
    		if((key_id < 0) || (key_id > 3)) {
    			printf("\n\rWrong WEP key id. Must be one of 0,1,2, or 3.");
    			return;
    		}
    		semaphore = NULL;
			break;
		case WIFI_UTILS_AUTH_WPA2_PSK:
			security_type = RTW_SECURITY_WPA2_AES_PSK;
    		password =ap_connect_config->passphrase;
    		ssid_len = strlen((const char *)ssid);
    		password_len = ap_connect_config->passphrase_length;
    		key_id = 0;
    		semaphore = NULL;
			break;
		default:
			printf("\n\rUsage: wifi_connect SSID [WPA PASSWORD / (5 or 13) ASCII WEP KEY] [WEP KEY ID 0/1/2/3]");
            return WIFI_UTILS_FAIL;
			break;
    }

	if (g_mode == WIFI_SOFT_AP_IF) {
		wifi_off();
		vTaskDelay(20);
	}

    if (wifi_on(RTW_MODE_STA) < 0){
		printf("\n\rERROR: Wifi on failed!");
		return;
	}

	ret = wifi_connect(ssid, 
					security_type, 
					password, 
					ssid_len, 
					password_len, 
					key_id,
					semaphore);

    if (ret != RTK_OK) {
		RTW_LOGE("\n\rrtk_wifi_connect failed\n");
		return WIFI_UTILS_FAIL;
	}

#if RTW_LWIP_LAYER
	LwIP_DHCP(0, DHCP_START);
#endif

	isStaConnected = 1;
    
	g_mode = WIFI_STATION_IF;

    //g_cbk.sta_connected(WIFI_UTILS_SUCCESS, NULL);
    
	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_disconnect_ap(void *arg)
{
	int ret = WIFI_UTILS_SUCCESS;

	if (g_mode != WIFI_STATION_IF) {
		RTW_LOGE("\r\n[%s] current mode is not station!\n", __func__);
		return WIFI_UTILS_FAIL;
	}

	if (isStaConnected > 0) {
        int timeout = 20;
	    char essid[33];

	    printf("\n\rDeassociating AP ...");

    	if(wext_get_ssid(WLAN0_NAME, (unsigned char *) essid) < 0) {
    		printf("\n\rWIFI disconnected");
    		return ret;
    	}

        if(wifi_disconnect() < 0) {
    		printf("\n\rERROR: Operation failed!");
    		return WIFI_UTILS_FAIL;
    	}
        
		while(1) {
    		if(wext_get_ssid(WLAN0_NAME, (unsigned char *) essid) < 0) {
    			printf("\n\rWIFI disconnected");
    			break;
    		}

    		if(timeout == 0) {
    			printf("\n\rERROR: Deassoc timeout!");
    			return WIFI_UTILS_FAIL;
    		}

            vTaskDelay(1);

    		timeout --;
	    }
	}
	/* Kill WLAN Network */
	isStaConnected = 0;
    g_cbk.sta_disconnected(NULL);
	return ret;
}

wifi_utils_result_e wifi_utils_get_info(wifi_utils_info_s *wifi_info)
{
	int ret;
	int32_t rssi = 0;

	if (!wifi_info) {
		RTW_LOGE("\r\nInvalid Arguments!\n");
		return WIFI_UTILS_INVALID_ARGS;
	}

	/* Get MAC address */
	if (g_mode == WIFI_SOFT_AP_IF) {
		wifi_info->wifi_status = WIFI_UTILS_SOFTAP_MODE;
		if(wifi_get_mac_address(wifi_info->mac_address)){
            RTW_LOGE("\r\nrtk get AP mac addr failed!\n");
            return WIFI_UTILS_FAIL;
        }     
	} else if (g_mode == WIFI_STATION_IF) {
		if(wifi_get_mac_address(wifi_info->mac_address)){
            RTW_LOGE("\r\nrtk get STA mac addr failed!\n");
            return WIFI_UTILS_FAIL;
        }
		/*rtw_bss_info_t ap_info;
		rtw_security_t sec;
		ret = wifi_get_ap_info(&ap_info, &sec);
		if (ret == RTW_SUCCESS) {
			wifi_info->wifi_status = WIFI_UTILS_CONNECTED;
			wifi_info->rssi = ap_info.RSSI;
		} else {
			RTW_LOGE("\r\nrtk get AP info failed!\n");
            return WIFI_UTILS_FAIL;
		}*/
#if RTW_LWIP_LAYER 
        wifi_info->ip4_address = xnetif[0].ip_addr.addr;
#endif
	}

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_start_softap(wifi_utils_softap_config_s *softap_config)
{
	int ret = WIFI_UTILS_SUCCESS;
    int timeout = 20;
    rtw_security_t		security_type;
    char *password;
    int ssid_len;
	int password_len;

#if RTW_LWIP_LAYER 
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;
	struct netif * pnetif = &xnetif[0];
    //dhcps_deinit();
#if LWIP_VERSION_MAJOR >= 2
        IP4_ADDR(ip_2_ip4(&ipaddr), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
        IP4_ADDR(ip_2_ip4(&netmask), NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
        IP4_ADDR(ip_2_ip4(&gw), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
        netif_set_addr(pnetif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask),ip_2_ip4(&gw));
#else
        IP4_ADDR(&ipaddr, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
        IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
        IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
        netif_set_addr(pnetif, &ipaddr, &netmask,&gw);
#endif
#ifdef CONFIG_DONT_CARE_TP
	pnetif->flags |= NETIF_FLAG_IPSWITCH;
#endif
#endif

	wifi_off();
    vTaskDelay(20);

    if (wifi_on(RTW_MODE_AP) < 0){
		printf("\n\rERROR: Wifi on failed!");
		return;
	}

    wifi_init = true;

    printf("\n\rStarting AP ...");

	switch (softap_config->ap_auth_type) {
	case WIFI_UTILS_AUTH_OPEN:
		security_type = RTW_SECURITY_OPEN;
        password = NULL;
		break;
	case WIFI_UTILS_AUTH_WPA2_PSK:
		security_type = RTW_SECURITY_WPA2_AES_PSK;
        password = softap_config->passphrase;
		break;
	case WIFI_UTILS_AUTH_UNKNOWN:
	default:
		RTW_LOGE("\r\nAP AUTH type is unknown %d;\n", softap_config->ap_auth_type);
		return WIFI_UTILS_INVALID_ARGS;
	}

	if (softap_config->channel > 14 || softap_config->channel < 1) {
		RTW_LOGE("\r\nAP channel is wrong: %d;\n", softap_config->channel);
		return WIFI_UTILS_INVALID_ARGS;
	}

	if (softap_config->ssid_length < 1) {
		RTW_LOGE("\r\nAP ssid_length is wrong: %s,  %d;\n", softap_config->ssid, softap_config->ssid_length);
		return WIFI_UTILS_INVALID_ARGS;
	}

	if (softap_config->ap_auth_type != WIFI_UTILS_AUTH_OPEN && softap_config->passphrase_length < 1) {
		RTW_LOGE("\r\nAP passphrase_length is wrong: %s,  %d;\n", softap_config->passphrase, softap_config->passphrase_length);
		return WIFI_UTILS_INVALID_ARGS;
	}

	if(wifi_start_ap(softap_config->ssid,
							 security_type,
							 password,
							 softap_config->ssid_length,
							 softap_config->passphrase_length,
							 softap_config->channel
							 ) != RTW_SUCCESS) {
			printf("\n\rERROR: Operation failed!");
			return WIFI_UTILS_FAIL;
		}

	g_mode = WIFI_SOFT_AP_IF;

    isStaConnected = 0;
    RTW_LOGE("\r\nap start");

	return ret;
}

wifi_utils_result_e wifi_utils_start_sta(void)
{
	int ret;

	if (g_mode == WIFI_STATION_IF) {
		RTW_LOGE("\r\nSTA mode is running\n");
		return WIFI_UTILS_SUCCESS;
	}

    wifi_off();
    vTaskDelay(20);

    ret = wifi_on(RTW_MODE_STA);
	
	if (ret != RTK_OK) {
		RTW_LOGE("\r\nrtk_wifi_start failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}
	g_mode = WIFI_STATION_IF;
    isStaConnected = 0;
	RTW_LOGE("\r\nsta start\n");
	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_stop_sta(void)
{
	int ret = WIFI_UTILS_SUCCESS;
    int timeout = 20;
    char essid[33];
    
    if(wifi_disconnect() < 0) {
        printf("\n\rERROR: Operation failed!");
        return WIFI_UTILS_FAIL;
    }
        
	while(1) {
		if(wext_get_ssid(WLAN0_NAME, (unsigned char *) essid) < 0) {
			printf("\n\rWIFI disconnected");
			break;
		}

		if(timeout == 0) {
			printf("\n\rERROR: Deassoc timeout!");
			return WIFI_UTILS_FAIL;
		}

        vTaskDelay(1);

		timeout --;
    }

	g_mode = WIFI_NONE;
    isStaConnected = 0;
	RTW_LOGE("\r\nsta stop\n");
	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_stop_softap(void)
{
	int ret = WIFI_UTILS_SUCCESS;

    ret = wifi_off();
	if (ret != RTK_OK) {
		RTW_LOGE("\n\rrtk_wifi_deinit failed\n");
		return WIFI_UTILS_FAIL;
	}

    wifi_init = false;

	g_mode = WIFI_NONE;
	isStaConnected = 0;
    RTW_LOGE("\n\rrtk_wifi_stop_softAP Success\n");
	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_set_autoconnect(uint8_t check)
{
	int ret = WIFI_UTILS_SUCCESS;
	ret = wifi_set_autoreconnect(check);
	if (ret != RTK_OK) {
		RTW_LOGE("\r\nesp_wifi_set_auto_connect failed, %d\n", ret);
		return WIFI_UTILS_FAIL;
	}else
		RTW_LOGE("\r\nesp_wifi_set_auto_connect Sucess, %d\n", ret);
	return ret;
}

void wifi_utils_ap_sta_assoc_hdl(void){
    RTW_LOGE("\r\nap ip assigned\n");
	g_cbk.softap_sta_joined(NULL);
}

void wifi_utils_ap_sta_disassoc_hdl(void){
    RTW_LOGE("\r\nstation is leave\n");
	g_cbk.softap_sta_left(NULL);
}


