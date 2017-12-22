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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <slsi_wifi_api.h>
#include <slsi_wifi_utils.h>
#ifndef CONFIG_DEBUG_FEATURES	/* the mm functions are badly guarded with CONFIG_DEBUG */
#undef CONFIG_EXAMPLES_SLSIDEMO_MEM_CHECK
#endif
#ifdef CONFIG_EXAMPLES_SLSIDEMO_MEM_CHECK
#include <tinyara/mm/mm.h>
#endif

#include "output_functions.h"
#include "slsiwifi_main.h"

/****************************************************************************
 * Defines
 ****************************************************************************/
#define DEBUG                  0
#define SLSI_AP_MODE_NAME      "SLSI Test AP"
#define min(a, b)              (((a) < (b)) ? (a) : (b))
#define MAXLEN(a, b)           (((a) > (b)) ? (a) : (b))
#define MODE_STRING_MAX        100
#define TOUPPER(character)     (((character) >= 'a') && ((character) <= 'z') ? ((character) - 0x20) : (character))
#define STATE_DISCONNECTED     0
#define STATE_CONNECTED        1
#define WPA_MAX_SSID_LEN       (4 * 32 + 1)	/* SSID encoded in a string - worst case is all 4-octet hex digits + '\0' */

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
static int g_connection_state = STATE_DISCONNECTED;
static WiFi_InterFace_ID_t g_mode;
static uint8_t numStations;
static slsi_vendor_ie_t *g_vsie;
static uint8_t g_join_result;

bool inAuto;
bool wifiStarted;
char *g_client_ip_str;
sem_t ap_conn_sem;
sem_t g_sem_result;
sem_t g_sem_join;

/****************************************************************************
 * Private Functions
 ***************************************************************************/

static bool hex2uint8(const char *str, u8 *ret)
{
	uint16_t idx = 0;

	*ret = 0;
	if ((str[idx] == '0') && (TOUPPER(str[idx + 1]) == 'X')) {
		str += 2;
	}
	if (((str[idx] >= '0') && (str[idx] <= '9')) || ((TOUPPER(str[idx]) >= 'A') && (TOUPPER(str[idx]) <= 'F'))) {
		while (((str[idx] >= '0') && (str[idx] <= '9')) || ((TOUPPER(str[idx]) >= 'A') && (TOUPPER(str[idx]) <= 'F'))) {
			*ret = (u8)(*ret * 16 + (((str[idx] >= '0') && (str[idx] <= '9')) ? str[idx] - '0' : TOUPPER(str[idx]) - 'A' + 10));
			idx++;
			if (idx >= 2) {
				break;
			}
		}
		return true;
	}
	return false;
}

#ifdef CONFIG_EXAMPLES_SLSIDEMO_MEM_CHECK
static struct mm_heap_s *g_user_heap;
int g_memstat_total = 0;

static int getMemUsage(void)
{
	g_user_heap = mm_get_heap_info();
	printf("\n\tCurrent memory usage (total): %d bytes\n", g_user_heap->total_alloc_size);
	return g_user_heap->total_alloc_size;
}

static void dumpHeapMemory(void)
{
	struct mm_heap_s *heap = mm_get_heap_info();
	int *startAddr = (int *)heap->mm_heapstart[0];
	int *endAddr = (int *)heap->mm_heapend[0];
	int i;

	printf("Heap memory dump:");
	for (i = 0; (startAddr + i) != endAddr; i++) {
		if (!(i % 8)) {
			printf("\n%08x: ", (startAddr + i));
		}
		printf("%08x ", *(startAddr + i));
	}
	printf("\n");
}

int getMemLeaks(void)
{
	usleep(100000);
	int memFinal = getMemUsage();
	int result = memFinal - g_memstat_total;
	if (result < -128 || result > 0) {
		/* 128 is the number of bytes we have seen being there in */
		/* situations where the the blocks are freed while exiting */
		/* print heapinfo */
		heapinfo_parse(mm_get_heap_info(), HEAPINFO_TRACE);
		printf(" ______________________________________________________________ \n");
		printf("|                                                              |\n");
		printf("| WARNING: %6.6d bytes leaked during this run                 |\n", result);
		printf("| Use the command 'heapinfo -v' to see the leaking address     |\n");
		printf("|______________________________________________________________|\n");
	} else {
		result = SLSI_STATUS_SUCCESS;
	}
	return result;
}
#endif

static bool check_security_str(char *sec)
{
	bool ret = FALSE;

	if (!sec) {
		return ret;
	}
	int len = 0;
	int i = 0;
	char *m[] = { "open",
			"wep", "wep_shared",
			"wpa_tkip", "wpa_aes", "wpa_mixed",
			"wpa2_tkip", "wpa2_aes", "wpa2_mixed"
	};

	len = sizeof(m) / sizeof(m[0]);
	for (i = 0; i < len; ++i) {
		if (!strncmp(m[i], sec, strlen(sec))) {
			ret = TRUE;
			break;
		}
	}
	return ret;
}

void sw_linkUpHandler(slsi_reason_t *reason)
{
	g_connection_state = STATE_CONNECTED;
	if (g_mode == SLSI_WIFI_STATION_IF) {
		char connectedApName[WPA_MAX_SSID_LEN];

		g_join_result = reason->reason_code;	/* store result code for main thread */
		memset(connectedApName, 0, WPA_MAX_SSID_LEN);
		printf_encode(connectedApName, WPA_MAX_SSID_LEN, reason->ssid, reason->ssid_len);
		printf("Connected to network: bssid: %s, ssid: %s\n", reason->bssid, connectedApName);
		sem_post(&g_sem_join);	/* tell the main thread to move on */
	} else {
		printf("New Station connected bssid: %s \n", reason->bssid);
		WiFiIsConnected(&numStations, NULL);
		if (inAuto) {	/* we need to post to the sanity test that we have a connection */
			sem_post(&ap_conn_sem);
		}
	}
}

