/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <wifi_manager/wifi_manager.h>
#include <stress_tool/st_perf.h>
#include "connect_test.h"
#include "connect_test_server.h"
#include "connect_test_log.h"
#include "connect_test_data.h"
#include "connect_test_utils.h"

#define TAG "[CT_SERVER]"

static bool is_network = false;
static bool is_ap_on = false;

bool g_tcp_server_recvd_data;
bool g_tcp_server_sentack_data;
bool g_tcp_server_sent_data;
bool g_tcp_server_recvdack_data;
bool g_tls_server_recvd_data;
bool g_tls_server_sent_data;

static struct ct_options* opt = NULL;
static char *g_WM_AP_SSID = "";
static char *g_WM_AP_PASSWORD = "";
static wifi_manager_ap_auth_type_e g_WM_AP_AUTH = WIFI_MANAGER_AUTH_WPA2_PSK;
static wifi_manager_ap_crypto_type_e g_WM_AP_CRYPTO = WIFI_MANAGER_CRYPTO_AES;
static char *g_WM_SOFTAP_SSID = "";
static char *g_WM_SOFTAP_PASSWORD = "";
static int g_WM_SOFTAP_CHANNEL = 1;
static uint32_t g_WM_REPEAT_COUNT = 300;
static uint8_t g_WM_IS_DTLS = 0;
static uint32_t g_WM_DTLS_DATA_SIZE = 64;
static uint8_t g_WM_IS_TLS = 0;
static uint32_t g_WM_TLS_DATA_SIZE = 64;
static char *g_WM_HOSTNAME_FILE = NULL;
static struct ct_queue *g_ct_queue = NULL;
static char g_ipv4_buf[18] = {
	'\0',
};
static char g_broadcast_message[30] = {
	'\0',
};

char g_recv_mac_buf_server[MAC_BUF_SIZE] = {
	'\0',
};
char g_mac_buf_server[MAC_BUF_SIZE] = {
	'\0',
};
volatile bool g_broadcast_flag = -1;


/*
 * callbacks
 */
static void wm_cb_sta_connected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_softap_sta_join(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_softap_sta_leave(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_scan_done(wifi_manager_cb_msg_s msg, void *arg);

static wifi_manager_cb_s g_wifi_callbacks = {
	wm_cb_sta_connected,
	wm_cb_sta_disconnected,
	wm_cb_softap_sta_join,
	wm_cb_softap_sta_leave,
	wm_cb_scan_done,
};

void wm_cb_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	CT_LOG(TAG, "--> res(%d)", msg.res);
	CT_LOG(TAG, "bssid %02x:%02x:%02x:%02x:%02x:%02x",
		   msg.bssid[0], msg.bssid[1],
		   msg.bssid[2], msg.bssid[3],
		   msg.bssid[4], msg.bssid[5]);
	int conn = 0;	
	if (WIFI_MANAGER_SUCCESS == msg.res) {
		conn = CT_CONN_SUCCESS;
		is_network = true;		
	} else {
		conn = CT_CONN_FAIL;
		is_network = false;		
	}	
	CT_TEST_SIGNAL(conn, g_ct_queue);
}

void wm_cb_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
	CT_LOG(TAG, "--> res(%d) reason %d", msg.res, msg.reason);	
	is_network = false;	
}

void wm_cb_softap_sta_join(wifi_manager_cb_msg_s msg, void *arg)
{
	CT_LOG(TAG, "--> res(%d)", msg.res);
	CT_LOG(TAG, "bssid %02x:%02x:%02x:%02x:%02x:%02x",
		   msg.bssid[0], msg.bssid[1],
		   msg.bssid[2], msg.bssid[3],
		   msg.bssid[4], msg.bssid[5]);	
	CT_TEST_SIGNAL(CT_CONN_SUCCESS, g_ct_queue);	
}

void wm_cb_softap_sta_leave(wifi_manager_cb_msg_s msg, void *arg)
{
	CT_LOG(TAG, "--> res(%d) reason %d", msg.res, msg.reason);	
}

