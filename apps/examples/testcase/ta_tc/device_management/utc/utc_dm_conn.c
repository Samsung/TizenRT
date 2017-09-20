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
#include <dm/dm_error.h>
#include <dm/dm_connectivity.h>

/**
* @testcase			utc_dm_conn_get_address_n
* @brief			Get IP address
* @scenario			Set null parameter to occur error
* @apicovered		dm_conn_get_address()
* @precondition		WiFi connected
* @postcondition
*/
static void utc_dm_conn_get_address_n(void)
{
	int ret;
	ret = dm_conn_get_address(NULL);

	TC_ASSERT_EQ("dm_conn_get_address", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_get_address_p
* @brief			Get IP address
* @scenario			Get IP address successfully
* @apicovered		dm_conn_get_address()
* @precondition		WiFi connected
* @postcondition
*/
static void utc_dm_conn_get_address_p(void)
{
	int ret;
	char val[20];
	ret = dm_conn_get_address(val);

	TC_ASSERT_EQ("dm_conn_get_address", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_get_channel_n
* @brief			Get WiFi channel
* @scenario			Set null parameter to occur error
* @apicovered		dm_conn_get_channel()
* @precondition		WiFi connected
* @postcondition
*/
static void utc_dm_conn_get_channel_n(void)
{
	int ret;
	ret = dm_conn_get_channel(NULL);

	TC_ASSERT_EQ("dm_conn_get_channel", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_get_channel_p
* @brief			Get WiFi channel
* @scenario			Get WiFi channel successfully
* @apicovered		dm_conn_get_channel()
* @precondition		WiFi connected
* @postcondition
*/
static void utc_dm_conn_get_channel_p(void)
{
	int ret;
	int val;
	ret = dm_conn_get_channel(&val);

	TC_ASSERT_EQ("dm_conn_get_channel", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_get_interface_n
* @brief			Get interface name
* @scenario			Set null parameter to occur error
* @apicovered		dm_conn_get_interface()
* @precondition
* @postcondition
*/
static void utc_dm_conn_get_interface_n(void)
{
	int ret;
	ret = dm_conn_get_interface(NULL);

	TC_ASSERT_EQ("dm_conn_get_interface", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_get_interface_p
* @brief			Get interface name
* @scenario			Get interface name successfully
* @apicovered		dm_conn_get_interface()
* @precondition
* @postcondition
*/
static void utc_dm_conn_get_interface_p(void)
{
	int ret;
	char val[10];
	ret = dm_conn_get_interface(val);

	TC_ASSERT_EQ("dm_conn_get_interface", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_get_rssi_n
* @brief			Get WiFi rssi
* @scenario			Set null parameter to occur error
* @apicovered		dm_conn_get_rssi()
* @precondition		WiFi connected
* @postcondition
*/
static void utc_dm_conn_get_rssi_n(void)
{
	int ret;
	ret = dm_conn_get_rssi(NULL);

	TC_ASSERT_EQ("dm_conn_get_rssi", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_get_rssi_p
* @brief			Get WiFi rssi
* @scenario			Get WiFi rssi successfully
* @apicovered		dm_conn_get_rssi()
* @precondition		WiFi connected
* @postcondition
*/
static void utc_dm_conn_get_rssi_p(void)
{
	int ret;
	int val;
	ret = dm_conn_get_rssi(&val);

	TC_ASSERT_EQ("dm_conn_get_rssi", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_get_tx_power_n
* @brief			Get wifi tx power
* @scenario			Set null parameter to occur error
* @apicovered		dm_conn_get_tx_power()
* @precondition		WiFi connected
* @postcondition
*/
static void utc_dm_conn_get_tx_power_n(void)
{
	int ret;
	ret = dm_conn_get_tx_power(NULL);

	TC_ASSERT_EQ("dm_conn_get_tx_power", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_get_tx_power_p
* @brief			Get wifi tx power
* @scenario			Get wifi tx power successfully
* @apicovered		dm_conn_get_tx_power()
* @precondition		WiFi connected
* @postcondition
*/
static void utc_dm_conn_get_tx_power_p(void)
{
	int ret;
	int val;
	ret = dm_conn_get_tx_power(&val);

	TC_ASSERT_EQ("dm_conn_get_tx_power", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_set_tx_power_n
* @brief			Set wifi tx power
* @scenario			Set null parameter to occur error
* @apicovered		dm_conn_set_tx_power()
* @precondition		WiFi connected
* @postcondition
*/
static void utc_dm_conn_set_tx_power_n(void)
{
	int ret;
	ret = dm_conn_set_tx_power(NULL);

	TC_ASSERT_EQ("dm_conn_set_tx_power", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_set_tx_power_p
* @brief			Set wifi tx power
* @scenario			Set wifi tx power successfully
* @apicovered		dm_conn_set_tx_power()
* @precondition		WiFi connected
* @postcondition
*/
static void utc_dm_conn_set_tx_power_p(void)
{
	int ret;
	int val = 20;
	/* Please make sure you provide a valid tx power value within
	 * the range of 13 to 29. Here, we provide a valid value of 20.
	 */
	ret = dm_conn_set_tx_power(&val);

	TC_ASSERT_EQ("dm_conn_set_tx_power", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

static void linkEvent(void)
{
	printf("Set link event\n");
}

/**
* @testcase			utc_dm_conn_regi_linkup_n
* @brief			Set link up callback function
* @scenario			Set null parameter to occur error
* @apicovered		dm_conn_register_linkup_cb()
* @precondition
* @postcondition
*/
static void utc_dm_conn_regi_linkup_n(void)
{
	int ret;
	ret = dm_conn_register_linkup_cb(NULL);

	TC_ASSERT_EQ("dm_conn_register_linkup_cb", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_regi_linkup_p
* @brief			Set link up callback function
* @scenario			Set link up callback function successfully
* @apicovered		dm_conn_register_linkup_cb()
* @precondition
* @postcondition
*/
static void utc_dm_conn_regi_linkup_p(void)
{
	int ret;
	ret = dm_conn_register_linkup_cb(linkEvent);

	TC_ASSERT_EQ("dm_conn_register_linkup_cb", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_regi_linkdown_n
* @brief			Set link down callback function
* @scenario			Set null parameter to occur error
* @apicovered		dm_conn_register_linkdown_cb()
* @precondition
* @postcondition
*/
static void utc_dm_conn_regi_linkdown_n(void)
{
	int ret;
	ret = dm_conn_register_linkdown_cb(NULL);

	TC_ASSERT_EQ("dm_conn_register_linkdown_cb", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_regi_linkdown_p
* @brief			Set link down callback function
* @scenario			Set link down callback function successfully
* @apicovered		dm_conn_register_linkdown_cb()
* @precondition
* @postcondition
*/
static void utc_dm_conn_regi_linkdown_p(void)
{
	int ret;
	ret = dm_conn_register_linkdown_cb(linkEvent);

	TC_ASSERT_EQ("dm_conn_register_linkdown_cb", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_unregi_linkup_n
* @brief			Unset link up callback function
* @scenario			Set null parameter to occur error
* @apicovered		dm_conn_unregister_linkup_cb()
* @precondition
* @postcondition
*/
static void utc_dm_conn_unregi_linkup_n(void)
{
	int ret;
	ret = dm_conn_unregister_linkup_cb(NULL);

	TC_ASSERT_EQ("dm_conn_unregister_linkup_cb", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_unregi_linkup_p
* @brief			Unset link up callback function
* @scenario			Unset link up callback function successfully
* @apicovered		dm_conn_unregister_linkup_cb()
* @precondition		The callback function is set up
* @postcondition
*/
static void utc_dm_conn_unregi_linkup_p(void)
{
	int ret;
	ret = dm_conn_unregister_linkup_cb(linkEvent);

	TC_ASSERT_EQ("dm_conn_unregister_linkup_cb", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_unregi_linkdown_n
* @brief			Unset link down callback function
* @scenario			Set null parameter to occur error
* @apicovered		dm_conn_unregister_linkdown_cb()
* @precondition
* @postcondition
*/
static void utc_dm_conn_unregi_linkdown_n(void)
{
	int ret;
	ret = dm_conn_unregister_linkdown_cb(NULL);

	TC_ASSERT_EQ("dm_conn_unregister_linkdown_cb", ret, DM_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase			utc_dm_conn_unregi_linkdown_p
* @brief			Unset link down callback function
* @scenario			Unset link down callback function successfully
* @apicovered		dm_conn_unregister_linkdown_cb()
* @precondition		The callback function is set up
* @postcondition
*/
static void utc_dm_conn_unregi_linkdown_p(void)
{
	int ret;
	ret = dm_conn_unregister_linkdown_cb(linkEvent);

	TC_ASSERT_EQ("dm_conn_unregister_linkdown_cb", ret, DM_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: dm_conn_get_address
 ****************************************************************************/

int dm_conn_get_address_main(void)
{
	utc_dm_conn_get_address_p();
	utc_dm_conn_get_address_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_get_channel
 ****************************************************************************/

int dm_conn_get_channel_main(void)
{
	utc_dm_conn_get_channel_p();
	utc_dm_conn_get_channel_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_get_interface
 ****************************************************************************/

int dm_conn_get_interface_main(void)
{
	utc_dm_conn_get_interface_p();
	utc_dm_conn_get_interface_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_get_rssi
 ****************************************************************************/

int dm_conn_get_rssi_main(void)
{
	utc_dm_conn_get_rssi_p();
	utc_dm_conn_get_rssi_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_get_tx_power
 ****************************************************************************/

int dm_conn_get_tx_power_main(void)
{
	utc_dm_conn_get_tx_power_p();
	utc_dm_conn_get_tx_power_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_set_tx_power
 ****************************************************************************/

int dm_conn_set_tx_power_main(void)
{
	utc_dm_conn_set_tx_power_p();
	utc_dm_conn_set_tx_power_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_regi_linkup_cb
 ****************************************************************************/

int dm_conn_regi_linkup_main(void)
{
	utc_dm_conn_regi_linkup_p();
	utc_dm_conn_regi_linkup_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_regi_linkdown_cb
 ****************************************************************************/

int dm_conn_regi_linkdown_main(void)
{
	utc_dm_conn_regi_linkdown_p();
	utc_dm_conn_regi_linkdown_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_unregi_linkup_cb
 ****************************************************************************/

int dm_conn_unregi_linkup_main(void)
{
	dm_conn_register_linkup_cb(linkEvent);
	utc_dm_conn_unregi_linkup_p();
	utc_dm_conn_unregi_linkup_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_unregi_linkdown_cb
 ****************************************************************************/

int dm_conn_unregi_linkdown_main(void)
{
	dm_conn_register_linkdown_cb(linkEvent);
	utc_dm_conn_unregi_linkdown_p();
	utc_dm_conn_unregi_linkdown_n();
	return 0;
}
