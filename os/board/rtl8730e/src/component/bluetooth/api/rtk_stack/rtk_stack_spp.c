/*
*******************************************************************************
* Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

/* -------------------------------- Includes -------------------------------- */
#include <bt_api_config.h>
#if defined(RTK_BREDR_SUPPORT) && RTK_BREDR_SUPPORT
#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <rtk_bt_def.h>
#include <rtk_bt_spp.h>
#include <rtk_bt_br_gap.h>
#include <rtk_bt_common.h>
#include <gap_br.h>
#include <bt_spp.h>
#include <btm.h>

T_SPP_CHANN_DB spp_chann_db[SPP_CAHNN_DB_MAX_LINK_NUM];// SPP service register config database

uint8_t spp_max_link_num; // SPP maximum link numbers
uint8_t spp_rfc_max_service_num; // SPP maximum service numbers
uint8_t default_mtu_size; // link default mtu size
uint8_t default_credits; // link default credits

extern T_APP_DB app_db;
extern T_APP_BR_LINK *app_find_br_link(uint8_t *bd_addr);

static uint8_t client_addr[6];// SPP client bd addr

/* Register service and spp channel index in SPP service database */
T_SPP_CHANN_DB *bt_stack_alloc_spp_chann(uint8_t server_chann, uint8_t* service_uuid, uint16_t uuid_length)
{
	T_SPP_CHANN_DB *p_chann = NULL;
	for (int i = 0; i < SPP_CAHNN_DB_MAX_LINK_NUM; i++)
	{
		if (spp_chann_db[i].used == false) 
		{
			p_chann = &spp_chann_db[i];
			
			p_chann->used = true;
			p_chann->spp_chann_num = server_chann;
			memcpy(p_chann->service_uuid.uuid_128, service_uuid, uuid_length);
			break;
		}
	}
	return p_chann;
}
/* Search the related element in SPP service database */
static T_SPP_CHANN_DB *bt_stack_find_spp_chann(uint8_t server_chann)
{
	T_SPP_CHANN_DB *p_chann = NULL;
	for (int i = 0; i < SPP_CAHNN_DB_MAX_LINK_NUM; i++)
	{
		if (spp_chann_db[i].used == true && spp_chann_db[i].spp_chann_num == server_chann)
		{
			p_chann = &spp_chann_db[i];
			break;	
		}
	}
	return p_chann;
}

/*
static bool app_free_spp_chann(T_SPP_CHANN_DB *p_chann)
{
	if (p_chann != NULL) 
	{
		if (p_chann->used == true)
		{
			memset(p_chann, 0, sizeof(T_SPP_CHANN_DB));
			return true;
		}
	}
	return false;
}
*/

