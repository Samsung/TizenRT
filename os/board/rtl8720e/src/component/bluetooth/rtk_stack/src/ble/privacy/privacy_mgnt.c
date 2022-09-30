/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     privacy_mgnt.c
* @brief    privacy1.2 management file.
* @details  Demonstration of how to implement privacy.
* @author
* @date     2016-02-18
* @version  v0.1
*********************************************************************************************************
*/
#include <trace.h>
#include <string.h>
#include <gap.h>
#include <privacy_mgnt.h>
#include <os_mem.h>
#include <gap_privacy.h>
#include <trace_app.h>

typedef enum
{
    PRIVACY_RESOLVING_LIST_IDLE,
    PRIVACY_RESOLVING_LIST_ADD_PENDING,
    PRIVACY_RESOLVING_LIST_REMOVE_PENDING,
} T_PRIVACY_MODIFY_STATE;

typedef enum
{
    PRIVACY_RESOLVING_ADDED_IDLE,
    PRIVACY_RESOLVING_ADDED_PENDING,
    PRIVACY_RESOLVING_ADDED,
} T_PRIVACY_ADDED_STATE;

typedef struct
{
    bool is_used;
    T_PRIVACY_ADDED_STATE  is_added;
    T_PRIVACY_MODIFY_STATE state;
    bool device_mode;
    T_GAP_IDENT_ADDR_TYPE  remote_bd_type;
    uint8_t  addr[6];
} T_LE_PRIVACY_ENTRY, *P_LE_PRIVACY_ENTRY;

T_LE_PRIVACY_ENTRY *privacy_table;
bool privacy_clear_pending = false;
bool privacy_modify_resolv_list_pending = false;
uint8_t privacy_modify_resolv_list_idx = 0xff;
T_PRIVACY_STATE privacy_state = PRIVACY_STATE_INIT;
T_LE_PRIVACY_STATE privacy_resolution_status = LE_PRIVACY_RESOLUTION_DISABLED;
bool privacy_whitelist = false;
bool privacy_mode_manage = true;
static P_FUN_PRIVACY_STATE_CB privacy_app_cb = NULL;

extern T_GAP_DEV_STATE gap_dev_state;

void privacy_modify_resolving_list(T_GAP_RESOLV_LIST_OP op, T_GAP_IDENT_ADDR_TYPE addr_type,
                                   uint8_t *addr, bool device_mode);

T_APP_RESULT privacy_msg_callback(uint8_t msg_type, T_LE_PRIVACY_CB_DATA msg_data);

void privacy_change_state(T_PRIVACY_STATE state)
{
    if (privacy_state != state)
    {

        privacy_state = state;
        if (privacy_app_cb)
        {
            T_PRIVACY_CB_DATA cb_data;
            cb_data.privacy_state = privacy_state;
            privacy_app_cb(PRIVACY_STATE_MSGTYPE, cb_data);
        }
    }
}

bool privacy_add_device(T_LE_KEY_ENTRY *p_entry)
{
    if (p_entry != NULL && p_entry->is_used)
    {
        bool device_mode = true;
        T_LE_PRIVACY_INFO privacy_info;
        if (le_get_privacy_info(p_entry, &privacy_info))
        {
            if (privacy_info.is_discov && privacy_info.resolv_addr_only)
            {
                device_mode = false;
            }
        }
        if ((p_entry->remote_bd.remote_bd_type == GAP_REMOTE_ADDR_LE_PUBLIC) ||
            ((p_entry->remote_bd.remote_bd_type == GAP_REMOTE_ADDR_LE_RANDOM) &&
             ((p_entry->remote_bd.addr[5] & RANDOM_ADDR_MASK) == RANDOM_ADDR_MASK_STATIC))
           )
        {
            privacy_modify_resolving_list(GAP_RESOLV_LIST_OP_ADD,
                                          (T_GAP_IDENT_ADDR_TYPE)p_entry->remote_bd.remote_bd_type,
                                          p_entry->remote_bd.addr, device_mode);
            return true;
        }
        else
        {
            if (p_entry->flags & LE_KEY_STORE_REMOTE_IRK_BIT)
            {
                privacy_modify_resolving_list(GAP_RESOLV_LIST_OP_ADD,
                                              (T_GAP_IDENT_ADDR_TYPE)p_entry->resolved_remote_bd.remote_bd_type,
                                              p_entry->resolved_remote_bd.addr, device_mode);
                return true;
            }
            else
            {
                APP_PRINT_ERROR1("[PRIVACY] privacy_add_device: failed, idx %d", p_entry->idx);
            }
        }
    }
    return false;
}

