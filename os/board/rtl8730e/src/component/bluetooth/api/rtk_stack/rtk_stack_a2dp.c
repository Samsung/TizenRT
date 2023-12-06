/*
*******************************************************************************
* Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

/* -------------------------------- Includes -------------------------------- */
#include <bt_api_config.h>
#if defined(RTK_BREDR_SUPPORT) && RTK_BREDR_SUPPORT
#include <rtk_bt_le_gap.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <stdio.h>
#include <string.h>
#include <dlist.h>
#include <osif.h>
#include <rtk_bt_br_gap.h>
#include <rtk_bt_a2dp.h>
#include <gap_br.h>
#include <btm.h>
#include <trace_app.h>
#include <remote.h>
#include <bt_a2dp.h>

rtk_bt_a2dp_media_codec_sbc_t rtk_codec_sbc = {0};
rtk_bt_a2dp_media_codec_aac_t rtk_codec_aac = {0};
static uint8_t a2dp_role;
static uint8_t remote_addr[6] = {0};
extern T_APP_DB app_db;
extern T_APP_BR_LINK *app_find_br_link(uint8_t *bd_addr);

static void app_a2dp_src_set_stream_status(T_APP_BR_LINK *p_link, bool streaming)
{
	if (p_link->is_streaming == true && streaming == false)
	{
		bt_avrcp_play_status_change_req(p_link->bd_addr, BT_AVRCP_PLAY_STATUS_PAUSED);
	}
	else if (p_link->is_streaming == false && streaming == true)
	{
		bt_avrcp_play_status_change_req(p_link->bd_addr, BT_AVRCP_PLAY_STATUS_PLAYING);
	}

	p_link->is_streaming = streaming;
}

