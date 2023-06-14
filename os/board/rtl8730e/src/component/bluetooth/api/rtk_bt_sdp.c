/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */
#include <platform_opts_bt.h>
#include <stdio.h>
#include <osif.h>
#include <rtk_bt_common.h>
#include <rtk_bt_def.h>
#include <rtk_bt_device.h>
#include <rtk_bt_sdp.h>
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
#include <bt_ipc_host_api.h>
#include <bt_ipc_profile_config.h>
#endif

#if !defined(CONFIG_BT_AP) || (!CONFIG_BT_AP)
extern uint16_t bt_stack_sdp_record_add(void *record, uint32_t length);
#endif

uint16_t rtk_sdp_record_add(void *record, uint32_t length)
{
	/* sdp record add should be invoked before bt enable */
	if (rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;
	/* Check validity */
	if (!record) {
		printf("rtk_sdp_record_add: record is NULL \r\n");
		return RTK_BT_ERR_NO_PERM;
	}
	/* length check */
	if (length > RTK_BT_SDK_MAX_RECORD_LENGTH) {
		printf("rtk_sdp_record_add: SDP record length %d is over max record memory \r\n", (int)length);
		return RTK_BT_FAIL;
	}
#if defined (CONFIG_BT_AP) && CONFIG_BT_AP
	{
		rtk_sdp_record_t *p_sdp_record_t = NULL;
		int *ret = NULL;
		uint16_t err;

		if ((length + 4) > IPC_HOST_API_DATA_MAX) {
			printf("rtk_sdp_record_add: param length %d is over host max buffer size \r\n", (int)length);
			return RTK_BT_FAIL;
		}
		p_sdp_record_t = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_sdp_record_t));
		if (!p_sdp_record_t) {
			printf("rtk_sdp_record_add: allocate sdp_record_t fail \r\n");
			return RTK_BT_FAIL;
		}
		memcpy((void *)p_sdp_record_t->record, record, length);
		p_sdp_record_t->length = length;
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_SDP, RTK_BT_SDP_ACT_RECORD_ADD,
									(uint8_t *)p_sdp_record_t, sizeof(rtk_sdp_record_t));
		if (ret[0] != RTK_BT_OK) {
			printf("[core CA7][IPC] %s fail ! \r\n", __func__);
		}
		err = ret[0];
		osif_mem_free(ret);
		osif_mem_free(p_sdp_record_t);

		return err;
	}
#else
	return bt_stack_sdp_record_add(record, length);
#endif
}

