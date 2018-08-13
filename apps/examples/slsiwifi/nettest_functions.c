/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <tinyara/clock.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include <tinyara/net/dns.h>
#include <netdb.h>
#include <slsi_wifi/slsi_wifi_api.h>
#include "slsiwifi_main.h"

#ifdef CONFIG_NET_LWIP
#include <net/lwip/mem.h>
#include <net/lwip/raw.h>
#include <net/lwip/inet.h>
#include <net/lwip/dhcp.h>
#include <net/lwip/sys.h>
#include <netutils/netlib.h>
#include <net/lwip/tcpip.h>
#include <net/lwip/netif.h>
#include <net/lwip/arch/sys_arch.h>
#include <net/lwip/dhcp.h>
#include <net/lwip/icmp.h>
#include <net/lwip/ip.h>
#include <net/lwip/timeouts.h>
#include <net/lwip/inet_chksum.h>
#include <net/lwip/err.h>
#include <net/lwip/opt.h>
#endif

#define SLSI_SANITY_DHCP_TIMEOUT 15000
extern bool inAuto;
extern bool wifiStarted;
extern sem_t ap_conn_sem;
extern sem_t g_sem_join;
extern char *g_client_ip_str;

#ifdef CONFIG_SLSI_WIFI_SANITY
static struct dhcp g_dhcp_handle;
static int32_t timeleft = SLSI_SANITY_DHCP_TIMEOUT;
static struct timespec g_dhcp_timer_start;
static struct timespec g_dhcp_timer_end;
static struct netif *g_netif;
static ip_addr_t client_ip;

#define SANITY_LOOP_COUNT    20

#define SANITY_AP_OPEN_SSID "t20_sanity_open"
#define SANITY_AP_OPEN_SSID_LEN strlen(SANITY_AP_OPEN_SSID)

#define SANITY_AP_WPA_SSID "t20_sanity_wpa_wpa2"
#define SANITY_AP_WPA_SSID_LEN strlen(SANITY_AP_WPA_SSID)
#define SANITY_AP_WPA_PASS "0123456789"

#define SANITY_AP_WPA2_SSID "t20_sanity_wpa_wpa2"
#define SANITY_AP_WPA2_SSID_LEN strlen(SANITY_AP_WPA2_SSID)
#define SANITY_AP_WPA2_PASS "0123456789"

#define SANITY_AP_WEP_SSID "t20_sanity_wep"
#define SANITY_AP_WEP_SSID_LEN strlen(SANITY_AP_WEP_SSID)
#define SANITY_AP_WEP_PASS "0123456789"

#define SANITY_SOFTAP_WPA2_SSID "t20_sanity_ap_wpa2"
#define SANITY_SOFTAP_WPA2_SSID_LEN strlen(SANITY_SOFTAP_WPA2_SSID)
#define SANITY_SOFTAP_WPA2_PASS "0123456789"
#define SANITY_SOFTAP_OPEN_SSID "t20_sanity_ap_open"
#define SANITY_SOFTAP_OPEN_SSID_LEN strlen(SANITY_SOFTAP_OPEN_SSID)

