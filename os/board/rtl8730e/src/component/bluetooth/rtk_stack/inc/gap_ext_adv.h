/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_ext_adv.h
* @brief     Header file for Gap ext adv
* @details   This file defines extended advertising related API.
* @author    ranhui
* @date      2016-02-18
* @version   v1.0
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_EXT_ADV_H
#define GAP_EXT_ADV_H

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

#if F_BT_LE_5_0_AE_ADV_SUPPORT

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_EXTENDED_ADV GAP LE Extended Adv Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_EXTENDED_ADV_Exported_Macros GAP LE Extended Adv Exported Macros
  * @{
  */

/** @defgroup EXT_ADV_PARAM Extended Advertising Parameter flag
  * @brief Use the combination of macro definitions to set extended advertising related parameters
           for a specified advertising set by calling @ref le_ext_adv_start_setting.
  * @{
  */
#define EXT_ADV_SET_AUTO           0x00   /**< Automatically set extended advertising related parameters (including advertising parameters,
                                               advertising data and scan response data) according to advertising event properties. */
#define EXT_ADV_SET_ADV_PARAS      0x01   /**< Set advertising parameters supplied by @ref le_ext_adv_set_adv_param. */
#define EXT_ADV_SET_ADV_DATA       0x02   /**< Set advertising data supplied by @ref le_ext_adv_set_adv_data. */
#define EXT_ADV_SET_SCAN_RSP_DATA  0x04   /**< Set scan response data supplied by @ref le_ext_adv_set_scan_response_data. */
#define EXT_ADV_SET_RANDOM_ADDR    0x08   /**< Set random address supplied by @ref le_ext_adv_set_random. */
/** End of EXT_ADV_PARAM
  * @}
  */

/** @defgroup EXT_ADV_EVT_PROP Extended Advertising Event Properties flag
  * @brief Use the combination of macro definitions to describe the type of advertising event.
           Optional values: @ref T_LE_EXT_ADV_LEGACY_ADV_PROPERTY and @ref T_LE_EXT_ADV_EXTENDED_ADV_PROPERTY.
  * @{
  */
#define GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV     0x01 /**< Connectable advertising. */
#define GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV       0x02 /**< Scannable advertising. */
#define GAP_EXT_ADV_EVT_PROP_DIRECTED_ADV        0x04 /**< Directed advertising. */
#define GAP_EXT_ADV_EVT_PROP_HDC_DIRECTED_ADV    0x08 /**< High Duty Cycle Directed Connectable advertising. */
#define GAP_EXT_ADV_EVT_PROP_USE_LEGACY_ADV      0x10 /**< Use legacy advertising PDUs. */
#define GAP_EXT_ADV_EVT_PROP_OMIT_ADV_ADDR       0x20 /**< Omit advertiser's address from all PDUs ("anonymous advertising"). */
#define GAP_EXT_ADV_EVT_PROP_INCLUDE_TX_POWER    0x40 /**< Include TxPower in the extended header of the advertising PDU. */
/** End of EXT_ADV_EVT_PROP
  * @}
  */

#define GAP_MAX_LEGACY_ADV_LEN                   31

#define GAP_INVALID_ADV_HANDLE   0xFF
/** End of GAP_LE_EXTENDED_ADV_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_EXTENDED_ADV_Exported_Types GAP LE Extended Adv Exported Types
  * @{
  */

/** @brief GAP extended advertising state. */
typedef enum
{
    EXT_ADV_STATE_IDLE,         /**< Idle, no advertising. */
    EXT_ADV_STATE_START,        /**< Start Advertising. A temporary state, haven't received the result. */
    EXT_ADV_STATE_ADVERTISING,  /**< Advertising. */
    EXT_ADV_STATE_STOP,         /**< Stop Advertising. A temporary state, haven't received the result. */
} T_GAP_EXT_ADV_STATE;

/** @brief Advertising Event Properties values for legacy advertising PDUs. */
typedef enum
{
    LE_EXT_ADV_LEGACY_ADV_CONN_SCAN_UNDIRECTED           = 0x13, /**<  Connectable and scannable undirected. Advertising data or scan response data shall not exceed 31 bytes. */
    LE_EXT_ADV_LEGACY_ADV_CONN_LOW_DUTY_DIRECTED         = 0x15, /**<  Connectable directed (low duty cycle). */
    LE_EXT_ADV_LEGACY_ADV_CONN_HIGH_DUTY_DIRECTED        = 0x1D, /**<  Connectable directed (high duty cycle). */
    LE_EXT_ADV_LEGACY_ADV_SCAN_UNDIRECTED                = 0x12, /**<  Scannable undirected. Advertising data or scan response data shall not exceed 31 bytes. */
    LE_EXT_ADV_LEGACY_ADV_NON_SCAN_NON_CONN_UNDIRECTED   = 0x10, /**<  Non-connectable and non-scannable undirected. Advertising data shall not exceed 31 bytes. */
} T_LE_EXT_ADV_LEGACY_ADV_PROPERTY;

