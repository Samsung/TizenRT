/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __RTK_BT_LE_ISO_H__
#define __RTK_BT_LE_ISO_H__

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <bt_api_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (defined RTK_BLE_ISO_SUPPORT) && (RTK_BLE_ISO_SUPPORT)

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP                                          
#define RTK_BT_LE_ISO_DATA_SEND_ASYNC    0              //data ptr is changed when run is different core, send iso data sync when AP + NP 
#endif 
                                 
#define RTK_BLE_ISO_DATA_SEND_TIMER_IN_API   1

#define RTK_BLE_ISO_DEFAULT_PHY_1M                  (1) /**< bit 0:LE PHY 1M used. */
#define RTK_BLE_ISO_DEFAULT_PHY_2M                  (2) /**< bit 1:LE PHY 2M used. */
#define RTK_BLE_ISO_DEFAULT_PHY_CODED               (4) /**< bit 2:LE Coded PHY used. */

#define RTK_BLE_ISO_DEFAULT_SDU_INTERVAL_M_S        (10) //units: ms
#define RTK_BLE_ISO_DEFAULT_SDU_INTERVAL_S_M        (10) //units: ms 
#define RTK_BLE_ISO_DEFAULT_SDU_INTERVAL_M_S_US     (10000) //units: us
#define RTK_BLE_ISO_DEFAULT_SDU_INTERVAL_S_M_US     (10000) //units: us 
#define RTK_BLE_ISO_DEFAULT_LATENCY_M_S             (10)
#define RTK_BLE_ISO_DEFAULT_LATENCY_S_M             (10)  
#define RTK_BLE_ISO_MAX_SDU_M_S                     (128)   //units: byte
#define RTK_BLE_ISO_MAX_SDU_S_M                     (128)   //units: byte
#define RTK_BLE_ISO_DEFAULT_RTN_M_S                 (2)
#define RTK_BLE_ISO_DEFAULT_RTN_S_M                 (2)   

/**
 * @struct    rtk_bt_le_iso_role_t
 * @brief     Bluetooth BLE ISO role.
 */
typedef enum
{ 
    RTK_BLE_ISO_ROLE_UNKNOWN,
    RTK_BLE_ISO_ROLE_CIS_INITIATOR,            /**< CIS - Central role. */
    RTK_BLE_ISO_ROLE_CIS_ACCEPTOR,             /**< CIS - Peripheral role. */
    RTK_BLE_ISO_ROLE_BIS_BROADCASTER,          /**< BIS - Broadcaster role. */
    RTK_BLE_ISO_ROLE_BIS_RECEIVER,             /**< BIS - Recevier role. */
} rtk_bt_le_iso_role_t;

/**
 * @typedef   rtk_bt_le_iso_data_path_direction_t
 * @brief     Bluetooth LE ISO management ISO Channels Data Path Definition
*/
typedef enum {
    RTK_BLE_ISO_DATA_PATH_ADD_INPUT  = 0x00,    /**< Input (Host to Controller). */
    RTK_BLE_ISO_DATA_PATH_ADD_OUTPUT,           /**< Output (Controller to Host). */
}rtk_bt_le_iso_data_path_direction_t;

/**
 * @typedef   rtk_bt_le_iso_data_path_direction_flag_t
 * @brief     Specify which directions have been configured.Or use the combination of macro definitions to specify which directions are to have the data path removed
*/
typedef enum {
    RTK_BLE_ISO_DATA_PATH_INPUT_FLAG  = 0x01,    /**< Input (Host to Controller). Remove input data path. */
    RTK_BLE_ISO_DATA_PATH_OUTPUT_FLAG,           /**< Output (Controller to Host). Remove output data path. */
}rtk_bt_le_iso_data_path_direction_flag_t;

typedef struct
{
    uint16_t conn_handle;                               /**< Connection handle of the ISO. */
    rtk_bt_le_iso_data_path_direction_t data_path_direction;  /**< Specify the direction for which the data path is being configured. @ref rtk_bt_le_iso_data_path_direction_t. 
                                                                @ref RTK_BLE_ISO_ISOCH_ROLE_BROADCASTER uses @ref RTK_BLE_ISO_DATA_PATH_ADD_INPUT
                                                                @ref RTK_BLE_ISO_ISOCH_ROLE_RECEIVER uses @ref RTK_BLE_ISO_DATA_PATH_ADD_OUTPUT*/
    uint8_t data_path_id;                                   /**< Specify the data transport path used. 
                                                                0x00: HCI.
                                                                0x01 to 0xFE: Logical_Channel_Number. The meaning of the logical channel is vendor-specific.*/
    uint8_t codec_id[5];                                    /*!< Specify the coding format used over the air. 
                                                                Octet 0: See Assigned Numbers for Coding Format.
                                                                Octets 1 to 2: Company ID, see Assigned Numbers for Company Identifier. Shall be ignored if octet 0 is not 0xFF.
                                                                Octets 3 to 4: Vendor-defined codec ID. Shall be ignored if octet 0 is not 0xFF. */
    uint32_t controller_delay;                              /*!< Controller delay in microseconds.
                                                                Range: 0x000000 to 0x3D0900 
                                                                Time range: 0 s to 4 s */
    uint8_t codec_config_len;                               /*!< Length of codec configuration */
    uint8_t *p_codec_config;                                  /*!< Specify codec-specific configuration information for the specified direction*/
} rtk_bt_le_iso_setup_path_param_t;

typedef struct
{
    uint16_t conn_handle;                               /**< Connection handle of ISO. */
    rtk_bt_le_iso_data_path_direction_flag_t data_path_direction;  /**< Specify which directions are to have the data path removed. @ref rtk_bt_le_iso_data_path_direction_flag_t. 
                                                                @ref RTK_BLE_ISO_ISOCH_ROLE_BROADCASTER uses @ref RTK_BLE_ISO_DATA_PATH_INPUT_FLAG
                                                                @ref RTK_BLE_ISO_ISOCH_ROLE_RECEIVER uses @ref RTK_BLE_ISO_DATA_PATH_OUTPUT_FLAG*/
} rtk_bt_le_iso_remove_path_param_t;

/**
 * @struct    rtk_bt_le_iso_data_send_info_t
 * @brief     Bluetooth LE iso mgr data send info definition .
 */
typedef struct
{
    uint16_t conn_handle;           /*!< connection handle of the CIS or BIS*/
    bool ts_flag;                   /*!< indicates whether contains time_stamp, true: contain time_stamp; false: not contain time_stamp */
    uint32_t time_stamp;            /*!< a time in microseconds. time_stamp is used when @ref ts_flag is True */
    uint16_t pkt_seq_num;           /*!< sequence number of the SDU */
    uint32_t sdu_interval;          /*!< interval of the SDU */
    uint16_t data_len;              /*!< length of the SDU to be sent */
    uint8_t *p_data;                /*!< point to data to be sent */
} rtk_bt_le_iso_data_send_info_t;

/**
 * @typedef   rtk_bt_le_iso_isoch_data_pkt_status_t
 * @brief     Packet Status Flag in Bluetooth LE HCI ISO Datat packet sent by the Controller
*/
typedef enum
{
    RTK_BLE_ISO_ISOCH_DATA_PKT_STATUS_VALID_DATA = 0,             /**< Valid data. The complete SDU was received correctly. */
    RTK_BLE_ISO_ISOCH_DATA_PKT_STATUS_POSSIBLE_ERROR_DATA = 1,    /**< Possibly invalid data. The contents of the SDU may
                                                                        contain errors or part of the SDU may be missing.
                                                                        This is reported as "data with possible errors". */
    RTK_BLE_ISO_ISOCH_DATA_PKT_STATUS_LOST_DATA = 2,              /**< Part(s) of the SDU were not received correctly.
                                                                        This is reported as "lost data". */
} rtk_bt_le_iso_isoch_data_pkt_status_t;

/**
 * @struct    rtk_bt_le_iso_direct_iso_data_ind_t
 * @brief     Indication of ISO Datat packet with cb_type @ref RTK_BT_LE_ISO_EVT_DATA_RECEIVE_IND .
 */
