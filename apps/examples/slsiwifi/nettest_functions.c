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
#include <slsi_wifi_api.h>

#include "slsiwifi_main.h"

#ifdef CONFIG_NET_LWIP
#include <apps/netutils/netlib.h>
#include <net/lwip/mem.h>
#include <net/lwip/raw.h>
#include <net/lwip/ipv4/inet.h>		// TODO: ipv4 check
#include <net/lwip/sys.h>
#include <net/lwip/tcpip.h>
#include <net/lwip/netif.h>
#include <net/lwip/arch/sys_arch.h>
#include <net/lwip/ipv4/icmp.h>		// TODO: ipv4 check
#include <net/lwip/ipv4/ip.h>		// TODO: ipv4 check
#include <net/lwip/timers.h>		// TODO: ipv4 check (timeouts.h)
#include <net/lwip/ipv4/inet_chksum.h>		// TODO: ipv4 check
#include <net/lwip/err.h>
#include <net/lwip/opt.h>
#endif

#define SLSI_SANITY_DHCP_TIMEOUT 15000
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

#define SANITY_P2P_CON_OUT_ADDR "72:2c:1f:26:df:70"
#define SANITY_P2P_CON_OUT_INTENT_LOW 1
#define SANITY_P2P_CON_OUT_INTENT_HIGH 15
#define SANITY_P2P_CON_OUT_PIN "0123456789"
#define SANITY_P2P_CON_OUT_CONF_PBC "pbc"

