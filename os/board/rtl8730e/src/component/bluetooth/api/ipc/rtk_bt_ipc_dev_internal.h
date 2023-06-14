/*
*******************************************************************************
* Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#ifndef __RTK_BT_IPC_DEV_INTERNAL_H__
#define __RTK_BT_IPC_DEV_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <rtk_bt_common.h>
#include <bt_ipc_host_api.h>
#include <bt_api_config.h>

void bt_device_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_common_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_power_control_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_vendor_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_mp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_gap_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_le_gap_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_gatts_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_gattc_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_le_mesh_stack_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_le_mesh_config_client_model_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_le_mesh_generic_onoff_client_model_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_le_mesh_datatrans_model_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_le_mesh_light_lightness_client_model_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_le_mesh_light_ctl_client_model_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_le_mesh_light_hsl_client_model_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_le_mesh_light_xyl_client_model_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_le_mesh_light_lc_client_model_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);

rtk_bt_evt_cb_ret_t bt_ipc_le_gap_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_gatts_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_gattc_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_stack_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_config_client_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_generic_onoff_client_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_generic_onoff_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_lightness_client_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_lightness_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_lightness_setup_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_ctl_client_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_ctl_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_ctl_setup_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_ctl_temperature_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_hsl_client_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_hsl_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_hsl_hue_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_hsl_saturation_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_hsl_setup_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_xyl_client_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_xyl_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_xyl_setup_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_lc_client_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_lc_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_light_lc_setup_server_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_mesh_datatrans_model_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);

rtk_bt_evt_cb_ret_t bt_ipc_gap_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);

#if defined(RTK_BREDR_SUPPORT) && RTK_BREDR_SUPPORT
void bt_br_gap_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_a2dp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_hfp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_avrcp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_sdp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
void bt_spp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);

rtk_bt_evt_cb_ret_t bt_ipc_br_gap_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_a2dp_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_hfp_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_avrcp_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_spp_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
rtk_bt_evt_cb_ret_t bt_ipc_sdp_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
#else
static inline void bt_br_gap_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	(void)p_ipc_msg;
}

static inline void bt_a2dp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	(void)p_ipc_msg;
}

static inline void bt_hfp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	(void)p_ipc_msg;
}

static inline void bt_avrcp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	(void)p_ipc_msg;
}

static inline void bt_sdp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	(void)p_ipc_msg;
}

static inline void bt_spp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	(void)p_ipc_msg;
}

static inline rtk_bt_evt_cb_ret_t bt_ipc_br_gap_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	(void)evt_code;
	(void)data;
	(void)data_len;
	return RTK_BT_EVT_CB_OK;
}

static inline rtk_bt_evt_cb_ret_t bt_ipc_a2dp_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	(void)evt_code;
	(void)data;
	(void)data_len;
	return RTK_BT_EVT_CB_OK;
}

static inline rtk_bt_evt_cb_ret_t bt_ipc_hfp_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	(void)evt_code;
	(void)data;
	(void)data_len;
	return RTK_BT_EVT_CB_OK;
}

static inline rtk_bt_evt_cb_ret_t bt_ipc_avrcp_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	(void)evt_code;
	(void)data;
	(void)data_len;
	return RTK_BT_EVT_CB_OK;
}

static inline rtk_bt_evt_cb_ret_t bt_ipc_spp_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	(void)evt_code;
	(void)data;
	(void)data_len;
	return RTK_BT_EVT_CB_OK;
}

static inline rtk_bt_evt_cb_ret_t bt_ipc_sdp_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	(void)evt_code;
	(void)data;
	(void)data_len;
	return RTK_BT_EVT_CB_OK;
}
#endif
#if (defined RTK_BLE_ISO_SUPPORT) && (RTK_BLE_ISO_SUPPORT)
void bt_le_iso_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
rtk_bt_evt_cb_ret_t bt_ipc_le_iso_cb_dev_to_host(uint8_t evt_code, void *data);
#else
static inline void bt_le_iso_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	(void)p_ipc_msg;
}
static inline rtk_bt_evt_cb_ret_t bt_ipc_le_iso_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	(void)evt_code;
	(void)data;
	(void)data_len;
	return RTK_BT_EVT_CB_OK;
}
#endif
#if (defined RTK_BLE_AUDIO_SUPPORT) && (RTK_BLE_AUDIO_SUPPORT)
void bt_le_audio_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg);
rtk_bt_evt_cb_ret_t bt_ipc_le_audio_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len);
void rtk_bt_device_le_audio_return_value(bt_ipc_host_request_message *p_ipc_msg);
#else
static inline void bt_le_audio_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	(void)p_ipc_msg;
}
static inline rtk_bt_evt_cb_ret_t bt_ipc_le_audio_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	(void)evt_code;
	(void)data;
	(void)data_len;
	return RTK_BT_EVT_CB_OK;
}
static inline void rtk_bt_device_le_audio_return_value(bt_ipc_host_request_message *p_ipc_msg)
{
	(void)p_ipc_msg;
}
#endif
#ifdef __cplusplus
}
#endif

#endif  /* __RTK_BT_IPC_DEV_INTERNAL_H__ */
