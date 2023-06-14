/**
 * @file      rtk_bt_avrcp.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth LE GAP part type and function deffinition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_AVRCP_H__
#define __RTK_BT_AVRCP_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* ------------------------------- Data Types ------------------------------- */
/**
 * @struct    rtk_bt_avrcp_absolute_volume_set_t
 * @brief     Bluetooth AVRCP absolute volume set.
 */
typedef struct {
	uint8_t bd_addr[6];
	uint8_t volume;
} rtk_bt_avrcp_absolute_volume_set_t;

/**
 * @struct    rtk_bt_avrcp_volume_t
 * @brief     Bluetooth AVRCP volume status.
 */
typedef struct {
	uint8_t volume;                                 /*!< indicate play volume */
} rtk_bt_avrcp_volume_t;

/**
 * @struct    rtk_bt_avrcp_sub_event_t
 * @brief     Bluetooth AVRCP play status.
 */
typedef struct {
	uint8_t avrcp_play_status;                      /*!< indicate play status */
} rtk_bt_avrcp_sub_event_t;

/**
 * @struct    rtk_bt_avrcp_conn_ind_t
 * @brief     avrcp connection connection indication.
 */
typedef struct {
	uint8_t bd_addr[6];                             /*!< address */
} rtk_bt_avrcp_conn_ind_t;

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_AVRCP_H__ */
