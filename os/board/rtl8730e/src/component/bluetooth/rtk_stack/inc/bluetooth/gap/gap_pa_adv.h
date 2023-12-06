/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_pa_adv.h
* @brief     Header file for GAP PA adv
* @details   This file defines advertising state of PA related API.
* @author
* @date      2021-07-12
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_PA_ADV_H
#define GAP_PA_ADV_H

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

#if F_BT_LE_5_0_PA_ADV_SUPPORT

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_PA_ADV GAP LE PA Adv Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_PA_ADV_Exported_Macros GAP LE PA Adv Exported Macros
  * @{
  */

/** @defgroup PA_ADV_PARAM Periodic Advertising Parameter flag
  * @brief    Use the combination of macro definitions to set periodic advertising related parameters
  *           for the specified advertising set by calling @ref le_pa_adv_start_setting.
  * @{
  */
#define PA_ADV_SET_PERIODIC_ADV_PARAS      0x01   /**< Set periodic advertising parameters supplied by @ref le_pa_adv_set_periodic_adv_param. */
#define PA_ADV_SET_PERIODIC_ADV_DATA       0x02   /**< Set periodic advertising data supplied by @ref le_pa_adv_set_periodic_adv_data. */
/** End of PA_ADV_PARAM
  * @}
  */

/** @defgroup PA_ADV_PERIODIC_ADV_PROP Periodic Advertising Properties flag
  * @brief Use the combination of macro definitions to indicate which fields should be included in the advertising packet.
  * @{
  */
#define PA_ADV_PROP_INCLUDE_TX_POWER      0x0040   /**< Include TxPower in the periodic header of the advertising PDU. */
/** End of PA_ADV_PERIODIC_ADV_PROP
  * @}
  */

/** @defgroup PA_ADV_ENABLE Periodic Advertising Enable flag
  * @brief Use the combination of macro definitions to describe the enable parameters.
  * @{
  */
#define PA_ADV_ENABLE_ENABLE_PERIODIC_ADVERTISING      0x01   /**< Enable periodic advertising. */
#define PA_ADV_ENABLE_INCLUDE_ADI                      0x02   /**< Include the ADI field in AUX_SYNC_IND PDUs. Controller in Synchronization State that supports the Periodic Advertising ADI Support feature could enable duplicate filtering to filter periodic advertising reports. */
/** End of PA_ADV_ENABLE
  * @}
  */
/** End of GAP_LE_PA_ADV_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_PA_ADV_Exported_Types GAP LE PA Adv Exported Types
  * @{
  */

/** @brief GAP PA periodic advertising state. */
typedef enum
{
    PA_ADV_STATE_IDLE,                            /**< Idle, no periodic advertising. */
    PA_ADV_STATE_START_EXT_ADV_STATE_IDLE,        /**< Start periodic advertising when extended advertising of the specified advertising set is disabled. A temporary state, haven't received the result. */
    PA_ADV_STATE_WAIT_EXT_ADV_STATE_ADVERTISING,  /**< Periodic advertising is not started until the extended advertising of the specified advertising set is enabled. */
    PA_ADV_STATE_START,                           /**< Start periodic advertising when extended advertising of the specified advertising set is enabled. A temporary state, haven't received the result. */
    PA_ADV_STATE_ADVERTISING,                     /**< Periodic advertising. */
    PA_ADV_STATE_STOP,                            /**< Stop periodic advertising. A temporary state, haven't received the result. */
} T_GAP_PA_ADV_STATE;

/** End of GAP_LE_PA_ADV_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_PA_ADV_Exported_Functions GAP LE PA Adv Exported Functions
  * @brief
  * @{
  */
/**
 * @brief       Initialize the number of advertising sets for advertising state in periodic advertising.
 *
 *              NOTE: If PA advertising state will be used, @ref le_ext_adv_gap_msg_info_way(false) should be invoked.
 *
 * @param[in]   adv_set_num Advertising sets number.
 * @return GAP Operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval GAP_CAUSE_INVALID_PARAM Operation failure, invalid parameter.
 * @retval GAP_CAUSE_NO_RESOURCE   Operation failure, memory acquisition failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_pa_adv_init(adv_set_num);
    }
 * \endcode
 */
T_GAP_CAUSE le_pa_adv_init(uint8_t adv_set_num);

/**
 * @brief       Set GAP PA advertising parameters for the specified advertising set.
 *
 * @param[in]   adv_handle                    Used to identify an advertising set, which is assigned by @ref le_ext_adv_create_adv_handle.
 *                                            adv_event_prop specified by adv_handle has been configured as @ref LE_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_UNDIRECTED or
 *                                            or LE_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_DIRECTED.
 * @param[in]   periodic_adv_interval_min     Minimum advertising interval for periodic advertising.
                                              Range: 0x0006 to 0xFFFF
                                              Time = N * 1.25 ms
                                              Time Range: 7.5 ms to 81.91875 s
 * @param[in]   periodic_adv_interval_max     Maximum advertising interval for periodic advertising.
                                              Range: 0x0006 to 0xFFFF
                                              Time = N * 1.25 ms
                                              Time Range: 7.5 ms to 81.91875 s
 * @param[in]   periodic_adv_prop             A bit field that indicates which fields should be included in the advertising packet. @ref PA_ADV_PERIODIC_ADV_PROP.
 *
 * @return GAP operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others             GAP operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
     void test(void)
     {
          T_GAP_CAUSE cause = le_pa_adv_set_periodic_adv_param(adv_handle, periodic_adv_interval_min,
                                                              periodic_adv_interval_max, periodic_adv_prop);
     }
 * \endcode
 */
T_GAP_CAUSE le_pa_adv_set_periodic_adv_param(uint8_t adv_handle, uint16_t periodic_adv_interval_min,
                                             uint16_t periodic_adv_interval_max, uint16_t periodic_adv_prop);

/**
 * @brief       Set GAP periodic advertising data for the specified advertising set.
 *
 *              NOTE: This function just saves the pointer of the advertising data and will not copy the advertising data.
 *                    So application should use the const array or the global array to save the advertising data.
 *                    If application uses a dynamically requested buffer to save the advertising data, do not release it before
 *                    configuration of advertising data is completed.
 *
 * @param[in]   adv_handle               Used to identify an advertising set, which is assigned by @ref le_ext_adv_create_adv_handle.
 * @param[in]   periodic_adv_data_len    The length of periodic advertising data.
 *                                       NOTE: If periodic advertising state is @ref PA_ADV_STATE_WAIT_EXT_ADV_STATE_ADVERTISING or
 *                                             @ref PA_ADV_STATE_ADVERTISING, periodic_adv_data_len should have the range of values
 *                                             0 to 252.
 * @param[in]   p_periodic_adv_data      Pointer to periodic advertising data to write.
 * @param[in]   pa_unchanged_data_flag   Unchanged data (just update the Advertising DID of the periodic advertising).
 *                                       NOTE: If periodic advertising state is @ref PA_ADV_STATE_WAIT_EXT_ADV_STATE_ADVERTISING or
 *                                             @ref PA_ADV_STATE_ADVERTISING, and configuration of non-zero periodic advertising data
 *                                             has been completed, then pa_unchanged_data_flag could be configured as True or False.
 *                                             Otherwise, pa_unchanged_data_flag should be configured as False.
 *                                             If pa_unchanged_data_flag is True, periodic_adv_data_len should be zero.
 *
 * @return GAP operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others             GAP operation failure.
 *
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_pa_adv_set_periodic_adv_data(adv_handle, periodic_adv_data_len,
                                                            (uint8_t *)periodic_adv_data, pa_unchanged_data_flag);
    }
 * \endcode
 */
T_GAP_CAUSE le_pa_adv_set_periodic_adv_data(uint8_t adv_handle, uint16_t periodic_adv_data_len,
                                            uint8_t *p_periodic_adv_data, bool pa_unchanged_data_flag);

/**
 * @brief       Request the Controller to set periodic advertising related parameters for the specified advertising set.
 *
 *              NOTE: This function can be called after stack is ready.
 *              Explanation: If stack is ready, Application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
 *                           with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
 *
 *              If sending request operation is success, the result of setting periodic advertising related parameters will be returned by
 *              @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PA_ADV_START_SETTING.
 *
 * @param[in]   adv_handle        Used to identify an advertising set.
 * @param[in]   pa_update_flags   A bit field that indicates periodic advertising related parameters that shall be set. @ref PA_ADV_PARAM.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = GAP_CAUSE_SUCCESS;

        cause = le_pa_adv_set_periodic_adv_param(adv_handle, periodic_adv_interval_min, periodic_adv_interval_max, periodic_adv_prop);
        cause = le_pa_adv_set_periodic_adv_data(adv_handle, periodic_adv_data_len, (uint8_t *)periodic_adv_data, pa_unchanged_data_flag);

        // e.g. pa_update_flags @ref PA_ADV_PARAM. 0x03 indicates that periodic advertising parameters and periodic advertising data
        // will be configured.
        cause = le_pa_adv_start_setting(adv_handle, pa_update_flags);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case GAP_MSG_LE_PA_ADV_START_SETTING:
            APP_PRINT_INFO3("GAP_MSG_LE_PA_ADV_START_SETTING: cause 0x%x, adv_handle 0x%x, flag 0x%x",
                            p_data->p_le_pa_adv_start_setting_rsp->cause,
                            p_data->p_le_pa_adv_start_setting_rsp->adv_handle,
                            p_data->p_le_pa_adv_start_setting_rsp->flag);

            // If result is success, APP could use @ref le_pa_adv_set_periodic_adv_enable to enable periodic advertising.
            break;

        ......
        }
    }
 * \endcode
 */
T_GAP_CAUSE le_pa_adv_start_setting(uint8_t adv_handle, uint8_t pa_update_flags);

/**
 * @brief       Request the Controller to enable or disable the periodic advertising for the specified advertising set.
 *
 *              NOTE: This function can be called after stack is ready.
 *              Explanation: If stack is ready, Application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
 *                           with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
 *
 *              If sending request operation is success, the result of enable or disable periodic advertising will be returned by
 *              @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PA_ADV_SET_PERIODIC_ADV_ENABLE, periodic advertising state will
 *              be returned by @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PA_ADV_STATE_CHANGE_INFO.
 *
 * @param[in]   adv_handle        Used to identify an advertising set.
 * @param[in]   enable            A bit field that describes the enable parameters. @ref PA_ADV_ENABLE.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        // Result of GAP_MSG_LE_EXT_ADV_START_SETTING is success.
        T_GAP_CAUSE cause = le_pa_adv_set_periodic_adv_enable(adv_handle, enable)
    }

    void app_handle_pa_adv_state_evt(uint8_t adv_handle, T_GAP_PA_ADV_STATE new_state, uint16_t cause)
    {
        ......
        switch (new_state)
        {
        case PA_ADV_STATE_IDLE:
            {
            }
            break;

        case PA_ADV_STATE_START_EXT_ADV_STATE_IDLE:
            {
            }
            break;

        case PA_ADV_STATE_WAIT_EXT_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case PA_ADV_STATE_START:
            {
            }
            break;

        case PA_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case PA_ADV_STATE_STOP:
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

        .......
        switch (cb_type)
        {
        case GAP_MSG_LE_PA_ADV_SET_PERIODIC_ADV_ENABLE:
            APP_PRINT_INFO5("GAP_MSG_LE_PA_ADV_SET_PERIODIC_ADV_ENABLE: cause 0x%x, adv_handle 0x%x, value of enable param 0x%x, enable periodic advertising %d, include ADI %d",
                            p_data->p_le_pa_adv_set_periodic_adv_enable_rsp->cause,
                            p_data->p_le_pa_adv_set_periodic_adv_enable_rsp->adv_handle,
                            p_data->p_le_pa_adv_set_periodic_adv_enable_rsp->enable,
                            p_data->p_le_pa_adv_set_periodic_adv_enable_rsp->enable &
                            PA_ADV_ENABLE_ENABLE_PERIODIC_ADVERTISING,
                            p_data->p_le_pa_adv_set_periodic_adv_enable_rsp->enable & PA_ADV_ENABLE_INCLUDE_ADI);
            break;

        case GAP_MSG_LE_PA_ADV_STATE_CHANGE_INFO:
            {
                app_handle_pa_adv_state_evt(p_data->p_le_pa_adv_state_change_info->adv_handle,
                                            (T_GAP_PA_ADV_STATE)p_data->p_le_pa_adv_state_change_info->state,
                                            p_data->p_le_pa_adv_state_change_info->cause);
            }
            break;

        ......
        }
    }
 * \endcode
 */
T_GAP_CAUSE le_pa_adv_set_periodic_adv_enable(uint8_t adv_handle, uint8_t enable);

/** End of GAP_LE_PA_ADV_Exported_Functions
  * @}
  */

/** End of GAP_LE_PA_ADV
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

#endif /* GAP_PA_ADV_H */
