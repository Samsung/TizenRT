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

#include <stdio.h>
#include "itc_internal.h"
#include <dm/dm_lwm2m.h>
#include <dm/dm_error.h>

#define ITC_DM_IPADDR_LEN 20
#define ITC_DM_SERVER_PORT 10
#define LIFE_TIME 300
#define SEC_1 1

#ifndef CONFIG_ITC_DM_SERVER_IPADDR
#define CONFIG_ITC_DM_SERVER_IPADDR "192.168.0.105"
#endif
#ifndef CONFIG_ITC_DM_SERVER_PORT
#define CONFIG_ITC_DM_SERVER_PORT "5683"
#endif

struct dm_lwm2m_context_s test_data_itc = { { CONFIG_ITC_DM_SERVER_IPADDR, CONFIG_ITC_DM_SERVER_PORT, false }, { LIFE_TIME } };


/**
* @testcase             itc_dm_lwm2m_start_stop_client_p
* @brief                Start and stop DM LWM2M client session
* @scenario             Pass valid DM LWM2M context
* @apicovered           dm_lwm2m_start_client, dm_lwm2m_stop_client
* @precondition         WiFi connected, DM LWM2M client is not started yet
* @postcondition        NA
*/
static void itc_dm_lwm2m_start_stop_client_p(void)
{
	int ret;
	ret = dm_lwm2m_start_client(&test_data_itc);
	TC_ASSERT_EQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_get_client_lifetime_p
* @brief                Get LWM2M client lifetime
* @scenario             Pass valid argument to hold client lifetime
* @apicovered           dm_lwm2m_get_client_lifetime
* @precondition         DM LWM2M client has been started
* @postcondition        DM LWM2M client has been stopped
*/
static void itc_dm_lwm2m_get_client_lifetime_p(void)
{
	int life = -1;
	int ret = -1;
	ret = dm_lwm2m_start_client(&test_data_itc);
	TC_ASSERT_EQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_get_client_lifetime(&life);
	TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_client_lifetime", ret, DM_ERROR_NONE, dm_lwm2m_stop_client());
	TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_client_lifetime", (life < 0), false, dm_lwm2m_stop_client());

	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_get_server_address_p
* @brief                Get LWM2M Server IP addr
* @scenario             Pass valid argument to hold LWM2M server IP addr
* @apicovered           dm_lwm2m_get_server_address
* @precondition         DM LWM2M client has been started
* @postcondition        DM LWM2M client has been stopped
*/
static void itc_dm_lwm2m_get_server_address_p(void)
{
	int ret = dm_lwm2m_start_client(&test_data_itc);
	TC_ASSERT_EQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);

	char ip_addr[ITC_DM_IPADDR_LEN];
	char *server_addr = CONFIG_ITC_DM_SERVER_IPADDR;
	ret = dm_lwm2m_get_server_address(ip_addr);
	TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_server_address", ret, DM_ERROR_NONE, dm_lwm2m_stop_client());
	TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_server_address", strncmp(ip_addr, server_addr, sizeof(server_addr)), 0, dm_lwm2m_stop_client());

	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_get_server_port_p
* @brief                Get LWM2M Server IP port
* @scenario             Pass valid argument to API to hold LWM2M server port
* @apicovered           dm_lwm2m_get_server_port
* @precondition         DM LWM2M client has been started
* @postcondition        DM LWM2M client has been stopped
*/
static void itc_dm_lwm2m_get_server_port_p(void)
{
	char port[ITC_DM_SERVER_PORT];
	char *server_port = CONFIG_ITC_DM_SERVER_PORT;
	int ret = dm_lwm2m_start_client(&test_data_itc);
	TC_ASSERT_EQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_get_server_port(port);
	TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_server_port", ret, DM_ERROR_NONE, dm_lwm2m_stop_client());
	TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_server_port", strncmp(port, server_port, sizeof(server_port)), 0, dm_lwm2m_stop_client());

	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_get_client_state_p
* @brief                Get LWM2M Client state
* @scenario             Pass valid argument to API
* @apicovered           dm_lwm2m_get_client_state
* @precondition         DM LWM2M client has been started
* @postcondition        DM LWM2M client has been stopped
*/
static void itc_dm_lwm2m_get_client_state_p(void)
{
	dm_lwm2m_client_state_e state;
	int ret = dm_lwm2m_start_client(&test_data_itc);
	TC_ASSERT_EQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_get_client_state(&state);
	TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_client_state", ret, DM_ERROR_NONE, dm_lwm2m_stop_client());
	TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_client_state", (state < 0), false, dm_lwm2m_stop_client());

	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_display_client_resource_p
* @brief                Display LWM2M Client resource
* @scenario             Pass valid argument to API
* @apicovered           dm_lwm2m_display_client_resource
* @precondition         DM LWM2M client has been started
* @postcondition        DM LWM2M client has been stopped
*/
static void itc_dm_lwm2m_display_client_resource_p(void)
{
	int ret = dm_lwm2m_start_client(&test_data_itc);
	TC_ASSERT_EQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);
	//wait for client to start
	sleep(SEC_1);
	char arg_buffer[10];
	memset(arg_buffer, 0x00, sizeof(arg_buffer));
	strcpy(arg_buffer, "/8/0");

	ret = dm_lwm2m_display_client_resource(arg_buffer);
	TC_ASSERT_EQ_CLEANUP("dm_lwm2m_display_client_resource", ret, DM_ERROR_NONE, dm_lwm2m_stop_client());

	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_start_client_p_restart_client
* @brief                Start and restart DM LWM2M client session
* @scenario             Pass valid DM_ERROR_ALREADY_STARTED
* @apicovered           dm_lwm2m_start_client
* @precondition         WiFi connected, DM LWM2M client is not started yet
* @postcondition        NA
*/
static void itc_dm_lwm2m_start_client_p_restart_client(void)
{
	int ret;
	ret = dm_lwm2m_start_client(&test_data_itc);
	TC_ASSERT_EQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_start_client(&test_data_itc);
	TC_ASSERT_EQ_CLEANUP("dm_lwm2m_start_client", ret, DM_ERROR_ALREADY_STARTED, dm_lwm2m_stop_client());

	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_stop_client_p_restop_client
* @brief                Stop and restop DM LWM2M client session
* @scenario             Pass valid DM_ERROR_ALREADY_STOPPED
* @apicovered           dm_lwm2m_stop_client
* @precondition         WiFi connected, DM LWM2M client is started
* @postcondition        NA
*/
static void itc_dm_lwm2m_stop_client_p_restop_client(void)
{
	int ret;
	ret = dm_lwm2m_start_client(&test_data_itc);
	TC_ASSERT_EQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_ALREADY_STOPPED);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_get_client_lifetime_n_before_start
* @brief                Get LWM2M client lifetime
* @scenario             Pass with return vlaue of less than DM_ERROR_NONE
* @apicovered           dm_lwm2m_get_client_lifetime
* @precondition         NA
* @postcondition        NA
*/
static void itc_dm_lwm2m_get_client_lifetime_n_before_start(void)
{
	int life = -1;
	int ret = -1;

	ret = dm_lwm2m_get_client_lifetime(&life);
	TC_ASSERT_EQ("dm_lwm2m_get_client_lifetime", ret, DM_ERROR_NO_DATA);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_get_client_lifetime_n_after_close
* @brief                Get LWM2M client lifetime
* @scenario             Pass with return vlaue of less than DM_ERROR_NONE
* @apicovered           dm_lwm2m_get_client_lifetime
* @precondition         DM LWM2M client has been started and stopped
* @postcondition        NA
*/
static void itc_dm_lwm2m_get_client_lifetime_n_after_close(void)
{
	int life = -1;
	int ret = -1;
	ret = dm_lwm2m_start_client(&test_data_itc);
	TC_ASSERT_EQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_get_client_lifetime(&life);
	TC_ASSERT_EQ("dm_lwm2m_get_client_lifetime", ret, DM_ERROR_NO_DATA);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_get_server_address_n_before_start
* @brief                Get LWM2M Server IP addr
* @scenario             Pass with return vlaue of less than DM_ERROR_NONE
* @apicovered           dm_lwm2m_get_server_address
* @precondition         NA
* @postcondition        NA
*/
static void itc_dm_lwm2m_get_server_address_n_before_start(void)
{
	char ip_addr[ITC_DM_IPADDR_LEN];
	int ret = dm_lwm2m_get_server_address(ip_addr);
	TC_ASSERT_EQ("dm_lwm2m_get_server_address", ret, DM_ERROR_NO_DATA);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_get_server_address_n_after_close
* @brief                Get LWM2M Server IP addr
* @scenario             Pass with return vlaue of less than DM_ERROR_NONE
* @apicovered           dm_lwm2m_get_server_address
* @precondition         DM LWM2M client has been started and stop
* @postcondition        NA
*/
static void itc_dm_lwm2m_get_server_address_n_after_close(void)
{
	int ret = dm_lwm2m_start_client(&test_data_itc);
	TC_ASSERT_EQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_NONE);

	char ip_addr[ITC_DM_IPADDR_LEN];
	ret = dm_lwm2m_get_server_address(ip_addr);
	TC_ASSERT_EQ("dm_lwm2m_get_server_address", ret, DM_ERROR_NO_DATA);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_get_server_port_n_before_start
* @brief                Get LWM2M Server IP port
* @scenario             Pass with return vlaue of less than DM_ERROR_NONE
* @apicovered           dm_lwm2m_get_server_port
* @precondition         NA
* @postcondition        NA
*/
static void itc_dm_lwm2m_get_server_port_n_before_start(void)
{
	char port[ITC_DM_SERVER_PORT];

	int ret = dm_lwm2m_get_server_port(port);
	TC_ASSERT_EQ("dm_lwm2m_get_server_port", ret, DM_ERROR_NO_DATA);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_get_server_port_n_after_close
* @brief                Get LWM2M Server IP port
* @scenario             Pass with return vlaue of less than DM_ERROR_NONE
* @apicovered           dm_lwm2m_get_server_port
* @precondition         DM LWM2M client has been started and stopped
* @postcondition        NA
*/
static void itc_dm_lwm2m_get_server_port_n_after_close(void)
{
	char port[ITC_DM_SERVER_PORT];
	int ret = dm_lwm2m_start_client(&test_data_itc);
	TC_ASSERT_EQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_get_server_port(port);
	TC_ASSERT_EQ("dm_lwm2m_get_server_port", ret, DM_ERROR_NO_DATA);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_get_client_state_n_before_start
* @brief                Get LWM2M Client state
* @scenario             Pass with return vlaue of less than DM_ERROR_NONE
* @apicovered           dm_lwm2m_get_client_state
* @precondition         NA
* @postcondition        NA
*/
static void itc_dm_lwm2m_get_client_state_n_before_start(void)
{
	dm_lwm2m_client_state_e state;

	int ret = dm_lwm2m_get_client_state(&state);
	TC_ASSERT_EQ("dm_lwm2m_get_client_state", state, DM_LWM2M_CLIENT_STOPPED);
	TC_ASSERT_EQ("dm_lwm2m_get_client_state", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_get_client_state_n_after_close
* @brief                Get LWM2M Client state
* @scenario             Pass with return vlaue of less than DM_ERROR_NONE
* @apicovered           dm_lwm2m_get_client_state
* @precondition         DM LWM2M client has been started and stopped
* @postcondition        NA
*/
static void itc_dm_lwm2m_get_client_state_n_after_close(void)
{
	dm_lwm2m_client_state_e state;
	int ret = dm_lwm2m_start_client(&test_data_itc);
	TC_ASSERT_EQ("dm_lwm2m_start_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_stop_client();
	TC_ASSERT_EQ("dm_lwm2m_stop_client", ret, DM_ERROR_NONE);

	ret = dm_lwm2m_get_client_state(&state);
	TC_ASSERT_EQ("dm_lwm2m_get_client_state", state, DM_LWM2M_CLIENT_STOPPED);
	TC_ASSERT_EQ("dm_lwm2m_get_client_state", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_display_client_resource_p_with_no_data
* @brief                Display LWM2M Client resource
* @scenario             Pass valid argument to API
* @apicovered           dm_lwm2m_display_client_resource
* @precondition         NA
* @postcondition        NA
*/
static void itc_dm_lwm2m_display_client_resource_p_with_no_data(void)
{
	int ret;
	char arg_buffer[10];
	memset(arg_buffer, 0x00, sizeof(arg_buffer));
	strcpy(arg_buffer, "/8/0");

	ret = dm_lwm2m_display_client_resource(arg_buffer);
	TC_ASSERT_EQ("dm_lwm2m_display_client_resource", ret, DM_ERROR_NO_DATA);

	TC_SUCCESS_RESULT();
}

/**
* @testcase             itc_dm_lwm2m_display_client_resource_n
* @brief                Display LWM2M Client resource
* @scenario             Pass invalid argument to API
* @apicovered           dm_lwm2m_display_client_resource
* @precondition         NA
* @postcondition        NA
*/
static void itc_dm_lwm2m_display_client_resource_n(void)
{
	int ret = dm_lwm2m_display_client_resource(NULL);
	TC_ASSERT_EQ("dm_lwm2m_display_client_resource", ret, DM_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name:
 ****************************************************************************/
int itc_dm_lwm2m_testcase_main(void)
{
#ifdef CONFIG_ITC_DM_START
#ifdef CONFIG_ITC_DM_CLOSE

	itc_dm_lwm2m_start_stop_client_p();
	sleep(SEC_1);
	itc_dm_lwm2m_start_client_p_restart_client();
	sleep(SEC_1);
	itc_dm_lwm2m_stop_client_p_restop_client();
	sleep(SEC_1);
#ifdef CONFIG_ITC_DM_GET_CLIENT_LIFETIME
	itc_dm_lwm2m_get_client_lifetime_p();
	sleep(SEC_1);
	itc_dm_lwm2m_get_client_lifetime_n_before_start();
	sleep(SEC_1);
	itc_dm_lwm2m_get_client_lifetime_n_after_close();
	sleep(SEC_1);
#endif
#ifdef CONFIG_ITC_DM_GET_SERVER_ADDR
	itc_dm_lwm2m_get_server_address_p();
	sleep(SEC_1);
	itc_dm_lwm2m_get_server_address_n_before_start();
	sleep(SEC_1);
	itc_dm_lwm2m_get_server_address_n_after_close();
	sleep(SEC_1);
#endif
#ifdef CONFIG_ITC_DM_GET_SERVER_PORT
	itc_dm_lwm2m_get_server_port_p();
	sleep(SEC_1);
	itc_dm_lwm2m_get_server_port_n_before_start();
	sleep(SEC_1);
	itc_dm_lwm2m_get_server_port_n_after_close();
	sleep(SEC_1);
#endif
#ifdef CONFIG_ITC_DM_GET_CLIENT_STATE
	itc_dm_lwm2m_get_client_state_p();
	sleep(SEC_1);
	itc_dm_lwm2m_get_client_state_n_before_start();
	sleep(SEC_1);
	itc_dm_lwm2m_get_client_state_n_after_close();
	sleep(SEC_1);
#endif
#ifdef CONFIG_ITC_DM_DISPLAY_CLIENT_RESOURCE
	itc_dm_lwm2m_display_client_resource_p();
	sleep(SEC_1);
	itc_dm_lwm2m_display_client_resource_p_with_no_data();//TC FAIL, Line no. 407, Should return DM_ERROR_NO_DATA, but it returning DM_ERROR_NONE
	sleep(SEC_1);
	itc_dm_lwm2m_display_client_resource_n();
	sleep(SEC_1);
#endif
#endif
#endif
	return 0;
}
