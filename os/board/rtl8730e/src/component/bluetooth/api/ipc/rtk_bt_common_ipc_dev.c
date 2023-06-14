/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <osdep_service.h>
#include <rtk_bt_def.h>
#include <rtk_stack_config.h>
#include <rtk_bt_common.h>
#include <bt_ipc_host_api.h>
#include <rtk_bt_ipc_dev_internal.h>

void bt_common_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	switch (p_ipc_msg->API_ID) {
	case RTK_BT_ACT_IPC_REGISTER_CB:
		switch (p_ipc_msg->param_buf[0]) {
		case RTK_BT_LE_GP_GAP: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_le_gap_cb_dev_to_host);
		}
		break;

		case RTK_BT_LE_GP_GATTC: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_gattc_cb_dev_to_host);
		}
		break;

		case RTK_BT_LE_GP_GATTS: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_gatts_cb_dev_to_host);
		}
		break;

#if defined(RKT_BLE_MESH_SUPPORT) && RKT_BLE_MESH_SUPPORT
		case RTK_BT_LE_GP_MESH_STACK: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_stack_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_CONFIG_CLIENT_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_config_client_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_GENERIC_ONOFF_CLIENT_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_generic_onoff_client_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_GENERIC_ONOFF_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_generic_onoff_server_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_DATATRANS_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_datatrans_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_lightness_client_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_lightness_server_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SETUP_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_lightness_setup_server_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_CTL_CLIENT_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_ctl_client_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_CTL_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_ctl_server_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_CTL_SETUP_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_ctl_setup_server_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_CTL_TEMPERATURE_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_ctl_temperature_server_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_HSL_CLIENT_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_hsl_client_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_HSL_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_hsl_server_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_HSL_HUE_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_hsl_hue_server_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_hsl_saturation_server_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_HSL_SETUP_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_hsl_setup_server_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_XYL_CLIENT_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_xyl_client_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_XYL_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_xyl_server_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_XYL_SETUP_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_xyl_setup_server_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_LC_CLIENT_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_lc_client_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_LC_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_lc_server_model_cb_dev_to_host);
		}
		break;
		case RTK_BT_LE_GP_MESH_LIGHT_LC_SETUP_SERVER_MODEL: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_mesh_light_lc_setup_server_model_cb_dev_to_host);
		}
		break;
#endif

		case RTK_BT_BR_GP_GAP: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_br_gap_cb_dev_to_host);
		}
		break;

		case RTK_BT_BR_GP_A2DP: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_a2dp_cb_dev_to_host);
		}
		break;

		case RTK_BT_BR_GP_HFP: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_hfp_cb_dev_to_host);
		}
		break;

		case RTK_BT_BR_GP_AVRCP: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_avrcp_cb_dev_to_host);
		}
		break;

		case RTK_BT_BR_GP_SPP: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_spp_cb_dev_to_host);
		}
		break;

		case RTK_BT_BR_GP_SDP: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_sdp_cb_dev_to_host);
		}
		break;

		case RTK_BT_LE_GP_ISO: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_le_iso_cb_dev_to_host);
		}
		break;

		case RTK_BT_LE_GP_AUDIO: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_le_audio_cb_dev_to_host);
		}
		break;

		case RTK_BT_COMMON_GP_GAP: {
			p_ipc_msg->ret[0] = rtk_bt_evt_register_callback(p_ipc_msg->param_buf[0], (rtk_bt_evt_cb_t)bt_ipc_gap_cb_dev_to_host);
		}
		break;

		default:
			break;
		}
		break;

	case RTK_BT_ACT_IPC_UNREGISTER_CB: {
		p_ipc_msg->ret[0] = rtk_bt_evt_unregister_callback(p_ipc_msg->param_buf[0]);
	}
	break;

	case RTK_BT_ACT_IPC_GET_HEAP_SIZE: {
		uint32_t heap_size = rtw_getFreeHeapSize();
		printf("[NP] The free heap size is %ld\r\n", heap_size);
	}
	break;

	default:
		break;
	}
	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}


