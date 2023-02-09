/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#ifndef _BT_IPC_PARAMETER_CONVERSION_H_
#define _BT_IPC_PARAMETER_CONVERSION_H_

/* -------------------------------- Includes -------------------------------- */
#include <stdint.h>
#include "bt_ipc_profile_config.h"
#include "platform_opts_bt.h"
#include "rtk_bt_common.h"

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP

/**
 * @fn        uint16_t rtk_bt_device_copy_act_ret_param(uint16_t act, void *dst, void *src)
 * @brief     common bt device memory copy value from return memory of ipc to app
 * @param[in] act: API cmd/evt group
 * @param[in] dst: destination address
 * @param[in] src: source address
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_bt_device_copy_act_ret_param(uint16_t act, void *dst, void *src);

/**
 * @fn        void *bt_gap_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert common gap app parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] act: API cmd/evt group
 * @param[in] data: pointer to app parameter struct memory
 * @param[in] buf_len: app parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_gap_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        uint16_t rtk_gap_copy_act_ret_param(uint16_t act, void *dst, void *src)
 * @brief     common gap memory copy value from return memory of ipc to app
 * @param[in] act: API cmd/evt group
 * @param[in] dst: destination address
 * @param[in] src: source address
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_gap_copy_act_ret_param(uint16_t act, void *dst, void *src);

/**
 * @fn        uint32_t rtk_le_get_act_ret_param_size(uint16_t act)
 * @brief     Get le ipc return value size from dev.
 * @param[in] act: API command
 * @return    
 *            - ipc return value size
 */
uint32_t rtk_le_get_act_ret_param_size(uint16_t act);

/**
 * @fn        uint16_t rtk_le_copy_act_ret_param(uint16_t act, void *dst, void *src)
 * @brief     le memory copy value from return memory of ipc to app
 * @param[in] act: API cmd/evt group
 * @param[in] dst: destination address
 * @param[in] src: source address
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_le_copy_act_ret_param(uint16_t act, void *dst, void *src);

/**
 * @fn        void *bt_le_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert le app parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] act: API cmd/evt group
 * @param[in] data: pointer to app parameter struct memory
 * @param[in] buf_len: app parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_le_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        void bt_le_ipc_pop_event_param(uint8_t evt, void *param)
 * @brief     pop received event param contained in ipc buffer
 * @param[in] evt: event code
 * @param[in] param: pointer to ipc buffer
 */
void bt_le_ipc_pop_event_param(uint8_t evt, void *param);

/**
 * @fn        uint32_t rtk_gatts_get_act_ret_param_size(uint16_t act)
 * @brief     Get gatts ipc return value size from dev.
 * @param[in] act: API command
 * @return    
 *            - ipc return value size
 */
uint32_t rtk_gatts_get_act_ret_param_size(uint16_t act);

/**
 * @fn        uint16_t rtk_gatts_copy_act_ret_param(uint16_t act, void *dst, void *src)
 * @brief     gatts memory copy value from return memory of ipc to app
 * @param[in] act: API cmd/evt group
 * @param[in] dst: destination address
 * @param[in] src: source address
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_gatts_copy_act_ret_param(uint16_t act, void *dst, void *src);

/**
 * @fn        void *bt_gatts_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert gatts app parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] act: API cmd/evt group
 * @param[in] data: pointer to app parameter struct memory
 * @param[in] buf_len: app parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_gatts_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        void bt_gatts_ipc_pop_event_param(uint8_t evt, void *param)
 * @brief     pop received event param contained in ipc buffer
 * @param[in] evt: event code
 * @param[in] param: pointer to ipc buffer
 */
void bt_gatts_ipc_pop_event_param(uint8_t evt, void *param);

/**
 * @fn        uint32_t rtk_gattc_get_act_ret_param_size(uint16_t act)
 * @brief     get gattc ipc return value size from dev
 * @param[in] act: API command
 * @return    
 *            - ipc return value size
 */
uint32_t rtk_gattc_get_act_ret_param_size(uint16_t act);

/**
 * @fn        uint16_t rtk_gattc_copy_act_ret_param(uint16_t act, void *dst, void *src)
 * @brief     gatts memory copy value from return memory of ipc to app
 * @param[in] act: API cmd/evt group
 * @param[in] dst: destination address
 * @param[in] src: source address
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_gattc_copy_act_ret_param(uint16_t act, void *dst, void *src);

/**
 * @fn        void *bt_gattc_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert gattc app parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] act: API cmd/evt group
 * @param[in] data: pointer to app parameter struct memory
 * @param[in] buf_len: app parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_gattc_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        void bt_gattc_ipc_pop_event_param(uint8_t evt, void *param)
 * @brief     pop received event param contained in ipc buffer
 * @param[in] evt: event code
 * @param[in] param: pointer to ipc buffer
 */
void bt_gattc_ipc_pop_event_param(uint8_t evt, void *param);

/**
 * @fn        uint32_t rtk_br_get_act_ret_param_size(uint16_t act)
 * @brief     Get br ipc return value size from dev.
 * @param[in] act: API command
 * @return    
 *            - ipc return value size
 */
uint32_t rtk_br_get_act_ret_param_size(uint16_t act);

/**
 * @fn        uint16_t rtk_br_copy_act_ret_param(uint16_t act, void *dst, void *src)
 * @brief     BR/EDR memory copy value from return memory of ipc to app
 * @param[in] act: API cmd/evt group
 * @param[in] dst: destination address
 * @param[in] src: source address
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_br_copy_act_ret_param(uint16_t act, void *dst, void *src);

/**
 * @fn        void *bt_br_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert br app parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] act: API cmd/evt group
 * @param[in] data: pointer to app parameter struct memory
 * @param[in] buf_len: app parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_br_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        void bt_br_ipc_pop_event_param(uint8_t evt, void *param)
 * @brief     pop received event param contained in ipc buffer
 * @param[in] evt: event code
 * @param[in] param: pointer to ipc buffer
 */
void bt_br_ipc_pop_event_param(uint8_t evt, void *param);

/**
 * @fn        uint32_t rtk_avrcp_get_act_ret_param_size(uint16_t act)
 * @brief     Get avrcp ipc return value size from dev.
 * @param[in] act: API command
 * @return    
 *            - ipc return value size
 */
uint32_t rtk_avrcp_get_act_ret_param_size(uint16_t act);

/**
 * @fn        uint16_t rtk_avrcp_copy_act_ret_param(uint16_t act, void *dst, void *src)
 * @brief     avrcp memory copy value from return memory of ipc to app
 * @param[in] act: API cmd/evt group
 * @param[in] dst: destination address
 * @param[in] src: source address
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_avrcp_copy_act_ret_param(uint16_t act, void *dst, void *src);

/**
 * @fn        void *bt_avrcp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert avrcp app parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] act: API cmd/evt group
 * @param[in] data: pointer to app parameter struct memory
 * @param[in] buf_len: app parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_avrcp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        void bt_avrcp_ipc_pop_event_param(uint8_t evt, void *param)
 * @brief     pop received event param contained in ipc buffer
 * @param[in] evt: event code
 * @param[in] param: pointer to ipc buffer
 */
void bt_avrcp_ipc_pop_event_param(uint8_t evt, void *param);

/**
 * @fn        uint32_t rtk_a2dp_get_act_ret_param_size(uint16_t act)
 * @brief     Get a2dp ipc return value size from dev.
 * @param[in] act: API command
 * @return    
 *            - ipc return value size
 */
uint32_t rtk_a2dp_get_act_ret_param_size(uint16_t act);

/**
 * @fn        uint16_t rtk_a2dp_copy_act_ret_param(uint16_t act, void *dst, void *src)
 * @brief     a2dp memory copy value from return memory of ipc to app
 * @param[in] act: API cmd/evt group
 * @param[in] dst: destination address
 * @param[in] src: source address
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_a2dp_copy_act_ret_param(uint16_t act, void *dst, void *src);

/**
 * @fn        void *bt_a2dp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert a2dp app parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] act: API cmd/evt group
 * @param[in] data: pointer to app parameter struct memory
 * @param[in] buf_len: app parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_a2dp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        void bt_a2dp_ipc_pop_event_param(uint8_t evt, void *param)
 * @brief     pop received event param contained in ipc buffer
 * @param[in] evt: event code
 * @param[in] param: pointer to ipc buffer
 */
