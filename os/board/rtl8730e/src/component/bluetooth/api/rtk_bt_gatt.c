/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include "platform_autoconf.h"
#include <osif.h>
#include <rtk_bt_common.h>
#include <rtk_bt_def.h>
#include <rtk_bt_device.h>
#include <rtk_bt_gatts.h>
#include <rtk_bt_gattc.h>

/********************************* GATTS API *********************************/
uint16_t rtk_bt_gatts_register_service(struct rtk_bt_gatt_service *param)
{
	uint16_t ret = 0;

	if (!param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (param->type >= GATT_SERVICE_TYPE_RESERVED) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (param->attr_count == 0) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTS, RTK_BT_GATTS_ACT_REGISTER_SERVICE,
						  (void *)param, sizeof(struct rtk_bt_gatt_service));

	return ret;
}

uint16_t rtk_bt_gatts_notify(rtk_bt_gatts_ntf_and_ind_param_t *param)
{
	uint16_t ret = 0;

	if (!param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTS, RTK_BT_GATTS_ACT_NOTIFY,
						  (void *)param, sizeof(rtk_bt_gatts_ntf_and_ind_param_t));

	return ret;
}

uint16_t rtk_bt_gatts_indicate(rtk_bt_gatts_ntf_and_ind_param_t *param)
{
	uint16_t ret = 0;

	if (!param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTS, RTK_BT_GATTS_ACT_INDICATE,
						  (void *)param, sizeof(rtk_bt_gatts_ntf_and_ind_param_t));

	return ret;
}

uint16_t rtk_bt_gatts_read_resp(rtk_bt_gatts_read_resp_param_t *param)
{
	uint16_t ret = 0;

	if (!param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTS, RTK_BT_GATTS_ACT_READ_RSP,
						  (void *)param, sizeof(rtk_bt_gatts_read_resp_param_t));

	return ret;
}

uint16_t rtk_bt_gatts_write_resp(rtk_bt_gatts_write_resp_param_t *param)
{
	uint16_t ret = 0;

	if (!param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}
	/* write_no_rsp and write_signed no need to response, so directly return here. */
	if ((param->type == RTK_BT_GATTS_WRITE_NO_RESP) ||
		(param->type == RTK_BT_GATTS_WRITE_NO_RESP_SIGNED)) {
		return 0;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTS, RTK_BT_GATTS_ACT_WRITE_RSP,
						  (void *)param, sizeof(rtk_bt_gatts_write_resp_param_t));

	return ret;
}

uint16_t rtk_bt_gatts_service_changed_indicate(uint16_t conn_handle, uint16_t cid,
											   uint16_t start_handle, uint16_t end_handle)
{
	uint16_t ret = 0;
	rtk_bt_gatts_service_changed_indicate_param_t param = {
		.conn_handle = conn_handle,
		.cid = cid,
		.start_handle = start_handle,
		.end_handle = end_handle,
	};

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTS, RTK_BT_GATTS_ACT_SERVICE_CHANGED_INDICATE,
						  &param, sizeof(rtk_bt_gatts_service_changed_indicate_param_t));

	return ret;
}

uint16_t rtk_bt_gatts_get_attribute_handle(uint16_t app_id, uint16_t attr_index, uint16_t *attr_handle)
{
	uint16_t ret = 0;
	rtk_bt_gatts_get_attr_handle_param_t param = {
		.app_id = app_id,
		.attr_index = attr_index,
		.attr_handle = attr_handle,
	};

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTS, RTK_BT_GATTS_ACT_GET_ATTR_HANDLE,
						  &param, sizeof(rtk_bt_gatts_get_attr_handle_param_t));

	return ret;
}

/********************************* GATTC API *********************************/
#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
uint16_t rtk_bt_gattc_register_profile(uint16_t profile_id, rtk_bt_gattc_uuid_t srv_uuid)
{
	uint16_t ret = 0;
	rtk_bt_gattc_register_t param = {
		.profile_id = profile_id,
		.srv_uuid = srv_uuid,
	};

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_ACT_REGISTER_PROFILE,
						  &param, sizeof(rtk_bt_gattc_register_t));

	return ret;
}

uint16_t rtk_bt_gattc_discover_all(uint16_t conn_handle)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_ACT_DISCOVER,
						  (void *)&conn_handle, sizeof(uint16_t));

	return ret;
}

uint16_t rtk_bt_gattc_find(rtk_bt_gattc_find_param_t *p_find_param)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_ACT_FIND,
						  (void *)p_find_param, sizeof(rtk_bt_gattc_find_param_t));

	return ret;
}

#else /* #if RTK_BLE_MGR_LIB */

uint16_t rtk_bt_gattc_register_profile(uint16_t profile_id)
{
	uint16_t ret = 0;
	uint16_t profile_id_local = profile_id;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_ACT_REGISTER_PROFILE,
						  (void *)&profile_id_local, 2);

	return ret;
}

uint16_t rtk_bt_gattc_exchange_mtu(uint16_t conn_handle)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_ACT_EXCHANGE_MTU,
						  (void *)&conn_handle, sizeof(uint16_t));

	return ret;
}

uint16_t rtk_bt_gattc_discover(rtk_bt_gattc_discover_param_t *p_dis_param)
{
	uint16_t ret = 0;

	if (!p_dis_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_ACT_DISCOVER,
						  (void *)p_dis_param, sizeof(rtk_bt_gattc_discover_param_t));

	return ret;
}

#endif /* #if RTK_BLE_MGR_LIB */

uint16_t rtk_bt_gattc_read(rtk_bt_gattc_read_param_t *p_read_param)
{
	uint16_t ret = 0;

	if (!p_read_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_ACT_READ,
						  (void *)p_read_param, sizeof(rtk_bt_gattc_read_param_t));

	return ret;
}

uint16_t rtk_bt_gattc_write(rtk_bt_gattc_write_param_t *p_write_param)
{
	uint16_t ret = 0;

	if (!p_write_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_ACT_WRITE,
						  (void *)p_write_param, sizeof(rtk_bt_gattc_write_param_t));

	return ret;
}

uint16_t rtk_bt_gattc_enable_notify_or_indicate(
	rtk_bt_gattc_update_cccd_param_t *p_update_cccd_param)
{
	uint16_t ret = 0;

	if (!p_update_cccd_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_ACT_CCCD_ENABLE,
						  (void *)p_update_cccd_param, sizeof(rtk_bt_gattc_update_cccd_param_t));

	return ret;
}

uint16_t rtk_bt_gattc_disable_notify_or_indicate(
	rtk_bt_gattc_update_cccd_param_t *p_update_cccd_param)
{
	uint16_t ret = 0;

	if (!p_update_cccd_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_ACT_CCCD_DISABLE,
						  (void *)p_update_cccd_param, sizeof(rtk_bt_gattc_update_cccd_param_t));

	return ret;
}