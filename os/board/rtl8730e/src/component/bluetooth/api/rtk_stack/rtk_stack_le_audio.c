/*
 *******************************************************************************
 * Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */
#include <string.h>
#include <stdio.h>
#include <bt_api_config.h>
#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT
#include <osif.h>
#include <ble_audio_flags.h>
#include <rtk_bt_common.h> 
#include <rtk_bt_device.h>
#include <rtk_stack_internal.h>
#include <rtk_stack_config.h>
#include <rtk_bt_le_audio.h> 
#include <trace_app.h>   
#include <bt_direct_msg.h>
#include <gap_iso_data.h>
#include <gap.h>
#include <bap.h>
#include <bap_unicast_client.h>
#include <cap.h>
#include <ble_audio.h>
#include <pacs_mgr.h>
#include <ascs_mgr.h>
#include <bass_mgr.h>
#include <bass_client.h>
#include <csis_mgr.h>
#include <csis_client.h>
#include <mcp_client.h>
#include <mcp_mgr.h>
#include <ble_mgr.h>
#include <metadata_def.h>
#include <ble_audio_sync.h>
#include <broadcast_source_sm.h>
#include <base_data_generate.h>
#include <vocs_mgr.h>
#include <vocs_client.h>
#include <vcs_def.h>
#include <vcs_mgr.h>
#include <vcs_client.h>
#include <mics_mgr.h>
#include <mics_client.h>
#include <aics_def.h>
#include <aics_mgr.h>
#include <aics_client.h>
#include <bt_gatt_svc.h>
#include <bt_gatt_client.h>
#include <tmas_mgr.h>
#include <tmas_client.h>
#include <tmas_def.h>

extern bool rtk_bt_check_evt_cb_direct_calling(uint8_t group, uint8_t evt_code);

#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
static T_BROADCAST_SOURCE_HANDLE g_broadcast_source_handle = NULL;
#endif

#if defined(RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT
static uint8_t default_csis_size = 2;
static uint8_t default_csis_rank = 1;
static uint8_t default_csis_feature = RTK_BT_LE_AUDIO_CSIS_SET_MEMBER_LOCK_EXIST | RTK_BT_LE_AUDIO_CSIS_SET_MEMBER_SIZE_EXIST | 
                                        RTK_BT_LE_AUDIO_CSIS_SET_MEMBER_RANK_EXIST | RTK_BT_LE_AUDIO_CSIS_SET_MEMBER_SIRK_NOTIFY_SUPPORT;
static uint8_t default_csis_sirk[RTK_BT_LE_CSIS_SIRK_LEN] = {0x63, 0x68, 0x65, 0x6e,
                                             0x67, 0x2d, 0x63, 0x61,
                                             0x69, 0x73, 0x37, 0x38,
                                             0x32, 0x53, 0xe8, 0x10
                                            };                                         
static rtk_bt_le_audio_csis_sirk_type_t default_csis_sirk_type = RTK_BT_LE_CSIS_SIRK_PLN;
#endif 

static uint8_t default_metadata[] =
{
    0x03,
    RTK_BT_LE_METADATA_TYPE_STREAMING_AUDIO_CONTEXTS,
    (uint8_t)(RTK_BT_LE_AUDIO_CONTEXT_MEDIA),
    (uint8_t)(RTK_BT_LE_AUDIO_CONTEXT_MEDIA >> 8),
    0x2,
    RTK_BT_LE_METADATA_TYPE_CCCD_LIST,
    0
};

//only for cap accept
#if (defined(RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) && RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT)
static uint8_t *p_default_pac_sink_codec = NULL;
static uint8_t default_pac_sink_codec[] = {
    //Number_of_PAC_records
    2,
    //PAC Record
    LC3_CODEC_ID, 0, 0, 0, 0,//Codec_ID
    //Codec_Specific_Capabilities_Length
    16,
    //Codec_Specific_Capabilities
    0x03,
    CODEC_CAP_TYPE_SUPPORTED_SAMPLING_FREQUENCIES,
    (uint8_t)(SAMPLING_FREQUENCY_16K | SAMPLING_FREQUENCY_32K | SAMPLING_FREQUENCY_48K),
    (uint8_t)((SAMPLING_FREQUENCY_16K | SAMPLING_FREQUENCY_32K | SAMPLING_FREQUENCY_48K) >> 8),
    0x02,
    CODEC_CAP_TYPE_SUPPORTED_FRAME_DURATIONS,
    FRAME_DURATION_PREFER_10_MS_BIT | FRAME_DURATION_10_MS_BIT,
    0x02,
    CODEC_CAP_TYPE_AUDIO_CHANNEL_COUNTS,
    AUDIO_CHANNEL_COUNTS_1 | AUDIO_CHANNEL_COUNTS_2,
    0x05,
    CODEC_CAP_TYPE_SUPPORTED_OCTETS_PER_CODEC_FRAME,
    0x28, 0x00, 0x78, 0x00,
    //Metadata_Length
    0x04,
    //Metadata
    0x03,
    METADATA_TYPE_PREFERRED_AUDIO_CONTEXTS,
    (uint8_t)(AUDIO_CONTEXT_MEDIA),
    (uint8_t)(AUDIO_CONTEXT_MEDIA >> 8), //must fit dongle
    //PAC Record
    LC3_CODEC_ID, 0, 0, 0, 0,//Codec_ID
    //Codec_Specific_Capabilities_Length
    16,
    //Codec_Specific_Capabilities
    0x03,
    CODEC_CAP_TYPE_SUPPORTED_SAMPLING_FREQUENCIES,
    (uint8_t)(SAMPLING_FREQUENCY_16K | SAMPLING_FREQUENCY_32K),
    (uint8_t)((SAMPLING_FREQUENCY_16K | SAMPLING_FREQUENCY_32K) >> 8),
    0x02,
    CODEC_CAP_TYPE_SUPPORTED_FRAME_DURATIONS,
    FRAME_DURATION_PREFER_10_MS_BIT | FRAME_DURATION_10_MS_BIT,
    0x02,
    CODEC_CAP_TYPE_AUDIO_CHANNEL_COUNTS,
    AUDIO_CHANNEL_COUNTS_1 | AUDIO_CHANNEL_COUNTS_2,
    0x05,
    CODEC_CAP_TYPE_SUPPORTED_OCTETS_PER_CODEC_FRAME,
    0x28, 0x00, 0x50, 0x00,
    //Metadata_Length
    0x04,
    //Metadata
    0x03,
    METADATA_TYPE_PREFERRED_AUDIO_CONTEXTS,
    (uint8_t)(AUDIO_CONTEXT_CONVERSATIONAL),
    (uint8_t)(AUDIO_CONTEXT_CONVERSATIONAL >> 8)
};
static uint8_t *p_default_pac_source_codec = NULL;
static uint8_t default_pac_source_codec[] = {
    //Number_of_PAC_records
    1,
    //PAC Record
    LC3_CODEC_ID, 0, 0, 0, 0,//Codec_ID
    //Codec_Specific_Capabilities_Length
    16,
    //Codec_Specific_Capabilities
    0x03,
    CODEC_CAP_TYPE_SUPPORTED_SAMPLING_FREQUENCIES,
    (uint8_t)(SAMPLING_FREQUENCY_16K | SAMPLING_FREQUENCY_32K),
    (uint8_t)((SAMPLING_FREQUENCY_16K | SAMPLING_FREQUENCY_32K) >> 8),
    0x02,
    CODEC_CAP_TYPE_SUPPORTED_FRAME_DURATIONS,
    FRAME_DURATION_PREFER_10_MS_BIT | FRAME_DURATION_10_MS_BIT,
    0x02,
    CODEC_CAP_TYPE_AUDIO_CHANNEL_COUNTS,
    AUDIO_CHANNEL_COUNTS_1 | AUDIO_CHANNEL_COUNTS_2,
    0x05,
    CODEC_CAP_TYPE_SUPPORTED_OCTETS_PER_CODEC_FRAME,
    0x28, 0x00, 0x28, 0x00,
    //Metadata_Length
    0x04,
    //Metadata
    0x03,
    METADATA_TYPE_PREFERRED_AUDIO_CONTEXTS,
    (uint8_t)(AUDIO_CONTEXT_CONVERSATIONAL),
    (uint8_t)(AUDIO_CONTEXT_CONVERSATIONAL >> 8)
};
#endif

static uint8_t default_sink_ase_num = 1;
static uint8_t default_source_ase_num = 1;

#if (defined(RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) && RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT)
static uint16_t default_sink_available_contexts = AUDIO_CONTEXT_MEDIA | AUDIO_CONTEXT_CONVERSATIONAL;
static uint16_t default_source_available_contexts = AUDIO_CONTEXT_MEDIA | AUDIO_CONTEXT_CONVERSATIONAL;
static uint32_t default_sink_audio_location = AUDIO_LOCATION_FL;
static uint32_t default_source_audio_location = AUDIO_LOCATION_FL;
#endif 

static uint16_t rtk_stack_bap_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;

    //BT_API_PRINT(BT_API_DEBUG,"%s: msg 0x%04x\r\n",__func__, msg);

    switch (msg) {
        case LE_AUDIO_MSG_BAP_DIS_ALL_DONE: {
                T_BAP_DIS_ALL_DONE *p_data = (T_BAP_DIS_ALL_DONE *)buf;
                APP_PRINT_INFO4("LE_AUDIO_MSG_BAP_DIS_ALL_DONE: conn_handle 0x%x, pacs_is_found %d, sink_pac_num %d, source_pac_num %d",
                            p_data->conn_handle,p_data->pacs_is_found,p_data->sink_pac_num,p_data->source_pac_num);
                APP_PRINT_INFO3("LE_AUDIO_MSG_BAP_DIS_ALL_DONE: ascs_is_found %d, sink_ase_num %d, source_ase_num %d",
                            p_data->ascs_is_found,p_data->sink_ase_num,p_data->source_ase_num);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BAP_DIS_ALL_DONE: conn_handle 0x%x, pacs_is_found %d, sink_pac_num %d, source_pac_num %d\r\n",
                            p_data->conn_handle,p_data->pacs_is_found,p_data->sink_pac_num,p_data->source_pac_num);
                BT_API_PRINT(BT_API_DEBUG,"ascs_is_found %d, sink_ase_num %d, source_ase_num %d\r\n",
                            p_data->ascs_is_found,p_data->sink_ase_num,p_data->source_ase_num);
                BT_API_PRINT(BT_API_DEBUG,"bass_is_found %d, brs_char_num %d\r\n",p_data->bass_is_found,p_data->brs_char_num);

                rtk_bt_le_audio_bap_discovery_done_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_BAP_DISCOVERY_DONE_IND,
                                        sizeof(rtk_bt_le_audio_bap_discovery_done_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_bap_discovery_done_ind_t *)p_evt->data;
                p_ind->conn_handle = p_data->conn_handle;
                p_ind->pacs_is_found = p_data->pacs_is_found;
                p_ind->sink_pac_num = p_data->sink_pac_num;
                p_ind->source_pac_num = p_data->source_pac_num;
                p_ind->ascs_is_found = p_data->ascs_is_found;
                p_ind->sink_ase_num = p_data->sink_ase_num;
                p_ind->source_ase_num = p_data->source_ase_num;
                p_ind->bass_is_found = p_data->bass_is_found;
                p_ind->brs_char_num = p_data->brs_char_num;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
#if 1                  
                T_BAP_PACS_INFO pacs_info = {0};    
                if (bap_pacs_get_info(p_data->conn_handle, &pacs_info)) {
                    APP_PRINT_INFO5("PAC info: value_exist 0x%x, sink_pac_num %d, source_pac_num %d, snk_audio_loc 0x%x, src_audio_loc 0x%x",
                                    pacs_info.value_exist,
                                    pacs_info.sink_pac_num,
                                    pacs_info.source_pac_num,
                                    pacs_info.snk_audio_loc,
                                    pacs_info.src_audio_loc);
                    BT_API_PRINT(BT_API_DEBUG,"PAC info: value_exist 0x%x, sink_pac_num %d, source_pac_num %d, snk_audio_loc 0x%x, src_audio_loc 0x%x\r\n",
                                    pacs_info.value_exist,
                                    pacs_info.sink_pac_num,
                                    pacs_info.source_pac_num,
                                    (unsigned int)pacs_info.snk_audio_loc,
                                    (unsigned int)pacs_info.src_audio_loc);
                    APP_PRINT_INFO4("PAC info: snk_sup_context 0x%x, src_sup_context 0x%x, snk_avail_context 0x%x, src_avail_context 0x%x",
                                    pacs_info.snk_sup_context,
                                    pacs_info.src_sup_context,
                                    pacs_info.snk_avail_context,
                                    pacs_info.src_avail_context);
                    BT_API_PRINT(BT_API_DEBUG,"PAC info: snk_sup_context 0x%x, src_sup_context 0x%x, snk_avail_context 0x%x, src_avail_context 0x%x\r\n",
                                    pacs_info.snk_sup_context,
                                    pacs_info.src_sup_context,
                                    pacs_info.snk_avail_context,
                                    pacs_info.src_avail_context);
                }
#endif
            }
            break;

        case LE_AUDIO_MSG_BAP_PACS_NOTIFY: {
                T_BAP_PACS_NOTIFY *p_data = (T_BAP_PACS_NOTIFY *)buf;
                APP_PRINT_INFO3("LE_AUDIO_MSG_BAP_PACS_NOTIFY: conn_handle 0x%x, pac_handle 0x%x, pacs_type 0x%x",
                            p_data->conn_handle,
                            p_data->pac_handle,
                            p_data->pacs_type);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BAP_PACS_NOTIFY: conn_handle 0x%x, pac_handle 0x%x, pacs_type 0x%x\r\n",
                            p_data->conn_handle,
                            p_data->pac_handle,
                            p_data->pacs_type);
            }
            break;

        default:
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg 0x%04x, not process!\r\n",__func__, msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }
    return app_result;
}

static uint16_t rtk_stack_cap_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;

    //BT_API_PRINT(BT_API_DEBUG,"%s: msg 0x%04x\r\n",__func__, msg);
    switch (msg) {
        case LE_AUDIO_MSG_CAP_DIS_DONE: {
                T_CAP_DIS_DONE *p_data = (T_CAP_DIS_DONE *)buf;
                APP_PRINT_INFO6("LE_AUDIO_MSG_CAP_DIS_DONE: conn_handle 0x%x, load_from_ftl %d, cas_is_found %d, cas_inc_csis %d, vcs_is_found %d, mics_is_found %d",
                                p_data->conn_handle, p_data->load_from_ftl,p_data->cas_is_found,p_data->cas_inc_csis,p_data->vcs_is_found,p_data->mics_is_found);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CAP_DIS_DONE: conn_handle 0x%x, load_from_ftl %d, cas_is_found %d, cas_inc_csis %d, vcs_is_found %d, mics_is_found %d\r\n",
                                p_data->conn_handle, p_data->load_from_ftl, p_data->cas_is_found, p_data->cas_inc_csis, p_data->vcs_is_found, p_data->mics_is_found);
                rtk_bt_le_audio_cap_discovery_done_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_CAP_DISCOVERY_DONE_IND,
                                        sizeof(rtk_bt_le_audio_cap_discovery_done_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_cap_discovery_done_ind_t *)p_evt->data;
                p_ind->conn_handle = p_data->conn_handle;
                p_ind->load_from_ftl = p_data->load_from_ftl;
                p_ind->cas_is_found = p_data->cas_is_found;
                p_ind->cas_inc_csis = p_data->cas_inc_csis;
                p_ind->vcs_is_found = p_data->vcs_is_found;
                p_ind->mics_is_found = p_data->mics_is_found;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);                                
            }
            break;

        default:
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg 0x%04x, not process!\r\n",__func__, msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }
    return app_result;
}
//only for cap accept
#if (defined(RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) && RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT)
static uint16_t rtk_stack_pacs_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;

    //BT_API_PRINT(BT_API_DEBUG,"%s: msg 0x%04x\r\n",__func__, msg);

    switch (msg) {
        case LE_AUDIO_MSG_PACS_READ_AVAILABLE_CONTEXTS_IND: {
                T_PACS_READ_AVAILABLE_CONTEXTS_IND *p_data = (T_PACS_READ_AVAILABLE_CONTEXTS_IND *)buf;
                APP_PRINT_INFO2("LE_AUDIO_MSG_PACS_READ_AVAILABLE_CONTEXTS_IND: conn_handle 0x%x, cid=%d",
                                p_data->conn_handle,p_data->cid);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_PACS_READ_AVAILABLE_CONTEXTS_IND: conn_handle 0x%x, cid=%d,default_sink_available_contexts=0x%x,default_source_available_contexts=0x%x\r\n",
                                p_data->conn_handle,p_data->cid,default_sink_available_contexts,default_source_available_contexts);
                pacs_available_contexts_read_cfm(p_data->conn_handle, p_data->cid, default_sink_available_contexts, default_source_available_contexts);
            }
            break;
#if 0
        case LE_AUDIO_MSG_PACS_WRITE_SINK_AUDIO_LOC_IND: {
                T_PACS_WRITE_SINK_AUDIO_LOC_IND *p_data = (T_PACS_WRITE_SINK_AUDIO_LOC_IND *)buf;
                APP_PRINT_INFO2("LE_AUDIO_MSG_PACS_WRITE_SINK_AUDIO_LOC_IND: conn_handle 0x%x, sink_audio_locations 0x%x",
                                p_data->conn_handle,
                                p_data->sink_audio_locations);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_PACS_WRITE_SINK_AUDIO_LOC_IND: conn_handle 0x%x, sink_audio_locations 0x%x\r\n",
                                p_data->conn_handle,
                                (unsigned int)p_data->sink_audio_locations);
            }
            break;

        case LE_AUDIO_MSG_PACS_WRITE_SOURCE_AUDIO_LOC_IND: {
                T_PACS_WRITE_SOURCE_AUDIO_LOC_IND *p_data = (T_PACS_WRITE_SOURCE_AUDIO_LOC_IND *)buf;
                APP_PRINT_INFO2("LE_AUDIO_MSG_PACS_WRITE_SOURCE_AUDIO_LOC_IND: conn_handle 0x%x, source_audio_locations 0x%x",
                                p_data->conn_handle,
                                p_data->source_audio_locations);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_PACS_WRITE_SOURCE_AUDIO_LOC_IND: conn_handle 0x%x, source_audio_locations 0x%x\r\n",
                                p_data->conn_handle,
                                (unsigned int)p_data->source_audio_locations);
            }
            break;
#endif
        default:
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg 0x%04x, not process!\r\n",__func__, msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }
    return app_result;
}
#endif 
#if 1 
static uint16_t rtk_stack_pacs_client_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;

    //BT_API_PRINT(BT_API_DEBUG,"%s: msg 0x%04x\r\n",__func__, msg);

    switch (msg) {
        case LE_AUDIO_MSG_PACS_CLIENT_DIS_DONE://LE_AUDIO_MSG_GROUP_PACS_CLIENT | 0x00,
            {
                T_PACS_CLIENT_DIS_DONE *p_data = (T_PACS_CLIENT_DIS_DONE *)buf;
                APP_PRINT_INFO5("LE_AUDIO_MSG_PACS_CLIENT_DIS_DONE: conn_handle 0x%x is_found = %d load_from_ftl=%d,sink_pac_num=%d,source_pac_num=%d",
                                p_data->conn_handle,p_data->is_found,p_data->load_from_ftl,
                                p_data->sink_pac_num,p_data->source_pac_num);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_PACS_CLIENT_DIS_DONE: conn_handle 0x%x is_found=%d load_from_ftl=%d,sink_pac_num=%d,source_pac_num=%d,sink_loc_writable=%d,sink_loc_exist=%d,source_loc_writable=%d,source_loc_exist=%d\r\n",
                                p_data->conn_handle,p_data->is_found,p_data->load_from_ftl,
                                p_data->sink_pac_num,p_data->source_pac_num,
                                p_data->sink_loc_writable,p_data->sink_loc_exist,p_data->source_loc_writable,p_data->source_loc_exist);
            }
            break;

        case LE_AUDIO_MSG_PACS_CLIENT_WRITE_SINK_AUDIO_LOC_RESULT: //LE_AUDIO_MSG_GROUP_PACS_CLIENT | 0x02,
            {
                T_PACS_CLIENT_WRITE_SINK_AUDIO_LOC_RESULT *p_data = (T_PACS_CLIENT_WRITE_SINK_AUDIO_LOC_RESULT *)buf;
                APP_PRINT_INFO2("LE_AUDIO_MSG_PACS_CLIENT_WRITE_SINK_AUDIO_LOC_RESULT: conn_handle 0x%x, cause 0x%x",
                                p_data->conn_handle,
                                p_data->cause);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_PACS_CLIENT_WRITE_SINK_AUDIO_LOC_RESULT: conn_handle 0x%x, cause 0x%x\r\n",
                                p_data->conn_handle,
                                p_data->cause);
            }
            break;

        case LE_AUDIO_MSG_PACS_CLIENT_WRITE_SOURCE_AUDIO_LOC_RESULT: //LE_AUDIO_MSG_GROUP_PACS_CLIENT | 0x03,
            {
                T_PACS_CLIENT_WRITE_SOURCE_AUDIO_LOC_RESULT *p_data = (T_PACS_CLIENT_WRITE_SOURCE_AUDIO_LOC_RESULT *)buf;
                APP_PRINT_INFO2("LE_AUDIO_MSG_PACS_CLIENT_WRITE_SOURCE_AUDIO_LOC_RESULT: conn_handle 0x%x, cause 0x%x",
                                p_data->conn_handle,
                                p_data->cause);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_PACS_CLIENT_WRITE_SOURCE_AUDIO_LOC_RESULT: conn_handle 0x%x, cause 0x%x\r\n",
                                p_data->conn_handle,
                                p_data->cause);
            }
            break;

        default:
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg 0x%04x, not process!\r\n",__func__, msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }
    return app_result;
}
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) && RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT
static uint16_t rtk_stack_ascs_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
    rtk_bt_evt_t *p_evt = NULL;
    uint8_t cb_ret = 0;
    //BT_API_PRINT(BT_API_DEBUG,"%s: msg %x\r\n", __func__,msg);

    switch (msg) {
        case LE_AUDIO_MSG_ASCS_SETUP_DATA_PATH: {
            T_ASCS_SETUP_DATA_PATH *p_data = (T_ASCS_SETUP_DATA_PATH *)buf;
            APP_PRINT_INFO4("LE_AUDIO_MSG_ASCS_SETUP_DATA_PATH: conn_handle 0x%x, ase_id %d, path_direction 0x%x, cis_conn_handle 0x%x",
                            p_data->conn_handle,
                            p_data->ase_id,
                            p_data->path_direction,
                            p_data->cis_conn_handle);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_ASCS_SETUP_DATA_PATH: conn_handle 0x%x, ase_id %d, path_direction 0x%x, cis_conn_handle 0x%x\r\n",
                            p_data->conn_handle,
                            p_data->ase_id,
                            p_data->path_direction,
                            p_data->cis_conn_handle);
            APP_PRINT_INFO6("type_exist 0x%x, frame_duration 0x%x, sample_frequency 0x%x, codec_frame_blocks_per_sdu %d, octets_per_codec_frame %d, audio_channel_allocation 0x%x",
                                p_data->codec_parsed_data.type_exist,
                                p_data->codec_parsed_data.frame_duration,
                                p_data->codec_parsed_data.sample_frequency,
                                p_data->codec_parsed_data.codec_frame_blocks_per_sdu,
                                p_data->codec_parsed_data.octets_per_codec_frame,
                                p_data->codec_parsed_data.audio_channel_allocation);    
            BT_API_PRINT(BT_API_DEBUG,"type_exist 0x%x, frame_duration 0x%x, sample_frequency 0x%x, codec_frame_blocks_per_sdu %d, octets_per_codec_frame %d, audio_channel_allocation 0x%x\r\n",
                                p_data->codec_parsed_data.type_exist,
                                p_data->codec_parsed_data.frame_duration,
                                p_data->codec_parsed_data.sample_frequency,
                                p_data->codec_parsed_data.codec_frame_blocks_per_sdu,
                                p_data->codec_parsed_data.octets_per_codec_frame,
                                (unsigned int)p_data->codec_parsed_data.audio_channel_allocation);                          
            rtk_bt_le_audio_ascs_setup_data_path_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                    RTK_BT_LE_AUDIO_EVT_ASCS_SETUP_DATA_PATH_IND,
                                    sizeof(rtk_bt_le_audio_ascs_setup_data_path_ind_t));
			if (!p_evt) {
				BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
				break;
			} 
            p_ind = (rtk_bt_le_audio_ascs_setup_data_path_ind_t *)p_evt->data;
            p_ind->conn_handle = p_data->conn_handle;
            p_ind->ase_id = p_data->ase_id;
            p_ind->path_direction = p_data->path_direction;
            p_ind->cis_conn_handle = p_data->cis_conn_handle;
            p_ind->codec_cfg.type_exist = p_data->codec_parsed_data.type_exist;
            p_ind->codec_cfg.frame_duration = p_data->codec_parsed_data.frame_duration;
            p_ind->codec_cfg.sample_frequency = p_data->codec_parsed_data.sample_frequency;
            p_ind->codec_cfg.codec_frame_blocks_per_sdu = p_data->codec_parsed_data.codec_frame_blocks_per_sdu;
            p_ind->codec_cfg.octets_per_codec_frame = p_data->codec_parsed_data.octets_per_codec_frame;
            p_ind->codec_cfg.audio_channel_allocation = p_data->codec_parsed_data.audio_channel_allocation;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);  
        }
        break;

        case LE_AUDIO_MSG_ASCS_REMOVE_DATA_PATH: {
            T_ASCS_REMOVE_DATA_PATH *p_data = (T_ASCS_REMOVE_DATA_PATH *)buf;
            APP_PRINT_INFO4("LE_AUDIO_MSG_ASCS_REMOVE_DATA_PATH: conn_handle 0x%x, ase_id %d, path_direction 0x%x, cis_conn_handle 0x%x",
                            p_data->conn_handle,
                            p_data->ase_id,
                            p_data->path_direction,
                            p_data->cis_conn_handle);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_ASCS_REMOVE_DATA_PATH: conn_handle 0x%x, ase_id %d, path_direction 0x%x, cis_conn_handle 0x%x\r\n",
                            p_data->conn_handle,
                            p_data->ase_id,
                            p_data->path_direction,
                            p_data->cis_conn_handle);
            rtk_bt_le_audio_ascs_remove_data_path_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                    RTK_BT_LE_AUDIO_EVT_ASCS_REMOVE_DATA_PATH_IND,
                                    sizeof(rtk_bt_le_audio_ascs_remove_data_path_ind_t));
			if (!p_evt) {
				BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
				break;
			} 
            p_ind = (rtk_bt_le_audio_ascs_remove_data_path_ind_t *)p_evt->data;
            p_ind->conn_handle = p_data->conn_handle;
            p_ind->ase_id = p_data->ase_id;
            p_ind->path_direction = p_data->path_direction;
            p_ind->cis_conn_handle = p_data->cis_conn_handle;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;

        case LE_AUDIO_MSG_ASCS_ASE_STATE: {
            T_ASCS_ASE_STATE *p_data = (T_ASCS_ASE_STATE *)buf;
            APP_PRINT_INFO3("LE_AUDIO_MSG_ASCS_ASE_STATE: conn_handle 0x%x, ase_id %d, ase_state %d",
                            p_data->conn_handle,
                            p_data->ase_data.ase_id,
                            p_data->ase_data.ase_state);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_ASCS_ASE_STATE: conn_handle 0x%x, ase_id %d, ase_state %d\r\n",p_data->conn_handle,p_data->ase_data.ase_id,p_data->ase_data.ase_state);

            rtk_bt_le_audio_ascs_ase_state_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_ASCS_ASE_STATE_IND,
                                            sizeof(rtk_bt_le_audio_ascs_ase_state_ind_t));
			if (!p_evt) {
				BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
				break;
			}                                            
            p_ind = (rtk_bt_le_audio_ascs_ase_state_ind_t *)p_evt->data;
            p_ind->conn_handle = p_data->conn_handle;
            p_ind->ase_id = p_data->ase_data.ase_id;
            p_ind->audio_role = p_data->ase_data.direction;
            p_ind->ase_state = p_data->ase_data.ase_state;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
            break;
        }

        case LE_AUDIO_MSG_ASCS_CP_CONFIG_CODEC_IND: {
            T_ASCS_CP_CONFIG_CODEC_IND *p_data = (T_ASCS_CP_CONFIG_CODEC_IND *)buf;
            APP_PRINT_INFO2("LE_AUDIO_MSG_ASCS_CP_CONFIG_CODEC_IND: conn_handle 0x%x, number_of_ase %d",
                            p_data->conn_handle,
                            p_data->number_of_ase);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_ASCS_CP_CONFIG_CODEC_IND: conn_handle 0x%x, number_of_ase %d\r\n",
                            p_data->conn_handle,
                            p_data->number_of_ase);
            for (uint8_t i = 0; i < p_data->number_of_ase; i++) {
                APP_PRINT_INFO5("ase param[%d]: ase_id %d, target_latency %d, target_phy %d, codec_id %b",
                                i, p_data->param[i].data.ase_id,
                                p_data->param[i].data.target_latency,
                                p_data->param[i].data.target_phy,
                                TRACE_BINARY(5, p_data->param[i].data.codec_id));
                APP_PRINT_INFO7("ase param[%d]: type_exist 0x%x, frame_duration 0x%x, sample_frequency 0x%x, codec_frame_blocks_per_sdu %d, octets_per_codec_frame %d, audio_channel_allocation 0x%x",
                                i, p_data->param[i].codec_parsed_data.type_exist,
                                p_data->param[i].codec_parsed_data.frame_duration,
                                p_data->param[i].codec_parsed_data.sample_frequency,
                                p_data->param[i].codec_parsed_data.codec_frame_blocks_per_sdu,
                                p_data->param[i].codec_parsed_data.octets_per_codec_frame,
                                p_data->param[i].codec_parsed_data.audio_channel_allocation);
                BT_API_PRINT(BT_API_DEBUG,"ase param[%d]: ase_id %d, target_latency %d, target_phy %d, codec_id:\r\n",
                                i, p_data->param[i].data.ase_id,
                                p_data->param[i].data.target_latency,
                                p_data->param[i].data.target_phy);
                BT_API_DUMPBUF(BT_API_DEBUG,__func__,p_data->param[i].data.codec_id,5);
                BT_API_PRINT(BT_API_DEBUG,"ase param[%d]: type_exist 0x%x, frame_duration 0x%x, sample_frequency 0x%x, codec_frame_blocks_per_sdu %d, octets_per_codec_frame %d, audio_channel_allocation 0x%x\r\n",
                                i, p_data->param[i].codec_parsed_data.type_exist,
                                p_data->param[i].codec_parsed_data.frame_duration,
                                p_data->param[i].codec_parsed_data.sample_frequency,
                                p_data->param[i].codec_parsed_data.codec_frame_blocks_per_sdu,
                                p_data->param[i].codec_parsed_data.octets_per_codec_frame,
                                (unsigned int)p_data->param[i].codec_parsed_data.audio_channel_allocation);

                rtk_bt_le_audio_ascs_cp_codec_cfg_ind_t *p_codec_cfg = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_ASCS_CP_CODEC_CFG_IND,
                                        sizeof(rtk_bt_le_audio_ascs_cp_codec_cfg_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_codec_cfg = (rtk_bt_le_audio_ascs_cp_codec_cfg_ind_t *)p_evt->data;
                p_codec_cfg->conn_handle = p_data->conn_handle;
                p_codec_cfg->ase_id = p_data->param[i].data.ase_id;
                p_codec_cfg->codec_cfg.type_exist = p_data->param[i].codec_parsed_data.type_exist;
                p_codec_cfg->codec_cfg.frame_duration = p_data->param[i].codec_parsed_data.frame_duration;
                p_codec_cfg->codec_cfg.sample_frequency = p_data->param[i].codec_parsed_data.sample_frequency;
                p_codec_cfg->codec_cfg.codec_frame_blocks_per_sdu = p_data->param[i].codec_parsed_data.codec_frame_blocks_per_sdu;
                p_codec_cfg->codec_cfg.octets_per_codec_frame =  p_data->param[i].codec_parsed_data.octets_per_codec_frame;
                p_codec_cfg->codec_cfg.audio_channel_allocation = p_data->param[i].codec_parsed_data.audio_channel_allocation;                    
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;
        }

        case LE_AUDIO_MSG_ASCS_CP_CONFIG_QOS_IND: {
            T_ASCS_CP_CONFIG_QOS_IND *p_data = (T_ASCS_CP_CONFIG_QOS_IND *)buf;
            APP_PRINT_INFO2("LE_AUDIO_MSG_ASCS_CP_CONFIG_QOS_IND: conn_handle 0x%x, number_of_ase %d",
                            p_data->conn_handle,
                            p_data->number_of_ase);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_ASCS_CP_CONFIG_QOS_IND: conn_handle 0x%x, number_of_ase %d\r\n",
                            p_data->conn_handle,
                            p_data->number_of_ase);
            for (uint8_t i = 0; i < p_data->number_of_ase; i++) {
                uint16_t max_sdu = 0;
                uint16_t max_transport_latency = 0;
                uint32_t sdu_interval = 0;
                uint32_t presentation_delay = 0;
                LE_ARRAY_TO_UINT24(sdu_interval, p_data->param[i].sdu_interval);
                LE_ARRAY_TO_UINT24(presentation_delay, p_data->param[i].presentation_delay);
                LE_ARRAY_TO_UINT16(max_sdu, p_data->param[i].max_sdu);
                LE_ARRAY_TO_UINT16(max_transport_latency, p_data->param[i].max_transport_latency);
                APP_PRINT_INFO4("ase param[%d]: ase_id %d,  cig_id %d, cis_id %d",
                                i, p_data->param[i].ase_id,
                                p_data->param[i].cig_id, p_data->param[i].cis_id);
                APP_PRINT_INFO8("ase param[%d]: sdu_interval %x %x %x, framing %d, phy 0x%x, max_sdu  %x %x",
                                i, p_data->param[i].sdu_interval[0], p_data->param[i].sdu_interval[1],
                                p_data->param[i].sdu_interval[2],
                                p_data->param[i].framing, p_data->param[i].phy,
                                p_data->param[i].max_sdu[0], p_data->param[i].max_sdu[1]);
                APP_PRINT_INFO7("ase param[%d]: retransmission_number %d, max_transport_latency %x %x, presentation_delay  %x %x %x",
                                i, p_data->param[i].retransmission_number,
                                p_data->param[i].max_transport_latency[0],
                                p_data->param[i].max_transport_latency[1],
                                p_data->param[i].presentation_delay[0],
                                p_data->param[i].presentation_delay[1],
                                p_data->param[i].presentation_delay[2]);
                BT_API_PRINT(BT_API_DEBUG,"ase param[%d]: ase_id %d,  cig_id %d, cis_id %d\r\n",
                                i, p_data->param[i].ase_id,
                                p_data->param[i].cig_id, p_data->param[i].cis_id);
                BT_API_PRINT(BT_API_DEBUG,"ase param[%d]: sdu_interval %x %x %x, framing %d, phy 0x%x, max_sdu  %x %x\r\n",
                                i, p_data->param[i].sdu_interval[0], p_data->param[i].sdu_interval[1],
                                p_data->param[i].sdu_interval[2],
                                p_data->param[i].framing, p_data->param[i].phy,
                                p_data->param[i].max_sdu[0], p_data->param[i].max_sdu[1]);
                BT_API_PRINT(BT_API_DEBUG,"ase param[%d]: retransmission_number %d, max_transport_latency %x %x, presentation_delay  %x %x %x\r\n",
                                i, p_data->param[i].retransmission_number,
                                p_data->param[i].max_transport_latency[0],
                                p_data->param[i].max_transport_latency[1],
                                p_data->param[i].presentation_delay[0],
                                p_data->param[i].presentation_delay[1],
                                p_data->param[i].presentation_delay[2]);

                rtk_bt_le_audio_ascs_cp_qos_cfg_ind_t *p_qos_cfg = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_ASCS_CP_QOS_CFG_IND,
                                        sizeof(rtk_bt_le_audio_ascs_cp_qos_cfg_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_qos_cfg = (rtk_bt_le_audio_ascs_cp_qos_cfg_ind_t *)p_evt->data;
                p_qos_cfg->conn_handle = p_data->conn_handle;
                p_qos_cfg->ase_id = p_data->param[i].ase_id;
                p_qos_cfg->cig_id = p_data->param[i].cig_id;
                p_qos_cfg->cis_id = p_data->param[i].cis_id;
                p_qos_cfg->framing = p_data->param[i].framing;
                p_qos_cfg->phy = p_data->param[i].phy;
                p_qos_cfg->retransmission_number = p_data->param[i].retransmission_number;
                p_qos_cfg->max_sdu = max_sdu;                    
                p_qos_cfg->max_transport_latency = max_transport_latency;
                p_qos_cfg->sdu_interval = sdu_interval;
                p_qos_cfg->presentation_delay = presentation_delay;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;
        }

        case LE_AUDIO_MSG_ASCS_CP_ENABLE_IND: {
            T_ASCS_CP_ENABLE_IND *p_data = (T_ASCS_CP_ENABLE_IND *)buf;
            uint8_t i = 0;
            APP_PRINT_INFO2("LE_AUDIO_MSG_ASCS_CP_ENABLE_IND: conn_handle 0x%x, number_of_ase %d",
                            p_data->conn_handle,
                            p_data->number_of_ase);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_ASCS_CP_ENABLE_IND: conn_handle 0x%x, number_of_ase %d\r\n",
                            p_data->conn_handle,
                            p_data->number_of_ase);
            for (i = 0; i < p_data->number_of_ase; i++) {
                APP_PRINT_INFO4("ase param[%d]: ase_id %d, metadata_length %d, metadata %b",
                                i, p_data->param[i].ase_id,
                                p_data->param[i].metadata_length,
                                TRACE_BINARY(p_data->param[i].metadata_length, p_data->param[i].p_metadata));
                BT_API_PRINT(BT_API_DEBUG,"ase param[%d]: ase_id %d\r\n",
                            i,p_data->param[i].ase_id);
                BT_API_DUMPBUF(BT_API_DEBUG,__func__,p_data->param[i].p_metadata,p_data->param[i].metadata_length);                 
            }
            rtk_bt_le_audio_ascs_cp_enable_ind_t *p_enable = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                    RTK_BT_LE_AUDIO_EVT_ASCS_CP_ENABLE_IND,
                                    sizeof(rtk_bt_le_audio_ascs_cp_enable_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_enable = (rtk_bt_le_audio_ascs_cp_enable_ind_t *)p_evt->data;
            p_enable->conn_handle = p_data->conn_handle;
            p_enable->ase_num = p_data->number_of_ase;
            for (i = 0; i < p_data->number_of_ase; i++) {
                p_enable->ase_id[i] = p_data->param[i].ase_id;
            }
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);

            break;
        }

        case LE_AUDIO_MSG_ASCS_CP_DISABLE_IND: {
            T_ASCS_CP_DISABLE_IND *p_data = (T_ASCS_CP_DISABLE_IND *)buf;
            uint8_t i = 0;
            APP_PRINT_INFO3("LE_AUDIO_MSG_ASCS_CP_DISABLE_IND: conn_handle 0x%x, number_of_ase %d, ase_ids %b",
                            p_data->conn_handle,
                            p_data->number_of_ase, TRACE_BINARY(p_data->number_of_ase, p_data->ase_id));
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_ASCS_CP_DISABLE_IND: conn_handle 0x%x, number_of_ase %d\r\n",
                            p_data->conn_handle,
                            p_data->number_of_ase);
            rtk_bt_le_audio_ascs_cp_disable_ind_t *p_disable = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                    RTK_BT_LE_AUDIO_EVT_ASCS_CP_DISABLE_IND,
                                    sizeof(rtk_bt_le_audio_ascs_cp_disable_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_disable = (rtk_bt_le_audio_ascs_cp_disable_ind_t *)p_evt->data;
            p_disable->conn_handle = p_data->conn_handle;  
            p_disable->ase_num = p_data->number_of_ase;                           
            for (i = 0; i < p_data->number_of_ase; i++) {
                p_disable->ase_id[i] = p_data->ase_id[i];                 
            }
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
            break;
        }

        case LE_AUDIO_MSG_ASCS_CP_UPDATE_METADATA_IND: {
            T_ASCS_CP_UPDATE_METADATA_IND *p_data = (T_ASCS_CP_UPDATE_METADATA_IND *)buf;
            APP_PRINT_INFO2("LE_AUDIO_MSG_ASCS_CP_UPDATE_METADATA_IND: conn_handle 0x%x, number_of_ase %d",
                            p_data->conn_handle,
                            p_data->number_of_ase);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_ASCS_CP_UPDATE_METADATA_IND: conn_handle 0x%x, number_of_ase %d\r\n",
                            p_data->conn_handle,
                            p_data->number_of_ase);
            for (uint8_t i = 0; i < p_data->number_of_ase; i++) {
                APP_PRINT_INFO4("ase param[%d]: ase_id %d, metadata_length %d, metadata %b",
                                i, p_data->param[i].ase_id,
                                p_data->param[i].metadata_length,
                                TRACE_BINARY(p_data->param[i].metadata_length, p_data->param[i].p_metadata));
                BT_API_PRINT(BT_API_DEBUG,"ase param[%d]: ase_id %d\r\n",
                            i,p_data->param[i].ase_id);
                BT_API_DUMPBUF(BT_API_DEBUG,__func__,p_data->param[i].p_metadata,p_data->param[i].metadata_length);
            }
            break;
        }

        case LE_AUDIO_MSG_ASCS_CIS_REQUEST_IND: {
            T_ASCS_CIS_REQUEST_IND *p_data = (T_ASCS_CIS_REQUEST_IND *)buf;
            APP_PRINT_INFO6("LE_AUDIO_MSG_ASCS_CIS_REQUEST_IND: conn_handle 0x%x, cis_conn_handle 0x%x, snk_ase_id %d, snk_ase_state %d, src_ase_id %d, src_ase_state %d",
                            p_data->conn_handle, p_data->cis_conn_handle,
                            p_data->snk_ase_id, p_data->snk_ase_state,
                            p_data->src_ase_id, p_data->src_ase_state);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_ASCS_CIS_REQUEST_IND: conn_handle 0x%x, cis_conn_handle 0x%x, snk_ase_id %d, snk_ase_state %d, src_ase_id %d, src_ase_state %d\r\n",
                            p_data->conn_handle, p_data->cis_conn_handle,
                            p_data->snk_ase_id, p_data->snk_ase_state,
                            p_data->src_ase_id, p_data->src_ase_state);
            app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
            break;
        }

        case LE_AUDIO_MSG_ASCS_GET_PREFER_QOS: {
            T_ASCS_GET_PREFER_QOS *p_data = (T_ASCS_GET_PREFER_QOS *)buf;
            APP_PRINT_INFO5("LE_AUDIO_MSG_ASCS_GET_PREFER_QOS: conn_handle 0x%x, ase_id 0x%x, direction 0x%x, target_latency 0x%x, target_phy 0x%x",
                            p_data->conn_handle, p_data->ase_id, p_data->direction,p_data->target_latency,p_data->target_phy);
            APP_PRINT_INFO6("LE_AUDIO_MSG_ASCS_GET_PREFER_QOS: codec_cfg.type_exist 0x%x, codec_cfg.frame_duration 0x%x,codec_cfg.sample_frequency 0x%x, codec_cfg.codec_frame_blocks_per_sdu 0x%x, codec_cfg.octets_per_codec_frame 0x%x,codec_cfg.audio_channel_allocation 0x%x",
                            p_data->codec_cfg.type_exist, p_data->codec_cfg.frame_duration, 
                            p_data->codec_cfg.sample_frequency,p_data->codec_cfg.codec_frame_blocks_per_sdu,p_data->codec_cfg.octets_per_codec_frame,
                            p_data->codec_cfg.audio_channel_allocation);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_ASCS_GET_PREFER_QOS: conn_handle 0x%x, ase_id 0x%x, direction 0x%x, target_latency 0x%x, target_phy 0x%x\r\n",
                            p_data->conn_handle, p_data->ase_id, p_data->direction,p_data->target_latency,p_data->target_phy);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_ASCS_GET_PREFER_QOS: codec_cfg.type_exist 0x%x, codec_cfg.frame_duration 0x%x,codec_cfg.sample_frequency 0x%x, codec_cfg.codec_frame_blocks_per_sdu 0x%x, codec_cfg.octets_per_codec_frame 0x%x,codec_cfg.audio_channel_allocation 0x%x\r\n",
                            p_data->codec_cfg.type_exist, p_data->codec_cfg.frame_duration, 
                            p_data->codec_cfg.sample_frequency,p_data->codec_cfg.codec_frame_blocks_per_sdu,p_data->codec_cfg.octets_per_codec_frame,
                            (unsigned int)p_data->codec_cfg.audio_channel_allocation);
          
            rtk_bt_le_audio_ascs_get_prefer_qos_ind_t *p_ind = NULL;
            T_QOS_CFG_PREFERRED qos = {0};
            T_ASCS_PREFER_QOS_DATA prefer_qos_data = {0};
            if(true == ascs_get_ase_prefer_qos(p_data->conn_handle, p_data->ase_id,&prefer_qos_data)) {
                BT_API_PRINT(BT_API_DEBUG,"%s ascs_get_ase_prefer_qos from stack ok\r\n",__func__);
                if(false == ascs_cfg_ase_prefer_qos(p_data->conn_handle, p_data->ase_id, &prefer_qos_data)) {
                    BT_API_PRINT(BT_API_ERROR,"%s: ascs_cfg_ase_prefer_qos fail!\r\n", __func__); 
                }                
            } else {
                BT_API_PRINT(BT_API_ERROR,"%s ascs_get_ase_prefer_qos fail, get it from app\r\n",__func__);
                if (false == rtk_bt_check_evt_cb_direct_calling(RTK_BT_LE_GP_AUDIO, RTK_BT_LE_AUDIO_EVT_ASCS_GET_PREFER_QOS_IND)) { 
                    BT_API_PRINT(BT_API_ERROR,"%s: RTK_BT_LE_AUDIO_EVT_ASCS_GET_PREFER_QOS_IND is not direct calling!\r\n", __func__);
                    break;
                }
                
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_ASCS_GET_PREFER_QOS_IND,
                                            sizeof(rtk_bt_le_audio_ascs_get_prefer_qos_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_ascs_get_prefer_qos_ind_t *)p_evt->data;
                p_ind->conn_handle = p_data->conn_handle;
                p_ind->ase_id = p_data->ase_id;
                p_ind->direction = p_data->direction;
                p_ind->codec_cfg.type_exist = p_data->codec_cfg.type_exist;
                p_ind->codec_cfg.frame_duration = p_data->codec_cfg.frame_duration;
                p_ind->codec_cfg.sample_frequency = p_data->codec_cfg.sample_frequency;
                p_ind->codec_cfg.codec_frame_blocks_per_sdu = p_data->codec_cfg.codec_frame_blocks_per_sdu;
                p_ind->codec_cfg.octets_per_codec_frame = p_data->codec_cfg.octets_per_codec_frame;
                p_ind->codec_cfg.audio_channel_allocation = p_data->codec_cfg.audio_channel_allocation;
                p_ind->target_latency = (rtk_bt_le_audio_ascs_ase_target_latency_t)p_data->target_latency;
                p_ind->target_phy = (rtk_bt_le_audio_ascs_ase_target_phy_t)p_data->target_phy;
                p_ind->p_prefer_qos_data = (rtk_bt_le_audio_ascs_prefer_qos_data_t *)&prefer_qos_data; //make sure the definition of T_ASCS_PREFER_QOS_DATA and rtk_bt_le_audio_ascs_prefer_qos_data_t are same 
                /* Send event */
                rtk_bt_evt_indicate(p_evt, &cb_ret);
                if (cb_ret == RTK_BT_EVT_CB_OK) { 
                    BT_API_PRINT(BT_API_DEBUG,"RTK_BT_LE_AUDIO_EVT_ASCS_GET_PREFER_QOS_IND get data from app, conn_handle = %d,ase_id = %d,direction = %d,supported_framing = %d,preferred_phy=%d, preferred_retrans_number = %d,max_transport_latency = %d,presentation_delay_min = %d,presentation_delay_max = %d,preferred_presentation_delay_min = %d,preferred_presentation_delay_max = %d\r\n",
                                    p_data->conn_handle,p_data->ase_id,p_data->direction,
                                    prefer_qos_data.supported_framing,prefer_qos_data.preferred_phy,prefer_qos_data.preferred_retrans_number,
                                    prefer_qos_data.max_transport_latency,(int)prefer_qos_data.presentation_delay_min,(int)prefer_qos_data.presentation_delay_max,
                                    (int)prefer_qos_data.preferred_presentation_delay_min,(int)prefer_qos_data.preferred_presentation_delay_max);
                    if(false == qos_preferred_cfg_get_by_codec(&p_data->codec_cfg,p_data->target_latency,&qos)) {
                        BT_API_PRINT(BT_API_ERROR,"%s: qos_preferred_cfg_get_by_codec fail!\r\n", __func__);
                        break;
                    }
                    if(prefer_qos_data.supported_framing == 0) {prefer_qos_data.supported_framing = RTK_BLE_AUDIO_UNFRAMED_SUPPORTED;}
                    if(prefer_qos_data.preferred_phy == 0) {prefer_qos_data.preferred_phy = p_data->target_phy;}
                    if(prefer_qos_data.preferred_retrans_number == 0) {prefer_qos_data.preferred_retrans_number = qos.retransmission_number;}
                    if(prefer_qos_data.max_transport_latency == 0) {prefer_qos_data.max_transport_latency = qos.max_transport_latency;}
                    if(prefer_qos_data.presentation_delay_max == 0) {prefer_qos_data.presentation_delay_max = qos.presentation_delay;}
                    if(prefer_qos_data.preferred_presentation_delay_max == 0) {prefer_qos_data.preferred_presentation_delay_max = qos.presentation_delay;}
                    BT_API_PRINT(BT_API_DEBUG,"RTK_BT_LE_AUDIO_EVT_ASCS_GET_PREFER_QOS_IND get data from app, conn_handle = %d,ase_id = %d,direction = %d,supported_framing = %d,preferred_phy=%d, preferred_retrans_number = %d,max_transport_latency = %d,presentation_delay_min = %d,presentation_delay_max = %d,preferred_presentation_delay_min = %d,preferred_presentation_delay_max = %d\r\n",
                                    p_data->conn_handle,p_data->ase_id,p_data->direction,
                                    prefer_qos_data.supported_framing,prefer_qos_data.preferred_phy,prefer_qos_data.preferred_retrans_number,
                                    prefer_qos_data.max_transport_latency,(int)prefer_qos_data.presentation_delay_min,(int)prefer_qos_data.presentation_delay_max,
                                    (int)prefer_qos_data.preferred_presentation_delay_min,(int)prefer_qos_data.preferred_presentation_delay_max);
                    if(false == ascs_cfg_ase_prefer_qos(p_data->conn_handle, p_data->ase_id, &prefer_qos_data)) {
                        BT_API_PRINT(BT_API_ERROR,"%s: ascs_cfg_ase_prefer_qos fail!\r\n", __func__);
                    }
                }
            }

            break;
        }

        case LE_AUDIO_MSG_ASCS_AUDIO_CONTEXTS_CHECK_IND: {
            T_ASCS_AUDIO_CONTEXTS_CHECK_IND *p_data = (T_ASCS_AUDIO_CONTEXTS_CHECK_IND *)buf;
            APP_PRINT_INFO5("LE_AUDIO_MSG_ASCS_AUDIO_CONTEXTS_CHECK_IND: conn_handle 0x%x, is_update_metadata 0x%x, ase_id 0x%x, direction 0x%x, available_contexts 0x%x",
                            p_data->conn_handle, p_data->is_update_metadata, p_data->ase_id,p_data->direction,p_data->available_contexts);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_ASCS_AUDIO_CONTEXTS_CHECK_IND: conn_handle 0x%x, is_update_metadata 0x%x, ase_id 0x%x, direction 0x%x, available_contexts 0x%x\r\n",
                            p_data->conn_handle, p_data->is_update_metadata, p_data->ase_id,p_data->direction,p_data->available_contexts);
            break;
        }  

        case LE_AUDIO_MSG_ASCS_CIS_DISCONN_INFO: {
            T_ASCS_CIS_DISCONN_INFO *p_data = (T_ASCS_CIS_DISCONN_INFO *)buf;
            APP_PRINT_INFO5("LE_AUDIO_MSG_ASCS_CIS_DISCONN_INFO: conn_handle 0x%x, cis_conn_handle 0x%x, cause 0x%x, cig_id 0x%x, cis_id 0x%x",
                            p_data->conn_handle, p_data->cis_conn_handle, p_data->cause,p_data->cig_id,p_data->cis_id);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_ASCS_CIS_DISCONN_INFO: conn_handle 0x%x, cis_conn_handle 0x%x, cause 0x%x, cig_id 0x%x, cis_id 0x%x\r\n",
                            p_data->conn_handle, p_data->cis_conn_handle, p_data->cause,p_data->cig_id,p_data->cis_id);
            break;
        }  

        default:
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process!\r\n", __func__,msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }
    return app_result;
}
#endif 
#if defined(RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT
static uint16_t rtk_stack_csis_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;

    //BT_API_PRINT(BT_API_DEBUG,"%s: msg = 0x%x\r\n",__func__,msg);

    switch (msg) {
        case LE_AUDIO_MSG_CSIS_READ_SIRK_IND: {
                T_CSIS_READ_SIRK_IND *p_data = (T_CSIS_READ_SIRK_IND *)buf;
                APP_PRINT_INFO3("LE_AUDIO_MSG_CSIS_READ_SIRK_IND: conn_handle 0x%x, service_id %d, sirk_type %d",
                                p_data->conn_handle, p_data->service_id,
                                p_data->sirk_type);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CSIS_READ_SIRK_IND: conn_handle 0x%x, service_id %d, sirk_type %d\r\n",
                                p_data->conn_handle, p_data->service_id,
                                p_data->sirk_type);
            }
            break;

        case LE_AUDIO_MSG_CSIS_LOCK_STATE: {
                T_CSIS_LOCK_STATE *p_data = (T_CSIS_LOCK_STATE *)buf;
                APP_PRINT_INFO4("LE_AUDIO_MSG_CSIS_LOCK_STATE: service_id %d, lock_state %d, lock_address_type %d, lock_address %s",
                                p_data->service_id, p_data->lock_state,
                                p_data->lock_address_type, TRACE_BDADDR(p_data->lock_address));
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CSIS_LOCK_STATE: service_id %d, lock_state %d, lock_address_type %d\r\n",
                                p_data->service_id, p_data->lock_state,
                                p_data->lock_address_type);
            }
            break;

        default:
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process!\r\n", __func__,msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }
    return app_result;
}
#endif
#if defined(RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT
static uint16_t rtk_stack_csis_client_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
    rtk_bt_evt_t *p_evt = NULL;

    //BT_API_PRINT(BT_API_DEBUG,"%s: msg = 0x%x\r\n",__func__,msg);

    switch (msg) {
        case LE_AUDIO_MSG_CSIS_CLIENT_DIS_DONE: {
            T_CSIS_CLIENT_DIS_DONE *p_dis_done = (T_CSIS_CLIENT_DIS_DONE *)buf;
            APP_PRINT_INFO4("LE_AUDIO_MSG_CSIS_CLIENT_DIS_DONE: conn_handle 0x%x, is_found %d, load_from_ftl %d, srv_num %d",
                            p_dis_done->conn_handle, p_dis_done->is_found, p_dis_done->load_from_ftl, p_dis_done->srv_num);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CSIS_CLIENT_DIS_DONE: conn_handle 0x%x, is_found %d, load_from_ftl %d, srv_num %d \r\n",
                            p_dis_done->conn_handle, p_dis_done->is_found, p_dis_done->load_from_ftl, p_dis_done->srv_num);
#if 0                            
            if (p_dis_done->is_found) {
                if (p_dis_done->srv_num) {
                    T_ATTR_UUID srv_uuid = {0};
                    T_ATTR_UUID include_srv = {0};
                    srv_uuid.is_uuid16 = true;
                    srv_uuid.p.uuid16 = GATT_UUID_CSIS;
                    for (uint8_t i = 0; i < p_dis_done->srv_num; i++) {
                        srv_uuid.instance_id = i;
                        if (gatt_client_find_primary_srv_by_include(p_dis_done->conn_handle, &srv_uuid, &include_srv)) {
                            PROTOCOL_PRINT_INFO2("LE_AUDIO_MSG_CSIS_CLIENT_DIS_DONE: instance_id %d, serv 0x%x",
                                                 include_srv.instance_id, include_srv.p.uuid16);
                            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CSIS_CLIENT_DIS_DONE: instance_id %d, serv 0x%x\r\n",
                                                 include_srv.instance_id, include_srv.p.uuid16);
                        }
                    }
                }
            }
#else
            rtk_bt_le_audio_csis_client_discovery_done_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_CSIS_CLIENT_DISCOVERY_DONE_IND,
                                        sizeof(rtk_bt_le_audio_csis_client_discovery_done_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_csis_client_discovery_done_ind_t *)p_evt->data;
            p_ind->conn_handle = p_dis_done->conn_handle;            
            p_ind->is_found = p_dis_done->is_found;
            p_ind->load_from_ftl = p_dis_done->load_from_ftl;
            p_ind->srv_num = p_dis_done->srv_num;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
#endif
        }
        break;

        case LE_AUDIO_MSG_CSIS_CLIENT_READ_RESULT: {
            T_CSIS_CLIENT_READ_RESULT *p_read_result = (T_CSIS_CLIENT_READ_RESULT *)buf;
            APP_PRINT_INFO6("LE_AUDIO_MSG_CSIS_CLIENT_READ_RESULT: cause 0x%x, conn_handle 0x%x, group_handle %p, dev_handle %p, bd_addr %s, addr_type %d",
                            p_read_result->cause,p_read_result->conn_handle,p_read_result->group_handle, p_read_result->dev_handle,
                            TRACE_BDADDR(p_read_result->mem_info.bd_addr),p_read_result->mem_info.addr_type);
            APP_PRINT_INFO7("LE_AUDIO_MSG_CSIS_CLIENT_READ_RESULT: srv_instance_id %d, char_exit 0x%x, srv_uuid 0x%x, rank %d, set_mem_size %d, sirk_type %d, SIRK %b",
                            p_read_result->mem_info.srv_instance_id, p_read_result->mem_info.char_exit,p_read_result->mem_info.srv_uuid,
                            p_read_result->mem_info.rank,p_read_result->mem_info.set_mem_size, p_read_result->mem_info.sirk_type,
                            TRACE_BINARY(CSIS_SIRK_LEN, p_read_result->mem_info.sirk));
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CSIS_CLIENT_READ_RESULT: cause 0x%x, conn_handle 0x%x, group_handle 0x%p, dev_handle 0x%p\r\n",
                            p_read_result->cause, p_read_result->conn_handle, p_read_result->group_handle,p_read_result->dev_handle);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CSIS_CLIENT_READ_RESULT: srv_instance_id %d, char_exit 0x%x, srv_uuid 0x%x, rank %d, set_mem_size %d\r\n",
                            p_read_result->mem_info.srv_instance_id, p_read_result->mem_info.char_exit,p_read_result->mem_info.srv_uuid,
                            p_read_result->mem_info.rank,p_read_result->mem_info.set_mem_size);
#if 0            
            if (p_read_result->cause == GAP_SUCCESS) {
                if (p_read_result->group_handle == NULL) {
                    if (set_coordinator_add_group(&p_read_result->group_handle, NULL,
                                                  &p_read_result->dev_handle, &p_read_result->mem_info) == false) {
                        BT_API_PRINT(BT_API_ERROR,"%s: set_coordinator_add_group fail!\r\n", __func__);
                        break;
                    }
                }
                else {
                    if (p_read_result->dev_handle == NULL) {
                        if (set_coordinator_add_dev(p_read_result->group_handle,
                                                    &p_read_result->dev_handle, &p_read_result->mem_info) == false) {
                            BT_API_PRINT(BT_API_ERROR,"%s: set_coordinator_add_dev fail!\r\n", __func__);
                            break;
                        }
                    }
                }
                T_SET_COORDINATOR_LOCK lock_state;
                if (set_coordinator_get_lock_state(p_read_result->group_handle, &lock_state)) {
                    APP_PRINT_INFO1("LE_AUDIO_MSG_CSIS_CLIENT_READ_RESULT: lock_state %d",lock_state);
                    BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CSIS_CLIENT_READ_RESULT: lock_state %d\r\n",lock_state);
                }
                T_CSIS_SET_MEM_INFO mem_info = {0};
                if (set_coordinator_get_mem_info(p_read_result->group_handle, p_read_result->dev_handle, &mem_info)) {
                    APP_PRINT_INFO2("LE_AUDIO_MSG_CSIS_CLIENT_READ_RESULT: mem_info bd_addr %s, addr_type %d",
                                    TRACE_BDADDR(p_read_result->mem_info.bd_addr), p_read_result->mem_info.addr_type);
                    APP_PRINT_INFO8("LE_AUDIO_MSG_CSIS_CLIENT_READ_RESULT: mem_info srv_instance_id %d, char_exit 0x%x, srv_uuid 0x%x, rank %d, lock %d, set_mem_size %d, sirk_type %d, SIRK %b",
                                    mem_info.srv_instance_id,mem_info.char_exit,mem_info.srv_uuid,mem_info.rank,
                                    mem_info.lock,mem_info.set_mem_size,mem_info.sirk_type,TRACE_BINARY(CSIS_SIRK_LEN, p_read_result->mem_info.sirk));
                    BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CSIS_CLIENT_READ_RESULT: mem_info srv_instance_id %d, char_exit 0x%x, srv_uuid 0x%x, rank %d, lock %d, set_mem_size %d, sirk_type %d\r\n",
                                    mem_info.srv_instance_id,mem_info.char_exit,mem_info.srv_uuid,mem_info.rank,
                                    mem_info.lock,mem_info.set_mem_size,mem_info.sirk_type);
                }
            } 
#else 
            rtk_bt_le_audio_csis_client_read_result_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_CSIS_CLIENT_READ_RESULT_IND,
                                        sizeof(rtk_bt_le_audio_csis_client_read_result_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            }
            p_ind = (rtk_bt_le_audio_csis_client_read_result_ind_t *)p_evt->data;
            p_ind->cause = p_read_result->cause; 
            p_ind->conn_handle = p_read_result->conn_handle;            
            p_ind->group_handle = p_read_result->group_handle;
            p_ind->dev_handle = p_read_result->dev_handle;
            p_ind->mem_info.bd_addr.type = p_read_result->mem_info.addr_type;
            memcpy(p_ind->mem_info.bd_addr.addr_val,p_read_result->mem_info.bd_addr,RTK_BD_ADDR_LEN);
            p_ind->mem_info.srv_uuid = p_read_result->mem_info.srv_uuid;
            p_ind->mem_info.srv_instance_id = p_read_result->mem_info.srv_instance_id;
            p_ind->mem_info.char_exit = p_read_result->mem_info.char_exit;
            p_ind->mem_info.rank = p_read_result->mem_info.rank;       
            p_ind->mem_info.lock = (rtk_bt_le_audio_csis_lock_t)p_read_result->mem_info.lock;
            p_ind->mem_info.set_mem_size = p_read_result->mem_info.set_mem_size;
            p_ind->mem_info.sirk_type = (rtk_bt_le_audio_csis_sirk_type_t)p_read_result->mem_info.sirk_type;      
            memcpy(p_ind->mem_info.sirk, p_read_result->mem_info.sirk, CSIS_SIRK_LEN);
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
#endif
        }
        break;
        case LE_AUDIO_MSG_CSIS_CLIENT_SEARCH_TIMEOUT: {
            T_CSIS_CLIENT_SEARCH_TIMEOUT *p_result = (T_CSIS_CLIENT_SEARCH_TIMEOUT *)buf;
            APP_PRINT_INFO3("LE_AUDIO_MSG_CSIS_CLIENT_SEARCH_TIMEOUT: group_handle %p, set_mem_size %d, search_done %d",
                            p_result->group_handle, p_result->set_mem_size, p_result->search_done);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CSIS_CLIENT_SEARCH_TIMEOUT: group_handle 0x%p, set_mem_size %d, search_done %d\r\n",
                            p_result->group_handle, p_result->set_mem_size, p_result->search_done);
#if 0
            set_coordinator_cfg_discover(NULL, false, 0);
#else 
            rtk_bt_le_audio_csis_client_search_done_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_CSIS_CLIENT_SEARCH_DONE_IND,
                                        sizeof(rtk_bt_le_audio_csis_client_search_done_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            }
            p_ind = (rtk_bt_le_audio_csis_client_search_done_ind_t *)p_evt->data;            
            p_ind->group_handle = p_result->group_handle;
            p_ind->set_mem_size = p_result->set_mem_size;
            p_ind->search_done = false;
            p_ind->search_timeout = p_result->search_done;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
#endif
        }
        break;

        case LE_AUDIO_MSG_CSIS_CLIENT_SEARCH_DONE: {
            T_CSIS_CLIENT_SEARCH_DONE *p_result = (T_CSIS_CLIENT_SEARCH_DONE *)buf;
            APP_PRINT_INFO3("LE_AUDIO_MSG_CSIS_CLIENT_SEARCH_DONE: group_handle %p, set_mem_size %d, search_done %d",
                            p_result->group_handle, p_result->set_mem_size, p_result->search_done);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CSIS_CLIENT_SEARCH_DONE: group_handle 0x%p, set_mem_size %d, search_done %d\r\n",
                            p_result->group_handle, p_result->set_mem_size, p_result->search_done);
#if 0
            set_coordinator_cfg_discover(NULL, false, 0);
#else
            rtk_bt_le_audio_csis_client_search_done_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_CSIS_CLIENT_SEARCH_DONE_IND,
                                        sizeof(rtk_bt_le_audio_csis_client_search_done_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            }
            p_ind = (rtk_bt_le_audio_csis_client_search_done_ind_t *)p_evt->data;            
            p_ind->group_handle = p_result->group_handle;
            p_ind->set_mem_size = p_result->set_mem_size;
            p_ind->search_done = p_result->search_done;
            p_ind->search_timeout = false;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
#endif
        }
        break;

        case LE_AUDIO_MSG_CSIS_CLIENT_SET_MEM_FOUND: {
            T_CSIS_CLIENT_SET_MEM_FOUND *p_add = (T_CSIS_CLIENT_SET_MEM_FOUND *)buf;
            APP_PRINT_INFO8("LE_AUDIO_MSG_CSIS_CLIENT_SET_MEM_FOUND: group_handle %p, dev_handle %p, bd_addr %s, addr_type %d, srv_uuid 0x%x, rank %d, set_mem_size %d, SIRK %b",
                            p_add->group_handle, p_add->dev_handle, TRACE_BDADDR(p_add->bd_addr), p_add->addr_type,
                            p_add->srv_uuid, p_add->rank, p_add->set_mem_size, TRACE_BINARY(CSIS_SIRK_LEN, p_add->sirk));
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CSIS_CLIENT_SET_MEM_FOUND: group_handle %p, dev_handle %p, bd_addr %s, addr_type %d, srv_uuid 0x%x, rank %d, set_mem_size %d",
                            p_add->group_handle, p_add->dev_handle, TRACE_BDADDR(p_add->bd_addr), p_add->addr_type,
                            p_add->srv_uuid, p_add->rank, p_add->set_mem_size);

            rtk_bt_le_audio_csis_client_set_mem_found_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_CSIS_CLIENT_SET_MEM_FOUND_IND,
                                        sizeof(rtk_bt_le_audio_csis_client_set_mem_found_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            }
            p_ind = (rtk_bt_le_audio_csis_client_set_mem_found_ind_t *)p_evt->data;            
            p_ind->group_handle = p_add->group_handle;
            p_ind->dev_handle = p_add->dev_handle;
            p_ind->bd_addr.type = p_add->addr_type;
            memcpy(p_ind->bd_addr.addr_val,p_add->bd_addr,RTK_BD_ADDR_LEN);
            p_ind->srv_uuid = p_add->srv_uuid;
            p_ind->rank = p_add->rank;
            p_ind->set_mem_size = p_add->set_mem_size;
            memcpy(p_ind->sirk, p_add->sirk, CSIS_SIRK_LEN);

            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;

        case LE_AUDIO_MSG_CSIS_CLIENT_LOCK_STATE: {
            T_CSIS_CLIENT_LOCK_STATE *p_result = (T_CSIS_CLIENT_LOCK_STATE *)buf;
            APP_PRINT_INFO3("LE_AUDIO_MSG_CSIS_CLIENT_LOCK_STATE: group_handle %p, event 0x%x, state %d",
                            p_result->group_handle,p_result->event, p_result->lock_state);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CSIS_CLIENT_LOCK_STATE: group_handle 0x%p, event 0x%x, state %d\r\n",
                            p_result->group_handle,p_result->event, p_result->lock_state);
        }
        break;

        case LE_AUDIO_MSG_CSIS_CLIENT_SIRK_NOTIFY: {
            T_CSIS_CLIENT_SIRK_NOTIFY *p_state = (T_CSIS_CLIENT_SIRK_NOTIFY *)buf;
            APP_PRINT_INFO3("LE_AUDIO_MSG_CSIS_CLIENT_SIRK_NOTIFY: group_handle %p, sirk_type %d, SIRK %b",
                            p_state->group_handle, p_state->sirk_type, TRACE_BINARY(CSIS_SIRK_LEN, p_state->sirk));
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CSIS_CLIENT_SIRK_NOTIFY: group_handle 0x%p, sirk_type %d\r\n",
                            p_state->group_handle, p_state->sirk_type);
        }
        break;

        case LE_AUDIO_MSG_CSIS_CLIENT_SIZE_NOTIFY: {
            T_CSIS_CLIENT_SIZE_NOTIFY *p_result = (T_CSIS_CLIENT_SIZE_NOTIFY *)buf;
            APP_PRINT_INFO3("LE_AUDIO_MSG_CSIS_CLIENT_SIZE_NOTIFY: group_handle %p, old_size %d, new_size %d",
                            p_result->group_handle,p_result->old_size,p_result->new_size);
        }
        break;

        default:
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process!\r\n", __func__,msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }
    return app_result;
}
#endif

#if (defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT)
static uint16_t bt_stack_le_audio_pa_sync_state_change(T_BLE_AUDIO_SYNC_HANDLE sync_handle,T_BLE_AUDIO_PA_SYNC_STATE *p_pa_sync_state)
{
    bool indicate = false;
	rtk_bt_cmd_t *p_cmd = NULL; 
	rtk_bt_evt_t *p_evt = NULL;

    if(p_pa_sync_state == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    switch (p_pa_sync_state->sync_state) {
        case GAP_PA_SYNC_STATE_TERMINATED: {
            BT_API_PRINT(BT_API_DEBUG,"%s GAP_PA_SYNC_STATE_TERMINATED\r\n",__func__);
            p_cmd = bt_stack_pending_cmd_search(RTK_BT_LE_AUDIO_ACT_PA_SYNC_TERMINATE);
            if (p_cmd) {
                bt_stack_pending_cmd_delete(p_cmd); 
                p_cmd->ret = 0;
                osif_sem_give(p_cmd->psem);
            }            
            indicate = true;              
            break;
        }
        case GAP_PA_SYNC_STATE_SYNCHRONIZING_SCAN_IDLE:{
            BT_API_PRINT(BT_API_DEBUG,"%s GAP_PA_SYNC_STATE_SYNCHRONIZING_SCAN_IDLE\r\n",__func__);
            break;
        }
        case GAP_PA_SYNC_STATE_SYNCHRONIZING_WAIT_SCANNING:{
            BT_API_PRINT(BT_API_DEBUG,"%s GAP_PA_SYNC_STATE_SYNCHRONIZING_WAIT_SCANNING\r\n",__func__);
            break;
        }
        case GAP_PA_SYNC_STATE_SYNCHRONIZING: {
            BT_API_PRINT(BT_API_DEBUG,"%s GAP_PA_SYNC_STATE_SYNCHRONIZING\r\n",__func__);
            indicate = true; 
            break;
        }
        case GAP_PA_SYNC_STATE_SYNCHRONIZED: {
            BT_API_PRINT(BT_API_DEBUG,"%s GAP_PA_SYNC_STATE_SYNCHRONIZED\r\n",__func__);
            indicate = true;   
            break;
        } 
        case GAP_PA_SYNC_STATE_TERMINATING: {
            indicate = true; 
            BT_API_PRINT(BT_API_DEBUG,"%s GAP_PA_SYNC_STATE_TERMINATING\r\n",__func__);

            break;
        }               
        default:
            break;
    }

    if(indicate) {
        rtk_bt_le_audio_pa_sync_state_ind_t *p_ind = NULL;
        p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                    RTK_BT_LE_AUDIO_EVT_PA_SYNC_STATE_IND,
                                    sizeof(rtk_bt_le_audio_pa_sync_state_ind_t));
        if (!p_evt) {
            BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
            return RTK_BT_FAIL;
        } 
        p_ind = (rtk_bt_le_audio_pa_sync_state_ind_t *)p_evt->data;
        p_ind->cause = p_pa_sync_state->cause;
        p_ind->sync_handle = sync_handle;
        p_ind->sync_state = p_pa_sync_state->sync_state;
        p_ind->action = p_pa_sync_state->action;
        /* Send event */
        rtk_bt_evt_indicate(p_evt, NULL);
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_big_sync_state_change(T_BLE_AUDIO_SYNC_HANDLE sync_handle,T_BLE_AUDIO_BIG_SYNC_STATE *p_big_sync_state)
{
    bool indicate = false;

	rtk_bt_evt_t *p_evt = NULL;

    if(p_big_sync_state == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    } 

    switch (p_big_sync_state->sync_state) {
        case BIG_SYNC_RECEIVER_SYNC_STATE_TERMINATED: {
            BT_API_PRINT(BT_API_DEBUG,"%s BIG_SYNC_RECEIVER_SYNC_STATE_TERMINATED\r\n",__func__);  
            indicate = true;         
            break;
        }
        case BIG_SYNC_RECEIVER_SYNC_STATE_SYNCHRONIZING: {
            BT_API_PRINT(BT_API_DEBUG,"%s BIG_SYNC_RECEIVER_SYNC_STATE_SYNCHRONIZING\r\n",__func__);
            indicate = true; 
            break;
        }
        case BIG_SYNC_RECEIVER_SYNC_STATE_SYNCHRONIZED: {
            BT_API_PRINT(BT_API_DEBUG,"%s BIG_SYNC_RECEIVER_SYNC_STATE_SYNCHRONIZED\r\n",__func__);
            indicate = true;
            break;
        } 
        case BIG_SYNC_RECEIVER_SYNC_STATE_TERMINATING: {
            BT_API_PRINT(BT_API_DEBUG,"%s BIG_SYNC_RECEIVER_SYNC_STATE_TERMINATING\r\n",__func__);
            indicate = true; 
            break;
        }               
        default:
            break;
    }

    if (indicate) {
        rtk_bt_le_audio_big_sync_state_ind_t *p_ind = NULL;
        p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                    RTK_BT_LE_AUDIO_EVT_BIG_SYNC_STATE_IND,
                                    sizeof(rtk_bt_le_audio_big_sync_state_ind_t));
        if (!p_evt) {
            BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
            return RTK_BT_FAIL;
        } 
        p_ind = (rtk_bt_le_audio_big_sync_state_ind_t *)p_evt->data;
        p_ind->cause = p_big_sync_state->cause;
        p_ind->sync_handle = sync_handle;
        p_ind->sync_state = p_big_sync_state->sync_state;
        p_ind->encryption = p_big_sync_state->encryption;
        p_ind->action = p_big_sync_state->action;
        /* Send event */
        rtk_bt_evt_indicate(p_evt, NULL);
    }

    return RTK_BT_OK;
}

static void bt_stack_le_audio_sync_cb(T_BLE_AUDIO_SYNC_HANDLE sync_handle, uint8_t cb_type, void *p_cb_data)
{
	rtk_bt_cmd_t *p_cmd = NULL; 
	rtk_bt_evt_t *p_evt = NULL;

    //BT_API_PRINT(BT_API_DEBUG,"[%s] sync_handle = %p, cb_type = 0x%x\r\n", __func__,sync_handle,cb_type);

    T_BLE_AUDIO_SYNC_CB_DATA *p_sync_cb = (T_BLE_AUDIO_SYNC_CB_DATA *)p_cb_data;
    switch (cb_type) {
        case MSG_BLE_AUDIO_SYNC_HANDLE_RELEASED: {
                APP_PRINT_TRACE1("MSG_BLE_AUDIO_SYNC_HANDLE_RELEASED: action_role %d", p_sync_cb->p_sync_handle_released->action_role);
                BT_API_PRINT(BT_API_DEBUG,"MSG_BLE_AUDIO_SYNC_HANDLE_RELEASED: action_role %d", p_sync_cb->p_sync_handle_released->action_role);
                p_cmd = bt_stack_pending_cmd_search(MSG_BLE_AUDIO_SYNC_HANDLE_RELEASED);
                if (p_cmd) {
                    bt_stack_pending_cmd_delete(p_cmd);
                    p_cmd->ret = 0;
                    osif_sem_give(p_cmd->psem);
                }  
            }
            break;

        case MSG_BLE_AUDIO_ADDR_UPDATE: {
                APP_PRINT_TRACE1("MSG_BLE_AUDIO_ADDR_UPDATE: advertiser_address %s", TRACE_BDADDR(p_sync_cb->p_addr_update->advertiser_address));
                BT_API_PRINT(BT_API_DEBUG,"MSG_BLE_AUDIO_ADDR_UPDATE: advertiser_address %s", TRACE_BDADDR(p_sync_cb->p_addr_update->advertiser_address));
            }
            break;

        case MSG_BLE_AUDIO_PA_SYNC_STATE: {
                PROTOCOL_PRINT_TRACE3("MSG_BLE_AUDIO_PA_SYNC_STATE: sync_state %d, action %d, cause 0x%x\r\n",
                                    p_sync_cb->p_pa_sync_state->sync_state,
                                    p_sync_cb->p_pa_sync_state->action,
                                    p_sync_cb->p_pa_sync_state->cause);
                BT_API_PRINT(BT_API_DEBUG,"MSG_BLE_AUDIO_PA_SYNC_STATE: sync_state %d, action %d, cause 0x%x\r\n",
                                    p_sync_cb->p_pa_sync_state->sync_state,
                                    p_sync_cb->p_pa_sync_state->action,
                                    p_sync_cb->p_pa_sync_state->cause);
                bt_stack_le_audio_pa_sync_state_change(sync_handle,p_sync_cb->p_pa_sync_state);
            }
            break;

        case MSG_BLE_AUDIO_PA_REPORT_INFO: {
                T_LE_PERIODIC_ADV_REPORT_INFO *p_info = p_sync_cb->p_le_periodic_adv_report_info;
                PROTOCOL_PRINT_TRACE5("MSG_BLE_AUDIO_PA_REPORT_INFO: sync_id %d,sync_handle 0x%x,tx_power %d,data_status %d,data_len %d\r\n",
                                    p_info->sync_id,
                                    p_info->sync_handle,
                                    p_info->tx_power,
                                    p_info->data_status,
                                    p_info->data_len); 
                BT_API_PRINT(BT_API_DUMP,"MSG_BLE_AUDIO_PA_REPORT_INFO: sync_id %d,sync_handle 0x%x,tx_power %d,data_status %d,data_len %d\r\n",
                                    p_info->sync_id,
                                    p_info->sync_handle,
                                    p_info->tx_power,
                                    p_info->data_status,
                                    p_info->data_len);                
                
                rtk_bt_le_audio_pa_adv_report_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_PA_ADV_REPORT_IND,
                                            sizeof(rtk_bt_le_audio_pa_adv_report_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_pa_adv_report_ind_t *)p_evt->data;                
                p_ind->sync_handle = sync_handle;
                p_ind->sync_id = p_info->sync_id;
                p_ind->pa_sync_handle = p_info->sync_handle;
                p_ind->tx_power = p_info->tx_power;
                p_ind->rssi = p_info->rssi;
                p_ind->cte_type = p_info->cte_type;
                p_ind->data_status = p_info->data_status;
                p_ind->data_len = p_info->data_len;
                p_ind->p_data = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, p_ind->data_len);
                if (!p_ind->p_data) {
                    BT_API_PRINT(BT_API_ERROR,"p_ind->p_data alloc fail, len = %d\r\n",p_ind->data_len);
                    rtk_bt_event_free(p_evt);
                    break;
                }
                memset(p_ind->p_data, 0, p_ind->data_len);
                memcpy((void *)p_ind->p_data, (void *)p_info->p_data, p_ind->data_len);
                p_evt->user_data = p_ind->p_data;  
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
            }

            break;

        case MSG_BLE_AUDIO_PA_BIGINFO: {
                PROTOCOL_PRINT_TRACE1("MSG_BLE_AUDIO_PA_BIGINFO: num_bis %d\r\n",
                                    p_sync_cb->p_le_biginfo_adv_report_info->num_bis);
                BT_API_PRINT(BT_API_DUMP,"MSG_BLE_AUDIO_PA_BIGINFO: num_bis %d\r\n",
                                    p_sync_cb->p_le_biginfo_adv_report_info->num_bis);
                rtk_bt_le_audio_pa_biginfo_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_PA_BIG_INFO_IND,
                                            sizeof(rtk_bt_le_audio_pa_biginfo_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_pa_biginfo_ind_t *)p_evt->data;
                p_ind->sync_handle = sync_handle;
                //make sure the definition of stack and api for big_info is SAME!!
                memcpy(&p_ind->biginfo,p_sync_cb->p_le_biginfo_adv_report_info,sizeof(rtk_bt_le_audio_biginfo_t));

                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;

        case MSG_BLE_AUDIO_BIG_SYNC_STATE: {
                PROTOCOL_PRINT_TRACE3("MSG_BLE_AUDIO_BIG_SYNC_STATE: sync_state %d, action %d, cause 0x%x\r\n",
                                    p_sync_cb->p_big_sync_state->sync_state,
                                    p_sync_cb->p_big_sync_state->action,
                                    p_sync_cb->p_big_sync_state->cause);
                BT_API_PRINT(BT_API_DEBUG,"MSG_BLE_AUDIO_BIG_SYNC_STATE: sync_state %d, action %d, cause 0x%x\r\n",
                                    p_sync_cb->p_big_sync_state->sync_state,
                                    p_sync_cb->p_big_sync_state->action,
                                    p_sync_cb->p_big_sync_state->cause);
                bt_stack_le_audio_big_sync_state_change(sync_handle,p_sync_cb->p_big_sync_state);
            }
            break;

        case MSG_BLE_AUDIO_BIG_SETUP_DATA_PATH: {
                //FIX TODO the direction shall be input
                PROTOCOL_PRINT_TRACE2("MSG_BLE_AUDIO_BIG_SETUP_DATA_PATH: bis_idx %d, cause 0x%x\r\n",
                                    p_sync_cb->p_setup_data_path->bis_idx,
                                    p_sync_cb->p_setup_data_path->cause);
                BT_API_PRINT(BT_API_DEBUG,"MSG_BLE_AUDIO_BIG_SETUP_DATA_PATH: bis_idx %d, cause 0x%x\r\n",
                                    p_sync_cb->p_setup_data_path->bis_idx,
                                    p_sync_cb->p_setup_data_path->cause);
                p_cmd = bt_stack_pending_cmd_search(RTK_BT_LE_AUDIO_ACT_SYNC_SETUP_DATA_PATH);
                if (p_cmd) {
                    bt_stack_pending_cmd_delete(p_cmd);
                    p_cmd->ret = p_sync_cb->p_setup_data_path->cause;
                    osif_sem_give(p_cmd->psem);
                } else {
                    BT_API_PRINT(BT_API_ERROR,"[%s] MSG_BLE_AUDIO_BIG_SETUP_DATA_PATH: find no pending command \r\n", __func__);
                }                                    
            }
            break;

        case MSG_BLE_AUDIO_BIG_REMOVE_DATA_PATH: {
                PROTOCOL_PRINT_TRACE2("MSG_BLE_AUDIO_BIG_REMOVE_DATA_PATH: bis_idx %d, cause 0x%x\r\n",
                                    p_sync_cb->p_remove_data_path->bis_idx,
                                    p_sync_cb->p_remove_data_path->cause);
                BT_API_PRINT(BT_API_DEBUG,"MSG_BLE_AUDIO_BIG_REMOVE_DATA_PATH: bis_idx %d, cause 0x%x\r\n",
                                    p_sync_cb->p_remove_data_path->bis_idx,
                                    p_sync_cb->p_remove_data_path->cause);
                p_cmd = bt_stack_pending_cmd_search(RTK_BT_LE_AUDIO_ACT_SYNC_REMOVE_DATA_PATH);
                if (p_cmd) {
                    bt_stack_pending_cmd_delete(p_cmd);
                    p_cmd->ret = p_sync_cb->p_remove_data_path->cause;
                    osif_sem_give(p_cmd->psem);
                } else {
                    BT_API_PRINT(BT_API_ERROR,"[%s] MSG_BLE_AUDIO_BIG_REMOVE_DATA_PATH: find no pending command \r\n", __func__);
                }                                     
            }
            break;

        case MSG_BLE_AUDIO_BASE_DATA_MODIFY_INFO: {
                APP_PRINT_TRACE1("MSG_BLE_AUDIO_BASE_DATA_MODIFY_INFO: p_base_mapping %p\r\n",
                                    p_sync_cb->p_base_data_modify_info->p_base_mapping);
                BT_API_PRINT(BT_API_DEBUG,"MSG_BLE_AUDIO_BASE_DATA_MODIFY_INFO: p_base_mapping %p\r\n",
                                    p_sync_cb->p_base_data_modify_info->p_base_mapping);
                if(p_sync_cb->p_base_data_modify_info->p_base_mapping) {
                    rtk_bt_le_audio_base_data_mapping_modify_ind_t *p_ind = NULL;
                    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                                RTK_BT_LE_AUDIO_EVT_BASE_DATA_MAPPING_MODIFY_IND,
                                                sizeof(rtk_bt_le_audio_base_data_mapping_modify_ind_t));
                    if (!p_evt) {
                        BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                        break;
                    } 
                    p_ind = (rtk_bt_le_audio_base_data_mapping_modify_ind_t *)p_evt->data;
                    p_ind->sync_handle = sync_handle;
                    p_ind->base_data_mapping = p_sync_cb->p_base_data_modify_info->p_base_mapping;
                    /* Send event */
                    rtk_bt_evt_indicate(p_evt, NULL);
                }               
            }
            break;
        default:
            BT_API_PRINT(BT_API_ERROR,"%s unhandled cb_type 0x%x\r\n", __func__,cb_type);
            break;
        }
    return;
}
#endif 
#if (defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT)
static rtk_bt_le_audio_bass_brs_data_t * rtk_stack_bass_brs_data_copy(T_BASS_BRS_DATA *p_src_brs_data) 
{
    rtk_bt_le_audio_bass_brs_data_t *p_dst_brs_data = NULL;    
    uint16_t len_offset = 0;
    uint16_t size = 0;

    if (!p_src_brs_data) {
        return NULL;
    }  

    size = sizeof(rtk_bt_le_audio_bass_brs_data_t) + p_src_brs_data->bis_info_size;
    p_dst_brs_data = (rtk_bt_le_audio_bass_brs_data_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, size);
    if (!p_dst_brs_data) {
        BT_API_PRINT(BT_API_ERROR,"p_dst_brs_data-> alloc fail, len = %d\r\n",size);
        return NULL;
    }   

    memset(p_dst_brs_data, 0, size);                  
    p_dst_brs_data->brs_is_used = p_src_brs_data->brs_is_used;   
    p_dst_brs_data->source_id = p_src_brs_data->source_id;         
    p_dst_brs_data->source_address.type = p_src_brs_data->source_address_type;
    memcpy(p_dst_brs_data->source_address.addr_val,p_src_brs_data->source_address,RTK_BD_ADDR_LEN);
    p_dst_brs_data->source_adv_sid = p_src_brs_data->source_adv_sid;
    memcpy(p_dst_brs_data->broadcast_id, p_src_brs_data->broadcast_id, RTK_BT_LE_AUDIO_BROADCAST_ID_LEN);
    p_dst_brs_data->pa_sync_state = p_src_brs_data->pa_sync_state;
    p_dst_brs_data->big_encryption = p_src_brs_data->big_encryption;
    memcpy(p_dst_brs_data->bad_code, p_src_brs_data->bad_code, RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN);
    p_dst_brs_data->num_subgroups = p_src_brs_data->num_subgroups;
    p_dst_brs_data->bis_info_size = p_src_brs_data->bis_info_size; 
    
    if(p_src_brs_data->bis_info_size) {        
        p_dst_brs_data->p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)((uint8_t*)p_dst_brs_data + sizeof(rtk_bt_le_audio_bass_brs_data_t));                                                            
        for(uint8_t i = 0 ; i < p_src_brs_data->num_subgroups; i++) { 
            p_dst_brs_data->p_cp_bis_info[i].bis_sync_state = p_src_brs_data->p_cp_bis_info[i].bis_sync;
            p_dst_brs_data->p_cp_bis_info[i].metadata_len = p_src_brs_data->p_cp_bis_info[i].metadata_len;
            len_offset += sizeof(rtk_bt_le_audio_bass_cp_bis_info_t); 
            p_dst_brs_data->p_cp_bis_info[i].p_metadata = (uint8_t *)((uint8_t*)p_dst_brs_data->p_cp_bis_info + len_offset);
            memcpy(p_dst_brs_data->p_cp_bis_info[i].p_metadata, p_src_brs_data->p_cp_bis_info[i].p_metadata, p_dst_brs_data->p_cp_bis_info[i].metadata_len);
            len_offset += p_dst_brs_data->p_cp_bis_info[i].metadata_len;
            if(len_offset > p_src_brs_data->bis_info_size)  {
                BT_API_PRINT(BT_API_ERROR,"alloc size(%d) less than actually len(%d)\r\n",p_src_brs_data->bis_info_size,len_offset);
                osif_mem_free(p_dst_brs_data);
                return NULL;
            }  
            BT_API_DUMPBUF(BT_API_DEBUG,__func__,p_dst_brs_data->p_cp_bis_info[i].p_metadata,p_dst_brs_data->p_cp_bis_info[i].metadata_len);          
        }
    }
    BT_API_DUMPBUF(BT_API_DUMP,__func__,(uint8_t *)p_dst_brs_data,sizeof(rtk_bt_le_audio_bass_brs_data_t) + p_src_brs_data->bis_info_size); 
    return p_dst_brs_data;
}

static rtk_bt_le_audio_bass_cp_bis_info_t * rtk_stack_bass_cp_bis_info_copy(T_BASS_CP_BIS_INFO *p_src_bis_info,uint16_t bis_info_size,uint8_t num_subgroups) 
{
    rtk_bt_le_audio_bass_cp_bis_info_t *p_dst_bis_info = NULL;    
    uint16_t len_offset = 0;

    if (!p_src_bis_info) {
        return NULL;
    }  

    p_dst_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, bis_info_size);
    if (!p_dst_bis_info) {
        BT_API_PRINT(BT_API_ERROR,"p_dst_bis_info alloc fail, len = %d\r\n",bis_info_size);
        return NULL;
    } 
    memset(p_dst_bis_info, 0, bis_info_size);                  
                                                          
    for(uint8_t i = 0 ; i < num_subgroups; i++) { 
        p_dst_bis_info[i].bis_sync_state = p_src_bis_info[i].bis_sync;
        p_dst_bis_info[i].metadata_len = p_src_bis_info[i].metadata_len;
        len_offset += sizeof(rtk_bt_le_audio_bass_cp_bis_info_t); 
        p_dst_bis_info[i].p_metadata = (uint8_t *)((uint8_t*)p_dst_bis_info + len_offset);
        memcpy(p_dst_bis_info[i].p_metadata, p_src_bis_info[i].p_metadata, p_dst_bis_info[i].metadata_len);
        len_offset += p_dst_bis_info[i].metadata_len;
        if(len_offset > bis_info_size)  {
            BT_API_PRINT(BT_API_ERROR,"alloc size(%d) less than actually len(%d)\r\n",bis_info_size,len_offset);
            osif_mem_free(p_dst_bis_info);
            return NULL;
        }  
        BT_API_DUMPBUF(BT_API_DEBUG,__func__,p_dst_bis_info[i].p_metadata,p_dst_bis_info[i].metadata_len);          
    }

    return p_dst_bis_info;
}

#if defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT
static uint16_t rtk_stack_bass_cp_action(T_BASS_CP_IND *p_data)
{
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
    rtk_bt_evt_t *p_evt = NULL;

    APP_PRINT_INFO2("LE_AUDIO_MSG_BASS_CP_IND: conn_handle 0x%x, cp_op 0x%x",p_data->conn_handle, p_data->p_cp_data->cp_op);
    BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_CP_IND: conn_handle 0x%x, cp_op 0x%x\r\n", p_data->conn_handle, p_data->p_cp_data->cp_op);

    rtk_bt_le_audio_bass_cp_ind_t *p_ind = NULL;
    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                RTK_BT_LE_AUDIO_EVT_BASS_CP_IND,
                                sizeof(rtk_bt_le_audio_bass_cp_ind_t));
    if (!p_evt) {
        BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
        return BLE_AUDIO_CB_RESULT_REJECT;
    } 
    p_ind = (rtk_bt_le_audio_bass_cp_ind_t *)p_evt->data;
    p_ind->conn_handle = p_data->conn_handle;
    p_ind->opcode = p_data->p_cp_data->cp_op;

    switch (p_data->p_cp_data->cp_op) {
        case BASS_CP_OP_ADD_SOURCE: {
            p_ind->op_param.add_source_param.adv_addr.type = p_data->p_cp_data->param.add_source.advertiser_address_type;
            memcpy(p_ind->op_param.add_source_param.adv_addr.addr_val, p_data->p_cp_data->param.add_source.advertiser_address,RTK_BD_ADDR_LEN);
            memcpy(p_ind->op_param.add_source_param.broadcast_id, p_data->p_cp_data->param.add_source.broadcast_id,RTK_BT_LE_AUDIO_BROADCAST_ID_LEN);
            p_ind->op_param.add_source_param.pa_sync = (rtk_bt_le_audio_bass_cp_pa_sync_t) p_data->p_cp_data->param.add_source.pa_sync;
            p_ind->op_param.add_source_param.pa_interval = p_data->p_cp_data->param.add_source.pa_interval;
            p_ind->op_param.add_source_param.num_subgroups = p_data->p_cp_data->param.add_source.num_subgroups;
            p_ind->op_param.add_source_param.bis_info_size = p_data->p_cp_data->param.add_source.bis_info_size;
#if 0
            p_ind->op_param.add_source_param.p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, p_data->p_cp_data->param.add_source.bis_info_size);
            if (!p_ind->op_param.add_source_param.p_cp_bis_info) {
                BT_API_PRINT(BT_API_ERROR,"p_ind->op_param.add_source_param.p_cp_bis_info fail, len = %d\r\n",p_data->p_cp_data->param.add_source.bis_info_size);
                rtk_bt_event_free(p_evt);
                break;
            }
            memset(p_ind->op_param.add_source_param.p_cp_bis_info,0,p_data->p_cp_data->param.add_source.bis_info_size);
            memcpy((void *)p_ind->op_param.add_source_param.p_cp_bis_info, (void *)p_data->p_cp_data->param.add_source.p_cp_bis_info,p_data->p_cp_data->param.add_source.bis_info_size);
#else 
            p_ind->op_param.add_source_param.p_cp_bis_info = rtk_stack_bass_cp_bis_info_copy(p_data->p_cp_data->param.add_source.p_cp_bis_info,
                                                                                            p_data->p_cp_data->param.add_source.bis_info_size,
                                                                                            p_data->p_cp_data->param.add_source.num_subgroups);
            if (!p_ind->op_param.add_source_param.p_cp_bis_info) {
                BT_API_PRINT(BT_API_ERROR,"p_ind->op_param.add_source_param.p_cp_bis_info fail, len = %d\r\n",p_data->p_cp_data->param.add_source.bis_info_size);
                rtk_bt_event_free(p_evt);
                break;
            }
#endif
            p_evt->user_data = p_ind->op_param.add_source_param.p_cp_bis_info; 
            
            break;
        }
        case BASS_CP_OP_MODIFY_SOURCE: {
            p_ind->op_param.modify_source_param.source_id = p_data->p_cp_data->param.modify_source.source_id;
            p_ind->op_param.modify_source_param.pa_sync = (rtk_bt_le_audio_bass_cp_pa_sync_t) p_data->p_cp_data->param.modify_source.pa_sync;
            p_ind->op_param.modify_source_param.pa_interval = p_data->p_cp_data->param.modify_source.pa_interval;
            p_ind->op_param.modify_source_param.num_subgroups = p_data->p_cp_data->param.modify_source.num_subgroups;
            p_ind->op_param.modify_source_param.bis_info_size = p_data->p_cp_data->param.modify_source.bis_info_size;
#if 0            
            p_ind->op_param.modify_source_param.p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, p_data->p_cp_data->param.modify_source.bis_info_size);
            if (!p_ind->op_param.modify_source_param.p_cp_bis_info) {
                BT_API_PRINT(BT_API_ERROR,"p_ind->op_param.modify_source_param.p_cp_bis_info fail, len = %d\r\n",p_data->p_cp_data->param.modify_source.bis_info_size);
                rtk_bt_event_free(p_evt);
                break;
            }
            memset(p_ind->op_param.modify_source_param.p_cp_bis_info,0,p_data->p_cp_data->param.modify_source.bis_info_size);
            memcpy((void *)p_ind->op_param.modify_source_param.p_cp_bis_info, (void *)p_data->p_cp_data->param.modify_source.p_cp_bis_info,p_data->p_cp_data->param.modify_source.bis_info_size);
#else 
            p_ind->op_param.modify_source_param.p_cp_bis_info = rtk_stack_bass_cp_bis_info_copy(p_data->p_cp_data->param.modify_source.p_cp_bis_info,
                                                                                            p_data->p_cp_data->param.modify_source.bis_info_size,
                                                                                            p_data->p_cp_data->param.modify_source.num_subgroups);
            if (!p_ind->op_param.modify_source_param.p_cp_bis_info) {
                BT_API_PRINT(BT_API_ERROR,"p_ind->op_param.modify_source_param.p_cp_bis_info fail, len = %d\r\n",p_data->p_cp_data->param.modify_source.bis_info_size);
                rtk_bt_event_free(p_evt);
                break;
            }
#endif
            p_evt->user_data = p_ind->op_param.modify_source_param.p_cp_bis_info; 
            break;
        }

        case BASS_CP_OP_REMOVE_SOURCE:{
            p_ind->op_param.remove_source_param.source_id = p_data->p_cp_data->param.remove_source.source_id; 
            break;
        }

        case BASS_CP_OP_SET_BROADCAST_CODE: {
            p_ind->op_param.set_broadcast_code_param.source_id = p_data->p_cp_data->param.set_broadcast_code.source_id;
            memcpy(p_ind->op_param.set_broadcast_code_param.broadcast_code, p_data->p_cp_data->param.set_broadcast_code.broadcast_code,RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN);
#if 1
            /* The scan delegator will set broadcast code from broadcast assistant. */
            BT_API_DUMPBUF(BT_API_DEBUG,__func__,p_data->p_cp_data->param.set_broadcast_code.broadcast_code,RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN);
            if(false == bass_cfg_broadcast_code(p_data->p_cp_data->param.set_broadcast_code.source_id, p_data->p_cp_data->param.set_broadcast_code.broadcast_code)) {
                BT_API_PRINT(BT_API_ERROR,"%s: bass_cfg_broadcast_code fail!\r\n", __func__); 
            }
#endif
            break;
        }
        
        default:
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }
    /* Send event */
    rtk_bt_evt_indicate(p_evt, NULL);

    return app_result;
}

static uint16_t rtk_stack_bass_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
    rtk_bt_evt_t *p_evt = NULL;
    uint8_t cb_ret = 0;

    //BT_API_PRINT(BT_API_DEBUG,"%s: msg = 0x%x\r\n",__func__,msg);
    switch (msg) {
        case LE_AUDIO_MSG_BASS_CP_IND: {
                app_result = rtk_stack_bass_cp_action((T_BASS_CP_IND *)buf);
            }
            break;

        case LE_AUDIO_MSG_BASS_BRS_MODIFY: {
                T_BASS_BRS_MODIFY *p_data = (T_BASS_BRS_MODIFY *)buf;
                APP_PRINT_INFO2("LE_AUDIO_MSG_BASS_BRS_MODIFY: sync handle %p, source_id %d", p_data->handle, p_data->source_id);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_BRS_MODIFY: sync handle %p, source_id %d\r\n", p_data->handle, p_data->source_id);
                if (p_data->p_brs_data) {
                    if (p_data->p_brs_data->brs_is_used) {
                        APP_PRINT_INFO2("source_address_type 0x%02x, source_adv_sid 0x%02x",
                                        p_data->p_brs_data->source_address_type, p_data->p_brs_data->source_adv_sid);
                        APP_PRINT_INFO4("pa_sync_state %d, bis_sync_state 0x%x, big_encryption %d, num_subgroups %d",
                                        p_data->p_brs_data->pa_sync_state, p_data->p_brs_data->bis_sync_state,
                                        p_data->p_brs_data->big_encryption, p_data->p_brs_data->num_subgroups);
                        for (uint8_t i = 0; i < p_data->p_brs_data->num_subgroups; i++)
                        {
                            if (p_data->p_brs_data->p_cp_bis_info[i].metadata_len == 0) {
                                APP_PRINT_INFO2("Subgroup[%d], BIS Sync: [0x%x], Metadata Length: [0]", i,
                                                p_data->p_brs_data->p_cp_bis_info[i].bis_sync);
                            } else {
                                APP_PRINT_INFO4("Subgroup[%d], BIS Sync: [0x%x] Metadata data[%d]: %b", i,
                                                p_data->p_brs_data->p_cp_bis_info[i].bis_sync,
                                                p_data->p_brs_data->p_cp_bis_info[i].metadata_len,
                                                TRACE_BINARY(p_data->p_brs_data->p_cp_bis_info[i].metadata_len,
                                                            p_data->p_brs_data->p_cp_bis_info[i].p_metadata));
                            }
                        }
                    } 
                }
                //p_data->handle is 0 when release ,so not indicate it
                if(p_data->handle) {
                    rtk_bt_le_audio_bass_brs_modify_ind_t *p_ind = NULL;
                    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                                RTK_BT_LE_AUDIO_EVT_BASS_BRS_MODIFY_IND,
                                                sizeof(rtk_bt_le_audio_bass_brs_modify_ind_t));
                    if (!p_evt) {
                        BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                        break;
                    } 
                    p_ind = (rtk_bt_le_audio_bass_brs_modify_ind_t *)p_evt->data;
                    p_ind->sync_handle = p_data->handle;
                    p_ind->source_id = p_data->source_id;
                    if(p_data->p_brs_data) {
                        p_ind->p_brs_data = rtk_stack_bass_brs_data_copy(p_data->p_brs_data);
                        if(p_ind->p_brs_data == NULL) {
                            rtk_bt_event_free(p_evt); 
                            break;                   
                        }                  
                        p_evt->user_data = p_ind->p_brs_data; 
                    }
                    /* Send event */
                    rtk_bt_evt_indicate(p_evt, NULL);
                } 
            }
            break;

        case LE_AUDIO_MSG_BASS_BA_ADD_SOURCE: {
                T_BASS_BA_ADD_SOURCE *p_data = (T_BASS_BA_ADD_SOURCE *)buf;
                APP_PRINT_INFO2("LE_AUDIO_MSG_BASS_BA_ADD_SOURCE: sync handle %p, source_id %d",
                                p_data->handle, p_data->source_id);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_BA_ADD_SOURCE: sync handle %p, source_id %d\r\n",
                                p_data->handle, p_data->source_id);
                if(false == ble_audio_sync_update_cb(p_data->handle, bt_stack_le_audio_sync_cb)) {
                    BT_API_PRINT(BT_API_ERROR,"%s: ble_audio_sync_update_cb fail!\r\n", __func__);
                }
                rtk_bt_le_audio_bass_ba_add_source_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_BASS_BA_ADD_SOURCE_IND,
                                            sizeof(rtk_bt_le_audio_bass_ba_add_source_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_bass_ba_add_source_ind_t *)p_evt->data;
                p_ind->sync_handle = p_data->handle;
                p_ind->source_id = p_data->source_id;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;

        case LE_AUDIO_MSG_BASS_LOCAL_ADD_SOURCE: {
                T_BASS_LOCAL_ADD_SOURCE *p_data = (T_BASS_LOCAL_ADD_SOURCE *)buf;
                APP_PRINT_INFO2("LE_AUDIO_MSG_BASS_LOCAL_ADD_SOURCE: sync handle %p, source_id %d",
                                p_data->handle, p_data->source_id);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_LOCAL_ADD_SOURCE: sync handle %p, source_id %d\r\n",
                                p_data->handle, p_data->source_id);
                rtk_bt_le_audio_bass_ba_add_source_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_BASS_BA_ADD_SOURCE_IND,
                                            sizeof(rtk_bt_le_audio_bass_ba_add_source_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_bass_ba_add_source_ind_t *)p_evt->data;
                p_ind->sync_handle = p_data->handle;
                p_ind->source_id = p_data->source_id;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;

        case LE_AUDIO_MSG_BASS_GET_PA_SYNC_PARAM: {
                T_BASS_GET_PA_SYNC_PARAM *p_data = (T_BASS_GET_PA_SYNC_PARAM *)buf;
                APP_PRINT_INFO4("LE_AUDIO_MSG_BASS_GET_PA_SYNC_PARAM: sync handle %p, source_id %d, is_past %d, pa_interval 0x%x",
                                p_data->handle, p_data->source_id, p_data->is_past, p_data->pa_interval);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_GET_PA_SYNC_PARAM: sync handle %p, source_id %d, is_past %d, pa_interval 0x%x\r\n",
                                p_data->handle, p_data->source_id, p_data->is_past, p_data->pa_interval);             
                rtk_bt_le_audio_bass_get_pa_sync_param_ind_t *p_ind = NULL;
                rtk_bt_le_audio_bass_pa_sync_param_t pa_sync_param = {0};
                if (false == rtk_bt_check_evt_cb_direct_calling(RTK_BT_LE_GP_AUDIO, RTK_BT_LE_AUDIO_EVT_BASS_GET_PA_SYNC_PARAM_IND)) { 
                    BT_API_PRINT(BT_API_ERROR,"%s: RTK_BT_LE_AUDIO_EVT_BASS_GET_PA_SYNC_PARAM_IND is not direct calling!\r\n", __func__);
                    break;
                }

                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_BASS_GET_PA_SYNC_PARAM_IND,
                                            sizeof(rtk_bt_le_audio_bass_get_pa_sync_param_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_bass_get_pa_sync_param_ind_t *)p_evt->data;
                p_ind->sync_handle = p_data->handle;
                p_ind->source_id = p_data->source_id;
                p_ind->is_past = p_data->is_past;
                p_ind->pa_interval = p_data->pa_interval;
                p_ind->p_pa_sync_param = &pa_sync_param;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, &cb_ret);
                if (cb_ret == RTK_BT_EVT_CB_OK) {
                    BT_API_PRINT(BT_API_DEBUG,"%s: RTK_BT_LE_AUDIO_EVT_BASS_GET_PA_SYNC_PARAM_IND, sync_handle = %p,source_id = %d,pa_sync_options = %d,pa_sync_skip = %d,pa_sync_timeout = %d,past_timeout = %d\r\n", __func__,
                                    p_data->handle,p_data->source_id,
                                    pa_sync_param.pa_sync_options,pa_sync_param.pa_sync_skip,pa_sync_param.pa_sync_timeout,pa_sync_param.past_timeout);
                    if(false == bass_cfg_pa_sync_param(p_data->source_id, pa_sync_param.pa_sync_options, pa_sync_param.pa_sync_skip, pa_sync_param.pa_sync_timeout, pa_sync_param.past_timeout))
                        BT_API_PRINT(BT_API_ERROR,"%s: bass_cfg_pa_sync_param fail!\r\n", __func__); 
                }
            }
            break;

        case LE_AUDIO_MSG_BASS_GET_BIG_SYNC_PARAM: {
                T_BASS_GET_BIG_SYNC_PARAM *p_data = (T_BASS_GET_BIG_SYNC_PARAM *)buf;
                APP_PRINT_INFO2("LE_AUDIO_MSG_BASS_GET_BIG_SYNC_PARAM: sync handle %p, source_id %d",
                                p_data->handle, p_data->source_id);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_GET_BIG_SYNC_PARAM: sync handle %p, source_id %d\r\n",
                                p_data->handle, p_data->source_id);      
                rtk_bt_le_audio_bass_get_big_sync_param_ind_t *p_ind = NULL;
                rtk_bt_le_audio_bass_big_sync_param_t big_sync_param = {0};
                if (false == rtk_bt_check_evt_cb_direct_calling(RTK_BT_LE_GP_AUDIO, RTK_BT_LE_AUDIO_EVT_BASS_GET_BIG_SYNC_PARAM_IND)) { 
                    BT_API_PRINT(BT_API_ERROR,"%s: RTK_BT_LE_AUDIO_EVT_BASS_GET_BIG_SYNC_PARAM_IND is not direct calling!\r\n", __func__);
                    break;
                }

                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_BASS_GET_BIG_SYNC_PARAM_IND,
                                            sizeof(rtk_bt_le_audio_bass_get_big_sync_param_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                }                                            
                p_ind = (rtk_bt_le_audio_bass_get_big_sync_param_ind_t *)p_evt->data;
                p_ind->sync_handle = p_data->handle;
                p_ind->source_id = p_data->source_id;
                p_ind->p_big_sync_param = &big_sync_param;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, &cb_ret);
                if (cb_ret == RTK_BT_EVT_CB_OK) {
                    BT_API_PRINT(BT_API_DEBUG,"%s: RTK_BT_LE_AUDIO_EVT_BASS_GET_BIG_SYNC_PARAM_IND big_mse=%d,big_sync_timeout_10=%d\r\n",__func__,big_sync_param.big_mse, big_sync_param.big_sync_timeout);
                    if(false == bass_cfg_big_sync_param(p_data->source_id, big_sync_param.big_mse, big_sync_param.big_sync_timeout))
                        BT_API_PRINT(BT_API_ERROR,"%s: bass_cfg_big_sync_param fail!\r\n", __func__); 
                }
            }
            break;

        case LE_AUDIO_MSG_BASS_GET_BROADCAST_CODE: {
                T_BASS_GET_BROADCAST_CODE *p_data = (T_BASS_GET_BROADCAST_CODE *)buf;
                APP_PRINT_INFO2("LE_AUDIO_MSG_BASS_GET_BROADCAST_CODE: sync handle %p, source_id %d",
                                p_data->handle, p_data->source_id);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_GET_BROADCAST_CODE: sync handle %p, source_id %d\r\n",
                                p_data->handle, p_data->source_id);               
                rtk_bt_le_audio_bass_get_broadcast_code_ind_t *p_ind = NULL;
                uint8_t broadcast_code[RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN] = {0}; 
                if (false == rtk_bt_check_evt_cb_direct_calling(RTK_BT_LE_GP_AUDIO, RTK_BT_LE_AUDIO_EVT_BASS_GET_BROADCAST_CODE_IND)) { 
                    BT_API_PRINT(BT_API_ERROR,"%s: RTK_BT_LE_AUDIO_EVT_BASS_GET_BROADCAST_CODE_IND is not direct calling!\r\n", __func__); 
                    break;
                }

                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_BASS_GET_BROADCAST_CODE_IND,
                                            sizeof(rtk_bt_le_audio_bass_get_broadcast_code_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_bass_get_broadcast_code_ind_t *)p_evt->data;
                p_ind->sync_handle = p_data->handle;
                p_ind->source_id = p_data->source_id;
                p_ind->p_broadcast_code = broadcast_code;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, &cb_ret);
#if 0
                /* The scan delegtor will set broadcast code obtained from APP, and not send Broadcast code request to broadcast assistant. */
                if (cb_ret == RTK_BT_EVT_CB_OK) {
                    BT_API_DUMPBUF(BT_API_DEBUG,__func__,broadcast_code,RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN);
                    if(false == bass_cfg_broadcast_code(p_data->source_id, broadcast_code))
                        BT_API_PRINT(BT_API_ERROR,"%s: bass_cfg_broadcast_code fail!\r\n", __func__);
                }
#endif
            }
            break;

        case LE_AUDIO_MSG_BASS_GET_PREFER_BIS_SYNC: {
                T_BASS_SET_PREFER_BIS_SYNC *p_data = (T_BASS_SET_PREFER_BIS_SYNC *)buf;
                APP_PRINT_INFO3("LE_AUDIO_MSG_BASS_GET_PREFER_BIS_SYNC: sync handle %p, source_id %d, num_subgroups %d",
                                p_data->handle, p_data->source_id,
                                p_data->num_subgroups);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_GET_PREFER_BIS_SYNC: sync handle %p, source_id %d, num_subgroups %d\r\n",
                                p_data->handle, p_data->source_id, p_data->num_subgroups);  

                T_BLE_AUDIO_SYNC_INFO sync_info = {0};
                rtk_bt_le_audio_bass_get_prefer_bis_sync_ind_t *p_ind = NULL;
                uint32_t support_bis_array = 0,select_bis_array = 0;            
                if (false == rtk_bt_check_evt_cb_direct_calling(RTK_BT_LE_GP_AUDIO, RTK_BT_LE_AUDIO_EVT_BASS_GET_PREFER_BIS_SYNC_IND)) { 
                    BT_API_PRINT(BT_API_ERROR,"%s: RTK_BT_LE_AUDIO_EVT_BASS_GET_PREFER_BIS_SYNC_IND is not direct calling!\r\n", __func__);
                    break;
                }

                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_BASS_GET_PREFER_BIS_SYNC_IND,
                                            sizeof(rtk_bt_le_audio_bass_get_prefer_bis_sync_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_bass_get_prefer_bis_sync_ind_t *)p_evt->data;
                p_ind->sync_handle = p_data->handle;
                p_ind->source_id = p_data->source_id;
                p_ind->p_support_bis_array = &support_bis_array;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, &cb_ret);
                if (cb_ret == RTK_BT_EVT_CB_OK) {
                    BT_API_PRINT(BT_API_DEBUG,"%s get support_bis_array=%d\r\n",__func__,(unsigned int)support_bis_array);
                    if(ble_audio_sync_get_info(p_data->handle, &sync_info)) {
                        if (sync_info.p_base_mapping) {
                            for (uint8_t i = 0; i < p_data->num_subgroups; i++) {
                                if (p_data->p_cp_bis_info[i].bis_sync == BASS_CP_BIS_SYNC_NO_PREFER) {                                        
                                    if (i < sync_info.p_base_mapping->num_subgroups) {
                                        select_bis_array = (support_bis_array & sync_info.p_base_mapping->p_subgroup[i].bis_array);
                                        if (select_bis_array != 0) {
                                            APP_PRINT_INFO2("BASE Mapping: bis_array 0x%x, select bis_array 0x%x",
                                                            sync_info.p_base_mapping->p_subgroup[i].bis_array, select_bis_array);
                                            BT_API_PRINT(BT_API_DEBUG,"BASE Mapping: bis_array 0x%x, select bis_array 0x%x\r\n",
                                                            (unsigned int)sync_info.p_base_mapping->p_subgroup[i].bis_array, (unsigned int)select_bis_array);
                                            bass_cfg_prefer_bis_sync(p_data->source_id, i, select_bis_array);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;

        case LE_AUDIO_MSG_BASS_BRS_CHAR_NO_EMPTY: {
                T_BASS_BRS_CHAR_NO_EMPTY *p_data = (T_BASS_BRS_CHAR_NO_EMPTY *)buf;
                APP_PRINT_INFO1("LE_AUDIO_MSG_BASS_BRS_CHAR_NO_EMPTY:conn_handle 0x%x",p_data->conn_handle);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_BRS_CHAR_NO_EMPTY:conn_handle 0x%x\r\n",p_data->conn_handle);
            }
            break;

        default:
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process!\r\n", __func__,msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }
    return app_result;
}
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
static uint16_t rtk_stack_bass_client_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    rtk_bt_cmd_t *p_cmd = NULL; 
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
    //BT_API_PRINT(BT_API_DEBUG,"%s: msg = 0x%x\r\n",__func__,msg);

    switch (msg) {
        case LE_AUDIO_MSG_BASS_CLIENT_CP_RESULT: {
            T_BASS_CLIENT_CP_RESULT *p_data = (T_BASS_CLIENT_CP_RESULT *)buf;
            APP_PRINT_INFO2("LE_AUDIO_MSG_BASS_CLIENT_CP_RESULT: conn_handle 0x%x, cause 0x%x", p_data->conn_handle, p_data->cause);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_CLIENT_CP_RESULT: conn_handle 0x%x, cause 0x%x\r\n", p_data->conn_handle, p_data->cause);
            p_cmd = bt_stack_pending_cmd_search(RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_WRITE_CP_OP);
            if (p_cmd) {
                bt_stack_pending_cmd_delete(p_cmd);
                p_cmd->ret = p_data->cause;
                osif_sem_give(p_cmd->psem);
            } else {
                BT_API_PRINT(BT_API_ERROR,"[%s] LE_AUDIO_MSG_BASS_CLIENT_CP_RESULT: find no pending command \r\n", __func__);
            }        
        }
        break;

        case LE_AUDIO_MSG_BASS_CLIENT_SYNC_INFO_REQ: {
            T_BASS_CLIENT_SYNC_INFO_REQ *p_data = (T_BASS_CLIENT_SYNC_INFO_REQ *)buf;
            APP_PRINT_INFO2("LE_AUDIO_MSG_BASS_CLIENT_SYNC_INFO_REQ: conn_handle 0x%x, char_instance_id %d",p_data->conn_handle,p_data->char_instance_id);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_CLIENT_SYNC_INFO_REQ: conn_handle 0x%x, char_instance_id %d\r\n",p_data->conn_handle,p_data->char_instance_id);
            rtk_bt_le_audio_bass_client_sync_info_req_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_BASS_CLIENT_SYNC_INFO_REQ_IND,
                                        sizeof(rtk_bt_le_audio_bass_client_sync_info_req_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_bass_client_sync_info_req_ind_t *)p_evt->data;
            p_ind->conn_handle = p_data->conn_handle;
            p_ind->char_instance_id = p_data->char_instance_id;            
            if(p_data->p_brs_data) {
                p_ind->p_brs_data = rtk_stack_bass_brs_data_copy(p_data->p_brs_data);
                if(p_ind->p_brs_data == NULL) {
                    rtk_bt_event_free(p_evt); 
                    break;                   
                }            
                p_evt->user_data = p_ind->p_brs_data; 
            }
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;

        case LE_AUDIO_MSG_BASS_CLIENT_BRS_DATA: {
            T_BASS_CLIENT_BRS_DATA *p_data = (T_BASS_CLIENT_BRS_DATA *)buf;
            APP_PRINT_INFO4("LE_AUDIO_MSG_BASS_CLIENT_BRS_DATA: conn_handle 0x%x, notify %d, read_cause 0x%x, char_instance_id %d",
                            p_data->conn_handle, p_data->notify, p_data->read_cause, p_data->char_instance_id);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_CLIENT_BRS_DATA: conn_handle 0x%x, notify %d, read_cause 0x%x, char_instance_id %d\r\n",
                            p_data->conn_handle, p_data->notify, p_data->read_cause, p_data->char_instance_id);
            if (p_data->p_brs_data && p_data->p_brs_data->brs_is_used) {
                APP_PRINT_INFO7("LE_AUDIO_MSG_BASS_CLIENT_BRS_DATA: source_id %d, source_address_type 0x%x, source_address %s, source_adv_sid %d, pa_sync_state %d, bis_sync_state 0x%x, big_encryption %d",
                                p_data->p_brs_data->source_id,
                                p_data->p_brs_data->source_address_type,
                                TRACE_BDADDR(p_data->p_brs_data->source_address),
                                p_data->p_brs_data->source_adv_sid,
                                p_data->p_brs_data->pa_sync_state,
                                p_data->p_brs_data->bis_sync_state,
                                p_data->p_brs_data->big_encryption);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_CLIENT_BRS_DATA: source_id %d, source_adv_sid %d, pa_sync_state %d, bis_sync_state 0x%x, big_encryption %d\r\n",
                                p_data->p_brs_data->source_id,
                                p_data->p_brs_data->source_adv_sid,
                                p_data->p_brs_data->pa_sync_state,
                                (unsigned int)p_data->p_brs_data->bis_sync_state,
                                p_data->p_brs_data->big_encryption);
                if (p_data->p_brs_data->num_subgroups) {
                    APP_PRINT_INFO4("LE_AUDIO_MSG_BASS_CLIENT_BRS_DATA: num_subgroups %d, bis_info_size %d, bis_sync 0x%x, metadata_len %d",
                                    p_data->p_brs_data->num_subgroups,
                                    p_data->p_brs_data->bis_info_size,
                                    p_data->p_brs_data->p_cp_bis_info[0].bis_sync,
                                    p_data->p_brs_data->p_cp_bis_info[0].metadata_len);
                    BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_BASS_CLIENT_BRS_DATA: num_subgroups %d, bis_info_size %d, bis_sync 0x%x, metadata_len %d\r\n",
                                    p_data->p_brs_data->num_subgroups,
                                    p_data->p_brs_data->bis_info_size,
                                    (unsigned int)p_data->p_brs_data->p_cp_bis_info[0].bis_sync,
                                    p_data->p_brs_data->p_cp_bis_info[0].metadata_len);
                    BT_API_DUMPBUF(BT_API_DEBUG,__func__,p_data->p_brs_data->p_cp_bis_info[0].p_metadata,p_data->p_brs_data->p_cp_bis_info[0].metadata_len);
                }                
            }
          
            rtk_bt_le_audio_bass_client_brs_data_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_BASS_CLIENT_BRS_DATA_IND,
                                        sizeof(rtk_bt_le_audio_bass_client_brs_data_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_bass_client_brs_data_ind_t *)p_evt->data;
            p_ind->conn_handle = p_data->conn_handle;
            p_ind->notify = p_data->notify;
            p_ind->read_cause = p_data->read_cause;
            p_ind->char_instance_id = p_data->char_instance_id;
            if(p_data->p_brs_data) {
                p_ind->p_brs_data = rtk_stack_bass_brs_data_copy(p_data->p_brs_data);
                if(p_ind->p_brs_data == NULL) {
                    rtk_bt_event_free(p_evt); 
                    break;                   
                }                  
                p_evt->user_data = p_ind->p_brs_data; 
            }
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
        app_result = BLE_AUDIO_CB_RESULT_REJECT;
        break;
    }
    return app_result;
}
#endif 
#endif 

#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
static uint16_t rtk_stack_mcp_client_read_result_handle(T_MCP_CLIENT_READ_RESULT *p_read_result)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t data_len = 0;
    uint8_t  *data_ptr =  NULL;
    rtk_bt_le_audio_mcp_client_read_result_ind_t *p_read_result_ind = NULL;

    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                RTK_BT_LE_AUDIO_EVT_MCP_CLIENT_READ_RESULT_IND,
                                sizeof(rtk_bt_le_audio_mcp_client_read_result_ind_t));
    if (!p_evt) {
        BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
        return RTK_BT_FAIL;
    } 
    
    p_read_result_ind = (rtk_bt_le_audio_mcp_client_read_result_ind_t *)p_evt->data;
    p_read_result_ind->cause = p_read_result->cause;
    p_read_result_ind->conn_handle = p_read_result->conn_handle;
    p_read_result_ind->general_mcs = p_read_result->gmcs;
    p_read_result_ind->srv_instance_id = p_read_result->srv_instance_id;
    p_read_result_ind->char_uuid = p_read_result->char_uuid;
    BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_MCP_CLIENT_READ_RESULT char_uuid=0x%x\r\n",p_read_result->char_uuid);
    switch (p_read_result->char_uuid) {
        case MCS_UUID_CHAR_MEDIA_PLAYER_NAME:
            data_len = p_read_result->data.media_player_name.media_player_name_len+1; 
            data_ptr = p_read_result->data.media_player_name.p_media_player_name;
            break;
        case MCS_UUID_CHAR_TRACK_TITLE:
            data_len = p_read_result->data.track_title.track_title_len+1; 
            data_ptr = p_read_result->data.track_title.p_track_title;
            break;
        default:
            break;
    }
    if(data_len) {
        p_evt->user_data = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, data_len);
        if (!p_evt->user_data) {
            BT_API_PRINT(BT_API_ERROR,"p_evt->user_data alloc fail, len = %d\r\n",data_len);
            return RTK_BT_ERR_NO_MEMORY;
        }
        memset(p_evt->user_data, 0, data_len);
        memcpy((void *)p_evt->user_data,(void *)data_ptr,data_len-1);
    }
    switch (p_read_result->char_uuid) {
        case MCS_UUID_CHAR_MEDIA_PLAYER_NAME:
            if (data_len != 0) {
                APP_PRINT_INFO1("rtk_stack_mcp_client_read_result_handle: player_name %s", TRACE_STRING(data_ptr));
                BT_API_PRINT(BT_API_DEBUG,"rtk_stack_mcp_client_read_result_handle: player_name %s\r\n", data_ptr);
                p_read_result_ind->data.media_player_name.p_media_player_name = p_evt->user_data;
                p_read_result_ind->data.media_player_name.media_player_name_len = data_len;
            }
            break;
        case MCS_UUID_CHAR_TRACK_TITLE:
            if (data_len != 0) {
                APP_PRINT_INFO1("rtk_stack_mcp_client_read_result_handle: title_url %s", TRACE_STRING(data_ptr));
                BT_API_PRINT(BT_API_DEBUG,"rtk_stack_mcp_client_read_result_handle: title_url %s\r\n", data_ptr);
                p_read_result_ind->data.track_title.p_track_title = p_evt->user_data;
                p_read_result_ind->data.track_title.track_title_len = data_len;
            }
            break;
        default:
            memcpy(&p_read_result_ind->data,&p_read_result->data, sizeof(p_read_result->data));
            break;
    }
    /* Send event */
    rtk_bt_evt_indicate(p_evt, NULL);

    return RTK_BT_OK;
}

static uint16_t rtk_stack_mcp_client_notify_handle(T_MCP_CLIENT_NOTIFY *p_notify_result)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t data_len = 0;
    uint8_t  *data_ptr =  NULL;
    rtk_bt_le_audio_mcp_client_notify_ind_t *p_notify_result_ind = NULL;

    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                RTK_BT_LE_AUDIO_EVT_MCP_CLIENT_NOTIFY_IND,
                                sizeof(rtk_bt_le_audio_mcp_client_notify_ind_t));
    if (!p_evt) {
        BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
        return RTK_BT_FAIL;
    } 
    p_notify_result_ind = (rtk_bt_le_audio_mcp_client_notify_ind_t *)p_evt->data;
    p_notify_result_ind->conn_handle = p_notify_result->conn_handle;
    p_notify_result_ind->general_mcs = p_notify_result->gmcs;
    p_notify_result_ind->srv_instance_id = p_notify_result->srv_instance_id;
    p_notify_result_ind->char_uuid = p_notify_result->char_uuid;
    BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_MCP_CLIENT_NOTIFY char_uuid=0x%x\r\n",p_notify_result->char_uuid);
    switch (p_notify_result->char_uuid) {
        case MCS_UUID_CHAR_MEDIA_PLAYER_NAME:
            data_len = p_notify_result->data.media_player_name.media_player_name_len+1;
            data_ptr = p_notify_result->data.media_player_name.p_media_player_name;
            break;
        case MCS_UUID_CHAR_TRACK_TITLE:
            data_len = p_notify_result->data.track_title.track_title_len+1; 
            data_ptr = p_notify_result->data.track_title.p_track_title;
            break;
        default:
            break;
    }
    if(data_len) {
        p_evt->user_data = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, data_len);
        if (!p_evt->user_data) {
            BT_API_PRINT(BT_API_ERROR,"p_evt->user_data alloc fail, len = %d\r\n",data_len);
            return RTK_BT_ERR_NO_MEMORY;
        }
        memset(p_evt->user_data, 0, data_len);
        memcpy((void *)p_evt->user_data,(void *)data_ptr,data_len-1);
    }
    switch (p_notify_result->char_uuid) {
        case MCS_UUID_CHAR_MEDIA_PLAYER_NAME:
            if (data_len != 0) {
                APP_PRINT_INFO1("rtk_stack_mcs_client_read_result_handle: player_name %s", TRACE_STRING(data_ptr));
                BT_API_PRINT(BT_API_DEBUG,"rtk_stack_mcs_client_read_result_handle: player_name %s\r\n", data_ptr);
                p_notify_result_ind->data.media_player_name.p_media_player_name = p_evt->user_data;
                p_notify_result_ind->data.media_player_name.media_player_name_len = data_len;
            }
            break;
        case MCS_UUID_CHAR_TRACK_TITLE:
            if (data_len != 0) {
                APP_PRINT_INFO1("rtk_stack_mcs_client_read_result_handle: title_url %s", TRACE_STRING(data_ptr));
                BT_API_PRINT(BT_API_DEBUG,"rtk_stack_mcs_client_read_result_handle: title_url %s\r\n", data_ptr);
                p_notify_result_ind->data.track_title.p_track_title = p_evt->user_data;
                p_notify_result_ind->data.track_title.track_title_len = data_len;
            }
            break;
        default:
            memcpy(&p_notify_result_ind->data,&p_notify_result->data, sizeof(p_notify_result->data));
            break;
    }
    /* Send event */
    rtk_bt_evt_indicate(p_evt, NULL);

    return RTK_BT_OK;
}

static uint16_t rtk_stack_mcp_client_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;

    APP_PRINT_INFO1("rtk_stack_mcp_client_handle_msg: msg %x", msg);
    BT_API_PRINT(BT_API_DEBUG,"rtk_stack_mcp_client_handle_msg: msg %x\r\n", msg);

    switch (msg) {
        case LE_AUDIO_MSG_MCP_CLIENT_DIS_DONE: {
            T_MCP_CLIENT_DIS_DONE *p_dis_done = (T_MCP_CLIENT_DIS_DONE *)buf;
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_MCP_CLIENT_DIS_DONE conn_handle=0x%x, general_mcs=%d, is_found=%d, load_from_ftl=%d, srv_num=%d\r\n",
                            p_dis_done->conn_handle,p_dis_done->gmcs,p_dis_done->is_found,p_dis_done->load_from_ftl,p_dis_done->srv_num);
            rtk_bt_le_audio_mcp_client_discovery_done_ind_t *p_discovery_done_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_MCP_CLIENT_DISCOVERY_DONE_IND,
                                        sizeof(rtk_bt_le_audio_mcp_client_discovery_done_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_discovery_done_ind = (rtk_bt_le_audio_mcp_client_discovery_done_ind_t *)p_evt->data;
            p_discovery_done_ind->conn_handle = p_dis_done->conn_handle;
            p_discovery_done_ind->general_mcs = p_dis_done->gmcs;
            p_discovery_done_ind->is_found = p_dis_done->is_found;
            p_discovery_done_ind->srv_num = p_dis_done->srv_num;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
  
    case LE_AUDIO_MSG_MCP_CLIENT_READ_RESULT: {
            T_MCP_CLIENT_READ_RESULT *p_read_result = (T_MCP_CLIENT_READ_RESULT *)buf;
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_MCP_CLIENT_READ_RESULT conn_handle=0x%x, general_mcs=%d, srv_instance_id=%d, char_uuid=0x%x, cause = 0x%x\r\n",
                            p_read_result->conn_handle,p_read_result->gmcs,p_read_result->srv_instance_id,p_read_result->char_uuid,p_read_result->cause);
            rtk_stack_mcp_client_read_result_handle(p_read_result);         
        }
        break;

    case LE_AUDIO_MSG_MCP_CLIENT_NOTIFY: {
            T_MCP_CLIENT_NOTIFY *p_notify_result = (T_MCP_CLIENT_NOTIFY *)buf;
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_MCP_CLIENT_NOTIFY conn_handle=0x%x, general_mcs=%d, srv_instance_id=%d, char_uuid=0x%x\r\n",
                            p_notify_result->conn_handle,p_notify_result->gmcs,p_notify_result->srv_instance_id,p_notify_result->char_uuid);
            rtk_stack_mcp_client_notify_handle(p_notify_result);
        }
        break;
        
    case LE_AUDIO_MSG_MCP_CLIENT_MEDIA_CP_NOTIFY: {
            T_MCP_CLIENT_MEDIA_CP_NOTIFY *p_cp_notify = (T_MCP_CLIENT_MEDIA_CP_NOTIFY *)buf;
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_MCP_CLIENT_MEDIA_CP_NOTIFY conn_handle=0x%x, general_mcs=%d, srv_instance_id=%d, requested_opcode=0x%x, result_code=0x%x\r\n",
                            p_cp_notify->conn_handle,p_cp_notify->gmcs,p_cp_notify->srv_instance_id,p_cp_notify->requested_opcode,p_cp_notify->result_code);
            rtk_bt_le_audio_mcs_media_cp_nodify_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_MCP_CLIENT_MEDIA_CP_NOTIFY_IND,
                                        sizeof(rtk_bt_le_audio_mcs_media_cp_nodify_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_mcs_media_cp_nodify_ind_t *)p_evt->data;
            p_ind->conn_handle = p_cp_notify->conn_handle;
            p_ind->general_mcs = p_cp_notify->gmcs;
            p_ind->srv_instance_id = p_cp_notify->srv_instance_id;
            p_ind->requested_opcode = p_cp_notify->requested_opcode;
            p_ind->result_code = p_cp_notify->result_code;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;

    default:
        BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
        app_result = BLE_AUDIO_CB_RESULT_REJECT;
        break;
    }
    return app_result;
}
#endif

#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
static uint16_t rtk_stack_mcp_server_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;

    APP_PRINT_INFO1("rtk_stack_mcp_server_handle_msg: msg %x", msg);
    //BT_API_PRINT(BT_API_DEBUG,"rtk_stack_mcp_server_handle_msg: msg %x\r\n", msg);

    switch (msg) {
        case LE_AUDIO_MSG_MCP_SERVER_WRITE_MEDIA_CP_IND: {
            T_MCP_SERVER_WRITE_MEDIA_CP_IND *p_cp_ind= (T_MCP_SERVER_WRITE_MEDIA_CP_IND *)buf;
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_MCP_SERVER_WRITE_MEDIA_CP_IND: conn_handle %d, cid %d, service_id %d, opcode %d\r\n", 
                            p_cp_ind->conn_handle, p_cp_ind->cid, p_cp_ind->service_id, p_cp_ind->opcode);
            rtk_bt_le_audio_mcp_server_write_media_cp_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_MCP_SERVER_WRITE_MEDIA_CP_IND,
                                        sizeof(rtk_bt_le_audio_mcp_server_write_media_cp_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_mcp_server_write_media_cp_ind_t *)p_evt->data;
            p_ind->conn_handle = p_cp_ind->conn_handle;
            p_ind->cid = p_cp_ind->cid;
            p_ind->service_id = p_cp_ind->service_id;
            p_ind->opcode = p_cp_ind->opcode;
            if(p_ind->opcode == RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_MOVE_RELATIVE ||
                p_ind->opcode == RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_GOTO_SEGMENT ||
                p_ind->opcode == RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_GOTO_TRACK ||
                p_ind->opcode == RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_GOTO_GROUP) {
                memcpy(&p_ind->param,&p_cp_ind->param, sizeof(p_cp_ind->param));
            }
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;

        case LE_AUDIO_MSG_MCP_SERVER_READ_IND: {
            T_MCP_SERVER_READ_IND *p_read_ind = (T_MCP_SERVER_READ_IND *)buf;
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_MCP_SERVER_READ_IND: conn_handle %d, cid %d, service_id %d, char_uuid 0x%x, offset 0x%x\r\n", 
                                p_read_ind->conn_handle, p_read_ind->cid, p_read_ind->service_id, p_read_ind->char_uuid,p_read_ind->offset);
            if(p_read_ind->char_uuid == MCS_UUID_CHAR_MEDIA_PLAYER_NAME || p_read_ind->char_uuid == MCS_UUID_CHAR_TRACK_TITLE ||
                p_read_ind->char_uuid == MCS_UUID_CHAR_TRACK_DURATION || p_read_ind->char_uuid == MCS_UUID_CHAR_TRACK_POSITION || p_read_ind->char_uuid == MCS_UUID_CHAR_CONTENT_CONTROL_ID ) {
                rtk_bt_le_audio_mcp_server_read_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_MCP_SERVER_READ_IND,
                                            sizeof(rtk_bt_le_audio_mcp_server_read_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_mcp_server_read_ind_t *)p_evt->data;
                p_ind->conn_handle = p_read_ind->conn_handle;
                p_ind->cid = p_read_ind->cid;
                p_ind->service_id = p_read_ind->service_id;
                p_ind->char_uuid = p_read_ind->char_uuid;
                p_ind->offset = p_read_ind->offset;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);

                app_result = BLE_AUDIO_CB_RESULT_PENDING; //first return pending, and then app call mcp_server_read_cfm
            } 
            break;
        }
        default:
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }
    return app_result;
}
#endif

#if defined(RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT
static uint16_t rtk_stack_vcs_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;

    APP_PRINT_INFO1("rtk_stack_vcs_handle_msg: msg %x", msg);
    BT_API_PRINT(BT_API_DEBUG,"rtk_stack_vcs_handle_msg: msg %x\r\n", msg);

    switch (msg) {
    case LE_AUDIO_MSG_VCS_VOLUME_CP_IND: {
            T_VCS_VOLUME_CP_IND *p_volume_state = (T_VCS_VOLUME_CP_IND *)buf;
            APP_PRINT_INFO4("LE_AUDIO_MSG_VCS_VOLUME_CP_IND: conn_handle 0x%x, volume_setting %d, mute %d,cp_op %d",
                            p_volume_state->conn_handle, p_volume_state->volume_setting, p_volume_state->mute, p_volume_state->cp_op);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_VCS_VOLUME_CP_IND: conn_handle 0x%x, volume_setting %d, mute %d,cp_op %d\r\n",
                            p_volume_state->conn_handle, p_volume_state->volume_setting, p_volume_state->mute, p_volume_state->cp_op);
            rtk_bt_le_audio_vcs_volume_cp_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_VCS_VOLUME_CP_IND,
                                        sizeof(rtk_bt_le_audio_vcs_volume_cp_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_vcs_volume_cp_ind_t *)p_evt->data;
            p_ind->conn_handle = p_volume_state->conn_handle;
            p_ind->volume_setting = p_volume_state->volume_setting;
            p_ind->mute = p_volume_state->mute;
            p_ind->cp_op = (rtk_bt_le_audio_vcs_cp_op_t)p_volume_state->cp_op;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;

    default:
        BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
        app_result = BLE_AUDIO_CB_RESULT_REJECT;
        break;
    }
    return app_result;
}
#endif 

#if defined(RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT
static uint16_t rtk_stack_vcs_client_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
    APP_PRINT_INFO1("rtk_stack_vcs_client_handle_msg: msg %x", msg);
    //BT_API_PRINT(BT_API_DEBUG,"rtk_stack_vcs_client_handle_msg: msg %x\r\n", msg);

    switch (msg) {
        case LE_AUDIO_MSG_VCS_CLIENT_DIS_DONE: {
            T_VCS_CLIENT_DIS_DONE *dis_done = (T_VCS_CLIENT_DIS_DONE *)buf;
            APP_PRINT_INFO4("LE_AUDIO_MSG_VCS_CLIENT_DIS_DONE: conn_handle %x,is_found %d,load_from_ftl %d,type_exist 0x%x\r\n", 
                        dis_done->conn_handle,dis_done->is_found,dis_done->load_from_ftl,dis_done->type_exist);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_VCS_CLIENT_DIS_DONE: conn_handle %x,is_found %d,load_from_ftl %d,type_exist 0x%x\r\n", 
                        dis_done->conn_handle,dis_done->is_found,dis_done->load_from_ftl,dis_done->type_exist);
            rtk_bt_le_audio_vcs_client_discovery_done_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_VCS_CLIENT_DISCOVERY_DONE_IND,
                                        sizeof(rtk_bt_le_audio_vcs_client_discovery_done_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_vcs_client_discovery_done_ind_t *)p_evt->data;
            p_ind->conn_handle = dis_done->conn_handle;
            p_ind->is_found = dis_done->is_found;
            p_ind->load_from_ftl = dis_done->load_from_ftl;
            p_ind->type_exist = dis_done->type_exist;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;

        case LE_AUDIO_MSG_VCS_CLIENT_CP_RESULT: {
            T_VCS_CLIENT_CP_RESULT *p_cp_result = (T_VCS_CLIENT_CP_RESULT *)buf;
            if (p_cp_result->cause != GAP_CAUSE_SUCCESS) {
                APP_PRINT_ERROR3("LE_AUDIO_MSG_VCS_CLIENT_CP_RESULT: conn_handle 0x%x, cause 0x%x, op 0x%x",
                                 p_cp_result->conn_handle, p_cp_result->cause, p_cp_result->cp_op);
                BT_API_PRINT(BT_API_ERROR,"LE_AUDIO_MSG_VCS_CLIENT_CP_RESULT: conn_handle 0x%x, cause 0x%x, op 0x%x\r\n",
                                 p_cp_result->conn_handle, p_cp_result->cause, p_cp_result->cp_op);
            }
        }
        break;

        case LE_AUDIO_MSG_VCS_CLIENT_VOLUME_STATE_DATA: {
            T_VCS_CLIENT_VOLUME_STATE_DATA *p_data = (T_VCS_CLIENT_VOLUME_STATE_DATA *)buf;
            APP_PRINT_INFO7("LE_AUDIO_MSG_VCS_CLIENT_VOLUME_STATE_DATA: conn_handle 0x%x, is_notify 0x%x, type 0x%x, volume_flags 0x%x, volume_setting 0x%x, mute 0x%x, change_counter 0x%x",
                                 p_data->conn_handle, p_data->is_notify, p_data->type, p_data->data.volume_flags,
                                 p_data->data.volume_state.volume_setting,p_data->data.volume_state.mute,p_data->data.volume_state.change_counter);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_VCS_CLIENT_VOLUME_STATE_DATA: conn_handle 0x%x, is_notify 0x%x, type 0x%x, volume_flags 0x%x, volume_setting 0x%x, mute 0x%x, change_counter 0x%x\r\n",
                                 p_data->conn_handle, p_data->is_notify, p_data->type, p_data->data.volume_flags,
                                 p_data->data.volume_state.volume_setting,p_data->data.volume_state.mute,p_data->data.volume_state.change_counter);
            if(p_data->type == VCS_CHAR_VOLUME_STATE) {
                rtk_bt_le_audio_vcs_client_volume_state_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_VCS_CLIENT_VOLUME_STATE_IND,
                                            sizeof(rtk_bt_le_audio_vcs_client_volume_state_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_vcs_client_volume_state_ind_t *)p_evt->data;
                p_ind->conn_handle = p_data->conn_handle;
                p_ind->is_notify = p_data->is_notify;
                p_ind->volume_state.volume_setting = p_data->data.volume_state.volume_setting;
                p_ind->volume_state.mute = p_data->data.volume_state.mute;
                p_ind->volume_state.change_counter = p_data->data.volume_state.change_counter;
            } else if(p_data->type == VCS_CHAR_VOLUME_FLAGS) {
                rtk_bt_le_audio_vcs_client_volume_flag_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_VCS_CLIENT_VOLUME_FLAG_IND,
                                            sizeof(rtk_bt_le_audio_vcs_client_volume_flag_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_vcs_client_volume_flag_ind_t *)p_evt->data;
                p_ind->conn_handle = p_data->conn_handle;
                p_ind->is_notify = p_data->is_notify;
                p_ind->volume_flags = p_data->data.volume_flags;
            } else {
                BT_API_PRINT(BT_API_ERROR,"%s unknown p_data->type = %d\r\n",__func__,p_data->type);
                break;                
            }
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;

    default:
        BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
        app_result = BLE_AUDIO_CB_RESULT_REJECT;
        break;
    }

    return app_result;
}
#endif

#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
static uint16_t rtk_stack_vocs_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;

    APP_PRINT_INFO1("rtk_stack_vocs_handle_msg: msg %x", msg);
    BT_API_PRINT(BT_API_DEBUG,"rtk_stack_vocs_handle_msg: msg %x\r\n", msg);

    switch (msg) {
    case LE_AUDIO_MSG_VOCS_WRITE_OFFSET_STATE_IND: {
            T_VOCS_WRITE_OFFSET_STATE_IND *p_volume_offset_state = (T_VOCS_WRITE_OFFSET_STATE_IND *)buf;
            APP_PRINT_INFO3("LE_AUDIO_MSG_VOCS_WRITE_OFFSET_STATE_IND: conn_handle 0x%x, srv_instance_id %d, volume_offset %d",
                            p_volume_offset_state->conn_handle, p_volume_offset_state->srv_instance_id, p_volume_offset_state->volume_offset);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_VOCS_WRITE_OFFSET_STATE_IND: conn_handle 0x%x, srv_instance_id %d, volume_offset %d\r\n",
                            p_volume_offset_state->conn_handle, p_volume_offset_state->srv_instance_id, p_volume_offset_state->volume_offset);
            rtk_bt_le_audio_vocs_write_offset_state_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_VOCS_WRITE_OFFSET_STATE_IND,
                                        sizeof(rtk_bt_le_audio_vocs_write_offset_state_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_vocs_write_offset_state_ind_t *)p_evt->data;
            p_ind->conn_handle = p_volume_offset_state->conn_handle;
            p_ind->srv_instance_id = p_volume_offset_state->srv_instance_id;
            p_ind->volume_offset = p_volume_offset_state->volume_offset;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case LE_AUDIO_MSG_VOCS_WRITE_AUDIO_LOCATION_IND: {
            T_VOCS_WRITE_AUDIO_LOCATION_IND *p_location = (T_VOCS_WRITE_AUDIO_LOCATION_IND *)buf;
            APP_PRINT_INFO3("LE_AUDIO_MSG_VOCS_WRITE_AUDIO_LOCATION_IND: conn_handle 0x%x, srv_instance_id %d, audio_location 0x%x",
                            p_location->conn_handle, p_location->srv_instance_id,(unsigned int)p_location->audio_location);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_VOCS_WRITE_AUDIO_LOCATION_IND: conn_handle 0x%x, srv_instance_id %d, audio_location 0x%x\r\n",
                            p_location->conn_handle, p_location->srv_instance_id,(unsigned int)p_location->audio_location);
            rtk_bt_le_audio_vocs_write_audio_location_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_VOCS_WRITE_AUDIO_LOCATION_IND,
                                        sizeof(rtk_bt_le_audio_vocs_write_audio_location_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_vocs_write_audio_location_ind_t *)p_evt->data;
            p_ind->conn_handle = p_location->conn_handle;
            p_ind->srv_instance_id = p_location->srv_instance_id;
            p_ind->audio_location = p_location->audio_location;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case LE_AUDIO_MSG_VOCS_WRITE_OUTPUT_DES_IND: {
            T_VOCS_WRITE_OUTPUT_DES_IND *p_des = (T_VOCS_WRITE_OUTPUT_DES_IND *)buf;
            APP_PRINT_INFO3("LE_AUDIO_MSG_VOCS_WRITE_OUTPUT_DES_IND: conn_handle 0x%x, srv_instance_id %d, output_des_len %d",
                            p_des->conn_handle, p_des->srv_instance_id, p_des->output_des.output_des_len);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_VOCS_WRITE_OUTPUT_DES_IND: conn_handle 0x%x, srv_instance_id %d, output_des_len %d, output_des %s\r\n",
                            p_des->conn_handle, p_des->srv_instance_id, p_des->output_des.output_des_len, p_des->output_des.p_output_des);
            rtk_bt_le_audio_vocs_write_output_des_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_VOCS_WRITE_OUTPUT_DES_IND,
                                        sizeof(rtk_bt_le_audio_vocs_write_output_des_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_vocs_write_output_des_ind_t *)p_evt->data;
            p_ind->conn_handle = p_des->conn_handle;
            p_ind->srv_instance_id = p_des->srv_instance_id;
            p_ind->output_des.output_des_len = p_des->output_des.output_des_len+1;
            p_ind->output_des.p_output_des = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, p_ind->output_des.output_des_len);
            if (!p_ind->output_des.p_output_des) {
                BT_API_PRINT(BT_API_ERROR,"p_ind->p_data alloc fail, len = %d\r\n",p_ind->output_des.output_des_len);
                rtk_bt_event_free(p_evt);
                break;
            }
            memset(p_ind->output_des.p_output_des, 0, p_ind->output_des.output_des_len);
            memcpy((void *)p_ind->output_des.p_output_des, (void *)p_des->output_des.p_output_des, p_des->output_des.output_des_len);
            p_evt->user_data = p_ind->output_des.p_output_des;             
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
        app_result = BLE_AUDIO_CB_RESULT_REJECT;
        break;
    }
    return app_result;
}

static uint16_t rtk_stack_vocs_client_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;

    APP_PRINT_INFO1("rtk_stack_vocs_client_handle_msg: msg %x", msg);
    BT_API_PRINT(BT_API_DEBUG,"rtk_stack_vocs_client_handle_msg: msg %x\r\n", msg);

    switch (msg) {
    case LE_AUDIO_MSG_VOCS_CLIENT_DIS_DONE: {
            T_VOCS_CLIENT_DIS_DONE *dis_done = (T_VOCS_CLIENT_DIS_DONE *)buf;
            APP_PRINT_INFO4("LE_AUDIO_MSG_VOCS_CLIENT_DIS_DONE: conn_handle %d,is_found %d, load_from_ftl %d, srv_num %d",
                                dis_done->conn_handle,dis_done->is_found,dis_done->load_from_ftl,dis_done->srv_num);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_VOCS_CLIENT_DIS_DONE: conn_handle %d,is_found %d, load_from_ftl %d, srv_num %d\r\n",
                                dis_done->conn_handle,dis_done->is_found,dis_done->load_from_ftl,dis_done->srv_num);
            rtk_bt_le_audio_vocs_client_discovery_done_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_DISCOVERY_DONE_IND,
                                        sizeof(rtk_bt_le_audio_vocs_client_discovery_done_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_vocs_client_discovery_done_ind_t *)p_evt->data;
            p_ind->conn_handle = dis_done->conn_handle;
            p_ind->is_found = dis_done->is_found;
            p_ind->load_from_ftl = dis_done->load_from_ftl;
            p_ind->srv_num = dis_done->srv_num;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;

    case LE_AUDIO_MSG_VOCS_CLIENT_READ_RESULT: {
            T_VOCS_CLIENT_READ_RESULT *read_result = (T_VOCS_CLIENT_READ_RESULT *)buf;
            APP_PRINT_INFO4("LE_AUDIO_MSG_VOCS_CLIENT_READ_RESULT: conn_handle %d,srv_instance_id %d, type %d, cause 0x%x",
                                read_result->conn_handle,read_result->srv_instance_id,read_result->type,read_result->cause);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_VOCS_CLIENT_READ_RESULT: conn_handle %d,srv_instance_id %d, type %d, cause 0x%x\r\n",
                                read_result->conn_handle,read_result->srv_instance_id,read_result->type,read_result->cause);
            rtk_bt_le_audio_vocs_client_read_result_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_READ_RESULT_IND,
                                        sizeof(rtk_bt_le_audio_vocs_client_read_result_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_vocs_client_read_result_ind_t *)p_evt->data;
            p_ind->conn_handle = read_result->conn_handle;
            p_ind->srv_instance_id = read_result->srv_instance_id;
            p_ind->type = (rtk_bt_le_audio_vocs_char_type_t)read_result->type;
            switch (p_ind->type) {
                case RTK_BT_LE_AUDIO_VOCS_CHAR_OFFSET_STATE:
                    p_ind->data.volume_offset.volume_offset = read_result->data.volume_offset.volume_offset;
                    p_ind->data.volume_offset.change_counter = read_result->data.volume_offset.change_counter;
                    break;
                case RTK_BT_LE_AUDIO_VOCS_CHAR_AUDIO_LOCATION:
                    p_ind->data.audio_location= read_result->data.audio_location;
                    break;
                case RTK_BT_LE_AUDIO_VOCS_CHAR_AUDIO_OUTPUT_DESC:
                    p_ind->data.output_des.output_des_len = read_result->data.output_des.output_des_len+1;
                    p_ind->data.output_des.p_output_des = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, p_ind->data.output_des.output_des_len);
                    if (!p_ind->data.output_des.p_output_des) {
                        BT_API_PRINT(BT_API_ERROR,"p_ind->p_data alloc fail, len = %d\r\n",p_ind->data.output_des.output_des_len);
                        rtk_bt_event_free(p_evt);
                        break;
                    }
                    memset(p_ind->data.output_des.p_output_des,0,p_ind->data.output_des.output_des_len);
                    memcpy((void *)p_ind->data.output_des.p_output_des, (void *)read_result->data.output_des.p_output_des, read_result->data.output_des.output_des_len);
                    p_evt->user_data = p_ind->data.output_des.p_output_des; 
                    break;
                default:
                    BT_API_PRINT(BT_API_ERROR,"%s RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_READ_RESULT_IND type (%d) error\r\n",__func__,p_ind->type);
                    break;
            }  
            p_ind->cause = read_result->cause;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    
    case LE_AUDIO_MSG_VOCS_CLIENT_NOTIFY: {
            T_VOCS_CLIENT_NOTIFY *notify_data = (T_VOCS_CLIENT_NOTIFY *)buf;
            APP_PRINT_INFO3("LE_AUDIO_MSG_VOCS_CLIENT_NOTIFY: conn_handle 0x%x, srv_instance_id %d, type=%d",
                                notify_data->conn_handle, notify_data->srv_instance_id,notify_data->type);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_VOCS_CLIENT_NOTIFY: conn_handle 0x%x, srv_instance_id %d, type=%d\r\n",
                                notify_data->conn_handle, notify_data->srv_instance_id,notify_data->type);
            rtk_bt_le_audio_vocs_client_notify_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_NOTIFY_IND,
                                        sizeof(rtk_bt_le_audio_vocs_client_notify_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_vocs_client_notify_ind_t *)p_evt->data;
            p_ind->conn_handle = notify_data->conn_handle;
            p_ind->srv_instance_id = notify_data->srv_instance_id;
            p_ind->type = (rtk_bt_le_audio_vocs_char_type_t)notify_data->type;
            switch (p_ind->type) {
                case RTK_BT_LE_AUDIO_VOCS_CHAR_OFFSET_STATE:
                    p_ind->data.volume_offset.volume_offset = notify_data->data.volume_offset.volume_offset;
                    p_ind->data.volume_offset.change_counter = notify_data->data.volume_offset.change_counter;
                    break;
                case RTK_BT_LE_AUDIO_VOCS_CHAR_AUDIO_LOCATION:
                    p_ind->data.audio_location= notify_data->data.audio_location;
                    break;
                case RTK_BT_LE_AUDIO_VOCS_CHAR_AUDIO_OUTPUT_DESC:
                    p_ind->data.output_des.output_des_len = notify_data->data.output_des.output_des_len+1;
                    p_ind->data.output_des.p_output_des = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, p_ind->data.output_des.output_des_len);
                    if (!p_ind->data.output_des.p_output_des) {
                        BT_API_PRINT(BT_API_ERROR,"p_ind->p_data alloc fail, len = %d\r\n",p_ind->data.output_des.output_des_len);
                        rtk_bt_event_free(p_evt);
                        break;
                    }
                    memset(p_ind->data.output_des.p_output_des,0,p_ind->data.output_des.output_des_len);
                    memcpy((void *)p_ind->data.output_des.p_output_des, (void *)notify_data->data.output_des.p_output_des, notify_data->data.output_des.output_des_len);
                    p_evt->user_data = p_ind->data.output_des.p_output_des;
                    BT_API_PRINT(BT_API_DEBUG,"vocs char output des: srv_instance_id %d, output_des_len %d, p_output_des %s\r\n",
						p_ind->srv_instance_id,p_ind->data.output_des.output_des_len,p_ind->data.output_des.p_output_des);	 
                    break;
                default:
                    BT_API_PRINT(BT_API_ERROR,"%s RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_READ_RESULT_IND type (%d) error\r\n",__func__,p_ind->type);
                    break;
            }          
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;  

    case LE_AUDIO_MSG_VOCS_CLIENT_CP_RESULT: {
            T_VOCS_CLIENT_CP_RESULT *p_cp_result = (T_VOCS_CLIENT_CP_RESULT *)buf;
            APP_PRINT_INFO4("LE_AUDIO_MSG_VOCS_CLIENT_CP_RESULT: conn_handle 0x%x, cause 0x%x, srv_instance_id %d, cp_op=%d",
                                p_cp_result->conn_handle, p_cp_result->cause, p_cp_result->srv_instance_id,p_cp_result->cp_op);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_VOCS_CLIENT_CP_RESULT: conn_handle 0x%x, cause 0x%x, srv_instance_id %d, cp_op=%d\r\n",
                                p_cp_result->conn_handle, p_cp_result->cause, p_cp_result->srv_instance_id,p_cp_result->cp_op);
            rtk_bt_le_audio_vocs_client_cp_result_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_CP_RESULT_IND,
                                        sizeof(rtk_bt_le_audio_vocs_client_cp_result_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_vocs_client_cp_result_ind_t *)p_evt->data;
            p_ind->conn_handle = p_cp_result->conn_handle;
            p_ind->srv_instance_id = p_cp_result->srv_instance_id;
            p_ind->cause = p_cp_result->cause;
            p_ind->cp_op = (rtk_bt_le_audio_vocs_cp_op_t)p_cp_result->cp_op;            
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;              
    default:
        BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
        app_result = BLE_AUDIO_CB_RESULT_REJECT;
        break;
    }

    return app_result;
}
#endif

#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
static uint16_t rtk_stack_aics_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;

    APP_PRINT_INFO1("rtk_stack_aics_handle_msg: msg %x", msg);
    BT_API_PRINT(BT_API_DEBUG,"rtk_stack_aics_handle_msg: msg %x\r\n", msg);

    switch (msg) {
    case LE_AUDIO_MSG_AICS_CP_IND: {
            T_AICS_CP_IND *p_aics_op = (T_AICS_CP_IND *)buf;
            APP_PRINT_INFO4("LE_AUDIO_MSG_AICS_CP_IND: conn_handle 0x%x, srv_instance_id %d, cp_op 0x%x, gain_setting %d",
                            p_aics_op->conn_handle, p_aics_op->srv_instance_id, p_aics_op->cp_op, p_aics_op->gain_setting);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_AICS_CP_IND: conn_handle 0x%x, srv_instance_id %d, cp_op 0x%x, gain_setting %d\r\n",
                            p_aics_op->conn_handle, p_aics_op->srv_instance_id, p_aics_op->cp_op, p_aics_op->gain_setting);
            rtk_bt_le_audio_aics_cp_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_AICS_CP_IND,
                                        sizeof(rtk_bt_le_audio_aics_cp_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_aics_cp_ind_t *)p_evt->data;
            p_ind->conn_handle = p_aics_op->conn_handle;
            p_ind->srv_instance_id = p_aics_op->srv_instance_id;
            p_ind->cp_op = (rtk_bt_le_audio_aics_cp_op_t)p_aics_op->cp_op;  
            p_ind->gain_setting = p_aics_op->gain_setting;          
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case LE_AUDIO_MSG_AICS_WRITE_INPUT_DES_IND: {
            T_AICS_WRITE_INPUT_DES_IND *p_input_des = (T_AICS_WRITE_INPUT_DES_IND *)buf;
            APP_PRINT_INFO3("LE_AUDIO_MSG_AICS_WRITE_INPUT_DES_IND: conn_handle %d, srv_instance_id %d, input_des_len %d",
                            p_input_des->conn_handle,p_input_des->srv_instance_id,p_input_des->input_des.input_des_len);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_AICS_WRITE_INPUT_DES_IND: conn_handle %d,srv_instance_id %d,input_des_len %d,p_input_des=%s\r\n",
                            p_input_des->conn_handle,p_input_des->srv_instance_id,p_input_des->input_des.input_des_len,p_input_des->input_des.p_input_des);
            rtk_bt_le_audio_aics_write_input_des_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_AICS_WRITE_INPUT_DES_IND,
                                        sizeof(rtk_bt_le_audio_aics_write_input_des_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_aics_write_input_des_ind_t *)p_evt->data;
            p_ind->srv_instance_id = p_input_des->srv_instance_id;
            p_ind->input_des.input_des_len = p_input_des->input_des.input_des_len+1;
            p_ind->input_des.p_input_des = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, p_ind->input_des.input_des_len);
            if (!p_ind->input_des.p_input_des) {
                BT_API_PRINT(BT_API_ERROR,"p_ind->p_data alloc fail, len = %d\r\n",p_ind->input_des.input_des_len);
                rtk_bt_event_free(p_evt);
                break;
            }
            memset(p_ind->input_des.p_input_des,0,p_ind->input_des.input_des_len);
            memcpy((void *)p_ind->input_des.p_input_des, (void *)p_input_des->input_des.p_input_des, p_input_des->input_des.input_des_len);
            p_evt->user_data = p_ind->input_des.p_input_des;  
                   
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
        app_result = BLE_AUDIO_CB_RESULT_REJECT;
        break;
    }
    return app_result;
}

static uint16_t rtk_stack_aics_client_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;

    APP_PRINT_INFO1("rtk_stack_aics_client_handle_msg: msg %x", msg);
    BT_API_PRINT(BT_API_DEBUG,"rtk_stack_aics_client_handle_msg: msg %x\r\n", msg);

    switch (msg) {
    case LE_AUDIO_MSG_AICS_CLIENT_DIS_DONE: {
            T_AICS_CLIENT_DIS_DONE *dis_done = (T_AICS_CLIENT_DIS_DONE *)buf;
            APP_PRINT_INFO4("LE_AUDIO_MSG_VOCS_CLIENT_DIS_DONE: conn_handle %d,is_found %d, load_from_ftl %d, srv_num %d",
                                dis_done->conn_handle,dis_done->is_found,dis_done->load_from_ftl,dis_done->srv_num);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_VOCS_CLIENT_DIS_DONE: conn_handle %d,is_found %d, load_from_ftl %d, srv_num %d\r\n",
                                dis_done->conn_handle,dis_done->is_found,dis_done->load_from_ftl,dis_done->srv_num);
            rtk_bt_le_audio_aics_client_discovery_done_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_AICS_CLIENT_DISCOVERY_DONE_IND,
                                        sizeof(rtk_bt_le_audio_aics_client_discovery_done_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_aics_client_discovery_done_ind_t *)p_evt->data;
            p_ind->conn_handle = dis_done->conn_handle;
            p_ind->is_found = dis_done->is_found;
            p_ind->load_from_ftl = dis_done->load_from_ftl;
            p_ind->srv_num = dis_done->srv_num;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;

    case LE_AUDIO_MSG_AICS_CLIENT_READ_RESULT: {
            T_AICS_CLIENT_READ_RESULT *read_result = (T_AICS_CLIENT_READ_RESULT *)buf;
            APP_PRINT_INFO4("LE_AUDIO_MSG_AICS_CLIENT_READ_RESULT: conn_handle %d,srv_instance_id %d, type %d, cause 0x%x",
                                read_result->conn_handle,read_result->srv_instance_id,read_result->type,read_result->cause);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_AICS_CLIENT_READ_RESULT: conn_handle %d,srv_instance_id %d, type %d, cause 0x%x\r\n",
                                read_result->conn_handle,read_result->srv_instance_id,read_result->type,read_result->cause);
            rtk_bt_le_audio_aics_client_read_result_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_AICS_CLIENT_READ_RESULT_IND,
                                        sizeof(rtk_bt_le_audio_aics_client_read_result_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_aics_client_read_result_ind_t *)p_evt->data;
            p_ind->conn_handle = read_result->conn_handle;
            p_ind->srv_instance_id = read_result->srv_instance_id;
            p_ind->type = (rtk_bt_le_audio_aics_char_type_t)read_result->type;
            switch (p_ind->type) {
                case RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_STATE:
                    memcpy(&p_ind->data.input_state, &read_result->data.input_state, sizeof(rtk_bt_le_audio_aics_input_state_t));
                    break;
                case RTK_BT_LE_AUDIO_AICS_CHAR_GAIN_SETTING_PROP:
                    memcpy(&p_ind->data.setting_prop, &read_result->data.setting_prop, sizeof(rtk_bt_le_audio_aics_gain_setting_prop_t));
                    break;
                case RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_TYPE:
                    p_ind->data.input_type = read_result->data.input_type;
                    break;
                case RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_STATUS:
                    p_ind->data.input_status= read_result->data.input_status;
                    break;
                case RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_DES:
                    p_ind->data.input_des.input_des_len = read_result->data.input_des.input_des_len+1;
                    p_ind->data.input_des.p_input_des = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, p_ind->data.input_des.input_des_len);
                    if (!p_ind->data.input_des.p_input_des) {
                        BT_API_PRINT(BT_API_ERROR,"p_ind->p_data alloc fail, len = %d\r\n",p_ind->data.input_des.input_des_len);
                        rtk_bt_event_free(p_evt);
                        break;
                    }
                    memset(p_ind->data.input_des.p_input_des,0,p_ind->data.input_des.input_des_len);
                    memcpy((void *)p_ind->data.input_des.p_input_des, (void *)read_result->data.input_des.p_input_des, read_result->data.input_des.input_des_len);
                    p_evt->user_data = p_ind->data.input_des.p_input_des; 
                    BT_API_PRINT(BT_API_DEBUG,"aics char input des: srv_instance_id %d, input_des_len %d, p_input_des %s\r\n",
					    p_ind->srv_instance_id,p_ind->data.input_des.input_des_len,p_ind->data.input_des.p_input_des);	
                    break;
                default:
                    BT_API_PRINT(BT_API_ERROR,"%s RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_READ_RESULT_IND type (%d) error\r\n",__func__,p_ind->type);
                    break;
            } 
            p_ind->cause = read_result->cause;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case LE_AUDIO_MSG_AICS_CLIENT_NOTIFY: {
            T_AICS_CLIENT_NOTIFY *notify = (T_AICS_CLIENT_NOTIFY *)buf;
            APP_PRINT_INFO3("LE_AUDIO_MSG_AICS_CLIENT_NOTIFY: conn_handle %d,srv_instance_id %d, type %d",
                                notify->conn_handle,notify->srv_instance_id,notify->type);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_AICS_CLIENT_NOTIFY: conn_handle %d,srv_instance_id %d, type %d\r\n",
                                notify->conn_handle,notify->srv_instance_id,notify->type);
            rtk_bt_le_audio_aics_client_notify_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_AICS_CLIENT_NOTIFY_IND,
                                        sizeof(rtk_bt_le_audio_aics_client_notify_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_aics_client_notify_ind_t *)p_evt->data;
            p_ind->conn_handle = notify->conn_handle;
            p_ind->srv_instance_id = notify->srv_instance_id;
            p_ind->type = (rtk_bt_le_audio_aics_char_type_t)notify->type;
            switch (p_ind->type) {
                case RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_STATE:
                    memcpy(&p_ind->data.input_state, &notify->data.input_state, sizeof(rtk_bt_le_audio_aics_input_state_t));
                    break;
                case RTK_BT_LE_AUDIO_AICS_CHAR_GAIN_SETTING_PROP:
                    memcpy(&p_ind->data.setting_prop, &notify->data.setting_prop, sizeof(rtk_bt_le_audio_aics_gain_setting_prop_t));
                    break;
                case RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_TYPE:
                    p_ind->data.input_type = notify->data.input_type;
                    break;
                case RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_STATUS:
                    p_ind->data.input_status= notify->data.input_status;
                    break;
                case RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_DES:
                    p_ind->data.input_des.input_des_len = notify->data.input_des.input_des_len+1;
                    p_ind->data.input_des.p_input_des = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, notify->data.input_des.input_des_len);
                    if (!p_ind->data.input_des.p_input_des) {
                        BT_API_PRINT(BT_API_ERROR,"p_ind->p_data alloc fail, len = %d\r\n",p_ind->data.input_des.input_des_len);
                        rtk_bt_event_free(p_evt);
                        break;
                    }
                    memset(p_ind->data.input_des.p_input_des,0,p_ind->data.input_des.input_des_len);
                    memcpy((void *)p_ind->data.input_des.p_input_des, (void *)notify->data.input_des.p_input_des, notify->data.input_des.input_des_len);
                    p_evt->user_data = p_ind->data.input_des.p_input_des; 
                    break;
                default:
                    BT_API_PRINT(BT_API_ERROR,"%s RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_READ_RESULT_IND type (%d) error\r\n",__func__,p_ind->type);
                    break;
            }
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;

    case LE_AUDIO_MSG_AICS_CLIENT_CP_RESULT: {
            T_AICS_CLIENT_CP_RESULT *read_result = (T_AICS_CLIENT_CP_RESULT *)buf;
            APP_PRINT_INFO4("LE_AUDIO_MSG_AICS_CLIENT_CP_RESULT: conn_handle %d,srv_instance_id %d, cp_op %d, cause 0x%x",
                                read_result->conn_handle,read_result->srv_instance_id,read_result->cp_op,read_result->cause);
            BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_AICS_CLIENT_CP_RESULT: conn_handle %d,srv_instance_id %d, cp_op %d, cause 0x%x\r\n",
                                read_result->conn_handle,read_result->srv_instance_id,read_result->cp_op,read_result->cause);
            rtk_bt_le_audio_aics_client_cp_result_ind_t *p_ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_AICS_CLIENT_CP_RESULT_IND,
                                        sizeof(rtk_bt_le_audio_aics_client_cp_result_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            p_ind = (rtk_bt_le_audio_aics_client_cp_result_ind_t *)p_evt->data;
            p_ind->conn_handle = read_result->conn_handle;
            p_ind->srv_instance_id = read_result->srv_instance_id;
            p_ind->cp_op = (rtk_bt_le_audio_aics_cp_op_t)read_result->cp_op;
            p_ind->cause = read_result->cause;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
        app_result = BLE_AUDIO_CB_RESULT_REJECT;
        break;
    }
    return app_result;
}
#endif

#if defined(RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT
static uint16_t rtk_stack_mics_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
    BT_API_PRINT(BT_API_DEBUG,"%s: msg %x\r\n",__func__,msg);

    switch (msg) {
        case LE_AUDIO_MSG_MICS_WRITE_MUTE_IND: {
                T_MICS_WRITE_MUTE_IND *p_write_ind = (T_MICS_WRITE_MUTE_IND *)buf;
                APP_PRINT_INFO1("LE_AUDIO_MSG_MICS_WRITE_MUTE_IND: mute_value %d", p_write_ind->mic_mute);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_MICS_WRITE_MUTE_IND: mute_value %d\r\n", p_write_ind->mic_mute);

                rtk_bt_le_audio_mics_write_mute_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_MICS_WRITE_MUTE_IND,
                                            sizeof(rtk_bt_le_audio_mics_write_mute_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_mics_write_mute_ind_t *)p_evt->data;
                p_ind->mic_mute = (rtk_bt_le_audio_mics_mute_state_t)p_write_ind->mic_mute;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;

        default:
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }

    return app_result;
}
#endif 

#if defined(RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT
static uint16_t rtk_stack_mics_client_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    rtk_bt_evt_t *p_evt = NULL;
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
    
    //BT_API_PRINT(BT_API_DEBUG,"%s: msg %x\r\n",__func__,msg);

    switch (msg) {
        case LE_AUDIO_MSG_MICS_CLIENT_DIS_DONE: {
                T_MICS_CLIENT_DIS_DONE *dis_done = (T_MICS_CLIENT_DIS_DONE *)buf;
                APP_PRINT_INFO4("LE_AUDIO_MSG_MICS_CLIENT_DIS_DONE: conn_handle %x,is_found %d,load_from_ftl %d,mic_mute 0x%x\r\n", 
                                dis_done->conn_handle,dis_done->is_found,dis_done->load_from_ftl,dis_done->mic_mute);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_MICS_CLIENT_DIS_DONE: conn_handle %x,is_found %d,load_from_ftl %d,mic_mute 0x%x\r\n", 
                            dis_done->conn_handle,dis_done->is_found,dis_done->load_from_ftl,dis_done->mic_mute);
                rtk_bt_le_audio_mics_client_discovery_done_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_MICS_CLIENT_DISCOVERY_DONE_IND,
                                            sizeof(rtk_bt_le_audio_mics_client_discovery_done_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_mics_client_discovery_done_ind_t *)p_evt->data;
                p_ind->conn_handle = dis_done->conn_handle;
                p_ind->is_found = dis_done->is_found;
                p_ind->load_from_ftl = dis_done->load_from_ftl;
                p_ind->mic_mute = dis_done->mic_mute;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);           
            }
            break;

        case LE_AUDIO_MSG_MICS_CLIENT_WRITE_RESULT: {
                T_MICS_CLIENT_WRITE_RESULT *write_result = (T_MICS_CLIENT_WRITE_RESULT *)buf;
                if (write_result->cause != GAP_SUCCESS) {
                    APP_PRINT_ERROR2("LE_AUDIO_MSG_MICS_CLIENT_WRITE_RESULT: conn_handle %x write fail error cause: 0x%x", write_result->conn_handle,write_result->cause);
                    BT_API_PRINT(BT_API_ERROR,"LE_AUDIO_MSG_MICS_CLIENT_WRITE_RESULT:conn_handle %x write fail error cause: 0x%x\r\n",write_result->conn_handle, write_result->cause);
                }
            }   
            break;

        case LE_AUDIO_MSG_MICS_CLIENT_NOTIFY: {
                T_MICS_CLIENT_NOTIFY *notify = (T_MICS_CLIENT_NOTIFY *)buf;
                APP_PRINT_INFO2("LE_AUDIO_MSG_MICS_CLIENT_NOTIFY: conn_handle %x,mic_mute %d\r\n", notify->conn_handle,notify->mic_mute);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_MICS_CLIENT_NOTIFY: conn_handle %x,mic_mute %d\r\n", notify->conn_handle,notify->mic_mute);
                rtk_bt_le_audio_mics_client_notify_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_MICS_CLIENT_NOTIFY_IND,
                                            sizeof(rtk_bt_le_audio_mics_client_notify_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_mics_client_notify_ind_t *)p_evt->data;
                p_ind->conn_handle = notify->conn_handle;
                p_ind->mic_mute = notify->mic_mute;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;

        default:    
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }

    return app_result;
}
#endif 

#if defined(RTK_BLE_AUDIO_TMAP_SUPPORT) && RTK_BLE_AUDIO_TMAP_SUPPORT
static uint16_t rtk_stack_tmas_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
    (void)buf;
    //BT_API_PRINT(BT_API_DEBUG,"%s: msg %x\r\n",__func__,msg);

    switch (msg) {

        default:
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }

    return app_result;
}

static uint16_t rtk_stack_tmas_client_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
    rtk_bt_evt_t *p_evt = NULL;

    switch (msg) {
        case LE_AUDIO_MSG_TMAS_CLIENT_DIS_DONE: {
                T_TMAS_CLIENT_DIS_DONE *discovery_ind = (T_TMAS_CLIENT_DIS_DONE *)buf;
                APP_PRINT_INFO3("LE_AUDIO_MSG_TMAS_CLIENT_DIS_DONE: conn_handle %x,is_found %d,load_from_ftl %d\r\n", discovery_ind->conn_handle, discovery_ind->is_found, discovery_ind->load_from_ftl);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_TMAS_CLIENT_DIS_DONE: conn_handle %x,is_found %d,load_from_ftl %d\r\n", discovery_ind->conn_handle, discovery_ind->is_found, discovery_ind->load_from_ftl);
                rtk_bt_le_audio_tmas_client_discovery_done_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO,
                                            RTK_BT_LE_AUDIO_EVT_TMAS_CLIENT_DISCOVERY_DONE_IND,
                                            sizeof(rtk_bt_le_audio_tmas_client_discovery_done_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_tmas_client_discovery_done_ind_t *)p_evt->data;
                p_ind->conn_handle = discovery_ind->conn_handle;
                p_ind->is_found = discovery_ind->is_found;
                p_ind->load_from_ftl = discovery_ind->load_from_ftl;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;
        case LE_AUDIO_MSG_TMAS_CLIENT_READ_ROLE_RESULT: {
                T_TMAS_CLIENT_READ_ROLE_RESULT *read_res = (T_TMAS_CLIENT_READ_ROLE_RESULT *)buf;
                APP_PRINT_INFO3("LE_AUDIO_MSG_TMAS_CLIENT_READ_ROLE_RESULT: conn_handle 0x%x,role 0x%x,cause %d \r\n", read_res->conn_handle,read_res->role,read_res->cause);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_TMAS_CLIENT_READ_ROLE_RESULT: conn_handle 0x%x,role 0x%x, cause %d\r\n", read_res->conn_handle,read_res->role,read_res->cause);
                rtk_bt_le_audio_tmas_read_role_result_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO,
                                            RTK_BT_LE_AUDIO_EVT_TMAS_CLIENT_READ_ROLE_RESULT,
                                            sizeof(rtk_bt_le_audio_tmas_read_role_result_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_tmas_read_role_result_t *)p_evt->data;
                p_ind->conn_handle = read_res->conn_handle;
                p_ind->role = read_res->role;
                p_ind->cause = read_res->cause;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;
        default:
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }

    return app_result;
}
#endif

static uint16_t rtk_stack_server_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
    
    //BT_API_PRINT(BT_API_DEBUG,"%s: msg %x\r\n",__func__,msg);

    switch (msg) {
        case LE_AUDIO_MSG_SERVER_ATTR_CCCD_INFO: {
                T_SERVER_ATTR_CCCD_INFO *cccd_info = (T_SERVER_ATTR_CCCD_INFO *)buf;
                APP_PRINT_INFO6("LE_AUDIO_MSG_SERVER_ATTR_CCCD_INFO: conn_handle %d,service_id %d,char_uuid 0x%x,char_attrib_index 0x%x,ccc_bits 0x%x,param 0x%x\r\n", 
                                cccd_info->conn_handle,cccd_info->service_id,cccd_info->char_uuid,cccd_info->char_attrib_index,cccd_info->ccc_bits,cccd_info->param);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_SERVER_ATTR_CCCD_INFO: conn_handle %d,service_id %d,char_uuid 0x%x,char_attrib_index 0x%x,ccc_bits 0x%x,param 0x%x\r\n", 
                                cccd_info->conn_handle,cccd_info->service_id,cccd_info->char_uuid,cccd_info->char_attrib_index,cccd_info->ccc_bits,(unsigned int)cccd_info->param);         
            }
            break;

        default:    
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }

    return app_result;
}

static uint16_t rtk_stack_client_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
{
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
    
    //BT_API_PRINT(BT_API_DEBUG,"%s: msg %x\r\n",__func__,msg);

    switch (msg) {
        case LE_AUDIO_MSG_CLIENT_ATTR_CCCD_INFO: {
                T_CLIENT_ATTR_CCCD_INFO *cccd_info = (T_CLIENT_ATTR_CCCD_INFO *)buf;
                APP_PRINT_INFO8("LE_AUDIO_MSG_CLIENT_ATTR_CCCD_INFO: conn_handle %d,srv_cfg %d,srv_instance_id 0x%x,srv_uuid 0x%x,char_instance_id 0x%x,char_uuid 0x%x,cccd_cfg 0x%x,cause 0x%x", 
                                cccd_info->conn_handle,cccd_info->srv_cfg,cccd_info->srv_instance_id,cccd_info->srv_uuid,cccd_info->char_instance_id,
                                cccd_info->char_uuid,cccd_info->cccd_cfg,cccd_info->cause);
                BT_API_PRINT(BT_API_DEBUG,"LE_AUDIO_MSG_CLIENT_ATTR_CCCD_INFO: conn_handle %d,srv_cfg %d,srv_instance_id 0x%x,srv_uuid 0x%x,char_instance_id 0x%x,char_uuid 0x%x,cccd_cfg 0x%x,cause 0x%x\r\n", 
                                cccd_info->conn_handle,cccd_info->srv_cfg,cccd_info->srv_instance_id,cccd_info->srv_uuid,cccd_info->char_instance_id,
                                cccd_info->char_uuid,cccd_info->cccd_cfg,cccd_info->cause);
                if(cccd_info->cause) {
                    BT_API_PRINT(BT_API_ERROR,"LE_AUDIO_MSG_CLIENT_ATTR_CCCD_INFO: conn_handle %d,srv_cfg %d,srv_instance_id 0x%x,srv_uuid 0x%x,char_instance_id 0x%x,char_uuid 0x%x,cccd_cfg 0x%x,cause 0x%x\r\n", 
                                cccd_info->conn_handle,cccd_info->srv_cfg,cccd_info->srv_instance_id,cccd_info->srv_uuid,cccd_info->char_instance_id,
                                cccd_info->char_uuid,cccd_info->cccd_cfg,cccd_info->cause);
                }         
            }
            break;

        default:    
            BT_API_PRINT(BT_API_ERROR,"%s: unknown msg %x, not process\r\n", __func__, msg);
            app_result = BLE_AUDIO_CB_RESULT_REJECT;
            break;
    }

    return app_result;
}

static uint16_t rtk_stack_le_audio_msg_cback(T_LE_AUDIO_MSG msg, void *buf)
{
    uint16_t app_result = BLE_AUDIO_CB_RESULT_SUCCESS;
    uint16_t msg_group = msg & 0xff00;

    APP_PRINT_TRACE2("%s: msg 0x%04x",__func__,msg);
    //BT_API_PRINT(BT_API_DEBUG,"%s: msg=0x%04x,msg_group=0x%04x\r\n",__func__, msg, msg_group);

    switch (msg_group) {   
        case LE_AUDIO_MSG_GROUP_BAP:
            app_result = rtk_stack_bap_handle_msg(msg, buf);
            break;

        case LE_AUDIO_MSG_GROUP_CAP:
            app_result = rtk_stack_cap_handle_msg(msg, buf);
            break;

//only for cap accept
#if (defined(RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) && RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT)
        case LE_AUDIO_MSG_GROUP_PACS:
            app_result = rtk_stack_pacs_handle_msg(msg, buf);
            break;
#endif 

//stack not indicate this msg
#if 1            
        case LE_AUDIO_MSG_GROUP_PACS_CLIENT:
            app_result = rtk_stack_pacs_client_handle_msg(msg, buf);
            break;
#endif 

#if defined(RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) && RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT
        case LE_AUDIO_MSG_GROUP_ASCS:
            app_result = rtk_stack_ascs_handle_msg(msg, buf);
            break;
#endif 

#if defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT
        case LE_AUDIO_MSG_GROUP_BASS:
            app_result = rtk_stack_bass_handle_msg(msg, buf);
            break;
#endif

#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
        case LE_AUDIO_MSG_GROUP_BASS_CLIENT:
            app_result = rtk_stack_bass_client_handle_msg(msg, buf);
            break;
#endif
//CSIP
#if defined(RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT
        case LE_AUDIO_MSG_GROUP_CSIS:
            app_result = rtk_stack_csis_handle_msg(msg, buf);
            break;
#endif
#if defined(RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT
        case LE_AUDIO_MSG_GROUP_CSIS_CLIENT:
            app_result = rtk_stack_csis_client_handle_msg(msg, buf);
            break;
#endif
//MCP
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
        case LE_AUDIO_MSG_GROUP_MCP_CLIENT:
            app_result = rtk_stack_mcp_client_handle_msg(msg, buf);
            break;
#endif 
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
        case LE_AUDIO_MSG_GROUP_MCP_SERVER:
            app_result = rtk_stack_mcp_server_handle_msg(msg, buf);
            break;
#endif
//VCP
#if defined(RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT
        case LE_AUDIO_MSG_GROUP_VCS:
            app_result = rtk_stack_vcs_handle_msg(msg, buf);
            break;
#endif 
#if defined(RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT
        case LE_AUDIO_MSG_GROUP_VCS_CLIENT:
            app_result = rtk_stack_vcs_client_handle_msg(msg, buf);
            break;
#endif
//VOCS
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
        case LE_AUDIO_MSG_GROUP_VOCS:
            app_result = rtk_stack_vocs_handle_msg(msg, buf);
            break;
        case LE_AUDIO_MSG_GROUP_VOCS_CLIENT:
            app_result = rtk_stack_vocs_client_handle_msg(msg, buf);
            break;
#endif 
//AICS
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
        case LE_AUDIO_MSG_GROUP_AICS:
            app_result = rtk_stack_aics_handle_msg(msg, buf);
            break;

        case LE_AUDIO_MSG_GROUP_AICS_CLIENT:
            app_result = rtk_stack_aics_client_handle_msg(msg, buf);
            break;
#endif
//MICP
#if defined(RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT
        case LE_AUDIO_MSG_GROUP_MICS:
            app_result = rtk_stack_mics_handle_msg(msg, buf);
            break;
#endif
#if defined(RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT
        case LE_AUDIO_MSG_GROUP_MICS_CLIENT:
            app_result = rtk_stack_mics_client_handle_msg(msg, buf);
            break;
#endif
#if defined(RTK_BLE_AUDIO_TMAP_SUPPORT) && RTK_BLE_AUDIO_TMAP_SUPPORT
        case LE_AUDIO_MSG_GROUP_TMAS:
            app_result = rtk_stack_tmas_handle_msg(msg, buf);
            break;
        case LE_AUDIO_MSG_GROUP_TMAS_CLIENT:
            app_result = rtk_stack_tmas_client_handle_msg(msg, buf);
            break;
#endif
        case LE_AUDIO_MSG_GROUP_SERVER:
            app_result = rtk_stack_server_handle_msg(msg, buf);
            break;

        case LE_AUDIO_MSG_GROUP_CLIENT:
            app_result = rtk_stack_client_handle_msg(msg, buf);
            break;

        default:
            BT_API_PRINT(BT_API_ERROR,"%s: default msg 0x%04x, not process!\r\n",__func__, msg);
            break;
    }

    return app_result;
}

void bt_stack_le_audio_data_direct_callback(uint8_t cb_type, void *p_cb_data)
{
    rtk_bt_evt_t *p_evt = NULL;
    T_BT_DIRECT_CB_DATA *p_data = (T_BT_DIRECT_CB_DATA *)p_cb_data;

	switch (cb_type) {
	case BT_DIRECT_MSG_ISO_DATA_IND: {
		BT_API_PRINT(BT_API_DUMP, "BT_DIRECT_MSG_ISO_DATA_IND, conn_handle 0x%x, pkt_status_flag 0x%x, pkt_seq_num 0x%x, ts_flag 0x%x, time_stamp 0x%x,iso_sdu_len 0x%x, p_buf %p, offset %d\r\n",
						p_data->p_bt_direct_iso->conn_handle, p_data->p_bt_direct_iso->pkt_status_flag,
						p_data->p_bt_direct_iso->pkt_seq_num, p_data->p_bt_direct_iso->ts_flag,
						(unsigned int)p_data->p_bt_direct_iso->time_stamp,
                        p_data->p_bt_direct_iso->iso_sdu_len, p_data->p_bt_direct_iso->p_buf, p_data->p_bt_direct_iso->offset);                      
        if(p_data->p_bt_direct_iso->iso_sdu_len) {
            BT_API_DUMPBUF(BT_API_DUMP,__func__,p_data->p_bt_direct_iso->p_buf+p_data->p_bt_direct_iso->offset,p_data->p_bt_direct_iso->iso_sdu_len);
            BT_API_PRINT(BT_API_DUMP,"%s pkt_seq_num=%d\r\n",__func__,p_data->p_bt_direct_iso->pkt_seq_num);
        }
        /* Send event */
        p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                    RTK_BT_LE_AUDIO_EVT_ISO_DATA_RECEIVE_IND,
                                    sizeof(rtk_bt_le_audio_direct_iso_data_ind_t));
        if (!p_evt) {
            BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
            break;
        } 
        rtk_bt_le_audio_direct_iso_data_ind_t *direct_iso_data_ind = (rtk_bt_le_audio_direct_iso_data_ind_t *)p_evt->data;
        //the definition of rtk_bt_le_iso_direct_iso_data_ind_t and T_BT_DIRECT_ISO_DATA_IND are different,cannot use memcpy 
        //memcpy((void *)direct_iso_data_ind, (void *)p_data->p_bt_direct_iso, sizeof(rtk_bt_le_iso_direct_iso_data_ind_t));
        direct_iso_data_ind->iso_conn_handle = p_data->p_bt_direct_iso->conn_handle;
        direct_iso_data_ind->pkt_status_flag = (rtk_bt_le_audio_isoch_data_pkt_status_t)p_data->p_bt_direct_iso->pkt_status_flag;
        direct_iso_data_ind->offset = p_data->p_bt_direct_iso->offset;
        direct_iso_data_ind->iso_sdu_len = p_data->p_bt_direct_iso->iso_sdu_len;
        direct_iso_data_ind->pkt_seq_num = p_data->p_bt_direct_iso->pkt_seq_num;
        direct_iso_data_ind->ts_flag = p_data->p_bt_direct_iso->ts_flag;
        direct_iso_data_ind->time_stamp = p_data->p_bt_direct_iso->time_stamp;
        direct_iso_data_ind->buf_len = p_data->p_bt_direct_iso->offset + p_data->p_bt_direct_iso->iso_sdu_len;              
        direct_iso_data_ind->p_buf = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, direct_iso_data_ind->buf_len);
        if (!direct_iso_data_ind->p_buf) {
            BT_API_PRINT(BT_API_ERROR,"direct_iso_data_ind->p_buf alloc fail, len = %d\r\n",direct_iso_data_ind->buf_len);
            rtk_bt_event_free(p_evt);
            break;
        }
        memset(direct_iso_data_ind->p_buf,0,direct_iso_data_ind->buf_len);
        memcpy((void *)direct_iso_data_ind->p_buf, (void *)p_data->p_bt_direct_iso->p_buf, direct_iso_data_ind->buf_len);
        /* 	user_data point to the memory alloced for 2nd level ptr, so it's convenient
            to free it when free p_evt */
        p_evt->user_data = direct_iso_data_ind->p_buf;      
        rtk_bt_evt_indicate(p_evt, NULL);
		gap_iso_data_cfm(p_data->p_bt_direct_iso->p_buf);
	}
	break;

	default:
		BT_API_PRINT(BT_API_ERROR,"%s unhandled cb_type 0x%x", __func__,cb_type);
		break;
	}
}
//only for cap accept
#if (defined(RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) && RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT)
static uint16_t bt_stack_le_audio_pacs_init(rtk_bt_le_audio_pacs_init_param_t *p_pacs)
{
    T_PACS_PARAMS pacs_params = {0};
    uint16_t  pac_sink_codec_len = 0, pac_source_codec_len = 0;
    uint8_t  * p_pac_sink_codec = NULL, * p_pac_source_codec = NULL;

    if(!p_pacs) {
        BT_API_PRINT(BT_API_ERROR,"%s p_pacs is NULL\r\n",__func__); 
        return RTK_BT_ERR_PARAM_INVALID;
    }

    pacs_params.sink_locations.is_exist = true;
    pacs_params.sink_locations.is_notify = true;
    pacs_params.sink_locations.is_write = true;
    if(p_pacs->sink_audio_location != 0) {
        pacs_params.sink_locations.sink_audio_location = p_pacs->sink_audio_location;
        default_sink_audio_location = p_pacs->sink_audio_location;
    } else 
        pacs_params.sink_locations.sink_audio_location = default_sink_audio_location;
    pacs_params.source_locations.is_exist = true;
    pacs_params.source_locations.is_notify = true;
    pacs_params.source_locations.is_write = true;
    if(p_pacs->source_audio_location != 0) {
        pacs_params.source_locations.source_audio_location = p_pacs->source_audio_location;
        default_source_audio_location = p_pacs->source_audio_location;
    } else 
        pacs_params.source_locations.source_audio_location = default_source_audio_location;
    BT_API_PRINT(BT_API_DEBUG,"%s sink_audio_location = 0x%x, source_audio_location = 0x%x\r\n",__func__,
                                (unsigned int)pacs_params.sink_locations.sink_audio_location,(unsigned int)pacs_params.source_locations.source_audio_location);
    pacs_params.supported_contexts.is_notify = true;   
    if(p_pacs->sink_available_contexts != 0) {
        pacs_params.supported_contexts.sink_supported_contexts = p_pacs->sink_available_contexts;
        default_sink_available_contexts = p_pacs->sink_available_contexts;
    } else 
        pacs_params.supported_contexts.sink_supported_contexts = default_sink_available_contexts; 
    if(p_pacs->source_available_contexts != 0) {
        pacs_params.supported_contexts.source_supported_contexts = p_pacs->source_available_contexts;
        default_source_available_contexts = p_pacs->source_available_contexts;
    } else 
        pacs_params.supported_contexts.source_supported_contexts = default_source_available_contexts;    
    BT_API_PRINT(BT_API_DEBUG,"%s sink_supported_contexts = 0x%x, source_supported_contexts = 0x%x\r\n",__func__,
                                pacs_params.supported_contexts.sink_supported_contexts,pacs_params.supported_contexts.source_supported_contexts);
    
    //register pac sink codec
    if(p_default_pac_sink_codec != NULL)
        osif_mem_free(p_default_pac_sink_codec);    
    if(!p_pacs->p_pac_sink_codec || p_pacs->pac_sink_codec_len == 0) {
        BT_API_PRINT(BT_API_WARNING,"%s p_pac_sink_codec is not set, use default value\r\n",__func__);
        pac_sink_codec_len = sizeof(default_pac_sink_codec);
        p_pac_sink_codec = default_pac_sink_codec;  
    } else {
        pac_sink_codec_len = p_pacs->pac_sink_codec_len;
        p_pac_sink_codec = p_pacs->p_pac_sink_codec;    
    }
    p_default_pac_sink_codec = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,pac_sink_codec_len);
    if(p_default_pac_sink_codec)
        memcpy(p_default_pac_sink_codec, p_pac_sink_codec,pac_sink_codec_len);     
    BT_API_DUMPBUF(BT_API_DEBUG,__func__,p_default_pac_sink_codec,pac_sink_codec_len);
    *p_pacs->p_sink_pac_id = pacs_pac_add(SERVER_AUDIO_SINK, p_default_pac_sink_codec, pac_sink_codec_len, true);

    //register pac source codec
    if(p_default_pac_source_codec != NULL)
        osif_mem_free(p_default_pac_source_codec);    
    if(!p_pacs->p_pac_source_codec || p_pacs->pac_source_codec_len == 0) {
        BT_API_PRINT(BT_API_WARNING,"%s p_pac_source_codec is not set, use default value\r\n",__func__);
        pac_source_codec_len = sizeof(default_pac_source_codec);
        p_pac_source_codec = default_pac_source_codec;  
    } else {
        pac_source_codec_len = p_pacs->pac_source_codec_len;
        p_pac_source_codec = p_pacs->p_pac_source_codec;    
    }
    p_default_pac_source_codec = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,pac_source_codec_len);
    if(p_default_pac_source_codec)
        memcpy(p_default_pac_source_codec, p_pac_source_codec,pac_source_codec_len);     
    BT_API_DUMPBUF(BT_API_DEBUG,__func__,p_default_pac_source_codec,pac_source_codec_len);
    *p_pacs->p_source_pac_id = pacs_pac_add(SERVER_AUDIO_SOURCE, p_default_pac_source_codec, pac_source_codec_len, true);

    if(pacs_init(&pacs_params) == false) {
        BT_API_PRINT(BT_API_ERROR,"%s pacs_init fail\r\n",__func__); 
        return RTK_BT_ERR_LOWER_STACK_API;      
    }
    else 
        BT_API_PRINT(BT_API_DEBUG,"%s pacs_init ok p_sink_pac_id=%d, p_source_pac_id=%d\r\n",__func__,*p_pacs->p_sink_pac_id,*p_pacs->p_source_pac_id); 

    return RTK_BT_OK;
}
#endif 

static uint16_t bt_stack_le_audio_bap_init(rtk_bt_le_audio_app_conf_t *p_le_audio_app_conf)
{
    T_BAP_ROLE_INFO role_info = {0};  
    rtk_bt_le_audio_ascs_init_param_t *p_ascs_param = NULL;

    if(p_le_audio_app_conf == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s pacs_init fail\r\n",__func__); 
        return RTK_BT_ERR_PARAM_INVALID;      
    }

    p_ascs_param = &p_le_audio_app_conf->ascs_param;

    BT_API_PRINT(BT_API_DEBUG,"%s bap_role=0x%x\r\n",__func__,p_le_audio_app_conf->bap_role);     
    if(p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_UNI_SER) {
        role_info.role_mask |= (BAP_UNICAST_SRV_SRC_ROLE | BAP_UNICAST_SRV_SNK_ROLE);
        role_info.isoc_cig_max_num = RTK_BT_LE_AUDIO_CIG_MAX_NUM;
        role_info.isoc_cis_max_num = RTK_BT_LE_AUDIO_CIS_MAX_NUM;  
        if(p_ascs_param->sink_ase_num != 0) {
            role_info.snk_ase_num = p_ascs_param->sink_ase_num;
            default_sink_ase_num = p_ascs_param->sink_ase_num;
        } else 
            role_info.snk_ase_num = default_sink_ase_num;
        if(p_ascs_param->source_ase_num != 0) {
            role_info.src_ase_num = p_ascs_param->source_ase_num;
            default_source_ase_num= p_ascs_param->source_ase_num;
        } else 
            role_info.src_ase_num = default_source_ase_num;

        BT_API_PRINT(BT_API_DEBUG,"%s role_info snk_ase_num=%d, src_ase_num=%d\r\n",__func__,role_info.snk_ase_num,role_info.src_ase_num); 
    }
    if (p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_UNI_CLI) {
        role_info.role_mask |= (BAP_UNICAST_CLT_SRC_ROLE | BAP_UNICAST_CLT_SNK_ROLE);
        role_info.isoc_cig_max_num = RTK_BT_LE_AUDIO_CIG_MAX_NUM;
        role_info.isoc_cis_max_num = RTK_BT_LE_AUDIO_CIS_MAX_NUM; 
    }
    if (p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_BRO_SINK) {
        role_info.role_mask |= BAP_BROADCAST_SINK_ROLE;
        role_info.pa_sync_num = RTK_BT_LE_AUDIO_SYNC_HANDLE_MAX_NUM; //bap_role_init: failed, err_idx 7 if not add this
        role_info.isoc_big_receiver_num = RTK_BT_LE_AUDIO_RECEIVER_BIG_MAX_NUM;
        role_info.isoc_bis_receiver_num = RTK_BT_LE_AUDIO_RECEIVER_BIS_MAX_NUM;
    }
    if (p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_BRO_SOUR) {
        role_info.role_mask |= BAP_BROADCAST_SOURCE_ROLE;
        role_info.pa_adv_num = RTK_BT_LE_AUDIO_PA_ADV_SET_MAX_NUM;
        role_info.isoc_big_broadcaster_num = RTK_BT_LE_AUDIO_BROADCASTER_GROUP_MAX_NUM;
        role_info.isoc_bis_broadcaster_num = RTK_BT_LE_AUDIO_BROADCASTER_BIS_MAX_NUM;       
    }
    if (p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_BRO_ASSI) {
        role_info.role_mask |= BAP_BROADCAST_ASSISTANT_ROLE;
        role_info.pa_sync_num = RTK_BT_LE_AUDIO_SYNC_HANDLE_MAX_NUM; 
    } 

    if (p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_SCAN_DELE) {
        role_info.role_mask |= BAP_SCAN_DELEGATOR_ROLE;
        role_info.pa_sync_num = RTK_BT_LE_AUDIO_SYNC_HANDLE_MAX_NUM; 
        role_info.brs_num = RTK_BT_LE_AUDIO_RECEIVER_BIG_MAX_NUM;
    } 

    role_info.init_gap = true; 
    if(bap_role_init(&role_info)== false) {
        BT_API_PRINT(BT_API_ERROR,"%s:bap_role_init fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }
    
    return RTK_BT_OK;
}

uint16_t bt_stack_le_audio_cap_init(rtk_bt_le_audio_app_conf_t *p_le_audio_app_conf)
{
    T_CAP_INIT_PARAMS cap_init_param = {0};   

    if(p_le_audio_app_conf == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s pacs_init fail\r\n",__func__); 
        return RTK_BT_ERR_PARAM_INVALID;      
    }

    //set cap role according bap_role
    if(p_le_audio_app_conf->cap_param.cap_role == RTK_BT_LE_AUDIO_CAP_ROLE_UNKNOWN) {
        	BT_API_PRINT(BT_API_WARNING,"%s set cap role according bap role\r\n",__func__); 

        if (p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_UNI_CLI || 
            p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_BRO_SOUR ) {
            p_le_audio_app_conf->cap_param.cap_role |= RTK_BT_LE_AUDIO_CAP_ROLE_INITIATOR;
        } 
        if (p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_UNI_SER || 
            p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_BRO_SINK ) {
            p_le_audio_app_conf->cap_param.cap_role |= RTK_BT_LE_AUDIO_CAP_ROLE_ACCEPTOR;
        } 
        if (p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_BRO_ASSI ||
            p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_SCAN_DELE) {
            p_le_audio_app_conf->cap_param.cap_role |= RTK_BT_LE_AUDIO_CAP_ROLE_COMMANDER;
        } 
    } 
    BT_API_PRINT(BT_API_DEBUG,"%s p_le_audio_app_conf->cap_param.cap_role=%d\r\n",__func__,p_le_audio_app_conf->cap_param.cap_role); 

    if (p_le_audio_app_conf->cap_param.cap_role & RTK_BT_LE_AUDIO_CAP_ROLE_ACCEPTOR) {
        cap_init_param.cap_role |= CAP_ACCEPTOR_ROLE;
        //cap_init_param.cas.enable = true;
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
        cap_init_param.mcp_media_control_client = true;
        BT_API_PRINT(BT_API_DEBUG,"%s mcp_media_control_client=%d\r\n",__func__,cap_init_param.mcp_media_control_client);
#endif 
#if defined(RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT
#endif
#if defined(RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT
        cap_init_param.csis_num = 1;
        cap_init_param.cas.enable = true;
        if (p_le_audio_app_conf->cap_param.csis_param.csis_feature != 0) {
            cap_init_param.cas.csis_feature = p_le_audio_app_conf->cap_param.csis_param.csis_feature;
            default_csis_feature = p_le_audio_app_conf->cap_param.csis_param.csis_feature;
        } else 
            cap_init_param.cas.csis_feature = default_csis_feature;   

        if (p_le_audio_app_conf->cap_param.csis_param.csis_size != 0) {
            cap_init_param.cas.csis_size = p_le_audio_app_conf->cap_param.csis_param.csis_size; 
            default_csis_size = p_le_audio_app_conf->cap_param.csis_param.csis_size;      
        } else 
            cap_init_param.cas.csis_size = default_csis_size;

        if (p_le_audio_app_conf->cap_param.csis_param.csis_rank != 0) {
            cap_init_param.cas.csis_rank = p_le_audio_app_conf->cap_param.csis_param.csis_rank; 
            default_csis_rank = p_le_audio_app_conf->cap_param.csis_param.csis_rank;      
        } else 
            cap_init_param.cas.csis_rank = default_csis_rank;    

        if(p_le_audio_app_conf->cap_param.csis_param.csis_sirk != NULL) {
            cap_init_param.cas.csis_sirk = p_le_audio_app_conf->cap_param.csis_param.csis_sirk;
            memcpy(default_csis_sirk,p_le_audio_app_conf->cap_param.csis_param.csis_sirk,RTK_BT_LE_CSIS_SIRK_LEN);              
        } else 
            cap_init_param.cas.csis_sirk = default_csis_sirk;

        if (p_le_audio_app_conf->cap_param.csis_param.csis_sirk_type != 0) {
            cap_init_param.cas.csis_sirk_type = p_le_audio_app_conf->cap_param.csis_param.csis_sirk_type;
            default_csis_sirk_type = p_le_audio_app_conf->cap_param.csis_param.csis_sirk_type;
        } else 
            cap_init_param.cas.csis_sirk_type = (T_CSIS_SIRK_TYPE)default_csis_sirk_type; 

        BT_API_PRINT(BT_API_DEBUG,"%s:cap_init_param.cas.csis_feature 0x%x, cap_init_param.cas.csis_sirk:\r\n",__func__,cap_init_param.cas.csis_feature);
        BT_API_DUMPBUF(BT_API_DEBUG,__func__,cap_init_param.cas.csis_sirk,RTK_BT_LE_CSIS_SIRK_LEN); 
#endif 
    } 
    if (p_le_audio_app_conf->cap_param.cap_role & RTK_BT_LE_AUDIO_CAP_ROLE_INITIATOR || 
            p_le_audio_app_conf->cap_param.cap_role & RTK_BT_LE_AUDIO_CAP_ROLE_COMMANDER ) {

        if(p_le_audio_app_conf->cap_param.cap_role & RTK_BT_LE_AUDIO_CAP_ROLE_INITIATOR) {
            cap_init_param.cap_role |= CAP_INITIATOR_ROLE;
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
            cap_init_param.mcs.mcp_media_control_server = true;
            cap_init_param.mcs.mcs_num = 1;
            cap_init_param.mcs.ots_num = 0;
#endif
        }
        if(p_le_audio_app_conf->cap_param.cap_role & RTK_BT_LE_AUDIO_CAP_ROLE_COMMANDER) {
            cap_init_param.cap_role |= CAP_COMMANDER_ROLE;
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
            cap_init_param.mcp_media_control_client = true;            
#endif   
#if defined(RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT
            cap_init_param.vcp_micp.vcp_vcs_client = true;
#endif 
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
            cap_init_param.vcp_micp.vcp_vocs_client = true;    
            cap_init_param.vcp_micp.vcp_vocs_cfg = AUDIO_DESCRIPTION_STORAGE;
#endif
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
            cap_init_param.vcp_micp.vcp_aics_client = true; 
            cap_init_param.vcp_micp.vcp_aics_cfg = AUDIO_DESCRIPTION_STORAGE;
#endif 
#if defined(RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT
            cap_init_param.vcp_micp.micp_mic_controller = true;    
#endif   
        }
        BT_API_PRINT(BT_API_DEBUG,"%s mcp_media_control_client=%d mcp_media_control_server=%d\r\n",__func__,cap_init_param.mcp_media_control_client,cap_init_param.mcs.mcp_media_control_server);
        cap_init_param.cas_client = true;
 
#if defined(RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT
        cap_init_param.csip_set_coordinator = true;  
#endif
    }
    //set tmap cap init
#if defined(RTK_BLE_AUDIO_TMAP_UNICAST_MEDIA_SENDER_SUPPORT) && RTK_BLE_AUDIO_TMAP_UNICAST_MEDIA_SENDER_SUPPORT
    if (p_le_audio_app_conf->tmap_role & RTK_BT_LE_AUDIO_TMAP_ROLE_UMS) {
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
        cap_init_param.mcp_media_control_client = false;
#endif   
#if defined(RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT
        cap_init_param.vcp_micp.vcp_vcs_client = true;
#endif 
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
        cap_init_param.vcp_micp.vcp_vocs_client = true;
        cap_init_param.vcp_micp.vcp_vocs_cfg = AUDIO_DESCRIPTION_STORAGE;
#endif
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
        cap_init_param.vcp_micp.vcp_aics_client = true; 
        cap_init_param.vcp_micp.vcp_aics_cfg = AUDIO_DESCRIPTION_STORAGE;
#endif
    }
#endif

#if defined(RTK_BLE_AUDIO_TMAP_BROADCAST_MEDIA_SENDER_SUPPORT) && RTK_BLE_AUDIO_TMAP_BROADCAST_MEDIA_SENDER_SUPPORT
    if (p_le_audio_app_conf->tmap_role & RTK_BT_LE_AUDIO_TMAP_ROLE_BMS) {
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
        cap_init_param.mcp_media_control_client = false;
#endif
    }
#endif

    if(cap_init(&cap_init_param) == false) {
        BT_API_PRINT(BT_API_ERROR,"%s:cap_init fail, return value is false\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
static uint16_t bt_stack_le_audio_mcs_server_init(void * le_audio_app_conf)
{ 
    T_MCP_SERVER_REG_SRV_PARAM reg_param = {0};
    T_MCP_SERVER_SET_PARAM set_param = {0};
    uint8_t mcs_service_id = 0xFF;
    rtk_bt_le_audio_mcs_init_param_t *p_mcs_param = NULL;
    rtk_bt_le_audio_app_conf_t *p_le_audio_app_conf = NULL;

    p_le_audio_app_conf = (rtk_bt_le_audio_app_conf_t *)le_audio_app_conf;
    p_mcs_param = &p_le_audio_app_conf->cap_param.mcs_param;

    reg_param.gmcs = p_mcs_param->general_mcs;
    reg_param.char_media_control_point.support = p_mcs_param->char_media_control_point_support;
    reg_param.char_media_control_point.char_media_control_point_opcodes_supported_optional_property_notify = p_mcs_param->char_media_control_point_opcodes_supported_optional_property_notify;
    reg_param.char_media_player_name.optional_property_notify = p_mcs_param->char_media_player_name_optional_property_notify;
    reg_param.char_track_title.optional_property_notify = p_mcs_param->char_track_title_optional_property_notify;
    reg_param.char_track_duration.optional_property_notify = p_mcs_param->char_track_duration_optional_property_notify;
    reg_param.char_track_position.optional_property_notify = p_mcs_param->char_track_position_optional_property_notify;
    reg_param.char_playing_order.support = p_mcs_param->char_playing_order_support;
    reg_param.char_playing_order.optional_property_notify = p_mcs_param->char_playing_order_optional_property_notify;
    reg_param.char_playing_orders_supported.support = p_mcs_param->char_playing_orders_supported_support;
    reg_param.char_playback_speed.support = p_mcs_param->char_playback_speed_support;
    reg_param.char_playback_speed.optional_property_notify = p_mcs_param->char_playback_speed_optional_property_notify;
    reg_param.char_seeking_speed.support = p_mcs_param->char_seeking_speed_support;
    reg_param.char_seeking_speed.optional_property_notify = p_mcs_param->char_seeking_speed_optional_property_notify;
    reg_param.char_media_player_icon_url.support = p_mcs_param->char_media_player_icon_url_support;

    mcs_service_id = mcp_server_reg_srv(&reg_param);
    if (mcs_service_id != 0xFF) {
        set_param.char_uuid = MCS_UUID_CHAR_MEDIA_STATE;
        set_param.param.media_state = p_mcs_param->media_state;
        if(false == mcp_server_set_param(mcs_service_id,&set_param)) {            
            BT_API_PRINT(BT_API_ERROR,"%s:mcp_server_set_param char_uuid 0x%x fail\r\n",__func__,set_param.char_uuid);
            return RTK_BT_ERR_LOWER_STACK_API;
        }
        set_param.char_uuid = MCS_UUID_CHAR_PLAYING_ORDERS_SUPPORTED;
        set_param.param.playing_orders_supported = p_mcs_param->playing_orders_supported;
        if(false == mcp_server_set_param(mcs_service_id,&set_param)) {            
            BT_API_PRINT(BT_API_ERROR,"%s:mcp_server_set_param char_uuid 0x%x fail\r\n",__func__,set_param.char_uuid);
            return RTK_BT_ERR_LOWER_STACK_API;
        }
        set_param.char_uuid = MCS_UUID_CHAR_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED;
        set_param.param.media_control_point_opcodes_supported = p_mcs_param->media_control_point_opcodes_supported;
        if(false == mcp_server_set_param(mcs_service_id,&set_param)) {            
            BT_API_PRINT(BT_API_ERROR,"%s:mcp_server_set_param char_uuid 0x%x fail\r\n",__func__,set_param.char_uuid);
            return RTK_BT_ERR_LOWER_STACK_API;
        }   
    } else {            
        BT_API_PRINT(BT_API_ERROR,"%s:mcp_server_reg_srv fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }   
    //return the mcs_id
    *p_mcs_param->p_mcs_service_id = mcs_service_id;
    BT_API_PRINT(BT_API_DEBUG,"%s:mcs_service_id=0x%x\r\n",__func__,mcs_service_id);

    return RTK_BT_OK;
}
#endif
#if (defined(RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT)
static uint16_t bt_stack_le_audio_vc_mic_init(void * le_audio_app_conf)
{
    (void)le_audio_app_conf;

    T_BLE_AUDIO_VC_MIC_PARAMS vc_mic_param = {0}; 
    rtk_bt_le_audio_app_conf_t *p_le_audio_app_conf = NULL; 

#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
    rtk_bt_le_audio_aics_init_param_t *p_aics_param = NULL;    
#endif 
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
    rtk_bt_le_audio_vocs_init_param_t *p_vocs_param = NULL;    
#endif 
    if(le_audio_app_conf == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s le_audio_app_conf fail\r\n",__func__); 
        return RTK_BT_ERR_PARAM_INVALID;      
    }
    p_le_audio_app_conf = (rtk_bt_le_audio_app_conf_t *)le_audio_app_conf;

#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
    p_aics_param = &p_le_audio_app_conf->cap_param.aics_param;
#endif 
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
    p_vocs_param = &p_le_audio_app_conf->cap_param.vocs_param;
#endif 

#if defined(RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT
    vc_mic_param.vcs_enable = true;
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
    vc_mic_param.aics_vcs_num = p_aics_param->aics_vcs_num;
    vc_mic_param.p_aics_vcs_tbl = p_aics_param->p_aics_vcs_tbl;
    vc_mic_param.aics_total_num += vc_mic_param.aics_vcs_num;
    BT_API_PRINT(BT_API_DEBUG,"%s aics_vcs_num %d, aics_vcs_tbl[0] %d, aics_total_num %d\r\n",__func__,vc_mic_param.aics_vcs_num, *vc_mic_param.p_aics_vcs_tbl, vc_mic_param.aics_total_num); 

#endif  
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
    vc_mic_param.vocs_num = p_vocs_param->vocs_num;
    vc_mic_param.p_vocs_feature_tbl = p_vocs_param->p_vocs_feature_tbl;
    BT_API_PRINT(BT_API_DEBUG,"%s vocs_num %d, vocs_feature_tbl[0] %d\r\n",__func__,vc_mic_param.vocs_num, *vc_mic_param.p_vocs_feature_tbl); 
#endif  
#endif
#if defined(RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT
    vc_mic_param.mics_enable = true;
#if defined(RTK_BLE_AUDIO_TMAP_BROADCAST_MEDIA_RECEIVER_SUPPORT) && RTK_BLE_AUDIO_TMAP_BROADCAST_MEDIA_RECEIVER_SUPPORT
    if (p_le_audio_app_conf->tmap_role & RTK_BT_LE_AUDIO_TMAP_ROLE_BMR) {
        vc_mic_param.mics_enable = false;
    }
#endif
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
    vc_mic_param.aics_mics_num = p_aics_param->aics_mics_num;
    vc_mic_param.p_aics_mics_tbl = p_aics_param->p_aics_mics_tbl;
    vc_mic_param.aics_total_num += vc_mic_param.aics_mics_num;
    BT_API_PRINT(BT_API_DEBUG,"%s aics_mics_num %d, aics_mics_tbl[0] %d, aics_total_num %d\r\n",__func__,vc_mic_param.aics_mics_num, *vc_mic_param.p_aics_mics_tbl, vc_mic_param.aics_total_num); 
#endif
#endif

    if(false == ble_audio_vc_mic_init(&vc_mic_param)) {
        BT_API_PRINT(BT_API_ERROR,"%s:ble_audio_vc_mic_init fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    (void)p_le_audio_app_conf;

    return RTK_BT_OK;
}
#endif

#if defined(RTK_BLE_AUDIO_TMAP_SUPPORT) && RTK_BLE_AUDIO_TMAP_SUPPORT
static uint16_t bt_stack_le_audio_tmap_init(void * le_audio_app_conf) 
{
    (void)le_audio_app_conf;
    uint16_t tmas_role = 0;
    rtk_bt_le_audio_app_conf_t *p_le_audio_app_conf = NULL; 

    if(le_audio_app_conf == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s le_audio_app_conf fail\r\n",__func__); 
        return RTK_BT_ERR_PARAM_INVALID;      
    }
    p_le_audio_app_conf = (rtk_bt_le_audio_app_conf_t *)le_audio_app_conf;

    if ((p_le_audio_app_conf->tmap_role & RTK_BT_LE_AUDIO_TMAP_ROLE_CT) || (p_le_audio_app_conf->tmap_role & RTK_BT_LE_AUDIO_TMAP_ROLE_UMR) ||
        (p_le_audio_app_conf->tmap_role & RTK_BT_LE_AUDIO_TMAP_ROLE_BMR) ) {
            tmas_role |= p_le_audio_app_conf->tmap_role;
            if(tmas_init(tmas_role) == false) {
                BT_API_PRINT(BT_API_ERROR,"%s:tmas_init fail, return value is false\r\n",__func__);
                return RTK_BT_ERR_LOWER_STACK_API;
            }
        } else {
            if(tmas_client_init() == false) {
                BT_API_PRINT(BT_API_ERROR,"%s:tmas_client_init fail, return value is false\r\n",__func__);
                return RTK_BT_ERR_LOWER_STACK_API;
            }
        }
    return RTK_BT_OK;
}
#endif

uint16_t bt_stack_le_audio_init(rtk_bt_app_conf_t *papp_conf,void *io_msg_q,void *evt_msg_q)
{
    uint16_t ret = RTK_BT_OK;
    bool cis_flag = true;
    T_GAP_CAUSE cause = GAP_CAUSE_SUCCESS;
    T_BLE_AUDIO_PARAMS ble_audio_param = {0}; 
    rtk_bt_le_audio_app_conf_t *p_le_audio_app_conf = NULL; 

    if(papp_conf == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s papp_conf fail\r\n",__func__); 
        return RTK_BT_ERR_PARAM_INVALID;      
    }
    p_le_audio_app_conf = &papp_conf->le_audio_app_conf;  

    if((papp_conf->app_profile_support & RTK_BT_PROFILE_GATTS) == 0) {
        if(!gatt_svc_init(GATT_SVC_USE_EXT_SERVER, RTK_BT_GATTS_SERVICE_NUM))
            printf("gatt_svc_init fail \r\n");
    }

    //set cis support when unicast enable
    if((p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_UNI_SER) || (p_le_audio_app_conf->bap_role & RTK_BT_LE_AUDIO_BAP_ROLE_UNI_CLI)) {
        cause = le_set_gap_param(GAP_PARAM_CIS_HOST_SUPPORT, sizeof(cis_flag), &cis_flag);
        if(cause != GAP_CAUSE_SUCCESS) {
            BT_API_PRINT(BT_API_ERROR,"%s le_set_gap_param fail (cause = 0x%x)\r\n",__func__,cause);  
            return false;
        }  
    }   
  
    ble_audio_param.evt_queue_handle = evt_msg_q;
    ble_audio_param.io_queue_handle = io_msg_q;
    ble_audio_param.acl_link_num = le_get_max_link_num();
    ble_audio_param.io_event_type = IO_MSG_TYPE_LE_AUDIO;
    //ble_audio_param.bt_gatt_client_init is the same with gatt_client_init
    if((papp_conf->app_profile_support & RTK_BT_PROFILE_GATTC) == 0) {
#if (defined(RTK_BT_5_2_EATT_SUPPORT) && RTK_BT_5_2_EATT_SUPPORT) && F_BT_5_2_EATT_SUPPORT
        ble_audio_param.bt_gatt_client_init = (GATT_CLIENT_DISCOV_MODE_REG_SVC_BIT |
                                            GATT_CLIENT_DISCOV_MODE_CCCD_STORAGE_BIT |
                                            GATT_CLIENT_DISCOV_MODE_USE_EXT_CLIENT |
                                            GATT_CLIENT_DISCOV_MODE_GATT_SVC);
        gatt_client_cfg_client_supported_feature(GATT_SVC_CLIENT_SUPPORTED_FEATURES_EATT_BEARER_BIT);
#else
        ble_audio_param.bt_gatt_client_init = (GATT_CLIENT_DISCOV_MODE_REG_SVC_BIT | GATT_CLIENT_DISCOV_MODE_CCCD_STORAGE_BIT);
#endif   
    } 
    if(ble_audio_init(&ble_audio_param) == false) {
        BT_API_PRINT(BT_API_ERROR,"%s:ble_audio_init fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    //init bap role, must call before bt_stack_le_audio_cap_init
    ret = bt_stack_le_audio_bap_init(p_le_audio_app_conf);
    if(ret != RTK_BT_OK) {
        BT_API_PRINT(BT_API_ERROR,"%s:bt_stack_le_audio_bap_init fail\r\n",__func__);
        return ret;
    } 

    //init cap
    ret = bt_stack_le_audio_cap_init(p_le_audio_app_conf);
    if(ret != RTK_BT_OK) {
        BT_API_PRINT(BT_API_ERROR,"%s:bt_stack_le_audio_cap_init fail,ret= 0x%x\r\n",__func__,ret);
        return ret;
    } 

//only for cap accept
#if (defined(RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) && RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT)
    if (p_le_audio_app_conf->cap_param.cap_role & RTK_BT_LE_AUDIO_CAP_ROLE_ACCEPTOR ) {
        ret = bt_stack_le_audio_pacs_init(&p_le_audio_app_conf->pacs_param);
        if(ret != RTK_BT_OK) {
            BT_API_PRINT(BT_API_ERROR,"%s:bt_stack_le_audio_pacs_init fail,ret= 0x%x\r\n",__func__,ret);
            return ret;
        } 
    }
#endif 

#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
    // init mcs_server if unicast client and support mcs 
    if (p_le_audio_app_conf->cap_param.cap_role & RTK_BT_LE_AUDIO_CAP_ROLE_INITIATOR) {
        ret = bt_stack_le_audio_mcs_server_init(p_le_audio_app_conf);
        if(ret != RTK_BT_OK) {
            BT_API_PRINT(BT_API_ERROR,"%s:bt_stack_le_audio_mcs_server_init fail,ret= 0x%x\r\n",__func__,ret);
            return ret;
        } 
    }
#endif
#if (defined(RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT)
    if (p_le_audio_app_conf->cap_param.cap_role & RTK_BT_LE_AUDIO_CAP_ROLE_ACCEPTOR) {
        ret = bt_stack_le_audio_vc_mic_init(p_le_audio_app_conf);
        if(ret != RTK_BT_OK) {
            BT_API_PRINT(BT_API_ERROR,"%s:bt_stack_le_audio_vcs_init fail,ret= 0x%x\r\n",__func__,ret);
            return ret;
        } 
    }
#endif
#if defined(RTK_BLE_AUDIO_TMAP_SUPPORT) && RTK_BLE_AUDIO_TMAP_SUPPORT
    ret = bt_stack_le_audio_tmap_init(p_le_audio_app_conf);
    if(ret != RTK_BT_OK) {
        BT_API_PRINT(BT_API_ERROR,"%s:bt_stack_le_audio_tmap_init fail,ret= 0x%x\r\n",__func__,ret);
        return ret;
    } 
#endif
    //set data_direct_callback
    gap_register_direct_cb(bt_stack_le_audio_data_direct_callback); 
    
    ble_audio_cback_register(rtk_stack_le_audio_msg_cback);

#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
	g_broadcast_source_handle = NULL;//for there is no broadcast_source_delete api
#endif

	return RTK_BT_OK;     
}

void bt_stack_le_audio_deinit(void)
{
	BT_API_PRINT(BT_API_DEBUG,"%s\n",__func__);

    ble_audio_cback_unregister(rtk_stack_le_audio_msg_cback);

    if(p_default_pac_sink_codec != NULL)
        osif_mem_free(p_default_pac_sink_codec);
    if(p_default_pac_source_codec != NULL)
        osif_mem_free(p_default_pac_source_codec);

    gap_register_direct_cb(NULL);
    // ble_audio_deinit();
}

uint16_t bt_stack_le_audio_data_send(void *data)
{
    T_GAP_CAUSE cause = GAP_CAUSE_SUCCESS;
    rtk_bt_le_audio_iso_data_send_info_t *param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   
    param = (rtk_bt_le_audio_iso_data_send_info_t *)data;

    BT_API_DUMPBUF(BT_API_DUMP,__func__,param->p_data,param->data_len);

#if 0
    static bool is_odd = false;
    static uint32_t last_even_time = 0, now_even_time = 0, last_odd_time = 0, now_odd_time = 0;
    if(is_odd) {
        now_odd_time = osif_sys_time_get();
        if(last_odd_time != 0 && (now_odd_time - last_odd_time > 11 || now_odd_time - last_odd_time < 9)) {
            BT_API_PRINT(BT_API_ERROR,"%s gap_iso_send_data iso_conn_handle = 0x%x,pkt_seq_num %d,last_odd_time %d, now_odd_time %d,delta_time %d\r\n",__func__,param->iso_conn_handle,param->pkt_seq_num,(int)last_odd_time,(int)now_odd_time, (int)(now_odd_time-last_odd_time));
        }
        last_odd_time = now_odd_time;
        is_odd = false;
    } else {
        now_even_time = osif_sys_time_get();
        if(last_even_time != 0 && (now_even_time - last_even_time > 11 || now_even_time - last_even_time < 9)) {
            BT_API_PRINT(BT_API_ERROR,"%s gap_iso_send_data iso_conn_handle = 0x%x,pkt_seq_num %d,last_even_time %d, now_even_time %d,delta_time %d\r\n",__func__,param->iso_conn_handle,param->pkt_seq_num,(int)last_even_time,(int)now_even_time, (int)(now_even_time-last_even_time));
        }
        last_even_time = now_even_time; 
        is_odd = true;     
    }

#endif

    cause = gap_iso_send_data(param->p_data,
                        param->iso_conn_handle,                                  
                        param->data_len,
                        param->ts_flag,
                        param->time_stamp,
                        param->pkt_seq_num);
    if(GAP_CAUSE_SUCCESS != cause) {        
        if(cause == GAP_CAUSE_ERROR_CREDITS) {
            BT_API_PRINT(BT_API_WARNING,"%s gap_iso_send_data warning (cause = 0x%x,iso_conn_handle = 0x%x)\r\n",__func__,cause,param->iso_conn_handle);
            return RTK_BT_ERR_NO_CREDITS;
        } else {
            BT_API_PRINT(BT_API_ERROR,"%s gap_iso_send_data fail (cause = 0x%x,iso_conn_handle = 0x%x)\r\n",__func__,cause,param->iso_conn_handle);
            return RTK_BT_ERR_LOWER_STACK_API;
        }
    } else {        
        BT_API_PRINT(BT_API_DUMP,"%s gap_iso_send_data ok,iso_conn_handle=0x%x,pkt_seq_num=%d,time_stamp=%d, time = %d\r\n",__func__, param->iso_conn_handle,param->pkt_seq_num,(int)param->time_stamp,(int)osif_sys_time_get());
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_get_prefer_codec_cfg(void *data)
{
    T_CODEC_CFG codec_cfg = {0};    
    rtk_bt_le_audio_get_prefer_codec_cfg_param_t *param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    param = (rtk_bt_le_audio_get_prefer_codec_cfg_param_t *)data;

    if(false == codec_preferred_cfg_get((T_CODEC_CFG_ITEM)param->item,&codec_cfg)) {
        BT_API_PRINT(BT_API_ERROR,"%s codec_preferred_cfg_get fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    BT_API_PRINT(BT_API_DEBUG,"%s type_exist 0x%x, frame_duration %d, sample_frequency %d, codec_frame_blocks_per_sdu %d,octets_per_codec_frame=%d,audio_channel_allocation=0x%x)\r\n",__func__, 
                            codec_cfg.type_exist,codec_cfg.frame_duration,codec_cfg.sample_frequency,
							codec_cfg.codec_frame_blocks_per_sdu,codec_cfg.octets_per_codec_frame,
							(unsigned int)codec_cfg.audio_channel_allocation);

    param->p_cfg_codec->type_exist = codec_cfg.type_exist;
    param->p_cfg_codec->frame_duration = codec_cfg.frame_duration;
    param->p_cfg_codec->sample_frequency = codec_cfg.sample_frequency;
    param->p_cfg_codec->codec_frame_blocks_per_sdu = codec_cfg.codec_frame_blocks_per_sdu;
    param->p_cfg_codec->octets_per_codec_frame = codec_cfg.octets_per_codec_frame;
    param->p_cfg_codec->audio_channel_allocation = codec_cfg.audio_channel_allocation;

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_get_prefer_qos_cfg(void *data)
{
    T_QOS_CFG_PREFERRED qos_cfg_preferred = {0};
    rtk_bt_le_audio_get_prefer_qos_cfg_param_t *param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    param = (rtk_bt_le_audio_get_prefer_qos_cfg_param_t *)data;

    if(false == qos_preferred_cfg_get((T_CODEC_CFG_ITEM)param->codec_cfg_item, (T_QOS_CFG_TYPE)param->qos_cfg_type, &qos_cfg_preferred)) {
        BT_API_PRINT(BT_API_ERROR,"%s qos_preferred_cfg_get fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    BT_API_PRINT(BT_API_DEBUG,"%s sdu_interval 0x%d, framing %d, max_sdu %d, retransmission_number %d,max_transport_latency=%d,presentation_delay=%d)\r\n",__func__, 
                            (int)qos_cfg_preferred.sdu_interval,qos_cfg_preferred.framing,qos_cfg_preferred.max_sdu,
                            qos_cfg_preferred.retransmission_number,qos_cfg_preferred.max_transport_latency,
                            (int)qos_cfg_preferred.presentation_delay);

    param->p_qos_cfg->sdu_interval = qos_cfg_preferred.sdu_interval;
    param->p_qos_cfg->framing = qos_cfg_preferred.framing;
    param->p_qos_cfg->max_sdu = qos_cfg_preferred.max_sdu;
    param->p_qos_cfg->retransmission_number = qos_cfg_preferred.retransmission_number;
    param->p_qos_cfg->max_transport_latency = qos_cfg_preferred.max_transport_latency;
    param->p_qos_cfg->presentation_delay = qos_cfg_preferred.presentation_delay;

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_pacs_get_info(void *data)
{
    T_BAP_PACS_INFO pacs_info = {0};
    rtk_bt_le_audio_pacs_get_info_param_t *param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    param = (rtk_bt_le_audio_pacs_get_info_param_t *)data;

    if(false == bap_pacs_get_info(param->conn_handle,&pacs_info)) {
        BT_API_PRINT(BT_API_ERROR,"%s bap_pacs_get_info fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    //Make sure rtk_bt_le_audio_bap_pacs_info_t is the same with T_BAP_PACS_INFO
    memcpy((void*)param->p_pacs_info, (void*)&pacs_info,sizeof(rtk_bt_le_audio_bap_pacs_info_t));

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_pacs_get_pac_record(void *data)
{
    T_BAP_PAC_RECORD *p_pac_tbl = NULL;
    uint16_t pac_tbl_size = 0;
    rtk_bt_le_audio_pacs_get_pac_record_param_t *param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    param = (rtk_bt_le_audio_pacs_get_pac_record_param_t *)data;

    if(!param->p_pac_tbl || !param->p_pac_num) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: p_pac_tbl or p_pac_num invalid \r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

	pac_tbl_size = (*param->p_pac_num) * sizeof(T_BAP_PAC_RECORD);
	p_pac_tbl = (T_BAP_PAC_RECORD *)osif_mem_alloc(RAM_TYPE_DATA_ON,pac_tbl_size);
    if (!p_pac_tbl) {
        BT_API_PRINT(BT_API_ERROR,"p_pac_tbl alloc fail, len = %d\r\n",pac_tbl_size);
        return RTK_BT_ERR_NO_MEMORY;
    }
    memset(p_pac_tbl, 0, pac_tbl_size);

    BT_API_PRINT(BT_API_DEBUG,"%s:conn_handle=0x%x,audio_role=%d, param->pac_num=%d\r\n",__func__,param->conn_handle,param->audio_role, *param->p_pac_num);

    if(false == bap_pacs_get_pac_record(param->conn_handle,(T_AUDIO_DIRECTION)param->audio_role,param->p_pac_num,p_pac_tbl)) {
        BT_API_PRINT(BT_API_ERROR,"%s bap_pacs_get_pac_record fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    for(uint8_t i = 0; i < (*param->p_pac_num); i++) {
        param->p_pac_tbl[i].pac_handle = p_pac_tbl[i].pac_handle;
        memcpy(param->p_pac_tbl[i].codec_id, p_pac_tbl[i].codec_id,RTK_BT_LE_AUDIO_CODEC_ID_LEN);
        memcpy(&param->p_pac_tbl[i].codec_cap, &p_pac_tbl[i].codec_cap,sizeof(T_CODEC_CAP));
        param->p_pac_tbl[i].lc3_sup_cfg_bits = p_pac_tbl[i].lc3_sup_cfg_bits;
        param->p_pac_tbl[i].pref_audio_contexts = p_pac_tbl[i].pref_audio_contexts;
        if(p_pac_tbl[i].metadata_length) { 
            BT_API_DUMPBUF(BT_API_DEBUG,__func__,p_pac_tbl[i].p_metadata,p_pac_tbl[i].metadata_length);
            if(p_pac_tbl[i].metadata_length <= RTK_BT_LE_AUDIO_METADATA_MAX_LEN) {
                param->p_pac_tbl[i].metadata_length = p_pac_tbl[i].metadata_length;
                memcpy(param->p_pac_tbl[i].metadata, p_pac_tbl[i].p_metadata,p_pac_tbl[i].metadata_length);
            } else {
                BT_API_PRINT(BT_API_ERROR,"%s conn_handle 0x%x audio_role %d metadata_length(%d) is too long, skip copy metadata, \r\n",__func__,param->conn_handle,param->audio_role,p_pac_tbl[i].metadata_length);
            }
        }
    }

 	if(p_pac_tbl) {
		osif_mem_free(p_pac_tbl);
	}
    return RTK_BT_OK;
}
                                    
static uint16_t bt_stack_le_audio_pacs_get_lc3_table_mask(void *data)
{
    uint32_t lc3_table_mask = 0;
    rtk_bt_le_audio_pacs_get_lc3_table_mask_param_t *param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    param = (rtk_bt_le_audio_pacs_get_lc3_table_mask_param_t *)data;

    if(RTK_BLE_AUDIO_SINK == param->audio_role) {
        lc3_table_mask = bap_pacs_get_lc3_snk_table_msk(param->conn_handle,param->pref_audio_contexts,param->channel_count,1);
    } else if(RTK_BLE_AUDIO_SOURCE == param->audio_role) {
        lc3_table_mask = bap_pacs_get_lc3_src_table_msk(param->conn_handle,param->pref_audio_contexts,param->channel_count,1);
    } else {
        BT_API_PRINT(BT_API_ERROR,"%s fail: direction(%d) error\r\n",__func__,param->audio_role);
        return RTK_BT_ERR_PARAM_INVALID;
    }

    *param->p_lc3_table_mask = lc3_table_mask;
    BT_API_PRINT(BT_API_DEBUG,"%s conn_handle %d, pref_audio_contexts 0x%x, channel_count %d, lc3_table_mask 0x%x\r\n",__func__,param->conn_handle,param->pref_audio_contexts,param->channel_count,(unsigned int)lc3_table_mask);

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_pacs_get_bis_array_by_sync_info(void *data)
{
    uint32_t bis_array = 0;
    rtk_bt_le_audio_pacs_get_bis_array_by_sync_info_param_t *param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    param = (rtk_bt_le_audio_pacs_get_bis_array_by_sync_info_param_t *)data;

    bis_array = bap_pacs_check_cfg_by_sync_info(param->sync_handle,param->conn_handle,param->filter);

    *param->p_bis_array = bis_array;
    BT_API_PRINT(BT_API_DEBUG,"%s conn_handle %d, sync_handle %p, filter %d, bis_array 0x%x\r\n",__func__,param->conn_handle,param->sync_handle,param->filter,(unsigned int)bis_array);

    return RTK_BT_OK;
}

#if (defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT)
static uint16_t bt_stack_le_audio_sync_allocate(void *data)
{
    T_BLE_AUDIO_SYNC_HANDLE sync_handle = NULL;
    rtk_bt_le_audio_sync_allocate_param_t *p_param = NULL;    

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_sync_allocate_param_t *)data;

    sync_handle = ble_audio_sync_allocate(bt_stack_le_audio_sync_cb,
                                            p_param->adv_addr.type,
                                            p_param->adv_addr.addr_val,
                                            p_param->adv_sid,
                                            p_param->broadcast_id);
    if(sync_handle == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s ble_audio_sync_allocate fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    *p_param->p_broadcast_sync_handle = (rtk_bt_le_audio_sync_handle_t *)sync_handle;

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_sync_release(void *data)
{
    rtk_bt_le_audio_sync_handle_t sync_handle = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    sync_handle = *(rtk_bt_le_audio_sync_handle_t*)data;

    BT_API_PRINT(BT_API_DEBUG,"%s sync_handle %p\r\n",__func__,sync_handle);
    
    if(false == ble_audio_sync_release(&sync_handle)){
        BT_API_PRINT(BT_API_ERROR,"%s ble_audio_sync_release fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_sync_find(void *data)
{
    T_BLE_AUDIO_SYNC_HANDLE sync_handle = NULL;
    rtk_bt_le_audio_sync_find_param_t *p_param = NULL;    

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_sync_find_param_t *)data;

    sync_handle = ble_audio_sync_find(p_param->adv_sid, p_param->broadcast_id);

    *p_param->p_broadcast_sync_handle = (rtk_bt_le_audio_sync_handle_t *)sync_handle;

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_pa_sync_establish(void *data)
{
    rtk_bt_le_audio_pa_sync_establish_param_t *p_param = NULL;    

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_pa_sync_establish_param_t *)data;

    if(false == ble_audio_pa_sync_establish(p_param->sync_handle,
                                            p_param->establish_param.options,
                                            p_param->establish_param.sync_cte_type,
                                            p_param->establish_param.skip,
                                            p_param->establish_param.sync_timeout)){
        BT_API_PRINT(BT_API_ERROR,"%s ble_audio_pa_sync_establish fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_pa_sync_terminate(void *data)
{
    rtk_bt_le_audio_sync_handle_t sync_handle = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    sync_handle = *(rtk_bt_le_audio_sync_handle_t*)data;

    if(false == ble_audio_pa_terminate(sync_handle)){
        BT_API_PRINT(BT_API_ERROR,"%s ble_audio_pa_terminate fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }
 
    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_sync_get_sync_info(void *data)
{
    T_BLE_AUDIO_SYNC_INFO sync_info = {0};
    rtk_bt_le_audio_sync_get_sync_info_param_t *param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    param = (rtk_bt_le_audio_sync_get_sync_info_param_t*)data;

    if(false == ble_audio_sync_get_info(param->sync_handle, &sync_info)) {
        BT_API_PRINT(BT_API_ERROR,"%s ble_audio_sync_get_info fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    param->p_sync_info->advertiser_address.type = sync_info.advertiser_address_type;
    memcpy(param->p_sync_info->advertiser_address.addr_val, sync_info.advertiser_address,GAP_BD_ADDR_LEN);
    param->p_sync_info->adv_sid = sync_info.adv_sid;
    memcpy(param->p_sync_info->broadcast_id, sync_info.broadcast_id,BROADCAST_ID_LEN);
    param->p_sync_info->sync_id = sync_info.sync_id;
    //make sure the definition of stack and api for rtk_bt_le_audio_pa_sync_state_t is SAME!!
    param->p_sync_info->pa_state = (rtk_bt_le_audio_pa_sync_state_t)sync_info.pa_state;
    param->p_sync_info->pa_interval = sync_info.pa_interval;
    param->p_sync_info->base_data_mapping = sync_info.p_base_mapping;
    param->p_sync_info->pa_sync_addr.type = sync_info.pa_sync_addr_type;
    memcpy(param->p_sync_info->pa_sync_addr.addr_val, sync_info.pa_sync_addr,GAP_BD_ADDR_LEN);
    param->p_sync_info->big_info_received = sync_info.big_info_received;
    //make sure the definition of stack and api for big_info is SAME!!
    memcpy(&param->p_sync_info->big_info, &sync_info.big_info,sizeof(rtk_bt_le_audio_biginfo_t));
    //make sure the definition of stack and api for rtk_bt_le_audio_big_sync_state_t is SAME!!
    param->p_sync_info->big_state = (rtk_bt_le_audio_big_sync_state_t)sync_info.big_state;

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_sync_get_bis_codec_cfg(void *data)
{ 
    T_BLE_AUDIO_SYNC_INFO sync_info = {0};
    T_CODEC_CFG codec_parsed_data = {0};
    rtk_bt_le_audio_sync_get_bis_codec_cfg_param_t *param = NULL;     

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    param = (rtk_bt_le_audio_sync_get_bis_codec_cfg_param_t*)data; 

	if(param->p_codec_cfg == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error p_codec_cfg == NULL\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    if(false == ble_audio_sync_get_info(param->sync_handle, &sync_info)) {
        BT_API_PRINT(BT_API_ERROR,"%s ble_audio_sync_get_info fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    if(false == base_data_get_bis_codec_cfg(sync_info.p_base_mapping, param->bis_idx, &codec_parsed_data)) {
        BT_API_PRINT(BT_API_ERROR,"%s base_data_get_bis_codec_cfg fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    param->p_codec_cfg->type_exist = codec_parsed_data.type_exist;
    param->p_codec_cfg->frame_duration = codec_parsed_data.frame_duration;
    param->p_codec_cfg->sample_frequency = codec_parsed_data.sample_frequency;
    param->p_codec_cfg->codec_frame_blocks_per_sdu = codec_parsed_data.codec_frame_blocks_per_sdu;
    param->p_codec_cfg->octets_per_codec_frame =  codec_parsed_data.octets_per_codec_frame;
    param->p_codec_cfg->audio_channel_allocation = codec_parsed_data.audio_channel_allocation;                    

    return RTK_BT_OK;
}
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
static uint16_t bt_stack_le_audio_big_sync_establish(void *data)
{
    rtk_bt_le_audio_big_sync_establish_param_t *p_param = NULL; 
    T_BIG_MGR_SYNC_RECEIVER_BIG_CREATE_SYNC_PARAM sync_param = {0};

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_big_sync_establish_param_t *)data;

    sync_param.encryption = p_param->establish_param.encryption;
    memcpy(sync_param.broadcast_code,p_param->establish_param.broadcast_code,RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN);
    sync_param.mse = p_param->establish_param.mse;
    sync_param.big_sync_timeout = p_param->establish_param.big_sync_timeout;
    sync_param.num_bis = p_param->establish_param.num_bis;
    memcpy(sync_param.bis,p_param->establish_param.bis,p_param->establish_param.num_bis);    

    BT_API_PRINT(BT_API_WARNING,"%s sync_handle %p, encryption = %d, mse = %d, big_sync_timeout = %d, num_bis = %d, bis[0]= %d\r\n",__func__,p_param->sync_handle, sync_param.encryption,sync_param.mse,sync_param.big_sync_timeout,sync_param.num_bis,sync_param.bis[0]);

    if(false == ble_audio_big_sync_establish(p_param->sync_handle, &sync_param)){
        BT_API_PRINT(BT_API_ERROR,"%s ble_audio_big_sync_establish fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_big_sync_terminate(void *data)
{
    rtk_bt_le_audio_sync_handle_t sync_handle = NULL; 

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    sync_handle = *(rtk_bt_le_audio_sync_handle_t*)data;

    BT_API_PRINT(BT_API_DEBUG,"%s sync_handle %p\r\n",__func__,sync_handle);

    if(false == ble_audio_big_terminate(sync_handle)){
        BT_API_PRINT(BT_API_ERROR,"%s ble_audio_big_terminate fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_sync_get_bis_info(void *data)
{
    uint8_t  i = 0;
    T_BLE_AUDIO_BIS_INFO bis_info = {0};
    rtk_bt_le_audio_sync_get_bis_info_param_t * p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_sync_get_bis_info_param_t *)data;
   
	if(p_param->p_bis_info == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error p_bis_info is NULL\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    } 

    if(false == ble_audio_get_bis_sync_info(p_param->sync_handle,&bis_info)){
        BT_API_PRINT(BT_API_ERROR,"%s ble_audio_get_bis_sync_info fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    p_param->p_bis_info->num_bis = bis_info.bis_num;
    if(bis_info.bis_num > RTK_BT_LE_AUDIO_RECEIVER_BIS_MAX_NUM){
        BT_API_PRINT(BT_API_ERROR,"%s bis_num(%d) > RTK_BT_LE_AUDIO_RECEIVER_BIS_MAX_NUM(%d) fail\r\n",__func__,bis_info.bis_num, RTK_BT_LE_AUDIO_RECEIVER_BIS_MAX_NUM);
        return RTK_BT_ERR_NO_RESOURCE;
    }

    for(i = 0; i < bis_info.bis_num; i++ ) {
        p_param->p_bis_info->bis_conn_info[i].bis_idx = bis_info.bis_info[i].bis_idx;
        p_param->p_bis_info->bis_conn_info[i].bis_conn_handle = bis_info.bis_info[i].bis_conn_handle;
    }

    return RTK_BT_OK;
}

static bool bt_stack_le_audio_sync_audio_channel_support(uint32_t source_audio_location,uint32_t sink_audio_location) 
{
    uint32_t chnl = 0;

    if (source_audio_location == 0)
        return true;

    chnl = source_audio_location;
    while(chnl) {
        if(chnl & sink_audio_location) {
            return true;
        } 
        chnl = chnl >> 2;
    }

    return false;
}

static uint16_t bt_stack_le_audio_sync_get_support_bis_array(void *data)
{  
    uint32_t support_bis_array = 0;
    T_BLE_AUDIO_SYNC_INFO sync_info = {0};
    T_BASE_DATA_SUBGROUP_PARAM *p_subgroup = NULL;
    T_BASE_DATA_BIS_PARAM *p_bis = NULL;
    uint8_t i = 0, j = 0;
    rtk_bt_le_audio_sync_get_support_bis_array_param_t *p_param = NULL;    

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_sync_get_support_bis_array_param_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s sync_handle=%p,sink_audio_location=0x%x\r\n",__func__,p_param->sync_handle,(unsigned int)p_param->sink_audio_location);

    if(false == ble_audio_sync_get_info(p_param->sync_handle, &sync_info)) {
        BT_API_PRINT(BT_API_ERROR,"%s ble_audio_sync_get_info fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    if (sync_info.p_base_mapping == NULL || sync_info.p_base_mapping->p_subgroup == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s sync_info.p_base_mapping == NULL || sync_info.p_base_mapping->p_subgroup == NULL\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    for (i = 0; i < sync_info.p_base_mapping->num_subgroups; i++) {
        p_subgroup = &sync_info.p_base_mapping->p_subgroup[i];
        if (p_subgroup == NULL) {
            continue;
        }        
        for (j = 0; j < p_subgroup->num_bis; j++) {
            p_bis = &p_subgroup->p_bis_param[j];
            if (p_bis == NULL) {
                continue;
            }            

            if (pacs_check_codec_cfg(SERVER_AUDIO_SINK, p_bis->codec_id, &p_bis->bis_codec_cfg)) {
                BT_API_PRINT(BT_API_DEBUG,"%s p_bis->bis_codec_cfg.audio_channel_allocation=0x%x\r\n",__func__,(unsigned int)p_bis->bis_codec_cfg.audio_channel_allocation);
                if(bt_stack_le_audio_sync_audio_channel_support(p_bis->bis_codec_cfg.audio_channel_allocation,p_param->sink_audio_location)) {
                    support_bis_array |= (1 << (sync_info.p_base_mapping->p_subgroup[i].p_bis_param[j].bis_index - 1));
                    BT_API_PRINT(BT_API_DEBUG,"%s support_bis_array=0x%x\r\n",__func__,(unsigned int)support_bis_array);
                }
            }
        }
    }

    *p_param->p_support_bis_array = support_bis_array;
    BT_API_PRINT(BT_API_DEBUG,"%s *p_param->p_support_bis_array=0x%x\r\n",__func__,(unsigned int)*p_param->p_support_bis_array);
    
    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_sync_setup_data_path(void *data)
{  
    rtk_bt_le_audio_sync_setup_data_path_param_t *p_param = NULL;    

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_sync_setup_data_path_param_t *)data; 

    if(false == ble_audio_bis_setup_data_path(p_param->sync_handle,
                                              p_param->bis_idx,
                                              p_param->setup_param.codec_id, 
                                              p_param->setup_param.controller_delay,
                                              p_param->setup_param.codec_config_len,
                                              p_param->setup_param.p_codec_config)) {
        BT_API_PRINT(BT_API_ERROR,"%s ble_audio_bis_setup_data_path fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}
static uint16_t bt_stack_le_audio_sync_remove_data_path(void *data)
{
    rtk_bt_le_audio_sync_remove_data_path_param_t *p_param = NULL;    

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_sync_remove_data_path_param_t *)data; 

    if(false == ble_audio_bis_remove_data_path(p_param->sync_handle, p_param->bis_idx)) {
        BT_API_PRINT(BT_API_ERROR,"%s ble_audio_bis_setup_data_path fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}
#endif 
#endif 
#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
static void bt_stack_le_audio_broadcast_source_cb(T_BROADCAST_SOURCE_HANDLE handle, uint8_t cb_type, void *p_cb_data)
{
    T_BROADCAST_SOURCE_SM_CB_DATA *p_sm_data = (T_BROADCAST_SOURCE_SM_CB_DATA *)p_cb_data;
	rtk_bt_cmd_t *p_cmd = NULL; 
    rtk_bt_evt_t *p_evt = NULL;

    BT_API_PRINT(BT_API_DEBUG,"%s: cb_type = 0x%x\r\n",__func__,cb_type);

    switch (cb_type) {
    case MSG_BROADCAST_SOURCE_STATE_CHANGE: {
            APP_PRINT_INFO2("MSG_BROADCAST_SOURCE_STATE_CHANGE: state %d, cause 0x%x", p_sm_data->p_state_change->state,p_sm_data->p_state_change->cause);
            BT_API_PRINT(BT_API_DEBUG,"MSG_BROADCAST_SOURCE_STATE_CHANGE: state %d, cause 0x%x\r\n", p_sm_data->p_state_change->state,p_sm_data->p_state_change->cause);
            //complete RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_DISABLE cmd                   
            if(p_sm_data->p_state_change->state == BROADCAST_SOURCE_STATE_CONFIGURED) {
                p_cmd = bt_stack_pending_cmd_search(RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_DISABLE);
                if (p_cmd) {               
                    bt_stack_pending_cmd_delete(p_cmd);
                    if(p_sm_data->p_state_change->cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE))
                        p_cmd->ret = p_sm_data->p_state_change->cause;
                    else 
                        p_cmd->ret = 0;
                    osif_sem_give(p_cmd->psem);
                } 
            }
            //complete RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RELEASE cmd   
            if(p_sm_data->p_state_change->state == BROADCAST_SOURCE_STATE_IDLE) {
                p_cmd = bt_stack_pending_cmd_search(RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RELEASE);
                if (p_cmd) {               
                    bt_stack_pending_cmd_delete(p_cmd);
                    p_cmd->ret = p_sm_data->p_state_change->cause;
                    osif_sem_give(p_cmd->psem);
                } 
            }
            rtk_bt_le_audio_broadcast_source_state_ind_t *ind = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                        RTK_BT_LE_AUDIO_EVT_BROADCAST_SOURCE_STATE_IND,
                                        sizeof(rtk_bt_le_audio_broadcast_source_state_ind_t));
            if (!p_evt) {
                BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                break;
            } 
            ind = (rtk_bt_le_audio_broadcast_source_state_ind_t *)p_evt->data;        
            ind->broadcast_source_handle = handle;
            ind->broadcast_source_state = p_sm_data->p_state_change->state;
            ind->cause = p_sm_data->p_state_change->cause;
            /* Send event */
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;

    case MSG_BROADCAST_SOURCE_RECONFIG: {
            APP_PRINT_INFO1("MSG_BROADCAST_SOURCE_RECONFIG: cause 0x%x", p_sm_data->cause);
            BT_API_PRINT(BT_API_DEBUG,"MSG_BROADCAST_SOURCE_RECONFIG: cause 0x%x\r\n", p_sm_data->cause);
            p_cmd = bt_stack_pending_cmd_search(RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RECONFIG);
            if (p_cmd) {
                bt_stack_pending_cmd_delete(p_cmd);
                p_cmd->ret = p_sm_data->cause;
                osif_sem_give(p_cmd->psem);
            } else {
                BT_API_PRINT(BT_API_ERROR,"[%s] MSG_BROADCAST_SOURCE_RECONFIG: find no pending command \r\n", __func__);
            }    
        }
        break;

    case MSG_BROADCAST_SOURCE_SETUP_DATA_PATH: {
            APP_PRINT_INFO3("MSG_BROADCAST_SOURCE_SETUP_DATA_PATH: bis_idx %d,bis_conn_handle 0x%x, cause 0x%x",
                                 p_sm_data->p_setup_data_path->bis_idx, p_sm_data->p_setup_data_path->bis_conn_handle,p_sm_data->p_setup_data_path->cause);
            BT_API_PRINT(BT_API_DEBUG,"MSG_BROADCAST_SOURCE_SETUP_DATA_PATH: bis_idx %d,bis_conn_handle 0x%x,  cause 0x%x\r\n",
                                 p_sm_data->p_setup_data_path->bis_idx,p_sm_data->p_setup_data_path->bis_conn_handle, p_sm_data->p_setup_data_path->cause);

            p_cmd = bt_stack_pending_cmd_search(RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_SETUP_DATA_PATH);
            if (p_cmd) {       
                rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *param = (rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *)p_cmd->param;         
                bt_stack_pending_cmd_delete(p_cmd);
                p_cmd->ret = p_sm_data->p_setup_data_path->cause;
                *param->p_bis_conn_handle = p_sm_data->p_setup_data_path->bis_conn_handle;
                osif_sem_give(p_cmd->psem);
            } else {
                BT_API_PRINT(BT_API_ERROR,"[%s] MSG_BROADCAST_SOURCE_SETUP_DATA_PATH: find no pending command \r\n", __func__);
            }                                  
        }
        break;

    case MSG_BROADCAST_SOURCE_REMOVE_DATA_PATH: {
            APP_PRINT_INFO2("MSG_BROADCAST_SOURCE_REMOVE_DATA_PATH: bis_conn_handle 0x%x, cause 0x%x",
                                 p_sm_data->p_remove_data_path->bis_conn_handle,p_sm_data->p_remove_data_path->cause);
            BT_API_PRINT(BT_API_DEBUG,"MSG_BROADCAST_SOURCE_REMOVE_DATA_PATH: bis_conn_handle %d, cause 0x%x\r\n",
                                 p_sm_data->p_remove_data_path->bis_conn_handle,p_sm_data->p_remove_data_path->cause);
            p_cmd = bt_stack_pending_cmd_search(RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_REMOVE_DATA_PATH);
            if (p_cmd) {
                bt_stack_pending_cmd_delete(p_cmd);
                p_cmd->ret = p_sm_data->p_remove_data_path->cause;
                osif_sem_give(p_cmd->psem);
            } else {
                BT_API_PRINT(BT_API_ERROR,"[%s] MSG_BROADCAST_SOURCE_REMOVE_DATA_PATH: find no pending command \r\n", __func__);
            }   
        }
        break;

    default:
        BT_API_PRINT(BT_API_ERROR,"%s: default cb_type 0x%04x, not process!\r\n",__func__, cb_type);
        break;
    }
}

static uint16_t bt_stack_le_audio_broadcast_source_create(void *data)
{
    T_BROADCAST_SOURCE_HANDLE broadcast_source_handle = NULL; 
    uint8_t group_idx = 0xFF, num_groups = 0;
    uint8_t subgroup_idx = 0xFF,num_subgroups = 0;
    uint8_t bis_idx = 0xFF,num_bis = 0;
    uint8_t i = 0, j = 0, k = 0;
    uint32_t presentation_delay = 0;
    rtk_bt_le_audio_broadcast_source_create_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_broadcast_source_create_param_t *)data;
    num_groups = p_param->num_groups;
    if(num_groups > RTK_BT_LE_AUDIO_BROADCASTER_GROUP_MAX_NUM){
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error,num_groups(%d) > max (%d)\r\n",__func__,num_groups,RTK_BT_LE_AUDIO_BROADCASTER_GROUP_MAX_NUM);
        return RTK_BT_ERR_NO_RESOURCE;
    }  

    //broadcast_source_add can add once only, because there is no broadcast_source_delete
    if(g_broadcast_source_handle == NULL) {
        broadcast_source_handle = broadcast_source_add(bt_stack_le_audio_broadcast_source_cb);
        if(broadcast_source_handle == NULL) {
            BT_API_PRINT(BT_API_ERROR,"%s broadcast_source_add fail\r\n",__func__);
            return RTK_BT_ERR_LOWER_STACK_API;
        }
        g_broadcast_source_handle = broadcast_source_handle;
    } else {
        broadcast_source_handle = g_broadcast_source_handle;
    }

    for (i = 0; i < num_groups; i++) {
        num_subgroups = p_param->group[i].num_subgroups;
        presentation_delay = p_param->group[i].presentation_delay;

        BT_API_PRINT(BT_API_DEBUG,"%s broadcast_source_add num_subgroups=%d,presentation_delay=%d\r\n",__func__,num_subgroups,(int)presentation_delay);
        if(false == base_data_add_group(&group_idx,presentation_delay)) {
            BT_API_PRINT(BT_API_ERROR,"%s broadcast_source_add for group %d fail\r\n",__func__,i);
            return RTK_BT_ERR_LOWER_STACK_API;
        }
        *p_param->group[i].group_idx = group_idx;        
        for (j = 0; j < num_subgroups; j++) {
            num_bis = p_param->group[i].subgroup[j].num_bis;
            BT_API_PRINT(BT_API_DEBUG,"%s num_bis = %d \r\n",__func__,num_bis);
            BT_API_DUMPBUF(BT_API_DEBUG,__func__,p_param->group[i].subgroup[j].p_codec_cfg_l2,p_param->group[i].subgroup[j].codec_cfg_l2_len);
            BT_API_DUMPBUF(BT_API_DEBUG,__func__,p_param->group[i].subgroup[j].p_metadata,p_param->group[i].subgroup[j].metadata_len);
            if(false == base_data_add_subgroup(&subgroup_idx,group_idx,
                                                p_param->group[i].subgroup[j].codec_id,
                                                p_param->group[i].subgroup[j].codec_cfg_l2_len,
                                                p_param->group[i].subgroup[j].p_codec_cfg_l2,
                                                p_param->group[i].subgroup[j].metadata_len,
                                                p_param->group[i].subgroup[j].p_metadata)) {
                BT_API_PRINT(BT_API_ERROR,"%s broadcast_source_add group %d subgroup %d fail\r\n",__func__,i,j);
                return RTK_BT_ERR_LOWER_STACK_API;
            } 
            *p_param->group[i].subgroup[j].subgroup_idx = subgroup_idx;
            for (k = 0;  k < num_bis; k++) {
                if(false == base_data_add_bis(&bis_idx,group_idx,subgroup_idx,
                                                p_param->group[i].subgroup[j].bis[k].codec_cfg_l3_len,
                                                p_param->group[i].subgroup[j].bis[k].p_codec_cfg_l3)) {
                    BT_API_PRINT(BT_API_ERROR,"%s broadcast_source_add group %d subgroup %d bis %d fail\r\n",__func__,i,j,k);
                    return RTK_BT_ERR_LOWER_STACK_API;
                } 
                *p_param->group[i].subgroup[j].bis[k].bis_idx = bis_idx; 
            }              
        }             
    } 
    BT_API_PRINT(BT_API_DEBUG,"%s broadcast_source_handle=%p\r\n",__func__, broadcast_source_handle);   
    *p_param->p_broadcast_source_handle = (rtk_bt_le_audio_broadcast_source_handle_t)broadcast_source_handle;    
    
    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_broadcast_source_get_info(void *data)
{
    rtk_bt_le_audio_broadcast_source_info_get_param_t *p_param = NULL; 
    T_BROADCAST_SOURCE_INFO info = {0};

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_broadcast_source_info_get_param_t *)data;    

    if(false == broadcast_source_get_info(p_param->broadcast_source_handle, &info)){
        BT_API_PRINT(BT_API_ERROR,"%s broadcast_source_get_info fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    //make sure the definition of stack and api for broadcast_source_info is SAME!!
    memcpy(p_param->p_info,&info,sizeof(rtk_bt_le_audio_broadcast_source_info_t));

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_broadcast_source_config(void *data)
{
    rtk_bt_le_audio_broadcast_source_config_param_t *p_param = NULL; 
    rtk_bt_le_audio_broadcast_source_config_t *p_config = NULL; 
    uint8_t *broadcast_audio_announcements = NULL;
    uint8_t *broadcast_id = NULL;
    uint8_t id_offset = 0;

    uint16_t pa_data_len = 0;
    uint8_t *pa_data = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_broadcast_source_config_param_t *)data;
    p_config = &p_param->config;
    broadcast_audio_announcements = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,p_param->p_announcements_len);
    memcpy((void *)broadcast_audio_announcements, p_param->p_broadcast_audio_announcements, p_param->p_announcements_len);

    BT_API_PRINT(BT_API_DEBUG,"%s: broadcast_source_handle = %p, group_idx = %d\r\n",__func__,p_param->broadcast_source_handle,p_param->group_idx);

    T_BROADCAST_SOURCE_INFO src_info = {0};
    if (broadcast_source_get_info(p_param->broadcast_source_handle, &src_info) == false) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: broadcast_source_get_info fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 
    id_offset = 4;//broacast id offset
    broadcast_id = broadcast_audio_announcements + id_offset;
    memcpy(broadcast_id, src_info.broadcast_id, RTK_BT_LE_AUDIO_BROADCAST_ID_LEN);

    BT_API_DUMPBUF(BT_API_DEBUG,__func__,broadcast_audio_announcements,p_param->p_announcements_len);

    if (false == broadcast_source_set_eadv_param(p_param->broadcast_source_handle,
                                                p_param->adv_sid,
                                                p_config->primary_adv_interval_min, 
                                                p_config->primary_adv_interval_max,
                                                p_config->primary_adv_channel_map, 
                                                (T_GAP_LOCAL_ADDR_TYPE)p_config->own_addr.type, 
                                                p_config->own_addr.addr_val,
                                                (T_GAP_ADV_FILTER_POLICY)p_config->filter_policy, 
                                                p_config->tx_power,
                                                (T_GAP_PHYS_PRIM_ADV_TYPE)p_config->primary_adv_phy, 
                                                p_config->secondary_adv_max_skip,
                                                (T_GAP_PHYS_TYPE)p_config->secondary_adv_phy,
                                                p_param->p_announcements_len, broadcast_audio_announcements)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: broadcast_source_set_eadv_param fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    if (false == base_data_gen_pa_data(p_param->group_idx, &pa_data_len, &pa_data)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: base_data_gen_pa_data fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    BT_API_DUMPBUF(BT_API_DEBUG,__func__,pa_data,pa_data_len);
    
    if (false == broadcast_source_set_pa_param(p_param->broadcast_source_handle, p_config->periodic_adv_interval_min,
                                                p_config->periodic_adv_interval_max, p_config->periodic_adv_prop,
                                                pa_data_len, pa_data)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: broadcast_source_set_pa_param fail\r\n",__func__);
        if (pa_data) {
            osif_mem_free(pa_data);
        }  
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    if (false == broadcast_source_config(p_param->broadcast_source_handle)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: broadcast_source_config fail\r\n",__func__);
        if (pa_data) {
            osif_mem_free(pa_data);
        }  
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    if (pa_data) {
        osif_mem_free(pa_data);
    }  

    if (broadcast_audio_announcements) {
        osif_mem_free(broadcast_audio_announcements);
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_broadcast_source_enable(void *data)
{
    rtk_bt_le_audio_broadcast_source_enable_param_t *p_param = NULL; 
    rtk_bt_le_audio_broadcast_source_create_big_t *p_create_big = NULL; 
    T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_PARAM big_param = {0};

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_broadcast_source_enable_param_t *)data;
    p_create_big = &p_param->create_big;

    //make sure the definition of stack T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_PARAM and api for rtk_bt_le_audio_broadcast_source_create_big_t is SAME!!
    memcpy(&big_param,p_create_big,sizeof(T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_PARAM));

    if (false == broadcast_source_establish(p_param->broadcast_source_handle, big_param)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: broadcast_source_establish fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 
 
    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_broadcast_source_disable(void *data)
{
    rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    broadcast_source_handle = *(rtk_bt_le_audio_broadcast_source_handle_t *)data;

    if (false == broadcast_source_disable(broadcast_source_handle, HCI_ERR_LOCAL_HOST_TERMINATE)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: broadcast_source_disable fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 
 
    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_broadcast_source_release(void *data)
{
    rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    broadcast_source_handle = *(rtk_bt_le_audio_broadcast_source_handle_t *)data;

    if (false == broadcast_source_release(broadcast_source_handle)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: broadcast_source_release fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 
 
    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_broadcast_source_setup_data_path(void *data)
{
    rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *)data;

    if (false == broadcast_source_setup_data_path(p_param->broadcast_source_handle, 
                                                    p_param->bis_idx, 
                                                    p_param->setup_param.codec_id,
                                                    p_param->setup_param.controller_delay,
                                                    p_param->setup_param.codec_config_len,
                                                    p_param->setup_param.p_codec_config)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: broadcast_source_setup_data_path fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 
 
    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_broadcast_source_remove_data_path(void *data)
{
    rtk_bt_le_audio_broadcast_source_remove_data_path_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_broadcast_source_remove_data_path_param_t *)data;

    if (false == broadcast_source_remove_data_path(p_param->broadcast_source_handle, p_param->bis_idx)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: broadcast_source_remove_data_path fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 
 
    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_broadcast_source_reconfig(void *data)
{
    uint16_t pa_data_len = 0;
    uint8_t *pa_data = NULL;
    rtk_bt_le_audio_broadcast_source_reconfig_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_broadcast_source_reconfig_param_t *)data;

    if (false == base_data_update_metadata(p_param->group_idx, 
                                            p_param->subgroup_idx,
                                            p_param->metadata_len, 
                                            p_param->p_metadata)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: base_data_update_metadata fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    if (false == base_data_gen_pa_data(p_param->group_idx, &pa_data_len, &pa_data)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: base_data_gen_pa_data fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    if (false == broadcast_source_reconfig(p_param->broadcast_source_handle, pa_data_len, pa_data)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: broadcast_source_reconfig fail\r\n",__func__);
        if (pa_data && pa_data_len) {
            osif_mem_free(pa_data);
        }
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    if (pa_data && pa_data_len) {
        osif_mem_free(pa_data);
    }
    return RTK_BT_OK;
}
#endif //#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT

#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
static uint16_t bt_stack_le_audio_bass_client_get_brs_data(void *data)
{
    T_BASS_BRS_DATA *p_brs_data = NULL;
    rtk_bt_le_audio_bass_client_get_brs_data_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_bass_client_get_brs_data_param_t *)data;

    BT_API_PRINT(BT_API_DEBUG,"%s conn_handle = %d, char_instance_id = %d\r\n",__func__,p_param->conn_handle,p_param->char_instance_id);

    p_brs_data = bass_get_brs_data(p_param->conn_handle,p_param->char_instance_id);
    if (p_brs_data == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bass_get_brs_data fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 
    
    if(p_param->p_brs_data) {
        p_param->p_brs_data->brs_is_used = p_brs_data->brs_is_used;
        p_param->p_brs_data->source_id = p_brs_data->source_id;
        p_param->p_brs_data->source_address.type = p_brs_data->source_address_type;
        memcpy(p_param->p_brs_data->source_address.addr_val,p_brs_data->source_address,RTK_BD_ADDR_LEN);
        p_param->p_brs_data->source_adv_sid = p_brs_data->source_adv_sid;
        memcpy(p_param->p_brs_data->broadcast_id,p_brs_data->broadcast_id,RTK_BT_LE_AUDIO_BROADCAST_ID_LEN);
        p_param->p_brs_data->pa_sync_state = p_brs_data->pa_sync_state;
        p_param->p_brs_data->big_encryption = p_brs_data->big_encryption;
        memcpy(p_param->p_brs_data->bad_code, p_brs_data->bad_code, RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN);
        p_param->p_brs_data->num_subgroups = p_brs_data->num_subgroups;
        p_param->p_brs_data->bis_info_size = p_brs_data->bis_info_size;
        if(p_param->is_get_bis_info && p_brs_data->num_subgroups && p_param->p_brs_data->p_cp_bis_info) {
            for (uint8_t i = 0; i < p_brs_data->num_subgroups; i++) {
                p_param->p_brs_data->p_cp_bis_info[i].bis_sync_state = p_brs_data->p_cp_bis_info[i].bis_sync;
                p_param->p_brs_data->p_cp_bis_info[i].metadata_len = p_brs_data->p_cp_bis_info[i].metadata_len;
                p_param->p_brs_data->p_cp_bis_info[i].p_metadata = p_brs_data->p_cp_bis_info[i].p_metadata;
            }
        }
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_bass_client_cp_add_source_by_sync_info(void *data)
{
    rtk_bt_le_audio_bass_client_cp_add_source_by_sync_info_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_bass_client_cp_add_source_by_sync_info_param_t *)data;

    BT_API_PRINT(BT_API_DEBUG,"%s sync_handle %p conn_handle = %d, pa_sync = %d\r\n",__func__,p_param->sync_handle,p_param->conn_handle,p_param->pa_sync);

    if (false == bass_cp_add_source_by_sync_info(p_param->sync_handle, 
                                            p_param->conn_handle,
                                            (T_BASS_PA_SYNC)p_param->pa_sync, 
                                            p_param->bis_array, 
                                            p_param->is_req)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bass_cp_add_source_by_sync_info fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_bass_client_cp_modify_source_by_sync_info(void *data)
{
    rtk_bt_le_audio_bass_client_cp_modify_source_by_sync_info_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_bass_client_cp_modify_source_by_sync_info_param_t *)data;

    BT_API_PRINT(BT_API_DEBUG,"%s sync_handle %p conn_handle = %d, source_id = %d, pa_sync = %d\r\n",__func__,p_param->sync_handle,p_param->conn_handle,p_param->source_id, p_param->pa_sync);

    if (false == bass_cp_modify_source_by_sync_info(p_param->sync_handle, 
                                            p_param->conn_handle,
                                            p_param->source_id,
                                            (T_BASS_PA_SYNC)p_param->pa_sync, 
                                            p_param->bis_array, 
                                            p_param->is_req)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bass_cp_add_source_by_sync_info fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_bass_client_write_cp_op(void *data)
{
    rtk_bt_le_audio_bass_client_write_cp_op_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_bass_client_write_cp_op_param_t *)data;

    BT_API_PRINT(BT_API_DEBUG,"%s conn_handle=%d,opcode=%d,is_req=%d\r\n",__func__,p_param->conn_handle,p_param->opcode,p_param->is_req);

    switch(p_param->opcode) {
        case RTK_BT_LE_AUDIO_BASS_CP_REMOTE_SCAN_STOP:{
            if (false == bass_cp_remote_scan_stop(p_param->conn_handle,p_param->is_req)) {
                BT_API_PRINT(BT_API_ERROR,"%s fail: bass_cp_remote_scan_stop fail\r\n",__func__);
                return RTK_BT_ERR_LOWER_STACK_API;
            } 
        }
        break;

        case RTK_BT_LE_AUDIO_BASS_CP_REMOTE_SCAN_START:{
            if (false == bass_cp_remote_scan_start(p_param->conn_handle,p_param->is_req)) {
                BT_API_PRINT(BT_API_ERROR,"%s fail: bass_cp_remote_scan_start fail\r\n",__func__);
                return RTK_BT_ERR_LOWER_STACK_API;
            } 
        }
        break;

        case RTK_BT_LE_AUDIO_BASS_CP_ADD_SOURCE:{
            T_BASS_CP_ADD_SOURCE cp_data = {0};
            cp_data.advertiser_address_type = p_param->op_param.add_source_param.adv_addr.type;
            memcpy(cp_data.advertiser_address,p_param->op_param.add_source_param.adv_addr.addr_val,RTK_BD_ADDR_LEN);
            cp_data.advertiser_sid = p_param->op_param.add_source_param.adv_sid;
            memcpy(cp_data.broadcast_id,p_param->op_param.add_source_param.broadcast_id,RTK_BT_LE_AUDIO_BROADCAST_ID_LEN);
            cp_data.pa_sync = (T_BASS_PA_SYNC)p_param->op_param.add_source_param.pa_sync;
            cp_data.pa_interval = p_param->op_param.add_source_param.pa_interval;
            cp_data.num_subgroups = p_param->op_param.add_source_param.num_subgroups;
            cp_data.bis_info_size = p_param->op_param.add_source_param.bis_info_size;
            cp_data.p_cp_bis_info = (T_BASS_CP_BIS_INFO *)osif_mem_alloc(RAM_TYPE_DATA_ON,p_param->op_param.add_source_param.bis_info_size);
            for(uint8_t i = 0; i < p_param->op_param.add_source_param.num_subgroups; i++) {
                cp_data.p_cp_bis_info[i].bis_sync = p_param->op_param.add_source_param.p_cp_bis_info[i].bis_sync_state;
                cp_data.p_cp_bis_info[i].metadata_len = p_param->op_param.add_source_param.p_cp_bis_info[i].metadata_len;
                cp_data.p_cp_bis_info[i].p_metadata = p_param->op_param.add_source_param.p_cp_bis_info[i].p_metadata;
            }
            if (false == bass_cp_add_source(p_param->conn_handle,&cp_data, p_param->is_req)) {
                BT_API_PRINT(BT_API_ERROR,"%s fail: bass_cp_add_source fail\r\n",__func__);
                return RTK_BT_ERR_LOWER_STACK_API;
            } 
            if(cp_data.p_cp_bis_info)
                osif_mem_free(cp_data.p_cp_bis_info);
        }
        break;

        case RTK_BT_LE_AUDIO_BASS_CP_MODIFY_SOURCE:{
            T_BASS_CP_MODIFY_SOURCE cp_data = {0};
            cp_data.source_id = p_param->op_param.modify_source_param.source_id;
            cp_data.pa_sync = (T_BASS_PA_SYNC)p_param->op_param.modify_source_param.pa_sync;
            cp_data.pa_interval = p_param->op_param.modify_source_param.pa_interval;
            cp_data.num_subgroups = p_param->op_param.modify_source_param.num_subgroups;
            cp_data.bis_info_size = p_param->op_param.modify_source_param.bis_info_size;
            cp_data.p_cp_bis_info = (T_BASS_CP_BIS_INFO *) osif_mem_alloc(RAM_TYPE_DATA_ON,p_param->op_param.modify_source_param.bis_info_size);
            for(uint8_t i = 0; i < p_param->op_param.modify_source_param.num_subgroups; i++) {
                cp_data.p_cp_bis_info[i].bis_sync = p_param->op_param.modify_source_param.p_cp_bis_info[i].bis_sync_state;
                cp_data.p_cp_bis_info[i].metadata_len = p_param->op_param.modify_source_param.p_cp_bis_info[i].metadata_len;
                cp_data.p_cp_bis_info[i].p_metadata = p_param->op_param.modify_source_param.p_cp_bis_info[i].p_metadata;
            }
            if (false == bass_cp_modify_source(p_param->conn_handle,&cp_data, p_param->is_req)) {
                BT_API_PRINT(BT_API_ERROR,"%s fail: bass_cp_modify_source fail\r\n",__func__);
                return RTK_BT_ERR_LOWER_STACK_API;
            }
            if(cp_data.p_cp_bis_info)
                osif_mem_free(cp_data.p_cp_bis_info); 
        }
        break;   
  
        case RTK_BT_LE_AUDIO_BASS_CP_REMOVE_SOURCE:{
            T_BASS_CP_REMOVE_SOURCE cp_data = {0};
            cp_data.source_id = p_param->op_param.remove_source_param.source_id;
            if (false == bass_cp_remove_source(p_param->conn_handle,&cp_data, p_param->is_req)) {
                BT_API_PRINT(BT_API_ERROR,"%s fail: bass_cp_remove_source fail\r\n",__func__);
                return RTK_BT_ERR_LOWER_STACK_API;
            }
        }
        break;  
     
        case RTK_BT_LE_AUDIO_BASS_CP_SET_BROADCAST_CODE:{
            T_BASS_CP_SET_BROADCAST_CODE cp_data = {0};
            cp_data.source_id = p_param->op_param.set_broadcast_code_param.source_id;
            memcpy(cp_data.broadcast_code,p_param->op_param.set_broadcast_code_param.broadcast_code,RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN);
            if (false == bass_cp_set_broadcast_code(p_param->conn_handle,&cp_data, p_param->is_req)) {
                BT_API_PRINT(BT_API_ERROR,"%s fail: bass_cp_set_broadcast_code fail\r\n",__func__);
                return RTK_BT_ERR_LOWER_STACK_API;
            }
        }
        break;     
    }
    return RTK_BT_OK;
}
                           
static uint16_t bt_stack_le_audio_bass_client_past(void *data)
{
    T_BASS_PAST_SRV_DATA srv_data = {0};
    rtk_bt_le_audio_bass_client_past_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_bass_client_past_param_t *)data;

    srv_data.adv_a_match_ext_adv = p_param->srv_data.adv_a_match_ext_adv;
    srv_data.adv_a_match_src = p_param->srv_data.adv_a_match_src;
    srv_data.source_id = p_param->srv_data.source_id;    

    //check remote past feature
    if (false == ble_audio_check_remote_features(p_param->conn_handle, LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX3, LE_SUPPORT_FEATURES_PAST_RECIPIENT_MASK_BIT)) { 
        BT_API_PRINT(BT_API_ERROR,"%s fail: ble_audio_check_remote_features fail\r\n",__func__);
        return RTK_BT_ERR_UNSUPPORTED;
    }

    //check local past feature
    if (false == le_check_supported_features(LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX3,LE_SUPPORT_FEATURES_PAST_SENDER_MASK_BIT)) { 
        BT_API_PRINT(BT_API_ERROR,"%s fail: le_check_supported_features fail\r\n",__func__);
        return RTK_BT_ERR_UNSUPPORTED;
    }  

    if(p_param->past_type == RTK_BT_LE_AUDIO_BASS_PAST_BY_REMOTE_SRC) {
        if (false == bass_past_by_remote_src((T_BLE_AUDIO_SYNC_HANDLE)p_param->handle, p_param->conn_handle, srv_data)) {
            BT_API_PRINT(BT_API_ERROR,"%s fail: bass_past_by_remote_src fail\r\n",__func__);
            return RTK_BT_ERR_LOWER_STACK_API;
        }
    } else {
        if (false == bass_past_by_local_src((T_BROADCAST_SOURCE_HANDLE)p_param->handle, p_param->conn_handle, srv_data)) {
            BT_API_PRINT(BT_API_ERROR,"%s fail: bass_past_by_local_src fail\r\n",__func__);
            return RTK_BT_ERR_LOWER_STACK_API;
        }                  
    }

    return RTK_BT_OK;
}

#endif

static void bt_stack_le_audio_group_dev_msg_ind(rtk_bt_le_audio_group_dev_msg_type_t msg_type, T_BLE_AUDIO_GROUP_HANDLE group_handle, T_BLE_AUDIO_DEV_HANDLE dev_handle, uint16_t cause)
{
    rtk_bt_evt_t *p_evt = NULL;
    rtk_bt_le_audio_group_dev_msg_ind_t *p_ind = NULL;
    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                RTK_BT_LE_AUDIO_EVT_GROUP_DEV_MSG_IND,
                                sizeof(rtk_bt_le_audio_group_dev_msg_ind_t));
    if (!p_evt) {
        BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
        return;
    } 
    p_ind = (rtk_bt_le_audio_group_dev_msg_ind_t *)p_evt->data;
    p_ind->cause = cause;
    p_ind->msg_type = msg_type;
    p_ind->group_handle = group_handle;
    p_ind->device_handle = dev_handle;
               
    /* Send event */
    rtk_bt_evt_indicate(p_evt, NULL);
}

static void bt_stack_le_audio_group_cb(T_AUDIO_GROUP_MSG msg, T_BLE_AUDIO_GROUP_HANDLE handle, void *buf)
{
    rtk_bt_cmd_t *p_cmd = NULL; 
    rtk_bt_evt_t *p_evt = NULL;
    uint32_t msg_type = 0;
    uint8_t cb_ret = 0;

    switch (msg) {
        case AUDIO_GROUP_MSG_DEV_CONN: {
                T_AUDIO_GROUP_MSG_DEV_CONN *p_data = (T_AUDIO_GROUP_MSG_DEV_CONN *)buf;
                APP_PRINT_INFO2("AUDIO_GROUP_MSG_DEV_CONN: group handle 0x%x, dev handle 0x%x",
                                handle, p_data->dev_handle);
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_DEV_CONN: group handle 0x%p, dev handle 0x%p",
                                handle, p_data->dev_handle);
                bt_stack_le_audio_group_dev_msg_ind(RTK_BT_LE_AUDIO_GROUP_MSG_DEV_CONN,handle, p_data->dev_handle,0);
            }
            break;

        case AUDIO_GROUP_MSG_DEV_DISCONN: {
                T_AUDIO_GROUP_MSG_DEV_DISCONN *p_data = (T_AUDIO_GROUP_MSG_DEV_DISCONN *)buf;
                APP_PRINT_INFO3("AUDIO_GROUP_MSG_DEV_DISCONN: group handle 0x%x, dev handle 0x%x, cause 0x%x",
                                handle, p_data->dev_handle, p_data->cause);
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_DEV_DISCONN: group handle 0x%p, dev handle 0x%p, cause 0x%x\r\n",
                                handle, p_data->dev_handle, p_data->cause);
                bt_stack_le_audio_group_dev_msg_ind(RTK_BT_LE_AUDIO_GROUP_MSG_DEV_DISCONN,handle, p_data->dev_handle,p_data->cause);
            }
            break;

        case AUDIO_GROUP_MSG_DEV_BOND_REMOVE: {
                T_AUDIO_GROUP_MSG_DEV_BOND_REMOVE *p_data = (T_AUDIO_GROUP_MSG_DEV_BOND_REMOVE *)buf;
                APP_PRINT_INFO2("AUDIO_GROUP_MSG_DEV_BOND_REMOVE: group handle 0x%x, dev handle 0x%x",
                                handle, p_data->dev_handle);
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_DEV_BOND_REMOVE: group handle 0x%p, dev handle 0x%p\r\n",
                                handle, p_data->dev_handle);
                bt_stack_le_audio_group_dev_msg_ind(RTK_BT_LE_AUDIO_GROUP_MSG_DEV_BOND_REMOVE,handle, p_data->dev_handle,0);
            }
            break;

        case AUDIO_GROUP_MSG_DEV_BOND_CLEAR: {
                APP_PRINT_INFO1("AUDIO_GROUP_MSG_DEV_BOND_CLEAR: group handle 0x%x", handle);
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_DEV_BOND_CLEAR: group handle 0x%p\r\n", handle);
                bt_stack_le_audio_group_dev_msg_ind(RTK_BT_LE_AUDIO_GROUP_MSG_DEV_BOND_CLEAR,handle, NULL,0);
            }
            break;

        case AUDIO_GROUP_MSG_DEV_EMPTY: {
                APP_PRINT_INFO1("AUDIO_GROUP_MSG_DEV_EMPTY: group handle 0x%x", handle);
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_DEV_EMPTY: group handle 0x%p\r\n", handle);
                bt_stack_le_audio_group_dev_msg_ind(RTK_BT_LE_AUDIO_GROUP_MSG_DEV_EMPTY,handle, NULL,0);
            }
            break;

        case AUDIO_GROUP_MSG_BAP_STATE: {
                T_AUDIO_GROUP_BAP_STATE *p_data = (T_AUDIO_GROUP_BAP_STATE *)buf;
                APP_PRINT_INFO6("AUDIO_GROUP_MSG_BAP_STATE: group handle 0x%x, session handle 0x%x, curr_action %d, state %d, result %d, cause 0x%x",
                            handle, p_data->handle, p_data->curr_action,p_data->state, p_data->result, p_data->cause);
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_BAP_STATE: group handle %p, session handle 0x%p, curr_action %d, state %d, result %d, cause 0x%x\r\n",
                            handle, p_data->handle, p_data->curr_action, p_data->state, p_data->result, p_data->cause);
                if (p_data->result == BAP_UNICAST_RESULT_ASE_CP_ERR) {
                    APP_PRINT_INFO5("AUDIO_GROUP_MSG_BAP_STATE: cp error, conn_handle 0x%x, opcode 0x%x, ase_id %d, response_code 0x%x, reason 0x%x",
                                    p_data->addl_info.cp_error.conn_handle, p_data->addl_info.cp_error.opcode,
                                    p_data->addl_info.cp_error.ase_id, p_data->addl_info.cp_error.response_code, p_data->addl_info.cp_error.reason);
                    BT_API_PRINT(BT_API_ERROR,"AUDIO_GROUP_MSG_BAP_STATE: cp error, conn_handle 0x%x, opcode 0x%x, ase_id %d, response_code 0x%x, reason 0x%x\r\n",
                                    p_data->addl_info.cp_error.conn_handle, p_data->addl_info.cp_error.opcode,
                                    p_data->addl_info.cp_error.ase_id, p_data->addl_info.cp_error.response_code, p_data->addl_info.cp_error.reason);
                }
#if 1
                rtk_bt_le_audio_bap_state_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_BAP_STATE_IND,
                                            sizeof(rtk_bt_le_audio_bap_state_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_bap_state_ind_t *)p_evt->data;
                p_ind->cause = p_data->cause;
                p_ind->group_handle = handle;
                p_ind->stream_session_handle = p_data->handle;
                p_ind->curr_action = p_data->curr_action;
                p_ind->stream_state = p_data->state;
                p_ind->result = p_data->result;                
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
#endif                 
                switch (p_data->curr_action) {
                    case BAP_UNICAST_ACTION_START:
                        if(p_data->state == AUDIO_STREAM_STATE_STARTING)
                            msg_type = RTK_BT_LE_AUDIO_ACT_UNICAST_START;
                        break;
                    case BAP_UNICAST_ACTION_STOP:
                        if(p_data->state == AUDIO_STREAM_STATE_STOPPING)
                            msg_type = RTK_BT_LE_AUDIO_ACT_UNICAST_STOP;
                        break;
                    case BAP_UNICAST_ACTION_RELEASE:
                        if(p_data->state == AUDIO_STREAM_STATE_IDLE_CONFIGURED)
                            msg_type = RTK_BT_LE_AUDIO_ACT_UNICAST_RELEASE;
                        break;
                    case BAP_UNICAST_ACTION_UPDATE:
                        msg_type = RTK_BT_LE_AUDIO_ACT_UNICAST_UPDATE;
                        break;
                    case BAP_UNICAST_ACTION_REMOVE_CFG:
                        if(p_data->state == AUDIO_STREAM_STATE_IDLE)
                            msg_type = RTK_BT_LE_AUDIO_ACT_UNICAST_REMOVE_CONFIG;
                        break;
#if 0
                    case BAP_UNICAST_ACTION_REMOVE_SESSION:
                        msg_type = RTK_BT_LE_AUDIO_ACT_STREAM_SESSION_RELEASE;
                        break;
#endif
                    default:
                        break;
                }
                if(msg_type) {
                    p_cmd = bt_stack_pending_cmd_search(msg_type);
                    if (p_cmd) {
                        bt_stack_pending_cmd_delete(p_cmd);
                        p_cmd->ret = p_data->cause;
                        osif_sem_give(p_cmd->psem);
                    } else {
                        BT_API_PRINT(BT_API_ERROR,"[%s] AUDIO_GROUP_MSG_BAP_STATE: find no pending command \r\n", __func__);
                    } 
                }   
          
            }
            break;

        case AUDIO_GROUP_MSG_BAP_START_QOS_CFG: {
                T_AUDIO_GROUP_BAP_START_QOS_CFG *p_data = (T_AUDIO_GROUP_BAP_START_QOS_CFG *)buf;
                APP_PRINT_INFO8("AUDIO_GROUP_MSG_BAP_START_QOS_CFG: group handle 0x%x, session handle 0x%x, sink presentation delay(0x%x-0x%x),source presentation delay(0x%x-0x%x), sink latency 0x%x, source latency 0x%x",
                            handle, p_data->handle,
                            p_data->sink_presentation_delay_min, p_data->sink_presentation_delay_max,
                            p_data->source_presentation_delay_min, p_data->source_presentation_delay_max,
                            p_data->sink_transport_latency_max, p_data->source_transport_latency_max);
                APP_PRINT_INFO4("sink_preferred_presentation_delay(0x%x-0x%x), source_preferred_presentation_delay(0x%x-0x%x)",
                            p_data->sink_preferred_presentation_delay_min, p_data->sink_preferred_presentation_delay_max,
                            p_data->source_preferred_presentation_delay_min, p_data->source_preferred_presentation_delay_max);
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_BAP_START_QOS_CFG: group handle %p, session handle %p, sink presentation delay(0x%x-0x%x),source presentation delay(0x%x-0x%x), sink latency 0x%x, source latency 0x%x\r\n",
                            handle, p_data->handle,
                            (unsigned int)p_data->sink_presentation_delay_min, (unsigned int)p_data->sink_presentation_delay_max,
                            (unsigned int)p_data->source_presentation_delay_min, (unsigned int)p_data->source_presentation_delay_max,
                            (unsigned int)p_data->sink_transport_latency_max, (unsigned int)p_data->source_transport_latency_max);
                BT_API_PRINT(BT_API_DEBUG,"sink_preferred_presentation_delay(0x%x-0x%x), source_preferred_presentation_delay(0x%x-0x%x)\r\n",
                            (unsigned int)p_data->sink_preferred_presentation_delay_min, (unsigned int)p_data->sink_preferred_presentation_delay_max,
                            (unsigned int)p_data->source_preferred_presentation_delay_min, (unsigned int)p_data->source_preferred_presentation_delay_max);

                rtk_bt_le_audio_bap_start_qos_cfg_ind_t *p_ind = NULL;
                T_AUDIO_SESSION_QOS_CFG session_qos = {0};
                if (false == rtk_bt_check_evt_cb_direct_calling(RTK_BT_LE_GP_AUDIO, RTK_BT_LE_AUDIO_EVT_BAP_START_QOS_CFG_IND)) { 
                    BT_API_PRINT(BT_API_ERROR,"%s: RTK_BT_LE_AUDIO_EVT_BAP_START_QOS_CFG_IND is not direct calling!\r\n", __func__);
                    break;
                }

                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_BAP_START_QOS_CFG_IND,
                                            sizeof(rtk_bt_le_audio_bap_start_qos_cfg_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_bap_start_qos_cfg_ind_t *)p_evt->data;
                p_ind->group_handle = handle;
                p_ind->stream_session_handle = p_data->handle;
                p_ind->sink_preferred_presentation_delay_min = p_data->sink_preferred_presentation_delay_min;
                p_ind->sink_preferred_presentation_delay_max = p_data->sink_preferred_presentation_delay_max;
                p_ind->sink_presentation_delay_min = p_data->sink_presentation_delay_min;
                p_ind->sink_presentation_delay_max = p_data->sink_presentation_delay_max;
                p_ind->source_preferred_presentation_delay_min = p_data->source_preferred_presentation_delay_min;
                p_ind->source_preferred_presentation_delay_max = p_data->source_preferred_presentation_delay_max;
                p_ind->source_presentation_delay_min = p_data->source_presentation_delay_min;
                p_ind->source_presentation_delay_max = p_data->source_presentation_delay_max;
                p_ind->sink_transport_latency_max = p_data->sink_transport_latency_max;
                p_ind->source_transport_latency_max = p_data->source_transport_latency_max;
                p_ind->p_session_qos = (rtk_bt_le_audio_unicast_session_qos_t*)&session_qos; //make sure the definition of T_AUDIO_SESSION_QOS_CFG and rtk_bt_le_audio_unicast_session_qos_t is same 
                /* Send event */
                rtk_bt_evt_indicate(p_evt, &cb_ret);
                if (cb_ret == RTK_BT_EVT_CB_OK) {
                    BT_API_PRINT(BT_API_DEBUG,"%s: sca = %d,packing = %d,framing = %d,sdu_interval_m_s = %d,sdu_interval_s_m = %d,latency_m_s = %d,latency_s_m = %d,sink_presentation_delay = %d,source_presentation_delay = %d\r\n", __func__,
                                    session_qos.sca,session_qos.packing,session_qos.framing,(int)session_qos.sdu_interval_m_s,(int)session_qos.sdu_interval_s_m,
                                    session_qos.latency_m_s,session_qos.latency_s_m,(int)session_qos.sink_presentation_delay,(int)session_qos.source_presentation_delay); 
                    if(false == bap_unicast_audio_cfg_session_qos(p_data->handle,&session_qos))
                        BT_API_PRINT(BT_API_ERROR,"%s: bap_unicast_audio_cfg_session_qos fail!\r\n", __func__); 
                }
            }
            break;

        case AUDIO_GROUP_MSG_BAP_START_METADATA_CFG: {
                T_AUDIO_GROUP_BAP_START_METADATA_CFG *p_data = (T_AUDIO_GROUP_BAP_START_METADATA_CFG *)buf;
                APP_PRINT_INFO5("AUDIO_GROUP_MSG_BAP_START_METADATA_CFG: group handle 0x%x, session handle 0x%x, dev_handle 0x%x, ase_id %d, direction %d",
                                handle, p_data->handle, p_data->dev_handle, p_data->ase_id, p_data->direction);
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_BAP_START_METADATA_CFG: group handle %p, session handle %p, dev_handle %p, ase_id %d, direction %d\r\n",
                                handle, p_data->handle, p_data->dev_handle, p_data->ase_id, p_data->direction);
                uint8_t metadata_len = 0;
                uint8_t metadata[RTK_BT_LE_AUDIO_METADATA_MAX_LEN] = {0};
                rtk_bt_le_audio_bap_start_metadata_cfg_ind_t *p_ind = NULL;
                if (false == rtk_bt_check_evt_cb_direct_calling(RTK_BT_LE_GP_AUDIO, RTK_BT_LE_AUDIO_EVT_BAP_START_METADATA_CFG_IND)) { 
                    BT_API_PRINT(BT_API_ERROR,"%s: RTK_BT_LE_AUDIO_EVT_BAP_START_METADATA_CFG_IND is not direct calling!\r\n", __func__);
                    break;
                }

                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_BAP_START_METADATA_CFG_IND,
                                            sizeof(rtk_bt_le_audio_bap_start_metadata_cfg_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_bap_start_metadata_cfg_ind_t *)p_evt->data;
                p_ind->group_handle = handle;
                p_ind->stream_session_handle = p_data->handle;
                p_ind->device_handle = p_data->dev_handle;
                p_ind->ase_id = p_data->ase_id;
                p_ind->p_metadata_len = &metadata_len;
                p_ind->p_metadata = metadata;
                /* Send event */
                rtk_bt_evt_indicate(p_evt, &cb_ret);
                if (cb_ret == RTK_BT_EVT_CB_OK) {
                    if(metadata_len > RTK_BT_LE_AUDIO_METADATA_MAX_LEN) {
                        BT_API_PRINT(BT_API_ERROR,"%s: RTK_BT_LE_AUDIO_EVT_BAP_START_METADATA_CFG_IND fail, use default metadata!\r\n", __func__);                            
                        metadata_len = sizeof(default_metadata); 
                        memcpy(metadata, default_metadata,metadata_len);                                                      
                    } 
                    BT_API_DUMPBUF(BT_API_DEBUG,__func__,metadata,metadata_len);
                    if(false == bap_unicast_audio_cfg_ase_metadata(p_data->handle, p_data->dev_handle,p_data->ase_id, metadata_len, metadata))
                        BT_API_PRINT(BT_API_ERROR,"%s: bap_unicast_audio_cfg_ase_metadata fail!\r\n", __func__); 
                }
            }
            break;
        case AUDIO_GROUP_MSG_BAP_CREATE_CIS: {
                T_AUDIO_GROUP_BAP_CREATE_CIS *p_data = (T_AUDIO_GROUP_BAP_CREATE_CIS *)buf;
                APP_PRINT_INFO3("AUDIO_GROUP_MSG_BAP_CREATE_CIS: group handle 0x%x, session handle 0x%x, dev_num 0x%x",
                                handle, p_data->handle, p_data->dev_num);
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_BAP_CREATE_CIS: group handle 0x%p, session handle 0x%p, dev_num 0x%x\r\n",
                                handle, p_data->handle, p_data->dev_num);
            }
            break;
        case AUDIO_GROUP_MSG_BAP_CIS_DISCONN: {
                T_AUDIO_GROUP_BAP_CIS_DISCONN *p_data = (T_AUDIO_GROUP_BAP_CIS_DISCONN *)buf;
                APP_PRINT_INFO4("AUDIO_GROUP_MSG_BAP_CIS_DISCONN: group handle 0x%x, session handle 0x%x, conn_handle 0x%x, reason 0x%x",
                                handle, p_data->handle, p_data->conn_handle, p_data->cause);
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_BAP_CIS_DISCONN: group handle 0x%p, session handle 0x%p, conn_handle 0x%x, reason 0x%x\r\n",
                                handle, p_data->handle, p_data->conn_handle, p_data->cause);
            }
            break;
        case AUDIO_GROUP_MSG_BAP_SESSION_REMOVE: {
                T_AUDIO_GROUP_BAP_SESSION_REMOVE *p_data = (T_AUDIO_GROUP_BAP_SESSION_REMOVE *)buf;
                APP_PRINT_INFO2("AUDIO_GROUP_MSG_BAP_SESSION_REMOVE: group handle 0x%x, session handle 0x%x",
                                handle, p_data->handle);
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_BAP_SESSION_REMOVE: group handle %p, session handle %p\r\n",
                                handle, p_data->handle);
                p_cmd = bt_stack_pending_cmd_search(RTK_BT_LE_AUDIO_ACT_STREAM_SESSION_RELEASE);
                if (p_cmd) {
                    bt_stack_pending_cmd_delete(p_cmd);
                    p_cmd->ret = 0;
                    osif_sem_give(p_cmd->psem);
                } else {
                   BT_API_PRINT(BT_API_ERROR,"AUDIO_GROUP_MSG_BAP_SESSION_REMOVE: find no pending command \r\n");
                }                                
            }
            break;

        case AUDIO_GROUP_MSG_BAP_SETUP_DATA_PATH: {
                T_AUDIO_GROUP_MSG_BAP_SETUP_DATA_PATH *p_data = (T_AUDIO_GROUP_MSG_BAP_SETUP_DATA_PATH *)buf;
                APP_PRINT_INFO6("AUDIO_GROUP_MSG_BAP_SETUP_DATA_PATH: group handle 0x%x, session handle 0x%x, dev_handle 0x%x, ase_id 0x%x, direction %d, cis_conn_handle 0x%x",
                                handle, p_data->handle,
                                p_data->dev_handle,
                                p_data->ase_id,
                                p_data->path_direction,
                                p_data->cis_conn_handle);
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_BAP_SETUP_DATA_PATH: group handle %p, session handle %p, dev_handle %p, ase_id 0x%x, direction %d, cis_conn_handle 0x%x\r\n",
                                handle, p_data->handle,
                                p_data->dev_handle,
                                p_data->ase_id,
                                p_data->path_direction,
                                p_data->cis_conn_handle);
                rtk_bt_le_audio_bap_setup_data_path_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_BAP_SETUP_DATA_PATH_IND,
                                            sizeof(rtk_bt_le_audio_bap_setup_data_path_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_bap_setup_data_path_ind_t *)p_evt->data;
                p_ind->group_handle = handle;
                p_ind->stream_session_handle = p_data->handle;
                p_ind->device_handle = p_data->dev_handle;
                p_ind->ase_id = p_data->ase_id;
                p_ind->path_direction = p_data->path_direction;
                p_ind->cis_conn_handle = p_data->cis_conn_handle; 
                memcpy(&p_ind->codec_parsed_data, &p_data->codec_parsed_data, sizeof(rtk_bt_le_audio_cfg_codec_t));              
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;

        case AUDIO_GROUP_MSG_BAP_REMOVE_DATA_PATH: {
                T_AUDIO_GROUP_MSG_BAP_REMOVE_DATA_PATH *p_data = (T_AUDIO_GROUP_MSG_BAP_REMOVE_DATA_PATH *)buf;
                APP_PRINT_INFO7("AUDIO_GROUP_MSG_BAP_REMOVE_DATA_PATH: group handle 0x%x, session handle 0x%x, dev_handle 0x%x, ase_id 0x%x, direction %d, cis_conn_handle 0x%x, cause 0x%x",
                                handle, p_data->handle,
                                p_data->dev_handle,
                                p_data->ase_id,
                                p_data->path_direction,
                                p_data->cis_conn_handle,
                                p_data->cause);
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_BAP_REMOVE_DATA_PATH: group handle %p, session handle %p, dev_handle %p, ase_id 0x%x, direction %d, cis_conn_handle 0x%x, cause 0x%x\r\n",
                                handle, p_data->handle,
                                p_data->dev_handle,
                                p_data->ase_id,
                                p_data->path_direction,
                                p_data->cis_conn_handle,
                                p_data->cause);
                rtk_bt_le_audio_bap_remove_data_path_ind_t *p_ind = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_AUDIO, 
                                            RTK_BT_LE_AUDIO_EVT_BAP_REMOVE_DATA_PATH_IND,
                                            sizeof(rtk_bt_le_audio_bap_remove_data_path_ind_t));
                if (!p_evt) {
                    BT_API_PRINT(BT_API_ERROR,"%s rtk_bt_event_create fail\r\n",__func__);
                    break;
                } 
                p_ind = (rtk_bt_le_audio_bap_remove_data_path_ind_t *)p_evt->data;
                p_ind->group_handle = handle;
                p_ind->stream_session_handle = p_data->handle;
                p_ind->device_handle = p_data->dev_handle;
                p_ind->ase_id = p_data->ase_id;
                p_ind->path_direction = p_data->path_direction;
                p_ind->cis_conn_handle = p_data->cis_conn_handle; 
                p_ind->cause = p_data->cause;              
                /* Send event */
                rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;

        case AUDIO_GROUP_MSG_BAP_METADATA_UPDATE: {
                T_AUDIO_GROUP_MSG_BAP_METADATA_UPDATE *p_data = (T_AUDIO_GROUP_MSG_BAP_METADATA_UPDATE *)buf;
                APP_PRINT_INFO7("AUDIO_GROUP_MSG_BAP_METADATA_UPDATE: group handle 0x%x, session handle 0x%x, dev_handle 0x%x, ase_id 0x%x, direction %d, metadata_length %d, p_metadata %b",
                                handle, p_data->handle,
                                p_data->dev_handle,
                                p_data->ase_id,
                                p_data->direction,
                                p_data->metadata_length,
                                TRACE_BINARY(p_data->metadata_length, p_data->p_metadata));
                BT_API_PRINT(BT_API_DEBUG,"AUDIO_GROUP_MSG_BAP_METADATA_UPDATE: group handle %p, session handle %p, dev_handle %p, ase_id 0x%x,direction %d",
                                handle, p_data->handle,
                                p_data->dev_handle,
                                p_data->ase_id,
                                p_data->direction);
            }
            break;

        default:
            BT_API_PRINT(BT_API_ERROR,"%s: default msg 0x%04x, not process!\r\n",__func__, msg);
            break;
    }
}

static uint16_t bt_stack_le_audio_group_allocate(void *data)
{
    T_BLE_AUDIO_GROUP_HANDLE group_handle = NULL;
    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    group_handle = ble_audio_group_allocate();
    if (group_handle == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: ble_audio_group_allocate fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    BT_API_PRINT(BT_API_DEBUG,"%s ble_audio_group_allocate ok,group_handle=%p\r\n",__func__,group_handle); 

    if (false == ble_audio_group_reg_cb(group_handle,bt_stack_le_audio_group_cb)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: ble_audio_group_reg_cb fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    *(rtk_bt_le_audio_group_handle_t *)data = group_handle;

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_group_release(void *data)
{
    T_BLE_AUDIO_GROUP_HANDLE group_handle = NULL;
    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    } 
    group_handle = *(T_BLE_AUDIO_GROUP_HANDLE *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s: group_handle=%p\r\n",__func__,group_handle); 

    if (false == ble_audio_group_release(&group_handle)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: ble_audio_group_release fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_group_add_device(void *data)
{
    uint8_t conn_id = 0;
	rtk_bt_le_addr_type_t addr_type = 0;
	uint8_t addr_val[RTK_BD_ADDR_LEN] = {0};
    T_BLE_AUDIO_DEV_HANDLE device_handle = NULL;
    rtk_bt_le_audio_group_add_device_param_t *p_param = NULL; 

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }

    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    } 
    p_param = (rtk_bt_le_audio_group_add_device_param_t *)data; 
 
	if (false == le_get_conn_id_by_handle(p_param->conn_handle, &conn_id)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: le_get_conn_id_by_handle fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

	if (false == le_get_conn_addr(conn_id, addr_val, &addr_type)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: le_get_conn_addr fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 
    device_handle = ble_audio_group_add_dev(p_param->group_handle, addr_val, addr_type);
    if (device_handle == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: ble_audio_group_add_dev fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    *(rtk_bt_le_audio_device_handle_t *)p_param->p_device_handle = device_handle;

    BT_API_PRINT(BT_API_DEBUG,"%s: group_handle=%p,device_handle=%p\r\n",__func__,p_param->group_handle,device_handle); 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_group_delete_device(void *data)
{
    rtk_bt_le_audio_group_delete_device_param_t *p_param = NULL; 

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    } 
    p_param = (rtk_bt_le_audio_group_delete_device_param_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s: group_handle=%p,device_handle=%p\r\n",__func__,p_param->group_handle,p_param->device_handle); 

    if (false == ble_audio_group_del_dev(p_param->group_handle,&p_param->device_handle)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: ble_audio_group_del_dev fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_group_find_device(void *data)
{
    T_BLE_AUDIO_DEV_HANDLE device_handle = NULL;
    rtk_bt_le_audio_group_find_device_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_group_find_device_param_t *)data; 

    device_handle = ble_audio_group_find_dev(p_param->group_handle,p_param->dev_addr.addr_val,p_param->dev_addr.type);
    if (device_handle == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: ble_audio_group_find_dev fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    *(rtk_bt_le_audio_device_handle_t *)p_param->p_device_handle = device_handle;
    BT_API_PRINT(BT_API_DEBUG,"%s: group_handle=%p,device_handle=%p\r\n",__func__,p_param->group_handle,device_handle); 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_group_find_device_by_conn_handle(void *data)
{
    T_BLE_AUDIO_DEV_HANDLE device_handle = NULL;
    rtk_bt_le_audio_group_find_device_by_conn_handle_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_group_find_device_by_conn_handle_param_t *)data; 

    device_handle = ble_audio_group_find_dev_by_conn_handle(p_param->group_handle,p_param->conn_handle);
    if (device_handle == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: ble_audio_group_find_dev_by_conn_handle fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    *(rtk_bt_le_audio_device_handle_t *)p_param->p_device_handle = device_handle;
    BT_API_PRINT(BT_API_DEBUG,"%s: group_handle=%p,device_handle=%p\r\n",__func__,p_param->group_handle,device_handle); 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_group_get_device_num(void *data)
{
    uint8_t device_num = 0;
    rtk_bt_le_audio_group_get_device_num_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_group_get_device_num_param_t *)data; 

    device_num = ble_audio_group_get_dev_num(p_param->group_handle);

    *(uint8_t *)p_param->p_device_num = device_num;
    BT_API_PRINT(BT_API_DEBUG,"%s: group_handle=%p, device_num=%d\r\n",__func__,p_param->group_handle,device_num); 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_group_get_used_device_num(void *data)
{
    uint8_t used_device_num  = 0;
    rtk_bt_le_audio_group_get_used_device_num_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_group_get_used_device_num_param_t *)data; 

    used_device_num = ble_audio_group_get_used_dev_num(p_param->group_handle,p_param->check_conn);

    *(uint8_t *)p_param->p_used_device_num = used_device_num;
    BT_API_PRINT(BT_API_DEBUG,"%s: group_handle=%p, used_device_num=%d\r\n",__func__,p_param->group_handle,used_device_num); 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_group_get_device_info(void *data)
{
    T_AUDIO_DEV_INFO info = {0};
    rtk_bt_le_audio_group_get_device_info_param_t *p_param = NULL;
    rtk_bt_le_audio_group_device_info_t *p_device_info = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_group_get_device_info_param_t *)data; 
    p_device_info = p_param->p_device_info;

    if(false == ble_audio_group_get_dev_info(p_param->group_handle,p_param->device_handle,&info)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: ble_audio_group_get_dev_info fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    if(p_param->device_handle != info.dev_handle){
        BT_API_PRINT(BT_API_ERROR,"%s fail: set dev_handle(%p) != get dev_handle(%p)\r\n",__func__,p_param->device_handle,info.dev_handle);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    BT_API_PRINT(BT_API_DEBUG,"%s: info.is_used=%d,info.conn_state=%d,info.conn_handle=0x%x,info.dev_handle=%p\r\n",__func__,info.is_used,info.conn_state,info.conn_handle,info.dev_handle);

    p_device_info->is_used = info.is_used;
    p_device_info->conn_state = (uint8_t)info.conn_state;
    p_device_info->conn_handle = info.conn_handle;
    p_device_info->addr.type = (rtk_bt_le_addr_type_t)info.addr_type;
    memcpy((void*)p_device_info->addr.addr_val, (void*)info.bd_addr, RTK_BD_ADDR_LEN);
    p_device_info->device_handle = info.dev_handle;    

    return RTK_BT_OK;
}    

static uint16_t bt_stack_le_audio_group_get_all_device_info(void *data)
{
    T_AUDIO_DEV_INFO *p_dev_tbl = NULL;
    uint8_t dev_num = 0, i = 0;
    rtk_bt_le_audio_group_get_all_device_info_param_t *p_param = NULL;
    rtk_bt_le_audio_group_device_info_t *p_device_info_tbl = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_group_get_all_device_info_param_t *)data; 
    p_device_info_tbl = p_param->p_device_info_tbl;

    dev_num = ble_audio_group_get_dev_num(p_param->group_handle);
    if (dev_num) {
        p_dev_tbl = osif_mem_alloc(RAM_TYPE_DATA_ON, dev_num * sizeof(T_AUDIO_DEV_INFO));
        if (p_dev_tbl) {
            memset(p_dev_tbl, 0, dev_num * sizeof(T_AUDIO_DEV_INFO));
            if(false == ble_audio_group_get_info(p_param->group_handle,p_param->p_device_num,p_dev_tbl)) {
                BT_API_PRINT(BT_API_ERROR,"%s fail: ble_audio_group_get_info fail\r\n",__func__);
                return RTK_BT_ERR_LOWER_STACK_API;
            } 
            for( i = 0 ; i < dev_num; i++ ) {
                p_device_info_tbl[i].is_used = p_dev_tbl[i].is_used;
                p_device_info_tbl[i].conn_state = (uint8_t)p_dev_tbl[i].conn_state;
                p_device_info_tbl[i].conn_handle = p_dev_tbl[i].conn_handle;
                p_device_info_tbl[i].addr.type = (rtk_bt_le_addr_type_t)p_dev_tbl[i].addr_type;
                memcpy((void*)p_device_info_tbl[i].addr.addr_val, (void*)p_dev_tbl[i].bd_addr, RTK_BD_ADDR_LEN);
                p_device_info_tbl[i].device_handle = p_dev_tbl[i].dev_handle;                  
            }
            osif_mem_free(p_dev_tbl);
        }
    }   

    return RTK_BT_OK;
} 

#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
static uint16_t bt_stack_le_audio_stream_session_allocate(void *data)
{
    T_AUDIO_STREAM_SESSION_HANDLE stream_session_handle = NULL;
    rtk_bt_le_audio_stream_session_allocate_param_t * p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_stream_session_allocate_param_t *)data; 

    stream_session_handle = audio_stream_session_allocate(p_param->group_handle);
    if (stream_session_handle == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: audio_stream_session_allocate fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 
    
    BT_API_PRINT(BT_API_DEBUG,"%s audio_stream_session_allocate ok,stream_session_handle=%p\r\n",__func__,stream_session_handle); 

    *(rtk_bt_le_audio_stream_session_handle_t *)p_param->p_stream_session_handle = stream_session_handle;

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_stream_session_release(void *data)
{
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    stream_session_handle = *(rtk_bt_le_audio_stream_session_handle_t *)data; 

    if (false == bap_unicast_audio_remove_session(stream_session_handle)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bap_unicast_audio_remove_session fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_stream_session_get_group_handle(void *data)
{
    T_BLE_AUDIO_GROUP_HANDLE group_handle = NULL;
    rtk_bt_le_audio_stream_session_get_group_handle_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_stream_session_get_group_handle_param_t *)data; 

    group_handle = audio_stream_session_get_group(p_param->stream_session_handle);
    if(group_handle == NULL) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: audio_stream_session_get_group fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    *(rtk_bt_le_audio_group_handle_t *)p_param->p_group_handle = group_handle;

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_unicast_get_session_info(void *data)
{
    T_BAP_UNICAST_SESSION_INFO session_info = {0};
    rtk_bt_le_audio_unicast_get_session_info_param_t *p_param = NULL;
    uint8_t i = 0, j = 0;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_unicast_get_session_info_param_t *)data;

    BT_API_PRINT(BT_API_DEBUG,"%s: stream_session_handle = %p\r\n",__func__,p_param->stream_session_handle);
    if(false == bap_unicast_audio_get_session_info(p_param->stream_session_handle,&session_info)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bap_unicast_audio_get_session_info fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 
    BT_API_PRINT(BT_API_DEBUG,"%s: state = %d, conn_dev_num = %d, dev_num= %d\r\n",__func__,session_info.state,session_info.conn_dev_num,session_info.dev_num);
#if 0
    for(i = 0; i < session_info.dev_num; i++) {
        BT_API_PRINT(BT_API_DEBUG,"i=%d,dev_handle=%p,conn_state=%d,ase_num=%d\r\n",i,
                        session_info.dev_info[i].dev_handle,session_info.dev_info[i].conn_state,session_info.dev_info[i].ase_num);	
        for(j = 0; j < session_info.dev_info[i].ase_num; j++) {
            BT_API_PRINT(BT_API_DEBUG,"j=%d, ase_id=%d,cfg_idx=%d,ase_state=%d,direction=%d,channel_num=%d\r\n",j,
                        session_info.dev_info[i].ase_info[j].ase_id,session_info.dev_info[i].ase_info[j].cfg_idx,
                        session_info.dev_info[i].ase_info[j].ase_state,session_info.dev_info[i].ase_info[j].direction,session_info.dev_info[i].ase_info[j].channel_num);
        }		
    }
#endif
    p_param->p_session_info->state = (uint8_t)session_info.state;
    p_param->p_session_info->cfg_type = (rtk_bt_le_audio_unicast_cfg_type_t)session_info.cfg_type;
    p_param->p_session_info->conn_dev_num = session_info.conn_dev_num;
    p_param->p_session_info->dev_num = session_info.dev_num;
    if(session_info.dev_num > RTK_BT_LE_AUDIO_UNICAST_SERVERS_MAX_NUM) {
        BT_API_PRINT(BT_API_ERROR,"%s fail:session_info.dev_num(%d) > max (%d)\r\n",__func__,session_info.dev_num,RTK_BT_LE_AUDIO_UNICAST_SERVERS_MAX_NUM);
        return RTK_BT_ERR_NO_RESOURCE;
    } 
    for(i = 0; i < session_info.dev_num; i++) {        
        p_param->p_session_info->dev_info[i].dev_handle = session_info.dev_info[i].dev_handle;
        p_param->p_session_info->dev_info[i].conn_state = (rtk_bt_le_audio_gap_conn_state_t)session_info.dev_info[i].conn_state;
        p_param->p_session_info->dev_info[i].ase_num = session_info.dev_info[i].ase_num;
        if(session_info.dev_info[i].ase_num > RTK_BT_LE_AUDIO_STREAM_SRV_ASE_MAX) {
            BT_API_PRINT(BT_API_ERROR,"%s fail:session_info.dev_info[%d].ase_num (%d) > max (%d)\r\n",__func__,i,session_info.dev_info[i].ase_num,RTK_BT_LE_AUDIO_STREAM_SRV_ASE_MAX);
            return RTK_BT_ERR_NO_RESOURCE;
        } 
        for(j = 0; j < session_info.dev_info[i].ase_num; j++) {
            p_param->p_session_info->dev_info[i].ase_info[j].ase_id = session_info.dev_info[i].ase_info[j].ase_id;
            p_param->p_session_info->dev_info[i].ase_info[j].cfg_idx = session_info.dev_info[i].ase_info[j].cfg_idx;
            p_param->p_session_info->dev_info[i].ase_info[j].ase_state = (rtk_bt_le_audio_ascs_ase_state_t)session_info.dev_info[i].ase_info[j].ase_state;
            p_param->p_session_info->dev_info[i].ase_info[j].direction = session_info.dev_info[i].ase_info[j].direction;
            p_param->p_session_info->dev_info[i].ase_info[j].channel_num = session_info.dev_info[i].ase_info[j].channel_num;
        }
    }
#if 0
    for(i = 0; i < p_param->p_session_info->dev_num; i++) {
        BT_API_PRINT(BT_API_DEBUG,"i=%d,dev_handle=%p,conn_state=%d,ase_num=%d\r\n",i,
                        p_param->p_session_info->dev_info[i].dev_handle,p_param->p_session_info->dev_info[i].conn_state,p_param->p_session_info->dev_info[i].ase_num);	
        for(j = 0; j < p_param->p_session_info->dev_info[i].ase_num; j++) {
            BT_API_PRINT(BT_API_DEBUG,"j=%d, ase_id=%d,cfg_idx=%d,ase_state=%d,direction=%d,channel_num=%d\r\n",j,
                        p_param->p_session_info->dev_info[i].ase_info[j].ase_id,p_param->p_session_info->dev_info[i].ase_info[j].cfg_idx,
                        p_param->p_session_info->dev_info[i].ase_info[j].ase_state,p_param->p_session_info->dev_info[i].ase_info[j].direction,p_param->p_session_info->dev_info[i].ase_info[j].channel_num);
        }		
    }
#endif
    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_unicast_get_cfg_mask(void *data)
{
    uint32_t cfg_mask = 0;
    rtk_bt_le_audio_unicast_get_cfg_mask_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_unicast_get_cfg_mask_param_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s: stream_session_handle = %p,prefer_cfg = 0x%x\r\n",__func__,p_param->stream_session_handle,(unsigned int)p_param->prefer_cfg);
    for(uint8_t i = 0 ; i < p_param->device_num; i++) {
        BT_API_PRINT(BT_API_DEBUG,"%s: dev_handle[%d]=%p \r\n",__func__,i,p_param->p_device_handle_tbl[i]);
    }

    cfg_mask = bap_unicast_get_audio_cfg_mask(p_param->stream_session_handle,p_param->prefer_cfg,p_param->device_num,p_param->p_device_handle_tbl);

    *(uint32_t *)p_param->p_cfg_mask = cfg_mask;

    return RTK_BT_OK;
}    

static uint16_t bt_stack_le_audio_unicast_get_cis_info(void *data)
{
    T_AUDIO_STREAM_CIS_INFO stream_cis_info = {0};
    rtk_bt_le_audio_unicast_get_cis_info_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_unicast_get_cis_info_param_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s: stream_session_handle = %p,device_handle = %p\r\n",__func__,p_param->stream_session_handle,p_param->device_handle);
    if(false == bap_unicast_audio_get_cis_info(p_param->stream_session_handle,p_param->device_handle,&stream_cis_info)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bap_unicast_audio_get_cis_info fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    if(stream_cis_info.cis_num > RTK_BT_LE_AUDIO_STREAM_SRV_CIS_MAX) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: cis_num(%d) > RTK_BT_LE_AUDIO_STREAM_SRV_CIS_MAX (%d)\r\n",__func__,stream_cis_info.cis_num,RTK_BT_LE_AUDIO_STREAM_SRV_CIS_MAX);
        return RTK_BT_ERR_NO_RESOURCE;
    } 

    p_param->p_cis_infos->cis_num = stream_cis_info.cis_num;
    for(uint8_t i = 0; i < stream_cis_info.cis_num; i++) {
        p_param->p_cis_infos->cis_info[i].cis_id = stream_cis_info.cis_info[i].cis_id;
        p_param->p_cis_infos->cis_info[i].cis_conn_handle = stream_cis_info.cis_info[i].cis_conn_handle;
        p_param->p_cis_infos->cis_info[i].data_path_flags = stream_cis_info.cis_info[i].data_path_flags;
        p_param->p_cis_infos->cis_info[i].sink_ase_id = stream_cis_info.cis_info[i].sink_ase_id;
        p_param->p_cis_infos->cis_info[i].sink_ase_state = (rtk_bt_le_audio_ascs_ase_state_t)stream_cis_info.cis_info[i].sink_ase_state;
        p_param->p_cis_infos->cis_info[i].source_ase_id = stream_cis_info.cis_info[i].source_ase_id;
        p_param->p_cis_infos->cis_info[i].source_ase_state = (rtk_bt_le_audio_ascs_ase_state_t)stream_cis_info.cis_info[i].source_ase_state;
    }

    return RTK_BT_OK;
}    

static uint16_t bt_stack_le_audio_unicast_config(void *data)
{
    rtk_bt_le_audio_unicast_config_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_unicast_config_param_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s: stream_session_handle=%p,cfg_type=%d,device_num=%d\r\n",__func__,p_param->stream_session_handle,p_param->cfg_type,p_param->device_num);
    
    for(uint8_t i = 0 ; i < p_param->device_num; i++) {
        BT_API_PRINT(BT_API_DEBUG,"%s: dev_handle[%d]=%p \r\n",__func__,i,p_param->p_device_handle_tbl[i]);
    }

    if(false == bap_unicast_audio_cfg(p_param->stream_session_handle,(T_UNICAST_AUDIO_CFG_TYPE)p_param->cfg_type,p_param->device_num,p_param->p_device_handle_tbl)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bap_unicast_audio_cfg fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}    

static uint16_t bt_stack_le_audio_unicast_cfg_ase_codec(void *data)
{
    T_AUDIO_ASE_CODEC_CFG cfg = {0};
    rtk_bt_le_audio_unicast_cfg_ase_codec_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_unicast_cfg_ase_codec_param_t *)data; 

    memcpy(cfg.codec_id, p_param->ase_codec.codec_id, RTK_BT_LE_AUDIO_CODEC_ID_LEN);
    cfg.target_latency = (T_ASE_TARGET_LATENCY)p_param->ase_codec.target_latency;
    cfg.target_phy = (T_ASE_TARGET_PHY)p_param->ase_codec.target_phy;    
    memcpy(&cfg.codec_cfg, &p_param->ase_codec.codec_cfg, sizeof(T_CODEC_CFG));

    BT_API_PRINT(BT_API_DEBUG,"%s: stream_session_handle=%p,device_handle=%p,cfg_idx=%d\r\n",__func__,p_param->stream_session_handle,p_param->device_handle,p_param->cfg_idx);
    
    if(false == bap_unicast_audio_cfg_ase_codec(p_param->stream_session_handle,p_param->device_handle,p_param->cfg_idx,&cfg)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bap_unicast_audio_cfg_ase_codec fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}                                       

static uint16_t bt_stack_le_audio_unicast_remove_config(void *data)
{
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    stream_session_handle = *(rtk_bt_le_audio_stream_session_handle_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s: stream_session_handle=%p\r\n",__func__,stream_session_handle);    

    if(false == bap_unicast_audio_remove_cfg(stream_session_handle)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bap_unicast_audio_remove_cfg fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}  

static uint16_t bt_stack_le_audio_unicast_get_session_qos(void *data)
{
    T_AUDIO_SESSION_QOS_CFG cfg = {0};
    rtk_bt_le_audio_unicast_get_session_qos_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_unicast_get_session_qos_param_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s: stream_session_handle=%p\r\n",__func__,p_param->stream_session_handle);
    
    if(false == bap_unicast_audio_get_session_qos(p_param->stream_session_handle,&cfg)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bap_unicast_audio_get_session_qos fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 
	else {
        BT_API_PRINT(BT_API_DEBUG,"bap_unicast_audio_get_session_qos ok, sca 0x%x, packing 0x%x, framing 0x%x, sdu_interval_m_s 0x%x, sdu_interval_s_m 0x%x, latency_m_s 0x%x, latency_s_m 0x%x \r\n",
							cfg.sca, cfg.packing, cfg.framing,
							(unsigned int)cfg.sdu_interval_m_s, (unsigned int)cfg.sdu_interval_s_m,
							cfg.latency_m_s, cfg.latency_s_m);	
    }
    p_param->p_session_qos->sca = cfg.sca;
    p_param->p_session_qos->packing = cfg.packing;
    p_param->p_session_qos->framing = cfg.framing;
    p_param->p_session_qos->sdu_interval_m_s = cfg.sdu_interval_m_s;
    p_param->p_session_qos->sdu_interval_s_m = cfg.sdu_interval_s_m;
    p_param->p_session_qos->latency_m_s = cfg.latency_m_s;
    p_param->p_session_qos->latency_s_m = cfg.latency_s_m;
    p_param->p_session_qos->sink_presentation_delay = cfg.sink_presentation_delay;
    p_param->p_session_qos->source_presentation_delay = cfg.source_presentation_delay;

    return RTK_BT_OK;
} 

static uint16_t bt_stack_le_audio_unicast_get_ase_qos(void *data)
{
    T_AUDIO_ASE_QOS_CFG cfg = {0};
    rtk_bt_le_audio_unicast_get_ase_qos_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_unicast_get_ase_qos_param_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s: stream_session_handle=%p,device_handle=%p,ase_id=%d\r\n",__func__,p_param->stream_session_handle,p_param->device_handle,p_param->ase_id);
    
    if(false == bap_unicast_audio_get_ase_qos(p_param->stream_session_handle,p_param->device_handle,p_param->ase_id,&cfg)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bap_unicast_audio_get_ase_qos fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    p_param->p_ase_qos->phy = cfg.phy;
    p_param->p_ase_qos->retransmission_number = cfg.retransmission_number;
    p_param->p_ase_qos->max_sdu = cfg.max_sdu;

    return RTK_BT_OK;
} 
static uint16_t bt_stack_le_audio_unicast_cfg_ase_qos(void *data)
{
    T_AUDIO_ASE_QOS_CFG cfg = {0};
    rtk_bt_le_audio_unicast_cfg_ase_qos_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    p_param = (rtk_bt_le_audio_unicast_cfg_ase_qos_param_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s: stream_session_handle=%p,device_handle=%p,ase_id=%d\r\n",__func__,p_param->stream_session_handle,p_param->device_handle,p_param->ase_id);

    cfg.phy = p_param->ase_qos.phy;
    cfg.retransmission_number = p_param->ase_qos.retransmission_number;
    cfg.max_sdu = p_param->ase_qos.max_sdu;

    if(false == bap_unicast_audio_cfg_ase_qos(p_param->stream_session_handle,p_param->device_handle,p_param->ase_id,&cfg)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bap_unicast_audio_cfg_ase_qos fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
} 
static uint16_t bt_stack_le_audio_unicast_start(void *data)
{
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    stream_session_handle = *(rtk_bt_le_audio_stream_session_handle_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s: stream_session_handle=%p\r\n",__func__,stream_session_handle);    

    if(false == bap_unicast_audio_start(stream_session_handle)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bap_unicast_audio_start fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}  
static uint16_t bt_stack_le_audio_unicast_stop(void *data)
{
    rtk_bt_le_audio_unicast_stop_param_t *param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    param = (rtk_bt_le_audio_unicast_stop_param_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s: stream_session_handle=%p cis_timeout_ms=%d\r\n",__func__,param->stream_session_handle,(int)param->cis_timeout_ms);    

    if(false == bap_unicast_audio_stop(param->stream_session_handle,param->cis_timeout_ms)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bap_unicast_audio_stop fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}  
static uint16_t bt_stack_le_audio_unicast_release(void *data)
{
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    stream_session_handle = *(rtk_bt_le_audio_stream_session_handle_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s: stream_session_handle=%p\r\n",__func__,stream_session_handle);    

    if(false == bap_unicast_audio_release(stream_session_handle)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bap_unicast_audio_release fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}  
static uint16_t bt_stack_le_audio_unicast_update(void *data)
{
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  
    stream_session_handle = *(rtk_bt_le_audio_stream_session_handle_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s: stream_session_handle=%p\r\n",__func__,stream_session_handle);    

    if(false == bap_unicast_audio_update(stream_session_handle)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bap_unicast_audio_update fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}  
#endif

#if defined(RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT
static uint16_t bt_stack_le_audio_csis_gen_rsi(void *data)
{
    rtk_bt_le_audio_csis_gen_rsi_info_t *p_csis_gen_rsi_info = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_csis_gen_rsi_info = (rtk_bt_le_audio_csis_gen_rsi_info_t *)data;

    if(csis_gen_rsi(p_csis_gen_rsi_info->sirk,p_csis_gen_rsi_info->p_rsi) == false) {
        BT_API_PRINT(BT_API_ERROR,"%s csis_gen_rsi fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }
    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_csis_update_sirk(void *data)
{
    uint8_t service_id = 0;
    T_ATTR_UUID srv_uuid = {0};
    rtk_bt_le_audio_csis_update_sirk_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_csis_update_sirk_param_t *)data;

    if(0 == memcmp(default_csis_sirk,p_param->sirk,RTK_BT_LE_CSIS_SIRK_LEN) && 
            default_csis_sirk_type == p_param->sirk_type) {
        BT_API_PRINT(BT_API_WARNING,"%s csis_sirk not changed, skip\r\n",__func__);
        return RTK_BT_OK;
    }

    BT_API_PRINT(BT_API_DEBUG,"%s:p_param->sirk_type:%d\r\n",__func__,p_param->sirk_type);
    BT_API_DUMPBUF(BT_API_DEBUG,__func__,p_param->sirk,RTK_BT_LE_CSIS_SIRK_LEN); 

    srv_uuid.is_uuid16 = true;
    srv_uuid.p.uuid16 = GATT_UUID_CAS;
    srv_uuid.instance_id = 0;
    if(false == csis_get_srv_id(&service_id, &srv_uuid)) {
        BT_API_PRINT(BT_API_ERROR,"%s csis_get_srv_id fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }
    BT_API_PRINT(BT_API_DEBUG,"%s:service_id = %d\r\n",__func__,service_id);

    if(false == csis_update_sirk(service_id, p_param->sirk,(T_CSIS_SIRK_TYPE)p_param->sirk_type)) {
        BT_API_PRINT(BT_API_ERROR,"%s csis_update_sirk fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    default_csis_sirk_type = p_param->sirk_type;  
    memcpy(default_csis_sirk,p_param->sirk,RTK_BT_LE_CSIS_SIRK_LEN);    

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_csis_update_size(void *data)
{
    uint8_t service_id = 0;
    T_ATTR_UUID srv_uuid = {0};
    rtk_bt_le_audio_csis_update_size_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_param = (rtk_bt_le_audio_csis_update_size_param_t *)data;

    if(default_csis_size == p_param->csis_size) {
        BT_API_PRINT(BT_API_WARNING,"%s csis_size not changed, skip\r\n",__func__);
        return RTK_BT_OK;
    }

    BT_API_PRINT(BT_API_DEBUG,"%s:csis_size:%d\r\n",__func__,p_param->csis_size);

    srv_uuid.is_uuid16 = true;
    srv_uuid.p.uuid16 = GATT_UUID_CAS;
    srv_uuid.instance_id = 0;
    if(false == csis_get_srv_id(&service_id, &srv_uuid)) {
        BT_API_PRINT(BT_API_ERROR,"%s csis_get_srv_id fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }
    BT_API_PRINT(BT_API_DEBUG,"%s:service_id = %d\r\n",__func__,service_id);

    if(false == csis_update_size(service_id, p_param->csis_size)) {
        BT_API_PRINT(BT_API_ERROR,"%s csis_update_size fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    default_csis_size = p_param->csis_size;  

    return RTK_BT_OK;
}
#endif 


#if defined(RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT
static uint16_t bt_stack_le_audio_csis_set_coordinator_check_adv_rsi(void *data)
{
    rtk_bt_le_audio_csis_set_coordinator_check_adv_rsi_info_t *p_info = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_info = (rtk_bt_le_audio_csis_set_coordinator_check_adv_rsi_info_t *)data;

    if(set_coordinator_check_adv_rsi(p_info->report_data_len,p_info->p_report_data, p_info->adv_addr.addr_val, p_info->adv_addr.type) == false) {
        BT_API_PRINT(BT_API_ERROR,"%s set_coordinator_check_adv_rsi fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_csis_set_coordinator_cfg_discover(void *data)
{
    rtk_bt_le_audio_csis_set_coordinator_cfg_discover_info_t *p_info = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_info = (rtk_bt_le_audio_csis_set_coordinator_cfg_discover_info_t *)data;

    if(set_coordinator_cfg_discover(p_info->group_handle,p_info->discover, p_info->timeout_ms) == false) {
        BT_API_PRINT(BT_API_ERROR,"%s set_coordinator_cfg_discover fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}
static uint16_t bt_stack_le_audio_csis_set_coordinator_add_group(void *data)
{
    rtk_bt_le_audio_csis_set_coordinator_add_group_info_t *p_info = NULL;
    T_CSIS_SET_MEM_INFO mem_info = {0};

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_info = (rtk_bt_le_audio_csis_set_coordinator_add_group_info_t *)data;

    memcpy(mem_info.bd_addr, p_info->mem_info.bd_addr.addr_val, RTK_BD_ADDR_LEN);
    mem_info.addr_type = p_info->mem_info.bd_addr.type;
    mem_info.srv_uuid = p_info->mem_info.srv_uuid;
    mem_info.srv_instance_id = p_info->mem_info.srv_instance_id;
    mem_info.char_exit = p_info->mem_info.char_exit;
    mem_info.rank = p_info->mem_info.rank;
    mem_info.lock = (T_CSIS_LOCK)p_info->mem_info.lock;
    mem_info.set_mem_size = p_info->mem_info.set_mem_size;
    mem_info.sirk_type = (T_CSIS_SIRK_TYPE)p_info->mem_info.sirk_type;
    memcpy(mem_info.sirk, p_info->mem_info.sirk, CSIS_SIRK_LEN);

    if(set_coordinator_add_group(p_info->p_group_handle,bt_stack_le_audio_group_cb,p_info->p_device_handle, &mem_info) == false) {
        BT_API_PRINT(BT_API_ERROR,"%s set_coordinator_check_add_group fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_csis_set_coordinator_add_device(void *data)
{
    rtk_bt_le_audio_csis_set_coordinator_add_device_info_t *p_info = NULL;
    T_CSIS_SET_MEM_INFO mem_info = {0};

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_info = (rtk_bt_le_audio_csis_set_coordinator_add_device_info_t *)data;

    memcpy(mem_info.bd_addr, p_info->mem_info.bd_addr.addr_val, RTK_BD_ADDR_LEN);
    mem_info.addr_type = p_info->mem_info.bd_addr.type;
    mem_info.srv_uuid = p_info->mem_info.srv_uuid;
    mem_info.srv_instance_id = p_info->mem_info.srv_instance_id;
    mem_info.char_exit = p_info->mem_info.char_exit;
    mem_info.rank = p_info->mem_info.rank;
    mem_info.lock = (T_CSIS_LOCK)p_info->mem_info.lock;
    mem_info.set_mem_size = p_info->mem_info.set_mem_size;
    mem_info.sirk_type = (T_CSIS_SIRK_TYPE)p_info->mem_info.sirk_type;
    memcpy(mem_info.sirk, p_info->mem_info.sirk, CSIS_SIRK_LEN);

    if(set_coordinator_add_dev(p_info->group_handle,p_info->p_device_handle, &mem_info) == false) {
        BT_API_PRINT(BT_API_ERROR,"%s set_coordinator_add_dev fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_csis_set_coordinator_write_lock(void *data)
{
    rtk_bt_le_audio_csis_set_coordinator_write_lock_info_t *p_info = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_info = (rtk_bt_le_audio_csis_set_coordinator_write_lock_info_t *)data;

    if(p_info->lock == true) {
        if(set_coordinator_write_lock(p_info->group_handle) == false) {
            BT_API_PRINT(BT_API_ERROR,"%s set_coordinator_write_lock fail\r\n",__func__);
            return RTK_BT_ERR_LOWER_STACK_API;
        }
    } else {
        if(set_coordinator_write_unlock(p_info->group_handle) == false) {
            BT_API_PRINT(BT_API_ERROR,"%s set_coordinator_write_unlock fail\r\n",__func__);
            return RTK_BT_ERR_LOWER_STACK_API;
        }
    }

    return RTK_BT_OK;
}
#endif

#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
static uint16_t bt_stack_le_audio_mcp_client_write_media_cp(void *data)
{
    T_MCP_CLIENT_WRITE_MEDIA_CP_PARAM mcp_param = {0};
    rtk_bt_le_audio_mcp_client_write_media_cp_info_t *p_info = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_info = (rtk_bt_le_audio_mcp_client_write_media_cp_info_t *)data;
    mcp_param.opcode = p_info->opcode;
    memcpy(&mcp_param.param,&p_info->op_param,sizeof(p_info->op_param));

    if(mcp_client_write_media_cp(p_info->conn_handle,p_info->srv_instance_id,p_info->general_mcs,&mcp_param,p_info->is_req) == false) {
        BT_API_PRINT(BT_API_ERROR,"%s mcp_client_write_media_cp fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_mcp_client_read_char_value(void *data)
{
    rtk_bt_le_audio_mcp_client_read_char_value_info_t *p_info = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_info = (rtk_bt_le_audio_mcp_client_read_char_value_info_t *)data;

    BT_API_PRINT(BT_API_DEBUG,"%s conn_handle=0x%x,srv_instance_id=%d,char_uuid=0x%x,general_mcs=%d\r\n",__func__,p_info->conn_handle,p_info->srv_instance_id,p_info->char_uuid,p_info->general_mcs);

    if(mcp_client_read_char_value(p_info->conn_handle,p_info->srv_instance_id,p_info->char_uuid,p_info->general_mcs) == false) {
        BT_API_PRINT(BT_API_ERROR,"%s mcp_client_read_char_value fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_mcp_client_cfg_cccd(void *data)
{
    rtk_bt_le_audio_mcp_client_cfg_cccd_info_t *p_info = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_info = (rtk_bt_le_audio_mcp_client_cfg_cccd_info_t *)data;

    BT_API_PRINT(BT_API_DEBUG,"%s conn_handle=0x%x,cfg_flags=0x%x,general_mcs=%d,srv_instance_id=%d,enable=%d\r\n",__func__,p_info->conn_handle,(unsigned int)p_info->cfg_flags,p_info->general_mcs, p_info->srv_instance_id, p_info->enable);

    mcp_client_cfg_cccd(p_info->conn_handle,p_info->cfg_flags,p_info->general_mcs, p_info->srv_instance_id, p_info->enable);

    return RTK_BT_OK;
}
#endif 

#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
static uint16_t bt_stack_le_audio_mcp_server_set_param(void *data)
{
    T_MCP_SERVER_SET_PARAM set_param = {0};
    rtk_bt_le_audio_mcp_server_set_param_t * p_info = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_info = (rtk_bt_le_audio_mcp_server_set_param_t *)data;

    set_param.char_uuid = p_info->char_uuid; 
    memcpy(&set_param.param,&p_info->param, sizeof(p_info->param)); 

    BT_API_PRINT(BT_API_DEBUG,"%s service_id %d, char_uuid=0x%x\r\n",__func__,p_info->service_id, p_info->char_uuid);

    if(false == mcp_server_set_param(p_info->service_id,&set_param)) {
        BT_API_PRINT(BT_API_ERROR,"%s mcp_server_set_param fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_mcp_server_send_data(void *data)
{
    T_MCP_SERVER_SEND_DATA_PARAM send_param = {0};
    rtk_bt_le_audio_mcp_server_send_data_param_t * p_info = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_info = (rtk_bt_le_audio_mcp_server_send_data_param_t *)data;

    send_param.char_uuid = p_info->char_uuid;

    if(p_info->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME) {
        send_param.param.media_player_name.p_media_player_name = p_info->param.media_player_name.p_media_player_name;
        send_param.param.media_player_name.media_player_name_len = p_info->param.media_player_name.media_player_name_len;
    } else if(p_info->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE) {
        send_param.param.track_title.p_track_title = p_info->param.track_title.p_track_title;
        send_param.param.track_title.track_title_len = p_info->param.track_title.track_title_len;
    } else 
        memcpy(&send_param.param,&p_info->param, sizeof(p_info->param));

    BT_API_PRINT(BT_API_DEBUG,"%s service_id %d, char_uuid=0x%x\r\n",__func__,p_info->service_id, p_info->char_uuid);

    if(false == mcp_server_send_data(p_info->service_id,&send_param)) {
        BT_API_PRINT(BT_API_ERROR,"%s mcp_server_send_data fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_mcp_server_read_cfm(void *data)
{
    T_MCP_SERVER_READ_CFM read_cfm = {0};
    rtk_bt_le_audio_mcp_server_read_confirm_param_t * p_info = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }   

    p_info = (rtk_bt_le_audio_mcp_server_read_confirm_param_t *)data;

    read_cfm.cause = BLE_AUDIO_CB_RESULT_SUCCESS;
    read_cfm.conn_handle = p_info->conn_handle;
    read_cfm.cid = p_info->cid;
    read_cfm.service_id = p_info->service_id;
    read_cfm.char_uuid = p_info->char_uuid;
    read_cfm.offset = p_info->offset;
    if(p_info->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME) {
        read_cfm.param.media_player_name.p_media_player_name = p_info->param.media_player_name.p_media_player_name;
        read_cfm.param.media_player_name.media_player_name_len = p_info->param.media_player_name.media_player_name_len;
    } else if(p_info->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE) {
        read_cfm.param.track_title.p_track_title = p_info->param.track_title.p_track_title;
        read_cfm.param.track_title.track_title_len = p_info->param.track_title.track_title_len;
    } else if(p_info->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_ICON_URL) {
        read_cfm.param.media_player_icon_url.p_media_player_icon_url = p_info->param.media_player_icon_url.p_media_player_icon_url;
        read_cfm.param.media_player_icon_url.media_player_icon_url_len = p_info->param.media_player_icon_url.media_player_icon_url_len;
    } else 
        memcpy(&read_cfm.param,&p_info->param, sizeof(p_info->param));  

    BT_API_PRINT(BT_API_DEBUG,"%s service_id %d, char_uuid=0x%x\r\n",__func__,p_info->service_id, p_info->char_uuid);
    switch (p_info->char_uuid) {
        case RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME:  
            BT_API_PRINT(BT_API_DEBUG,"media_player_name_len %d, p_media_player_name %s\r\n",read_cfm.param.media_player_name.media_player_name_len, read_cfm.param.media_player_name.p_media_player_name);
            break;
        case RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE: 
            BT_API_PRINT(BT_API_DEBUG,"track_title_len %d, p_track_title %s\r\n",read_cfm.param.track_title.track_title_len,read_cfm.param.track_title.p_track_title);
            break;
        case RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_DURATION:
            BT_API_PRINT(BT_API_DEBUG,"read track duration %d\r\n",(int)read_cfm.param.track_duration);
            break;
        case RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_POSITION:
            BT_API_PRINT(BT_API_DEBUG,"read track position %d \r\n",(int)read_cfm.param.track_position); 
            break;
        case RTK_BT_LE_AUDIO_MCS_UUID_CHAR_CONTENT_CONTROL_ID: 
            BT_API_PRINT(BT_API_DEBUG,"read content control id %d\r\n",read_cfm.param.content_control_id);
            break;
        default:
            BT_API_PRINT(BT_API_WARNING, "%s unprocess char_uuid 0x%x for MCS server\r\n",__func__,p_info->char_uuid);
            return RTK_BT_OK;
    }

    if(false == mcp_server_read_cfm(&read_cfm)) {
        BT_API_PRINT(BT_API_ERROR,"%s mcp_server_read_cfm fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    }

    return RTK_BT_OK;
}
#endif

#if defined(RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT
static uint16_t bt_stack_le_audio_vcs_param_set(void *data)
{
    T_VCS_PARAM vcs_param = {0};
    rtk_bt_le_audio_vcs_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vcs_param_t *)data;

    vcs_param.volume_setting = p_param->volume_setting;
    vcs_param.mute = p_param->mute;
    vcs_param.change_counter = p_param->change_counter;
    vcs_param.volume_flags = p_param->volume_flags;
    vcs_param.step_size = p_param->step_size;

    if (false == vcs_set_param(&vcs_param)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: vcs_set_param fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_vcs_param_get(void *data)
{
    T_VCS_PARAM vcs_param = {0};
    rtk_bt_le_audio_vcs_param_t *p_param = NULL; 

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vcs_param_t *)data;

    if (false == vcs_get_param(&vcs_param)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: vcs_get_param fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    p_param->volume_setting = vcs_param.volume_setting;
    p_param->mute = vcs_param.mute;
    p_param->change_counter = vcs_param.change_counter;
    p_param->volume_flags = vcs_param.volume_flags;
    p_param->step_size = vcs_param.step_size;

    return RTK_BT_OK;
}
#endif

#if defined(RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT
static uint16_t bt_stack_le_audio_vcs_write_cp(void *data)
{
    T_VCS_VOLUME_CP_PARAM cp_param = {0};
    rtk_bt_le_audio_vcs_write_cp_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vcs_write_cp_param_t *)data;

    if(VCS_CP_SET_ABSOLUTE_VOLUME == (T_VCS_CP_OP)p_param->cp_op)
        cp_param.volume_setting = p_param->cp_param.volume_setting;    

    BT_API_PRINT(BT_API_DEBUG,"%s conn_handle=%d,cp_op=%d \r\n",__func__,p_param->conn_handle,p_param->cp_op);

    if (false == vcs_write_cp(p_param->conn_handle, (T_VCS_CP_OP)p_param->cp_op, &cp_param)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: vcs_write_cp fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_vcs_get_volume_state(void *data)
{
    T_VOLUME_STATE volume_state = {0};
    rtk_bt_le_audio_vcs_volume_state_t *p_vcs_volume_state = NULL; 
    rtk_bt_le_audio_vcs_get_volume_state_param_t *p_param = NULL; 

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vcs_get_volume_state_param_t *)data; 
    p_vcs_volume_state = p_param->p_vcs_volume_state; 

    if (false == vcs_get_volume_state(p_param->conn_handle, &volume_state)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: vcs_get_volume_state fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    p_vcs_volume_state->volume_setting = volume_state.volume_setting;
    p_vcs_volume_state->mute = volume_state.mute;
    p_vcs_volume_state->change_counter = volume_state.change_counter;    

    BT_API_PRINT(BT_API_DEBUG,"%s volume_setting=%d,mute=%d,muchange_countere=%d \r\n",__func__,p_vcs_volume_state->volume_setting,p_vcs_volume_state->mute,p_vcs_volume_state->change_counter);

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_vcs_change_mute(void *data)
{
    T_LE_AUDIO_CAUSE cause = LE_AUDIO_CAUSE_SUCCESS;
    rtk_bt_le_audio_vcs_change_mute_param_t *p_param = NULL; 

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vcs_change_mute_param_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s mute %d\r\n",__func__,p_param->mute);

    cause = cap_vcs_change_mute(p_param->group_handle, p_param->mute);
    if (LE_AUDIO_CAUSE_SUCCESS != cause) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: cap_vcs_change_mute cause = 0x%x\r\n",__func__,cause);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_vcs_change_volume(void *data)
{
    T_LE_AUDIO_CAUSE cause = LE_AUDIO_CAUSE_SUCCESS;
    rtk_bt_le_audio_vcs_change_volume_param_t *p_param = NULL; 

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vcs_change_volume_param_t *)data; 
    
    cause = cap_vcs_change_volume(p_param->group_handle, p_param->volume_setting);
    if (LE_AUDIO_CAUSE_SUCCESS != cause) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: cap_vcs_change_volume cause = 0x%x\r\n",__func__,cause);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}
#endif

#if defined(RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT
static uint16_t bt_stack_le_audio_mics_change_mute(void *data)
{
    T_LE_AUDIO_CAUSE cause = LE_AUDIO_CAUSE_SUCCESS;
    rtk_bt_le_audio_mics_change_mute_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_mics_change_mute_param_t *)data; 
    
    cause = cap_mics_change_mute(p_param->group_handle, (T_MICS_MUTE)p_param->mic_mute);
    if (LE_AUDIO_CAUSE_SUCCESS != cause) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: cap_mics_change_mute cause = 0x%x\r\n",__func__,cause);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_mics_set_mute_value(void *data)
{
    rtk_bt_le_audio_mics_set_mute_value_param_t *p_param = NULL;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_mics_set_mute_value_param_t *)data; 

    BT_API_PRINT(BT_API_ERROR,"%s conn_handle=%d mic_mute=%d\r\n",__func__,p_param->conn_handle,p_param->mic_mute);

    if (false == mics_write_mute_value(p_param->conn_handle,(T_MICS_MUTE)p_param->mic_mute)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: mics_write_mute_value fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_mics_get_mute_value(void *data)
{
    rtk_bt_le_audio_mics_get_mute_value_param_t *p_param = NULL;
    T_MICS_MUTE mic_mute = MICS_RFU;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_mics_get_mute_value_param_t *)data; 

    if (false == mics_get_mute_value(p_param->conn_handle,&mic_mute)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: mics_get_mute_value fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 
    BT_API_PRINT(BT_API_ERROR,"%s conn_handle=%d mic_mute=%d\r\n",__func__,p_param->conn_handle,mic_mute);
    
    *p_param->p_mic_mute = (rtk_bt_le_audio_mics_mute_state_t)mic_mute; 

    return RTK_BT_OK;
}
#endif

#if defined(RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT
static uint16_t bt_stack_le_audio_mics_set_param(void *data)
{
    T_MICS_PARAM mics_param = {0};
    rtk_bt_le_audio_mics_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_mics_param_t *)data; 

    mics_param.mic_mute = (T_MICS_MUTE)p_param->mic_mute;

    if (false == mics_set_param(&mics_param)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: mics_set_param fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_mics_get_param(void *data)
{
    T_MICS_PARAM mics_param = {0};
    rtk_bt_le_audio_mics_param_t *p_param = NULL; 

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_mics_param_t *)data; 

    if (false == mics_get_param(&mics_param)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: mics_get_param fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    p_param->mic_mute = (rtk_bt_le_audio_mics_mute_state_t)mics_param.mic_mute;

    return RTK_BT_OK;
}
#endif

#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
static uint16_t bt_stack_le_audio_vocs_set_param(void *data)
{
    T_VOCS_PARAM_SET vocs_param = {0};
    rtk_bt_le_audio_vocs_set_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vocs_set_param_t *)data; 

    vocs_param.set_mask = p_param->set_mask;
    vocs_param.volume_offset = p_param->vocs_param.volume_offset;
    vocs_param.change_counter = p_param->vocs_param.change_counter;
    vocs_param.audio_location = p_param->vocs_param.audio_location;
    vocs_param.output_des.output_des_len = p_param->vocs_param.output_des.output_des_len;
    vocs_param.output_des.p_output_des = p_param->vocs_param.output_des.p_output_des;

    if (false == vocs_set_param(p_param->srv_instance_id, &vocs_param)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: vocs_set_param fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_vocs_get_param(void *data)
{
    T_VOCS_PARAM_GET vocs_param = {0};
    rtk_bt_le_audio_vocs_get_param_t *p_param = NULL; 

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vocs_get_param_t *)data; 

    if (false == vocs_get_param(p_param->srv_instance_id, &vocs_param)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: vocs_get_param fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    p_param->p_vocs_param->volume_offset = vocs_param.volume_offset;
    p_param->p_vocs_param->change_counter = vocs_param.change_counter;
    p_param->p_vocs_param->audio_location = vocs_param.audio_location;
    p_param->p_vocs_param->output_des.output_des_len = vocs_param.output_des.output_des_len;

    //if p_output_des is NULL, APP want to query output descripiton length only
    if(p_param->p_vocs_param->output_des.p_output_des != NULL)
        memcpy(p_param->p_vocs_param->output_des.p_output_des,vocs_param.output_des.p_output_des,vocs_param.output_des.output_des_len);

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_vocs_cfg_cccd(void *data)
{
    rtk_bt_le_audio_vocs_cfg_cccd_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vocs_cfg_cccd_param_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s conn_handle %d,srv_instance_id %d,cfg_flags 0x%x, enable %d\r\n",__func__,p_param->conn_handle, p_param->srv_instance_id, p_param->cfg_flags, p_param->enable);
    vocs_cfg_cccd(p_param->conn_handle, p_param->srv_instance_id, p_param->cfg_flags, p_param->enable);

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_vocs_read_char_value(void *data)
{
    rtk_bt_le_audio_vocs_read_char_value_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vocs_read_char_value_param_t *)data; 

    if(false == vocs_read_char_value(p_param->conn_handle, p_param->srv_instance_id, (T_VOCS_CHAR_TYPE)p_param->type)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: vocs_read_char_value fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_vocs_write_cp(void *data)
{
    T_VOCS_CP_PARAM cp_param = {0};
    rtk_bt_le_audio_vocs_write_cp_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vocs_write_cp_param_t *)data; 

    cp_param.volume_offset = p_param->cp_param.volume_offset;

	BT_API_PRINT(BT_API_DEBUG,"%s conn_handle %d, srv_instance_id %d,cp_op 0x%x, volume_offset %d\r\n",__func__,
                p_param->conn_handle, p_param->srv_instance_id, p_param->cp_op, cp_param.volume_offset );	

    if(false == vocs_write_cp(p_param->conn_handle, p_param->srv_instance_id, (T_VOCS_CP_OP)p_param->cp_op,&cp_param)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: vocs_write_cp fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_vocs_write_cp_by_group(void *data)
{
    T_VOCS_CP_PARAM cp_param = {0};
    T_LE_AUDIO_CAUSE ret = LE_AUDIO_CAUSE_SUCCESS;
    rtk_bt_le_audio_vocs_write_cp_by_group_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vocs_write_cp_by_group_param_t *)data; 

    cp_param.volume_offset = p_param->cp_param.volume_offset;

	BT_API_PRINT(BT_API_DEBUG,"%s group_handle %p, srv_instance_id %d,cp_op 0x%x,volume_offset %d\r\n",__func__,
                p_param->group_handle, p_param->srv_instance_id, p_param->cp_op, cp_param.volume_offset );

    ret = vocs_write_cp_by_group(p_param->group_handle, p_param->srv_instance_id, (T_VOCS_CP_OP)p_param->cp_op,&cp_param);
    if(LE_AUDIO_CAUSE_SUCCESS != ret) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: vocs_write_cp_by_group ret = 0x%x\r\n",__func__,ret);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_vocs_write_audio_location(void *data)
{
    T_LE_AUDIO_CAUSE ret = LE_AUDIO_CAUSE_SUCCESS;
    rtk_bt_le_audio_vocs_write_audio_location_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vocs_write_audio_location_param_t *)data; 

	BT_API_PRINT(BT_API_DEBUG,"%s conn_handle %d, srv_instance_id %d,audio_location 0x%x\r\n",__func__, p_param->conn_handle, p_param->srv_instance_id, (unsigned int)p_param->audio_location);

    ret = vocs_write_audio_location(p_param->conn_handle, p_param->srv_instance_id, p_param->audio_location);
    if(LE_AUDIO_CAUSE_SUCCESS != ret) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: vocs_write_audio_location ret = 0x%x\r\n",__func__,ret);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_vocs_write_output_des(void *data)
{
    T_LE_AUDIO_CAUSE ret = LE_AUDIO_CAUSE_SUCCESS;
    rtk_bt_le_audio_vocs_write_output_des_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vocs_write_output_des_param_t *)data; 

	BT_API_PRINT(BT_API_DEBUG,"%s conn_handle %d, srv_instance_id %d,output_des_len %d,p_output_des %s\r\n",__func__,
                p_param->conn_handle, p_param->srv_instance_id, p_param->output_des_len, p_param->p_output_des);

    if(false == vocs_write_output_des(p_param->conn_handle, p_param->srv_instance_id, p_param->p_output_des, p_param->output_des_len)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: vocs_write_output_des ret = 0x%x\r\n",__func__,ret);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_vocs_get_srv_data(void *data)
{
    T_VOCS_SRV_DATA value = {0};
    rtk_bt_le_audio_vocs_get_srv_data_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_vocs_get_srv_data_param_t *)data; 
	
    if(false == vocs_get_srv_data(p_param->conn_handle, p_param->srv_instance_id, &value)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: vocs_get_srv_data fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    p_param->p_srv_data->srv_instance_id = value.srv_instance_id;
    p_param->p_srv_data->type_exist = value.type_exist;
    p_param->p_srv_data->volume_offset.volume_offset = value.volume_offset.volume_offset;
    p_param->p_srv_data->volume_offset.change_counter = value.volume_offset.change_counter;
    p_param->p_srv_data->audio_location = value.audio_location;
    p_param->p_srv_data->output_des.output_des_len = value.output_des.output_des_len;
    BT_API_PRINT(BT_API_DEBUG,"%s vocs_get_srv_data output_des_len %d,p_output_des %s\r\n",__func__,value.output_des.output_des_len,value.output_des.p_output_des);
    //if p_output_des is NULL, APP want to query output descripiton length only
    if(p_param->p_srv_data->output_des.output_des_len)
        memcpy(p_param->p_srv_data->output_des.p_output_des, value.output_des.p_output_des, sizeof(value.output_des.output_des_len));
	
    return RTK_BT_OK;
}
#endif

#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
static uint16_t bt_stack_le_audio_aics_set_param(void *data)
{
    rtk_bt_le_audio_aics_set_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_aics_set_param_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s srv_instance_id %d,aics_param_type 0x%x\r\n",__func__,p_param->srv_instance_id, p_param->aics_param_type);

    if (false == aics_set_param(p_param->srv_instance_id, (T_AICS_PARAM_TYPE)p_param->aics_param_type,p_param->value_len,p_param->p_value,p_param->set_change_counter)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: aics_set_param fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_aics_get_param(void *data)
{
    rtk_bt_le_audio_aics_get_param_t *p_param = NULL; 

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_aics_get_param_t *)data; 

    if(!p_param->p_value) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: returned param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    } 

    if (false == aics_get_param(p_param->srv_instance_id,(T_AICS_PARAM_TYPE)p_param->aics_param_type,p_param->p_value)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: aics_get_param fail,srv_instance_id=%d,aics_param_type=%d\r\n",__func__,p_param->srv_instance_id,p_param->aics_param_type);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_aics_cfg_cccd(void *data)
{
    rtk_bt_le_audio_aics_cfg_cccd_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_aics_cfg_cccd_param_t *)data; 

    BT_API_PRINT(BT_API_DEBUG,"%s conn_handle %d,srv_instance_id %d,cfg_flags 0x%x, enable %d\r\n",__func__,p_param->conn_handle, p_param->srv_instance_id, p_param->cfg_flags, p_param->enable);
    aics_cfg_cccd(p_param->conn_handle, p_param->srv_instance_id, p_param->cfg_flags, p_param->enable);

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_aics_read_char_value(void *data)
{
    rtk_bt_le_audio_aics_read_char_value_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_aics_read_char_value_param_t *)data; 

    if(false == aics_read_char_value(p_param->conn_handle, p_param->srv_instance_id, (T_AICS_CHAR_TYPE)p_param->type)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: aics_read_char_value fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_aics_write_cp(void *data)
{
    T_AICS_CP_PARAM cp_param = {0};
    rtk_bt_le_audio_aics_write_cp_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_aics_write_cp_param_t *)data; 	

    cp_param.gaining_setting = p_param->cp_param.gaining_setting;

	BT_API_PRINT(BT_API_DEBUG,"%s conn_handle %d, srv_instance_id %d,cp_op 0x%x, gaining_setting %d\r\n",__func__,
                p_param->conn_handle, p_param->srv_instance_id, p_param->cp_op, cp_param.gaining_setting );

    if(false == aics_write_cp(p_param->conn_handle, p_param->srv_instance_id, (T_AICS_CP_OP)p_param->cp_op, &cp_param)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: aics_write_cp fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_aics_write_cp_by_group(void *data)
{
    T_AICS_CP_PARAM cp_param = {0};
    T_LE_AUDIO_CAUSE ret = LE_AUDIO_CAUSE_SUCCESS;
    rtk_bt_le_audio_aics_write_cp_by_group_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_aics_write_cp_by_group_param_t *)data; 

    cp_param.gaining_setting = p_param->cp_param.gaining_setting;

	BT_API_PRINT(BT_API_DEBUG,"%s group_handle %p, srv_instance_id %d,cp_op 0x%x,gaining_setting %d\r\n",__func__,
                p_param->group_handle, p_param->srv_instance_id, p_param->cp_op,cp_param.gaining_setting);

    ret = aics_write_cp_by_group(p_param->group_handle, p_param->srv_instance_id, (T_AICS_CP_OP)p_param->cp_op,&cp_param);
    if(LE_AUDIO_CAUSE_SUCCESS != ret) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: aics_write_cp_by_group ret = 0x%x\r\n",__func__,ret);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_aics_write_input_des(void *data)
{
    T_LE_AUDIO_CAUSE ret = LE_AUDIO_CAUSE_SUCCESS;
    rtk_bt_le_audio_aics_write_input_des_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_aics_write_input_des_param_t *)data; 

	BT_API_PRINT(BT_API_DEBUG,"%s conn_handle %d, srv_instance_id %d,input_des_len %d,p_input_des %s\r\n",__func__,
                p_param->conn_handle, p_param->srv_instance_id, p_param->input_des_len, p_param->p_input_des);

    if(false == aics_write_input_des(p_param->conn_handle, p_param->srv_instance_id, p_param->p_input_des, p_param->input_des_len)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: aics_write_input_des ret = 0x%x\r\n",__func__,ret);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}

static uint16_t bt_stack_le_audio_aics_get_srv_data(void *data)
{
    T_AICS_SRV_DATA value = {0};
    rtk_bt_le_audio_aics_get_srv_data_param_t *p_param = NULL;  

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }  

    p_param = (rtk_bt_le_audio_aics_get_srv_data_param_t *)data; 
	
    if(false == aics_get_srv_data(p_param->conn_handle, p_param->srv_instance_id, &value)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: aics_get_srv_data fail\r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    p_param->p_srv_data->srv_instance_id = value.srv_instance_id;
    p_param->p_srv_data->type_exist = value.type_exist;
    p_param->p_srv_data->input_state.gain_setting = value.input_state.gain_setting;
    p_param->p_srv_data->input_state.mute = value.input_state.mute;
    p_param->p_srv_data->input_state.gain_mode = value.input_state.gain_mode;
    p_param->p_srv_data->input_state.change_counter = value.input_state.change_counter;
    p_param->p_srv_data->setting_prop.gain_setting_units = value.setting_prop.gain_setting_units;
    p_param->p_srv_data->setting_prop.gain_setting_min = value.setting_prop.gain_setting_min;
    p_param->p_srv_data->setting_prop.gain_setting_max = value.setting_prop.gain_setting_max;
    p_param->p_srv_data->input_type = value.input_type;
    p_param->p_srv_data->input_status = value.input_status;
    p_param->p_srv_data->input_des.input_des_len = value.input_des.input_des_len;

    //if p_input_des is NULL, APP want to query input descripiton length only
    if(p_param->p_srv_data->input_des.input_des_len)
        memcpy(p_param->p_srv_data->input_des.p_input_des, value.input_des.p_input_des, sizeof(value.input_des.input_des_len));
	
    return RTK_BT_OK;
}

#endif

#if defined(RTK_BLE_AUDIO_TMAP_SUPPORT) && RTK_BLE_AUDIO_TMAP_SUPPORT
static uint16_t bt_stack_le_audio_tmas_client_read_role(void *data)
{
    uint16_t p_conn_handle = 0;

    if(!rtk_bt_is_enable()) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: bt is not enable\r\n",__func__);
        return RTK_BT_ERR_NOT_READY;
    }
    if(!data) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: param error\r\n",__func__);
        return RTK_BT_ERR_PARAM_INVALID;
    }

    p_conn_handle = *((uint16_t *)data); 

    BT_API_PRINT(BT_API_DEBUG,"%s conn_handle %d\r\n",__func__, p_conn_handle);

    if(false == tmas_read_role(p_conn_handle)) {
        BT_API_PRINT(BT_API_ERROR,"%s fail: tmas_read_role failed \r\n",__func__);
        return RTK_BT_ERR_LOWER_STACK_API;
    } 

    return RTK_BT_OK;
}
#endif

uint16_t bt_stack_le_audio_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = 0;
    BT_API_PRINT(BT_API_DUMP,"bt_stack_le_audio_act_handle: act = %d \r\n", p_cmd->act);
    switch(p_cmd->act) {
        case RTK_BT_LE_AUDIO_ACT_ISO_DATA_SEND: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_DATA_SEND \r\n");
            ret = bt_stack_le_audio_data_send(p_cmd->param);
            break;       
        }  
        case RTK_BT_LE_AUDIO_ACT_GET_PREFER_CODEC_CFG: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_GET_PREFER_CODEC_CFG \r\n");
            ret = bt_stack_le_audio_get_prefer_codec_cfg(p_cmd->param);
            break;       
        }   
        case RTK_BT_LE_AUDIO_ACT_GET_PREFER_QOS_CFG: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_GET_PREFER_QOS_CFG \r\n");
            ret = bt_stack_le_audio_get_prefer_qos_cfg(p_cmd->param);
            break;
        }   
        case RTK_BT_LE_AUDIO_ACT_PACS_GET_INFO: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_PACS_GET_INFO \r\n");
            ret = bt_stack_le_audio_pacs_get_info(p_cmd->param);
            break;       
        }  
        case RTK_BT_LE_AUDIO_ACT_PACS_GET_PAC_RECORD: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_PACS_GET_PAC_RECORD \r\n");            
            ret = bt_stack_le_audio_pacs_get_pac_record(p_cmd->param);
            break;       
        } 
        case RTK_BT_LE_AUDIO_ACT_PACS_GET_LC3_TABLE_MASK: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_PACS_GET_LC3_TABLE_MASK \r\n");            
            ret = bt_stack_le_audio_pacs_get_lc3_table_mask(p_cmd->param);
            break;       
        } 
        case RTK_BT_LE_AUDIO_ACT_PACS_GET_BIS_ARRAY_BY_SYNC_INFO: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_PACS_GET_BIS_ARRAY_BY_SYNC_INFO \r\n");            
            ret = bt_stack_le_audio_pacs_get_bis_array_by_sync_info(p_cmd->param);
            break;       
        } 
#if (defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) 
        case RTK_BT_LE_AUDIO_ACT_SYNC_ALLOCATE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_SYNC_ALLOCATE \r\n");
            ret = bt_stack_le_audio_sync_allocate(p_cmd->param);
            break;               
        }  
        case RTK_BT_LE_AUDIO_ACT_SYNC_RELEASE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_SYNC_RELEASE \r\n");
            ret = bt_stack_le_audio_sync_release(p_cmd->param);
            break;               
        } 
        case RTK_BT_LE_AUDIO_ACT_SYNC_FIND: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_SYNC_FIND \r\n");
            ret = bt_stack_le_audio_sync_find(p_cmd->param);
            break;               
        }           
        case RTK_BT_LE_AUDIO_ACT_PA_SYNC_ESTABLISH: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_PA_SYNC_ESTABLISH \r\n");
            ret = bt_stack_le_audio_pa_sync_establish(p_cmd->param);
            break;               
        }  
        case RTK_BT_LE_AUDIO_ACT_PA_SYNC_TERMINATE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_PA_SYNC_TERMINATE \r\n");
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_PA_SYNC_TERMINATE;
            bt_stack_pending_cmd_insert(p_cmd);
            ret = bt_stack_le_audio_pa_sync_terminate(p_cmd->param);
            goto async_handle;              
        } 
        case RTK_BT_LE_AUDIO_ACT_SYNC_GET_SYNC_INFO: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_SYNC_GET_SYNC_INFO \r\n");
            ret = bt_stack_le_audio_sync_get_sync_info(p_cmd->param);
            break;               
        } 
        case RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_CODEC_CFG: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_CODEC_CFG \r\n");
            ret = bt_stack_le_audio_sync_get_bis_codec_cfg(p_cmd->param);
            break;               
        } 
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_BIG_SYNC_ESTABLISH: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BIG_SYNC_ESTABLISH \r\n");
            ret = bt_stack_le_audio_big_sync_establish(p_cmd->param);
            break;               
        }  
        case RTK_BT_LE_AUDIO_ACT_BIG_SYNC_TERMINATE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BIG_SYNC_TERMINATE \r\n");
            ret = bt_stack_le_audio_big_sync_terminate(p_cmd->param);
            break;               
        } 
        case RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_INFO: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_INFO \r\n");
            ret = bt_stack_le_audio_sync_get_bis_info(p_cmd->param);
            break;               
        }
        case RTK_BT_LE_AUDIO_ACT_SYNC_GET_SUPPORT_BIS_ARRAY: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_SYNC_GET_SUPPORT_BIS_ARRAY \r\n");
            ret = bt_stack_le_audio_sync_get_support_bis_array(p_cmd->param);
            break;               
        }          
        case RTK_BT_LE_AUDIO_ACT_SYNC_SETUP_DATA_PATH: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_SYNC_SETUP_DATA_PATH \r\n");
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_SYNC_SETUP_DATA_PATH;
            bt_stack_pending_cmd_insert(p_cmd);
            ret = bt_stack_le_audio_sync_setup_data_path(p_cmd->param);
            goto async_handle;
        } 
        case RTK_BT_LE_AUDIO_ACT_SYNC_REMOVE_DATA_PATH: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_SYNC_REMOVE_DATA_PATH \r\n");
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_SYNC_REMOVE_DATA_PATH;
            bt_stack_pending_cmd_insert(p_cmd);
            ret = bt_stack_le_audio_sync_remove_data_path(p_cmd->param);
            goto async_handle;              
        }  
#endif  //#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CREATE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CREATE \r\n");
            ret = bt_stack_le_audio_broadcast_source_create(p_cmd->param);
            break;              
        }   

        case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_GET_INFO: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_GET_INFO \r\n");
            ret = bt_stack_le_audio_broadcast_source_get_info(p_cmd->param);
            break;              
        }   

        case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CONFIG: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CONFIG \r\n");
            ret = bt_stack_le_audio_broadcast_source_config(p_cmd->param);
            break;              
        } 

        case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_ENABLE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_ENABLE \r\n");
            ret = bt_stack_le_audio_broadcast_source_enable(p_cmd->param);
            break;              
        } 

        case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_DISABLE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_DISABLE \r\n");
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_DISABLE;
            bt_stack_pending_cmd_insert(p_cmd);
            ret = bt_stack_le_audio_broadcast_source_disable(p_cmd->param);
            goto async_handle;               
        } 

        case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RELEASE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RELEASE \r\n");
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RELEASE;
            bt_stack_pending_cmd_insert(p_cmd);
            ret = bt_stack_le_audio_broadcast_source_release(p_cmd->param);
            goto async_handle;              
        } 

        case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_SETUP_DATA_PATH: {
            BT_API_PRINT(BT_API_DEBUG,"RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_SETUP_DATA_PATH \r\n");            
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_SETUP_DATA_PATH;
            bt_stack_pending_cmd_insert(p_cmd);
            ret = bt_stack_le_audio_broadcast_source_setup_data_path(p_cmd->param);
            goto async_handle;             
        } 

        case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_REMOVE_DATA_PATH: {
            BT_API_PRINT(BT_API_DEBUG,"RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_REMOVE_DATA_PATH \r\n");
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_REMOVE_DATA_PATH;
            bt_stack_pending_cmd_insert(p_cmd);
            ret = bt_stack_le_audio_broadcast_source_remove_data_path(p_cmd->param);
            goto async_handle;              
        } 

        case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RECONFIG: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RECONFIG \r\n");            
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RECONFIG;
            bt_stack_pending_cmd_insert(p_cmd);
            ret = bt_stack_le_audio_broadcast_source_reconfig(p_cmd->param);
            goto async_handle;             
        } 
#endif  
#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_GET_BRS_CHAR_DATA: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_GET_BRS_CHAR_DATA \r\n");            
            ret = bt_stack_le_audio_bass_client_get_brs_data(p_cmd->param);
            break;             
        } 
        case RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_CP_ADD_SOURCE_BY_SYNC_INFO: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_CP_ADD_SOURCE_BY_SYNC_INFO \r\n");            
            ret = bt_stack_le_audio_bass_client_cp_add_source_by_sync_info(p_cmd->param);
            break;             
        } 
        case RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_CP_MODIFY_SOURCE_BY_SYNC_INFO: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_CP_MODIFY_SOURCE_BY_SYNC_INFO \r\n");            
            ret = bt_stack_le_audio_bass_client_cp_modify_source_by_sync_info(p_cmd->param);
            break;             
        } 
        case RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_WRITE_CP_OP: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_WRITE_CP_OP \r\n");    
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_WRITE_CP_OP;
            bt_stack_pending_cmd_insert(p_cmd);        //NEED REVISE
            ret = bt_stack_le_audio_bass_client_write_cp_op(p_cmd->param);
            goto async_handle;              
        }
        case RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_PAST: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_PAST \r\n");            
            ret = bt_stack_le_audio_bass_client_past(p_cmd->param);
            break;             
        }        
#endif
        case RTK_BT_LE_AUDIO_ACT_GROUP_ALLOCATE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_GROUP_ALLOCATE \r\n");            
            ret = bt_stack_le_audio_group_allocate(p_cmd->param);
            break;           
        } 
        case RTK_BT_LE_AUDIO_ACT_GROUP_RELEASE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_GROUP_RELEASE \r\n");            
            ret = bt_stack_le_audio_group_release(p_cmd->param);
            break;           
        } 
        case RTK_BT_LE_AUDIO_ACT_GROUP_ADD_DEVICE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_GROUP_ADD_DEVICE \r\n");            
            ret = bt_stack_le_audio_group_add_device(p_cmd->param);
            break;           
        } 
        case RTK_BT_LE_AUDIO_ACT_GROUP_DELETE_DEVICE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_GROUP_DELETE_DEVICE \r\n");            
            ret = bt_stack_le_audio_group_delete_device(p_cmd->param);
            break;           
        }  
        case RTK_BT_LE_AUDIO_ACT_GROUP_FIND_DEVICE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_GROUP_FIND_DEVICE \r\n");            
            ret = bt_stack_le_audio_group_find_device(p_cmd->param);
            break;           
        } 
        case RTK_BT_LE_AUDIO_ACT_GROUP_FIND_DEVICE_BY_CONN_HANDLE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_GROUP_FIND_DEVICE_BY_CONN_HANDLE \r\n");            
            ret = bt_stack_le_audio_group_find_device_by_conn_handle(p_cmd->param);
            break;           
        }  
        case RTK_BT_LE_AUDIO_ACT_GROUP_GET_DEVICE_NUM: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_GROUP_GET_DEVICE_NUM \r\n");            
            ret = bt_stack_le_audio_group_get_device_num(p_cmd->param);
            break;           
        }
        case RTK_BT_LE_AUDIO_ACT_GROUP_GET_USED_DEVICE_NUM: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_GROUP_GET_USED_DEVICE_NUM \r\n");            
            ret = bt_stack_le_audio_group_get_used_device_num(p_cmd->param);
            break;           
        }
        case RTK_BT_LE_AUDIO_ACT_GROUP_GET_DEVICE_INFO: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_GROUP_GET_DEVICE_INFO \r\n");            
            ret = bt_stack_le_audio_group_get_device_info(p_cmd->param);
            break;           
        }
        case RTK_BT_LE_AUDIO_ACT_GROUP_GET_ALL_DEVICE_INFO: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_GROUP_GET_ALL_DEVICE_INFO \r\n");            
            ret = bt_stack_le_audio_group_get_all_device_info(p_cmd->param);
            break;           
        }        
#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_STREAM_SESSION_ALLOCATE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_STREAM_SESSION_ALLOCATE \r\n");            
            ret = bt_stack_le_audio_stream_session_allocate(p_cmd->param);
            break;           
        }
        case RTK_BT_LE_AUDIO_ACT_STREAM_SESSION_RELEASE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_STREAM_SESSION_RELEASE \r\n");            
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_STREAM_SESSION_RELEASE;
            bt_stack_pending_cmd_insert(p_cmd);            
            ret = bt_stack_le_audio_stream_session_release(p_cmd->param);
            goto async_handle;           
        }     
        case RTK_BT_LE_AUDIO_ACT_STREAM_SESSION_GET_GROUP_HANDLE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_STREAM_SESSION_GET_GROUP_HANDLE \r\n");            
            ret = bt_stack_le_audio_stream_session_get_group_handle(p_cmd->param);
            break;           
        }    
        case RTK_BT_LE_AUDIO_ACT_UNICAST_GET_SESSION_INFO: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_UNICAST_GET_SESSION_INFO \r\n");            
            ret = bt_stack_le_audio_unicast_get_session_info(p_cmd->param);
            break;  
        } 
        case RTK_BT_LE_AUDIO_ACT_UNICAST_GET_CFG_MASK: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_UNICAST_GET_CFG_MASK \r\n");            
            ret = bt_stack_le_audio_unicast_get_cfg_mask(p_cmd->param);
            break;           
        }          
        case RTK_BT_LE_AUDIO_ACT_UNICAST_GET_CIS_INFO: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_UNICAST_GET_CIS_INFO \r\n");            
            ret = bt_stack_le_audio_unicast_get_cis_info(p_cmd->param);
            break;           
        } 
        case RTK_BT_LE_AUDIO_ACT_UNICAST_CONFIG: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_UNICAST_CONFIG \r\n");            
            ret = bt_stack_le_audio_unicast_config(p_cmd->param);
            break;           
        } 
        case RTK_BT_LE_AUDIO_ACT_UNICAST_CFG_ASE_CODEC: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_UNICAST_CFG_ASE_CODEC \r\n");            
            ret = bt_stack_le_audio_unicast_cfg_ase_codec(p_cmd->param);
            break;           
        }  
        case RTK_BT_LE_AUDIO_ACT_UNICAST_REMOVE_CONFIG: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_UNICAST_REMOVE_CONFIG \r\n");            
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_UNICAST_REMOVE_CONFIG;
            bt_stack_pending_cmd_insert(p_cmd);            
            ret = bt_stack_le_audio_unicast_remove_config(p_cmd->param);
            goto async_handle;         
        } 
        case RTK_BT_LE_AUDIO_ACT_UNICAST_GET_SESSION_QOS: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_UNICAST_GET_SESSION_QOS \r\n");  
            ret = bt_stack_le_audio_unicast_get_session_qos(p_cmd->param);
            break;           
        } 
        case RTK_BT_LE_AUDIO_ACT_UNICAST_CFG_ASE_QOS: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_UNICAST_CFG_ASE_QOS \r\n");            
            ret = bt_stack_le_audio_unicast_cfg_ase_qos(p_cmd->param);
            break;           
        } 
        case RTK_BT_LE_AUDIO_ACT_UNICAST_GET_ASE_QOS: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_UNICAST_GET_ASE_QOS \r\n");            
            ret = bt_stack_le_audio_unicast_get_ase_qos(p_cmd->param);
            break;           
        }               
        case RTK_BT_LE_AUDIO_ACT_UNICAST_START: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_UNICAST_START \r\n");  
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_UNICAST_START; 
            bt_stack_pending_cmd_insert(p_cmd);            
            ret = bt_stack_le_audio_unicast_start(p_cmd->param);
            goto async_handle;          
      
        }  
        case RTK_BT_LE_AUDIO_ACT_UNICAST_STOP: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_UNICAST_STOP \r\n");   
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_UNICAST_STOP;
            bt_stack_pending_cmd_insert(p_cmd);         
            ret = bt_stack_le_audio_unicast_stop(p_cmd->param);
            goto async_handle;    
        } 
        case RTK_BT_LE_AUDIO_ACT_UNICAST_RELEASE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_UNICAST_RELEASE \r\n");  
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_UNICAST_RELEASE;
            bt_stack_pending_cmd_insert(p_cmd);            
            ret = bt_stack_le_audio_unicast_release(p_cmd->param);
            goto async_handle;            
        } 
        case RTK_BT_LE_AUDIO_ACT_UNICAST_UPDATE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_UNICAST_UPDATE \r\n"); 
            p_cmd->user_data = RTK_BT_LE_AUDIO_ACT_UNICAST_UPDATE;
            bt_stack_pending_cmd_insert(p_cmd);            
            ret = bt_stack_le_audio_unicast_update(p_cmd->param);
            goto async_handle;                 
        }                   
#endif
#if defined(RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_CSIS_GEN_RSI: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_CSIS_GEN_RSI \r\n");
            ret = bt_stack_le_audio_csis_gen_rsi(p_cmd->param);
            break;               
        }  
        case RTK_BT_LE_AUDIO_ACT_CSIS_UPDATE_SIRK: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_CSIS_UPDATE_SIRK \r\n");
            ret = bt_stack_le_audio_csis_update_sirk(p_cmd->param);
            break;               
        } 
        case RTK_BT_LE_AUDIO_ACT_CSIS_UPDATE_SIZE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_CSIS_UPDATE_SIZE \r\n");
            ret = bt_stack_le_audio_csis_update_size(p_cmd->param);
            break;               
        }         
#endif
#if defined(RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_CHECK_ADV_RSI: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_CHECK_ADV_RSI \r\n");
            ret = bt_stack_le_audio_csis_set_coordinator_check_adv_rsi(p_cmd->param);
            break;               
        } 
        case RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_CFG_DISCOVER: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_CFG_DISCOVER \r\n");
            ret = bt_stack_le_audio_csis_set_coordinator_cfg_discover(p_cmd->param);
            break;               
        }
        case RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_ADD_GROUP: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_ADD_GROUP \r\n");
            ret = bt_stack_le_audio_csis_set_coordinator_add_group(p_cmd->param);
            break;               
        } 
        case RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_ADD_DEVICE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_ADD_DEVICE \r\n");
            ret = bt_stack_le_audio_csis_set_coordinator_add_device(p_cmd->param);
            break;               
        } 
        case RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_WRITE_LOCK: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_WRITE_LOCK \r\n");
            ret = bt_stack_le_audio_csis_set_coordinator_write_lock(p_cmd->param);
            break;               
        } 
#endif

#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_MCP_SERVER_SET_PARAM: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_MCP_SERVER_SET_PARAM \r\n");
            ret = bt_stack_le_audio_mcp_server_set_param(p_cmd->param);
            break;               
        }       
        case RTK_BT_LE_AUDIO_ACT_MCP_SERVER_SEND_DATA: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_MCP_SERVER_SEND_DATA \r\n");
            ret = bt_stack_le_audio_mcp_server_send_data(p_cmd->param);
            break;               
        }   
        case RTK_BT_LE_AUDIO_ACT_MCP_SERVER_READ_CFM: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_MCP_SERVER_READ_CFM \r\n");
            ret = bt_stack_le_audio_mcp_server_read_cfm(p_cmd->param);
            break;               
        }      
#endif   
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_MCP_CLIENT_WRITE_MEDIA_CP: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_MCP_CLIENT_WRITE_MEDIA_CP \r\n");
            ret = bt_stack_le_audio_mcp_client_write_media_cp(p_cmd->param);
            break;               
        }       
        case RTK_BT_LE_AUDIO_ACT_MCP_CLIENT_READ_CHAR_VALUE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_MCP_CLIENT_READ_CHAR_VALUE \r\n");
            ret = bt_stack_le_audio_mcp_client_read_char_value(p_cmd->param);
            break;               
        }   
        case RTK_BT_LE_AUDIO_ACT_MCP_CLIENT_CFG_CCCD: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_MCP_CLIENT_CFG_CCCD \r\n");
            ret = bt_stack_le_audio_mcp_client_cfg_cccd(p_cmd->param);
            break;               
        }
#endif

#if defined(RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_VCS_SET_PARAM: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VCS_SET_PARAM \r\n");
            ret = bt_stack_le_audio_vcs_param_set(p_cmd->param);
            break;             
        } 
        case RTK_BT_LE_AUDIO_ACT_VCS_GET_PARAM: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VCS_GET_PARAM \r\n");
            ret = bt_stack_le_audio_vcs_param_get(p_cmd->param);
            break;             
        } 
#endif
#if defined(RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_VCS_WRITE_CP: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VCS_WRITE_CP \r\n");
            ret = bt_stack_le_audio_vcs_write_cp(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_VCS_GET_VOLUME_STATE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VCS_GET_VOLUME_STATE \r\n");
            ret = bt_stack_le_audio_vcs_get_volume_state(p_cmd->param);
            break;             
        }

        case RTK_BT_LE_AUDIO_ACT_VCS_CHANGE_MUTE:{
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VCS_CHANGE_MUTE \r\n");
            ret = bt_stack_le_audio_vcs_change_mute(p_cmd->param);
            break;             
        }

        case RTK_BT_LE_AUDIO_ACT_VCS_CHANGE_VOLUME:{
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VCS_CHANGE_VOLUME \r\n");
            ret = bt_stack_le_audio_vcs_change_volume(p_cmd->param);
            break;             
        }            
#endif
#if defined(RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_MICS_CHANGE_MUTE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_MICS_CHANGE_MUTE \r\n");
            ret = bt_stack_le_audio_mics_change_mute(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_MICS_SET_MUTE_VALUE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_MICS_SET_MUTE_VALUE \r\n");
            ret = bt_stack_le_audio_mics_set_mute_value(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_MICS_GET_MUTE_VALUE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_MICS_GET_MUTE_VALUE \r\n");
            ret = bt_stack_le_audio_mics_get_mute_value(p_cmd->param);
            break;             
        }
#endif
#if defined(RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_MICS_SET_PARAM: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_MICS_SET_PARAM \r\n");
            ret = bt_stack_le_audio_mics_set_param(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_MICS_GET_PARAM: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_MICS_GET_PARAM \r\n");
            ret = bt_stack_le_audio_mics_get_param(p_cmd->param);
            break;             
        }
#endif
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_VOCS_SET_PARAM: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VOCS_SET_PARAM \r\n");
            ret = bt_stack_le_audio_vocs_set_param(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_VOCS_GET_PARAM: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VOCS_GET_PARAM \r\n");
            ret = bt_stack_le_audio_vocs_get_param(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_VOCS_CFG_CCCD: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VOCS_CFG_CCCD \r\n");
            ret = bt_stack_le_audio_vocs_cfg_cccd(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_VOCS_READ_CHAR_VALUE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VOCS_READ_CHAR_VALUE \r\n");
            ret = bt_stack_le_audio_vocs_read_char_value(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_VOCS_WRITE_CP: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VOCS_WRITE_CP \r\n");
            ret = bt_stack_le_audio_vocs_write_cp(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_VOCS_WRITE_CP_BY_GROUP: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VOCS_WRITE_CP_BY_GROUP \r\n");
            ret = bt_stack_le_audio_vocs_write_cp_by_group(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_VOCS_WRITE_AUDIO_LOCATION: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VOCS_WRITE_AUDIO_LOCATION \r\n");
            ret = bt_stack_le_audio_vocs_write_audio_location(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_VOCS_WRITE_OUTPUT_DES: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VOCS_WRITE_OUTPUT_DES \r\n");
            ret = bt_stack_le_audio_vocs_write_output_des(p_cmd->param);
            break;             
        } 
        case RTK_BT_LE_AUDIO_ACT_VOCS_GET_SRV_DATA: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_VOCS_GET_SRV_DATA \r\n");
            ret = bt_stack_le_audio_vocs_get_srv_data(p_cmd->param);
            break;             
        }
#endif
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_AICS_SET_PARAM: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_AICS_SET_PARAM \r\n");
            ret = bt_stack_le_audio_aics_set_param(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_AICS_GET_PARAM: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_AICS_GET_PARAM \r\n");
            ret = bt_stack_le_audio_aics_get_param(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_AICS_CFG_CCCD: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_AICS_CFG_CCCD \r\n");
            ret = bt_stack_le_audio_aics_cfg_cccd(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_AICS_READ_CHAR_VALUE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_AICS_READ_CHAR_VALUE \r\n");
            ret = bt_stack_le_audio_aics_read_char_value(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_AICS_WRITE_CP: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_AICS_WRITE_CP \r\n");
            ret = bt_stack_le_audio_aics_write_cp(p_cmd->param);
            break;             
        }
        case RTK_BT_LE_AUDIO_ACT_AICS_WRITE_CP_BY_GROUP: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_AICS_WRITE_CP_BY_GROUP \r\n");
            ret = bt_stack_le_audio_aics_write_cp_by_group(p_cmd->param);
            break;             
        }	
        case RTK_BT_LE_AUDIO_ACT_AICS_WRITE_INPUT_DES: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_AICS_WRITE_INPUT_DES \r\n");
            ret = bt_stack_le_audio_aics_write_input_des(p_cmd->param);
            break;             
        }        
        case RTK_BT_LE_AUDIO_ACT_AICS_GET_SRV_DATA: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_AICS_GET_SRV_DATA \r\n");
            ret = bt_stack_le_audio_aics_get_srv_data(p_cmd->param);
            break;             
        }
#endif
#if defined(RTK_BLE_AUDIO_TMAP_SUPPORT) && RTK_BLE_AUDIO_TMAP_SUPPORT
        case RTK_BT_LE_AUDIO_ACT_TMAS_READ_ROLE: {
            BT_API_PRINT(BT_API_DUMP,"RTK_BT_LE_AUDIO_ACT_TMAS_READ_ROLE \r\n");
            ret = bt_stack_le_audio_tmas_client_read_role(p_cmd->param);
            break;             
        }

#endif
        default:
            BT_API_PRINT(BT_API_ERROR,"bt_stack_le_audio_act_handle: unknown act: %d \r\n", p_cmd->act);
			ret = 0;
            break;
    }

	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);
    return ret;
	
async_handle:
    if (ret) {
        p_cmd->ret = ret;
        osif_sem_give(p_cmd->psem);
    }
    return ret;

}
#endif //end of #if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT