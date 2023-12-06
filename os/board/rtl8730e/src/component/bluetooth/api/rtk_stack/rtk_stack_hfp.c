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
#include <rtk_bt_hfp.h>
#include <gap_br.h>
#include <btm.h>
#include <trace_app.h>
#include <remote.h>
#include <bt_hfp.h>
#include <bt_hfp_ag.h>

static uint8_t battery_power = RTK_BT_HFP_DEMO_DEFAULT_BATTERY_POWER_LEVEL;
static uint8_t microphone_gain = RTK_BT_DEFAULT_MICROPHONE_GAIN;
static uint8_t speaker_gain = RTK_BT_DEFAULT_SPEAKER_GAIN;

rtk_bt_hfp_ag_conf_t ag_conf = {
								.link_num = 1,
								.rfc_hfp_ag_chann_num = RTK_BT_RFC_HFP_AG_CHANN_NUM,
								.rfc_hsp_ag_chann_num = RTK_BT_RFC_HSP_AG_CHANN_NUM,
								.ag_supported_features = RTK_BT_HFP_AG_LOCAL_CAPABILITY_3WAY |
								RTK_BT_HFP_AG_LOCAL_CAPABILITY_VOICE_RECOGNITION |
								RTK_BT_HFP_AG_LOCAL_CAPABILITY_INBAND_RINGING |
								RTK_BT_HFP_AG_LOCAL_CAPABILITY_CODEC_NEGOTIATION |
								RTK_BT_HFP_AG_LOCAL_CAPABILITY_HF_INDICATORS |
								RTK_BT_HFP_AG_LOCAL_CAPABILITY_ESCO_S4_T2_SUPPORTED
};
rtk_bt_hfp_hf_conf_t hf_conf = {
								.link_num = 1,
								.rfc_hfp_chann_num = RTK_BT_RFC_HFP_CHANN_NUM,
								.rfc_hsp_chann_num = RTK_BT_RFC_HSP_CHANN_NUM,
								.hf_supported_features = RTK_BT_HFP_HF_LOCAL_THREE_WAY_CALLING |
								RTK_BT_HFP_HF_LOCAL_CLI_PRESENTATION_CAPABILITY |
								RTK_BT_HFP_HF_LOCAL_VOICE_RECOGNITION_ACTIVATION |
								RTK_BT_HFP_HF_LOCAL_ESCO_S4_SETTINGS |
								RTK_BT_HFP_HF_LOCAL_REMOTE_VOLUME_CONTROL
};
static uint8_t hfp_role;
extern T_APP_DB app_db;
extern T_APP_BR_LINK *app_find_br_link(uint8_t *bd_addr);