void bt_a2dp_ipc_pop_event_param(uint8_t evt, void *param);

/**
 * @fn        uint32_t rtk_spp_get_act_ret_param_size(uint16_t act)
 * @brief     Get spp ipc return value size from dev.
 * @param[in] act: API command
 * @return    
 *            - ipc return value size
 */
uint32_t rtk_spp_get_act_ret_param_size(uint16_t act);

/**
 * @fn        uint16_t rtk_spp_copy_act_ret_param(uint16_t act, void *dst, void *src)
 * @brief     spp memory copy value from return memory of ipc to app
 * @param[in] act: API cmd/evt group
 * @param[in] dst: destination address
 * @param[in] src: source address
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_spp_copy_act_ret_param(uint16_t act, void *dst, void *src);

/**
 * @fn        void *bt_spp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert spp app parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] act: API cmd/evt group
 * @param[in] data: pointer to app parameter struct memory
 * @param[in] buf_len: app parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_spp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        void bt_spp_ipc_pop_event_param(uint8_t evt, void *param)
 * @brief     pop received event param contained in ipc buffer
 * @param[in] evt: event code
 * @param[in] param: pointer to ipc buffer
 */
void bt_spp_ipc_pop_event_param(uint8_t evt, void *param);

/**
 * @fn        uint32_t rtk_sdp_get_act_ret_param_size(uint16_t act)
 * @brief     Get sdp ipc return value size from dev.
 * @param[in] act: API command
 * @return    
 *            - ipc return value size
 */
uint32_t rtk_sdp_get_act_ret_param_size(uint16_t act);

/**
 * @fn        uint32_t rtk_hfp_get_act_ret_param_size(uint16_t act)
 * @brief     Get hfp ipc return value size from dev.
 * @param[in] act: API command
 * @return    
 *            - ipc return value size
 */
uint32_t rtk_hfp_get_act_ret_param_size(uint16_t act);

/**
 * @fn        uint16_t rtk_hfp_copy_act_ret_param(uint16_t act, void *dst, void *src)
 * @brief     hfp memory copy value from return memory of ipc to app
 * @param[in] act: API cmd/evt group
 * @param[in] dst: destination address
 * @param[in] src: source address
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_hfp_copy_act_ret_param(uint16_t act, void *dst, void *src);

/**
 * @fn        void *bt_hfp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert hfp app parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] act: API cmd/evt group
 * @param[in] data: pointer to app parameter struct memory
 * @param[in] buf_len: app parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_hfp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        void bt_hfp_ipc_pop_event_param(uint8_t evt, void *param)
 * @brief     pop received event param contained in ipc buffer
 * @param[in] evt: event code
 * @param[in] param: pointer to ipc buffer
 */
void bt_hfp_ipc_pop_event_param(uint8_t evt, void *param);

/**
 * @fn        uint16_t rtk_sdp_copy_act_ret_param(uint16_t act, void *dst, void *src)
 * @brief     sdp memory copy value from return memory of ipc to app
 * @param[in] act: API cmd/evt group
 * @param[in] dst: destination address
 * @param[in] src: source address
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_sdp_copy_act_ret_param(uint16_t act, void *dst, void *src);

/**
 * @fn        void *bt_sdp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert sdp app parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] act: API cmd/evt group
 * @param[in] data: pointer to app parameter struct memory
 * @param[in] buf_len: app parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_sdp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        void bt_sdp_ipc_pop_event_param(uint8_t evt, void *param)
 * @brief     pop received event param contained in ipc buffer
 * @param[in] evt: event code
 * @param[in] param: pointer to ipc buffer
 */
void bt_sdp_ipc_pop_event_param(uint8_t evt, void *param);

/**
 * @fn        uint32_t rtk_le_iso_get_act_ret_param_size(uint16_t act)
 * @brief     Get le iso ipc return value size from dev.
 * @param[in] act: API command
 * @return    
 *            - ipc return value size
 */
uint32_t rtk_le_iso_get_act_ret_param_size(uint16_t act);

