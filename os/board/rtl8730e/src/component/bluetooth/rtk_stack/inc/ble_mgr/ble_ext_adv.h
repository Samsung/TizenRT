/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */
#ifndef _BLE_EXT_ADV_MGR__
#define _BLE_EXT_ADV_MGR__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup BLE_EXTENDED_ADV Ble Extended Adv
  * @brief Ble extended advertising manager module
  * @{
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include "gap_ext_adv.h"
#include "gap_le.h"
#include "gap_msg.h"

/*============================================================================*
 *                              Constants
 *============================================================================*/
#define BLE_EXT_ADV_STATE_CHANGE  0x01 /**< used to notify adv set application callback function about the change of ext adv state*/
#define BLE_EXT_ADV_SET_CONN_INFO 0x02 /**< used to notify adv set application callback function about the link connection info*/

#define BLE_EXT_ADV_MGR_VERSION   1 /**< used to set ble adv manager module version*/
/** @brief  the maximum of extend advertising event stored in extend adv queue */
#define MAX_NUM_OF_EXTEND_ADV_EVENT       0x64

/*============================================================================*
 *                         Types
 *============================================================================*/
/**
* @brief define application adv sate, for application only have two adv state :BLE_EXT_ADV_MGR_ADV_DISABLED or BLE_EXT_ADV_MGR_ADV_ENABLED
*/
typedef enum
{
    BLE_EXT_ADV_MGR_ADV_DISABLED,/**< when call api ble_ext_adv_mgr_disable, the application adv state will be set to BLE_EXT_ADV_MGR_ADV_DISABLED*/
    BLE_EXT_ADV_MGR_ADV_ENABLED, /**< when call api ble_ext_adv_mgr_enable, the application adv state will be set to BLE_EXT_ADV_MGR_ADV_ENABLED*/
} T_BLE_EXT_ADV_MGR_STATE;

/**
 * @brief define the reason of adv stop
 */
typedef enum
{
    BLE_EXT_ADV_STOP_CAUSE_UNKNOWN,
    BLE_EXT_ADV_STOP_CAUSE_APP,
    BLE_EXT_ADV_STOP_CAUSE_CONN,
    BLE_EXT_ADV_STOP_CAUSE_TIMEOUT,
    BLE_EXT_ADV_STOP_CAUSE_ENABLE_FAILED,
} T_BLE_EXT_ADV_STOP_CAUSE;

/**
 * @brief used to notify application callback function about the change of adv state
 * stop_cause: @ref T_BLE_EXT_ADV_STOP_CAUSE
 * app_cause: app stop cause
 */
typedef struct
{
    uint8_t                  adv_handle;
    T_BLE_EXT_ADV_MGR_STATE  state;
    T_BLE_EXT_ADV_STOP_CAUSE stop_cause;
    uint8_t app_cause;
} T_BLE_EXT_ADV_STATE_CHANGE;

typedef struct
{
    uint8_t adv_handle;
    uint8_t conn_id;
} T_BLE_EXT_ADV_SET_CONN_INFO;

/**
 * @brief The callback data of application callback function
 * @ref T_BLE_EXT_ADV_STATE_CHANGE and @ref T_BLE_EXT_ADV_SET_CONN_INFO
 */
typedef union
{
    T_BLE_EXT_ADV_STATE_CHANGE  *p_ble_state_change;
    T_BLE_EXT_ADV_SET_CONN_INFO *p_ble_conn_info;
} T_BLE_EXT_ADV_CB_DATA;

/*============================================================================*
 *                              Functions
 *============================================================================*/
/**
*@brief used to register application callback function for each adv set
*@note
*@param[in] app_callback  application callback function @ref P_FUN_GAP_APP_CB
*@param[in] adv_handle    ble adv handle, each adv set has a adv handle
*@return    T_GAP_CAUSE   @ref T_GAP_CAUSE
*/
T_GAP_CAUSE ble_ext_adv_mgr_register_callback(P_FUN_GAP_APP_CB app_callback, uint8_t adv_handle);

/**
*@brief used to create a adv handle and initialized adv param, adv data, scan response data and random address.
* This api shall only be called after @ref ble_mgr_init() and shall only be called once.
* This api can not be used with @ref ble_ext_adv_mgr_init_adv_params_all() at the same time.
* That means in a adv set @ref ble_ext_adv_mgr_init_adv_params_all() and @ref ble_ext_adv_mgr_init_adv_params() can only chose one.
*@note
*@param[out] adv_handle  ble adv handle, each adv set has a adv handle
*@param[in]  adv_event_prop  @ref T_LE_EXT_ADV_LEGACY_ADV_PROPERTY
* @param[in] primary_adv_interval_min  Minimum advertising interval for undirected and low duty directed advertising.
*                                      In units of 0.625ms, range: 0x000020 to 0xFFFFFF.
* @param[in] primary_adv_interval_max  Maximum advertising interval for undirected and low duty directed advertising.
*                                      In units of 0.625ms, range: 0x000020 to 0xFFFFFF.
*@param[in]  own_address_type  @ref T_GAP_LOCAL_ADDR_TYPE
*@param[in]  peer_address_type  @ref T_GAP_REMOTE_ADDR_TYPE
*@param[in]  p_peer_address  if not used, set default value NULL
*@param[in]  filter_policy  @ref T_GAP_ADV_FILTER_POLICY
*@param[in]  adv_data_len  if you don't want to set adv data, set default length 0
*@param[in]  p_adv_data  ble ext adv mgr will not reallocate memory for adv data, so p_adv_data shall point to a global memory.
*                        if you don't want to set adv data, set default value NULL
*@param[in]  scan_data_len  if you don't want to set scan response data,set default length 0
*@param[in]  p_scan_data  ble ext adv mgr will not reallocate memory for scan response data, so p_scan_data shall point to a global memory.
*                         if you don't want to set scan response data,set default value NULL
*@param[in]  random_address  if you don't want to set random_address,set default value NULL
*@return     T_GAP_CAUSE
*
 * <b>Example usage</b>
 * \code{.c}
    void app_ble_common_adv_init(void)
    {   //app_ble_common_adv_data and scan_rsp_data shall point to a global memory.
        ble_ext_adv_mgr_init_adv_params(&le_common_adv.adv_handle, adv_event_prop, adv_interval_min,
                                    adv_interval_max, own_address_type, peer_address_type, peer_address,
                                    filter_policy, 23 + data_len, app_ble_common_adv_data,
                                    scan_rsp_data_len, scan_rsp_data, le_random_addr);

        ble_ext_adv_mgr_register_callback(app_ble_common_adv_callback, le_common_adv.adv_handle);
    }
 * \endcode
*/
T_GAP_CAUSE ble_ext_adv_mgr_init_adv_params(uint8_t *adv_handle, uint16_t adv_event_prop,
                                            uint32_t primary_adv_interval_min, uint32_t primary_adv_interval_max,
                                            T_GAP_LOCAL_ADDR_TYPE own_address_type, T_GAP_REMOTE_ADDR_TYPE peer_address_type,
                                            uint8_t *p_peer_address,
                                            T_GAP_ADV_FILTER_POLICY filter_policy, uint16_t adv_data_len, uint8_t *p_adv_data,
                                            uint16_t scan_data_len, uint8_t *p_scan_data, uint8_t *random_address);


/**
*@brief used to create adv handle and initial adv param, adv data, scan response data and random address.
* This api shall only be called after @ref ble_mgr_init() and shall only be called once.
* This api can not be used with @ref ble_ext_adv_mgr_init_adv_params() at the same time.
* That means in a adv set @ref ble_ext_adv_mgr_init_adv_params_all() and @ref ble_ext_adv_mgr_init_adv_params() can only chose one.
*@note
*@param[out] adv_handle  ble adv handle, each adv set has a adv handle
*@param[in]  adv_event_prop  @ref T_LE_EXT_ADV_LEGACY_ADV_PROPERTY
*@param[in]  adv_sid  Specify the value to be transmitted in Advertising SID subfield of those advertising channel
*@param[in]  tx_power Advertising Tx power.
* @param[in] primary_adv_interval_min  Minimum advertising interval for undirected and low duty directed advertising.
*                                      In units of 0.625ms, range: 0x000020 to 0xFFFFFF.
* @param[in] primary_adv_interval_max  Maximum advertising interval for undirected and low duty directed advertising.
*                                      In units of 0.625ms, range: 0x000020 to 0xFFFFFF.
*@param[in]  primary_adv_channel_map A bit field that indicates the advertising channels that shall be used when
                                              transmitting advertising packets. @ref ADV_CHANNEL_MAP.
*@param[in]  primary_adv_phy Indicate the PHY on which the advertising packets are transmitted on the primary advertising channel.
*@param[in]  secondary_adv_max_skip Maximum number of advertising events that can be skipped. Usually set to zero.
*@param[in]  secondary_adv_phy Indicate the PHY on which the advertising packets are transmitted on the secondary advertising channel.
*@param[in]  own_address_type  @ref T_GAP_LOCAL_ADDR_TYPE
*@param[in]  peer_address_type  @ref T_GAP_REMOTE_ADDR_TYPE
*@param[in]  p_peer_address  if not used, set default value NULL
*@param[in]  filter_policy  @ref T_GAP_ADV_FILTER_POLICY
*@param[in]  adv_data_len  if you don't want to set adv data,set default length 0
*@param[in]  p_adv_data  ble ext adv mgr will not reallocate memory for adv data, so p_adv_data shall point to a global memory.
*            if you don't want to set adv data,set default value NULL
*@param[in]  scan_data_len  if you don't want to set scan response data,set default length 0
*@param[in]  p_scan_data  ble ext adv mgr will not reallocate memory for scan response data, so p_scan_data shall point to a global memory.
*            if you don't want to set scan response data,set default value NULL
*@param[in]  random_address  if you don't want to set random_address,set default value NULL
*@return     T_GAP_CAUSE
*
 * <b>Example usage</b>
 * \code{.c}
    void app_ble_common_adv_init(void)
    {   //app_ble_common_adv_data and scan_rsp_data shall point to a global memory.
        ble_ext_adv_mgr_init_adv_params_all(&le_common_adv.adv_handle, adv_event_prop, adv_sid, tx_power,
                                            primary_adv_interval_min, primary_adv_interval_max,
                                            primary_adv_channel_map, primary_adv_phy,
                                            secondary_adv_max_skip, secondary_adv_phy,
                                            own_address_type, peer_address_type,
                                            p_peer_address,
                                            filter_policy, adv_data_len, p_adv_data,
                                            scan_data_len, p_scan_data, random_address);

        ble_ext_adv_mgr_register_callback(app_ble_common_adv_callback, le_common_adv.adv_handle);
    }
 * \endcode
*/
T_GAP_CAUSE ble_ext_adv_mgr_init_adv_params_all(uint8_t *adv_handle, uint16_t adv_event_prop,
                                                uint8_t adv_sid, uint8_t tx_power,
                                                uint32_t primary_adv_interval_min, uint32_t primary_adv_interval_max,
                                                uint8_t primary_adv_channel_map, T_GAP_PHYS_PRIM_ADV_TYPE primary_adv_phy,
                                                uint8_t secondary_adv_max_skip, T_GAP_PHYS_TYPE secondary_adv_phy,
                                                T_GAP_LOCAL_ADDR_TYPE own_address_type, T_GAP_REMOTE_ADDR_TYPE peer_address_type,
                                                uint8_t *p_peer_address,
                                                T_GAP_ADV_FILTER_POLICY filter_policy, uint16_t adv_data_len, uint8_t *p_adv_data,
                                                uint16_t scan_data_len, uint8_t *p_scan_data, uint8_t *random_address);


/**
 * @brief enable an advertising set.
 * @note  if you want use @ref ble_ext_adv_mgr_enable() to enable a adv set, please get ble adv mgr adv state firstly,
 *        and when ble adv mgr adv state is BLE_EXT_ADV_MGR_ADV_DISABLED, you can use @ref ble_ext_adv_mgr_enable() to enable this adv set;
 *        otherwise this adv set already in enabled state,there is no need to call this api again.
 * @param[in] adv_handle  Identify an advertising set, which is assigned by @ref ble_ext_adv_mgr_init_adv_params.
 * @param[in] duration  If non-zero, indicates the duration that advertising set is enabled.
                        0x0000:        No advertising duration.
                        0x0001-0xFFFF: Advertising duration, in units of 10ms.
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS  Operation success.
 * @retval GAP_CAUSE_NOT_FIND Operation failure, the advertising handle is not found.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint16_t duration = 0;
        if(ble_ext_adv_mgr_get_adv_state(adv_handle) == BLE_EXT_ADV_MGR_ADV_DISABLED)
        {
         ble_ext_adv_mgr_enable(adv_handle,duration);
        }
        else
        {
         APP_PRINT_INFO0("this adv set has already in enabled state");
        }
    }
 * \endcode
 */
T_GAP_CAUSE ble_ext_adv_mgr_enable(uint8_t adv_handle, uint16_t duration_10ms);

/**
 * @brief disable an advertising set.
 * @note: if you want use @ref ble_ext_adv_mgr_disable() to disable a adv set, please get ble adv mgr adv state firstly,
 *        and when ble adv mgr adv state is @ref BLE_EXT_ADV_MGR_ADV_ENABLED, you can use @ref ble_ext_adv_mgr_disable() to disable this adv set;
 *        otherwise this adv set already in disabled state,there is no need to call this api again.
 * @param[in] adv_handle  Identify an advertising set, which is assigned by @ref ble_ext_adv_mgr_init_adv_params.
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS  Operation success.
 * @retval GAP_CAUSE_NOT_FIND Operartion failure, the advertising handle is not found.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
       if(ble_ext_adv_mgr_get_adv_state(adv_handle) == BLE_EXT_ADV_MGR_ADV_ENABLED)
       {
        ble_ext_adv_mgr_disable(adv_handle,0xFF);
       }
       else
       {
        APP_PRINT_INFO0("this adv set has already in disabled state");
       }

    }
 * \endcode
 */
T_GAP_CAUSE ble_ext_adv_mgr_disable(uint8_t adv_handle, uint8_t app_cause);

/**
 * @brief disable all advertising set.
 * @note:
 * @param[in]   app_cause  the reason of disable all adv set
 * @return none
 *
 */
T_GAP_CAUSE ble_ext_adv_mgr_disable_all(uint8_t app_cause);

/**
 * @brief used to update adv data
 * this api shall be used after @ref ble_ext_adv_mgr_init_adv_params_all() or @ref ble_ext_adv_mgr_init_adv_params().
 *
 * @param[in] adv_handle  Identify an advertising set, which is assigned by @ref ble_ext_adv_mgr_init_adv_params.
 * @param[in] adv_data_len  adv data length
 * @param[in] p_adv_data  ble ext adv mgr will not reallocate memory for adv data, so p_adv_data shall point to a global memory.
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS  Operation success.
 * @retval GAP_CAUSE_NOT_FIND Operation failure, the advertising handle is not found.
 *
 */
T_GAP_CAUSE ble_ext_adv_mgr_set_adv_data(uint8_t adv_handle, uint16_t adv_data_len,
                                         uint8_t *p_adv_data);

/**
 * @brief used to update random address
 * this api shall be used after @ref ble_ext_adv_mgr_init_adv_params_all() or @ref ble_ext_adv_mgr_init_adv_params().
 *
 * @param[in] adv_handle  Identify an advertising set, which is assigned by @ref ble_ext_adv_mgr_init_adv_params.
 * @param[in] random_address
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS  Operation success.
 * @retval GAP_CAUSE_NOT_FIND Operation failure, the advertising handle is not found.
 *
 */
T_GAP_CAUSE ble_ext_adv_mgr_set_random(uint8_t adv_handle, uint8_t *random_address);

/**
 * @brief used to update scan response data
 * this api shall be used after @ref ble_ext_adv_mgr_init_adv_params_all() or @ref ble_ext_adv_mgr_init_adv_params().
 *
 * @param[in] adv_handle  Identify an advertising set, which is assigned by @ref ble_ext_adv_mgr_init_adv_params.
 * @param[in] scan_data_len  scan response data length
 * @param[in] p_scan_data  ble ext adv mgr will not reallocate memory for scan response data, so p_scan_data shall point to a global memory.
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS  Operation success.
 * @retval GAP_CAUSE_NOT_FIND Operation failure, the advertising handle is not found.
 *
 */
T_GAP_CAUSE ble_ext_adv_mgr_set_scan_response_data(uint8_t adv_handle, uint16_t scan_data_len,
                                                   uint8_t *p_scan_data);

/**
 * @brief used to set random_address, adv interval, adv data and scan response data.
 * this api shall be used after @ref ble_ext_adv_mgr_init_adv_params_all() or @ref ble_ext_adv_mgr_init_adv_params().
 *
 * @param adv_handle  ble adv handle, each adv set has a adv handle
 * @param random_address
 * @param adv_interval
 * @param[in] adv_data_len  adv data length
 * @param[in] p_adv_data  ble ext adv mgr will not reallocate memory for adv data, so p_adv_data shall point to a global memory.
 * @param[in] scan_data_len  scan response data length
 * @param[in] p_scan_data  ble ext adv mgr will not reallocate memory for scan response data, so p_scan_data shall point to a global memory.
 * @return T_GAP_CAUSE
 */
T_GAP_CAUSE ble_ext_adv_mgr_set_multi_param(uint8_t adv_handle, uint8_t *random_address,
                                            uint16_t adv_interval,
                                            uint16_t adv_data_len, uint8_t *p_adv_data, uint16_t scan_data_len,
                                            uint8_t *p_scan_data);

/**
*@brief used to update adv interval
*this api shall be used after @ref ble_ext_adv_mgr_init_adv_params_all() or @ref ble_ext_adv_mgr_init_adv_params().
*
*@param[in] adv_handle ble adv handle, each adv set has a adv handle
*@param[in] adv_interval
*@return    T_GAP_CAUSE
* @retval   GAP_CAUSE_SUCCESS  Operation success.
* @retval   GAP_CAUSE_NOT_FIND Operation failure, the advertising handle is not found.
*/
T_GAP_CAUSE ble_ext_adv_mgr_change_adv_interval(uint8_t adv_handle, uint16_t adv_interval);

/**
*@brief used to update adv tx power
*this api shall be used after @ref ble_ext_adv_mgr_init_adv_params_all() or @ref ble_ext_adv_mgr_init_adv_params().
*@note
*@param[in] adv_handle  ble adv handle, each adv set has a adv handle
*@param[in] adv_tx_power  Range: -127 to +20, Units: dBm, 0x7F Host has no preference, the default value is 0x7F
*@return    T_GAP_CAUSE
* @retval   GAP_CAUSE_SUCCESS  Operation success.
* @retval   GAP_CAUSE_NOT_FIND  Operation failure, the advertising handle is not found.
*/
T_GAP_CAUSE ble_ext_adv_mgr_change_adv_tx_power(uint8_t adv_handle, int8_t adv_tx_power);

/**
*@brief used to update adv_event_prop
*this api shall be used after @ref ble_ext_adv_mgr_init_adv_params_all() or @ref ble_ext_adv_mgr_init_adv_params().
*@note
*@param[in] adv_handle  ble adv handle, each adv set has a adv handle
*@param[in] adv_event_prop  @ref T_LE_EXT_ADV_LEGACY_ADV_PROPERTY
*@return    T_GAP_CAUSE
* @retval   GAP_CAUSE_SUCCESS  Operation success.
* @retval   GAP_CAUSE_NOT_FIND  Operation failure, the advertising handle is not found.
*/
T_GAP_CAUSE ble_ext_adv_mgr_change_adv_event_prop(uint8_t adv_handle, uint16_t adv_event_prop);

/**
*@brief used to update primary and secondary adv phy
*this api shall be used after @ref ble_ext_adv_mgr_init_adv_params_all() or @ref ble_ext_adv_mgr_init_adv_params().
*@note
*@param[in] adv_handle  ble adv handle, each adv set has a adv handle
*@param[in] primary_adv_phy  @ref T_GAP_PHYS_PRIM_ADV_TYPE
*@param[in] secondary_adv_phy  @ref T_GAP_PHYS_TYPE
*@return    T_GAP_CAUSE
* @retval   GAP_CAUSE_SUCCESS  Operation success.
* @retval   GAP_CAUSE_NOT_FIND  Operation failure, the advertising handle is not found.
*/
T_GAP_CAUSE ble_ext_adv_mgr_change_adv_phy(uint8_t adv_handle,
                                           T_GAP_PHYS_PRIM_ADV_TYPE primary_adv_phy, T_GAP_PHYS_TYPE secondary_adv_phy);

/**
*@brief used to update peer addr
*this api shall be used after @ref ble_ext_adv_mgr_init_adv_params_all() or @ref ble_ext_adv_mgr_init_adv_params().
*@note
*\xrefitem Added_API_2_11_1_0 "Added Since 2.11.1.0" "Added API"
*@param[in] adv_handle  ble adv handle, each adv set has a adv handle
*@param[in] peer_addr_type  @ref T_GAP_REMOTE_ADDR_TYPE
*@param[in] peer_addr
*@return    T_GAP_CAUSE
* @retval   GAP_CAUSE_SUCCESS  Operation success.
* @retval   GAP_CAUSE_NOT_FIND  Operation failure, the advertising handle is not found.
*/
T_GAP_CAUSE ble_ext_adv_mgr_change_peer_addr(uint8_t adv_handle,
                                             T_GAP_REMOTE_ADDR_TYPE peer_addr_type, uint8_t *peer_addr);

/**
 * @brief ble_ext_adv_mgr_change_channel_map
 * used to update primary_adv_channel_map
 * this api shall be used after @ref ble_ext_adv_mgr_init_adv_params_all() or @ref ble_ext_adv_mgr_init_adv_params().
 *@note
 *\xrefitem Added_API_2_12_1_0 "Added Since 2.12.1.0" "Added API"
 * @param adv_handle ble adv handle, each adv set has a adv handle.
 * @param primary_adv_channel_map @ref GAP_ADVCHAN_ALL or @ref GAP_ADVCHAN_37 or @ref GAP_ADVCHAN_38 or @ref GAP_ADVCHAN_39
 * @return T_GAP_CAUSE
 */
T_GAP_CAUSE ble_ext_adv_mgr_change_channel_map(uint8_t adv_handle, uint8_t primary_adv_channel_map);

/**
 * @brief ble_ext_adv_mgr_change_filter_policy
 * used to update filter policy
 * this api shall be used after @ref ble_ext_adv_mgr_init_adv_params_all() or @ref ble_ext_adv_mgr_init_adv_params().
 *@note
 *\xrefitem Added_API_2_12_1_0 "Added Since 2.12.1.0" "Added API"
 * @param adv_handle ble adv handle, each adv set has a adv handle.
 * @param filter_policy @ref T_GAP_ADV_FILTER_POLICY
 * @return T_GAP_CAUSE
 */
T_GAP_CAUSE ble_ext_adv_mgr_change_filter_policy(uint8_t adv_handle,
                                                 T_GAP_ADV_FILTER_POLICY filter_policy);

/**
*@brief used to get ble_ext_adv_mgr_adv_state @ref T_BLE_EXT_ADV_MGR_STATE
*@note
*@param[in] adv_handle  ble adv handle, each adv set has a adv handle
*@return    T_BLE_EXT_ADV_MGR_STATE @ref T_BLE_EXT_ADV_MGR_STATE
*/
T_BLE_EXT_ADV_MGR_STATE ble_ext_adv_mgr_get_adv_state(uint8_t adv_handle);

/**
*@brief used to get current used maximum advertising interval
*@note
*@param[in] adv_handle  ble adv handle, each adv set has a adv handle
*@return    uint16_t
*@retval    0 Get the interval failed
*@retval    0x0020 - 0x4000 the current used maximum adv interval
*/
uint16_t ble_ext_adv_mgr_get_adv_interval(uint8_t adv_handle);

/**
 * @brief print adv set information
 *
 */
void ble_ext_adv_print_info(void);

/**
*@brief used to determine if there is a BLE advertising is ongoing
*@note
*@param[in] void
*@return    bool
*@retval    true  BLE advertising is ongoing
*@retval    false  BLE advertising is not ongoing
*/
bool ble_ext_adv_is_ongoing(void);

/** @} */ /* End of group APP_DEVICE_Exported_Functions */
/** End of BLE_ADV
* @}
*/

#ifdef __cplusplus
}
#endif

#endif
