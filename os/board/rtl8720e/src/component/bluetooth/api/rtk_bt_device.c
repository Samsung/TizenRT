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
#include <rtk_coex.h>
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
		printf("[core CA7][IPC] %s fail ! \r\n", __func__);
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

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	{
		int *ret = NULL;

		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_DEVICE, RTK_BT_DEVICE_IPC_ACT_BT_ENABLE,
										(uint8_t *)app_default_conf, sizeof(rtk_bt_app_conf_t));
		if (ret[0] != RTK_BT_OK) {
			printf("[core AP][IPC] %s fail ! \r\n", __func__);
		} else {
			/* check whether need memcpy return value */
			rtk_bt_device_copy_act_ret_param(RTK_BT_DEVICE_IPC_ACT_BT_ENABLE, (void*)app_default_conf, &ret[1]);
		}
		err = (uint16_t)ret[0];
		osif_mem_free(ret);
	}
#else
	err = bt_stack_enable(app_default_conf);
#endif
	if (err) {
		rtk_bt_evt_deinit();
		return err;
	}

#if defined(CONFIG_BT_SINGLE_CORE) && CONFIG_BT_SINGLE_CORE || \
	defined(CONFIG_BT_NP) && CONFIG_BT_NP
	bt_coex_init();
#endif
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
	if (err)
		return err;
	err = rtk_bt_evt_deinit();
	if (err)
		return err;

	/* set the bt enable flag off */
	b_bt_enabled = false;

	return 0;
}


