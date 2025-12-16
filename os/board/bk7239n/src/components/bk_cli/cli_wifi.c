#include <string.h>
#include <common/sys_config.h>
#include "bk_uart.h"
#if defined(CONFIG_LWIP)
#include <../../lwip_intf_v2_1/lwip-2.1.2/port/net.h>
#endif
#include "lwip/ip4.h"
#include "bk_private/bk_wifi.h"
#include "bk_wifi_private.h"
#include "bk_cli.h"
#include "cli.h"
#include <components/event.h>
#include <components/netif.h>
#include "bk_wifi.h"
#include "bk_wifi_types.h"
#if defined(CONFIG_WIFI_CLI_RAW_LINK_ENABLE)
#include <modules/raw_link.h>
#endif
#if defined(CONFIG_WIFI6_CODE_STACK)
#include "bk_wifi.h"
#endif
#if defined(CONFIG_BRIDGE)
#include "netif/bridgeif.h"
#include "lwip/netifapi.h"
#include "lwip/inet.h"
#endif

#if defined(CONFIG_ENABLE_WIFI_DEFAULT_CONNECT)
#include "driver/flash.h"
#include <driver/flash_partition.h>
#include <lwip/sockets.h>
#include "bk_wifi_private.h"
#include "boot.h"
#endif

#define TAG "wifi_cli"
#define CMD_WLAN_MAX_BSS_CNT	50
beken_semaphore_t wifi_cmd_sema = NULL;
int wifi_cmd_status = 0;

#if defined(CONFIG_BRIDGE)
extern uint8 bridge_is_enabled;
uint8 bridge_open;
char br_ssid[WIFI_SSID_STR_LEN];
char br_pwd[WIFI_PASSWORD_LEN];
#endif

#if (CLI_CFG_WIFI == 1)
int wifi_cli_find_id(int argc, char **argv, char *param)
{
	int i;
	int index;

	index = -1;
	if (NULL == param)
		goto find_over;

	for (i = 1; i < argc; i ++) {
		if (os_strcmp(argv[i], param) == 0) {
			index = i;
			break;
		}
	}

find_over:
	return index;
}

#if defined(CONFIG_ENABLE_WIFI_DEFAULT_CONNECT)
typedef struct bk_fast_connect_t
{
	uint8_t flag;		//to check if ssid/pwd saved in easy flash is valid, default 0x70
					//bit[0]:write sta deault info;bit[1]:write ap deault info
	uint8_t sta_ssid[33];
	uint8_t sta_pwd[65];
	uint8_t ap_ssid[33];
	uint8_t ap_pwd[65];
	uint8_t ap_channel;
}BK_FAST_CONNECT_T;

static BK_FAST_CONNECT_T info_t;
static int fast_connect_cb(void *arg, event_module_t event_module,
                int event_id, void *event_data)
{
	bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_USR_CONFIG);
	BK_FAST_CONNECT_T info_tmp;

	CLI_LOGI("%s, flag:%x\r\n", __func__, info_t.flag);
	bk_flash_read_bytes(pt->partition_start_addr + pt->partition_length -4096,
						(uint8_t *)&info_tmp, sizeof(BK_FAST_CONNECT_T));

	if (info_t.flag == 0x71l) {
		if ((info_tmp.flag & 0xf8l) == 0x70l)
			info_tmp.flag |= 0x1l;
		else
			info_tmp.flag = 0x71l;
		os_strcpy((char *)info_tmp.sta_ssid, (char *)info_t.sta_ssid);
		os_strcpy((char *)info_tmp.sta_pwd, (char *)info_t.sta_pwd);
	} else if (info_t.flag == 0x72l) {
		if ((info_tmp.flag & 0xf8l) == 0x70l)
			info_tmp.flag |= 0x2l;
		else
			info_tmp.flag = 0x72l;
		os_strcpy((char *)info_tmp.ap_ssid, (char *)info_t.ap_ssid);
		os_strcpy((char *)info_tmp.ap_pwd, (char *)info_t.ap_pwd);
	} else
		return -1;

	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
	bk_flash_erase_sector(pt->partition_start_addr + pt->partition_length -4096);
	bk_flash_write_bytes(pt->partition_start_addr + pt->partition_length -4096,
						(uint8_t *)&info_tmp, sizeof(BK_FAST_CONNECT_T));
	bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);

	return 0;
}

#if 0
static int  demo_tcp_send(void *arg, event_module_t event_module,
                int event_id, void *event_data)
{
	struct sockaddr_in addr;
	int flag = 1, sock, ret = -1, len = 40*1024;
	struct timeval tv;
	uint8_t *send_buf;

	send_buf = (uint8_t *) os_malloc(len);
	for (int i = 0; i < len; i++)
		send_buf[i] = i & 0xff;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		CLI_LOGI("create socket failed, err=%d!\n", errno);
		return -1;
	}
	addr.sin_family = PF_INET;
	addr.sin_port = htons(5001);
	addr.sin_addr.s_addr = inet_addr((char *)"192.168.1.102");

	ret = connect(sock, (const struct sockaddr *)&addr, sizeof(addr));
	if (ret == -1) {
		CLI_LOGI("connect failed, err=%d!\n", errno);
		closesocket(sock);
		return -1;
	}

	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY,
			   (void *)&flag, sizeof(int));

	tv.tv_sec = 3;
	tv.tv_usec = 0;
	setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

	tv.tv_sec = 3;
	tv.tv_usec = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

	ret = send(sock, send_buf, len, 0);

	closesocket(sock);

	return 0;
}
#endif

void demo_wifi_fast_connect(void)
{
	bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_USR_CONFIG);
	BK_FAST_CONNECT_T info;

	bk_flash_read_bytes(pt->partition_start_addr + pt->partition_length -4096,
						(uint8_t *)&info, sizeof(BK_FAST_CONNECT_T));
	CLI_LOGD("%s, flag:%x\r\n", __func__, info.flag);
	if (info.flag == 0x71l) {
		demo_sta_app_init((char *)info.sta_ssid, (char *)info.sta_pwd);
#if 0
		bk_event_register_cb(EVENT_MOD_NETIF, EVENT_NETIF_GOT_IP4,
								demo_tcp_send, &info_t);
#endif
	} else if (info.flag == 0x72l) {
		demo_softap_app_init((char *)info.ap_ssid, (char *)info.ap_pwd, NULL);
	} else if (info.flag == 0x73l) {
		demo_sta_app_init((char *)info.sta_ssid, (char *)info.sta_pwd);
		demo_softap_app_init((char *)info.ap_ssid, (char *)info.ap_pwd, NULL);
	}
}
#endif

void cli_wifi_scan_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	char *msg = NULL;

	if (argc < 2) {
		demo_scan_app_init();
	} else {
		uint8_t *ap_ssid;

		ap_ssid = (uint8_t *)argv[1];
		demo_scan_adv_app_init(ap_ssid);
	}

	if(ret == 0)
		msg = WIFI_CMD_RSP_SUCCEED;
	else
		msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

void cli_wifi_scan_opt_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	char *msg = NULL;

	if (argc < 3) {
	    CLI_LOGE("need scantype duration\n");
	    msg = WIFI_CMD_RSP_ERROR;
	    return;
	} else {
		demo_scan_opt_app_init(atoi(argv[1]), atoi(argv[2]));
	}

	if(ret == 0)
		msg = WIFI_CMD_RSP_SUCCEED;
	else
		msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

#include "conv_utf8_pub.h"
void cli_wifi_ap_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *ap_ssid = NULL;
	char *ap_key = "";
	char *ap_channel = NULL;
	int ret = 0;
	char *msg = NULL;

#if defined(CONFIG_ENABLE_WIFI_DEFAULT_CONNECT)
	if (wifi_cli_find_id(argc, argv, "-w") > 0 ||
		wifi_cli_find_id(argc, argv, "-e") > 0) {
		if (argc == 3)
			ap_ssid = argv[2];
		else if (argc == 4) {
			ap_ssid = argv[2];
			if (os_strlen(argv[3]) <= 2)
				ap_channel = argv[3];
			else
				ap_key = argv[3];
		} else if (argc == 5) {
			ap_ssid = argv[2];
			ap_key = argv[3];
			ap_channel = argv[4];
		}
	} else {
#endif
		if (argc == 2)
			ap_ssid = argv[1];
		else if (argc == 3) {
			ap_ssid = argv[1];
			if (os_strlen(argv[2]) <= 2)
				ap_channel = argv[2];
			else
				ap_key = argv[2];
		} else if (argc == 4) {
			ap_ssid = argv[1];
			ap_key = argv[2];
			ap_channel = argv[3];
		}else{
			CLI_LOGI("Invalid parameters\n");
			return;
		}
#if defined(CONFIG_ENABLE_WIFI_DEFAULT_CONNECT)
	}
#endif
	char *oob_ssid_softap = ap_ssid;
#if defined(CONFIG_USE_CONV_UTF8)
	oob_ssid_softap = (char *)conv_utf8((uint8_t *)ap_ssid);
#endif
	if (oob_ssid_softap) {
		ret = demo_softap_app_init((char *)oob_ssid_softap, ap_key, ap_channel);
#if defined(CONFIG_ENABLE_WIFI_DEFAULT_CONNECT)
		bk_event_unregister_cb(EVENT_MOD_WIFI, EVENT_WIFI_STA_CONNECTED,
								fast_connect_cb);
		if (wifi_cli_find_id(argc, argv, "-w") > 0) {
			bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_USR_CONFIG);

			bk_flash_read_bytes(pt->partition_start_addr + pt->partition_length -4096,
						(uint8_t *)&info_t, sizeof(BK_FAST_CONNECT_T));
			if ((info_t.flag & 0xf0l) == 0x70l)
				info_t.flag |= 0x2l;
			else
				info_t.flag = 0x72l;
			os_strcpy((char *)info_t.ap_ssid, (char *)ap_ssid);
			os_strcpy((char *)info_t.ap_pwd, ap_key);
			fast_connect_cb(NULL, 0, 0, NULL);
		} else if (wifi_cli_find_id(argc, argv, "-e") > 0) {
			bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_USR_CONFIG);

			bk_flash_read_bytes(pt->partition_start_addr + pt->partition_length -4096,
						(uint8_t *)&info_t, sizeof(BK_FAST_CONNECT_T));
			if (info_t.flag == 0x72l || info_t.flag == 0x73l) {
				info_t.flag &= ~0x2l;
				bk_flash_set_protect_type(FLASH_PROTECT_NONE);
				bk_flash_erase_sector(pt->partition_start_addr + pt->partition_length -4096);
				bk_flash_write_bytes(pt->partition_start_addr + pt->partition_length -4096,
									(uint8_t *)&info_t, sizeof(BK_FAST_CONNECT_T));
				bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
			}
		}
