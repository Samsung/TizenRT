/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_aox_connless_receiver.h
* @brief     Head file for GAP AoA/AoD connectionless receiver
* @details
* @author
* @date      2021-07-12
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_AOX_CONNLESS_RECEIVER_H
#define GAP_AOX_CONNLESS_RECEIVER_H

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
#include "gap_le_types.h"

#if F_BT_LE_5_1_AOX_CONNLESS_RECEIVER_SUPPORT

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_AOX_CONNLESS_RECEIVER GAP LE AoA/AoD Connectionless Receiver Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_AOX_CONNLESS_RECEIVER_Exported_Macros GAP LE AoA/AoD Connectionless Receiver Exported Macros
  * @{
  */
/** @defgroup AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE AoA/AoD Connectionless Receiver Sampling Enable flag
  * @brief Use the macro definitions to describe the sampling enable parameters.
  * @{
  */
#define AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE_IQ_SAMPLING_DISABLED      0x00   /**< Connectionless IQ sampling is disabled (default). */
#define AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE_IQ_SAMPLING_ENABLED       0x01   /**< Connectionless IQ sampling is enabled. */
/** End of AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE
  * @}
  */
/** End of GAP_LE_AOX_CONNLESS_RECEIVER_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_AOX_CONNLESS_RECEIVER_Exported_Types GAP LE AoA/AoD Connectionless Receiver Exported Types
  * @brief
  * @{
  */

/** End of GAP_LE_AOX_CONNLESS_RECEIVER_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/

/** @defgroup GAP_LE_AOX_CONNLESS_RECEIVER_Exported_Functions GAP LE AoA/AoD Connectionless Receiver Exported Functions
  * @brief
  * @{
  */
/**
 * @brief       Request the Controller to enable or disable capturing IQ samples from the Constant Tone Extensions
 *              of periodic advertising packets in the periodic advertising train.
 *
 *              If sending request operation is success, the result of enabling or disabling will be returned by
 *              @ref app_gap_aox_callback with cb_type @ref GAP_MSG_LE_AOX_CONNLESS_RECEIVER_SET_IQ_SAMPLING_ENABLE.
 *
 *              IQ information from the Constant Tone Extension of a received advertising packet forming part of
 *              the periodic advertising train will be reported by @ref app_gap_aox_callback with cb_type
 *              @ref GAP_MSG_LE_AOX_CONNLESS_RECEIVER_CONNLESS_IQ_REPORT_INFO.
 *
 * @param[in]   sync_id                    Identify the periodic advertising train.
 * @param[in]   sampling_enable            @ref AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE.
 * @param[in]   slot_durations             @ref T_GAP_SLOT_DUATIONS_TYPE.
 * @param[in]   max_sampled_ctes           Specify the maximum number of Constant Tone Extensions in each periodic advertising
 *                                         event that the Controller should collect and report IQ samples from.
 *                                         0x00: Sample and report all available Constant Tone Extensions
 *                                         0xXX: The maximum number of Constant Tone Extensions to sample and
 *                                               report in each periodic advertising interval
 *                                               Range: 0x01 to 0x10
 * @param[in]   switching_pattern_length   The number of Antenna IDs in the pattern.
 *                                         Range: 0x02 to max_switching_pattern_length supported by controller returned by
 *                                                @ref le_aox_read_antenna_information.
 * @param[in]   p_antenna_ids              Antenna ID in the pattern.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_aox_connless_receiver_set_iq_sampling_enable(sync_id, sampling_enable, slot_durations, max_sampled_ctes, switching_pattern_length, p_antenna_ids);
    }

    T_APP_RESULT app_gap_aox_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_AOX_CB_DATA *p_data = (T_LE_AOX_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case GAP_MSG_LE_AOX_CONNLESS_RECEIVER_SET_IQ_SAMPLING_ENABLE:
            {
                APP_PRINT_INFO5("GAP_MSG_LE_AOX_CONNLESS_RECEIVER_SET_IQ_SAMPLING_ENABLE: cause 0x%x, sync_id %d, sync_handle 0x%x, sampling_enable 0x%x, AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE_IQ_SAMPLING_ENABLED %d",
                                p_data->p_le_aox_connless_receiver_set_iq_sampling_enable_rsp->cause,
                                p_data->p_le_aox_connless_receiver_set_iq_sampling_enable_rsp->sync_id,
                                p_data->p_le_aox_connless_receiver_set_iq_sampling_enable_rsp->sync_handle,
                                p_data->p_le_aox_connless_receiver_set_iq_sampling_enable_rsp->sampling_enable,
                                (p_data->p_le_aox_connless_receiver_set_iq_sampling_enable_rsp->sampling_enable &
                                AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE_IQ_SAMPLING_ENABLED));
            }
            break;

        case GAP_MSG_LE_AOX_CONNLESS_RECEIVER_CONNLESS_IQ_REPORT_INFO:
            {
                APP_PRINT_INFO8("GAP_MSG_LE_AOX_CONNLESS_RECEIVER_CONNLESS_IQ_REPORT_INFO: sync_id %d, sync_handle 0x%x, channel_index %d, rssi %d, rssi_antenna_id %d, cte_type %d, slot_durations %d, packet_status %d",
                                p_data->p_le_aox_connless_receiver_connectionless_iq_report_info->sync_id,
                                p_data->p_le_aox_connless_receiver_connectionless_iq_report_info->sync_handle,
                                p_data->p_le_aox_connless_receiver_connectionless_iq_report_info->channel_index,
                                p_data->p_le_aox_connless_receiver_connectionless_iq_report_info->rssi,
                                p_data->p_le_aox_connless_receiver_connectionless_iq_report_info->rssi_antenna_id,
                                p_data->p_le_aox_connless_receiver_connectionless_iq_report_info->cte_type,
                                p_data->p_le_aox_connless_receiver_connectionless_iq_report_info->slot_durations,
                                p_data->p_le_aox_connless_receiver_connectionless_iq_report_info->packet_status);
                APP_PRINT_INFO4("GAP_MSG_LE_AOX_CONNLESS_RECEIVER_CONNLESS_IQ_REPORT_INFO: sync_id %d, sync_handle 0x%x, periodic_event_counter %d, sample_count %d",
                                p_data->p_le_aox_connless_receiver_connectionless_iq_report_info->sync_id,
                                p_data->p_le_aox_connless_receiver_connectionless_iq_report_info->sync_handle,
                                p_data->p_le_aox_connless_receiver_connectionless_iq_report_info->periodic_event_counter,
                                p_data->p_le_aox_connless_receiver_connectionless_iq_report_info->sample_count);
                ......
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE le_aox_connless_receiver_set_iq_sampling_enable(uint8_t sync_id,
                                                            uint8_t sampling_enable, T_GAP_SLOT_DUATIONS_TYPE slot_durations, uint8_t max_sampled_ctes,
                                                            uint8_t switching_pattern_length, uint8_t *p_antenna_ids);

/** End of GAP_LE_AOX_CONNLESS_RECEIVER_Exported_Functions
  * @}
  */

/** End of GAP_LE_AOX_CONNLESS_RECEIVER
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

#endif /* GAP_AOX_CONNLESS_RECEIVER_H */