typedef struct
{
    uint16_t conn_handle;                                       /**< Connection handle of the CIS or BIS. */
    rtk_bt_le_iso_isoch_data_pkt_status_t pkt_status_flag;    /**< @ref rtk_bt_le_iso_isoch_data_pkt_status_t. */
    uint16_t  offset;                                           /**< Offset from start of the SDU to @ref p_buf.
                                                                    e.g. p_buf + offset indicates the start of the SDU. */
    uint16_t  iso_sdu_len;                                      /**< Length of the SDU. */
    uint16_t  pkt_seq_num;                                      /**< The sequence number of the SDU. */
    bool      ts_flag;                                          /**< Indicates whether contains time_stamp.
                                                                    True: contain time_stamp.
                                                                    False: not contain time_stamp. */
    uint32_t  time_stamp;                                       /**< A time in microseconds. time_stamp is valid when @ref ts_flag is True. */
    uint16_t  buf_len;                                            /**< indicate buffer length. */
    uint8_t   *p_buf;                                           /**< Point the buffer that needs to release. */    
} rtk_bt_le_iso_direct_iso_data_ind_t;

/**
 * @struct    rtk_bt_le_iso_data_send_done_t
 * @brief     Bluetooth BLE ISO management ISO data send done.
 */
typedef struct {
    uint16_t cause;                             /*!< process result */
    uint16_t conn_handle;                       /*!< connection handle of the CIS or BIS*/
    uint16_t data_len;                          /*!< data length sent */
    uint8_t *p_data;                            /*!< point to data sent */
} rtk_bt_le_iso_data_send_done_t;

/**
  * @typedef   rtk_bt_le_iso_direct_msg_cb_t
  * @brief     BLE ISO management direct message callback.
  * @param[in] cb_type    Callback msy type @ref BT_DIRECT_MSG_CALLBACK_TYPE.
  * @param[in] p_cb_data  Point to callback data @ref T_BT_DIRECT_CB_DATA.
  * @retval void
  */
typedef void(*rtk_bt_le_iso_direct_msg_cb_t)(uint8_t cb_type, void *p_cb_data);

#if (defined RTK_BLE_ISO_CIS_SUPPORT) && (RTK_BLE_ISO_CIS_SUPPORT)
#define RTK_BLE_CIG_MAX_NUM 4
#define RTK_BLE_CIS_MAX_NUM 4

/**
 * @struct    rtk_bt_le_iso_cig_cis_info_t
 * @brief     Bluetooth LE ISO management cis info.
 */
typedef struct {
    uint8_t cig_id;         /**< Identifier of a CIG.*/
    uint8_t cis_id;         /**< Identifier of a CIS.*/
} rtk_bt_le_iso_cig_cis_info_t;

/**
 * @struct    rtk_bt_le_iso_cis_connetion_state_t
 * @brief     Bluetooth BLE ISO management CIS connection state.
 */
typedef enum
{
    RTK_BLE_ISO_ISOCH_STATE_IDLE,               /**< CIS idle. */
    RTK_BLE_ISO_ISOCH_STATE_CONNECTING,         /**< CIS connecting. */
    RTK_BLE_ISO_ISOCH_STATE_CONN,               /**< CIS connected. */
    RTK_BLE_ISO_ISOCH_STATE_DISCONNECTING,      /**< CIS disconnecting. */
    RTK_BLE_ISO_ISOCH_STATE_CONNECTING_CANCEL,  /**< Cancel CIS connecting. */
} rtk_bt_le_iso_cis_connetion_state_t;

/**
 * @struct    rtk_bt_le_iso_cis_role_t
 * @brief     Bluetooth BLE ISO management CIS role.
 */
typedef enum
{
    RTK_BLE_ISO_ISOCH_ROLE_INITIATOR,            /**< CIS - Central role. */
    RTK_BLE_ISO_ISOCH_ROLE_ACCEPTOR,             /**< CIS - Peripheral role. */
} rtk_bt_le_iso_cis_role_t;

/**
 * @struct    rtk_bt_le_iso_cig_init_param_t
 * @brief     Bluetooth LE ISO management init definition for cig.
 */
typedef struct
{
    uint8_t cig_num; /*!< CIG number*/
    uint8_t cis_num; /*!< CIS number*/
} rtk_bt_le_iso_cig_init_param_t;

/**
 * @struct    rtk_bt_le_iso_cig_get_conn_id_param_t
 * @brief     Bluetooth LE ISO management get conn id.
 */
typedef struct {
    uint16_t cis_conn_handle;       /**< Connection handle of the CIS */
    uint8_t *p_conn_id;             /**< Connection id */
} rtk_bt_le_iso_cig_get_conn_id_param_t;

/**
 * @struct    rtk_bt_le_iso_cig_get_cis_info_param_t
 * @brief     Bluetooth LE ISO management get cis info.
 */
typedef struct {
    uint16_t cis_conn_handle;           /**< Connection handle of the CIS */
    rtk_bt_le_iso_cig_cis_info_t *p_cis_info;
} rtk_bt_le_iso_cig_get_cis_info_param_t;

/**
 * @struct    rtk_bt_le_iso_cis_channel_info_t
 * @brief     Bluetooth LE ISO management CIS connection handle.
 */
typedef struct
{
    rtk_bt_le_iso_cis_connetion_state_t state; /**< CIS Connection state. @ref rtk_bt_le_iso_cis_connetion_state_t. */
    rtk_bt_le_iso_cis_role_t  role;           /**< CIS role. @ref rtk_bt_le_iso_cis_role_t. */
    uint8_t  data_path_flags;               /**< Specify which directions have been configured.
                                                @ref rtk_bt_le_iso_data_path_direction_flag_t. */
    uint8_t  data_path_adding_path;         /**< @ref rtk_bt_le_iso_data_path_direction_t. */
    bool     data_path_adding;              /**< Indicate whether data path is setup. */
    uint8_t  data_path_removing_path;       /**< Specify which directions are to have the data path
                                                removed. @ref rtk_bt_le_iso_data_path_direction_flag_t. */
    bool     data_path_removing;           /**< Indicate whether data path is removing. */
    uint32_t cig_sync_delay;               /**< The maximum time, in microseconds, for transmission of PDUs of all CISes in a CIG event.
                                                Range: 0x0000EA to 0x7FFFFF */
    uint32_t cis_sync_delay;               /**< The maximum time, in microseconds, for transmission of PDUs of the specified CIS in a CIG event.
                                                Range: 0x0000EA to 0x7FFFFF */
    uint32_t transport_latency_m_to_s;     /**< The actual transport latency, in microseconds, from Central to Peripheral.
                                                Range: 0x0000EA to 0x7FFFFF */
    uint32_t transport_latency_s_to_m;     /**< The actual transport latency, in microseconds, from Peripheral to Central.
                                                Range: 0x0000EA to 0x7FFFFF */
    uint8_t phy_m_to_s;                    /**< Indicate the PHY selected for packets from the Central to Peripheral*/
                                           /**< 0x01:LE PHY 1M used. */
                                           /**< 0x02:LE PHY 2M used. */
                                           /**< 0x03:LE Coded PHY used. */
    uint8_t phy_s_to_m;                    /**< Indicate the PHY selected for packets from the Peripheral to Central*/
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
    bool acceptor_config_sdu_flag;         /**< Only used by @ref RTK_BLE_ISO_ISOCH_ROLE_ACCEPTOR.
                                                True:  sdu parameters listed below are configured by Application.
                                                       check sdu length when @ref RTK_BLE_ISO_ISOCH_ROLE_ACCEPTOR calls @ref rtk_bt_le_iso_data_send.
                                                False: Default value.
                                                       sdu parameters listed below are not configrued by Application.
                                                       Not check sdu length when @ref RTK_BLE_ISO_ISOCH_ROLE_ACCEPTOR calls @ref rtk_bt_le_iso_data_send. */
    uint32_t sdu_interval_m_to_s;          /**< Valid for @ref RTK_BLE_ISO_ISOCH_ROLE_INITIATOR.
                                                Valid for @ref RTK_BLE_ISO_ISOCH_ROLE_ACCEPTOR if @ref acceptor_config_sdu_flag is True.
                                                The interval, in microseconds, between the start of consecutive SDUs from
                                                the Central's Host for all the CISes in the CIG.
                                                Range: 0x0000FF to 0x0FFFFF
                                               */
    uint32_t sdu_interval_s_to_m;          /**< Valid for @ref RTK_BLE_ISO_ISOCH_ROLE_INITIATOR.
                                                Valid for @ref RTK_BLE_ISO_ISOCH_ROLE_ACCEPTOR if @ref acceptor_config_sdu_flag is True.
                                                The interval, in microseconds, between the start of consecutive SDUs from
                                                the Peripheral's Host for all the CISes in the CIG.
                                                Range: 0x0000FF to 0x0FFFFF
                                                */
    uint16_t max_sdu_m_to_s;               /**< Valid for @ref RTK_BLE_ISO_ISOCH_ROLE_INITIATOR.
                                                Valid for @ref RTK_BLE_ISO_ISOCH_ROLE_ACCEPTOR if @ref acceptor_config_sdu_flag is True.
                                                Maximum size, in octets, of the payload from the Central's Host. */
    uint16_t max_sdu_s_to_m;               /**< Valid for @ref RTK_BLE_ISO_ISOCH_ROLE_INITIATOR.
                                                Valid for @ref RTK_BLE_ISO_ISOCH_ROLE_ACCEPTOR if @ref acceptor_config_sdu_flag is True.
                                                Maximum size, in octets, of the payload from the Peripheral's Host. */
} rtk_bt_le_iso_cis_channel_info_t;