static void bt_stack_spp_evt_ind_cback(T_BT_EVENT event_type, void *event_buf, uint16_t buf_len) 
{
	(void)buf_len;
	T_BT_EVENT_PARAM *param = (T_BT_EVENT_PARAM *)event_buf;
	rtk_bt_evt_t *p_evt = NULL;
	T_APP_BR_LINK *p_link;
	T_SPP_CHANN_DB *p_chann = NULL;
	switch (event_type) {
	//sdp client(spp server) accept BT_EVENT_SDP_ATTR_INFO from sdp server(spp client)
	case BT_EVENT_SDP_ATTR_INFO: 
	{
		rtk_bt_spp_attr_info_t *p_info = NULL;
		T_BT_SDP_ATTR_INFO *sdp_info = &param->sdp_attr_info.info;
		
		uint8_t temp_local_server_chann = 0;
		//Remote SPP UUID check and get local SPP server channel
		T_BT_SPP_UUID_DATA p_data;
		memcpy((void *)&p_data, &sdp_info->srv_class_uuid_data, sizeof(sdp_info->srv_class_uuid_data));
		if (bt_spp_registered_uuid_check((T_BT_SPP_UUID_TYPE)sdp_info->srv_class_uuid_type,
										&p_data, &temp_local_server_chann)) 
			{
				printf("bt_stack_spp_evt_ind_cback: BT_EVENT_SDP_ATTR_INFO The uuid is registered and the local server channel is bonded\r\n");
				p_chann = bt_stack_find_spp_chann(sdp_info->server_channel);
				if (p_chann == NULL) 
				{
					return ;
				}
				p_chann->is_spp_sdp_ok = true;

				memcpy((void *)client_addr, (void *)param->sdp_attr_info.bd_addr, 6);
				// The local server channel is the same channel with the remote channel.
				p_chann->local_server_chann = temp_local_server_chann;
				p_chann->remote_server_chann = sdp_info->server_channel;
				printf("bt_stack_spp_evt_ind_cback: local server channel is 0x%x, remote server channel is 0x%x\r\n",
						p_chann->local_server_chann, p_chann->remote_server_chann);
				/* Create event*/
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_SPP, RTK_BT_SPP_EVT_SDP_ATTR_INFO, sizeof(rtk_bt_spp_attr_info_t));
				if (!p_evt) {
					printf("bt_stack_spp_evt_ind_cback: evt_t allocate failed \r\n");
					break;
				}
				p_info = (rtk_bt_spp_attr_info_t *)p_evt->data;
				memcpy((void *)p_info->bd_addr, (void *)param->sdp_attr_info.bd_addr, 6);
				p_info->srv_class_uuid_type = (rtk_bt_spp_uuid_type_t)sdp_info->srv_class_uuid_type;
				memcpy((void *)&p_info->srv_class_uuid_data, (void *)&sdp_info->srv_class_uuid_data, sizeof(T_GAP_UUID_DATA));
				p_info->profile_version = sdp_info->profile_version;
				p_info->protocol_version = sdp_info->protocol_version;
				p_info->server_channel = sdp_info->server_channel;
				p_info->supported_feat = sdp_info->supported_feat;
				p_info->l2c_psm = sdp_info->l2c_psm;
				p_info->supported_repos = sdp_info->supported_repos;
				p_info->pbap_supported_feat = sdp_info->pbap_supported_feat;
				/* Send event */
				rtk_bt_evt_indicate(p_evt, NULL);
				break;
			}
		printf("bt_stack_spp_evt_ind_cback: Get sdp attr discov info, but uuid is not registered!!\r\n");
	}
	break;

	case BT_EVENT_SDP_DISCOV_CMPL: 
	{
		rtk_bt_spp_sdp_discov_cmpl_t *p_discov = NULL;

		// Traverse the spp_chann_db[] to find the registered element which satisfy is_ spp_ sdp_ ok == true && is_on_connection == false
		for (int i = 0; i < SPP_CAHNN_DB_MAX_LINK_NUM; i++)
		{
			if (spp_chann_db[i].used == true && spp_chann_db[i].is_spp_sdp_ok == true && spp_chann_db[i].is_on_connection == false)
			{
				p_chann = &spp_chann_db[i];
				if (param->sdp_discov_cmpl.cause == 0x00) 
				{
					printf("bt_stack_spp_evt_ind_cback: BT_EVENT_SDP_DISCOV_CMPL SDP Discovery Completely\r\n");
					if(bt_spp_connect_req(client_addr,p_chann->remote_server_chann,default_mtu_size, 
									default_credits, p_chann->local_server_chann)) 
					{
						printf("bt_stack_spp_evt_ind_cback: BT_EVENT_SDP_DISCOV_CMPL send spp connection request success\r\n");
						/* create event */
						p_evt = rtk_bt_event_create(RTK_BT_BR_GP_SPP,RTK_BT_SPP_EVT_SDP_DISCOV_CMPL, sizeof(rtk_bt_spp_sdp_discov_cmpl_t));
						if (!p_evt) 
						{
							printf("bt_stack_spp_evt_ind_cback: evt_t allocate failed \r\n");
							break;
						}
						p_discov = (rtk_bt_spp_sdp_discov_cmpl_t *)p_evt->data;
						memcpy((void *)p_discov->bd_addr, (void *)param->sdp_discov_cmpl.bd_addr, 6);
						p_discov->cause = param->sdp_discov_cmpl.cause;
						/* Send event */
						rtk_bt_evt_indicate(p_evt, NULL);
					}
				}
			}
		}
	}
	break;
	
	case BT_EVENT_SPP_CONN_IND: 
	{
		rtk_bt_spp_conn_ind_t *p_conn_ind = NULL;
		p_chann = bt_stack_find_spp_chann(param->spp_conn_ind.local_server_chann);
		if (p_chann == NULL)
		{
			printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_CONN_IND wrong local server channel\r\n");
			return ;
		}

		p_link = app_find_br_link(param->spp_conn_ind.bd_addr);

		if (!p_link) {
			printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_CONN_IND no acl link found\r\n");
			return ;
		}

		//printf("bt_stack_spp_evt_ind_cback: SPP Connection Indication\r\n");

		uint8_t local_server_chann = param->spp_conn_ind.local_server_chann;
		uint16_t frame_size = param->spp_conn_ind.frame_size;
		/* Send spp connect confirmation */
		if (false == bt_spp_connect_cfm(p_link->bd_addr, local_server_chann, true, frame_size, default_credits)) 
		{
			printf("bt_stack_spp_evt_ind_cback: BT_EVENT_SPP_CONN_IND send spp connection confirm failed\r\n");
			return ;
		}
		//printf("bt_stack_spp_evt_ind_cback: BT_EVENT_SPP_CONN_IND send spp connection confirm success\r\n");

		/* Create event */
		p_evt = rtk_bt_event_create(RTK_BT_BR_GP_SPP, RTK_BT_SPP_EVT_CONN_IND, sizeof(rtk_bt_spp_conn_ind_t));
		if (!p_evt) {
			printf("bt_spp_stack_evt_ind_caback: evt_t allocate failed\r\n");
			break;
		}
		p_conn_ind = (rtk_bt_spp_conn_ind_t *)p_evt->data;
		memcpy((void *)p_conn_ind->bd_addr, (void *)param->spp_conn_ind.bd_addr, 6);
		p_conn_ind->local_server_chann = param->spp_conn_ind.local_server_chann;
		p_conn_ind->frame_size = param->spp_conn_ind.frame_size;
		
		/* Send event */
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;

	case BT_EVENT_SPP_CONN_CMPL: 
	{
		rtk_bt_spp_conn_cmpl_t *p_conn_cmpl = NULL;

		p_chann = bt_stack_find_spp_chann(param->spp_conn_cmpl.local_server_chann);
		if (p_chann == NULL)
		{
			printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_CONN_CMPL wrong local server channel\r\n");
			return ;
		}

		// This means that the spp connection has been established and the related channel is occupied.
		p_chann->is_on_connection = true;

		p_link = app_find_br_link(param->spp_conn_cmpl.bd_addr);
		if (!p_link) {
			printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_CONN_CMPL no acl link found\r\n");
			return ;
		}
		p_link->rfc_spp_frame_size = param->spp_conn_cmpl.frame_size;
		p_link->rfc_spp_credit = param->spp_conn_cmpl.link_credit;

		//printf("bt_stack_spp_evt_ind_cback: BT_EVENT_SPP_CONN_CMPL SPP Connection Completed\r\n");

		/* Create event */
		p_evt = rtk_bt_event_create(RTK_BT_BR_GP_SPP, RTK_BT_SPP_EVT_CONN_COMPL, sizeof(rtk_bt_spp_conn_cmpl_t));
		if (!p_evt) {
			printf("bt_spp_stack_evt_ind_caback: evt_t allocate failed\r\n");
			break;
		}
		p_conn_cmpl = (rtk_bt_spp_conn_cmpl_t *)p_evt->data;
		memcpy((void *)p_conn_cmpl->bd_addr, (void *)param->spp_conn_cmpl.bd_addr, 6);
		p_conn_cmpl->local_server_chann = param->spp_conn_cmpl.local_server_chann;
		p_conn_cmpl->link_credit = param->spp_conn_cmpl.link_credit;
		p_conn_cmpl->frame_size = param->spp_conn_cmpl.frame_size;
		/* Send event */
		rtk_bt_evt_indicate(p_evt, NULL);
		//printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_CONN_CMPL evt indicate success!!\r\n");   
	}
	break;

	case BT_EVENT_SPP_CREDIT_RCVD: 
	{
		rtk_bt_spp_credit_rcvd_t *p_credit_rcvd = NULL;

		p_chann = bt_stack_find_spp_chann(param->spp_credit_rcvd.local_server_chann);
		if (p_chann == NULL)
		{
			printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_CREDIT_RCVD wrong local server channel\r\n");
			return ;
		}

		p_link = app_find_br_link(param->spp_credit_rcvd.bd_addr);
		if (!p_link) {
			printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_CREDIT_RCVD no acl link found\r\n");
			return ;
		}
		/* = or += ?*/
		p_link->rfc_spp_credit = param->spp_credit_rcvd.link_credit;
		//printf("bt_stack_spp_evt_ind_cback: BT_EVENT_SPP_CREDIT_RCVD SPP %d Credits received\r\n", param->spp_credit_rcvd.link_credit);
		//printf("bt_stack_spp_evt_ind_cback: BT_EVENT_SPP_CREDIT_RCVD link Credits now is %d\r\n", p_link->rfc_spp_credit);

		/* Create Event*/
		p_evt = rtk_bt_event_create(RTK_BT_BR_GP_SPP, RTK_BT_SPP_EVT_CREDIT_RCVD, sizeof(rtk_bt_spp_credit_rcvd_t));
		if (!p_evt) {
			printf("bt_spp_stack_evt_ind_caback: evt_t allocate failed\r\n");
			break;
		}
		p_credit_rcvd = (rtk_bt_spp_credit_rcvd_t *)p_evt->data;
		memcpy((void *)p_credit_rcvd->bd_addr, (void *)param->spp_credit_rcvd.bd_addr, 6);
		p_credit_rcvd->local_server_chann = param->spp_credit_rcvd.local_server_chann;
		p_credit_rcvd->link_credit = param->spp_credit_rcvd.link_credit;
		/* Send event */
		rtk_bt_evt_indicate(p_evt, NULL);
		//printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_CREDIT_RCVD evt indicate success!!\r\n");
	}
	break;

	case BT_EVENT_SPP_DATA_IND: 
	{
		rtk_bt_spp_data_ind_t *p_data_ind = NULL;

		p_chann = bt_stack_find_spp_chann(param->spp_data_ind.local_server_chann);
		if (p_chann == NULL)
		{
			printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_DATA_IND wrong local server channel\r\n");
			return ;
		}

		p_link = app_find_br_link(param->spp_data_ind.bd_addr);
		if (!p_link) {
			printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_DATA_IND no acl link found\r\n");
			return ;
		}

		//printf("bt_stack_spp_evt_ind_cback: BT_EVENT_SPP_DATA_IND SPP Data Indication\r\n");

		/* Create event */
		p_evt = rtk_bt_event_create(RTK_BT_BR_GP_SPP, RTK_BT_SPP_EVT_DATA_IND, sizeof(rtk_bt_spp_data_ind_t));
		if (!p_evt) {
			printf("bt_spp_stack_evt_ind_caback: evt_t allocate failed\r\n");
			break;
		}

		//printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_DATA_IND server_chann = %x\r\n", param->spp_data_ind.local_server_chann);
		p_data_ind = (rtk_bt_spp_data_ind_t *)p_evt->data;
		memcpy((void *)p_data_ind->bd_addr, (void *)param->spp_data_ind.bd_addr, 6);
		p_data_ind->local_server_chann = param->spp_data_ind.local_server_chann;
		memcpy((void *)p_data_ind->data, (void *)param->spp_data_ind.data, param->spp_data_ind.len);
		p_data_ind->len = param->spp_data_ind.len;
		/* Send event */
		if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
			break;
		}
		//printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_DATA_IND evt indicate success!!\r\n");

		p_link = app_find_br_link(param->spp_credit_rcvd.bd_addr);
		if (!p_link) {
			printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_CREDIT_RCVD no acl link found\r\n");
			return ;
		}
	}
	break;

	// case BT_EVENT_SPP_DATA_RSP: 
	// {
	//     (rtk_bt_spp_data_rsp_t *)p_data_rsp = NULL;
	//     printf("bt_stack_spp_evt_ind_cback: SPP Data Response with ack flag set\r\n");
	//     p_evt = rtk_bt_event_create(RTK_BT_BR_GP_SPP,RTK_BT_SPP_EVT_DATA_RSP, sizeof(rtk_bt_spp_data_rsp_t));
	//     if (!p_evt) {
	//         printf("bt_stack_spp_evt_ind_cback: evt_t allocate failed\r\n");
	//         break;
	//     }
	//     p_data_rsp = (rtk_bt_spp_data_rsp_t *)p_evt->data;
	//     memcpy((void *)p_data_rsp->bd_addr, (void *)param->spp_data_rsp.bd_addr, 6);
	//     p_data_rsp->local_server_chann = param->spp_data_rsp.local_server_chann;
	//     /* Send event */
	//     rtk_bt_evt_indicate(p_evt, NULL);
	// }
	// break;

	case BT_EVENT_SPP_DISCONN_CMPL: 
	{
		rtk_bt_spp_disconn_ind_t *p_disconn_ind = NULL;

		p_chann = bt_stack_find_spp_chann(param->spp_disconn_cmpl.local_server_chann);
		if (p_chann == NULL)
		{
			printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_DISCONN_CMPL wrong local server channel\r\n");
			return ;
		}

		// If the related spp connection disconnect, set this flag false.
		p_chann->is_on_connection = false;

		p_link = app_find_br_link((param->spp_disconn_cmpl.bd_addr));
		if (!p_link) {
			printf("bt_spp_stack_evt_ind_caback: BT_EVENT_SPP_DISCONN_CMPL no acl link found\r\n");
			return ;
		}

		/* Create event */
		p_evt = rtk_bt_event_create(RTK_BT_BR_GP_SPP, RTK_BT_SPP_EVT_DISCONN_CMPL, sizeof(rtk_bt_spp_disconn_ind_t));
		if (!p_evt) {
			printf("bt_stack_spp_evt_ind_cback: evt_t allocate failed \r\n");
			break;
		}
		p_disconn_ind = (rtk_bt_spp_disconn_ind_t *)p_evt->data;
		memcpy((void*)p_disconn_ind->bd_addr, (void*)param->spp_disconn_cmpl.bd_addr, 6);
		p_disconn_ind->local_server_chann = param->spp_disconn_cmpl.local_server_chann;
		p_disconn_ind->cause = param->spp_disconn_cmpl.cause;
		/* Send event */
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;

	default:{
		//printf("bt_stack_spp_evt_ind_cback:default event_type 0x%04x\r\n", event_type);
	}
	break;
	}
}

