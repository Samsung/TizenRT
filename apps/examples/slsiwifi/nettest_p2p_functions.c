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
#include <sys/socket.h>
#include "net/lwip/mem.h"
#include "net/lwip/raw.h"
#include <net/lwip/ipv4/inet.h>		// TODO: ipv4 check
#include <net/lwip/dhcp.h>
#include <net/lwip/sys.h>
#include <apps/netutils/netlib.h>
#include <net/lwip/tcpip.h>
#include <net/lwip/netif.h>
#include <net/lwip/arch/sys_arch.h>
#include <net/lwip/dhcp.h>
#include <tinyara/config.h>
#include "net/lwip/ipv4/icmp.h"		// TODO: ipv4 check
#include "net/lwip/ipv4/ip.h"		// TODO: ipv4 check
#include "net/lwip/ipv4/inet_chksum.h"		// TODO: ipv4 check
#include "net/lwip/err.h"
#include "net/lwip/opt.h"
#endif

#ifdef CONFIG_SLSI_WIFI_P2P_API
extern bool inAuto;
extern bool wifiStarted;
extern slsi_vendor_ie_t *g_vsie;
extern WiFi_InterFace_ID_t g_mode;
extern sem_t g_sem_connect;
extern char *g_devaddr;
extern uint8_t g_p2p_intent_in;
extern uint8_t g_join_result;

#define SANITY_P2P_TEST_SLEEP           1000000
#define SANITY_LOOP_COUNT               20
#define SANITY_P2P_CON_OUT_INTENT_CLI   1
#define SANITY_P2P_CON_OUT_INTENT_GO    15

#define WPA_MAX_SSID_LEN                (4*32+1)	/* SSID encoded in a string - worst case is all 4-octet hex digits + '\0' */

#define TESTPRINT(fmt, ...) \
	printf("\n\n**************************************************************************\n" \
	"[P2P TESTCASE %d] " fmt "\n**************************************************************************\n" \
	, tc++, ##__VA_ARGS__)

int8_t doP2pStartStopTest(int iterations)
{
	int loop = 0;

	TESTPRINT("Trying to start/Stop P2P mode %d times\n", iterations);
	for (loop = 0; loop < iterations; loop++) {
		printf("==> Trial: %d\n", loop + 1);
		printf("Start P2p mode\n");
		if (doStartP2p() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		} else {
			printf("Successfully started. Now lets stop\n");
		}
		/*********** STOP **************/
		printf("Stop P2p mode\n");
		if (doStop() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		} else {
			// we need to Register Link Callback handlers again as they are
			// unregistered in doStop
			WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);

		}
	}
	return SLSI_STATUS_SUCCESS;
}

#if 0
int8_t doP2pConnectDisconnectTest(int iterations, char *devaddr, char *pin, slsi_config_method_t method, uint8_t intent)
{
	int loop = 0;

	TESTPRINT("Trying to connect/disconnect in P2P %d times,\ndevice address %s, pin %s, config method %d, intent %d", iterations, devaddr, pin, method, intent);
	printf("Start p2p mode\n");
	if (doStartP2p() == SLSI_STATUS_ERROR) {
		inAuto = false;
		return tc - 1;
	} else {
		printf("Successfully started. Now lets connect out\n");
	}
	WiFiP2PRegisterConnectionHandler(&sw_connectionHandler);
	/*********** CONNECT OUT **************/
	for (loop = 0; loop < iterations; loop++) {
		sem_init(&g_sem_connect, 0, 0);
		printf("==> Trial: %d\n", loop + 1);
		printf("Connect to peer %s with config mode %d, pin %s and go intent %d\n", devaddr, method, pin, intent);
		if (doP2pConnect(devaddr, pin, method, intent) != SLSI_STATUS_SUCCESS) {
			goto errout;
		} else {
			printf("Connect sent - now lets wait for a connection is up\n");
			if (sem_wait(&g_sem_connect)) {
				printf("sem_wait failed");
			}
			printf("after semaphore is opened\n");
			if (g_join_result != SLSI_STATUS_SUCCESS) {
				goto errout;
			}
		}

		usleep(SANITY_P2P_TEST_SLEEP);
		printf("Disconnecting from peer\n");
		if (doP2pDisconnect() != SLSI_STATUS_SUCCESS) {
			goto errout;
		}
		usleep(SANITY_P2P_TEST_SLEEP);
		sem_post(&g_sem_connect);
		sem_destroy(&g_sem_connect);
	}
	/*********** STOP **************/
	printf("Stop p2p mode\n");
	if (doStop() == SLSI_STATUS_ERROR) {
		inAuto = false;
		return tc - 1;
	} else {
		// we need to Register Link Callback handlers again as they are
		// unregistered in doStop
		WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
	}
	return SLSI_STATUS_SUCCESS;

errout:
	inAuto = false;
	sem_post(&g_sem_connect);
	sem_destroy(&g_sem_connect);
	return tc - 1;
}