/**
 * @struct    rtk_bt_le_iso_cig_get_isoch_info_param_t
 * @brief     Bluetooth LE ISO management get isoch info.
 */
typedef struct {
    uint16_t cis_conn_handle;                               /**< Connection handle of the CIS */
    rtk_bt_le_iso_cis_channel_info_t *p_isoch_info;
} rtk_bt_le_iso_cig_get_isoch_info_param_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_set_cis_acl_link_param_t
 * @brief     Bluetooth LE ISO management set CIS acl link parameter.
 */
typedef struct {
    uint8_t cis_id;                 /*!< Identifier of a CIS.*/
    uint8_t conn_id;                /*!< connection id */
} rtk_bt_le_iso_cig_initiator_set_cis_acl_link_param_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_set_cig_param_t
 * @brief     Bluetooth LE ISO management set cig param for initiator .
 */
typedef struct {
    uint8_t  cig_id;             /**< Identifier of a CIG.*/
    uint32_t sdu_interval_m_s;  /**< The interval, in microseconds, of periodic SDUs from the Central's Host for all the CISes in the CIG.
                                     This parameter shall be ignored for all CISes that are unidirectional from Peripheral to Central.
                                     Range: 0x0000FF to 0x0FFFFF */
    uint32_t sdu_interval_s_m;  /**< The interval, in microseconds, of periodic SDUs from the Peripheral's Host for all the CISes in the CIG.
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
    uint16_t latency_m_s;       /**< Maximum transport latency, in milliseconds, from the Central's Controller to the Peripheral's Controller.
                                     Range: 0x0005 to 0x0FA0 */
    uint16_t latency_s_m;       /**< Maximum transport latency, in milliseconds, from the Peripheral's Controller to the Central's Controller.
                                     Range: 0x0005 to 0x0FA0 */
} rtk_bt_le_iso_cig_initiator_set_cig_param_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_set_cis_param_t
 * @brief     Bluetooth LE ISO management set cis param for initiator .
 */
typedef struct {
    uint8_t cis_id;        /**< Identifier of a CIS.*/
    uint16_t max_sdu_m_s;  /**< Maximum size, in octets, of the payload from the Central's Host.
                                If the CIS is unidirectional from Peripheral to Central, this parameter shall be set to 0. */
    uint16_t max_sdu_s_m;  /**< Maximum size, in octets, of the payload from the Peripheral's Host.
                                If the CIS is unidirectional from Central to Peripheral, this parameter shall be set to 0. */
    uint8_t  phy_m_s;      /**< Identify which PHY to use for transmission from the Central to the Peripheral.
                                0x01: LE 1M, 0x02: LE 2M, 0x03: LE Coded */
    uint8_t  phy_s_m;      /**< Identify which PHY to use for transmission from the Peripheral to the Central.
                                0x01: LE 1M, 0x02: LE 2M, 0x03: LE Coded  */
    uint8_t  rtn_m_s;      /**< Number of times every CIS Data PDU should be retransmitted from the Central to the Peripheral. */
    uint8_t  rtn_s_m;      /**< Number of times every CIS Data PDU should be retransmitted from the Peripheral to the Central. */
} rtk_bt_le_iso_cig_initiator_set_cis_param_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_create_cis_by_cis_conn_handle_param_t
 * @brief     Bluetooth LE ISO management create cis by cis_conn_handle for initiator.
 */
typedef struct {
    uint8_t cig_id;                 /**< Identifier of a CIG.*/
    uint8_t cis_count;              /**< CIS count.*/
    uint16_t *p_cis_conn_handle;    /**< Connection handle of the CIS */
} rtk_bt_le_iso_cig_initiator_create_cis_by_cis_conn_handle_param_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_get_cis_handle_param_t
 * @brief     Bluetooth LE ISO management get cis handle for initiator.
 */
typedef struct {
    uint8_t cis_id;                 /**< Identifier of a CIS.*/
    uint16_t *p_cis_handle;         /**< Connection handle of the CIS */
} rtk_bt_le_iso_cig_initiator_get_cis_handle_param_t;

/**
 * @struct    rtk_bt_le_iso_cig_acceptor_reject_cis_param_t
 * @brief     Bluetooth LE ISO management reject cis for acceptor.
 */
typedef struct {
    uint16_t cis_conn_handle;   /**< Connection handle of the CIS */
    uint8_t reason;             /**<  Reason the CIS request was rejected. @ref BT_HCI_ERROR (except @ref HCI_SUCCESS) */
} rtk_bt_le_iso_cig_acceptor_reject_cis_param_t;

/**
 * @struct    rtk_bt_le_iso_cig_acceptor_config_sdu_param_t
 * @brief     Bluetooth LE ISO management config sdu parameters for acceptor.
 */
typedef struct {
    uint16_t cis_conn_handle;               /*!< Connection handle of the CIS */
    bool acceptor_config_sdu_flag;          /*!< Whether to check sdu length when acceptor calls @ref rtk_bt_le_iso_data_send.*/
    uint32_t sdu_interval_m_s;              /*!< The interval, in microseconds, between the start of consecutive SDUs from the Central's Host for all the CISes in the CIG.
                                                 Range: 0x0000FF to 0x0FFFFF*/
    uint32_t sdu_interval_s_m;              /*!< The interval, in microseconds, between the start of consecutive SDUs from the Peripheral's Host for all the CISes in the CIG.
                                                Range: 0x0000FF to 0x0FFFFF.*/
    uint16_t max_sdu_m_s;                   /*!< Maximum size, in octets, of the payload from the Central's Host.*/
    uint16_t max_sdu_s_m;                   /*!< Maximum size, in octets, of the payload from the Peripheral's Host*/
} rtk_bt_le_iso_cig_acceptor_config_sdu_param_t;

/**
 * @typedef   rtk_bt_le_iso_cig_acceptor_config_cis_req_action_t
 * @brief     Bluetooth LE ISO management config CIS request action for acceptor.
 */
typedef enum {
    RTK_BLE_ISO_ACCEPTOR_CIS_REQ_ACTION_ACCEPT  = 0x00,    /**< accept CIS request when acceptor received CIS_REQUEST_IND from initiator. */
    RTK_BLE_ISO_ACCEPTOR_CIS_REQ_ACTION_REJECT,            /**< reject CIS request when acceptor received CIS_REQUEST_IND from initiator. */
    RTK_BLE_ISO_ACCEPTOR_CIS_REQ_ACTION_PENDING,           /**< pending CIS request when acceptor received CIS_REQUEST_IND from initiator, then APP need call
                                                                    rtk_bt_le_iso_cig_acceptor_accept_cis or rtk_bt_le_iso_cig_acceptor_reject_cis according to the usage scenario. */
}rtk_bt_le_iso_cig_acceptor_config_cis_req_action_t;

/**
 * @struct    rtk_bt_le_iso_cig_setup_data_path_done_t
 * @brief     Bluetooth BLE ISO management CIG setup data path done event definition.
 */