/** @brief Advertising Event Properties values for extended advertising PDUs. */
typedef enum
{
    LE_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_UNDIRECTED = 0x00, /**<  Non-connectable and non-scannable undirected. */
    LE_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_DIRECTED   = 0x04, /**<  Non-connectable and non-scannable directed. */
    LE_EXT_ADV_EXTENDED_ADV_CONN_UNDIRECTED              = 0x01, /**<  Connectable undirected. Advertising data shall not exceed 245 bytes. */
    LE_EXT_ADV_EXTENDED_ADV_CONN_DIRECTED                = 0x05, /**<  Connectable directed. Advertising data shall not exceed 239 bytes. */
    LE_EXT_ADV_EXTENDED_ADV_SCAN_UNDIRECTED              = 0x02, /**<  Scannable undirected. */
    LE_EXT_ADV_EXTENDED_ADV_SCAN_DIRECTED                = 0x06, /**<  Scannable directed. */
} T_LE_EXT_ADV_EXTENDED_ADV_PROPERTY;

/** @brief Supported advertising parameters type. */
typedef enum
{
    GAP_PARAM_EXT_ADV_MAX_DATA_LEN   = 0x330, /**<  Maximum length of supported data for use as advertisement data or scan
                                                    response data. Read only. Size is 2 bytes. */
    GAP_PARAM_EXT_ADV_MAX_SETS       = 0x331, /**<  Maximum number of supported advertising sets. Read only. Size is 1 byte. */
} T_LE_EXT_ADV_PARAM_TYPE;

/** End of GAP_LE_EXTENDED_ADV_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_EXTENDED_ADV_Exported_Functions GAP LE Extended Adv Exported Functions
  * @brief
  * @{
  */
/**
 * @brief       Initialization the number of advertising sets.
 *
 * @param[in]   adv_set_num Advertising sets number, range: 1 - 10.
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS  Operation success.
 * @retval GAP_CAUSE_INVALID_PARAM Operation failure, invalid parameter.
 * @retval GAP_CAUSE_NO_RESOURCE   Operation failure, memory acquisition failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        le_ext_adv_init(6);
    }
 * \endcode
 */
T_GAP_CAUSE le_ext_adv_init(uint8_t adv_set_num);

/**
 * @brief   Get a GAP extended advertising parameter.
 *
 *          NOTE: You can call this function with a extended advertising parameter type and it will get a
 *          extended advertising parameter. Extended advertising parameters are defined in @ref T_LE_EXT_ADV_PARAM_TYPE.
 *
 * @param[in]      param   Advertising parameter type: @ref T_LE_EXT_ADV_PARAM_TYPE
 * @param[in,out]  p_value Pointer to the location to get the parameter value.  This is dependent on
 *                         the parameter type and will be cast to the appropriate data type (For example:
 *                         if data type of param is uint16_t, p_value will be cast to pointer of uint16_t).
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS       Operation success.
 * @retval GAP_CAUSE_INVALID_PARAM Operation failure, invalid parameter.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint16_t max_adv_data_len;
        le_ext_adv_get_param(GAP_PARAM_EXT_ADV_MAX_DATA_LEN, &max_adv_data_len);
    }
 * \endcode
 */
T_GAP_CAUSE le_ext_adv_get_param(T_LE_EXT_ADV_PARAM_TYPE param, void *p_value);

/**
 * @brief  Create an advertising handle which is used to identify an advertising set.
 *
 * @return Advertising handle.
 * @retval 0x00-0xFE Operation success.
 * @retval 0xFF      Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t adv_handle;
        adv_handle = le_ext_adv_create_adv_handle();
    }
 * \endcode
 */
uint8_t le_ext_adv_create_adv_handle(void);

/**
 * @brief  Get adv handle from connection ID.
 *
 *         If the advertising ends because a connection was created,
 *         application can call le_ext_adv_get_adv_handle_by_conn_id() to get the adverting handle information
 *         when the advertising set state switches to EXT_ADV_STATE_IDLE.
 *
 * @param[in]   conn_id Connection ID.
 * @return adv_handle.
 * @retval 0-0xFE  Operation success.
 * @retval 0xFF Operation failure, Get advertising handle failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t adv_handle;
        adv_handle = le_ext_adv_get_adv_handle_by_conn_id(conn_id);
    }
 * \endcode
 */