void privacy_init(P_FUN_PRIVACY_STATE_CB privacy_callback, bool whitelist)
{
    uint8_t i;
    uint16_t size;
    T_LE_KEY_ENTRY *p_entry;
    uint8_t bond_storage_num = le_get_max_le_paired_device_num();

    APP_PRINT_INFO1("[PRIVACY] privacy_init: whitelist %d", whitelist);
    size = bond_storage_num * sizeof(T_LE_PRIVACY_ENTRY);
    privacy_table = os_mem_zalloc(RAM_TYPE_DATA_ON, size);
    privacy_app_cb = privacy_callback;
    privacy_whitelist = whitelist;
    le_privacy_register_cb(privacy_msg_callback);

    privacy_change_state(PRIVACY_STATE_IDLE);

    for (i = 0; i < bond_storage_num; i++)
    {
        p_entry = le_find_key_entry_by_idx(i);
        privacy_add_device(p_entry);
    }
    return;
}

T_PRIVACY_STATE privacy_handle_resolv_list_int(void)
{
    uint8_t i;
    T_GAP_CAUSE cause;
    T_PRIVACY_STATE state = PRIVACY_STATE_BUSY;
    uint8_t bond_storage_num = le_get_max_le_paired_device_num();

    if (privacy_modify_resolv_list_pending)
    {
        APP_PRINT_INFO0("[PRIVACY] wait rsp");
        return state;
    }
    if (privacy_clear_pending)
    {
        APP_PRINT_INFO0("[PRIVACY] clear");
        cause = le_privacy_modify_resolv_list(GAP_RESOLV_LIST_OP_CLEAR, GAP_IDENT_ADDR_PUBLIC, NULL);
        if (cause == GAP_CAUSE_SUCCESS)
        {
            if (privacy_whitelist)
            {
                le_modify_white_list(GAP_WHITE_LIST_OP_CLEAR, NULL, GAP_REMOTE_ADDR_LE_PUBLIC);
            }
            privacy_modify_resolv_list_pending = true;
            return state;
        }
        else
        {
            APP_PRINT_ERROR1("[PRIVACY] clear failed: cause %d", cause);
            return state;
        }
    }
    for (i = 0; i < bond_storage_num; i++)
    {
        if (privacy_table[i].is_used && privacy_table[i].state != PRIVACY_RESOLVING_LIST_IDLE)
        {
            if (privacy_table[i].state == PRIVACY_RESOLVING_LIST_ADD_PENDING)
            {

                APP_PRINT_INFO3("[PRIVACY] Add: i %d, BD %s, type %d\n", i,
                                TRACE_BDADDR(privacy_table[i].addr),
                                privacy_table[i].remote_bd_type);
                cause = le_privacy_modify_resolv_list(GAP_RESOLV_LIST_OP_ADD,
                                                      privacy_table[i].remote_bd_type,
                                                      privacy_table[i].addr);
                if (cause == GAP_CAUSE_SUCCESS)
                {
                    privacy_modify_resolv_list_pending = true;
                    privacy_modify_resolv_list_idx = i;
                    privacy_table[i].is_added = PRIVACY_RESOLVING_ADDED_PENDING;
                    if (privacy_whitelist)
                    {
                        le_modify_white_list(GAP_WHITE_LIST_OP_ADD, privacy_table[i].addr,
                                             (T_GAP_REMOTE_ADDR_TYPE)privacy_table[i].remote_bd_type);
                    }
                    if (privacy_table[i].device_mode && privacy_mode_manage)
                    {
                        le_privacy_set_mode(privacy_table[i].remote_bd_type, privacy_table[i].addr,
                                            GAP_PRIVACY_MODE_DEVICE);
                    }
                    return state;
                }
                else if (cause != GAP_CAUSE_INVALID_STATE)
                {
                    APP_PRINT_ERROR2("[PRIVACY] Add failed: cause %d, remove i=%d from pend add\n", cause, i);
                    memset(&privacy_table[i], 0, sizeof(T_LE_PRIVACY_ENTRY));
                }
                else
                {
                    APP_PRINT_ERROR0("[PRIVACY] Add failed: invalid state");
                    return state;
                }
            }
            else
            {
                APP_PRINT_INFO3("[PRIVACY] Remove: i %d, BD %s, type %d", i,
                                TRACE_BDADDR(privacy_table[i].addr),
                                privacy_table[i].remote_bd_type);
                cause = le_privacy_modify_resolv_list(GAP_RESOLV_LIST_OP_REMOVE,
                                                      privacy_table[i].remote_bd_type,
                                                      privacy_table[i].addr);
                if (cause == GAP_CAUSE_SUCCESS)
                {
                    privacy_modify_resolv_list_pending = true;
                    privacy_modify_resolv_list_idx = i;
                    if (privacy_whitelist)
                    {
                        le_modify_white_list(GAP_WHITE_LIST_OP_REMOVE, privacy_table[i].addr,
                                             (T_GAP_REMOTE_ADDR_TYPE)privacy_table[i].remote_bd_type);
                    }
                    return state;
                }
                else if (cause != GAP_CAUSE_INVALID_STATE)
                {
                    APP_PRINT_ERROR2("[PRIVACY] Remove failed: cause %d, remove i=%d from pend add\n", cause, i);
                    memset(&privacy_table[i], 0, sizeof(T_LE_PRIVACY_ENTRY));
                }
                else
                {
                    APP_PRINT_ERROR0("[PRIVACY] Remove failed: invalid state");
                    return state;
                }
            }
        }
    }
    APP_PRINT_INFO0("[PRIVACY] privacy_handle_resolv_list_int: idle");
    state = PRIVACY_STATE_IDLE;
    return state;
}