typedef struct {
    uint16_t cause;                     /**< process result */
    uint16_t cis_conn_handle;           /**< Connection handle of the CIS */
    uint8_t conn_id;                    /**< Connection Id. If cause is not 0, conn_id is meaningless.*/    
    uint8_t cig_id;                     /**< Identifier of a CIG. If cause is not 0, cig_id is meaningless.*/              
    uint8_t cis_id;                     /**< Identifier of a CIS. If cause is not 0, cis_id is meaningless.*/
    rtk_bt_le_iso_data_path_direction_t data_path_direction;/**< Remove path direction. If cause is not 0, data_path_direction is meaningless.*/
} rtk_bt_le_iso_cig_setup_data_path_done_t;

/**
 * @struct    rtk_bt_le_iso_cig_remove_data_path_done_t
 * @brief     Bluetooth BLE ISO management CIG remove data path done event definition.
 */
typedef struct {
    uint16_t cause;                     /**< process result */
    uint16_t cis_conn_handle;           /**< Connection handle of the CIS */
    uint8_t conn_id;                    /**< Connection Id. If cause is not 0, conn_id is meaningless.*/    
    uint8_t cig_id;                     /**< Identifier of a CIG. If cause is not 0, cig_id is meaningless.*/              
    uint8_t cis_id;                     /**< Identifier of a CIS. If cause is not 0, cis_id is meaningless.*/
    rtk_bt_le_iso_data_path_direction_flag_t data_path_direction /**< Remove path direction. If cause is not 0, data_path_direction is meaningless.*/;
} rtk_bt_le_iso_cig_remove_data_path_done_t;

/**
 * @struct    rtk_bt_le_iso_cig_disconnect_rsp_t
 * @brief     Bluetooth BLE ISO management CIG disconnect response info.
 */
typedef struct {
    uint16_t cause;                     /**< process result */
    uint16_t cis_conn_handle;           /**< Connection handle of the CIS */
    uint8_t conn_id;                    /**< Connection Id. If cause is not 0, conn_id is meaningless.*/   
    uint8_t cig_id;                     /**< Identifier of a CIG. If cause is not 0, cig_id is meaningless.*/ 
    uint8_t cis_id;                     /**< Identifier of a CIS. If cause is not 0, cis_id is meaningless.*/
} rtk_bt_le_iso_cig_disconnect_rsp_t;

/**
 * @struct    rtk_bt_le_iso_cig_disconnect_info_t
 * @brief     Bluetooth BLE ISO management CIG disconnect indication info.
 */
typedef rtk_bt_le_iso_cig_disconnect_rsp_t rtk_bt_le_iso_cig_disconnect_info_t;

/**
 * @struct    rtk_bt_le_iso_cig_read_iso_tx_sync_info_t
 * @brief     Bluetooth BLE ISO management CIG read ISO tx sync info.
 */
typedef struct {
    uint8_t conn_id;                  /**< Connection Id. If cause is not 0, conn_id is meaningless.*/  
    uint8_t cig_id;                   /**< Identifier of a CIG. If cause is not 0, cig_id is meaningless.*/ 
    uint8_t cis_id;                   /**< Identifier of a CIS. If cause is not 0, cis_id is meaningless.*/
    uint16_t packet_sequence_number;  /**< The packet sequence number of an SDU. If cause is not 0, packet_sequence_number is meaningless.*/
    uint32_t time_stamp;              /**< The CIG reference point of a transmitted SDU derived
                                           using the Controller's free running reference clock (in microseconds). If cause is not 0, time_stamp is meaningless.*/
    uint32_t time_offset;             /**< The time offset, in microseconds, that is associated with a transmitted SDU. If cause is not 0, time_offset is meaningless.*/
} rtk_bt_le_iso_cig_read_iso_tx_sync_info_t;

typedef struct {
    uint16_t cis_conn_handle;                                   /**< Connection handle of the CIS */
    rtk_bt_le_iso_cig_read_iso_tx_sync_info_t * p_tx_sync_info;   /**< Read tx sync info result */
} rtk_bt_le_iso_cig_read_iso_tx_sync_t;

/**
 * @struct    rtk_bt_le_iso_cig_read_link_quality_info_t
 * @brief     Bluetooth BLE ISO management CIG read link quality info.
 */
typedef struct {
    uint8_t conn_id;                   /**< Connection Id.  If cause is not 0, conn_id is meaningless.*/  
    uint8_t cig_id;                    /**< Identifier of a CIG. If cause is not 0, cig_id is meaningless.*/ 
    uint8_t cis_id;                    /**< Identifier of a CIS. If cause is not 0, cis_id is meaningless.*/
    uint32_t tx_unacked_packets;       /**< Value of the Tx_UnACKed_Packets counter. If cause is not 0, tx_unacked_packets is meaningless.
                                            When Incremented: The Link Layer does not receive an acknowledgment for a CIS
                                                              Data PDU that it transmitted at least once by its flush point. */
    uint32_t tx_flushed_packets;       /**< Value of the Tx_Flushed_Packets counter. If cause is not 0, tx_flushed_packets is meaningless.
                                            When Incremented: The Link Layer does not transmit a specific payload by its flush point. */
    uint32_t tx_last_subevent_packets; /**< Value of the Tx_Last_Subevent_Packets counter for @ref RTK_BLE_ISO_ISOCH_ROLE_ACCEPTOR.If cause is not 0, tx_last_subevent_packets is meaningless.
                                            When Incremented: The Link Layer transmits a CIS Data PDU in the last subevent of a CIS event. */
    uint32_t retransmitted_packets;    /**< Value of the Retransmitted_Packets counter. If cause is not 0, retransmitted_packets is meaningless.
                                            When Incremented: The Link Layer retransmits a CIS Data PDU. */
    uint32_t crc_error_packets;        /**< Value of the CRC_Error_Packets counter. If cause is not 0, crc_error_packets is meaningless.
                                            When Incremented: The Link Layer receives a packet with a CRC error. */
    uint32_t rx_unreceived_packets;    /**< Value of the Rx_Unreceived_Packets counter. If cause is not 0, rx_unreceived_packets is meaningless.
                                            When Incremented: The Link Layer does not receive a specific payload by its flush point (on
                                                              a CIS) or the end of the event it is associated with (on a BIS). */
    uint32_t duplicate_packets;        /**< Value of the Duplicate_Packets counter. If cause is not 0, duplicate_packets is meaningless.
                                            When Incremented: The Link Layer receives a retransmission of a CIS Data PDU. */
} rtk_bt_le_iso_cig_read_link_quality_info_t;

typedef struct {
    uint16_t      cis_conn_handle;                                  /**< Connection handle of the CIS */
    rtk_bt_le_iso_cig_read_link_quality_info_t *p_link_quality_info;  /**< Read link quality info result */
} rtk_bt_le_iso_cig_read_link_quality_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_add_cis_done_t
 * @brief     Bluetooth BLE ISO management CIG add CIS done for initiator.
 */
typedef struct {
    uint16_t      cause;                /**< process result */
    uint8_t       cig_id;               /**< Identifier of a CIG.*/
    uint8_t       cis_id;               /**< Identifier of a CIS.*/
} rtk_bt_le_iso_cig_initiator_add_cis_done_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_set_cig_param_done_t
 * @brief     Bluetooth BLE ISO management CIG set CIG param done for initiator.
 */
typedef struct {
    uint16_t      cause;                /**< process result */
    uint8_t       cig_id;               /**< Identifier of a CIG.*/
} rtk_bt_le_iso_cig_initiator_set_cig_param_done_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_set_cis_param_done_t
 * @brief     Bluetooth BLE ISO management CIG set CIS param done for initiator.
 */
typedef struct {
    uint16_t      cause;                /**< process result */
    uint8_t       cis_id;               /**< Identifier of a CIS.*/
} rtk_bt_le_iso_cig_initiator_set_cis_param_done_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_start_setting_cis_info_t
 * @brief     Bluetooth BLE ISO management CIG start setting CIS info for initiator.
 */
typedef struct
{
    uint8_t       cis_id;               /**< Identifier of a CIS.*/
    uint16_t      cis_conn_handle;      /**< Connection handle of the CIS */
} rtk_bt_le_iso_cig_initiator_start_setting_cis_info_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_start_setting_done_t
 * @brief     Bluetooth BLE ISO management CIG start setting done for initiator.
 */
