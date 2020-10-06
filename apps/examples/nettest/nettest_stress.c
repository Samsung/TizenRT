/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <time.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <arpa/inet.h>

/*
 *  * HEAPINFO
 *   */
#define PRINT_HEAPINFO 0

#if PRINT_HEAPINFO
#include <tinyara/mm/mm.h>
static int disconnect_count = 0;
#endif

/*
 *  * NETWORK
 *   */
#define AF_INETX AF_INET
#define PF_INETX PF_INET
#define PKT_NUM 10
#define BUF_SIZE 301
#define SELECT_TIME_OUT	100000

/*
 *  * WiFi
 *   */
#define WIFI_AUTO_CONNECT 1

#if WIFI_AUTO_CONNECT
#include <wifi_manager/wifi_manager.h>

#define WIFI_SSID "Gorani"
#define WIFI_PASSPHRASE "jonbeo1@"
static sem_t g_wifi_sem = SEM_INITIALIZER(0);

#define WIFITEST_SIGNAL                                          \
	do {                                                         \
		sem_post(&g_wifi_sem);                      \
		printf("%s: T%d send signal\n", __FUNCTION__, getpid()); \
	} while (0)

#define WIFITEST_WAIT                                            \
	do {                                                         \
		printf("%s: T%d wait signal\n", __FUNCTION__, getpid()); \
		sem_wait(&g_wifi_sem);                      \
	} while (0)

static void wifi_sta_connected(wifi_manager_result_e result)
{
	WIFITEST_SIGNAL;
}

static void wifi_sta_disconnected(wifi_manager_disconnect_e disconn)
{
	WIFITEST_SIGNAL;
}

static void wifi_scan_ap_done(wifi_manager_scan_info_s **scan_info, wifi_manager_scan_result_e res)
{
	/* Make sure you copy the scan results onto a local data structure.
	 *	 * It will be deleted soon eventually as you exit this function.
	 *		 */
	if (res == WIFI_SCAN_FAIL) {
		WIFITEST_SIGNAL;
		return;
	}
	wifi_manager_scan_info_s *wifi_scan_iter = *scan_info;
	while (wifi_scan_iter != NULL) {
		printf("SSID: %-20s, BSSID: %-20s, RSSI: %d, CH: %d, Phy_type: %d\n", \
				wifi_scan_iter->ssid, wifi_scan_iter->bssid, wifi_scan_iter->rssi, \
				wifi_scan_iter->channel, wifi_scan_iter->phy_mode);
		wifi_scan_iter = wifi_scan_iter->next;
	}
	WIFITEST_SIGNAL;
}
static wifi_manager_cb_s wifi_callbacks = {
	wifi_sta_connected,
	wifi_sta_disconnected,
	NULL,
	NULL,
	wifi_scan_ap_done,
};
#endif

static int g_sockfd;
static struct sockaddr_in g_myaddr;
static int terminate_flag = 0;
static int sending_now = 0;
static int retry_sender = 0;
static int retry_receiver = 0;
static sem_t g_sem_signal;

void *sender_thread(void *arg)
{
	int ret = 0;
	int i = 0;
	uint32_t sbuf_size = 0;
	char buf[BUF_SIZE];
	memset(buf, 0xff, BUF_SIZE);
	sbuf_size = BUF_SIZE;


	/* Then send PKT_NUM number of messages */
	while (1) {
		printf("[SEND] Start to send\n");
		for (i = 0; i < PKT_NUM; i++) {
			sending_now = 1;
			int send_sum = 0;
			while (send_sum < BUF_SIZE) {
				ret = send(g_sockfd, buf + send_sum, sbuf_size - send_sum, 0);
				if (ret <= 0) {
					if (ret == 0) {
						printf("[SEND] connection closed\n");
						goto terminate;
					}
					printf("[SEND] socket error ret(%d) err(%d)\n", ret, errno);
					if (errno == ECONNRESET || errno == EBADF) {
						retry_sender = 1;
					}
					goto terminate;
				}
				send_sum += ret;
			}
			printf("[SEND] send msg #%d nbytes=%d\n", i+1, send_sum);
		}
		printf("[SEND] Wait Response\n");
		sending_now = 0;
		sem_wait(&g_sem_signal);
	}

	printf("[SEND] Closed Successfully\n");

	sending_now = 0;
	return 0;

terminate:
	printf("[SEND] Closed due to fail\n");
	return 0;
}

void *receiver_thread(void *arg)
{
	int count = 0;
	int recv_len = 0;
	int nbytes = 0;
	int nbytes_sum = 0;
	int ret;
	char msg[BUF_SIZE];
	fd_set rfds;
	fd_set fds;

	FD_ZERO(&fds);
	FD_SET(g_sockfd, &fds);

	recv_len = BUF_SIZE;
	while (!terminate_flag) {

		struct timeval timeout = { .tv_usec = SELECT_TIME_OUT };
		rfds = fds;
		ret = select(g_sockfd + 1, &rfds, NULL, NULL, &timeout);
		printf("[RECV] Select out (timeout? %d)\n", ret);
		if (ret < 0) {
			printf("[RECV] Select failed (%d)\n", errno);
			sem_post(&g_sem_signal);
			close(g_sockfd);
			return 0;
		}


		if (FD_ISSET(g_sockfd, &rfds)) {
			printf("[RECV] - required data size %d\n", BUF_SIZE - nbytes_sum);
			nbytes = recv(g_sockfd, msg + nbytes_sum, recv_len - nbytes_sum, 0);
			if (nbytes <= 0) {
				printf("[RECV] received failed (errno=%d, nbytes=%d)\n", errno, nbytes);
				if (errno == ECONNRESET) {
					retry_receiver = 1;
				} else if (errno == EAGAIN) {
					continue;
				}
				close(g_sockfd);
				sem_post(&g_sem_signal);
				return 0;
			}
			nbytes_sum += nbytes;
			if (nbytes_sum < BUF_SIZE) {
				continue;
			}
			nbytes_sum = 0;

			count++;
			printf("[RECV] - Received Msg # %d] Received Msg (%d)\n", count, nbytes_sum);
			if (count == PKT_NUM) {
				sem_post(&g_sem_signal);
				count = 0;
			}
		}
	}
	printf("[RECV] Closed successfully \n");
	sem_post(&g_sem_signal);
	close(g_sockfd);

	return 0;
}

