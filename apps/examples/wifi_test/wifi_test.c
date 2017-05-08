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
 * examples/hello_tash/hello_tash_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/socket.h>

#include <dm/dm_connectivity.h>
#include <dm/dm_error.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define DEVICE_NAME 	"ATK/002" 		// "ATK" or "STA"
#define SERVER_IP	"192.168.1.117"
#define SERVER_PORT 	5987

#define WIFI_BSSID_SIZE	17
#define BUF_MAX_SIZE 	128

#ifdef __has_include
#  if __has_include("ap_check_list.txt")
#    define AP_CHECK_LIST
char aps_list[] = {
#    include "ap_check_list.txt"
};

typedef struct {
    int id;
    char bssid[WIFI_BSSID_SIZE + 1];
    int8_t rssi;
} Ap_t;

static Ap_t *aps = NULL;
static int aps_cnt = 0;
#  else
#    undef AP_CHECK_LIST
#  endif
#endif

static int isWiFiConnected;
/****************************************************************************
 * Prototype Functions
 ****************************************************************************/
extern int wifi_test_main(int argc, char *argv[]);
static int wifi_test_proc(int argc, char *argv[]);

#ifdef AP_CHECK_LIST
void parse_aps_list(void);
#endif
static void printScanResult(dm_scan_info_t *list);

void WiFi_Link_Up(void);
void WiFi_Link_Down(void);

/****************************************************************************
 * Private Data & Functions
 ****************************************************************************/
void WiFi_Link_Up() {
    isWiFiConnected = 1;
    printf("\n[WiFi] Connected!\n");
}

void WiFi_Link_Down() {
    isWiFiConnected = 0;
    printf("\n[WiFi] Disconnected!\n");
}

#ifdef AP_CHECK_LIST
void parse_aps_list(void) {
    char *ptr, *p;
    int i, c, cnt = 0;

    char id[5];
    char buf[WIFI_BSSID_SIZE + 1];

    for (i = 0;i < strlen(aps_list);i++) {
	if (aps_list[i] == ';') cnt++;
    }

    aps = (Ap_t *)malloc(sizeof(Ap_t) * cnt);
    aps_cnt = cnt;

    cnt = 0;
    ptr = strtok(aps_list, ";");
    while(ptr != NULL){
        c = 0;
	p = id;
	for (i = 0;i < strlen(ptr);i++) {
	    if (ptr[i] == ',') {
		p[c] = '\0';
		aps[cnt].id = (int)atoi(id);
		p = buf;
		c = 0;
		continue;
	    }
	    p[c++] = ptr[i];
	}
	p[c] = '\0';
	strncpy(aps[cnt].bssid, buf, WIFI_BSSID_SIZE + 1);

	cnt++;

        ptr = strtok(NULL, ";");
    }
}
#endif

static void printScanResult(dm_scan_info_t *list) {

    dm_scan_info_t *current_element = list;

#ifdef AP_CHECK_LIST
    int i;
    for (i = 0;i < aps_cnt;i++) {
	aps[i].rssi = 0;
    }
    while (current_element) {
	for (i = 0;i < aps_cnt;i++) {
	    if (strncmp(aps[i].bssid, current_element->bssid, WIFI_BSSID_SIZE) == 0) {
		aps[i].rssi = current_element->rssi;
		printf("[WiFi] Bingo! - %d : %s : %d\n", aps[i].id, aps[i].bssid, aps[i].rssi);
	    }
	}
#else
    int count = 0;
    while (current_element) {	
        printf("%d) %s %d \n",count, current_element->bssid, current_element->rssi);
        current_element = current_element->next;
#endif
    }
}

int wifi_test_proc(int argc, char *argv[])
{
	int fd;
	struct sockaddr_in dest;
	char buf[BUF_MAX_SIZE];
	char payload[BUF_MAX_SIZE];

	memset(&dest, 0, sizeof(struct sockaddr_in));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr(SERVER_IP);
	dest.sin_port = htons(SERVER_PORT);

	//Init
	isWiFiConnected = 0;
	dm_scan_info_t *scan_info = NULL;
	dm_cb_register_init();

	if ((fd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
	    printf("[WiFi] Open Socket Failed!\n");
	    return -1;
	}

	//Auto Connection
	dm_conn_wifi_connect(WiFi_Link_Up, WiFi_Link_Down);
	
	while(1) {
	    if (isWiFiConnected == 1) {
		break;	
	    }
	    printf("[WiFi] Waiting Connect......\n");
	    sleep(1);
	}
	dm_conn_dhcp_init();
	sleep(5);
	
#ifdef AP_CHECK_LIST
	//Parsing AP List from Array
	int i;
	if (aps == NULL)
		parse_aps_list();

	printf("[WiFi Pre-fixed APs : %d / %p]\n", aps_cnt, (void *)aps);
	for (i = 0;i < aps_cnt;i++) 
	    printf("[WiFi] %d %s\n", aps[i].id, aps[i].bssid);
#endif
	
	//AP Scan	
	printf("[WiFi] Let's Do Wifi Scan\n");
		
	while(1) {
		// Type in code to perform WiFi scan. Please check return code
		// and handle appropriately.
		
		
		
		
		// Type in code to read WiFi scan data. Please check return code
		// and handle appropriately.
		
		
		

		memset(buf, 0, BUF_MAX_SIZE);
		memset(payload, 0, BUF_MAX_SIZE);

		printScanResult(scan_info);
#ifdef AP_CHECK_LIST
		sprintf(payload, "/%s|", DEVICE_NAME);
		for (i = 0;i < aps_cnt;i++) {
		    sprintf(buf, "%02d:%d/", aps[i].id, aps[i].rssi);
		    strcat(payload, buf);		
		}
#else
		sprintf(payload, "%s", DEVICE_NAME);
#endif
		printf("[WiFi] PAYLOAD : %s\n", payload);
		if (sendto(fd, payload, strlen(payload) , 0 , (struct sockaddr *)&dest, sizeof(struct sockaddr_in)) == -1)
		{
		    printf("[WiFi] Failed to sendto UDP.\n");
		    return -1;
		}

		// Type in code to free WiFi scan data. Please check return code
		// and handle appropriately.
		
		
		
		
		sleep(5);
	}

	printf("[WiFi] APIs Test Done.\n");
#ifdef AP_CHECK_LIST
	free(aps);
#endif

	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * wifi_test_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int wifi_test_main(int argc, char *argv[])
#endif
{

	int pid;

	/*
	 * Create Task For async job
	 */
	pid = task_create("wifi_test_proc", SCHED_PRIORITY_DEFAULT, 16840, wifi_test_proc, argv);
	if (pid < 0) {
		/*
		 * Error : Can't Create task
		 */
		printf("WiFi APIs Test is not started, err = %d\n", pid);
	}

	return 0;
}
