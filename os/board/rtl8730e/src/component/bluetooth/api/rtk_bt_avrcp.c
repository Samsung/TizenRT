/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */
#include <string.h>
#include <platform_opts_bt.h>
#include <osif.h>
#include <rtk_bt_common.h>
#include <rtk_bt_def.h>
#include <rtk_bt_device.h>
#include <rtk_bt_avrcp.h>
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
#include <bt_ipc_host_api.h>
#include <bt_ipc_profile_config.h>
#endif

uint16_t rtk_bt_avrcp_connect(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_AVRCP, RTK_BT_AVRCP_ACT_CONNECT, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_avrcp_disconnect(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_AVRCP, RTK_BT_AVRCP_ACT_DISCONNECT, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_avrcp_play(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_AVRCP, RTK_BT_AVRCP_ACT_PLAY, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_avrcp_pause(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_AVRCP, RTK_BT_AVRCP_ACT_PAUSE, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_avrcp_stop(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_AVRCP, RTK_BT_AVRCP_ACT_STOP, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_avrcp_forward(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_AVRCP, RTK_BT_AVRCP_ACT_FORWARD, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_avrcp_backward(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_AVRCP, RTK_BT_AVRCP_ACT_BACKWARD, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_avrcp_rewind_start(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_AVRCP, RTK_BT_AVRCP_ACT_REWIND_START, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_avrcp_rewind_stop(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_AVRCP, RTK_BT_AVRCP_ACT_REWIND_STOP, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_avrcp_fast_forward_start(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_AVRCP, RTK_BT_AVRCP_ACT_FAST_FORWARD_START, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_avrcp_fast_forward_stop(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_AVRCP, RTK_BT_AVRCP_ACT_FAST_FORWARD_STOP, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_avrcp_absolute_volume_set(uint8_t *bd_addr, uint8_t volume)
{
	uint16_t ret = 0;
	rtk_bt_avrcp_absolute_volume_set_t volume_t = {0};

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	memcpy((void *)&volume_t.bd_addr, (void *)bd_addr, 6);
	volume_t.volume = volume;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_AVRCP, RTK_BT_AVRCP_ACT_ABSOLUTE_VOLUME_SET, 
							(void *)&volume_t, sizeof(rtk_bt_avrcp_absolute_volume_set_t));

	return ret;
}

uint16_t rtk_bt_avrcp_volume_change_req(uint8_t *bd_addr, uint8_t volume)
{
	uint16_t ret = 0;
	rtk_bt_avrcp_volume_change_req_t volume_t = {0};

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	memcpy((void *)&volume_t.bd_addr, (void *)bd_addr, 6);
	volume_t.volume = volume;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_AVRCP, RTK_BT_AVRCP_ACT_VOLUME_CHANGE_REQ, 
							(void *)&volume_t, sizeof(rtk_bt_avrcp_volume_change_req_t));

	return ret;
}