void *disconnector_thread(void *arg)
{
	printf("[DISC] SHUTDOWN START\n");
	shutdown(g_sockfd, SHUT_RDWR);
	printf("[DISC] SHUTDOWN DONE\n");
#if PRINT_HEAPINFO
	disconnect_count++;
	if (disconnect_count == 10) {
		heapinfo_parse_heap(BASE_HEAP, HEAPINFO_SIMPLE, HEAPINFO_PID_ALL);
		disconnect_count = 0;
	}
#endif
	terminate_flag = 1;
	return 0;
}

void nettest_stress(char *t_addr, int port)
{
	pthread_t sender;
	pthread_t receiver;
	pthread_t disconnector;
	int ret = 0;
	int iter = 0;
	int connect_trial = 0;
	int addrlen = sizeof(struct sockaddr_in);

#if WIFI_AUTO_CONNECT
	/* Connect Info
	 * SSID : Gorani
	 * PASS : jonbeo1@
	 */
	wifi_manager_result_e wret = WIFI_MANAGER_FAIL;
	wret = wifi_manager_init(&wifi_callbacks);
	if (wret != WIFI_MANAGER_SUCCESS) {
		printf("WiFi connection failed\n");
		return;
	}

	wifi_manager_ap_config_s config;
	config.ssid_length = strlen(WIFI_SSID);
	config.passphrase_length = strlen(WIFI_PASSPHRASE);
	strncpy(config.ssid, WIFI_SSID, config.ssid_length + 1);
	strncpy(config.passphrase, WIFI_PASSPHRASE, config.passphrase_length + 1);
	config.ap_auth_type = 3;
	config.ap_crypto_type = 3;
	/* current wifi mode is station, then this try will succeed */
	wret = wifi_manager_connect_ap(&config);
	if (wret != WIFI_MANAGER_SUCCESS) {
		printf("WiFi connection failed\n");
		return;
	}
	WIFITEST_WAIT;
	printf("Wi-Fi connection established\n");
#endif

	g_myaddr.sin_family = AF_INET;
	g_myaddr.sin_port = htons(port);
	inet_pton(AF_INET, t_addr, &(g_myaddr.sin_addr));

	while (1) {
		terminate_flag = 0;
		retry_sender = 0;
		retry_receiver = 0;
		printf("\n\n[MAIN] Iteration START %d\n", iter + 1);
		g_sockfd = socket(PF_INET, SOCK_STREAM, 0);
		if (g_sockfd < 0) {
			printf("[MAIN] TCP socket failure %d\n", errno);
			return;
		}

		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		if (setsockopt(g_sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) == -1) {
			printf("[MAIN] rcv timeout set failed\n");
			close(g_sockfd);
			return;
		}

		printf("[MAIN] Connecting...\n");
		if (connect(g_sockfd, (struct sockaddr *)&g_myaddr, addrlen) < 0) {
			printf("[MAIN] connect %d-th fail: %d\n", ++connect_trial, errno);
			close(g_sockfd);
			sleep(1);
			continue;
		}
		connect_trial = 0;
		sem_init(&g_sem_signal, 0, 0);

		int tmp = 7 + (rand() % 11);
		printf("[MAIN] SHUTDOWN after %d\n", tmp);

		ret = pthread_create(&sender, NULL, sender_thread, NULL);
		if (ret != 0) {
			printf("[MAIN] Failed to start sender_thread (%d)\n", ret);
			sem_destroy(&g_sem_signal);
			return;
		}

		ret = pthread_create(&receiver, NULL, receiver_thread, NULL);
		if (ret != 0) {
			printf("[MAIN] Failed to start receiver_thread (%d)\n", ret);
			sem_destroy(&g_sem_signal);
			return;
		}

		for (int j = 0; j < tmp; j++) {
			if (retry_sender && retry_receiver) {
				break;
			}
			sleep(1);
		}

		ret = pthread_create(&disconnector, NULL, disconnector_thread, NULL);
		if (ret != 0) {
			printf("[MAIN] Failed to start disconnector_thread (%d)\n", ret);
			sem_destroy(&g_sem_signal);
			return;
		}

		int status = pthread_join(sender, NULL);
		if (status != 0) {
			printf("[MAIN] ERROR sender pthread_join failed, status=%d\n", status);
		} else {
			printf("[MAIN] sender closed\n");
		}
		status = pthread_join(receiver, NULL);
		if (status != 0) {
			printf("[MAIN] ERROR receiver pthread_join failed, status=%d\n", status);
		} else {
			printf("[MAIN] receiver closed\n");
		}
		status = pthread_join(disconnector, NULL);
		if (status != 0) {
			printf("[MAIN] ERROR disconnector pthread_join failed, status=%d\n", status);
		} else {
			printf("[MAIN] disconnector closed\n");
		}
		printf("[MAIN] Iteration FINISH %d\n", iter + 1);
		iter++;
		sem_destroy(&g_sem_signal);
#if WIFI_AUTO_CONNECT
		wifi_manager_deinit();
#endif

	}
}