static int8_t tc = 1;
#define TESTPRINT(fmt, ...) \
	printf("\n\n**************************************************************************\n" \
			"[TESTCASE %d] " fmt "\n**************************************************************************\n" \
			, tc++, ##__VA_ARGS__)

#define PING_MAX_TRY_COUNTER    10

static int ping_recv_counter = 0;
static int ping_try_counter = 0;

#define PING_RCV_TIMEO 10
#define PING_DELAY     1000
#define PING_ID        0xAFAF

#define PING_DATA_SIZE 32
#define PING_RESULT(ping_ok)

static u16_t ping_seq_num;
static clock_t ping_time;

u16_t g_ping_counter = PING_MAX_TRY_COUNTER;

static uint8_t doPing(ip_addr_t ip, int pingcount);

static void clear_timers(void)
{
	g_dhcp_timer_end.tv_nsec = 0;
	g_dhcp_timer_end.tv_sec = 0;
	g_dhcp_timer_start.tv_nsec = 0;
	g_dhcp_timer_start.tv_sec = 0;
}

static uint8_t doDhcpStart(WiFi_InterFace_ID_t mode)
{

	uint8_t result = SLSI_STATUS_ERROR;
	ip_addr_t local_ipaddr, local_netmask, local_gateway;

	g_netif = netif_find(CTRL_IFNAME);
	if (g_netif == NULL) {
		printf("doDhcpStart No network interface\n");
		return SLSI_STATUS_ERROR;
	}
	if (mode == SLSI_WIFI_STATION_IF) {
		if (g_netif->dhcp != NULL) {
			g_netif->dhcp = NULL;	//it will be set to g_dhcp_handle on next dhcp start
		}
		dhcp_set_struct(g_netif, &g_dhcp_handle);

		local_ipaddr.addr = IPADDR_ANY;
		local_netmask.addr = IPADDR_BROADCAST;
		local_gateway.addr = IPADDR_ANY;

		netif_set_addr(g_netif, &local_ipaddr, &local_netmask, &local_gateway);
		err_t res = dhcp_start(g_netif);
		if (res) {
			printf("doDhcpStart dhcp_start result %d\n", res);
			return SLSI_STATUS_ERROR;
		}
		printf("doDhcpStart start success state %d result %d\n", g_dhcp_handle.state, res);

		// To give time frtikwifior connection to establishment first
		clock_gettime(CLOCK_REALTIME, &g_dhcp_timer_start);	//start dhcp timing
		timeleft = SLSI_SANITY_DHCP_TIMEOUT;
		printf("DHCP: Awaiting ip address...(max %d seconds)\n", (timeleft / 1000));
		while (g_dhcp_handle.state != DHCP_BOUND) {
			usleep(10);
			timeleft -= 10;
			if (timeleft <= 0) {
				break;
			}
		}

		if (g_dhcp_timer_start.tv_nsec != 0 && g_dhcp_timer_start.tv_sec != 0) {
			clock_gettime(CLOCK_REALTIME, &g_dhcp_timer_end);	//stop dhcp timing, started in linkup
			// Calculate time it took
			int accum = (g_dhcp_timer_end.tv_sec - g_dhcp_timer_start.tv_sec);
			unsigned long accum2 = (unsigned long)((g_dhcp_timer_end.tv_nsec - g_dhcp_timer_start.tv_nsec) / 1000000);
			printf("DHCP request took: %d.%ul seconds\n", accum, accum2);
			clear_timers();
		}
		if (g_dhcp_handle.state == DHCP_BOUND) {
			result = SLSI_STATUS_SUCCESS;
			printf("DHCP Client - got IP address %u.%u.%u.%u\n", (unsigned char)((htonl(g_netif->ip_addr.addr) >> 24) & 0xff), (unsigned char)((htonl(g_netif->ip_addr.addr) >> 16) & 0xff), (unsigned char)((htonl(g_netif->ip_addr.addr) >> 8) & 0xff), (unsigned char)((htonl(g_netif->ip_addr.addr) >> 0) & 0xff));
			local_ipaddr.addr = g_netif->ip_addr.addr;
			local_netmask.addr = g_netif->netmask.addr;
			local_gateway.addr = g_netif->gw.addr;
			netif_set_addr(g_netif, &local_ipaddr, &local_netmask, &local_gateway);
			netif_set_up(g_netif);

		} else {
			if (timeleft <= 0) {
				printf("DHCP Client - Timeout fail to get ip address\n");
			}
		}

	} else if (mode == SLSI_WIFI_SOFT_AP_IF) {
		// Setting static IP as 192.168.47.1 in AP mode
		local_ipaddr.addr = 0x012FA8C0;
		local_netmask.addr = 0x00FFFFFF;
		local_gateway.addr = 0x012FA8C0;
		netif_set_addr(g_netif, &local_ipaddr, &local_netmask, &local_gateway);
		netif_set_up(g_netif);
		printf("doDhcpStart - dhcps_start\n");
		if (dhcps_start(g_netif) == ERR_OK) {
			result = SLSI_STATUS_SUCCESS;
			printf("DHCP Server - started success\n");
		}
	}
	return result;
}

static uint8_t doDhcpStop(WiFi_InterFace_ID_t mode)
{
	uint8_t result = SLSI_STATUS_ERROR;

	if (g_netif == NULL) {
		// Nothing to take down
		printf("doDhcpStop - nothing to stop\n");
		result = SLSI_STATUS_SUCCESS;
	} else {
		struct netif *l_netif = g_netif;
		printf("doDhcpStop\n");

		if (mode == SLSI_WIFI_STATION_IF) {
			if (l_netif->dhcp != NULL) {
				dhcp_stop(l_netif);
				printf("DHCP Client stopped \n");
			}
		} else if (mode == SLSI_WIFI_SOFT_AP_IF) {
			if (l_netif->dhcps_pcb != NULL) {
				dhcps_stop(l_netif);
				printf("DHCP Server stopped \n");
			}
		}
		g_netif = NULL;
		result = SLSI_STATUS_SUCCESS;
	}
	return result;
}

int8_t doStaJoinLeaveTest(int iterations, char *ap, char *sec, char *pass)
{
	/* Join */
	int loop = 0;
	TESTPRINT("Try to join/leave AP with name \"%s\" and secmode %s %d times\n", ap, sec, iterations);
	for (loop = 0; loop < iterations; loop++) {
		printf("==> Trial: %d\n", loop + 1);
		if (doJoin((uint8_t *)ap, strlen(ap), /*bssid */ NULL,
				   sec, pass) != SLSI_STATUS_SUCCESS) {
			inAuto = false;
			return tc - 1;
		}
		/* Leave */
		//        printf("[TESTCASE %d] Leave network\n",tc++);
		if (doLeave() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		}
	}
	return SLSI_STATUS_SUCCESS;
}

/***********************************************/
/*         DATAPATH TEST                       */
/***********************************************/

int8_t doDataPathTest(int pingcount)
{
	inAuto = true;
	if (pingcount == 0) {
		pingcount = SANITY_LOOP_COUNT;
	}
	/********* START STA **********/
	TESTPRINT("start station mode and join AP with name %s\n", SANITY_AP_WPA2_SSID);
	if (doStartSta() == SLSI_STATUS_ERROR) {
		inAuto = false;
		return tc - 1;
	}
	printf("Joining\n");
	if (doJoin((uint8_t *)SANITY_AP_WPA2_SSID, SANITY_AP_WPA2_SSID_LEN, /*bssid */ NULL,
			   "wpa2_aes", SANITY_AP_WPA2_PASS)
		!= SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}

	TESTPRINT("Start DHCPC for Station mode");
	if (doDhcpStart(SLSI_WIFI_STATION_IF) == SLSI_STATUS_SUCCESS) {
		printf("DHCP success\n");
	} else {
		printf("DHCP failed\n");
		return tc - 1;
	}
	// AP is the gateway (gw) so we can use that to ping
	client_ip.addr = g_netif->gw.addr;
	TESTPRINT("Ping gateway ip address %s", ipaddr_ntoa(&client_ip));
	if (doPing(client_ip, pingcount) == SLSI_STATUS_SUCCESS) {
		printf("Successfully pinged\n");
	} else {
		printf("Failed to ping\n");
		return tc - 1;
	}

	TESTPRINT("Stop DHCPC for Station mode");
	if (doDhcpStop(SLSI_WIFI_STATION_IF) == SLSI_STATUS_SUCCESS) {
		printf("DHCP stop success\n");
	} else {
		printf("DHCP stop failed\n");
		return tc - 1;
	}
	/* Leave */
	if (doLeave() == SLSI_STATUS_ERROR) {
		inAuto = false;
		return tc - 1;
	}
	return 0;
}

static int8_t doStopTest(void)
{
	/* Stop */
	TESTPRINT("Stop STA mode\n");

	if (doStop() == SLSI_STATUS_ERROR) {
		inAuto = false;
		return tc - 1;
	} else {
		// we need to Register Link Callback handlers again as they are
		// unregistered in doStop
		WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler);
	}
	return SLSI_STATUS_SUCCESS;
}

