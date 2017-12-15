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
#include <dm/dm_error.h>

#define BUF_SIZE 20

/**
* @testcase         itc_dm_conn_get_address_p
* @brief            Get IP address
* @scenario         Get IP address successfully
* @apicovered       dm_conn_get_address
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_address_p(void)
{
	int ret;
	char ip_addr[BUF_SIZE];
	ret = dm_conn_get_address(ip_addr);
	TC_ASSERT_EQ("dm_conn_get_address", ret, DM_ERROR_NONE);
	//client adress is allocated at run time, so checking only '.' in ip_addr
	TC_ASSERT_EQ("dm_conn_get_address", ip_addr[3], '.');
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_address_n
* @brief            Get IP address with invalid parameter
* @scenario         Get IP address with invalid parameter
* @apicovered       dm_conn_get_address
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_address_n(void)
{
	int ret = dm_conn_get_address(NULL);
	TC_ASSERT_EQ("dm_conn_get_address", ret, DM_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_channel_p
* @brief            Get WiFi channel
* @scenario         Get WiFi channel successfully
* @apicovered       dm_conn_get_channel
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_channel_p(void)
{
	int ret;
	int val = -1;
	ret = dm_conn_get_channel(&val);
	TC_ASSERT_EQ("dm_conn_get_channel", ret, DM_ERROR_NONE);
	TC_ASSERT_GEQ("dm_conn_get_channel", val, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_channel_n
* @brief            Get WiFi channel
* @scenario         Get WiFi channel with invalid parameter
* @apicovered       dm_conn_get_channel
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_channel_n(void)
{
	int ret = dm_conn_get_channel(NULL);
	TC_ASSERT_EQ("dm_conn_get_channel", ret, DM_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_interface_p
* @brief            Get interface name
* @scenario         Get interface name successfully
* @apicovered       dm_conn_get_interface
* @precondition     NA
* @postcondition    NA
*/
static void itc_dm_conn_get_interface_p(void)
{
	int ret;
	char interface[BUF_SIZE] = { 0, };
	ret = dm_conn_get_interface(interface);
	TC_ASSERT_EQ("dm_conn_get_interface", ret, DM_ERROR_NONE);
	TC_ASSERT_NEQ("dm_conn_get_interface", interface, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_interface_n
* @brief            Get interface name
* @scenario         Get interface name with invalid parameter
* @apicovered       dm_conn_get_interface
* @precondition     NA
* @postcondition    NA
*/
static void itc_dm_conn_get_interface_n(void)
{
	int ret = dm_conn_get_interface(NULL);
	TC_ASSERT_EQ("dm_conn_get_interface", ret, DM_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_rssi_p
* @brief            Get WiFi rssi
* @scenario         Get WiFi rssi negative value successfully
* @apicovered       dm_conn_get_rssi
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_rssi_p(void)
{
	int ret;
	int val = 0;
	ret = dm_conn_get_rssi(&val);
	TC_ASSERT_EQ("dm_conn_get_rssi", ret, DM_ERROR_NONE);
	TC_ASSERT_LT("dm_conn_get_rssi", val, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_rssi_n
* @brief            Get WiFi rssi
* @scenario         Get WiFi rssi with value invalid parameter
* @apicovered       dm_conn_get_rssi
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_rssi_n(void)
{
	int ret = dm_conn_get_rssi(NULL);
	TC_ASSERT_EQ("dm_conn_get_rssi", ret, DM_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_set_get_tx_power_p
* @brief            Get and set wifi tx power
* @scenario         Set wifi tx power and get the same tx power value successfully
* @apicovered       dm_conn_get_tx_power, dm_conn_set_tx_power
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_set_get_tx_power_p(void)
{
	int ret;
	int set_val = 20;
	int get_val;
	int default_val;
	// To get default value
	dm_conn_get_tx_power(&default_val);
	ret = dm_conn_set_tx_power(&set_val);
	TC_ASSERT_EQ("dm_conn_set_tx_power", ret, DM_ERROR_NONE);
	ret = dm_conn_get_tx_power(&get_val);
	TC_ASSERT_EQ("dm_conn_get_tx_power", ret, DM_ERROR_NONE);
	TC_ASSERT_EQ_CLEANUP("itc_dm_conn_set_get_power", set_val, get_val, dm_conn_set_tx_power(&default_val));
	ret = dm_conn_set_tx_power(&default_val);
	TC_ASSERT_EQ("dm_conn_set_tx_power", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_adress_channel_p_after_set_tx_power
* @brief            Set wifi tx power and get address and channel value
* @scenario         Set wifi tx power and get address and channel value
* @apicovered       dm_conn_set_tx_power, dm_conn_get_tx_power, dm_conn_get_address, dm_conn_get_channel
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_adress_channel_p_after_set_tx_power(void)
{
	int ret;
	int set_val = 20;
	int get_val;
	int default_val;
	int val = -1;
	char ip_addr[BUF_SIZE];
	// To get default value
	dm_conn_get_tx_power(&default_val);
	ret = dm_conn_set_tx_power(&set_val);
	TC_ASSERT_EQ("dm_conn_set_tx_power", ret, DM_ERROR_NONE);

	ret = dm_conn_get_tx_power(&get_val);
	TC_ASSERT_EQ("dm_conn_get_tx_power", ret, DM_ERROR_NONE);
	TC_ASSERT_EQ("itc_dm_conn_set_get_power", set_val, get_val);

	ret = dm_conn_get_address(ip_addr);
	TC_ASSERT_EQ("dm_conn_get_address", ret, DM_ERROR_NONE);
	TC_ASSERT_EQ("dm_conn_get_address", ip_addr[3], '.');

	ret = dm_conn_get_channel(&val);
	TC_ASSERT_EQ("dm_conn_get_channel", ret, DM_ERROR_NONE);
	TC_ASSERT_GEQ("dm_conn_get_channel", val, 0);

	ret = dm_conn_set_tx_power(&default_val);
	TC_ASSERT_EQ("dm_conn_set_tx_power", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_interface_rssi_p_after_set_tx_power
* @brief            Set wifi tx power and get interface and rssi value
* @scenario         Set wifi tx power and get interface and rssi value
* @apicovered       dm_conn_set_tx_power, dm_conn_get_tx_power, dm_conn_get_interface, dm_conn_get_rssi
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_interface_rssi_p_after_set_tx_power(void)
{
	int ret;
	int set_val = 20;
	int get_val;
	int default_val;
	int val = -1;
	char interface[BUF_SIZE] = { 0, };
	// To get default value
	dm_conn_get_tx_power(&default_val);
	ret = dm_conn_set_tx_power(&set_val);
	TC_ASSERT_EQ("dm_conn_set_tx_power", ret, DM_ERROR_NONE);

	ret = dm_conn_get_tx_power(&get_val);
	TC_ASSERT_EQ("dm_conn_get_tx_power", ret, DM_ERROR_NONE);
	TC_ASSERT_EQ("itc_dm_conn_set_get_power", set_val, get_val);

	ret = dm_conn_get_interface(interface);
	TC_ASSERT_EQ("dm_conn_get_interface", ret, DM_ERROR_NONE);
	TC_ASSERT_NEQ("dm_conn_get_interface", interface, NULL);

	ret = dm_conn_get_rssi(&val);
	TC_ASSERT_EQ("dm_conn_get_rssi", ret, DM_ERROR_NONE);
	TC_ASSERT_LT("dm_conn_get_rssi", val, 0);

	ret = dm_conn_set_tx_power(&default_val);
	TC_ASSERT_EQ("dm_conn_set_tx_power", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_set_tx_power_n
* @brief            Set wifi tx power with invalid parameter
* @scenario         Set wifi tx power with invalid parameter
* @apicovered       dm_conn_set_tx_power
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_set_tx_power_n(void)
{
	int ret = dm_conn_set_tx_power(NULL);
	TC_ASSERT_EQ("dm_conn_set_tx_power", ret, DM_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_tx_power_n
* @brief            Get wifi tx power with invalid parameter
* @scenario         Get wifi tx power with invalid parameter
* @apicovered       dm_conn_get_tx_power
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_tx_power_n(void)
{
	int ret = dm_conn_get_tx_power(NULL);
	TC_ASSERT_EQ("dm_conn_get_tx_power", ret, DM_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         link_event
* @brief            callback function
* @scenario         callback function called successfully from itc_dm_conn_register_unregister_linkup_p and itc_dm_conn_register_unregister_linkdown_p
* @apicovered       NA
* @precondition     NA
* @postcondition    NA
*/
static void link_event(void)
{
	printf("Set link event\n");
}

/**
* @testcase         itc_dm_conn_register_unregister_linkup_p
* @brief            Set and unset link up callback function
* @scenario         Set link up callback function successfully and then unset link up callback function successfully
* @apicovered       dm_conn_register_linkup_cb, dm_conn_unregister_linkup_cb
* @precondition     NA
* @postcondition    NA
*/
static void itc_dm_conn_register_unregister_linkup_p(void)
{
	int ret;
	ret = dm_conn_register_linkup_cb(link_event);
	TC_ASSERT_EQ("dm_conn_register_linkup_cb", ret, DM_ERROR_NONE);
	ret = dm_conn_unregister_linkup_cb(link_event);
	TC_ASSERT_EQ("dm_conn_unregister_linkup_cb", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_re_register_linkup_p
* @brief            Re-register link up callback function
* @scenario         Set link up callback function successfully and then re-register link up callback function
* @apicovered       dm_conn_register_linkup_cb
* @precondition     NA
* @postcondition    NA
*/
static void itc_dm_conn_re_register_linkup_p(void)
{
	int ret;
	ret = dm_conn_register_linkup_cb(link_event);
	TC_ASSERT_EQ("dm_conn_register_linkup_cb", ret, DM_ERROR_NONE);
	// re-register linkup callback function
	ret = dm_conn_register_linkup_cb(link_event);
	TC_ASSERT_EQ_CLEANUP("dm_conn_register_linkup_cb", ret, DM_ERROR_NONE, dm_conn_unregister_linkup_cb(link_event));

	ret = dm_conn_unregister_linkup_cb(link_event);
	TC_ASSERT_EQ("dm_conn_unregister_linkup_cb", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_address_channel_p_after_register_linkup
* @brief            Register linkup callback and get address and channel value
* @scenario         Register linkup callback and get address and channel value
* @apicovered       dm_conn_get_address, dm_conn_get_channel, dm_conn_register_linkup_cb, dm_conn_unregister_linkup_cb
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_address_channel_p_after_register_linkup(void)
{
	int ret;
	int val = -1;
	char ip_addr[BUF_SIZE];

	ret = dm_conn_register_linkup_cb(link_event);
	TC_ASSERT_EQ("dm_conn_register_linkup_cb", ret, DM_ERROR_NONE);

	ret = dm_conn_get_address(ip_addr);
	TC_ASSERT_EQ_CLEANUP("dm_conn_get_address", ret, DM_ERROR_NONE, dm_conn_unregister_linkup_cb(link_event));
	TC_ASSERT_EQ_CLEANUP("dm_conn_get_address", ip_addr[3], '.', dm_conn_unregister_linkup_cb(link_event));

	ret = dm_conn_get_channel(&val);
	TC_ASSERT_EQ_CLEANUP("dm_conn_get_channel", ret, DM_ERROR_NONE, dm_conn_unregister_linkup_cb(link_event));
	TC_ASSERT_GEQ_CLEANUP("dm_conn_get_channel", val, 0, dm_conn_unregister_linkup_cb(link_event));

	ret = dm_conn_unregister_linkup_cb(link_event);
	TC_ASSERT_EQ("dm_conn_unregister_linkup_cb", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_interface_rssi_p_after_register_linkup
* @brief            Register linkup callback and get address and channel value
* @scenario         Register linkup callback and get address and channel value
* @apicovered       dm_conn_get_address, dm_conn_get_channel, dm_conn_register_linkup_cb, dm_conn_unregister_linkup_cb
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_interface_rssi_p_after_register_linkup(void)
{
	int ret;
	char interface[BUF_SIZE] = { 0, };
	int val = 0;

	ret = dm_conn_register_linkup_cb(link_event);
	TC_ASSERT_EQ("dm_conn_register_linkup_cb", ret, DM_ERROR_NONE);

	ret = dm_conn_get_interface(interface);
	TC_ASSERT_EQ_CLEANUP("dm_conn_get_interface", ret, DM_ERROR_NONE, dm_conn_unregister_linkup_cb(link_event));
	TC_ASSERT_NEQ_CLEANUP("dm_conn_get_interface", interface, NULL, dm_conn_unregister_linkup_cb(link_event));

	ret = dm_conn_get_rssi(&val);
	TC_ASSERT_EQ_CLEANUP("dm_conn_get_rssi", ret, DM_ERROR_NONE, dm_conn_unregister_linkup_cb(link_event));
	TC_ASSERT_LT_CLEANUP("dm_conn_get_rssi", val, 0, dm_conn_unregister_linkup_cb(link_event));

	ret = dm_conn_unregister_linkup_cb(link_event);
	TC_ASSERT_EQ("dm_conn_unregister_linkup_cb", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_setget_tx_power_p_after_register_linkup
* @brief            Set and unset link up callback function with set and get tx power
* @scenario         Set link up callback and then set and get tx power and then unset link up callback
* @apicovered       dm_conn_register_linkup_cb, dm_conn_unregister_linkup_cb, dm_conn_set_tx_power, dm_conn_get_tx_power
* @precondition     NA
* @postcondition    NA
*/
static void itc_dm_conn_setget_tx_power_p_after_register_linkup(void)
{
	int ret;
	int set_val = 20;
	int get_val;

	ret = dm_conn_register_linkup_cb(link_event);
	TC_ASSERT_EQ("dm_conn_register_linkup_cb", ret, DM_ERROR_NONE);

	ret = dm_conn_set_tx_power(&set_val);
	TC_ASSERT_EQ_CLEANUP("dm_conn_set_tx_power", ret, DM_ERROR_NONE, dm_conn_unregister_linkup_cb(link_event));
	ret = dm_conn_get_tx_power(&get_val);
	TC_ASSERT_EQ_CLEANUP("dm_conn_get_tx_power", ret, DM_ERROR_NONE, dm_conn_unregister_linkup_cb(link_event));

	ret = dm_conn_unregister_linkup_cb(link_event);
	TC_ASSERT_EQ("dm_conn_unregister_linkup_cb", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_register_linkup_n
* @brief            Set link up callback function with invalid parameter
* @scenario         Set link up callback function with invalid parameter
* @apicovered       dm_conn_register_linkup_cb
* @precondition     NA
* @postcondition    NA
*/
static void itc_dm_conn_register_linkup_n(void)
{
	int ret = dm_conn_register_linkup_cb(NULL);
	TC_ASSERT_EQ("dm_conn_register_linkup_cb", ret, DM_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_unregister_linkup_n
* @brief            Unset link up callback function with invalid parameter
* @scenario         Unset link up callback function with invalid parameter
* @apicovered       dm_conn_unregister_linkup_cb
* @precondition     NA
* @postcondition    NA
*/
static void itc_dm_conn_unregister_linkup_n(void)
{
	int ret = dm_conn_unregister_linkup_cb(NULL);
	TC_ASSERT_EQ("dm_conn_unregister_linkup_cb", ret, DM_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_register_unregister_linkdown_p
* @brief            Set and unset link down callback function
* @scenario         Set link down callback function successfully and then unset link down callback function successfully
* @apicovered       dm_conn_register_linkdown_cb, dm_conn_unregister_linkdown_cb
* @precondition     NA
* @postcondition    NA
*/
static void itc_dm_conn_register_unregister_linkdown_p(void)
{
	int ret;
	ret = dm_conn_register_linkdown_cb(link_event);
	TC_ASSERT_EQ("dm_conn_register_linkdown_cb", ret, DM_ERROR_NONE);
	ret = dm_conn_unregister_linkdown_cb(link_event);
	TC_ASSERT_EQ("dm_conn_unregister_linkup_cb", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_register_linkdown_p_reregister
* @brief            Re-register link down callback function
* @scenario         Set link down callback function successfully and then re-register link down callback function
* @apicovered       dm_conn_register_linkup_cb
* @precondition     NA
* @postcondition    NA
*/
static void itc_dm_conn_register_linkdown_p_reregister(void)
{
	int ret;
	ret = dm_conn_register_linkdown_cb(link_event);
	TC_ASSERT_EQ("dm_conn_register_linkdown_cb", ret, DM_ERROR_NONE);
	// re-register linkdown callback function
	ret = dm_conn_register_linkdown_cb(link_event);
	TC_ASSERT_EQ_CLEANUP("dm_conn_register_linkdown_cb", ret, DM_ERROR_NONE, dm_conn_unregister_linkdown_cb(link_event));

	ret = dm_conn_unregister_linkdown_cb(link_event);
	TC_ASSERT_EQ("dm_conn_unregister_linkup_cb", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_address_channel_p_after_register_linkdown
* @brief            Register linkdown callback and get address and channel value
* @scenario         Register linkdown callback and get address and channel value and unregister callback
* @apicovered       dm_conn_get_address, dm_conn_get_channel, dm_conn_register_linkdown_cb, dm_conn_unregister_linkdown_cb
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_address_channel_p_after_register_linkdown(void)
{
	int ret;
	int val = -1;
	char ip_addr[BUF_SIZE];

	ret = dm_conn_register_linkdown_cb(link_event);
	TC_ASSERT_EQ("dm_conn_register_linkdown_cb", ret, DM_ERROR_NONE);

	ret = dm_conn_get_address(ip_addr);
	TC_ASSERT_EQ_CLEANUP("dm_conn_get_address", ret, DM_ERROR_NONE, dm_conn_unregister_linkdown_cb(link_event));
	TC_ASSERT_EQ_CLEANUP("dm_conn_get_address", ip_addr[3], '.', dm_conn_unregister_linkdown_cb(link_event));

	ret = dm_conn_get_channel(&val);
	TC_ASSERT_EQ_CLEANUP("dm_conn_get_channel", ret, DM_ERROR_NONE, dm_conn_unregister_linkdown_cb(link_event));
	TC_ASSERT_GEQ_CLEANUP("dm_conn_get_channel", val, 0, dm_conn_unregister_linkdown_cb(link_event));

	ret = dm_conn_unregister_linkdown_cb(link_event);
	TC_ASSERT_EQ("dm_conn_unregister_linkdown_cb", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_interface_rssi_p_after_register_linkdown
* @brief            Register linkdown callback and get interface and rssi value
* @scenario         Register linkdown callback and get interface and rssi value and unregister callback
* @apicovered       dm_conn_get_interface, dm_conn_get_rssi, dm_conn_register_linkdown_cb, dm_conn_unregister_linkdown_cb
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_interface_rssi_p_after_register_linkdown(void)
{
	int ret;
	char interface[BUF_SIZE] = { 0, };
	int val = 0;

	ret = dm_conn_register_linkdown_cb(link_event);
	TC_ASSERT_EQ("dm_conn_register_linkdown_cb", ret, DM_ERROR_NONE);

	ret = dm_conn_get_interface(interface);
	TC_ASSERT_EQ_CLEANUP("dm_conn_get_interface", ret, DM_ERROR_NONE, dm_conn_unregister_linkdown_cb(link_event));
	TC_ASSERT_NEQ_CLEANUP("dm_conn_get_interface", interface, NULL, dm_conn_unregister_linkdown_cb(link_event));

	ret = dm_conn_get_rssi(&val);
	TC_ASSERT_EQ_CLEANUP("dm_conn_get_rssi", ret, DM_ERROR_NONE, dm_conn_unregister_linkdown_cb(link_event));
	TC_ASSERT_LT_CLEANUP("dm_conn_get_rssi", val, 0, dm_conn_unregister_linkdown_cb(link_event));

	ret = dm_conn_unregister_linkdown_cb(link_event);
	TC_ASSERT_EQ("dm_conn_unregister_linkdown_cb", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_setget_tx_power_p_after_register_linkdown
* @brief            Set and unset link up callback function with set and get tx power
* @scenario         Set link up callback and then set and get tx power and then unset link up callback
* @apicovered       dm_conn_register_linkdown_cb, dm_conn_unregister_linkdown_cb, dm_conn_set_tx_power, dm_conn_get_tx_power
* @precondition     NA
* @postcondition    NA
*/
static void itc_dm_conn_setget_tx_power_p_after_register_linkdown(void)
{
	int ret;
	int set_val = 20;
	int get_val;

	ret = dm_conn_register_linkdown_cb(link_event);
	TC_ASSERT_EQ("dm_conn_register_linkdown_cb", ret, DM_ERROR_NONE);

	ret = dm_conn_set_tx_power(&set_val);
	TC_ASSERT_EQ_CLEANUP("dm_conn_set_tx_power", ret, DM_ERROR_NONE, dm_conn_unregister_linkdown_cb(link_event));
	ret = dm_conn_get_tx_power(&get_val);
	TC_ASSERT_EQ_CLEANUP("dm_conn_get_tx_power", ret, DM_ERROR_NONE, dm_conn_unregister_linkdown_cb(link_event));

	ret = dm_conn_unregister_linkdown_cb(link_event);
	TC_ASSERT_EQ("dm_conn_unregister_linkdown_cb", ret, DM_ERROR_NONE);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_register_linkdown_n
* @brief            Set link down callback function with invalid parameter
* @scenario         Set link down callback function with invalid parameter
* @apicovered       dm_conn_register_linkdown_cb
* @precondition     NA
* @postcondition    NA
*/
static void itc_dm_conn_register_linkdown_n(void)
{
	int ret = dm_conn_register_linkdown_cb(NULL);
	TC_ASSERT_EQ("dm_conn_register_linkdown_cb", ret, DM_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_unregister_linkdown_n
* @brief            Unset link down callback function with invalid parameter
* @scenario         Unset link down callback function with invalid parameter
* @apicovered       dm_conn_unregister_linkdown_cb
* @precondition     NA
* @postcondition    NA
*/
static void itc_dm_conn_unregister_linkdown_n(void)
{
	int ret = dm_conn_unregister_linkdown_cb(NULL);
	TC_ASSERT_EQ("dm_conn_unregister_linkdown_cb", ret, DM_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: dm_conn_get_address
 ****************************************************************************/

int itc_dm_conn_get_address_main(void)
{
	itc_dm_conn_get_address_p();
	itc_dm_conn_get_address_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_get_channel
 ****************************************************************************/

int itc_dm_conn_get_channel_main(void)
{
	itc_dm_conn_get_channel_p();
	itc_dm_conn_get_channel_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_get_interface
 ****************************************************************************/

int itc_dm_conn_get_interface_main(void)
{
	itc_dm_conn_get_interface_p();
	itc_dm_conn_get_interface_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_get_rssi
 ****************************************************************************/

int itc_dm_conn_get_rssi_main(void)
{
	itc_dm_conn_get_rssi_p();
	itc_dm_conn_get_rssi_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_get_tx_power
 ****************************************************************************/

int itc_dm_conn_set_get_tx_power_main(void)
{
	itc_dm_conn_set_get_tx_power_p();
	itc_dm_conn_get_adress_channel_p_after_set_tx_power();
	itc_dm_conn_get_interface_rssi_p_after_set_tx_power();
	itc_dm_conn_set_tx_power_n();
	itc_dm_conn_get_tx_power_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_regi_linkup_cb
 ****************************************************************************/

int itc_dm_conn_regi_unreg_linkup_main(void)
{
	itc_dm_conn_register_unregister_linkup_p();
	itc_dm_conn_re_register_linkup_p();
	itc_dm_conn_get_address_channel_p_after_register_linkup();
	itc_dm_conn_get_interface_rssi_p_after_register_linkup();
	itc_dm_conn_setget_tx_power_p_after_register_linkup();
	itc_dm_conn_register_linkup_n();
	itc_dm_conn_unregister_linkup_n();
	return 0;
}

/****************************************************************************
 * Name: dm_conn_regi_linkdown_cb
 ****************************************************************************/

int itc_dm_conn_regi_unreg_linkdown_main(void)
{
	itc_dm_conn_register_unregister_linkdown_p();
	itc_dm_conn_register_linkdown_p_reregister();
	itc_dm_conn_get_address_channel_p_after_register_linkdown();
	itc_dm_conn_get_interface_rssi_p_after_register_linkdown();//TC FAIL when run along with all other DM TCs, individual TC is PASS, return value is 0xffffffc3 not equal to 0x0
	itc_dm_conn_setget_tx_power_p_after_register_linkdown();
	itc_dm_conn_register_linkdown_n();
	itc_dm_conn_unregister_linkdown_n();
	return 0;
}