#endif
#if defined(CONFIG_USE_CONV_UTF8)
		os_free(oob_ssid_softap);
#endif

	}
	if(ret == 0)
		msg = WIFI_CMD_RSP_SUCCEED;
	else
		msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

void cli_wifi_hidden_ap_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	wifi_ap_config_t ap_config = WIFI_DEFAULT_AP_CONFIG();
	netif_ip4_config_t ip4_config = {0};
	int len;
	char *ap_ssid = NULL;
	char *ap_key = "";
	char *ap_channel = NULL;
	int ret = 0;
	char *msg = NULL;

	if (argc == 2)
			ap_ssid = argv[1];
	else if (argc == 3) {
		ap_ssid = argv[1];
		if (os_strlen(argv[2]) <= 2)
			ap_channel = argv[2];
		else
			ap_key = argv[2];
	} else if (argc == 4) {
		ap_ssid = argv[1];
		ap_key = argv[2];
		ap_channel = argv[3];
	}else{
		CLI_LOGI("Invalid parameters\n");
		return;
	}

	if (ap_ssid) {
		len = os_strlen(ap_ssid);
		if (32 < len) {
			CLI_LOGE("ssid name more than 32 Bytes\r\n");
			return;
		}

		os_strcpy(ip4_config.ip, WLAN_DEFAULT_IP);
		os_strcpy(ip4_config.mask, WLAN_DEFAULT_MASK);
		os_strcpy(ip4_config.gateway, WLAN_DEFAULT_GW);
		os_strcpy(ip4_config.dns, WLAN_DEFAULT_GW);
		ret = bk_netif_set_ip4_config(NETIF_IF_AP, &ip4_config);

		os_strcpy(ap_config.ssid, ap_ssid);
		os_strcpy(ap_config.password, ap_key);

		if (ap_channel) {
			int channel;
			char *end;

			channel = strtol(ap_channel, &end, 0);
			if (*end) {
				CLI_LOGE("Invalid number '%s'", ap_channel);
				return;
			}
			ap_config.channel = channel;
		}

		CLI_LOGI("ssid:%s  key:%s\r\n", ap_config.ssid, ap_config.password);
		ap_config.hidden = true;
		ret = bk_wifi_ap_set_config(&ap_config);
		ret = bk_wifi_ap_start();
	}


	if(ret == 0)
		msg = WIFI_CMD_RSP_SUCCEED;
	else
		msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));

}

#if defined(CONFIG_SOFTAP_WPA3)
void cli_wifi_wpa3_ap_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	wifi_ap_config_t ap_config = WIFI_DEFAULT_AP_CONFIG();
	netif_ip4_config_t ip4_config = {0};
	int len;
	char *ap_ssid = NULL;
	char *ap_key = "";
	char *ap_channel = NULL;
	int ret = 0;
	char *msg = NULL;

	if (argc == 2)
			ap_ssid = argv[1];
	else if (argc == 3) {
		ap_ssid = argv[1];
		if (os_strlen(argv[2]) <= 2)
			ap_channel = argv[2];
		else
			ap_key = argv[2];
	} else if (argc == 4) {
		ap_ssid = argv[1];
		ap_key = argv[2];
		ap_channel = argv[3];
	}else{
		CLI_LOGI("Invalid parameters\n");
		return;
	}

	if (ap_ssid) {
		len = os_strlen(ap_ssid);
		if (32 < len) {
			CLI_LOGE("ssid name more than 32 Bytes\r\n");
			return;
		}

		os_strcpy(ip4_config.ip, WLAN_DEFAULT_IP);
		os_strcpy(ip4_config.mask, WLAN_DEFAULT_MASK);
		os_strcpy(ip4_config.gateway, WLAN_DEFAULT_GW);
		os_strcpy(ip4_config.dns, WLAN_DEFAULT_GW);
		ret = bk_netif_set_ip4_config(NETIF_IF_AP, &ip4_config);

		os_strcpy(ap_config.ssid, ap_ssid);
		os_strcpy(ap_config.password, ap_key);

		if (ap_channel) {
			int channel;
			char *end;

			channel = strtol(ap_channel, &end, 0);
			if (*end) {
				CLI_LOGE("Invalid number '%s'", ap_channel);
				return;
			}
			ap_config.channel = channel;
		}

		CLI_LOGI("ssid:%s  key:%s\r\n", ap_config.ssid, ap_config.password);
		ap_config.security= WIFI_SECURITY_WPA3_SAE;
		ret = bk_wifi_ap_set_config(&ap_config);
		ret = bk_wifi_ap_start();
	}


	if(ret == 0)
		msg = WIFI_CMD_RSP_SUCCEED;
	else
		msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));

}
#endif

void cli_wifi_stop_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	char *msg = NULL;

	if (argc == 2) {
		if (os_strcmp(argv[1], "sta") == 0) {
#if defined(CONFIG_ENABLE_WIFI_DEFAULT_CONNECT)
			bk_event_unregister_cb(EVENT_MOD_WIFI, EVENT_WIFI_STA_CONNECTED,
									fast_connect_cb);
#endif
			ret = bk_wifi_sta_stop();
		} else if (os_strcmp(argv[1], "ap") == 0)
			ret = bk_wifi_ap_stop();
		else {
			CLI_LOGI("unknown WiFi interface\n");
			goto error;
		}
	} else {
		CLI_LOGI("bad parameters\r\n");
		goto error;
	}

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

void cli_wifi_iplog_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *iplog_mode = NULL;
	char *iplog_type = NULL;
	int ret = 0;
	char *msg = NULL;

	if (argc == 3)
	{
		iplog_mode = argv[1];
		iplog_type = argv[2];
		if (iplog_mode && iplog_type)
			demo_wifi_iplog_init(iplog_mode, iplog_type);
	}
	else if (argc == 2)
	{
		iplog_mode = argv[1];
		if (iplog_mode)
			demo_wifi_iplog_init(iplog_mode, iplog_type);
	}
	else
	{
		CLI_LOGI("cli_wifi_iplog_cmd:invalid argc num\r\n");
		goto error;
	}

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}
void cli_wifi_ipdbg_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t func = 0;
	uint16_t value = 0;
	int ret = 0;
	char *msg = NULL;

	if (argc == 3)
	{
		func = (uint32_t)os_strtoul(argv[1], NULL, 16);
		value =  (uint16_t)os_strtoul(argv[2], NULL, 10);
		demo_wifi_ipdbg_init(func, value);
	}
	else
	{
		CLI_LOGI("cli_wifi_ipdbg_cmd:invalid argc num\r\n");
		goto error;
	}

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

void cli_wifi_mem_apply_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t value = 0;
	uint8_t module = 0;
	int ret = 0;
	char *msg = NULL;

	if (argc < 3)
	{
		CLI_LOGI("cli_wifi_mem_cmd_cmd:invalid argc num");
		goto error;
	}
	else if(3 == argc)
	{
		module = (uint8_t)(os_strtoul(argv[1], NULL, 16) & 0xFF);
		value =  (uint8_t)os_strtoul(argv[2], NULL, 10);
		demo_wifi_mem_apply_init(module, value);
	}
	else
	{
		CLI_LOGI("cli_wifi_mem_cmd_cmd:invalid argc num\r\n");
		goto error;
	}

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

typedef struct {
	uint8_t channel;
	uint32_t rx_cnt_mgmt;
	uint32_t rx_cnt_data;
	uint32_t rx_cnt_ctrl;
	uint32_t rx_cnt_0_255;
	uint32_t rx_cnt_256_511;
	uint32_t rx_cnt_512_1023;
	uint32_t rx_cnt_1024;
	uint32_t rx_cnt_total;
} cli_monitor_result_t;
static cli_monitor_result_t *s_monitor_result = NULL;

bk_err_t cli_monitor_cb(const uint8_t *data, uint32_t len, const wifi_frame_info_t *info)
{
	if (s_monitor_result) {
		s_monitor_result->rx_cnt_total++;

		if (data) {
			if ((data[0] & 0xc) == 0x8)
				s_monitor_result->rx_cnt_data ++;
			else if ((data[0] & 0xc) == 0x0)
				s_monitor_result->rx_cnt_mgmt ++;
			else
				s_monitor_result->rx_cnt_ctrl ++;
		}

		if (len < 256)
			s_monitor_result->rx_cnt_0_255++;
		else if (len < 512)
			s_monitor_result->rx_cnt_256_511++;
		else if (len < 1024)
			s_monitor_result->rx_cnt_512_1023++;
		else
			s_monitor_result->rx_cnt_1024++;
	}

	return BK_OK;
}

void cli_monitor_show(void)
{
	if (s_monitor_result) {
		BK_LOG_RAW("total:      %u\n", s_monitor_result->rx_cnt_total);
		BK_LOG_RAW("mgmt:       %u\n", s_monitor_result->rx_cnt_mgmt);
		BK_LOG_RAW("data:       %u\n", s_monitor_result->rx_cnt_data);
		BK_LOG_RAW("ctrl:       %u\n", s_monitor_result->rx_cnt_ctrl);
		BK_LOG_RAW("0 - 255:    %u\n", s_monitor_result->rx_cnt_0_255);
		BK_LOG_RAW("256 - 511:  %u\n", s_monitor_result->rx_cnt_256_511);
		BK_LOG_RAW("512 - 1023: %u\n", s_monitor_result->rx_cnt_512_1023);
		BK_LOG_RAW(">=1024:     %u\n", s_monitor_result->rx_cnt_1024);
	}
}