static uint16_t bt_stack_spp_connect(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	T_GAP_UUID_DATA uuid;
	T_GAP_UUID_TYPE uuid_type = GAP_UUID16;
	uuid.uuid_16 = UUID_RFCOMM;

	if (gap_br_stop_inquiry() != GAP_CAUSE_SUCCESS) {
		printf("[bt_stack_spp_connect] SPP server stop inquiry failed!\r\n");
		//return RTK_BT_FAIL;
	} else {
		printf("[bt_stack_spp_connect] SPP server stop inquiry success! \r\n");
	}
	
	if(gap_br_start_sdp_discov(bd_addr, uuid_type, uuid) == GAP_CAUSE_SUCCESS) {
		printf("[bt_stack_spp_connect] SPP server start sdp discov success!\r\n");
		return RTK_BT_OK;
	}
	printf("[bt_stack_spp_connect] SPP server start sdp discov failed!\r\n");
	return RTK_BT_FAIL;
}

static uint16_t bt_stack_spp_disconnect(void *param)
{
	rtk_bt_spp_disconn_req_t *p_disconn_req_t = (rtk_bt_spp_disconn_req_t *)param;
	T_APP_BR_LINK *p_link;

	p_link = app_find_br_link(p_disconn_req_t->bd_addr);
	if (p_link != NULL) {
		if (bt_spp_disconnect_req(p_disconn_req_t->bd_addr, p_disconn_req_t->local_server_chann)) {
			return RTK_BT_OK;
		}
	}
	return RTK_BT_FAIL;
}