typedef struct {
    uint16_t cause;                     /**< process result */
    uint8_t  cig_id;                    /**< Identifier of a CIG.*/
    uint8_t  cis_count;                 /**< CIS count. If cause is not 0, cis_count is meaningless. */
    rtk_bt_le_iso_cig_initiator_start_setting_cis_info_t  cis_info[8]; /**< CIS info. If cause is not 0, cis_info is meaningless. */
} rtk_bt_le_iso_cig_initiator_start_setting_done_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_set_cis_acl_link_done_t
 * @brief     Bluetooth BLE ISO management CIG set CIS ACL link for initiator.
 */
typedef struct {
    uint16_t cause;                   /**< process result */
    uint8_t  cis_id;                  /**< Identifier of a CIS.*/
    uint8_t  conn_id;                 /**< Connection Id.*/ 
} rtk_bt_le_iso_cig_initiator_set_cis_acl_link_done_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_create_cis_info_t
 * @brief     Bluetooth BLE ISO management create cis info for initiator.
 */
typedef struct
{
    uint8_t       cig_id;                                   /**< Identifier of a CIG.*/
    uint8_t       cis_id;                                   /**< Identifier of a CIS.*/
    rtk_bt_le_iso_cis_connetion_state_t state;            /**< CIS connection state. @ref rtk_bt_le_iso_cis_connetion_state_t. */
    uint16_t      cis_conn_handle;                          /**< Connection handle of the CIS */
} rtk_bt_le_iso_cig_initiator_create_cis_info_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_create_cis_done_t
 * @brief     Bluetooth BLE ISO management CIG create CIS for initiator.
 */
typedef struct {
    uint16_t          cause;                    /**< process result */
    uint8_t           cis_count;                /**< CIS count.  If cause is not 0, cis_count is meaningless*/
    rtk_bt_le_iso_cig_initiator_create_cis_info_t cis_info[8]; /**< CIS info. If cause is not 0, cis_info is meaningless. */
} rtk_bt_le_iso_cig_initiator_create_cis_done_t;

/**
 * @struct    rtk_bt_le_iso_cig_cis_established_info_t
 * @brief     Bluetooth BLE ISO management CIG CIS established indication definition.
 */
typedef struct {
    uint16_t cause;                   /**< process result */
    uint8_t  conn_id;                 /**< Connection Id.*/ 
    uint8_t cig_id;                   /**< Identifier of a CIG.*/
    uint8_t cis_id;                   /**< Identifier of a CIS.*/
    uint16_t cis_conn_handle;         /**< Connection handle of the CIS. */
    uint32_t cig_sync_delay;          /**< The maximum time, in microseconds, for transmission of PDUs of all CISes in a CIG event. */
    uint32_t cis_sync_delay;          /**< The maximum time, in microseconds, for transmission of PDUs of the specified CIS in a CIG event. */
    uint32_t transport_latency_m_s;   /**< The actual transport latency, in microseconds, from Central to Peripheral. */
    uint32_t transport_latency_s_m;   /**< The actual transport latency, in microseconds, from Peripheral to Central. */
    uint8_t phy_m_s;                  /**< The transmitter PHY of packets from the Central*/
                                      /**< 0x01:LE PHY 1M used. */
                                      /**< 0x02:LE PHY 2M used. */
                                      /**< 0x03:LE Coded PHY used. */
    uint8_t phy_s_m;                  /**< The transmitter PHY of packets from the Peripheral*/
                                      /**< 0x01:LE PHY 1M used. */
                                      /**< 0x02:LE PHY 2M used. */
                                      /**< 0x03:LE Coded PHY used. */
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
} rtk_bt_le_iso_cig_cis_established_info_t;

/**
 * @struct    rtk_bt_le_iso_cig_initiator_remove_cig_done_t
 * @brief     Bluetooth BLE ISO management CIG remove CIG for initiator.
 */
typedef struct {
    uint16_t cause;                 /**< process result */
    uint8_t cig_id;                 /**< Identifier of a CIG.*/
} rtk_bt_le_iso_cig_initiator_remove_cig_done_t;

/**
 * @struct    rtk_bt_le_iso_cig_acceptor_request_cis_ind_t
 * @brief     Bluetooth BLE ISO management CIG CIS request indication for acceptor.
 */
typedef struct {
    uint16_t cis_conn_handle;       /**< Connection handle of the CIS */
    uint8_t conn_id;                /**< Connection Id.*/ 
    uint8_t cig_id;                 /**< Identifier of a CIG.*/
    uint8_t cis_id;                 /**< Identifier of a CIS.*/
    rtk_bt_le_iso_cig_acceptor_config_cis_req_action_t cis_req_action;         /**< Action of CIS request Indicate, default is RTK_BLE_ISO_ACCEPTOR_CIS_REQ_ACTION_ACCEPT.
                                                                                        can be configed by @ref rtk_bt_le_iso_cig_acceptor_config_cis_req_action. */
} rtk_bt_le_iso_cig_acceptor_request_cis_ind_t;

/**
 * @struct    rtk_bt_le_iso_cig_acceptor_accept_cis_done_t
 * @brief     Bluetooth BLE ISO management CIG accept CIS request for acceptor.
 */
typedef struct {
    uint16_t cause;                     /*!< process result */
    uint16_t cis_conn_handle;           /**< Connection handle of the CIS */
    uint8_t conn_id;                    /**< Connection Id. If cause is not 0, conn_id is meaningless.*/ 
    uint8_t cig_id;                     /**< Identifier of a CIG. If cause is not 0, cig_id is meaningless.*/
    uint8_t cis_id;                     /**< Identifier of a CIS. If cause is not 0, cis_id is meaningless.*/
} rtk_bt_le_iso_cig_acceptor_accept_cis_done_t;

/**
 * @struct    rtk_bt_le_iso_cig_acceptor_reject_cis_done_t
 * @brief     Bluetooth BLE ISO management CIG reject CIS request for acceptor.
 */
typedef rtk_bt_le_iso_cig_acceptor_accept_cis_done_t rtk_bt_le_iso_cig_acceptor_reject_cis_done_t  ;
#endif //#if (defined RTK_BLE_ISO_CIS_SUPPORT) && (RTK_BLE_ISO_CIS_SUPPORT)

#if (defined RTK_BLE_ISO_BIS_SUPPORT) && (RTK_BLE_ISO_BIS_SUPPORT)
#define RTK_BLE_BIG_BROADCASTER_MAX_NUM  4 //must less than 4
#define RTK_BLE_BIS_BROADCASTER_MAX_NUM  4 //must less than 4
#define RTK_BLE_BIG_RECEIVER_MAX_NUM  4 //must less than 4
#define RTK_BLE_BIS_RECEIVER_MAX_NUM  4 //must less than 4

/**
 * @struct    rtk_bt_le_iso_bis_role_t
 * @brief     Bluetooth BLE ISO management BIS role.
 */
typedef enum
{
    RTK_BLE_ISO_ISOCH_ROLE_BROADCASTER = 1,            /**< BIS - Broadcaster role. */
    RTK_BLE_ISO_ISOCH_ROLE_RECEIVER,                   /**< BIS - Receiver role. */
} rtk_bt_le_iso_bis_role_t;

/**
 * @struct    rtk_bt_le_iso_big_init_param_t
 * @brief     Bluetooth LE ISO management init definition for big.
 */
typedef struct
{
    uint8_t big_num; /*!< BIG number*/
    uint8_t bis_num; /*!< BIS number*/
} rtk_bt_le_iso_big_init_param_t;

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
} rtk_bt_le_iso_big_broadcaster_param_t;

typedef struct
{
    uint8_t adv_handle;                             /**< Identify the periodic advertising train. */
    rtk_bt_le_iso_big_broadcaster_param_t *p_big_param;  /**< Identify the periodic advertising train. */
    uint8_t *big_handle;                            /**< Identify the BIG. */
} rtk_bt_le_iso_big_broadcaster_create_param_t;

typedef struct
{
    uint8_t bis_idx;
    uint16_t bis_conn_handle;         /**< Connection handle of a BIS. */
} rtk_bt_le_iso_big_bis_conn_handle_info_t;

