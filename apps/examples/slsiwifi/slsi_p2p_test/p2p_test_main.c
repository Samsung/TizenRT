/**
 * @file apps/example/slsiwifi/slsi_p2p_test/p2p_test_main.c
 *
 *   Copyright (c) 2016 Samsung Electronics, Inc.
 *   All rights reserved.
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
 */

#ifdef CONFIG_SLSI_WIFI_P2P_API

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
#include "output_functions.h"
#include "slsiwifi_main.h"

/****************************************************************************
 * Defines
 ****************************************************************************/
#define DEBUG                     0
#define min(a, b)                 (((a) < (b)) ? (a) : (b))
#define MAXLEN(a, b)              (((a) > (b)) ? (a) : (b))
#define TOUPPER(character)        (((character) >= 'a') && ((character) <= 'z') ? ((character) - 0x20) : (character))
#define WPA_MAX_SSID_LEN          (4 * 32 + 1)	/* SSID encoded in a string - worst case is all 4-octet hex digits + '\0' */
#define STATE_DISCONNECTED        0
#define STATE_CONNECTED           1

#define SLSI_GO_MODE_NAME         "SLSI-P2P-TEST"
#define DEFAULT_GO_INTENT         7
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
static bool wifiStarted;
static WiFi_InterFace_ID_t g_mode;
static int g_connection_state;
static char *g_devaddr;
static uint8_t g_intent;
static uint8_t g_join_result;
sem_t g_sem_disconnect;
sem_t g_sem_connect;
sem_t g_sem_connect_in;
sem_t g_sem_find;

/****************************************************************************
 * Private Functions
 ***************************************************************************/
static void p2pTestLinkUpHandler(slsi_reason_t *reason)
{
	if (g_mode == SLSI_WIFI_P2P_IF) {
		g_join_result = reason->reason_code;	/* store result code for main thread */
		if (g_join_result == 0) {
			char connectedP2pName[WPA_MAX_SSID_LEN];

			memset(connectedP2pName, 0, WPA_MAX_SSID_LEN);
			printf_encode(connectedP2pName, WPA_MAX_SSID_LEN, reason->ssid, reason->ssid_len);
			printf("Connected to p2p network\n             bssid: %s, ssid: %s\n", reason->bssid, connectedP2pName);
			g_connection_state = STATE_CONNECTED;
			sem_post(&g_sem_connect);	/* tell the main thread to move on */
		} else {
			printf("Connect failed!\n");
		}
	} else {
		printf("State wrong\n");
	}
}

static void p2pTestLinkDownHandler(slsi_reason_t *reason)
{
	if (g_mode == SLSI_WIFI_P2P_IF) {
		if (reason) {
			printf("Disconnected from p2p network %s reason_code: %d %s\n", reason->bssid, reason->reason_code, reason->locally_generated ? "(locally_generated)" : "");
		} else {
			printf("Disconnected from p2p network\n");
		}
		sem_post(&g_sem_disconnect);	/* need to tell the system that we have a link-down. Needed for doLeave() */
		g_connection_state = STATE_DISCONNECTED;
	} else {
		printf("State wrong\n");
	}

}