int8_t doP2pStartConnectDisconnectStopTest(int iterations, char *devaddr, char *pin, slsi_config_method_t method, uint8_t intent)
{
	int loop = 0;

	TESTPRINT("Trying to start/connect/disconnect/Stop in P2P %d times,\ndevice address %s, pin %s, config method %d, intent %d", iterations, devaddr, pin, method, intent);

	for (loop = 0; loop < iterations; loop++) {
		printf("==> Trial: %d\n", loop + 1);
		sem_init(&g_sem_connect, 0, 0);
		printf("Start p2p mode\n");
		if (doStartP2p() == SLSI_STATUS_ERROR) {
			goto errout;
		} else {
			printf("Successfully started. Now lets connect out\n");
		}
		WiFiP2PRegisterConnectionHandler(&sw_connectionHandler);
		/*********** CONNECT OUT **************/
		printf("Connect to peer %s with config mode %d, pin %s and go intent %d\n", devaddr, method, pin, intent);
		if (doP2pConnect(devaddr, pin, method, intent) != SLSI_STATUS_SUCCESS) {
			goto errout;
		} else {
			printf("Connect sent - now lets wait for a connection is up\n");
			if (sem_wait(&g_sem_connect)) {
				printf("sem_wait failed");
			}
			printf("after semaphore is opened\n");
			if (g_join_result != SLSI_STATUS_SUCCESS) {
				goto errout;
			}
		}
		usleep(SANITY_P2P_TEST_SLEEP);
		printf("Disconnecting from peer\n");
		if (doP2pDisconnect() != SLSI_STATUS_SUCCESS) {
			goto errout;
		}
		/*********** STOP **************/
		printf("Stop p2p mode\n");
		if (doStop() == SLSI_STATUS_ERROR) {
			goto errout;
		} else {
			// we need to Register Link Callback handlers again as they are
			// unregistered in doStop
			WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);

		}
		usleep(SANITY_P2P_TEST_SLEEP);
		sem_post(&g_sem_connect);
		sem_destroy(&g_sem_connect);
	}
	return SLSI_STATUS_SUCCESS;

errout:
	inAuto = false;
	sem_post(&g_sem_connect);
	sem_destroy(&g_sem_connect);
	return tc - 1;
}