#if defined(CONFIG_RWNX_PROTO_DEBUG)
void cli_wifi_set_proto_debug_flag(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t pd_flag = 0;
	int ret = 0;
	char *msg = NULL;

	if (argc < 2) {
		CLI_LOGI("invalid argc num");
		goto error;
	}

	pd_flag = (uint8_t)os_strtoul(argv[1], NULL, 10);
	if(pd_flag == 1) {
		ret = bk_wifi_enable_proto_debug(pd_flag);

		if (!ret) {
			CLI_LOGI("enable proto debug ok");
			msg = WIFI_CMD_RSP_SUCCEED;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			return;
		}
		else {
			CLI_LOGI("enable proto debug failed");
			goto error;
		}
	} else if(pd_flag == 0){
		ret = bk_wifi_disable_proto_debug(pd_flag);
		if (!ret) {
			CLI_LOGI("disable proto debug ok");
			msg = WIFI_CMD_RSP_SUCCEED;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			return;
		}
		else {
			CLI_LOGI("disable proto debug failed");
			goto error;
		}
	} else {
		CLI_LOGI("invalid argv of pd_flag");
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}
#endif

void cli_wifi_set_arp_rate_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	float arp_tx_rate = 0;
	uint16_t set_arp_tx_rate;
	int ret = 0;
	char *msg = NULL;

	if (argc < 2) {
		CLI_LOGI("invalid argc num");
		goto error;
	}

	arp_tx_rate = (float)os_strtoul(argv[1], NULL, 10);
	set_arp_tx_rate = (uint16_t)(arp_tx_rate * 10);
	ret = bk_wifi_send_arp_set_rate_req(set_arp_tx_rate);

	if (!ret) {
		CLI_LOGI("set_arp_tx_rate ok");
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		CLI_LOGI("set_arp_tx_rate failed");
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

void cli_wifi_set_interval_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t interval = 0;
	int ret = 0;
	char *msg = NULL;

	if (argc < 2) {
		CLI_LOGI("invalid argc num");
		goto error;
	}

	interval = (uint8_t)os_strtoul(argv[1], NULL, 10);
	ret = bk_wifi_send_listen_interval_req(interval);

	if (!ret) {
		CLI_LOGI("set_interval ok");
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		CLI_LOGI("set_interval failed");
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

void cli_wifi_bcn_loss_intv_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t interval = 0;
	uint8_t repeat_num = 0;
	int ret = 0;
	char *msg = NULL;

	if (argc < 3) {
		CLI_LOGI("invalid argc num");
		goto error;
	}

	interval = (uint8_t)os_strtoul(argv[1], NULL, 10);
	repeat_num = (uint8_t)os_strtoul(argv[2], NULL, 10);
	ret = bk_wifi_send_bcn_loss_int_req(interval, repeat_num);

	if (!ret) {
		CLI_LOGI("set_bcn_loss_int ok");
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		CLI_LOGI("set_bcn_loss_int failed");
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

void cli_wifi_set_bcn_loss_time_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t wait_cnt = 0;
	uint8_t wake_cnt = 0;
	int ret = 0;
	char *msg = NULL;

	if (argc < 3) {
		CLI_LOGI("invalid argc num");
		goto error;
	}

	wait_cnt = (uint8_t)os_strtoul(argv[1], NULL, 10);
	wake_cnt = (uint8_t)os_strtoul(argv[2], NULL, 10);
	ret = bk_wifi_set_bcn_loss_time(wait_cnt, wake_cnt);

	if (!ret) {
		CLI_LOGI("set_bcn_loss_time ok");
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		CLI_LOGI("set_bcn_loss_time failed");
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

void cli_wifi_set_bcn_recv_win_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t default_win = 0;
	uint8_t max_win = 0;
	uint8_t step = 0;
	int ret = 0;
	char *msg = NULL;

	if (argc < 4) {
		CLI_LOGI("invalid argc num");
		goto error;
	}

	default_win = (uint8_t)os_strtoul(argv[1], NULL, 10);
	max_win = (uint8_t)os_strtoul(argv[2], NULL, 10);
	step = (uint8_t)os_strtoul(argv[3], NULL, 10);
	ret = bk_wifi_set_bcn_recv_win(default_win, max_win, step);

	if (!ret) {
		CLI_LOGI("set_bcn_recv_win ok");
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		CLI_LOGI("set_bcn_recv_win failed");
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

void cli_wifi_set_bcn_miss_time_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t bcnmiss_time = 0;
	int ret = 0;
	char *msg = NULL;

	if (argc < 2) {
		CLI_LOGI("invalid argc num");
		goto error;
	}

	bcnmiss_time = (uint8_t)os_strtoul(argv[1], NULL, 10);
	ret = bk_wifi_set_bcn_miss_time(bcnmiss_time);

	if (!ret) {
		CLI_LOGI("set_bcn_miss_time ok");
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		CLI_LOGI("set_bcn_miss_time failed");
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

void cli_monitor_stop(void)
{
	if (s_monitor_result) {
		os_free(s_monitor_result);
		s_monitor_result = NULL;
	}

	BK_LOG_ON_ERR(bk_wifi_monitor_stop());
}

void cli_monitor_start(uint32_t primary_channel)
{
	wifi_channel_t chan = {0};

	chan.primary = primary_channel;

	if (!s_monitor_result) {
		s_monitor_result = os_zalloc(sizeof(cli_monitor_result_t));
		if (!s_monitor_result)
			CLI_LOGI("failed to alloc monitor result\n");
	}

	BK_LOG_ON_ERR(bk_wifi_monitor_register_cb(cli_monitor_cb));
	BK_LOG_ON_ERR(bk_wifi_monitor_start());
	BK_LOG_ON_ERR(bk_wifi_monitor_set_channel(&chan));
}

void cli_wifi_monitor_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t primary_channel;
	int ret = 0;
	char *msg = NULL;

	if (argc != 2) {
		CLI_LOGI("monitor_parameter invalid\r\n");
		goto error;
	}

	primary_channel = os_strtoul(argv[1], NULL, 10);
	if (99 == primary_channel)
		cli_monitor_stop();
	else if ((primary_channel > 0) && (primary_channel < 15))
		cli_monitor_start(primary_channel);
	else
		cli_monitor_show();

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

void cli_wifi_sta_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *ssid = NULL;
	char *password = "";
#ifdef CONFIG_CONNECT_THROUGH_PSK_OR_SAE_PASSWORD
	u8 *psk = 0;
#endif
	char *msg = NULL;
	int ret;

	if ((argc < 2) || (argc > 6)) {
		CLI_LOGI("invalid argc number\n");
		goto error;
	}

	int err = rtos_init_semaphore(&wifi_cmd_sema, 1);
	if (err != kNoErr) {
		goto error;
	}

#ifdef CONFIG_BSSID_CONNECT
	uint8_t bssid[6] = {0};
	if (os_strcmp(argv[1], "bssid") == 0) {
		if(argc >= 3) {
			hexstr2bin_cli(argv[2], bssid, 6);
		}
		if(argc >= 4) {
			password = argv[3];
		}
		demo_sta_bssid_app_init(bssid, password);
		if (wifi_cmd_sema != NULL)
		{
			err = rtos_get_semaphore(&wifi_cmd_sema, 10000);
			if (err != kNoErr)
			{
				goto error;
			}
			else
			{
				if (wifi_cmd_status == 1)
				{
					msg = WIFI_EVT_GOT_IP;
					os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
					rtos_deinit_semaphore(&wifi_cmd_sema);
					wifi_cmd_status = 0;
					return;
				}
				else
				{
					goto error;
				}
			}
		}

	}
#endif

#if defined(CONFIG_ENABLE_WIFI_DEFAULT_CONNECT)
	if (wifi_cli_find_id(argc, argv, "-w") > 0 ||
		wifi_cli_find_id(argc, argv, "-e") > 0) {
		if (argc >= 2)
			ssid = argv[2];

		if (argc >= 3)
			password = argv[3];
	} else {
#endif
		if (argc >= 2)
			ssid = argv[1];

		if (argc >= 3)
			password = argv[2];

#ifdef CONFIG_CONNECT_THROUGH_PSK_OR_SAE_PASSWORD
		if (argc >= 4) {
			psk = (u8 *)argv[2];
			password = argv[3];
		}
#endif
#if defined(CONFIG_ENABLE_WIFI_DEFAULT_CONNECT)
	}
#endif

#if 0 //TODO support BSSID/Channel configuration
	if (argc >= 4)
		bssid = argv[3];

	if (argc >= 5)
		channel = argv[4];
#endif

	char *oob_ssid_tp = ssid;
#if defined(CONFIG_USE_CONV_UTF8)
	oob_ssid_tp = (char *)conv_utf8((uint8_t *)ssid);
#endif

	if (oob_ssid_tp) {
#ifdef CONFIG_CONNECT_THROUGH_PSK_OR_SAE_PASSWORD
	ret = demo_sta_app_init((char *)oob_ssid_tp, psk, password);
#else
	ret = demo_sta_app_init((char *)oob_ssid_tp, password);
#endif
	if (ret == -1)
		goto error;

#if defined(CONFIG_ENABLE_WIFI_DEFAULT_CONNECT)
		if (wifi_cli_find_id(argc, argv, "-w") > 0) {
			bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_USR_CONFIG);
			bk_flash_read_bytes(pt->partition_start_addr + pt->partition_length -4096,
						(uint8_t *)&info_t, sizeof(BK_FAST_CONNECT_T));
			if ((info_t.flag & 0xf0l) == 0x70l)
				info_t.flag |= 0x1l;
			else
				info_t.flag = 0x71l;
			os_strcpy((char *)info_t.sta_ssid, (char *)oob_ssid_tp);
			os_strcpy((char *)info_t.sta_pwd, password);
			bk_event_register_cb(EVENT_MOD_WIFI, EVENT_WIFI_STA_CONNECTED,
									fast_connect_cb, &info_t);
		} else if (wifi_cli_find_id(argc, argv, "-e") > 0) {
			bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_USR_CONFIG);

			bk_flash_read_bytes(pt->partition_start_addr + pt->partition_length -4096,
						(uint8_t *)&info_t, sizeof(BK_FAST_CONNECT_T));
			if (info_t.flag == 0x71l || info_t.flag == 0x73l) {
				info_t.flag &= ~0x1l;
				bk_flash_set_protect_type(FLASH_PROTECT_NONE);
				bk_flash_write_bytes(pt->partition_start_addr + pt->partition_length -4096,
									(uint8_t *)&info_t, sizeof(BK_FAST_CONNECT_T));
				bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
			}
		}
#endif
#if defined(CONFIG_USE_CONV_UTF8)
		os_free(oob_ssid_tp);
#endif

#if !defined(CONFIG_SOC_BK7236XX) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
		if (wifi_cmd_sema != NULL)
		{
			err = rtos_get_semaphore(&wifi_cmd_sema, 10000);
			if (err != kNoErr)
			{
				goto error;
			}
			else
			{
				if (wifi_cmd_status == 1)
				{
					msg = WIFI_EVT_GOT_IP;
					os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
					rtos_deinit_semaphore(&wifi_cmd_sema);
					wifi_cmd_status = 0;
					return;
				}
				else
				{
					goto error;
				}
			}
		}
#endif
	} else {
		CLI_LOGI("not buf for utf8\r\n");
		goto error;
	}

	goto exit;

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
exit:
	if (wifi_cmd_sema != NULL)
		rtos_deinit_semaphore(&wifi_cmd_sema);
	return;
}

#if defined(CONFIG_AP_STATYPE_LIMIT)
void cli_wifi_sta_vsie_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *ssid = NULL;
	char *password = "";
	BK_STA_TYPE sta_type = STA_TYPE1;
	wifi_sta_config_t sta_config = {"0"};

	if (argc >= 2)
			ssid = argv[1];

	if (argc == 3 || argc == 4) {
		if (os_strlen(argv[2]) <= 1) {
			sta_type = *argv[2] - '0';
			u8 mac[6];
			sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ] = (struct bk_vise *)os_zalloc(sizeof(struct bk_vise));
			/*set EID*/
			sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf[0] = WLAN_EID_VENDOR_SPECIFIC;
			sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf[1] = 10;
			/*set OUI*/
			//os_memcpy(sta_config.vsies[sta_type].buf, OUI_BEKEN);
			sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf[2] = (OUI_BEKEN >> 16) & 0xff;
			sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf[3] = (OUI_BEKEN >> 8) & 0xff;
			sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf[4] = OUI_BEKEN & 0xff;
			/*set payload*/
			bk_wifi_sta_get_mac(mac);
			os_memcpy(sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf+5, mac, WIFI_MAC_LEN);
			sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf[11] = sta_type;
			sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->len = 12;
		} else {
			password = argv[2];
			if (argc == 4)
				sta_type = *argv[3] - '0';
			else sta_type = 0;

				u8 mac[6];
				sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ] = (struct bk_vise *)os_zalloc(sizeof(struct bk_vise));
				/*set EID*/
				sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf[0] = WLAN_EID_VENDOR_SPECIFIC;
				sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf[1] = 10;
				/*set OUI*/
				//os_memcpy(sta_config.vsies[sta_type].buf, OUI_BEKEN);
				sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf[2] = (OUI_BEKEN >> 16) & 0xff;
				sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf[3] = (OUI_BEKEN >> 8) & 0xff;
				sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf[4] = OUI_BEKEN & 0xff;
				/*set payload*/
				bk_wifi_sta_get_mac(mac);
				os_memcpy(sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf+5, mac, WIFI_MAC_LEN);
				sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->buf[11] = sta_type;
				sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]->len = 12;
		}
	}

	strncpy(sta_config.ssid, ssid, WIFI_SSID_STR_LEN);
	strncpy(sta_config.password, password, WIFI_PASSWORD_LEN);

	BK_LOGI(TAG, "ssid:%s password:%s\n", sta_config.ssid, sta_config.password);
	BK_LOG_ON_ERR(bk_wifi_sta_set_config(&sta_config));
	BK_LOG_ON_ERR(bk_wifi_sta_start());
	if (sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ])
		os_free(sta_config.vsies[WIFI_VENDOR_ELEM_ASSOC_REQ]);
}

