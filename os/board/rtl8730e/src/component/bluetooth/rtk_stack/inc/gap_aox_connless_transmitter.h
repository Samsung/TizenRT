/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_aox_connless_transmitter.h
* @brief     Head file for GAP AoA/AoD connectionless transmitter
* @details
* @author
* @date      2021-07-12
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_AOX_CONNLESS_TRANSMITTER_H
#define GAP_AOX_CONNLESS_TRANSMITTER_H

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

#if F_BT_LE_5_1_AOX_CONNLESS_TRANSMITTER_SUPPORT

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_AOX_CONNLESS_TRANSMITTER GAP LE AoA/AoD Connectionless Transmitter Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_AOX_CONNLESS_TRANSMITTER_Exported_Macros GAP LE AoA/AoD Connectionless Transmitter Exported Macros
  * @{
  */

/** @defgroup AOX_CONNLESS_TRANSMITTER_CTE_ENABLE AoA/AoD Connectionless Transmitter CTE Enable flag
  * @brief Use the macro definitions to describe the CTE enable parameters.
  * @{
  */
#define AOX_CONNLESS_TRANSMITTER_CTE_ENABLE_ADV_WITH_CTE_DISABLED      0x00   /**< Advertising with Constant Tone Extension is disabled (default). */
#define AOX_CONNLESS_TRANSMITTER_CTE_ENABLE_ADV_WITH_CTE_ENABLED       0x01   /**< Advertising with Constant Tone Extension is enabled. */
/** End of AOX_CONNLESS_TRANSMITTER_CTE_ENABLE
  * @}
  */
/** End of GAP_LE_AOX_CONNLESS_TRANSMITTER_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_AOX_CONNLESS_TRANSMITTER_Exported_Types GAP LE AoA/AoD Connectionless Transmitter Exported Types
  * @brief
  * @{
  */
typedef enum
{
    GAP_AOX_CONNLESS_TRANSMITTER_CTE_TYPES_AOA = 0x00,      /**< AoA Constant Tone Extension. */
    GAP_AOX_CONNLESS_TRANSMITTER_CTE_TYPES_AOD_1US = 0x01,  /**< AoD Constant Tone Extension with 1 ��s slots. */
    GAP_AOX_CONNLESS_TRANSMITTER_CTE_TYPES_AOD_2US = 0x02,  /**< AoD Constant Tone Extension with 2 ��s slots. */
} T_GAP_AOX_CONNLESS_TRANSMITTER_CTE_TYPES;

typedef enum
{
    AOX_CONNLESS_TRANSMITTER_STATE_IDLE = 0x00,                                       /**< Idle, not enable the use of Constant Tone Extensions. */
    AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_EXT_ADV_STATE_PA_ADV_STATE_IDLE = 0x01,   /**< Enable the use of Constant Tone Extensions in periodic
                                                                                           advertising when extended advertising and periodic
                                                                                           advertising of the specified advertising set are disabled.
                                                                                           A temporary state, haven't received the result. */
    AOX_CONNLESS_TRANSMITTER_STATE_WAIT_EXT_ADV_STATE_PA_ADV_STATE_ADVERTISING = 0x02,/**< Sending Periodic advertisements containing Constant Tone
                                                                                           Extensions is not started until the extended advertising
                                                                                           and extended advertising and periodic advertising
                                                                                           of the specified advertising set are enabled. */
    AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_PA_ADV_STATE_IDLE = 0x03,    /**< Enable the use of Constant Tone Extensions in periodic advertising
                                                                              when the periodic advertising of the specified advertising set is
                                                                              disabled.
                                                                              A temporary state, haven't received the result. */
    AOX_CONNLESS_TRANSMITTER_STATE_WAIT_PA_ADV_STATE_ADVERTISING = 0x04, /**< Sending Periodic advertisements containing Constant Tone
                                                                              Extensions is not started until the periodic advertising of
                                                                              the specified advertising set is enabled. */
    AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_EXT_ADV_STATE_IDLE = 0x05,   /**< Enable the use of Constant Tone Extensions in periodic advertising
                                                                              when the extended advertising of the specified advertising set is
                                                                              disabled.
                                                                              A temporary state, haven't received the result. */
    AOX_CONNLESS_TRANSMITTER_STATE_WAIT_EXT_ADV_STATE_ADVERTISING = 0x06,/**< Sending Periodic advertisements containing Constant Tone
                                                                              Extensions is not started until the extended advertising of
                                                                              the specified advertising set is enabled. */
    AOX_CONNLESS_TRANSMITTER_STATE_ENABLING = 0x07,    /**< Enable the use of Constant Tone Extensions in periodic advertising when extended
                                                            advertising and periodic advertising of the specified advertising set are enabled.
                                                            A temporary state, haven't received the result. */
    AOX_CONNLESS_TRANSMITTER_STATE_TRANSMITTING = 0x08,/**< Sending periodic advertisements containing Constant Tone Extensions. */
    AOX_CONNLESS_TRANSMITTER_STATE_DISABLING = 0x09,   /**< Disable the use of Constant Tone Extensions in periodic advertising.
                                                            A temporary state, haven't received the result. */
} T_GAP_AOX_CONNLESS_TRANSMITTER_STATE;
/** End of GAP_LE_AOX_CONNLESS_TRANSMITTER_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/

/** @defgroup GAP_LE_AOX_CONNLESS_TRANSMITTER_Exported_Functions GAP LE AoA/AoD Connectionless Transmitter Exported Functions
  * @brief
  * @{
  */
/**
 * @brief       Initialize the number of advertising sets for connectionless transmitter.
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
        T_GAP_CAUSE cause = le_aox_connless_transmitter_init(adv_set_num);
    }
 * \endcode
 */
T_GAP_CAUSE le_aox_connless_transmitter_init(uint8_t adv_set_num);

/**
 * @brief       Request the Controller to set parameters for the transmission of Constant Tone Extensions in periodic advertising.
 *
 *              NOTE: This function can be called after stack is ready.
 *              Explanation: If stack is ready, Application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
 *                           with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
 *
 *              If sending request operation is success, the result of setting parameters related parameters will be returned by
 *              @ref app_gap_aox_callback with cb_type @ref GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_SET_CTE_TRANSMIT_PARAMS.
 *
 * @param[in]   adv_handle                 Used to identify an advertising set.
 * @param[in]   cte_length                 Constant Tone Extension length in 8 ��s units.
 *                                         Range: 0x02 to max_cte_length supported by controller returned by
 *                                                @ref le_aox_read_antenna_information.
 * @param[in]   cte_type                   @ref T_GAP_AOX_CONNLESS_TRANSMITTER_CTE_TYPES.
 * @param[in]   cte_count                  The number of Constant Tone Extensions to transmit in each periodic advertising interval.
 *                                         Range: 0x01 to 0x10
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
        T_GAP_CAUSE cause = le_aox_connless_transmitter_set_cte_transmit_params(adv_handle, cte_length, cte_type, cte_count, switching_pattern_length, p_antenna_ids);
    }

    T_APP_RESULT app_gap_aox_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_AOX_CB_DATA *p_data = (T_LE_AOX_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_SET_CTE_TRANSMIT_PARAMS:
            {
                APP_PRINT_INFO2("GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_SET_CTE_TRANSMIT_PARAMS: cause 0x%x, adv_handle 0x%x",
                                p_data->p_le_aox_connless_transmitter_set_cte_transmit_params_rsp->cause,
                                p_data->p_le_aox_connless_transmitter_set_cte_transmit_params_rsp->adv_handle);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE le_aox_connless_transmitter_set_cte_transmit_params(uint8_t adv_handle,
                                                                uint8_t cte_length, T_GAP_AOX_CONNLESS_TRANSMITTER_CTE_TYPES cte_type, uint8_t cte_count,
                                                                uint8_t switching_pattern_length, uint8_t *p_antenna_ids);

/**
 * @brief       Request the Controller to enable or disable the use of Constant Tone Extensions in periodic advertising.
 *
 *              NOTE: This function can be called after stack is ready.
 *              Explanation: If stack is ready, Application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
 *                           with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
 *
 *              If sending request operation is success, the result of enabling or disabling will be returned by
 *              @ref app_gap_aox_callback with cb_type @ref GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_STATE_CHANGE_INFO.
 *
 * @param[in]   adv_handle  Identifier for the advertising set in which Constant Tone Extension is being enabled or disabled.
 * @param[in]   cte_enable  @ref AOX_CONNLESS_TRANSMITTER_CTE_ENABLE.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_aox_connless_transmitter_set_cte_transmit_enable(adv_handle, cte_enable);
    }

    void app_handle_aox_connless_transmitter_state_evt(uint8_t adv_handle,
                                                      T_GAP_AOX_CONNLESS_TRANSMITTER_STATE state, uint16_t cause)
    {
        ......
        switch (state)
        {
        case AOX_CONNLESS_TRANSMITTER_STATE_IDLE:
            {
            }
            break;

        case AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_EXT_ADV_STATE_PA_ADV_STATE_IDLE:
            {
            }
            break;

        case AOX_CONNLESS_TRANSMITTER_STATE_WAIT_EXT_ADV_STATE_PA_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_PA_ADV_STATE_IDLE:
            {
            }
            break;

        case AOX_CONNLESS_TRANSMITTER_STATE_WAIT_PA_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_EXT_ADV_STATE_IDLE:
            {
            }
            break;

        case AOX_CONNLESS_TRANSMITTER_STATE_WAIT_EXT_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case AOX_CONNLESS_TRANSMITTER_STATE_ENABLING:
            {
            }
            break;

        case AOX_CONNLESS_TRANSMITTER_STATE_TRANSMITTING:
            {
            }
            break;

        case AOX_CONNLESS_TRANSMITTER_STATE_DISABLING:
            {
            }
            break;
        ......
    }

    T_APP_RESULT app_gap_aox_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_AOX_CB_DATA *p_data = (T_LE_AOX_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_STATE_CHANGE_INFO:
            {
                app_handle_aox_connless_transmitter_state_evt(
                    p_data->p_le_aox_connless_transmitter_state_change_info->adv_handle,
                    (T_GAP_AOX_CONNLESS_TRANSMITTER_STATE)
                    p_data->p_le_aox_connless_transmitter_state_change_info->state,
                    p_data->p_le_aox_connless_transmitter_state_change_info->cause);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE le_aox_connless_transmitter_set_cte_transmit_enable(uint8_t adv_handle,
                                                                uint8_t cte_enable);

/** End of GAP_LE_AOX_CONNLESS_TRANSMITTER_Exported_Functions
  * @}
  */

/** End of GAP_LE_AOX_CONNLESS_TRANSMITTER
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

#endif /* GAP_AOX_CONNLESS_TRANSMITTER_H */