static int8_t doStaApModeSwitchTest(int iterations, char *sta_ap, char *ap, char *sta_sec, char *ap_sec, char *sta_pass, char *ap_pass, int ap_chan)
{
	int loop = 0;
	uint8_t result;
	sem_init(&ap_conn_sem, 0, 0);	// need to make sure it is cleared it the sta has reconnected twice
	/********* START STA, JOIN, MODESWITCH TO AP LOOP **********/
	/* Join */
	TESTPRINT("Try to modeswitch between STA mode(%s) and AP mode(%s) %d times", sta_sec, ap_sec, iterations);
	for (loop = 0; loop < iterations; loop++) {
		printf("==> Trial: %d\n", loop + 1);
		result = doStartSta();
		if (!(result == SLSI_STATUS_SUCCESS || result == SLSI_STATUS_ALREADY_STARTED)) {
			inAuto = false;
			return tc - 1;
		}
		if (doJoin((uint8_t *)sta_ap, strlen(sta_ap), /*bssid */ NULL, sta_sec,
				   sta_pass) != SLSI_STATUS_SUCCESS) {
			inAuto = false;
			sem_post(&ap_conn_sem);
			sem_destroy(&ap_conn_sem);
			return tc - 1;
		}
		/* mode switch */
		if (doStartAP(ap, ap_sec, ap_pass, ap_chan) == SLSI_STATUS_ERROR) {
			inAuto = false;
			sem_post(&ap_conn_sem);
			sem_destroy(&ap_conn_sem);
			return tc - 1;
		} else {
			printf("Waiting for STA to connect\n");
			sem_wait(&ap_conn_sem);
		}
	}
	/********* STOP STA **********/
	if (doStopTest() != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		sem_post(&ap_conn_sem);
		sem_destroy(&ap_conn_sem);
		return tc - 1;
	}
	sem_post(&ap_conn_sem);
	sem_destroy(&ap_conn_sem);

	return SLSI_STATUS_SUCCESS;
}