void cli_wifi_ap_vsie_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	wifi_ap_config_t ap_config = {"0"};
	netif_ip4_config_t ip4_config = {0};
	char *ap_ssid = NULL;
	char *ap_key = "";
	int i;

	ap_ssid = argv[1];
	if (argc >= 3) {
		if (os_strlen(argv[2]) <= 1) {
			i = 0;
		} else {
			i = 1;
			ap_key = argv[2];
		}
		switch (argc - i - 2) {
			case 4:
				ap_config.max_statype_num[3] = *argv[i+5] - '0';
			case 3:
				ap_config.max_statype_num[2] = *argv[i+4] - '0';
			case 2:
				ap_config.max_statype_num[1] = *argv[i+3] - '0';
			case 1:
				ap_config.max_statype_num[0] = *argv[i+2] - '0';
			default:
				break;
		}
	}

	strncpy(ip4_config.ip, "192.168.0.1", NETIF_IP4_STR_LEN);
	strncpy(ip4_config.mask, "255.255.255.0", NETIF_IP4_STR_LEN);
	strncpy(ip4_config.gateway, "192.168.0.1", NETIF_IP4_STR_LEN);
	strncpy(ip4_config.dns, "0.0.0.0", NETIF_IP4_STR_LEN);
	BK_LOG_ON_ERR(bk_netif_set_ip4_config(NETIF_IF_AP, &ip4_config));

	strncpy(ap_config.ssid, ap_ssid, WIFI_SSID_STR_LEN);
	strncpy(ap_config.password, ap_key, WIFI_PASSWORD_LEN);

	BK_LOGI(TAG, "ssid:%s  key:%s\r\n", ap_config.ssid, ap_config.password);
	BK_LOG_ON_ERR(bk_wifi_ap_set_config(&ap_config));
	BK_LOG_ON_ERR(bk_wifi_ap_start());

}
#endif

#if defined(CONFIG_COMPONENTS_WPA2_ENTERPRISE)
/**
 * cli command: sta_eap <ssid>, connect to EAP-TLS AP.
 *
 * restrictions: EAP-TLS is based on PKI, both AP and STA may have certificate. So
 * we must install certificate and private key to system. For example, `beken-iot-1.pem'
 * is STA's certificate, `beken-iot-1.key' is private key, `rootca.pem' is the RootCA.
 * These certificates and private key may be registered to system via `register_xfile'
 * function.
 */
void cli_wifi_sta_eap_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *ssid = NULL;
	char *ca = "cacert.pem";
	char *client_cert = "beken-iot-1.pem";
	char *private_key = "beken-iot-1.key";
	char *private_key_passwd = "12345678";
	char *identity = "user";
	int ret = 0;
	int err = 0;
	char *msg = NULL;

	if ((argc < 2) || (argc > 5)) {
		CLI_LOGI("invalid argc number\n");
		goto error;
	}

	ssid = argv[1];

	char *oob_ssid_tp = ssid;
#if defined(CONFIG_USE_CONV_UTF8)
	oob_ssid_tp = (char *)conv_utf8((uint8_t *)ssid);
#endif

	if (oob_ssid_tp) {
		int len;
		wifi_sta_config_t *sta_config;

		len = os_strlen((char *)oob_ssid_tp);
		if (WLAN_SSID_MAX_LEN < len) {
			CLI_LOGI("ssid name more than 32 Bytes\n");
			goto error;
		}

		sta_config = os_zalloc(sizeof(*sta_config));
		if (!sta_config) {
			CLI_LOGI("Cannot alloc STA config\n");
			goto error;
		}

		os_strlcpy(sta_config->ssid, oob_ssid_tp, sizeof(sta_config->ssid));
		sta_config->password[0] = '\0';	// No passwd needed fo EAP.
		os_strlcpy(sta_config->eap, "TLS", sizeof(sta_config->eap));
		os_strlcpy(sta_config->identity, identity, sizeof(sta_config->identity));
		os_strlcpy(sta_config->ca, ca, sizeof(sta_config->ca));
		os_strlcpy(sta_config->client_cert, client_cert, sizeof(sta_config->client_cert));
		os_strlcpy(sta_config->private_key, private_key, sizeof(sta_config->private_key));
		os_strlcpy(sta_config->private_key_passwd, private_key_passwd, sizeof(sta_config->private_key_passwd));
		os_strlcpy(sta_config->phase1, "tls_disable_time_checks=1", sizeof(sta_config->phase1));

		CLI_LOGI("ssid:%s key:%s\n", sta_config->ssid, sta_config->password);
		CLI_LOGI("eap:%s identity:%s\n", sta_config->eap, sta_config->identity);
		CLI_LOGI("ca:%s client_cert:%s\n", sta_config->ca, sta_config->client_cert);
		CLI_LOGI("private_key:%s\n", sta_config->private_key);
		ret = bk_wifi_sta_set_config(sta_config);
		err = bk_wifi_sta_start();

		os_free(sta_config);

#if defined(CONFIG_USE_CONV_UTF8)
		os_free(oob_ssid_tp);
#endif
	} else {
		CLI_LOGI("not buf for utf8\r\n");
		goto error;
	}

	if (!ret && !err) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}
#endif

void cli_wifi_state_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	char *msg = NULL;

	ret = demo_state_app_init();

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}

}

#if defined(CONFIG_WIFI_SENSOR)
static void cli_wifi_sensor_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int status;
	int ret = 0;
	char *msg = NULL;

	if (argc != 2) {
		bk_printf("param error");
		goto error;
	}

	if (os_strcmp(argv[1], "start") == 0)
		bk_wifi_detect_movement_start();

	if (os_strcmp(argv[1], "stop") == 0)
		bk_wifi_detect_movement_stop();

	if (os_strcmp(argv[1], "status") == 0) {
		status = bk_get_movement_status();

		if (status == 0) {
			bk_printf("detect something");
		}
		else {
			bk_printf("detect nothing");
		}
	}

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}
#endif

#if defined(CONFIG_COMPONENTS_WFA_CA)
extern void wfa_ca_start();
extern void wfa_ca_stop();

void cli_wifi_wfa_ca_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	char *msg = NULL;

	if (argc != 2) {
		CLI_LOGE("param error");
		goto error;
	}

	if (os_strcmp(argv[1], "start") == 0)
		wfa_ca_start();
	else if (os_strcmp(argv[1], "stop") == 0)
		wfa_ca_stop();

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}
#endif

typedef struct {
	uint16_t cnt_probe_req;
	uint16_t cnt_probe_rsp;
	uint16_t cnt_beacon;
	uint16_t cnt_action;
	uint16_t cnt_auth;
	uint16_t cnt_assoc_req;
	uint16_t cnt_assoc_rsp;
	uint16_t cnt_others;
	uint16_t cnt_total;
} wifi_filter_result_t;

static wifi_filter_result_t *s_filter_result = NULL;

static int wifi_filter_cb(const uint8_t *data, uint32_t len, const wifi_frame_info_t *frame_info)
{
	if (!data) {
		CLI_LOGE("null data\n");
		return BK_OK;
	}

	if (!s_filter_result)
		return BK_OK;

	uint16_t framectrl = *(uint16_t*)(data);
	uint16_t type_subtype = framectrl & MAC_FCTRL_TYPESUBTYPE_MASK;

	if (type_subtype == MAC_FCTRL_BEACON)
		s_filter_result->cnt_beacon ++;
	else if (type_subtype == MAC_FCTRL_PROBEREQ)
		s_filter_result->cnt_probe_req++;
	else if (type_subtype == MAC_FCTRL_PROBERSP)
		s_filter_result->cnt_probe_rsp++;
	else if (type_subtype == MAC_FCTRL_ACTION)
		s_filter_result->cnt_action++;
	else if (type_subtype == MAC_FCTRL_AUTHENT)
		s_filter_result->cnt_auth++;
	else if (type_subtype == MAC_FCTRL_ASSOCREQ)
		s_filter_result->cnt_assoc_req++;
	else if (type_subtype == MAC_FCTRL_ASSOCRSP)
		s_filter_result->cnt_assoc_rsp++;
	else
		s_filter_result->cnt_others++;

	s_filter_result->cnt_total++;
	return BK_OK;
}

static void wifi_filter_result_dump(void)
{
	if (!s_filter_result)
		return;

	BK_LOG_RAW("filter result:\n");
	BK_LOG_RAW("total: %u\n", s_filter_result->cnt_total);
	BK_LOG_RAW("beacon: %u\n", s_filter_result->cnt_beacon);
	BK_LOG_RAW("probe req: %u\n", s_filter_result->cnt_probe_req);
	BK_LOG_RAW("probe rsp: %u\n", s_filter_result->cnt_probe_rsp);
	BK_LOG_RAW("auth: %u\n", s_filter_result->cnt_auth);
	BK_LOG_RAW("assoc req: %u\n", s_filter_result->cnt_assoc_req);
	BK_LOG_RAW("assoc rsp: %u\n", s_filter_result->cnt_assoc_rsp);
	BK_LOG_RAW("action: %u\n", s_filter_result->cnt_action);
	BK_LOG_RAW("others: %u\n", s_filter_result->cnt_others);
}