T_PRIVACY_STATE privacy_handle_resolv_list(void)
{
    T_GAP_DEV_STATE dev_state;
    le_get_gap_param(GAP_PARAM_DEV_STATE, &dev_state);
    if (privacy_resolution_status == LE_PRIVACY_RESOLUTION_DISABLED
        || ((dev_state.gap_adv_state == GAP_ADV_STATE_IDLE)
            && (dev_state.gap_conn_state == GAP_CONN_DEV_STATE_IDLE)))
    {
        T_PRIVACY_STATE state = privacy_handle_resolv_list_int();
        privacy_change_state(state);
    }
    return privacy_state;
}

void privacy_handle_bond_modify_msg(T_LE_BOND_MODIFY_TYPE type, T_LE_KEY_ENTRY *p_entry,
                                    bool handle_add)
{
    APP_PRINT_INFO1("[PRIVACY] privacy_handle_bond_modify_msg: type 0x%x", type);

    if (type == LE_BOND_CLEAR)
    {
        privacy_modify_resolving_list(GAP_RESOLV_LIST_OP_CLEAR, GAP_IDENT_ADDR_PUBLIC, NULL, false);
    }
    else if (type == LE_BOND_DELETE)
    {
        if (p_entry->flags & LE_KEY_STORE_REMOTE_IRK_BIT)
        {
            privacy_modify_resolving_list(GAP_RESOLV_LIST_OP_REMOVE,
                                          (T_GAP_IDENT_ADDR_TYPE)p_entry->resolved_remote_bd.remote_bd_type,
                                          p_entry->resolved_remote_bd.addr, false);
        }
        else
        {
            privacy_modify_resolving_list(GAP_RESOLV_LIST_OP_REMOVE,
                                          (T_GAP_IDENT_ADDR_TYPE)p_entry->remote_bd.remote_bd_type,
                                          p_entry->remote_bd.addr, false);
        }
    }
    else if (type == LE_BOND_ADD)
    {
        if (handle_add)
        {
            privacy_add_device(p_entry);
        }
    }
    privacy_handle_resolv_list();
}

T_GAP_CAUSE privacy_set_addr_resolution(bool enable)
{
    return le_privacy_set_addr_resolution(enable);
}