static int8_t doApStartStopTest(int iterations, char *ap, char *sec, char *pass, int chan)
{
	sem_init(&ap_conn_sem, 0, 0);
	TESTPRINT("Start softAP mode with SSID: \"%s\" and %s security (channel: %d)\n", ap, sec, chan);
	if (doStartAP(ap, sec, pass, chan)
		== SLSI_STATUS_ERROR) {
		inAuto = false;
		sem_destroy(&ap_conn_sem);
		return tc - 1;
	} else {
		printf("Waiting for STA to connect\n");
		sem_wait(&ap_conn_sem);
	}
	/*********** STOP **************/
	TESTPRINT("Stop softAP mode\n");
	if (doStop() == SLSI_STATUS_ERROR) {
		inAuto = false;
		sem_post(&ap_conn_sem);
		sem_destroy(&ap_conn_sem);
		return tc - 1;
	} else {
		// we need to Register Link Callback handlers again as they are
		// unregistered in doStop
		WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler);

	}
	sem_post(&ap_conn_sem);
	sem_destroy(&ap_conn_sem);
	return SLSI_STATUS_SUCCESS;
}

/***********************************************/
/*              SANITY BADCASE TEST            */
/***********************************************/

int8_t doSanityBadCaseTest(int iterations)
{
	uint8_t result;
	inAuto = true;
	if (iterations == 0) {
		iterations = SANITY_LOOP_COUNT;
	}
	/********* START STA **********/
	TESTPRINT("start station mode\n");
	result = doStartSta();
	if (result != SLSI_STATUS_SUCCESS && result != SLSI_STATUS_ALREADY_STARTED) {
		inAuto = false;
		return tc - 1;
	}
	/********* JOIN OPEN AP WITH SEC SET, JOIN CORRECTLY, LEAVE  **********/
	if (doStaJoinLeaveTest(1, SANITY_AP_OPEN_SSID, "wpa_aes", SANITY_AP_WPA_PASS) == SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc = 1;
	}
	if (doStaJoinLeaveTest(1, SANITY_AP_OPEN_SSID, "open", NULL) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc = 1;
	}
	/********* JOIN WPA2 AP WITHOUT PASS, JOIN CORRECTLY, LEAVE  **********/
	if (doStaJoinLeaveTest(1, SANITY_AP_WPA2_SSID, "wpa2_aes", NULL) == SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc = 1;
	}
	if (doStaJoinLeaveTest(1, SANITY_AP_WPA2_SSID, "wpa2_aes", SANITY_AP_WPA2_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc = 1;
	}
	/********* JOIN WPA2 AP WITH WRONG PASS, JOIN CORRECTLY, LEAVE  **********/
	if (doStaJoinLeaveTest(1, SANITY_AP_WPA2_SSID, "wpa2_aes", "WRONGWRONG") == SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc = 1;
	}
	if (doStaJoinLeaveTest(1, SANITY_AP_WPA2_SSID, "wpa2_aes", SANITY_AP_WPA2_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc = 1;
	}
	/********* STARTAP FOR ALL CHANNELS - COUNTRY=00  **********/
	int channel = 1;
	WiFiSetCountryCode("00");
	for (channel = 1; channel <= 14; channel++) {
		if (doApStartStopTest(1, SANITY_SOFTAP_WPA2_SSID, "wpa2_mixed", SANITY_SOFTAP_WPA2_PASS, channel) != SLSI_STATUS_SUCCESS) {
			if (channel < 12) {	//not allowed for country "00" so expect it to fail
				inAuto = false;
				return tc - 1;
			}
		}
	}

	/********* STARTAP FOR ALL CHANNELS - COUNTRY=KR  **********/
	WiFiSetCountryCode("KR");
	for (channel = 1; channel <= 14; channel++) {
		printf("Testing on Channel %d\n", channel);
		if (doApStartStopTest(1, SANITY_SOFTAP_WPA2_SSID, "wpa2_mixed", SANITY_SOFTAP_WPA2_PASS, channel) != SLSI_STATUS_SUCCESS) {
			if (channel != 14) {	//not allowed for country "KR" so expect it to fail
				inAuto = false;
				return tc - 1;
			}
		}
	}
	/********* STARTAP FOR ALL CHANNELS - COUNTRY=JP  **********/
	WiFiSetCountryCode("JP");
	for (channel = 1; channel <= 14; channel++) {
		if (doApStartStopTest(1, SANITY_SOFTAP_WPA2_SSID, "wpa2_mixed", SANITY_SOFTAP_WPA2_PASS, 6) != SLSI_STATUS_SUCCESS) {
			inAuto = false;
			return tc - 1;
		}
	}

	return SLSI_STATUS_SUCCESS;
}

/***********************************************/
/*              NIGHTLY SANITY TESTS           */
/***********************************************/

int8_t doNightlyTest(int iterations)
{
	inAuto = true;
	tc = 0;
	if (iterations == 0) {
		iterations = SANITY_LOOP_COUNT;
	}
	if (doSanityTest(iterations) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	if (doSanityBadCaseTest(iterations) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	inAuto = false;
	return SLSI_STATUS_SUCCESS;
}

/***********************************************/
/*              SANITY TEST                    */
/***********************************************/

int8_t doSanityTest(int iterations)
{
	uint8_t result;
	inAuto = true;
	tc = 0;
	if (iterations == 0) {
		iterations = SANITY_LOOP_COUNT;
	}
	if (wifiStarted) {
		/*  we need to cleanp before starting the test */
		printf("need to stop running wpa_supplicant before start\n");
		doStop();
		// we need to Register Link Callback handlers again as they are
		// unregistered in doStop
		WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler);

	}
	/*(1-4)********* DATAPATH TESTS *********** */
	if (doDataPathTest(iterations) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}

	/*(5)******** START STA ********* */
	TESTPRINT("start station mode\n");
	result = doStartSta();
	if (result != SLSI_STATUS_SUCCESS && result != SLSI_STATUS_ALREADY_STARTED) {
		inAuto = false;
		return tc - 1;
	}
	/*(6)******** SCAN ********* */
	TESTPRINT("start scan for devices\n");
	if (doScan() != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/*(7)******** JOIN OPEN, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_OPEN_SSID, "open", NULL) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc = 1;
	}
	/*(8)******** JOIN WEP, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_WEP_SSID, "wep", SANITY_AP_WEP_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc = 1;
	}
	/*(9)******** JOIN WPA_TKIP, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_WPA_SSID, "wpa_tkip", SANITY_AP_WPA_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc = 1;
	}
	/*(10)******** JOIN WPA_AES, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_WPA_SSID, "wpa_aes", SANITY_AP_WPA_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc = 1;
	}
	/*(11)******** JOIN WPA_MIXED, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_WPA_SSID, "wpa_mixed", SANITY_AP_WPA_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc = 1;
	}
	/*(12)******** JOIN WPA2_AES, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_WPA2_SSID, "wpa2_aes", SANITY_AP_WPA2_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc = 1;
	}
	/*(13)******** JOIN WPA2_MIXED, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_WPA2_SSID, "wpa2_mixed", SANITY_AP_WPA2_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc = 1;
	}

	/*(14)******** STOP STA ********* */
	if (doStopTest() != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	} else {
		// we need to Register Link Callback handlers again as they are
		// unregistered in doStop
		WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler);
	}

	sem_init(&ap_conn_sem, 0, 0);	// need to make sure it is cleared it the sta has reconnected twice
	/*(15)******** START STA, JOIN, MODESWITCH TO AP LOOP ********* */
	if (doStaApModeSwitchTest(iterations, SANITY_AP_WPA2_SSID, SANITY_SOFTAP_OPEN_SSID, "wpa2_aes", "open", SANITY_AP_WPA2_PASS, NULL, 6) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/*(16-17)************ START AP, STOP AP LOOP, CHANNEL 6 ******* */

	if (doApStartStopTest(iterations, SANITY_SOFTAP_WPA2_SSID, "wpa2_mixed", SANITY_SOFTAP_WPA2_PASS, 6) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}

	inAuto = false;
	return SLSI_STATUS_SUCCESS;
}

#endif							// CONFIG_SLSI_WIFI_SANITY

int8_t doAutoTest(char *filename)
{
	int8_t result = SLSI_STATUS_ERROR;
	inAuto = true;
	if (wifiStarted) {
		/*  we need to cleanp before starting the test */
		printf("need to stop running wpa_supplicant before start\n");
		doStop();
		// we need to Register Link Callback handlers again as they are
		// unregistered in doStop
		WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler);
	}
	if (filename) {
		FILE *fp;
		char line[250];
		size_t len = 250;
		ssize_t read;

		fp = fopen(filename, "r");
		if (fp == NULL) {
			printf("could not open test input file\n");
			inAuto = false;
			return result;
		}

		while ((read = readline(&line, len, fp, stdout)) != -1) {
			printf("%s\n", line);
			char *args = strtok(line, " ");
			char *pArgs = args;
			int8_t count = 0;
			while (pArgs++) {
				count++;
			}
			parseCmdLine(count, &args);
		}

		fclose(fp);

	} else {
		/* Station mode */
		printf("[1] start station mode\n");
		if (doStartSta() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return result;
		}
		/* Scan */
		printf("[2] start scan for devices\n");
		if (doScan() != SLSI_STATUS_SUCCESS) {
			inAuto = false;
			return result;
		}
		/* Join */
		printf("[3] Try to join AP with name \"setup\"\n");
		if (doJoin((uint8_t *)"setup", 5, /*bssid */ NULL, "open", 0) != SLSI_STATUS_SUCCESS) {
			inAuto = false;
			return result;
		}
		/* Leave */
		printf("[4] Leave network\n");
		if (doLeave() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return result;
		}
		/* Stop */
		printf("[5] Stop station mode\n");

		if (doStop() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return result;
		} else {
			// we need to Register Link Callback handlers again as they are
			// unregistered in doStop
			WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler);
		}
		/* AP mode */
		printf("[6] Start softAP mode with SSID: \"t20_auto_test\" and open security\n");
		if (doStartAP("t20_auto_test", "open", NULL, 6) == SLSI_STATUS_ERROR) {
			inAuto = false;
			return result;
		}
		/* Stop */
		printf("[7] Stop softAP mode\n");
		if (doStop() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return result;
		}

	}
	printf("\n\tWiFi Auto Test Result : SUCCESS\n\n");
	inAuto = false;
	return SLSI_STATUS_SUCCESS;
}

int slsi_tcp_server(void)
{
	int socket_desc, client_sock, c, read_size;
	struct sockaddr_in server, client;
	char client_message[100];

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		printf("Could not create socket");
		return -1;
	}
	printf("Socket created");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	//Bind
	if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
		//print the error message
		perror("bind failed. Error");
		close(socket_desc);
		return 1;
	}
	printf("bind done");

	//Listen
	if (listen(socket_desc, 3) < 0) {
		printf("listen failed\n");
		close(socket_desc);
		return 1;
	}
	//Accept and incoming connection
	printf("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	//accept connection from an incoming client
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
	if (client_sock < 0) {
		perror("accept failed");
		close(socket_desc);
		return 1;
	}
	printf("Connection accepted");

	//Receive a message from client
	while ((read_size = recv(client_sock, client_message, 100, 0)) > 0) {
		//Send the message back to client
		printf("recv message %s\n", client_message);
		if (send(client_sock, client_message, strlen(client_message), 0) < 0) {
			printf("failed to send message\n");
			break;
		}
	}

	if (read_size == 0) {
		printf("client disconnected");
	} else if (read_size == -1) {
		printf("recv failed");
	}

	close(client_sock);
	close(socket_desc);

	return 0;
}

int slsi_udp_server(void)
{
	int udpSocket, nBytes;
	char buffer[100];
	struct sockaddr_in serverAddr;
	struct sockaddr_in serverStorage;
	socklen_t addr_size;

	/*Create UDP socket */
	udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (udpSocket == -1) {
		nlldbg("Could not create socket");
		return -1;
	}

	/*Configure settings in address struct */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7891);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	/*Bind socket with address struct */
	if (bind(udpSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
		printf("failed to bind\n");
		close(udpSocket);
		return 0;
	}

	/*Initialize size variable to be used later on */
	addr_size = sizeof serverStorage;

	while (1) {
		/* Try to receive any incoming UDP datagram. Address and port of
		   requesting client will be stored on serverStorage variable */
		nBytes = recvfrom(udpSocket, buffer, 100, 0, (struct sockaddr *)&serverStorage, &addr_size);

		/*Send uppercase message back to client, using serverStorage as the address */
		if (sendto(udpSocket, buffer, nBytes, 0, (struct sockaddr *)&serverStorage, addr_size) < 0) {
			printf("failed to sendto\n");
			break;
		}
	}

	close(udpSocket);
	return 0;
}

int slsi_tcp_client(void)
{
	int sock;
	struct sockaddr_in server;
	char message[100], server_reply[100];

	nlldbg("start tcp client...\n");
	//Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		nlldbg("Could not create socket");
		return -1;
	}
	nlldbg("Socket created");

	server.sin_addr.s_addr = inet_addr("192.168.0.219");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to remote server
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		nlldbg("connect failed. Error");
		close(sock);
		return 1;
	}

	nlldbg("Connected\n");

	//keep communicating with server
	//while (1)
	{
		printf("Enter message : ");
		strncpy(message, "message", sizeof(message) - 1);

		//Send some data
		if (send(sock, message, strlen(message), 0) < 0) {
			printf("Send failed");
			close(sock);
			return 1;
		}
		//Receive a reply from the server
		if (recv(sock, server_reply, 100, 0) < 0) {
			printf("recv failed");
		}

		printf("Server reply %s:", server_reply);
	}

	close(sock);
	return 0;
}

