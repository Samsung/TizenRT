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
#include <protocols/netlib.h>
#include <sys/socket.h>
#include "itc_internal.h"
#include "dm_lwm2m.h"
#include "dm_error.h"
#include <tinyara/wqueue.h>

#define ITC_DM_IPADDR_LEN 20
#define ITC_DM_SERVER_PORT 10

struct dm_lwm2m_context_s test_data_itc = { {"192.168.0.105", "5683", false}, {300} };
static struct work_s itc_dm_wq;

/**
* @testcase             display_resource
* @brief                utillity function for itc_dm_lwm2m_display_client_resource_p
* @scenario             utillity function for itc_dm_lwm2m_display_client_resource_p
* @apicovered           dm_lwm2m_display_client_resource
* @precondition         DM LWM2M client has been started
* @postcondition        DM LWM2M client has been stopped
*/
static int display_resource(FAR void *arg)
{
    int ret;
    char argbuffer[10];
    memset(argbuffer, 0x00, sizeof(argbuffer));
    strcpy(argbuffer, "/8/0");

    ret = dm_lwm2m_display_client_resource(argbuffer);
    TC_ASSERT_EQ_CLEANUP("dm_lwm2m_display_client_resource", ret, DM_ERROR_NONE, dm_lwm2m_stop_client());
    ret = dm_lwm2m_stop_client();
    TC_ASSERT_EQ("dm_lwm2m_stop_client" , ret , DM_ERROR_NONE);
}

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
    TC_ASSERT_EQ("dm_lwm2m_start_client" , ret , DM_ERROR_NONE);

    ret = dm_lwm2m_stop_client();
    TC_ASSERT_EQ("dm_lwm2m_stop_client" , ret , DM_ERROR_NONE);

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
    TC_ASSERT_EQ("dm_lwm2m_start_client" , ret , DM_ERROR_NONE);

    ret = dm_lwm2m_get_client_lifetime(&life);
    TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_client_lifetime", ret, DM_ERROR_NONE, dm_lwm2m_stop_client());
    TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_client_lifetime", (life < 0), false, dm_lwm2m_stop_client());

    ret = dm_lwm2m_stop_client();
    TC_ASSERT_EQ("dm_lwm2m_stop_client" , ret , DM_ERROR_NONE);

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
    TC_ASSERT_EQ("dm_lwm2m_start_client" , ret , DM_ERROR_NONE);

    char ipAddr[ITC_DM_IPADDR_LEN];
    ret = dm_lwm2m_get_server_address(ipAddr);
    TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_server_address", ret, DM_ERROR_NONE, dm_lwm2m_stop_client());
    TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_server_address" , ipAddr[3] , '.', dm_lwm2m_stop_client());

    ret = dm_lwm2m_stop_client();
    TC_ASSERT_EQ("dm_lwm2m_stop_client" , ret , DM_ERROR_NONE);

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
    int ret = dm_lwm2m_start_client(&test_data_itc);
    TC_ASSERT_EQ("dm_lwm2m_start_client" , ret , DM_ERROR_NONE);

    ret = dm_lwm2m_get_server_port(port);
    TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_server_port", ret, DM_ERROR_NONE, dm_lwm2m_stop_client());
    TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_server_port", (port[0] < 0), false, dm_lwm2m_stop_client());

    ret = dm_lwm2m_stop_client();
    TC_ASSERT_EQ("dm_lwm2m_stop_client" , ret , DM_ERROR_NONE);

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
    int8_t state = -1;
    int ret = dm_lwm2m_start_client(&test_data_itc);
    TC_ASSERT_EQ("dm_lwm2m_start_client" , ret , DM_ERROR_NONE);

    ret = dm_lwm2m_get_client_state(&state);
    TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_client_state", ret, DM_ERROR_NONE, dm_lwm2m_stop_client());
    TC_ASSERT_EQ_CLEANUP("dm_lwm2m_get_client_state", (state < 0), false, dm_lwm2m_stop_client());

    ret = dm_lwm2m_stop_client();
    TC_ASSERT_EQ("dm_lwm2m_stop_client" , ret , DM_ERROR_NONE);

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
    TC_ASSERT_EQ("dm_lwm2m_start_client" , ret , DM_ERROR_NONE);
    work_queue(HPWORK, &itc_dm_wq, display_resource, NULL, MSEC2TICK(6000));
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
    sleep(1);
#ifdef CONFIG_ITC_DM_GET_CLIENT_LIFETIME
    itc_dm_lwm2m_get_client_lifetime_p();
    sleep(1);
#endif
#ifdef CONFIG_ITC_DM_GET_SERVER_ADDR
    itc_dm_lwm2m_get_server_address_p();
    sleep(1);
#endif
#ifdef CONFIG_ITC_DM_GET_SERVER_PORT
    itc_dm_lwm2m_get_server_port_p();
    sleep(1);
#endif
#ifdef CONFIG_ITC_DM_GET_CLIENT_STATE
    itc_dm_lwm2m_get_client_state_p();
    sleep(1);
#endif
#ifdef CONFIG_ITC_DM_DISPLAY_CLIENT_RESOURCE
    itc_dm_lwm2m_display_client_resource_p();
    sleep(1);
#endif
#endif
#endif
    return 0;
}
