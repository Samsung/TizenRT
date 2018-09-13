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

/**
 * @file dm_connectivity.c
 * @brief DM Connectivity APIs
 */
#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <debug.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <dm/dm_error.h>
#include <dm/dm_connectivity.h>

void dm_cb_register_init(void)
{
	return;
}

int dm_conn_get_rssi(int *rssi)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_get_channel(int *channel)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_get_tx_power(int *dbm)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_set_tx_power(const int *dbm)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_get_address(char *ipAddr)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_get_interface(char *interface)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_register_linkup_cb(conn_cb cb)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_register_linkdown_cb(conn_cb cb)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_unregister_linkup_cb(conn_cb cb)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_unregister_linkdown_cb(conn_cb cb)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_wifi_scan(void)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_get_scan_result(dm_scan_info_t **result)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_free_scan_result(dm_scan_info_t **result)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_wifi_connect(conn_cb linkUpEvent, conn_cb linkDownEvent)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_dhcp_init(void)
{
	return DM_ERROR_NOT_SUPPORTED;
}

int dm_conn_wifi_disconnect(void)
{
	return DM_ERROR_NOT_SUPPORTED;
}
