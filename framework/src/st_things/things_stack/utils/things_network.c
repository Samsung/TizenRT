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

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "logging/things_logger.h"
#include "utils/things_malloc.h"
#include "utils/things_string.h"
#include "things_api.h"
#include "things_network.h"

#include "easy-setup/resource_handler.h"
#include "cloud/cloud_manager.h"
#include "easy-setup/easysetup_manager.h"
#include "things_ping.h"

#define TAG "[things_network]"

static things_set_ap_connection_func_type gSetAPConnection = NULL;

static int app_state = -1;
static char *app_ap_name = NULL;
static char *app_ip_addr = NULL;
volatile static bool is_connected_target_ap = false;
extern bool b_reset_continue_flag;

#define IP_NULL_VAL     "0.0.0.0"

/*******************************************************************
 *
 * state : 0 --> disconnect
 *           1 --> connected
 *
 *******************************************************************/
static int check_cb_skip(int state, char *ap_name, char *ip_addr)
{
	int ret = 0;

	if (state != 0 && state != 1) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "invalid state value.(%d)", state);
		return 0;
	}

	if (app_state == -1) {
		app_state = 0;
		app_ap_name = (char *)things_malloc(sizeof(char) * 1);
		app_ip_addr = (char *)things_malloc(sizeof(char) * 1);
	}
	// Process skip checking.
	if ((state == 1 && ((ap_name == NULL || strlen(ap_name) == 0) || (ip_addr == NULL || strlen(ip_addr) == 0 || strncmp(ip_addr, IP_NULL_VAL, strlen(IP_NULL_VAL)) == 0))) || (state != 1 && state != 0)) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "invalid value.(state=%d, AP_SSID=%s, IP=%s)", state, ap_name, ip_addr);
		return 0;
	}

	if (app_state != state) {
		ret = 1;
	} else if (state == 1 && (strncmp(ap_name, app_ap_name, strlen(app_ap_name)) != 0 || strncmp(ip_addr, app_ip_addr, strlen(app_ip_addr)) != 0)) {
		ret = 1;
	}

	if (ret == 1) {
		// Update contents
		app_state = state;
		if (ap_name) {
			things_free(app_ap_name);
			app_ap_name = things_strdup(ap_name);
		}

		if (ip_addr) {
			things_free(app_ip_addr);
			app_ip_addr = things_strdup(ip_addr);
		}
	}

	return ret;
}

void things_wifi_state_changed_cb_init(void)	// when Soft-AP on, Not called Disconnected Call-Back Func. So, it's alternate.
{
	if (app_state == -1) {
		app_state = 0;
		app_ap_name = (char *)things_malloc(sizeof(char) * 1);
		app_ip_addr = (char *)things_malloc(sizeof(char) * 1);
	} else {
		app_ap_name[0] = 0;
		app_ip_addr[0] = 0;
	}

	is_connected_target_ap = false;
	ci_retry_stop_by_wifi_cb(false);
}

/*****************************************************************************
 *
 * It's called from st_things Application when WiFi state changed.
 *
 * state : 1 -> Connected to WiFi AP.
 *         0 -> Disconnected from WiFi AP.
 * ap_name : AP SSID.
 * ip_addr : IP address.
 *
 *****************************************************************************/
int things_wifi_state_changed_cb(int state, char *ap_name, char *ip_addr)
{
	bool Retry1Stop0 = false;

	THINGS_LOG_D(THINGS_DEBUG, TAG, "state=%d, ap_name=%s, ip_name=%s", state, ap_name, ip_addr);

	if (things_is_net_initialize() == 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "gGetSoftAPState is NULL.");
		return 0;
	}

	if (check_cb_skip(state, ap_name, ip_addr) != 1) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Call-back process skiped.");
		return 0;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "app_state=%d, app_ap_name=%s, app_ip_addr=%s", app_state, app_ap_name, app_ip_addr);

	// Process.
	if (state == 1) {			// [Info] Always, when Soft-AP enabling, Call-Back State maintain disconnected. So, Soft-AP Check is not need.
		THINGS_LOG_D(THINGS_DEBUG, TAG, "AP CONNECTED.");
		is_connected_target_ap = true;

		set_ssid_in_wifi_resource(app_ap_name);
		Retry1Stop0 = true;		// Re-Try with Last failed Access info.
		esm_wifi_prov_check_cb(state, app_ap_name, app_ip_addr);
	} else {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "AP DISCONNECTED.");
		is_connected_target_ap = false;
		set_ssid_in_wifi_resource(NULL);
	}

	// Call Retry or forceStop Connection
	ci_retry_stop_by_wifi_cb(Retry1Stop0);

	return 1;
}