T_GAP_CAUSE privacy_read_peer_resolv_addr(T_GAP_REMOTE_ADDR_TYPE peer_address_type,
                                          uint8_t *peer_address)
{
    uint8_t *peer_identity_address = peer_address;
    uint8_t resolved_addr[6];
    T_GAP_IDENT_ADDR_TYPE peer_identity_address_type = le_privacy_convert_addr_type(peer_address_type);
    if (peer_address_type == GAP_REMOTE_ADDR_LE_RANDOM)
    {
        if (le_resolve_random_address(peer_address, resolved_addr, &peer_identity_address_type))
        {
            peer_identity_address = resolved_addr;
        }
    }
    return le_privacy_read_peer_resolv_addr(peer_identity_address_type, peer_identity_address);
}

T_GAP_CAUSE privacy_read_local_resolv_addr(T_GAP_REMOTE_ADDR_TYPE peer_address_type,
                                           uint8_t *peer_address)
{
    uint8_t *peer_identity_address = peer_address;
    uint8_t resolved_addr[6];
    T_GAP_IDENT_ADDR_TYPE peer_identity_address_type = le_privacy_convert_addr_type(peer_address_type);
    if (peer_address_type == GAP_REMOTE_ADDR_LE_RANDOM)
    {
        if (le_resolve_random_address(peer_address, resolved_addr, &peer_identity_address_type))
        {
            peer_identity_address = resolved_addr;
        }
    }
    return le_privacy_read_local_resolv_addr(peer_identity_address_type, peer_identity_address);
}

void privacy_manage_mode(bool is_manage)
{
    privacy_mode_manage = is_manage;
}

T_GAP_CAUSE privacy_set_peer_mode(T_GAP_REMOTE_ADDR_TYPE peer_address_type,
                                  uint8_t *peer_address, bool device_mode)
{
    uint8_t *peer_identity_address = peer_address;
    uint8_t resolved_addr[6];
    T_GAP_PRIVACY_MODE privacy_mode = GAP_PRIVACY_MODE_NETWORK;
    T_GAP_IDENT_ADDR_TYPE peer_identity_address_type = le_privacy_convert_addr_type(peer_address_type);
    if (peer_address_type == GAP_REMOTE_ADDR_LE_RANDOM)
    {
        if (le_resolve_random_address(peer_address, resolved_addr, &peer_identity_address_type))
        {
            peer_identity_address = resolved_addr;
        }
    }
    if (device_mode)
    {
        privacy_mode = GAP_PRIVACY_MODE_DEVICE;
    }
    return le_privacy_set_mode(peer_identity_address_type, peer_identity_address, privacy_mode);
}

