#ifndef __WIFI_FAST_CONNECT_H__
#define __WIFI_FAST_CONNECT_H__


/******************************************************************************
 *
 * Copyright(c) 2007 - 2015 Realtek Corporation. All rights reserved.
 *
 *
 ******************************************************************************/
#include "wifi_structures.h"
#include "main.h"

#ifdef CONFIG_WLAN
#define IW_PASSPHRASE_MAX_SIZE 64
//#define FAST_RECONNECT_DATA (0x80000 - 0x1000)
#define NDIS_802_11_LENGTH_SSID         32
#define A_SHA_DIGEST_LEN		20


struct wlan_fast_reconnect {
	unsigned char version;
	unsigned char psk_essid[NDIS_802_11_LENGTH_SSID + 4];
	unsigned char psk_passphrase[IW_PASSPHRASE_MAX_SIZE + 1];
	unsigned char wpa_global_PSK[A_SHA_DIGEST_LEN * 2];
	unsigned int  channel;
	unsigned int    security_type;
#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
	unsigned int offer_ip;
	unsigned int server_ip;
#endif
#if ATCMD_VER == ATVER_2
	unsigned int    enable;
#endif
};

//Function
extern wifi_do_fast_connect_ptr p_wifi_do_fast_connect;
extern write_fast_connect_info_ptr p_store_fast_connect_info;

int check_is_the_same_ap(void);
void wifi_fast_connect_enable(unsigned char enable);
void wifi_fast_connect_load_fast_dhcp(void);
int wifi_check_fast_connect_data(struct wlan_fast_reconnect *data);
#endif
#endif //#ifndef __WIFI_FAST_CONNECT__