int8_t doP2pStartConnectStopTest(int iterations, char *devaddr, char *pin, slsi_config_method_t method, uint8_t intent)
{
	int loop = 0;

	TESTPRINT("Trying to start/connect/Stop in P2P %d times,\ndevice address %s, pin %s, config method %d, intent %d", iterations, devaddr, pin, method, intent);

	for (loop = 0; loop < iterations; loop++) {
		printf("==> Trial: %d\n", loop + 1);
		sem_init(&g_sem_connect, 0, 0);
		printf("Start p2p mode\n");
		if (doStartP2p() == SLSI_STATUS_ERROR) {
			goto errout;
		} else {
			printf("Successfully started. Now lets connect out\n");
		}
		WiFiP2PRegisterConnectionHandler(&sw_connectionHandler);
		/*********** CONNECT OUT **************/
		printf("Connect to peer %s with config mode %s, pin %s and go intent %d\n", devaddr, method, pin, intent);
		if (doP2pConnect(devaddr, pin, method, intent) != SLSI_STATUS_SUCCESS) {
			goto errout;
		} else {
			printf("Connect sent - now lets wait for a connection is up\n");
			if (sem_wait(&g_sem_connect)) {
				printf("sem_wait failed");
			}
			printf("after semaphore is opened\n");
			if (g_join_result != SLSI_STATUS_SUCCESS) {
				goto errout;
			}
		}
		usleep(SANITY_P2P_TEST_SLEEP);
		/*********** STOP **************/
		printf("Stop p2p mode\n");
		if (doStop() == SLSI_STATUS_ERROR) {
			goto errout;
		} else {
			// we need to Register Link Callback handlers again as they are
			// unregistered in doStop
			WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
		}
		sem_post(&g_sem_connect);
		sem_destroy(&g_sem_connect);
	}
	return SLSI_STATUS_SUCCESS;

errout:
	inAuto = false;
	sem_post(&g_sem_connect);
	sem_destroy(&g_sem_connect);
	return tc - 1;
}
#endif

int8_t doP2pAddGroupRemoveGroupTest(int iterations, uint32_t timeout, int8_t channel)
{
	int loop = 0;

	TESTPRINT("\nTrying to start/add group/remove group/Stop in P2P %d times on channel %d\n", iterations, channel);
	for (loop = 0; loop < iterations; loop++) {
		printf("==> Trial: %d\n", loop + 1);
		printf("Start P2p mode\n");
		/**********START*************/
		if (doStartP2p() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		} else {
			printf("Successfully started. Now lets start the group\n");
			//sem_wait(&g_sem_connect);
		}
		/**********GROUP*************/
		printf("Start p2p group\n");
		if (doP2pAddGroup(channel, FALSE /* persistent */) == SLSI_STATUS_SUCCESS) {
			printf("Group added now waiting for %d seconds\n", timeout);
			usleep(SANITY_P2P_TEST_SLEEP);
			if (doP2pRemoveGroup() != SLSI_STATUS_SUCCESS) {
				inAuto = false;
				return tc - 1;
			}
		} else {
			inAuto = false;
			return tc - 1;
		}
		/*********** STOP **************/
		printf("Stop P2P mode\n");
		if (doStop() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		} else {
			/* we need to Register Link Callback handlers again as they are unregistered in doStop */
			WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
		}
	}
	return SLSI_STATUS_SUCCESS;
}

int8_t doP2pAddGroupIncomingTest(int iterations, char *devaddr, char *pin, slsi_config_method_t method, uint8_t intent, uint32_t timeout, int8_t channel)
{
	int loop = 0;

	TESTPRINT("Trying to start/add group/wait for connect/disconnect/Stop in P2P %d times\n", iterations);
	for (loop = 0; loop < iterations; loop++) {
		sem_init(&g_sem_connect, 0, 0);
		printf("==> Trial: %d\n", loop + 1);
		printf("Start P2p mode\n");
		/* set address to look for if it exists (else null) */
		g_devaddr = devaddr;
		/**********START*************/
		if (doStartP2p() == SLSI_STATUS_ERROR) {
			goto errout;
		} else {
			printf("Successfully started. Now lets start the group\n");
		}
		/**********GROUP*************/
		WiFiP2PRegisterConnectionHandler(&sw_connectionHandler);
		g_p2p_intent_in = intent;	/* store the intent for incoming connections */
		printf("Start p2p group\n");
		if (doP2pAddGroup(channel, FALSE /* persistent */) == SLSI_STATUS_SUCCESS) {
			printf("Group added now waiting for incoming connection\n");
			if (sem_wait(&g_sem_connect)) {
				printf("sem_wait failed:");    /* cause DOUBLE LOCK issue in static analysis tool */
			}

			/* TODO: eventually we could check the incoming connect against devaddr here. */
			printf("We should now have a connection - lets remove the group\n");
			if (doP2pRemoveGroup() != SLSI_STATUS_SUCCESS) {
				goto errout;
			}
			sem_post(&g_sem_connect);
			sem_destroy(&g_sem_connect);
		} else {
			goto errout;
		}
		/*********** STOP **************/
		printf("Stop P2P mode\n");
		if (doStop() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		} else {
			/* we need to Register Link Callback handlers again as they are unregistered in doStop */
			WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
		}
	}
	return SLSI_STATUS_SUCCESS;

errout:
	inAuto = false;
	sem_post(&g_sem_connect);
	sem_destroy(&g_sem_connect);
	return tc - 1;
}