static uint16_t bt_stack_spp_disconnect_all(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	T_APP_BR_LINK *p_link;

	p_link = app_find_br_link(bd_addr);
	if (p_link != NULL) {
		if (bt_spp_disconnect_all_req(bd_addr)) {
			return RTK_BT_OK;
		}
	}
	printf("bt_stack_spp_disconnect_all failed: no link found\r\n");
	return RTK_BT_FAIL;
}

static uint16_t bt_stack_spp_send_data(void *param)
{
	rtk_bt_spp_send_data_t *p_send_data_t = (rtk_bt_spp_send_data_t *)param;
	T_APP_BR_LINK *p_link;

	p_link = app_find_br_link(p_send_data_t->bd_addr);
	if (!p_link) {
		printf("bt_stack_spp_send_data failed: no link found\r\n");
		return RTK_BT_FAIL;
	}
	
	if (p_link->rfc_spp_credit) {
		if (bt_spp_data_send(p_send_data_t->bd_addr,
								p_send_data_t->local_server_chann,
								p_send_data_t->data,
								p_send_data_t->len,
								false)) 
			{
				p_link->rfc_spp_credit--;
				printf("bt_stack_spp_send_data: link credit is %d\r\n",p_link->rfc_spp_credit);
				return RTK_BT_OK;
			}
		printf("bt_stack_spp_send_data failed: bt_spp_data_send() failed\r\n");
	} else {
		printf("bt_stack_spp_send_data failed: have no rfc_spp_credit\r\n");
	}
	
	return RTK_BT_FAIL;
}

