/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_past_recipient.h
* @brief     Header file for GAP PAST recipient
* @details
* @author
* @date      2021-07-12
* @version   v0.8
* *********************************************************************************************************
*/

/* Define to prevent recursive inclusion **/
#ifndef GAP_PAST_RECIPIENT_H
#define GAP_PAST_RECIPIENT_H

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#if F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT
#include "upperstack_config.h"
#include "gap_le.h"

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_PAST_RECIPIENT GAP LE PAST Recipient Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_PAST_RECIPIENT_Exported_Macros GAP LE PAST Recipient Exported Macros
  * @{
  */
/** @defgroup PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_CTE_TYPE PAST Recipient Periodic Advertising Sync Transfer CTE Type
  * @brief    Use the combination of macro definitions to specify whether to only synchronize to
  *           periodic advertising with certain types of Constant Tone Extension (a value of 0
  *           indicates that the presence or absence of a Constant Tone Extension is irrelevant).
  * @{
  */
#define PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_CTE_TYPE_CTE_IRRELEVANT             0x00   /**< A value of 0 (i.e. all bits clear) indicates that the presence or absence of
                                                                                                  a Constant Tone Extension is irrelevant. */
#define PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_CTE_TYPE_NOT_SYNC_WITH_AOA_CTE      0x01   /**< Do not sync to packets with an AoA Constant Tone Extension. */
#define PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_CTE_TYPE_NOT_SYNC_WITH_AOD_CTE_1US  0x02   /**< Do not sync to packets with an AoD Constant Tone Extension with 1 ��s slots. */
#define PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_CTE_TYPE_NOT_SYNC_WITH_AOD_CTE_2US  0x04   /**< Do not sync to packets with an AoD Constant Tone Extension with 2 ��s slots. */
#define PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_CTE_TYPE_NOT_SYNC_WITHOUT_CTE       0x10   /**< Do not sync to packets without a Constant Tone Extension. */
/** End of PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_CTE_TYPE
  * @}
  */
/** End of GAP_LE_PAST_RECIPIENT_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_PAST_RECIPIENT_Exported_Types GAP LE PAST Recipient Exported Types
  * @{
  */

/** @brief GAP PAST recipient periodic advertising sync transfer mode. */
typedef enum
{
    PERIODIC_ADV_SYNC_TRANSFER_MODE_NO_ATTEMPT_TO_SYNCHRONIZE = 0x00,                                   /**< No attempt is made to synchronize to the periodic advertising and
                                                                                                             no GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO is sent (default). */
    PERIODIC_ADV_SYNC_TRANSFER_MODE_PERIODIC_ADV_REPORT_DISABLED = 0x01,                                /**< GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO is sent.
                                                                                                             GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO will be disabled. */
    PERIODIC_ADV_SYNC_TRANSFER_MODE_PERIODIC_ADV_REPORT_ENABLED_WITH_DUPLICATE_FILTER_DISABLED = 0x02,  /**< GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO is sent.
                                                                                                             GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO will be enabled with duplicate filtering disabled. */
    PERIODIC_ADV_SYNC_TRANSFER_MODE_PERIODIC_ADV_REPORT_ENABLED_WITH_DUPLICATE_FILTER_ENABLED = 0x03,   /**< GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO is sent.
                                                                                                             GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO will be enabled with duplicate filtering enabled. */
} T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_MODE;

/** @brief  Definition of GAP PAST recipient periodic advertising sync transfer parameter. */
typedef struct
{
    T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_MODE mode;
    uint8_t cte_type;                                           /**< @ref PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_CTE_TYPE. */
    uint16_t skip;                                              /**< The maximum number of periodic advertising events that can be skipped after a successful receive.
                                                                     Range: 0x0000 to 0x01F3 */
    uint16_t sync_timeout;                                      /**< Synchronization timeout for the periodic advertising train.
                                                                     Range: 0x000A to 0x4000
                                                                     Time = N*10 ms
                                                                     Time Range: 100 ms to 163.84 s */
} T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM;
/** End of GAP_LE_PAST_RECIPIENT_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_PAST_RECIPIENT_Exported_Functions GAP LE PAST Recipient Exported Functions
  * @brief
  * @{
  */
/**
 * @brief       Specify the initial value for PAST recipient periodic advertising sync transfer parameter to be used
 *              for all subsequent connections.
 *              NOTE: This function does not affect any existing connection.
 *
 *              NOTE: This function can be called after stack is ready.
 *              Explanation: If stack is ready, Application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
 *                           with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
 *
 *              If sending request operation is success, the result of setting parameters will be returned by
 *              @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PAST_RECIPIENT_SET_DEFAULT_PERIODIC_ADV_SYNC_TRANSFER_PARAMS.
 *
 *              Reception of periodic advertising synchronization information will be returned by @ref app_gap_callback with
 *              cb_type @ref GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO.
 *              GAP PA synchronization states will be returned by @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PA_SYNC_STATE_CHANGE_INFO.
 *              Periodic advertisement will be returned by @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO.
 *
 * @param[in]   p_periodic_adv_sync_transfer_param    PAST recipient periodic advertising sync transfer parameter. @ref T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM periodic_adv_sync_transfer_param;

        ......
        // Initialization of periodic_adv_sync_transfer_param is dependent on Application.

        T_GAP_CAUSE cause = le_past_recipient_set_default_periodic_adv_sync_transfer_params(&periodic_adv_sync_transfer_param);
    }

    void app_handle_pa_sync_state_evt(uint8_t sync_id, uint16_t sync_handle,
                                      T_GAP_PA_SYNC_STATE new_state, bool sync_transfer_received_flag,
                                      uint16_t terminate_cause)
    {
        ......
        switch (new_state)
        {
        case GAP_PA_SYNC_STATE_TERMINATED:
            {
            }
            break;

        case GAP_PA_SYNC_STATE_SYNCHRONIZED:
            {
            }
            break;

        case GAP_PA_SYNC_STATE_TERMINATING:
            {
            }
            break;

        default:
            break;
        }
        ......
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case GAP_MSG_LE_PAST_RECIPIENT_SET_DEFAULT_PERIODIC_ADV_SYNC_TRANSFER_PARAMS:
            APP_PRINT_INFO1("GAP_MSG_LE_PAST_RECIPIENT_SET_DEFAULT_PERIODIC_ADV_SYNC_TRANSFER_PARAMS: cause 0x%x",
                            p_data->p_le_past_recipient_set_default_periodic_adv_sync_transfer_params_rsp->cause);
            break;

        case GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO:
            APP_PRINT_INFO8("GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO: cause 0x%x, conn_id %d, sync_id %d, sync_handle 0x%x, service_data 0x%x, adv_sid 0x%x, adv_addr_type %d, adv_addr %s",
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->cause,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->conn_id,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->sync_id,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->sync_handle,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->service_data,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->adv_sid,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->adv_addr_type,
                            TRACE_BDADDR(p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->adv_addr));
            APP_PRINT_INFO7("GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO: cause 0x%x, conn_id %d, sync_id %d, sync_handle 0x%x, adv_phy %d, periodic_adv_interval 0x%x, adv_clock_accuracy %d",
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->cause,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->conn_id,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->sync_id,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->sync_handle,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->adv_phy,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->periodic_adv_interval,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->adv_clock_accuracy);
            break;

        case GAP_MSG_LE_PA_SYNC_STATE_CHANGE_INFO:
            {
                app_handle_pa_sync_state_evt(p_data->p_le_pa_sync_state_change_info->sync_id,
                                            p_data->p_le_pa_sync_state_change_info->sync_handle,
                                            (T_GAP_PA_SYNC_STATE)p_data->p_le_pa_sync_state_change_info->state,
                                            p_data->p_le_pa_sync_state_change_info->sync_transfer_received_flag,
                                            p_data->p_le_pa_sync_state_change_info->cause);
            }
            break;

        case GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO:
            APP_PRINT_INFO7("GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO: sync_id %d, sync_handle 0x%x, tx_power %d, rssi %d, cte_type %d, data_status 0x%x, data_len %d",
                            p_data->p_le_periodic_adv_report_info->sync_id,
                            p_data->p_le_periodic_adv_report_info->sync_handle,
                            p_data->p_le_periodic_adv_report_info->tx_power,
                            p_data->p_le_periodic_adv_report_info->rssi,
                            p_data->p_le_periodic_adv_report_info->cte_type,
                            p_data->p_le_periodic_adv_report_info->data_status,
                            p_data->p_le_periodic_adv_report_info->data_len);
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE le_past_recipient_set_default_periodic_adv_sync_transfer_params(
    T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM *p_periodic_adv_sync_transfer_param);

/**
 * @brief       Specify how the Controller will process periodic advertising synchronization information received
 *              from the device identified by conn_id.
 *
 *              If sending request operation is success, the result of setting parameters will be returned by
 *              @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PAST_RECIPIENT_SET_PERIODIC_ADV_SYNC_TRANSFER_PARAMS.
 *
 *              Reception of periodic advertising synchronization information will be returned by @ref app_gap_callback with
 *              cb_type @ref GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO.
 *              GAP PA synchronization states will be returned by @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PA_SYNC_STATE_CHANGE_INFO.
 *              Periodic advertisement will be returned by @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO.
 *
 * @param[in]   conn_id                               Used to identify a connection.
 * @param[in]   p_periodic_adv_sync_transfer_param    PAST recipient periodic advertising sync transfer parameter. @ref T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM periodic_adv_sync_transfer_param;

        ......
        // Initialization of periodic_adv_sync_transfer_param is dependent on Application.

        T_GAP_CAUSE cause = le_past_recipient_set_periodic_adv_sync_transfer_params(conn_id, &periodic_adv_sync_transfer_param);
    }

    void app_handle_pa_sync_state_evt(uint8_t sync_id, uint16_t sync_handle,
                                      T_GAP_PA_SYNC_STATE new_state, bool sync_transfer_received_flag,
                                      uint16_t terminate_cause)
    {
        ......
        switch (new_state)
        {
        case GAP_PA_SYNC_STATE_TERMINATED:
            {
            }
            break;

        case GAP_PA_SYNC_STATE_SYNCHRONIZED:
            {
            }
            break;

        case GAP_PA_SYNC_STATE_TERMINATING:
            {
            }
            break;

        default:
            break;
        }
        ......
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case GAP_MSG_LE_PAST_RECIPIENT_SET_PERIODIC_ADV_SYNC_TRANSFER_PARAMS:
            APP_PRINT_INFO2("GAP_MSG_LE_PAST_RECIPIENT_SET_PERIODIC_ADV_SYNC_TRANSFER_PARAMS: cause 0x%x, conn_id %d",
                            p_data->p_le_past_recipient_set_periodic_adv_sync_transfer_params_rsp->cause,
                            p_data->p_le_past_recipient_set_periodic_adv_sync_transfer_params_rsp->conn_id);
            break;

        case GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO:
            APP_PRINT_INFO8("GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO: cause 0x%x, conn_id %d, sync_id %d, sync_handle 0x%x, service_data 0x%x, adv_sid 0x%x, adv_addr_type %d, adv_addr %s",
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->cause,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->conn_id,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->sync_id,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->sync_handle,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->service_data,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->adv_sid,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->adv_addr_type,
                            TRACE_BDADDR(p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->adv_addr));
            APP_PRINT_INFO7("GAP_MSG_LE_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_RECEIVED_INFO: cause 0x%x, conn_id %d, sync_id %d, sync_handle 0x%x, adv_phy %d, periodic_adv_interval 0x%x, adv_clock_accuracy %d",
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->cause,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->conn_id,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->sync_id,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->sync_handle,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->adv_phy,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->periodic_adv_interval,
                            p_data->p_le_past_recipient_periodic_adv_sync_transfer_received_info->adv_clock_accuracy);
            break;

        case GAP_MSG_LE_PA_SYNC_STATE_CHANGE_INFO:
            {
                app_handle_pa_sync_state_evt(p_data->p_le_pa_sync_state_change_info->sync_id,
                                            p_data->p_le_pa_sync_state_change_info->sync_handle,
                                            (T_GAP_PA_SYNC_STATE)p_data->p_le_pa_sync_state_change_info->state,
                                            p_data->p_le_pa_sync_state_change_info->sync_transfer_received_flag,
                                            p_data->p_le_pa_sync_state_change_info->cause);
            }
            break;

        case GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO:
            APP_PRINT_INFO7("GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO: sync_id %d, sync_handle 0x%x, tx_power %d, rssi %d, cte_type %d, data_status 0x%x, data_len %d",
                            p_data->p_le_periodic_adv_report_info->sync_id,
                            p_data->p_le_periodic_adv_report_info->sync_handle,
                            p_data->p_le_periodic_adv_report_info->tx_power,
                            p_data->p_le_periodic_adv_report_info->rssi,
                            p_data->p_le_periodic_adv_report_info->cte_type,
                            p_data->p_le_periodic_adv_report_info->data_status,
                            p_data->p_le_periodic_adv_report_info->data_len);
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE le_past_recipient_set_periodic_adv_sync_transfer_params(uint8_t conn_id,
                                                                    T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM *p_periodic_adv_sync_transfer_param);
#endif

/** End of GAP_LE_PAST_RECIPIENT_Exported_Functions
  * @}
  */

/** End of GAP_LE_PAST_RECIPIENT
  * @}
  */

/** End of GAP_LE
  * @}
  */

/** End of GAP
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif

#endif /* GAP_PAST_RECIPIENT_H */