static void wifi_mgmt_filter_help(void)
{
	BK_LOG_RAW("filter {filter_bitmap}\n");
	BK_LOG_RAW("    bit0 - default management\n");
	BK_LOG_RAW("    bit1 - probe req\n");
	BK_LOG_RAW("    bit2 - probe rsp\n");
	BK_LOG_RAW("    bit3 - all beacon\n");
	BK_LOG_RAW("    bit4 - action\n");
	BK_LOG_RAW("       0 - stop filter\n");
	BK_LOG_RAW("      -1 - display result\n");
}

static void cli_wifi_filter_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	wifi_filter_config_t filter_config = {0};
	uint32_t filter = 0;
	int ret = 0;
	int err = 0;
	char *msg = NULL;
	if (argc != 2) {
		wifi_mgmt_filter_help();
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}

	filter = os_strtoul(argv[1], NULL, 0);

	if (filter == 0) {
		if (s_filter_result) {
			os_free(s_filter_result);
			s_filter_result = NULL;
		}
		ret = bk_wifi_filter_set_config(&filter_config);
		err = bk_wifi_filter_register_cb(NULL);
		if (!ret && !err) {
			msg = WIFI_CMD_RSP_SUCCEED;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			return;
		}
		else {
			goto error;
		}
	} else if (filter == -1) {
		wifi_filter_result_dump();
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}

	if (!s_filter_result) {
		s_filter_result = (wifi_filter_result_t *)os_zalloc(sizeof(wifi_filter_result_t));
		if (!s_filter_result) {
			goto error;
		}
	}

	if (filter & (1 << 0))
		filter_config.rx_all_default_mgmt = 1;

	if (filter & (1 << 1))
		filter_config.rx_probe_req = 1;

	if (filter & (1 << 2))
		filter_config.rx_probe_rsp = 1;

	if (filter & (1 << 3))
		filter_config.rx_all_beacon = 1;

	if (filter & (1 << 4))
		filter_config.rx_action = 1;

	ret = bk_wifi_filter_set_config(&filter_config);
	err = bk_wifi_filter_register_cb(wifi_filter_cb);
	if (!ret && !err) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

#if defined(CONFIG_WIFI_RAW_TX_TEST)

typedef struct {
	uint32_t interval;
	uint32_t counter;
} wifi_raw_tx_param_t;

static void wifi_raw_tx_thread(void *arg)
{
	char frame[] = {
		0xB0, //version, type, subtype
		0x00, //frame control
		0x3A, 0x01, //duration
		0xC8, 0x47, 0x8C, 0x42, 0x00, 0x48, //Address1 - destination
		0x4C, 0xD1, 0xA1, 0xC5, 0x38, 0xE4, //Address2 - source
		0x4C, 0xD1, 0xA1, 0xC5, 0x38, 0xE4, //Address3 - bssid
		0x20, 0xC0, //sequence

		//Auth Response
		0x00, 0x00, //Auth algorithm - open system
		0x02, 0x00, //Auth seq num
		0x00, 0x00, //Status code
	};
	wifi_raw_tx_param_t *tx_param;
	int ret;

	tx_param = (wifi_raw_tx_param_t *)arg;
	CLI_LOGI("wifi raw tx begin, interval=%u counter=%d\n", tx_param->interval,
			 tx_param->counter);

	for (uint32_t i = 0; i < tx_param->counter; i++) {
		ret = bk_wlan_send_80211_raw_frame((unsigned char *)frame, sizeof(frame));
		if (ret != kNoErr)
			CLI_LOGI("raw tx error, ret=%d\n", ret);

		rtos_delay_milliseconds(tx_param->interval);
	}

	os_free(arg);
	CLI_LOGI("wifi raw tx end\n");
	rtos_delete_thread(NULL);
}

static void cli_wifi_raw_tx_cmd(char *pcWriteBuffer, int xWriteBufferLen,
								int argc, char **argv)
{
	bk_err_t ret;
	char *msg = NULL;
	if (argc != 3) {
		CLI_LOGE("param error");
		CLI_LOGI("usage: wifi_raw_tx interval counter");
		goto error;
	}

	wifi_raw_tx_param_t *tx_param;
	tx_param = (wifi_raw_tx_param_t *)os_malloc(sizeof(wifi_raw_tx_param_t));
	if (!tx_param) {
		CLI_LOGE("out of memory\n");
		goto error;
	}

	tx_param->interval = os_strtoul(argv[1], NULL, 10);
	tx_param->counter = os_strtoul(argv[2], NULL, 10);
	ret = rtos_create_thread(NULL, 2, "raw_tx",
		(beken_thread_function_t)wifi_raw_tx_thread,
		2048, tx_param);
	if (kNoErr != ret) {
		os_free(tx_param);
		CLI_LOGI("Create raw tx thread failed, ret=%d\r\n", ret);
		goto error;
	}
	else {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}
#endif

static void cli_wifi_monitor_channel_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	wifi_channel_t chan = {0};
	int channel, i = 0;
	int ret = 0;
	char *msg = NULL;

	if (argc == 1) {
		CLI_LOGI("Usage: channel [1~13].");
		goto error;
	}

	while (argv[1][i]) {
		if ((argv[1][i] < '0') || (argv[1][i] > '9')) {
			CLI_LOGE("parameter should be a number\r\n");
			goto error ;
		}
		i++;
	}

	channel = atoi(argv[1]);

	if ((channel < 1) || (channel > 13)) {
		CLI_LOGE("Invalid channel number \r\n");
		goto error;
	}
	BK_LOG_RAW("monitor mode, set to channel %d\r\n", channel);
	chan.primary = channel;
	ret = bk_wifi_monitor_set_channel(&chan);

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

int cli_netif_event_cb(void *arg, event_module_t event_module,
					   int event_id, void *event_data)
{
	netif_event_got_ip4_t *got_ip;

	switch (event_id) {
	case EVENT_NETIF_GOT_IP4:
#if defined(CONFIG_BRIDGE) && !defined(CONFIG_CLI_CODE_SIZE_OPTIMIZE_ENABLE)
		if ((!bridge_is_enabled) && bridge_open) {
			uint8_t mac[6] = {0};
			bridgeif_initdata_t mybr_initdata = {0};
			netif_ip4_config_t ip4_config = {0};
			ip4_addr_t my_ip, my_gw, my_mask;
			wifi_link_status_t link_status = {0};

			bridge_is_enabled = 1;
			/*confige bridgeif and add sta to bridgeif*/
			bk_wifi_sta_get_mac(mac);
			os_memcpy(((struct netif *)net_get_br_handle())->hwaddr, mac, 6);
			os_memcpy(&mybr_initdata.ethaddr, mac, 6);
			mybr_initdata.max_fdb_dynamic_entries = 64;
			mybr_initdata.max_fdb_static_entries = 4;
			mybr_initdata.max_ports = 16;
			bk_netif_get_ip4_config(NETIF_IF_STA, &ip4_config);
			inet_aton((char *)&ip4_config.ip, &my_ip);
			inet_aton((char *)&ip4_config.gateway, &my_gw);
			inet_aton((char *)&ip4_config.mask, &my_mask);

			netifapi_netif_add((struct netif *)net_get_br_handle(), &my_ip, &my_mask, &my_gw,
								&mybr_initdata, bridgeif_init, netif_input);
			bridgeif_add_port((struct netif *)net_get_br_handle(), (struct netif *)net_get_sta_handle());
			netif_set_hostname((struct netif *)net_get_sta_handle(), "LGE");

			bk_wifi_sta_get_link_status(&link_status);
			if (link_status.security == WIFI_SECURITY_NONE)
				demo_bridge_softap_init(br_ssid, NULL, link_status.security);
			else
				demo_bridge_softap_init(br_ssid, br_pwd, link_status.security);
			bridgeif_add_port((struct netif *)net_get_br_handle(), (struct netif *)net_get_uap_handle());
			netifapi_netif_set_default(net_get_br_handle());
			netifapi_netif_set_up((struct netif *)net_get_br_handle());
			bridge_set_ip_start_flag(true);
		}
#endif
	case EVENT_NETIF_GOT_IP6:
		if (wifi_cmd_sema != NULL) {
			wifi_cmd_status = 1;
			rtos_set_semaphore(&wifi_cmd_sema);
		}
		got_ip = (netif_event_got_ip4_t *)event_data;
		CLI_LOGW("%s got ip%d\n", got_ip->netif_if == NETIF_IF_STA ? "BK STA" : "unknown netif",
			event_id== EVENT_NETIF_GOT_IP4 ? 4 : 6);
		break;
	case EVENT_NETIF_DHCP_TIMEOUT:
		CLI_LOGW("dhcp timeout, still retrying\r\n");
		break;
	default:
		CLI_LOGI("rx event <%d %d>\n", event_module, event_id);
		break;
	}

	return BK_OK;
}

int cli_wifi_event_cb(void *arg, event_module_t event_module,
					  int event_id, void *event_data)
{
	wifi_event_sta_disconnected_t *sta_disconnected;
	wifi_event_sta_connected_t *sta_connected;
	wifi_event_ap_disconnected_t *ap_disconnected;
	wifi_event_ap_connected_t *ap_connected;
	wifi_event_network_found_t *network_found;

	switch (event_id) {
	case EVENT_WIFI_STA_CONNECTED:
		sta_connected = (wifi_event_sta_connected_t *)event_data;
		CLI_LOGI("BK STA connected %s\n", sta_connected->ssid);
		break;

	case EVENT_WIFI_STA_DISCONNECTED:
		sta_disconnected = (wifi_event_sta_disconnected_t *)event_data;
		CLI_LOGW("BK STA disconnected, reason(%d)%s\n", sta_disconnected->disconnect_reason,
			sta_disconnected->local_generated ? ", local_generated" : "");
#if defined(CONFIG_BRIDGE) && !defined(CONFIG_CLI_CODE_SIZE_OPTIMIZE_ENABLE)
		if (bridge_is_enabled && bridge_open) {
			bridge_is_enabled = 0;
			bridge_ip_stop();
			/*just stop softap, station still work*/
			netifapi_netif_set_default(net_get_sta_handle());
			bridge_set_ip_start_flag(false);

			bk_wifi_ap_stop();
		}
#endif
		break;

	case EVENT_WIFI_AP_CONNECTED:
		ap_connected = (wifi_event_ap_connected_t *)event_data;
		CLI_LOGI(BK_MAC_FORMAT" connected to BK AP\n", BK_MAC_STR(ap_connected->mac));
		break;

	case EVENT_WIFI_AP_DISCONNECTED:
		ap_disconnected = (wifi_event_ap_disconnected_t *)event_data;
		CLI_LOGI(BK_MAC_FORMAT" disconnected from BK AP\n", BK_MAC_STR(ap_disconnected->mac));
		break;

	case EVENT_WIFI_NETWORK_FOUND:
		network_found = (wifi_event_network_found_t *)event_data;
		CLI_LOGI(" target AP: %s, bssid %pm found\n", network_found->ssid, network_found->bssid);
		break;

#if CONFIG_WIFI_REGDOMAIN
	case EVENT_WIFI_REGDOMAIN_CHANGED: {
		wifi_event_reg_change_t *event = event_data;

		bk_printf("Regulatory domain changed to %c%c\n", event->alpha2[0], event->alpha2[1]);
	}	break;
#endif

	default:
		CLI_LOGI("rx event <%d %d>\n", event_module, event_id);
		break;
	}

	return BK_OK;
}

void cli_wifi_net_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char buf[128];
	int i, left = sizeof(buf) - 1, len = 0;
	int ret = 0;
	char *msg = NULL;
	// net sta xxx
	// net ap xxx
	if (argc <= 2) {
		CLI_LOGI("Usage: net sta/ap <param...>\n");
		goto error;
	}

	buf[0] = 0;
	for (i = 2; i < argc; i++) {
		len = os_strlen(buf);
		snprintf(buf + len, left - len, "%s ", argv[i]);
	}
	buf[strlen(buf) - 1] = 0;
	//CLI_LOGI("CMD: |%s|\n", buf);

#if 1
	if (os_strcmp(argv[1], "sta") == 0)
		ret = cmd_wlan_sta_exec(buf);
	else if (os_strcmp(argv[1], "ap") == 0)
		ret = cmd_wlan_ap_exec(buf);
#if defined(CONFIG_P2P)
	else if (os_strcmp(argv[1], "p2p") == 0)
		ret = cmd_wlan_p2p_exec(buf);
#endif
	else {
		CLI_LOGI("Usage: net sta/ap <param...>\n");
		goto error;
	}
#endif

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

void cli_wifi_get_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	char *msg = NULL;
	// get pm status
	// get xx status
	if (argc <= 2) {
		CLI_LOGI("Usage get xx status\n");
		goto error;
	}

	if(os_strcmp(argv[1], "ps") == 0) {
		int state = 0;
		if(os_strcmp(argv[2], "status") == 0) {
			state = cmd_wlan_get_ps_status();
			CLI_LOGI("ps status: %s \n", (state?"sleep":"active"));
		} else {
			CLI_LOGI("Usage get ps status\n");
			goto error;
		}
	}
	else if (os_strcmp(argv[1], "mac_trx") == 0) {

		bool reset_status = false;

		if ((argc == 4) && (os_strcmp(argv[3], "-r") == 0))
		{
			reset_status = true;
		}

		if(os_strcmp(argv[2], "status") == 0) {
			bk_wifi_get_mac_trx_status(reset_status);
		} else {
			CLI_LOGI("Usage get MAC TRX status\n");
			goto error;
		}
	}
	else if (os_strcmp(argv[1], "wifi") == 0) {

		if(os_strcmp(argv[2], "config") == 0) {
			CLI_LOGI("wifi config: \n");
			bk_wifi_get_wifi_config();
		} else {
			CLI_LOGI("Usage get ps status\n");
			goto error;
		}
	}

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

#if defined(CONFIG_WIFI_CLI_RAW_LINK_ENABLE)
void cli_rlk_cfg_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t rlk_cfg = 0;
	int ret = 0;
	char *msg = NULL;

	if (argc <= 2) {
		CLI_LOGI("invalid RLK command\n");
		goto error;
	}

	if(os_strcmp(argv[1], "rate") == 0) {
		rlk_cfg = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		bk_rlk_set_tx_rate(rlk_cfg);
	}
	else if(os_strcmp(argv[1], "power") == 0) {
		rlk_cfg = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		bk_rlk_set_tx_power(rlk_cfg);
	}
	else if(os_strcmp(argv[1], "ac") == 0) {
		rlk_cfg = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		bk_rlk_set_tx_ac(rlk_cfg);
	}
	else if(os_strcmp(argv[1], "timeout") == 0) {
		rlk_cfg = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		bk_rlk_set_tx_timeout_ms(rlk_cfg);
	}
	else {
		CLI_LOGI("invalid RLK paramter\n");
		goto error;
	}

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}
#endif

