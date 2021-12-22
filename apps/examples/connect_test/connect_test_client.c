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
#include "connect_test.h"
#include "connect_test_log.h"
#include "connect_test_utils.h"
#include "connect_test_data.h"

#define TAG "[CT_CLIENT]"
#define UDP_SERVER_ADDR CONFIG_LWIP_DHCPS_SERVER_IP

static bool is_network = false;
static bool is_ap_on = false;

bool g_tcp_client_sent_data;
bool g_tcp_client_recvdack_data;
bool g_tcp_client_recvd_data;
bool g_tcp_client_sentack_data;
bool g_tls_client_recvd_data;
bool g_tls_client_sent_data;

static struct ct_options* client_info = NULL;

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
static struct ct_queue *g_ct_queue = NULL;
static char g_broadcast_msg[30] = {
	'\0',
};
static char g_tcp_server_address[20] = {
	'\0',
};

char g_mac_buf_client[MAC_BUF_SIZE] = {
	'\0',
};
char g_recv_mac_buf_client[MAC_BUF_SIZE] = {
	'\0',
};

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
	CT_TEST_SIGNAL(CT_CONN_SUCCESS, g_ct_queue);	
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

static int broadcast_receive_init(int *sock)
{
	if ((*sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		CT_LOGE(TAG, "Socket creation failed, error description-value is : %s-%d", strerror(errno), errno);
		return -1;
	}

	int broadcast = 1;
	if (setsockopt(*sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
	    CT_LOGE(TAG, "Error in setting Broadcast option, error description-value is : %s-%d", strerror(errno), errno);
		return -1;
	}

	/* Set send and receive timeout values */
	struct timeval tv;	
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	if (setsockopt(*sock, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) < 0) {
		CT_LOGE(TAG, "Set time out failed for recvfrom");
	}

	struct sockaddr_in recv_addr;
	memset(&recv_addr, 0, sizeof(recv_addr));

	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons(PORT);
	recv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(*sock, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0) {
		CT_LOGE(TAG, "Error in BINDING , error description-value is : %s-%d", strerror(errno), errno);
		return -1;
	}
	return 0;
}

static int broadcast_receive(int sock, char *msg, int msg_len)
{
	int nbytes = 0;
	unsigned int len = sizeof(struct sockaddr_in);
	int retry_count = 10;
	struct sockaddr_in sender_addr;
	while (retry_count--) {
		nbytes = recvfrom(sock, msg, msg_len, 0, (struct sockaddr *)&sender_addr, &len);
		if (nbytes > 0) {
			CT_LOG(TAG, "received message: %s", msg);
			char *broadcast_mac, *broadcast_ip;
			broadcast_mac = strtok(msg, " ");
			broadcast_ip = strtok(NULL, " ");
			CT_LOG(TAG, "The stored mac of peer is %s", g_recv_mac_buf_client);
			if (!strncmp(g_recv_mac_buf_client, broadcast_mac, strlen(g_recv_mac_buf_client))) {			
				strncpy(g_tcp_server_address, broadcast_ip, strlen(broadcast_ip) + 1);
				CT_LOG(TAG, "The mac of peer is %s and ip of peer is %s", broadcast_mac, broadcast_ip);
				CT_LOG(TAG, "server ip %s", g_tcp_server_address);
				break;
			} else {
				continue;
			}
		} else {
			if (!is_network) {
				// connection_got_broken
				return -2;
			}
			if (errno == EWOULDBLOCK) {
				CT_LOGE(TAG, "timeout trying again, error %s-%d", strerror(errno), errno);
				continue;
			} else {
				CT_LOGE(TAG, "Receving broadcast message failed");
				return -1;
			}
		}
	}
	return 0;
}

static int connect_test_run_client()
{
	int res = 0, conn = 0;
	is_network = false;
	is_ap_on = false;	

	/*  connect to softAP */
	wifi_manager_ap_config_s apconfig;
	wm_get_apinfo(&apconfig, g_WM_SOFTAP_SSID, g_WM_SOFTAP_PASSWORD, g_WM_AP_AUTH, g_WM_AP_CRYPTO);	
	res = connect_AP(apconfig, g_ct_queue, 100000);
	if (res != 0) {
		CT_LOGE(TAG, "connect_ap return %d", res);
		return -1;
	}

	CT_LOG(TAG, "add 2 seconds sleep, let peer machine go in receive mode");
	sleep(2);
	res = get_device_mac(g_mac_buf_client, MAC_BUF_SIZE);
	if (res != 0) {
		CT_LOGE(TAG, "get_device_mac return %d", res);
		return -1;
	}

	if (g_WM_IS_DTLS) {
		_dtls_client(UDP_SERVER_ADDR, g_WM_DTLS_DATA_SIZE);
	}
	else {
		_udp_client(UDP_SERVER_ADDR, g_WM_DTLS_DATA_SIZE);
	}
	CT_LOG(TAG, "wait for turnoff softap");		
	usleep(500000);	 // Wait for server softap turnoff before its STA mode start
	res = wifi_manager_disconnect_ap();
	if (res != WIFI_MANAGER_SUCCESS) {
		CT_LOGE(TAG, "disconnect fail (%d)", res);		
	}
	CT_LOG(TAG, "disconnect done");
	
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
		CT_LOGE(TAG, "ap_scan failed even afer %d retry or passed ap does not exist", retry_ap_scan);
		return -1;
	}

	/* connect to AP */
	wm_get_apinfo(&apconfig, g_WM_AP_SSID, g_WM_AP_PASSWORD, g_WM_AP_AUTH, g_WM_AP_CRYPTO);
	unsigned int retry_ap_connect = 30;
	while (!is_network && retry_ap_connect--) {
		res = connect_AP(apconfig, g_ct_queue, 100000);
		if (res != 0) {
			CT_LOGE(TAG, "connect_ap return %d", res);
			sleep(2);
			continue;			
		}
	}
	if (0 == retry_ap_connect) {
		CT_LOGE(TAG, "ap_connect failed even after %d retry", retry_ap_connect);
		return -1;
	}

	int broadcast_sock, ret_val;	
retry_broadcast_recv:
	ret_val = broadcast_receive_init(&broadcast_sock);
	if (0 == ret_val) {
		ret_val = broadcast_receive(broadcast_sock, g_broadcast_msg, sizeof(g_broadcast_msg));		
		if (0 == ret_val) {
			/* Data transfer using TCP or TLS */
			CT_LOG(TAG, "Data_transfer_using_TCP_or_TLS");
			while (true) {
				res = 0;
				if (g_WM_IS_TLS) {
					res = _tls_client(g_tcp_server_address, g_WM_TLS_DATA_SIZE);
				}
				else {
					res = _tcp_client(g_tcp_server_address, g_WM_TLS_DATA_SIZE);
				}
				if (res < 0) {					
					if (!is_network) {
						CT_LOG(TAG, "connection_got_broken");
						unsigned int retry_ap_connect = 10;
						while (true && retry_ap_connect--) {
							res = connect_AP(apconfig, g_ct_queue, 10000000);
							if (res != 0) {
								CT_LOGE(TAG, "connect_ap_failed_return %d", res);								
								continue;							
							}
							if (!is_network) {
								sleep(2); // Wait for 2 secs for station to get connected
							} 							
							if(is_network) {
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
				}
				else {
					CT_LOG(TAG, "Data transfer success");
					break;
				}				
			}
		}
		else if (-2 == ret_val) {
			CT_LOG(TAG, "connection_got_broken");
			close(broadcast_sock);
			CT_TEST_WAIT(conn, g_ct_queue);
			CT_LOG(TAG, "connection_resumed");
			goto retry_broadcast_recv;
		} else {
			close(broadcast_sock);
			CT_LOGE(TAG, "ip address cannot be received");
		}		
	}
	else {
		close(broadcast_sock);
		CT_LOGE(TAG, "ip address cannot be received");
	}

	return 0;
}

void connect_test_client(void *arg)
{
	client_info = (struct ct_options*)arg;
	g_WM_AP_SSID = client_info->ssid;
	g_WM_AP_PASSWORD = client_info->password;
	g_WM_AP_AUTH = client_info->auth_type;
	g_WM_AP_CRYPTO = client_info->crypto_type;
	g_WM_SOFTAP_SSID = client_info->softap_ssid;
	g_WM_SOFTAP_PASSWORD = client_info->softap_password;
	g_WM_SOFTAP_CHANNEL = client_info->softap_channel;
	g_WM_REPEAT_COUNT = client_info->repeat;
	g_WM_IS_DTLS = client_info->is_dtls;
	g_WM_DTLS_DATA_SIZE = client_info->dtls_data_size * 1024;
	g_WM_IS_TLS = client_info->is_tls;
	g_WM_TLS_DATA_SIZE = client_info->tls_data_size * 1024;

	CT_LOG(TAG, "init sem");
	g_ct_queue = ct_create_queue();
	if (!g_ct_queue) {
		CT_LOGE(TAG, "create queue fail");
		return;
	}
	
	for (int count = 0; count < g_WM_REPEAT_COUNT; count++) {		
		g_tcp_client_sent_data = false;
		g_tcp_client_recvdack_data = false;

		g_tcp_client_recvd_data = false;
		g_tcp_client_sentack_data = false;
		
		g_tls_client_recvd_data = false;
		g_tls_client_sent_data = false;

		CT_LOG(TAG, "====== Iteration %d starts ======", count + 1);
		/* Initialise Wifi */
		wifi_manager_result_e wres = WIFI_MANAGER_SUCCESS;
		CT_LOG(TAG, "init wi-fi");
		wres = wifi_manager_init(&g_wifi_callbacks);
		if (wres != WIFI_MANAGER_SUCCESS) {
			CT_LOGE(TAG, "fail to init %d", wres);
		} else {			
			if (connect_test_run_client() != 0) {
				CT_LOG(TAG, "Failed to run client");
			}
		}

		/* Deinitialise Wifi */
		CT_LOG(TAG, "deinit wi-fi");
		wres = wifi_manager_deinit();
		if (wres != WIFI_MANAGER_SUCCESS) {
			CT_LOGE(TAG, "fail to deinit %d", wres);
		}
		CT_LOG(TAG, "====== Iteration %d ends ======", count + 1);
	}

	CT_LOG(TAG, "deinit sem");	
	ct_destroy_queue(g_ct_queue);	
}