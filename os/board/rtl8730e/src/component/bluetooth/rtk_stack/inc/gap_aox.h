/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_aox.h
* @brief    Head file for GAP AoA/AoD
* @details
* @author
* @date      2020-06-18
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_AOX_H
#define GAP_AOX_H

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

#if F_BT_LE_5_1_AOA_AOD_SUPPORT

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_AOX GAP LE AoA/AoD Module
  * @{
  */


/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_AOX_Exported_Macros GAP LE AoA/AoD Exported Macros
  * @{
  */

/** @defgroup GAP_LE_AOX_CALLLBACK_MSG GAP LE AoA/AoD Callback Msg
  * @brief    Used to identify LE AoA/AoD Callback Message.
 * @{
 */
#define GAP_MSG_LE_AOX_READ_ANTENNA_INFORMATION     0x01 /**<Response msg type for le_aox_read_antenna_information. */

#define GAP_MSG_LE_AOX_SET_CONN_CTE_RECEIVE_PARAMS  0x21 /**<Response msg type for le_aox_set_conn_cte_receive_params. */
#define GAP_MSG_LE_AOX_SET_CONN_CTE_TRANSMIT_PARAMS 0x22 /**<Response msg type for le_aox_set_conn_cte_transmit_params. */
#define GAP_MSG_LE_AOX_CONN_CTE_RESPONSE_ENABLE     0x23 /**<Response msg type for le_aox_conn_cte_response_enable. */
#define GAP_MSG_LE_AOX_CONN_CTE_REQUEST_ENABLE      0x24 /**<Response msg type for le_aox_conn_cte_request_enable. */
#define GAP_MSG_LE_AOX_CONN_IQ_REPORT_INFO          0x25 /**<Notification msg type for connection IQ report. */
#define GAP_MSG_LE_AOX_CTE_REQUEST_FAILED_INFO      0x26 /**<Notification msg type for failure of CTE request. */

#define GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_SET_CTE_TRANSMIT_PARAMS  0x40 /**<Response msg type for le_aox_connless_transmitter_set_cte_transmit_params. */
#define GAP_MSG_LE_AOX_CONNLESS_TRANSMITTER_STATE_CHANGE_INFO        0x41 /**<Connectionless CTE transmitter state change info. */

#define GAP_MSG_LE_AOX_CONNLESS_RECEIVER_SET_IQ_SAMPLING_ENABLE      0x50 /**<Response msg type for le_aox_connless_receiver_set_iq_sampling_enable. */
#define GAP_MSG_LE_AOX_CONNLESS_RECEIVER_CONNLESS_IQ_REPORT_INFO     0x51 /**<Notification msg type for LE connectionless IQ report info. */
/** End of GAP_LE_AOX_CALLLBACK_MSG
  * @}
  */

/** @defgroup GAP_AOX_SUPPORTED_SWITCH_SAMPLE_BITS LE AoX Supported Switching Sampling Rates Bits
* @{
 */
#define GAP_AOX_SUPPORTED_SWITCH_AOD_TRANS_BIT          0x01     /**< 1 us switching supported for AoD transmission. */
#define GAP_AOX_SUPPORTED_SAMPLE_AOD_RECEP_BIT          0x02     /**< 1 us sampling supported for AoD reception. */
#define GAP_AOX_SUPPORTED_SWITCH_SAMPLE_AOA_RECEP_BIT   0x04     /**< 1 us switching and sampling supported for AoA reception. */
/** End of GAP_AOX_SUPPORTED_SWITCH_SAMPLE_BITS
  * @}
  */
/** End of GAP_LE_AOX_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_AOX_Exported_Types GAP LE AoA/AoD Exported Types
  * @brief
  * @{
  */
typedef enum
{
    GAP_AOX_PACKET_CRC_CORRECT = 0x00,                         /**< CRC was correct. */
    GAP_AOX_PACKET_CRC_INCORRECT_DETERMINE = 0x01,             /**< CRC was incorrect, and the Length and CTETime fields
                                                                    of the packet used to determine sampling points. */
    GAP_AOX_PACKET_CRC_INCORRECT_DETERMINE_OTHERWAY = 0x02,    /**< CRC was incorrect but the Controller has determined
                                                                    the position and length of the Constant Tone Extension
                                                                    in some other way. */
    GAP_AOX_PACKET_INSUFFI_RESOURCES = 0xFF,                  /**< Insufficient resources to sample
                                                                   (Data_Channel_Index, CTE_Type, and Slot_Durations invalid). */
} T_GAP_AOX_PACKET_STATUS_TYPE;

typedef struct
{
    uint16_t cause;
    uint8_t supported_switching_sampling_rates;  /**< A bit field that indicates the type of switching sampling rates
                                                      supported by the Controller. @ref GAP_AOX_SUPPORTED_SWITCH_SAMPLE_BITS.*/
    uint8_t num_antennae;                        /**< The number of antennae supported by the Controller. */
    uint8_t max_switching_pattern_length;        /**< Maximum length of antenna switching pattern supported by the Controller. */
    uint8_t max_cte_length;                      /**< Maximum length of a transmitted Constant Tone Extension supported in 8 ��s units. */
} T_LE_AOX_READ_ANTENNA_INFORMATION_RSP;

#if F_BT_LE_5_1_AOX_CONN_SUPPORT
typedef struct
{
    uint16_t    cause;
    uint8_t     conn_id;
} T_LE_AOX_SET_CONN_CTE_RECEIVE_PARAMS_RSP;

typedef struct
{
    uint16_t    cause;
    uint8_t     conn_id;
} T_LE_AOX_SET_CONN_CTE_TRANSMIT_PARAMS_RSP;

typedef struct
{
    uint16_t    cause;
    uint8_t     conn_id;
} T_LE_AOX_CONN_CTE_REQUEST_ENABLE_RSP;

typedef struct
{
    uint16_t    cause;
    uint8_t     conn_id;
} T_LE_AOX_CONN_CTE_RESPONSE_ENABLE_RSP;

typedef struct
{
    uint8_t                             conn_id;
    uint8_t                             rx_phy;/**< 0x01: The receiver PHY for the connection is LE 1M.
                                                    0x02: The receiver PHY for the connection is LE 2M. */
    uint8_t                             data_chan_index;/**< The index of the data channel on which the
                                                             Data Physical Channel PDU was received.
                                                             Range: 0x00 to 0x24. */
    int16_t                             rssi;          /**< RSSI of the packet.
                                                            Range: -1270 to +200, Units: 0.1 dBm. */
    uint8_t                             rssi_antenna_id;/**< ID of the antenna on which the
                                                             RSSI is measured. */
    T_GAP_CTE_TYPE                      cte_type;
    T_GAP_SLOT_DUATIONS_TYPE            slot_durations;
    T_GAP_AOX_PACKET_STATUS_TYPE        packet_status;
    uint16_t                            connection_event_counter; /**< The value of connEventCounter
                                                                       for the reported PDU. */
    uint8_t                             sample_count; /**< 0x00: No samples provided
                                                                (only permitted if Packet_Status is 0xFF)
                                                           0x09 to 0x52: Total number of sample pairs (there shall be the
                                                                         same number of I samples and Q samples). */
    int8_t                             *p_iq_sample; /**< Length is sample_cout * 2.
                                                          I_Sample[0],Q_Sample[0], ... ,
                                                          I_Sample[i],Q_Sample[i]
                                                          Value 0x80 indicates No valid sample available. */
} T_LE_AOX_CONN_IQ_REPORT_INFO;

typedef struct
{
    uint16_t    cause;
    uint8_t     conn_id;
} T_LE_AOX_CTE_REQUEST_FAILED_INFO;
#endif

#if F_BT_LE_5_1_AOX_CONNLESS_SUPPORT
#if F_BT_LE_5_1_AOX_CONNLESS_TRANSMITTER_SUPPORT
typedef struct
{
    uint16_t    cause;
    uint8_t     adv_handle; /**< Used to identify an advertising set. */
} T_LE_AOX_CONNLESS_TRANSMITTER_SET_CTE_TRANSMIT_PARAMS_RSP;

typedef struct
{
    uint16_t cause;
    uint8_t  adv_handle;    /**< Used to identify an advertising set. */
    uint8_t  state;         /**< @ref T_GAP_AOX_CONNLESS_TRANSMITTER_STATE. */
} T_LE_AOX_CONNLESS_TRANSMITTER_STATE_CHANGE_INFO;
#endif
#if F_BT_LE_5_1_AOX_CONNLESS_RECEIVER_SUPPORT
typedef struct
{
    uint8_t             sync_id;    /**< Identify the periodic advertising train. */
    uint16_t            sync_handle;/**< Sync_Handle identifying the periodic advertising train. */
    uint16_t            cause;
    uint8_t             sampling_enable;
} T_LE_AOX_CONNLESS_RECEIVER_SET_IQ_SAMPLING_ENABLE_RSP;

typedef struct
{
    uint8_t                             sync_id;    /**< Identify the periodic advertising train. */
    uint16_t                            sync_handle;/**< Sync_Handle identifying the periodic
                                                         advertising train. */
    uint8_t                             channel_index;/**< The index of the channel on which the
                                                           packet was received.
                                                           Range: 0x00 to 0x27.
                                                           Note: 0x25 to 0x27 can be used only for packets generated
                                                                 during test modes. */
    int16_t                             rssi;         /**< RSSI of the packet.
                                                           Range: -1270 to +200
                                                           Units: 0.1 dBm. */
    uint8_t                             rssi_antenna_id;     /**< Antenna ID. */
    T_GAP_CTE_TYPE                      cte_type;
    T_GAP_SLOT_DUATIONS_TYPE            slot_durations;/**< Specify the sampling rate used by
                                                            the Controller.*/
    T_GAP_AOX_PACKET_STATUS_TYPE        packet_status;
    uint16_t                            periodic_event_counter;/**< The value of paEventCounter
                                                                    for the reported AUX_SYNC_IND PDU. */
    uint8_t                             sample_count; /**< 0x00: No samples provided
                                                                 (only permitted if Packet_Status is 0xFF)
                                                           0x09 to 0x52: Total number of sample pairs
                                                                         (there shall be the same number of
                                                                         I samples and Q samples). */
    int8_t                             *p_iq_sample;  /**< Length is sample_cout * 2.
                                                           I_Sample[0],Q_Sample[0], ... ,
                                                           I_Sample[i],Q_Sample[i]
                                                           Value 0x80 indicates No valid sample available. */
} T_LE_AOX_CONNLESS_RECEIVER_CONNECTIONLESS_IQ_REPORT_INFO;
#endif
#endif

/** @brief  GAP AoX Callback Data*/
typedef union
{
    T_LE_AOX_READ_ANTENNA_INFORMATION_RSP       *p_le_aox_read_antenna_information_rsp;
#if F_BT_LE_5_1_AOX_CONN_SUPPORT
    T_LE_AOX_SET_CONN_CTE_RECEIVE_PARAMS_RSP    *p_le_aox_set_conn_cte_receive_params_rsp;
    T_LE_AOX_SET_CONN_CTE_TRANSMIT_PARAMS_RSP   *p_le_aox_set_conn_cte_transmit_params_rsp;
    T_LE_AOX_CONN_CTE_REQUEST_ENABLE_RSP        *p_le_aox_conn_cte_request_enable_rsp;
    T_LE_AOX_CONN_CTE_RESPONSE_ENABLE_RSP       *p_le_aox_conn_cte_response_enable_rsp;
    T_LE_AOX_CONN_IQ_REPORT_INFO                *p_le_aox_conn_iq_report_info;
    T_LE_AOX_CTE_REQUEST_FAILED_INFO            *p_le_aox_cte_request_failed_info;
#endif
#if F_BT_LE_5_1_AOX_CONNLESS_SUPPORT
#if F_BT_LE_5_1_AOX_CONNLESS_TRANSMITTER_SUPPORT
    T_LE_AOX_CONNLESS_TRANSMITTER_SET_CTE_TRANSMIT_PARAMS_RSP
    *p_le_aox_connless_transmitter_set_cte_transmit_params_rsp;
    T_LE_AOX_CONNLESS_TRANSMITTER_STATE_CHANGE_INFO
    *p_le_aox_connless_transmitter_state_change_info;
#endif
#if F_BT_LE_5_1_AOX_CONNLESS_RECEIVER_SUPPORT
    T_LE_AOX_CONNLESS_RECEIVER_SET_IQ_SAMPLING_ENABLE_RSP
    *p_le_aox_connless_receiver_set_iq_sampling_enable_rsp;
    T_LE_AOX_CONNLESS_RECEIVER_CONNECTIONLESS_IQ_REPORT_INFO
    *p_le_aox_connless_receiver_connectionless_iq_report_info;
#endif
#endif
} T_LE_AOX_CB_DATA;
/** End of GAP_LE_AOX_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_AOX_Exported_Functions GAP LE AoA/AoD Exported Functions
  * @brief
  * @{
  */
/**
  * @brief Callback for gap aox to notify app
  *
  * @param[in] cb_type    Callback msy type @ref GAP_LE_AOX_CALLLBACK_MSG.
  * @param[in] p_cb_data  Point to callback data @ref T_LE_AOX_CB_DATA.
  * @retval    result     @ref T_APP_RESULT
  */
typedef T_APP_RESULT(*P_FUN_LE_AOX_CB)(uint8_t cb_type, void *p_cb_data);

/**
 * @brief  Register callback to gap, when messages in @ref GAP_LE_AOX_CALLLBACK_MSG happens, it will callback to app.
 * @param[in]   aox_callback Callback function provided by the APP to handle gap aox command messages sent from the GAP.
 *              @arg NULL -> Not send gap aox command messages to APP.
 *              @arg Other -> Use application defined callback function.
 * @return void
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_le_gap_init(void)
    {
        ......
        le_register_aox_cb(app_gap_aox_callback);
    }
    T_APP_RESULT app_gap_aox_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_AOX_CB_DATA *p_data = (T_LE_AOX_CB_DATA *)p_cb_data;

        switch (cb_type)
        {
        case GAP_MSG_LE_AOX_READ_ANTENNA_INFORMATION:
            APP_PRINT_INFO5("GAP_MSG_LE_AOX_READ_ANTENNA_INFORMATION: cause 0x%x, supported_switching_sampling_rates 0x%x, num_antennae %d, max_switching_pattern_length %d, max_cte_length %d",
                            p_data->p_le_aox_read_antenna_information_rsp->cause,
                            p_data->p_le_aox_read_antenna_information_rsp->supported_switching_sampling_rates,
                            p_data->p_le_aox_read_antenna_information_rsp->num_antennae,
                            p_data->p_le_aox_read_antenna_information_rsp->max_switching_pattern_length,
                            p_data->p_le_aox_read_antenna_information_rsp->max_cte_length
                          );
            break;
        ......
        }
    ......
    }
   \endcode
 */
void le_register_aox_cb(P_FUN_LE_AOX_CB aox_callback);

/**
 * @brief   Read antenna information.
 *          Read the supported_switching_sampling_rates, num_antennae, max_switching_pattern_length, max_cte_length
 *          of a transmitted Constant Tone Extension supported by the Controller.
 *          Antenna information will be returned by @ref app_gap_aox_callback with cb_type @ref GAP_MSG_LE_AOX_READ_ANTENNA_INFORMATION.
 *
 *          Application can only call this API after stack is ready.
 *                 Explanation: If stack is ready, Application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
 *                              with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
 *
 * @return  Result of sending request.
 * @retval  GAP_CAUSE_SUCCESS: Send request success.
 * @retval  GAP_CAUSE_SEND_REQ_FAILED: Send request sent fail.
 *
 * <b>Example usage</b>
 * \code{.c}
   void test()
   {
        T_GAP_CAUSE cause = le_aox_read_antenna_information();
   }

  T_APP_RESULT app_gap_aox_callback(uint8_t cb_type, void *p_cb_data)
  {
      T_APP_RESULT result = APP_RESULT_SUCCESS;
      T_LE_AOX_CB_DATA *p_data = (T_LE_AOX_CB_DATA *)p_cb_data;

      switch (cb_type)
      {
      ......
      case GAP_MSG_LE_AOX_READ_ANTENNA_INFORMATION:
          APP_PRINT_INFO5("GAP_MSG_LE_AOX_READ_ANTENNA_INFORMATION: cause 0x%x, supported_switching_sampling_rates 0x%x, num_antennae %d, max_switching_pattern_length %d, max_cte_length %d",
                          p_data->p_le_aox_read_antenna_information_rsp->cause,
                          p_data->p_le_aox_read_antenna_information_rsp->supported_switching_sampling_rates,
                          p_data->p_le_aox_read_antenna_information_rsp->num_antennae,
                          p_data->p_le_aox_read_antenna_information_rsp->max_switching_pattern_length,
                          p_data->p_le_aox_read_antenna_information_rsp->max_cte_length
                        );
          break;
      ......
      }
    ......
    }
 * \endcode
 */
T_GAP_CAUSE le_aox_read_antenna_information(void);

/** End of GAP_LE_AOX_Exported_Functions
  * @}
  */

/** End of GAP_LE_AOX
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

#endif /* GAP_AOX_H */
