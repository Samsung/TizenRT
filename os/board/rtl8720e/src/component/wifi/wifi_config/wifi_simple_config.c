#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#if CONFIG_LWIP_LAYER
#include "udp.h"
#include <sockets.h>
#include <lwip_netconf.h>
#endif
#include <osdep_service.h>
#include "platform_stdlib.h"
#include "wifi_simple_config_parser.h"
#include "wifi_simple_config.h"
#include "wifi_conf.h"

#if defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_8735B) || defined(CONFIG_PLATFORM_AMEBALITE)
#include "platform_opts_bt.h"
#endif

#if defined(CONFIG_BT_CONFIG) && CONFIG_BT_CONFIG
#include "bt_config_wifi.h"
#include "bt_config_app_main.h"
#endif

#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
#include "atcmd_wifi.h"
#endif

#define WIFI_MESH_EN	0
#define SC_SOFTAP_EN      1 // on/off softAP mode
#define STACKSIZE         512
#define LEAVE_ACK_EARLY   1

#if (CONFIG_LWIP_LAYER == 0)
extern u32 _ntohl(u32 n);
#endif

#if LWIP_VERSION_MAJOR >= 2
#undef lwip_ntohl
#define lwip_ntohl lwip_htonl
#endif

#ifdef CONFIG_WIFI_MESH
#undef SC_SOFTAP_EN
#define SC_SOFTAP_EN	0
#undef WIFI_MESH_EN
#define WIFI_MESH_EN	1
#endif

#if SC_SOFTAP_EN
#define SOFTAP_SSID_LEN		33
#endif

#if CONFIG_WLAN
#if (CONFIG_INCLUDE_SIMPLE_CONFIG)
#include "wifi_conf.h"
int is_promisc_callback_unlock = 0;
int is_fixed_channel;
int fixed_channel_num;
unsigned char g_ssid[33];
int g_ssid_len;

extern unsigned char is_promisc_enabled(void);
extern int promisc_get_fixed_channel(void *, u8 *, int *);
extern struct netif xnetif[NET_IF_NUM];

extern wifi_do_fast_connect_ptr p_wifi_do_fast_connect;
wifi_do_fast_connect_ptr p_wifi_do_fast_connect_backup;

struct rtk_test_sc;
static enum sc_result promisc_mode_ret = SC_SUCCESS;
static int is_need_connect_to_AP = 0;
static u8 mac_addr[6];

extern int get_sc_profile_fmt(void);
extern int get_sc_profile_info(void *fmt_info_t);
extern int get_sc_dsoc_info(void *dsoc_info_t);
extern int rtl_dsoc_parse(u8 *mac_addr, u8 *buf, void *userdata, unsigned int *len);
void filter1_add_enable(void);
extern void remove_filter(void);
void remove1_filter(void);
static void sc_callback_handler(unsigned char *buf, unsigned int len, void *userdata);
int fmt_val = 0;
rtw_security_t security_type = -1;
static _sema sc_dsoc_sema;
struct fmt_info *fmt_info_t;
struct dsoc_info *dsoc_info_t;
int ssid_hidden;

