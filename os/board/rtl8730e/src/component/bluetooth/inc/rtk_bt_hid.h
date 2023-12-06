/**
 * @file      rtk_bt_hid.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth hid type and function deffinition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_HID_H__
#define __RTK_BT_HID_H__

#include <rtk_bt_common.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define RTK_BT_HID_MAX_DESCRIPTOR_LENGTH 400
#define RTK_BT_HID_MAX_INPUT_DATA_LENGTH 20

/**
 * @typedef   rtk_bt_hid_role_t
 * @brief     hid role 
 */
typedef enum {
	RTK_BT_HID_ROLE_GAMEPAD = 0x00,					/*!< BT HID GAMEPAD */
	RTK_BT_HID_ROLE_MOUSE = 0x01,					/*!< BT HID MOUSE */
} rtk_bt_hid_role_t;

/* ------------------------------- Data Types ------------------------------- */

/**
 * @struct    rtk_bt_hid_des_t
 * @brief     hid descriptor struct.
 */
typedef struct {
	uint8_t des[RTK_BT_HID_MAX_DESCRIPTOR_LENGTH];	/*!< descriptor */
	uint32_t len;									/*!< descriptor data length */
} rtk_bt_hid_des_t;

/**
 * @struct    rtk_bt_hid_input_data_t
 * @brief     hid interrupt input data struct.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
	uint8_t data[RTK_BT_HID_MAX_INPUT_DATA_LENGTH];		/*!< data */
	uint32_t len;										/*!< indicate data length */
} rtk_bt_hid_input_data_t;

/**
 * @struct    rtk_bt_hid_conn_ind_t
 * @brief     hid connection indication.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
} rtk_bt_hid_conn_ind_t;

/**
 * @struct    rtk_bt_hid_disconn_ind_t
 * @brief     hid disconnection indication.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
	uint16_t cause;										/*!< disconnection cause */
} rtk_bt_hid_disconn_ind_t;

/* ------------------------------ Functions Declaration ------------------------------ */

/**
 * @fn        uint16_t rtk_hid_descriptor_add(void *des, uint32_t length)
 * @brief     set hid descriptor data.
 * @param[in] des: descriptor data pointer
 * @param[in] length: data length
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_hid_descriptor_add(void *des, uint32_t length);

/**
 * @fn        uint16_t rtk_bt_hid_disconnect(uint8_t *bd_addr)
 * @brief     disconnect hid.
 * @param[in] bd_addr: bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_hid_disconnect(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_hid_get_report_rsp(uint8_t *bd_addr)
 * @brief     get report response.
 * @param[in] bd_addr: bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_hid_get_report_rsp(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_hid_input_data_send(uint8_t *bd_addr, uint8_t *data, uint32_t len)
 * @brief     hid input data send.
 * @param[in] bd_addr: bt address
 * @param[in] data: pointer to input data
 * @param[in] len: input data length
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_hid_input_data_send(uint8_t *bd_addr, uint8_t *data, uint32_t len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_A2DP_H__ */
