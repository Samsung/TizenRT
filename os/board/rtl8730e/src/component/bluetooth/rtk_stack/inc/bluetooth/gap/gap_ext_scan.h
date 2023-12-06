/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_ext_scan.h
* @brief     Header file for Gap ext scan
* @details
* @author    jane
* @date      2016-02-18
* @version   v1.0
* *********************************************************************************************************
*/

/* Define to prevent recursive inclusion **/
#ifndef GAP_EXT_SCAN_H
#define GAP_EXT_SCAN_H

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#ifdef __cplusplus
extern "C"
{
#endif

#include "upperstack_config.h"
#include "gap_le.h"
#if F_BT_LE_5_0_AE_SCAN_SUPPORT

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_EXTENDED_SCAN GAP LE Extended Scan Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_EXTENDED_SCAN_Exported_Macros GAP LE Extended Scan Exported Macros
  * @brief
  * @{
  */

#define GAP_EXT_SCAN_MAX_PHYS_NUM               2     /**< Maximum number of scan PHYs. */

/** @defgroup EXT_SCAN_PHY Extended Scan PHY
  * @brief Indicate the PHY(s) on which the advertising packets should be received
           on the primary advertising channel. LE 1M PHY and LE Coded PHY are primary
           advertising channel.
  * @{
  */
#define GAP_EXT_SCAN_PHYS_1M_BIT                0x01   /**< Scan advertisements on the LE 1M PHY. */
#define GAP_EXT_SCAN_PHYS_CODED_BIT             0x04   /**< Scan advertisements on the LE Coded PHY. */
#define GAP_EXT_SCAN_PHYS_ALL                   0x05   /**< Scan advertisements on the LE 1M PHY and LE Coded PHY. */
/** End of EXT_SCAN_PHY
  * @}
  */

/** @defgroup EXT_ADV_REPORT Extended Advertising Report flag
  * @brief Indicate properties of advertising event in extended advertising report.
           When a scan response is received, bits 0-2 and 4 of event type shall
           indicate the properties of the original advertising event.
  * @{
  */
#define GAP_EXT_ADV_REPORT_BIT_CONNECTABLE_ADV  0x01   /**< Connectable advertising. */
#define GAP_EXT_ADV_REPORT_BIT_SCANNABLE_ADV    0x02   /**< Scannable advertising. */
#define GAP_EXT_ADV_REPORT_BIT_DIRECTED_ADV     0x04   /**< Directed advertising. */
#define GAP_EXT_ADV_REPORT_BIT_SCAN_RESPONSE    0x08   /**< Scan response. */
#define GAP_EXT_ADV_REPORT_BIT_USE_LEGACY_ADV   0x10   /**< Legacy advertising PDUs used. */
/** End of EXT_ADV_REPORT
  * @}
  */

/** End of GAP_LE_EXTENDED_SCAN_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_EXTENDED_SCAN_Exported_Types GAP LE Extended Scan Exported Types
  * @brief
  * @{
  */
/** @brief Scan PHY type used in @ref le_ext_scan_set_phy_param. */
typedef enum
{
    LE_SCAN_PHY_LE_1M,     /**< Set @ref T_GAP_LE_EXT_SCAN_PARAM for LE 1M PHY. */
    LE_SCAN_PHY_LE_CODED,  /**< Set @ref T_GAP_LE_EXT_SCAN_PARAM for LE Coded PHY. */
} T_LE_EXT_SCAN_PHY_TYPE;

/** @brief Extended scan parameter. */
typedef struct
{
    T_GAP_SCAN_MODE
    scan_type;      /**< Scan type. Write only. Default is GAP_SCAN_MODE_ACTIVE, @ref T_GAP_SCAN_MODE. */
    uint16_t
    scan_interval;  /**< Scan Interval. Write only. In units of 0.625ms, range: 0x0004 to 0xFFFF. Default is 0x40. */
    uint16_t
    scan_window;    /**< Scan Window. Write only. In units of 0.625ms, range: 0x0004 to 0xFFFF. Default is 0x20. */
} T_GAP_LE_EXT_SCAN_PARAM;

/** @brief Extended scan parameter type. */
typedef enum
{
    GAP_PARAM_EXT_SCAN_LOCAL_ADDR_TYPE   = 0x340,  /**< The type of address being used in the scan request packets. Read/Write. Size is uint8_t. Default is GAP_LOCAL_ADDR_LE_PUBLIC (@ref T_GAP_LOCAL_ADDR_TYPE). */
    GAP_PARAM_EXT_SCAN_FILTER_POLICY     = 0x341,  /**< Scanning filter policy. Read/Write. Size is uint8_t. Default is GAP_SCAN_FILTER_ANY (@ref T_GAP_SCAN_FILTER_POLICY). */
    GAP_PARAM_EXT_SCAN_PHYS              = 0x342,  /**< Scanning PHYs.  window. Read/Write. Size is uint8_t. Default is GAP_EXT_SCAN_PHYS_1M_BIT (@ref EXT_SCAN_PHY). */
    GAP_PARAM_EXT_SCAN_FILTER_DUPLICATES = 0x343,  /**< Scan Filter Duplicates. Read/Write. Size is uint8_t. Default is GAP_SCAN_FILTER_DUPLICATE_ENABLE (@ref T_GAP_SCAN_FILTER_DUPLICATE). */
    GAP_PARAM_EXT_SCAN_DURATION          = 0x344,  /**< Scan duration. Read/Write. Size is uint16_t. In units of 10ms, range: 0x0000~0xFFFF. Default is zero. */
    GAP_PARAM_EXT_SCAN_PERIOD            = 0x345,  /**< Scan period. Read/Write. Size is uint16_t. In units of 1.28sec, range: 0x0000~0xFFFF. Default is zero. */
} T_LE_EXT_SCAN_PARAM_TYPE;

/** End of GAP_LE_EXTENDED_SCAN_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_EXTENDED_SCAN_Exported_Functions GAP LE Extended Scan Exported Functions
  * @brief
  * @{
  */
#if F_BT_LE_GAP_SCAN_FILTER_SUPPORT
/**
  * @brief   Set extended scan information filter.
  *
  *          NOTE: This function can be called before @ref gap_start_bt_stack is invoked.
  *
  * @param[in]  enable      Whether to open the extend scan info filter function.
  * @param[in]  report_type A logical OR of the bit values that provide the list of report type to receive.
  * @param[in]  legacy_adv  Whether to receive the legacy scan info.
  * @return bool.
  * @retval  TRUE  Operation success.
  * @retval  FALSE Operation Failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        le_ext_scan_report_filter(true,
            GAP_EXT_ADV_REPORT_BIT_CONNECTABLE_ADV|GAP_EXT_ADV_REPORT_BIT_SCANNABLE_ADV, false);
    }
  * \endcode
  */
bool le_ext_scan_report_filter(bool enable, uint16_t report_type, bool legacy_adv);
#endif

/**
 * @brief       Set a GAP extended scan parameter.
 *
 *              NOTE: You can call this function with a extended scan parameter type and it will set the
 *              extended scan parameter. Extended scan parameters are defined in @ref T_LE_EXT_SCAN_PARAM_TYPE.
 *              If the "len" field sets to the size of a "uint16_t" ,the "p_value" field must
 *              point to a data with type "uint16".
 *              Combination of Duration parameter (@ref GAP_PARAM_EXT_SCAN_DURATION) and
 *              Period parameter(@ref GAP_PARAM_EXT_SCAN_PERIOD) determines scan mode,
 *              please refer to below code about scan_mode.
 *
 * @param[in]   param    Extended scan parameter type: @ref T_LE_EXT_SCAN_PARAM_TYPE
 * @param[in]   len      Length of data to write.
 * @param[in]   p_value  Pointer to data to write. This is dependent on the parameter type and will
                         be cast to the appropriate data type (For example: if data type param is uint16,
                         p_value will be cast to pointer of uint16_t).
 *
 * @return Set result
 * @retval GAP_CAUSE_SUCCESS Set parameter success.
 * @retval other Set parameter failed.
  *
  * <b>Example usage</b>
  * \code{.c}
  static T_USER_CMD_PARSE_RESULT cmd_escan(T_USER_CMD_PARSED_VALUE *p_parse_value)
  {
    T_GAP_CAUSE cause;
    T_GAP_LOCAL_ADDR_TYPE  own_address_type = GAP_LOCAL_ADDR_LE_PUBLIC;
    T_GAP_SCAN_FILTER_POLICY  ext_scan_filter_policy = GAP_SCAN_FILTER_ANY;
    T_GAP_SCAN_FILTER_DUPLICATE  ext_scan_filter_duplicate = GAP_SCAN_FILTER_DUPLICATE_ENABLE;
    uint16_t ext_scan_duration;
    uint16_t ext_scan_period;
    uint8_t  scan_phys = GAP_EXT_SCAN_PHYS_1M_BIT | GAP_EXT_SCAN_PHYS_CODED_BIT;
    T_EXT_SCAN_MODE  scan_mode = (T_EXT_SCAN_MODE)p_parse_value->dw_param[0];

    link_mgr_clear_device_list();
    if (scan_mode == SCAN_UNTIL_DISABLED)
    {
        // If Duration paramter is zero, continue scanning until scanning is disabled.
        ext_scan_duration = 0;
        ext_scan_period = 0;
    }
    else if (scan_mode == PERIOD_SCAN_UNTIL_DISABLED)
    {
        // If Duration and Period parameters are non-zero, scan for the duration within a scan period,
        // and scan periods continue until scanning is disabled. Duration shall be less than Period.
        ext_scan_duration = 500;
        ext_scan_period = 8;
        ext_scan_filter_duplicate = GAP_SCAN_FILTER_DUPLICATE_ENABLED_RESET_FOR_EACH_PERIOD;
    }
    else if (scan_mode == SCAN_UNTIL_DURATION_EXPIRED)
    {
        // If Duration parameter is non-zero and Period parameter is zero, continue scanning until duration has expired.
        ext_scan_duration = 500;
        ext_scan_period = 0;
    }
    if (p_parse_value->param_count > 1)
    {
        scan_phys = p_parse_value->dw_param[1];
    }

    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_LOCAL_ADDR_TYPE, sizeof(own_address_type), &own_address_type);
    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_PHYS, sizeof(scan_phys), &scan_phys);
    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_DURATION, sizeof(ext_scan_duration), &ext_scan_duration);
    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_PERIOD, sizeof(ext_scan_period),&ext_scan_period);
    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_FILTER_POLICY, sizeof(ext_scan_filter_policy), &ext_scan_filter_policy);
    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_FILTER_DUPLICATES, sizeof(ext_scan_filter_duplicate), &ext_scan_filter_duplicate);

    cause = le_ext_scan_start();
    return (T_USER_CMD_PARSE_RESULT)cause;
  }
  * \endcode
  */
T_GAP_CAUSE le_ext_scan_set_param(T_LE_EXT_SCAN_PARAM_TYPE param, uint8_t len, void *p_value);

/**
 * @brief       Get a extended scan parameter.
 *
 *              NOTE: You can call this function with a extended scan parameter type and it will get a
 *              extended scan parameter. Extended scan parameters are defined in @ref T_LE_EXT_SCAN_PARAM_TYPE.
 *
 * @param[in]      param   Extended scan parameter type: @ref T_LE_EXT_SCAN_PARAM_TYPE.
 * @param[in,out]  p_value Pointer to location to get the parameter value. This is dependent on
 *                         the parameter type and will be cast to the appropriate
 *                         data type (For example: if data type of param is uint16_t, p_value will be cast to
 *                         pointer of uint16_t).
 *
 * @return  Get result
 * @retval  GAP_CAUSE_SUCCESS Get parameter success.
 * @retval  GAP_CAUSE_INVALID_PARAM Get parameter failed, invalid parameter.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        uint16_t scan_duration;
        le_ext_scan_get_param(GAP_PARAM_EXT_SCAN_DURATION, &scan_duration);
    }
  * \endcode
  */
T_GAP_CAUSE le_ext_scan_get_param(T_LE_EXT_SCAN_PARAM_TYPE param, void *p_value);

/**
 * @brief       Set a extended scan phy parameter including scan type, scan interval and scan window
                for scan advertisements on LE 1M PHY or/and LE Coded PHY.
 *
 * @param[in]   type    Scan PHY type: @ref T_LE_EXT_SCAN_PHY_TYPE
 * @param[in]   p_param Pointer to data to write, @ref T_GAP_LE_EXT_SCAN_PARAM.
                        scan_type: Passive scanning or active scanning, @ref T_GAP_SCAN_MODE.
                        scan_interval: The frequency of scan, in units of 0.625ms, range: 0x0004 to 0xFFFF.
                        scan_window: The length of scan, in units of 0.625ms, range: 0x0004 to 0xFFFF.
 *
 * @return void
  *
  * <b>Example usage</b>
  * \code{.c}
  static T_USER_CMD_PARSE_RESULT cmd_escan(T_USER_CMD_PARSED_VALUE *p_parse_value)
  {
    uint8_t  scan_phys = GAP_EXT_SCAN_PHYS_1M_BIT | GAP_EXT_SCAN_PHYS_CODED_BIT;
    T_GAP_LE_EXT_SCAN_PARAM extended_scan_param[GAP_EXT_SCAN_MAX_PHYS_NUM];
    extended_scan_param[0].scan_type = GAP_SCAN_MODE_ACTIVE;
    extended_scan_param[0].scan_interval = 400;
    extended_scan_param[0].scan_window = 200;
    extended_scan_param[1].scan_type = GAP_SCAN_MODE_ACTIVE;
    extended_scan_param[1].scan_interval = 440;
    extended_scan_param[1].scan_window = 220;

    le_ext_scan_set_param(GAP_PARAM_EXT_SCAN_PHYS, sizeof(scan_phys), &scan_phys);

    le_ext_scan_set_phy_param(LE_SCAN_PHY_LE_1M, &extended_scan_param[0]);
    le_ext_scan_set_phy_param(LE_SCAN_PHY_LE_CODED, &extended_scan_param[1]);

    return (RESULT_SUCESS);
  }
  * \endcode
  */
void le_ext_scan_set_phy_param(T_LE_EXT_SCAN_PHY_TYPE type, T_GAP_LE_EXT_SCAN_PARAM *p_param);

/**
 * @brief   Start a device discovery extended scan.
            If device changes to scanning state, @ref app_handle_dev_state_evt will be called. And the
            advertising data or scan response data will be returned by @ref app_gap_callback with
            cb_type @ref GAP_MSG_LE_EXT_ADV_REPORT_INFO.
 *
 * @return  Operation result.
 * @retval  GAP_CAUSE_SUCCESS: Start request success.
 * @retval  GAP_CAUSE_INVALID_STATE: Start request failed, invalid device state.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test()
    {
        le_ext_scan_start();
    }

    void app_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
    {
        APP_PRINT_INFO5("app_handle_dev_state_evt: init state = %d scan state = %d adv state = %d conn state = %d cause = 0x%x",
                       new_state.gap_init_state,
                       new_state.gap_scan_state, new_state.gap_adv_state, new_state.gap_conn_state, cause);

        if (gap_dev_state.gap_scan_state != new_state.gap_scan_state)
        {
           if (new_state.gap_scan_state == GAP_SCAN_STATE_IDLE)
           {
               APP_PRINT_INFO0("GAP scan stop");
               data_uart_print("GAP scan stop\r\n");
           }
           else if (new_state.gap_scan_state == GAP_SCAN_STATE_SCANNING)
           {
               APP_PRINT_INFO0("GAP scan start");
               data_uart_print("GAP scan start\r\n");
           }
        }

        gap_dev_state = new_state;
    }

    //Received advertising or scan response data will be handled in app_gap_callback
    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;
        APP_PRINT_TRACE1("app_gap_callback: cb_type = %d", cb_type);

        switch (cb_type)
        {
        case GAP_MSG_LE_EXT_ADV_REPORT_INFO:
        APP_PRINT_INFO6("GAP_MSG_LE_EXT_ADV_REPORT_INFO:connectable %d, scannable %d, direct %d, scan response %d, legacy %d, data status 0x%x",
                        p_data->p_le_ext_adv_report_info->event_type & GAP_EXT_ADV_REPORT_BIT_CONNECTABLE_ADV,
                        p_data->p_le_ext_adv_report_info->event_type & GAP_EXT_ADV_REPORT_BIT_SCANNABLE_ADV,
                        p_data->p_le_ext_adv_report_info->event_type & GAP_EXT_ADV_REPORT_BIT_DIRECTED_ADV,
                        p_data->p_le_ext_adv_report_info->event_type & GAP_EXT_ADV_REPORT_BIT_SCAN_RESPONSE,
                        p_data->p_le_ext_adv_report_info->event_type & GAP_EXT_ADV_REPORT_BIT_USE_LEGACY_ADV,
                        p_data->p_le_ext_adv_report_info->data_status);
        APP_PRINT_INFO5("GAP_MSG_LE_EXT_ADV_REPORT_INFO:event_type 0x%x, bd_addr %s, addr_type %d, rssi %d, data_len %d",
                        p_data->p_le_ext_adv_report_info->event_type,
                        TRACE_BDADDR(p_data->p_le_ext_adv_report_info->bd_addr),
                        p_data->p_le_ext_adv_report_info->addr_type,
                        p_data->p_le_ext_adv_report_info->rssi,
                        p_data->p_le_ext_adv_report_info->data_len);
        APP_PRINT_INFO5("GAP_MSG_LE_EXT_ADV_REPORT_INFO:primary_phy %d, secondary_phy %d, adv_sid %d, tx_power %d, peri_adv_interval %d",
                        p_data->p_le_ext_adv_report_info->primary_phy,
                        p_data->p_le_ext_adv_report_info->secondary_phy,
                        p_data->p_le_ext_adv_report_info->adv_sid,
                        p_data->p_le_ext_adv_report_info->tx_power,
                        p_data->p_le_ext_adv_report_info->peri_adv_interval);
        APP_PRINT_INFO2("GAP_MSG_LE_EXT_ADV_REPORT_INFO:direct_addr_type 0x%x, direct_addr %s",
                        p_data->p_le_ext_adv_report_info->direct_addr_type,
                        TRACE_BDADDR(p_data->p_le_ext_adv_report_info->direct_addr));

        link_mgr_add_device(p_data->p_le_ext_adv_report_info->bd_addr,
                            p_data->p_le_ext_adv_report_info->addr_type);

        #if APP_RECOMBINE_ADV_DATA
        if (!(p_data->p_le_ext_adv_report_info->event_type & GAP_EXT_ADV_REPORT_BIT_USE_LEGACY_ADV))
        {
            app_handle_ext_adv_report(p_data->p_le_ext_adv_report_info->event_type,
                                      p_data->p_le_ext_adv_report_info->data_status, p_data->p_le_ext_adv_report_info->bd_addr,
                                      p_data->p_le_ext_adv_report_info->data_len, p_data->p_le_ext_adv_report_info->p_data);
        }
        #endif
        break;
        }
    }
  * \endcode
  */
T_GAP_CAUSE le_ext_scan_start(void);

/**
  * @brief   Stop a device discovery extended scan.
             If device changes to idle state, @ref app_handle_dev_state_evt will be called.
  *
  * @return  Operation result.
  * @retval  GAP_CAUSE_SUCCESS: Stop request success.
  * @retval  GAP_CAUSE_INVALID_STATE: Stop request failed. Invalid state, not in scan state.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test()
    {
        le_ext_scan_stop();
    }

    void app_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
    {
        APP_PRINT_INFO5("app_handle_dev_state_evt: init state = %d scan state = %d adv state = %d conn state = %d cause = 0x%x",
                       new_state.gap_init_state,
                       new_state.gap_scan_state, new_state.gap_adv_state, new_state.gap_conn_state, cause);

        if (gap_dev_state.gap_scan_state != new_state.gap_scan_state)
        {
            if (new_state.gap_scan_state == GAP_SCAN_STATE_IDLE)
            {
                APP_PRINT_INFO0("GAP scan stop");
                data_uart_print("GAP scan stop\r\n");
            }
            else if (new_state.gap_scan_state == GAP_SCAN_STATE_SCANNING)
            {
                APP_PRINT_INFO0("GAP scan start");
                data_uart_print("GAP scan start\r\n");
            }
        }

        gap_dev_state = new_state;
    }
  * \endcode
  */
T_GAP_CAUSE le_ext_scan_stop(void);

#if F_BT_LE_GAP_MSG_INFO_WAY
/**
* @brief  Set extended scanning gap message inform way.
*
*         Default value is True.
*         If use_msg is True, gap will send the extended scanning gap message to io_queue registered by
*         gap_start_bt_stack. Message type is @ref GAP_MSG_LE_DEV_STATE_CHANGE.
*         If use_msg is False, gap will send the extended scanning gap message using callback function registered by
*         @ref app_gap_callback. Message type is @ref GAP_MSG_LE_EXT_SCAN_STATE_CHANGE_INFO.
*
* @param[in] use_msg Whether to use message.
* @retval void
*
* <b>Example usage</b>
* \code{.c}
    int test(void)
    {
        le_ext_scan_gap_msg_info_way(false);
    }

    void app_handle_ext_scan_state_evt(uint8_t new_state, uint16_t cause)
    {
        ......
        if (new_state == GAP_SCAN_STATE_IDLE)
        {
        }
        else if (new_state == GAP_SCAN_STATE_SCANNING)
        {
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
        case GAP_MSG_LE_EXT_SCAN_STATE_CHANGE_INFO:
            {
                app_handle_ext_scan_state_evt(p_data->p_le_ext_scan_state_change_info->state,
                                              p_data->p_le_ext_scan_state_change_info->cause);
            }
            break;
        }
        ......
    }
* \endcode
*/
void le_ext_scan_gap_msg_info_way(bool use_msg);
#endif
/** End of GAP_LE_EXTENDED_SCAN_Exported_Functions
  * @}
  */

/** End of GAP_LE_EXTENDED_SCAN
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

#endif /* GAP_EXT_SCAN_H */