int8_t doP2pChannelTest(void)
{
	/********* START P2P FOR ALL CHANNELS - COUNTRY=00 **********/
	int8_t p2p_channel = 0;

	WiFiSetCountryCode("00");
	printf("WiFiSetCountryCode 00\n");
	for (p2p_channel = 1; p2p_channel <= 14; p2p_channel++) {
		if (doP2pAddGroupRemoveGroupTest(1, 1, p2p_channel) != SLSI_STATUS_SUCCESS) {
			if (p2p_channel < 12) {	/* not allowed for country "00" so expect it to fail */
				inAuto = false;
				return tc - 1;
			} else {
				printf("Expected to fail - now stop P2P mode\n");
				if (doStop() == SLSI_STATUS_ERROR) {
					inAuto = false;
					return tc - 1;
				} else {
					/* we need to Register Link Callback handlers again as they are unregistered in doStop */
					WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
				}
			}
		} else {
			/* should fail above channel 11 */
			if (p2p_channel >= 12) {	/* not allowed for country "00" so expect it to fail */
				/* TODO: REMOVE if sucessful - this check should be added when FW and driver is fixed */
				printf("Expected to FAIL for channels above 11 - but returned SUCCESS - waiting for FW changes\n");
			} else {
				printf("Expected to fail\n");
			}
		}
	}
	/********* START P2P GROUP FOR ALL CHANNELS - COUNTRY=KR **********/
	WiFiSetCountryCode("KR");
	printf("WiFiSetCountryCode KR\n");
	for (p2p_channel = 1; p2p_channel <= 14; p2p_channel++) {
		printf("Testing on Channel %d\n", p2p_channel);
		if (doP2pAddGroupRemoveGroupTest(1, 1, p2p_channel) != SLSI_STATUS_SUCCESS) {
			if (p2p_channel != 14) {	/* not allowed for country "KR" so expect it to fail */
				inAuto = false;
				return tc - 1;
			} else {
				printf("Expected to fail - now stop P2P mode\n");
				if (doStop() == SLSI_STATUS_ERROR) {
					inAuto = false;
					return tc - 1;
				} else {
					/* we need to Register Link Callback handlers again as they are unregistered in doStop */
					WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
				}
			}
		} else {
			/* Channel 14 should fail */
			if (p2p_channel == 14) {
				inAuto = false;
				return tc - 1;
			}
		}
	}
	/********* START P2P GROUP FOR ALL CHANNELS - COUNTRY=JP **********/
	WiFiSetCountryCode("JP");
	printf("WiFiSetCountryCode JP\n");
	for (p2p_channel = 1; p2p_channel <= 14; p2p_channel++) {
		if (doP2pAddGroupRemoveGroupTest(1, 1, p2p_channel) != SLSI_STATUS_SUCCESS) {
			if (p2p_channel != 14) {	/* not allowed for P2P even for JP countrycode */
				inAuto = false;
				return tc - 1;
			} else {
				printf("Expected to fail - now stop P2P mode\n");
				if (doStop() == SLSI_STATUS_ERROR) {
					inAuto = false;
					return tc - 1;
				} else {
					/* we need to Register Link Callback handlers again as they are unregistered in doStop */
					WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
				}
			}
		} else {
			/* Channel 14 should fail */
			if (p2p_channel == 14) {
				inAuto = false;
				return tc - 1;
			}
		}
	}
	WiFiSetCountryCode("00");
	printf("WiFiSetCountryCode 00\n");
	return SLSI_STATUS_SUCCESS;
}