uint8_t le_ext_adv_get_adv_handle_by_conn_id(uint8_t conn_id);

/**
 * @brief       Set GAP extended advertising parameters for an advertising set.
 *
 * @param[in]   adv_handle                    Identify an advertising set, which is assigned by @ref le_ext_adv_create_adv_handle.
 * @param[in]   adv_event_prop                Type of advertising event.
                                              Values for extended advertising PDUs: @ref T_LE_EXT_ADV_EXTENDED_ADV_PROPERTY.
                                              Values for legacy advertising PDUs: @ref T_LE_EXT_ADV_LEGACY_ADV_PROPERTY.
 * @param[in]   primary_adv_interval_min      Minimum advertising interval for undirected and low duty directed advertising.
                                              In units of 0.625ms, range: 0x000020 to 0xFFFFFF.
 * @param[in]   primary_adv_interval_max      Maximum advertising interval for undirected and low duty directed advertising.
                                              In units of 0.625ms, range: 0x000020 to 0xFFFFFF.
 * @param[in]   primary_adv_channel_map       A bit field that indicates the advertising channels that shall be used when
                                              transmitting advertising packets. @ref ADV_CHANNEL_MAP.
 * @param[in]   own_address_type              Local address type, @ref T_GAP_LOCAL_ADDR_TYPE.
 * @param[in]   peer_address_type             Remote address type, GAP_REMOTE_ADDR_LE_PUBLIC or GAP_REMOTE_ADDR_LE_RANDOM in @ref T_GAP_REMOTE_ADDR_TYPE.
                                              GAP_REMOTE_ADDR_LE_PUBLIC: Public Device Address or Public Identity Address.
                                              GAP_REMOTE_ADDR_LE_RANDOM: Random Device Address or Random(static) Identity Address.
 * @param[in]   p_peer_address                Remote address.
 * @param[in]   filter_policy                 Advertising filter policy: @ref T_GAP_ADV_FILTER_POLICY.
 * @param[in]   tx_power                      Advertising Tx power.
 * @param[in]   primary_adv_phy               Indicate the PHY on which the advertising packets are transmitted on the primary advertising channel.
                                              @ref T_GAP_PHYS_PRIM_ADV_TYPE.
                                              If legacy advertising PDUs are being used, the primary_adv_phy shall indicate the LE 1M PHY (@ref GAP_PHYS_PRIM_ADV_1M).
 * @param[in]   secondary_adv_max_skip        Maximum number of advertising events that can be skipped. Usually set to zero.
 * @param[in]   secondary_adv_phy             Indicate the PHY on which the advertising packets are transmitted on the secondary advertising channel.
                                              @ref T_GAP_PHYS_TYPE.
 * @param[in]   adv_sid                       Specify the value to be transmitted in Advertising SID subfield of those advertising channel
                                              PDUs that have this field. Usually set to zero.
 * @param[in]   scan_req_notification_enable  Indicate whether Host will be notified upon the receipt of a scan request PDU.
                                              Usually set to false.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS  Operation success.
 * @retval GAP_CAUSE_NOT_FIND Operation failure, the advertising handle is not found.
 *
 * <b>Example usage</b>
 * \code{.c}
     void le_init_ext_adv_params_ext_conn(void)
     {
        uint8_t adv_handle;
        T_LE_EXT_ADV_EXTENDED_ADV_PROPERTY adv_event_prop = LE_EXT_ADV_EXTENDED_ADV_CONN_UNDIRECTED;
        uint32_t primary_adv_interval_min = DEFAULT_ADVERTISING_INTERVAL_MIN;
        uint32_t primary_adv_interval_max = DEFAULT_ADVERTISING_INTERVAL_MAX;
        uint8_t  primary_adv_channel_map = GAP_ADVCHAN_ALL;
        T_GAP_LOCAL_ADDR_TYPE own_address_type = GAP_LOCAL_ADDR_LE_PUBLIC;
        T_GAP_REMOTE_ADDR_TYPE peer_address_type = GAP_REMOTE_ADDR_LE_PUBLIC;
        uint8_t p_peer_address[6] = {0};
        T_GAP_ADV_FILTER_POLICY filter_policy = GAP_ADV_FILTER_ANY;
        uint8_t tx_power = 127; //Host has no preference.
        T_GAP_PHYS_PRIM_ADV_TYPE primary_adv_phy = GAP_PHYS_PRIM_ADV_1M;
        uint8_t secondary_adv_max_skip = 0;
        T_GAP_PHYS_TYPE secondary_adv_phy = GAP_PHYS_2M;
        uint8_t adv_sid = 0;
        bool scan_req_notification_enable = false;

        adv_handle = le_ext_adv_create_adv_handle();
        le_ext_adv_set_adv_param(adv_handle,
                                 adv_event_prop,
                                 primary_adv_interval_min,
                                 primary_adv_interval_max,
                                 primary_adv_channel_map,
                                 own_address_type,
                                 peer_address_type,
                                 p_peer_address,
                                 filter_policy,
                                 tx_power,
                                 primary_adv_phy,
                                 secondary_adv_max_skip,
                                 secondary_adv_phy,
                                 adv_sid,
                                 scan_req_notification_enable);
     }
 * \endcode
 */