void sw_linkDownHandler(slsi_reason_t *reason)
{
	g_connection_state = STATE_DISCONNECTED;
	if (g_mode == SLSI_WIFI_STATION_IF) {
		if (reason) {
			printf("Disconnected from network %s reason_code: %d %s\n", reason->bssid, reason->reason_code, reason->locally_generated ? "(locally_generated)" : "");
		} else {
			printf("Disconnected from network\n");
		}
		sem_post(&g_sem_join);	/* need to tell the system that we have a link-down. Needed for doLeave() */
	} else if (g_mode == SLSI_WIFI_SOFT_AP_IF) {
		printf("Station %s disconnected from network with reason_code: %d\n", reason->bssid, reason->reason_code);
		WiFiIsConnected(&numStations, NULL);
	}
}

int8_t sw_ScanResultHandler(slsi_reason_t *reason)
{
	if (reason->reason_code == 0) {
		slsi_scan_info_t *results;
		WiFiGetScanResults(&results);
		printScanResult(results);
		WiFiFreeScanResults(&results);
	} else {
		printf("Scan failed reason: %d, locally_generated: %d\n", reason->reason_code, reason->locally_generated);
	}
	sem_post(&g_sem_result);
	return SLSI_STATUS_SUCCESS;
}

int8_t doStartSta(void)
{
	int8_t result = SLSI_STATUS_ERROR;

	result = WiFiStart(SLSI_WIFI_STATION_IF, NULL);
	if (result == SLSI_STATUS_SUCCESS) {
		printf("STA mode started successfully\n");
		g_mode = SLSI_WIFI_STATION_IF;
		wifiStarted = true;
	} else if (result == SLSI_STATUS_ALREADY_STARTED) {
		g_mode = SLSI_WIFI_STATION_IF;
		wifiStarted = true;
		printf("STA mode already started\n");
	} else {
		printf("Failed to start STA mode (error: %d)\n", result);
	}
	return result;
}

int8_t doStop(void)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (WiFiStop() == SLSI_STATUS_SUCCESS) {
		wifiStarted = false;
		g_mode = SLSI_WIFI_NONE;
		g_connection_state = STATE_DISCONNECTED;
		result = SLSI_STATUS_SUCCESS;
		WiFiRegisterLinkCallback(NULL, NULL);
		WiFiRegisterScanCallback(NULL);
		if (g_vsie) {
			if (g_vsie->content) {
				free(g_vsie->content);
			}
			free(g_vsie);
			g_vsie = NULL;
		}
		printf("Stopped STA/SoftAP/P2P mode\n");
	} else {
		printf("Failed to stop STA/SoftAP mode (not started?)\n");
	}
	return result;
}

/* @ssid - An ASCII character str - hence will not ssid's with e.g. 0 */
int8_t doJoin(uint8_t *ssid, uint8_t ssid_len, uint8_t *bssid, char *sec, char *passphrase)
{
	int8_t result = SLSI_STATUS_ERROR;
	sem_init(&g_sem_join, 0, 0);
	char *quotedpass = NULL;

	/* We need to check if the passphrase is ascii if we are using wep/wep_shared */
	if (strncmp("wep", sec, 3) == 0) {	/* covers both wep and wep_shared */
		if (strlen(passphrase) == 5 || strlen(passphrase) == 13) {
			/* we expect it to be ascii in WEP mode -- add quotes */
			int passlen = strlen(passphrase);

			quotedpass = (char *)zalloc(passlen + 3);	/* make room for quotes */
			if (!quotedpass) {
				return result;
			}
			quotedpass[0] = '"';
			memcpy(&quotedpass[1], passphrase, passlen);
			quotedpass[passlen + 1] = '"';
			quotedpass[passlen + 2] = '\0';
			passphrase = quotedpass;
		}
	}
	slsi_security_config_t *conf = getSecurityConfig(sec, passphrase, g_mode);
	result = WiFiNetworkJoin(ssid, ssid_len, bssid, conf);
	if (wifiStarted && result == SLSI_STATUS_SUCCESS) {
		result = SLSI_STATUS_SUCCESS;
		if (sem_wait(&g_sem_join)) {
			printf("sem_wait failed");    /* wait for it to join */
		}

		if (!g_join_result) {
			printf("Successfully joined the network with SSID %s\n", ssid);
		} else {
			printf("Join failed - reason %d\n", g_join_result);
			result = g_join_result;
		}
	} else {
		printf("Failed to join the network %s. Maybe you forgot to do \"startsta\" (error code: %d)\n", ssid, result);
		result = g_join_result;
	}
	free(conf);
	free(quotedpass);
	sem_destroy(&g_sem_join);
	return result;
}

int8_t doLeave(void)
{
	int8_t result = SLSI_STATUS_ERROR;

	sem_init(&g_sem_join, 0, 0);
	if (wifiStarted && WiFiNetworkLeave() == SLSI_STATUS_SUCCESS) {
		if (sem_wait(&g_sem_join)) {
			printf("sem_wait failed\n");
		}
		result = SLSI_STATUS_SUCCESS;
		printf("Left network\n");
	} else {
		printf("Failed to leave network or no network to leave. Maybe you forgot to do \"startsta\"\n");
	}
	sem_destroy(&g_sem_join);
	return result;
}

int8_t doScan(void)
{
	int8_t result = SLSI_STATUS_ERROR;

	sem_init(&g_sem_result, 0, 0);
	WiFiRegisterScanCallback(sw_ScanResultHandler);
	if (wifiStarted && WiFiScanNetwork() == SLSI_STATUS_SUCCESS) {
		result = SLSI_STATUS_SUCCESS;
		printf("Successfully started scan...waiting for result!\n");
		if (sem_wait(&g_sem_result)) {
			printf("sem_wait failed");
		}
	} else {
		printf("Failed to start scan for networks. maybe you forgot to do \"startsta/startap\"\n");
	}
	sem_destroy(&g_sem_result);
	return result;
}

