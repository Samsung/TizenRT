#include <stdio.h>
#include "csifw/csifw.h"
#include "csifw/PingGenerator.h"
#include "csifw/CSIParser.h"
#include "csifw/CSIPacketReceiver.h"
#include "csifw/CSINetworkMonitor.h"

COLLECT_STATE g_service_state = CSI_STATE_UNITIALIZED;
CONNECTION_STATE g_nw_state;

int SYSTEM_TAG_CSIFW;
static float *g_parsed_buffptr;
static unsigned int gParsedDataBufferLen;
get_empty_data_buffer g_get_buf_cb;
upd_raw_data_listener g_raw_callback; 
upd_parsed_data_listener g_parsed_callback;
static rtw_csi_accuracy g_accuracy;
static void* g_ptr;
static rtw_csi_action_parm_t g_csifw_config __attribute__((aligned(64))) = {0,};

static void CSIRawDataListener(CSIFW_RES res, int raw_csi_buff_len, unsigned char *raw_csi_buff, int raw_csi_data_len)
{
	//Send raw data to UPD
	if (g_parsed_callback) {
		gParsedDataBufferLen = raw_csi_data_len / (g_accuracy + 1);
		g_get_buf_cb(BTYPE_PARSED, &g_parsed_buffptr, gParsedDataBufferLen);
		getParsedData(raw_csi_buff, g_parsed_buffptr, raw_csi_buff_len, g_accuracy);
		g_parsed_callback(res, gParsedDataBufferLen, g_parsed_buffptr, g_ptr);
	}
	if (g_raw_callback) {
		g_raw_callback(res, raw_csi_buff_len, raw_csi_buff, g_ptr);
	}
}

CSIFW_RES csi_manager_init(void)
{
	/* @todo perform thread creation, other initialization tasks similiar to other framework modules in this function */
	printf("[CSIFW] initialization function is called\n");
	SYSTEM_TAG_CSIFW = -1;
	int res = system_log_set_reserved_tag_for_common(LOG_TAG, &SYSTEM_TAG_CSIFW); 
	if (res != 0 || SYSTEM_TAG_CSIFW == -1) {
		printf("Failed to set reserved tag for common log\n");
		return CSIFW_ERROR;
	}
	CSIFW_LOGE("START SYSTEM_TAG_CSIFW [%d]", SYSTEM_TAG_CSIFW);
	return CSIFW_OK;
}

CSIFW_RES csi_service_init(get_empty_data_buffer get_buf_cb, upd_raw_data_listener raw_callback, upd_parsed_data_listener parsed_callback, unsigned int interval, void* ptr)
{	
	g_ptr = ptr;
	if (g_service_state != CSI_STATE_UNITIALIZED) {
		CSIFW_LOGE("Already Initialized");
		return CSIFW_ERROR;
	}

	CSIFW_RES res = CSIFW_OK;
	g_get_buf_cb = get_buf_cb;
	g_raw_callback = raw_callback;
	g_parsed_callback = parsed_callback;
	csi_set_data_collect_interval(interval);
	gParsedDataBufferLen = MAX_CSI_BUFF_LEN;
	g_service_state = CSI_STATE_INITIALIZED;

	CSIFW_RES ret;
	if(raw_callback)
	{
		ret = csi_packet_receiver_init(CSIRawDataListener, g_get_buf_cb);
	} else {
		ret = csi_packet_receiver_init(CSIRawDataListener, NULL);
	}

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
	ping_generator_start();
	csi_packet_receiver_start_collect(&g_csifw_config);
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

CSIFW_RES csi_service_set_config(rtw_csi_action_parm_t config)
{
	g_csifw_config = config; 
	g_accuracy = config.accuracy;
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
	return CSIFW_OK;
}