T_GAP_CAUSE le_ext_adv_set_adv_param(uint8_t adv_handle, uint16_t adv_event_prop,
                                     uint32_t primary_adv_interval_min, uint32_t primary_adv_interval_max,
                                     uint8_t primary_adv_channel_map, T_GAP_LOCAL_ADDR_TYPE own_address_type,
                                     T_GAP_REMOTE_ADDR_TYPE peer_address_type, uint8_t *p_peer_address,
                                     T_GAP_ADV_FILTER_POLICY filter_policy, uint8_t tx_power,
                                     T_GAP_PHYS_PRIM_ADV_TYPE primary_adv_phy, uint8_t secondary_adv_max_skip,
                                     T_GAP_PHYS_TYPE secondary_adv_phy, uint8_t adv_sid,
                                     bool scan_req_notification_enable);

/**
 * @brief       Set GAP advertising data for an advertising set.
 *
 *     NOTE: This function just saves the pointer of the advertising data and will not copy the advertising data.
 *         So application should the const array or the global array to save the advertising data.
 *         If application uses a dynamically requested buffer to save the advertising data, do not release it.
 *
 * @param[in]   adv_handle       Identify an advertising set, which is assigned by @ref le_ext_adv_create_adv_handle.
 * @param[in]   adv_data_len     The length of advertising data.
 * @param[in]   p_adv_data       Pointer to advertising data to write.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS       Operation success.
 * @retval GAP_CAUSE_NOT_FIND      Operation failure, the advertising handle is not found.
 * @retval GAP_CAUSE_INVALID_PARAM Operation failure, the length of advertising data exceeds
                                   1024 bytes(Maximum total length of GAP extended advertising data).
 *
 * <b>Example usage</b>
 * \code{.c}
    static const uint8_t ext_adv_data[] =
    {
        // Flags
        0x02,
        GAP_ADTYPE_FLAGS,
        GAP_ADTYPE_FLAGS_LIMITED | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
        // Local name
        0x13,
        GAP_ADTYPE_LOCAL_NAME_COMPLETE,
        'B', 'L', 'E', '_', 'B', 'T', '5', '_', 'P', 'e', 'r', 'i', 'p', 'h', 'e', 'r', 'a', 'l',
        // Manufacturer Specific Data
        0xdd,
        GAP_ADTYPE_MANUFACTURER_SPECIFIC,
        0x5d, 0x00,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
        0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
        0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5,
        0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7,
        0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9,
        0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb,
        0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd
    };

    void test(void)
    {
        le_ext_adv_set_adv_data(adv_handle, sizeof(ext_adv_data), (uint8_t *)ext_adv_data);
    }
 * \endcode
 */
T_GAP_CAUSE le_ext_adv_set_adv_data(uint8_t adv_handle, uint16_t adv_data_len, uint8_t *p_adv_data);

/**
 * @brief       Set GAP scan response data for an advertising set.
 *
 * @param[in]   adv_handle       Identify an advertising set, which is assigned by @ref le_ext_adv_create_adv_handle.
 * @param[in]   scan_data_len    The length of scan response data.
 * @param[in]   p_scan_data      Pointer to scan response data to write.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS       Operation success.
 * @retval GAP_CAUSE_NOT_FIND      Operation failure, the advertising handle is not found.
 * @retval GAP_CAUSE_INVALID_PARAM Operation failure, the length of advertising data exceeds
                                   1024 bytes(Maximum total length of GAP extended advertising data).
 *
 * <b>Example usage</b>
 * \code{.c}
    static const uint8_t ext_scan_rsp_data[] =
    {
        // Manufacturer Specific Data
        0xfc,
        GAP_ADTYPE_MANUFACTURER_SPECIFIC,
        0x5d, 0x00,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
        0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
        0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5,
        0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7,
        0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9,
        0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb,
        0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd,
        0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf
    };

    void test(void)
    {
        le_ext_adv_set_scan_response_data(adv_handle, sizeof(ext_scan_rsp_data), (uint8_t *)ext_scan_rsp_data);
    }
 * \endcode
 */
