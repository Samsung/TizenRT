/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include "platform_autoconf.h"
#include <stdio.h>
#include <osif.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>

static bool b_bt_enabled = false;

extern uint16_t bt_stack_enable(void *app_default_conf);
extern uint16_t bt_stack_disable(void);

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
		BT_LOGE("%s: bt is enabled already.\r\n", __func__);
		return RTK_BT_ERR_ALREADY_DONE;
	}

#if defined(RTK_BT_API_MEM_PRE_ALLOC) && RTK_BT_API_MEM_PRE_ALLOC
	if (!bt_api_sem_pool_init()) {
		return RTK_BT_ERR_NO_RESOURCE;
	}
	if (!bt_evt_mem_pool_init()) {
		err = RTK_BT_ERR_NO_RESOURCE;
		goto evt_pool_fail;
	}
#endif

	err = rtk_bt_evt_init();
	if (err) {
		goto evt_fail;
	}

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	if (app_default_conf->app_profile_support & RTK_BT_PROFILE_MESH) {
		mesh_stack_is_init = true;
	}
#endif

	err = bt_stack_enable(app_default_conf);
	if (err) {
		goto stack_fail;
	}

	rtk_bt_log_init();

	/* set the bt enable flag on */
	b_bt_enabled = true;
	return 0;

stack_fail:
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	if (app_default_conf->app_profile_support & RTK_BT_PROFILE_MESH) {
		mesh_stack_is_init = false;
	}
#endif
	rtk_bt_evt_deinit();
evt_fail:
#if defined(RTK_BT_API_MEM_PRE_ALLOC) && RTK_BT_API_MEM_PRE_ALLOC
	bt_evt_mem_pool_deinit();
evt_pool_fail:
	bt_api_sem_pool_deinit();
#endif
	return err;
}

uint16_t rtk_bt_disable(void)
{
	uint16_t err = 0;

	if (b_bt_enabled == false) {
		BT_LOGE("%s: bt is not enabled.\r\n", __func__);
		return RTK_BT_ERR_ALREADY_DONE;
	}

	/* set the bt enable flag off */
	b_bt_enabled = false;

	/* wait for sending cmd complete, prevent cmd send task interrupted by deinit task */
	bt_wait_cmd_send_complete();

	err = bt_stack_disable();
	if (err) {
		return err;
	}

	err = rtk_bt_evt_deinit();
	if (err) {
		return err;
	}

#if defined(RTK_BT_API_MEM_PRE_ALLOC) && RTK_BT_API_MEM_PRE_ALLOC
	bt_api_sem_pool_deinit();
	bt_evt_mem_pool_deinit();
#endif

	rtk_bt_log_deinit();

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	mesh_stack_is_init = false;
#endif

	return 0;
}