/**
 * @fn        uint16_t rtk_le_iso_copy_act_ret_param(uint16_t act, void *dst, void *src)
 * @brief     le iso memory copy value from return memory of ipc to app
 * @param[in] act: API cmd/evt group
 * @param[in] dst: destination address
 * @param[in] src: source address
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_le_iso_copy_act_ret_param(uint16_t act, void *dst, void *src);

/**
 * @fn        void *bt_le_iso_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert le iso app parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] act: API cmd/evt group
 * @param[in] data: pointer to app parameter struct memory
 * @param[in] buf_len: app parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_le_iso_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        void bt_le_iso_ipc_pop_event_param(uint8_t evt, void *param)
 * @brief     pop received event param contained in ipc buffer
 * @param[in] evt: event code
 * @param[in] param: pointer to ipc buffer
 */
void bt_le_iso_ipc_pop_event_param(uint8_t evt, void *param);

/**
 * @fn        uint32_t rtk_le_audio_get_act_ret_param_size(uint16_t act)
 * @brief     Get le audio ipc return value size from dev.
 * @param[in] act: API command
 * @return    
 *            - ipc return value size
 */
uint32_t rtk_le_audio_get_act_ret_param_size(uint16_t act);

/**
 * @fn        uint16_t rtk_le_audio_copy_act_ret_param(uint16_t act, void *dst, void *src)
 * @brief     le audio memory copy value from return memory of ipc to app
 * @param[in] act: API cmd/evt group
 * @param[in] dst: destination address
 * @param[in] src: source address
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_le_audio_copy_act_ret_param(uint16_t act, void *dst, void *src);

/**
 * @fn        void *bt_le_audio_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert le audio app parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] act: API cmd/evt group
 * @param[in] data: pointer to app parameter struct memory
 * @param[in] buf_len: app parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_le_audio_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        void bt_le_audio_ipc_pop_event_param(uint8_t evt, void *param)
 * @brief     pop received event param contained in ipc buffer
 * @param[in] evt: event code
 * @param[in] param: pointer to ipc buffer
 */
void bt_le_audio_ipc_pop_event_param(uint8_t evt, void *param);

#elif defined(CONFIG_BT_NP) && CONFIG_BT_NP
/**
 * @fn        uint32_t get_bt_gap_evt_mem_size(uint8_t evt_code)
 * @brief     get common gap event need memory size
 * @param[in] evt_code: Event code
 * @return    
 *            - need memory size
 */
uint32_t get_bt_gap_evt_mem_size(uint8_t evt_code);

/**
 * @fn        void bt_gap_ipc_pop_cmd_param(uint8_t act, void *param)
 * @brief     pop received parameter contained in ipc buffer
 * @param[in] act: API command
 * @param[in] param: pointer to ipc buffer
 */
void bt_gap_ipc_pop_cmd_param(uint8_t act, void *param);

/**
 * @fn        uint32_t get_bt_le_gap_evt_mem_size(uint8_t evt_code)
 * @brief     get le event need memory size
 * @param[in] evt_code: Event code
 * @return    
 *            - need memory size
 */
uint32_t get_bt_le_gap_evt_mem_size(uint8_t evt_code);

/**
 * @fn        void bt_le_ipc_pop_cmd_param(uint8_t act, void *param)
 * @brief     pop received parameter contained in ipc buffer
 * @param[in] act: API command
 * @param[in] param: pointer to ipc buffer
 */
void bt_le_ipc_pop_cmd_param(uint8_t act, void *param);

/**
 * @fn        void *bt_le_push_event_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert le event parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] event: event code
 * @param[in] data: pointer to event parameter struct memory
 * @param[in] buf_len: event parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_le_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        void *bt_le_push_event_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert le event parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] event: event code
 * @param[in] data: pointer to event parameter struct memory
 * @param[in] buf_len: event parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_le_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        uint32_t get_bt_gatts_evt_mem_size(uint8_t evt_code)
 * @brief     get gatts event need memory size
 * @param[in] evt_code: Event code
 * @return    
 *            - need memory size
 */
uint32_t get_bt_gatts_evt_mem_size(uint8_t evt_code);