T_GAP_CAUSE le_ext_adv_set_scan_response_data(uint8_t adv_handle, uint16_t scan_data_len,
                                              uint8_t *p_scan_data);

/**
 * @brief       Set GAP random device address for an advertising set.
 *
 * @param[in]   adv_handle       Identify an advertising set, which is assigned by @ref le_ext_adv_create_adv_handle.
 * @param[in]   random_address   Pointer to random address to write.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS  Operation success.
 * @retval GAP_CAUSE_NOT_FIND Operation failure, the advertising handle is not found.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t rand_addr[6];
        le_gen_rand_addr(GAP_RAND_ADDR_STATIC, rand_addr);
        le_ext_adv_set_random(adv_handle, rand_addr);
    }
 * \endcode
 */
T_GAP_CAUSE le_ext_adv_set_random(uint8_t adv_handle, uint8_t *random_address);

/**
 * @brief       Set extended advertising parameters for an advertising set.
                If request success, the result of setting extended advertising parameters will be returned by
                @ref app_gap_callback with cb_type @ref GAP_MSG_LE_EXT_ADV_START_SETTING.
 *
 * @param[in]   adv_handle     Identify an advertising set, which is assigned by @ref le_ext_adv_create_adv_handle.
 * @param[in]   update_flags   A bit field that indicates extended advertising parameters that shall be set. @ref EXT_ADV_PARAM.
                               Recommendation: if random address is not used, set update_flags to @ref EXT_ADV_SET_AUTO.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS  Set parameters request success.
 * @retval GAP_CAUSE_NOT_FIND Set parameters request failed, the advertising handle is not found.
 *
 * <b>Example usage</b>
 * \code{.c}
    void le_init_ext_adv_params_ext(void)
    {
        T_LE_EXT_ADV_EXTENDED_ADV_PROPERTY adv_event_prop =
            LE_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_UNDIRECTED;
        uint32_t primary_adv_interval_min = DEFAULT_ADVERTISING_INTERVAL_MIN;
        uint32_t primary_adv_interval_max = DEFAULT_ADVERTISING_INTERVAL_MAX;
        uint8_t  primary_adv_channel_map = GAP_ADVCHAN_ALL;
        T_GAP_LOCAL_ADDR_TYPE own_address_type = GAP_LOCAL_ADDR_LE_PUBLIC;
        T_GAP_REMOTE_ADDR_TYPE peer_address_type = GAP_REMOTE_ADDR_LE_PUBLIC;
        uint8_t p_peer_address[6] = {0};
        T_GAP_ADV_FILTER_POLICY filter_policy = GAP_ADV_FILTER_ANY;
        uint8_t tx_power = 127; //Host has no preference.
        T_GAP_PHYS_PRIM_ADV_TYPE primary_adv_phy = GAP_PHYS_PRIM_ADV_1M;
        uint8_t secondary_adv_max_skip = 0;
        T_GAP_PHYS_TYPE secondary_adv_phy = GAP_PHYS_2M;
        uint8_t adv_sid = 0;
        bool scan_req_notification_enable = false;

        adv_handle = le_ext_adv_create_adv_handle();
        le_ext_adv_set_adv_param(adv_handle,
                                 adv_event_prop,
                                 primary_adv_interval_min,
                                 primary_adv_interval_max,
                                 primary_adv_channel_map,
                                 own_address_type,
                                 peer_address_type,
                                 p_peer_address,
                                 filter_policy,
                                 tx_power,
                                 primary_adv_phy,
                                 secondary_adv_max_skip,
                                 secondary_adv_phy,
                                 adv_sid,
                                 scan_req_notification_enable);

        le_ext_adv_set_adv_data(adv_handle, sizeof(ext_adv_data), (uint8_t *)ext_adv_data);
    }

    void test(void)
    {
        le_init_ext_adv_params_ext();
        le_ext_adv_start_setting(adv_handle, EXT_ADV_SET_AUTO);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        switch (cb_type)
        {
        case GAP_MSG_LE_EXT_ADV_START_SETTING:
        APP_PRINT_INFO3("GAP_MSG_LE_EXT_ADV_START_SETTING:cause 0x%x, flag 0x%x, adv_handle %d",
                        p_data->p_le_ext_adv_start_setting_rsp->cause, p_data->p_le_ext_adv_start_setting_rsp->flag,
                        p_data->p_le_ext_adv_start_setting_rsp->adv_handle);

        if (p_data->p_le_ext_adv_start_setting_rsp->cause == GAP_CAUSE_SUCCESS)
        {
            // Initialize enable parameters
            le_init_ext_adv_enable_params(p_data->p_le_ext_adv_start_setting_rsp->adv_handle);
            // Enable one advertising set
            le_ext_adv_enable(1, &p_data->p_le_ext_adv_start_setting_rsp->adv_handle);
        }
        break;
        }
    }
 * \endcode
 */
