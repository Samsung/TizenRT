/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_big_mgr.h
* @brief     Header file for GAP BIG
* @details
* @author
* @date      2021-07-12
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_BIG_MGR_H
#define GAP_BIG_MGR_H

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#ifdef __cplusplus
extern "C"
{
#endif

#if F_BT_LE_5_2_ISOC_BIS_SUPPORT
/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "gap.h"

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_BIG_MGR GAP LE BIG Mgr Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_BIG_MGR_Exported_Macros GAP LE BIG Mgr Exported Macros
  * @{
  */
#define GAP_BIG_MGR_MAX_BIS_NUM 4

/** @defgroup BIG_MGR_INVALID_PARAM BIG Mgr invalid parameters
  * @brief    Use macro definitions to indicate that BIG parameters are
  *           invalid in certain scenarios.
  * @{
  */
#define GAP_INVALID_BIG_HANDLE   0xFF   /**< A big_handle value of 0xFF means invalid. */
/** End of BIG_MGR_INVALID_PARAM
  * @}
  */

/** @defgroup GAP_BIG_MGR_SYNC_RECEIVER_CREATE_SYNC_DEV_STATE GAP BIG Mgr Synchronized Receiver create sync device state
  * @{
  */
#define GAP_BIG_MGR_SYNC_RECEIVER_CREATE_SYNC_DEV_STATE_IDLE              0   /**< Idle */
#define GAP_BIG_MGR_SYNC_RECEIVER_CREATE_SYNC_DEV_STATE_SYNCHRONIZING     1   /**< Synchronizing */
/** End of GAP_BIG_MGR_SYNC_RECEIVER_CREATE_SYNC_DEV_STATE
  * @}
  */

/** @defgroup BIG_MGR_CALLBACK_MSG BIG Mgr Callback Msg
  * @brief    Used to identify BIG Manager Callback Message.
  * @{
  */
/** @brief Response type for @ref BIG_MGR_ROLE_ISOC_BROADCASTER and @ref BIG_MGR_ROLE_SYNC_RECEIVER. */
#define MSG_BIG_MGR_SETUP_DATA_PATH       0x60 //!<Response msg type for gap_big_mgr_setup_data_path
#define MSG_BIG_MGR_REMOVE_DATA_PATH      0x61 //!<Response msg type for gap_big_mgr_remove_data_path

/** @brief Test command response type for @ref BIG_MGR_ROLE_ISOC_BROADCASTER and @ref BIG_MGR_ROLE_SYNC_RECEIVER. */
#define MSG_BIG_MGR_ISO_TEST_END          0x70 //!<Response msg type for gap_big_mgr_iso_test_end
/** @brief Test command response type for @ref BIG_MGR_ROLE_ISOC_BROADCASTER. */
#define MSG_BIG_MGR_TRANSMIT_TEST         0x74 //!<Response msg type for gap_big_mgr_transmit_test
/** @brief Test command response type for @ref BIG_MGR_ROLE_SYNC_RECEIVER. */
#define MSG_BIG_MGR_RECEIVE_TEST          0x78 //!<Response msg type for gap_big_mgr_receive_test
#define MSG_BIG_MGR_READ_TEST_COUNTERS    0x79 //!<Response msg type for gap_big_mgr_read_test_counters

/** @brief Notification and response type for @ref BIG_MGR_ROLE_ISOC_BROADCASTER. */
#define MSG_BIG_MGR_ISOC_BROADCAST_STATE_CHANGE_INFO          0x80 //!<Isochronous Broadcaster broadcasting state change info
#define MSG_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_CMPL_INFO     0x81 //!<Notification msg type for gap_big_mgr_isoc_broadcaster_create_big
#define MSG_BIG_MGR_READ_ISO_TX_SYNC                          0x82 //!<Response msg type for gap_big_mgr_read_iso_tx_sync

/** @brief Notification and response type for @ref BIG_MGR_ROLE_SYNC_RECEIVER. */
#define MSG_BIG_MGR_SYNC_RECEIVER_DEV_STATE_CHANGE_INFO       0x90 //!<Synchronized Receiver device state change info
#define MSG_BIG_MGR_SYNC_RECEIVER_SYNC_STATE_CHANGE_INFO      0x91 //!<Synchronized Receiver synchronous state change info
#define MSG_BIG_MGR_SYNC_RECEIVER_BIG_SYNC_ESTABLISHED_INFO   0x92 //!<Notification msg type for gap_big_mgr_sync_receiver_big_create_sync
#define MSG_BIG_MGR_READ_ISO_LINK_QUALITY                     0x93 //!<Response msg type for gap_big_mgr_read_iso_link_quality
/** End of BIG_MGR_CALLBACK_MSG
  * @}
  */
/** End of GAP_LE_BIG_MGR_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_BIG_MGR_Exported_Types GAP LE BIG Mgr Exported Types
  * @{
  */

/** @brief GAP LE BIG Mgr role. */
typedef enum
{
    BIG_MGR_ROLE_ISOC_BROADCASTER = 1,        /**< BIS - Isochronous Broadcaster role. */
    BIG_MGR_ROLE_SYNC_RECEIVER = 2,           /**< BIS - Synchronized Receiver role. */
} T_GAP_BIG_MGR_ROLE;

#if F_BT_LE_5_2_ISOC_BIS_BROADCASTER_SUPPORT
/** @brief GAP LE BIG Mgr Isochronous Broadcaster broadcasting state for @ref BIG_MGR_ROLE_ISOC_BROADCASTER. */
typedef enum
{
    BIG_ISOC_BROADCAST_STATE_IDLE = 0x00,                                        /**< Idle, no Isochronous Broadcasting. */
    BIG_ISOC_BROADCAST_STATE_CREATING_EXT_ADV_STATE_PA_ADV_STATE_IDLE = 0x01,    /**< Start Isochronous Broadcasting when extended advertising and periodic advertising of the
                                                                                      specified advertising set are disabled. A temporary state, haven't received the result. */
    BIG_ISOC_BROADCAST_STATE_WAIT_EXT_ADV_STATE_PA_ADV_STATE_ADVERTISING = 0x02, /**< Isochronous Broadcasting is not started until the extended advertising and periodic advertising
                                                                                      of the specified advertising set are enabled. */
    BIG_ISOC_BROADCAST_STATE_CREATING_PA_ADV_STATE_IDLE = 0x03,                  /**< Start Isochronous Broadcasting when the periodic advertising of the specified advertising set is disabled.
                                                                                      A temporary state, haven't received the result. */
    BIG_ISOC_BROADCAST_STATE_WAIT_PA_ADV_STATE_ADVERTISING = 0x04,               /**< Isochronous Broadcasting is not started until the periodic advertising of the specified advertising
                                                                                      set is enabled. */
    BIG_ISOC_BROADCAST_STATE_CREATING_EXT_ADV_STATE_IDLE = 0x05,                 /**< Start Isochronous Broadcasting when the extended advertising of the specified advertising set is disabled.
                                                                                      A temporary state, haven't received the result. */
    BIG_ISOC_BROADCAST_STATE_WAIT_EXT_ADV_STATE_ADVERTISING = 0x06,              /**< Isochronous Broadcasting is not started until the extended advertising of the specified advertising
                                                                                      set is enabled. */
    BIG_ISOC_BROADCAST_STATE_CREATING = 0x07,                                    /**< Start Isochronous Broadcasting when extended advertising and periodic advertising of the
                                                                                      specified advertising set are enabled. A temporary state, haven't received the result. */
    BIG_ISOC_BROADCAST_STATE_BROADCASTING = 0x08,                                /**< Isochronous Broadcasting. */
    BIG_ISOC_BROADCAST_STATE_TERMINATING = 0x09,                                 /**< Terminate Isochronous Broadcasting. A temporary state, haven't received the result. */
} T_GAP_BIG_ISOC_BROADCAST_STATE;
#endif

#if F_BT_LE_5_2_ISOC_BIS_RECEIVER_SUPPORT
/** @brief GAP LE BIG Mgr Synchronized Receiver synchronization state for @ref BIG_MGR_ROLE_SYNC_RECEIVER. */
typedef enum
{
    BIG_SYNC_RECEIVER_SYNC_STATE_TERMINATED = 0x00,     /**< Terminated. */
    BIG_SYNC_RECEIVER_SYNC_STATE_SYNCHRONIZING = 0x01,  /**< Start synchronizing to a BIG. A temporary state, haven't received the result.*/
    BIG_SYNC_RECEIVER_SYNC_STATE_SYNCHRONIZED = 0x02,   /**< Synchronized to a BIG. */
    BIG_SYNC_RECEIVER_SYNC_STATE_TERMINATING = 0x03,    /**< Terminate synchronization to the BIG. A temporary state, haven't received the result.*/
} T_GAP_BIG_SYNC_RECEIVER_SYNC_STATE;
#endif

typedef struct
{
    uint16_t cause;
    uint16_t bis_conn_handle;        /**< Connection handle of a BIS. */
    uint8_t  big_handle;             /**< Used to identify the BIG. */
    uint8_t  data_path_adding_path;
} T_BIG_MGR_SETUP_DATA_PATH_RSP;

typedef struct
{
    uint16_t cause;
    uint16_t bis_conn_handle;        /**< Connection handle of a BIS. */
    uint8_t  big_handle;             /**< Used to identify the BIG. */
    uint8_t  data_path_removing_path;
} T_BIG_MGR_REMOVE_DATA_PATH_RSP;

typedef struct
{
    uint8_t bis_idx;
    uint16_t bis_conn_handle;         /**< Connection handle of a BIS. */
} T_BIG_MGR_BIS_CONN_HANDLE_INFO;

#if F_BT_LE_5_2_ISOC_TEST_MODE_BIS_SUPPORT
typedef struct
{
    uint16_t cause;
    uint16_t bis_conn_handle;       /**< Connection handle of a BIS. */
    uint8_t  big_handle;            /**< Used to identify the BIG. */
    uint32_t received_packet_count; /**< Number in the Received_SDU_Count.
                                         The Received_SDU_Count shall be incremented by one for every valid received test
                                         ISO Data SDU containing a size in the expected range and an SDU counter that matches
                                         the expected value. */
    uint32_t missed_packet_count;   /**< Number in the Missed_SDU_Count.
                                         The Missed_SDU_Count shall be incremented by one for each SDU that is made up of
                                         one or more invalid or missing PDUs. */
    uint32_t failed_packet_count;   /**< Number in the Failed_SDU_Count.
                                         The Failed_SDU_Count shall be incremented by one for each valid received test SDU
                                         where the size or SDU counter does not match the expected size or value. */
} T_BIG_MGR_ISO_TEST_END_RSP;
#endif

#if F_BT_LE_5_2_ISOC_BIS_BROADCASTER_SUPPORT
typedef struct
{
    uint8_t num_bis;                 /**< Total number of BISes in the BIG. */
    uint32_t sdu_interval;           /**< The interval, in microseconds, of periodic SDUs.
                                          Range: 0x0000FF to 0x0FFFFF */
    uint16_t max_sdu;                /**< Maximum size of an SDU, in octets. */
    uint16_t max_transport_latency;  /**< Maximum transport latency, in milliseconds.
                                          Range: 0x0005 to 0x0FA0 */
    uint8_t rtn;                     /**< The number of times that every BIS Data PDU should be retransmitted.
                                          Range: 0x00 to 0x1E */
    uint8_t phy;                     /**< A bit field that indicates the PHY used for transmission of PDUs of BISes in the BIG.
                                          bit 0: The transmitter PHY of packets is LE 1M.
                                          bit 1: The transmitter PHY of packets is LE 2M.
                                          bit 2: The transmitter PHY of packets is LE Coded. */
    uint8_t  packing;                /**< Indicate the preferred method of arranging subevents of multiple BISes.
                                          0x00: Sequential
                                          0x01: Interleaved */
    uint8_t  framing;                /**< Indicate the format for sending BIS Data PDUs.
                                          0x00: Unframed
                                          0x01: Framed */
    uint8_t encryption;              /**< Identify the encryption mode of the BISes.
                                          0x00: Unencrypted
                                          0x01: Encrypted */
    uint8_t broadcast_code[16];      /**< Used to generate the encryption key for encrypting payloads of all BISes. */
} T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_PARAM;

typedef struct
{
    uint16_t cause;
    uint8_t  big_handle;            /**< Used to identify the BIG. */
    uint8_t adv_handle;             /**< Used to identify the periodic advertising train. */
    uint32_t big_sync_delay;        /**< The maximum time in microseconds for transmission of PDUs of all BISes in a BIG event. */
    uint32_t transport_latency_big; /**< The actual transport latency, in microseconds. */
    uint8_t phy;                    /**< The PHY used to create the BIG. @ref T_GAP_PHYS_TYPE. */
    uint8_t nse;                    /**< The number of subevents in each BIS event in the BIG. */
    uint8_t bn;                     /**< The number of new payloads in each BIS event. */
    uint8_t pto;                    /**< Offset used for pre-transmissions. */
    uint8_t irc;                    /**< The number of times a payload is transmitted in a BIS event. */
    uint16_t max_pdu;               /**< Maximum size, in octets, of the payload. */
    uint16_t iso_interval;          /**< The time between two consecutive BIG anchor points.
                                         Range: 0x0004 to 0x0C80
                                         Time = N * 1.25 ms
                                         Time Range: 5 ms to 4 s. */
    uint8_t num_bis;                /**< Total number of BISes in the BIG. */
    T_BIG_MGR_BIS_CONN_HANDLE_INFO bis_conn_handle_info[GAP_BIG_MGR_MAX_BIS_NUM];/**< Connection handle
                                                                                      of BIS. */
} T_BIG_MGR_ISOC_BC_CREATE_BIG_CMPL_INFO;

typedef struct
{
    uint16_t cause;
    uint8_t big_handle;                       /**< Used to identify the BIG. */
    uint8_t adv_handle;                       /**< Used to identify the periodic advertising train. */
    T_GAP_BIG_ISOC_BROADCAST_STATE new_state; /**< @ref T_GAP_BIG_ISOC_BROADCAST_STATE. */
} T_BIG_MGR_ISOC_BROADCAST_STATE_CHANGE_INFO;

typedef struct
{
    uint16_t cause;
    uint16_t bis_conn_handle;        /**< Connection handle of a BIS. */
    uint8_t  big_handle;             /**< Used to identify the BIG. */
    uint8_t adv_handle;              /**< Used to identify the periodic advertising train. */
    uint16_t packet_sequence_number; /**< The packet sequence number of an SDU. */
    uint32_t time_stamp;             /**< The BIG anchor point of a transmitted SDU derived
                                          using the Controller's free running reference clock (in microseconds). */
    uint32_t time_offset;            /**< The time offset, in microseconds, that is associated with a transmitted SDU. */
} T_BIG_MGR_READ_ISO_TX_SYNC_RSP;

#if F_BT_LE_5_2_ISOC_TEST_MODE_BIS_BROADCASTER_SUPPORT
typedef struct
{
    uint8_t num_bis;       /**< Total number of BISes in the BIG. */
    uint32_t sdu_interval; /**< The interval, in microseconds, of periodic SDUs.
                                Range: 0x0000FF to 0x0FFFFF */
    uint16_t iso_interval; /**< The time between consecutive BIG anchor points.
                                Range: 0x0004 to 0x0C80
                                Time = N * 1.25 ms
                                Time Range: 5 ms to 4 s. */
    uint8_t nse;           /**< The number of subevents in each BIS event in the BIG.
                                Range: 0x01 to 0x1F */
    uint16_t max_sdu;      /**< Maximum size, in octets, of an SDU. */
    uint16_t max_pdu;      /**< Maximum size, in octets, of the payload.
                                Range: 0x0001 to 0x00FB*/
    uint8_t phy;           /**< A bit field that indicates the PHY used for transmission of PDUs of BISes in the BIG.
                                bit 0: The transmitter PHY of packets is LE 1M.
                                bit 1: The transmitter PHY of packets is LE 2M.
                                bit 2: The transmitter PHY of packets is LE Coded. */
    uint8_t packing;       /**< Indicate the preferred method of arranging subevents of multiple BISes.
                                0x00: Sequential
                                0x01: Interleaved */
    uint8_t framing;       /**< Indicate the format for sending BIS Data PDUs.
                                0x00: Unframed
                                0x01: Framed */
    uint8_t bn;            /**< The number of new payloads in each interval for each BIS.
                                Range: 0x01 to 0x07 */
    uint8_t irc;           /**< The number of times the scheduled payloads are transmitted in a given event.
                                Range: 0x01 to 0x0F */
    uint8_t pto;           /**< Offset used for pre-transmissions.
                                Range: 0x00 to 0x0F */
    uint8_t encryption;    /**< Identify the encryption mode of the BISes.
                                0x00: Unencrypted
                                0x01: Encrypted */
    uint8_t broadcast_code[16];  /**< Used to generate the encryption key for encrypting payloads of all BISes. */
} T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_TEST_PARAM;

typedef struct
{
    uint16_t cause;
    uint16_t bis_conn_handle;    /**< Connection handle of a BIS. */
    uint8_t  big_handle;         /**< Used to identify the BIG. */
    uint8_t adv_handle;          /**< Used to identify the periodic advertising train. */
} T_BIG_MGR_TRANSMIT_TEST_RSP;
#endif
#endif

#if F_BT_LE_5_2_ISOC_BIS_RECEIVER_SUPPORT
typedef struct
{
    uint8_t gap_big_create_sync_state: 1;/**< @ref GAP_BIG_MGR_SYNC_RECEIVER_CREATE_SYNC_DEV_STATE. */
    uint8_t gap_big_sync_receiver_rfu_state: 7;  /**< Reserved for future use. */
} T_BIG_MGR_SYNC_RECEIVER_DEV_STATE;

typedef struct
{
    T_BIG_MGR_SYNC_RECEIVER_DEV_STATE  state;
    uint16_t cause;
} T_BIG_MGR_SYNC_RECEIVER_DEV_STATE_CHANGE_INFO;

typedef struct
{
    uint16_t cause;
    uint8_t big_handle;                         /**< The identifier of the BIG. */
    uint8_t sync_id;                            /**< Identify the periodic advertising train. */
    uint16_t  sync_handle;                      /**< Sync_Handle identifying the periodic advertising train. */
    T_GAP_BIG_SYNC_RECEIVER_SYNC_STATE state;   /**< @ref T_GAP_BIG_SYNC_RECEIVER_SYNC_STATE. */
} T_BIG_MGR_SYNC_RECEIVER_SYNC_STATE_CHANGE_INFO;

typedef struct
{
    uint8_t encryption;                    /**< Identify the encryption mode of the BIG.
                                                0x00: Unencrypted
                                                0x01: Encrypted */
    uint8_t broadcast_code[16];            /**< Used to generate the session key to decrypt payloads of an encrypted BIS. */
    uint8_t mse;                           /**< The maximum number of subevents that a Controller should use to receive data
                                                payloads in each interval for a BIS.
                                                0x00: The Controller can schedule reception of any number of subevents up
                                                      to NSE.
                                                0x01 to 0x1F: Maximum number of subevents that should be used to receive data
                                                              payloads in each BIS event      */
    uint16_t big_sync_timeout;             /**< Specify the maximum permitted time between successful receptions of BIS PDUs.
                                                Range: 0x000A to 0x4000
                                                Time = N*10 ms
                                                Time Range: 100 ms to 163.84 s */
    uint8_t  num_bis;                      /**< Total number of BISes to synchronize. */
    uint8_t  bis[GAP_BIG_MGR_MAX_BIS_NUM]; /**< A list of indices corresponding to BIS(es). */
} T_BIG_MGR_SYNC_RECEIVER_BIG_CREATE_SYNC_PARAM;

typedef struct
{
    uint16_t cause;
    uint8_t  big_handle;             /**< The identifier of the BIG. */
    uint8_t sync_id;                 /**< Identify the periodic advertising train. */
    uint16_t sync_handle;            /**< Sync_Handle identifying the periodic advertising train. */
    uint32_t transport_latency_big;  /**< The actual transport latency, in microseconds. */
    uint8_t nse;                     /**< The number of subevents in each BIS event in the BIG. */
    uint8_t bn;                      /**< The number of new payloads in each BIS event. */
    uint8_t pto;                     /**< Offset used for pre-transmissions. */
    uint8_t irc;                     /**< The number of times a payload is transmitted in a BIS event. */
    uint16_t max_pdu;                /**< Maximum size, in octets, of the payload. */
    uint16_t iso_interval;           /**< The time between two consecutive BIG anchor points.
                                          Range: 0x0004 to 0x0C80
                                          Time = N * 1.25 ms
                                          Time Range: 5 ms to 4 s. */
    uint8_t num_bis;                 /**< Indicate the number of BISes in the synchronized BIG specified
                                          by @ref gap_big_mgr_sync_receiver_big_create_sync. */
    T_BIG_MGR_BIS_CONN_HANDLE_INFO bis_conn_handle_info[GAP_BIG_MGR_MAX_BIS_NUM]; /**< Connection handle
                                                                                       of BIS. */
} T_BIG_MGR_SYNC_RECEIVER_BIG_SYNC_ESTABLISHED_INFO;

typedef struct
{
    uint16_t cause;
    uint16_t bis_conn_handle;          /**< Connection handle of BIS. */
    uint8_t  big_handle;               /**< The identifier of the BIG. */
    uint8_t sync_id;                   /**< Identify the periodic advertising train. */
    uint16_t sync_handle;              /**< Sync_Handle identifying the periodic advertising train. */
    uint32_t tx_unacked_packets;       /**< The value shall be ignored for BIS. */
    uint32_t tx_flushed_packets;       /**< The value shall be ignored for BIS. */
    uint32_t tx_last_subevent_packets; /**< The value shall be ignored for BIS. */
    uint32_t retransmitted_packets;    /**< The value shall be ignored for BIS. */
    uint32_t crc_error_packets;         /**< Value of the CRC_Error_Packets counter.
                                             When Incremented: The Link Layer receives a packet with a CRC error. */
    uint32_t rx_unreceived_packets;     /**< Value of the Rx_Unreceived_Packets counter.
                                             When Incremented: The Link Layer does not receive a specific payload by its flush point (on
                                                               a BIS) or the end of the event it is associated with (on a BIS). */
    uint32_t duplicate_packets;         /**< The value shall be ignored for BIS. */
} T_BIG_MGR_READ_ISO_LINK_QUALITY_RSP;

#if F_BT_LE_5_2_ISOC_TEST_MODE_BIS_RECEIVER_SUPPORT
typedef struct
{
    uint16_t cause;
    uint16_t bis_conn_handle; /**< Connection handle of BIS. */
    uint8_t big_handle;       /**< The identifier of the BIG. */
    uint8_t sync_id;          /**< Identify the periodic advertising train. */
    uint16_t  sync_handle;    /**< Sync_Handle identifying the periodic advertising train. */
} T_BIG_MGR_RECEIVE_TEST_RSP;

typedef struct
{
    uint16_t cause;
    uint16_t bis_conn_handle;        /**< Connection handle of BIS. */
    uint8_t big_handle;              /**< The identifier of the BIG. */
    uint8_t sync_id;                 /**< Identify the periodic advertising train. */
    uint16_t  sync_handle;           /**< Sync_Handle identifying the periodic advertising train. */
    uint32_t received_packet_count;  /**< Number in the Received_SDU_Count.
                                          The Received_SDU_Count shall be incremented by one for every valid received test
                                          ISO Data SDU containing a size in the expected range and an SDU counter that matches
                                          the expected value. */
    uint32_t missed_packet_count;    /**< Number in the Missed_SDU_Count.
                                          The Missed_SDU_Count shall be incremented by one for each SDU that is made up of
                                          one or more invalid or missing PDUs. */
    uint32_t failed_packet_count;    /**< Number in the Failed_SDU_Count.
                                          The Failed_SDU_Count shall be incremented by one for each valid received test SDU
                                          where the size or SDU counter does not match the expected size or value. */
} T_BIG_MGR_READ_TEST_COUNTERS_RSP;
#endif
#endif

typedef union
{
    T_BIG_MGR_SETUP_DATA_PATH_RSP                  *p_big_mgr_setup_data_path_rsp;
    T_BIG_MGR_REMOVE_DATA_PATH_RSP                 *p_big_mgr_remove_data_path_rsp;
#if F_BT_LE_5_2_ISOC_TEST_MODE_BIS_SUPPORT
    T_BIG_MGR_ISO_TEST_END_RSP                     *p_big_mgr_iso_test_end_rsp;
#endif

#if F_BT_LE_5_2_ISOC_BIS_BROADCASTER_SUPPORT
    T_BIG_MGR_ISOC_BROADCAST_STATE_CHANGE_INFO     *p_big_mgr_isoc_broadcast_state_change_info;
    T_BIG_MGR_ISOC_BC_CREATE_BIG_CMPL_INFO         *p_big_mgr_isoc_bc_create_big_cmpl_info;
    T_BIG_MGR_READ_ISO_TX_SYNC_RSP                 *p_big_mgr_read_iso_tx_sync_rsp;
#if F_BT_LE_5_2_ISOC_TEST_MODE_BIS_BROADCASTER_SUPPORT
    T_BIG_MGR_TRANSMIT_TEST_RSP                    *p_big_mgr_transmit_test_rsp;
#endif
#endif
#if F_BT_LE_5_2_ISOC_BIS_RECEIVER_SUPPORT
    T_BIG_MGR_SYNC_RECEIVER_DEV_STATE_CHANGE_INFO
    *p_big_mgr_sync_receiver_dev_state_change_info;
    T_BIG_MGR_SYNC_RECEIVER_SYNC_STATE_CHANGE_INFO
    *p_big_mgr_sync_receiver_sync_state_change_info;
    T_BIG_MGR_SYNC_RECEIVER_BIG_SYNC_ESTABLISHED_INFO
    *p_big_mgr_sync_rx_big_sync_established_info;
    T_BIG_MGR_READ_ISO_LINK_QUALITY_RSP    *p_big_mgr_read_iso_link_quality_rsp;
#if F_BT_LE_5_2_ISOC_TEST_MODE_BIS_RECEIVER_SUPPORT
    T_BIG_MGR_RECEIVE_TEST_RSP             *p_big_mgr_receive_test_rsp;
    T_BIG_MGR_READ_TEST_COUNTERS_RSP       *p_big_mgr_read_test_counters_rsp;
#endif
#endif
} T_BIG_MGR_CB_DATA;
/** End of GAP_LE_BIG_MGR_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_BIG_MGR_Exported_Functions GAP LE BIG Mgr Exported Functions
  * @brief
  * @{
  */

/**
 * @brief       Initialize the number of BIG and BIS.
 *
 *              NOTE: This function is used for @ref BIG_MGR_ROLE_ISOC_BROADCASTER and @ref BIG_MGR_ROLE_SYNC_RECEIVER.
 *
 * @param[in]   big_handle_num     BIG handle number.
 * @param[in]   bis_num            BIS number.
 * @return      GAP Operation result.
 * @retval      GAP_CAUSE_SUCCESS       GAP operation success.
 * @retval      GAP_CAUSE_NO_RESOURCE   Operation failure, memory acquisition failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = gap_big_mgr_init(1, 1);
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_init(uint8_t big_handle_num, uint8_t bis_num);

/**
 * @brief       Identify and create the isochronous data path for BIS.
 *
 *              NOTE: This function is used for @ref BIG_MGR_ROLE_ISOC_BROADCASTER and @ref BIG_MGR_ROLE_SYNC_RECEIVER.
 *
 *              If sending request operation is success, the result of creating isochronous data path will be returned by
 *              @ref app_isoc_bis_broadcaster_cb for @ref BIG_MGR_ROLE_ISOC_BROADCASTER
 *              or @ref app_isoc_bis_receiver_cb for @ref BIG_MGR_ROLE_SYNC_RECEIVER
 *              with cb_type @ref MSG_BIG_MGR_SETUP_DATA_PATH.
 *
 * @param[in]   bis_conn_handle      Connection handle of the BIS.
 * @param[in]   data_path_direction  Specify the direction for which the data path is being configured. @ref LE_ISOC_DEF_SETUP_DATA_PATH_DATA_PATH_DIRECTION.
 *                                   @ref BIG_MGR_ROLE_ISOC_BROADCASTER uses @ref DATA_PATH_ADD_INPUT.
 *                                   @ref BIG_MGR_ROLE_SYNC_RECEIVER uses @ref DATA_PATH_ADD_OUTPUT.
 * @param[in]   data_path_id         Specify the data transport path used.
 *                                   0x00: HCI. @ref DATA_PATH_ID_HCI.
 *                                   0x01 to 0xFE: Logical_Channel_Number. The meaning of the logical channel is vendor-specific.
 * @param[in]   codec_id             Specify the coding format used over the air.
 *                                   Octet 0: See Assigned Numbers for Coding Format
 *                                   Octets 1 to 2: Company ID, see Assigned Numbers for Company Identifier. Shall be ignored if octet 0 is not 0xFF.
 *                                   Octets 3 to 4: Vendor-defined codec ID. Shall be ignored if octet 0 is not 0xFF.
 * @param[in]   controller_delay     Controller delay in microseconds.
 *                                   Range: 0x000000 to 0x3D0900
 *                                   Time range: 0 s to 4 s
 * @param[in]   codec_config_len     Length of codec configuration.
 * @param[in]   p_codec_config       Specify codec-specific configuration information for the specified direction.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = gap_big_mgr_setup_data_path(bis_conn_handle, data_path_direction, data_path_id, codec_id, controller_delay, codec_config_len, p_codec_config);
    }

    T_APP_RESULT app_isoc_bis_broadcaster_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case MSG_BIG_MGR_SETUP_DATA_PATH:
            {
                APP_PRINT_INFO3("MSG_BIG_MGR_SETUP_DATA_PATH: cause 0x%x, big_handle 0x%x, bis_conn_handle 0x%x",
                                p_data->p_big_mgr_setup_data_path_rsp->cause,
                                p_data->p_big_mgr_setup_data_path_rsp->big_handle,
                                p_data->p_big_mgr_setup_data_path_rsp->bis_conn_handle);
            }
            break;
            ......
    }

    T_APP_RESULT app_isoc_bis_receiver_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case MSG_BIG_MGR_SETUP_DATA_PATH:
            {
                APP_PRINT_INFO3("MSG_BIG_MGR_SETUP_DATA_PATH: cause 0x%x, big_handle 0x%x, bis_conn_handle 0x%x",
                                p_data->p_big_mgr_setup_data_path_rsp->cause,
                                p_data->p_big_mgr_setup_data_path_rsp->big_handle,
                                p_data->p_big_mgr_setup_data_path_rsp->bis_conn_handle);
            }
            break;
            ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_setup_data_path(uint16_t bis_conn_handle, uint8_t data_path_direction,
                                        uint8_t data_path_id, uint8_t codec_id[5], uint32_t controller_delay,
                                        uint8_t codec_config_len, uint8_t *p_codec_config);

/**
 * @brief       Remove the input or output data path associated with BIS.
 *
 *              NOTE: This function is used for @ref BIG_MGR_ROLE_ISOC_BROADCASTER and @ref BIG_MGR_ROLE_SYNC_RECEIVER.
 *
 *              If sending request operation is success, the result of removing data path will be returned by
 *              @ref app_isoc_bis_broadcaster_cb for @ref BIG_MGR_ROLE_ISOC_BROADCASTER
 *              or @ref app_isoc_bis_receiver_cb for @ref BIG_MGR_ROLE_SYNC_RECEIVER
 *              with cb_type @ref MSG_BIG_MGR_REMOVE_DATA_PATH.
 *
 * @param[in]   bis_conn_handle      Connection handle of the BIS.
 * @param[in]   data_path_direction  Specify which directions are to have the data path removed. @ref LE_ISOC_DEF_DATA_PATH_DIRECTION_FLAG.
 *                                   @ref BIG_MGR_ROLE_ISOC_BROADCASTER uses @ref DATA_PATH_INPUT_FLAG.
 *                                   @ref BIG_MGR_ROLE_SYNC_RECEIVER uses @ref DATA_PATH_OUTPUT_FLAG.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = gap_big_mgr_remove_data_path(bis_conn_handle, data_path_direction);
    }

    T_APP_RESULT app_isoc_bis_broadcaster_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case MSG_BIG_MGR_REMOVE_DATA_PATH:
            {
                APP_PRINT_INFO3("MSG_BIG_MGR_REMOVE_DATA_PATH: cause 0x%x, big_handle 0x%x, bis_conn_handle 0x%x",
                                p_data->p_big_mgr_remove_data_path_rsp->cause,
                                p_data->p_big_mgr_remove_data_path_rsp->big_handle,
                                p_data->p_big_mgr_remove_data_path_rsp->bis_conn_handle);
            }
            break;
            ......
    }

    T_APP_RESULT app_isoc_bis_receiver_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case MSG_BIG_MGR_REMOVE_DATA_PATH:
            {
                APP_PRINT_INFO3("MSG_BIG_MGR_REMOVE_DATA_PATH: cause 0x%x, big_handle 0x%x, bis_conn_handle 0x%x",
                                p_data->p_big_mgr_remove_data_path_rsp->cause,
                                p_data->p_big_mgr_remove_data_path_rsp->big_handle,
                                p_data->p_big_mgr_remove_data_path_rsp->bis_conn_handle);
            }
            break;
            ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_remove_data_path(uint16_t bis_conn_handle, uint8_t data_path_direction);

#if F_BT_LE_5_2_ISOC_TEST_MODE_BIS_SUPPORT
/**
 * @brief       Terminate the ISO Transmit or Receive Test mode for a BIS.
 *
 *              NOTE: This function is used in ISO Test mode and only for testing purposes for @ref BIG_MGR_ROLE_ISOC_BROADCASTER and @ref BIG_MGR_ROLE_SYNC_RECEIVER.
 *
 *              If sending request operation is success, the result of termination will be returned by
 *              @ref app_isoc_bis_broadcaster_cb for @ref BIG_MGR_ROLE_ISOC_BROADCASTER
 *              or @ref app_isoc_bis_receiver_cb for @ref BIG_MGR_ROLE_SYNC_RECEIVER
 *              with cb_type @ref MSG_BIG_MGR_ISO_TEST_END.
 *
 * @param[in]   bis_conn_handle      Connection handle of the BIS.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = gap_big_mgr_iso_test_end(bis_conn_handle);
    }

    T_APP_RESULT app_isoc_bis_broadcaster_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case MSG_BIG_MGR_ISO_TEST_END:
            {
                APP_PRINT_INFO6("MSG_BIG_MGR_ISO_TEST_END: cause 0x%x, bis_conn_handle 0x%x, big_handle 0x%x, received_packet_count 0x%x, missed_packet_count 0x%x, failed_packet_count 0x%x",
                                p_data->p_big_mgr_iso_test_end_rsp->cause, p_data->p_big_mgr_iso_test_end_rsp->bis_conn_handle,
                                p_data->p_big_mgr_iso_test_end_rsp->big_handle,
                                p_data->p_big_mgr_iso_test_end_rsp->received_packet_count,
                                p_data->p_big_mgr_iso_test_end_rsp->missed_packet_count,
                                p_data->p_big_mgr_iso_test_end_rsp->failed_packet_count);
            }
            break;
            ......
    }

    T_APP_RESULT app_isoc_bis_receiver_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case MSG_BIG_MGR_ISO_TEST_END:
            {
                APP_PRINT_INFO6("MSG_BIG_MGR_ISO_TEST_END: cause 0x%x, bis_conn_handle 0x%x, big_handle 0x%x, received_packet_count 0x%x, missed_packet_count 0x%x, failed_packet_count 0x%x",
                                p_data->p_big_mgr_iso_test_end_rsp->cause, p_data->p_big_mgr_iso_test_end_rsp->bis_conn_handle,
                                p_data->p_big_mgr_iso_test_end_rsp->big_handle,
                                p_data->p_big_mgr_iso_test_end_rsp->received_packet_count,
                                p_data->p_big_mgr_iso_test_end_rsp->missed_packet_count,
                                p_data->p_big_mgr_iso_test_end_rsp->failed_packet_count);
            }
            break;
            ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_iso_test_end(uint16_t bis_conn_handle);
#endif

#if F_BT_LE_5_2_ISOC_BIS_BROADCASTER_SUPPORT
/**
  * @brief     Callback for GAP BIG Mgr @ref BIG_MGR_ROLE_ISOC_BROADCASTER to notify app
  *
  * @param[in] big_handle  Used to identify the BIG.
  * @param[in] cb_type     Callback msy type @ref BIG_MGR_ROLE_ISOC_BROADCASTER in @ref BIG_MGR_CALLBACK_MSG.
  * @param[in] p_cb_data   Point to callback data @ref T_BIG_MGR_CB_DATA.
  * @retval    result      @ref T_APP_RESULT
  */
typedef T_APP_RESULT(*P_FUN_LE_BIG_MGR_ISOC_BC_CB)(uint8_t big_handle, uint8_t cb_type,
                                                   void *p_cb_data);

/**
 * @brief       Initialize big configuration and register callback for GAP BIG Mgr @ref BIG_MGR_ROLE_ISOC_BROADCASTER.
 *
 *              NOTE: This function is used for @ref BIG_MGR_ROLE_ISOC_BROADCASTER.
 *
 * @param[in]   big_num   BIG handle number.
 * @param[in]   bis_num   BIS number.
 * @param[in]   cb_pfn    Callback.
 *
 * @return GAP operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others             GAP operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = gap_big_mgr_init(big_num, bis_num);
        cause = gap_big_mgr_isoc_broadcaster_init(broadcaster_big_num, broadcaster_bis_num, app_isoc_bis_broadcaster_cb);
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_isoc_broadcaster_init(uint8_t big_num, uint8_t bis_num,
                                              P_FUN_LE_BIG_MGR_ISOC_BC_CB cb_pfn);

/**
 * @brief       Create a BIG with one or more BISes.
 *
 *              NOTE: This function is used for @ref BIG_MGR_ROLE_ISOC_BROADCASTER.
 *
 *              If sending request operation is success, the result of creating BIG will be returned by
 *              @ref app_isoc_bis_broadcaster_cb for @ref BIG_MGR_ROLE_ISOC_BROADCASTER with cb_type
 *              @ref MSG_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_CMPL_INFO and @ref MSG_BIG_MGR_ISOC_BROADCAST_STATE_CHANGE_INFO.
 *
 * @param[in]     adv_handle          Identify the periodic advertising train.
 * @param[in]     p_create_big_param  GAP Isochrounous Broadcaster create BIG parameter. @ref T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_PARAM.
 * @param[in,out] p_big_handle        Identify the BIG.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t big_handle = GAP_INVALID_BIG_HANDLE;
        T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_PARAM create_big_param;
        memset(&create_big_param, 0, sizeof(T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_PARAM));

        ......
        // Initialization of create_big_param is dependent on Application.

        T_GAP_CAUSE cause = gap_big_mgr_isoc_broadcaster_create_big(adv_handle, &create_big_param, &big_handle);
    }

    void app_handle_bis_broadcast_state_evt(uint8_t big_handle, uint8_t adv_handle,
                                            T_GAP_BIG_ISOC_BROADCAST_STATE new_state, uint16_t cause)
    {
        ......
        switch (new_state)
        {
        case BIG_ISOC_BROADCAST_STATE_IDLE:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_CREATING_EXT_ADV_STATE_PA_ADV_STATE_IDLE:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_WAIT_EXT_ADV_STATE_PA_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_CREATING_PA_ADV_STATE_IDLE:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_WAIT_PA_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_CREATING_EXT_ADV_STATE_IDLE:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_WAIT_EXT_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_CREATING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_BROADCASTING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_TERMINATING:
            {
            }
            break;

        default:
            break;
        }
    }

    T_APP_RESULT app_isoc_bis_broadcaster_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case MSG_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_CMPL_INFO:
            APP_PRINT_INFO7("MSG_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_CMPL_INFO: big_handle 0x%x, adv_handle 0x%x, cause 0x%x, big_sync_delay 0x%x, transport_latency_big 0x%x, phy %d, nse %d",
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->big_handle,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->adv_handle,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->cause,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->big_sync_delay,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->transport_latency_big,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->phy,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->nse);
            APP_PRINT_INFO8("MSG_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_CMPL_INFO: big_handle 0x%x, adv_handle 0x%x, bn %d, pto %d, irc %d, max_pdu %d, iso_interval 0x%x, num_bis %d",
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->big_handle,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->adv_handle,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->bn,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->pto,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->irc,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->max_pdu,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->iso_interval,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->num_bis);
            if (p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->cause == GAP_SUCCESS)
            {
                for (uint8_t i = 0; i < p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->num_bis; i++)
                {
                    APP_PRINT_INFO2("MSG_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_CMPL_INFO: bis index %d, bis_conn_handle 0x%x",
                                    p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->bis_conn_handle_info[i].bis_idx,
                                    p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->bis_conn_handle_info[i].bis_conn_handle);
                }
            }
            break;

        case MSG_BIG_MGR_ISOC_BROADCAST_STATE_CHANGE_INFO:
            {
                app_handle_bis_broadcast_state_evt(p_data->p_big_mgr_isoc_broadcast_state_change_info->big_handle,
                                                p_data->p_big_mgr_isoc_broadcast_state_change_info->adv_handle,
                                                (T_GAP_BIG_ISOC_BROADCAST_STATE)p_data->p_big_mgr_isoc_broadcast_state_change_info->new_state,
                                                p_data->p_big_mgr_isoc_broadcast_state_change_info->cause);
            }
            break;
            ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_isoc_broadcaster_create_big(uint8_t adv_handle,
                                                    T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_PARAM *p_create_big_param, uint8_t *p_big_handle);

/**
 * @brief       Terminate a BIG.
 *
 *              NOTE: This function is used for @ref BIG_MGR_ROLE_ISOC_BROADCASTER.
 *
 *              If sending request operation is success, the result of creating BIG will be returned by
 *              @ref app_isoc_bis_broadcaster_cb for @ref BIG_MGR_ROLE_ISOC_BROADCASTER with cb_type
 *              @ref MSG_BIG_MGR_ISOC_BROADCAST_STATE_CHANGE_INFO.
 *
 * @param[in]     big_handle          Identify the BIG.
 * @param[in]     reason              Reason the BIG is terminated. @ref BT_HCI_ERROR (except @ref HCI_SUCCESS).
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = gap_big_mgr_isoc_broadcaster_terminate_big(big_handle, reason);
    }

    void app_handle_bis_broadcast_state_evt(uint8_t big_handle, uint8_t adv_handle,
                                            T_GAP_BIG_ISOC_BROADCAST_STATE new_state, uint16_t cause)
    {
        ......
        switch (new_state)
        {
        case BIG_ISOC_BROADCAST_STATE_IDLE:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_CREATING_EXT_ADV_STATE_PA_ADV_STATE_IDLE:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_WAIT_EXT_ADV_STATE_PA_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_CREATING_PA_ADV_STATE_IDLE:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_WAIT_PA_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_CREATING_EXT_ADV_STATE_IDLE:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_WAIT_EXT_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_CREATING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_BROADCASTING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_TERMINATING:
            {
            }
            break;

        default:
            break;
        }
    }

    T_APP_RESULT app_isoc_bis_broadcaster_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case MSG_BIG_MGR_ISOC_BROADCAST_STATE_CHANGE_INFO:
            {
                app_handle_bis_broadcast_state_evt(p_data->p_big_mgr_isoc_broadcast_state_change_info->big_handle,
                                                p_data->p_big_mgr_isoc_broadcast_state_change_info->adv_handle,
                                                (T_GAP_BIG_ISOC_BROADCAST_STATE)p_data->p_big_mgr_isoc_broadcast_state_change_info->new_state,
                                                p_data->p_big_mgr_isoc_broadcast_state_change_info->cause);
            }
            break;
            ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_isoc_broadcaster_terminate_big(uint8_t big_handle, uint8_t reason);

/**
 * @brief       Read the TX_Time_Stamp and Time_Offset of a transmitted SDU identified by the Packet_Sequence_Number on a BIS.
 *
 *              NOTE: This function is used for @ref BIG_MGR_ROLE_ISOC_BROADCASTER.
 *
 *              If sending request operation is success, the reading result will be returned by
 *              @ref app_isoc_bis_broadcaster_cb for @ref BIG_MGR_ROLE_ISOC_BROADCASTER
 *              with cb_type @ref MSG_BIG_MGR_READ_ISO_TX_SYNC.
 *
 * @param[in]   bis_conn_handle   Connection handle of the BIS.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = gap_big_mgr_read_iso_tx_sync(bis_conn_handle);
    }

    T_APP_RESULT app_isoc_bis_broadcaster_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case MSG_BIG_MGR_READ_ISO_TX_SYNC:
            {
                APP_PRINT_INFO7("MSG_BIG_MGR_READ_ISO_TX_SYNC: cause 0x%x, bis_conn_handle 0x%x, big_handle 0x%x, adv_handle 0x%x, packet_sequence_number 0x%x, time_stamp 0x%x, time_offset 0x%x",
                                p_data->p_big_mgr_read_iso_tx_sync_rsp->cause,
                                p_data->p_big_mgr_read_iso_tx_sync_rsp->bis_conn_handle,
                                p_data->p_big_mgr_read_iso_tx_sync_rsp->big_handle,
                                p_data->p_big_mgr_read_iso_tx_sync_rsp->adv_handle,
                                p_data->p_big_mgr_read_iso_tx_sync_rsp->packet_sequence_number,
                                p_data->p_big_mgr_read_iso_tx_sync_rsp->time_stamp,
                                p_data->p_big_mgr_read_iso_tx_sync_rsp->time_offset);
            }
            break;
            ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_read_iso_tx_sync(uint16_t bis_conn_handle);


#if F_BT_LE_5_2_ISOC_TEST_MODE_BIS_BROADCASTER_SUPPORT
/**
 * @brief       Create a BIG with one or more BISes in ISO Test mode.
 *
 *              NOTE: This function is used in ISO Test mode and only for testing purposes for @ref BIG_MGR_ROLE_ISOC_BROADCASTER.
 *
 *              If sending request operation is success, the result of creating BIG will be returned by
 *              @ref app_isoc_bis_broadcaster_cb for @ref BIG_MGR_ROLE_ISOC_BROADCASTER with cb_type
 *              @ref MSG_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_CMPL_INFO and @ref MSG_BIG_MGR_ISOC_BROADCAST_STATE_CHANGE_INFO.
 *
 * @param[in]     adv_handle               Identify the periodic advertising train.
 * @param[in]     p_create_big_test_param  GAP Isochrounous Broadcaster create BIG test parameter. @ref T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_TEST_PARAM.
 * @param[in,out] p_big_handle             Identify the BIG.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t big_handle = GAP_INVALID_BIG_HANDLE;
        T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_TEST_PARAM create_big_test_param;
        memset(&create_big_test_param, 0, sizeof(T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_TEST_PARAM));

        ......
        // Initialization of create_big_test_param is dependent on Application.

        T_GAP_CAUSE cause = gap_big_mgr_isoc_broadcaster_create_big_test(adv_handle, &create_big_test_param, &big_handle);
    }

    void app_handle_bis_broadcast_state_evt(uint8_t big_handle, uint8_t adv_handle,
                                            T_GAP_BIG_ISOC_BROADCAST_STATE new_state, uint16_t cause)
    {
        ......
        switch (new_state)
        {
        case BIG_ISOC_BROADCAST_STATE_IDLE:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_CREATING_EXT_ADV_STATE_PA_ADV_STATE_IDLE:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_WAIT_EXT_ADV_STATE_PA_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_CREATING_PA_ADV_STATE_IDLE:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_WAIT_PA_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_CREATING_EXT_ADV_STATE_IDLE:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_WAIT_EXT_ADV_STATE_ADVERTISING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_CREATING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_BROADCASTING:
            {
            }
            break;

        case BIG_ISOC_BROADCAST_STATE_TERMINATING:
            {
            }
            break;

        default:
            break;
        }
    }

    T_APP_RESULT app_isoc_bis_broadcaster_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case MSG_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_CMPL_INFO:
            APP_PRINT_INFO7("MSG_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_CMPL_INFO: big_handle 0x%x, adv_handle 0x%x, cause 0x%x, big_sync_delay 0x%x, transport_latency_big 0x%x, phy %d, nse %d",
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->big_handle,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->adv_handle,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->cause,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->big_sync_delay,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->transport_latency_big,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->phy,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->nse);
            APP_PRINT_INFO8("MSG_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_CMPL_INFO: big_handle 0x%x, adv_handle 0x%x, bn %d, pto %d, irc %d, max_pdu %d, iso_interval 0x%x, num_bis %d",
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->big_handle,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->adv_handle,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->bn,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->pto,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->irc,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->max_pdu,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->iso_interval,
                            p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->num_bis);
            if (p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->cause == GAP_SUCCESS)
            {
                for (uint8_t i = 0; i < p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->num_bis; i++)
                {
                    APP_PRINT_INFO2("MSG_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_CMPL_INFO: bis index %d, bis_conn_handle 0x%x",
                                    p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->bis_conn_handle_info[i].bis_idx,
                                    p_data->p_big_mgr_isoc_bc_create_big_cmpl_info->bis_conn_handle_info[i].bis_conn_handle);
                }
            }
            break;

        case MSG_BIG_MGR_ISOC_BROADCAST_STATE_CHANGE_INFO:
            {
                app_handle_bis_broadcast_state_evt(p_data->p_big_mgr_isoc_broadcast_state_change_info->big_handle,
                                                p_data->p_big_mgr_isoc_broadcast_state_change_info->adv_handle,
                                                (T_GAP_BIG_ISOC_BROADCAST_STATE)p_data->p_big_mgr_isoc_broadcast_state_change_info->new_state,
                                                p_data->p_big_mgr_isoc_broadcast_state_change_info->cause);
            }
            break;
            ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_isoc_broadcaster_create_big_test(uint8_t adv_handle,
                                                         T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_TEST_PARAM *p_create_big_test_param, uint8_t *p_big_handle);

/**
 * @brief       Configure an established BIS, and transmit test payloads which are generated by the Controller.
 *
 *              NOTE: This function is used in ISO Test mode and only for testing purposes for @ref BIG_MGR_ROLE_ISOC_BROADCASTER.
 *
 *              If sending request operation is success, the result will be returned by
 *              @ref app_isoc_bis_broadcaster_cb for @ref BIG_MGR_ROLE_ISOC_BROADCASTER with cb_type
 *              @ref MSG_BIG_MGR_TRANSMIT_TEST.
 *
 * @param[in]   bis_conn_handle   Connection handle of the BIS.
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
        T_GAP_CAUSE cause = gap_big_mgr_transmit_test(bis_conn_handle, payload_type);
    }

    T_APP_RESULT app_isoc_bis_broadcaster_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case MSG_BIG_MGR_TRANSMIT_TEST:
            {
                APP_PRINT_INFO4("MSG_BIG_MGR_TRANSMIT_TEST: cause 0x%x, bis_conn_handle 0x%x, big_handle 0x%x, adv_handle 0x%x",
                                p_data->p_big_mgr_transmit_test_rsp->cause, p_data->p_big_mgr_transmit_test_rsp->bis_conn_handle,
                                p_data->p_big_mgr_transmit_test_rsp->big_handle,
                                p_data->p_big_mgr_transmit_test_rsp->adv_handle);
            }
            break;
            ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_transmit_test(uint16_t bis_conn_handle, uint8_t payload_type);
#endif
#endif

#if F_BT_LE_5_2_ISOC_BIS_RECEIVER_SUPPORT
/**
  * @brief     Callback for GAP BIG Mgr @ref BIG_MGR_ROLE_SYNC_RECEIVER to notify app
  *
  * @param[in] big_handle  Used to identify the BIG.
  * @param[in] cb_type     Callback msy type @ref BIG_MGR_ROLE_SYNC_RECEIVER in @ref BIG_MGR_CALLBACK_MSG.
  * @param[in] p_cb_data   Point to callback data @ref T_BIG_MGR_CB_DATA.
  * @retval    result      @ref T_APP_RESULT
  */
typedef T_APP_RESULT(*P_FUN_LE_BIG_MGR_SYNC_RX_CB)(uint8_t big_handle, uint8_t cb_type,
                                                   void *p_cb_data);

/**
 * @brief       Initialize big configuration and register callback for GAP BIG Mgr @ref BIG_MGR_ROLE_SYNC_RECEIVER.
 *
 *              NOTE: This function is used for @ref BIG_MGR_ROLE_SYNC_RECEIVER.
 *
 *              NOTE: @ref gap_register_direct_cb should be invoked to receive ISO Data
 *                    via cb_type @ref BT_DIRECT_MSG_ISO_DATA_IND.
 *
 * @param[in]   big_num   BIG handle number.
 * @param[in]   bis_num   BIS number.
 * @param[in]   cb_pfn    Callback.
 *
 * @return GAP operation result.
 * @retval GAP_CAUSE_SUCCESS  GAP operation success.
 * @retval Others             GAP operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = gap_big_mgr_init(big_num, bis_num);
        cause = gap_big_mgr_sync_receiver_init(receiver_big_num, receiver_bis_num, app_isoc_bis_receiver_cb);
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_sync_receiver_init(uint8_t big_num, uint8_t bis_num,
                                           P_FUN_LE_BIG_MGR_SYNC_RX_CB cb_pfn);

/**
 * @brief       Synchronize to a BIG described in the periodic advertising train.
 *
 *              NOTE: This function is used for @ref BIG_MGR_ROLE_SYNC_RECEIVER.
 *
 *              If sending request operation is success, the result of synchronization will be returned by
 *              @ref app_isoc_bis_receiver_cb for @ref BIG_MGR_ROLE_SYNC_RECEIVER with cb_type
 *              @ref MSG_BIG_MGR_SYNC_RECEIVER_DEV_STATE_CHANGE_INFO, @ref MSG_BIG_MGR_SYNC_RECEIVER_SYNC_STATE_CHANGE_INFO
 *              and @ref MSG_BIG_MGR_SYNC_RECEIVER_BIG_SYNC_ESTABLISHED_INFO.
 *
 * @param[in]   sync_handle              Identify the periodic advertising train.
 * @param[in]   p_big_create_sync_param  GAP Synchronized Receiver BIG create sync parameter. @ref T_BIG_MGR_SYNC_RECEIVER_BIG_CREATE_SYNC_PARAM.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t big_handle = GAP_INVALID_BIG_HANDLE;
        T_BIG_MGR_SYNC_RECEIVER_BIG_CREATE_SYNC_PARAM big_create_sync_param;
        memset(&big_create_sync_param, 0, sizeof(T_BIG_MGR_SYNC_RECEIVER_BIG_CREATE_SYNC_PARAM));

        ......
        // Initialization of big_create_sync_param is dependent on Application.

        T_GAP_CAUSE cause = gap_big_mgr_sync_receiver_big_create_sync(sync_handle, &big_create_sync_param, &big_handle);
    }

    void app_handle_big_sync_receiver_dev_state_evt(uint8_t big_handle,
                                                    T_BIG_MGR_SYNC_RECEIVER_DEV_STATE new_state, uint16_t cause)
    {
        ......
        if (app_big_sync_receiver_dev_state.gap_big_create_sync_state !=
            new_state.gap_big_create_sync_state)
        {
            if (new_state.gap_big_create_sync_state == GAP_BIG_MGR_SYNC_RECEIVER_CREATE_SYNC_DEV_STATE_IDLE)
            {
            }
            else if (new_state.gap_big_create_sync_state == GAP_BIG_MGR_SYNC_RECEIVER_CREATE_SYNC_DEV_STATE_SYNCHRONIZING)
            {
            }
        }

        app_big_sync_receiver_dev_state = new_state;
        ......
    }

    void app_handle_big_sync_receiver_sync_state_evt(uint8_t big_handle, uint8_t sync_id,
                                                    uint16_t sync_handle,
                                                    T_GAP_BIG_SYNC_RECEIVER_SYNC_STATE new_state, uint16_t terminate_cause)
    {
        ......
        switch (new_state)
        {
        case BIG_SYNC_RECEIVER_SYNC_STATE_TERMINATED:
            {
            }
            break;

        case BIG_SYNC_RECEIVER_SYNC_STATE_SYNCHRONIZING:
            {
            }
            break;

        case BIG_SYNC_RECEIVER_SYNC_STATE_SYNCHRONIZED:
            {
            }
            break;

        case BIG_SYNC_RECEIVER_SYNC_STATE_TERMINATING:
            {
            }
            break;

        default:
            break;

        }
        ......
    }

    T_APP_RESULT app_isoc_bis_receiver_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case MSG_BIG_MGR_SYNC_RECEIVER_DEV_STATE_CHANGE_INFO:
            {
                app_handle_big_sync_receiver_dev_state_evt(big_handle,
                                                        p_data->p_big_mgr_sync_receiver_dev_state_change_info->state,
                                                        p_data->p_big_mgr_sync_receiver_dev_state_change_info->cause);
            }
            break;

        case MSG_BIG_MGR_SYNC_RECEIVER_SYNC_STATE_CHANGE_INFO:
            {
                app_handle_big_sync_receiver_sync_state_evt(
                    p_data->p_big_mgr_sync_receiver_sync_state_change_info->big_handle,
                    p_data->p_big_mgr_sync_receiver_sync_state_change_info->sync_id,
                    p_data->p_big_mgr_sync_receiver_sync_state_change_info->sync_handle,
                    p_data->p_big_mgr_sync_receiver_sync_state_change_info->state,
                    p_data->p_big_mgr_sync_receiver_sync_state_change_info->cause);
            }
            break;

        case MSG_BIG_MGR_SYNC_RECEIVER_BIG_SYNC_ESTABLISHED_INFO:
            APP_PRINT_INFO6("MSG_BIG_MGR_SYNC_RECEIVER_BIG_SYNC_ESTABLISHED_INFO: big_handle 0x%x, sync_handle 0x%x, sync_id %d, cause 0x%x, transport_latency_big 0x%x, nse %d",
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->big_handle,
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->sync_handle,
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->sync_id,
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->cause,
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->transport_latency_big,
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->nse);
            APP_PRINT_INFO8("MSG_BIG_MGR_SYNC_RECEIVER_BIG_SYNC_ESTABLISHED_INFO: big_handle 0x%x, sync_handle 0x%x, bn %d, pto %d, irc %d, max_pdu %d, iso_interval 0x%x, num_bis %d",
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->big_handle,
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->sync_handle,
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->bn,
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->pto,
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->irc,
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->max_pdu,
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->iso_interval,
                            p_data->p_big_mgr_sync_rx_big_sync_established_info->num_bis);
            if (p_data->p_big_mgr_sync_rx_big_sync_established_info->cause == GAP_SUCCESS)
            {
                for (uint8_t i = 0; i < p_data->p_big_mgr_sync_rx_big_sync_established_info->num_bis; i++)
                {
                    APP_PRINT_INFO2("MSG_BIG_MGR_SYNC_RECEIVER_BIG_SYNC_ESTABLISHED_INFO: bis index %d, bis_conn_handle 0x%x",
                                    p_data->p_big_mgr_sync_rx_big_sync_established_info->bis_conn_handle_info[i].bis_idx,
                                    p_data->p_big_mgr_sync_rx_big_sync_established_info->bis_conn_handle_info[i].bis_conn_handle);
                }
            }
            break;
            ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_sync_receiver_big_create_sync(uint16_t sync_handle,
                                                      T_BIG_MGR_SYNC_RECEIVER_BIG_CREATE_SYNC_PARAM *p_big_create_sync_param, uint8_t *p_big_handle);

/**
 * @brief       Terminate synchronization to the BIG.
 *
 *              NOTE: This function is used for @ref BIG_MGR_ROLE_SYNC_RECEIVER.
 *
 *              If sending request operation is success, the result of termination will be returned by
 *              @ref app_isoc_bis_receiver_cb for @ref BIG_MGR_ROLE_SYNC_RECEIVER with cb_type
 *              @ref MSG_BIG_MGR_SYNC_RECEIVER_SYNC_STATE_CHANGE_INFO.
 *
 * @param[in]   big_handle              Identify the BIG.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = gap_big_mgr_sync_receiver_big_terminate_sync(big_handle);
    }

    void app_handle_big_sync_receiver_sync_state_evt(uint8_t big_handle, uint8_t sync_id,
                                                    uint16_t sync_handle,
                                                    T_GAP_BIG_SYNC_RECEIVER_SYNC_STATE new_state, uint16_t terminate_cause)
    {
        ......
        switch (new_state)
        {
        case BIG_SYNC_RECEIVER_SYNC_STATE_TERMINATED:
            {
            }
            break;

        case BIG_SYNC_RECEIVER_SYNC_STATE_SYNCHRONIZING:
            {
            }
            break;

        case BIG_SYNC_RECEIVER_SYNC_STATE_SYNCHRONIZED:
            {
            }
            break;

        case BIG_SYNC_RECEIVER_SYNC_STATE_TERMINATING:
            {
            }
            break;

        default:
            break;

        }
        ......
    }

    T_APP_RESULT app_isoc_bis_receiver_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        switch (cb_type)
        {
        case MSG_BIG_MGR_SYNC_RECEIVER_SYNC_STATE_CHANGE_INFO:
            {
                app_handle_big_sync_receiver_sync_state_evt(
                    p_data->p_big_mgr_sync_receiver_sync_state_change_info->big_handle,
                    p_data->p_big_mgr_sync_receiver_sync_state_change_info->sync_id,
                    p_data->p_big_mgr_sync_receiver_sync_state_change_info->sync_handle,
                    p_data->p_big_mgr_sync_receiver_sync_state_change_info->state,
                    p_data->p_big_mgr_sync_receiver_sync_state_change_info->cause);
            }
            break;
            ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_sync_receiver_big_terminate_sync(uint8_t big_handle);

/**
 * @brief       Read the values of various counters related to link quality.
 *
 *              NOTE: This function is used for @ref BIG_MGR_ROLE_SYNC_RECEIVER.
 *
 *              If sending request operation is success, the reading result will be returned by
 *              @ref app_isoc_bis_receiver_cb for @ref BIG_MGR_ROLE_SYNC_RECEIVER with cb_type
 *              @ref MSG_BIG_MGR_READ_ISO_LINK_QUALITY.
 *
 * @param[in]   bis_conn_handle              Connection handle of the BIS.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = gap_big_mgr_read_iso_link_quality(bis_conn_handle);
    }

    T_APP_RESULT app_isoc_bis_receiver_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        case MSG_BIG_MGR_READ_ISO_LINK_QUALITY:
            {
                APP_PRINT_INFO7("MSG_BIG_MGR_READ_ISO_LINK_QUALITY: cause 0x%x, bis_conn_handle 0x%x, big_handle 0x%x, sync_id %d, sync_handle 0%x, crc_error_packets 0x%x, rx_unreceived_packets 0x%x",
                                p_data->p_big_mgr_read_iso_link_quality_rsp->cause,
                                p_data->p_big_mgr_read_iso_link_quality_rsp->bis_conn_handle,
                                p_data->p_big_mgr_read_iso_link_quality_rsp->big_handle,
                                p_data->p_big_mgr_read_iso_link_quality_rsp->sync_id,
                                p_data->p_big_mgr_read_iso_link_quality_rsp->sync_handle,
                                p_data->p_big_mgr_read_iso_link_quality_rsp->crc_error_packets,
                                p_data->p_big_mgr_read_iso_link_quality_rsp->rx_unreceived_packets);
            }
            break;
            ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_read_iso_link_quality(uint16_t bis_conn_handle);

#if F_BT_LE_5_2_ISOC_TEST_MODE_BIS_RECEIVER_SUPPORT
/**
 * @brief       Configure a synchronized BIG to receive payloads.
 *
 *              NOTE: This function is used in ISO Test mode and only for testing purposes for @ref BIG_MGR_ROLE_SYNC_RECEIVER.
 *
 *              If sending request operation is success, the result will be returned by
 *              @ref app_isoc_bis_receiver_cb for @ref BIG_MGR_ROLE_SYNC_RECEIVER with cb_type
 *              @ref MSG_BIG_MGR_RECEIVE_TEST.
 *
 * @param[in]   bis_conn_handle   Connection handle of the BIS.
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
        T_GAP_CAUSE cause = gap_big_mgr_receive_test(bis_conn_handle, payload_type);
    }

    T_APP_RESULT app_isoc_bis_receiver_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        case MSG_BIG_MGR_RECEIVE_TEST:
            {
                APP_PRINT_INFO5("MSG_BIG_MGR_RECEIVE_TEST: cause 0x%x, bis_conn_handle 0x%x, big_handle 0x%x, sync_id %d, sync_handle 0x%x",
                                p_data->p_big_mgr_receive_test_rsp->cause, p_data->p_big_mgr_receive_test_rsp->bis_conn_handle,
                                p_data->p_big_mgr_receive_test_rsp->big_handle,
                                p_data->p_big_mgr_receive_test_rsp->sync_id,
                                p_data->p_big_mgr_receive_test_rsp->sync_handle);
            }
            break;
            ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_receive_test(uint16_t bis_conn_handle, uint8_t payload_type);

/**
 * @brief       Read the test counters in the Controller which is configured in ISO Receive Test mode for a BIS.
 *
 *              NOTE: This function is used in ISO Test mode and only for testing purposes for @ref BIG_MGR_ROLE_SYNC_RECEIVER.
 *
 *              If sending request operation is success, the result will be returned by
 *              @ref app_isoc_bis_receiver_cb for @ref BIG_MGR_ROLE_SYNC_RECEIVER with cb_type
 *              @ref MSG_BIG_MGR_READ_TEST_COUNTERS.
 *
 * @param[in]   bis_conn_handle   Connection handle of the BIS.
 *
 * @return Send request operation.
 * @retval GAP_CAUSE_SUCCESS  Send request operation success.
 * @retval Others             Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = gap_big_mgr_read_test_counters(bis_conn_handle);
    }

    T_APP_RESULT app_isoc_bis_receiver_cb(uint8_t big_handle, uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_BIG_MGR_CB_DATA *p_data = (T_BIG_MGR_CB_DATA *)p_cb_data;

        ......
        case MSG_BIG_MGR_READ_TEST_COUNTERS:
            {
                APP_PRINT_INFO8("MSG_BIG_MGR_READ_TEST_COUNTERS: cause 0x%x, bis_conn_handle 0x%x, big_handle 0x%x, sync_id 0x%x, sync_handle 0x%x, received_packet_count 0x%x, missed_packet_count 0x%x, failed_packet_count 0x%x",
                                p_data->p_big_mgr_read_test_counters_rsp->cause,
                                p_data->p_big_mgr_read_test_counters_rsp->bis_conn_handle,
                                p_data->p_big_mgr_read_test_counters_rsp->big_handle,
                                p_data->p_big_mgr_read_test_counters_rsp->sync_id,
                                p_data->p_big_mgr_read_test_counters_rsp->sync_handle,
                                p_data->p_big_mgr_read_test_counters_rsp->received_packet_count,
                                p_data->p_big_mgr_read_test_counters_rsp->missed_packet_count,
                                p_data->p_big_mgr_read_test_counters_rsp->failed_packet_count);
            }
            break;
            ......
    }
 * \endcode
 */
T_GAP_CAUSE gap_big_mgr_read_test_counters(uint16_t bis_conn_handle);
#endif
#endif
#endif

/** End of GAP_LE_BIG_MGR_Exported_Functions
  * @}
  */

/** End of GAP_LE_BIG_MGR
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

#endif /* GAP_BIG_MGR_H */
