/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_cig_mgr.h
* @brief     Header file for GAP CIG
* @details
* @author
* @date      2021-07-12
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _CIG_MGR_H_
#define _CIG_MGR_H_

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#ifdef  __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "gap.h"
#include "ble_isoch_def.h"

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_CIG_MGR GAP LE CIG Mgr Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_CIG_MGR_Exported_Macros GAP LE CIG Mgr Exported Macros
  * @{
  */

/** @defgroup CIG_MGR_CIG_ID CIG Mgr CIG ID
  * @brief    Used to identify the CIG.
  * @{
  */
#define CIG_ID_MIN 0x00
#define CIG_ID_MAX 0xEF
/** End of CIG_MGR_CIG_ID
  * @}
  */

/** @defgroup CIG_MGR_CIS_ID CIG Mgr CIS ID
  * @brief    Used to identify a CIS.
  * @{
  */
#define CIS_ID_MIN 0x00
#define CIS_ID_MAX 0xEF
/** End of CIG_MGR_CIS_ID
  * @}
  */

/** @defgroup CIG_MGR_CALLBACK_MSG CIG Mgr Callback Msg
  * @brief    Used to identify CIG Manager Callback Message.
  * @{
  */
/** @brief Response type for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR. */
#define MSG_CIG_MGR_SETUP_DATA_PATH       0x01 //!<Response msg type for cig_mgr_setup_data_path
#define MSG_CIG_MGR_REMOVE_DATA_PATH      0x02 //!<Response msg type for cig_mgr_remove_data_path
#define MSG_CIG_MGR_READ_ISO_TX_SYNC      0x03 //!<Response msg type for cig_mgr_read_iso_tx_sync
#define MSG_CIG_MGR_READ_ISO_LINK_QUALITY 0x04 //!<Response msg type for cig_mgr_read_iso_link_quality

/** @brief Test command response type for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR. */
#define MSG_CIG_MGR_ISO_TEST_END          0x10 //!<Response msg type for cig_mgr_iso_test_end
#define MSG_CIG_MGR_TRANSMIT_TEST         0x11 //!<Response msg type for cig_mgr_transmit_test
#define MSG_CIG_MGR_RECEIVE_TEST          0x12 //!<Response msg type for cig_mgr_receive_test
#define MSG_CIG_MGR_READ_TEST_COUNTERS    0x13 //!<Response msg type for cig_mgr_read_test_counters

/** @brief Notification and response type for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR. */
#define MSG_CIG_MGR_CIS_ESTABLISHED_INFO  0x20 //!<Notification msg type for LE CIS Established
#define MSG_CIG_MGR_DISCONNECT            0x21 //!<Response msg type for cig_mgr_disconnect
#define MSG_CIG_MGR_DISCONNECT_INFO       0x22 //!<Notification msg type for LE CIS Disconnection

/** @brief Response type for @ref ISOCH_ROLE_INITIATOR. */
#define MSG_CIG_MGR_START_SETTING         0x30 //!<Response msg type for cig_mgr_start_setting
#define MSG_CIG_MGR_CREATE_CIS            0x31 //!<Response msg type for cig_mgr_create_cis_by_cig_id or cig_mgr_create_cis_by_cis_conn_handle
#define MSG_CIG_MGR_REMOVE_CIG            0x32 //!<Response msg type for cig_mgr_remove_cig

/** @brief Test command response type for @ref ISOCH_ROLE_INITIATOR. */
#define MSG_CIG_MGR_START_SETTING_TEST    0x3A //!<Response msg type for cig_mgr_start_setting_test

/** @brief Indication type for @ref ISOCH_ROLE_ACCEPTOR. */
#define MSG_CIG_MGR_CIS_REQUEST_IND       0x40 //!<Indication msg type for LE CIS Request
#define MSG_CIG_MGR_ACCEPT_CIS_INFO       0x41 //!<Response for cig_mgr_acceptor_accept_cis; Notification when return value of @ref MSG_CIG_MGR_CIS_REQUEST_IND is @ref APP_RESULT_ACCEPT
#define MSG_CIG_MGR_REJECT_CIS_INFO       0x42 //!<Response for cig_mgr_acceptor_reject_cis; Notification when return value of @ref MSG_CIG_MGR_CIS_REQUEST_IND is neither @ref APP_RESULT_ACCEPT nor APP_RESULT_PENDING
/** End of CIG_MGR_CALLBACK_MSG
  * @}
  */
/** End of GAP_LE_CIG_MGR_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_CIG_MGR_Exported_Types GAP LE CIG Mgr Exported Types
  * @{
  */

typedef struct
{
    uint16_t cause;
    uint16_t cis_conn_handle;
    uint8_t conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
    uint8_t data_path_adding_path;
} T_CIG_MGR_SETUP_DATA_PATH_RSP;

typedef struct
{
    uint16_t cause;
    uint16_t cis_conn_handle;
    uint8_t conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
    uint8_t data_path_removing_path;
} T_CIG_MGR_REMOVE_DATA_PATH_RSP;

typedef struct
{
    uint16_t cause;
    uint16_t cis_conn_handle;
    uint8_t conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
    uint16_t packet_sequence_number;  /**< The packet sequence number of an SDU. */
    uint32_t time_stamp;              /**< The CIG reference point of a transmitted SDU derived
                                           using the Controller's free running reference clock (in microseconds). */
    uint32_t time_offset;             /**< The time offset, in microseconds, that is associated with a transmitted SDU. */
} T_CIG_MGR_READ_ISO_TX_SYNC_RSP;

typedef struct
{
    uint16_t cause;
    uint16_t cis_conn_handle;
    uint8_t conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
    uint32_t tx_unacked_packets;       /**< Value of the Tx_UnACKed_Packets counter.
                                            When Incremented: The Link Layer does not receive an acknowledgment for a CIS
                                                              Data PDU that it transmitted at least once by its flush point. */
    uint32_t tx_flushed_packets;       /**< Value of the Tx_Flushed_Packets counter.
                                            When Incremented: The Link Layer does not transmit a specific payload by its flush point. */
    uint32_t tx_last_subevent_packets; /**< Value of the Tx_Last_Subevent_Packets counter for @ref ISOCH_ROLE_ACCEPTOR.
                                            When Incremented: The Link Layer transmits a CIS Data PDU in the last subevent of a CIS event. */
    uint32_t retransmitted_packets;    /**< Value of the Retransmitted_Packets counter.
                                            When Incremented: The Link Layer retransmits a CIS Data PDU. */
    uint32_t crc_error_packets;        /**< Value of the CRC_Error_Packets counter.
                                            When Incremented: The Link Layer receives a packet with a CRC error. */
    uint32_t rx_unreceived_packets;    /**< Value of the Rx_Unreceived_Packets counter.
                                            When Incremented: The Link Layer does not receive a specific payload by its flush point (on
                                                              a CIS) or the end of the event it is associated with (on a BIS). */
    uint32_t duplicate_packets;        /**< Value of the Duplicate_Packets counter.
                                            When Incremented: The Link Layer receives a retransmission of a CIS Data PDU. */
} T_CIG_MGR_READ_ISO_LINK_QUALITY_RSP;

typedef struct
{
    uint16_t cause;
    uint16_t cis_conn_handle;
    uint8_t conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
    uint32_t received_packet_count;    /**< Number in the Received_SDU_Count.
                                           The Received_SDU_Count shall be incremented by one for every valid received test
                                           ISO Data SDU containing a size in the expected range and an SDU counter that matches
                                           the expected value. */
    uint32_t missed_packet_count;      /**< Number in the Missed_SDU_Count.
                                           The Missed_SDU_Count shall be incremented by one for each SDU that is made up of
                                           one or more invalid or missing PDUs. */
    uint32_t failed_packet_count;      /**< Number in the Failed_SDU_Count.
                                           The Failed_SDU_Count shall be incremented by one for each valid received test SDU
                                           where the size or SDU counter does not match the expected size or value. */
} T_CIG_MGR_ISO_TEST_END_RSP;

typedef struct
{
    uint16_t cause;
    uint16_t cis_conn_handle;
    uint8_t conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
} T_CIG_MGR_TRANSMIT_TEST_RSP;

typedef struct
{
    uint16_t cause;
    uint16_t cis_conn_handle;
    uint8_t conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
} T_CIG_MGR_RECEIVE_TEST_RSP;

typedef struct
{
    uint16_t cause;
    uint16_t cis_conn_handle;
    uint8_t conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
    uint32_t received_packet_count;   /**< Number in the Received_SDU_Count.
                                           The Received_SDU_Count shall be incremented by one for every valid received test
                                           ISO Data SDU containing a size in the expected range and an SDU counter that matches
                                           the expected value. */
    uint32_t missed_packet_count;     /**< Number in the Missed_SDU_Count.
                                           The Missed_SDU_Count shall be incremented by one for each SDU that is made up of
                                           one or more invalid or missing PDUs. */
    uint32_t failed_packet_count;     /**< Number in the Failed_SDU_Count.
                                           The Failed_SDU_Count shall be incremented by one for each valid received test SDU
                                           where the size or SDU counter does not match the expected size or value. */
} T_CIG_MGR_READ_TEST_COUNTERS_RSP;

typedef struct
{
    uint16_t cause;
    uint8_t  conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
    uint16_t cis_conn_handle;         /**< Connection handle of the CIS. */
    uint32_t cig_sync_delay;          /**< The maximum time, in microseconds, for transmission of PDUs of all CISes in a CIG event. */
    uint32_t cis_sync_delay;          /**< The maximum time, in microseconds, for transmission of PDUs of the specified CIS in a CIG event. */
    uint32_t transport_latency_m_s;   /**< The actual transport latency, in microseconds, from Central to Peripheral. */
    uint32_t transport_latency_s_m;   /**< The actual transport latency, in microseconds, from Peripheral to Central. */
    uint8_t phy_m_s;                  /**< The transmitter PHY of packets from the Central. @ref T_GAP_PHYS_TYPE. */
    uint8_t phy_s_m;                  /**< The transmitter PHY of packets from the Peripheral. @ref T_GAP_PHYS_TYPE. */
    uint8_t nse;                      /**< Maximum number of subevents in each isochronous event. */
    uint8_t bn_m_s;                   /**< 0x00: No isochronous data from the Central to the Peripheral
                                           0x01 to 0x0F: The burst number for Central to Peripheral transmission */
    uint8_t bn_s_m;                   /**< 0x00: No isochronous data from the Peripheral to the Central
                                           0x01 to 0x0F: The burst number for Peripheral to Central transmission */
    uint8_t ft_m_s;                   /**< The flush timeout, in multiples of the ISO_Interval for the CIS, for each payload sent
                                           from the Central to the Peripheral. */
    uint8_t ft_s_m;                   /**< The flush timeout, in multiples of the ISO_Interval for the CIS, for each payload sent
                                           from the Peripheral to the Central. */
    uint16_t max_pdu_m_s;             /**< Maximum size, in octets, of the payload from Central to Peripheral. */
    uint16_t max_pdu_s_m;             /**< Maximum size, in octets, of the payload from Peripheral to Central. */
    uint16_t iso_interval;            /**< The time between two consecutive CIS anchor points.
                                           Range: 0x0004 to 0x0C80
                                           Time = N * 1.25 ms
                                           Time Range: 5 ms to 4 s. */
} T_CIG_MGR_CIS_ESTABLISHED_INFO;

typedef struct
{
    uint16_t cause;
    uint16_t cis_conn_handle;
    uint8_t conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
} T_CIG_MGR_DISCONNECT_RSP;

typedef struct
{
    uint16_t cause;
    uint16_t cis_conn_handle;
    uint8_t conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
} T_CIG_MGR_DISCONNECT_INFO;

typedef struct
{
    uint8_t       cis_id;
    uint16_t      cis_conn_handle;
} T_CIG_START_SETTING_INFO;

typedef struct
{
    uint16_t cause;
    uint8_t  cig_id;
    uint8_t  cis_count;
    T_CIG_START_SETTING_INFO  cis_info[8];
} T_CIG_MGR_START_SETTING_RSP;

typedef struct
{
    uint8_t       cig_id;
    uint8_t       cis_id;
    T_ISOCH_STATE state;            /**< CIS connection state. @ref T_ISOCH_STATE. */
    uint16_t      cis_conn_handle;
} T_CIS_CREATE_INFO;

typedef struct
{
    uint16_t          cause;
    uint8_t           cis_count;
    T_CIS_CREATE_INFO cis_info[8];
} T_CIG_MGR_CREATE_CIS_RSP;

typedef struct
{
    uint16_t cause;
    uint8_t cig_id;
} T_CIG_MGR_REMOVE_CIG_RSP;

typedef struct
{
    uint16_t cause;
    uint8_t  cig_id;
    uint8_t  cis_count;
    T_CIG_START_SETTING_INFO  cis_info[8];
} T_CIG_MGR_START_SETTING_TEST_RSP;

typedef struct
{
    uint16_t cis_conn_handle;
    uint8_t conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
} T_CIG_MGR_CIS_REQUEST_IND;

typedef struct
{
    uint16_t cause;
    uint16_t cis_conn_handle;
    uint8_t conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
} T_CIG_MGR_ACCEPT_CIS_INFO;

typedef struct
{
    uint16_t cause;
    uint16_t cis_conn_handle;
    uint8_t conn_id;
    uint8_t cig_id;
    uint8_t cis_id;
} T_CIG_MGR_REJECT_CIS_INFO;

/** @brief  GAP LE CIG Mgr Callback Data. */
typedef union
{
    T_CIG_MGR_SETUP_DATA_PATH_RSP          *p_cig_mgr_setup_data_path_rsp;
    T_CIG_MGR_REMOVE_DATA_PATH_RSP         *p_cig_mgr_remove_data_path_rsp;
    T_CIG_MGR_READ_ISO_TX_SYNC_RSP         *p_cig_mgr_read_iso_tx_sync_rsp;
    T_CIG_MGR_READ_ISO_LINK_QUALITY_RSP    *p_cig_mgr_read_iso_link_quality_rsp;

    T_CIG_MGR_ISO_TEST_END_RSP             *p_cig_mgr_iso_test_end_rsp;
    T_CIG_MGR_TRANSMIT_TEST_RSP            *p_cig_mgr_transmit_test_rsp;
    T_CIG_MGR_RECEIVE_TEST_RSP             *p_cig_mgr_receive_test_rsp;
    T_CIG_MGR_READ_TEST_COUNTERS_RSP       *p_cig_mgr_read_test_counters_rsp;

    T_CIG_MGR_CIS_ESTABLISHED_INFO         *p_cig_mgr_cis_established_info;
    T_CIG_MGR_DISCONNECT_RSP               *p_cig_mgr_disconnect_rsp;
    T_CIG_MGR_DISCONNECT_INFO              *p_cig_mgr_disconnect_info;

    T_CIG_MGR_START_SETTING_RSP            *p_cig_mgr_start_setting_rsp;
    T_CIG_MGR_CREATE_CIS_RSP               *p_cig_mgr_create_cis_rsp;
    T_CIG_MGR_REMOVE_CIG_RSP               *p_cig_mgr_remove_cig_rsp;
    T_CIG_MGR_START_SETTING_TEST_RSP       *p_cig_mgr_start_setting_test_rsp;

    T_CIG_MGR_CIS_REQUEST_IND              *p_cig_mgr_cis_request_ind;
    T_CIG_MGR_ACCEPT_CIS_INFO              *p_cig_mgr_accept_cis_info;
    T_CIG_MGR_REJECT_CIS_INFO              *p_cig_mgr_reject_cis_info;
} T_CIG_MGR_CB_DATA;

typedef struct
{
    uint8_t cis_id;
    uint8_t cig_id;
} T_CIS_INFO;

typedef struct
{
    uint32_t sdu_interval_m_s;  /**< The interval, in microseconds, of periodic SDUs from the Central's Host for all the CISes in the CIG.
                                     This parameter shall be ignored for all CISes that are unidirectional from Peripheral to Central.
                                     Range: 0x0000FF to 0x0FFFFF */
    uint32_t sdu_interval_s_m;  /**< The interval, in microseconds, of periodic SDUs from the Peripheral's Host for all the CISes in the
                                     CIG.
                                     This parameter shall be ignored for all CISes that are unidirectional from Central to Peripheral.
                                     Range: 0x0000FF to 0x0FFFFF */
    uint8_t  sca;               /**< The worst-case sleep clock accuracy of all the Peripherals that will participate in the CIG.
                                     0x00: 251 ppm to 500 ppm
                                     0x01: 151 ppm to 250 ppm
                                     0x02: 101 ppm to 150 ppm
                                     0x03: 76 ppm to 100 ppm
                                     0x04: 51 ppm to 75 ppm
                                     0x05: 31 ppm to 50 ppm
                                     0x06: 21 ppm to 30 ppm
                                     0x07: 0 ppm to 20 ppm */
    uint8_t  packing;           /**< Indicate the preferred method of arranging subevents of multiple CISes.
                                     0x00: Sequential
                                     0x01: Interleaved */
    uint8_t  framing;           /**< Indicate the format of the CIS Data PDUs.
                                     0x00: Unframed
                                     0x01: Framed */
    uint16_t latency_m_s;       /**< Maximum transport latency, in milliseconds, from the Central's Controller
                                     to the Peripheral's Controller.
                                     Range: 0x0005 to 0x0FA0 */
    uint16_t latency_s_m;       /**< Maximum transport latency, in milliseconds, from the Peripheral's Controller
                                     to the Central's Controller.
                                     Range: 0x0005 to 0x0FA0 */
} T_CIG_CFG_PARAM;

typedef struct
{
    uint16_t max_sdu_m_s;  /**< Maximum size, in octets, of the payload from the Central's Host.
                                If the CIS is unidirectional from Peripheral to Central, this parameter shall be set to 0. */
    uint16_t max_sdu_s_m;  /**< Maximum size, in octets, of the payload from the Peripheral's Host.
                                If the CIS is unidirectional from Central to Peripheral, this parameter shall be set to 0. */
    uint8_t  phy_m_s;      /**< Identify which PHY to use for transmission from the Central to the Peripheral.
                                bit 0: The transmitter PHY of packets from the Peripheral is LE 1M
                                bit 1: The transmitter PHY of packets from the Peripheral is LE 2M
                                bit 2: The transmitter PHY of packets from the Peripheral is LE Coded */
    uint8_t  phy_s_m;      /**< Identify which PHY to use for transmission from the Peripheral to the Central.
                                bit 0: The transmitter PHY of packets from the Peripheral is LE 1M
                                bit 1: The transmitter PHY of packets from the Peripheral is LE 2M
                                bit 2: The transmitter PHY of packets from the Peripheral is LE Coded */
    uint8_t  rtn_m_s;      /**< Number of times every CIS Data PDU should be retransmitted from the Central to the Peripheral. */
    uint8_t  rtn_s_m;      /**< Number of times every CIS Data PDU should be retransmitted from the Peripheral to the Central. */
} T_CIS_CFG_PARAM;

typedef struct
{
    uint32_t sdu_interval_m_s;  /**< The interval, in microseconds, of periodic SDUs from the Central's Host.
                                     Range: 0x0000FF to 0x0FFFFF */
    uint32_t sdu_interval_s_m;  /**< The interval, in microseconds, of periodic SDUs from the Peripheral's Host.
                                     Range: 0x0000FF to 0x0FFFFF */
    uint8_t  ft_m_s;            /**< The flush timeout in multiples of ISO_Interval for each payload sent
                                     from the Central to Peripheral.
                                     Range: 0x01 to 0xFF */
    uint8_t  ft_s_m;            /**< The flush timeout in multiples of ISO_Interval for each payload sent
                                     from the Peripheral to Central.
                                     Range: 0x01 to 0xFF */
    uint16_t iso_interval;      /**< Time between consecutive CIS anchor points.
                                     Range: 0x0004 to 0x0C80
                                     Time = N * 1.25 ms
                                     Time Range: 5 ms to 4 s */
    uint8_t sca;                /**< The worst-case sleep clock accuracy of all the Peripherals that will participate in the CIG.
                                     0x00: 251 ppm to 500 ppm
                                     0x01: 151 ppm to 250 ppm
                                     0x02: 101 ppm to 150 ppm
                                     0x03: 76 ppm to 100 ppm
                                     0x04: 51 ppm to 75 ppm
                                     0x05: 31 ppm to 50 ppm
                                     0x06: 21 ppm to 30 ppm
                                     0x07: 0 ppm to 20 ppm */
    uint8_t  packing;           /**< Indicate the preferred method of arranging subevents of multiple CISes.
                                     0x00: Sequential
                                     0x01: Interleaved */
    uint8_t  framing;           /**< Indicate the format of the CIS Data PDUs.
                                     0x00: Unframed
                                     0x01: Framed */
} T_CIG_CFG_PARAM_TEST;

typedef struct
{
    uint8_t             nse;          /**< Maximum number of subevents in each CIS event.
                                           Range: 0x01 to 0x1F */
    uint16_t            max_sdu_m_s;  /**< Maximum size, in octets, of the payload from
                                           the Central's Host.
                                           If the CIS is unidirectional from Peripheral
                                           to Central, this parameter shall be set to 0. */
    uint16_t            max_sdu_s_m;  /**< Maximum size, in octets, of the payload from
                                           the Peripheral's Host.
                                           If the CIS is unidirectional from Central to
                                           Peripheral, this parameter shall be set to 0. */
    uint16_t            max_pdu_m_s;  /**< Maximum size, in octets, of the payload from
                                           the Central's Link Layer to the Peripheral's
                                           Link Layer.
                                           Range: 0x0000 to 0x00FB */
    uint16_t            max_pdu_s_m;  /**< Maximum size, in octets, of the payload from
                                           the Peripheral's Link Layer to the Central's
                                           Link Layer.
                                           Range: 0x0000 to 0x00FB */
    uint8_t             phy_m_s;      /**< Identify which PHY to use for transmission from
                                           the Central to the Peripheral.
                                           bit 0: The transmitter PHY of packets from the Peripheral is LE 1M
                                           bit 1: The transmitter PHY of packets from the Peripheral is LE 2M
                                           bit 2: The transmitter PHY of packets from the Peripheral is LE Coded */
    uint8_t             phy_s_m;      /**< Identify which PHY to use for transmission from
                                           the Peripheral to the Central.
                                           bit 0: The transmitter PHY of packets from the Peripheral is LE 1M
                                           bit 1: The transmitter PHY of packets from the Peripheral is LE 2M
                                           bit 2: The transmitter PHY of packets from the Peripheral is LE Coded */
    uint8_t             bn_m_s;       /**< Identify the burst number for Central to Peripheral.
                                           0x00: No isochronous data from the Central to the Peripheral
                                           0x01 to 0x0F: The burst number for Central to Peripheral transmission */
    uint8_t             bn_s_m;       /**< Identify the burst number for Peripheral to Central.
                                           0x00: No isochronous data from the Peripheral to the Central
                                           0x01 to 0x0F: The burst number for Peripheral to Central transmission */
} T_CIS_CFG_PARAM_TEST;
/** End of GAP_LE_CIG_MGR_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_CIG_MGR_Exported_Functions GAP LE CIG Mgr Exported Functions
  * @brief
  * @{
  */

/**
  * @brief     Callback for GAP CIG Mgr @ref ISOCH_ROLE_INITIATOR to notify app
  *
  * @param[in] cig_id    Identifier of a CIG.
  * @param[in] cb_type   Callback msy type @ref ISOCH_ROLE_INITIATOR in @ref CIG_MGR_CALLBACK_MSG.
  * @param[in] p_cb_data Point to callback data @ref T_CIG_MGR_CB_DATA.
  * @retval    result    @ref T_APP_RESULT
  */
typedef T_APP_RESULT(*P_FUN_LE_CIG_MGR_CB)(uint8_t cig_id, uint8_t cb_type, void *p_cb_data);

/**
  * @brief     Callback for GAP CIG Mgr @ref ISOCH_ROLE_ACCEPTOR to notify app
  *
  * @param[in] cb_type   Callback msy type @ref ISOCH_ROLE_ACCEPTOR in @ref CIG_MGR_CALLBACK_MSG.
  * @param[in] p_cb_data Point to callback data @ref T_CIG_MGR_CB_DATA.
  * @retval    result    @ref T_APP_RESULT
  */
typedef T_APP_RESULT(*P_FUN_LE_CIG_MGR_ACCEPTOR_CB)(uint8_t cb_type, void *p_cb_data);

/**
 * @brief       Initialize the number of CIG and CIS.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              NOTE: If CIS will be used, @ref GAP_PARAM_CIS_HOST_SUPPORT by calling @ref le_set_gap_param()
 *                    should be configured as True before @ref gap_start_bt_stack is invoked.
 *
 *              NOTE: @ref gap_register_direct_cb should be invoked to receive ISO Data
 *                    via cb_type @ref BT_DIRECT_MSG_ISO_DATA_IND.
 *
 *
 * @param[in]   cig_num     CIG number for @ref ISOCH_ROLE_INITIATOR.
 * @param[in]   cis_num     CIS number for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 * @return      GAP Operation result.
 * @retval      GAP_CAUSE_SUCCESS       GAP operation success.
 * @retval      GAP_CAUSE_NO_RESOURCE   Operation failure, memory acquisition failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool cis_flag = true;

        // e.g.  @ref ISOCH_ROLE_ACCEPTOR
        T_GAP_CAUSE cause = cig_mgr_init(0, 1);

        le_set_gap_param(GAP_PARAM_CIS_HOST_SUPPORT, sizeof(cis_flag), &cis_flag);
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_init(uint8_t cig_num, uint8_t cis_num);

/**
 * @brief       Get connection ID by CIS connection handle.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 *
 * @param[in]       cis_conn_handle  Connection handle of the CIS.
 * @param[in, out]  p_conn_id        Connection ID.
 * @return          Get result.
 * @retval          True             Success
 * @retval          False            Get failed
 */
bool cig_mgr_get_conn_id(uint16_t cis_conn_handle, uint8_t *p_conn_id);

/**
 * @brief       Get CIG ID and CIS ID by CIS connection handle.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 *
 * @param[in]       cis_conn_handle  Connection handle of the CIS.
 * @param[in, out]  p_info           CIG ID and CIS ID.
 * @return          Get result.
 * @retval          True             Success
 * @retval          False            Get failed
 */
bool cig_mgr_get_cis_info(uint16_t cis_conn_handle, T_CIS_INFO *p_info);

/**
 * @brief       Get information about specified CIS connection handle.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 *
 * @param[in]       cis_conn_handle  Connection handle of the CIS.
 * @param[in, out]  p_info           Information about specified CIS connection handle.
 * @return          Get result.
 * @retval          True             Success
 * @retval          False            Get failed
 */
bool cig_mgr_get_isoch_info(uint16_t cis_conn_handle, T_ISOCH_INFO *p_info);

/**
 * @brief       Identify and create the isochronous data path for CIS.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              If sending request operation is success, the result of creating isochronous data path will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb for @ref ISOCH_ROLE_INITIATOR
 *              or @ref app_isoc_cis_acceptor_cb for @ref ISOCH_ROLE_ACCEPTOR
 *              with cb_type @ref MSG_CIG_MGR_SETUP_DATA_PATH.
 *
 * @param[in]   cis_conn_handle   Connection handle of the CIS.
 * @param[in]   path_direction    Specify the direction for which the data path is being configured. @ref LE_ISOC_DEF_SETUP_DATA_PATH_DATA_PATH_DIRECTION.
 * @param[in]   path_id           Specify the data transport path used.
 *                                0x00: HCI. @ref DATA_PATH_ID_HCI.
 *                                0x01 to 0xFE: Logical_Channel_Number. The meaning of the logical channel is vendor-specific.
 * @param[in]   codec_id          Specify the coding format used over the air.
 *                                Octet 0: See Assigned Numbers for Coding Format
 *                                Octets 1 to 2: Company ID, see Assigned Numbers for Company Identifier. Shall be ignored if octet 0 is not 0xFF.
 *                                Octets 3 to 4: Vendor-defined codec ID. Shall be ignored if octet 0 is not 0xFF.
 * @param[in]   controller_delay  Controller delay in microseconds.
 *                                Range: 0x000000 to 0x3D0900
 *                                Time range: 0 s to 4 s
 * @param[in]   codec_len         Length of codec configuration.
 * @param[in]   p_codec_data      Specify codec-specific configuration information for the specified direction.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_setup_data_path(cis_conn_handle, path_direction, path_id, codec_id, controller_delay, codec_len, p_codec_data);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_SETUP_DATA_PATH:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_SETUP_DATA_PATH: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_setup_data_path_rsp->cause,
                                p_data->p_cig_mgr_setup_data_path_rsp->conn_id,
                                p_data->p_cig_mgr_setup_data_path_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_setup_data_path_rsp->cig_id,
                                p_data->p_cig_mgr_setup_data_path_rsp->cis_id);
            }
            break;
        ......
    }

    T_APP_RESULT app_isoc_cis_acceptor_cb(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_SETUP_DATA_PATH:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_SETUP_DATA_PATH: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_setup_data_path_rsp->cause,
                                p_data->p_cig_mgr_setup_data_path_rsp->conn_id,
                                p_data->p_cig_mgr_setup_data_path_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_setup_data_path_rsp->cig_id,
                                p_data->p_cig_mgr_setup_data_path_rsp->cis_id);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_setup_data_path(uint16_t cis_conn_handle, uint8_t path_direction,
                                    uint8_t path_id,
                                    uint8_t codec_id[5], uint32_t controller_delay,
                                    uint8_t codec_len, uint8_t *p_codec_data);

/**
 * @brief       Remove the input and/or output data path(s) associated with CIS.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              If sending request operation is success, the result of removing data path will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb for @ref ISOCH_ROLE_INITIATOR
 *              or @ref app_isoc_cis_acceptor_cb for @ref ISOCH_ROLE_ACCEPTOR
 *              with cb_type @ref MSG_CIG_MGR_REMOVE_DATA_PATH.
 *
 * @param[in]   cis_conn_handle   Connection handle of the CIS.
 * @param[in]   path_direction    Specify which directions are to have the data path removed. @ref LE_ISOC_DEF_DATA_PATH_DIRECTION_FLAG.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_setup_data_path(cis_conn_handle, path_direction);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_REMOVE_DATA_PATH:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_REMOVE_DATA_PATH: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_remove_data_path_rsp->cause,
                                p_data->p_cig_mgr_remove_data_path_rsp->conn_id,
                                p_data->p_cig_mgr_remove_data_path_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_remove_data_path_rsp->cig_id,
                                p_data->p_cig_mgr_remove_data_path_rsp->cis_id);
            }
            break;
        ......
    }

    T_APP_RESULT app_isoc_cis_acceptor_cb(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_REMOVE_DATA_PATH:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_REMOVE_DATA_PATH: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_remove_data_path_rsp->cause,
                                p_data->p_cig_mgr_remove_data_path_rsp->conn_id,
                                p_data->p_cig_mgr_remove_data_path_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_remove_data_path_rsp->cig_id,
                                p_data->p_cig_mgr_remove_data_path_rsp->cis_id);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_remove_data_path(uint16_t cis_conn_handle, uint8_t path_direction);

/**
 * @brief       Read the TX_Time_Stamp and Time_Offset of a transmitted SDU identified by the Packet_Sequence_Number on a CIS.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              If sending request operation is success, the reading result will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb for @ref ISOCH_ROLE_INITIATOR
 *              or @ref app_isoc_cis_acceptor_cb for @ref ISOCH_ROLE_ACCEPTOR
 *              with cb_type @ref MSG_CIG_MGR_READ_ISO_TX_SYNC.
 *
 * @param[in]   cis_conn_handle   Connection handle of the CIS.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_read_iso_tx_sync(cis_conn_handle);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_READ_ISO_TX_SYNC:
            {
                APP_PRINT_INFO8("MSG_CIG_MGR_READ_ISO_TX_SYNC: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x, packet_sequence_number 0x%x, time_stamp 0x%x, time_offset 0x%x",
                                p_data->p_cig_mgr_read_iso_tx_sync_rsp->cause, p_data->p_cig_mgr_read_iso_tx_sync_rsp->conn_id,
                                p_data->p_cig_mgr_read_iso_tx_sync_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_read_iso_tx_sync_rsp->cig_id, p_data->p_cig_mgr_read_iso_tx_sync_rsp->cis_id,
                                p_data->p_cig_mgr_read_iso_tx_sync_rsp->packet_sequence_number,
                                p_data->p_cig_mgr_read_iso_tx_sync_rsp->time_stamp,
                                p_data->p_cig_mgr_read_iso_tx_sync_rsp->time_offset);
            }
            break;
        ......
    }

    T_APP_RESULT app_isoc_cis_acceptor_cb(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_READ_ISO_TX_SYNC:
            {
                APP_PRINT_INFO8("MSG_CIG_MGR_READ_ISO_TX_SYNC: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x, packet_sequence_number 0x%x, time_stamp 0x%x, time_offset 0x%x",
                                p_data->p_cig_mgr_read_iso_tx_sync_rsp->cause, p_data->p_cig_mgr_read_iso_tx_sync_rsp->conn_id,
                                p_data->p_cig_mgr_read_iso_tx_sync_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_read_iso_tx_sync_rsp->cig_id, p_data->p_cig_mgr_read_iso_tx_sync_rsp->cis_id,
                                p_data->p_cig_mgr_read_iso_tx_sync_rsp->packet_sequence_number,
                                p_data->p_cig_mgr_read_iso_tx_sync_rsp->time_stamp,
                                p_data->p_cig_mgr_read_iso_tx_sync_rsp->time_offset);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_read_iso_tx_sync(uint16_t cis_conn_handle);

/**
 * @brief       Read the values of various counters related to link quality.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              If sending request operation is success, the reading result will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb for @ref ISOCH_ROLE_INITIATOR
 *              or @ref app_isoc_cis_acceptor_cb for @ref ISOCH_ROLE_ACCEPTOR
 *              with cb_type @ref MSG_CIG_MGR_READ_ISO_LINK_QUALITY.
 *
 * @param[in]   cis_conn_handle   Connection handle of the CIS.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_read_iso_link_quality(cis_conn_handle);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_READ_ISO_LINK_QUALITY:
            {
                APP_PRINT_INFO8("MSG_CIG_MGR_READ_ISO_LINK_QUALITY: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x, tx_unacked_packets 0x%x, tx_flushed_packets 0x%x, tx_last_subevent_packets 0x%x",
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->cause,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->conn_id,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->cig_id,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->cis_id,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->tx_unacked_packets,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->tx_flushed_packets,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->tx_last_subevent_packets);
                APP_PRINT_INFO4("MSG_CIG_MGR_READ_ISO_LINK_QUALITY: retransmitted_packets 0x%x, crc_error_packets %d, rx_unreceived_packets 0x%x, duplicate_packets 0x%x",
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->retransmitted_packets,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->crc_error_packets,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->rx_unreceived_packets,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->duplicate_packets);
            }
            break;
        ......
    }

    T_APP_RESULT app_isoc_cis_acceptor_cb(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_READ_ISO_LINK_QUALITY:
            {
                APP_PRINT_INFO8("MSG_CIG_MGR_READ_ISO_LINK_QUALITY: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x, tx_unacked_packets 0x%x, tx_flushed_packets 0x%x, tx_last_subevent_packets 0x%x",
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->cause,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->conn_id,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->cig_id,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->cis_id,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->tx_unacked_packets,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->tx_flushed_packets,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->tx_last_subevent_packets);
                APP_PRINT_INFO4("MSG_CIG_MGR_READ_ISO_LINK_QUALITY: retransmitted_packets 0x%x, crc_error_packets %d, rx_unreceived_packets 0x%x, duplicate_packets 0x%x",
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->retransmitted_packets,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->crc_error_packets,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->rx_unreceived_packets,
                                p_data->p_cig_mgr_read_iso_link_quality_rsp->duplicate_packets);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_read_iso_link_quality(uint16_t cis_conn_handle);

/**
 * @brief       Terminate a CIS connection.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              If sending request operation is success, the response of request will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb for @ref ISOCH_ROLE_INITIATOR
 *              or @ref app_isoc_cis_acceptor_cb for @ref ISOCH_ROLE_ACCEPTOR
 *              with cb_type @ref MSG_CIG_MGR_DISCONNECT.
 *              The notification of disconnection will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb for @ref ISOCH_ROLE_INITIATOR
 *              or @ref app_isoc_cis_acceptor_cb for @ref ISOCH_ROLE_ACCEPTOR
 *              with cb_type @ref MSG_CIG_MGR_DISCONNECT_INFO.
 *
 *              NOTE: When CIS connection is disconnected with cb_type @ref MSG_CIG_MGR_DISCONNECT_INFO:
 *                    If role is @ref ISOCH_ROLE_INITIATOR for specified CIS, the handle and the associated data paths of that CIS
 *                    shall remain valid.
 *                    If role is @ref ISOCH_ROLE_ACCEPTOR for specified CIS, the handle and the associated data path of
 *                    that CIS will be deleted.
 *
 * @param[in]   cis_conn_handle   Connection handle of the CIS.
 * @param[in]   reason            Indicate the reason for ending the connection.
 *                                0x05, 0x13 to 0x15, 0x1A, 0x29, 0x3B in @ref BT_HCI_ERROR.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_disconnect(cis_conn_handle, reason);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_DISCONNECT:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_DISCONNECT: conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x, cause 0x%x",
                                p_data->p_cig_mgr_disconnect_rsp->conn_id, p_data->p_cig_mgr_disconnect_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_disconnect_rsp->cig_id, p_data->p_cig_mgr_disconnect_rsp->cis_id,
                                p_data->p_cig_mgr_disconnect_rsp->cause);
            }
            break;

        case MSG_CIG_MGR_DISCONNECT_INFO:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_DISCONNECT_INFO: conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x, cause 0x%x",
                                p_data->p_cig_mgr_disconnect_info->conn_id, p_data->p_cig_mgr_disconnect_info->cis_conn_handle,
                                p_data->p_cig_mgr_disconnect_info->cig_id, p_data->p_cig_mgr_disconnect_info->cis_id,
                                p_data->p_cig_mgr_disconnect_info->cause);
            }
            break;
        ......
    }

    T_APP_RESULT app_isoc_cis_acceptor_cb(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_DISCONNECT:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_DISCONNECT: conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x, cause 0x%x",
                                p_data->p_cig_mgr_disconnect_rsp->conn_id, p_data->p_cig_mgr_disconnect_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_disconnect_rsp->cig_id,  p_data->p_cig_mgr_disconnect_rsp->cis_id,
                                p_data->p_cig_mgr_disconnect_rsp->cause);
            }
            break;

        case MSG_CIG_MGR_DISCONNECT_INFO:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_DISCONNECT_INFO: conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x, cause 0x%x",
                                p_data->p_cig_mgr_disconnect_info->conn_id, p_data->p_cig_mgr_disconnect_info->cis_conn_handle,
                                p_data->p_cig_mgr_disconnect_info->cig_id, p_data->p_cig_mgr_disconnect_info->cis_id,
                                p_data->p_cig_mgr_disconnect_info->cause);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_disconnect(uint16_t cis_conn_handle, uint8_t reason);

/**
 * @brief       Terminate the ISO Transmit and/or Receive Test mode for a CIS.
 *
 *              NOTE: This function is used in ISO Test mode and only for testing purposes for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              If sending request operation is success, the result of termination will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb for @ref ISOCH_ROLE_INITIATOR
 *              or @ref app_isoc_cis_acceptor_cb for @ref ISOCH_ROLE_ACCEPTOR
 *              with cb_type @ref MSG_CIG_MGR_ISO_TEST_END.
 *
 * @param[in]   cis_conn_handle   Connection handle of the CIS.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_iso_test_end(cis_conn_handle);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_ISO_TEST_END:
            {
                APP_PRINT_INFO6("MSG_CIG_MGR_ISO_TEST_END: cause 0x%x, cis_conn_handle 0x%x, conn_id %d, cig_id 0x%x, cis_id 0x%x, received_packet_count 0x%x, missed_packet_count 0x%x, failed_packet_count 0x%x",
                                p_data->p_cig_mgr_iso_test_end_rsp->cause, p_data->p_cig_mgr_iso_test_end_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_iso_test_end_rsp->conn_id, p_data->p_cig_mgr_iso_test_end_rsp->cig_id,
                                p_data->p_cig_mgr_iso_test_end_rsp->cis_id, p_data->p_cig_mgr_iso_test_end_rsp->received_packet_count,
                                p_data->p_cig_mgr_iso_test_end_rsp->missed_packet_count,
                                p_data->p_cig_mgr_iso_test_end_rsp->failed_packet_count);
            }
            break;
        ......
    }

    T_APP_RESULT app_isoc_cis_acceptor_cb(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_ISO_TEST_END:
            {
                APP_PRINT_INFO8("MSG_CIG_MGR_ISO_TEST_END: cause 0x%x, cis_conn_handle 0x%x, conn_id %d, cig_id 0x%x, cis_id 0x%x, received_packet_count 0x%x, missed_packet_count 0x%x, failed_packet_count 0x%x",
                                p_data->p_cig_mgr_iso_test_end_rsp->cause, p_data->p_cig_mgr_iso_test_end_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_iso_test_end_rsp->conn_id, p_data->p_cig_mgr_iso_test_end_rsp->cig_id,
                                p_data->p_cig_mgr_iso_test_end_rsp->cis_id, p_data->p_cig_mgr_iso_test_end_rsp->received_packet_count,
                                p_data->p_cig_mgr_iso_test_end_rsp->missed_packet_count,
                                p_data->p_cig_mgr_iso_test_end_rsp->failed_packet_count);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_iso_test_end(uint16_t cis_conn_handle);

/**
 * @brief       Configure an established CIS, and transmit test payloads which are generated by the Controller.
 *
 *              NOTE: This function is used in ISO Test mode and only for testing purposes for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              If sending request operation is success, the result will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb for @ref ISOCH_ROLE_INITIATOR
 *              or @ref app_isoc_cis_acceptor_cb for @ref ISOCH_ROLE_ACCEPTOR
 *              with cb_type @ref MSG_CIG_MGR_TRANSMIT_TEST.
 *
 * @param[in]   cis_conn_handle   Connection handle of the CIS.
 * @param[in]   payload_type      Define the configuration of SDUs in the payload.
 *                                0x00: Zero length payload
 *                                0x01: Variable length payload
 *                                0x02: Maximum length payload
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_transmit_test(cis_conn_handle, payload_type);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_TRANSMIT_TEST:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_TRANSMIT_TEST: cause 0x%x, cis_conn_handle 0x%x, conn_id %d, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_transmit_test_rsp->cause, p_data->p_cig_mgr_transmit_test_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_transmit_test_rsp->conn_id, p_data->p_cig_mgr_transmit_test_rsp->cig_id,
                                p_data->p_cig_mgr_transmit_test_rsp->cis_id);
            }
            break;
        ......
    }

    T_APP_RESULT app_isoc_cis_acceptor_cb(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_TRANSMIT_TEST:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_TRANSMIT_TEST: cause 0x%x, cis_conn_handle 0x%x, conn_id %d, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_transmit_test_rsp->cause, p_data->p_cig_mgr_transmit_test_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_transmit_test_rsp->conn_id, p_data->p_cig_mgr_transmit_test_rsp->cig_id,
                                p_data->p_cig_mgr_transmit_test_rsp->cis_id);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_transmit_test(uint16_t cis_conn_handle, uint8_t payload_type);

/**
 * @brief       Configure an established CIS to receive payloads.
 *
 *              NOTE: This function is used in ISO Test mode and only for testing purposes for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              If sending request operation is success, the result will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb for @ref ISOCH_ROLE_INITIATOR
 *              or @ref app_isoc_cis_acceptor_cb for @ref ISOCH_ROLE_ACCEPTOR
 *              with cb_type @ref MSG_CIG_MGR_RECEIVE_TEST.
 *
 * @param[in]   cis_conn_handle   Connection handle of the CIS.
 * @param[in]   payload_type      Define the configuration of SDUs in the payload.
 *                                0x00: Zero length payload
 *                                0x01: Variable length payload
 *                                0x02: Maximum length payload
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_receive_test(cis_conn_handle, payload_type);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_RECEIVE_TEST:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_RECEIVE_TEST: cause 0x%x, cis_conn_handle 0x%x, conn_id %d, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_receive_test_rsp->cause, p_data->p_cig_mgr_receive_test_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_receive_test_rsp->conn_id,
                                p_data->p_cig_mgr_receive_test_rsp->cig_id,
                                p_data->p_cig_mgr_receive_test_rsp->cis_id);
            }
            break;
        ......
    }

    T_APP_RESULT app_isoc_cis_acceptor_cb(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_RECEIVE_TEST:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_RECEIVE_TEST: cause 0x%x, cis_conn_handle 0x%x, conn_id %d, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_receive_test_rsp->cause, p_data->p_cig_mgr_receive_test_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_receive_test_rsp->conn_id,
                                p_data->p_cig_mgr_receive_test_rsp->cig_id,
                                p_data->p_cig_mgr_receive_test_rsp->cis_id);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_receive_test(uint16_t cis_conn_handle, uint8_t payload_type);

/**
 * @brief       Read the test counters in the Controller which is configured in ISO Receive Test mode for a CIS.
 *
 *              NOTE: This function is used in ISO Test mode and only for testing purposes for @ref ISOCH_ROLE_INITIATOR and @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              If sending request operation is success, the result will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb for @ref ISOCH_ROLE_INITIATOR
 *              or @ref app_isoc_cis_acceptor_cb for @ref ISOCH_ROLE_ACCEPTOR
 *              with cb_type @ref MSG_CIG_MGR_READ_TEST_COUNTERS.
 *
 * @param[in]   cis_conn_handle   Connection handle of the CIS.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_read_test_counters(cis_conn_handle);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_READ_TEST_COUNTERS:
            {
                APP_PRINT_INFO8("MSG_CIG_MGR_READ_TEST_COUNTERS: cause 0x%x, cis_conn_handle 0x%x, conn_id %d, cig_id 0x%x, cis_id 0x%x, received_packet_count 0x%x, missed_packet_count 0x%x, failed_packet_count 0x%x",
                                p_data->p_cig_mgr_read_test_counters_rsp->cause,
                                p_data->p_cig_mgr_read_test_counters_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_read_test_counters_rsp->conn_id,
                                p_data->p_cig_mgr_read_test_counters_rsp->cig_id,
                                p_data->p_cig_mgr_read_test_counters_rsp->cis_id,
                                p_data->p_cig_mgr_read_test_counters_rsp->received_packet_count,
                                p_data->p_cig_mgr_read_test_counters_rsp->missed_packet_count,
                                p_data->p_cig_mgr_read_test_counters_rsp->failed_packet_count);
            }
            break;
        ......
    }

    T_APP_RESULT app_isoc_cis_acceptor_cb(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_READ_TEST_COUNTERS:
            {
                APP_PRINT_INFO8("MSG_CIG_MGR_READ_TEST_COUNTERS: cause 0x%x, cis_conn_handle 0x%x, conn_id %d, cig_id 0x%x, cis_id 0x%x, received_packet_count 0x%x, missed_packet_count 0x%x, failed_packet_count 0x%x",
                                p_data->p_cig_mgr_read_test_counters_rsp->cause,
                                p_data->p_cig_mgr_read_test_counters_rsp->cis_conn_handle,
                                p_data->p_cig_mgr_read_test_counters_rsp->conn_id,
                                p_data->p_cig_mgr_read_test_counters_rsp->cig_id,
                                p_data->p_cig_mgr_read_test_counters_rsp->cis_id,
                                p_data->p_cig_mgr_read_test_counters_rsp->received_packet_count,
                                p_data->p_cig_mgr_read_test_counters_rsp->missed_packet_count,
                                p_data->p_cig_mgr_read_test_counters_rsp->failed_packet_count);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_read_test_counters(uint16_t cis_conn_handle);

/**
 * @brief       Register CIG and specified callback.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR.
 *
 * @param[in]   cig_id   Identifier of a CIG.
 * @param[in]   cb_pfn   Callback.
 *
 * @return GAP operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others             GAP operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_reg_cig(cig_id, app_isoc_cis_initiator_cig_cb);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        ......
        case MSG_CIG_MGR_REMOVE_CIG:
            {
                APP_PRINT_INFO2("MSG_CIG_MGR_REMOVE_CIG: cause 0x%x, cig_id 0x%x",
                                p_data->p_cig_mgr_remove_cig_rsp->cause, p_data->p_cig_mgr_remove_cig_rsp->cig_id);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_reg_cig(uint8_t cig_id, P_FUN_LE_CIG_MGR_CB cb_pfn);

/**
 * @brief       Add CIS for specified CIG.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR.
 *
 * @param[in]   cig_id   Identifier of a CIG.
 * @param[in]   cis_id   Used to identify a CIS.
 *
 * @return GAP operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others             GAP operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        // After registration of CIG by @ref cig_mgr_reg_cig
        T_GAP_CAUSE cause = cig_mgr_add_cis(cig_id, cis_id);
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_add_cis(uint8_t cig_id, uint8_t cis_id);

/**
 * @brief       Set GAP CIG parameters.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR.
 *
 * @param[in]   cig_id   Identifier of a CIG.
 * @param[in]   p_param  @ref T_CIG_CFG_PARAM.
 *
 * @return GAP operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others             GAP operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        // After registration of CIG by @ref cig_mgr_reg_cig

        T_CIG_CFG_PARAM cig_param;

        ......
        // Initialization of cig_param is dependent on Application.

        T_GAP_CAUSE cause = cig_mgr_set_cig_param(cig_id, &cig_param);
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_set_cig_param(uint8_t cig_id, T_CIG_CFG_PARAM *p_param);

/**
 * @brief       Set GAP CIS parameters.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR.
 *
 * @param[in]   cis_id   Used to identify a CIS.
 * @param[in]   p_param  @ref T_CIS_CFG_PARAM.
 *
 * @return GAP operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others             GAP operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        // After adding CIS by @ref cig_mgr_add_cis

        T_CIS_CFG_PARAM cis_param;

        ......
        // Initialization of cis_param is dependent on Application.

        T_GAP_CAUSE cause = cig_mgr_set_cis_param(cis_id, &cis_param);
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_set_cis_param(uint8_t cis_id, T_CIS_CFG_PARAM *p_param);

/**
 * @brief       Request to create a CIG and to set the parameters of one or more CISes in the Controller.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR.
 *
 *              If sending request operation is success, the result will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb with cb_type @ref MSG_CIG_MGR_START_SETTING.
 *
 * @param[in]   cig_id   Identifier of a CIG.
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

        ......
        // Initialization is dependent on Application.
        cause = cig_mgr_reg_cig(cig_id, app_isoc_cis_initiator_cig_cb);
        cause = cig_mgr_add_cis(cig_id, cis_id);
        cause = cig_mgr_set_cig_param(cig_id, &cig_param);
        cause = cig_mgr_set_cis_param(cis_id, &cis_param);
        ......

        cause = cig_mgr_start_setting(cig_id);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_START_SETTING:
            {
                APP_PRINT_INFO3("MSG_CIG_MGR_START_SETTING: cause 0x%x, cig_id %d, cis_count %d",
                                p_data->p_cig_mgr_start_setting_rsp->cause,
                                p_data->p_cig_mgr_start_setting_rsp->cig_id,
                                p_data->p_cig_mgr_start_setting_rsp->cis_count);
                if ((p_data->p_cig_mgr_start_setting_rsp->cause == GAP_SUCCESS) &&
                    p_data->p_cig_mgr_start_setting_rsp->cis_count)
                {
                    for (uint8_t i = 0; i < p_data->p_cig_mgr_start_setting_rsp->cis_count; i++)
                    {
                        APP_PRINT_INFO4("MSG_CIG_MGR_START_SETTING: cis_info[%d], cig_id 0x%x, cis_id 0x%x, cis_conn_handle 0x%x",
                                        i,
                                        p_data->p_cig_mgr_start_setting_rsp->cig_id,
                                        p_data->p_cig_mgr_start_setting_rsp->cis_info[i].cis_id,
                                        p_data->p_cig_mgr_start_setting_rsp->cis_info[i].cis_conn_handle);
                    }
                }
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_start_setting(uint8_t cig_id);

/**
 * @brief       Used by the Central's Host to remove the CIG.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR.
 *
 *              NOTE: CIG ID and CIS Connection Handles will be deleted.
 *                    Isochronous data paths that are associated with CIS Connection Handles are also removed, which is
 *                    equivalent to call @ref cig_mgr_remove_data_path.
 *
 *              If sending request operation is success, the result of removing CIG will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb with cb_type @ref MSG_CIG_MGR_REMOVE_CIG.
 *
 * @param[in]   cig_id   Identifier of a CIG.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_remove_cig(cig_id);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_REMOVE_CIG:
            {
                APP_PRINT_INFO2("MSG_CIG_MGR_REMOVE_CIG: cause 0x%x, cig_id 0x%x",
                                p_data->p_cig_mgr_remove_cig_rsp->cause, p_data->p_cig_mgr_remove_cig_rsp->cig_id);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_remove_cig(uint8_t cig_id);

/**
 * @brief       Specify the connection handle of the ACL connection associated with
 *              each CIS to be created.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR.
 *
 *              NOTE: This function should be invoked before creating CIS by
 *                    @ref cig_mgr_create_cis_by_cig_id or @ref cig_mgr_create_cis_by_cis_conn_handle.
 *
 * @param[in]   cis_id   Used to identify a CIS.
 * @param[in]   conn_id  Identifier of ACL connection.
 *
 * @return GAP operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others             GAP operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_set_cis_acl_link(cis_id, conn_id);
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_set_cis_acl_link(uint8_t cis_id, uint8_t conn_id);

/**
 * @brief       Request to create all CISes in a CIG.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR.
 *
 *              If sending request operation is success, the result will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb with cb_type @ref MSG_CIG_MGR_CIS_ESTABLISHED_INFO
 *              and @ref MSG_CIG_MGR_CREATE_CIS.
 *
 * @param[in]   cig_id   Identifier of a CIG.
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

        ......
        // Create a CIG and set the parameters of one or more CISes in the Controller

        ......
        cause = cig_mgr_set_cis_acl_link(cis_id, conn_id);
        ......

        cause = cig_mgr_create_cis_by_cig_id(cig_id);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_CIS_ESTABLISHED_INFO:
            {
                APP_PRINT_INFO7("MSG_CIG_MGR_CIS_ESTABLISHED_INFO: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x, cig_sync_delay 0x%x, cis_sync_delay 0x%x",
                                p_data->p_cig_mgr_cis_established_info->cause,
                                p_data->p_cig_mgr_cis_established_info->conn_id,
                                p_data->p_cig_mgr_cis_established_info->cis_conn_handle,
                                p_data->p_cig_mgr_cis_established_info->cig_id,
                                p_data->p_cig_mgr_cis_established_info->cis_id,
                                p_data->p_cig_mgr_cis_established_info->cig_sync_delay,
                                p_data->p_cig_mgr_cis_established_info->cis_sync_delay);
                APP_PRINT_INFO5("MSG_CIG_MGR_CIS_ESTABLISHED_INFO: transport_latency_m_s 0x%x, transport_latency_s_m 0x%x, phy_m_s 0x%x, phy_s_m 0x%x, nse 0x%x",
                                p_data->p_cig_mgr_cis_established_info->transport_latency_m_s,
                                p_data->p_cig_mgr_cis_established_info->transport_latency_s_m,
                                p_data->p_cig_mgr_cis_established_info->phy_m_s,
                                p_data->p_cig_mgr_cis_established_info->phy_s_m,
                                p_data->p_cig_mgr_cis_established_info->nse);
                APP_PRINT_INFO7("MSG_CIG_MGR_CIS_ESTABLISHED_INFO: bn_m_s 0x%x, bn_s_m 0x%x, ft_m_s 0x%x, ft_s_m 0x%x, max_pdu_m_s 0x%x, max_pdu_s_m 0x%x, iso_interval 0x%x",
                                p_data->p_cig_mgr_cis_established_info->bn_m_s,
                                p_data->p_cig_mgr_cis_established_info->bn_s_m,
                                p_data->p_cig_mgr_cis_established_info->ft_m_s,
                                p_data->p_cig_mgr_cis_established_info->ft_s_m,
                                p_data->p_cig_mgr_cis_established_info->max_pdu_m_s,
                                p_data->p_cig_mgr_cis_established_info->max_pdu_s_m,
                                p_data->p_cig_mgr_cis_established_info->iso_interval);
            }
            break;

        case MSG_CIG_MGR_CREATE_CIS:
            {
                APP_PRINT_INFO2("MSG_CIG_MGR_CREATE_CIS: cause 0x%x, cis_count 0x%x",
                                p_data->p_cig_mgr_create_cis_rsp->cause,
                                p_data->p_cig_mgr_create_cis_rsp->cis_count);
                for (uint8_t i = 0; i < p_data->p_cig_mgr_create_cis_rsp->cis_count; i++)
                {
                    APP_PRINT_INFO5("cis_info[%d]: cig_id 0x%x, cis_id 0x%x, state 0x%x, cis_conn_handle 0x%x", i,
                                    p_data->p_cig_mgr_create_cis_rsp->cis_info[i].cig_id,
                                    p_data->p_cig_mgr_create_cis_rsp->cis_info[i].cis_id,
                                    p_data->p_cig_mgr_create_cis_rsp->cis_info[i].state,
                                    p_data->p_cig_mgr_create_cis_rsp->cis_info[i].cis_conn_handle);
                }
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_create_cis_by_cig_id(uint8_t cig_id);

/**
 * @brief       Request to create one or more CISes in a CIG.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR.
 *
 *              If sending request operation is success, the result will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb with cb_type @ref MSG_CIG_MGR_CIS_ESTABLISHED_INFO
 *              and @ref MSG_CIG_MGR_CREATE_CIS.
 *
 * @param[in]   cig_id             Identifier of a CIG.
 * @param[in]   cis_count          Total number of CISes to be created.
 * @param[in]   p_cis_conn_handle  Connection handle of CIS.
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

        ......
        // Create a CIG and set the parameters of one or more CISes in the Controller

        ......
        cause = cig_mgr_set_cis_acl_link(cis_id, conn_id);
        ......

        cause = cig_mgr_create_cis_by_cis_conn_handle(cig_id, cis_count, p_cis_conn_handle);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_CIS_ESTABLISHED_INFO:
            {
                APP_PRINT_INFO7("MSG_CIG_MGR_CIS_ESTABLISHED_INFO: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x, cig_sync_delay 0x%x, cis_sync_delay 0x%x",
                                p_data->p_cig_mgr_cis_established_info->cause,
                                p_data->p_cig_mgr_cis_established_info->conn_id,
                                p_data->p_cig_mgr_cis_established_info->cis_conn_handle,
                                p_data->p_cig_mgr_cis_established_info->cig_id,
                                p_data->p_cig_mgr_cis_established_info->cis_id,
                                p_data->p_cig_mgr_cis_established_info->cig_sync_delay,
                                p_data->p_cig_mgr_cis_established_info->cis_sync_delay);
                APP_PRINT_INFO5("MSG_CIG_MGR_CIS_ESTABLISHED_INFO: transport_latency_m_s 0x%x, transport_latency_s_m 0x%x, phy_m_s 0x%x, phy_s_m 0x%x, nse 0x%x",
                                p_data->p_cig_mgr_cis_established_info->transport_latency_m_s,
                                p_data->p_cig_mgr_cis_established_info->transport_latency_s_m,
                                p_data->p_cig_mgr_cis_established_info->phy_m_s,
                                p_data->p_cig_mgr_cis_established_info->phy_s_m,
                                p_data->p_cig_mgr_cis_established_info->nse);
                APP_PRINT_INFO7("MSG_CIG_MGR_CIS_ESTABLISHED_INFO: bn_m_s 0x%x, bn_s_m 0x%x, ft_m_s 0x%x, ft_s_m 0x%x, max_pdu_m_s 0x%x, max_pdu_s_m 0x%x, iso_interval 0x%x",
                                p_data->p_cig_mgr_cis_established_info->bn_m_s,
                                p_data->p_cig_mgr_cis_established_info->bn_s_m,
                                p_data->p_cig_mgr_cis_established_info->ft_m_s,
                                p_data->p_cig_mgr_cis_established_info->ft_s_m,
                                p_data->p_cig_mgr_cis_established_info->max_pdu_m_s,
                                p_data->p_cig_mgr_cis_established_info->max_pdu_s_m,
                                p_data->p_cig_mgr_cis_established_info->iso_interval);
            }
            break;

        case MSG_CIG_MGR_CREATE_CIS:
            {
                APP_PRINT_INFO2("MSG_CIG_MGR_CREATE_CIS: cause 0x%x, cis_count 0x%x",
                                p_data->p_cig_mgr_create_cis_rsp->cause,
                                p_data->p_cig_mgr_create_cis_rsp->cis_count);
                for (uint8_t i = 0; i < p_data->p_cig_mgr_create_cis_rsp->cis_count; i++)
                {
                    APP_PRINT_INFO5("cis_info[%d]: cig_id 0x%x, cis_id 0x%x, state 0x%x, cis_conn_handle 0x%x", i,
                                    p_data->p_cig_mgr_create_cis_rsp->cis_info[i].cig_id,
                                    p_data->p_cig_mgr_create_cis_rsp->cis_info[i].cis_id,
                                    p_data->p_cig_mgr_create_cis_rsp->cis_info[i].state,
                                    p_data->p_cig_mgr_create_cis_rsp->cis_info[i].cis_conn_handle);
                }
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_create_cis_by_cis_conn_handle(uint8_t cig_id, uint8_t cis_count,
                                                  uint16_t *p_cis_conn_handle);

/**
 * @brief       Get CIS connection handle.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_INITIATOR.
 *
 * @param[in]      cis_id          Used to identify a CIS.
 * @param[in,out]  p_conn_handle   CIS connection handle.
 *
 * @return GAP operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others             GAP operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool cause = cig_mgr_get_cis_handle(cis_id, p_conn_handle);
    }
 * \endcode
 */
bool cig_mgr_get_cis_handle(uint8_t cis_id, uint16_t *p_conn_handle);

/**
 * @brief       Set GAP CIG Test parameters in ISO Test mode.
 *
 *              NOTE: This function is used in ISO Test mode and only for testing purposes for @ref ISOCH_ROLE_INITIATOR.
 *
 * @param[in]   cig_id   Identifier of a CIG.
 * @param[in]   p_param  @ref T_CIG_CFG_PARAM_TEST.
 *
 * @return GAP operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others             GAP operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        // After registration of CIG by @ref cig_mgr_reg_cig

        T_CIG_CFG_PARAM_TEST cig_param_test;

        ......
        // Initialization of cig_param_test is dependent on Application.

        T_GAP_CAUSE cause = cig_mgr_set_cig_param_test(cig_id, &cig_param_test);
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_set_cig_param_test(uint8_t cig_id, T_CIG_CFG_PARAM_TEST *p_param);

/**
 * @brief       Set GAP CIS Test parameters in ISO Test mode.
 *
 *              NOTE: This function is used in ISO Test mode and only for testing purposes for @ref ISOCH_ROLE_INITIATOR.
 *
 * @param[in]   cis_id   Used to identify a CIS.
 * @param[in]   p_param  @ref T_CIS_CFG_PARAM_TEST.
 *
 * @return GAP operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others             GAP operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        // After adding CIS by @ref cig_mgr_add_cis

        T_CIS_CFG_PARAM_TEST cis_param_test;

        ......
        // Initialization of cis_param_test is dependent on Application.

        T_GAP_CAUSE cause = cig_mgr_set_cis_param_test(cis_id, &cis_param_test);
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_set_cis_param_test(uint8_t cis_id, T_CIS_CFG_PARAM_TEST *p_param);

/**
 * @brief       Request to create a CIG and to set the parameters of one or more CISes in the Controller for ISO Test mode.
 *
 *              NOTE: This function is used in ISO Test mode and only for testing purposes for @ref ISOCH_ROLE_INITIATOR.
 *
 *              If sending request operation is success, the result will be returned by
 *              @ref app_isoc_cis_initiator_cig_cb with cb_type @ref MSG_CIG_MGR_START_SETTING_TEST.
 *
 * @param[in]   cig_id   Identifier of a CIG.
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

        ......
        // Initialization is dependent on Application.
        cause = cig_mgr_reg_cig(cig_id, app_isoc_cis_initiator_cig_cb);
        cause = cig_mgr_add_cis(cig_id, cis_id);
        cause = cig_mgr_set_cig_param_test(cig_id, &cig_param_test);
        cause = cig_mgr_set_cis_param_test(cis_id, &cis_param_test);
        ......

        cause = cig_mgr_start_setting_test(cig_id);
    }

    T_APP_RESULT app_isoc_cis_initiator_cig_cb(uint8_t cig_id, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_START_SETTING_TEST:
            {
                APP_PRINT_INFO3("MSG_CIG_MGR_START_SETTING_TEST: cause 0x%x, cig_id %d, cis_count %d",
                                p_data->p_cig_mgr_start_setting_test_rsp->cause,
                                p_data->p_cig_mgr_start_setting_test_rsp->cig_id,
                                p_data->p_cig_mgr_start_setting_test_rsp->cis_count);
                if ((p_data->p_cig_mgr_start_setting_test_rsp->cause == GAP_SUCCESS) &&
                    p_data->p_cig_mgr_start_setting_test_rsp->cis_count)
                {
                    for (uint8_t i = 0; i < p_data->p_cig_mgr_start_setting_test_rsp->cis_count; i++)
                    {
                        APP_PRINT_INFO4("MSG_CIG_MGR_START_SETTING_TEST: cis_info[%d], cig_id 0x%x, cis_id 0x%x, cis_conn_handle 0x%x",
                                        i,
                                        p_data->p_cig_mgr_start_setting_test_rsp->cig_id,
                                        p_data->p_cig_mgr_start_setting_test_rsp->cis_info[i].cis_id,
                                        p_data->p_cig_mgr_start_setting_test_rsp->cis_info[i].cis_conn_handle);
                    }
                }
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_start_setting_test(uint8_t cig_id);

/**
 * @brief       Register callback for GAP CIG Mgr @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              NOTE: cb_type @ref MSG_CIG_MGR_CIS_REQUEST_IND indicates that Controller has received a request to establish a CIS.
 *                    Meaning for return value of @ref MSG_CIG_MGR_CIS_REQUEST_IND by Application:
 *                          @ref APP_RESULT_ACCEPT: Inform the Controller to accept the request for the CIS.
 *                                                  Result will be notified by cb_type @ref MSG_CIG_MGR_ACCEPT_CIS_INFO
 *                                                  and @ref MSG_CIG_MGR_CIS_ESTABLISHED_INFO.
 *                          neither @ref APP_RESULT_ACCEPT nor @ref APP_RESULT_PENDING: e.g. @ref BT_HCI_ERROR (except @ref HCI_SUCCESS),
 *                                                                                      inform the Controller to reject the request for
 *                                                                                      the CIS. Result will be notified by cb_type
 *                                                                                      @ref MSG_CIG_MGR_REJECT_CIS_INFO.
 *                          @ref APP_RESULT_PENDING: Application should call @ref cig_mgr_acceptor_accept_cis to accept or
 *                                                   @ref cig_mgr_acceptor_reject_cis to reject the request for the CIS.
 *
 * @param[in]   cb_pfn   Callback.
 *
 * @return void.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        cig_mgr_reg_acceptor_cb(app_isoc_cis_acceptor_cb);
    }

    T_APP_RESULT app_isoc_cis_acceptor_cb(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_CIS_REQUEST_IND:
            {
                result = APP_RESULT_ACCEPT;
                APP_PRINT_INFO4("MSG_CIG_MGR_CIS_REQUEST_IND: conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_cis_request_ind->conn_id,
                                p_data->p_cig_mgr_cis_request_ind->cis_conn_handle,
                                p_data->p_cig_mgr_cis_request_ind->cig_id,
                                p_data->p_cig_mgr_cis_request_ind->cis_id);
            }
            break;

        case MSG_CIG_MGR_ACCEPT_CIS_INFO:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_ACCEPT_CIS_INFO: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_accept_cis_info->cause,
                                p_data->p_cig_mgr_accept_cis_info->conn_id,
                                p_data->p_cig_mgr_accept_cis_info->cis_conn_handle,
                                p_data->p_cig_mgr_accept_cis_info->cig_id,
                                p_data->p_cig_mgr_accept_cis_info->cis_id);
            }
            break;

        case MSG_CIG_MGR_REJECT_CIS_INFO:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_REJECT_CIS_INFO: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_reject_cis_info->cause,
                                p_data->p_cig_mgr_reject_cis_info->conn_id,
                                p_data->p_cig_mgr_reject_cis_info->cis_conn_handle,
                                p_data->p_cig_mgr_reject_cis_info->cig_id,
                                p_data->p_cig_mgr_reject_cis_info->cis_id);
            }
            break;
        ......
    }
 * \endcode
 */
void cig_mgr_reg_acceptor_cb(P_FUN_LE_CIG_MGR_ACCEPTOR_CB cb_pfn);

/**
 * @brief       Accept the request for the CIS.
 *
 *              NOTE: This function is only used after return value of @ref MSG_CIG_MGR_CIS_REQUEST_IND
 *                    by Application is @ref APP_RESULT_PENDING for @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              If sending request operation is success, the result of accepting request for the CIS will be returned by
 *              @ref app_isoc_cis_acceptor_cb for @ref ISOCH_ROLE_ACCEPTOR
 *              with cb_type @ref MSG_CIG_MGR_ACCEPT_CIS_INFO and @ref MSG_CIG_MGR_CIS_ESTABLISHED_INFO.
 *
 * @param[in]   cis_conn_handle   Connection handle of the CIS.
 *
 * @return void.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_acceptor_accept_cis(cis_conn_handle);
    }

    T_APP_RESULT app_isoc_cis_acceptor_cb(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_CIS_REQUEST_IND:
            {
                result = APP_RESULT_PENDING;
                APP_PRINT_INFO4("MSG_CIG_MGR_CIS_REQUEST_IND: conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_cis_request_ind->conn_id,
                                p_data->p_cig_mgr_cis_request_ind->cis_conn_handle,
                                p_data->p_cig_mgr_cis_request_ind->cig_id,
                                p_data->p_cig_mgr_cis_request_ind->cis_id);
            }
            break;

        case MSG_CIG_MGR_ACCEPT_CIS_INFO:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_ACCEPT_CIS_INFO: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_accept_cis_info->cause,
                                p_data->p_cig_mgr_accept_cis_info->conn_id,
                                p_data->p_cig_mgr_accept_cis_info->cis_conn_handle,
                                p_data->p_cig_mgr_accept_cis_info->cig_id,
                                p_data->p_cig_mgr_accept_cis_info->cis_id);
            }
            break;

        case MSG_CIG_MGR_CIS_ESTABLISHED_INFO:
            {
                APP_PRINT_INFO7("MSG_CIG_MGR_CIS_ESTABLISHED_INFO: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x, cig_sync_delay 0x%x, cis_sync_delay 0x%x",
                                p_data->p_cig_mgr_cis_established_info->cause,
                                p_data->p_cig_mgr_cis_established_info->conn_id,
                                p_data->p_cig_mgr_cis_established_info->cis_conn_handle,
                                p_data->p_cig_mgr_cis_established_info->cig_id,
                                p_data->p_cig_mgr_cis_established_info->cis_id,
                                p_data->p_cig_mgr_cis_established_info->cig_sync_delay,
                                p_data->p_cig_mgr_cis_established_info->cis_sync_delay);
                APP_PRINT_INFO5("MSG_CIG_MGR_CIS_ESTABLISHED_INFO: transport_latency_m_s 0x%x, transport_latency_s_m 0x%x, phy_m_s 0x%x, phy_s_m 0x%x, nse 0x%x",
                                p_data->p_cig_mgr_cis_established_info->transport_latency_m_s,
                                p_data->p_cig_mgr_cis_established_info->transport_latency_s_m,
                                p_data->p_cig_mgr_cis_established_info->phy_m_s,
                                p_data->p_cig_mgr_cis_established_info->phy_s_m,
                                p_data->p_cig_mgr_cis_established_info->nse);
                APP_PRINT_INFO7("MSG_CIG_MGR_CIS_ESTABLISHED_INFO: bn_m_s 0x%x, bn_s_m 0x%x, ft_m_s 0x%x, ft_s_m 0x%x, max_pdu_m_s 0x%x, max_pdu_s_m 0x%x, iso_interval 0x%x",
                                p_data->p_cig_mgr_cis_established_info->bn_m_s,
                                p_data->p_cig_mgr_cis_established_info->bn_s_m,
                                p_data->p_cig_mgr_cis_established_info->ft_m_s,
                                p_data->p_cig_mgr_cis_established_info->ft_s_m,
                                p_data->p_cig_mgr_cis_established_info->max_pdu_m_s,
                                p_data->p_cig_mgr_cis_established_info->max_pdu_s_m,
                                p_data->p_cig_mgr_cis_established_info->iso_interval);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_acceptor_accept_cis(uint16_t cis_conn_handle);

/**
 * @brief       Reject the request for the CIS.
 *
 *              NOTE: This function is only used after return value of @ref MSG_CIG_MGR_CIS_REQUEST_IND
 *                    by Application is @ref APP_RESULT_PENDING for @ref ISOCH_ROLE_ACCEPTOR.
 *
 *              If sending request operation is success, the result of rejecting request for the CIS will be returned by
 *              @ref app_isoc_cis_acceptor_cb for @ref ISOCH_ROLE_ACCEPTOR
 *              with cb_type @ref MSG_CIG_MGR_REJECT_CIS_INFO.
 *
 * @param[in]   cis_conn_handle   Connection handle of the CIS.
 * @param[in]   reason            Reason the CIS request was rejected. @ref BT_HCI_ERROR (except @ref HCI_SUCCESS)..
 *
 * @return void.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_acceptor_reject_cis(cis_conn_handle, reason);
    }

    T_APP_RESULT app_isoc_cis_acceptor_cb(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_CIG_MGR_CB_DATA *p_data = (T_CIG_MGR_CB_DATA *)p_cb_data;

        .......
        switch (cb_type)
        {
        case MSG_CIG_MGR_CIS_REQUEST_IND:
            {
                result = APP_RESULT_PENDING;
                APP_PRINT_INFO4("MSG_CIG_MGR_CIS_REQUEST_IND: conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_cis_request_ind->conn_id,
                                p_data->p_cig_mgr_cis_request_ind->cis_conn_handle,
                                p_data->p_cig_mgr_cis_request_ind->cig_id,
                                p_data->p_cig_mgr_cis_request_ind->cis_id);
            }
            break;

        case MSG_CIG_MGR_REJECT_CIS_INFO:
            {
                APP_PRINT_INFO5("MSG_CIG_MGR_REJECT_CIS_INFO: cause 0x%x, conn_id %d, cis_conn_handle 0x%x, cig_id 0x%x, cis_id 0x%x",
                                p_data->p_cig_mgr_reject_cis_info->cause,
                                p_data->p_cig_mgr_reject_cis_info->conn_id,
                                p_data->p_cig_mgr_reject_cis_info->cis_conn_handle,
                                p_data->p_cig_mgr_reject_cis_info->cig_id,
                                p_data->p_cig_mgr_reject_cis_info->cis_id);
            }
            break;
        ......
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_acceptor_reject_cis(uint16_t cis_conn_handle, uint8_t reason);

/**
 * @brief       Whether to check sdu length when @ref ISOCH_ROLE_ACCEPTOR calls @ref gap_iso_send_data.
 *
 *              NOTE: This function is used for @ref ISOCH_ROLE_ACCEPTOR.
 *
 * @param[in]       cis_conn_handle            Connection handle of the CIS.
 * @param[in]       acceptor_config_sdu_flag   Whether to check sdu length when @ref ISOCH_ROLE_ACCEPTOR calls @ref gap_iso_send_data.
 *                                             Whether to configure sdu parameters supplied by the relevant profile(s) for @ref ISOCH_ROLE_ACCEPTOR.
 *                                             True:  sdu parameters listed below are configured by Application.
 *                                                    check sdu length when @ref ISOCH_ROLE_ACCEPTOR calls @ref gap_iso_send_data.
 *                                             False: Default value.
 *                                                    sdu parameters listed below are not configrued by Application.
 *                                                    Not check sdu length when @ref ISOCH_ROLE_ACCEPTOR calls @ref gap_iso_send_data.
 * @param[in]       sdu_interval_m_s           The interval, in microseconds, between the start of consecutive SDUs from
 *                                             the Central's Host for all the CISes in the CIG.
 *                                             Range: 0x0000FF to 0x0FFFFF
 * @param[in]       sdu_interval_s_m           The interval, in microseconds, between the start of consecutive SDUs from
 *                                             the Peripheral's Host for all the CISes in the CIG.
 *                                             Range: 0x0000FF to 0x0FFFFF.
 * @param[in]       max_sdu_m_s                Maximum size, in octets, of the payload from the Central's Host.
 * @param[in]       max_sdu_s_m                Maximum size, in octets, of the payload from the Peripheral's Host.
 *
 * @return      GAP Operation result.
 * @retval      GAP_CAUSE_SUCCESS       GAP operation success.
 * @retval      GAP_CAUSE_NO_RESOURCE   Operation failure, memory acquisition failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = cig_mgr_acceptor_config_sdu_param(cis_conn_handle, acceptor_config_sdu_flag, sdu_interval_m_s, sdu_interval_s_m, max_sdu_m_s, max_sdu_s_m);
    }
 * \endcode
 */
T_GAP_CAUSE cig_mgr_acceptor_config_sdu_param(uint16_t cis_conn_handle,
                                              bool acceptor_config_sdu_flag, uint32_t sdu_interval_m_s, uint32_t sdu_interval_s_m,
                                              uint16_t max_sdu_m_s, uint16_t max_sdu_s_m);

/** End of GAP_LE_CIG_MGR_Exported_Functions
  * @}
  */

/** End of GAP_LE_CIG_MGR
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

#endif /* _CIG_MGR_H_ */