T_GAP_CAUSE le_ext_adv_start_setting(uint8_t adv_handle, uint8_t update_flags);

/**
 * @brief       Set GAP extended advertising enable parameters for an advertising set.
 *
 * @param[in]   adv_handle       Identify an advertising set, which is assigned by @ref le_ext_adv_create_adv_handle.
 * @param[in]   duration         If non-zero, indicates the duration that advertising set is enabled.
                                 0x0000:        No advertising duration.
                                 0x0001-0xFFFF: Advertising duration, in units of 10ms.
 * @param[in]   max_ext_adv_evt  If non-zero, indicates the maximum extended advertising events that shall be
                                 sent prior to disabling the extended advertising set.
                                 0x00:      No maximum number of advertising events.
                                 0x01-0xFF: Maximum number of extended advertising events to send prior to terminating
                                            the extended advertising.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS  Operation success.
 * @retval GAP_CAUSE_NOT_FIND Operartion failure, the advertising handle is not found.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint16_t duration = 0;
        uint8_t  max_ext_adv_evt  = 0;

        le_ext_adv_set_adv_enable_param(adv_handle, duration, max_ext_adv_evt);
    }
 * \endcode
 */
T_GAP_CAUSE le_ext_adv_set_adv_enable_param(uint8_t adv_handle, uint16_t duration,
                                            uint8_t max_ext_adv_evt);

/**
 * @brief       Enable extended advertising for one or more advertising sets.
                If device changes to advertising state, @ref app_handle_ext_adv_state_evt will be called, and
                @ref app_gap_callback with cb_type @ref GAP_MSG_LE_EXT_ADV_ENABLE will be called.
 *
 * @param[in]   num_of_sets      Number of advertising sets to enable.
 * @param[in]   adv_handle       Pointer to advertising set to enable.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS  Enable request success.
 * @retval GAP_CAUSE_NOT_FIND Enable request failed, the advertising handle is not found.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        case GAP_MSG_LE_EXT_ADV_START_SETTING:
        APP_PRINT_INFO3("GAP_MSG_LE_EXT_ADV_START_SETTING:cause 0x%x, flag 0x%x, adv_handle %d",
                        p_data->p_le_ext_adv_start_setting_rsp->cause, p_data->p_le_ext_adv_start_setting_rsp->flag,
                        p_data->p_le_ext_adv_start_setting_rsp->adv_handle);

        if (p_data->p_le_ext_adv_start_setting_rsp->cause == GAP_CAUSE_SUCCESS)
        {
            // Initialize enable parameters
            le_init_ext_adv_enable_params(p_data->p_le_ext_adv_start_setting_rsp->adv_handle);
            // Enable one advertising set
            le_ext_adv_enable(1, &p_data->p_le_ext_adv_start_setting_rsp->adv_handle);
        }
        break;
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        switch (cb_type)
        {
        case GAP_MSG_LE_EXT_ADV_ENABLE:
            APP_PRINT_INFO1("GAP_MSG_LE_EXT_ADV_ENABLE:cause 0x%x", p_data->le_cause.cause);
            break;
        }
    }

    void app_handle_gap_msg(T_IO_MSG *p_gap_msg)
    {
        T_LE_GAP_MSG gap_msg;
        uint8_t conn_id;
        memcpy(&gap_msg, &p_gap_msg->u.param, sizeof(p_gap_msg->u.param));

        APP_PRINT_TRACE1("app_handle_gap_msg: subtype %d", p_gap_msg->subtype);
        switch (p_gap_msg->subtype)
        {
        case GAP_MSG_LE_EXT_ADV_STATE_CHANGE:
            {
                app_handle_ext_adv_state_evt(gap_msg.msg_data.gap_ext_adv_state_change.adv_handle,
                                             (T_GAP_EXT_ADV_STATE)gap_msg.msg_data.gap_ext_adv_state_change.new_state,
                                             gap_msg.msg_data.gap_ext_adv_state_change.cause);
            }
            break;
        }
     }

     void app_handle_ext_adv_state_evt(uint8_t adv_handle, T_GAP_EXT_ADV_STATE new_state, uint16_t cause)
     {
        for (int i = 0; i < APP_MAX_ADV_SET; i++)
        {
            if (ext_adv_state[i].adv_handle == adv_handle)
            {
                APP_PRINT_INFO2("app_handle_ext_adv_state_evt: adv_handle = %d oldState = %d",
                                ext_adv_state[i].adv_handle, ext_adv_state[i].ext_adv_state);
                ext_adv_state[i].ext_adv_state = new_state;
                break;
            }
        }
        APP_PRINT_INFO2("app_handle_ext_adv_state_evt: adv_handle = %d newState = %d",
                        adv_handle, new_state);
        switch (new_state)
        {
        // device is idle
        case EXT_ADV_STATE_IDLE:
            {
                APP_PRINT_INFO2("EXT_ADV_STATE_IDLE: adv_handle %d, cause 0x%x", adv_handle, cause);
            }
            break;

        // device is advertising
        case EXT_ADV_STATE_ADVERTISING:
            {
                APP_PRINT_INFO2("EXT_ADV_STATE_ADVERTISING: adv_handle %d, cause 0x%x", adv_handle, cause);
            }
            break;

        default:
            break;
        }
     }
 * \endcode
 */
