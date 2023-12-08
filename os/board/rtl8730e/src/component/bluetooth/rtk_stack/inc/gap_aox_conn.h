/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_aox_conn.h
* @brief    Head file for GAP AoA/AoD connection
* @details
* @author
* @date      2020-06-18
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_AOX_CONN_H
#define GAP_AOX_CONN_H

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

#if F_BT_LE_5_1_AOX_CONN_SUPPORT

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_AOX_CONN GAP LE AoA/AoD Connection Module
  * @{
  */


/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_AOX_CONN_Exported_Macros GAP LE AoA/AoD Connection Exported Macros
  * @{
  */

/** @defgroup GAP_AOX_CONN_CTE_TYPES_BITS LE AoX Connection CTE Types Bits
* @{
 */
#define GAP_AOX_CONN_CTE_TYPES_AOA_BIT       0x01     /**< Allow AoA Constant Tone Extension Response. */
#define GAP_AOX_CONN_CTE_TYPES_AOD_1US_BIT   0x02     /**< Allow AoD Constant Tone Extension Response with 1 us slots. */
#define GAP_AOX_CONN_CTE_TYPES_AOD_2US_BIT   0x04     /**< Allow AoD Constant Tone Extension Response with 2 us slots. */
/** End of GAP_AOX_CONN_CTE_TYPES_BITS
  * @}
  */
/** End of GAP_LE_AOX_CONN_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_AOX_CONN_Exported_Types GAP LE AoA/AoD Connection Exported Types
  * @brief
  * @{
  */
typedef enum
{
    GAP_AOX_SAMPLING_DISABLE = 0x00,     /**< Connection IQ sampling is disabled (default). */
    GAP_AOX_SAMPLING_ENABLE = 0x01,      /**< Connection IQ sampling is enabled. */
} T_GAP_AOX_SAMPLING_ENABLE_TYPE;

typedef enum
{
    GAP_AOX_CTE_REQUEST_DISABLE = 0x00,     /**< Disable Constant Tone Extension Request for the connection (default). */
    GAP_AOX_CTE_REQUEST_ENABLE = 0x01,      /**< Enable Constant Tone Extension Request for the connection. */
} T_GAP_AOX_CTE_REQUEST_ENABLE_TYPE;

typedef enum
{
    GAP_AOX_CTE_RESPONSE_DISABLE = 0x00,     /**< Disable Constant Tone Extension Response for the connection (default). */
    GAP_AOX_CTE_RESPONSE_ENABLE = 0x01,      /**< Enable Constant Tone Extension Response for the connection. */
} T_GAP_AOX_CTE_RESPONSE_ENABLE_TYPE;
/** End of GAP_LE_AOX_CONN_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_AOX_CONN_Exported_Functions GAP LE AoA/AoD Connection Exported Functions
  * @brief
  * @{
  */
/**
 * @brief   Set Connection CTE Receive Parameters.
 *          Result will be returned by @ref app_gap_aox_callback with cb_type @ref GAP_MSG_LE_AOX_SET_CONN_CTE_RECEIVE_PARAMS.
 *
 * @param[in] conn_id  Connection ID.
 * @param[in] sampling_enable  Enable/disable Connection IQ sampling: @ref T_GAP_AOX_SAMPLING_ENABLE_TYPE.
 * @param[in] slot_durations  Switching and sampling slots: @ref T_GAP_SLOT_DUATIONS_TYPE.
 * @param[in] switching_pattern_length  The number of Antenna IDs in the pattern.
 *                                      Range: 0x02 to max_switching_pattern_length supported by controller returned by
 *                                             @ref le_aox_read_antenna_information.
 * @param[in] p_antenna_ids  Antenna ID in the pattern.
 * @return  Result of sending request.
 * @retval  GAP_CAUSE_SUCCESS: Send request success.
 * @retval  GAP_CAUSE_SEND_REQ_FAILED: Send request sent fail.
 * @retval  GAP_CAUSE_NON_CONN: Failed. No connection.
 * @retval  GAP_CAUSE_INVALID_PARAM: Failed. Invalid parameter.
 *
 * <b>Example usage</b>
 * \code{.c}
   void test()
   {
        uint8_t conn_id = 0;
        T_GAP_AOX_SAMPLING_ENABLE_TYPE sampling_enable = GAP_AOX_SAMPLING_ENABLE;
        T_GAP_SLOT_DUATIONS_TYPE slot_durations = GAP_SLOT_DURATIONS_SWITCH_SAMPLE_2US;
        uint8_t switching_pattern_length = 2;
        uint8_t p_antenna_ids[2] = {0, 1};

        T_GAP_CAUSE cause = le_aox_set_conn_cte_receive_params(conn_id, sampling_enable, slot_durations,
                                           switching_pattern_length, p_antenna_ids);
   }

  T_APP_RESULT app_gap_aox_callback(uint8_t cb_type, void *p_cb_data)
  {
      T_APP_RESULT result = APP_RESULT_SUCCESS;
      T_LE_AOX_CB_DATA *p_data = (T_LE_AOX_CB_DATA *)p_cb_data;

      switch (cb_type)
      {
      ......
      case GAP_MSG_LE_AOX_SET_CONN_CTE_RECEIVE_PARAMS:
        APP_PRINT_INFO2("GAP_MSG_LE_AOX_SET_CONN_CTE_RECEIVE_PARAMS: cause 0x%x, conn_id %d",
                        p_data->p_le_aox_set_conn_cte_receive_params_rsp->cause,
                        p_data->p_le_aox_set_conn_cte_receive_params_rsp->conn_id);
        break;
      ......
      }
    ......
    }
 * \endcode
 */
T_GAP_CAUSE le_aox_set_conn_cte_receive_params(uint8_t conn_id,
                                               T_GAP_AOX_SAMPLING_ENABLE_TYPE sampling_enable,
                                               T_GAP_SLOT_DUATIONS_TYPE slot_durations,
                                               uint8_t switching_pattern_length,
                                               uint8_t *p_antenna_ids);

/**
 * @brief   Set Connection CTE Transmit Parameters.
 *          Result will be returned by @ref app_gap_aox_callback with cb_type @ref GAP_MSG_LE_AOX_SET_CONN_CTE_TRANSMIT_PARAMS.
 *
 * @param[in] conn_id  Connection ID
 * @param[in] cte_types   A bit field that indicates the type of Constant Tone Extension that the Controller support
                          @ref GAP_AOX_CONN_CTE_TYPES_BITS.
 * @param[in] switching_pattern_length  The number of Antenna IDs in the pattern, and shall be ignored when cte_types does not
 *                                      have a bit set for an AoD Constant Tone Extension.
 *                                      Range: 0x02 to max_switching_pattern_length supported by controller returned by
 *                                             @ref le_aox_read_antenna_information.
 * @param[in] p_antenna_ids  Antenna ID in the pattern, and shall be ignored when cte_types does not
                             have a bit set for an AoD Constant Tone Extension.
 * @return  Result of sending request.
 * @retval  GAP_CAUSE_SUCCESS: Send request success.
 * @retval  GAP_CAUSE_SEND_REQ_FAILED: Send request sent fail.
 * @retval  GAP_CAUSE_NON_CONN: Failed. No connection.
 * @retval  GAP_CAUSE_INVALID_PARAM: Failed. Invalid parameter.
 *
 * <b>Example usage</b>
 * \code{.c}
   void test()
   {
        uint8_t conn_id = 0;
        uint8_t cte_types = GAP_AOX_CONN_CTE_TYPES_AOA_BIT | GAP_AOX_CONN_CTE_TYPES_AOD_2US_BIT;
        uint8_t switching_pattern_length = 2;
        uint8_t p_antenna_ids[2] = {0, 1};

        T_GAP_CAUSE cause = le_aox_set_conn_cte_transmit_params(conn_id, cte_types, switching_pattern_length, p_antenna_ids);
   }

  T_APP_RESULT app_gap_aox_callback(uint8_t cb_type, void *p_cb_data)
  {
      T_APP_RESULT result = APP_RESULT_SUCCESS;
      T_LE_AOX_CB_DATA *p_data = (T_LE_AOX_CB_DATA *)p_cb_data;

      switch (cb_type)
      {
      ......
      case GAP_MSG_LE_AOX_SET_CONN_CTE_TRANSMIT_PARAMS:
        APP_PRINT_INFO2("GAP_MSG_LE_AOX_SET_CONN_CTE_TRANSMIT_PARAMS: cause 0x%x, conn_id %d",
                        p_data->p_le_aox_set_conn_cte_transmit_params_rsp->cause,
                        p_data->p_le_aox_set_conn_cte_transmit_params_rsp->conn_id);
        break;
      ......
      }
    ......
    }
 * \endcode
 */
T_GAP_CAUSE le_aox_set_conn_cte_transmit_params(uint8_t conn_id, uint8_t cte_types,
                                                uint8_t switching_pattern_length,
                                                uint8_t *p_antenna_ids);

/**
 * @brief   Request the Controller to start or stop initiating the Constant Tone Extension Request procedure.
 *          Result will be returned by @ref app_gap_aox_callback with cb_type @ref GAP_MSG_LE_AOX_CONN_CTE_REQUEST_ENABLE.
 *
 *          IQ samples will be returned by @ref app_gap_aox_callback with cb_type @ref GAP_MSG_LE_AOX_CONN_IQ_REPORT_INFO
 *          on Constant Tone Extensions received.
 *          Otherwise, request failed info will be returned by @ref app_gap_aox_callback with cb_type
 *          @ref GAP_MSG_LE_AOX_CTE_REQUEST_FAILED_INFO.
 *
 * @param[in] conn_id  Connection ID
 * @param[in] enable  Enable/disable Constant Tone Extension Request: @ref T_GAP_AOX_CTE_REQUEST_ENABLE_TYPE.
                      If Enable is set to GAP_AOX_CTE_REQUEST_DISABLE, the remaining parameters shall be ignored.
 * @param[in] cte_request_interval  Defines whether the Constant Tone Extension Request procedure is initiated
                                    only once or periodically.
                                    0x0000: Initiate the Constant Tone Extension Request procedure once, at the
                                            earliest practical opportunity
                                    0x0001 to 0xFFFF: Requested interval for initiating the Constant Tone Extension
                                                      Request procedure in number of connection events.
 * @param[in] requested_cte_length  The minimum length of the Constant Tone Extension that the Controller shall
                                    request from the remote device.
                                    Range: 0x02 to 0x14.
                                    Units: 8 us.
 * @param[in] requested_cte_type  The type of Constant Tone Extension that the Controller shall request from the remote device:
 *                                @ref T_GAP_CTE_TYPE.
 * @return  Result of sending request.
 * @retval  GAP_CAUSE_SUCCESS: Send request success.
 * @retval  GAP_CAUSE_SEND_REQ_FAILED: Send request sent fail.
 * @retval  GAP_CAUSE_NON_CONN: Failed. No connection.
 * @retval  GAP_CAUSE_INVALID_PARAM: Failed. Invalid parameter.
 *
 * <b>Example usage</b>
 * \code{.c}
   void test()
   {
        uint8_t conn_id = 0;
        T_GAP_AOX_CTE_REQUEST_ENABLE_TYPE enable = GAP_AOX_CTE_REQUEST_ENABLE;
        uint16_t cte_request_interval = 0;
        uint8_t requested_cte_length = 2;
        T_GAP_CTE_TYPE requested_cte_type = GAP_CTE_TYPE_AOA;

        T_GAP_CAUSE cause = le_aox_conn_cte_request_enable(conn_id, enable, cte_request_interval,
                                       requested_cte_length, requested_cte_type);
   }

  T_APP_RESULT app_gap_aox_callback(uint8_t cb_type, void *p_cb_data)
  {
      T_APP_RESULT result = APP_RESULT_SUCCESS;
      T_LE_AOX_CB_DATA *p_data = (T_LE_AOX_CB_DATA *)p_cb_data;

      switch (cb_type)
      {
      ......
      case GAP_MSG_LE_AOX_CONN_CTE_REQUEST_ENABLE:
        APP_PRINT_INFO2("GAP_MSG_LE_AOX_CONN_CTE_REQUEST_ENABLE: cause 0x%x, conn_id %d",
                        p_data->p_le_aox_conn_cte_request_enable_rsp->cause,
                        p_data->p_le_aox_conn_cte_request_enable_rsp->conn_id);
        break;
          case GAP_MSG_LE_AOX_CONN_IQ_REPORT_INFO:
        APP_PRINT_INFO8("GAP_MSG_LE_AOX_CONN_IQ_REPORT_INFO: conn_id %d, rx_phy %d, data_chan_index %d, rssi %d, rssi_antenna_id %d, cte_type %d, slot_durations %d, packet_status %d",
                        p_data->p_le_aox_conn_iq_report_info->conn_id,
                        p_data->p_le_aox_conn_iq_report_info->rx_phy,
                        p_data->p_le_aox_conn_iq_report_info->data_chan_index,
                        p_data->p_le_aox_conn_iq_report_info->rssi,
                        p_data->p_le_aox_conn_iq_report_info->rssi_antenna_id,
                        p_data->p_le_aox_conn_iq_report_info->cte_type,
                        p_data->p_le_aox_conn_iq_report_info->slot_durations,
                        p_data->p_le_aox_conn_iq_report_info->packet_status);
        APP_PRINT_INFO2("GAP_MSG_LE_AOX_CONN_IQ_REPORT_INFO: connection_event_counter %d, sample_count %d",
                        p_data->p_le_aox_conn_iq_report_info->connection_event_counter,
                        p_data->p_le_aox_conn_iq_report_info->sample_count);
        if (p_data->p_le_aox_conn_iq_report_info->sample_count)
        {
            APP_PRINT_INFO1("GAP_MSG_LE_AOX_CONN_IQ_REPORT_INFO: iq_sample %b",
                             TRACE_BINARY(p_data->p_le_aox_conn_iq_report_info->sample_count * 2,
                                          p_data->p_le_aox_conn_iq_report_info->p_iq_sample));
        }
        break;
      case GAP_MSG_LE_AOX_CTE_REQUEST_FAILED_INFO:
        APP_PRINT_INFO2("GAP_MSG_LE_AOX_CTE_REQUEST_FAILED_INFO: cause 0x%x, conn_id %d",
                        p_data->p_le_aox_cte_request_failed_info->cause,
                        p_data->p_le_aox_cte_request_failed_info->conn_id);
        break;
      ......
      }
    ......
    }
 * \endcode
 */
T_GAP_CAUSE le_aox_conn_cte_request_enable(uint8_t conn_id,
                                           T_GAP_AOX_CTE_REQUEST_ENABLE_TYPE enable,
                                           uint16_t cte_request_interval, uint8_t requested_cte_length,
                                           T_GAP_CTE_TYPE requested_cte_type);

/**
 * @brief   Request the Controller to respond to Constant Tone Extension Request.
 *          Result will be returned by @ref app_gap_aox_callback with cb_type @ref GAP_MSG_LE_AOX_CONN_CTE_RESPONSE_ENABLE.
 *
 * @param[in] conn_id  Connection ID.
 * @param[in] enable  Enable/disable Constant Tone Extension Response: @ref T_GAP_AOX_CTE_RESPONSE_ENABLE_TYPE.
 * @return  Result of sending request.
 * @retval  GAP_CAUSE_SUCCESS: Send request success.
 * @retval  GAP_CAUSE_SEND_REQ_FAILED: Send request sent fail.
 * @retval  GAP_CAUSE_NON_CONN: Failed. No connection.
 * @retval  GAP_CAUSE_INVALID_PARAM: Failed. Invalid parameter.
 *
 * <b>Example usage</b>
 * \code{.c}
   void test()
   {
        uint8_t conn_id = 0;
        T_GAP_AOX_CTE_RESPONSE_ENABLE_TYPE enable = GAP_AOX_CTE_RESPONSE_ENABLE;

        T_GAP_CAUSE cause = le_aox_conn_cte_response_enable(conn_id, enable);
   }

  T_APP_RESULT app_gap_aox_callback(uint8_t cb_type, void *p_cb_data)
  {
      T_APP_RESULT result = APP_RESULT_SUCCESS;
      T_LE_AOX_CB_DATA *p_data = (T_LE_AOX_CB_DATA *)p_cb_data;

      switch (cb_type)
      {
      ......
       case GAP_MSG_LE_AOX_CONN_CTE_RESPONSE_ENABLE:
        APP_PRINT_INFO2("GAP_MSG_LE_AOX_CONN_CTE_RESPONSE_ENABLE: cause 0x%x, conn_id %d",
                        p_data->p_le_aox_conn_cte_response_enable_rsp->cause,
                        p_data->p_le_aox_conn_cte_response_enable_rsp->conn_id);
        break;
      ......
      }
    ......
    }
 * \endcode
 */
T_GAP_CAUSE le_aox_conn_cte_response_enable(uint8_t conn_id,
                                            T_GAP_AOX_CTE_RESPONSE_ENABLE_TYPE enable);

/** End of GAP_LE_AOX_CONN_Exported_Functions
  * @}
  */

/** End of GAP_LE_AOX_CONN
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

#endif /* GAP_AOX_CONN_H */