int slsi_udp_client(void)
{
	int clientSocket, nBytes;
	char buffer[100];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	nlldbg("start udpclient\n");

	/*Create UDP socket */
	clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

	if (clientSocket == -1) {
		nlldbg("Could not create socket");
		return -1;
	}
	/*Configure settings in address struct */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7891);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.0.219");
	//memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	/*Initialize size variable to be used later on */
	addr_size = sizeof serverAddr;

	while (1) {
		nlldbg("Type a sentence to send to server:\n");
		strncpy(buffer, "test hello\n", 11);
		nlldbg("You typed: %s", buffer);

		nBytes = strlen(buffer) + 1;

		/*Send message to server */
		if (sendto(clientSocket, buffer, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size) < 0) {
			printf("failed to sendto UDP server\n");
			break;
		}

		/*Receive message from server */
		if ((nBytes = recvfrom(clientSocket, buffer, 100, 0, NULL, NULL)) < 0) {
			printf("failed to recvfrom UDP server\n");
			break;
		}
		UNUSED(nBytes);
		nlldbg("Received from server: %s\n", buffer);
		up_mdelay(3000);

	}
	close(clientSocket);
	return 0;
}

#ifdef SLSI_WIFI_SANITY
/** Prepare a echo ICMP request */
static void ping_prepare_echo(struct icmp_echo_hdr *iecho, u16_t len)
{
	size_t i;
	size_t data_len = len - sizeof(struct icmp_echo_hdr);

	ICMPH_TYPE_SET(iecho, ICMP_ECHO);
	ICMPH_CODE_SET(iecho, 0);
	iecho->chksum = 0;
	iecho->id = PING_ID;
	iecho->seqno = htons(++ping_seq_num);

	/* fill the additional data buffer with some data */
	for (i = 0; i < data_len; i++) {
		((char *)iecho)[sizeof(struct icmp_echo_hdr) + i] = (char)i;
	}

	iecho->chksum = inet_chksum(iecho, len);
}