static uint16_t bt_stack_spp_give_credits(void *param)
{
	rtk_bt_spp_credits_give_t *p_credits_give = (rtk_bt_spp_credits_give_t *)param;
	T_APP_BR_LINK *p_link;

	p_link = app_find_br_link(p_credits_give->bd_addr);
	if (p_link != NULL) {
		if (bt_spp_credits_give(p_credits_give->bd_addr, p_credits_give->local_server_chann, 
								p_credits_give->credits)) 
		{
			//printf("bt_stack_spp_give_credits: bt_spp_credits_give() success\r\n");
			return RTK_BT_OK;
		}
		printf("bt_stack_spp_give_credits: bt_spp_credits_give() failed\r\n");
	} else {
		printf("bt_stack_spp_give_credits failed: no link found\r\n");
	}

	return RTK_BT_FAIL;
}

uint16_t bt_stack_spp_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = 0;
	API_PRINT("bt_stack_spp_act_handle: act = %d \r\n", p_cmd->act);
	switch(p_cmd->act)
	{
		case RTK_BT_SPP_ACT_CONNECT:
			ret = bt_stack_spp_connect(p_cmd->param);
			break;

		case RTK_BT_SPP_ACT_DISCONNECT:
			ret = bt_stack_spp_disconnect(p_cmd->param);
			break;

		case RTK_BT_SPP_ACT_DISCONNECT_ALL:
			ret = bt_stack_spp_disconnect_all(p_cmd->param);
			break;

		case RTK_BT_SPP_ACT_SEND_DATA:
			ret = bt_stack_spp_send_data(p_cmd->param);
			break;

		case RTK_BT_SPP_ACT_GIVE_CREDITS:
			ret = bt_stack_spp_give_credits(p_cmd->param);
			break;

		default:
			printf("bt_stack_spp_act_handle: unknown act: %d \r\n", p_cmd->act);
			ret = 0;
			break;
	}

	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);

	return ret;
}

uint16_t bt_stack_spp_init(uint8_t role)
{
	(void)role;

	/* SPP init */
	printf("bt_stack_spp_init\r\n");
	bt_spp_init(spp_max_link_num, spp_rfc_max_service_num);

	/* Register spp service with uuid, and bond it with allocated spp server channel */ 
	for (int i = 0; i < SPP_CAHNN_DB_MAX_LINK_NUM; i++)
	{
		if (spp_chann_db[i].used == true) 
		{
			bt_spp_service_register(spp_chann_db[i].service_uuid.uuid_128, spp_chann_db[i].spp_chann_num);
		}
	}

	/* Register event callback function*/ 
	bt_mgr_cback_register(bt_stack_spp_evt_ind_cback);

	/* Disable spp L2CAP ERTM mode by default */
	bt_spp_ertm_mode_set(false);

	return RTK_BT_OK;
}

extern void spp_deinit(void);

void bt_stack_spp_deinit(void) 
{
	/* SPP deinit */
	printf("spp_demo_deinit\r\n");
	spp_deinit();
}

#endif