void wm_cb_scan_done(wifi_manager_cb_msg_s msg, void *arg)
{
	CT_LOG(TAG, "--> res(%d)", msg.res);
	if (msg.res != WIFI_MANAGER_SUCCESS || msg.scanlist == NULL) {		
		CT_TEST_SIGNAL(CT_CONN_FAIL, g_ct_queue);
		return;
	}	
	ct_print_scanlist(msg.scanlist, g_WM_AP_SSID, &is_ap_on);	
	CT_TEST_SIGNAL(CT_CONN_SUCCESS, g_ct_queue);	
}

static void broadcast_data(void)
{
	CT_LOG(TAG, "ENTRY");
	struct sockaddr_in dest_addr;
	int sock;
	unsigned int retry = 10;
retry_connection:
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		CT_LOGE(TAG, "Socket creation failed, error value is : %s-%d", strerror(errno), errno);
		return;
	}
	int broadcast = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
		CT_LOGE(TAG, "Error in setting Broadcast option, error description-value is : %s-%d", strerror(errno), errno);
		close(sock);
		return;
	}

	struct timeval tv;	
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) < 0) {
		CT_LOGE(TAG, "Set time out failed for sendto");
		close(sock);
		return;
	}

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	dest_addr.sin_addr.s_addr = INADDR_BROADCAST;	
	int conn = 0;
	while (g_broadcast_flag && retry--) {
		int res = sendto(sock, g_broadcast_message, sizeof(g_broadcast_message), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
		if (res < 0) {			
			if (!is_network) {
				CT_LOG(TAG, "connection_got_broken");
				close(sock);
				CT_TEST_WAIT(conn, g_ct_queue);				
				CT_LOG(TAG, "connection_resumed");				
				goto retry_connection;
			}
			if (errno == EWOULDBLOCK) {
				CT_LOGE(TAG, "send failed, error description-value is : %s-%d, try again", strerror(errno), errno);
				continue;
			} else {
				CT_LOGE(TAG, "send failed, error description-value is : %s-%d", strerror(errno), errno);
				close(sock);
				return;
			}
		} else {
			CT_LOG(TAG, "send message success");
			CT_LOG(TAG, "adding 1 sec sleep which will give enough time to client to receive broadcast.");
			sleep(1);
		}
	}
	if (0 == retry) {
		CT_LOGE(TAG, "broadcast IP send failed even after %d retry", retry);
	}
	close(sock);
}