T_GAP_CAUSE le_ext_adv_enable(uint8_t num_of_sets, uint8_t *adv_handle);

/**
 * @brief       Disable extended advertising for one or more advertising sets.
                If device changes to idle state, @ref app_handle_ext_adv_state_evt will be called, and
                @ref app_gap_callback with cb_type @ref GAP_MSG_LE_EXT_ADV_DISABLE will be called.
 *
 * @param[in]   num_of_sets      Number of advertising sets to enable.
 * @param[in]   adv_handle       Pointer to advertising set to enable.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS  Disable request success.
 * @retval GAP_CAUSE_NOT_FIND Disable request failed, the advertising handle is not found.
 *
 * <b>Example usage</b>
 * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_stopeadv(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t adv_handle[4];
        uint8_t num_of_sets = 1;
        T_GAP_CAUSE cause;
        adv_handle[0] = p_parse_value->dw_param[0];

        cause = le_ext_adv_disable(num_of_sets, adv_handle);
        return (T_USER_CMD_PARSE_RESULT)cause;
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        switch (cb_type)
        {
        case GAP_MSG_LE_EXT_ADV_DISABLE:
        APP_PRINT_INFO1("GAP_MSG_LE_EXT_ADV_DISABLE:cause 0x%x", p_data->le_cause.cause);
        break;
        }
    }

    void app_handle_gap_msg(T_IO_MSG *p_gap_msg)
    {
        T_LE_GAP_MSG gap_msg;
        uint8_t conn_id;
        memcpy(&gap_msg, &p_gap_msg->u.param, sizeof(p_gap_msg->u.param));

        APP_PRINT_TRACE1("app_handle_gap_msg: subtype %d", p_gap_msg->subtype);
        switch (p_gap_msg->subtype)
        {
        case GAP_MSG_LE_EXT_ADV_STATE_CHANGE:
            {
                app_handle_ext_adv_state_evt(gap_msg.msg_data.gap_ext_adv_state_change.adv_handle,
                                             (T_GAP_EXT_ADV_STATE)gap_msg.msg_data.gap_ext_adv_state_change.new_state,
                                             gap_msg.msg_data.gap_ext_adv_state_change.cause);
            }
            break;
        }
     }

     void app_handle_ext_adv_state_evt(uint8_t adv_handle, T_GAP_EXT_ADV_STATE new_state, uint16_t cause)
     {
        for (int i = 0; i < APP_MAX_ADV_SET; i++)
        {
            if (ext_adv_state[i].adv_handle == adv_handle)
            {
                APP_PRINT_INFO2("app_handle_ext_adv_state_evt: adv_handle = %d oldState = %d",
                                ext_adv_state[i].adv_handle, ext_adv_state[i].ext_adv_state);
                ext_adv_state[i].ext_adv_state = new_state;
                break;
            }
        }
        APP_PRINT_INFO2("app_handle_ext_adv_state_evt: adv_handle = %d newState = %d",
                        adv_handle, new_state);
        switch (new_state)
        {
        // device is idle
        case EXT_ADV_STATE_IDLE:
            {
                APP_PRINT_INFO2("EXT_ADV_STATE_IDLE: adv_handle %d, cause 0x%x", adv_handle, cause);
            }
            break;

        // device is advertising
        case EXT_ADV_STATE_ADVERTISING:
            {
                APP_PRINT_INFO2("EXT_ADV_STATE_ADVERTISING: adv_handle %d, cause 0x%x", adv_handle, cause);
            }
            break;

        default:
            break;
        }
     }
 * \endcode
 */