#define TESTPRINT(fmt, ...) \
	printf("\n\n****************************************************************\n" \
	"[TESTCASE %d] " fmt "\n****************************************************************\n" \
	, tc++, ##__VA_ARGS__)

#define PING_MAX_TRY_COUNTER    10
#define PING_RCV_TIMEO 10
#define PING_DELAY     1000
#define PING_ID        0xAFAF

#define PING_DATA_SIZE 32
#define PING_RESULT(ping_ok)

extern bool inAuto;
extern bool wifiStarted;
extern slsi_vendor_ie_t *g_vsie;
extern sem_t ap_conn_sem;
u16_t g_ping_counter = PING_MAX_TRY_COUNTER;

static uint8_t doPing(ip_addr_t ip, int pingcount);

int8_t doStaJoinLeaveTest(int iterations, char *ap, char *sec, char *pass)
{
	/* Join */
	int loop = 0;

	TESTPRINT("Try to join/leave AP with name \"%s\" and secmode %s %d times\n", ap, sec, iterations);
	for (loop = 0; loop < iterations; loop++) {
		printf("==> Trial: %d\n", loop + 1);
		if (doJoin((uint8_t *)ap, strlen(ap), NULL /* bssid */, sec, pass) != SLSI_STATUS_SUCCESS) {
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

int8_t doStaJoinStopTest(char *ap, char *sec, char *pass)
{
	/* Join */
	TESTPRINT("Try to join AP with name \"%s\" and secmode %s and then stop without leave\n", ap, sec);
	if (doJoin((uint8_t *)ap, strlen(ap), NULL /* bssid */, sec, pass) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/* Leave */
	//        printf("[TESTCASE %d] Leave network\n",tc++);
	if (doStop() == SLSI_STATUS_ERROR) {
		inAuto = false;
		return tc - 1;
	} else {
		/* we need to Register Link Callback handlers again as they are unregistered in doStop */
		WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler);
	}
	return SLSI_STATUS_SUCCESS;
}

static int8_t doStopTest(void)
{
	/* Stop */
	TESTPRINT("Stop STA mode\n");

	if (doStop() == SLSI_STATUS_ERROR) {
		inAuto = false;
		return tc - 1;
	} else {
		/* we need to Register Link Callback handlers again as they are unregistered in doStop */
		WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler);
	}
	return SLSI_STATUS_SUCCESS;
}

static int8_t doVsieTest(char *oui, char *data)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (data == NULL) {
		printf("data is NULL\n");
		return result;
	}
	printf("Storing new OUI=%s IE=%s\n", oui, data);
	if (g_vsie == NULL) {
		g_vsie = (slsi_vendor_ie_t *)zalloc(sizeof(slsi_vendor_ie_t));
		if (g_vsie == NULL) {
			printf("Could not allocate buffer for vendor IE\n");
			return result;
		}
	}
	char *poui = oui;

	if (hex2uint8(poui, &g_vsie->oui[0]) != TRUE || hex2uint8(poui + 2, &g_vsie->oui[1]) != TRUE || hex2uint8(poui + 4, &g_vsie->oui[2]) != TRUE) {
		printf("OUI hex representation not valid\n");
		return result;
	}
	if (g_vsie->content != NULL) {
		free(g_vsie->content);
	}
	g_vsie->content = (uint8_t *)zalloc(strlen(data));
	if (g_vsie->content == NULL) {
		printf("Could not allocate buffer for vendor IE\n");
		return result;
	}
	g_vsie->content_length = strlen(data);
	memcpy(g_vsie->content, data, strlen(data));
	return SLSI_STATUS_SUCCESS;
}

static int8_t doStaApModeSwitchTest(int iterations, char *sta_ap, char *ap, char *sta_sec, char *ap_sec, char *sta_pass, char *ap_pass, int ap_chan)
{
	int loop = 0;
	uint8_t result;

	/********* START STA, JOIN, MODESWITCH TO AP LOOP **********/
	/* Join */
	TESTPRINT("Try to modeswitch between STA mode(%s)\n and AP mode(%s) %d times", sta_sec, ap_sec, iterations);
	for (loop = 0; loop < iterations; loop++) {
		sem_init(&ap_conn_sem, 0, 0);	/* need to make sure it is cleared it the sta has reconnected twice */
		printf("==> Trial: %d\n", loop + 1);
		result = doStartSta();
		if (!(result == SLSI_STATUS_SUCCESS || result == SLSI_STATUS_ALREADY_STARTED)) {
			inAuto = false;
			sem_post(&ap_conn_sem);
			sem_destroy(&ap_conn_sem);
			return tc - 1;
		}
		if (doJoin((uint8_t *)sta_ap, strlen(sta_ap), NULL /* bssid */, sta_sec, sta_pass) != SLSI_STATUS_SUCCESS) {
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
		}
		printf("Waiting for STA to connect\n");
		if (sem_wait(&ap_conn_sem)) {
			printf("sem_wait failed:");    /* cause DOUBLE LOCK issue in static analysis tool */
		}
		sem_post(&ap_conn_sem);
		sem_destroy(&ap_conn_sem);
	}
	/********* STOP STA **********/
	if (doStopTest() != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	return SLSI_STATUS_SUCCESS;
}

static int8_t doApStartConnectStopTest(int iterations, char *ap, char *sec, char *pass, int chan)
{
	sem_init(&ap_conn_sem, 0, 0);
	TESTPRINT("Start softAP mode with SSID: \"%s\"\n and %s security (channel: %d)\n", ap, sec, chan);
	if (doStartAP(ap, sec, pass, chan) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		sem_post(&ap_conn_sem);
		sem_destroy(&ap_conn_sem);
		return tc - 1;
	} else {
		printf("Waiting for STA to connect\n");
		if (sem_wait(&ap_conn_sem)) {
			printf("sem_wait failed");
		}
	}
	/*********** STOP **************/
	printf("Stop softAP mode\n");
	if (doStop() != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		sem_post(&ap_conn_sem);
		sem_destroy(&ap_conn_sem);
		return tc - 1;
	} else {
		/* we need to Register Link Callback handlers again as they are unregistered in doStop */
		WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler);
	}
	sem_post(&ap_conn_sem);
	sem_destroy(&ap_conn_sem);
	return SLSI_STATUS_SUCCESS;
}

static int8_t doApStartStopTest(int iterations, char *ap, char *sec, char *pass, int chan)
{
	TESTPRINT("Start softAP mode with SSID: \"%s\"\n and %s security (channel: %d)\n", ap, sec, chan);
	if (doStartAP(ap, sec, pass, chan) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	} else {
		printf("Sleep before stop\n");
		sleep(1);
	}
	/*********** STOP **************/
	printf("Stop softAP mode\n");
	if (doStop() != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	} else {
		/* we need to Register Link Callback handlers again as they are unregistered in doStop */
		WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler);
	}
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
		return tc - 1;
	}
	if (doStaJoinLeaveTest(1, SANITY_AP_OPEN_SSID, "open", NULL) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/********* JOIN WPA2 AP WITHOUT PASS, JOIN CORRECTLY, LEAVE  **********/
	if (doStaJoinLeaveTest(1, SANITY_AP_WPA2_SSID, "wpa2_aes", NULL) == SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	if (doStaJoinLeaveTest(1, SANITY_AP_WPA2_SSID, "wpa2_aes", SANITY_AP_WPA2_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/********* JOIN WPA2 AP WITH WRONG PASS, JOIN CORRECTLY, LEAVE  **********/
	if (doStaJoinLeaveTest(1, SANITY_AP_WPA2_SSID, "wpa2_aes", "WRONGWRONG") == SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	if (doStaJoinLeaveTest(1, SANITY_AP_WPA2_SSID, "wpa2_aes", SANITY_AP_WPA2_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}

	return SLSI_STATUS_SUCCESS;
}

/***********************************************/
/*              NIGHTLY SANITY TESTS           */
/***********************************************/
int8_t doNightlyTest(int iterations, char *softAPname, char *p2pGOname)
{
	inAuto = true;
	tc = 0;
	if (iterations == 0) {
		iterations = SANITY_LOOP_COUNT;
	}
	if (doSanityTest(iterations, softAPname) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
#ifdef CONFIG_SLSI_WIFI_P2P_API
	if (doP2pSanityTest(iterations, p2pGOname) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
#endif
	inAuto = false;
	return SLSI_STATUS_SUCCESS;
}

/***********************************************/
/*              SANITY TEST                    */
/***********************************************/
#ifdef CONFIG_SLSI_WIFI_P2P_API
int8_t doP2pSanityTest(int iterations, char *p2pName)
{
	inAuto = true;
	if (iterations == 0) {
		iterations = SANITY_LOOP_COUNT;
	}
	if (wifiStarted) {
		/* we need to cleanp before starting the test */
		printf("need to stop running wpa_supplicant before start\n");
		doStop();
		/* we need to Register Link Callback handlers again as they are unregistered in doStop */
		WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
	}

	/*(1)******** START/STOP TEST ********* */
	if (doP2pStartStopTest(iterations) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}

	/*(21)*********** START P2P, FIND, STOP P2P ******* */
	if (doP2pFindTest(iterations, NULL, 10) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/*(22)************ START P2P, LISTEN, STOP LISTEN,  STOP P2P ******* */
	if (doP2pListenStopTest(iterations, NULL, 10) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/*(21)*********** START P2P, FIND, STOP FIND, STOP P2P ******* */
	if (doP2pFindStopTest(iterations, NULL, 10) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/*(22)******** START STA ********* */
	TESTPRINT("start station mode after P2P mode\n");
	int8_t result = doStartSta();

	if (result != SLSI_STATUS_SUCCESS && result != SLSI_STATUS_ALREADY_STARTED) {
		inAuto = false;
		return tc - 1;
	}
	if (doStopTest() != SLSI_STATUS_SUCCESS && result) {
		inAuto = false;
		return tc - 1;
	}
	/* add group looping all channel with different country codes */
	if (doP2pChannelTest() != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	inAuto = false;
	return SLSI_STATUS_SUCCESS;
}
#endif	/* CONFIG_SLSI_WIFI_P2P_API */

/***********************************************/
/*              SANITY TEST                    */
/***********************************************/
int8_t doSanityTest(int iterations, char *softAPname)
{
	uint8_t result;

	inAuto = true;
	tc = 0;
	if (iterations == 0) {
		iterations = SANITY_LOOP_COUNT;
	}
	if (wifiStarted) {
		/* we need to cleanp before starting the test */
		printf("need to stop running wpa_supplicant before start\n");
		doStop();
		/* we need to Register Link Callback handlers again as they are unregistered in doStop */
		WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler);
	}
	/*(1)********* DATAPATH TESTS *********** */
/*	if (doDataPathTest(iterations) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
*/
	/*(2)******** START STA ********* */
	TESTPRINT("start station mode\n");
	result = doStartSta();
	if (result != SLSI_STATUS_SUCCESS && result != SLSI_STATUS_ALREADY_STARTED) {
		inAuto = false;
		return tc - 1;
	}
	/*(3)******** SCAN ********* */
	TESTPRINT("start scan for devices\n");
	if (doScan() != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/*(4)******** JOIN OPEN, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_OPEN_SSID, "open", NULL) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/*(5)******** JOIN WEP, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_WEP_SSID, "wep", SANITY_AP_WEP_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/*(6)******** JOIN WPA_TKIP, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_WPA_SSID, "wpa_tkip", SANITY_AP_WPA_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/*(7)******** JOIN WPA_AES, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_WPA_SSID, "wpa_aes", SANITY_AP_WPA_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/*(8)******** JOIN WPA_MIXED, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_WPA_SSID, "wpa_mixed", SANITY_AP_WPA_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/*(9)******** JOIN WPA2_AES, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_WPA2_SSID, "wpa2_aes", SANITY_AP_WPA2_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/*(10)******** JOIN WPA2_MIXED, LEAVE LOOP ********* */
	if (doStaJoinLeaveTest(iterations, SANITY_AP_WPA2_SSID, "wpa2_mixed", SANITY_AP_WPA2_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
	/*(11)******** STOP STA ********* */
	if (doStaJoinStopTest(SANITY_AP_WPA2_SSID, "wpa2_mixed", SANITY_AP_WPA2_PASS) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	} else {
		/* we need to Register Link Callback handlers again as they are unregistered in doStop */
		WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler);
	}

	doVsieTest("001122", "SLSI SANITY TEST");
	sem_init(&ap_conn_sem, 0, 0);	/* need to make sure it is cleared it the sta has reconnected twice */
	/*(12)******** START STA, JOIN, MODESWITCH TO AP LOOP ********* */
	int ssidlen = 0;
	char *ssid = NULL;

	if (softAPname != NULL) {
		ssidlen = strlen(SANITY_SOFTAP_OPEN_SSID) + 1 /* underscore */+ strlen(softAPname) + 1 /* null */;
		ssid = (char *)zalloc(ssidlen);
		if (ssid != NULL) {
			snprintf(ssid, ssidlen, "%s_%s", SANITY_SOFTAP_OPEN_SSID, softAPname);
		} else {
			inAuto = false;
			return tc - 1;
		}
	} else {
		ssid = strdup(SANITY_SOFTAP_OPEN_SSID);
	}
	if (doStaApModeSwitchTest(iterations, SANITY_AP_WPA2_SSID, ssid, "wpa2_aes", "open", SANITY_AP_WPA2_PASS, NULL, 6) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		free(ssid);	/* ssid is only allocated if softAP is set */
		return tc - 1;
	}
	free(ssid);
	ssid = NULL;
	/*(13)************ START AP, STOP AP LOOP, CHANNEL 6 ******* */
	if (softAPname != NULL) {
		ssidlen = strlen(SANITY_SOFTAP_WPA2_SSID) + 1 /* underscore */+ strlen(softAPname) + 1 /* null */;
		ssid = (char *)zalloc(ssidlen);
		if (ssid != NULL) {
			snprintf(ssid, ssidlen, "%s_%s", SANITY_SOFTAP_WPA2_SSID, softAPname);
		}
	} else {
		ssid = strdup(SANITY_SOFTAP_WPA2_SSID);
	}
	if (doApStartConnectStopTest(iterations, ssid, "wpa2_mixed", SANITY_SOFTAP_WPA2_PASS, 6) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		if (softAPname != NULL) {
			free(ssid);	/* ssid is only allocated if softAP is set */
		}
		return tc - 1;
	}
	free(ssid);
	ssid = NULL;

	if (softAPname != NULL) {
		ssidlen = strlen(SANITY_SOFTAP_WPA2_SSID) + 1 /* underscore */+ strlen(softAPname) + 1 /* null */;
		ssid = (char *)zalloc(ssidlen);
		if (ssid != NULL) {
			snprintf(ssid, ssidlen, "%s_%s", SANITY_SOFTAP_WPA2_SSID, softAPname);
		}
	} else {
		ssid = strdup(SANITY_SOFTAP_WPA2_SSID);
	}
	/*(14-28)******* STARTAP FOR ALL CHANNELS - COUNTRY=00  ********* */
	int channel = 1;

	WiFiSetCountryCode("00");
	printf("WiFiSetCountryCode 00\n");
	for (channel = 1; channel <= 14; channel++) {
		if (doApStartStopTest(1, ssid, "wpa2_mixed", SANITY_SOFTAP_WPA2_PASS, channel) != SLSI_STATUS_SUCCESS) {
			if (channel < 12) {	/* not allowed for country "00" so expect it to fail */
				inAuto = false;
				return tc - 1;
			} else {
				printf("WiFiSetCountryCode 00 - expected fail for channel: %d\n", channel);
				inAuto = true;
			}
		} else {
			/* Channels above 12 should fail */
			if (channel >= 12) {
				inAuto = false;
				return tc - 1;
			}
		}
	}
	/*(29-33)******* STARTAP FOR ALL CHANNELS - COUNTRY=KR  ********* */
	WiFiSetCountryCode("KR");
	printf("WiFiSetCountryCode KR\n");
	for (channel = 1; channel <= 14; channel++) {
		printf("Testing on Channel %d\n", channel);
		if (doApStartStopTest(1, ssid, "wpa2_mixed", SANITY_SOFTAP_WPA2_PASS, channel) != SLSI_STATUS_SUCCESS) {
			if (channel != 14) {	/* not allowed for country "KR" so expect it to fail */
				inAuto = false;
				return tc - 1;
			} else {
				printf("WiFiSetCountryCode KR - expected fail for channel: %d\n", channel);
				inAuto = true;
			}
		} else {
			/* Channel 14 should fail */
			if (channel == 14) {
				inAuto = false;
				return tc - 1;
			}
		}
	}
	/*(34-47)******* STARTAP FOR ALL CHANNELS - COUNTRY=JP  ********* */
	WiFiSetCountryCode("JP");
	printf("WiFiSetCountryCode JP\n");
	for (channel = 1; channel <= 14; channel++) {
		if (doApStartStopTest(1, ssid, "wpa2_mixed", SANITY_SOFTAP_WPA2_PASS, channel) != SLSI_STATUS_SUCCESS) {
			inAuto = false;
			return tc - 1;
		}
	}
	WiFiSetCountryCode("00");
	printf("WiFiSetCountryCode 00\n");
	free(ssid);
	ssid = NULL;
#ifdef CONFIG_SLSI_WIFI_P2P_API
	if (doP2pSanityTest(iterations, NULL) != SLSI_STATUS_SUCCESS) {
		inAuto = false;
		return tc - 1;
	}
#endif
	inAuto = false;
	return SLSI_STATUS_SUCCESS;
}

int8_t doAutoTest(char *filename)
{
	int8_t result = SLSI_STATUS_ERROR;

	inAuto = true;
	if (wifiStarted) {
		/* we need to cleanp before starting the test */
		printf("need to stop running wpa_supplicant before start\n");
		doStop();
		/* we need to Register Link Callback handlers again as they are unregistered in doStop */
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
		if (doJoin((uint8_t *) "setup", 5, NULL /* bssid */, "open", 0) != SLSI_STATUS_SUCCESS) {
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
			/* we need to Register Link Callback handlers again as they are unregistered in doStop */
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

	/* Create socket */
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		printf("Could not create socket");
		return -1;
	}
	printf("Socket created");

	/* Prepare the sockaddr_in structure */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);
	/* Bind */
	if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
		/* print the error message */
		perror("bind failed. Error");
		close(socket_desc);
		return 1;
	}
	printf("bind done");

	/* Listen */
	if (listen(socket_desc, 3) < 0) {
		printf("listen failed\n");
		close(socket_desc);
		return 1;
	}
	/* Accept and incoming connection */
	printf("Waiting for incoming connections...");

	/* accept connection from an incoming client */
	c = sizeof(struct sockaddr_in);
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
	if (client_sock < 0) {
		perror("accept failed");
		close(socket_desc);
		return 1;
	}
	printf("Connection accepted");

	/* Receive a message from client */
	while ((read_size = recv(client_sock, client_message, 100, 0)) > 0) {
		/* Send the message back to client */
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

	/* Create UDP socket */
	udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (udpSocket == -1) {
		nlldbg("Could not create socket");
		return -1;
	}

	/* Configure settings in address struct */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7891);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	/* Bind socket with address struct */
	if (bind(udpSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
		printf("failed to bind\n");
		close(udpSocket);
		return 0;
	}

	/* Initialize size variable to be used later on */
	addr_size = sizeof(serverStorage);

	while (1) {
		/*
		 * Try to receive any incoming UDP datagram. Address and port of
		 * requesting client will be stored on serverStorage variable
		 */
		nBytes = recvfrom(udpSocket, buffer, 100, 0, (struct sockaddr *)&serverStorage, &addr_size);

		/* Send uppercase message back to client, using serverStorage as the address */
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
	/* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		nlldbg("Could not create socket");
		return -1;
	}
	nlldbg("Socket created");

	server.sin_addr.s_addr = inet_addr("192.168.0.219");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	/* Connect to remote server */
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		nlldbg("connect failed. Error");
		close(sock);
		return 1;
	}

	nlldbg("Connected\n");

	/* keep communicating with server */
	//while(1)
	{
		printf("Enter message : ");
		strncpy(message, "message", sizeof(message) - 1);

		/* Send some data */
		if (send(sock, message, strlen(message), 0) < 0) {
			printf("Send failed");
			close(sock);
			return 1;
		}
		/* Receive a reply from the server */
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

	/* Create UDP socket */
	clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

	if (clientSocket == -1) {
		nlldbg("Could not create socket");
		return -1;
	}
	/* Configure settings in address struct */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7891);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.0.219");
	//memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	/* Initialize size variable to be used later on */
	addr_size = sizeof(serverAddr);

	while (1) {
		nlldbg("Type a sentence to send to server:\n");
		strncpy(buffer, "test hello\n", 11);
		nlldbg("You typed: %s", buffer);
		nBytes = strlen(buffer) + 1;

		/* Send message to server */
		if (sendto(clientSocket, buffer, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size) < 0) {
			printf("failed to sendto UDP server\n");
			break;
		}

		/* Receive message from server */
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