int8_t doP2pListenIncomingTest(int iterations, char *devaddr, char *pin, slsi_config_method_t method, uint8_t intent, uint32_t timeout)
{
	int loop = 0;

	TESTPRINT("Trying to start/listen/connect/disconnect/Stop in  P2P %d times\n", iterations);
	for (loop = 0; loop < iterations; loop++) {
		sem_init(&g_sem_connect, 0, 0);
		printf("==> Trial: %d\n", loop + 1);
		printf("Start P2p mode\n");
		/* set address to look for if it exists (else null) */
		g_devaddr = devaddr;
		/**********START*************/
		if (doStartP2p() == SLSI_STATUS_ERROR) {
			goto errout;
		} else {
			printf("Successfully started. Now lets start to listen\n");
		}
		/**********LISTEN*************/
		WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
		WiFiP2PRegisterFindCallback(&sw_findResultHandler);
		WiFiP2PRegisterConnectionHandler(&sw_connectionHandler);
		g_p2p_intent_in = intent;	/* store the intent for incoming connections */
		printf("Start p2p listen\n");
		if (doP2pListen() == SLSI_STATUS_SUCCESS) {
			printf("listening for incoming connection\n");
			if (sem_wait(&g_sem_connect)) {
				printf("sem_wait failed:");
			}
			/* TODO: eventually we could check the incoming connect against devaddr here. */
			printf("We should now have a connection - lets disconnect\n");
			if (doP2pDisconnect() != SLSI_STATUS_SUCCESS) {
				goto errout;
			}
		} else {
			goto errout;
		}
		/***********STOP**************/
		printf("Stop P2P mode\n");
		if (doStop() == SLSI_STATUS_ERROR) {
			goto errout;
		} else {
			/* we need to Register Link Callback handlers again as they are unregistered in doStop */
			WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
		}
		sem_post(&g_sem_connect);
		sem_destroy(&g_sem_connect);
	}
	return SLSI_STATUS_SUCCESS;

errout:
	inAuto = false;
	sem_post(&g_sem_connect);
	sem_destroy(&g_sem_connect);
	return tc - 1;
}

int8_t doP2pFindTest(int iterations, char *devaddr, uint32_t timeout)
{
	int loop = 0;

	TESTPRINT("Trying to Start/FindTimeout/Stop in  P2P %d times\n", iterations);
	for (loop = 0; loop < iterations; loop++) {
		printf("==> Trial: %d\n", loop + 1);
		printf("Start P2P mode\n");
		/* set address to look for if it exists (else null) */
		g_devaddr = devaddr;
		/**********START*************/
		if (doStartP2p() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		} else {
			printf("Successfully started. Now lets start to do p2p find\n");
		}
		/**********LISTEN*************/
		WiFiP2PRegisterFindCallback(&sw_findResultHandler);
		printf("Start p2p find\n");
		if (doP2pFind(timeout, NULL) == SLSI_STATUS_SUCCESS) {
			printf("finding for %d seconds\n", timeout);
			sleep(timeout + 1);	/* need a bit extra to take care of latency */
			printf("Find should now be done\n");
		} else {
			return tc - 1;
		}
		/***********STOP**************/
		printf("Stop P2P mode\n");
		if (doStop() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		} else {
			/* we need to Register Link Callback handlers again as they are unregistered in doStop */
			WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
		}
	}
	return SLSI_STATUS_SUCCESS;
}