#if SC_SOFTAP_EN
SC_softAP_decode_ctx *softAP_decode_ctx = NULL;
static u8 simple_config_softAP_onAuth = 0;
static u8 simple_config_softAP_channel = 6;
static int softAP_socket = -1;
#ifndef SUPPORT_5G_CHANNEL
static int simple_config_promisc_channel_tbl[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 1};
#else
static int simple_config_promisc_channel_tbl[] = {
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
	36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 149, 153, 157, 161, 165,
	1
};
#endif
static int softAP_decode_success = -1;
static _sema sc_sta_assoc_sema;
static unsigned char softap_prefix[16] = {0};
#else
#ifndef SUPPORT_5G_CHANNEL
static const int simple_config_promisc_channel_tbl[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
#else
static const int simple_config_promisc_channel_tbl[] = {
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
	36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 149, 153, 157, 161, 165
};
#endif
#endif

static _sema simple_config_finish_sema;

#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/bpstruct.h"
#endif

// support scan list function from APP, comment by default
//#define SC_SCAN_SUPPORT

PACK_STRUCT_BEGIN
struct ack_msg {
	PACK_STRUCT_FIELD(u8_t flag);
	PACK_STRUCT_FIELD(u16_t length);
	PACK_STRUCT_FIELD(u8_t smac[6]);
	PACK_STRUCT_FIELD(u8_t status);
	PACK_STRUCT_FIELD(u16_t device_type);
	PACK_STRUCT_FIELD(u32_t device_ip);
	PACK_STRUCT_FIELD(u8_t device_name[64]);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif



#define 	MULCAST_PORT		(8864)
#define 	SCAN_BUFFER_LENGTH	(1024)
#define     SC_SOFTAP_TIMEOUT   (60000)
#define		SSID_SOFTAP_TIMEOUT	(30000)

#ifndef WLAN0_NAME
#define WLAN0_NAME		"wlan0"
#endif
char simple_config_terminate = 0;

int simple_config_result;
static struct ack_msg  *ack_content;
struct rtk_test_sc *backup_sc_ctx;
// listen scan command and ACK
#ifdef SC_SCAN_SUPPORT

static int pin_enable = 0;
static int scan_start = 0;

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#include "pack_begin.h"
#endif
RTW_PACK_STRUCT_BEGIN
struct ack_msg_scan {
	u8_t flag;
	u16_t length;
	u8_t smac[6];
	u8_t status;
	u16_t device_type;
	u32_t device_ip;
	u8_t device_name[64];
	u8_t pin_enabled;
}
RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#include "pack_end.h"
#endif

static void set_device_name(char *device_name, int max_size)
{
	int pos = 0;
	memcpy(device_name, "ameba_", 6);
	for (int i = 0; i < 3; i++) {
		snprintf(device_name + 6 + pos, max_size - 6 - pos, "%02x", LwIP_GetMAC(0) + 3);
		pos += 2;
		if (i != 2) {
			device_name[6 + pos++] = ':';
		}
	}
	return;
}
void SC_scan_thread(void *para)
{
	int sockfd_scan;
	struct sockaddr_in device_addr;
	unsigned char packet[256];
	struct sockaddr from;
	struct sockaddr_in *from_sin = (struct sockaddr_in *) &from;
	socklen_t fromLen = sizeof(from);
	struct ack_msg_scan ack_msg;

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#include "pack_begin.h"
#endif
	RTW_PACK_STRUCT_BEGIN
	struct scan_msg {
		unsigned char 	flag;
		unsigned short 	length;
		unsigned char		sec_level;
		unsigned char 	nonce[64];
		unsigned char 	digest[16];
		unsigned char 	smac[6];
		unsigned short 	device_type;
	};
	RTW_PACK_STRUCT_STRUCT;
	RTW_PACK_STRUCT_END
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#include "pack_end.h"
#endif

	struct scan_msg *pMsg;

	if ((sockfd_scan = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		printf("SC scan socket error\n");
		return;
	}
	memset(&device_addr, 0, sizeof(struct sockaddr_in));
	device_addr.sin_family = AF_INET;
	device_addr.sin_port = htons(18864);
	device_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd_scan, (struct sockaddr *)&device_addr, sizeof(struct sockaddr)) == -1) {
		printf("SC scan bind error\n");
		close(sockfd_scan);
		return;
	}
	memset(packet, 0, sizeof(packet));

	// for now, no checking for the validity of received data, wf, 0225
	while (1) {
		if ((recvfrom(sockfd_scan, &packet, sizeof(packet), MSG_DONTWAIT, &from, &fromLen)) >= 0) {
			uint16_t from_port = ntohs(from_sin->sin_port);
			//printf("SC_scan: recv %d bytes from %d.%d.%d.%d:%d\n",packetLen, ip[0], ip[1], ip[2], ip[3], from_port);

			from_sin->sin_port = htons(8864);
			// send ACK for scan
			pMsg = (struct scan_msg *)packet;
			if (pMsg->flag == 0x00) { // scan flag
				ack_msg.flag = 0x21;
				ack_msg.length = sizeof(struct ack_msg_scan);
				ack_msg.status = 1;
				memcpy(ack_msg.smac, LwIP_GetMAC(0), 6);

				ack_msg.device_type = 0;
				ack_msg.device_ip = LwIP_GetIP(0);
				memset(ack_msg.device_name, 0, 64);
				set_device_name((char *)ack_msg.device_name, sizeof(ack_msg.device_name));
				// set the device_name to: ameba_xxxxxx(last 3 bytes of MAC)
				ack_msg.pin_enabled = pin_enable;
				for (int i = 0; i < 3; i++) {
					int ret = sendto(sockfd_scan, (unsigned char *)&ack_msg, sizeof(struct ack_msg_scan), 0, (struct sockaddr *)&from, fromLen);
					if (ret < 0) {
						printf("send ACK for scan fail\n");
					}
					//else
					//printf("send %d bytes of ACK to scan\n", ret);
				}
			} else {
				continue;
			}
		}
		vTaskDelay(500);
	}
}

void SC_listen_ACK_scan()
{
	if (xTaskCreate(SC_scan_thread, ((const char *)"SC_scan_thread"), 512, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate(SC_scan_thread) failed", __FUNCTION__);
	}
}

#endif

static void sc_sta_asso_cb(char *buf, int buf_len, int flags, void *userdata);

void SC_set_ack_content(void)
{
	memset(ack_content, 0, sizeof(struct ack_msg));
	ack_content->flag = 0x20;
	ack_content->length = htons(sizeof(struct ack_msg) - 3);
	memcpy(ack_content->smac, LwIP_GetMAC(0), 6);
	ack_content->status = 0;
	ack_content->device_type = 0;
#if LWIP_VERSION_MAJOR >= 2
	ack_content->device_ip = ip4_addr_get_u32(ip_2_ip4(netif_ip_addr4(&xnetif[0])));
#else
	ack_content->device_ip = (LwIP_GetIP(0));
#endif
	memset(ack_content->device_name, 0, 64);
}

int SC_send_simple_config_ack(u8 round)
{
#if CONFIG_LWIP_LAYER
	int ack_transmit_round, ack_num_each_sec;
	int ack_socket;
	//int sended_data = 0;
	struct sockaddr_in to_addr;
#if LEAVE_ACK_EARLY
	u8 check_phone_ack = 0;
#endif
	SC_set_ack_content();

	ack_socket = socket(PF_INET, SOCK_DGRAM, IP_PROTO_UDP);
	if (ack_socket == -1) {
		return 	-1;
	}
#if LEAVE_ACK_EARLY
	else {
		struct sockaddr_in bindAddr;
		bindAddr.sin_family = AF_INET;
		bindAddr.sin_port = htons(8864);
		bindAddr.sin_addr.s_addr = INADDR_ANY;
		if (bind(ack_socket, (struct sockaddr *) &bindAddr, sizeof(bindAddr)) == 0) {
			check_phone_ack = 1;
		}
	}
#endif
	printf("\nSending simple config ack\n");
	FD_ZERO(&to_addr);
	to_addr.sin_family = AF_INET;
	to_addr.sin_port = htons(8864);
	to_addr.sin_addr.s_addr = (backup_sc_ctx->ip_addr);
	for (ack_transmit_round = 0; ack_transmit_round < round; ack_transmit_round++) {
		for (ack_num_each_sec = 0; ack_num_each_sec < 20; ack_num_each_sec++) {
			//sended_data =
			sendto(ack_socket, (unsigned char *)ack_content, sizeof(struct ack_msg), 0, (struct sockaddr *) &to_addr, sizeof(struct sockaddr));
			//printf("\r\nAlready send %d bytes data\n", sended_data);
			vTaskDelay(50);	/* delay 50 ms */

#if LEAVE_ACK_EARLY
			if (check_phone_ack) {
				unsigned char packet[100];
				int packetLen;
				struct sockaddr from;
				struct sockaddr_in *from_sin = (struct sockaddr_in *) &from;
				socklen_t fromLen = sizeof(from);

				if ((packetLen = recvfrom(ack_socket, &packet, sizeof(packet), MSG_DONTWAIT, &from, &fromLen)) >= 0) {
					uint8_t *ip = (uint8_t *) &from_sin->sin_addr.s_addr;
					uint16_t from_port = ntohs(from_sin->sin_port);
					printf("recv %d bytes from %d.%d.%d.%d:%d at round=%d, num=%d\n",
						   packetLen, ip[0], ip[1], ip[2], ip[3], from_port,
						   ack_transmit_round, ack_num_each_sec);

#if defined(CONFIG_WIFI_MESH) && CONFIG_WIFI_MESH
					int ret = Parse_Server_info(&packet, packetLen);
					if (ret == 1) {
						printf("get ip and port sucessfully ");
						goto leave_ack;
					}

#else
					goto leave_ack;
#endif

				}
			}
#endif
		}
	}

leave_ack:
	close(ack_socket);
#endif

	return 0;
}


#if defined(CONFIG_WIFI_MESH) && CONFIG_WIFI_MESH
extern void rmesh_set_server_info(u8 *ip, u16 ip_len, u16 port);

int Parse_Server_info(u8 *data, int data_len)
{
	u8 *pbuf;
	u16 msg_len;
	int msg_header = 3;
	int flag_ip = 0;
	int flag_port = 0;
	u8 type;
	u8 *pip;
	u16 ip_len;
	u16 port;

	if (data_len <= 90) {
		return 0;
	}

	if (*data != 0x05) {
		return 0;
	}

	pbuf = data + 4;

	while (1) {
		type = *pbuf;
		pbuf++;

		msg_len = (((*(pbuf) & 0xFF) << 8) | (*(pbuf + 1) & 0xFF));
		pbuf += 2;

		switch (type) {
		case 0x0a:
			flag_ip = 1;
			pip = pbuf;
			ip_len = msg_len;
			break;

		case 0x0b:
			flag_port = 1;
			port = (((*pbuf) << 8) | (*(pbuf + 1)));
			break;
		}

		pbuf += msg_len;

		if (flag_ip == 1 && flag_port == 1) {
			rmesh_set_server_info(pip, ip_len, port);
			return 1;
		}

	}

	return 0;

}
#endif
static void SC_show_wifi_setting(const char *ifname, rtw_wifi_setting_t *pSetting)
{
	printf("\n\r\nWIFI  %s Setting:", ifname);
	printf("\n\r==============================");

	switch (pSetting->mode) {
	case RTW_MODE_AP:
		printf("\n\r		MODE => AP");
		break;
	case RTW_MODE_STA:
		printf("\n\r		MODE => STATION");
		break;
	default:
		printf("\n\r		MODE => UNKNOWN");
	}
	printf("\n\r		SSID => %s", pSetting->ssid);
	printf("\n\r	 CHANNEL => %d", pSetting->channel);

	switch (pSetting->security_type) {
	case RTW_SECURITY_OPEN:
		printf("\n\r	SECURITY => OPEN");
		break;
	case RTW_SECURITY_WEP_PSK:
		printf("\n\r	SECURITY => WEP");
		printf("\n\r KEY INDEX => %d", pSetting->key_idx);
		break;
	case RTW_SECURITY_WPA_TKIP_PSK:
		printf("\n\r	SECURITY => TKIP");
		break;
	case RTW_SECURITY_WPA2_AES_PSK:
		printf("\n\r	SECURITY => AES");
		break;
	default:
		printf("\n\r	SECURITY => UNKNOWN");
	}

	printf("\n\r	PASSWORD => %s", pSetting->password);
	printf("\n\r");
}
static int  SC_check_and_show_connection_info(void)
{
	rtw_wifi_setting_t setting;

#if CONFIG_LWIP_LAYER
	int ret = -1;
	int retry = 0;

	vTaskPrioritySet(NULL, tskIDLE_PRIORITY + 3);
	while (retry < 2) {
		/* If not rise priority, LwIP DHCP may timeout */
		/* Start DHCP Client */
		ret = LwIP_DHCP(0, DHCP_START);
		if (ret == DHCP_ADDRESS_ASSIGNED) {
			break;
		} else {
			retry++;
		}
	}
	vTaskPrioritySet(NULL, tskIDLE_PRIORITY + 1);
#endif

#if !(defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD)  || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
	wifi_get_setting(WLAN0_IDX, &setting);
	SC_show_wifi_setting(WLAN0_NAME, &setting);
#endif

#if CONFIG_LWIP_LAYER
	if (ret != DHCP_ADDRESS_ASSIGNED) {
		return SC_DHCP_FAIL;
	} else
#endif
		return SC_SUCCESS;

}

static void check_and_set_security_in_connection(rtw_security_t security_mode, rtw_network_info_t *wifi)
{

	if (security_mode == RTW_SECURITY_WPA2_AES_PSK) {
		printf("\r\nwifi->security_type = RTW_SECURITY_WPA2_AES_PSK\n");
		wifi->security_type = RTW_SECURITY_WPA2_AES_PSK;
	} else if (security_mode == RTW_SECURITY_WEP_PSK) {
		printf("\r\nwifi->security_type = RTW_SECURITY_WEP_PSK\n");
		wifi->security_type = RTW_SECURITY_WEP_PSK;
		wifi->key_id = 0;
	} else if (security_mode == RTW_SECURITY_WPA_AES_PSK) {
		printf("\r\nwifi->security_type = RTW_SECURITY_WPA_AES_PSK\n");
		wifi->security_type = RTW_SECURITY_WPA_AES_PSK;
	} else {
		printf("\r\nwifi->security_type = RTW_SECURITY_OPEN\n");
		wifi->security_type = RTW_SECURITY_OPEN;
	}
}

#if SC_SOFTAP_EN
static int SC_softAP_find_ap_from_scan_buf(char *target_ssid, void *user_data, int ap_num)
{
	rtw_wifi_setting_t *pwifi = (rtw_wifi_setting_t *)user_data;
	rtw_scan_result_t *scanned_ap_info;
	int i = 0;
	char *scan_buf = NULL;

	scan_buf = (char *)rtw_zmalloc(ap_num * sizeof(rtw_scan_result_t));
	if (scan_buf == NULL) {
		printf("malloc scan buf for simple config fail\n");
		return -1;
	}

	if (wifi_get_scan_records((unsigned int *)(&ap_num), scan_buf) < 0) {
		rtw_mfree((u8 *)scan_buf, 0);
		return -1;
	}

	for (i = 0; i < ap_num; i++) {
		scanned_ap_info = (rtw_scan_result_t *)(scan_buf + i * sizeof(rtw_scan_result_t));
		if ((scanned_ap_info->SSID.len == strlen(target_ssid)) && (!memcmp(scanned_ap_info->SSID.val, target_ssid, scanned_ap_info->SSID.len))) {
			strncpy((char *)pwifi->ssid, target_ssid, sizeof(pwifi->ssid));
			pwifi->channel = scanned_ap_info->channel;
			pwifi->security_type = scanned_ap_info->security;
		}
	}
	rtw_mfree((u8 *)scan_buf, 0);
	return 0;

}

static int SC_softAP_get_ap_security_mode(IN char *ssid, OUT rtw_security_t *security_mode)
{
	rtw_wifi_setting_t wifi;
	rtw_scan_param_t scan_param;
	int scanned_ap_num = 0;
	int ret = 0;

	memset(&wifi, 0, sizeof(wifi));

	rtw_memset(&scan_param, 0, sizeof(rtw_scan_param_t));
	scan_param.ssid = ssid;

	scanned_ap_num = wifi_scan_networks(&scan_param, 1);

	if (scanned_ap_num <= 0) {
		printf("Wifi scan failed!\n");
		ret = 0;
	} else {
		SC_softAP_find_ap_from_scan_buf(ssid, (void *)&wifi, scanned_ap_num);

		if (strcmp((char *)wifi.ssid, ssid) == 0) {
			*security_mode = wifi.security_type;
			ret = 1;
		}
	}

	return ret;
}

#endif // end of SC_SOFTAP_EN

int sc_set_val1(rtw_network_info_t *wifi)
{
	int ret = -1;
	if (fmt_val == 1) {
		fmt_info_t = malloc(sizeof(struct fmt_info));
		memset(fmt_info_t, 0, sizeof(struct fmt_info));
		get_sc_profile_info(fmt_info_t);
		fixed_channel_num = fmt_info_t->fmt_channel[1];
		ssid_hidden = fmt_info_t->fmt_hidden[0];
		//printf("\nhidden: %d\n", ssid_hidden);
		//printf("\nchannel: %d\n",fixed_channel_num);
		//printf(MAC_FMT, MAC_ARG(fmt_info_t->fmt_bssid));
		rtw_memcpy(mac_addr, fmt_info_t->fmt_bssid, 6);
		memset(backup_sc_ctx->ssid, 0, sizeof(backup_sc_ctx->ssid));

#if SC_SOFTAP_EN
		wifi->ssid.len = strlen((const char *)backup_sc_ctx->ssid);
		rtw_memcpy(wifi->ssid.val, backup_sc_ctx->ssid, wifi->ssid.len);
		wifi->ssid.val[wifi->ssid.len] = 0;
#else
		if (memcmp(mac_addr, g_bssid, 6) == 0) {
			rtw_memcpy(backup_sc_ctx->ssid, g_ssid, g_ssid_len);
			wifi->ssid.len = strlen(backup_sc_ctx->ssid);
			rtw_memcpy(wifi->ssid.val, backup_sc_ctx->ssid, wifi->ssid.len);
			wifi->ssid.val[wifi->ssid.len] = 0;
			printf("using ssid from profile and scan result\n");
		} else {
			rtw_memcpy(g_bssid, mac_addr, 6);
		}
#endif
		free(fmt_info_t);
		ret = 0;
	}
	return ret;
}

int get_connection_info_from_profile(rtw_security_t security_mode, rtw_network_info_t *wifi)
{

	printf("\r\n======= Connection Information =======\n");

#if !SC_SOFTAP_EN
	check_and_set_security_in_connection(security_mode, wifi);
#endif

	wifi->password = backup_sc_ctx->password;
	wifi->password_len = (int)strlen((char const *)backup_sc_ctx->password);

#if SC_SOFTAP_EN
	// reconfigure the security mode, when under softAP mode; do not support WEP now
	if (softAP_decode_success == 0) {

		if (sc_set_val1(wifi) == -1) {
			wifi->ssid.len = strlen((char const *)backup_sc_ctx->ssid);
			rtw_memcpy(wifi->ssid.val, backup_sc_ctx->ssid, wifi->ssid.len);
			wifi->ssid.val[wifi->ssid.len] = 0;
			if (0 == SC_softAP_get_ap_security_mode((char *)wifi->ssid.val, &wifi->security_type)) {
				if (wifi->password_len) {
					wifi->security_type = RTW_SECURITY_WPA2_AES_PSK;
				} else {
					wifi->security_type = RTW_SECURITY_OPEN;
				}
			}
		}

		if (wifi->security_type == RTW_SECURITY_WPA2_AES_PSK) {
			printf("\r\nwifi->security_type = RTW_SECURITY_WPA2_AES_PSK\n");
		} else if (wifi->security_type == RTW_SECURITY_WEP_PSK) {
			printf("\r\nwifi->security_type = RTW_SECURITY_WEP_PSK\n");
			wifi->key_id = 0;
		} else if (wifi->security_type == RTW_SECURITY_WPA_AES_PSK) {
			printf("\r\nwifi->security_type = RTW_SECURITY_WPA_AES_PSK\n");
		} else {
			printf("\r\nwifi->security_type = RTW_SECURITY_OPEN\n");
		}

		goto ssid_set_done;

	} else {
		check_and_set_security_in_connection(security_mode, wifi);
	}
#endif

	/* 1.both scanned g_ssid and ssid from profile are null, return fail */
	if ((0 == g_ssid_len) && (0 == strlen((char const *)backup_sc_ctx->ssid))) {
		printf("no ssid info found, connect will fail\n");
		return -1;
	}

	if (sc_set_val1(wifi) == 0) {
		goto ssid_set_done;
	}

	/* g_ssid and ssid from profile are same, enter connect and retry */
	if (0 == strcmp((char const *)backup_sc_ctx->ssid, (char const *)g_ssid)) {
		wifi->ssid.len = strlen((char const *)backup_sc_ctx->ssid);
		rtw_memcpy(wifi->ssid.val, backup_sc_ctx->ssid, wifi->ssid.len);
		wifi->ssid.val[wifi->ssid.len] = 0;
		printf("using ssid from profile and scan result\n");
		goto ssid_set_done;
	}

	/* if there is profile, but g_ssid and profile are different, using profile to connect and retry */
	if (strlen((char const *)backup_sc_ctx->ssid) > 0) {
		wifi->ssid.len = strlen((char const *)backup_sc_ctx->ssid);
		rtw_memcpy(wifi->ssid.val, backup_sc_ctx->ssid, wifi->ssid.len);
		wifi->ssid.val[wifi->ssid.len] = 0;
		printf("using ssid only from profile\n");
		goto ssid_set_done;

	}

	/* if there is no profile but have scanned ssid, using g_ssid to connect and retry
		(maybe ssid is right and password is wrong) */
	if (g_ssid_len > 0) {
		wifi->ssid.len = g_ssid_len;
		rtw_memcpy(wifi->ssid.val, g_ssid, wifi->ssid.len);
		wifi->ssid.val[wifi->ssid.len] = 0;
		printf("using ssid only from scan result\n");
		goto ssid_set_done;
	}


ssid_set_done:


	if (wifi->security_type == RTW_SECURITY_WEP_PSK) {
		if (wifi->password_len == 10) {
			unsigned int p[5] = {0};
			u8 pwd[6], i = 0;
			sscanf((const char *)backup_sc_ctx->password, "%02x%02x%02x%02x%02x", &p[0], &p[1], &p[2], &p[3], &p[4]);
			for (i = 0; i < 5; i++) {
				pwd[i] = (u8)p[i];
			}
			pwd[5] = '\0';
			memset(backup_sc_ctx->password, 0, 65);
			strncpy((char *)backup_sc_ctx->password, (char *)pwd, sizeof(backup_sc_ctx->password));
			wifi->password_len = 5;
		} else if (wifi->password_len == 26) {
			unsigned int p[13] = {0};
			u8 pwd[14], i = 0;
			sscanf((const char *)backup_sc_ctx->password, "%02x%02x%02x%02x%02x%02x%02x"\
				   "%02x%02x%02x%02x%02x%02x", &p[0], &p[1], &p[2], &p[3], &p[4], \
				   &p[5], &p[6], &p[7], &p[8], &p[9], &p[10], &p[11], &p[12]);
			for (i = 0; i < 13; i++) {
				pwd[i] = (u8)p[i];
			}
			pwd[13] = '\0';
			memset(backup_sc_ctx->password, 0, 64);
			strncpy((char *)backup_sc_ctx->password, (char *)pwd, sizeof(backup_sc_ctx->password));
			wifi->password_len = 13;
		}
	}
	printf("\r\nwifi.password = %s\n", wifi->password);
	printf("\r\nwifi.password_len = %d\n", wifi->password_len);
	printf("\r\nwifi.ssid = %s\n", wifi->ssid.val);
	printf("\r\nwifi.ssid_len = %d\n", wifi->ssid.len);
	printf("\r\nwifi.channel = %d\n", fixed_channel_num);
	printf("\r\n===== start to connect target AP =====\n");
	return 0;
}




#pragma pack(1)
struct scan_with_ssid_result {
	u8 len; /* len of a memory area store ap info */
	u8 mac[ETH_ALEN];
	int rssi;
	u8 sec_mode;
	u8 password_id;
	u8 channel;
	//char ssid[65];
};

struct sc_ap_info {

	char *ssid;
	int ssid_len;

};

rtw_security_t	SC_translate_iw_security_mode(u8 security_type)
{

	rtw_security_t security_mode = RTW_SECURITY_UNKNOWN;


	switch (security_type) {
	case RTW_ENCODE_ALG_NONE:
		security_mode = RTW_SECURITY_OPEN;
		break;
	case RTW_ENCODE_ALG_WEP:
		security_mode = RTW_SECURITY_WEP_PSK;
		break;
	case RTW_ENCODE_ALG_CCMP:
		security_mode = RTW_SECURITY_WPA2_AES_PSK;
		break;
	default:
		printf("error: security type not supported\n");
		break;
	};

	return security_mode;
}

/*

	scan buf format:

	len	mac	rssi	sec	wps	channel	ssid
	1B	6B	4B	1B	1B		1B		(len - 14)B

*/
enum sc_result SC_parse_scan_result_and_connect(char *scan_buf, int scanned_ap_num, rtw_network_info_t *wifi)
{
	rtw_scan_result_t *scanned_ap_info;
	rtw_network_info_t connect_param = {0};
	u8 scan_channel = 0;
	int i, j = 0;
	enum sc_result ret = SC_ERROR;

	/* if wifi_is_connected_to_ap and we run here, ther will be hardfault(caused by auto reconnect) */
	printf("Scan result got, start to connect AP with scanned bssid\n");

	for (i = 0; i < scanned_ap_num; i++) {
		scanned_ap_info = (rtw_scan_result_t *)(scan_buf + i * sizeof(rtw_scan_result_t));
		/* run here means there is a match */
		if (scanned_ap_info->SSID.len == wifi->ssid.len) {
			if (memcmp(scanned_ap_info->SSID.val, wifi->ssid.val, scanned_ap_info->SSID.len) == 0) {

				printf("Connecting to  MAC=%02x:%02x:%02x:%02x:%02x:%02x, ssid = %s, SEC=%d\n",
					   scanned_ap_info->BSSID.octet[0], scanned_ap_info->BSSID.octet[1], scanned_ap_info->BSSID.octet[2],
					   scanned_ap_info->BSSID.octet[3], scanned_ap_info->BSSID.octet[4], scanned_ap_info->BSSID.octet[5],
					   scanned_ap_info->SSID.val, (int)scanned_ap_info->security);

				scan_channel = scanned_ap_info->channel;

				// translate wep key if get_connection_info_from_profile() does not do it due to wrong security form locked ssid for dual band router
				if (scanned_ap_info->security == RTW_SECURITY_WEP_PSK) {
					if (wifi->password_len == 10) {
						unsigned int p[5] = {0};
						u8 pwd[6], i = 0;
						sscanf((const char *)backup_sc_ctx->password, "%02x%02x%02x%02x%02x", &p[0], &p[1], &p[2], &p[3], &p[4]);
						for (i = 0; i < 5; i++) {
							pwd[i] = (u8)p[i];
						}
						pwd[5] = '\0';
						memset(backup_sc_ctx->password, 0, 65);
						strncpy((char *)backup_sc_ctx->password, (char *)pwd, sizeof(backup_sc_ctx->password));
						wifi->password_len = 5;
					} else if (wifi->password_len == 26) {
						unsigned int p[13] = {0};
						u8 pwd[14], i = 0;
						sscanf((const char *)backup_sc_ctx->password, "%02x%02x%02x%02x%02x%02x%02x"\
							   "%02x%02x%02x%02x%02x%02x", &p[0], &p[1], &p[2], &p[3], &p[4], \
							   &p[5], &p[6], &p[7], &p[8], &p[9], &p[10], &p[11], &p[12]);
						for (i = 0; i < 13; i++) {
							pwd[i] = (u8)p[i];
						}
						pwd[13] = '\0';
						memset(backup_sc_ctx->password, 0, 64);
						strncpy((char *)backup_sc_ctx->password, (char *)pwd, sizeof(backup_sc_ctx->password));
						wifi->password_len = 13;
					}
				}

				/* try 3 times to connect */
				for (j = 0; j < 3; j++) {
					connect_param.channel = scan_channel;
					connect_param.pscan_option = PSCAN_SIMPLE_CONFIG;
					rtw_memcpy(connect_param.ssid.val, wifi->ssid.val, wifi->ssid.len);
					connect_param.ssid.len = wifi->ssid.len;
					rtw_memcpy(connect_param.bssid.octet, scanned_ap_info->BSSID.octet, ETH_ALEN);
					connect_param.security_type = scanned_ap_info->security;
					connect_param.password = wifi->password;
					connect_param.password_len =  wifi->password_len;
					ret = (enum sc_result) wifi_connect(&connect_param, 1);
					if (ret == (enum sc_result)RTW_SUCCESS) {
						goto sc_connect_wifi_success;
					}
				}

			}
		}
	}


sc_connect_wifi_success:
	printf("%s success\n", __FUNCTION__);
	return ret;


}


/*

	When BSSID_CHECK_SUPPORT is not set, there will be problems:

	1.AP1 and AP2 (different SSID) both forward simple config packets,
		profile is from AP2, but Ameba connect with AP1
	2.AP1 and AP2 (same SSID, but different crypto or password), both forward simple config packets,
		profile is from AP2, but Ameba connect with AP1
	3. ...

	fix: using SSID to query matched BSSID(s) in scan result, traverse and connect.


	Consideration:
	1.Only take ssid and password
	2.Assume they have different channel.
	3.Assume they have different encrypt methods

*/
int  SC_connect_to_candidate_AP(rtw_network_info_t *wifi)
{
	int ret;
	rtw_scan_param_t scan_param;
	char *scan_buf = NULL;
	unsigned int scan_cnt = 0;
	char *ssid = (char *)wifi->ssid.val;


	printf("\nConnect with SSID=%s  password=%s\n", wifi->ssid.val, wifi->password);

	rtw_memset(&scan_param, 0, sizeof(rtw_scan_param_t));
	scan_param.ssid = ssid;

	/* call wifi scan to scan */
	if ((scan_cnt = wifi_scan_networks(&scan_param, 1)) <= 0) {
		printf("\n\rERROR: wifi scan failed");
		ret = RTW_ERROR;
	} else {
		scan_buf = (char *)rtw_zmalloc(scan_cnt * sizeof(rtw_scan_result_t));
		if (scan_buf == NULL) {
			printf("malloc scan buf fail for simlple config\n");
			ret = RTW_ERROR;
			goto EXIT;
		}
		if (wifi_get_scan_records(&scan_cnt, scan_buf) < 0) {
			ret = RTW_ERROR;
			goto EXIT;
		} else {
			ret  = SC_parse_scan_result_and_connect(scan_buf, scan_cnt, wifi);
		}
	}
EXIT:
	if (scan_buf) {
		rtw_mfree((u8 *)scan_buf, 0);
	}
	return ret;
}

rtw_security_t SC_translate_security(u8 security_type)
{

	rtw_security_t security_mode = RTW_SECURITY_UNKNOWN;

	switch (security_type) {
	case RTW_ENCRYPTION_OPEN:
		security_mode = RTW_SECURITY_OPEN;
		break;
	case RTW_ENCRYPTION_WEP40:
	case RTW_ENCRYPTION_WEP104:
		security_mode = RTW_SECURITY_WEP_PSK;
		break;
	case RTW_ENCRYPTION_WPA_TKIP:
	case RTW_ENCRYPTION_WPA_AES:
	case RTW_ENCRYPTION_WPA2_TKIP:
	case RTW_ENCRYPTION_WPA2_AES:
	case RTW_ENCRYPTION_WPA2_MIXED:
		security_mode = RTW_SECURITY_WPA2_AES_PSK;
		break;
	case RTW_ENCRYPTION_UNKNOWN:
	case RTW_ENCRYPTION_UNDEF:
	default:
		printf("unknow security mode,connect fail\n");
	}

	return security_mode;

}

static void sc_callback_handler(unsigned char *buf, unsigned int len, void *userdata)
{
	int ret = -1;
	taskENTER_CRITICAL();
	ret = rtl_dsoc_parse(mac_addr, buf, userdata, &len);
	taskEXIT_CRITICAL();
	if (ret == 0) {
		//printf("\nhandler part\n");
		rtw_up_sema(&sc_dsoc_sema);
		return;
	}
}

int sc_set_val2(rtw_network_info_t *wifi)
{
	int ret = 1;
	if (fmt_val == 1) {
#if !SC_SOFTAP_EN
		remove_filter();
		filter1_add_enable();
#endif
		wifi_set_channel(fixed_channel_num);
		rtw_init_sema(&sc_dsoc_sema, 0);
		if (wifi_set_promisc(RTW_PROMISC_ENABLE_2, sc_callback_handler, 1) != 0) {
			printf("\nset promisc failed\n");
			rtw_free_sema(&sc_dsoc_sema);
			ret = -1;
		}
		if (rtw_down_timeout_sema(&sc_dsoc_sema, SSID_SOFTAP_TIMEOUT) == RTW_FALSE) {
			printf("\nsc callback failed\n");
			wifi_set_promisc(RTW_PROMISC_DISABLE, NULL, 0);
			rtw_free_sema(&sc_dsoc_sema);
			ret = -1;
		} else {
			wifi_set_promisc(RTW_PROMISC_DISABLE, NULL, 0);
			dsoc_info_t = malloc(sizeof(struct dsoc_info));
			memset(dsoc_info_t, 0, sizeof(struct dsoc_info));
			get_sc_dsoc_info(dsoc_info_t);
			wifi->ssid.len = dsoc_info_t->dsoc_length;
			rtw_memcpy(wifi->ssid.val, dsoc_info_t->dsoc_ssid, wifi->ssid.len);
			wifi->ssid.val[wifi->ssid.len] = 0;
#if SC_SOFTAP_EN
			if (0 == SC_softAP_get_ap_security_mode((char *)wifi->ssid.val, &wifi->security_type)) {
				if (wifi->password_len) {
					wifi->security_type = RTW_SECURITY_WPA2_AES_PSK;
				} else {
					wifi->security_type = RTW_SECURITY_OPEN;
				}
			}
#endif
			rtw_free_sema(&sc_dsoc_sema);
			free(dsoc_info_t);
			ret = 1;
		}
#if !SC_SOFTAP_EN
		remove1_filter();
#endif
	}
	return ret;
}

enum sc_result SC_connect_to_AP(void)
{
	enum sc_result ret = SC_ERROR;
	int max_retry = 5, retry = 0;
	rtw_security_t security_mode;
	rtw_network_info_t wifi = {0};
#ifndef SUPPORT_5G_CHANNEL
	u8 scan_channel;
	if (!(fixed_channel_num == 0)) {
		scan_channel = fixed_channel_num;
	}
#endif

#if SC_SOFTAP_EN
	if (softAP_decode_success != 0)
#endif
	{
		security_mode = SC_translate_security(g_security_mode);
		g_security_mode = 0xff;//clear it
	}

	fmt_val = get_sc_profile_fmt();

	if (-1 == get_connection_info_from_profile(security_mode, &wifi)) {
		ret = SC_CONTROLLER_INFO_PARSE_FAIL;
		goto wifi_connect_fail;
	}

#if CONFIG_AUTO_RECONNECT
	/* disable auto reconnect */
	wifi_config_autoreconnect(0, 0, 0);
#endif

#if 1
	/* optimization: get g_bssid to connect with only pscan */
	while (1) {

#if SC_SOFTAP_EN
		if (softAP_decode_success != 0)
#endif
		{
			ret = (enum sc_result)sc_set_val2(&wifi);
			if (ret == 1) {
// for dual band router, locked channel may not be target channel
#ifndef SUPPORT_5G_CHANNEL
				wifi.channel = scan_channel;
				wifi.pscan_option = PSCAN_SIMPLE_CONFIG;
#endif
				rtw_memcpy(wifi.bssid.octet, g_bssid, ETH_ALEN);
				ret = (enum sc_result)wifi_connect(&wifi, 1);
			} else {
				goto wifi_connect_fail;
			}

		}

#if SC_SOFTAP_EN

		// when configured by softAP mode
		else {
			ret = (enum sc_result)sc_set_val2(&wifi);
			if (ret == 1) {
				ret = (enum sc_result)wifi_connect(&wifi, 1);
			} else {
				goto wifi_connect_fail;
			}

		}
#endif
		if (ret == (enum sc_result) RTW_SUCCESS) {
			goto wifi_connect_success;
		}

		if (retry == max_retry) {
			printf("connect fail with bssid, try ssid instead\n");
			break;
		}
		retry ++;
	}
#endif




#if 1
	/* when optimization fail: if connect with bssid fail because of we have connect to the wrong AP */
	ret = (enum sc_result)SC_connect_to_candidate_AP(&wifi);
	if ((enum sc_result)RTW_SUCCESS == ret) {
		goto wifi_connect_success;
	} else {
		ret = SC_JOIN_BSS_FAIL;
		goto wifi_connect_fail;
	}
#endif


wifi_connect_success:
	ret = (enum sc_result)SC_check_and_show_connection_info();
	goto wifi_connect_end;


wifi_connect_fail:
	printf("SC_connect_to_AP failed\n");
	goto wifi_connect_end;

wifi_connect_end:
#if CONFIG_AUTO_RECONNECT
	wifi_config_autoreconnect(1, 10, 5);
#endif
	return ret;


}

void simple_config_callback(unsigned char *buf, unsigned int len, void *userdata)
{
	int ret = 0;

	unsigned char *da = NULL;
	unsigned char *sa = NULL;

#if SC_SOFTAP_EN
	ret = rtl_pre_parse(mac_addr, buf, userdata, &da, &sa, &len);
	if (ret == -1) {
		return;
	} else if (ret == 1) {
		simple_config_softAP_onAuth = 1;
		wifi_reg_event_handler(WIFI_EVENT_STA_ASSOC, sc_sta_asso_cb, NULL);
	} else
#elif WIFI_MESH_EN
	ret = rmesh_help_config_parse(buf, userdata, &da, &sa, &len);
	if (ret != 0) {
		return;
	}
#else
	da = buf;
	sa = buf + ETH_ALEN;
#endif

	{
		taskENTER_CRITICAL();
		if (is_promisc_callback_unlock == 1) {
			simple_config_result = rtk_start_parse_packet(da, sa, len, userdata, (void *)backup_sc_ctx);
		}
		taskEXIT_CRITICAL();
	}
}

static unsigned int simple_config_cmd_start_time;
static unsigned int simple_config_cmd_current_time;
extern int simple_config_status;
extern void rtk_restart_simple_config(void);


extern void rtk_sc_deinit(void);

void init_simple_config_lib_config(struct simple_config_lib_config *config)
{
	config->free_fn = (simple_config_free_fn)rtw_mfree;
	config->malloc_fn = (simple_config_malloc_fn)rtw_malloc;
	config->memcmp_fn = (simple_config_memcmp_fn)memcmp;
	config->memcpy_fn = (simple_config_memcpy_fn)memcpy;
	config->memset_fn = (simple_config_memset_fn)memset;
	config->printf_fn = (simple_config_printf_fn)printf;
	config->strcpy_fn = (simple_config_strcpy_fn)strcpy;
	config->strlen_fn = (simple_config_strlen_fn)strlen;
	config->zmalloc_fn = (simple_config_zmalloc_fn)rtw_zmalloc;
#if CONFIG_LWIP_LAYER
	config->ntohl_fn = (simple_config_ntohl_fn)lwip_ntohl;
#else
	config->ntohl_fn = _ntohl;
#endif
	config->is_promisc_callback_unlock = &is_promisc_callback_unlock;
}


int init_test_data(char *custom_pin_code)
{
#if (CONFIG_INCLUDE_SIMPLE_CONFIG)
	rtw_join_status_t join_status = wifi_get_join_status();

	if (is_promisc_enabled()) {
		printf("\r\npromisc mode is running!");
		return -1;
	} else if ((join_status > RTW_JOINSTATUS_UNKNOWN) && (join_status < RTW_JOINSTATUS_SUCCESS)) {
		printf("\r\nwifi connect is running!");
		return -1;
	}

	is_promisc_callback_unlock = 1;
	is_fixed_channel = 0;
	fixed_channel_num = 0;
	simple_config_result = 0;
	rtw_memset(g_ssid, 0, 33);
	g_ssid_len = 0;
	simple_config_cmd_start_time = xTaskGetTickCount();
	promisc_mode_ret = SC_SUCCESS;
	is_need_connect_to_AP = 0;
	memset(mac_addr, 0, sizeof(mac_addr));

	ssid_hidden = 0;


#if SC_SOFTAP_EN
	simple_config_softAP_onAuth = 0;
	softAP_socket = -1;
	softAP_decode_success = -1;
	rtw_init_sema(&sc_sta_assoc_sema, 0);

	softAP_decode_ctx = pvPortMalloc(sizeof(SC_softAP_decode_ctx));
	if (!softAP_decode_ctx) {
		printf("malloc softAP_decode_cxt fail");
		return -1;
	} else {
		memset(softAP_decode_ctx, 0, sizeof(SC_softAP_decode_ctx));
	}
#endif

#ifdef CONFIG_WIFI_MESH
	rmesh_help_config_init(NULL);
#endif

	rtw_init_sema(&simple_config_finish_sema, 0);

	if (ack_content != NULL) {
		vPortFree(ack_content);
		ack_content = NULL;
	}
	ack_content = pvPortMalloc(sizeof(struct ack_msg));
	if (!ack_content) {
		printf("\n\rrtk_sc_init fail by allocate ack\n");
	}
	memset(ack_content, 0, sizeof(struct ack_msg));

#ifdef SC_SCAN_SUPPORT
	if (custom_pin_code) {
		pin_enable = 1;
	} else {
		pin_enable = 0;
	}
#endif


	backup_sc_ctx = pvPortMalloc(sizeof(struct rtk_test_sc));
	if (!backup_sc_ctx) {
		printf("\n\r[Mem]malloc SC context fail\n");
	} else {
		memset(backup_sc_ctx, 0, sizeof(struct rtk_test_sc));
		struct simple_config_lib_config lib_config;
		init_simple_config_lib_config(&lib_config);
		//custom_pin_code can be null
		if (rtk_sc_init(custom_pin_code, &lib_config) < 0) {
			printf("\n\rRtk_sc_init fail\n");
		} else {
			return 0;
		}
	}

#else
	printf("\n\rPlatform no include simple config now\n");
#endif
	return -1;
}


void deinit_test_data(void)
{
#if (CONFIG_INCLUDE_SIMPLE_CONFIG)
	rtk_sc_deinit();
	if (backup_sc_ctx != NULL) {
		vPortFree(backup_sc_ctx);
		backup_sc_ctx = NULL;
	}
	if (ack_content != NULL) {
		vPortFree(ack_content);
		ack_content = NULL;
	}

#if SC_SOFTAP_EN
	if (softAP_decode_ctx != NULL) {
		vPortFree(softAP_decode_ctx);
		softAP_decode_ctx = NULL;
	}
	rtw_free_sema(&sc_sta_assoc_sema);
#endif
#ifdef CONFIG_WIFI_MESH
	rmesh_help_config_deinit();
#endif

	rtw_free_sema(&simple_config_finish_sema);

#endif
}

void stop_simple_config(void)
{
	simple_config_terminate = 1;
}

#if SC_SOFTAP_EN
// use new ssid generation function
static void simpleConfig_get_softAP_profile(unsigned char *SimpleConfig_SSID, unsigned char *SimpleConfig_password)
{
	char MAC_sum_complement = 0;

	memcpy(mac_addr, LwIP_GetMAC(0), 6);
	if (mac_addr == NULL) {
		printf("Get MAC error\n");
		return;
	}

	// copy MAC to softAP decode ctx
	memcpy(softAP_decode_ctx -> mac, mac_addr, 6);

	MAC_sum_complement = -(mac_addr[3] + mac_addr[4] + mac_addr[5]);
	if (strlen((char const *)softap_prefix) > 0)
		snprintf((char *)SimpleConfig_SSID, SOFTAP_SSID_LEN, "%s-%02X%02X%02X00%02X",
				 softap_prefix, mac_addr[3], mac_addr[4], mac_addr[5], (MAC_sum_complement & 0xff));
	else
		snprintf((char *)SimpleConfig_SSID, SOFTAP_SSID_LEN, "@RSC-%02X%02X%02X00%02X",
				 mac_addr[3], mac_addr[4], mac_addr[5], (MAC_sum_complement & 0xff));

	memcpy(SimpleConfig_password, "12345678", 8);

	printf("softAP ssid: %s, password: %s\n", SimpleConfig_SSID, SimpleConfig_password);
	return;

}
static void sc_sta_asso_cb(char *buf, int buf_len, int flags, void *userdata)
{
	/* To avoid gcc warnings */
	(void) buf;
	(void) buf_len;
	(void) flags;
	(void) userdata;

	rtw_up_sema(&sc_sta_assoc_sema);
	return;
}

static void simple_config_kick_STA(void)
{
	int client_idx = 0;
	struct {
		int    count;
		rtw_mac_t mac_list[AP_STA_NUM];
	} client_info;

	memset(&client_info, 0, sizeof(client_info));
	client_info.count = AP_STA_NUM;
	wifi_get_associated_client_list(&client_info, sizeof(client_info));

	while (client_idx < client_info.count) {
		unsigned char *pmac = client_info.mac_list[client_idx].octet;
		printf("kick out sta: %02x:%02x:%02x:%02x:%02x:%02x\n",
			   pmac[0], pmac[1], pmac[2], pmac[3], pmac[4], pmac[5]);
		wifi_del_station(WLAN0_IDX, pmac);
		++client_idx;
	}
	return;

}
// triggered by sta association event
static int simple_config_softap_config(void)
{
	int ret = -1;
	struct sockaddr from;
	socklen_t fromLen = sizeof(from);
	unsigned char recv_buf[128];
	int client_fd = -1;
	int tcp_err = 0;
	SC_softAP_status decode_ret = SOFTAP_ERROR;

	// block 10s to receive udp packet from smart phone
	client_fd = accept(softAP_socket, (struct sockaddr *) &from, &fromLen);

	if (client_fd < 0) {
		printf("no client connection, timeout\n");
		//kick the sta, wf, 0817
		simple_config_kick_STA();
		return -1;
	}

	//printf("accept a new client: %d\n", client_fd);
	while (1) {
		int sock_err = 0;
		size_t err_len = sizeof(sock_err);

		vTaskDelay(10);
		ret = recv(client_fd, recv_buf, sizeof(recv_buf), MSG_DONTWAIT);
		getsockopt(client_fd, SOL_SOCKET, SO_ERROR, &sock_err, &err_len);

		// ret == -1 and socket error == EAGAIN when no data received for nonblocking
		if ((ret == -1) && ((sock_err == EAGAIN)
#if LWIP_VERSION_MAJOR >= 2
							|| (sock_err == 0)
#endif
						   )) {
			continue;
		} else if (ret <= 0) {
			tcp_err = -1;
			break;
		} else {
			// decode simpleConfig pkt
			decode_ret = softAP_simpleConfig_parse(recv_buf, ret, backup_sc_ctx, softAP_decode_ctx);
			switch (decode_ret) {
			case SOFTAP_ERROR:
				continue;
			case SOFTAP_RECV_A: //send nonceB
				if (send(client_fd, softAP_decode_ctx -> nonceB, sizeof(softAP_decode_ctx -> nonceB), 0) < 0) {
					tcp_err = -2;
					break;
				} else {
					continue;
				}
			case SOFTAP_HANDSHAKE_DONE:// send response to finish handshake
				if (send(client_fd, "OK", 2, 0) < 0) {
					tcp_err = -2;
					break;
				}
				continue;
			case SOFTAP_DECODE_SUCCESS: {
				char softAP_ack_content[18];
				//printf("softAP mode simpleConfig success, send response\n");
				// ack content: MAC address in string mode
				snprintf(softAP_ack_content, sizeof(softAP_ack_content), "%02x:%02x:%02x:%02x:%02x:%02x",
						 mac_addr[0], mac_addr[1], mac_addr[2],
						 mac_addr[3], mac_addr[4], mac_addr[5]);

				if (send(client_fd, softAP_ack_content, sizeof(softAP_ack_content) - 1, 0) <= 0) {
					tcp_err = -3;
				} else {
					vTaskDelay(500);    // make sure the response pkt has enough time to be transmitted
				}
				break;
			}
			default:
				break;
			}

			break; // break the while loop
		}
	}
	// close the client socket
	close(client_fd);

	//error handler here, wf, 0816
	if (tcp_err < 0) {
		if (tcp_err == -1) {
			printf("tcp recv error\n");
		} else {
			printf("tcp send response error\n");
		}

		tcp_err = 0;
		return -1;
	} else {
		return 0;
	}

}
static void stop_fast_connect(void)
{
	p_wifi_do_fast_connect_backup = p_wifi_do_fast_connect;
	p_wifi_do_fast_connect = NULL;

	return;
}

static void resume_fast_connect(void)
{
	p_wifi_do_fast_connect = p_wifi_do_fast_connect_backup;
	return;
}

// use the softAP channel to reset the promisc channel table
static void init_promisc_scan_channel(unsigned char softAP_ch)
{
	int i = sizeof(simple_config_promisc_channel_tbl) / sizeof(simple_config_promisc_channel_tbl[0]);
	simple_config_promisc_channel_tbl[i - 1] = softAP_ch;
	return;
}


extern void dhcps_deinit(void);
extern void dhcps_init(struct netif *pnetif);
static int SimpleConfig_softAP_start(const char *ap_name, const char *ap_password)
{
	int timeout = 20;
#if CONFIG_LWIP_LAYER
	struct netif *pnetif = &xnetif[0];
#endif
	rtw_softap_info_t softAP_config = {0};
	int ret = 0;

	wifi_set_mode(RTW_MODE_AP);

#if CONFIG_LWIP_LAYER
	dhcps_deinit();

	u32 addr = WIFI_MAKEU32(AP_IP_ADDR0, AP_IP_ADDR1, AP_IP_ADDR2, AP_IP_ADDR3);
	u32 netmask = WIFI_MAKEU32(AP_NETMASK_ADDR0, AP_NETMASK_ADDR1, AP_NETMASK_ADDR2, AP_NETMASK_ADDR3);
	u32 gw = WIFI_MAKEU32(AP_GW_ADDR0, AP_GW_ADDR1, AP_GW_ADDR2, AP_GW_ADDR3);
	LwIP_SetIP(0, addr, netmask, gw);
#endif

	wifi_set_powersave_mode(IPS_MODE_NONE, LPS_MODE_NONE);//add to close powersave
#ifdef CONFIG_WPS_AP
	wpas_wps_dev_config(LwIP_GetMAC(0), 1);
#endif

	softAP_config.ssid.len = strlen(ap_name);
	rtw_memcpy(softAP_config.ssid.val, (unsigned char *)ap_name, softAP_config.ssid.len);
	softAP_config.channel = simple_config_softAP_channel;
	if (ap_password) {
		softAP_config.password = (unsigned char *)ap_password;
		softAP_config.password_len = strlen(ap_password);
		softAP_config.security_type = RTW_SECURITY_WPA2_AES_PSK;
	} else {
		softAP_config.security_type = RTW_SECURITY_OPEN;
	}
	if (wifi_start_ap(&softAP_config) != RTW_SUCCESS) {
		printf("wifi start ap mode failed!\n\r");
		return -1;
	}

	while (1) {
		rtw_wifi_setting_t setting;
		wifi_get_setting(WLAN0_IDX, &setting);
		if (strlen((char *)setting.ssid) > 0) {
			if (strcmp((const char *) setting.ssid, (const char *)ap_name) == 0) {
				//printf("%s started\n", ap_name);
				ret = 0;
				break;
			}
		}

		if (timeout == 0) {
			printf("Start AP timeout!\n\r");
			ret = -1;
			break;
		}

		vTaskDelay(1 * configTICK_RATE_HZ);
		timeout --;
	}
#if CONFIG_LWIP_LAYER
	dhcps_init(pnetif);
#endif
	return ret;
}

#endif // end of SC_SOFTAP_EN

static int simple_config_get_channel_interval(int ch_idx)
{
	int interval = 105;
#if SC_SOFTAP_EN
	int ch_len = sizeof(simple_config_promisc_channel_tbl) / sizeof(simple_config_promisc_channel_tbl[0]);

	if (ch_idx == ch_len - 1) { // this is the softAP channel idx
		interval = 5000;
	}
#endif
	return interval;
}

static void simple_config_channel_control(void *para)
{
	int ch_idx = 0;
	unsigned int start_time;
	int fix_channel = 0;
	int delta_time = 0;
#if SC_SOFTAP_EN
	unsigned char softAP_SSID[33];
	unsigned char softAP_password[65];
#endif
	rtw_network_info_t *wifi = (rtw_network_info_t *)para;
	start_time = xTaskGetTickCount();

	while (simple_config_terminate != 1) {
		vTaskDelay(50);	//delay 0.5s to release CPU usage

#if defined(CONFIG_BT_CONFIG) && CONFIG_BT_CONFIG
		if (get_bt_config_state() == BC_DEV_BT_CONNECTED) {
			if (is_promisc_enabled()) {
				wifi_set_promisc(RTW_PROMISC_DISABLE, NULL, 0);
			}
			simple_config_cmd_start_time = xTaskGetTickCount();
			start_time = xTaskGetTickCount();
			continue;
		} else {
			if ((wifi_get_join_status() == RTW_JOINSTATUS_SUCCESS) && (*(u32 *)LwIP_GetIP(0) != IP_ADDR_INVALID)) {
				break;
			} else if (! is_promisc_enabled()) {
#if SC_SOFTAP_EN
				wifi_set_promisc(RTW_PROMISC_ENABLE_2, simple_config_callback, 1);
#else
				wifi_set_promisc(RTW_PROMISC_ENABLE, simple_config_callback, 1);
#endif
			}
		}
#endif // defined(CONFIG_BT_CONFIG)

#if SC_SOFTAP_EN
		// softAP mode already get the AP profile
		if (is_need_connect_to_AP == 1) {
			break;
		}

		if (simple_config_softAP_onAuth == 1) {
			wifi_set_promisc(RTW_PROMISC_DISABLE, NULL, 0);
			wifi_set_channel(simple_config_softAP_channel);
			//move to simple_config_callback() for assoc req recved before go to here if keep auth and assoc req after promisc_recv_func.
			//wifi_reg_event_handler(WIFI_EVENT_STA_ASSOC, sc_sta_asso_cb, NULL);

			if (rtw_down_timeout_sema(&sc_sta_assoc_sema, SC_SOFTAP_TIMEOUT) == RTW_FALSE) {
				//printf("no sta associated after 10s, start promisc\n");
				simple_config_softAP_onAuth = 0;
				wifi_set_promisc(RTW_PROMISC_ENABLE_2, simple_config_callback, 1);
			} else {
				//printf("new sta associated, wait tcp connection\n");
				softAP_decode_success = simple_config_softap_config();
				if (softAP_decode_success == 0) {
					is_need_connect_to_AP = 1;
					wifi_unreg_event_handler(WIFI_EVENT_STA_ASSOC, sc_sta_asso_cb);
					break;
				} else {
					simple_config_softAP_onAuth = 0;
					wifi_set_promisc(RTW_PROMISC_ENABLE_2, simple_config_callback, 1);
				}
			}
			wifi_unreg_event_handler(WIFI_EVENT_STA_ASSOC, sc_sta_asso_cb);
		}
#endif

#ifdef CONFIG_WIFI_MESH
		if (remsh_configed_by_help() == _TRUE) {
			break;
		}
#endif

		simple_config_cmd_current_time = xTaskGetTickCount();

#if CONFIG_GAGENT
		if (simple_config_cmd_current_time - simple_config_cmd_start_time < ((50 + delta_time)*configTICK_RATE_HZ))
#else
		if (simple_config_cmd_current_time - simple_config_cmd_start_time < ((120 + delta_time)*configTICK_RATE_HZ))
#endif
		{
			unsigned int current_time = xTaskGetTickCount();
			int interval = simple_config_get_channel_interval(ch_idx);

#ifdef CONFIG_WIFI_MESH
			interval = interval + (rmesh_get_mesh_offset(simple_config_promisc_channel_tbl[ch_idx]) * 200);
#endif


			if (((current_time - start_time) * 1000 / configTICK_RATE_HZ < (u32)interval)
				|| (is_fixed_channel == 1)) {
				if (is_fixed_channel == 0 && get_channel_flag == 1) {
					fix_channel = promisc_get_fixed_channel(g_bssid, g_ssid, &g_ssid_len);
					if (fix_channel != 0) {
						printf("\r\nin simple_config_test fix channel = %d ssid: %s\n", fix_channel, g_ssid);
						is_fixed_channel = 1;
						fixed_channel_num = fix_channel;
						wifi_set_channel(fix_channel);
#if SC_SOFTAP_EN
						if (simple_config_softAP_channel != fixed_channel_num) {
							simple_config_softAP_channel = fixed_channel_num;
							memset(softAP_SSID, 0, sizeof(softAP_SSID));
							memset(softAP_password, 0, sizeof(softAP_password));
							simpleConfig_get_softAP_profile(softAP_SSID, softAP_password);
							SimpleConfig_softAP_start((char const *)softAP_SSID, (char const *)softAP_password);
							wifi_set_promisc(RTW_PROMISC_ENABLE_2, simple_config_callback, 1);
						}
#endif
					} else {
						printf("get channel fail\n");
					}
				}

				if (simple_config_result == 1) {
					is_need_connect_to_AP = 1;
					is_fixed_channel = 0;
					break;
				}
				if (simple_config_result == -1) {
					printf("\r\nsimple_config_test restart for result = -1");
					delta_time = 60;
					wifi_set_channel(1);
					is_need_connect_to_AP = 0;
					is_fixed_channel = 0;
					fixed_channel_num = 0;
					memset(g_ssid, 0, 33);
					g_ssid_len = 0;
					simple_config_result = 0;
					g_security_mode = 0xff;
					rtk_restart_simple_config();
				}
			} else {
				ch_idx++;
				if (ch_idx >= (int)sizeof(simple_config_promisc_channel_tbl) / (int)sizeof(simple_config_promisc_channel_tbl[0])) {
					ch_idx = 0;
#ifdef CONFIG_WIFI_MESH
					rmesh_change_mesh_beacon_save_life();
#endif
				}

				if (wifi_set_channel(simple_config_promisc_channel_tbl[ch_idx]) == 0) {
					start_time = xTaskGetTickCount();
					printf("\n\rSwitch to channel(%d)\n", simple_config_promisc_channel_tbl[ch_idx]);
				}



#ifdef CONFIG_WIFI_MESH
				rmesh_help_config_request_sends(_FALSE, simple_config_promisc_channel_tbl[ch_idx], NULL, 0);
#endif
			}

		} else {
			promisc_mode_ret = SC_NO_CONTROLLER_FOUND;
			break;
		}
	} // end of while

	if (is_promisc_enabled()) {
		wifi_set_promisc(RTW_PROMISC_DISABLE, NULL, 0);
	}

#if SC_SOFTAP_EN
#if defined(CONFIG_BT_CONFIG) && CONFIG_BT_CONFIG
	rtw_wifi_setting_t setting = {0};
	wifi_get_setting(WLAN0_IDX, &setting); // mode is equal to RTW_MODE_STA if breaked by BT Config
	if (setting.mode != RTW_MODE_STA) {
		simple_config_kick_STA();
	}
#else
	simple_config_kick_STA();
#endif
	close(softAP_socket);

	stop_fast_connect();

	wifi_set_mode(RTW_MODE_STA);

	resume_fast_connect();

#endif // end of SC_SOFTAP_EN

#ifdef CONFIG_WIFI_MESH
	if (remsh_configed_by_help() == _TRUE) {
		example_rmesh(1);
	}
#endif

	if (is_need_connect_to_AP == 1) {
		if (NULL == wifi) {
			int tmp_res = SC_connect_to_AP();
			if (SC_SUCCESS == tmp_res) {

				if (-1 == SC_send_simple_config_ack(30)) {
					promisc_mode_ret = SC_UDP_SOCKET_CREATE_FAIL;
				}
#ifdef CONFIG_WIFI_MESH
				example_rmesh(2);
#endif

#ifdef SC_SCAN_SUPPORT
				// check whether the thread of listen scan command is already created
				if (scan_start == 0) {
					scan_start = 1;
					SC_listen_ACK_scan();
				}
#endif
			} else {
				promisc_mode_ret = (enum sc_result) tmp_res;
			}
		} else {
			if (-1 == get_connection_info_from_profile(wifi->security_type, wifi)) {
				promisc_mode_ret = SC_CONTROLLER_INFO_PARSE_FAIL;
			} else {
				promisc_mode_ret = SC_SUCCESS;
			}
		}
	} else {
		promisc_mode_ret = SC_NO_CONTROLLER_FOUND;
	}

	rtw_up_sema(&simple_config_finish_sema);
	vTaskDelete(NULL);
	return;
}

enum sc_result simple_config_test(rtw_network_info_t *wifi)
{

#if SC_SOFTAP_EN
	unsigned char softAP_SSID[33];
	unsigned char softAP_password[65];
	struct sockaddr_in softAP_addr;
	//unsigned char channel_set[11];
	unsigned char channel_set[3];
	int auto_chl = 0;
	int timeout = 60000;
	int tcp_reuse_timeout = 1;

	memset(softAP_SSID, 0, sizeof(softAP_SSID));
	memset(softAP_password, 0, sizeof(softAP_password));
	simpleConfig_get_softAP_profile(softAP_SSID, softAP_password);

	channel_set[0] = 1;
	channel_set[1] = 6;
	channel_set[2] = 11;

	auto_chl = wifi_get_auto_chl(WLAN0_IDX, channel_set, sizeof(channel_set) / sizeof(channel_set[0]));

	if (auto_chl <= 0) {
		printf("Get softAP channel error\n, use static channel\n");
		simple_config_softAP_channel = 6;
	} else {
		simple_config_softAP_channel = auto_chl;
	}

	//printf("softAP channel is set to %d\n", simple_config_softAP_channel);

	init_promisc_scan_channel(simple_config_softAP_channel);

	SimpleConfig_softAP_start((char const *)softAP_SSID, (char const *)softAP_password);

	wifi_set_promisc(RTW_PROMISC_ENABLE_2, simple_config_callback, 1);
	softAP_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (softAP_socket == -1) {
		printf("softAP_socket create error\n");
		return SC_UDP_SOCKET_CREATE_FAIL;
	}

	setsockopt(softAP_socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&tcp_reuse_timeout, sizeof(tcp_reuse_timeout));

	memset(&softAP_addr, 0, sizeof(softAP_addr));
	softAP_addr.sin_family = AF_INET;
	softAP_addr.sin_port = htons(18884);
	softAP_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(softAP_socket, (struct sockaddr *) &softAP_addr, sizeof(softAP_addr)) != 0) {
		printf("softAP bind error\n");
		close(softAP_socket);
		return SC_UDP_SOCKET_CREATE_FAIL;
	}

	if (lwip_setsockopt(softAP_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(int)) < 0) {
		printf("set socket timeout error\n");
	}

	if (listen(softAP_socket, 2) != 0) {
		printf("ERROR: listen\n");
		close(softAP_socket);
		return SC_UDP_SOCKET_CREATE_FAIL;
	}

#elif WIFI_MESH_EN
	wifi_set_promisc(RTW_PROMISC_ENABLE_2, simple_config_callback, 1);
#else

	wifi_set_promisc(RTW_PROMISC_ENABLE, simple_config_callback, 1);
#endif

	if (xTaskCreate(simple_config_channel_control, ((const char *)"simple_config_channel_control"), 1024, wifi, tskIDLE_PRIORITY + 5, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate(simple_config_channel_control) failed", __FUNCTION__);
	}

	if (rtw_down_sema(&simple_config_finish_sema) == _FAIL) {
		printf("%s, Take Semaphore Fail\n", __FUNCTION__);
	}

	return promisc_mode_ret;
}

//Filter packet da[] = {0x01, 0x00, 0x5e}
//add another filter for bcast, {0xff, 0xff, 0xff, 0xff}
#define MASK_SIZE 3
void filter_add_enable(void)
{
	u8 mask[MASK_SIZE] = {0xFF, 0xFF, 0xFF};
	u8 pattern[MASK_SIZE] = {0x01, 0x00, 0x5e};
#ifdef CONFIG_WIFI_MESH
	u8 pattern2[MASK_SIZE] = {0xFF, 0xFF, 0xFF};
#endif
	rtw_packet_filter_pattern_t packet_filter;
	rtw_packet_filter_rule_t rule;

	packet_filter.offset = 0;
	packet_filter.mask_size = 3;
	packet_filter.mask = mask;
	packet_filter.pattern = pattern;

	rule = RTW_POSITIVE_MATCHING;

	wifi_init_packet_filter();
	wifi_add_packet_filter(1, &packet_filter, rule);
	wifi_enable_packet_filter(1);

#ifdef CONFIG_WIFI_MESH
	packet_filter.pattern = pattern2;
	wifi_add_packet_filter(2, &packet_filter, rule);
	wifi_enable_packet_filter(2);
#endif
}

void remove_filter(void)
{
	wifi_disable_packet_filter(1);
	wifi_remove_packet_filter(1);
}

#define MASK1_SIZE 12
#define MASK2_SIZE 18

void filter1_add_enable(void)
{
	u8 mask1[MASK1_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	u8 pattern[MASK1_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]};
	u8 pattern2[MASK1_SIZE] = {mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5], mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]};

	u8 mask2[MASK2_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	u8 pattern3[MASK2_SIZE] = {mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5], 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]};

	rtw_packet_filter_pattern_t packet_filter, packet_filter2, packet_filter3;
	rtw_packet_filter_rule_t rule;

	packet_filter.offset = 4;
	packet_filter.mask_size = 12;
	packet_filter.mask = mask1;
	packet_filter.pattern = pattern;

	packet_filter2.offset = 10;
	packet_filter2.mask_size = 12;
	packet_filter2.mask = mask1;
	packet_filter2.pattern = pattern2;

	packet_filter3.offset = 4;
	packet_filter3.mask_size = 18;
	packet_filter3.mask = mask2;
	packet_filter3.pattern = pattern3;

	rule = RTW_POSITIVE_MATCHING;

	wifi_init_packet_filter();
	wifi_add_packet_filter(1, &packet_filter2, rule);
	wifi_enable_packet_filter(1);
	wifi_add_packet_filter(2, &packet_filter3, rule);
	wifi_enable_packet_filter(2);
	if (ssid_hidden == 0) {
		wifi_add_packet_filter(3, &packet_filter, rule);
		wifi_enable_packet_filter(3);
	}
}