static void wifi_mgmt_rc_help(void)
{
	bk_printf("rc {param1} {param2} ...\n");
	bk_printf("param1: fix/rssi_of/media/max_rc \n");
	bk_printf("param1: /idx_min/he_gi/ht_gi/prb_type \n");
	bk_printf("param1: /tx_cnt/max_tot_ms/wait_num\n");
}

void cli_wifi_rc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t sta_idx = 0;
	uint16_t rate_cfg = 0;
	int ret = 0;
	char *msg = NULL;

	if (argc <= 2) {
		wifi_mgmt_rc_help();
		CLI_LOGI("invalid RC command\n");
		goto error;
	}

	if(os_strcmp(argv[1], "fix") == 0) {
		sta_idx = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		rate_cfg = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
		CLI_LOGI("rc fix: %x\n",rate_cfg);
		bk_wifi_rc_config(sta_idx, rate_cfg);
	}
	else if (os_strcmp(argv[1], "rssi_of") == 0) {
			CLI_LOGI("rc rssi offset: %d\n",os_strtoul(argv[2], NULL, 10));
			bk_wifi_set_rc_rssi_offset(os_strtoul(argv[2], NULL, 10));
	}
	else if (os_strcmp(argv[1], "media") == 0) {
			CLI_LOGI("rc media: %d\n",os_strtoul(argv[2], NULL, 10));
			bk_wifi_set_wifi_media_mode(os_strtoul(argv[2], NULL, 10));
	}
	else if (os_strcmp(argv[1], "max_rc") == 0) {
			CLI_LOGI("rc max_rc: %d\n",os_strtoul(argv[2], NULL, 10));
			bk_wifi_set_video_quality(os_strtoul(argv[2], NULL, 10));
	}
	else if (os_strcmp(argv[1], "idx_min") == 0) {
			CLI_LOGI("rc idx_min: %d\n",os_strtoul(argv[2], NULL, 10));
			bk_wifi_set_idx_min(os_strtoul(argv[2], NULL, 10));
	}
	else if (os_strcmp(argv[1], "he_gi") == 0) {
			CLI_LOGI("rc he_gi: %d\n",os_strtoul(argv[2], NULL, 10));
			bk_wifi_rc_set_he_gi(os_strtoul(argv[2], NULL, 10));
	}
	else if (os_strcmp(argv[1], "ht_gi") == 0) {
			CLI_LOGI("rc ht_gi: %d\n",os_strtoul(argv[2], NULL, 10));
			bk_wifi_rc_set_ht_gi(os_strtoul(argv[2], NULL, 10));
	}
	else if (os_strcmp(argv[1], "prb_type") == 0) {
			CLI_LOGI("rc prb_type: %d\n",os_strtoul(argv[2], NULL, 10));
			bk_wifi_rc_set_type(os_strtoul(argv[2], NULL, 10));
	}
	else if (os_strcmp(argv[1], "tx_cnt") == 0) {
			CLI_LOGI("rc tx_cnt: %d\n",os_strtoul(argv[2], NULL, 10));
			bk_wifi_set_stats_tx_count(os_strtoul(argv[2], NULL, 10));
	}
	else if (os_strcmp(argv[1], "max_tot_ms") == 0) {
			CLI_LOGI("rc max_tot_ms: %d\n",os_strtoul(argv[2], NULL, 10));
			bk_wifi_set_stats_max_tot(os_strtoul(argv[2], NULL, 10));
	}
	else if (os_strcmp(argv[1], "wait_num") == 0) {
			CLI_LOGI("rc wait_num: %d\n",os_strtoul(argv[2], NULL, 10));
			bk_wifi_set_wait_num(os_strtoul(argv[2], NULL, 10));
	}
	else {
		CLI_LOGI("invalid RC paramter\n");
		goto error;
	}

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

void cli_wifi_ps_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t ps_id = 0;
	uint8_t ps_val = 0;
	uint8_t ps_val1 = 0;
	int ret = 0;
	char *msg = NULL;

	if (os_strcmp(argv[1], "open") == 0) {
		ps_id = 0;
	} else if (os_strcmp(argv[1], "close") == 0) {
		ps_id = 1;
	} else if (os_strcmp(argv[1], "debug") == 0) {
		ps_id = 2;
		ps_val = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
	} else if (os_strcmp(argv[1], "set_td") == 0){
		if (argc <= 3) {
			CLI_LOGI("error need 2 info\n");
			goto error;
		}
		ps_id = 3;
		ps_val = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		ps_val1 = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
	} else if (os_strcmp(argv[1], "mask_td") == 0){
		ps_id = 4;
	} else {
		CLI_LOGI("invalid ps paramter\n");
		goto error;
	}

	bk_wifi_ps_config(ps_id, ps_val, ps_val1);

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
error:
	CLI_LOGI("ps {open|close|debug [1|...|]|}\n");
	CLI_LOGI("ps debug 0	[close ps debug]\r\n");
	CLI_LOGI("ps debug 1	[show ps fail reason]\r\n");
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;

}

