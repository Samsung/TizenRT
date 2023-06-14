/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <bt_api_config.h>
#if defined(RTK_BREDR_SUPPORT) && RTK_BREDR_SUPPORT
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <stdio.h>
#include <string.h>
#include <dlist.h>
#include <osif.h>
#include <rtk_bt_br_gap.h>
#include <rtk_bt_sdp.h>
#include <gap_br.h>
#include <trace_app.h>
#include <bt_sdp.h>
#include <remote.h>
#include <btm.h>

/* sdp record list */
static struct list_head			rtk_sdp_record_list;
static uint8_t					rtk_sdp_record_init_flag = 0;

static uint16_t rtk_sdp_record_list_enqueue(struct list_head *plist)
{
	/* initialize first */
	if (!rtk_sdp_record_init_flag) {
		INIT_LIST_HEAD(&rtk_sdp_record_list);
		rtk_sdp_record_init_flag = 1;
	}
	/* insert list */
	list_add_tail(plist, &rtk_sdp_record_list);

	return RTK_BT_OK;
}

static struct list_head *rtk_sdp_record_list_dequeue(void)
{
	struct list_head *plist = NULL;

	if (list_empty(&rtk_sdp_record_list)) {
		printf("%s(): rtk_sdp_record_list is empty !\r\n", __func__);
		return NULL;
	} else {
		plist = rtk_sdp_record_list.next;
		list_del_init(plist);
		return plist;
	}
}

uint16_t bt_stack_sdp_record_add(void *record, uint32_t length)
{
	rtk_sdp_record_t *p_temp_record_t = NULL;

	/* allocate record mem */
	p_temp_record_t = (rtk_sdp_record_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_sdp_record_t));
	if (!p_temp_record_t) {
		printf("%s(): p_temp_record_t allocate fail !\r\n", __func__);
		return RTK_BT_FAIL;
	}
	/* memcpy record data */
	memcpy((void *)p_temp_record_t->record, record, length);
	/* construct sdp record struct */
	INIT_LIST_HEAD(&p_temp_record_t->list);
	p_temp_record_t->length = length;
	/* add to record list, waiting for rtk_sdp_init() invoking */
	rtk_sdp_record_list_enqueue(&p_temp_record_t->list);

	return RTK_BT_OK;
}

uint16_t bt_stack_sdp_act_handle(rtk_bt_cmd_t *p_cmd)
{
	int16_t ret = 0;
	API_PRINT("bt_stack_sdp_act_handle: act = %d \r\n", p_cmd->act);
	switch(p_cmd->act){

		default:
			printf("bt_stack_sdp_act_handle: unknown act: %d \r\n", p_cmd->act);
			ret = 0;
			break;
	}

	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);

	return ret;
}

uint16_t bt_stack_sdp_init(void)
{
	printf("[SDP]app_sdp_init\n");

	struct list_head *plist = NULL;
	struct list_head *plist_next = NULL;
	rtk_sdp_record_t *precord = NULL;

	if (list_empty(&rtk_sdp_record_list)) {
		printf("%s(): rtk_sdp_record_list is empty !\r\n", __func__);
		return RTK_BT_FAIL;
	}
	plist = rtk_sdp_record_list.next;
	while (1) {
		if (&rtk_sdp_record_list == plist) {
			break;
		} else {
			plist_next = plist->next;
			precord = (rtk_sdp_record_t *)plist;
			bt_sdp_record_add((void *)precord->record);
			plist = plist_next;
		}
	}

	return RTK_BT_OK;
}

extern void sdp_deinit(void);

void bt_stack_sdp_deinit(void)
{
	struct list_head *plist = NULL;
	rtk_sdp_record_t *precord = NULL;

	printf("[SDP]app_sdp_deinit\n");
	rtk_sdp_record_init_flag = 0;
	while (1) {
		if (list_empty(&rtk_sdp_record_list)) {
			printf("%s(): rtk_sdp_record_list is empty !\r\n", __func__);
			break;
		}
		plist = rtk_sdp_record_list_dequeue();
		precord = (rtk_sdp_record_t *)plist;
		bt_sdp_record_delete((void *)precord->record);
		osif_mem_free((void *)precord);
	}
	sdp_deinit();
}

#endif