void remove1_filter(void)
{
	wifi_disable_packet_filter(1);
	wifi_remove_packet_filter(1);
	wifi_disable_packet_filter(2);
	wifi_remove_packet_filter(2);
	if (ssid_hidden == 0) {
		wifi_disable_packet_filter(3);
		wifi_remove_packet_filter(3);
	}
}

void print_simple_config_result(enum sc_result sc_code)
{
	printf("\r\n");
	switch (sc_code) {
	case SC_NO_CONTROLLER_FOUND:
		printf("Simple Config timeout!! Can't get Ap profile. Please try again\n");
		break;
	case SC_CONTROLLER_INFO_PARSE_FAIL:
		printf("Simple Config fail, cannot parse target ap info from controller\n");
		break;
	case SC_TARGET_CHANNEL_SCAN_FAIL:
		printf("Simple Config cannot scan the target channel\n");
		break;
	case SC_JOIN_BSS_FAIL:
		printf("Simple Config Join bss failed\n");
		break;
	case SC_DHCP_FAIL:
		printf("Simple Config fail, cannot get dhcp ip address\n");
		break;
	case SC_UDP_SOCKET_CREATE_FAIL:
		printf("Simple Config Ack socket create fail!!!\n");
		break;
	case SC_TERMINATE:
		printf("Simple Config terminate\n");
		break;
	case SC_SUCCESS:
		printf("Simple Config success\n");
		break;

	case SC_ERROR:
	default:
		printf("unknown error when simple config!\n");

	}
}