static int _run_procedure()
{
	struct timeval start, end;
	long sum = 0;
	int conn = 0;
	int res;
	is_network = false;
	is_ap_on = false;

	g_tcp_server_recvd_data = false;
	g_tcp_server_sentack_data = false;

	g_tcp_server_sent_data = false;
	g_tcp_server_recvdack_data = false;
	
	g_tls_server_recvd_data = false;
	g_tls_server_sent_data = false;	

	wifi_manager_result_e wres = WIFI_MANAGER_SUCCESS;	

	/* Initialise Wifi */
	CT_LOG(TAG, "init wi-fi");
	wres = wifi_manager_init(&g_wifi_callbacks);
	if (wres != WIFI_MANAGER_SUCCESS) {
		CT_LOGE(TAG, "fail to init %d", wres);
		return -1;
	}

	/*  Start softAP */
	CT_LOG(TAG, "start softAP");
	wifi_manager_softap_config_s softap_config;
	SET_START_TIME(Change to SoftAP mode);
	wm_get_softapinfo(&softap_config, g_WM_SOFTAP_SSID, g_WM_SOFTAP_PASSWORD, g_WM_SOFTAP_CHANNEL);
	wres = wifi_manager_set_mode(SOFTAP_MODE, &softap_config);
	if (wres != WIFI_MANAGER_SUCCESS) {
		CT_LOGE(TAG, "fail to start softap %d", wres);
		return -1;
	}
	CALCULATE_ELAPSED_TIME(Change to SoftAP mode, sum);

	/*  wait join event */
	CT_LOG(TAG, "wait join event");	
	CT_TEST_WAIT(conn, g_ct_queue);

	/*  scan in softAP mode */
	CT_LOG(TAG, "scan in softAP mode");
	SET_START_TIME(Scan in SoftAP mode);
	wres = wifi_manager_scan_ap(NULL);
	if (wres != WIFI_MANAGER_SUCCESS) {
		CT_LOGE(TAG, "fail to scan %d", wres);
		return -1;
	}

	/*  wait scan event */
	CT_LOG(TAG, "wait scan done event");	
	CT_TEST_WAIT(conn, g_ct_queue);
	CALCULATE_ELAPSED_TIME(Scan in SoftAP mode, sum);

	/* Prepare MAC for transfer during UDP/DTLS */
	res = get_device_mac(g_mac_buf_server, MAC_BUF_SIZE);
	if (res != 0) {
		CT_LOGE(TAG, "get_device_mac return %d", res);
		return -1;
	}

	/* Data transfer using UDP or DTLS */
	if (g_WM_IS_DTLS) {
		_dtls_server(g_WM_DTLS_DATA_SIZE, &sum);
	}
	else {
		_udp_server(g_WM_DTLS_DATA_SIZE, &sum);
	}

	/*  set STA */
	CT_LOG(TAG, "start STA mode");
	SET_START_TIME(Change to Station Mode);
	wres = wifi_manager_set_mode(STA_MODE, NULL);
	if (wres != WIFI_MANAGER_SUCCESS) {
		CT_LOGE(TAG, "start STA fail %d", wres);
		return -1;
	}	

	CALCULATE_ELAPSED_TIME(Change to STA mode, sum);
	
	/*  scan in STA mode */
	CT_LOG(TAG, "scan in STA mode");
	SET_START_TIME(Scan in STA mode);
	wres = wifi_manager_scan_ap(NULL);
	if (wres != WIFI_MANAGER_SUCCESS) {
		CT_LOGE(TAG, "fail to scan %d", wres);
		return -1;
	}

	/*  wait scan event */
	CT_LOG(TAG, "wait scan done event in STA mode");	
	CT_TEST_WAIT(conn, g_ct_queue);
	CALCULATE_ELAPSED_TIME(Scan in STA mode, sum);
	
	// Scan till AP gets ON is_ap_on
	unsigned int retry_ap_scan = 30;
	while (!is_ap_on && retry_ap_scan--) {
		wifi_manager_result_e wres = wifi_manager_scan_ap(NULL);
		if (wres != WIFI_MANAGER_SUCCESS) {
			CT_LOGE(TAG, "fail to scan %d", wres);
			sleep(2);
			continue;
		}
		CT_TEST_WAIT(conn, g_ct_queue); // Wait scan done event
		sleep(2);
	}
	if (0 == retry_ap_scan) {
		CT_LOGE(TAG, "ap_scan failed even afer %d retry or passed ap does not exit", retry_ap_scan);
		return -1;
	}

	/*  connect to AP */
	wifi_manager_ap_config_s apconfig;
	wm_get_apinfo(&apconfig, g_WM_AP_SSID, g_WM_AP_PASSWORD, g_WM_AP_AUTH, g_WM_AP_CRYPTO);
	unsigned int retry_ap_connect = 30;
	while (!is_network && retry_ap_connect--) {
		wres = connect_AP(apconfig, g_ct_queue, 100000);
		if (wres != WIFI_MANAGER_SUCCESS) {
			CT_LOGE(TAG, "connect_AP return %d", wres);
			sleep(2);
			continue;			
		}
	}
	if (0 == retry_ap_connect) {
		CT_LOGE(TAG, "ap_connect failed even after %d retry", retry_ap_connect);
		return -1;
	}

	/* gethostname service  */
	dns_service(g_WM_HOSTNAME_FILE);

	/* Prepare broadcast data and broadcast it */
	res = get_device_ip(g_ipv4_buf, IP_BUF_SIZE);
	if (res != 0) {
		CT_LOGE(TAG, "get_device_ip return %d", res);
		return -1;
	}
	strncpy(g_broadcast_message, (const char *)g_mac_buf_server,
			strlen((const char *)g_mac_buf_server));
	strcat(g_broadcast_message, " ");
	strncat(g_broadcast_message, (const char *)g_ipv4_buf,
			strlen((const char *)g_ipv4_buf));
	g_broadcast_flag = true;

	pthread_t tid;
	pthread_create(&tid, NULL, (void*)&broadcast_data, NULL);

	/* Data transfer using TCP or TLS */
	CT_LOG(TAG, "Data_transfer_using_TCP_or_TLS");
	while (true) {
		res = 0;
		if (g_WM_IS_TLS) {
			res = _tls_server(g_WM_TLS_DATA_SIZE, &sum);
		} else {
			res = _tcp_server(g_WM_TLS_DATA_SIZE, &sum);
		}
		if (res < 0) {			
			if (!is_network) {
				CT_LOG(TAG, "connection_got_broken");
				unsigned int retry_ap_connect = 10;
				while (true && retry_ap_connect--) {
					res = connect_AP(apconfig, g_ct_queue, 10000000);
					if (res != 0) {
						CT_LOGE(TAG, "connect_ap_failed_return %d", res);
						sleep(2);
						continue;						
					}
					if (!is_network) {
						sleep(2); // Wait for 2 secs for station to get connected
					}
					if (is_network) {
						CT_LOG(TAG, "connection_resumed_station_connected");
						break;
					}
				}
				if (0 == retry_ap_connect) {
					CT_LOGE(TAG, "ap_connect failed even after %d retry", retry_ap_connect);
					return -1;
				}
			}
			else {
				CT_LOG(TAG, "Connection is Ok but data transfer failed");
				break;
			}
		} else {			
			CT_LOG(TAG, "Data transfer success");
			break;
		}
	}

	/* Deinitialise Wifi */
	CT_LOG(TAG, "deinit wi-fi");
	wres = wifi_manager_deinit();
	if (wres != WIFI_MANAGER_SUCCESS) {
		CT_LOGE(TAG, "fail to deinit %d", wres);
		return -1;
	}

	CT_LOG(TAG, "Total time taken(for 7 operations) is %ld", sum);
	return 0;
}