/* Ping using the socket ip */
static err_t ping_send(int s, ip_addr_t *addr)
{
	int err;
	struct icmp_echo_hdr *iecho;
	struct sockaddr_in to;
	size_t ping_size = sizeof(struct icmp_echo_hdr) + PING_DATA_SIZE;
	LWIP_ASSERT("ping_size is too big", ping_size <= 0xffff);
	iecho = (struct icmp_echo_hdr *)mem_malloc((mem_size_t)ping_size);
	if (!iecho) {
		return ERR_MEM;
	}

	ping_prepare_echo(iecho, (u16_t)ping_size);

	to.sin_len = sizeof(to);
	to.sin_family = AF_INET;
	inet_addr_from_ipaddr(&to.sin_addr, addr);

	err = sendto(s, iecho, ping_size, 0, (struct sockaddr *)&to, sizeof(to));

	mem_free(iecho);

	return (err ? ERR_OK : ERR_VAL);
}

static uint8_t ping_recv(int s)
{
	char *buf;
	int fromlen, len = 0;
	struct sockaddr_in from;
	struct ip_hdr *iphdr;
	struct icmp_echo_hdr *iecho;
	int ping_size = sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr) + PING_DATA_SIZE;

	fromlen = sizeof(struct sockaddr_in);

	buf = (char *)mem_malloc(sizeof(char) * ping_size);
	if (!buf) {
		printf("failed to allocate memory\n");
		return SLSI_STATUS_ERROR;
	}

	len = recvfrom(s, buf, ping_size, 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);
	if (len >= (int)(sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr))) {
		ip_addr_t fromaddr;

		iphdr = (struct ip_hdr *)buf;
		iecho = (struct icmp_echo_hdr *)(buf + (IPH_HL(iphdr) * 4));

		if (iecho->type == ICMP_ER) {
			inet_addr_to_ipaddr(&fromaddr, &from.sin_addr);
			ping_recv_counter++;
#ifdef CONFIG_SYSTEM_TIME64
			printf(" %d bytes from %d.%d.%d.%d: icmp_seq=%d ttl=255 time=%llu ms\n", len, fromaddr.addr >> 0 & 0xff, fromaddr.addr >> 8 & 0xff, fromaddr.addr >> 16 & 0xff, fromaddr.addr >> 24 & 0xff, ping_recv_counter, ((sys_now() - ping_time) * USEC_PER_TICK) / 1000);
#else
			printf(" %d bytes from %d.%d.%d.%d: icmp_seq=%d ttl=255 time=%d ms\n", len, fromaddr.addr >> 0 & 0xff, fromaddr.addr >> 8 & 0xff, fromaddr.addr >> 16 & 0xff, fromaddr.addr >> 24 & 0xff, ping_recv_counter, ((sys_now() - ping_time) * USEC_PER_TICK) / 1000);
#endif

			if ((iecho->id == PING_ID) && (iecho->seqno == htons(ping_seq_num))) {
				/* do some ping result processing */
				PING_RESULT((ICMPH_TYPE(iecho) == ICMP_ER));
				if (buf) {
					mem_free(buf);
				}
				return SLSI_STATUS_SUCCESS;
			} else {
				printf("drop\n");
				return SLSI_STATUS_ERROR;
			}
		}
	}
	mem_free(buf);

	if (len <= 0) {
		printf("ping: recv timeout\n");
		return SLSI_STATUS_ERROR;
	}
	/* do some ping result processing */
	PING_RESULT(0);
	return SLSI_STATUS_SUCCESS;
}

