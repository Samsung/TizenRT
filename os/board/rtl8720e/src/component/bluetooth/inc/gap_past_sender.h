/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_past_sender.h
* @brief     Header file for GAP PAST sender
* @details   This file defines sender of PAST related API.
* @author
* @date      2021-07-12
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_PAST_SENDER_H
#define GAP_PAST_SENDER_H

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "upperstack_config.h"
#include "gap_le.h"

#if F_BT_LE_5_1_PAST_SENDER_SUPPORT

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_PAST_SENDER GAP LE PAST Sender Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_PAST_SENDER_Exported_Macros GAP LE PAST Sender Exported Macros
  * @{
  */

/** End of GAP_LE_PAST_SENDER_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_PAST_SENDER_Exported_Types GAP LE PAST Sender Exported Types
  * @{
  */

/** End of GAP_LE_PAST_SENDER_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_PAST_SENDER_Exported_Functions GAP LE PAST Sender Exported Functions
  * @brief
  * @{
  */
#if F_BT_LE_5_1_PAST_SENDER_ADV_SUPPORT
/**
 * @brief       Request the Controller to send synchronization information about the periodic advertising in an advertising set
 *              to a connected device.
 *
 *              If sending request operation is success, the result will be returned by
 *              @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PAST_SENDER_PERIODIC_ADV_SET_INFO_TRANSFER.
 *
 * @param[in]   conn_id        Used to identify a connection.
 * @param[in]   service_data   A value provided by the Host to identify the periodic advertising train to the peer device.
 * @param[in]   adv_handle     Used to identify the advertising set.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_past_sender_periodic_adv_set_info_transfer(conn_id, service_data, adv_handle);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case GAP_MSG_LE_PAST_SENDER_PERIODIC_ADV_SET_INFO_TRANSFER:
            APP_PRINT_INFO2("GAP_MSG_LE_PAST_SENDER_PERIODIC_ADV_SET_INFO_TRANSFER: cause 0x%x, conn_id 0x%x",
                            p_data->p_le_past_sender_periodic_adv_set_info_transfer_rsp->cause,
                            p_data->p_le_past_sender_periodic_adv_set_info_transfer_rsp->conn_id);
            break;

        ......
        }
    }
 * \endcode
 */
T_GAP_CAUSE le_past_sender_periodic_adv_set_info_transfer(uint8_t conn_id, uint16_t service_data,
                                                          uint8_t adv_handle);
#endif

#if F_BT_LE_5_1_PAST_SENDER_SYNC_SUPPORT
/**
 * @brief       Request the Controller to send synchronization information about the periodic advertising train identified by sync_id
 *              to a connected device.
 *
 *              If sending request operation is success, the result will be returned by
 *              @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PAST_SENDER_PERIODIC_ADV_SYNC_TRANSFER.
 *
 * @param[in]   conn_id        Used to identify a connection.
 * @param[in]   service_data   A value provided by the Host to identify the periodic advertising train to the peer device.
 * @param[in]   sync_id        Identify the periodic advertising train.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_past_sender_periodic_adv_sync_transfer(conn_id, service_data, sync_id);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case GAP_MSG_LE_PAST_SENDER_PERIODIC_ADV_SYNC_TRANSFER:
            APP_PRINT_INFO2("GAP_MSG_LE_PAST_SENDER_PERIODIC_ADV_SYNC_TRANSFER: cause 0x%x, conn_id 0x%x",
                            p_data->p_le_past_sender_periodic_adv_sync_transfer_rsp->cause,
                            p_data->p_le_past_sender_periodic_adv_sync_transfer_rsp->conn_id);
            break;

        ......
        }
    }
 * \endcode
 */
T_GAP_CAUSE le_past_sender_periodic_adv_sync_transfer(uint8_t conn_id, uint16_t service_data,
                                                      uint8_t sync_id);
#endif
/** End of GAP_LE_PAST_SENDER_Exported_Functions
  * @}
  */

/** End of GAP_LE_PAST_SENDER
  * @}
  */

/** End of GAP_LE
  * @}
  */

/** End of GAP
  * @}
  */
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif /* GAP_PAST_SENDER_H */