TEST_F(easysetup_tc)
{
	ST_START_TEST;		
	ST_EXPECT_EQ(0, _run_procedure());
	ST_END_TEST;
}

void connect_test_server(void *arg)
{
	opt = (struct ct_options*)arg;
	g_WM_AP_SSID = opt->ssid;
	g_WM_AP_PASSWORD = opt->password;
	g_WM_AP_AUTH = opt->auth_type;
	g_WM_AP_CRYPTO = opt->crypto_type;
	g_WM_SOFTAP_SSID = opt->softap_ssid;
	g_WM_SOFTAP_PASSWORD = opt->softap_password;
	g_WM_SOFTAP_CHANNEL = opt->softap_channel;
	g_WM_REPEAT_COUNT = opt->repeat;
	g_WM_IS_DTLS = opt->is_dtls;
	g_WM_DTLS_DATA_SIZE = opt->dtls_data_size * 1024;
	g_WM_IS_TLS = opt->is_tls;
	g_WM_TLS_DATA_SIZE = opt->tls_data_size * 1024;
	if (opt->path) {
		g_WM_HOSTNAME_FILE = opt->path;
	}

	CT_LOG(TAG, "init sem");
	g_ct_queue = ct_create_queue();
	if (!g_ct_queue) {
		CT_LOGE(TAG, "create queue fail");
		return;
	}

	ST_SET_PACK(wifi);
	ST_SET_SMOKE1(wifi, g_WM_REPEAT_COUNT, 60000000, "easysetup TC", easysetup_tc);
	ST_RUN_TEST(wifi);
	ST_RESULT_TEST(wifi);

	CT_LOG(TAG, "deinit sem");	
	ct_destroy_queue(g_ct_queue);
}