int8_t doSpecificScan(uint8_t *ssid, uint8_t ssid_len, char *passphrase, char *sec)
{
	int8_t result = SLSI_STATUS_ERROR;
	char *quotedpass = NULL;

	sem_init(&g_sem_result, 0, 0);
	/* We need to check if the passphrase is ascii if we are using wep/wep_shared */
	if (strncmp("wep", sec, 3) == 0) {	/* covers both wep and wep_shared */
		if (strlen(passphrase) == 5 || strlen(passphrase) == 13) {
			/* we expect it to be ascii in WEP mode -- add quotes */
			int passlen = strlen(passphrase);

			quotedpass = (char *)zalloc(passlen + 3);	/* make room for quotes */
			if (!quotedpass) {
				return result;
			}
			quotedpass[0] = '"';
			memcpy(&quotedpass[1], passphrase, passlen);
			quotedpass[passlen + 1] = '"';
			quotedpass[passlen + 2] = '\0';
			passphrase = quotedpass;
		}
	}
	slsi_security_config_t *conf = getSecurityConfig(sec, passphrase, g_mode);

	WiFiRegisterScanCallback(sw_ScanResultHandler);
	if (wifiStarted && WiFiScanSpecificNetwork(ssid, ssid_len, conf) == SLSI_STATUS_SUCCESS) {
		result = SLSI_STATUS_SUCCESS;
		printf("Successfully started scan...waiting for result!\n");
		if (sem_wait(&g_sem_result)) {
			printf("sem_wait failed");
		}
	} else {
		printf("Failed to start scan for networks. maybe you forgot to do \"startsta/startap\"\n");
	}
	sem_destroy(&g_sem_result);
	return result;
}

int8_t doStartAP(char *ssid, char *sec, char *passphrase, uint8_t channel)
{
	int8_t result = SLSI_STATUS_ERROR;

	slsi_ap_config_t *ap_config = (slsi_ap_config_t *)zalloc(sizeof(slsi_ap_config_t));
	if (!ap_config) {
		return result;
	}
	/* add initialization code as slsi_app */
	ap_config->beacon_period = 100;
	ap_config->DTIM = 2;
	ap_config->phy_mode = 1;
	ap_config->vsie = g_vsie;
	if (WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler)) {
		printf("Link call back handles registered - status failed !\n");
		free(ap_config);
		return result;
	}

	if (channel > 14 || channel < 1) {
		printf("Channel needs to be between 1 and 14 (highest channel depends on country)\n");
		free(ap_config);
		return result;
	} else {
		ap_config->channel = channel;
	}

	if (ssid != NULL && strlen(ssid) <= SLSI_SSID_LEN) {
		/* NOTE: Here the implementation assumes a plain '\0' terminated ascii-str - hence a subset of valid SSID's */
		memcpy(&ap_config->ssid, ssid, strlen(ssid));
		ap_config->ssid_len = strlen(ssid);
	} else {
		sw_printStartapHelp();
		free(ap_config);
		return result;
	}

	if (sec != NULL) {
		if (!check_security_str(sec)) {
			sw_printStartapHelp();
			printf("Security mode not supported: %s\n", sec);
			free(ap_config);
			return result;
		} else {
			ap_config->security = getSecurityConfig(sec, passphrase, SLSI_WIFI_SOFT_AP_IF);
		}
	} else {
		sw_printStartapHelp();
	}

	result = WiFiStart(SLSI_WIFI_SOFT_AP_IF, ap_config);
	if (result == SLSI_STATUS_SUCCESS || result == SLSI_STATUS_ALREADY_STARTED) {
		wifiStarted = TRUE;
		g_mode = SLSI_WIFI_SOFT_AP_IF;
		if (sec && ssid && result != SLSI_STATUS_ALREADY_STARTED) {
			printf("SoftAP with SSID: %s and security: %s successfully started!\n", ssid, sec);
		} else {
			printf("SoftAP was already started so it will continue to run with previous configuration\n");
		}
		result = SLSI_STATUS_SUCCESS;
	} else {
		printf("Failed to start SoftAP making sure supplicant is stopped\n");
		if (WiFiStop()) {
			printf("failed to stop supplicant\n");
		}
		g_mode = SLSI_WIFI_NONE;
	}
	if (ap_config) {
		if (ap_config->security) {
			free(ap_config->security);
			ap_config->security = NULL;
		}
		free(ap_config);
		ap_config = NULL;
	}
	return result;
}

void doIsConnected(void)
{
	uint8_t result;

	if (WiFiIsConnected(&result, NULL) != SLSI_STATUS_SUCCESS) {
		printf("failed to WifiIsConnected\n");
		return;
	}

	if (result > 0) {
		printf("Wi-Fi status - Connected : %d\n", result);
	} else {
		printf("Wi-Fi status - Not Connected\n");
	}
}

#ifdef CONFIG_SLSI_WIFI_P2P_APP
void sw_p2pLinkUpHandler(slsi_reason_t *reason)
{
	if (g_mode == SLSI_WIFI_P2P_IF) {
		g_join_result = reason->reason_code;	/* store result code for main thread */
		if (g_join_result == SLSI_STATUS_SUCCESS) {
			char connectedP2pName[WPA_MAX_SSID_LEN];

			memset(connectedP2pName, 0, WPA_MAX_SSID_LEN);
			printf_encode(connectedP2pName, WPA_MAX_SSID_LEN, reason->ssid, reason->ssid_len);
			printf("Connected to p2p network\n             bssid: %s, ssid: %s\n", reason->bssid, connectedP2pName);
			if (inAuto) {
				if (g_devaddr != NULL) {
					if (strncmp(reason->bssid, g_devaddr, SLSI_MACADDR_STR_LEN) != SLSI_STATUS_SUCCESS) {
						printf("             ERROR - Not expected device address!\n");
						g_join_result = SLSI_STATUS_ERROR;
					}
				}
			}
		} else {
			printf("Connect failed!\n");
		}
		if (inAuto) {
			printf("Unlocking the test connection semaphore\n");
			sem_post(&g_sem_connect);
		}
	} else {
		printf("State wrong\n");
	}
}

void sw_p2pLinkDownHandler(slsi_reason_t *reason)
{
	if (g_mode == SLSI_WIFI_P2P_IF) {
		if (reason) {
			printf("Disconnected from p2p network %s reason_code: %d %s\n", reason->bssid, reason->reason_code, reason->locally_generated ? "(locally_generated)" : "");
		} else {
			printf("Disconnected from p2p network\n");
		}
		g_p2p_outgoing = FALSE;
	} else {
		printf("State wrong\n");
	}
}