typedef struct
{
    uint16_t cause;
    uint8_t  big_handle;            /**< Used to identify the BIG. */
    uint8_t adv_handle;             /**< Used to identify the periodic advertising train. */
    uint32_t big_sync_delay;        /**< The maximum time in microseconds for transmission of PDUs of all BISes in a BIG event. */
    uint32_t transport_latency_big; /**< The actual transport latency, in microseconds. */
    uint8_t phy;                    /**< The PHY used to create the BIG. @ref rtk_bt_le_phy_type_t. */
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
    rtk_bt_le_iso_big_bis_conn_handle_info_t bis_conn_handle_info[RTK_BLE_BIS_BROADCASTER_MAX_NUM];/**< Connection handle of BIS. */
} rtk_bt_le_iso_big_broadcaster_create_cmpl_info_t;

typedef struct
{    
    uint8_t big_handle;                          /**< Identify the BIG. */
    uint8_t adv_handle;                          /**< Used to identify the periodic advertising train. */
    uint8_t cause;                               
} rtk_bt_le_iso_big_broadcaster_terminate_done_t;

typedef enum
{
    RTK_BLE_ISO_BIG_BOARDCASTER_SYNC_STATE_IDLE = 0x00,                                        /**< Idle, no Isochronous Broadcasting. */
    RTK_BLE_ISO_BIG_BOARDCASTER_SYNC_STATE_CREATING_EXT_ADV_STATE_PA_ADV_STATE_IDLE = 0x01,    /**< Start Isochronous Broadcasting when extended advertising and periodic advertising of the
                                                                                                    specified advertising set are disabled. A temporary state, haven't received the result. */
    RTK_BLE_ISO_BIG_BOARDCASTER_SYNC_STATE_WAIT_EXT_ADV_STATE_PA_ADV_STATE_ADVERTISING = 0x02, /**< Isochronous Broadcasting is not started until the extended advertising and periodic advertising
                                                                                                    of the specified advertising set are enabled. */
    RTK_BLE_ISO_BIG_BOARDCASTER_SYNC_STATE_CREATING_PA_ADV_STATE_IDLE = 0x03,                  /**< Start Isochronous Broadcasting when the periodic advertising of the specified advertising set is disabled.
                                                                                                    A temporary state, haven't received the result. */
    RTK_BLE_ISO_BIG_BOARDCASTER_SYNC_STATE_WAIT_PA_ADV_STATE_ADVERTISING = 0x04,               /**< Isochronous Broadcasting is not started until the periodic advertising of the specified advertising
                                                                                                    set is enabled. */
    RTK_BLE_ISO_BIG_BOARDCASTER_SYNC_STATE_CREATING_EXT_ADV_STATE_IDLE = 0x05,                 /**< Start Isochronous Broadcasting when the extended advertising of the specified advertising set is disabled.
                                                                                                    A temporary state, haven't received the result. */
    RTK_BLE_ISO_BIG_BOARDCASTER_SYNC_STATE_WAIT_EXT_ADV_STATE_ADVERTISING = 0x06,              /**< Isochronous Broadcasting is not started until the extended advertising of the specified advertising
                                                                                                    set is enabled. */
    RTK_BLE_ISO_BIG_BOARDCASTER_SYNC_STATE_CREATING = 0x07,                                    /**< Start Isochronous Broadcasting when extended advertising and periodic advertising of the
                                                                                                    specified advertising set are enabled. A temporary state, haven't received the result. */
    RTK_BLE_ISO_BIG_BOARDCASTER_SYNC_STATE_BROADCASTING = 0x08,                                /**< Isochronous Broadcasting. */
    RTK_BLE_ISO_BIG_BOARDCASTER_SYNC_STATE_TERMINATING = 0x09,                                 /**< Terminate Isochronous Broadcasting. A temporary state, haven't received the result. */
} rtk_bt_le_iso_big_broadcaster_sync_state_t;

typedef struct
{    
    uint8_t big_handle;                          /**< Identify the BIG. */
    uint8_t adv_handle;                          /**< Used to identify the periodic advertising train. */
    uint8_t sync_state;                          /**< Synchronization state. @ref rtk_bt_le_iso_big_broadcaster_sync_state_t*/
    uint8_t cause;                               
} rtk_bt_le_iso_big_broadcaster_sync_state_ind_t;

typedef struct
{
    uint8_t  big_handle;                        /**< Used to identify the BIG. */
    uint8_t  adv_handle;                        /**< Used to identify the periodic advertising train. */
    uint16_t packet_sequence_number;            /**< The packet sequence number of an SDU. */
    uint32_t time_stamp;                        /**< The BIG anchor point of a transmitted SDU derived
                                                    using the Controller's free running reference clock (in microseconds). */
    uint32_t time_offset;                       /**< The time offset, in microseconds, that is associated with a transmitted SDU. */
} rtk_bt_le_iso_big_broadcaster_read_tx_sync_info_t;

typedef struct
{
    uint16_t bis_conn_handle;                                           /**< Connection handle of a BIS. */
    rtk_bt_le_iso_big_broadcaster_read_tx_sync_info_t * p_tx_sync_info;     /**< Read tx sync info result. */
} rtk_bt_le_iso_big_broadcaster_read_tx_sync_t;

typedef struct
{
    uint8_t encryption;                    /**< Identify the encryption mode of the BIG.
                                                0x00: Unencrypted
                                                0x01: Encrypted */
    uint8_t broadcast_code[16];            /**< Used to generate the session key to decrypt payloads of an encrypted BIS. */
    uint8_t mse;                           /**< The maximum number of subevents that a Controller should use to receive data
                                                payloads in each interval for a BIS.
                                                0x00: The Controller can schedule reception of any number of subevents up to NSE.
                                                0x01 to 0x1F: Maximum number of subevents that should be used to receive data
                                                              payloads in each BIS event      */
    uint16_t big_sync_timeout;             /**< Specify the maximum permitted time between successful receptions of BIS PDUs.
                                                Range: 0x000A to 0x4000
                                                Time = N*10 ms
                                                Time Range: 100 ms to 163.84 s */
    uint8_t  num_bis;                      /**< Total number of BISes to synchronize. */
    uint8_t  bis[RTK_BLE_BIS_RECEIVER_MAX_NUM]; /**< A list of indices corresponding to BIS(es). */
} rtk_bt_le_iso_big_receiver_create_sync_param_t;

typedef struct
{
    uint16_t sync_handle;                             /**< Identify the periodic advertising train. */  
    rtk_bt_le_iso_big_receiver_create_sync_param_t *p_sync_param;/**< GAP Synchronized Receiver BIG create sync parameter. */
    uint8_t *p_big_handle;                            /**< Used to identify the BIG. */
} rtk_bt_le_iso_big_receiver_create_sync_t;

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
    rtk_bt_le_iso_big_bis_conn_handle_info_t bis_conn_handle_info[RTK_BLE_BIS_RECEIVER_MAX_NUM]; /**< Connection handle of BIS. */
} rtk_bt_le_iso_big_receiver_sync_established_info_t;

typedef enum
{
    RTK_BLE_ISO_BIG_RECEIVER_SYNC_STATE_TERMINATED = 0x00,     /**< Terminated. */
    RTK_BLE_ISO_BIG_RECEIVER_SYNC_STATE_SYNCHRONIZING = 0x01,  /**< Start synchronizing to a BIG. A temporary state, haven't received the result.*/
    RTK_BLE_ISO_BIG_RECEIVER_SYNC_STATE_SYNCHRONIZED = 0x02,   /**< Synchronized to a BIG. */
    RTK_BLE_ISO_BIG_RECEIVER_SYNC_STATE_TERMINATING = 0x03,    /**< Terminate synchronization to the BIG. A temporary state, haven't received the result.*/
} rtk_bt_le_iso_big_receiver_sync_state_t;

typedef struct
{    
    uint8_t big_handle;              /**< Identify the BIG. */
    uint8_t sync_state;              /**< Synchronization state. @ref rtk_bt_le_iso_big_receiver_sync_state_t*/
    uint8_t sync_id;                 /**< Identify the periodic advertising train. */
    uint16_t sync_handle;            /**< Sync_Handle identifying the periodic advertising train. */
    uint8_t cause;                               
} rtk_bt_le_iso_big_receiver_sync_state_ind_t;

typedef struct
{
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
} rtk_bt_le_iso_big_receiver_read_link_quality_info_t;

