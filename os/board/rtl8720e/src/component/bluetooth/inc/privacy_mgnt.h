/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     privacy_mgnt.h
  * @brief    privacy management module.
  * @details  privacy management module.
  * @author   jane
  * @date     2018-06-19
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PRIVACY_MGNT_H_
#define _PRIVACY_MGNT_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <gap_bond_le.h>

/** @defgroup BLE_PRIV_MODULE BLE Privacy Management Module
  * @brief Application uses this module to handle privacy procedures.
  * @{
  */
/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup BLE_PRIV_MODULE_Exported_Types BLE Privacy Management Module Exported Types
  * @{
  */
/** @brief Privacy management module callback type*/
typedef enum
{
    PRIVACY_STATE_MSGTYPE,                 //!< Privacy management module state.
    PRIVACY_RESOLUTION_STATUS_MSGTYPE,     //!< Response msg type for privacy_set_addr_resolution
    PRIVACY_READ_PEER_RESOLV_ADDR_MSGTYPE, //!< Response msg type for privacy_read_peer_resolv_addr
    PRIVACY_READ_LOCAL_RESOLV_ADDR_MSGTYPE,//!< Response msg type for privacy_read_local_resolv_addr
    PRIVACY_GEN_PRIV_ADDR_INTERVAL_MSGTYPE,//!< Response msg type for privacy_set_gen_priv_addr_interval
    PRIVACY_SET_PEER_MODE_MSGTYPE,
} T_PRIVACY_CB_TYPE;

/** @brief Privacy management module state*/
typedef enum
{
    PRIVACY_STATE_INIT,  //!< Privacy management module is not initialization.
    PRIVACY_STATE_IDLE,  //!< Idle. No pending resolving list modification procedure.
    PRIVACY_STATE_BUSY   //!< Busy. Resolving list modification procedure is not completed.
} T_PRIVACY_STATE;

/** @brief Define the privacy address resolution state */
typedef enum
{
    PRIVACY_ADDR_RESOLUTION_DISABLED,
    PRIVACY_ADDR_RESOLUTION_DISABLING,
    PRIVACY_ADDR_RESOLUTION_ENABLING,
    PRIVACY_ADDR_RESOLUTION_ENABLED
} T_PRIVACY_ADDR_RESOLUTION_STATE;

/** @brief Callback data of PRIVACY_READ_PEER_RESOLV_ADDR_MSGTYPE or PRIVACY_READ_LOCAL_RESOLV_ADDR_MSGTYPE*/
typedef struct
{
    uint16_t             cause;
    uint8_t              resolv_addr[6];
} T_PRIVACY_READ_RESOLV_ADDR_RSP;

/** @brief Privacy management module callback data  */
typedef union
{
    uint16_t                        cause; //!< Callback data of PRIVACY_GEN_PRIV_ADDR_INTERVAL_MSGTYPE
    T_PRIVACY_STATE                 privacy_state; //!< Callback data of PRIVACY_STATE_MSGTYPE
    T_PRIVACY_ADDR_RESOLUTION_STATE
    resolution_state; //!< Callback data of PRIVACY_RESOLUTION_STATUS_MSGTYPE
    T_PRIVACY_READ_RESOLV_ADDR_RSP  *p_privacy_read_resolv_addr_rsp;
} T_PRIVACY_CB_DATA;
/** End of BLE_PRIV_MODULE_Exported_Types
  * @}
  */
/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * @defgroup BLE_PRIV_MODULE_EXPORT_Functions BLE Privacy Management Module Exported Functions
 *
 * @{
 */
/**
  * @brief Callback for privacy management module to notify app
  * @param[in] type     callback msy type @ref T_PRIVACY_CB_TYPE.
  * @param[in] cb_data  callback data @ref T_PRIVACY_CB_DATA.
  * @retval void
  */
typedef void(*P_FUN_PRIVACY_STATE_CB)(T_PRIVACY_CB_TYPE type, T_PRIVACY_CB_DATA cb_data);

/**
 * @brief  Initialize privacy management module.
 * @param[in] p_fun     Callback function provided by the APP to handle privacy messages sent from the privacy management module.
 * @param[in] whitelist Whether manage the white list when modify the resolving list.
 * @return none
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_le_gap_init(void)
    {
        ......
        privacy_init(app_privacy_callback, true);
    }
    void app_privacy_callback(T_PRIVACY_CB_TYPE type, T_PRIVACY_CB_DATA cb_data)
    {
        APP_PRINT_INFO1("app_privacy_callback: type %d", type);
        switch (type)
        {
        case PRIVACY_STATE_MSGTYPE:
            app_privacy_state = cb_data.privacy_state;
            break;

        case PRIVACY_RESOLUTION_STATUS_MSGTYPE:
            app_privacy_resolution_state = cb_data.resolution_state;
            break;

        default:
            break;
        }
    }
 * \endcode
 */
void privacy_init(P_FUN_PRIVACY_STATE_CB p_fun, bool whitelist);

/**
 * @brief  Handle the pending resolving list modification procedure.
 *
 * Application shall call this funtion when the device state is in the idle state.
 *
 * @return The current privacy management module state.
 * @retval T_PRIVACY_STATE module state
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
    {
        APP_PRINT_INFO3("app_handle_dev_state_evt: init state %d, adv state %d, cause 0x%x",
                        new_state.gap_init_state, new_state.gap_adv_state, cause);

        if ((new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
            && (new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
            && (new_state.gap_conn_state == GAP_CONN_DEV_STATE_IDLE))
        {
            privacy_handle_resolv_list();
        }

        if (gap_dev_state.gap_init_state != new_state.gap_init_state)
        {
            if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
            {
                APP_PRINT_INFO0("GAP stack ready");
                app_adv_start();
            }
        }
        ......
    }
 * \endcode
 */
T_PRIVACY_STATE privacy_handle_resolv_list(void);

/**
 * @brief  Hande the GAP_MSG_LE_BOND_MODIFY_INFO message.
 *
 * Application shall call this funtion to handle the message GAP_MSG_LE_BOND_MODIFY_INFO.
 *
 * @param[in] type        Bond modification type @ref T_LE_BOND_MODIFY_TYPE.
 * @param[in] p_entry     The key entry of the modified device.
 * @param[in] handle_add  Whether handle the type @ref LE_BOND_ADD.
 * @return none
 *
 * <b>Example usage</b>
 * \code{.c}
    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        switch (cb_type)
        {
        ......
        case GAP_MSG_LE_BOND_MODIFY_INFO:
            APP_PRINT_INFO1("GAP_MSG_LE_BOND_MODIFY_INFO: type 0x%x",
                            p_data->p_le_bond_modify_info->type);
            privacy_handle_bond_modify_msg(p_data->p_le_bond_modify_info->type,
                                           p_data->p_le_bond_modify_info->p_entry, true);
            break;

        default:
            APP_PRINT_ERROR1("app_gap_callback: unhandled cb_type 0x%x", cb_type);
            break;
        }
        return result;
    }
 * \endcode
 */
void privacy_handle_bond_modify_msg(T_LE_BOND_MODIFY_TYPE type, T_LE_KEY_ENTRY *p_entry,
                                    bool handle_add);

/**
 * @brief  Add the device to the resolving list.
 *
 * Application can call this funcation when the parameter handle_add of the privacy_handle_bond_modify_msg is false.
 *
 * @param[in] p_entry The key entry of the device.
 * @return result
 * @retval true  Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        ......
        switch (cb_type)
        {
        ......
        case GAP_MSG_LE_BOND_MODIFY_INFO:
            APP_PRINT_INFO1("GAP_MSG_LE_BOND_MODIFY_INFO: type 0x%x",
                            p_data->p_le_bond_modify_info->type);
            privacy_handle_bond_modify_msg(p_data->p_le_bond_modify_info->type,
                                           p_data->p_le_bond_modify_info->p_entry, false);
            break;
        }
        return result;
    }
    bool app_save_privacy_info(uint8_t conn_id)
    {
        if (app_link_table[conn_id].p_entry != NULL)
        {
            le_set_privacy_info(app_link_table[conn_id].p_entry, &app_link_table[conn_id].privacy_info);
            privacy_add_device(app_link_table[conn_id].p_entry);
            privacy_handle_resolv_list();
        }
        return true;
    }
 * \endcode
 */
bool privacy_add_device(T_LE_KEY_ENTRY *p_entry);

/**
 * @brief  Enable/disable le privacy address resolution mode.
 * @param[in] enable Enable or disable address resolution.
 * @return Operation result
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_adv_start(void)
    {
        uint8_t  adv_evt_type = GAP_ADTYPE_ADV_IND;
        uint8_t  adv_filter_policy = GAP_ADV_FILTER_ANY;
        T_LE_KEY_ENTRY *p_entry;
        p_entry = le_get_high_priority_bond();

        if (p_entry == NULL)
        {
            app_work_mode = APP_PAIRABLE_MODE;
            adv_filter_policy = GAP_ADV_FILTER_ANY;
            if (app_privacy_resolution_state == PRIVACY_ADDR_RESOLUTION_ENABLED)
            {
                privacy_set_addr_resolution(false);
            }
        }
        ......
    }
    void app_privacy_callback(T_PRIVACY_CB_TYPE type, T_PRIVACY_CB_DATA cb_data)
    {
        APP_PRINT_INFO1("app_privacy_callback: type %d", type);
        switch (type)
        {
        case PRIVACY_RESOLUTION_STATUS_MSGTYPE:
            app_privacy_resolution_state = cb_data.resolution_state;
            break;

        ......
        }
    }
 * \endcode
 */
T_GAP_CAUSE privacy_set_addr_resolution(bool enable);

/**
 * @brief  Read peer resolvable random address.
 * @param[in] peer_address_type Peer address type.
 * @param[in] peer_address      Peer address.
 * @return Operation result
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_readpra(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t idx = p_parse_value->dw_param[0];
        T_GAP_CAUSE cause;
        T_LE_KEY_ENTRY *p_entry;
        p_entry = le_find_key_entry_by_idx(idx);
        if (p_entry)
        {
            cause = privacy_read_peer_resolv_addr((T_GAP_REMOTE_ADDR_TYPE)p_entry->remote_bd.remote_bd_type,
                                                  p_entry->remote_bd.addr);
            return (T_USER_CMD_PARSE_RESULT)cause;
        }
        else
        {
            return RESULT_ERR;
        }
    }
    void app_privacy_callback(T_PRIVACY_CB_TYPE type, T_PRIVACY_CB_DATA cb_data)
    {
        APP_PRINT_INFO1("app_privacy_callback: type %d", type);
        switch (type)
        {
        case PRIVACY_READ_PEER_RESOLV_ADDR_MSGTYPE:
            APP_PRINT_INFO2("PRIVACY_READ_PEER_RESOLV_ADDR_MSGTYPE: cause 0x%x, addr %b",
                            cb_data.p_privacy_read_resolv_addr_rsp->cause,
                            TRACE_BDADDR(cb_data.p_privacy_read_resolv_addr_rsp->resolv_addr));
            break;

        default:
            break;
        }
    }
 * \endcode
 */
T_GAP_CAUSE privacy_read_peer_resolv_addr(T_GAP_REMOTE_ADDR_TYPE peer_address_type,
                                          uint8_t *peer_address);

/**
 * @brief  Read local resolvable random address.
 * @param[in] peer_address_type Peer address type.
 * @param[in] peer_address      Peer address.
 * @return Operation result
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_readlra(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t idx = p_parse_value->dw_param[0];
        T_GAP_CAUSE cause;
        T_LE_KEY_ENTRY *p_entry;
        p_entry = le_find_key_entry_by_idx(idx);
        if (p_entry)
        {
            cause = privacy_read_local_resolv_addr((T_GAP_REMOTE_ADDR_TYPE)p_entry->remote_bd.remote_bd_type,
                                                   p_entry->remote_bd.addr);
            return (T_USER_CMD_PARSE_RESULT)cause;
        }
        else
        {
            return RESULT_ERR;
        }
    }
    void app_privacy_callback(T_PRIVACY_CB_TYPE type, T_PRIVACY_CB_DATA cb_data)
    {
        APP_PRINT_INFO1("app_privacy_callback: type %d", type);
        switch (type)
        {
        case PRIVACY_READ_LOCAL_RESOLV_ADDR_MSGTYPE:
            APP_PRINT_INFO2("PRIVACY_READ_LOCAL_RESOLV_ADDR_MSGTYPE: cause 0x%x, addr %b",
                            cb_data.p_privacy_read_resolv_addr_rsp->cause,
                            TRACE_BDADDR(cb_data.p_privacy_read_resolv_addr_rsp->resolv_addr));
            break;

        default:
            break;
        }
    }
 * \endcode
 */
T_GAP_CAUSE privacy_read_local_resolv_addr(T_GAP_REMOTE_ADDR_TYPE peer_address_type,
                                           uint8_t *peer_address);

/**
 * @brief  Set privacy address generation interavl.
 * @param[in] interval Privacy address generation interavl.
 *                    Range:0x0001 - 0xA1B8(1s/step). Default value is 0x0384.
 * @return Operation result
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint16_t privacy_timeout = 0x384;
        privacy_set_gen_priv_addr_interval(privacy_timeout);
    }
    void app_privacy_callback(T_PRIVACY_CB_TYPE type, T_PRIVACY_CB_DATA cb_data)
    {
        APP_PRINT_INFO1("app_privacy_callback: type %d", type);
        switch (type)
        {
        case PRIVACY_GEN_PRIV_ADDR_INTERVAL_MSGTYPE:
            APP_PRINT_INFO1("PRIVACY_GEN_PRIV_ADDR_INTERVAL_MSGTYPE: cause 0x%x", cb_data.cause);
            break;

        default:
            break;
        }
    }
 * \endcode
 */
T_GAP_CAUSE privacy_set_gen_priv_addr_interval(uint16_t interval);

/**
 * @brief  Configure privacy management module whether manage privacy mode when add device to the resolving list.
 * @param[in] is_manage Whether manage privacy mode.
 * @return None
 */
void privacy_manage_mode(bool is_manage);

/**
 * @brief  Configure privacy mode.
 *
 * The default mode is network privacy mode.
 * Application can call this funcation when the parameter is_manage of the privacy_manage_mode() is false.
 *
 * @param[in] peer_address_type Peer address type.
 * @param[in] peer_address      Peer address.
 * @param[in] device_mode       Configure the privacy mode.
 *                              false - Network Privacy mode
 *                              true - Device Privacy mode
 * @return Operation result
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_le_gap_init(void)
    {
        ......
        privacy_manage_mode(false);
        privacy_init(app_privacy_callback, true);
    }
    static T_USER_CMD_PARSE_RESULT cmd_setprivacy(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t idx = p_parse_value->dw_param[0];
        bool mode = p_parse_value->dw_param[1];
        T_LE_KEY_ENTRY *p_entry;
        T_GAP_CAUSE cause;
        p_entry = le_find_key_entry_by_idx(idx);
        if (p_entry != NULL)
        {
            cause = privacy_set_peer_mode((T_GAP_REMOTE_ADDR_TYPE)p_entry->remote_bd.remote_bd_type,
                                          p_entry->remote_bd.addr,
                                          mode);
            return (T_USER_CMD_PARSE_RESULT)cause;
        }
        else
        {
            return RESULT_CMD_ERR_PARAM;
        }
    }
    void app_privacy_callback(T_PRIVACY_CB_TYPE type, T_PRIVACY_CB_DATA cb_data)
    {
        APP_PRINT_INFO1("app_privacy_callback: type %d", type);
        switch (type)
        {
        case PRIVACY_SET_PEER_MODE_MSGTYPE:
            APP_PRINT_INFO1("PRIVACY_SET_PEER_MODE_MSGTYPE: cause 0x%x", cb_data.cause);
            break;

        default:
            break;
        }
    }
 * \endcode
 */
T_GAP_CAUSE privacy_set_peer_mode(T_GAP_REMOTE_ADDR_TYPE peer_address_type,
                                  uint8_t *peer_address, bool device_mode);


/** @} */ /* End of group BLE_PRIV_MODULE_EXPORT_Functions */
/** @} */ /* End of group BLE_PRIV_MODULE */
#ifdef __cplusplus
}
#endif

#endif /* _PRIVACY_MGNT_H_ */