void cli_wifi_capa_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t capa_id = 0;
	uint32_t capa_val = 0;
	int ret = 0;
	char *msg = NULL;

	if (argc <= 2) {
		CLI_LOGI("invalid CAPA command\n");
		goto error;
	}

	if (os_strcmp(argv[1], "erp") == 0) {
		capa_id = WIFI_CAPA_ID_ERP_EN;
	} else if (os_strcmp(argv[1], "ht") == 0) {
		capa_id = WIFI_CAPA_ID_HT_EN;
	} else if (os_strcmp(argv[1], "vht") == 0) {
		capa_id = WIFI_CAPA_ID_VHT_EN;
	} else if (os_strcmp(argv[1], "he") == 0) {
		capa_id = WIFI_CAPA_ID_HE_EN;
	} else if (os_strcmp(argv[1], "tx_ampdu") == 0) {
		capa_id = WIFI_CAPA_ID_TX_AMPDU_EN;
	} else if (os_strcmp(argv[1], "rx_ampdu") == 0) {
		capa_id = WIFI_CAPA_ID_RX_AMPDU_EN;
	} else if (os_strcmp(argv[1], "tx_ampdu_num") == 0) {
		capa_id = WIFI_CAPA_ID_TX_AMPDU_NUM;
	} else if (os_strcmp(argv[1], "rx_ampdu_num") == 0) {
		capa_id = WIFI_CAPA_ID_RX_AMPDU_NUM;
	} else if (os_strcmp(argv[1], "vht_mcs") == 0) {
		capa_id = WIFI_CAPA_ID_VHT_MCS;
	} else if (os_strcmp(argv[1], "he_mcs") == 0) {
		capa_id = WIFI_CAPA_ID_HE_MCS;
	} else if (os_strcmp(argv[1], "b40") == 0) {
		capa_id = WIFI_CAPA_ID_B40_EN;
	} else if (os_strcmp(argv[1], "stbc") == 0) {
		capa_id = WIFI_CAPA_ID_STBC_EN;
	} else if (os_strcmp(argv[1], "sgi") == 0) {
		capa_id = WIFI_CAPA_ID_SGI_EN;
	} else if (os_strcmp(argv[1], "ldpc") == 0) {
		capa_id = WIFI_CAPA_ID_LDPC_EN;
	} else if (os_strcmp(argv[1], "bfmee") == 0) {
		capa_id = WIFI_CAPA_ID_BEAMFORMEE_EN;
	} else if (os_strcmp(argv[1], "11b_only") == 0) {
		capa_id = WIFI_CAPA_ID_11B_ONLY_EN;
	} else {
		CLI_LOGI("invalid CAPA paramter\n");
		goto error;
	}

	capa_val = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
	bk_wifi_capa_config(capa_id, capa_val);

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}

void cli_set_pkt_trx_dbg_cfg(uint32_t cfg_bit)
{
    bk_wifi_set_pkt_trx_dbg_cfg(cfg_bit);
    #if defined(CONFIG_LWIP_DEBUG_LOG)
    lwip_set_pkt_trx_dbg_cfg(cfg_bit);
    #endif
}

void cli_pkt_debug_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    uint32_t cfg_bit = 0;
    int ret = 0;

    if (argc != 2) {
        CLI_LOGI("invalid debug command %d\n",argc);
        goto error;
    }
    else if (os_strcmp(argv[1], "help") == 0)
    {
        CLI_LOGI("WIFI_TX_DBG_LOG_DATA_PKT     (1<<0)\r\n");
        CLI_LOGI("WIFI_TX_DBG_LOG_MGMT_PKT     (1<<1)\r\n");
        CLI_LOGI("WIFI_TX_DBG_LOG_PUSH_PKT     (1<<2)\r\n");
        CLI_LOGI("WIFI_RX_DBG_LOG_DATA_PKT     (1<<8)\r\n");
        CLI_LOGI("WIFI_RX_DBG_LOG_MGMT_PKT     (1<<9)\r\n");
        CLI_LOGI("LWIP_TX_DBG_LOG_SOCKET       (1<<16)\r\n");
        CLI_LOGI("LWIP_TX_DBG_LOG_IP           (1<<17)\r\n");
        CLI_LOGI("LWIP_TX_DBG_LOG_TCP          (1<<18)\r\n");
        CLI_LOGI("LWIP_RX_DBG_LOG_SOCKET       (1<<20)\r\n");
        CLI_LOGI("LWIP_RX_DBG_LOG_IP           (1<<21)\r\n");
        CLI_LOGI("LWIP_RX_DBG_LOG_TCP          (1<<22)\r\n");
        CLI_LOGI("LWIP_TX_DBG_LOG_PING         (1<<24)\r\n");
        CLI_LOGI("LWIP_RX_DBG_LOG_PING         (1<<25)\r\n");
    }
    else
    {
        cfg_bit = os_strtoul(argv[1], NULL, 0);
        cli_set_pkt_trx_dbg_cfg(cfg_bit);
        CLI_LOGI("open debug log\n");
    }

    if (!ret) {
        msg = WIFI_CMD_RSP_SUCCEED;
        os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
        return;
    }

error:
    msg = WIFI_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return;
}

void cli_wifi_diag_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    uint8_t diag_type = 0;
    uint16_t diag_no = 0;

    if (argc > 4) {
        CLI_LOGI("invalid debug command %d\n",argc);
        goto error;
    }
    else if (os_strcmp(argv[1], "help") == 0)
    {
        CLI_LOGI("wifi_diag test                                \r\n");
        CLI_LOGI("--test the connection of GPIO with diagnostics\r\n");
        CLI_LOGI("wifi_diag set [diag_type] [diag_no]           \r\n");
        CLI_LOGI("--diag_type - mac/phy                         \r\n");
        CLI_LOGI("--diag_no - HW diagnostics no                 \r\n");
    }
    else if (os_strcmp(argv[1], "test") == 0)
    {
        bk_wifi_test_hw_diag();
    }
    else if ((os_strcmp(argv[1], "set") == 0) && (os_strcmp(argv[2], "mac") == 0))
    {
        diag_type = 4;
        diag_no = os_strtoul(argv[3], NULL, 0) & 0xFFFF;
        bk_wifi_set_hw_diag(diag_type, diag_no);
    }
    else if ((os_strcmp(argv[1], "set") == 0) && (os_strcmp(argv[2], "phy") == 0))
    {
        diag_type = 5;
        diag_no = os_strtoul(argv[3], NULL, 0) & 0xFFFF;
        bk_wifi_set_hw_diag(diag_type, diag_no);
    }
    else
    {
        CLI_LOGI("invalid debug command %d\n",argc);
        goto error;
    }

    msg = WIFI_CMD_RSP_SUCCEED;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return;

error:

    CLI_LOGI("wifi_diag test                                \r\n");
    CLI_LOGI("--test the connection of GPIO with diagnostics\r\n");
    CLI_LOGI("wifi_diag set [diag_type] [diag_no]           \r\n");
    CLI_LOGI("--diag_type - mac/phy                         \r\n");
    CLI_LOGI("--diag_no - HW diagnostics no                 \r\n");

    msg = WIFI_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return;
}

#ifdef CONFIG_WPA_TWT_TEST
void cli_wifi_twt_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint16_t mantissa = 0;
	uint8_t min_twt = 0;
	int ret = 0;
	char *msg = NULL;

	if(os_strcmp(argv[1], "setup") == 0) {
		int setup_command = 0;

		if(os_strcmp(argv[2], "suggest") == 0) {
			setup_command = 1;
		}
		else if(os_strcmp(argv[2], "demand") == 0) {
			setup_command = 2;
		}
		else {
			CLI_LOGI("Usage: twt setup suggest/demand <param...>\n");
			goto error;
		}
		mantissa = os_strtoul(argv[3], NULL, 10) & 0xFF;
		min_twt = os_strtoul(argv[4], NULL, 10) & 0xFF;
		ret = bk_wifi_twt_setup(setup_command, mantissa, min_twt);
	}
	else if (os_strcmp(argv[1], "teardown") == 0)
		ret = bk_wifi_twt_teardown();
	else {
		CLI_LOGI("Usage: twt setup/teardown \n");
		goto error;
	}

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}
#endif

void blacklist_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int blacklist_ena = 0;

    if (argc != 2)
    {
        CLI_LOGE("blacklist <0|1>\n");
    }
    else
    {
        blacklist_ena = strtoul(argv[1], NULL, 0);
        if (blacklist_ena) {
            wlan_sta_enable_ssid_blacklist();
        }
        else {
            wlan_sta_disable_ssid_blacklist();
        }
        CLI_LOGI("blacklist %s\n", blacklist_ena ? "enabled" : "disabled");
    }
}

#if defined(CONFIG_RTP)
#include "RTP.h"
static void rtp_send(void *thread_param)
{
	u8 ip[4] = {192, 168, 0, 192};
	test_rtp_send(ip, 7180, 7180);
	rtos_delete_thread(NULL);
}

static void rtp_recv(void *thread_param)
{
	u8 ip[4] = {192, 168, 0, 154};
	test_rtp_recv(ip, 7180);
	rtos_delete_thread(NULL);
}

void cli_wifi_rtp_cmd(char * pcWriteBuffer, int xWriteBufferLen, int argc, char * * argv)
{
	if (wifi_cli_find_id(argc, argv, "-s") > 0) {
		rtos_create_thread(NULL, 4, "rtp_send",
							   rtp_send, 10*1024,
							   (beken_thread_arg_t) 0);
	} else if (wifi_cli_find_id(argc, argv, "-r") > 0) {
		rtos_create_thread(NULL, 4, "rtp_recv",
							   rtp_recv, 10*1024,
							   (beken_thread_arg_t) 0);
	}
}
#endif

void cli_wifi_close_coex_csa_cmd(char * pcWriteBuffer, int xWriteBufferLen, int argc, char * * argv)
{
	uint8_t close_csa;
	int ret = 0;

	if (argc != 2) {
		CLI_LOGI("invalid close_coex_csa command %d\n");
		return;
	}

	close_csa = (uint8_t)os_strtoul(argv[1], NULL, 10);
	if ((close_csa == 1) || (close_csa == 0))
	{
		ret = bk_wifi_set_csa_coexist_mode_flag(!!close_csa);

		if (ret)
			CLI_LOGI("bad state\r\n");
	}
	else
	{
		CLI_LOGI("bad parameters\r\n",close_csa);
	}

}

#if defined(CONFIG_BRIDGE)
extern uint8_t bk_wlan_has_role(uint8_t role);
enum mac_vif_type
{
    VIF_STA,
    VIF_IBSS,
    VIF_AP,
    VIF_MESH_POINT,
    VIF_MONITOR,
    VIF_UNKNOWN
};

void bk_bridge_start(char *bridge_ssid, char *ext_ssid, char *key) {
	if (bridge_open) {
		CLI_LOGE("bridge already opened\r\n");
		return;
	}
	bridge_open = 1;
	os_memset(br_ssid, 0, WIFI_SSID_STR_LEN);
	os_memset(br_pwd, 0, WIFI_PASSWORD_LEN);
	os_strcpy(br_ssid, bridge_ssid);
	os_strcpy(br_pwd, key);
	demo_sta_app_init(ext_ssid, key);
}

void bk_wifi_bridge_stop() {
	bridge_ip_stop();
}

void bk_bridge_stop() {
	if (!bridge_open) {
		CLI_LOGE("bridge was not opened\r\n");
		return;
	}
	bk_wifi_bridge_stop();
	if(bk_wifi_sta_stop())
		CLI_LOGE("bridge stop sta fail\r\n");
	if(bk_wifi_ap_stop())
		CLI_LOGE("bridge stop ap fail\r\n");
	bridge_is_enabled = 0;
	bridge_open = 0;
}