static void app_a2dp_bt_cback(T_BT_EVENT event_type, void *event_buf, uint16_t buf_len)
{
	(void)buf_len;
	T_BT_EVENT_PARAM *param = event_buf;
	T_APP_BR_LINK *p_link;
	bool handle = true;
	rtk_bt_evt_t *p_evt = NULL;

	switch (event_type) {

	/* source role */
	case BT_EVENT_SDP_ATTR_INFO: {
		rtk_bt_a2dp_sdp_attr_info_t *p_info = NULL;
		T_BT_SDP_ATTR_INFO *sdp_info = &param->sdp_attr_info.info;
		bt_a2dp_connect_req(param->sdp_attr_info.bd_addr, sdp_info->protocol_version);
		{
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_A2DP,RTK_BT_A2DP_EVT_SDP_ATTR_INFO,sizeof(rtk_bt_a2dp_sdp_attr_info_t));
			if (!p_evt) {
				printf("app_a2dp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_info = (rtk_bt_a2dp_sdp_attr_info_t *)p_evt->data;
			memcpy((void *)&p_info->bd_addr, (void *)&param->sdp_attr_info.bd_addr, 6);
			p_info->srv_class_uuid_type = (br_gap_uuid_type_t)sdp_info->srv_class_uuid_type;
			memcpy((void *)&p_info->srv_class_uuid_data, (void *)&sdp_info->srv_class_uuid_data, sizeof(T_GAP_UUID_DATA));
			p_info->profile_version = sdp_info->profile_version;
			p_info->protocol_version = sdp_info->protocol_version;
			p_info->server_channel = sdp_info->server_channel;
			p_info->supported_feat = sdp_info->supported_feat;
			p_info->l2c_psm = sdp_info->l2c_psm;
			p_info->supported_repos = sdp_info->supported_repos;
			p_info->pbap_supported_feat = sdp_info->pbap_supported_feat;
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;
	
	case BT_EVENT_A2DP_CONN_IND: {
		rtk_bt_a2dp_conn_ind_t *p_a2dp_conn_ind = NULL;

		p_link = app_find_br_link(param->a2dp_conn_ind.bd_addr);
		if (p_link != NULL) {
			bt_a2dp_connect_cfm(p_link->bd_addr, true);
			APP_PRINT_INFO0("A2DP p_link confirmed");
			printf("app_a2dp_bt_cback: A2DP p_link confirmed \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_A2DP,RTK_BT_A2DP_EVT_CONN_IND,sizeof(rtk_bt_a2dp_conn_ind_t));
				if (!p_evt) {
					printf("app_a2dp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_a2dp_conn_ind = (rtk_bt_a2dp_conn_ind_t *)p_evt->data;
				memcpy((void *)p_a2dp_conn_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("A2DP p_link is NULL");
			printf("app_a2dp_bt_cback: A2DP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_A2DP_CONN_CMPL: {
		rtk_bt_a2dp_conn_ind_t *p_a2dp_conn_ind = NULL;

		printf("app_a2dp_bt_cback: A2DP Connected \r\n");
		p_link = app_find_br_link(param->a2dp_conn_cmpl.bd_addr);
		if (p_link != NULL) {
			memcpy((void *)remote_addr, (void *)param->a2dp_conn_cmpl.bd_addr, 6);
			if (a2dp_role == BT_A2DP_ROLE_SRC) {
				/* requeset avrcp connection */
				bt_avrcp_connect_req(param->a2dp_conn_cmpl.bd_addr);
			}
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_A2DP,RTK_BT_A2DP_EVT_CONN_CMPL,sizeof(rtk_bt_a2dp_conn_ind_t));
				if (!p_evt) {
					printf("app_a2dp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_a2dp_conn_ind = (rtk_bt_a2dp_conn_ind_t *)p_evt->data;
				memcpy((void *)p_a2dp_conn_ind->bd_addr, (void *)param->a2dp_conn_cmpl.bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			printf("app_a2dp_bt_cback: no acl link found \r\n");
		}
	}
	break;

	case BT_EVENT_A2DP_DISCONN_CMPL: {
		rtk_bt_a2dp_disconn_ind_t *p_a2dp_disconn_ind = NULL;
		{
			printf("app_a2dp_bt_cback: A2DP Disconnected \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_A2DP,RTK_BT_A2DP_EVT_DISCONN_CMPL,sizeof(rtk_bt_a2dp_disconn_ind_t));
				if (!p_evt) {
					printf("app_a2dp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_a2dp_disconn_ind = (rtk_bt_a2dp_disconn_ind_t *)p_evt->data;
				memcpy((void *)p_a2dp_disconn_ind->bd_addr, (void *)param->a2dp_disconn_cmpl.bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		}
	}
	break;

	case BT_EVENT_A2DP_STREAM_OPEN: {
		APP_PRINT_INFO0("A2DP STREAM is opened ");
		printf("app_a2dp_bt_cback: A2DP STREAM is opened \r\n");
		{
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_A2DP,RTK_BT_A2DP_EVT_STREAM_OPEN,0);
			if (!p_evt) {
				printf("app_a2dp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_A2DP_STREAM_START_IND: {
		rtk_bt_a2dp_stream_start_t *p_a2dp_stream_start = NULL;

		if (a2dp_role != BT_A2DP_ROLE_SNK) {
			printf("app_a2dp_bt_cback: BT_EVENT_A2DP_STREAM_START_IND Wrong A2DP Role ! \r\n");
			break;
		}
		p_link = app_find_br_link(param->a2dp_stream_start_ind.bd_addr);
		if (!p_link) {
			printf("app_a2dp_bt_cback: BT_EVENT_A2DP_STREAM_START_IND no link found \r\n");
			break;
		}
		if (p_link->streaming_fg == false ||
				p_link->avrcp_play_status != BT_AVRCP_PLAY_STATUS_PLAYING) {
			bt_a2dp_stream_start_cfm(param->a2dp_stream_start_ind.bd_addr, true);
			p_link->streaming_fg = true;
			APP_PRINT_INFO2("app_a2dp_bt_cback: BT_EVENT_A2DP_STREAM_START_IND active_a2dp_idx %d, streaming_fg %d",
							0, p_link->streaming_fg);
			printf("app_a2dp_bt_cback: BT_EVENT_A2DP_STREAM_START_IND active_a2dp_idx %d, streaming_fg %d \r\n",
							0, p_link->streaming_fg);
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_A2DP,RTK_BT_A2DP_EVT_STREAM_START_IND,sizeof(rtk_bt_a2dp_stream_start_t));
				if (!p_evt) {
					printf("app_a2dp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_a2dp_stream_start = (rtk_bt_a2dp_stream_start_t *)p_evt->data;
				p_a2dp_stream_start->active_a2dp_link_index = 0;
				p_a2dp_stream_start->stream_cfg = p_link->streaming_fg;
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		}
	}
	break;

	case BT_EVENT_A2DP_STREAM_START_RSP: {
		APP_PRINT_INFO0("A2DP STREAM START RSP ");
		rtk_bt_a2dp_stream_start_t *p_a2dp_stream_start = NULL;

		if (a2dp_role != BT_A2DP_ROLE_SRC) {
			printf("app_a2dp_bt_cback: BT_EVENT_A2DP_STREAM_START_RSP Wrong A2DP Role ! \r\n");
			break;
		}
		p_link = app_find_br_link(param->a2dp_stream_start_rsp.bd_addr);
		if (!p_link) {
			printf("app_a2dp_bt_cback: BT_EVENT_A2DP_STREAM_START_RSP no link found \r\n");
			break;
		}
		app_a2dp_src_set_stream_status(p_link, true);
		{
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_A2DP,RTK_BT_A2DP_EVT_STREAM_START_RSP,sizeof(rtk_bt_a2dp_stream_start_t));
			if (!p_evt) {
				printf("app_a2dp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_a2dp_stream_start = (rtk_bt_a2dp_stream_start_t *)p_evt->data;
			p_a2dp_stream_start->active_a2dp_link_index = 0;
			p_a2dp_stream_start->stream_cfg = p_link->streaming_fg;
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_A2DP_STREAM_DATA_IND: {
		rtk_bt_a2dp_stream_data_ind_t *p_a2dp_stream_data = NULL;

		// APP_PRINT_INFO1("stream data len %d", param->a2dp_stream_data_ind.len);
		// printf("app_a2dp_bt_cback: stream data len %d \r\n", param->a2dp_stream_data_ind.len);
		// {
		// 	int i = 0;                             
		// 	printf(" Dump KM4 \n\r ");  
		// 	while(i < a2dp_data_stream.length) {                  
		// 		printf("%02x ", a2dp_data_stream.data[i++]);        
		// 		if (i%16 == 0)                     
		// 			printf("\n\r");                
		// 	}                                      
		// 	printf("\n\r");                        
		// }
		{
			uint8_t cb_ret = 0;
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_A2DP,RTK_BT_A2DP_EVT_STREAM_DATA_IND,sizeof(rtk_bt_a2dp_stream_data_ind_t));
			if (!p_evt) {
				printf("app_a2dp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_a2dp_stream_data = (rtk_bt_a2dp_stream_data_ind_t *)p_evt->data;
			memcpy((void *)p_a2dp_stream_data->data, (void *)param->a2dp_stream_data_ind.payload, param->a2dp_stream_data_ind.len);
			p_a2dp_stream_data->length = param->a2dp_stream_data_ind.len;
			p_a2dp_stream_data->seq_num = param->a2dp_stream_data_ind.seq_num;
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, &cb_ret)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_A2DP_STREAM_DATA_RSP: {
		rtk_bt_a2dp_stream_data_rsp_t *p_a2dp_stream_data_rsp = NULL;
		APP_PRINT_INFO0("BT_EVENT_A2DP_STREAM_DATA_RSP ");
		if (a2dp_role == BT_A2DP_ROLE_SRC) {
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_A2DP,RTK_BT_A2DP_EVT_STREAM_DATA_RSP,sizeof(rtk_bt_a2dp_stream_data_rsp_t));
			if (!p_evt) {
				printf("app_a2dp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_a2dp_stream_data_rsp = (rtk_bt_a2dp_stream_data_rsp_t *)p_evt->data;
			memcpy((void *)p_a2dp_stream_data_rsp->bd_addr, (void *)param->a2dp_stream_data_rsp.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
			APP_PRINT_INFO0("BT_EVENT_A2DP_STREAM_DATA_RSP DONE");
		}
	}
	break;

	case BT_EVENT_A2DP_STREAM_STOP: {
		rtk_bt_a2dp_conn_ind_t *p_a2dp_conn_ind = NULL;

		p_link = app_find_br_link(param->a2dp_stream_stop.bd_addr);
		if (!p_link) {
			printf("app_a2dp_bt_cback: BT_EVENT_A2DP_STREAM_STOP no link found \r\n");
			break;
		}
		APP_PRINT_INFO0("BT_EVENT_A2DP_STREAM_STOP");
		printf("app_a2dp_bt_cback: BT_EVENT_A2DP_STREAM_STOP \r\n");
		p_link->streaming_fg = false;
		if (a2dp_role == BT_A2DP_ROLE_SRC) {
			app_a2dp_src_set_stream_status(p_link, false);
		}
		{
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_A2DP,RTK_BT_A2DP_EVT_STREAM_STOP,sizeof(rtk_bt_a2dp_conn_ind_t));
			if (!p_evt) {
				printf("app_a2dp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_a2dp_conn_ind = (rtk_bt_a2dp_conn_ind_t *)p_evt->data;
			memcpy((void *)p_a2dp_conn_ind->bd_addr, (void *)param->a2dp_stream_stop.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_A2DP_STREAM_CLOSE: {
		rtk_bt_a2dp_conn_ind_t *p_a2dp_conn_ind = NULL;

		p_link = app_find_br_link(param->a2dp_stream_close.bd_addr);
		if (!p_link) {
			printf("app_a2dp_bt_cback: BT_EVENT_A2DP_STREAM_CLOSE no link found \r\n");
			break;
		}
		APP_PRINT_INFO0("BT_EVENT_A2DP_STREAM_CLOSE");
		printf("app_a2dp_bt_cback: BT_EVENT_A2DP_STREAM_CLOSE \r\n");
		p_link->streaming_fg = false;
		{
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_A2DP,RTK_BT_A2DP_EVT_STREAM_CLOSE,sizeof(rtk_bt_a2dp_conn_ind_t));
			if (!p_evt) {
				printf("app_a2dp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_a2dp_conn_ind = (rtk_bt_a2dp_conn_ind_t *)p_evt->data;
			memcpy((void *)p_a2dp_conn_ind->bd_addr, (void *)param->a2dp_stream_stop.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_A2DP_CONFIG_CMPL: {
		rtk_bt_a2dp_codec_t *p_a2dp_codec = NULL;

		p_link = app_find_br_link(param->a2dp_config_cmpl.bd_addr);
		if (p_link != NULL) {
			T_BT_EVENT_PARAM_A2DP_CONFIG_CMPL *cfg = &param->a2dp_config_cmpl;
			p_link->a2dp_codec_type = param->a2dp_config_cmpl.codec_type;

			if (p_link->a2dp_codec_type == BT_A2DP_CODEC_TYPE_SBC) {
				APP_PRINT_INFO0("A2DP config: SBC ");
				printf("app_a2dp_bt_cback: A2DP config: SBC \r\n");
				p_link->a2dp_codec_info.sbc.sampling_frequency = cfg->codec_info.sbc.sampling_frequency;
				p_link->a2dp_codec_info.sbc.channel_mode = cfg->codec_info.sbc.channel_mode;
				p_link->a2dp_codec_info.sbc.block_length = cfg->codec_info.sbc.block_length;
				p_link->a2dp_codec_info.sbc.subbands = cfg->codec_info.sbc.subbands;
				p_link->a2dp_codec_info.sbc.allocation_method = cfg->codec_info.sbc.allocation_method;
				p_link->a2dp_codec_info.sbc.min_bitpool = cfg->codec_info.sbc.min_bitpool;
				p_link->a2dp_codec_info.sbc.max_bitpool = cfg->codec_info.sbc.max_bitpool;
			} else if (p_link->a2dp_codec_type == BT_A2DP_CODEC_TYPE_AAC) {
				APP_PRINT_INFO0("A2DP config: AAC ");
				printf("app_a2dp_bt_cback: A2DP config: AAC \r\n");
				p_link->a2dp_codec_info.aac.object_type = cfg->codec_info.aac.object_type;
				p_link->a2dp_codec_info.aac.sampling_frequency = cfg->codec_info.aac.sampling_frequency;
				p_link->a2dp_codec_info.aac.channel_number = cfg->codec_info.aac.channel_number;
				p_link->a2dp_codec_info.aac.vbr_supported = cfg->codec_info.aac.vbr_supported;
				p_link->a2dp_codec_info.aac.bit_rate = cfg->codec_info.aac.bit_rate;
			} else {
				APP_PRINT_INFO0("A2DP config: Vendor ");
				printf("app_a2dp_bt_cback: A2DP config: Vendor \r\n");
				memcpy(p_link->a2dp_codec_info.vendor.info, cfg->codec_info.vendor.info, 12);
			}
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_A2DP,RTK_BT_A2DP_EVT_CONFIG_CMPL,sizeof(rtk_bt_a2dp_codec_t));
				if (!p_evt) {
					printf("app_a2dp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_a2dp_codec = (rtk_bt_a2dp_codec_t *)p_evt->data;
				p_a2dp_codec->codec_type = p_link->a2dp_codec_type;
				if (p_a2dp_codec->codec_type == BT_A2DP_CODEC_TYPE_SBC) {
					p_a2dp_codec->sbc.sampling_frequency = p_link->a2dp_codec_info.sbc.sampling_frequency;
					p_a2dp_codec->sbc.channel_mode = p_link->a2dp_codec_info.sbc.channel_mode;
					p_a2dp_codec->sbc.block_length = p_link->a2dp_codec_info.sbc.block_length;
					p_a2dp_codec->sbc.subbands = p_link->a2dp_codec_info.sbc.subbands;
					p_a2dp_codec->sbc.allocation_method = p_link->a2dp_codec_info.sbc.allocation_method;
					p_a2dp_codec->sbc.min_bitpool = p_link->a2dp_codec_info.sbc.min_bitpool;
					p_a2dp_codec->sbc.max_bitpool = p_link->a2dp_codec_info.sbc.max_bitpool;
					p_a2dp_codec->codec_type = RTK_BT_AUDIO_CODEC_SBC;
				} else if (p_a2dp_codec->codec_type == BT_A2DP_CODEC_TYPE_AAC) {
					p_a2dp_codec->aac.object_type = p_link->a2dp_codec_info.aac.object_type;
					p_a2dp_codec->aac.sampling_frequency = p_link->a2dp_codec_info.aac.sampling_frequency;
					p_a2dp_codec->aac.channel_number = p_link->a2dp_codec_info.aac.channel_number;
					p_a2dp_codec->aac.vbr_supported = p_link->a2dp_codec_info.aac.vbr_supported;
					p_a2dp_codec->aac.bit_rate = p_link->a2dp_codec_info.aac.bit_rate;
					p_a2dp_codec->codec_type = RTK_BT_AUDIO_CODEC_AAC;
				} else {
					printf("app_a2dp_bt_cback: not support \r\n");
				}
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		}
	}
	break;

	default: {
		APP_PRINT_INFO1("app_a2dp_bt_cback: default event_type 0x%04x", event_type);
		// printf("app_a2dp_bt_cback: default event_type 0x%04x \r\n", event_type);
		handle = false;
	}
	break;
	}

	if (handle == true) {
		// APP_PRINT_INFO1("app_a2dp_bt_cback: event_type 0x%04x", event_type);
		// printf("app_a2dp_bt_cback: event_type 0x%04x \r\n", event_type);
	}
}

static uint16_t bt_stack_a2dp_connect(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	T_GAP_UUID_DATA uuid;

	if (a2dp_role == BT_A2DP_ROLE_SRC) {
		uuid.uuid_16 = UUID_AUDIO_SINK;
	} else {
		uuid.uuid_16 = UUID_AUDIO_SOURCE;
	}

	if (gap_br_start_sdp_discov(bd_addr, GAP_UUID16, uuid) == GAP_CAUSE_SUCCESS) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_a2dp_disconnect(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	T_APP_BR_LINK *p_link;

	p_link = app_find_br_link(bd_addr);
	if (p_link != NULL) {
		if (bt_a2dp_disconnect_req(bd_addr)) {
			return RTK_BT_OK;
		}
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_a2dp_start(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	T_APP_BR_LINK *p_link;

	p_link = app_find_br_link(bd_addr);
	if (p_link != NULL) {
		if (bt_a2dp_stream_start_req(bd_addr)) {
			return RTK_BT_OK;
		}
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_a2dp_suspend(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	T_APP_BR_LINK *p_link;

	p_link = app_find_br_link(bd_addr);
	if (p_link != NULL) {
		if (bt_a2dp_stream_suspend_req(bd_addr)) {
			return RTK_BT_OK;
		}
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_a2dp_stream_data_send(void *param)
{
	rtk_bt_a2dp_stream_data_send_t *p_data_send_t = (rtk_bt_a2dp_stream_data_send_t *)param;
	T_APP_BR_LINK *p_link;
	uint8_t a2dp_send_retry_count = 5;

	p_link = app_find_br_link(p_data_send_t->bd_addr);
	if (!p_link) {
		printf("app_a2dp_bt_cback: bt_stack_a2dp_stream_data_send no link found \r\n");
		return RTK_BT_FAIL;
	}
	do {
		if (bt_a2dp_stream_data_send(p_data_send_t->bd_addr, 
								p_data_send_t->frame_num,
								p_data_send_t->frame_buf,
								p_data_send_t->len)) {
			return RTK_BT_OK;
		}
		osif_delay(1);
	} while(a2dp_send_retry_count --); //increase retry count bt framework buffer is not enough
	APP_PRINT_INFO0("bt_stack_a2dp_stream_data_send fail");
	printf("bt_stack_a2dp_stream_data_send fail %d %d\r\n", (int)p_data_send_t->frame_num, (int)p_data_send_t->len);

	return RTK_BT_FAIL;
}

uint16_t bt_stack_a2dp_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = 0;
	API_PRINT("bt_stack_a2dp_act_handle: act = %d \r\n", p_cmd->act);
	switch(p_cmd->act){

		case RTK_BT_A2DP_ACT_CONNECT:
			ret = bt_stack_a2dp_connect(p_cmd->param);
			break;

		case RTK_BT_A2DP_ACT_DISCONNECT:
			ret = bt_stack_a2dp_disconnect(p_cmd->param);
			break;

		case RTK_BT_A2DP_ACT_START:
			ret = bt_stack_a2dp_start(p_cmd->param);
			break;

		case RTK_BT_A2DP_ACT_SUSPEND:
			ret = bt_stack_a2dp_suspend(p_cmd->param);
			break;

		case RTK_BT_A2DP_ACT_SEND_DATA:
			ret = bt_stack_a2dp_stream_data_send(p_cmd->param);
			break;

		default:
			printf("bt_stack_a2dp_act_handle: unknown act: %d \r\n", p_cmd->act);
			ret = 0;
			break;
	}

	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);

	return ret;
}

uint16_t bt_stack_a2dp_init(uint8_t role)
{
	printf("[A2DP]app_a2dp_init\n");

	if (!bt_a2dp_init(1, 280)) {
		printf("[A2DP]bt_a2dp_init FAIL\n");
		return RTK_BT_FAIL;
	}
	bt_a2dp_role_set((T_BT_A2DP_ROLE)role);
	{
		T_BT_A2DP_STREAM_ENDPOINT sep;

		sep.codec_type = BT_A2DP_CODEC_TYPE_SBC;
		sep.u.codec_sbc.sampling_frequency_mask = rtk_codec_sbc.sampling_frequency_mask;
		sep.u.codec_sbc.channel_mode_mask = rtk_codec_sbc.channel_mode_mask;
		sep.u.codec_sbc.block_length_mask = rtk_codec_sbc.block_length_mask;
		sep.u.codec_sbc.subbands_mask = rtk_codec_sbc.subbands_mask;
		sep.u.codec_sbc.allocation_method_mask = rtk_codec_sbc.allocation_method_mask;
		sep.u.codec_sbc.min_bitpool = rtk_codec_sbc.min_bitpool;
		sep.u.codec_sbc.max_bitpool = rtk_codec_sbc.max_bitpool;
		if (!bt_a2dp_stream_endpoint_add(sep)) {
			printf("[A2DP]bt_a2dp_stream_endpoint_add FAIL\n");
			return RTK_BT_FAIL;
		}
	}
	a2dp_role = (T_BT_A2DP_ROLE)role;
#if 0
	{
		T_BT_A2DP_STREAM_ENDPOINT sep;

		sep.codec_type = BT_A2DP_CODEC_TYPE_AAC;
		sep.u.codec_aac.object_type_mask = rtk_codec_aac.object_type_mask;
		sep.u.codec_aac.sampling_frequency_mask = rtk_codec_aac.sampling_frequency_mask;
		sep.u.codec_aac.channel_number_mask = rtk_codec_aac.channel_number_mask;
		sep.u.codec_aac.vbr_supported = rtk_codec_aac.vbr_supported;
		sep.u.codec_aac.bit_rate = rtk_codec_aac.bit_rate;
		if (!bt_a2dp_stream_endpoint_add(sep)) {
			printf("[A2DP]bt_a2dp_stream_endpoint_add FAIL\n");
			return RTK_BT_FAIL;
		}
	}
#endif
	bt_mgr_cback_register(app_a2dp_bt_cback);

	return RTK_BT_OK;
}

extern void a2dp_deinit(void);

void bt_stack_a2dp_deinit(void)
{
	printf("[A2DP]app_a2dp_init\n");
	a2dp_deinit();
}

#endif