int8_t doP2pListenTest(int iterations, char *devaddr, uint32_t wait_time)
{
	int loop = 0;

	TESTPRINT("Trying to Start/Listen/Sleep/FullStop in  P2P %d times\n", iterations);
	for (loop = 0; loop < iterations; loop++) {
		printf("==> Trial: %d\n", loop + 1);
		printf("Start P2P mode\n");
		/* set address to look for if it exists (else null) */
		g_devaddr = devaddr;
		/**********START*************/
		if (doStartP2p() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		} else {
			printf("Successfully started. Now lets start to listen\n");
		}
		/**********LISTEN*************/
		WiFiP2PRegisterFindCallback(sw_findResultHandler);
		printf("Start p2p listen\n");
		if (doP2pListen() == SLSI_STATUS_SUCCESS) {
			printf("listening for %d seconds\n", wait_time);
			sleep(wait_time);
			printf("Listen should now be done\n");
		} else {
			return SLSI_STATUS_ERROR;
		}
		/***********STOP**************/
		printf("Stop P2P mode\n");
		if (doStop() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		} else {
			/* we need to Register Link Callback handlers again as they are unregistered in doStop */
			WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
		}
	}
	return SLSI_STATUS_SUCCESS;
}

int8_t doP2pFindStopTest(int iterations, char *devaddr, uint32_t wait_time)
{
	int loop = 0;

	TESTPRINT("Trying to Start/Find/StopFind/Stop in P2P %d times\n", iterations);
	for (loop = 0; loop < iterations; loop++) {
		printf("==> Trial: %d\n", loop + 1);
		printf("Start P2p mode\n");
		/* set address to look for if it exists (else null) */
		g_devaddr = devaddr;
		/**********START*************/
		if (doStartP2p() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		} else {
			printf("Successfully started. Now lets start to do p2p find\n");
		}
		/**********LISTEN*************/
		WiFiP2PRegisterFindCallback(sw_findResultHandler);
		printf("Start P2P find\n");
		if (doP2pFind(0, NULL) == SLSI_STATUS_SUCCESS) {
			printf("finding until stopped\n");
			sleep(wait_time);
			printf("Stopping P2P find\n");
			if (doP2pStopFind() != SLSI_STATUS_SUCCESS) {
				inAuto = false;
				return tc - 1;
			}
		} else {
			inAuto = false;
			return tc - 1;
		}
		/***********STOP**************/
		printf("Stop P2P mode\n");
		if (doStop() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		} else {
			/* we need to Register Link Callback handlers again as they are unregistered in doStop */
			WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
		}
	}
	return SLSI_STATUS_SUCCESS;
}

int8_t doP2pListenStopTest(int iterations, char *devaddr, uint32_t wait_time)
{
	int loop = 0;

	TESTPRINT("Trying to Start/Listen/StopListen/Stop in P2P %d times\n", iterations);
	for (loop = 0; loop < iterations; loop++) {
		printf("==> Trial: %d\n", loop + 1);
		printf("Start P2p mode\n");
		/* set address to look for if it exists (else null) */
		g_devaddr = devaddr;
		/**********START*************/
		if (doStartP2p() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		} else {
			printf("Successfully started. Now lets start to listen\n");
		}
		/**********LISTEN*************/
		printf("Start p2p listen\n");
		if (doP2pListen() == SLSI_STATUS_SUCCESS) {
			printf("listening until stopped\n");
			sleep(wait_time);
			printf("Stopping P2P listen\n");
			if (doP2pStopListen() != SLSI_STATUS_SUCCESS) {
				inAuto = false;
				return tc - 1;
			}
		} else {
			inAuto = false;
			return tc - 1;
		}
		/***********STOP**************/
		printf("Stop softAP mode\n");
		if (doStop() == SLSI_STATUS_ERROR) {
			inAuto = false;
			return tc - 1;
		} else {
			/* we need to Register Link Callback handlers again as they are unregistered in doStop */
			WiFiP2PRegisterLinkCallback(&sw_p2pLinkUpHandler, &sw_p2pLinkDownHandler);
		}
	}
	return SLSI_STATUS_SUCCESS;
}

#endif	/* CONFIG_SLSI_WIFI_P2P_API */
