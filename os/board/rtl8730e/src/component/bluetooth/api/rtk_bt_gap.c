/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <string.h>
#include <stdio.h>
#include <osif.h>
#include <platform_opts_bt.h>
#include <bt_api_config.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_gap.h>
#include <rtk_bt_device.h>

#if defined(RTK_BT_5_2_L2C_ECFC_SUPPORT) && RTK_BT_5_2_L2C_ECFC_SUPPORT
void rtk_bt_gap_ecfc_print_cid(uint16_t *cid, uint8_t num)
{
	int i;
	if(num > RTK_BT_GAP_ECFC_CHANN_MAX_NUM)
		return;

	printf("[APP] cid_num(%d):", num);
	for(i = 0; i < num; i++) {
		printf(" %d", cid[i]);
	}
	printf("\r\n");
}

uint16_t rtk_bt_gap_ecfc_set_param(rtk_bt_gap_ecfc_param_t type, uint16_t value)
{
	uint16_t ret = 0;
    rtk_bt_gap_ecfc_param_set_t param = {
        .type = type,
        .value = value,
    };

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;
	
	ret = rtk_bt_send_cmd(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_ACT_ECFC_SET_PARAM, &param, sizeof(rtk_bt_gap_ecfc_param_set_t));

	return ret;
}

uint16_t rtk_bt_gap_ecfc_register(rtk_bt_gap_ecfc_register_t *p_param)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;
	
	ret = rtk_bt_send_cmd(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_ACT_ECFC_REGISTER, p_param, sizeof(rtk_bt_gap_ecfc_register_t));

	return ret;
}

uint16_t rtk_bt_gap_ecfc_send_sec_reg_req(rtk_bt_gap_ecfc_sec_reg_req_t *p_param)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;
	
	ret = rtk_bt_send_cmd(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_ACT_ECFC_SEC_REG_REQ, p_param, sizeof(rtk_bt_gap_ecfc_sec_reg_req_t));

	return ret;
}

uint16_t rtk_bt_gap_ecfc_send_conn_req(rtk_bt_gap_ecfc_conn_req_t *p_param)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;
	
	ret = rtk_bt_send_cmd(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_ACT_ECFC_CONN_REQ, p_param, sizeof(rtk_bt_gap_ecfc_conn_req_t));

	return ret;
}

uint16_t rtk_bt_gap_ecfc_send_conn_cfm(rtk_bt_gap_ecfc_conn_cfm_t *p_param)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;
	
	ret = rtk_bt_send_cmd(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_ACT_ECFC_CONN_CFM, p_param, sizeof(rtk_bt_gap_ecfc_conn_cfm_t));

	return ret;
}

uint16_t rtk_bt_gap_ecfc_send_disconn_req(uint16_t conn_handle, uint16_t cid)
{
	uint16_t ret = 0;
    rtk_bt_gap_ecfc_disconn_req_t param = {
        .conn_handle = conn_handle,
        .cid = cid
    };

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;
	
	ret = rtk_bt_send_cmd(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_ACT_ECFC_DISCONN_REQ, &param, sizeof(rtk_bt_gap_ecfc_disconn_req_t));

	return ret;
}

uint16_t rtk_bt_gap_ecfc_send_data(uint16_t conn_handle, uint16_t cid, uint8_t *p_data, uint16_t length)
{
	uint16_t ret = 0;
    rtk_bt_gap_ecfc_send_data_t param = {
        .conn_handle = conn_handle,
        .cid = cid,
        .p_data = p_data,
        .length = length
    };

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;
	
	ret = rtk_bt_send_cmd(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_ACT_ECFC_SEND_DATA, &param, sizeof(rtk_bt_gap_ecfc_send_data_t));

	return ret;
}

uint16_t rtk_bt_gap_ecfc_send_reconf_req(rtk_bt_gap_ecfc_reconf_req_t *p_param)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;
	
	ret = rtk_bt_send_cmd(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_ACT_ECFC_RECONF_REQ, p_param, sizeof(rtk_bt_gap_ecfc_reconf_req_t));

	return ret;
}
#endif

uint16_t rtk_bt_gap_vendor_cmd_req(rtk_bt_gap_vendor_cmd_param_t *vendor_param)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable()) {
		return RTK_BT_ERR_NOT_READY;
	}

	ret = rtk_bt_send_cmd(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_ACT_VENDOR_CMD_REQ, vendor_param, sizeof(rtk_bt_gap_vendor_cmd_param_t));

	return ret;
}
