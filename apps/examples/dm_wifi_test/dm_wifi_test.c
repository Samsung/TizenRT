/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdlib.h>
#include <tinyara/config.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <dm/dm_connectivity.h>
#include <dm/dm_error.h>

#include <apps/shell/tash.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define DEVICE_NAME	"ATK/002"	// "ATK" or "STA"
#define SERVER_IP	"192.168.1.117"
#define SERVER_PORT	5987

#define WIFI_BSSID_SIZE	17
#define BUF_MAX_SIZE	128

char aps_list[] = {
#include "ap_check_list.txt"
};

typedef struct {
	int id;
	char bssid[WIFI_BSSID_SIZE + 1];
	int8_t rssi;
} Ap_t;

static Ap_t *g_aps = NULL;
static int aps_cnt = 0;

static int isWiFiConnected;
/****************************************************************************
 * Prototype Functions
 ****************************************************************************/
extern int wifi_test_main(int argc, char *argv[]);
static int wifi_test_proc(int argc, char *argv[]);

void parse_aps_list(void);
static void printScanResult(dm_scan_info_t *list);

void WiFi_Link_Up(void);
void WiFi_Link_Down(void);

/****************************************************************************
 * Private Data & Functions
 ****************************************************************************/
void WiFi_Link_Up(void)
{
	isWiFiConnected = 1;
	printf("\n[WiFi] Connected!\n");
}

void WiFi_Link_Down(void)
{
	isWiFiConnected = 0;
	printf("\n[WiFi] Disconnected!\n");
}

void parse_aps_list(void)
{
	char *ptr;
	char *p;
	int i;
	int c;
	int cnt;

	char id[5];
	char buf[WIFI_BSSID_SIZE + 1];

	cnt = 0;
	for (i = 0; i < strlen(aps_list); i++) {
		if (aps_list[i] == ';') {
			cnt++;
		}
	}

	g_aps = (Ap_t *)malloc(sizeof(Ap_t) * cnt);
	aps_cnt = cnt;

	cnt = 0;
	ptr = strtok(aps_list, ";");
	while (ptr != NULL) {
		c = 0;
		p = id;
		for (i = 0; i < strlen(ptr); i++) {
			if (ptr[i] == ',') {
				p[c] = '\0';
				g_aps[cnt].id = (int)atoi(id);
				p = buf;
				c = 0;
				continue;
			}
			p[c++] = ptr[i];
		}
		p[c] = '\0';
		strncpy(g_aps[cnt].bssid, buf, WIFI_BSSID_SIZE + 1);

		cnt++;

		ptr = strtok(NULL, ";");
	}
}

static void printScanResult(dm_scan_info_t *list)
{

	dm_scan_info_t *current_element = list;

	int i;
	for (i = 0; i < aps_cnt; i++) {
		g_aps[i].rssi = 0;
	}
	while (current_element) {
		for (i = 0; i < aps_cnt; i++) {
			if (strncmp(g_aps[i].bssid, current_element->bssid, WIFI_BSSID_SIZE) == 0) {
				g_aps[i].rssi = current_element->rssi;
				printf("[WiFi] Bingo! - %d : %s : %d\n", g_aps[i].id, g_aps[i].bssid, g_aps[i].rssi);
			}
		}
	}
}

int wifi_test_proc(int argc, char *argv[])
{
	int fd;
	int i;
	int ret;
	struct sockaddr_in dest;
	char buf[BUF_MAX_SIZE];
	char payload[BUF_MAX_SIZE];

	memset(&dest, 0, sizeof(struct sockaddr_in));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr(SERVER_IP);
	dest.sin_port = htons(SERVER_PORT);

	//Init
	isWiFiConnected = 0;
	ret = 0;
	dm_scan_info_t *scan_info = NULL;
	dm_cb_register_init();

	if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		printf("[WiFi] Open Socket Failed!\n");
		ret = -1;
		goto error_out;
	}
	//Auto Connection
	dm_conn_wifi_connect(WiFi_Link_Up, WiFi_Link_Down);

	while (1) {
		if (isWiFiConnected == 1) {
			break;
		}
		printf("[WiFi] Waiting Connect......\n");
		sleep(1);
	}
	dm_conn_dhcp_init();
	sleep(5);

	if (g_aps == NULL) {
		parse_aps_list();
	}

	printf("[WiFi Pre-fixed APs : %d / %p]\n", aps_cnt, (void *)g_aps);
	for (i = 0; i < aps_cnt; i++) {
		printf("[WiFi] %d %s\n", g_aps[i].id, g_aps[i].bssid);
	}

	printf("[WiFi] Let's Do Wifi Scan procedure\n");

	while (1) {
		/* Type in code to perform WiFi scan. Do check and
		 * handle appropriately
		 */

		/* Type in code to get WiFi scan data. Do check and
		 * handle appropriately
		 */

		memset(payload, 0, BUF_MAX_SIZE);

		printScanResult(scan_info);
		snprintf(payload, BUF_MAX_SIZE - 1, "/%s|", DEVICE_NAME);
		for (i = 0; i < aps_cnt; i++) {
			memset(buf, 0, BUF_MAX_SIZE);
			snprintf(buf, BUF_MAX_SIZE - 1, "%02d:%d/", g_aps[i].id, g_aps[i].rssi);
			if (BUF_MAX_SIZE - strlen(payload) > strlen(buf)) {
				strncat(payload, buf, strlen(buf) + 1);
			} else {
				break;
			}
		}
		printf("[WiFi] PAYLOAD : %s\n", payload);
		if (sendto(fd, payload, strlen(payload), 0, (struct sockaddr *)&dest, sizeof(struct sockaddr_in)) == -1) {
			printf("[WiFi] Failed to sendto UDP.\n");
			ret = -1;
			closesocket(fd);
			goto error_out;
		}
		/* Type in code to free WiFi scan data. Do check and
		 * handle appropriately
		 */

		sleep(5);
	}

	printf("[WiFi] APIs Test Done.\n");
	
	
error_out:
	if (g_aps) {
		free(g_aps);
	}

	/* Type in code to free WiFi scan data, if not done already. Do check and
	 * handle appropriately
	 */

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int wifi_init_task(int argc, char *argv[])
{
	/* Create Task For async job */
	if (task_create("wifi_test_proc", SCHED_PRIORITY_DEFAULT, 16840, wifi_test_proc, argv) < 0) {
		/* Error : Can't Create task */
		printf("WiFi APIs Test is not started\n");
		return -1;
	}

	return 0;
}
/****************************************************************************
 * wifi_test_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int wifi_test_main(int argc, char *argv[])
#endif
{
#ifdef CONFIG_TASH
	tash_cmd_install("wifi_scan", wifi_init_task, TASH_EXECMD_SYNC);
#else
	wifi_init_task(argc, argv);
#endif

	return 0;
}