/**
 * @fn        void bt_gatts_ipc_pop_cmd_param(uint8_t act, void *param)
 * @brief     pop received parameter contained in ipc buffer
 * @param[in] act: API command
 * @param[in] param: pointer to ipc buffer
 */
void bt_gatts_ipc_pop_cmd_param(uint8_t act, void *param);

/**
 * @fn        void *bt_gatts_push_event_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert gatts event parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] event: event code
 * @param[in] data: pointer to event parameter struct memory
 * @param[in] buf_len: event parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_gatts_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        uint32_t get_bt_gattc_evt_mem_size(uint8_t evt_code)
 * @brief     get gattc event need memory size
 * @param[in] evt_code: Event code
 * @return    
 *            - need memory size
 */
uint32_t get_bt_gattc_evt_mem_size(uint8_t evt_code);

/**
 * @fn        void bt_gattc_ipc_pop_cmd_param(uint8_t act, void *param)
 * @brief     pop received parameter contained in ipc buffer
 * @param[in] act: API command
 * @param[in] param: pointer to ipc buffer
 */
void bt_gattc_ipc_pop_cmd_param(uint8_t act, void *param);

/**
 * @fn        void *bt_gattc_push_event_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert gattc event parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] event: event code
 * @param[in] data: pointer to event parameter struct memory
 * @param[in] buf_len: event parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_gattc_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        uint32_t get_bt_br_gap_evt_mem_size(uint8_t evt_code)
 * @brief     get BR/EDR event need memory size
 * @param[in] evt_code: Event code
 * @return    
 *            - need memory size
 */
uint32_t get_bt_br_gap_evt_mem_size(uint8_t evt_code);

/**
 * @fn        void bt_br_ipc_pop_cmd_param(uint8_t act, void *param)
 * @brief     pop received parameter contained in ipc buffer
 * @param[in] act: API command
 * @param[in] param: pointer to ipc buffer
 */
void bt_br_ipc_pop_cmd_param(uint8_t act, void *param);

/**
 * @fn        void *bt_br_push_event_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert BR/EDR event parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] event: event code
 * @param[in] data: pointer to event parameter struct memory
 * @param[in] buf_len: event parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_br_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        uint32_t get_bt_avrcp_evt_mem_size(uint8_t evt_code)
 * @brief     get avrcp event need memory size
 * @param[in] evt_code: Event code
 * @return    
 *            - need memory size
 */
uint32_t get_bt_avrcp_evt_mem_size(uint8_t evt_code);

/**
 * @fn        void bt_avrcp_ipc_pop_cmd_param(uint8_t act, void *param)
 * @brief     pop received parameter contained in ipc buffer
 * @param[in] act: API command
 * @param[in] param: pointer to ipc buffer
 */
void bt_avrcp_ipc_pop_cmd_param(uint8_t act, void *param);

/**
 * @fn        void *bt_avrcp_push_event_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert avrcp event parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] event: event code
 * @param[in] data: pointer to event parameter struct memory
 * @param[in] buf_len: event parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_avrcp_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        uint32_t get_bt_a2dp_evt_mem_size(uint8_t evt_code)
 * @brief     get a2dp event need memory size
 * @param[in] evt_code: Event code
 * @return    
 *            - need memory size
 */
uint32_t get_bt_a2dp_evt_mem_size(uint8_t evt_code);

/**
 * @fn        void bt_a2dp_ipc_pop_cmd_param(uint8_t act, void *param)
 * @brief     pop received parameter contained in ipc buffer
 * @param[in] act: API command
 * @param[in] param: pointer to ipc buffer
 */
void bt_a2dp_ipc_pop_cmd_param(uint8_t act, void *param);

/**
 * @fn        void *bt_a2dp_push_event_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert a2dp event parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] event: event code
 * @param[in] data: pointer to event parameter struct memory
 * @param[in] buf_len: event parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_a2dp_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        uint32_t get_bt_spp_evt_mem_size(uint8_t evt_code)
 * @brief     get spp event need memory size
 * @param[in] evt_code: Event code
 * @return    
 *            - need memory size
 */
uint32_t get_bt_spp_evt_mem_size(uint8_t evt_code);