int8_t doStartP2p(void)
{
	int8_t result = SLSI_STATUS_ERROR;

	result = WiFiStart(SLSI_WIFI_P2P_IF, NULL);
	if (result == SLSI_STATUS_SUCCESS) {
		printf("P2P mode started successfully\n");
		g_mode = SLSI_WIFI_P2P_IF;
		if (WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler)) {
			printf("Link call back handles registered - status failed !\n");
		} else {
			wifiStarted = true;
		}
	} else if (result == SLSI_STATUS_ALREADY_STARTED) {
		g_mode = SLSI_WIFI_P2P_IF;
		wifiStarted = true;
		printf("P2P mode already started\n");
	} else {
		printf("Failed to start P2P mode (error: %d)\n", result);
	}
	return result;
}

void sw_findResultHandler(slsi_find_info_t *find_info)
{
	if (find_info != NULL) {
		if (find_info->reason_code == 0) {
			char name[MAX_SSID_LEN];

			snprintf(name, MAX_SSID_LEN, "%s", find_info->peer_info.device_name);
			printf("Find Result\n");
			printf("     name: %s\n", name);
			printf("     address: %s\n", find_info->peer_info.device_address);
			/* Print device information */
		} else if (find_info->reason_code == SLSI_REASON_TIMEOUT) {
			printf("Search timed out\n");
		}
	} else {
		printf("Find failed\n");
	}
	// sem_post(&g_sem_result);
}

int8_t doP2pSetName(uint8_t *device_name, uint8_t length)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (wifiStarted) {
		WiFiP2PRegisterFindCallback(sw_findResultHandler);
		if (device_name) {
			printf("Setting device name to %s\n", device_name);
			result = WiFiP2PSetDeviceName(device_name, length);
		} else {
			sw_printHelp();
		}
	}
	return result;
}

int8_t doP2pFind(uint32_t timeout)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (wifiStarted) {
		WiFiP2PRegisterFindCallback(sw_findResultHandler);
		/* TODO: should the timeout have a max value and what should it be?? */
		printf("Setting timeout to %d seconds\n", timeout);
		result = WiFiP2PFind(timeout, NULL);
	}
	return result;
}

int8_t doP2pStopFind(void)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (wifiStarted) {
		result = WiFiP2PStopFind();
		if (result == SLSI_STATUS_SUCCESS) {
			printf("Successfully stopped scan!\n");
		} else {
			printf("Failed to stop find for networks\n");
		}
		WiFiP2PRegisterFindCallback(NULL);
	}
	return result;
}

int8_t doP2pListen(void)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (wifiStarted) {
		WiFiP2PRegisterFindCallback(sw_findResultHandler);
		result = WiFiP2PListen();
	}
	return result;
}

int8_t doP2pStopListen(void)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (wifiStarted) {
		result = WiFiP2PStopListen();
		if (result == SLSI_STATUS_SUCCESS) {
			printf("Successfully stopped listen!\n");
		} else {
			printf("Failed to stop listen for networks\n");
		}
	}
	return result;
}

int8_t doP2pAddGroup(uint8_t channel, bool persistent)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (wifiStarted) {
		result = WiFiP2PCreateGroup(channel, persistent);
		if (result == SLSI_STATUS_SUCCESS) {
			printf("Successfully created p2p group!\n");
		} else {
			printf("Failed to create p2p group\n");
		}
	}
	return result;
}

int8_t doP2pRemoveGroup(void)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (wifiStarted) {
		result = WiFiP2PRemoveGroup();
		if (result == SLSI_STATUS_SUCCESS) {
			printf("Successfully removed p2p group!\n");
		} else {
			printf("Failed to remove p2p group\n");
		}
	}
	return result;
}

void sw_connectionHandler(slsi_peer_info_t *peer_info, slsi_config_method_t method, slsi_p2p_connect_response_t *response, uint8_t *go_intent, char *passphrase[SLSI_PASSPHRASE_LEN])
{
	if (g_p2p_outgoing) {
		*go_intent = g_p2p_intent_out;
	} else {
		*go_intent = g_p2p_intent_in;
	}

	if (peer_info != NULL) {
		char name[MAX_SSID_LEN];

		snprintf(name, MAX_SSID_LEN, "%s", peer_info->device_name);

		printf("\nConnection indication:\n");
		printf("       name: %s\n", name);
		printf("       address: %s\n", peer_info->device_address);
		switch (method) {
		case SLSI_P2P_PBC: {
			printf("       method: Push Button\n");
			printf("       using saved values (%s and intent=%d)\n", (g_p2p_accept == 1) ? "accept" : "reject", *go_intent);
			if (g_p2p_accept == 1) {
				*response = SLSI_P2P_CONN_ACCEPT;
				printf("       ACCEPTED connection\n");
			} else {
				*response = SLSI_P2P_CONN_REJECT;
				printf("       REJECTED connection\n");
			}
			break;
		}
		case SLSI_P2P_DISPLAY: {
			printf("       method: Display\n");
			printf("       passphrase: %s\n", *passphrase);
			printf("       using saved values (%s and intent=%d)\n", (g_p2p_accept == 1) ? "accept" : "reject", *go_intent);
			if (g_p2p_accept == 1) {
				*response = SLSI_P2P_CONN_ACCEPT;
				printf("       ACCEPTED connection\n");
			} else {
				*response = SLSI_P2P_CONN_REJECT;
				printf("       REJECTED connection\n");
			}
			break;
		}
		case SLSI_P2P_KEYPAD: {
			printf("       method: Keypad\n");
			printf("       using saved intent=%d\n", *go_intent);
			printf("       Enter passphrase: (e.g. 1 2 3 4 5 6 7 8 , work-around: enter pwd with spaces)\n");
			char input[10 + 2];

			memset(input, 0, 10 + 2);
			fflush(stdin);
			fflush(stdout);
			/* TODO: how to enter pwd - needs decoupling?!? for now */
			readline(&input, 20, stdin, stdout);
			input[strcspn(input, "\r\n")] = '\0';	/* remove line ending */
			if (strlen(input) >= 4 && strlen(input) <= 10) {
				*response = SLSI_P2P_CONN_ACCEPT;
				printf("       ACCEPTED connection\n");
				memcpy(*passphrase, input, strlen(input));
			} else {
				*response = SLSI_P2P_CONN_REJECT;
				printf("       REJECTED connection\n");
			}
			break;
		}
		case SLSI_P2P_P2PS:
			printf("       method: P2PS not supported\n");
			*response = SLSI_P2P_CONN_REJECT;
			printf("(auto) REJECTED for now");
			break;
		}
		fflush(stdout);
		/* Print device information */
	} else {
		printf("Incoming connection from unknown device\n");
	}
}