/*****************************************************************************
 *
 * It's called from Iotivity Framework when Adaptor state changed.
 *
 * adapter : Adapter Type.
 * enabled  : true -> connected.
 *            false-> disconnected.
 *
 *****************************************************************************/
void things_adapter_state_cb(CATransportAdapter_t adapter, bool enabled)
{
	(void)adapter;

	THINGS_LOG_D(THINGS_DEBUG, TAG, "IN");

	if (enabled) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "CONNECTED but Not Guarantee to be Communication-Available");
	} else {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "AP DISCONNECTED.");
		is_connected_target_ap = false;
		set_ssid_in_wifi_resource(NULL);
		ci_retry_stop_by_wifi_cb(false);
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "OUT");
}

/*****************************************************************************
 *
 * It's called from Iotivity Framework when TCP session state changed.
 *
 * info : information about remote-peer.
 * connected  : true -> connected
 *              false-> disconnected
 *
 *****************************************************************************/
void things_tcp_session_state_cb(const CAEndpoint_t *info, bool connected)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter.");

	if (b_reset_continue_flag == true) {
		return;
	}

	if (info == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[IoTivity Error] invalid call-back parameter.(info is null.)");
		return;
	}

	if (info->adapter != CA_ADAPTER_TCP) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Don't support adapter type.(%d)", info->adapter);
		return;
	}

	if (strlen(info->addr) == 0 || info->port == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[IoTivity Error] invalid call-back parameter.(addrIP=%s, port=%d)", info->addr, info->port);
		return;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "IP = %s, port = %d", info->addr, info->port);

	if (connected == true) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "CONNECTED");
		things_ping_set_mask(info->addr, info->port, PING_ST_TCPCONNECT);
		return;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "DISCONNECTED");
	oic_ping_unset_mask(info->addr, PING_ST_SIGNIN | PING_ST_TCPCONNECT);

	if (ci_retry_stop_by_tcp_cb(info->addr, info->port) == -1) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] System Error.");
		return;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.");
}

int things_set_ap_connection(access_point_info_s *APinfo)
{
	if (things_is_net_initialize() == 0 || APinfo == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not yet Initialized or APinfo(0x%X) is NULL.", APinfo);
		return 0;
	}

	return gSetAPConnection(APinfo, "0");
}

/*******************************************************************
 *
 * st_things API Function register
 *
 *******************************************************************/
void things_register_set_ap_conn_func(things_set_ap_connection_func_type func)
{
	gSetAPConnection = func;
}

/*****************************************************************************
 *
 * Ready Check All of Network APIs.
 *
 * return : 0 --> Not Ready.
 *          1 --> Initialize Success.
 *         -1 --> Initialize Failed.
 *
 *****************************************************************************/
int things_is_net_initialize(void)
{
	if (gSetAPConnection == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] Not Initialized Network API.");
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] gSetAPConnection = 0x%X", gSetAPConnection);
		return 0;
	}

	return 1;
}

bool things_is_connected_ap(void)
{
	return is_connected_target_ap;
}

/*
 * If Easy-Setup is Reseted, then need to recover about Current Connected-AP info.
 * This Function serve to recover Connected-AP info with Customized-method.
 */
void things_apply_current_ap(void)
{
	if (app_state == 1) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Connected AP.(AP_name=%s) Update AP info.", app_ap_name);
		set_ssid_in_wifi_resource(app_ap_name);
	} else {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "DisConnected AP. So, Clear rested-AP info.");
		set_ssid_in_wifi_resource(NULL);
	}
}