static void app_hfp_bt_cback(T_BT_EVENT event_type, void *event_buf, uint16_t buf_len)
{
	(void)buf_len;
	T_BT_EVENT_PARAM *param = event_buf;
	T_APP_BR_LINK *p_link;
	bool handle = true;
	rtk_bt_evt_t *p_evt = NULL;

	switch (event_type) {

	case BT_EVENT_SDP_ATTR_INFO: {
		rtk_bt_hfp_sdp_attr_info_t *p_info = NULL;
		T_BT_SDP_ATTR_INFO *sdp_info = &param->sdp_attr_info.info;
		if (RTK_BT_AUDIO_HFP_ROLE_AG == hfp_role) {
			if (UUID_HANDSFREE == sdp_info->srv_class_uuid_data.uuid_16) {
				bt_hfp_ag_connect_req(param->sdp_attr_info.bd_addr, sdp_info->server_channel, true);
				{
					p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP,RTK_BT_HFP_EVT_SDP_ATTR_INFO,sizeof(rtk_bt_hfp_sdp_attr_info_t));
					if (!p_evt) {
						printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
						handle = false;
						break;
					}
					p_info = (rtk_bt_hfp_sdp_attr_info_t *)p_evt->data;
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
		}
	}
	break;

	case BT_EVENT_HFP_SUPPORTED_FEATURES_IND: {
		rtk_bt_hfp_ag_features_ind_t *p_capabilitys = NULL;

		p_link = app_find_br_link(param->hfp_supported_features_ind.bd_addr);
		if (p_link != NULL) {
			p_link->remote_hfp_brsf_capability = param->hfp_supported_features_ind.ag_bitmap;
			if (param->hfp_supported_features_ind.ag_bitmap & BT_HFP_HF_REMOTE_CAPABILITY_INBAND_RINGING) {
				p_link->is_inband_ring = true;
				printf("app_hfp_bt_cback: support inband ring \r\n");
			} else {
				p_link->is_inband_ring = false;
				printf("app_hfp_bt_cback: not support inband ring, use outband ring alert \r\n");
			}
			APP_PRINT_INFO1("app_audio_bt_cback: BT_EVENT_HFP_SUPPORTED_FEATURES_IND, remote_hfp_brsf_capability 0x%04x",
							p_link->remote_hfp_brsf_capability);
			printf("app_hfp_bt_cback: remote_hfp_brsf_capability 0x%04x \r\n", p_link->remote_hfp_brsf_capability);
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_SUPPORTED_FEATURES_IND, sizeof(rtk_bt_hfp_ag_features_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_capabilitys = (rtk_bt_hfp_ag_features_ind_t *)p_evt->data;
				memcpy((void *)&p_capabilitys->capability, (void *)&p_link->remote_hfp_brsf_capability, sizeof(uint16_t));
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	} 
	break;

	case BT_EVENT_HFP_CONN_IND: {
		rtk_bt_hfp_conn_ind_t *p_hfp_conn_ind = NULL;

		p_link = app_find_br_link(param->hfp_conn_ind.bd_addr);
		if (p_link != NULL) {
			bt_hfp_connect_cfm(p_link->bd_addr, true);
			APP_PRINT_INFO0("BT_EVENT_HFP_CONN_IND");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_CONN_IND \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_CONN_IND, sizeof(rtk_bt_hfp_conn_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_conn_ind = (rtk_bt_hfp_conn_ind_t *)p_evt->data;
				memcpy((void *)p_hfp_conn_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_CONN_CMPL: {
		rtk_bt_hfp_conn_ind_t *p_hfp_conn_ind = NULL;

		p_link = app_find_br_link(param->hfp_conn_cmpl.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_CONN_CMPL");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_CONN_CMPL \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_CONN_CMPL, sizeof(rtk_bt_hfp_conn_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_conn_ind = (rtk_bt_hfp_conn_ind_t *)p_evt->data;
				memcpy((void *)p_hfp_conn_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
			bt_hfp_batt_level_report(p_link->bd_addr, battery_power);
			bt_hfp_speaker_gain_level_report(p_link->bd_addr, speaker_gain);
			bt_hfp_microphone_gain_level_report(p_link->bd_addr, microphone_gain);
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_CALL_STATUS: {
		rtk_bt_hfp_call_status_ind_t *p_hfp_call_status_ind = NULL;

		p_link = app_find_br_link(param->hfp_call_status.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_CALL_STATUS");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_CALL_STATUS \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_CALL_STATUS, sizeof(rtk_bt_hfp_call_status_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_call_status_ind = (rtk_bt_hfp_call_status_ind_t *)p_evt->data;
				memcpy((void *)p_hfp_call_status_ind->bd_addr, (void *)p_link->bd_addr, 6);
				p_hfp_call_status_ind->prev_status = param->hfp_call_status.prev_status;
				p_hfp_call_status_ind->curr_status = param->hfp_call_status.curr_status;
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_CALLER_ID_IND: {
		rtk_bt_hfp_caller_id_ind_t *p_hfp_caller_id_ind = NULL;

		p_link = app_find_br_link(param->hfp_caller_id_ind.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_CALLER_ID_IND");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_CALLER_ID_IND \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_CALLER_ID_IND, sizeof(rtk_bt_hfp_caller_id_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_caller_id_ind = (rtk_bt_hfp_caller_id_ind_t *)p_evt->data;
				memcpy((void *)p_hfp_caller_id_ind->bd_addr, (void *)p_link->bd_addr, 6);
				memcpy((void *)p_hfp_caller_id_ind->number, (void *)param->hfp_caller_id_ind.number, 20);
				p_hfp_caller_id_ind->type = param->hfp_caller_id_ind.type;
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_DISCONN_CMPL: {
		rtk_bt_hfp_disconn_ind_t *p_hfp_disconn_ind = NULL;

		p_link = app_find_br_link(param->hfp_disconn_cmpl.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_DISCONN_CMPL");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_DISCONN_CMPL \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_DISCONN_CMPL, sizeof(rtk_bt_hfp_disconn_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_disconn_ind = (rtk_bt_hfp_disconn_ind_t *)p_evt->data;
				memcpy((void *)p_hfp_disconn_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_BATTERY_IND: {
		rtk_bt_hfp_hf_battery_ind_t *p_hfp_batt_ind = NULL;

		p_link = app_find_br_link(param->hfp_battery_ind.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_BATTERY_IND");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_BATTERY_IND \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_HF_BATTERY_IND, sizeof(rtk_bt_hfp_hf_battery_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_batt_ind = (rtk_bt_hfp_hf_battery_ind_t *)p_evt->data;
				memcpy((void *)p_hfp_batt_ind->bd_addr, (void *)p_link->bd_addr, 6);
				p_hfp_batt_ind->state = param->hfp_battery_ind.state;
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, &battery_power)) {
					handle = false;
					break;
				}
				printf("app_hfp_bt_cback: get battery power %d \r\n", (int)battery_power);
				bt_hfp_batt_level_report(p_link->bd_addr, battery_power);
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_SPK_VOLUME_CHANGED: {
		rtk_bt_hfp_hf_speaker_volume_changed_ind_t *p_speaker_vol_ind = NULL;

		p_link = app_find_br_link(param->hfp_spk_volume_changed.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_SPK_VOLUME_CHANGED");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_SPK_VOLUME_CHANGED %d \r\n", param->hfp_spk_volume_changed.volume);
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_HF_SPEAKER_VOL_CHANGED_IND, sizeof(rtk_bt_hfp_hf_speaker_volume_changed_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_speaker_vol_ind = (rtk_bt_hfp_hf_speaker_volume_changed_ind_t *)p_evt->data;
				memcpy((void *)p_speaker_vol_ind->bd_addr, (void *)p_link->bd_addr, 6);
				p_speaker_vol_ind->volume = param->hfp_spk_volume_changed.volume;
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_MIC_VOLUME_CHANGED: {
		rtk_bt_hfp_hf_mic_volume_changed_ind_t *p_mic_vol_ind = NULL;

		p_link = app_find_br_link(param->hfp_mic_volume_changed.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_MIC_VOLUME_CHANGED");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_MIC_VOLUME_CHANGED %d \r\n", param->hfp_mic_volume_changed.volume);
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_HF_MIC_VOL_CHANGED_IND, sizeof(rtk_bt_hfp_hf_mic_volume_changed_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_mic_vol_ind = (rtk_bt_hfp_hf_mic_volume_changed_ind_t *)p_evt->data;
				memcpy((void *)p_mic_vol_ind->bd_addr, (void *)p_link->bd_addr, 6);
				p_mic_vol_ind->volume = param->hfp_mic_volume_changed.volume;
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_SCO_CONN_IND: {
		rtk_bt_hfp_conn_ind_t *p_hfp_conn_ind = NULL;

		p_link = app_find_br_link(param->sco_conn_ind.bd_addr);
		if (p_link != NULL) {
			bt_sco_conn_cfm(p_link->bd_addr, true);
			APP_PRINT_INFO0("BT_EVENT_SCO_CONN_IND");
			printf("app_hfp_bt_cback: BT_EVENT_SCO_CONN_IND \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_SCO_CONN_IND, sizeof(rtk_bt_hfp_conn_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_conn_ind = (rtk_bt_hfp_conn_ind_t *)p_evt->data;
				memcpy((void *)p_hfp_conn_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_SCO_CONN_CMPL: {
		rtk_bt_hfp_codec_t *p_hfp_codec_t = NULL;

		p_link = app_find_br_link(param->sco_conn_cmpl.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_SCO_CONN_CMPL");
			printf("app_hfp_bt_cback: BT_EVENT_SCO_CONN_CMPL sco handle %x \r\n", param->sco_conn_cmpl.handle);
			p_link->sco_handle = param->sco_conn_cmpl.handle;
			if (param->sco_conn_cmpl.air_mode == 3) {
				APP_PRINT_INFO0("HFP config: mSBC ");
				printf("app_hfp_bt_cback: HFP config: mSBC \r\n");
				p_link->hfp_codec_type = RTK_BT_AUDIO_CODEC_mSBC;
				p_link->hfp_sco_codec_info.msbc.channel_mode = 1;
				p_link->hfp_sco_codec_info.msbc.sample_rate = 16000;
				p_link->hfp_sco_codec_info.msbc.bitpool = 26;
				p_link->hfp_sco_codec_info.msbc.allocation_method = 0;
				p_link->hfp_sco_codec_info.msbc.subbands = 8;
				p_link->hfp_sco_codec_info.msbc.block_length = 15;
			} else if (param->sco_conn_cmpl.air_mode == 2) {
				APP_PRINT_INFO0("HFP config: CVSD ");
				printf("app_hfp_bt_cback: HFP config: CVSD \r\n");
				p_link->hfp_codec_type = RTK_BT_AUDIO_CODEC_CVSD;
				p_link->hfp_sco_codec_info.cvsd.channel_num = 1;
				p_link->hfp_sco_codec_info.cvsd.sample_rate = 8000;
				if (param->sco_conn_cmpl.rx_pkt_len == 30) {
					p_link->hfp_sco_codec_info.cvsd.frame_duration = RTK_BT_AUDIO_CVSD_FRAME_DURATION_3_75_MS;
				} else {
					p_link->hfp_sco_codec_info.cvsd.frame_duration = RTK_BT_AUDIO_CVSD_FRAME_DURATION_7_5_MS;
				}
			} else {
				APP_PRINT_INFO0("HFP config: unknow ");
				printf("app_hfp_bt_cback: HFP unknow \r\n");
				break;
			}
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_SCO_CONN_CMPL, sizeof(rtk_bt_hfp_codec_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_codec_t = (rtk_bt_hfp_codec_t *)p_evt->data;
				p_hfp_codec_t->codec_type = p_link->hfp_codec_type;
				if (p_hfp_codec_t->codec_type == RTK_BT_AUDIO_CODEC_mSBC) {
					p_hfp_codec_t->msbc.channel_mode = p_link->hfp_sco_codec_info.msbc.channel_mode;
					p_hfp_codec_t->msbc.sample_rate = p_link->hfp_sco_codec_info.msbc.sample_rate;
					p_hfp_codec_t->msbc.bitpool = p_link->hfp_sco_codec_info.msbc.bitpool;
					p_hfp_codec_t->msbc.allocation_method = p_link->hfp_sco_codec_info.msbc.allocation_method;
					p_hfp_codec_t->msbc.subbands = p_link->hfp_sco_codec_info.msbc.subbands;
					p_hfp_codec_t->msbc.block_length = p_link->hfp_sco_codec_info.msbc.block_length;
				} else {
					p_hfp_codec_t->cvsd.channel_num = p_link->hfp_sco_codec_info.cvsd.channel_num;
					p_hfp_codec_t->cvsd.sample_rate = p_link->hfp_sco_codec_info.cvsd.sample_rate;
					p_hfp_codec_t->cvsd.frame_duration = p_link->hfp_sco_codec_info.cvsd.frame_duration;
				}
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_SCO_DATA_IND: {
		rtk_bt_hfp_sco_data_ind_t *p_hfp_sc_data_ind = NULL;

		p_link = app_find_br_link(param->sco_data_ind.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_SCO_DATA_IND");
			// printf("app_hfp_bt_cback: BT_EVENT_SCO_DATA_IND \r\n");
			if (param->sco_data_ind.status != BT_SCO_PKT_STATUS_OK) {
				printf("app_hfp_bt_cback: sco data ind status error %d \r\n", param->sco_data_ind.status);
				break;
			}
			{
				uint8_t cb_ret = 0;
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_SCO_DATA_IND, sizeof(rtk_bt_hfp_sco_data_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_sc_data_ind = (rtk_bt_hfp_sco_data_ind_t *)p_evt->data;
				memcpy((void *)p_hfp_sc_data_ind->data, (void *)param->sco_data_ind.p_data, param->sco_data_ind.length);
				p_hfp_sc_data_ind->length = param->sco_data_ind.length;
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, &cb_ret)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_SCO_DISCONNECTED: {
		rtk_bt_hfp_disconn_ind_t *p_hfp_disconn_ind = NULL;

		p_link = app_find_br_link(param->sco_disconnected.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_SCO_DISCONNECTED");
			printf("app_hfp_bt_cback: BT_EVENT_SCO_DISCONNECTED \r\n");
			p_link->sco_handle = 0;
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_SCO_DISCONNCTED_IND, sizeof(rtk_bt_hfp_disconn_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_disconn_ind = (rtk_bt_hfp_disconn_ind_t *)p_evt->data;
				memcpy((void *)p_hfp_disconn_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_AG_CONN_IND: {
		rtk_bt_hfp_conn_ind_t *p_hfp_conn_ind = NULL;

		p_link = app_find_br_link(param->hfp_ag_conn_ind.bd_addr);
		if (p_link != NULL) {
			bt_hfp_ag_connect_cfm(p_link->bd_addr, true);
			APP_PRINT_INFO0("BT_EVENT_HFP_AG_CONN_IND");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_AG_CONN_IND \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_AG_CONN_IND, sizeof(rtk_bt_hfp_conn_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_conn_ind = (rtk_bt_hfp_conn_ind_t *)p_evt->data;
				memcpy((void *)p_hfp_conn_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_AG_CONN_CMPL: {
		rtk_bt_hfp_conn_ind_t *p_hfp_conn_ind = NULL;

		p_link = app_find_br_link(param->hfp_ag_conn_cmpl.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_AG_CONN_CMPL");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_AG_CONN_CMPL \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_AG_CONN_CMPL, sizeof(rtk_bt_hfp_conn_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_conn_ind = (rtk_bt_hfp_conn_ind_t *)p_evt->data;
				memcpy((void *)p_hfp_conn_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_AG_DISCONN_CMPL: {
		rtk_bt_hfp_disconn_ind_t *p_hfp_disconn_ind = NULL;

		p_link = app_find_br_link(param->hfp_ag_disconn_cmpl.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_AG_DISCONN_CMPL");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_AG_DISCONN_CMPL \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_AG_DISCONN_CMPL, sizeof(rtk_bt_hfp_disconn_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_disconn_ind = (rtk_bt_hfp_disconn_ind_t *)p_evt->data;
				memcpy((void *)p_hfp_disconn_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_AG_INDICATORS_STATUS_REQ: {
		rtk_bt_hfp_ag_indicators_status_req_t *p_ag_ind_status_req = NULL;

		p_link = app_find_br_link(param->hfp_ag_indicators_status_req.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_AG_INDICATORS_STATUS_REQ");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_AG_INDICATORS_STATUS_REQ \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_AG_INDICATORS_STATUS_REQ, sizeof(rtk_bt_hfp_ag_indicators_status_req_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_ag_ind_status_req = (rtk_bt_hfp_ag_indicators_status_req_t *)p_evt->data;
				memcpy((void *)p_ag_ind_status_req->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
				/* use directly calling and waiting for return TODO*/
				//Td App provide current network status.
				// T_BT_HFP_AG_SERVICE_INDICATOR service_indicator = BT_HFP_AG_SERVICE_STATUS_AVAILABLE;
				// T_BT_HFP_AG_CALL_INDICATOR call_indicator = BT_HFP_AG_NO_CALL_IN_PROGRESS;
				// T_BT_HFP_AG_CALL_SETUP_INDICATOR call_setup_indicator = BT_HFP_AG_CALL_SETUP_STATUS_IDLE;
				// T_BT_HFP_AG_CALL_HELD_INDICATOR call_held_indicator = BT_HFP_AG_CALL_HELD_STATUS_IDLE;

				// //Td App provide current signal status.
				// uint8_t signal_indicator = 5;
				// //Td App provide current roaming status.
				// T_BT_HFP_AG_ROAMING_INICATOR roaming_indicator = BT_HFP_AG_ROAMING_STATUS_ACTIVE;
				// //Td App provide current battery status.
				// uint8_t batt_chg_indicator = 5;
				// bt_hfp_ag_indicators_send(p_link->bd_addr,
				// 						service_indicator,
				// 						call_indicator,
				// 						call_setup_indicator,
				// 						call_held_indicator,
				// 						signal_indicator,
				// 						roaming_indicator,
				// 						batt_chg_indicator);
				// bt_hfp_ag_ok_send(param->hfp_ag_indicators_status_req.bd_addr);
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_AG_CURR_CALLS_LIST_QUERY: {
		rtk_bt_hfp_ag_curr_calls_list_query_t *p_ag_query_calls_list_ind = NULL;

		p_link = app_find_br_link(param->hfp_ag_curr_calls_list_query.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_AG_CURR_CALLS_LIST_QUERY");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_AG_CURR_CALLS_LIST_QUERY \r\n");
			bt_hfp_ag_ok_send(p_link->bd_addr);
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_AG_CURR_CALLS_LIST_QUERY, sizeof(rtk_bt_hfp_ag_curr_calls_list_query_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_ag_query_calls_list_ind = (rtk_bt_hfp_ag_curr_calls_list_query_t *)p_evt->data;
				memcpy((void *)p_ag_query_calls_list_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_AG_DIAL_LAST_NUMBER: {
		rtk_bt_hfp_ag_dial_last_number_ind_t *p_ag_dial_last_num_ind = NULL;

		p_link = app_find_br_link(param->hfp_ag_dial_last_number.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_AG_DIAL_LAST_NUMBER");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_AG_DIAL_LAST_NUMBER \r\n");
			bt_hfp_ag_ok_send(p_link->bd_addr);
			bt_hfp_ag_call_dial(p_link->bd_addr);
			bt_hfp_ag_audio_connect_req(p_link->bd_addr);
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_AG_DIAL_LAST_NUMBER, sizeof(rtk_bt_hfp_ag_dial_last_number_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_ag_dial_last_num_ind = (rtk_bt_hfp_ag_dial_last_number_ind_t *)p_evt->data;
				memcpy((void *)p_ag_dial_last_num_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_AG_CALL_ANSWER_REQ: {
		rtk_bt_hfp_ag_call_answer_req_ind_t *p_ag_call_answer_req_ind = NULL;

		p_link = app_find_br_link(param->hfp_ag_call_answer_req.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_AG_CALL_ANSWER_REQ");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_AG_CALL_ANSWER_REQ \r\n");
			bt_hfp_ag_call_answer(p_link->bd_addr);
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_AG_CALL_ANSWER_REQ, sizeof(rtk_bt_hfp_ag_call_answer_req_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_ag_call_answer_req_ind = (rtk_bt_hfp_ag_call_answer_req_ind_t *)p_evt->data;
				memcpy((void *)p_ag_call_answer_req_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_AG_CALL_TERMINATE_REQ: {
		rtk_bt_hfp_ag_call_terminate_req_ind_t *p_ag_call_terminate_req_ind = NULL;

		p_link = app_find_br_link(param->hfp_ag_call_answer_req.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_AG_CALL_TERMINATE_REQ");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_AG_CALL_TERMINATE_REQ \r\n");
			bt_hfp_ag_call_terminate(p_link->bd_addr);
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_AG_CALL_TERMINATE_REQ, sizeof(rtk_bt_hfp_ag_call_terminate_req_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_ag_call_terminate_req_ind = (rtk_bt_hfp_ag_call_terminate_req_ind_t *)p_evt->data;
				memcpy((void *)p_ag_call_terminate_req_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HFP_AG_CALL_STATUS_CHANGED: {
		rtk_bt_hfp_call_status_ind_t *p_hfp_call_status_ind = NULL;

		p_link = app_find_br_link(param->hfp_ag_call_status_changed.bd_addr);
		if (p_link != NULL) {
			APP_PRINT_INFO0("BT_EVENT_HFP_AG_CALL_STATUS_CHANGED");
			printf("app_hfp_bt_cback: BT_EVENT_HFP_AG_CALL_STATUS_CHANGED \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HFP, RTK_BT_HFP_EVT_AG_CALL_STATUS, sizeof(rtk_bt_hfp_call_status_ind_t));
				if (!p_evt) {
					printf("app_hfp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hfp_call_status_ind = (rtk_bt_hfp_call_status_ind_t *)p_evt->data;
				memcpy((void *)p_hfp_call_status_ind->bd_addr, (void *)p_link->bd_addr, 6);
				p_hfp_call_status_ind->prev_status = param->hfp_ag_call_status_changed.prev_status;
				p_hfp_call_status_ind->curr_status = param->hfp_ag_call_status_changed.curr_status;
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HFP p_link is NULL");
			printf("app_hfp_bt_cback: HFP p_link is NULL \r\n");
		}
	}
	break;

	default: {
		APP_PRINT_INFO1("app_hfp_bt_cback: default event_type 0x%04x", event_type);
		// printf("app_hfp_bt_cback: default event_type 0x%04x \r\n", event_type);
		handle = false;
	}
	break;
	}

	if (handle == true) {
		// APP_PRINT_INFO1("app_hfp_bt_cback: event_type 0x%04x", event_type);
		// printf("app_hfp_bt_cback: event_type 0x%04x \r\n", event_type);
	}
}

static uint16_t bt_stack_hfp_connect(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	T_GAP_UUID_DATA uuid;

	if (RTK_BT_AUDIO_HFP_ROLE_AG == hfp_role) {
		uuid.uuid_16 = UUID_HANDSFREE;
	} else {
		uuid.uuid_16 = UUID_HANDSFREE_AUDIO_GATEWAY;
	}

	if (gap_br_start_sdp_discov(bd_addr, GAP_UUID16, uuid) == GAP_CAUSE_SUCCESS) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_hfp_disconnect(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	T_APP_BR_LINK *p_link;

	p_link = app_find_br_link(bd_addr);
	if (p_link != NULL) {
		if (RTK_BT_AUDIO_HFP_ROLE_AG == hfp_role) {
			if (bt_hfp_ag_disconnect_req(bd_addr)) {
				return RTK_BT_OK;
			}
		} else {
			if (bt_hfp_disconnect_req(bd_addr)) {
				return RTK_BT_OK;
			}
		}
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_hfp_sco_connect(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	
	if (RTK_BT_AUDIO_HFP_ROLE_AG == hfp_role) {
		if (bt_hfp_ag_audio_connect_req(bd_addr)) {
			return RTK_BT_OK;
		}
	} else {
		if (bt_hfp_audio_connect_req(bd_addr)) {
			return RTK_BT_OK;
		}
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_hfp_sco_disconnect(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	T_APP_BR_LINK *p_link;

	p_link = app_find_br_link(bd_addr);
	if (p_link != NULL) {
		if (RTK_BT_AUDIO_HFP_ROLE_AG == hfp_role) {
			if (bt_hfp_ag_audio_disconnect_req(bd_addr)) {
				return RTK_BT_OK;
			}
		} else {
			if (bt_hfp_audio_disconnect_req(bd_addr)) {
				return RTK_BT_OK;
			}
		}
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_hfp_call_incoming(void *param)
{
	rtk_bt_hfp_call_incoming_t *p_call_incoming_t = (rtk_bt_hfp_call_incoming_t *)param;
	
	if (RTK_BT_AUDIO_HFP_ROLE_AG == hfp_role) {
		if (bt_hfp_ag_call_incoming(p_call_incoming_t->bd_addr,
									p_call_incoming_t->call_num,
									p_call_incoming_t->call_num_len,
									p_call_incoming_t->call_num_type)) {
			return RTK_BT_OK;
		}
	} else {
		if (bt_hfp_dial_last_number_req(p_call_incoming_t->bd_addr)) {
			return RTK_BT_OK;
		}
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_hfp_call_answer(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	
	if (RTK_BT_AUDIO_HFP_ROLE_AG == hfp_role) {
		if (bt_hfp_ag_call_answer(bd_addr)) {
			return RTK_BT_OK;
		}
	} else {
		if (bt_hfp_call_answer_req(bd_addr)) {
			return RTK_BT_OK;
		}
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_hfp_call_terminate(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	
	if (RTK_BT_AUDIO_HFP_ROLE_AG == hfp_role) {
		if (bt_hfp_ag_call_terminate(bd_addr)) {
			return RTK_BT_OK;
		}
	} else {
		if (bt_hfp_call_terminate_req(bd_addr)) {
			return RTK_BT_OK;
		}
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_hfp_data_send(void *param)
{
	rtk_bt_hfp_sco_data_send_t *p_data_send_t = (rtk_bt_hfp_sco_data_send_t *)param;
	T_APP_BR_LINK *p_link = NULL;

	p_link = app_find_br_link(p_data_send_t->bd_addr);
	if (!p_link) {
		printf("bt_stack_hfp_data_send: No link found \r\n");
		return RTK_BT_FAIL;
	}
	do {
		if (bt_sco_data_send(p_data_send_t->bd_addr, p_data_send_t->seq_num, p_data_send_t->buf, p_data_send_t->len)) {
			return RTK_BT_OK;
		}
		osif_delay(1);
	} while(p_link->sco_handle);
	APP_PRINT_INFO0("bt_stack_hfp_data_send fail");
	printf("send fail %d %d \r\n", p_data_send_t->seq_num, p_data_send_t->len);

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_hfp_report_batt_level(void *param)
{
	rtk_bt_hfp_batt_level_t *pbatt_level_t = (rtk_bt_hfp_batt_level_t *)param;

	if (bt_hfp_batt_level_report(pbatt_level_t->bd_addr, pbatt_level_t->level)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_hfp_report_speaker_gain(void *param)
{
	rtk_bt_hfp_speaker_gain_t *p_param_t = (rtk_bt_hfp_speaker_gain_t *)param;

	if (bt_hfp_speaker_gain_level_report(p_param_t->bd_addr, p_param_t->gain)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_hfp_report_microphone_gain(void *param)
{
	rtk_bt_hfp_microphone_gain_t *p_param_t = (rtk_bt_hfp_microphone_gain_t *)param;

	if (bt_hfp_microphone_gain_level_report(p_param_t->bd_addr, p_param_t->gain)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

uint16_t bt_stack_hfp_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = 0;
	API_PRINT("bt_stack_hfp_act_handle: act = %d \r\n", p_cmd->act);
	switch(p_cmd->act){

		case RTK_BT_HFP_ACT_CONNECT:
			ret = bt_stack_hfp_connect(p_cmd->param);
			break;

		case RTK_BT_HFP_ACT_DISCONNECT:
			ret = bt_stack_hfp_disconnect(p_cmd->param);
			break;

		case RTK_BT_HFP_ACT_SCO_CONNECT:
			ret = bt_stack_hfp_sco_connect(p_cmd->param);
			break;

		case RTK_BT_HFP_ACT_SCO_DISCONNECT:
			ret = bt_stack_hfp_sco_disconnect(p_cmd->param);
			break;

		case RTK_BT_HFP_ACT_CALL_INCOMING:
			ret = bt_stack_hfp_call_incoming(p_cmd->param);
			break;

		case RTK_BT_HFP_ACT_CALL_ANSWER:
			ret = bt_stack_hfp_call_answer(p_cmd->param);
			break;

		case RTK_BT_HFP_ACT_CALL_TERMINATE:
			ret = bt_stack_hfp_call_terminate(p_cmd->param);
			break;

		case RTK_BT_HFP_ACT_SEND_SCO_DATA:
			ret = bt_stack_hfp_data_send(p_cmd->param);
			break;

		case RTK_BT_HFP_ACT_REPORT_BATT_LEVEL:
			ret = bt_stack_hfp_report_batt_level(p_cmd->param);
			break;

		case RTK_BT_HFP_ACT_REPORT_SPEAKER_GAIN:
			ret = bt_stack_hfp_report_speaker_gain(p_cmd->param);
			break;

		case RTK_BT_HFP_ACT_REPORT_MICROPHONE_GAIN:
			ret = bt_stack_hfp_report_microphone_gain(p_cmd->param);
			break;

		default:
			printf("bt_stack_hfp_act_handle: unknown act: %d \r\n", p_cmd->act);
			ret = 0;
			break;
	}

	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);

	return ret;
}

uint16_t bt_stack_hfp_init(uint8_t role)
{
	printf("[HFP]app_hfp_init\n");

	hfp_role = role;
	if (role == RTK_BT_AUDIO_HFP_ROLE_AG) {
		if (bt_hfp_ag_init(ag_conf.link_num,
							ag_conf.rfc_hfp_ag_chann_num,
							ag_conf.rfc_hsp_ag_chann_num,
							ag_conf.ag_supported_features, NULL) == false) {
			printf("[HFP]bt_hfp_ag_init FAIL \n");
			return RTK_BT_FAIL;
		}
	} else if (role == RTK_BT_AUDIO_HFP_ROLE_HF) {
		if (bt_hfp_init(hf_conf.link_num,
						hf_conf.rfc_hfp_chann_num,
						hf_conf.rfc_hsp_chann_num,
						hf_conf.hf_supported_features) == false) {
			printf("[HFP]bt_hfp_init FAIL \n");
			return RTK_BT_FAIL;
		}
	} else {
		printf("[HFP]unknown role %d \n", role);
		return RTK_BT_FAIL;
	}

	bt_mgr_cback_register(app_hfp_bt_cback);

	return RTK_BT_OK;
}

extern void hfp_deinit(void);

void bt_stack_hfp_deinit(void)
{
	printf("[HFP]app_hfp_init\n");
	hfp_deinit();
}

#endif