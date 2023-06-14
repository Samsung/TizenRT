/*
*******************************************************************************
* Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <platform_opts_bt.h>
#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <rtk_bt_spp.h>
#include <rtk_bt_def.h>
#include <rtk_bt_device.h>
#include <rtk_bt_common.h>
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
#include <bt_ipc_host_api.h>
#include <bt_ipc_profile_config.h>
#endif

#if !defined(CONFIG_BT_AP) || (!CONFIG_BT_AP)
/* SPP service register config */
extern uint8_t spp_max_link_num;
extern uint8_t spp_rfc_max_service_num;
extern T_SPP_CHANN_DB spp_chann_db[SPP_CAHNN_DB_MAX_LINK_NUM];
extern T_SPP_CHANN_DB *bt_stack_alloc_spp_chann(uint8_t server_chann, uint8_t* service_uuid, uint16_t uuid_length);

extern uint8_t default_mtu_size;
extern uint8_t default_credits;
#endif

uint16_t rtk_bt_spp_init_cfg(uint8_t max_link_num, uint8_t max_service_num, uint16_t mtu_size, uint8_t credits)
{
	if (rtk_bt_is_enable()) {
		printf("[SPP]: rtk_bt_spp_init_cfg should not be invoked after bt enable \r\n");
		return RTK_BT_FAIL;
	}
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	{
		int *ret = NULL;
		uint16_t err;
		rtk_bt_spp_init_cfg_t *pinit_param = NULL;

		pinit_param = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_spp_init_cfg_t));
		if (!pinit_param) {
			printf("%s allocate spp_init_cfg_t fail \r\n", __func__);
			return RTK_BT_FAIL;
		}
		pinit_param->spp_max_link_num = max_link_num;
		pinit_param->spp_rfc_max_service_num = max_service_num;
		pinit_param->default_mtu_size = mtu_size;
		pinit_param->default_credits = credits;
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_SPP, RTK_BT_SPP_ACT_INIT_CFG,
									(uint8_t *)pinit_param, sizeof(rtk_bt_spp_init_cfg_t));
		if (ret[0] != RTK_BT_OK) {
			printf("[core AP][IPC] %s fail ! \r\n", __func__);
		}
		err = ret[0];
		osif_mem_free(ret);
		osif_mem_free(pinit_param);

		return err;
	}
#else
	spp_max_link_num = max_link_num;
	spp_rfc_max_service_num = max_service_num;
	default_mtu_size = mtu_size;
	default_credits = credits;
	return RTK_BT_OK;
#endif
}

uint16_t rtk_bt_service_register_cfg(void *spp_service_class_uuid, uint32_t length, uint8_t local_server_chann)
{
	if (rtk_bt_is_enable()) {
		printf("[SPP]: rtk_bt_service_register_cfg should not be invoked after bt enable \r\n");
		return RTK_BT_FAIL;
	}
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	{
		int *ret = NULL;
		uint16_t err;
		rtk_bt_spp_srv_cfg_t *psrv_param = NULL;

		if (length > RTK_BT_SPP_MAX_SRV_CLASS_UUID_LENGTH) {
			printf("The param of %s is over flow\r\n", __func__);
			return RTK_BT_FAIL;
		}
		psrv_param = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_spp_srv_cfg_t));
		if (!psrv_param) {
			printf("%s allocate spp_srv_cfg_t fail \r\n", __func__);
			return RTK_BT_FAIL;
		}
		memcpy((void *)psrv_param->srv_class_uuid128, spp_service_class_uuid, length);
		psrv_param->length = length;
		psrv_param->local_server_chann = local_server_chann;
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_SPP, RTK_BT_SPP_ACT_SRV_CFG,
									(uint8_t *)psrv_param, sizeof(rtk_bt_spp_srv_cfg_t));
		if (ret[0] != RTK_BT_OK) {
			printf("[core AP][IPC] %s fail ! \r\n", __func__);
		}
		err = ret[0];
		osif_mem_free(ret);
		osif_mem_free(psrv_param);

		return err;
	}
#else
	if (bt_stack_alloc_spp_chann(local_server_chann, (uint8_t*)spp_service_class_uuid, length) == NULL){
		return RTK_BT_FAIL;
	}
	return RTK_BT_OK;
#endif
}

uint16_t rtk_bt_spp_connect(uint8_t *bd_addr) 
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable()) {
		return RTK_BT_ERR_NOT_READY;
	}

	if (!bd_addr) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_SPP, RTK_BT_SPP_ACT_CONNECT,
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_spp_disconnect(rtk_bt_spp_disconn_req_t *p_disconn_req_t) 
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable()) {
		return RTK_BT_ERR_NOT_READY;
	}

	if (!p_disconn_req_t) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_SPP, RTK_BT_SPP_ACT_DISCONNECT,
							p_disconn_req_t, sizeof(rtk_bt_spp_disconn_req_t));

	return ret;
}

uint16_t rtk_bt_spp_disconnect_all(uint8_t *bd_addr) 
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable()) {
		return RTK_BT_ERR_NOT_READY;
	}

	if (!bd_addr) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_SPP, RTK_BT_SPP_ACT_DISCONNECT_ALL,
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_spp_send_data(rtk_bt_spp_send_data_t *p_send_data_t) 
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable()) {
		return RTK_BT_ERR_NOT_READY;
	}

	if (!p_send_data_t) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_SPP, RTK_BT_SPP_ACT_SEND_DATA,
							p_send_data_t, sizeof(rtk_bt_spp_send_data_t));

	return ret;
}

uint16_t rtk_bt_spp_credits_give(rtk_bt_spp_credits_give_t *p_credits_give_t) 
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable()) {
		return RTK_BT_ERR_NOT_READY;
	}

	if (!p_credits_give_t) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_SPP, RTK_BT_SPP_ACT_GIVE_CREDITS,
							p_credits_give_t, sizeof(rtk_bt_spp_credits_give_t));

	return ret;
}