T_GAP_CAUSE privacy_set_gen_priv_addr_interval(uint16_t interval)
{
    T_GAP_DEV_STATE dev_state;
    le_get_gap_param(GAP_PARAM_DEV_STATE, &dev_state);
    le_privacy_set_param(GAP_PARAM_PRIVACY_TIMEOUT, sizeof(uint16_t), &interval);
    if (dev_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
    {
        return le_privacy_set_resolv_priv_addr_timeout();
    }
    return GAP_CAUSE_SUCCESS;
}

void privacy_modify_resolving_list(T_GAP_RESOLV_LIST_OP op, T_GAP_IDENT_ADDR_TYPE addr_type,
                                   uint8_t *addr, bool device_mode)
{
    uint8_t bond_storage_num = le_get_max_le_paired_device_num();
    APP_PRINT_INFO1("[PRIVACY] privacy_modify_resolving_list op = %d", op);
    switch (op)
    {
    case GAP_RESOLV_LIST_OP_CLEAR:
        privacy_clear_pending = true;
        privacy_change_state(PRIVACY_STATE_BUSY);
        break;

    case GAP_RESOLV_LIST_OP_ADD:
        {
            uint8_t i;
            APP_PRINT_INFO2("[PRIVACY] privacy_add_device add: addr %s, addr type %d", TRACE_BDADDR(addr),
                            addr_type);
            for (i = 0; i < bond_storage_num; i++)
            {
                if (privacy_table[i].is_used)
                {
                    if ((privacy_table[i].remote_bd_type == addr_type)
                        && (memcmp(privacy_table[i].addr, addr, 6) == 0))
                    {
                        APP_PRINT_ERROR0("[PRIVACY] privacy_add_device add: failed, exist");
                        return;
                    }
                }
            }
            for (i = 0; i < bond_storage_num; i++)
            {
                if (!privacy_table[i].is_used)
                {
                    privacy_table[i].is_used = true;
                    privacy_table[i].remote_bd_type = addr_type;
                    memcpy(privacy_table[i].addr, addr, 6);
                    privacy_table[i].state = PRIVACY_RESOLVING_LIST_ADD_PENDING;
                    privacy_table[i].device_mode = device_mode;
                    privacy_change_state(PRIVACY_STATE_BUSY);
                    return;
                }
            }
            APP_PRINT_ERROR0("[PRIVACY] privacy_add_device add: failed, no free entry");
        }
        break;

    case GAP_RESOLV_LIST_OP_REMOVE:
        {
            uint8_t i;
            APP_PRINT_INFO2("[PRIVACY] privacy_add_device remove: addr %s, addr type %d", TRACE_BDADDR(addr),
                            addr_type);
            for (i = 0; i < bond_storage_num; i++)
            {
                if (privacy_table[i].is_used)
                {
                    if ((privacy_table[i].remote_bd_type == addr_type)
                        && (memcmp(privacy_table[i].addr, addr, 6) == 0))
                    {
                        if (privacy_table[i].is_added != PRIVACY_RESOLVING_ADDED_IDLE)
                        {
                            privacy_table[i].state = PRIVACY_RESOLVING_LIST_REMOVE_PENDING;
                            privacy_change_state(PRIVACY_STATE_BUSY);
                        }
                        else
                        {
                            memset(&privacy_table[i], 0, sizeof(T_LE_PRIVACY_ENTRY));
                            APP_PRINT_INFO0("[PRIVACY] privacy_add_device remove: not added");
                        }
                        break;
                    }
                }
            }
            APP_PRINT_ERROR0("[PRIVACY] privacy_add_device remove: failed, no find");
        }
        break;

    default:
        break;
    }
}

void privacy_handle_le_privacy_resolution_status_info(T_LE_PRIVACY_RESOLUTION_STATUS_INFO
                                                      resolv_status)
{
    APP_PRINT_INFO1("[PRIVACY] privacy_handle_le_privacy_resolution_status_info: status 0x%x\n",
                    resolv_status.status);
    privacy_resolution_status = resolv_status.status;
    if (privacy_app_cb)
    {
        T_PRIVACY_CB_DATA cb_data;
        cb_data.resolution_state = (T_PRIVACY_ADDR_RESOLUTION_STATE)resolv_status.status;
        privacy_app_cb(PRIVACY_RESOLUTION_STATUS_MSGTYPE, cb_data);
    }
}

void privacy_handle_le_privacy_modify_resolv_list_rsp(T_LE_PRIVACY_MODIFY_RESOLV_LIST_RSP *p_rsp)
{
    uint8_t bond_storage_num = le_get_max_le_paired_device_num();
    APP_PRINT_INFO2("[PRIVACY] privacy_handle_le_privacy_modify_resolv_list_rsp: operation  0x%x, casue 0x%x",
                    p_rsp->operation, p_rsp->cause);
    privacy_modify_resolv_list_pending = false;
    if (p_rsp->cause != GAP_SUCCESS)
    {
        if (p_rsp->cause == (HCI_ERR | HCI_ERR_UNKNOWN_CONN_ID)
            || p_rsp->cause == (HCI_ERR | HCI_ERR_MEMORY_FULL)
            || p_rsp->cause == (HCI_ERR | HCI_ERR_INVALID_PARAM))
        {
            memset(&privacy_table[privacy_modify_resolv_list_idx], 0, sizeof(T_LE_PRIVACY_ENTRY));
        }
        privacy_modify_resolv_list_idx = 0xff;
    }
    else
    {
        if (p_rsp->operation == GAP_RESOLV_LIST_OP_CLEAR)
        {
            privacy_clear_pending = false;
            memset(privacy_table, 0, bond_storage_num * sizeof(T_LE_PRIVACY_ENTRY));
        }
        else if (p_rsp->operation == GAP_RESOLV_LIST_OP_ADD)
        {
            if (privacy_modify_resolv_list_idx < bond_storage_num)
            {
                privacy_table[privacy_modify_resolv_list_idx].is_added = PRIVACY_RESOLVING_ADDED;
                privacy_table[privacy_modify_resolv_list_idx].state = PRIVACY_RESOLVING_LIST_IDLE;
                privacy_modify_resolv_list_idx = 0xff;
            }
        }
        else
        {
            if (privacy_modify_resolv_list_idx < bond_storage_num)
            {
                memset(&privacy_table[privacy_modify_resolv_list_idx], 0, sizeof(T_LE_PRIVACY_ENTRY));
                privacy_modify_resolv_list_idx = 0xff;
            }
        }
        privacy_handle_resolv_list();
    }
}

T_APP_RESULT privacy_msg_callback(uint8_t msg_type, T_LE_PRIVACY_CB_DATA msg_data)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    APP_PRINT_INFO1("[PRIVACY] privacy_msg_callback: msg_type  %d", msg_type);

    switch (msg_type)
    {
    case GAP_MSG_LE_PRIVACY_RESOLUTION_STATUS_INFO:
        privacy_handle_le_privacy_resolution_status_info(msg_data.le_privacy_resolution_status_info);
        break;

    case GAP_MSG_LE_PRIVACY_MODIFY_RESOLV_LIST:
        privacy_handle_le_privacy_modify_resolv_list_rsp(msg_data.p_le_privacy_modify_resolv_list_rsp);
        break;

    case GAP_MSG_LE_PRIVACY_SET_MODE:
        APP_PRINT_INFO1("[PRIVACY] GAP_MSG_LE_PRIVACY_SET_MODE: cause 0x%x",
                        msg_data.p_le_privacy_set_mode_rsp->cause);
        if (privacy_mode_manage == false && privacy_app_cb)
        {
            T_PRIVACY_CB_DATA cb_data;
            cb_data.cause = msg_data.p_le_privacy_set_mode_rsp->cause;
            privacy_app_cb(PRIVACY_SET_PEER_MODE_MSGTYPE, cb_data);
        }
        break;

    case GAP_MSG_LE_PRIVACY_SET_RESOLV_PRIV_ADDR_TIMEOUT:
        APP_PRINT_INFO1("[PRIVACY] GAP_MSG_LE_PRIVACY_SET_RESOLV_PRIV_ADDR_TIMEOUT: cause 0x%x",
                        msg_data.p_le_privacy_set_resolv_priv_addr_timeout_rsp->cause);
        if (privacy_app_cb)
        {
            T_PRIVACY_CB_DATA cb_data;
            cb_data.cause = msg_data.p_le_privacy_set_resolv_priv_addr_timeout_rsp->cause;
            privacy_app_cb(PRIVACY_GEN_PRIV_ADDR_INTERVAL_MSGTYPE, cb_data);
        }
        break;

    case GAP_MSG_LE_PRIVACY_READ_PEER_RESOLV_ADDR:
        APP_PRINT_INFO1("[PRIVACY] GAP_MSG_LE_PRIVACY_READ_PEER_RESOLV_ADDR: cause 0x%x",
                        msg_data.p_le_privacy_read_peer_resolv_addr_rsp->cause);
        if (privacy_app_cb)
        {
            T_PRIVACY_CB_DATA cb_data;
            cb_data.p_privacy_read_resolv_addr_rsp = (T_PRIVACY_READ_RESOLV_ADDR_RSP *)
                                                     msg_data.p_le_privacy_read_peer_resolv_addr_rsp;
            privacy_app_cb(PRIVACY_READ_PEER_RESOLV_ADDR_MSGTYPE, cb_data);
        }
        break;

    case GAP_MSG_LE_PRIVACY_READ_LOCAL_RESOLV_ADDR:
        APP_PRINT_INFO1("[PRIVACY] GAP_MSG_LE_PRIVACY_READ_LOCAL_RESOLV_ADDR: cause 0x%x",
                        msg_data.p_le_privacy_read_local_resolv_addr_rsp->cause);
        if (privacy_app_cb)
        {
            T_PRIVACY_CB_DATA cb_data;
            cb_data.p_privacy_read_resolv_addr_rsp = (T_PRIVACY_READ_RESOLV_ADDR_RSP *)
                                                     msg_data.p_le_privacy_read_local_resolv_addr_rsp;
            privacy_app_cb(PRIVACY_READ_LOCAL_RESOLV_ADDR_MSGTYPE, cb_data);
        }
        break;

    default:
        break;
    }
    return result;
}

