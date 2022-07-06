/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      ble_isoch_def.h
* @brief     Header file for LE Isochronous Channels definition
* @details
* @author
* @date      2021-07-12
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _BLE_ISOCH_DEF_H_
#define _BLE_ISOCH_DEF_H_

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#ifdef  __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_ISOC_DEF GAP LE Isochronous Channels Definition Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_ISOC_DEF_Exported_Macros GAP LE Isochronous Channels Definition Exported Macros
  * @{
  */

/** @defgroup LE_ISOC_DEF_SETUP_DATA_PATH_DATA_PATH_DIRECTION LE Isochronous Channels Definition Setup Data Path data path direction flag
  * @brief    Macro definitions to specify the direction for which the data path is being configured.
  * @{
  */
#define DATA_PATH_ADD_INPUT  0x00   /**< Input (Host to Controller). */
#define DATA_PATH_ADD_OUTPUT 0x01   /**< Output (Controller to Host). */
/** End of LE_ISOC_DEF_SETUP_DATA_PATH_DATA_PATH_DIRECTION
  * @}
  */

/** @defgroup LE_ISOC_DEF_DATA_PATH_DIRECTION_FLAG LE Isochronous Channels Definition data path direction flag
  * @brief    Specify which directions have been configured.
  *           Or use the combination of macro definitions to specify which directions are to have the data path
  *           removed.
  * @{
  */
#define DATA_PATH_INPUT_FLAG  0x01 /**< Input (Host to Controller). Remove input data path. */
#define DATA_PATH_OUTPUT_FLAG 0x02 /**< Output (Controller to Host). Remove output data path. */
/** End of LE_ISOC_DEF_DATA_PATH_DIRECTION_FLAG
  * @}
  */

/** @defgroup LE_ISOC_DEF_SETUP_DATA_PATH_DATA_PATH_ID LE Isochronous Channels Definition Setup Data Path data path ID flag
  * @brief    Macro definitions to specify the data transport path used.
  * @{
  */
#define DATA_PATH_ID_HCI     0x00  /**< HCI. */
/** End of LE_ISOC_DEF_SETUP_DATA_PATH_DATA_PATH_ID
  * @}
  */
/** End of GAP_LE_ISOC_DEF_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_ISOC_DEF_Exported_Types GAP LE Isochronous Channels Definition Exported Types
  * @{
  */

/** @brief GAP LE Isochronous Channels CIS connection state. */
typedef enum
{
    ISOCH_STATE_IDLE,               /**< CIS idle. */
    ISOCH_STATE_CONNECTING,         /**< CIS connecting. */
    ISOCH_STATE_CONN,               /**< CIS connected. */
    ISOCH_STATE_DISCONNECTING,      /**< CIS disconnecting. */
    ISOCH_STATE_CONNECTING_CANCEL,  /**< Cancel CIS connecting. */
} T_ISOCH_STATE;

/** @brief GAP LE Isochronous Channels CIS role. */
typedef enum
{
    ISOCH_ROLE_INITIATOR,            /**< CIS - Central role. */
    ISOCH_ROLE_ACCEPTOR,             /**< CIS - Peripheral role. */
} T_ISOCH_ROLE;

/** @brief  Definition of LE Isochronous Channels CIS connection handle. */
typedef struct
{
    T_ISOCH_STATE state;                   /**< CIS Connection state. @ref T_ISOCH_STATE. */
    T_ISOCH_ROLE  role;                    /**< CIS role. @ref T_ISOCH_STATE. */
    uint8_t       data_path_flags;         /**< Specify which directions have been configured.
                                                @ref LE_ISOC_DEF_DATA_PATH_DIRECTION_FLAG. */
    uint8_t       data_path_adding_path;   /**< @ref LE_ISOC_DEF_SETUP_DATA_PATH_DATA_PATH_DIRECTION. */
    bool          data_path_adding;        /**< Indicate whether data path is setup. */
    uint8_t       data_path_removing_path; /**< Specify which directions are to have the data path
                                                removed. @ref LE_ISOC_DEF_DATA_PATH_DIRECTION_FLAG. */
    bool          data_path_removing;      /**< Indicate whether data path is removing. */

    uint32_t cig_sync_delay;               /**< The maximum time, in microseconds, for transmission of PDUs of all CISes in a CIG event.
                                                Range: 0x0000EA to 0x7FFFFF */
    uint32_t cis_sync_delay;               /**< The maximum time, in microseconds, for transmission of PDUs of the specified CIS in a CIG event.
                                                Range: 0x0000EA to 0x7FFFFF */
    uint32_t transport_latency_m_to_s;     /**< The actual transport latency, in microseconds, from Central to Peripheral.
                                                Range: 0x0000EA to 0x7FFFFF */
    uint32_t transport_latency_s_to_m;     /**< The actual transport latency, in microseconds, from Peripheral to Central.
                                                Range: 0x0000EA to 0x7FFFFF */
    uint8_t phy_m_to_s;                    /**< Indicate the PHY selected for packets from the Central to Peripheral. @ref T_GAP_PHYS_TYPE. */
    uint8_t phy_s_to_m;                    /**< Indicate the PHY selected for packets from the Peripheral to Central. @ref T_GAP_PHYS_TYPE. */
    uint8_t nse;                           /**< Maximum number of subevents in each isochronous event. */
    uint8_t bn_m_to_s;                     /**< 0x00: No isochronous data from the Central to the Peripheral.
                                                0x01 to 0x0F: The burst number for Central to Peripheral transmission. */
    uint8_t bn_s_to_m;                     /**< 0x00: No isochronous data from the Peripheral to the Central.
                                                0x01 to 0x0F: The burst number for Peripheral to Central transmission. */
    uint8_t ft_m_to_s;                     /**< The flush timeout, in multiples of the ISO_Interval for the CIS, for each
                                                payload sent from the Central to the Peripheral.
                                                Range: 0x01 to 0xFF */
    uint8_t ft_s_to_m;                     /**< The flush timeout, in multiples of the ISO_Interval for the CIS, for each
                                                payload sent from the Peripheral to the Central.
                                                Range: 0x01 to 0xFF */
    uint16_t max_pdu_m_to_s;               /**< Maximum size, in octets, of the payload from Central to Peripheral. */
    uint16_t max_pdu_s_to_m;               /**< Maximum size, in octets, of the payload from Peripheral to Central. */
    uint16_t iso_interval;                 /**< The time between two consecutive CIS anchor points.
                                                Range: 0x0004 to 0x0C80
                                                Time = N * 1.25 ms
                                                Time Range: 5 ms to 4 s */

    bool acceptor_config_sdu_flag;         /**< Only used by @ref ISOCH_ROLE_ACCEPTOR.
                                                True:  sdu parameters listed below are configured by Application.
                                                       check sdu length when @ref ISOCH_ROLE_ACCEPTOR calls @ref gap_iso_send_data.
                                                False: Default value.
                                                       sdu parameters listed below are not configrued by Application.
                                                       Not check sdu length when @ref ISOCH_ROLE_ACCEPTOR calls @ref gap_iso_send_data. */
    uint32_t sdu_interval_m_to_s;          /**< Valid for @ref ISOCH_ROLE_INITIATOR.
                                                Valid for @ref ISOCH_ROLE_ACCEPTOR if @ref acceptor_config_sdu_flag is True.
                                                The interval, in microseconds, between the start of consecutive SDUs from
                                                the Central's Host for all the CISes in the CIG.
                                                Range: 0x0000FF to 0x0FFFFF
                                               */
    uint32_t sdu_interval_s_to_m;          /**< Valid for @ref ISOCH_ROLE_INITIATOR.
                                                Valid for @ref ISOCH_ROLE_ACCEPTOR if @ref acceptor_config_sdu_flag is True.
                                                The interval, in microseconds, between the start of consecutive SDUs from
                                                the Peripheral's Host for all the CISes in the CIG.
                                                Range: 0x0000FF to 0x0FFFFF
                                                */
    uint16_t max_sdu_m_to_s;               /**< Valid for @ref ISOCH_ROLE_INITIATOR.
                                                Valid for @ref ISOCH_ROLE_ACCEPTOR if @ref acceptor_config_sdu_flag is True.
                                                Maximum size, in octets, of the payload from the Central's Host. */
    uint16_t max_sdu_s_to_m;               /**< Valid for @ref ISOCH_ROLE_INITIATOR.
                                                Valid for @ref ISOCH_ROLE_ACCEPTOR if @ref acceptor_config_sdu_flag is True.
                                                Maximum size, in octets, of the payload from the Peripheral's Host. */
} T_ISOCH_INFO;
/** End of GAP_LE_ISOC_DEF_Exported_Types
  * @}
  */

/** End of GAP_LE_ISOC_DEF
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

#endif /* _BLE_ISOCH_DEF_H_ */
