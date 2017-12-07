
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
#ifndef NETTEST_FUNCTIONS_H__
#define NETTEST_FUNCTIONS_H__

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <slsi_wifi_api.h>

int8_t tc;
int8_t doAutoTest(char *filename);
int8_t doSanityTest(int iterations, char *softApName);
int8_t doNightlyTest(int iterations, char *softApName, char *p2pGOname);
int slsi_tcp_server(void);
int slsi_tcp_client(void);
int slsi_udp_server(void);
int slsi_udp_client(void);
#ifdef CONFIG_SLSI_WIFI_P2P_API
int8_t doP2pSanityTest(int iterations, char *p2pGOname);
int8_t doP2pStartStopTest(int iterations);
int8_t doP2pStartConnectStopTest(int iterations, char *devaddr, char *pin, slsi_config_method_t method, uint8_t intent);
int8_t doP2pListenStopTest(int iterations, char *devaddr, uint32_t wait_time);
int8_t doP2pFindStopTest(int iterations, char *devaddr, uint32_t wait_time);
int8_t doP2pListenTest(int iterations, char *devaddr, uint32_t wait_time);
int8_t doP2pFindTest(int iterations, char *devaddr, uint32_t timeout);
void sw_p2pTestLinkUpHandler(slsi_reason_t *);
void sw_p2pTestLinkDownHandler(slsi_reason_t *);
void sw_findResultHandler(slsi_find_info_t *find_info);
#endif	/* CONFIG_SLSI_WIFI_P2P_API */

#endif	/* NETTEST_FUNCTIONS_H__ */