typedef struct
{
        uint16_t bis_conn_handle;                                               /**< Connection handle of the BIS. */
        rtk_bt_le_iso_big_receiver_read_link_quality_info_t * p_link_quality_info;  /**< Read link quality info result. */
} rtk_bt_le_iso_big_receiver_read_link_quality_t;
#endif //#if (defined RTK_BLE_ISO_BIS_SUPPORT) && (RTK_BLE_ISO_BIS_SUPPORT)


/********************************* Functions Declaration *******************************/
/**
 * @defgroup  bt_le_iso BT LE ISO APIs
 * @brief     BT LE ISO related function APIs
 * @ingroup   BT_APIs
 * @{
 */

#if (defined RTK_BLE_ISO_CIS_SUPPORT) && (RTK_BLE_ISO_CIS_SUPPORT)
/**
 * @fn        uint16_t rtk_bt_le_iso_cig_init(rtk_bt_le_iso_cig_init_param_t *param)
 * @brief     Initialize the number of CIG and CIS.
 * @param[in] param: The param for CIG init
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_init(rtk_bt_le_iso_cig_init_param_t *param);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_get_conn_id(uint16_t cis_conn_handle, uint8_t *p_conn_id)
 * @brief     Get connection ID by CIS connection handle.
 * @param[in]   cis_conn_handle:  Connection handle of the CIS.
 * @param[out]  p_conn_id:        Connection ID.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_get_conn_id(uint16_t cis_conn_handle, uint8_t *p_conn_id);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_get_cis_info(uint16_t cis_conn_handle, rtk_bt_le_iso_cig_cis_info_t * p_info)
 * @brief     Get CIG ID and CIS ID by CIS connection handle.
 * @param[in]   cis_conn_handle:  Connection handle of the CIS.
 * @param[out]  p_info:       CIG ID and CIS ID.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_get_cis_info(uint16_t cis_conn_handle, rtk_bt_le_iso_cig_cis_info_t * p_info);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_get_isoch_info(uint16_t cis_conn_handle, rtk_bt_le_iso_cis_channel_info_t *p_info)
 * @brief     Get information about specified CIS connection handle.
 * @param[in]   cis_conn_handle  Connection handle of the CIS.
 * @param[out]  p_info           Information about specified CIS connection handle.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_get_isoch_info(uint16_t cis_conn_handle, rtk_bt_le_iso_cis_channel_info_t *p_info);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_setup_path(rtk_bt_le_iso_setup_path_param_t *param);
 * @brief     Identify and create the isochronous data path for CIS.
 * @param[in]       param:  The param of setup cis data path.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_setup_path(rtk_bt_le_iso_setup_path_param_t *param);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_remove_path(uint16_t cis_conn_handle,rtk_bt_le_iso_data_path_direction_flag_t data_path_direction)
 * @brief     Remove the input and/or output data path(s) associated with CIS.
 * @param[in]       cis_conn_handle:   Connection handle of the CIS.
 * @param[in]       data_path_direction:  The param for CIG remove path.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_remove_path(uint16_t cis_conn_handle,rtk_bt_le_iso_data_path_direction_flag_t data_path_direction);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_disconnect(uint16_t cis_conn_handle)
 * @brief     Terminate a CIS connection.
 * @param[in]   cis_conn_handle:   Connection handle of the CIS.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_disconnect(uint16_t cis_conn_handle);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_read_iso_tx_sync(uint16_t cis_conn_handle, rtk_bt_le_iso_cig_read_iso_tx_sync_info_t * p_tx_sync_info)
 * @brief     Read the TX_Time_Stamp and Time_Offset of a transmitted SDU identified by the Packet_Sequence_Number on a CIS.
 * @param[in]   cis_conn_handle:   Connection handle of the CIS.
 * @param[out]  p_tx_sync_info:    Point to tx sync info result. 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_read_iso_tx_sync(uint16_t cis_conn_handle, rtk_bt_le_iso_cig_read_iso_tx_sync_info_t * p_tx_sync_info);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_read_link_quality(uint16_t cis_conn_handle, rtk_bt_le_iso_cig_read_link_quality_info_t * p_link_quality_info)
 * @param[in]   cis_conn_handle:        Connection handle of the CIS.
 * @param[out]  p_link_quality_info:    Point to link quality info result. 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_read_link_quality(uint16_t cis_conn_handle,rtk_bt_le_iso_cig_read_link_quality_info_t * p_link_quality_info);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_initiator_add_cis(uint8_t cig_id, uint8_t cis_id)
 * @brief     Add CIS for specified CIG. 
 * @param[in] cig_id: Identifier of a CIG
 * @param[in] cis_id: Identifier of a CIS
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_initiator_add_cis(uint8_t cig_id, uint8_t cis_id);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_initiator_set_cig_param(rtk_bt_le_iso_cig_initiator_set_cig_param_t *param)
 * @brief     Set GAP CIG parameters. 
 * @param[in] param: The param for set CIG param
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_initiator_set_cig_param(rtk_bt_le_iso_cig_initiator_set_cig_param_t *param);

/**
 * @fn        uint16_t rtk_bt_le_iso_cis_initiator_set_cig_param(rtk_bt_le_iso_cig_initiator_set_cis_param_t *param)
 * @brief     Set GAP CIS parameters. 
 * @param[in] param: The param for set CIS param
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_initiator_set_cis_param(rtk_bt_le_iso_cig_initiator_set_cis_param_t *param);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_initiator_start_setting(uint8_t cig_id)
 * @brief     Request to create a CIG and to set the parameters of one or more CISes in the Controller. 
 * @param[in] cig_id: Identifier of a CIG
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_initiator_start_setting(uint8_t cig_id);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_initiator_set_cis_acl_link(uint8_t cis_id,uint8_t conn_id)
 * @brief     Specify the connection handle of the ACL connection associated with each CIS to be created. 
 * @param[in] cis_id: Identifier of a CIS
 * @param[in] conn_id: connection id
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_initiator_set_cis_acl_link(uint8_t cis_id,uint8_t conn_id);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_initiator_create_cis_by_cig_id(uint8_t cig_id)
 * @brief     Request to create all CISes in a CIG by CIG id. 
 * @param[in] cig_id: Identifier of a CIG
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_initiator_create_cis_by_cig_id(uint8_t cig_id);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_initiator_create_cis_by_cis_conn_handle(uint8_t cig_id,uint8_t cis_count,uint16_t *p_cis_conn_handle)
 * @brief     Request to create one or more CISes by cis conn handle. 
 * @param[in] cig_id: Identifier of a CIG
 * @param[in] cis_count: CIS count
 * @param[in] p_cis_conn_handle: Connection handle of the CIS 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_initiator_create_cis_by_cis_conn_handle(uint8_t cig_id,uint8_t cis_count,uint16_t *p_cis_conn_handle);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_initiator_get_cis_conn_handle(uint8_t cis_id, uint16_t *p_cis_conn_handle)
 * @brief     Get CIS connection handle by CIS id. 
 * @param[in]   cis_id:         Used to identify a CIS.
 * @param[out]  p_cis_conn_handle:   CIS connection handle.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_initiator_get_cis_conn_handle(uint8_t cis_id, uint16_t *p_cis_conn_handle);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_initiator_remove_cig(uint8_t cig_id)
 * @brief     Used by the Central's Host to remove the CIG.
 * @param[in]   cig_id:   Identifier of a CIG.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_initiator_remove_cig(uint8_t cig_id);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_initiator_register_callback(uint8_t cig_id)
 * @brief     Register CIG by cig_id. 
 * @param[in]   cig_id:   Identifier of a CIG.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_initiator_register_callback(uint8_t cig_id);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_acceptor_accept_cis(uint16_t cis_conn_handle)
 * @brief     Accept the request for the CIS. 
 * @param[in]   cis_conn_handle:   Connection handle of the CIS.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_acceptor_accept_cis(uint16_t cis_conn_handle);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_acceptor_reject_cis(uint16_t cis_conn_handle,uint8_t reason)
 * @brief     Reject the request for the CIS.. 
 * @param[in]   cis_conn_handle:  Connection handle of the CIS.
 * @param[in]   reason:   Reason the CIS request was rejected. @ref BT_HCI_ERROR (except @ref HCI_SUCCESS) 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_acceptor_reject_cis(uint16_t cis_conn_handle,uint8_t reason);

/**
 * @fn        uint16_t rtk_bt_le_iso_cig_acceptor_config_sdu_param(uint16_t cis_conn_handle,bool acceptor_config_sdu_flag,uint32_t sdu_interval_m_s,uint32_t sdu_interval_s_m,uint16_t max_sdu_m_s,uint16_t max_sdu_s_m)
 * @brief     Whether to check sdu length when rtk_bt_le_iso_data_send. 
 * @param[in]   cis_conn_handle:   Connection handle of the CIS.
 * @param[in]   acceptor_config_sdu_flag:   Whether to check sdu length when acceptor calls @ref rtk_bt_le_iso_data_send.
 * @param[in]   sdu_interval_m_s:   The interval, in microseconds, between the start of consecutive SDUs from the Central's Host for all the CISes in the CIG. Range: 0x0000FF to 0x0FFFFF
 * @param[in]   sdu_interval_s_m:   The interval, in microseconds, between the start of consecutive SDUs from the Peripheral's Host for all the CISes in the CIG. Range: 0x0000FF to 0x0FFFFF
 * @param[in]   max_sdu_m_s:   Maximum size, in octets, of the payload from the Central's Host.
 * @param[in]   max_sdu_s_m:   Maximum size, in octets, of the payload from the Peripheral's Host.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_acceptor_config_sdu_param(uint16_t cis_conn_handle,bool acceptor_config_sdu_flag,uint32_t sdu_interval_m_s,uint32_t sdu_interval_s_m,uint16_t max_sdu_m_s,uint16_t max_sdu_s_m);

/**
 * @fn        rtk_bt_err_t rtk_bt_le_iso_cig_acceptor_config_cis_request_action(rtk_bt_le_iso_cig_acceptor_config_cis_req_action_t cis_req_action)
 * @brief     Config the acceptor action when received cis request indication. Default action is RTK_BLE_ISO_ACCEPTOR_CIS_REQ_ACTION_ACCEPT. 
 * @param[in]   param:   The param for config CIS param.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_acceptor_config_cis_req_action(rtk_bt_le_iso_cig_acceptor_config_cis_req_action_t cis_req_action);


/**
 * @fn        uint16_t rtk_bt_le_iso_cig_acceptor_register_callback(void)
 * @brief     Register acceptor gap app callback. 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_cig_acceptor_register_callback(void);
#endif //end of #if (defined RTK_BLE_ISO_CIS_SUPPORT) && (RTK_BLE_ISO_CIS_SUPPORT)

#if (defined RTK_BLE_ISO_BIS_SUPPORT) && (RTK_BLE_ISO_BIS_SUPPORT)
/**
 * @fn        uint16_t rtk_bt_le_iso_big_broadcaster_init(uint8_t big_num,uint8_t bis_num)
 * @brief     Initialize the number of BIG and BIS for Boardcaster.
 * @param[in] big_num: BIG number
 * @param[in] bis_num: BIS number
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_big_broadcaster_init(uint8_t big_num,uint8_t bis_num);

/**
 * @fn        uint16_t rtk_bt_le_iso_big_broadcaster_create(uint8_t adv_handle, rtk_bt_le_iso_big_broadcaster_param_t *big_param, uint8_t *big_handle)
 * @brief     Create BIG broadcaster. 
 * @param[in]   adv_handle: Identify the periodic advertising train
 * @param[in]   param: The param for create BIG broadcaster
 * @param[out]  big_handle: Identify the BIG
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_big_broadcaster_create(uint8_t adv_handle, rtk_bt_le_iso_big_broadcaster_param_t *big_param, uint8_t *big_handle);

/**
 * @fn        uint16_t rtk_bt_le_iso_big_broadcaster_terminate(uint8_t big_handle)
 * @brief     Terminate BIG broadcaster. 
 * @param[in]  big_handle: Identify the BIG
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_big_broadcaster_terminate(uint8_t big_handle);

/**
 * @fn        uint16_t rtk_bt_le_iso_big_broadcaster_read_tx_sync(uint16_t bis_conn_handle, rtk_bt_le_iso_big_broadcaster_read_tx_sync_info_t * p_tx_sync_info)
 * @brief     Read the TX_Time_Stamp and Time_Offset of a transmitted SDU identified by the Packet_Sequence_Number on a BIS.
 * @param[in]   bis_conn_handle: Connection handle of the BIS. 
 * @param[out]  p_tx_sync_info: Read tx sync info result. 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_big_broadcaster_read_tx_sync(uint16_t bis_conn_handle,rtk_bt_le_iso_big_broadcaster_read_tx_sync_info_t * p_tx_sync_info);

/**
 * @fn        uint16_t rtk_bt_le_iso_big_receiver_init(uint8_t big_num,uint8_t bis_num)
 * @brief     Initialize the number of BIG and BIS for Receiver.
 * @param[in] big_num: BIG number
 * @param[in] bis_num: BIS number
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_big_receiver_init(uint8_t big_num,uint8_t bis_num);

/**
 * @fn        uint16_t rtk_bt_le_iso_big_receiver_create_sync(uint16_t sync_handle,rtk_bt_le_iso_big_receiver_create_sync_param_t *sync_param,uint8_t *p_big_handle)
 * @brief     Synchronize to a BIG described in the periodic advertising train. 
 * @param[in] sync_handle: Identify the periodic advertising train
 * @param[in] sync_param: GAP Synchronized Receiver BIG create sync parameter
 * @param[out] p_big_handle: Identify the BIG
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_big_receiver_create_sync(uint16_t sync_handle,rtk_bt_le_iso_big_receiver_create_sync_param_t *sync_param,uint8_t *p_big_handle);

/**
 * @fn        uint16_t rtk_bt_le_iso_big_receiver_terminate_sync(uint8_t big_handle)
 * @brief     Terminate synchronization to the BIG.
 * @param[in] big_handle: Identify the BIG
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_big_receiver_terminate_sync(uint8_t big_handle);

/**
 * @fn        uint16_t rtk_bt_le_iso_big_receiver_read_iso_link_quality(uint16_t bis_conn_handle,rtk_bt_le_iso_big_receiver_read_link_quality_info_t * p_link_quality_info)
 * @brief     Read the values of various counters related to link quality.
 * @param[in] bis_conn_handle: Connection handle of the BIS
 * @param[out] p_link_quality_info: Read link quality info result
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_big_receiver_read_iso_link_quality(uint16_t bis_conn_handle,rtk_bt_le_iso_big_receiver_read_link_quality_info_t * p_link_quality_info);

/**
 * @fn        uint16_t rtk_bt_le_iso_big_setup_path(rtk_bt_le_iso_setup_path_param_t *param)
 * @brief     Setup data path for BIS.
 * @param[in]   param: The param for setup BIS data path
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_big_setup_path(rtk_bt_le_iso_setup_path_param_t *param);

/**
 * @fn        uint16_t rtk_bt_le_iso_big_remove_path(uint16_t bis_conn_handle,rtk_bt_le_iso_data_path_direction_flag_t data_path_direction)
 * @brief     Remove data path for BIS.
 * @param[in]   bis_conn_handle: Connection handle of the BIS. 
 * @param[in]   data_path_direction: Specify which directions are to have the data path removed. 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_big_remove_path(uint16_t bis_conn_handle,rtk_bt_le_iso_data_path_direction_flag_t data_path_direction);

#endif //end of #if RTK_BLE_ISO_BIS_SUPPORT

/**
 * @fn        uint16_t rtk_bt_le_iso_data_send(rtk_bt_le_iso_data_send_info_t *info)
 * @brief     Send ISO data [ASYNC/SYNC].
 *            If sending request operation is success, the result will be returned by RTK_BT_LE_ISO_EVT_DATA_SEND_DONE 
 *            when enable RTK_BT_LE_ISO_DATA_SEND_ASYNC, otherwise the result will be returned when the API is done.
 * @param[in] info: The info of iso send data
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_iso_data_send(rtk_bt_le_iso_data_send_info_t *info);

/**
 * @}
 */

#endif //end of #if (defined RTK_BLE_ISO_SUPPORT) && (RTK_BLE_ISO_SUPPORT)

#ifdef __cplusplus
}
#endif

#endif  /* __RTK_BT_LE_ISO_H__ */