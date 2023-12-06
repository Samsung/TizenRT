/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <stdio.h>
#include <platform_opts_bt.h>
#include <osif.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
#include <bt_ipc_host_api.h>
#include <bt_ipc_profile_config.h>
#include <bt_ipc_parameter_conversion.h>
#endif

static bool b_bt_enabled = false;

#if defined(CONFIG_BT_SINGLE_CORE) && CONFIG_BT_SINGLE_CORE || \
	defined(CONFIG_BT_NP) && CONFIG_BT_NP
extern uint16_t bt_stack_enable(void *app_default_conf);
extern uint16_t bt_stack_disable(void);
#endif

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
bool rtk_bt_pre_enable(void)
{
	int *ret = NULL;
	bool err = false;

	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_DEVICE, RTK_BT_DEVICE_IPC_ACT_BT_PRE_ENABLE,
									   NULL, 0);
	if (ret[0] == 0) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	}
	err = (bool)ret[0];
	osif_mem_free(ret);
	return err;
}
#endif

bool rtk_bt_is_enable(void)
{
	return b_bt_enabled;
}

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
static bool mesh_stack_is_init = false;
bool rtk_bt_mesh_is_enable(void)
{
	return mesh_stack_is_init;
}
#endif
uint16_t rtk_bt_enable(rtk_bt_app_conf_t *app_default_conf)
{
	uint16_t err = 0;

	if (b_bt_enabled == true) {
		printf("%s: bt has been enabled \r\n", __func__);
		return RTK_BT_ERR_ALREADY_DONE;
	}

	err = rtk_bt_evt_init();
	if (err) {
		return err;
	}
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	if (app_default_conf->app_profile_support & RTK_BT_PROFILE_MESH) {
		mesh_stack_is_init = true;
	}
#endif

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	{
		int *ret = NULL;
		uint32_t actual_size = 0;
		bt_ipc_host_request_message *host_msg = NULL;

		/* depy copy secondary memory pointed by pointer */
		host_msg = (bt_ipc_host_request_message *)bt_device_push_cmd_ipc_buf(RTK_BT_DEVICE_IPC_ACT_BT_ENABLE, (void *)app_default_conf, sizeof(rtk_bt_app_conf_t), &actual_size);
		if (!host_msg) {
			printf("%s le get ipc buf fail \r\n", __func__);
			goto exit;
		}
		if (actual_size > IPC_HOST_API_DATA_MAX) {
			printf("%s: The param of %d is over ipc message memory\r\n", __func__, RTK_BT_DEVICE_IPC_ACT_BT_ENABLE);
			goto exit;
		}

		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_DEVICE, RTK_BT_DEVICE_IPC_ACT_BT_ENABLE,
										   host_msg->param_buf, actual_size);
		if (ret[0] != RTK_BT_OK) {
			printf("[core AP][IPC] %s fail ! \r\n", __func__);
		} else {
			/* check whether need memcpy return value */
			rtk_bt_device_copy_act_ret_param(RTK_BT_DEVICE_IPC_ACT_BT_ENABLE, (void *)app_default_conf, &ret[1]);
		}
		err = (uint16_t)ret[0];
		osif_mem_free(ret);
exit:
		if(host_msg)
			osif_mem_free(host_msg);
	}
#else
	err = bt_stack_enable(app_default_conf);
#endif
	if (err) {
		rtk_bt_evt_deinit();
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
		if (app_default_conf->app_profile_support & RTK_BT_PROFILE_MESH) {
			mesh_stack_is_init = false;
		}
#endif
		return err;
	}

	/* set the bt enable flag on */
	b_bt_enabled = true;

	return 0;
}

uint16_t rtk_bt_disable(void)
{

	uint16_t err = 0;

	if (b_bt_enabled == false) {
		printf("%s: bt has not been enabled \r\n", __func__);
		return RTK_BT_ERR_ALREADY_DONE;
	}

	b_bt_enabled = false;
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	{
		int *ret = NULL;

		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_DEVICE, RTK_BT_DEVICE_IPC_ACT_BT_DISABLE,
										   NULL, 0);
		if (ret[0] != RTK_BT_OK) {
			printf("[core AP][IPC] %s fail ! \r\n", __func__);
		}
		err = (uint16_t)ret[0];
		osif_mem_free(ret);
	}
#else
	err = bt_stack_disable();
#endif
	if (err) {
		return err;
	}
	err = rtk_bt_evt_deinit();
	if (err) {
		return err;
	}

	/* set the bt enable flag off */
	b_bt_enabled = false;
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	mesh_stack_is_init = false;
#endif

	return 0;
}


