/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_iso_data.h
* @brief     Header file for GAP ISO Data
* @details
* @author
* @date      2021-07-12
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _GAP_ISO_DATA_H_
#define _GAP_ISO_DATA_H_

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#include "gap.h"

#ifdef  __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_ISO_DATA GAP LE ISO Data Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_ISO_DATA_Exported_Macros GAP LE ISO Data Exported Macros
  * @{
  */

/** End of GAP_LE_ISO_DATA_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_ISO_DATA_Exported_Types GAP LE ISO Data Exported Types
  * @{
  */

/** End of GAP_LE_ISO_DATA_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_ISO_DATA_Exported_Functions GAP LE ISO Data Exported Functions
  * @brief
  * @{
  */

/**
 * @brief       Release the ISO Data buffer that GAP supplied.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR, @ref ISOCH_ROLE_ACCEPTOR and @ref BIG_MGR_ROLE_SYNC_RECEIVER.
 *
 * @param[in]   p_buf   Point the buffer that needs to release.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Operation success.
 * @retval Others             Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_gap_direct_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_BT_DIRECT_CB_DATA *p_data = (T_BT_DIRECT_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case BT_DIRECT_MSG_ISO_DATA_IND:
            {
                // Start of the SDU
                uint8_t *p_iso_data = p_data->p_bt_direct_iso->p_buf + p_data->p_bt_direct_iso->offset;
                ......
                gap_iso_data_cfm(p_data->p_bt_direct_iso->p_buf);
            }
            break;
            ......
        }

    }
 * \endcode
 */
bool gap_iso_data_cfm(void *p_buf);

/**
 * @brief       Send isochronous data to Controller.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR, @ref ISOCH_ROLE_ACCEPTOR and @ref BIG_MGR_ROLE_ISOC_BROADCASTER.
 *
 * @param[in]   p_data        Point to data to be sent.
 * @param[in]   handle        Connection handle of the CIS or BIS.
 * @param[in]   iso_sdu_len   Length of the SDU to be sent.
 * @param[in]   ts_flag       Indicates whether contains time_stamp.
 *                            True: contain time_stamp.
 *                            False: not contain time_stamp.
 * @param[in]   time_stamp    A time in microseconds. time_stamp is used when @ref ts_flag is True.
 * @param[in]   pkt_seq_num   The sequence number of the SDU.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = gap_iso_send_data(p_data, handle, iso_sdu_len, ts_flag, time_stamp, pkt_seq_num);
    }
 * \endcode
 */
T_GAP_CAUSE gap_iso_send_data(uint8_t *p_data, uint16_t handle, uint16_t iso_sdu_len, bool ts_flag,
                              uint32_t time_stamp, uint16_t pkt_seq_num);

/**
 * @brief       Get buffer size of isochronous data packets sent from the Host to the Controller.
 *
 *              NOTE: This function shall not be used to get length or number of SDU.
 *
 *                    An SDU sent by @ref gap_iso_send_data will be held in one or more HCI ISO Data packets.
 *
 *                    This function can be called after stack is ready.
 *
 * @param[in,out]   p_iso_data_packet_length           Maximum length (in octets) of the data portion of each HCI ISO Data packet
 *                                                     sent from the Host to the Controller.
 * @param[in,out]   p_available_num_iso_data_packets   Available number of HCI ISO Data packets that can be stored in the ISO buffers.
 *
 * @return Result of operation.
 * @retval GAP_CAUSE_SUCCESS  Operation is successful
 * @retval Others             Operation is failed
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = gap_iso_get_buffer_size(p_iso_data_packet_length, p_available_num_iso_data_packets);
    }
 * \endcode
 */
T_GAP_CAUSE gap_iso_get_buffer_size(uint16_t *p_iso_data_packet_length,
                                    uint8_t *p_available_num_iso_data_packets);

/** End of GAP_LE_ISO_DATA_Exported_Functions
  * @}
  */

/** End of GAP_LE_ISO_DATA
  * @}
  */

/** End of GAP_LE
  * @}
  */

/** End of GAP
  * @}
  */

#ifdef  __cplusplus
}
#endif

#endif

#endif /* _GAP_ISO_DATA_H_ */