T_GAP_CAUSE le_ext_adv_disable(uint8_t num_of_sets, uint8_t *adv_handle);

/**
 * @brief       Remove all existing advertising sets.
                The result of removing all existing advertising sets will be returned by
                @ref app_gap_callback with cb_type @ref GAP_MSG_LE_EXT_ADV_CLEAR_SET.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS Clear request success.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        le_ext_adv_clear_set();
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        switch (cb_type)
        {
        case GAP_MSG_LE_EXT_ADV_CLEAR_SET:
        APP_PRINT_INFO1("GAP_MSG_LE_EXT_ADV_CLEAR_SET:cause 0x%x",
                        p_data->p_le_ext_adv_clear_set_rsp->cause);
        break;
        }
    }
 * \endcode
 */
T_GAP_CAUSE le_ext_adv_clear_set(void);

/**
 * @brief       Remove an advertising set.
                If request success, the result of removing an advertising set will be returned by
                @ref app_gap_callback with cb_type @ref GAP_MSG_LE_EXT_ADV_REMOVE_SET.
 *
 * @param[in]   adv_handle     Identify an advertising set.
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS        Remove request success.
 * @retval GAP_CAUSE_NOT_FIND       Remove request failed, the advertising handle is not found.
 * @retval GAP_CAUSE_INVALID_STATE  Remove request failed, invalid state.
 * @retval GAP_CAUSE_ALREADY_IN_REQ Remove request failed, operation is already in progress.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        le_ext_adv_remove_set(adv_handle);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        switch (cb_type)
        {
        case GAP_MSG_LE_EXT_ADV_REMOVE_SET:
        APP_PRINT_INFO2("GAP_MSG_LE_EXT_ADV_REMOVE_SET:cause 0x%x, adv_handle %d",
                        p_data->p_le_ext_adv_remove_set_rsp->cause, p_data->p_le_ext_adv_remove_set_rsp->adv_handle);
        break;
        }
    }
 * \endcode
 */
T_GAP_CAUSE le_ext_adv_remove_set(uint8_t adv_handle);

/**
 * @brief       Check whether all advertising sets are idle state.
 *
 * @return Operation result.
 * @retval true        All advertising sets are @ref EXT_ADV_STATE_IDLE.
 * @retval false       At least one advertising set is not @ref EXT_ADV_STATE_IDLE.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool ret = le_ext_adv_check_adv_idle();
    }
 * \endcode
 */
bool le_ext_adv_check_adv_idle(void);

#if F_BT_LE_GAP_MSG_INFO_WAY
/**
* @brief  Set extended advertising gap message inform way.
*
*         Default value is True.
*         If use_msg is True, gap will send the extended advertising gap message to io_queue registered by
*         gap_start_bt_stack. Message type is @ref GAP_MSG_LE_EXT_ADV_STATE_CHANGE.
*         If use_msg is False, gap will send the extended advertising gap message using callback function registered by
*         @ref app_gap_callback. Message type is @ref GAP_MSG_LE_EXT_ADV_STATE_CHANGE_INFO.
*
* @param[in] use_msg Whether to use message.
* @retval void
*
* <b>Example usage</b>
* \code{.c}
    int test(void)
    {
        le_ext_adv_gap_msg_info_way(false);
    }

    void app_handle_ext_adv_state_evt(uint8_t adv_handle, T_GAP_EXT_ADV_STATE new_state, uint16_t cause)
    {
        ......
        switch (new_state)
        {
        case EXT_ADV_STATE_IDLE:
            {
            }
            break;

        case EXT_ADV_STATE_ADVERTISING:
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
        case GAP_MSG_LE_EXT_ADV_STATE_CHANGE_INFO:
            {
                app_handle_ext_adv_state_evt(p_data->p_le_ext_adv_state_change_info->adv_handle,
                                            (T_GAP_EXT_ADV_STATE)p_data->p_le_ext_adv_state_change_info->state,
                                            p_data->p_le_ext_adv_state_change_info->cause);
            }
            break;
        }
        ......
    }
* \endcode
*/
void le_ext_adv_gap_msg_info_way(bool use_msg);
#endif

/** End of GAP_LE_EXTENDED_ADV_Exported_Functions
  * @}
  */

/** End of GAP_LE_EXTENDED_ADV
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

#endif /* GAP_EXT_ADV_H */