void cli_wifi_open_bridge_cmd(char * pcWriteBuffer, int xWriteBufferLen, int argc, char * * argv)
{
	char *oob_ssid = NULL;
    char *bridge_ssid = NULL;
    char *connect_key = NULL;

    CLI_LOGD("bridge_Command\r\n");

    if (argc < 2) {
usage:
		CLI_LOGE("Usage: \n");
		CLI_LOGE("  %s open bridge_ssid extap_ssid key\n", argv[0]);
		CLI_LOGE("  %s close\n", argv[0]);
		return;
	}

	if (!strcmp(argv[1], "open")) {
		if (argc < 4)
			goto usage;

		bridge_ssid = argv[2];
		oob_ssid = argv[3];

		if(argc == 5)
			connect_key = argv[4];
		else
			connect_key = "";

		bk_bridge_start(bridge_ssid, oob_ssid, connect_key);
	} else if (!strcmp(argv[1], "close")) {
        bk_bridge_stop();
	} else {
		goto usage;
	}
}
#endif



void cli_wifi_set_country_cmd(char * pcWriteBuffer, int xWriteBufferLen, int argc, char * * argv)
{
    wifi_country_t wifi_country = {0};
    
    if (argc >= 2)
    {
        if(os_strlen(argv[1]) > 3)
        {
            CLI_LOGI("country code length should be less than 3 characters \r\n");
            return ;
        }
        os_memset(&wifi_country,0,sizeof(wifi_country));
        os_memcpy(wifi_country.cc, argv[1], os_strlen(argv[1]));
        CLI_LOGI("cli set country code cc=%c%c%c \r\n",wifi_country.cc[0], wifi_country.cc[1],wifi_country.cc[2]);
        bk_wifi_set_country(&wifi_country);
    }
    else
    {
        CLI_LOGI("Usage: \n");
        CLI_LOGI("set_country cc \r\n");
        return ;
    }
}



void cli_wifi_set_pwr_limit_cmd(char * pcWriteBuffer, int xWriteBufferLen, int argc, char * * argv)
{
    wifi_tx_pwr_lmt_t wifi_tx_pwr_lmt;
    if (argc >= 5)
    {
        if(os_strcmp(argv[1], "FCC") == 0)
        {
            wifi_tx_pwr_lmt.regulation = WIFI_REGU_FCC;
        }
        else if(os_strcmp(argv[1], "ETSI") == 0)
        {
            wifi_tx_pwr_lmt.regulation = WIFI_REGU_ETSI;
        }        
        else if(os_strcmp(argv[1], "MKK") == 0)
        {
            wifi_tx_pwr_lmt.regulation = WIFI_REGU_MKK;
        }        
        else
        {
            CLI_LOGI("wrong regualtion\r\n");
        }
        
        if(os_strcmp(argv[2], "CCK") == 0)
        {
            wifi_tx_pwr_lmt.ratesection = WIFI_RS_CCK;
        }
        else if(os_strcmp(argv[2], "OFDM") == 0)
        {
            wifi_tx_pwr_lmt.ratesection = WIFI_RS_OFDM;
        }        
        else if(os_strcmp(argv[2], "HT") == 0)
        {
            wifi_tx_pwr_lmt.ratesection = WIFI_RS_HT;
        }        
        else if(os_strcmp(argv[2], "HE") == 0)
        {
            wifi_tx_pwr_lmt.ratesection = WIFI_RS_HE;
        }        
        else
        {
            bk_printf("wrong ratesection\r\n");
        }
        wifi_tx_pwr_lmt.channel = (uint8_t)os_strtoul(argv[3], NULL, 10);
        wifi_tx_pwr_lmt.value = (uint8_t)os_strtoul(argv[4], NULL, 10);
        
        bk_wifi_set_pwr_limit(&wifi_tx_pwr_lmt);
    }
    else
    {
        CLI_LOGI("Usage: \n");
        CLI_LOGI("set_pwr_limit regu ratesect chan powervalue\r\n");
        return ;
    }

}

#if CONFIG_WIFI_REGDOMAIN
/*
 * regulatory domain command.
 *     regd get
 *     regd set <alpha2>
 */
void cli_wifi_regd_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGD("regulatory command\r\n");

	if (!strcmp(argv[1], "get")) {
		bk_wifi_print_regdomain();
		return;
	} else if (argc > 2 && !strcmp(argv[1], "set")) {
		char alpha2[3] = {0};
		os_strlcpy(alpha2, argv[2], 3);
		bk_wifi_set_country_code(alpha2);
		return;
	}

	CLI_LOGE("Usage: \n");
	CLI_LOGE("	%s get\n", argv[0]);
	CLI_LOGE("	%s set <alpha2>\n", argv[0]);
}
#endif

#define WIFI_CMD_CNT (sizeof(s_wifi_commands) / sizeof(struct cli_command))
static const struct cli_command s_wifi_commands[] = {
#if !defined(CONFIG_CLI_CODE_SIZE_OPTIMIZE_ENABLE)
	{"scan", "scan [ssid]", cli_wifi_scan_cmd},
	{"ap", "ap ssid [password] [channel[1:14]]", cli_wifi_ap_cmd},
	{"start_hidden_softap", "start_hidden_softap ssid [password] [channel[1:14]]", cli_wifi_hidden_ap_cmd},
	{"sta", "sta ssid [password][bssid][channel]", cli_wifi_sta_cmd}, //TODO support connect speicific BSSID
#if CONFIG_WIFI_REGDOMAIN
	{"regd", "get|set <alpha2>", cli_wifi_regd_cmd},
#endif
#if CONFIG_COMPONENTS_WPA2_ENTERPRISE
	{"sta_eap", "sta_eap ssid password [identity] [client_cert] [private_key]", cli_wifi_sta_eap_cmd},
#endif
	{"stop", "stop {sta|ap}", cli_wifi_stop_cmd},
#if defined(CONFIG_RWNX_PROTO_DEBUG)
	{"set_pd_flag", "set proto flag for enable or disable proto debug {1|0}", cli_wifi_set_proto_debug_flag},
#endif
	{"set_interval", "set listen interval}", cli_wifi_set_interval_cmd},
	{"set_arp_rate", "set arp rate}", cli_wifi_set_arp_rate_cmd},
	{"bcn_loss_intv", "bcn_loss_intv interval repeat_num}", cli_wifi_bcn_loss_intv_cmd},
	{"bcn_loss_time", "bcn_loss_time wait_cnt wake_cnt}", cli_wifi_set_bcn_loss_time_cmd},
	{"bcn_recv_win", "bcn_recv_win default_win max_win step}", cli_wifi_set_bcn_recv_win_cmd},
	{"bcn_miss_time", "bcn misstime}", cli_wifi_set_bcn_miss_time_cmd},
	{"monitor", "monitor {1~13|15|99}", cli_wifi_monitor_cmd},
	{"state", "state - show STA/AP state", cli_wifi_state_cmd},
	{"channel", "channel {1~13} - set monitor channel", cli_wifi_monitor_channel_cmd},
	//{"net", "net {sta/ap} ... - wifi net config", cli_wifi_net_cmd},
	{"get", "get wifi status", cli_wifi_get_cmd},
	{"iplog", "iplog [modle][type]", cli_wifi_iplog_cmd},
	{"ipdbg", "ipdbg [function][value]", cli_wifi_ipdbg_cmd},
	{"mem_apply", "mem_apply [module][value]", cli_wifi_mem_apply_cmd},
	{"ps","ps enable and debug info config", cli_wifi_ps_cmd},

#ifdef CONFIG_WPA_TWT_TEST
	{"twt", "twt {setup|teardown}", cli_wifi_twt_cmd},
#endif

#if defined(CONFIG_COMPONENTS_WFA_CA)
	{"wfa_ca", "wfa_ca <start|stop>", cli_wifi_wfa_ca_cmd},
#endif

#if defined(CONFIG_WIFI_SENSOR)
	{"wifisensor", "wifi sensor", cli_wifi_sensor_cmd},
#endif
	{"filter", "filter <bits> - bit0/d, 1/preq, 2/prsp, 3/b, 4/a", cli_wifi_filter_cmd},
#if defined(CONFIG_WIFI_RAW_TX_TEST)
	{"wifi_tx", "wifi_tx - Tx WiFi raw frame", cli_wifi_raw_tx_cmd},
#endif
	{"rc", "wifi rate control config", cli_wifi_rc_cmd},
	{"capa", "wifi capability config", cli_wifi_capa_cmd},
	{"blacklist", "Set ssid blacklist", blacklist_Command},
#if defined(CONFIG_RTP)
	{"rtp", "rtp -s/c ip", cli_wifi_rtp_cmd},
#endif
#if defined(CONFIG_AP_STATYPE_LIMIT)
	{"sta_vsie", "sta ssid [password][bssid][sta_type]", cli_wifi_sta_vsie_cmd},
	{"ap_vsie", "ap ssid [password][bssid][statype_num]", cli_wifi_ap_vsie_cmd},
#endif
	{"pkt_dbg", "packet debug config", cli_pkt_debug_cmd},
	{"wifi_diag", "Wi-Fi HW diagnostics config", cli_wifi_diag_cmd},
	#if defined(CONFIG_WIFI_CLI_RAW_LINK_ENABLE)
	{"rlk_cfg", "rlk config", cli_rlk_cfg_cmd},
	#endif
	{"close_coex_csa","close csa in coexist mode {1|0}", cli_wifi_close_coex_csa_cmd},
#if defined(CONFIG_BRIDGE)
	{"bridge", "bridge open|close", cli_wifi_open_bridge_cmd},
#endif
#if defined(CONFIG_SOFTAP_WPA3)
	{"ap_wpa3", "ap_wpa3 ssid [password] [channel[1:14]", cli_wifi_wpa3_ap_cmd},
#endif

    {"set_country","set_country cc", cli_wifi_set_country_cmd},
    {"set_pwr_limit","set_pwr_limit regu ratesect chan value", cli_wifi_set_pwr_limit_cmd},

#else
	{"state", "state - show STA/AP state", cli_wifi_state_cmd},
	{"sta", "sta ssid [password][bssid][channel]", cli_wifi_sta_cmd},
	{"ap", "ap ssid [password] [channel[1:14]]", cli_wifi_ap_cmd},
	{"scanopt", "scanopt [type 0:active 1:passive] [duration 120|70|40|20]", cli_wifi_scan_opt_cmd},
#endif
};

int cli_wifi_init(void)
{
#if !defined(CONFIG_CLI_CODE_SIZE_OPTIMIZE_ENABLE)
	BK_LOG_ON_ERR(bk_event_register_cb(EVENT_MOD_WIFI, EVENT_ID_ALL, cli_wifi_event_cb, NULL));
	BK_LOG_ON_ERR(bk_event_register_cb(EVENT_MOD_NETIF, EVENT_ID_ALL, cli_netif_event_cb, NULL));
#endif
	return cli_register_commands(s_wifi_commands, WIFI_CMD_CNT);
}

#endif //#if (CLI_CFG_WIFI == 1)