static uint8_t ping_thread(ip_addr_t ping_target)
{
	int s;
	struct timeval tv;
	int ret;
	int fail_count = 0;
	int max_fail;

	if (g_ping_counter < 10) {
		max_fail = 1;
	} else {
		max_fail = 5;
	}

	if ((s = socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP)) < 0) {
		printf("fail to create raw socket...\n");
		return SLSI_STATUS_ERROR;
	}

	tv.tv_sec = PING_RCV_TIMEO;
	tv.tv_usec = 0;

	ret = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));
	if (ret < 0) {
		printf("fail to setsockopt ret:%d\n", ret);
		close(s);
		return SLSI_STATUS_ERROR;
	}

	while (1) {
		ping_try_counter++;

		if (ping_send(s, &ping_target) == ERR_OK) {
			ping_time = sys_now();
			if (ping_recv(s) != SLSI_STATUS_SUCCESS) {
				fail_count++;
				if (fail_count == max_fail) {
					close(s);
					return SLSI_STATUS_ERROR;
				} else {
					printf("failure %d of %d allowed\n", fail_count, max_fail);
				}
			}
		}
		//sys_msleep(PING_DELAY);
		usleep(1000 * PING_DELAY);
		if (ping_try_counter == g_ping_counter) {
			break;
		}
	}

	close(s);
	return SLSI_STATUS_SUCCESS;
}

static uint8_t doPing(ip_addr_t ip, int pingcount)
{
	ping_recv_counter = 0;
	ping_try_counter = 0;
	g_ping_counter = pingcount;
	return ping_thread(ip);
}
#endif