/**
 * @fn        void bt_spp_ipc_pop_cmd_param(uint8_t act, void *param)
 * @brief     pop received parameter contained in ipc buffer
 * @param[in] act: API command
 * @param[in] param: pointer to ipc buffer
 */
void bt_spp_ipc_pop_cmd_param(uint8_t act, void *param);

/**
 * @fn        void *bt_spp_push_event_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert spp event parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] event: event code
 * @param[in] data: pointer to event parameter struct memory
 * @param[in] buf_len: event parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_spp_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        uint32_t get_bt_sdp_evt_mem_size(uint8_t evt_code)
 * @brief     get sdp event need memory size
 * @param[in] evt_code: Event code
 * @return    
 *            - need memory size
 */
uint32_t get_bt_sdp_evt_mem_size(uint8_t evt_code);

/**
 * @fn        void bt_sdp_ipc_pop_cmd_param(uint8_t act, void *param)
 * @brief     pop received parameter contained in ipc buffer
 * @param[in] act: API command
 * @param[in] param: pointer to ipc buffer
 */
void bt_sdp_ipc_pop_cmd_param(uint8_t act, void *param);

/**
 * @fn        void *bt_sdp_push_event_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert sdp event parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] event: event code
 * @param[in] data: pointer to event parameter struct memory
 * @param[in] buf_len: event parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_sdp_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        uint32_t get_bt_hfp_evt_mem_size(uint8_t evt_code)
 * @brief     get hfp event need memory size
 * @param[in] evt_code: Event code
 * @return    
 *            - need memory size
 */
uint32_t get_bt_hfp_evt_mem_size(uint8_t evt_code);

/**
 * @fn        void bt_hfp_ipc_pop_cmd_param(uint8_t act, void *param)
 * @brief     pop received parameter contained in ipc buffer
 * @param[in] act: API command
 * @param[in] param: pointer to ipc buffer
 */
void bt_hfp_ipc_pop_cmd_param(uint8_t act, void *param);

/**
 * @fn        void *bt_hfp_push_event_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert hfp event parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] event: event code
 * @param[in] data: pointer to event parameter struct memory
 * @param[in] buf_len: event parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_hfp_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        uint32_t get_bt_le_iso_evt_mem_size(uint8_t evt_code)
 * @brief     get le iso event need memory size
 * @param[in] evt_code: Event code
 * @return    
 *            - need memory size
 */
uint32_t get_bt_le_iso_evt_mem_size(uint8_t evt_code);

/**
 * @fn        void bt_le_iso_ipc_pop_cmd_param(uint8_t act, void *param)
 * @brief     pop received parameter contained in ipc buffer
 * @param[in] act: API command
 * @param[in] param: pointer to ipc buffer
 */
void bt_le_iso_ipc_pop_cmd_param(uint8_t act, void *param);

/**
 * @fn        void *bt_le_iso_push_event_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert le iso event parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] event: event code
 * @param[in] data: pointer to event parameter struct memory
 * @param[in] buf_len: event parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_le_iso_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size);

/**
 * @fn        uint32_t get_bt_le_audio_evt_mem_size(uint8_t evt_code)
 * @brief     get le audio event need memory size
 * @param[in] evt_code: Event code
 * @return    
 *            - need memory size
 */
uint32_t get_bt_le_audio_evt_mem_size(uint8_t evt_code);

/**
 * @fn        void bt_le_audio_ipc_pop_cmd_param(uint8_t act, void *param)
 * @brief     pop received parameter contained in ipc buffer
 * @param[in] act: API command
 * @param[in] param: pointer to ipc buffer
 */
void bt_le_audio_ipc_pop_cmd_param(uint8_t act, void *param);

/**
 * @fn        void *bt_le_audio_push_event_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
 * @brief     convert le audio event parameters to ipc buffer(copy memory from pointer of app parameter to ipc buffer)
 * @param[in] event: event code
 * @param[in] data: pointer to event parameter struct memory
 * @param[in] buf_len: event parameter struct length
 * @param[out] pactual_size: pointer to returned ipc buffer size
 * @return    
 *            - pointer to ipc buffer memory
 */
void *bt_le_audio_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size);

#endif

#endif /* _BT_IPC_PARAMETER_CONVERSION_H_ */

