/**
 * @file      rtk_bt_sdp.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth LE GAP part type and function deffinition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_SDP_H__
#define __RTK_BT_SDP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define RTK_BT_SDK_MAX_RECORD_LENGTH    100

/* ------------------------------- Data Types ------------------------------- */

/**
 * @struct    rtk_sdp_record_t
 * @brief     Bluetooth sdp srecord struct.
 */
typedef struct {
	struct list_head	list;									//!< list of .rtk_sdp_record_t.
	uint8_t				record[RTK_BT_SDK_MAX_RECORD_LENGTH];	//!< record data
	uint32_t			length;									//!< indicate length of record data
} rtk_sdp_record_t;

/**
 * @fn        uint16_t rtk_sdp_record_add(void *record, uint32_t length)
 * @brief     set profile record.
 * @param[in] record: record data pointer
 * @param[in] length: data length
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_sdp_record_add(void *record, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_SDP_H__ */
