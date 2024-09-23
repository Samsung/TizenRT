/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#include "include/csifw.h"
#include "include/PingGenerator.h"
#include "include/CSIParser.h"
#include "include/CSIPacketReceiver.h"
#include "include/CSINetworkMonitor.h"

COLLECT_STATE g_service_state = CSI_STATE_UNITIALIZED;
CONNECTION_STATE g_nw_state;

static float *g_parsed_buffptr;
static uint16_t gParsedDataBufferLen;
upd_raw_data_listener g_raw_callback; 
upd_parsed_data_listener g_parsed_callback;
static void* g_ptr;
static csi_action_param_t g_csifw_config __attribute__((aligned(64))) = {0,};

static void CSIRawDataListener(CSIFW_RES res, int raw_csi_buff_len, unsigned char *raw_csi_buff, int raw_csi_data_len)
{
	//Send raw data to UPD
	if (g_parsed_callback) {
		getParsedData(raw_csi_buff, raw_csi_buff_len, &g_csifw_config, g_parsed_buffptr, &gParsedDataBufferLen);
		g_parsed_callback(res, gParsedDataBufferLen, g_parsed_buffptr, g_ptr);
	}
	if (g_raw_callback) {
		g_raw_callback(res, raw_csi_buff_len, raw_csi_buff, g_ptr);
	}
}

CSIFW_RES csi_manager_init(void)
{
	CSIFW_LOGI("[CSIFW] initialization function is called\n");
	return CSIFW_OK;
}

CSIFW_RES csi_service_init(upd_raw_data_listener raw_callback, upd_parsed_data_listener parsed_callback, unsigned int interval, void* ptr)
{	
	g_ptr = ptr;
	if (g_service_state != CSI_STATE_UNITIALIZED) {
		CSIFW_LOGE("Already Initialized");
		return CSIFW_ERROR;
	}

	CSIFW_RES res = CSIFW_OK;
	g_raw_callback = raw_callback;
	g_parsed_callback = parsed_callback;
	csi_set_data_collect_interval(interval);
	gParsedDataBufferLen = CSIFW_MAX_RAW_BUFF_LEN;
	if (g_parsed_callback) {
		g_parsed_buffptr = (float *)malloc(sizeof(float) * gParsedDataBufferLen);
		if (!g_parsed_buffptr) {
			CSIFW_LOGE("Failed to allocate memory for parsed data buffer, size: %zu", sizeof(float) * gParsedDataBufferLen);
			return CSIFW_ERROR;
		}
	}
	g_service_state = CSI_STATE_INITIALIZED;

	CSIFW_RES ret;
	ret = csi_packet_receiver_init(CSIRawDataListener);

	if (ret != CSIFW_OK) {
		CSIFW_LOGE("csi_packet_receiver_init failed");
		return CSIFW_ERROR;
	}
	CSIFW_LOGD("CSI Packet receiver init done");

	return res;
}

CSIFW_RES csi_set_data_collect_interval(unsigned interval)
{
	return ping_generator_change_interval(interval);
}

CSIFW_RES csi_service_start()
{	
	if (g_service_state == CSI_COLLECT_STATE_STARTED) {
		CSIFW_LOGE(" Already running");
		return CSIFW_ERROR;
	}
	csi_packet_receiver_start_collect(&g_csifw_config);
	ping_generator_start();
	g_service_state = CSI_COLLECT_STATE_STARTED;
	return CSIFW_OK;
}

CSIFW_RES csi_service_stop(CSIFW_REASON reason)
{
	if (g_service_state != CSI_COLLECT_STATE_STARTED) {
		CSIFW_LOGE("Already stopped");
		g_service_state = CSI_COLLECT_STATE_STOPPED;
		return CSIFW_ERROR;
	}
	g_service_state = CSI_COLLECT_STATE_STOPPED;
	if (reason == CSIFW_WIFI_DISCONNECTED)
	{
		CSIFW_LOGI("[CSIFW_REASON] CSIFW_WIFI_DISCONNECTED %d", reason);
	}
	ping_generator_stop(reason);
	csi_packet_receiver_stop_collect(reason, &g_csifw_config);
	return CSIFW_OK;
}

CSIFW_RES csi_service_set_config(csi_action_param_t config)
{
	g_csifw_config = config; 
	return CSIFW_OK;
}

CSIFW_RES csi_get_ap_mac_addr(csifw_mac_info *mac_info) {
	return csi_packet_receiver_get_mac_addr(mac_info);
}

CSIFW_RES csi_service_deinit()
{
	CSIFW_LOGD("csi_service_deinit");
	if (g_service_state != CSI_COLLECT_STATE_STOPPED) {
		csi_service_stop(CSIFW_NORMAL);
	}
	g_service_state = CSI_STATE_UNITIALIZED;
	csi_packet_receiver_deinit();
	if (g_parsed_buffptr) {
		free(g_parsed_buffptr);
		g_parsed_buffptr = NULL;
		CSIFW_LOGD("csi_service_deinit");
	}
	return CSIFW_OK;
}

