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

#include "itc_internal.h"
#include <dm/dm_error.h>
#include <dm/dm_connectivity.h>

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
    char ipAddr[BUF_SIZE];
    ret = dm_conn_get_address(ipAddr);
    TC_ASSERT_EQ("dm_conn_get_address" , ret , DM_ERROR_NONE);
    printf("itc_dm_conn_get_address_p : %s \n", ipAddr);
    TC_ASSERT_EQ("dm_conn_get_address" , ipAddr[3] , '.');
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
    TC_ASSERT_EQ("dm_conn_get_channel" , ret , DM_ERROR_NONE);
    TC_ASSERT_GEQ("dm_conn_get_channel" , val , 0 );
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
    char interface[BUF_SIZE];
    ret = dm_conn_get_interface(interface);
    TC_ASSERT_EQ("dm_conn_get_interface" , ret , DM_ERROR_NONE);
    TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_get_rssi_p
* @brief            Get WiFi rssi
* @scenario         Get WiFi rssi successfully
* @apicovered       dm_conn_get_rssi
* @precondition     WiFi connected
* @postcondition    NA
*/
static void itc_dm_conn_get_rssi_p(void)
{
    int ret;
    int val = -1;
    ret = dm_conn_get_rssi(&val);
    TC_ASSERT_EQ("dm_conn_get_rssi" , ret , DM_ERROR_NONE);
    TC_ASSERT_LEQ("dm_conn_get_rssi" , val , 0 );
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
    int setVal = 1, getVal, defaultvalue;
    // To get default value    dm_conn_get_tx_power(&defaultvalue);
    ret = dm_conn_set_tx_power(&setVal);
    TC_ASSERT_EQ("dm_conn_set_tx_power" , ret , DM_ERROR_NONE);
    ret = dm_conn_get_tx_power(&getVal);
    TC_ASSERT_EQ("dm_conn_get_tx_power" , ret, DM_ERROR_NONE);
    TC_ASSERT_EQ_CLEANUP("itc_dm_conn_set_get_power" , setVal , getVal, dm_conn_set_tx_power(&defaultvalue));
    dm_conn_set_tx_power(&defaultvalue);
    TC_SUCCESS_RESULT();
}

/**
* @testcase         linkEvent
* @brief            callback function
* @scenario         callback function called successfully from itc_dm_conn_register_unregister_linkup_p and itc_dm_conn_register_unregister_linkdown_p
* @apicovered       NA
* @precondition     NA
* @postcondition    NA
*/
static conn_cb linkEvent()
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
    ret = dm_conn_register_linkup_cb(linkEvent);
    TC_ASSERT_EQ("dm_conn_register_linkup_cb" , ret, DM_ERROR_NONE);
    ret = dm_conn_unregister_linkup_cb(linkEvent);
    TC_ASSERT_EQ("dm_conn_unregister_linkup_cb" , ret, DM_ERROR_NONE);
    TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_dm_conn_register_unregister_linkdown_p
* @brief            Set and unset link down callback function
* @scenario         Set link down callback function successfully and then unset link down callback function successfully
* @apicovered       dm_conn_register_linkup_cb, dm_conn_unregister_linkup_cb
* @precondition     NA
* @postcondition    NA
*/
static void itc_dm_conn_register_unregister_linkdown_p(void)
{
    int ret;
    ret = dm_conn_register_linkdown_cb(linkEvent);
    TC_ASSERT_EQ("dm_conn_register_linkdown_cb" , ret, DM_ERROR_NONE);
    ret = dm_conn_unregister_linkup_cb(linkEvent);
    TC_ASSERT_EQ("dm_conn_unregister_linkup_cb" , ret, DM_ERROR_NONE);
    TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: dm_conn_get_address
 ****************************************************************************/

int itc_dm_conn_get_address_main(void)
{
    itc_dm_conn_get_address_p();
    return 0;
}

/****************************************************************************
 * Name: dm_conn_get_channel
 ****************************************************************************/

int itc_dm_conn_get_channel_main(void)
{
    itc_dm_conn_get_channel_p();
    return 0;
}

/****************************************************************************
 * Name: dm_conn_get_interface
 ****************************************************************************/

int itc_dm_conn_get_interface_main(void)
{
    itc_dm_conn_get_interface_p();
    return 0;
}

/****************************************************************************
 * Name: dm_conn_get_rssi
 ****************************************************************************/

int itc_dm_conn_get_rssi_main(void)
{
    itc_dm_conn_get_rssi_p();
    return 0;
}

/****************************************************************************
 * Name: dm_conn_get_tx_power
 ****************************************************************************/

int itc_dm_conn_set_get_tx_power_main(void)
{
    itc_dm_conn_set_get_tx_power_p();
    return 0;
}

/****************************************************************************
 * Name: dm_conn_regi_linkup_cb
 ****************************************************************************/

int itc_dm_conn_regi_unreg_linkup_main(void)
{
    itc_dm_conn_register_unregister_linkup_p();
    return 0;
}

/****************************************************************************
 * Name: dm_conn_regi_linkdown_cb
 ****************************************************************************/

int itc_dm_conn_regi_unreg_linkdown_main(void)
{
    itc_dm_conn_register_unregister_linkdown_p();
    return 0;
}