#endif //CONFIG_INCLUDE_SIMPLE_CONFIG

void cmd_simple_config(int argc, char **argv)
{
#if CONFIG_INCLUDE_SIMPLE_CONFIG
	char *custom_pin_code = NULL;
	enum sc_result ret = SC_ERROR;

	if (argc > 2) {
		printf("\n\rInput Error!");
	}

	if (argc == 2) {
		custom_pin_code = (argv[1]);
		if (strlen(custom_pin_code) != 8) {
			printf("Pin length error, please input 8 byte pin code");
			return;
		}
	}

// check whether the pin code is valid
	simple_config_terminate = 0;

#if !SC_SOFTAP_EN
	wifi_set_mode(RTW_MODE_PROMISC);
#endif

	if (init_test_data(custom_pin_code) == 0) {

#if ((!SC_SOFTAP_EN) && (!WIFI_MESH_EN))
		filter_add_enable();
#endif
		ret = simple_config_test(NULL);
		deinit_test_data();

#if ((!SC_SOFTAP_EN) && (!WIFI_MESH_EN))
		remove_filter();
#endif
		print_simple_config_result(ret);
	}

#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
	if (ret == SC_SUCCESS) {
		at_printf("\n\r[ATWQ] OK");
	} else {
		at_printf("\n\r[ATWQ] ERROR:%d", ret);
	}
#endif

#else
	UNUSED(argc);
	UNUSED(argv);
#endif
}
#endif	//#if CONFIG_WLAN