static int8_t doStartP2p(void)
{
	int8_t result = SLSI_STATUS_ERROR;

	result = WiFiStart(SLSI_WIFI_P2P_IF, NULL);
	if (result == SLSI_STATUS_SUCCESS) {
		printf("P2P mode started successfully\n");
		g_mode = SLSI_WIFI_P2P_IF;
		if (WiFiP2PRegisterLinkCallback(&p2pTestLinkUpHandler, &p2pTestLinkDownHandler)) {
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

static int8_t doP2pFind(uint32_t timeout, char *devaddr)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (wifiStarted) {
		WiFiP2PRegisterFindCallback(sw_findResultHandler);
		/* TODO: should the timeout have a max value and what should it be?? */
		if (timeout > 0 && timeout <= 3600) {
			printf("Setting timeout to %d seconds\n", timeout);
			result = WiFiP2PFind(timeout, devaddr);
		} else {
			sw_printHelp();
		}
	}
	return result;
}

static int8_t doP2pStopFind(void)
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

static int8_t doP2pListen(void)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (wifiStarted) {
		result = WiFiP2PListen();
	}
	return result;
}

static int8_t doP2pStopListen(void)
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

static void p2ptest_connectionHandler(slsi_peer_info_t *peer_info, slsi_config_method_t method, slsi_p2p_connect_response_t *response, uint8_t *go_intent, char *passphrase[SLSI_PASSPHRASE_LEN])
{
	*response = SLSI_P2P_CONN_REJECT;
	*go_intent = g_intent;
	if (peer_info != NULL) {
		char name[MAX_SSID_LEN];

		snprintf(name, MAX_SSID_LEN, "%s", peer_info->device_name);
		printf("Incoming connection :\n");
		printf("       name: %s\n", name);
		printf("       address: %s\n", peer_info->device_address);
		switch (method) {
		case SLSI_P2P_PBC:
			printf("       method: Push Button\n");
			break;
		case SLSI_P2P_DISPLAY:
			printf("       method: Display\n");
			printf("       passphrase: %s\n", *passphrase);
			break;
		case SLSI_P2P_KEYPAD:
			printf("       method: Keypad\n");
			printf("       Enter passphrase:\n");
			/* TODO: how to enter pwd??? */
			break;
		case SLSI_P2P_P2PS:
			printf("       method: P2PS not supported\n");
			break;
		}
		printf("(auto) ACCEPTED\n");
		*response = SLSI_P2P_CONN_ACCEPT;
		/* Print device information */
	} else {
		printf("Incoming connection from unknown device\n");
	}
}

static int8_t doP2pConnect(char *devaddr, char *passphrase, slsi_config_method_t method, uint8_t intent)
{
	int8_t result = SLSI_STATUS_ERROR;

	if (wifiStarted) {
		char daddr[SLSI_MACADDR_STR_LEN + 1] = { 0 };

		strncpy(&daddr[0], devaddr, SLSI_MACADDR_STR_LEN);
		result = WiFiP2PConnect(daddr, method);
		if (result == SLSI_STATUS_SUCCESS) {
			printf("Connecting to %s\n", devaddr);
		} else {
			printf("Failed to start connect\n");
		}
	}
	return result;
}

static int8_t doP2pDisconnect(void)
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

/****************************************************************************
 * app_main for p2p_test_listen
 * Used from the command line as :
 * p2p_test_listen <Addr to listen for> <pin> <config method> <go-intent>
 *
 * This will run until board is rebooted and continiously listen for incoming
 * connections. If address does not match the one from the commandline then
 * it will get rejected.
 ****************************************************************************/
int slsi_p2p_test_listen_main(int argc, char *argv[])
{
	int8_t ret = SLSI_STATUS_SUCCESS;

	sw_printHeader();
	if (argc == 1) {
		printf("Usage:\np2p_test_listen [devaddr] [pin] [config_method] [go_intent]");
		return ret;
	} else {
		/*
		 * we have no way of knowing if the link up/down handlers have been
		 * changed behind our back so we will always re-register them here.
		 * They are critical for the system to work
		 */
		if (!WiFiP2PRegisterConnectionHandler(p2ptest_connectionHandler)) {
			printf("Incoming connection call back handler registered!\n");
		} else {
			printf("Connection call back handles not registered - Cannot continue !\n");
			return ret;
		}
		if (argc > 1) {
			g_devaddr = argv[1];
			if (strlen(g_devaddr) != SLSI_MACADDR_STR_LEN - 1) {
				printf("device address seems wrong. Format is xx:xx:xx:xx:xx:xx. was: %s\n", argv[1]);
			}
			if (strlen(argv[2]) > SLSI_PASSPHRASE_LEN) {
				printf("Passphrase too long. Max is %d characters\n", SLSI_PASSPHRASE_LEN);
				return ret;
			}
			char *passphrase = argv[2];
			slsi_config_method_t method = 0;

			if (strncmp(argv[3], "pbc", MAXLEN(3, strlen(argv[3]))) == 0) {
				method = SLSI_P2P_PBC;
			} else if (strncmp(argv[3], "display", MAXLEN(7, strlen(argv[3]))) == 0) {
				method = SLSI_P2P_DISPLAY;
			} else if (strncmp(argv[3], "keypad", MAXLEN(6, strlen(argv[3]))) == 0) {
				method = SLSI_P2P_KEYPAD;
			} else {
				printf("Configuration method \"%s\" not known. Can be pbc/display/keypad\n", argv[3]);
				return ret;
			}
			g_intent = (uint8_t) atoi(argv[4]);

			if (g_intent == 0 || g_intent > 15) {
				printf("intent value needs to be in the range 1-15\n");
				return ret;
			}
			(void)doStartP2p();
			while (TRUE) {
				sem_init(&g_sem_connect, 0, 0);
				sem_init(&g_sem_disconnect, 0, 0);
				printf("Listening with following params:\n g_devaddr = %s\n pass = %s\n method = %d(%s)\n intent = %d\n", g_devaddr, passphrase, method, argv[3], g_intent);
				if (doP2pListen() != SLSI_STATUS_SUCCESS) {
					printf("could not start p2p listen\n");
				} else {
					if (sem_wait(&g_sem_disconnect)) {
						printf("sem_wait failed");
					}
				}
				sem_post(&g_sem_disconnect);
				sem_destroy(&g_sem_disconnect);
				sem_post(&g_sem_connect);
				sem_destroy(&g_sem_connect);
			}
		}
	}
	return ret;
}

/****************************************************************************
 * app_main for p2p_test_connect
 * Used from the command line as :
 * p2p_test_connect <Addr to listen for> <pin> <config method> <go-intent>
 *
 * This will run until board is rebooted and continiously try to connect to
 * the device given on the commmand line.
 * ***************************************************************************/
int slsi_p2p_test_connect_main(int argc, char *argv[])
{
	int8_t ret = SLSI_STATUS_SUCCESS;

	sw_printHeader();
	if (argc == 1) {
		printf("Usage:\np2p_test_connect [devaddr] [pin] [config_method] [go_intent]");
		return ret;
	} else {
		/*
		 * we have no way of knowing if the link up/down handlers have been
		 * changed behind our back so we will always re-register them here.
		 * They are critical for the system to work
		 */
		if (!WiFiP2PRegisterConnectionHandler(p2ptest_connectionHandler)) {
			printf("Incoming connection call back handler registered!\n");
		} else {
			printf("Connection call back handles not registered - Cannot continue !\n");
			return ret;
		}
		if (argc > 1) {
			g_devaddr = argv[1];
			if (strlen(g_devaddr) != SLSI_MACADDR_STR_LEN - 1) {
				printf("device address seems wrong. Format is xx:xx:xx:xx:xx:xx. was: %s\n", argv[1]);
			}

			if (strlen(argv[2]) > SLSI_PASSPHRASE_LEN) {
				printf("Passphrase too long. Max is %d characters\n", SLSI_PASSPHRASE_LEN);
				return ret;
			}
			char *passphrase = argv[2];
			slsi_config_method_t method = 0;

			if (strncmp(argv[3], "pbc", MAXLEN(3, strlen(argv[3]))) == 0) {
				method = SLSI_P2P_PBC;
			} else if (strncmp(argv[3], "display", MAXLEN(7, strlen(argv[3]))) == 0) {
				method = SLSI_P2P_DISPLAY;
			} else if (strncmp(argv[3], "keypad", MAXLEN(6, strlen(argv[3]))) == 0) {
				method = SLSI_P2P_KEYPAD;
			} else {
				printf("Configuration method \"%s\" not known. Can be pbc/display/keypad\n", argv[3]);
				return ret;
			}
			uint8_t intent = (uint8_t)atoi(argv[4]);

			if (intent == 0 || intent > 15) {
				printf("intent value needs to be in the range 1-15\n");
				return ret;
			}
			(void)doStartP2p();
			while (TRUE) {
				sem_init(&g_sem_connect, 0, 0);
				printf("Connecting with following params:\n g_devaddr = %s\n pass = %s\n method = %d(%s)\n intent = %d\n", g_devaddr, passphrase, method, argv[3], intent);
				(void)doP2pConnect(g_devaddr, passphrase, method, intent);
				if (sem_wait(&g_sem_connect)) {
					printf("sem_wait failed\n");
				}
				sem_post(&g_sem_connect);
				sem_destroy(&g_sem_connect);
			}
		}
	}
	return ret;
}
#endif	/* CONFIG_SLSI_WIFI_P2P_API */