int8_t doP2pConnect(char *devaddr, char *passphrase, slsi_config_method_t method, uint8_t intent)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (wifiStarted) {
		g_p2p_outgoing = TRUE;	/* need to store for later */
		g_p2p_intent_out = intent;
		char daddr[SLSI_MACADDR_STR_LEN] = { 0 };

		strncpy(daddr, devaddr, SLSI_MACADDR_STR_LEN);
		daddr[SLSI_MACADDR_STR_LEN - 1] = '\0';
		result = WiFiP2PConnect(daddr, method);
		if (result == SLSI_STATUS_SUCCESS) {
			printf("Connecting to %s\n", devaddr);
		} else {
			printf("Failed to start connect\n");
		}
	}
	return result;
}

int8_t doP2pDisconnect(void)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (wifiStarted) {
		result = WiFiP2PDisconnect();
		if (result == SLSI_STATUS_SUCCESS) {
			printf("Successfully disconnected p2p!\n");
		} else {
			printf("Failed to disconnect p2p\n");
		}
	}
	return result;
}

#endif	/* CONFIG_SLSI_WIFI_P2P_APP */

int8_t parseCmdLine(int argc, char *argv[])
{
	int8_t result = SLSI_STATUS_ERROR;
	uint8_t ssid_len = 0;

	if (strncmp("join", argv[1], strlen(argv[1])) == 0) {
		/*
		 * slsiwifi join <ssid> <key> <security>
		 *  <ssid> : name of Wi-Fi AP (maximum 32 bytes)
		 *  <key> : passphrase
		 *  <security> : type of security, open, wep, wep_shared, wpa2_tkip, wpa2_aes, wpa2_mixed, wpa_aes, wpa_tkip, wpa_mixed
		 */
		char *secmode = SLSI_WIFI_SECURITY_OPEN;
		char *passphrase = NULL;

		if (argc <= 2) {
			sw_printJoinHelp();
			return result;
		} else {
			ssid_len = strlen(argv[2]);
			if (argc == 3) {
				secmode = SLSI_WIFI_SECURITY_OPEN;
			} else if (argc == 4) {
				if (strncmp(SLSI_WIFI_SECURITY_OPEN, argv[3], strlen(argv[3])) == 0) {
					secmode = SLSI_WIFI_SECURITY_OPEN;
				} else {
					/* we expect it to be a passphrase in argv[3] so use default security mode of wpa2_aes */
					secmode = SLSI_WIFI_SECURITY_WPA2_AES;
					passphrase = argv[3];
				}
			} else if (argc == 5) {
				passphrase = argv[3];
				secmode = argv[4];
			} else {
				sw_printJoinHelp();
				return result;
			}
		}

		/*
		 * An SSID is 32 octets, we assume all ascii here and can safely type-cast to correct data type.
		 * as this interface only supports a subset of possible SSID's
		 */
		printf("Joining network %s\n", argv[2]);
		printf("Security: %s\n", secmode);
		if (passphrase) {
			printf("Passphrase: %s\n", passphrase);
		}
		if (!check_security_str(secmode)) {
			sw_printJoinHelp();
			printf("Security mode not supported: %s\n", secmode);
		} else {
			if (doJoin((uint8_t *)argv[2], ssid_len, NULL /* bssid */, secmode, passphrase) != SLSI_STATUS_SUCCESS) {
				printf("calling join failed");
			}
		}
	} else if (strncmp("startsta", argv[1], MAXLEN(8, strlen(argv[1]))) == 0) {
		/* no more arguments - just start sta mode */
		if (doStartSta() != SLSI_STATUS_SUCCESS) {
			printf("failed to start station mode");
		}
#ifdef CONFIG_SLSI_WIFI_P2P_API
	} else if (strncmp("startp2p", argv[0], MAXLEN(8, strlen(argv[0]))) == 0) {
		/* no more arguments - just start p2p mode */
		(void)doStartP2p();
#endif
	} else if (strncmp("stop", argv[1], MAXLEN(4, strlen(argv[1]))) == 0) {
		/* no more arguments - just stop sta/softap mode */
		(void)doStop();
#ifdef CONFIG_EXAMPLES_SLSIDEMO_MEM_CHECK
		getMemLeaks();
#endif
	} else if (strncmp("opmode", argv[1], MAXLEN(6, strlen(argv[1]))) == 0) {
		if (g_mode == SLSI_WIFI_STATION_IF) {
			printf("System is in STA mode\n");
		} else if (g_mode == SLSI_WIFI_SOFT_AP_IF) {
			printf("System is in Soft AP mode\n");
		} else {
			printf("WiFi is not yet started\n");
		}
	} else if (strncmp("leave", argv[1], MAXLEN(5, strlen(argv[1]))) == 0) {
		/* no more arguments - just leave */
		(void)doLeave();
	} else if (strncmp("country", argv[1], MAXLEN(7, strlen(argv[1]))) == 0) {
		if (!argv[2] || strlen(argv[2]) != 2) {
			sw_printHelp();
		} else {
			WiFiSetCountryCode(argv[2]);
		}
	} else if (strncmp("getcountry", argv[1], MAXLEN(10, strlen(argv[1]))) == 0) {
		char ccode[10] = {0,};

		if (WiFiGetCountryCode(ccode) == SLSI_STATUS_SUCCESS) {
			printf("Current Country Code = %s\n", ccode);
		} else {
			printf("Failed to get country code\n");
		}
	} else if (strncmp("txpower", argv[1], MAXLEN(7, strlen(argv[1]))) == 0) {
		if (!argv[2] || strlen(argv[2]) != 2) {
			sw_printHelp();
		} else {
			uint8_t tx_power = (uint8_t)atoi(argv[2]);
			if (tx_power >= 12 && tx_power <= 30) {
				printf("Setting tx power to %ddBm\n", tx_power);
				WiFiSetTxPower(&tx_power);
			} else {
				sw_printHelp();
			}
		}
	} else if (strncmp("scan", argv[1], MAXLEN(4, strlen(argv[1]))) == 0) {
		/* no more arguments - just scan */
		(void)doScan();
	} else if (strncmp("scan_specific", argv[1], MAXLEN(13, strlen(argv[1]))) == 0) {
		if (argc < 5) {
			sw_printHelp();
			printf("\nNot enough parameters\n");
		} else {
			char *secmode = argv[4];
			char *passphrase = argv[3];
			char *ssid = argv[2];

			ssid_len = strlen(ssid);
			printf("Starting scan for specific AP with info\n:  ssid: %s\n Pass: %s\n  Secmode: %s\n", ssid, passphrase, secmode);
			(void)doSpecificScan((uint8_t *) ssid, ssid_len, passphrase, secmode);
		}
	} else if (strncmp("startap", argv[1], MAXLEN(7, strlen(argv[1]))) == 0) {
		/* slsiwifi startap <ssid> <security> <key> <channel> */
		if (argc >= 3 && strlen(argv[2]) > 32) {
			printf("SSID cannot be longer than 32 characters\n");
			return result;
		}
		uint8_t channel = 1;
		char *sec = "open";
		char *passphrase = NULL;

		/*
		 * Options for argc = 3:
		 * startap <ssid>
		 * channel defaults to 1
		 * security defaults to open
		 */
		if (argc == 3) {
			/* fall through */
		} else if (argc == 4) {
			/*
			 * Options for argc = 4:
			 * startap <ssid> <channel>
			 * startap <ssid> open
			 * startap <ssid> <passphrase>
			 * channel defaults to 1
			 * security defaults to wpa2_aes for passphrase only
			 */
			if (strlen(argv[3]) <= 4) {
				/* We expect it to be a number = channel */
				/* TODO: isdigit() will make this more robust */
				channel = atoi(argv[3]);
			} else if (strncmp("open", argv[3], MAXLEN(4, strlen(argv[3]))) == 0) {
				/* use default settings */
				/* TODO: better to have length check for argv[3] prior to current condition with && */
			} else {
				/* assume passphrase */
				sec = SLSI_WIFI_SECURITY_WPA2_AES;
				passphrase = argv[3];
			}
		} else if (argc == 5) {
			/*
			 * Options for argc = 5:
			 * startap <ssid> open <channel>
			 * startap <ssid> <passphrase> <sec>
			 * startap <ssid> <passphrase> <channel>
			 * channel defaults to 1
			 * security defaults to wpa2_aes for passphrase only
			 */
			if (strncmp("open", argv[3], MAXLEN(4, strlen(argv[3]))) == 0) {
				/* use default settings */
				if (strlen(argv[4]) <= 4) {
					/* We expect it to be a number = channel */
					/* TODO: isdigit() will make this more robust */
					channel = atoi(argv[4]);
				} else {
					printf("%s does not look like a channel number\n", argv[4]);
				}
			} else if (strncmp("wep", argv[4], MAXLEN(3, strlen(argv[4]))) == 0 || strncmp("wep_shared", argv[4], MAXLEN(10, strlen(argv[4]))) == 0) {
				/* TODO: better to have length check for argv[3] prior to current condition with && */
				printf("wep security is not allowed for Soft AP\n");
				sw_printStartapHelp();
				return result;
			} else {
				/* assume passphrase */
				passphrase = argv[3];
				sec = SLSI_WIFI_SECURITY_WPA2_AES;
				if (strlen(argv[4]) <= 4) {
					/* We expect it to be a number = channel */
					/* TODO: isdigit() will make this more robust */
					channel = atoi(argv[4]);
				} else {
					sec = argv[4];
				}
			}
		} else if (argc == 6) {
			passphrase = argv[3];
			sec = argv[4];
			channel = atoi(argv[5]);
		} else {
			sw_printStartapHelp();
			return result;
		}
		result = doStartAP(argv[2], sec, passphrase, channel);
	} else if (strncmp("status", argv[1], MAXLEN(6, strlen(argv[1]))) == 0) {
		(void)doIsConnected();
	} else if (strncmp(argv[1], "tcpserver", MAXLEN(9, strlen(argv[1]))) == 0) {
		if (slsi_tcp_server() != 0) {
			printf("TCP server failed\n");
		}
	} else if (strncmp(argv[1], "tcpclient", MAXLEN(9, strlen(argv[1]))) == 0) {
		if (slsi_tcp_client() != 0) {
			printf("TCP Client failed\n");
		}
	} else if (strncmp(argv[1], "udpserver", MAXLEN(9, strlen(argv[1]))) == 0) {
		if (slsi_udp_server() != 0) {
			printf("UDP Server failed\n");
		}
	} else if (strncmp(argv[1], "ie", MAXLEN(2, strlen(argv[1]))) == 0) {
		if (argv[2] != NULL && argv[3] != NULL && strlen(argv[2]) == 6 && strlen(argv[3]) <= 253) {
			printf("Storing new OUI=%s IE=%s\n", argv[2], argv[3]);
			if (g_vsie == NULL) {
				g_vsie = (slsi_vendor_ie_t *)zalloc(sizeof(slsi_vendor_ie_t));
				if (g_vsie == NULL) {
					printf("Could not allocate buffer for vendor IE\n");
					return result;
				}
			}
			char *poui = argv[2];

			if (hex2uint8(poui, &g_vsie->oui[0]) != TRUE || hex2uint8(poui + 2, &g_vsie->oui[1]) != TRUE || hex2uint8(poui + 4, &g_vsie->oui[2]) != TRUE) {
				printf("OUI hex representation not valid\n");
				return result;
			}
			if (g_vsie->content != NULL) {
				free(g_vsie->content);
			}
			g_vsie->content = (uint8_t *)zalloc(strlen(argv[3]));
			if (g_vsie->content == NULL) {
				printf("Could not allocate buffer for vendor IE\n");
				return result;
			}
			g_vsie->content_length = strlen(argv[3]);
			memcpy(g_vsie->content, argv[3], strlen(argv[3]));
		}
	} else if (strncmp(argv[1], "udpclient", MAXLEN(9, strlen(argv[1]))) == 0) {
		if (slsi_udp_client() != 0) {
			printf("UDP Client failed\n");
		}
	} else if (strncmp(argv[1], "saveconfig", MAXLEN(10, strlen(argv[1]))) == 0) {
		int ret = WiFiSaveConfig();

		if (ret == SLSI_STATUS_SUCCESS) {
			printf("Configuration successfully saved\n");
		} else if (ret == SLSI_STATUS_NOT_SUPPORTED) {
			printf("Save Config not supported in this build\n");
		} else if (ret == SLSI_STATUS_NOT_STARTED) {
			printf("WiFi stack not started so cannot save config\n");
		} else if (ret == SLSI_STATUS_NOT_ALLOWED) {
			printf("Save Config not allowed in AP mode\n");
		}
	} else if (strncmp(argv[1], "auto", MAXLEN(4, strlen(argv[1]))) == 0 && inAuto == false) {
		sem_init(&ap_conn_sem, 0, 0);
		if (argc > 1) {
			result = doAutoTest(argv[2]);    /* filename if not null */
		} else {
			result = doAutoTest(NULL);
		}
		sem_destroy(&ap_conn_sem);
		if (result == SLSI_STATUS_SUCCESS) {
			return 0;
		} else {
			return 1;
		}
#ifdef CONFIG_SLSI_WIFI_P2P_APP
	} else if (strncmp(argv[1], "p2p_set_name", MAXLEN(12, strlen(argv[1]))) == 0) {
		int ret = SLSI_STATUS_ERROR;

		ret = doP2pSetName((uint8_t *) argv[2], strlen(argv[2]));
		if (ret == SLSI_STATUS_SUCCESS) {
			printf("Device name updated\n");
		} else {
			printf("Updating device name failed\n");
		}
	} else if (strncmp(argv[1], "p2p_find", MAXLEN(8, strlen(argv[1]))) == 0) {
		int ret = SLSI_STATUS_ERROR;
		uint32_t timeout = 0;

		if (argv[2]) {
			timeout = strtoul(argv[2], 0, 10);
		}
		ret = doP2pFind(timeout);
		if (ret == SLSI_STATUS_SUCCESS) {
			printf("Find initiated\n");
		} else {
			printf("Find start failed\n");
		}
	} else if (strncmp(argv[1], "p2p_connect_in", MAXLEN(14, strlen(argv[1]))) == 0) {
		int ret = SLSI_STATUS_ERROR;

		if (argv[2]) {
			int tmp_accept = atoi(argv[2]);

			if (tmp_accept == 1) {
				g_p2p_accept = 1;
				ret = SLSI_STATUS_SUCCESS;
			} else if (tmp_accept == 0) {
				g_p2p_accept = 0;
				ret = SLSI_STATUS_SUCCESS;
			}
		}
		if (ret != SLSI_STATUS_ERROR && argv[3]) {
			int tmp_intent = atoi(argv[3]);

			if (tmp_intent >= 0 && tmp_intent <= 15) {
				g_p2p_intent_in = tmp_intent;
				ret = SLSI_STATUS_SUCCESS;
			}
		}
		if (ret == SLSI_STATUS_SUCCESS) {
			printf("Incoming connect response values set: %s intent=%d\n", g_p2p_accept == 1 ? "ACCEPT" : "REJECT", g_p2p_intent_in);
		} else {
			printf("Setting incoming connect response values failed\n");
		}
	} else if (strncmp(argv[1], "p2p_stop_find", MAXLEN(13, strlen(argv[1]))) == 0) {
		(void)doP2pStopFind();
	} else if (strncmp(argv[1], "p2p_listen", MAXLEN(10, strlen(argv[1]))) == 0) {
		int ret = SLSI_STATUS_ERROR;

		ret = doP2pListen();
		if (ret == SLSI_STATUS_SUCCESS) {
			printf("Listen initiated\n");
		} else {
			printf("Listen failed\n");
		}
	} else if (strncmp(argv[1], "p2p_add_group", MAXLEN(13, strlen(argv[1]))) == 0) {
		int ret = SLSI_STATUS_ERROR;
		bool persistent = FALSE;
		uint8_t channel = 0;

		if (argc == 4 && strncmp(argv[3], "persistent", MAXLEN(10, strlen(argv[3])) == 0)) {
			persistent = TRUE;
		}
		if (argv[2]) {
			channel = atoi(argv[2]);
			printf("channel = %d\n", channel);
		}
		ret = doP2pAddGroup(channel, persistent);
	} else if (strncmp(argv[1], "p2p_remove_group", MAXLEN(16, strlen(argv[1]))) == 0) {
		int ret = SLSI_STATUS_ERROR;

		ret = doP2pRemoveGroup();
	} else if (strncmp(argv[1], "p2p_stop_listen", MAXLEN(15, strlen(argv[1]))) == 0) {
		(void)doP2pStopListen();
	} else if (strncmp(argv[1], "p2p_connect", MAXLEN(11, strlen(argv[1]))) == 0) {
		int ret = SLSI_STATUS_ERROR;
		uint8_t intent = 0;

		if (argc == 6) {
			char *devaddr = argv[2];

			if (strlen(argv[3]) > SLSI_PASSPHRASE_LEN) {
				printf("Passphrase too long. Max is %d characters\n", SLSI_PASSPHRASE_LEN);
				return ret;
			}
			if (strlen(devaddr) != SLSI_MACADDR_STR_LEN - 1) {
				printf("device address seems wrong. Format is xx:xx:xx:xx:xx:xx. was: %s\n", argv[2]);
			}
			char *passphrase = argv[3];
			slsi_config_method_t method = 0;

			if (strncmp(argv[4], "pbc", MAXLEN(3, strlen(argv[4]))) == 0) {
				method = SLSI_P2P_PBC;
			} else if (strncmp(argv[4], "display", MAXLEN(7, strlen(argv[4]))) == 0) {
				method = SLSI_P2P_DISPLAY;
			} else if (strncmp(argv[4], "keypad", MAXLEN(6, strlen(argv[4]))) == 0) {
				method = SLSI_P2P_KEYPAD;
			} else {
				printf("Configuration method \"%s\" not known. Can be pbc/display/keypad\n", argv[4]);
				return ret;
			}
			g_p2p_outgoing = TRUE;
			intent = (uint8_t) atoi(argv[5]);
			if (intent == 0 || intent > 15) {
				printf("intent value needs to be in the range 1-15\n");
				return ret;
			}
			printf("Connecting with following params:\n devaddr = %s\n pass = %s\n method = %d(%s)\n intent = %d\n", devaddr, passphrase, method, argv[4], intent);
			ret = doP2pConnect(devaddr, passphrase, method, intent);
		} else {
			sw_printHelp();
		}
	} else if (strncmp(argv[1], "p2p_disconnect", MAXLEN(14, strlen(argv[1]))) == 0) {
		(void)doP2pDisconnect();
#endif
#ifdef CONFIG_SLSI_WIFI_SANITY
	} else if (strncmp(argv[1], "sanity", MAXLEN(6, strlen(argv[1]))) == 0 && inAuto == false) {
		int iterations = 0;
		char *softApName = NULL;

		sem_init(&ap_conn_sem, 0, 0);
		if (argc > 2) {
			iterations = atoi(argv[2]);
		}
		if (argc > 3) {
			softApName = argv[3];
		}
		result = doSanityTest(iterations, softApName);
		sem_destroy(&ap_conn_sem);
		if (result != SLSI_STATUS_SUCCESS) {
			/* we need to do some cleanup and make sure the supplicant is stopped */
			(void)doStop();
			printf("STATUS:\n\ttests passed: %d\n", result);
		}
#ifdef CONFIG_EXAMPLES_SLSIDEMO_MEM_CHECK
		int leakresult = getMemLeaks();
		if (leakresult != SLSI_STATUS_SUCCESS || result != SLSI_STATUS_SUCCESS) {
#else
		if (result != SLSI_STATUS_SUCCESS) {
#endif
			/* needed for CI systems to parse for SUCCESS/FAILED */
			printf("\n\tWiFi Sanity Test Result: FAILED\n\n");
		} else {
			printf("\n\tWiFi Sanity Test Result: SUCCESS\n\n");
		}

		if (result == SLSI_STATUS_SUCCESS) {
			return 0;
		} else {
			return 1;
		}
#ifdef CONFIG_SLSI_WIFI_P2P_API
	} else if (strncmp(argv[1], "p2p_sanity", MAXLEN(10, strlen(argv[1]))) == 0 && inAuto == false) {
		int iterations = 0;
		char *p2pGOname = NULL;

		sem_init(&ap_conn_sem, 0, 0);
		if (argc > 2) {
			iterations = atoi(argv[2]);
		}
		if (argc > 3) {
			p2pGOname = argv[3];
		}
		result = doP2pSanityTest(iterations, p2pGOname);
		sem_destroy(&ap_conn_sem);
		if (result != SLSI_STATUS_SUCCESS) {
			/* we need to do some cleanup and make sure the supplicant is stopped */
			(void)doStop();
			printf("STATUS:\n\ttests passed: %d\n", result);
		}
#ifdef CONFIG_EXAMPLES_SLSIDEMO_MEM_CHECK
		int leakresult = getMemLeaks();
		if (leakresult != SLSI_STATUS_SUCCESS || result != SLSI_STATUS_SUCCESS) {
#else
		if (result != SLSI_STATUS_SUCCESS) {
#endif
			/* needed for CI systems to parse for SUCCESS/FAILED */
			printf("\n\tWiFi P2P Sanity Test Result: FAILED\n\n");
		} else {
			printf("\n\tWiFi P2P Sanity Test Result: SUCCESS\n\n");
		}

		if (result == SLSI_STATUS_SUCCESS) {
			return 0;
		} else {
			return 1;
		}
#endif							/* CONFIG_SLSI_WIFI_P2P_SANITY */
	} else if (strncmp(argv[1], "nightly", MAXLEN(7, strlen(argv[1]))) == 0 && inAuto == false) {
		int iterations = 0;
		char *softApName = NULL;
		char *p2pGOname = NULL;

		sem_init(&ap_conn_sem, 0, 0);
		if (argc > 2) {
			iterations = atoi(argv[2]);
		}
		if (argc > 3) {
			softApName = argv[3];
		}
		if (argc > 4) {
			p2pGOname = argv[4];
		}
		result = doNightlyTest(iterations, softApName, p2pGOname);
		sem_destroy(&ap_conn_sem);
		if (result != SLSI_STATUS_SUCCESS) {
			/* we need to do some cleanup and make sure the supplicant is stopped */
			(void)doStop();
			printf("STATUS:\n\ttests passed: %d\n", result);
		}
#ifdef CONFIG_EXAMPLES_SLSIDEMO_MEM_CHECK
		int leakresult = getMemLeaks();

		if (leakresult != SLSI_STATUS_SUCCESS || result != SLSI_STATUS_SUCCESS) {
#else
		if (result != SLSI_STATUS_SUCCESS) {
#endif
			/* needed for CI systems to parse for SUCCESS/FAILED */
			printf("\n\tWiFi Nightly Sanity Test Result: FAILED\n\n");
		} else {
			printf("\n\tWiFi Nightly Sanity Test Result: SUCCESS\n\n");
		}

		if (result == SLSI_STATUS_SUCCESS) {
			return 0;
		} else {
			return 1;
		}
#endif
	} else if (strncmp(argv[1], "help", MAXLEN(4, strlen(argv[1]))) == 0) {
		sw_printFullHelp();
	} else {
		sw_printHelp();
	}
	return result;
}

/****************************************************************************
 * app_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, char *argv[])
{
#else
int slsi_wifi_main(int argc, char *argv[])
{
#endif
#ifdef CONFIG_EXAMPLES_SLSIDEMO_MEM_CHECK
	if (!wifiStarted) {
		g_memstat_total = getMemUsage();
	}
#endif
	int8_t result = SLSI_STATUS_ERROR;

	sw_printHeader();
	if (argc == 1) {
		sw_printHelp();
		return result;
	} else {
		/*
		 * we have no way of knowing if the link up/down handlers have been
		 * changed behind our back so we will always re-register them here.
		 * They are critical for the system to work
		 */
		if (!WiFiRegisterLinkCallback(&sw_linkUpHandler, &sw_linkDownHandler)) {
			printf("Link call back handles registered - per default!\n");
		} else {
			printf("Link call back handles not registered - Cannot continue !\n");
			return result;
		}
#ifdef CONFIG_SLSI_WIFI_P2P_APP
		if (!WiFiP2PRegisterConnectionHandler(sw_connectionHandler)) {
			printf("Incoming connection call back handler registered!\n");
		} else {
			printf("Connection call back handles not registered - Cannot continue !\n");
			return result;
		}
#endif
		result = parseCmdLine(argc, argv);
	}
	return result;
}
