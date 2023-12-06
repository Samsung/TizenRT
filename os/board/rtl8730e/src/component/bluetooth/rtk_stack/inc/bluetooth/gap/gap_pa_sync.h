/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_pa_sync.h
* @brief     Header file for GAP PA sync
* @details
* @author
* @date      2021-07-12
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_PA_SYNC_H
#define GAP_PA_SYNC_H

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#if F_BT_LE_5_0_PA_SYNC_SUPPORT
#include "upperstack_config.h"
#include "gap_le.h"

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_PA_SYNC GAP LE PA Sync Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_PA_SYNC_Exported_Macros GAP LE PA Sync Exported Macros
  * @{
  */

/** @defgroup PA_SYNC_INVALID_PARAM PA Synchronization invalid parameters
  * @brief    Use macro definitions to indicate that synchronization parameters are
  *           invalid in certain scenarios.
  *           e.g. sync_handle is invalid before device is synchronized to a periodic advertising train.
  * @{
  */
#define GAP_INVALID_SYNC_ID      0xFF    /**< A sync_id value of 0xFF means invalid. */
#define GAP_INVALID_SYNC_HANDLE  0xFFFF  /**< A sync_handle value of 0xFFFF means invalid. */
/** End of PA_SYNC_INVALID_PARAM
  * @}
  */

/** @defgroup PA_SYNC_CREATE_SYNC_OPTIONS Synchronization State of Periodic Advertising Create Sync Options
  * @brief    Use the combination of macro definitions to determine whether the Periodic Advertiser List is used,
  *           whether periodic advertising report for this periodic advertising train are initially enabled or disabled,
  *           and whether duplicate reports are filtered or not.
  * @{
  */
#define PA_SYNC_CREATE_SYNC_OPTIONS_USE_PERIODIC_ADV_LIST               0x01   /**< Use the Periodic Advertiser List to determine which advertiser to listen to.
                                                                                    Otherwise, use the Advertising_SID, Advertiser_Address_Type, and
                                                                                    Advertiser_Address parameters to determine which advertiser to listen to.*/
#define PA_SYNC_CREATE_SYNC_OPTIONS_REPORT_INITIALLY_DISABLED           0x02   /**< Reporting initially disabled.
                                                                                    Otherwise, Reporting initially enabled.*/
#define PA_SYNC_CREATE_SYNC_OPTIONS_DUPLICATE_FILTER_INITIALLY_ENABLED  0x04   /**< Duplicate filtering initially enabled.
                                                                                    Otherwise, Duplicate filtering initially disabled.*/
/** End of PA_SYNC_CREATE_SYNC_OPTIONS
  * @}
  */

/** @defgroup PA_SYNC_CREATE_SYNC_CTE_TYPE Synchronization State of Periodic Advertising Create Sync CTE Type
  * @brief    Use the combination of macro definitions to specify whether to only synchronize to
  *           periodic advertising with certain types of Constant Tone Extension (a value of 0
  *           indicates that the presence or absence of a Constant Tone Extension is irrelevant).
  * @{
  */
#define PA_SYNC_CREATE_SYNC_CTE_TYPE_CTE_IRRELEVANT             0x00   /**< A value of 0 (i.e. all bits clear) indicates that the presence or absence of
                                                                            a Constant Tone Extension is irrelevant. */

#define PA_SYNC_CREATE_SYNC_CTE_TYPE_NOT_SYNC_WITH_AOA_CTE      0x01   /**< Do not sync to packets with an AoA Constant Tone Extension. */
#define PA_SYNC_CREATE_SYNC_CTE_TYPE_NOT_SYNC_WITH_AOD_CTE_1US  0x02   /**< Do not sync to packets with an AoD Constant Tone Extension with 1 ��s slots. */
#define PA_SYNC_CREATE_SYNC_CTE_TYPE_NOT_SYNC_WITH_AOD_CTE_2US  0x04   /**< Do not sync to packets with an AoD Constant Tone Extension with 2 ��s slots. */
#define PA_SYNC_CREATE_SYNC_CTE_TYPE_NOT_SYNC_WITH_TYPE_3_CTE   0x08   /**< Do not sync to packets with a type 3 Constant Tone Extension (currently reserved for future use). */
#define PA_SYNC_CREATE_SYNC_CTE_TYPE_NOT_SYNC_WITHOUT_CTE       0x10   /**< Do not sync to packets without a Constant Tone Extension*/
/** End of PA_SYNC_CREATE_SYNC_CTE_TYPE
  * @}
  */

#if ((F_BT_LE_5_0_PA_SYNC_SUPPORT && F_BT_LE_5_1_SUPPORT) || F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
/** @defgroup PA_SYNC_PA_RECEIVE_ENABLE_PARAM Enable parameter of Periodic Advertising Receive Enable
  * @brief    Use the combination of macro definitions to determine whether reporting and duplicate filtering
  *           are enabled or disabled.
  * @{
  */
#define PA_SYNC_PA_RECEIVE_ENABLE_PARAM_REPORT_ENABLED            0x01   /**< Reporting enabled */
#define PA_SYNC_PA_RECEIVE_ENABLE_PARAM_DUPLICATE_FILTER_ENABLED  0x02   /**< Duplicate filtering enabled*/
/** End of PA_SYNC_PA_RECEIVE_ENABLE_PARAM
  * @}
  */
#endif

/** @defgroup GAP_PA_TERMINATE_SYNC_DEV_STATE GAP PA Terminate Sync Device State
  * @{
  */
#define GAP_PA_TERMINATE_SYNC_DEV_STATE_IDLE           0   /**< Idle */
#define GAP_PA_TERMINATE_SYNC_DEV_STATE_TERMINATING    1   /**< Terminating */
/** End of GAP_PA_TERMINATE_SYNC_DEV_STATE
  * @}
  */

#if F_BT_LE_5_0_PA_SYNC_SCAN_SUPPORT
/** @defgroup GAP_PA_CREATE_SYNC_DEV_STATE GAP PA Create Sync Device State
  * @{
  */
#define GAP_PA_CREATE_SYNC_DEV_STATE_IDLE              0   /**< Idle */
#define GAP_PA_CREATE_SYNC_DEV_STATE_SYNCHRONIZING     1   /**< Synchronizing */
/** End of GAP_PA_CREATE_SYNC_DEV_STATE
  * @}
  */
#endif

#if ((F_BT_LE_5_0_PA_SYNC_SUPPORT && F_BT_LE_5_1_SUPPORT) || F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
/** @defgroup GAP_PA_RECEIVE_ENABLE_DEV_STATE GAP PA Receive Enable Device State
  * @{
  */
#define GAP_PA_RECEIVE_ENABLE_DEV_STATE_IDLE              0   /**< Idle */
#define GAP_PA_RECEIVE_ENABLE_DEV_STATE_ENABLING          1   /**< Enabling */
/** End of GAP_PA_RECEIVE_ENABLE_DEV_STATE
  * @}
  */
#endif
/** End of GAP_LE_PA_SYNC_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_PA_SYNC_Exported_Types GAP LE PA Sync Exported Types
  * @{
  */

/** @brief GAP PA synchronization states. */
typedef enum
{
    GAP_PA_SYNC_STATE_TERMINATED = 0x00,                   /**< Terminated. */
#if F_BT_LE_5_0_PA_SYNC_SCAN_SUPPORT
    GAP_PA_SYNC_STATE_SYNCHRONIZING_SCAN_IDLE = 0x01,      /**< Start synchronizing when extended scanning is disabled. A temporary state, haven't received the result. */
    GAP_PA_SYNC_STATE_SYNCHRONIZING_WAIT_SCANNING = 0x02,  /**< No attempt to synchronize will take place while extended scanning is disabled. */
#endif
    GAP_PA_SYNC_STATE_SYNCHRONIZING = 0x03,                /**< Start synchronizing when extended scanning is enabled. */
    GAP_PA_SYNC_STATE_SYNCHRONIZED = 0x04,                 /**< Synchronized. */
    GAP_PA_SYNC_STATE_TERMINATING = 0x05,                  /**< Terminate synchronization. A temporary state, haven't received the result. */
} T_GAP_PA_SYNC_STATE;

/** @brief GAP PA sync parameter types. */
typedef enum
{
    PA_SYNC_PARAM_PERIODIC_ADV_LIST_SIZE = 0x2A0,          /**< Periodic advertiser list size. Read only. */
    PA_SYNC_PARAM_SYNCHRONIZED_PARAM     = 0x2A1,          /**< PA synchronized parameters. Read only. Identifier is sync_id. */
    PA_SYNC_PARAM_DEV_STATE              = 0x2A2,          /**< PA synchronization device state. Read only. */
} T_GAP_PA_SYNC_PARAM_TYPE;

/** @brief  Definition of GAP PA Sync Common Sync parameter. */
typedef struct
{
    uint16_t         sync_handle;  /**< Sync_Handle identifying the periodic advertising train. */
    uint8_t          adv_sid;      /**< Value of the Advertising SID subfield in the ADI field
                                        of the PDU. */
    uint8_t          adv_addr_type;/**< Advertiser address type. @ref T_GAP_PA_SYNC_ADV_ADDR_TYPE
                                        for reception of info. */
    uint8_t          adv_addr[GAP_BD_ADDR_LEN];/**< Public Device Address, Random Device Address,
                                                    Public Identity Address, or Random (static)
                                                    Identity Address of the advertiser. */
    uint16_t         skip;              /**< Only for sync_transfer_received_flag that is False. */
    uint16_t         sync_timeout;      /**< Only for sync_transfer_received_flag that is False. */
    uint8_t          sync_cte_type;     /**< Only for sync_transfer_received_flag that is False. */
    T_GAP_PHYS_TYPE  adv_phy;           /**< Advertiser_PHY specifies the PHY used for the periodic
                                             advertising. */
    uint8_t          adv_clock_accuracy;/**< Advertiser_Clock_Accuracy specifies the accuracy of
                                             the periodic advertiser's clock.
                                             0x00: 500 ppm
                                             0x01: 250 ppm
                                             0x02: 150 ppm
                                             0x03: 100 ppm
                                             0x04: 75  ppm
                                             0x05: 50  ppm
                                             0x06: 30  ppm
                                             0x07: 20  ppm */
    uint16_t         periodic_adv_interval;      /**< Periodic advertising interval.
                                                      Range: 0x0006 to 0xFFFF
                                                      Time = N * 1.25 ms
                                                      Time Range: 7.5 ms to 81.91875 s */
    bool             sync_transfer_received_flag;/**< If successfully synchronized to the periodic
                                                      advertising train,
                                                      False: Synchronization is established by
                                                             @ref le_pa_sync_create_sync.
                                                      True:  Synchronization is received by
                                                             @ref le_past_recipient_set_default_periodic_adv_sync_transfer_params
                                                             or @ref le_past_recipient_set_periodic_adv_sync_transfer_params. */
} T_GAP_PA_SYNC_COMMON_SYNC_PARAM;

#if F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT
/** @brief  Definition of GAP PAST Sync Transfer Received parameter. */
typedef struct
{
    uint8_t         conn_id;                   /**< Identify a connection. */
    uint16_t        service_data;              /**< A value provided by the peer device. */
} T_GAP_PAST_SYNC_TRANSFER_RECEIVED_PARAM;
#endif

#if F_BT_LE_5_0_PA_SYNC_SCAN_SUPPORT
/** @brief  Definition of GAP PA Sync create sync parameter.*/
typedef struct
{
    uint8_t options;                            /**< @ref PA_SYNC_CREATE_SYNC_OPTIONS */
    uint8_t sync_cte_type;                      /**< @ref PA_SYNC_CREATE_SYNC_CTE_TYPE */
    uint8_t adv_sid;                            /**< If Periodic Advertiser List is not used (@ref PA_SYNC_CREATE_SYNC_OPTIONS),
                                                    Advertising SID subfield in the ADI field used to identify the Periodic Advertising. */
    T_GAP_PA_SYNC_ADV_ADDR_TYPE adv_addr_type;  /**< If Periodic Advertiser List is not used
                                                     (@ref PA_SYNC_CREATE_SYNC_OPTIONS),
                                                     only @ref PA_SYNC_ADV_ADDR_PUBLIC and
                                                     @ref PA_SYNC_ADV_ADDR_RANDOM could be
                                                     used for creating sync. */
    uint8_t adv_addr[GAP_BD_ADDR_LEN];          /**< If Periodic Advertiser List is not used (@ref PA_SYNC_CREATE_SYNC_OPTIONS),
                                                     Public Device Address, Random Device Address, Public Identity Address, or Random (static) Identity Address of the advertiser */
    uint16_t skip;                              /**< The maximum number of periodic advertising events that can be skipped after a successful receive.
                                                     Range: 0x0000 to 0x01F3 */
    uint16_t sync_timeout;                      /**< Synchronization timeout for the periodic advertising train.
                                                     Range: 0x000A to 0x4000
                                                     Time = N*10 ms
                                                     Time Range: 100 ms to 163.84 s */
} T_GAP_PA_SYNC_CREATE_SYNC_PARAM;
#endif
/** End of GAP_LE_PA_SYNC_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_PA_SYNC_Exported_Functions GAP LE PA Sync Exported Functions
  * @brief
  * @{
  */
/**
 * @brief       Initialize the number of sync handle for synchronization state in periodic advertising.
 *
 *              NOTE: If PA synchronization state will be used, @ref le_ext_scan_gap_msg_info_way(false) should be invoked.
 *
 * @param[in]   sync_handle_num Sync handle number.
 * @return GAP Operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval GAP_CAUSE_INVALID_PARAM Operation failure, invalid parameter.
 * @retval GAP_CAUSE_NO_RESOURCE   Operation failure, memory acquisition failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_pa_sync_init(sync_handle_num);
    }
 * \endcode
 */
T_GAP_CAUSE le_pa_sync_init(uint8_t sync_handle_num);

/**
 * @brief       Get GAP PA sync parameters.
 *
 * @param[in]      param     GAP PA sync parameter types. @ref T_GAP_PA_SYNC_PARAM_TYPE
 * @param[in,out]  p_value   Pointer to location to get the parameter value. This is dependent on the parameter type, and will be cast to the appropriate
 *                           data type (e.g. if data type of param is uint16_t, p_value will be cast to pointer of uint16_t).
 * @param[in]      sync_id   Identify the periodic advertising train.
 *                           NOTE: If param is @ref PA_SYNC_PARAM_PERIODIC_ADV_LIST_SIZE or @ref PA_SYNC_PARAM_DEV_STATE,
 *                                 sync_id is irrelevant.
 *
 * @return GAP operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others             GAP operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
     void test(void)
     {
          uint8_t periodic_adv_list_size = 0;
          T_GAP_CAUSE cause = le_pa_sync_get_param(PA_SYNC_PARAM_PERIODIC_ADV_LIST_SIZE, &periodic_adv_list_size, 0xFF);
     }

     void app_handle_pa_sync_state_evt()
     {
          ......
          case GAP_PA_SYNC_STATE_SYNCHRONIZED:
          {
            T_GAP_PA_SYNC_COMMON_SYNC_PARAM common_sync_param;

            if (le_pa_sync_get_param(PA_SYNC_PARAM_SYNCHRONIZED_PARAM, &common_sync_param, sync_id) == GAP_CAUSE_SUCCESS)
            ......
     }
 * \endcode
 */
T_GAP_CAUSE le_pa_sync_get_param(T_GAP_PA_SYNC_PARAM_TYPE param, void *p_value, uint8_t sync_id);

/**
 * @brief       Stop reception of the periodic advertising train, or cancel the synchronization creation while it is pending.
 *
 *              If sending request operation is success, the result of stop reception or cancel pending synchronization creation will be returned by
 *              @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PA_SYNC_DEV_STATE_CHANGE_INFO, GAP PA synchronization states will
 *              be returned by @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PA_SYNC_STATE_CHANGE_INFO.
 *
 * @param[in]      sync_id   Identify the periodic advertising train.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_pa_sync_terminate_sync(sync_id);
    }

    void app_handle_pa_sync_dev_state_evt(T_GAP_PA_SYNC_DEV_STATE new_state, uint16_t cause)
    {
        ......
        if (app_pa_sync_dev_state.gap_terminate_sync_state != new_state.gap_terminate_sync_state)
        {
            if (new_state.gap_terminate_sync_state == GAP_PA_TERMINATE_SYNC_DEV_STATE_IDLE)
            {
            }
            else if (new_state.gap_terminate_sync_state == GAP_PA_TERMINATE_SYNC_DEV_STATE_TERMINATING)
            {
            }
        }

        if (app_pa_sync_dev_state.gap_create_sync_state != new_state.gap_create_sync_state)
        {
            if (new_state.gap_create_sync_state == GAP_PA_CREATE_SYNC_DEV_STATE_IDLE)
            {
            }
            else if (new_state.gap_create_sync_state == GAP_PA_CREATE_SYNC_DEV_STATE_SYNCHRONIZING)
            {
            }
        }
        ......
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

        case GAP_PA_SYNC_STATE_SYNCHRONIZING_SCAN_IDLE:
            {
            }
            break;

        case GAP_PA_SYNC_STATE_SYNCHRONIZING_WAIT_SCANNING:
            {
            }
            break;

        case GAP_PA_SYNC_STATE_SYNCHRONIZING:
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
        case GAP_MSG_LE_PA_SYNC_DEV_STATE_CHANGE_INFO:
            {
                app_handle_pa_sync_dev_state_evt(p_data->p_le_pa_sync_dev_state_change_info->state,
                                                p_data->p_le_pa_sync_dev_state_change_info->cause);
            }
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
        ......
    }
 * \endcode
 */
T_GAP_CAUSE le_pa_sync_terminate_sync(uint8_t sync_id);

/**
 * @brief       Modify Periodic Advertiser list.
 *
 *              NOTE: This function to add, remove or clear Periodic Advertiser list can be called after stack is ready.
 *              Explanation: If stack is ready, Application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
 *                           with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
 *
 *              If sending request operation is success, the result of modifying Periodic Advertiser list will be returned by
 *              @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PA_SYNC_MODIFY_PERIODIC_ADV_LIST.
 *
 * @param[in]   operation      Add entry to Periodic Advertiser list, remove entry from Periodic Advertiser list or clear all entries
 *                             from Periodic Advertiser list. @ref T_GAP_PA_SYNC_PERIODIC_ADV_LIST_OP.
 *                             NOTE: If operation is @ref GAP_PA_SYNC_PERIODIC_ADV_LIST_OP_CLEAR, adv_addr, adv_addr_type and adv_sid are irrelevant.
 * @param[in]   adv_addr       Public Device Address, Random Device Address, Public Identity Address, or Random (static) Identity Address
 *                             of the advertiser.
 * @param[in]   adv_addr_type  Only @ref PA_SYNC_ADV_ADDR_PUBLIC and @ref PA_SYNC_ADV_ADDR_RANDOM could be used for modifying Periodic Advertiser list.
 * @param[in]   adv_sid        Advertising SID subfield in the ADI field used to identify the Periodic Advertising.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_pa_sync_modify_periodic_adv_list(operation, adv_addr, adv_addr_type, adv_sid);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case GAP_MSG_LE_PA_SYNC_MODIFY_PERIODIC_ADV_LIST:
            APP_PRINT_INFO2("GAP_MSG_LE_PA_SYNC_MODIFY_PERIODIC_ADV_LIST: operation %d, cause 0x%x",
                            p_data->p_le_pa_sync_modify_periodic_adv_list_rsp->operation,
                            p_data->p_le_pa_sync_modify_periodic_adv_list_rsp->cause);
            break;
        ......
        }
    }
 * \endcode
 */
T_GAP_CAUSE le_pa_sync_modify_periodic_adv_list(T_GAP_PA_SYNC_PERIODIC_ADV_LIST_OP operation,
                                                uint8_t *adv_addr,
                                                T_GAP_PA_SYNC_ADV_ADDR_TYPE adv_addr_type, uint8_t adv_sid);

#if F_BT_LE_5_0_PA_SYNC_SCAN_SUPPORT
/**
 * @brief       Synchronize with a periodic advertising train from an advertiser.
 *
 *              NOTE: This function can be called after stack is ready.
 *              Explanation: If stack is ready, Application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
 *                           with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
 *
 *              If sending request operation is success, the result of synchronization creation will be returned by
 *              @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PA_SYNC_DEV_STATE_CHANGE_INFO, GAP PA synchronization states will
 *              be returned by @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PA_SYNC_STATE_CHANGE_INFO.
 *              Periodic advertisement will be returned by @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO.
 *
 * @param[in]      p_pa_sync_create_sync_param     GAP PA Sync create sync parameter. @ref T_GAP_PA_SYNC_CREATE_SYNC_PARAM.
 * @param[in,out]  p_sync_id                       Identify the periodic advertising train.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_PA_SYNC_CREATE_SYNC_PARAM pa_create_sync_param;
        memset(&pa_create_sync_param, 0, sizeof(T_GAP_PA_SYNC_CREATE_SYNC_PARAM));
        uint8_t sync_id = GAP_INVALID_SYNC_ID;

        ......
        // Initialization of pa_create_sync_param is dependent on Application.

        T_GAP_CAUSE cause = le_pa_sync_create_sync(&pa_create_sync_param, &sync_id);
    }

    void app_handle_pa_sync_dev_state_evt(T_GAP_PA_SYNC_DEV_STATE new_state, uint16_t cause)
    {
        ......
        if (app_pa_sync_dev_state.gap_create_sync_state != new_state.gap_create_sync_state)
        {
            if (new_state.gap_create_sync_state == GAP_PA_CREATE_SYNC_DEV_STATE_IDLE)
            {
            }
            else if (new_state.gap_create_sync_state == GAP_PA_CREATE_SYNC_DEV_STATE_SYNCHRONIZING)
            {
            }
        }
        ......
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

        case GAP_PA_SYNC_STATE_SYNCHRONIZING_SCAN_IDLE:
            {
            }
            break;

        case GAP_PA_SYNC_STATE_SYNCHRONIZING_WAIT_SCANNING:
            {
            }
            break;

        case GAP_PA_SYNC_STATE_SYNCHRONIZING:
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
        case GAP_MSG_LE_PA_SYNC_DEV_STATE_CHANGE_INFO:
            {
                app_handle_pa_sync_dev_state_evt(p_data->p_le_pa_sync_dev_state_change_info->state,
                                                p_data->p_le_pa_sync_dev_state_change_info->cause);
            }
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
T_GAP_CAUSE le_pa_sync_create_sync(T_GAP_PA_SYNC_CREATE_SYNC_PARAM *p_pa_sync_create_sync_param,
                                   uint8_t *p_sync_id);
#endif

#if ((F_BT_LE_5_0_PA_SYNC_SUPPORT && F_BT_LE_5_1_SUPPORT) || F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
/**
 * @brief       Enable or disable reports for specified periodic advertising train.
 *
 *              If sending request operation is success, the result enable or disable reports will be returned by
 *              @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PA_SYNC_DEV_STATE_CHANGE_INFO,
 *              @ref GAP_MSG_LE_PA_SYNC_SET_PERIODIC_ADV_RECEIVE_ENABLE.
 *              Periodic advertisement will be returned by @ref app_gap_callback with cb_type @ref GAP_MSG_LE_PERIODIC_ADV_REPORT_INFO.
 *
 * @param[in]  sync_id    Identify the periodic advertising train.
 * @param[in]  enable     @ref PA_SYNC_PA_RECEIVE_ENABLE_PARAM.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_pa_sync_set_periodic_adv_receive_enable(sync_id, enable);
    }

    void app_handle_pa_sync_dev_state_evt(T_GAP_PA_SYNC_DEV_STATE new_state, uint16_t cause)
    {
        ......
        if (app_pa_sync_dev_state.gap_periodic_adv_receive_enable_state !=
            new_state.gap_periodic_adv_receive_enable_state)
        {
            if (new_state.gap_periodic_adv_receive_enable_state == GAP_PA_RECEIVE_ENABLE_DEV_STATE_IDLE)
            {
            }
            else if (new_state.gap_periodic_adv_receive_enable_state ==
                    GAP_PA_RECEIVE_ENABLE_DEV_STATE_ENABLING)
            {
            }
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
        case GAP_MSG_LE_PA_SYNC_DEV_STATE_CHANGE_INFO:
            {
                app_handle_pa_sync_dev_state_evt(p_data->p_le_pa_sync_dev_state_change_info->state,
                                                p_data->p_le_pa_sync_dev_state_change_info->cause);
            }
            break;

        case GAP_MSG_LE_PA_SYNC_SET_PERIODIC_ADV_RECEIVE_ENABLE:
            APP_PRINT_INFO6("GAP_MSG_LE_PA_SYNC_SET_PERIODIC_ADV_RECEIVE_ENABLE: cause 0x%x, sync_id %d, sync_handle 0x%x, value of enable param 0x%x, reporting enabled %d, duplicate filtering enabled %d",
                            p_data->p_le_pa_set_periodic_adv_receive_enable_rsp->cause,
                            p_data->p_le_pa_set_periodic_adv_receive_enable_rsp->sync_id,
                            p_data->p_le_pa_set_periodic_adv_receive_enable_rsp->sync_handle,
                            p_data->p_le_pa_set_periodic_adv_receive_enable_rsp->enable,
                            p_data->p_le_pa_set_periodic_adv_receive_enable_rsp->enable &
                            PA_SYNC_PA_RECEIVE_ENABLE_PARAM_REPORT_ENABLED,
                            p_data->p_le_pa_set_periodic_adv_receive_enable_rsp->enable &
                            PA_SYNC_PA_RECEIVE_ENABLE_PARAM_DUPLICATE_FILTER_ENABLED);
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
T_GAP_CAUSE le_pa_sync_set_periodic_adv_receive_enable(uint8_t sync_id, uint8_t enable);
#endif
#endif

/** End of GAP_LE_PA_SYNC_Exported_Functions
  * @}
  */

/** End of GAP_LE_PA_SYNC
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

#endif /* GAP_PA_SYNC_H */
