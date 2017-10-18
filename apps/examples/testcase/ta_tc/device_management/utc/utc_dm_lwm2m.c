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

#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netutils/netlib.h>
#include <sys/socket.h>

#include "utc_internal.h"
#include <dm/dm_lwm2m.h>
#include <dm/dm_error.h>
#include "tc_common.h"

#define UTC_DM_IPADDR_LEN 20
#define UTC_DM_SERVER_PORT 6

extern dm_lwm2m_client_state_e dm_state;

struct dm_lwm2m_context_s test_data1 = { {CONFIG_UTC_DM_SERVER_IPADDR, CONFIG_UTC_DM_SERVER_PORT, false}, {300} };
struct dm_lwm2m_context_s test_data2 = { {CONFIG_UTC_DM_SERVER_IPADDR, "123AB", false}, {300} };

/**
* @testcase             utc_dm_lwm2m_start_client_p
* @brief                Start DM LWM2M client session
* @scenario             Pass valid DM LWM2M context
* @apicovered           dm_lwm2m_start_client
* @precondition         WiFi connected, DM LWM2M client is not started yet
* @postcondition
*/
static void utc_dm_lwm2m_start_client_p(void)
{
	int ret;
	ret = dm_lwm2m_start_client(&test_data1);
	TC_ASSERT_EQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             utc_dm_lwm2m_start_client_n
* @brief                Start DM LWM2M client session
* @scenario             Passing invalid LWM2M server port
* @apicovered           dm_lwm2m_start_client
* @precondition         WiFi connected, DM LWM2M client is not started yet
* @postcondition
*/
static void utc_dm_lwm2m_start_client_n(void)
{
	int ret;
	ret = dm_lwm2m_start_client(&test_data2);
	TC_ASSERT_NEQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             utc_dm_lwm2m_get_client_lifetime_n
* @brief                Get LWM2M client lifetime
* @scenario             Pass NULL argument to API
* @apicovered           dm_lwm2m_get_client_lifetime
* @precondition         DM LWM2M client has been started
* @postcondition
*/
static void utc_dm_lwm2m_get_client_lifetime_n(void)
{
	int ret;
	ret = dm_lwm2m_get_client_lifetime(NULL);
	TC_ASSERT_EQ("dm_lwm2m_get_client_lifetime", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             utc_dm_lwm2m_get_client_lifetime_p
* @brief                Get LWM2M client lifetime
* @scenario             Pass valid argument to hold client lifetime
* @apicovered           dm_lwm2m_get_client_lifetime
* @precondition         DM LWM2M client has been started
* @postcondition
*/
static void utc_dm_lwm2m_get_client_lifetime_p(void)
{
	int life;
	int ret;
	ret = dm_lwm2m_get_client_lifetime(&life);
	TC_ASSERT_EQ("dm_lwm2m_get_client_lifetime", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             utc_dm_lwm2m_get_server_address_n
* @brief                Get LWM2M Server IP addr
* @scenario             Pass NULL argument to API
* @apicovered           dm_lwm2m_get_server_address
* @precondition         DM LWM2M client has been started
* @postcondition
*/
static void utc_dm_lwm2m_get_server_address_n(void)
{
	int ret;
	ret = dm_lwm2m_get_server_address(NULL);
	TC_ASSERT_EQ("dm_lwm2m_get_server_address", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             utc_dm_lwm2m_get_server_address_p
* @brief                Get LWM2M Server IP addr
* @scenario             Pass valid argument to hold LWM2M server IP addr
* @apicovered           dm_lwm2m_get_server_address
* @precondition         DM LWM2M client has been started
* @postcondition
*/
static void utc_dm_lwm2m_get_server_address_p(void)
{
	char ipAddr[UTC_DM_IPADDR_LEN];
	int ret = dm_lwm2m_get_server_address(ipAddr);
	TC_ASSERT_EQ("dm_lwm2m_get_server_address", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             utc_dm_lwm2m_get_server_port_n
* @brief                Get LWM2M Server IP port
* @scenario             Pass NULL argument to API
* @apicovered           dm_lwm2m_get_server_port
* @precondition         DM LWM2M client has been started
* @postcondition
*/
static void utc_dm_lwm2m_get_server_port_n(void)
{
	int ret;
	ret = dm_lwm2m_get_server_port(NULL);
	TC_ASSERT_EQ("dm_lwm2m_get_server_port", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             utc_dm_lwm2m_get_server_port_p
* @brief                Get LWM2M Server IP port
* @scenario             Pass valid argument to API to hold LWM2M server port
* @apicovered           dm_lwm2m_get_server_port
* @precondition         DM LWM2M client has been started
* @postcondition
*/
static void utc_dm_lwm2m_get_server_port_p(void)
{
	char port[UTC_DM_SERVER_PORT];
	int ret = dm_lwm2m_get_server_port(port);
	TC_ASSERT_EQ("dm_lwm2m_get_server_port", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             utc_dm_lwm2m_get_client_state_n
* @brief                Get LWM2M Client state
* @scenario             Pass NULL argument to API
* @apicovered           dm_lwm2m_get_client_state
* @precondition
* @postcondition
*/
static void utc_dm_lwm2m_get_client_state_n(void)
{
	int ret;
	ret = dm_lwm2m_get_client_state(NULL);
	TC_ASSERT_EQ("dm_lwm2m_get_client_state", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             utc_dm_lwm2m_get_client_state_p
* @brief                Get LWM2M Client state
* @scenario             Pass valid argument to API
* @apicovered           dm_lwm2m_get_client_state
* @precondition
* @postcondition
*/
static void utc_dm_lwm2m_get_client_state_p(void)
{
	dm_lwm2m_client_state_e state_val;
	int ret;
	ret = dm_lwm2m_get_client_state(&state_val);
	TC_ASSERT_EQ("dm_lwm2m_get_client_state", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             utc_dm_lwm2m_stop_client_n
* @brief                Get LWM2M Client state
* @scenario             DM LWM2M client has been already stopped
* @apicovered           dm_lwm2m_stop_client
* @precondition         DM LWM2M client has been started
* @postcondition
*/
static void utc_dm_lwm2m_stop_client_n(void)
{
	int ret;
	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_ALREADY_STOPPED);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             utc_dm_lwm2m_stop_client_p
* @brief                Get LWM2M Client state
* @scenario             DM L2M2M client is active
* @apicovered           dm_lwm2m_stop_client
* @precondition         DM LWM2M client has been started
* @postcondition
*/
static void utc_dm_lwm2m_stop_client_p(void)
{
	int ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             utc_dm_lwm2m_display_client_resource_n
* @brief                Display LWM2M Client resource
* @scenario             Pass NULL argument to API
* @apicovered           dm_lwm2m_display_client_resource
* @precondition         DM LWM2M client has been started
* @postcondition
*/
static void utc_dm_lwm2m_display_client_resource_n(void)
{
	int ret;
	ret = dm_lwm2m_display_client_resource(NULL);
	TC_ASSERT_EQ("dm_lwm2m_display_client_resource", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

static void utc_dm_lwm2m_display_client_resource_p(void)
{
	int ret;
	char argbuffer[10];
	memset(argbuffer, 0x00, sizeof(argbuffer));
	strcpy(argbuffer, "/8/0");
	ret = dm_lwm2m_display_client_resource(argbuffer);
	TC_ASSERT_EQ("dm_lwm2m_display_client_resource", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name:
 ****************************************************************************/

int dm_lwm2m_testcase_main(void)
{
#ifdef CONFIG_TC_DM_START
	utc_dm_lwm2m_start_client_p();
	utc_dm_lwm2m_start_client_n();
#endif
#ifdef CONFIG_TC_DM_GET_CLIENT_LIFETIME
	utc_dm_lwm2m_get_client_lifetime_n();
	utc_dm_lwm2m_get_client_lifetime_p();
#endif
#ifdef CONFIG_TC_DM_GET_SERVER_ADDR
	utc_dm_lwm2m_get_server_address_n();
	utc_dm_lwm2m_get_server_address_p();
#endif
#ifdef CONFIG_TC_DM_GET_SERVER_PORT
	utc_dm_lwm2m_get_server_port_n();
	utc_dm_lwm2m_get_server_port_p();
#endif
#ifdef CONFIG_TC_DM_GET_CLIENT_STATE
	utc_dm_lwm2m_get_client_state_n();
	utc_dm_lwm2m_get_client_state_p();
#endif

#ifdef CONFIG_TC_DM_DISPLAY_CLIENT_RESOURCE
	utc_dm_lwm2m_display_client_resource_n();
	utc_dm_lwm2m_display_client_resource_p();
#endif

#ifdef CONFIG_TC_DM_CLOSE
	if (dm_state == DM_LWM2M_CLIENT_STOPPED) {
		utc_dm_lwm2m_stop_client_n();
		utc_dm_lwm2m_stop_client_p();
	} else {
		utc_dm_lwm2m_stop_client_p();
		utc_dm_lwm2m_stop_client_n();
	}
#endif
	return 0;
}
