/**
 * @file      rtk_bt_a2dp.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth LE GAP part type and function deffinition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_A2DP_H__
#define __RTK_BT_A2DP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define RTK_BT_A2DP_MAX_CODEC_PARAM_LENGTH 12
#define RTK_BT_A2DP_MAX_STREAM_DATA_LENGTH 1024

/* ------------------------------- Data Types ------------------------------- */
/**
 * @struct    rtk_bt_a2dp_conn_ind_t
 * @brief     a2dp connection connection indication.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
} rtk_bt_a2dp_conn_ind_t;

/**
 * @struct    rtk_bt_a2dp_codec_param_t
 * @brief     Bluetooth A2DP stream data indication.
 */
typedef struct {
	uint8_t  codec_type;								/*!< codec type */
	uint32_t length;									/*!< codec param data length */
	uint8_t  data[RTK_BT_A2DP_MAX_CODEC_PARAM_LENGTH];	/*!< codec data */
} rtk_bt_a2dp_codec_param_t;

/**
 * @struct    rtk_bt_a2dp_codec_t
 * @brief     Bluetooth A2DP codec status.
 */
typedef struct {
	uint8_t codec_type;									/*!< indicate codec */
} rtk_bt_a2dp_codec_t;

/**
 * @struct    rtk_bt_a2dp_stream_start_t
 * @brief     Bluetooth A2DP stream start indication.
 */
typedef struct {
	uint8_t active_a2dp_link_index;						/*!< stream index */
	uint8_t stream_cfg;									/*!< stream cfg */
} rtk_bt_a2dp_stream_start_t;

/**
 * @struct    rtk_bt_a2dp_stream_start_t
 * @brief     Bluetooth A2DP stream start indication.
 */
typedef struct {
	uint32_t length;									/*!< stream data length */
	uint8_t  data[RTK_BT_A2DP_MAX_STREAM_DATA_LENGTH];	/*!< stream data */
} rtk_bt_a2dp_stream_data_t;

/**
 * @fn        uint16_t rtk_a2dp_codec_cfg(uint8_t codec_type, void *codec_t, uint32_t length)
 * @brief     Set a2dp codec configuration.
 * @param[in] codec_type: codec index
 * @param[in] codec_t: codec struct pointer
 * @param[in] length: data length
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_a2dp_codec_cfg(uint8_t codec_type, void *codec_t, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_A2DP_H__ */
