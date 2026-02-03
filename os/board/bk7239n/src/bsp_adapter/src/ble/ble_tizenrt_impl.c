#include <tinyara/net/if/ble.h>
#include "ble_tizenrt_impl.h"
#include "bk_ble_tizenrt.h"

#include "components/bluetooth/bk_ble_types.h"
#include "components/bluetooth/bk_ble.h"
#include "components/bluetooth/bk_dm_bluetooth.h"

#include "ble_tizenrt_advertiser.h"
#include "ble_tizenrt_central.h"
#include "ble_tizenrt_common.h"
#include "ble_tizenrt_impl.h"
#include "ble_tizenrt_peripheral.h"
#include "ble_tizenrt_scanner.h"

#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#if CONFIG_EASY_FLASH
    #include "easyflash.h"
#endif


#ifdef LOGE
    #undef LOGE
#endif

#ifdef LOGW
    #undef LOGW
#endif

#ifdef LOGI
    #undef LOGI
#endif

#ifdef LOGD
    #undef LOGD
#endif

#ifdef LOGV
    #undef LOGV
#endif

#ifdef LOG_TAG
    #undef LOG_TAG
#endif

#ifdef LOG_LEVEL
    #undef LOG_LEVEL
#endif

enum
{
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_VERBOSE,
};

#define LOG_TAG "bkble_hal"
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define LOGE(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_ERROR)   BK_BLE_LOGE(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGW(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_WARNING) BK_BLE_LOGW(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGI(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_INFO)    BK_BLE_LOGI(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGD(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_DEBUG)   BK_BLE_LOGD(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGV(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_VERBOSE) BK_BLE_LOGV(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)


static void hal_ble_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param);
static void bk_adapter_ble_notice_cb(ble_notice_t notice, void *param);

hal_ble_env_t hal_ble_env = {0};
hal_ble_adv_env_t hal_ble_adv_env = {0};
hal_ble_scan_env_t hal_ble_scan_env = {0};
hal_ble_con_env_t hal_ble_con_env = {0};

static beken_queue_t ble_evt_que = NULL;
static beken_queue_t ble_cmd_que = NULL;
static beken_thread_t ble_evt_handle = NULL;
static beken_thread_t ble_cmd_handle = NULL;

static beken_semaphore_t g_ble_cmd_sema = NULL;
static beken_semaphore_t g_ble_whitelist_sema = NULL;

static bool is_ble_init = false;

extern bt_bond_info_storage_t *bt_bond_info_storage;
extern bt_whitelist_info_storage_t *bt_whitelist_info_storage;
extern const uint8_t s_bt_empty_addr[6];
extern const uint8_t s_bt_invaild_addr[6];
extern const uint8_t s_bt_empty_irk[16];

/// util function
int ble_cmd_queue_push(uint16_t event, uint16_t idx)
{
    struct ble_cmd_msg msg;
    os_memset(&msg, 0, sizeof(struct ble_cmd_msg));

    msg.event = event;
    msg.idx = idx;

    if (!ble_cmd_que || rtos_push_to_queue(&ble_cmd_que, &msg, BEKEN_NO_WAIT) != kNoErr)
    {
        LOGE("push failed evt %d", event);
        return -1;
    }

    return 0;
}

int ble_cmd_queue_push_ext(uint16_t event, uint16_t idx,
                           void *param,
                           uint32_t size,
                           msg_cmd_cb cb)
{
    struct ble_cmd_msg msg = {0};

    msg.event = event;
    msg.idx = idx;
    msg.cb = cb;
    msg.size = size;

    if (param && size)
    {
        msg.param = os_zalloc(size);

        if (!msg.param)
        {
            LOGE("alloc err size %d", size);
            return -1;
        }

        os_memcpy(msg.param, param, size);
    }
    else
    {
        msg.param = param;
    }

    if (!ble_cmd_que || rtos_push_to_queue(&ble_cmd_que, &msg, BEKEN_NO_WAIT) != kNoErr)
    {
        LOGE("push failed evt %d", event);

        if(param && size && msg.param)
        {
            os_free(msg.param);
        }

        return -1;
    }

    return 0;
}

int ble_evt_queue_push(uint16_t type, void *param)
{
    ble_evt_msg_t msg = {0};
    msg.type = type;
    msg.u.buf = param;
    msg.buff_size = 0;

    if ( !ble_evt_que || rtos_push_to_queue(&ble_evt_que, &msg, BEKEN_NO_WAIT) != kNoErr)
    {
        LOGE("push failed evt %d", type);

        return -1;
    }

    return 0;
}

int ble_evt_queue_push_ext(uint16_t type, void *param, uint32_t size, msg_evt_cb cb)
{
    ble_evt_msg_t msg = {0};
    int32_t ret = 0;

    msg.type = type;
    msg.cb = cb;
    msg.buff_size = size;

    if (param && size)
    {
        msg.u.buf = os_zalloc(size);

        if (!msg.u.buf)
        {
            LOGE("alloc err size %d", size);
            return -1;
        }

        os_memcpy(msg.u.buf, param, size);
    }
    else
    {
        msg.u.buf = param;
    }

    if (!ble_evt_que || (ret = rtos_push_to_queue(&ble_evt_que, &msg, BEKEN_NO_WAIT)) != kNoErr)
    {
        LOGE("push failed evt %d ret %d", type, ret);

        if (param && size && msg.u.buf)
        {
            os_free(msg.u.buf);
        }

        return -1;
    }

    return 0;
}

static int32_t bk_wait_for_ble_idle(int timeout)
{
    int32_t ret = 0;

    if (g_ble_cmd_sema != NULL)
    {
        if ((ret = rtos_get_semaphore(&g_ble_cmd_sema, timeout)) != 0)
        {
            LOGD("get cmd sem err %d", ret);
            return -1;
        }
    }

    return 0;
}

void bk_set_ble_cmd_sema(void)
{
    if (g_ble_cmd_sema != NULL)
    {
        LOGV("set g_ble_cmd_sema");
        rtos_set_semaphore(&g_ble_cmd_sema);
    }
}

static void bk_adapter_ble_whitelist_init(void)
{
    if (!bt_whitelist_info_storage)
    {
        LOGE(" not init");
        return;
    }

    bk_err_t result = rtos_init_semaphore(&g_ble_whitelist_sema, 1);
    BK_ASSERT(0 == result);

    for (int i = 0; i < BT_WHITELIST_MAX_SAVE_COUNT; ++i)
    {
        if (os_memcmp(s_bt_empty_addr, bt_whitelist_info_storage->device[i].peer_addr, 6)
                && os_memcmp(s_bt_invaild_addr, bt_whitelist_info_storage->device[i].peer_addr, 6))
        {
            le_modify_white_list(GAP_WHITE_LIST_OP_ADD, bt_whitelist_info_storage->device[i].peer_addr, bt_whitelist_info_storage->device[i].peer_addr_type);
        }
    }
}

static void bk_adapter_ble_whitelist_deinit(void)
{
    if (g_ble_whitelist_sema)
    {
        rtos_deinit_semaphore(&g_ble_whitelist_sema);
        g_ble_whitelist_sema = NULL;
    }
}

uint16_t bk_adapter_ble_hci_error_code(uint16_t code)
{
#define HCI_ERR_CODE                 0x0100
    return (HCI_ERR_CODE | code);
}

static void hal_ble_evt_thread(void)
{
    beken_semaphore_t *exit_sem = NULL;

    while (1)
    {
        int err;
        // int32_t ret_status = 0;  // Unused variable

        ble_evt_msg_t msg;
        err = rtos_pop_from_queue(&ble_evt_que, &msg, BEKEN_WAIT_FOREVER);

        if (kNoErr == err)
        {
            switch (msg.type)
            {
            case EVT_BLE_BONDED_MSG:
            {
                if (msg.u.buf)
                {
                    trble_device_connected *bonded_dev = (trble_device_connected *)msg.u.buf;
                    LOGD("bonded device msg, con_idx:%d, addr_type:%d, addr:%02x:%02x:%02x:%02x:%02x:%02x ", bonded_dev->conn_handle,
                         bonded_dev->conn_info.addr.type, bonded_dev->conn_info.addr.mac[5], bonded_dev->conn_info.addr.mac[4], bonded_dev->conn_info.addr.mac[3],
                         bonded_dev->conn_info.addr.mac[2], bonded_dev->conn_info.addr.mac[1], bonded_dev->conn_info.addr.mac[0]);

                    if (hal_ble_con_env.con_dev[bonded_dev->conn_handle].role == LINK_ROLE_MASTER)
                    {
                        if (bktr_ble_client_get_param()->trble_device_connected_cb)
                        {
                            hal_ble_con_env.con_dev[bonded_dev->conn_handle].app_connected = 1;
                            bktr_ble_client_get_param()->trble_device_connected_cb(bonded_dev);
                        }
                    }

                    os_free(bonded_dev);
                }
            }
            break;

            case EVT_BLE_CONNECTED_MSG:
            {
                if (msg.u.buf)
                {
                    trble_device_connected *con_dev = (trble_device_connected *)msg.u.buf;
                    LOGD("connect device msg, con_idx:%d, addr_type:%d, addr:%02x:%02x:%02x:%02x:%02x:%02x ", con_dev->conn_handle,
                         con_dev->conn_info.addr.type, con_dev->conn_info.addr.mac[5], con_dev->conn_info.addr.mac[4], con_dev->conn_info.addr.mac[3],
                         con_dev->conn_info.addr.mac[2], con_dev->conn_info.addr.mac[1], con_dev->conn_info.addr.mac[0]);

                    if (hal_ble_con_env.con_dev[con_dev->conn_handle].role == LINK_ROLE_MASTER)
                    {
                        if (!os_memcmp(hal_ble_con_env.peer_addr.addr, con_dev->conn_info.addr.mac, TRBLE_BD_ADDR_MAX_LEN))
                        {
                            if (con_dev->conn_info.is_secured_connect && hal_ble_con_env.con_dev[con_dev->conn_handle].auth_state != GAP_AUTHEN_STATE_COMPLETE)
                            {
                                LOGD("start bond and encrypt ");
                                le_bond_pair(con_dev->conn_handle);
                            }
                            else
                            {
                                if (bktr_ble_client_get_param()->trble_device_connected_cb)
                                {
                                    hal_ble_con_env.con_dev[con_dev->conn_handle].app_connected = 1;
                                    bktr_ble_client_get_param()->trble_device_connected_cb(con_dev);
                                }
                            }
                        }
                        else
                        {
                            LOGE("connect device msg, addr not match ");
                        }
                    }

                    os_free(con_dev);
                }
            }
            break;

            case EVT_BLE_SCAN_STATE_MSG:
            {
                uint32_t scan_state = msg.u.param;
                LOGD("scan state msg, scan_state:%d ", scan_state);

                if (bktr_ble_client_get_param()->trble_scan_state_changed_cb)
                {
                    if (scan_state == GAP_SCAN_STATE_SCANNING)
                    {
                        bktr_ble_client_get_param()->trble_scan_state_changed_cb(TRBLE_SCAN_STARTED);

                    }
                    else if (scan_state == GAP_SCAN_STATE_IDLE)
                    {
                        bktr_ble_client_get_param()->trble_scan_state_changed_cb(TRBLE_SCAN_STOPPED);
                    }
                }
            }
            break;

            case EVT_BLE_SCANNED_DEVICE_MSG:
            {
                if (msg.u.buf)
                {
                    trble_scanned_device *scan_device = (trble_scanned_device *)msg.u.buf;

                    if (bktr_ble_client_get_param()->trble_device_scanned_cb)
                    {
                        bktr_ble_client_get_param()->trble_device_scanned_cb(scan_device);
                    }

                    LOGV("scan device msg, addr_type:%d, adv_addr:%02x:%02x:%02x:%02x:%02x:%02x ", scan_device->addr.type,
                         scan_device->addr.mac[5], scan_device->addr.mac[4], scan_device->addr.mac[3],
                         scan_device->addr.mac[2], scan_device->addr.mac[1], scan_device->addr.mac[0]);
                    os_free(scan_device);
                }

                break;
            }

            case EVT_BLE_DISCONNECTED_MSG:
            {
                if (msg.u.buf)
                {
                    ble_disconnected_msg_t *dis_msg = (ble_disconnected_msg_t *)msg.u.buf;
                    LOGD("disconnected msg, conn_id:%d, reason:0x%x, role:%d ", dis_msg->conn_id, dis_msg->reason, dis_msg->role);

                    if (bktr_ble_client_get_param()->trble_device_disconnected_cb)
                    {
                        bktr_ble_client_get_param()->trble_device_disconnected_cb(dis_msg->conn_id);
                    }

                    os_free(dis_msg);
                }
            }
            break;

            case EVT_BLE_NOTIFICATION_MSG:
            {
                if (msg.u.buf)
                {
                    ble_tizenrt_notification_t *noti = (ble_tizenrt_notification_t *)msg.u.buf;
                    LOGD("notification msg, con_idx:%d, handle:%d, data_len:%d ", noti->handle.conn_handle, noti->handle.attr_handle, noti->noti_data.length);

                    if (bktr_ble_client_get_param()->trble_operation_notification_cb)
                    {
                        bktr_ble_client_get_param()->trble_operation_notification_cb(&(noti->handle), &(noti->noti_data));
                    }

                    if (noti->noti_data.data)
                    {
                        os_free(noti->noti_data.data);
                    }

                    os_free(noti);
                }
            }
            break;

            case EVT_BLE_INDICATION_MSG:
            {
                if (msg.u.buf)
                {
                    ble_tizenrt_indication_t *ind = (ble_tizenrt_indication_t *)msg.u.buf;
                    LOGD("indication msg, con_idx:%d, handle:%d, data_len:%d ", ind->handle.conn_handle, ind->handle.attr_handle, ind->indi_data.length);

                    if (bktr_ble_client_get_param()->trble_operation_indication_cb)
                    {
                        bktr_ble_client_get_param()->trble_operation_indication_cb(&(ind->handle), &(ind->indi_data));
                    }

                    if (ind->indi_data.data)
                    {
                        os_free(ind->indi_data.data);
                    }

                    os_free(ind);
                }
            }
            break;

            case EVT_BLE_READ_RESULT_MSG:
                break;

            case EVT_BLE_PAIRING_NUMBER_COMPARE_REQ_MSG:
            {
                ble_auth_key_cfm_t *key_cfm_ind = (typeof(key_cfm_ind))msg.u.buf;
                hal_ble_conn_t *info = (hal_ble_conn_t *)le_get_conn_info_ext(key_cfm_ind->con_idx);

                LOGD("EVT_BLE_PAIRING_NUMBER_COMPARE_REQ_MSG conn_idx:%d, passkey:%06d", key_cfm_ind->con_idx, key_cfm_ind->passkey);

                if (!info)
                {
                    LOGE("can't find conn info by %d", key_cfm_ind->con_idx);
                    break;
                }

                if (info->role == LINK_ROLE_MASTER && bktr_ble_client_get_param()->trble_device_passkey_display_cb)
                {
                    bktr_ble_client_get_param()->trble_device_passkey_display_cb(key_cfm_ind->passkey, key_cfm_ind->con_idx);
                }
                else if (info->role == LINK_ROLE_SLAVE)
                {
                    bk_tr_ble_server_report_passkey_evt(key_cfm_ind->con_idx, key_cfm_ind->passkey);
                }

                os_free(key_cfm_ind);
            }
            break;

            case EVT_BLE_SERVER_CONNECTED:
            {
                ble_evt_msg_elem_t *elem = (typeof(elem))msg.u.buf;

                if (bktr_ble_server_get_param()->connected_cb)
                {
                    bktr_ble_server_get_param()->connected_cb(elem->server_connect_evt.conn_idx, elem->server_connect_evt.type,
                            elem->server_connect_evt.peer_addr, elem->server_connect_evt.relate_adv_handle);
                }
            }
            break;

            case EVT_BLE_SERVER_DISCONNECT:
            {
                ble_evt_msg_elem_t *elem = (typeof(elem))msg.u.buf;

                if (bktr_ble_server_get_param()->disconnected_cb)
                {
                    bktr_ble_server_get_param()->disconnected_cb(elem->server_disconnect_evt.conn_idx, elem->server_disconnect_evt.reason);
                }
            }
            break;

            case EVT_BLE_SERVER_MTU_CHANGE:
            {
                ble_evt_msg_elem_t *elem = (typeof(elem))msg.u.buf;

                if (bktr_ble_server_get_param()->mtu_update_cb)
                {
                    bktr_ble_server_get_param()->mtu_update_cb(elem->mtu_change_evt.conn_idx, elem->mtu_change_evt.mtu);
                }
            }
            break;

            case EVT_BLE_SERVER_PASSKEY:
            {
                ble_evt_msg_elem_t *elem = (typeof(elem))msg.u.buf;

                if (bktr_ble_server_get_param()->passkey_display_cb)
                {
                    bktr_ble_server_get_param()->passkey_display_cb(elem->passkey_evt.conn_idx, elem->passkey_evt.passkey);
                }
            }
            break;

            case EVT_BLE_SERVER_ATTR_CB:
            {
                ble_evt_msg_elem_t *elem = (typeof(elem))msg.u.buf;

                if (elem->attr_cb_evt.tmp_buffer && elem->attr_cb_evt.tmp_buffer_len)
                {
                    uint8_t *current_buffer = NULL;
                    uint16_t current_buffer_len = 0;
                    uint16_t current_buffer_max_len = 0;

                    bk_tr_ble_server_attr_get_data_ptr(elem->attr_cb_evt.handle, &current_buffer, &current_buffer_len, &current_buffer_max_len);

                    if (current_buffer && current_buffer_max_len)
                    {
                        uint16_t final_len = (current_buffer_max_len < elem->attr_cb_evt.tmp_buffer_len ? current_buffer_max_len : elem->attr_cb_evt.tmp_buffer_len);
                        os_memcpy(current_buffer, elem->attr_cb_evt.tmp_buffer, final_len);
                        bk_tr_ble_server_attr_set_data_ptr_private(elem->attr_cb_evt.service_index, elem->attr_cb_evt.att_index,
                                current_buffer, final_len, current_buffer_max_len);
                    }
                }

                if (elem->attr_cb_evt.tmp_buffer && elem->attr_cb_evt.tmp_buffer_len)
                {
                    os_free(elem->attr_cb_evt.tmp_buffer);
                    elem->attr_cb_evt.tmp_buffer = NULL;
                    elem->attr_cb_evt.tmp_buffer_len = 0;
                }

                if (elem->attr_cb_evt.cb)
                {
                    elem->attr_cb_evt.cb(elem->attr_cb_evt.type,
                                         elem->attr_cb_evt.con_handle,
                                         elem->attr_cb_evt.handle,
                                         elem->attr_cb_evt.arg,
                                         elem->attr_cb_evt.result,
                                         elem->attr_cb_evt.pending);
                }
            }
            break;

            case EVT_BLE_SERVER_SET_BUFFER:
            {
                ble_evt_msg_elem_t *elem = (typeof(elem))msg.u.buf;

                bk_tr_ble_server_attr_set_data_ptr_private(elem->set_server_buffer_cmd.service_index,
                        elem->set_server_buffer_cmd.att_index,
                        elem->set_server_buffer_cmd.buffer,
                        elem->set_server_buffer_cmd.buffer_len,
                        elem->set_server_buffer_cmd.buffer_max_len);
            }
            break;

            case EVT_BLE_TASK_EXIT:
            {
                exit_sem = (typeof(exit_sem))msg.u.buf;

                if (msg.u.buf && msg.buff_size)
                {
                    os_free(msg.u.buf);
                    msg.u.buf = NULL;
                }

                goto end;
            }
            break;

            case EVT_BLE_DO_CB_ONLY:
            {

            }
            break;

            default:
                break;
            }

            if (msg.cb)
            {
                msg.cb(msg.type, 0, msg.u.buf);
            }

            if (msg.u.buf && msg.buff_size)
            {
                os_free(msg.u.buf);
                msg.u.buf = NULL;
            }
        }
    }

end:;

    LOGW("thread end");

    if (exit_sem)
    {
        rtos_set_semaphore(exit_sem);
    }

    rtos_delete_thread(NULL);
}

uint8_t le_modify_white_list(T_GAP_WHITE_LIST_OP operation, uint8_t *bd_addr,
                             T_GAP_REMOTE_ADDR_TYPE bd_type)
{
    switch (operation)
    {
    case GAP_WHITE_LIST_OP_CLEAR:
    {
        LOGD("WHITE_LIST_OP_CLEAR ");
        bk_ble_clear_white_list();
        break;
    }

    case GAP_WHITE_LIST_OP_ADD:
    {
        LOGD("WHITE_LIST_OP_ADD ,0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x(%d)", bd_addr[0], bd_addr[1],
             bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5], bd_type);
        bd_addr_t btaddr;
        os_memcpy(btaddr.addr, bd_addr, 6);
        bk_ble_add_devices_to_while_list(&btaddr, bd_type);
        break;
    }

    case GAP_WHITE_LIST_OP_REMOVE:
    {
        LOGD("WHITE_LIST_OP_REMOVE ,0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x(%d)", bd_addr[0], bd_addr[1],
             bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5], bd_type);
        bd_addr_t btaddr;
        os_memcpy(btaddr.addr, bd_addr, 6);
        bk_ble_remove_devices_from_while_list(&btaddr, bd_type);
        break;
    }

    default:
        LOGE("operation(%x) not supported", operation);
        return 0;
    }

    if ( g_ble_whitelist_sema != NULL)
    {
        if (kTimeoutErr == rtos_get_semaphore(&g_ble_whitelist_sema, BLE_SYNC_CMD_TIMEOUT_MS))
        {
            LOGE(" get sem timeout");
            return 1;
        }
    }

    return 0;
}


uint8_t le_read_rssi(uint8_t conn_id)
{
    LOGD("conn_id %d", conn_id);
    bk_ble_read_rssi(conn_id);
    return 0;
}

void bk_adapter_notify_bt_status(uint16_t sub_type, T_LE_GAP_MSG_DATA *msg_data)
{
    LOGD("sub_type %d ", sub_type);

    if (msg_data)
    {
        switch (sub_type)
        {
        case GAP_MSG_LE_DEV_STATE_CHANGE:
        {
            LOGD("new_scan_state %d, new_adv_state %d, new_init_state %d ",
                 msg_data->gap_dev_state_change.new_state.gap_scan_state,
                 msg_data->gap_dev_state_change.new_state.gap_adv_state,
                 msg_data->gap_dev_state_change.new_state.gap_init_state);

            if (hal_ble_env.dev_state.gap_scan_state != msg_data->gap_dev_state_change.new_state.gap_scan_state)
            {
                uint32_t scan_state = (uint32_t)msg_data->gap_dev_state_change.new_state.gap_scan_state;

                if (ble_evt_queue_push(EVT_BLE_SCAN_STATE_MSG, (void *)scan_state) != 0)
                {
                    LOGE("push scan state failed");
                }
            }

            if (hal_ble_env.dev_state.gap_init_state != msg_data->gap_dev_state_change.new_state.gap_init_state)
            {
                if (msg_data->gap_dev_state_change.new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
                {
                    LOGD("GAP stack ready ");
                }

                hal_ble_env.dev_state.gap_init_state = msg_data->gap_dev_state_change.new_state.gap_init_state;
            }
        }
        break;

        case GAP_MSG_LE_CONN_STATE_CHANGE:
        {
            LOGD("conn_id %d, new_state %d ", msg_data->gap_conn_state_change.conn_id, msg_data->gap_conn_state_change.new_state);

            if (msg_data->gap_conn_state_change.new_state == GAP_CONN_STATE_CONNECTED)
            {
                hal_ble_conn_t *c_ind = &hal_ble_con_env.con_dev[msg_data->gap_conn_state_change.conn_id];
                trble_device_connected *connected_dev = os_zalloc(sizeof(trble_device_connected));
                connected_dev->conn_handle = msg_data->gap_conn_state_change.conn_id;
                connected_dev->is_bonded = false;

                if (!connected_dev)
                {
                    LOGE("os_zalloc failed");
                    break;
                }

                os_memcpy(connected_dev->conn_info.addr.mac, c_ind->peer_addr, TRBLE_BD_ADDR_MAX_LEN);
                connected_dev->conn_info.addr.type = bk_adapter_ble_convert_addr_type_2_tr(c_ind->peer_addr_type, c_ind->peer_addr);
                connected_dev->conn_info.conn_interval = c_ind->intv;
                connected_dev->conn_info.slave_latency = c_ind->con_latency;
                connected_dev->conn_info.mtu = c_ind->mtu_size;
                connected_dev->conn_info.is_secured_connect = hal_ble_con_env.is_secured_connect;
                connected_dev->conn_info.scan_timeout = hal_ble_con_env.conn_scan_timeout;

                if (ble_evt_queue_push(EVT_BLE_CONNECTED_MSG, connected_dev) != 0)
                {
                    LOGE("push connected dev failed");
                    os_free(connected_dev);
                }
            }
            else if (msg_data->gap_conn_state_change.new_state == GAP_CONN_STATE_DISCONNECTED)
            {
                ble_disconnected_msg_t *dis_msg = os_zalloc(sizeof(ble_disconnected_msg_t));

                if (!dis_msg)
                {
                    LOGE("os_zalloc failed");
                    break;
                }

                dis_msg->conn_id = msg_data->gap_conn_state_change.conn_id;
                dis_msg->reason = msg_data->gap_conn_state_change.disc_cause;
                dis_msg->role = hal_ble_con_env.con_dev[msg_data->gap_conn_state_change.conn_id].role;

                if (ble_evt_queue_push(EVT_BLE_DISCONNECTED_MSG, dis_msg) != 0)
                {
                    LOGE("push disconnected dev failed");
                    os_free(dis_msg);
                }
            }
            else
            {
                //nothing todo
            }
        }
        break;

        case GAP_MSG_LE_CONN_PARAM_UPDATE:
            break;

        case GAP_MSG_LE_CONN_MTU_INFO:
            break;

        case GAP_MSG_LE_AUTHEN_STATE_CHANGE:
        {
            LOGD("bond new_state %d, conn_id %d, statu:%d ", msg_data->gap_authen_state.new_state, msg_data->gap_authen_state.conn_id, msg_data->gap_authen_state.status);

            if (msg_data->gap_authen_state.new_state == GAP_AUTHEN_STATE_COMPLETE && msg_data->gap_authen_state.new_state != hal_ble_con_env.con_dev[msg_data->gap_authen_state.conn_id].auth_state)
            {
                if (msg_data->gap_authen_state.status == GAP_SUCCESS)
                {
                    trble_device_connected *bonded_dev = os_zalloc(sizeof(trble_device_connected));
                    hal_ble_conn_t *c_ind = &hal_ble_con_env.con_dev[msg_data->gap_authen_state.conn_id];
                    bonded_dev->conn_handle = msg_data->gap_authen_state.conn_id;
                    bonded_dev->is_bonded = true;
                    os_memcpy(bonded_dev->conn_info.addr.mac, c_ind->peer_addr, TRBLE_BD_ADDR_MAX_LEN);
                    bonded_dev->conn_info.addr.type = bk_adapter_ble_convert_addr_type_2_tr(c_ind->peer_addr_type, c_ind->peer_addr);
                    bonded_dev->conn_info.conn_interval = c_ind->intv;
                    bonded_dev->conn_info.slave_latency = c_ind->con_latency;
                    bonded_dev->conn_info.mtu = c_ind->mtu_size;
                    bonded_dev->conn_info.is_secured_connect = hal_ble_con_env.is_secured_connect;
                    bonded_dev->conn_info.scan_timeout = hal_ble_con_env.conn_scan_timeout;

                    if (ble_evt_queue_push(EVT_BLE_BONDED_MSG, bonded_dev) != 0)
                    {
                        LOGE("push bonded dev failed");
                        os_free(bonded_dev);
                    }
                }
            }

            hal_ble_con_env.con_dev[msg_data->gap_authen_state.conn_id].auth_state = msg_data->gap_authen_state.new_state;
        }
        break;

        case GAP_MSG_LE_BOND_JUST_WORK:
            break;

        default:
            LOGE("sub_type(%x) not supported", sub_type);
            break;
        }
    }
}

int8_t hal_ble_find_adv_index_by_handle(uint8 adv_handle)
{
    for (uint32_t i = 0; i < sizeof(hal_ble_adv_env.array) / sizeof(hal_ble_adv_env.array[0]); ++i)
    {
        if (adv_handle != 0xff && adv_handle == hal_ble_adv_env.array[i].adv_idx)
        {
            return i;
        }
    }

    return -1;
}

static void hal_ble_cmd_thread(void)
{
    beken_semaphore_t *exit_sem = NULL;

    while (1)
    {
        int err;
        struct ble_cmd_msg msg;

        err = rtos_pop_from_queue(&ble_cmd_que, &msg, BEKEN_WAIT_FOREVER);

        if (kNoErr == err)
        {
            int32_t ret_status = 0;
            LOGD("CMD %d", msg.event);

            switch (msg.event)
            {
            case CMD_BLE_CREATE_ADV:
            {
                ble_cmd_msg_elem_t *elem = (typeof(elem))msg.param;
                uint8_t adv_index = bk_ble_get_idle_actv_idx_handle();
                int8_t hal_adv_index = -1;

                if (adv_index == 0xff)
                {
                    LOGE("can't alloc adv, full !");
                    ret_status = -1;
                    break;
                }

                for (int i = 0; i < sizeof(hal_ble_adv_env.array) / sizeof(hal_ble_adv_env.array[0]); ++i)
                {
                    if (hal_ble_adv_env.array[i].adv_idx == 0xff)
                    {
                        //hal_ble_adv_env.array[i].adv_idx = adv_handle;
                        hal_adv_index = i;
                        LOGI("alloc hal_adv_index %d adv_index %d", hal_adv_index, adv_index);
                        break;
                    }
                }

                if (hal_adv_index < 0)
                {
                    LOGE("can't alloc adv_index %d", adv_index);
                    ret_status = -1;
                    break;
                }

                elem->adv_index = adv_index;
                elem->hal_adv_index = hal_adv_index;

                LOGI("CMD_BLE_CREATE_ADV adv_index %d hal_adv_index %d adv_elem %d", adv_index, hal_adv_index, elem->adv_elem);

                if (elem->adv_elem == ADV_ELEM_STATIC)
                {
                    os_memcpy(&hal_ble_adv_env.array[hal_adv_index], &hal_ble_adv_env.static_adv_param, sizeof(hal_ble_adv_env.static_adv_param));
                }
                else if (elem->adv_elem == ADV_ELEM_ONESHOT)
                {
                    os_memcpy(&hal_ble_adv_env.array[hal_adv_index], &hal_ble_adv_env.static_oneshot_adv_param[elem->oneshot_index],
                              sizeof(hal_ble_adv_env.static_oneshot_adv_param[elem->oneshot_index]));
                }

                hal_ble_adv_env.array[hal_adv_index].adv_status = BLE_HAL_ADV_STATE_IDLE;
                hal_ble_adv_env.array[hal_adv_index].adv_idx = adv_index;
                hal_ble_adv_env.array[hal_adv_index].adv_elem = elem->adv_elem;

                if (!bk_ble_create_advertising(hal_ble_adv_env.array[hal_adv_index].adv_idx, &hal_ble_adv_env.array[hal_adv_index].adv_param, hal_ble_cmd_cb))
                {
                    ret_status = bk_wait_for_ble_idle(BLE_SYNC_CMD_TIMEOUT_MS);
                }
                else
                {
                    LOGE("create adv err");
                    ret_status = -1;
                    break;
                }
            }
            break;

            case CMD_BLE_START_ADV:
            {
                ble_cmd_msg_elem_t *elem = (typeof(elem))msg.param;
                uint8_t adv_index = elem->adv_index;
                int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(adv_index);

                if (adv_index == 0xff)
                {
                    LOGE("can't start adv, invalid adv_index !");
                    ret_status = -1;
                    break;
                }

                if (hal_adv_index < 0)
                {
                    LOGE("can't find adv index by %d %d", adv_index, adv_index);
                    ret_status = -1;
                    break;
                }

                elem->adv_index = adv_index;
                elem->hal_adv_index = hal_adv_index;

                LOGI("CMD_BLE_START_ADV handle %d index %d", adv_index, hal_adv_index);

                if (BLE_HAL_ADV_STATE_CREATED == hal_ble_adv_env.array[hal_adv_index].adv_status)
                {
                    err = bk_ble_modify_advertising(hal_ble_adv_env.array[hal_adv_index].adv_idx, &hal_ble_adv_env.array[hal_adv_index].adv_param, hal_ble_cmd_cb);

                    if (err)
                    {
                        LOGE("modify adv param err %d", err);
                        ret_status = -1;
                        break;
                    }

                    err = bk_wait_for_ble_idle(BLE_SYNC_CMD_TIMEOUT_MS);

                    if (err)
                    {
                        LOGE("wait modify adv param err err");
                        ret_status = -1;
                        break;
                    }
                }
                else
                {
                    LOGE("start adv status not match %d", hal_ble_adv_env.array[hal_adv_index].adv_status);
                    ret_status = -1;
                }
            }
            break;

            case CMD_BLE_STOP_ADV:
            {
                ble_cmd_msg_elem_t *elem = (typeof(elem))msg.param;
                uint8_t adv_index = elem->adv_index;
                int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(elem->adv_index);

                if (hal_adv_index < 0)
                {
                    LOGE("can't find adv index by %d", elem->adv_index);
                    ret_status = -1;
                    break;
                }

                elem->adv_index = adv_index;
                elem->hal_adv_index = hal_adv_index;

                if (BLE_HAL_ADV_STATE_ADVERTISING == hal_ble_adv_env.array[hal_adv_index].adv_status)
                    //&& 0 == hal_ble_adv_env.array[adv_index].deleting)
                {
                    hal_ble_env.dev_state.gap_adv_state = GAP_ADV_STATE_STOP;
                    bk_adapter_notify_bt_status(GAP_MSG_LE_DEV_STATE_CHANGE, NULL);

                    //hal_ble_adv_env.array[adv_index].adv_status = BLE_HAL_ADV_STATE_CREATED;

                    if (!bk_ble_stop_advertising(hal_ble_adv_env.array[hal_adv_index].adv_idx, hal_ble_cmd_cb))
                    {
                        ret_status = bk_wait_for_ble_idle(BLE_SYNC_CMD_TIMEOUT_MS);
                    }
                    else
                    {
                        LOGE("stop adv err");
                        ret_status = -1;
                    }
                }
                else if (BLE_HAL_ADV_STATE_CREATED == hal_ble_adv_env.array[hal_adv_index].adv_status)
                {
                    LOGW("already stop");
                }
                else
                {
                    LOGD("stop adv err status not match %d", hal_ble_adv_env.array[hal_adv_index].adv_status);
                    ret_status = -1;
                }
            }
            break;

            case CMD_BLE_DEL_ADV:
            {
                ble_cmd_msg_elem_t *elem = (typeof(elem))msg.param;
                uint8_t adv_index = elem->adv_index;
                int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(elem->adv_index);

                if (hal_adv_index < 0)
                {
                    LOGE("can't find adv index by %d", elem->adv_index);
                    ret_status = -1;
                    break;
                }

                elem->adv_index = adv_index;
                elem->hal_adv_index = hal_adv_index;

                if (BLE_HAL_ADV_STATE_CREATED == hal_ble_adv_env.array[hal_adv_index].adv_status ||
                        BLE_HAL_ADV_STATE_DELETING == hal_ble_adv_env.array[hal_adv_index].adv_status)
                {
                    if (!bk_ble_delete_advertising(hal_ble_adv_env.array[hal_adv_index].adv_idx, hal_ble_cmd_cb))
                    {
                        ret_status = bk_wait_for_ble_idle(BLE_SYNC_CMD_TIMEOUT_MS);
                    }
                    else
                    {
                        LOGE("del adv err");
                        ret_status = -1;
                    }
                }
                else
                {
                    LOGD("del adv err, status not match %d", hal_ble_adv_env.array[hal_adv_index].adv_status);
                    ret_status = -1;
                }
            }
            break;

            case CMD_BLE_START_SCAN:
                if (BLE_HAL_SCAN_STATE_IDLE == hal_ble_scan_env.scan_status)
                {
                    T_LE_GAP_MSG_DATA msg_data = {0};
                    int ret = 0;
                    msg_data.gap_dev_state_change.new_state.gap_scan_state = GAP_SCAN_STATE_START;
                    bk_adapter_notify_bt_status(GAP_MSG_LE_DEV_STATE_CHANGE, &msg_data);
                    hal_ble_env.dev_state.gap_scan_state = GAP_SCAN_STATE_START;

                    hal_ble_scan_env.scan_idx = bk_ble_get_idle_actv_idx_handle();
                    hal_ble_scan_env.scan_status = BLE_HAL_SCAN_STATE_START;
                    ret = bk_ble_create_scaning(hal_ble_scan_env.scan_idx, &hal_ble_scan_env.scan_param, hal_ble_cmd_cb);

                    if (BK_ERR_BLE_SUCCESS == ret)
                    {
                        ret_status = bk_wait_for_ble_idle(BLE_SYNC_CMD_TIMEOUT_MS);
                    }
                    else
                    {
                        LOGE("create scan err %d", ret);
                    }

                    if (BLE_HAL_SCAN_STATE_SCANNING == hal_ble_scan_env.scan_status)
                    {
                        msg_data.gap_dev_state_change.new_state.gap_scan_state = GAP_SCAN_STATE_SCANNING;
                        bk_adapter_notify_bt_status(GAP_MSG_LE_DEV_STATE_CHANGE, &msg_data);
                        hal_ble_env.dev_state.gap_scan_state = GAP_SCAN_STATE_SCANNING;
                    }
                    else
                    {
                        LOGE(" ,start scan failed, %d", hal_ble_scan_env.scan_status);
                        ret_status = -1;
                    }
                }
                else
                {
                    LOGE("can not start scan, %d", hal_ble_scan_env.scan_status);
                    ret_status = -1;
                }

                break;

            case CMD_BLE_STOP_SCAN:
                if (BLE_HAL_SCAN_STATE_SCANNING == hal_ble_scan_env.scan_status)
                {
                    if (BK_ERR_BLE_SUCCESS == bk_ble_stop_scaning(hal_ble_scan_env.scan_idx, hal_ble_cmd_cb))
                    {
                        ret_status = bk_wait_for_ble_idle(BLE_SYNC_CMD_TIMEOUT_MS);
                    }
                    else
                    {
                        LOGE("stop scan err");
                    }
                }
                else
                {
                    LOGD("scan has not been started, %d", hal_ble_scan_env.scan_status);
                    ret_status = -1;
                }

                break;

            case CMD_BLE_CONN_DIS_CONN:
                if (HAL_CONN_STATE_CONNECTED == hal_ble_con_env.con_dev[msg.idx].con_status)
                {
                    if (bk_ble_disconnect(msg.idx) == BK_ERR_BLE_SUCCESS)
                    {
                        hal_ble_con_env.con_dev[msg.idx].con_status = HAL_CONN_STATE_DISCONNECTING;

                        if (hal_ble_con_env.con_dev[msg.idx].role == LINK_ROLE_MASTER)
                        {
                            bk_wait_for_ble_idle((hal_ble_con_env.con_dev[msg.idx].sup_to*10 + BLE_SYNC_CMD_TIMEOUT_MS));
                        }
                    }
                }
                else
                {
                    LOGD("the link(%d) is not connected, status %d", msg.idx, hal_ble_con_env.con_dev[msg.idx].con_status);
                    ret_status = -1;
                }

                break;
#if 0

            case CMD_BLE_INIT_STOP_CONN:
                if (BK_ERR_BLE_SUCCESS == bk_ble_init_stop_conn(msg.idx, hal_ble_cmd_cb))
                {
                    bk_wait_for_ble_idle(BLE_SYNC_CMD_TIMEOUT_MS);
                }

                break;
#endif

            case CMD_BLE_INIT_CREATE:
                if (BLE_HAL_CONN_DEV_STATE_IDLE == hal_ble_con_env.init_status)
                {
                    hal_ble_env.dev_state.gap_conn_state = GAP_CONN_DEV_STATE_INITIATING;
                    bk_adapter_notify_bt_status(GAP_MSG_LE_DEV_STATE_CHANGE, NULL);

                    if (BK_ERR_BLE_SUCCESS == bk_ble_create_init(hal_ble_con_env.init_idx, &hal_ble_con_env.conn_param, hal_ble_cmd_cb))
                    {
                        hal_ble_con_env.init_status = BLE_HAL_CONN_DEV_STATE_INITIATING;
                        ret_status = bk_wait_for_ble_idle(BLE_SYNC_CMD_TIMEOUT_MS);
                    }
                }
                else
                {
                    LOGD("INIT_CREATE error, %d", hal_ble_con_env.init_status);
                    ret_status = -1;
                }

                break;

            case CMD_BLE_DELETE_THREAD:
            {
                exit_sem = msg.param;
                goto exit;
            }
            break;

            default:
                break;
            }

            if (msg.cb)
            {
                msg.cb(msg.event, ret_status, msg.param);
            }

            if (msg.param && msg.size)
            {
                os_free(msg.param);
                msg.param = NULL;
            }
        }
    }

exit:
    LOGD("thread end");

    if (exit_sem)
    {
        rtos_set_semaphore(exit_sem);
    }

    rtos_delete_thread(NULL);
}

static void ble_delete_all_thread(void)
{
    int32_t ret = 0;

    beken_semaphore_t exit_sem = NULL;

    ret = rtos_init_semaphore(&exit_sem, 1);

    if (ret)
    {
        LOGE("init exit_sem err %d", ret);
    }

    if (ble_cmd_handle)
    {
        ble_cmd_queue_push_ext(CMD_BLE_DELETE_THREAD, 0xff, &exit_sem, 0, NULL);

        LOGW("wait cmd thread end");

        ret = rtos_get_semaphore(&exit_sem, BEKEN_WAIT_FOREVER);

        if (ret)
        {
            LOGE("wait exit_sem %d err", ret);
        }

        LOGW("cmd thread end completed");

        if (ble_cmd_que)
        {
            struct ble_cmd_msg msg;

            while (!rtos_pop_from_queue(&ble_cmd_que, &msg, BEKEN_NO_WAIT))
            {
                if (msg.param && msg.size)
                {
                    os_free(msg.param);
                    msg.param = NULL;
                }
            }

            rtos_deinit_queue(&ble_cmd_que);
            ble_cmd_que = NULL;
        }

        if (g_ble_cmd_sema)
        {
            rtos_deinit_semaphore(&g_ble_cmd_sema);
            g_ble_cmd_sema = NULL;
        }

        ble_cmd_handle = NULL;
    }

    if (ble_evt_handle)
    {
        ble_evt_queue_push_ext(EVT_BLE_TASK_EXIT, &exit_sem, 0, NULL);

        LOGW("wait evt thread end");

        ret = rtos_get_semaphore(&exit_sem, BEKEN_WAIT_FOREVER);

        if (ret)
        {
            LOGE("wait exit_sem %d err", ret);
        }

        LOGW("evt thread end completed");

        if (ble_evt_que)
        {
            ble_evt_msg_t msg;

            while (!rtos_pop_from_queue(&ble_evt_que, &msg, BEKEN_NO_WAIT))
            {
                if (msg.u.buf && msg.buff_size)
                {
                    os_free(msg.u.buf);
                    msg.u.buf = NULL;
                }
            }

            rtos_deinit_queue(&ble_evt_que);
        }

        ble_evt_handle = NULL;
    }

    if (exit_sem)
    {
        rtos_deinit_semaphore(&exit_sem);
    }
}

static void bk_adapter_ble_stack_start(void)
{
    uint8_t result = 0;

    if (!ble_cmd_que && !ble_cmd_handle && !ble_evt_handle && !ble_evt_que && !g_ble_cmd_sema)
    {

        result = rtos_init_semaphore(&g_ble_cmd_sema, 1);

        BK_ASSERT(0 == result);

        result = rtos_init_queue(&ble_evt_que,
                                 "ble_evt_queue",
                                 sizeof(ble_evt_msg_t),
                                 160);
        BK_ASSERT(0 == result);

        result = rtos_create_thread(&ble_evt_handle,
                                    4,
                                    "bk_ble_evt",
                                    (beken_thread_function_t)hal_ble_evt_thread,
                                    4096,
                                    (beken_thread_arg_t)0);

        result = rtos_init_queue(&ble_cmd_que,
                                 "ble_cmd_queue",
                                 sizeof(struct ble_cmd_msg),
                                 10);

        BK_ASSERT(0 == result);

        result = rtos_create_thread(&ble_cmd_handle,
                                    4,
                                    "bk_ble_cmd",
                                    (beken_thread_function_t)hal_ble_cmd_thread,
                                    1024 * 4,
                                    (beken_thread_arg_t)0);

        BK_ASSERT(0 == result);
    }

}

static void bk_adapter_ble_stack_stop(void)
{
    ble_delete_all_thread();
    hal_ble_env.dev_state.gap_init_state = GAP_INIT_STATE_INIT;
}

static void bk_adapter_ble_hal_env_init(void)
{
    int32_t ret = 0;

    os_memset(&hal_ble_con_env, 0, sizeof(hal_ble_con_env));

    for (uint8_t i = 0; i < HAL_BLE_CON_NUM; i++)
    {
        hal_ble_con_env.con_dev[i].con_status = HAL_CONN_STATE_DISCONNECTED;
        hal_ble_con_env.con_dev[i].role = LINK_ROLE_UNDEFINED;
        //hal_ble_con_env.con_dev[i].notify_list = ble_list_new();

        ret = rtos_init_mutex(&hal_ble_con_env.con_dev[i].notify_list_mutex);

        if (ret)
        {
            LOGE("init mutex err %d index %d !!!", ret, i);
        }

        hal_ble_con_env.con_dev[i].rd_list = ble_list_new();
        hal_ble_con_env.con_dev[i].wr_list = ble_list_new();
        hal_ble_con_env.con_dev[i].dis_list = ble_list_new();
        hal_ble_con_env.con_dev[i].relate_adv_index = 0xff;
        hal_ble_con_env.con_dev[i].auth_state = 0xff;
    }

    hal_ble_con_env.init_idx = 0xFF;
    hal_ble_con_env.init_status = BLE_HAL_CONN_DEV_STATE_IDLE;
    hal_ble_con_env.conn_scan_timeout = BLE_CON_SCAN_TIMEOUT;
    hal_ble_con_env.conn_param.intv_min = BLE_CON_INTERVAL_MIN;
    hal_ble_con_env.conn_param.intv_max = BLE_CON_INTERVAL_MAX;
    hal_ble_con_env.conn_param.con_latency = BLE_CON_CON_LATENCY;
    hal_ble_con_env.conn_param.sup_to = BLE_CON_CON_TIMEOUT;
    hal_ble_con_env.conn_param.init_phys = INIT_PHY_TYPE_LE_1M;
    hal_ble_con_env.conn_param.filter_policy = INIT_TYPE_LE_DIRECT_CONN_EST;


    hal_ble_env.dev_state.gap_init_state = GAP_INIT_STATE_INIT;
    hal_ble_env.dev_state.gap_adv_sub_state = GAP_ADV_TO_IDLE_CAUSE_STOP;
    hal_ble_env.dev_state.gap_adv_state = GAP_ADV_STATE_IDLE;
    hal_ble_env.dev_state.gap_scan_state = GAP_SCAN_STATE_IDLE;
    hal_ble_env.dev_state.gap_conn_state = GAP_CONN_DEV_STATE_IDLE;
    hal_ble_env.auth_flags = GAP_AUTH_BOND;
    hal_ble_env.sec_req = GAP_SEC1_NOAUTH_PAIR_ENC;
    hal_ble_env.auth_io_cap = BK_BLE_GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
    hal_ble_env.auth_oob = 0;
    hal_ble_env.app_evt_queue_handle = 0;
    hal_ble_env.app_io_queue_handle = 0;
    hal_ble_env.slave_connected = 0;
    hal_ble_env.max_services = 0;
    hal_ble_env.client_idx = 0;
    hal_ble_env.client_num = 0;
    hal_ble_env.master_count = 0;

    hal_ble_scan_env.scan_idx = 0xFF;
    hal_ble_scan_env.scan_status = BLE_HAL_SCAN_STATE_IDLE;
    hal_ble_scan_env.scan_param.own_addr_type = OWN_ADDR_TYPE_PUBLIC_ADDR;
    hal_ble_scan_env.scan_param.scan_phy = INIT_PHY_TYPE_LE_1M;
    hal_ble_scan_env.scan_param.scan_intv = BLE_SCAN_INTERVAL;
    hal_ble_scan_env.scan_param.scan_wd = BLE_SCAN_WINDOW;
    hal_ble_scan_env.scan_param.scan_type = ACTIVE_SCANNING;
    hal_ble_scan_env.filt_duplicate = 0;
    hal_ble_scan_env.duration = 0;
    hal_ble_scan_env.period = 0;
    hal_ble_scan_env.curr_pos = 0;
    hal_ble_scan_env.nb_adv = 0;
    hal_ble_scan_env.table = (struct adv_dup_filter_entry *)os_zalloc(sizeof(struct adv_dup_filter_entry) * BLE_HAL_FILTER_DUPLICATES_MAX);

    if (0 == hal_ble_scan_env.table)
    {
        LOGE("hal_ble_scan_env.table malloc failed!!!");
        return;
    }
}

static void bk_adapter_ble_hal_env_deinit(void)
{
    for (uint8_t i = 0; i < HAL_BLE_CON_NUM; i++)
    {
        if (hal_ble_con_env.con_dev[i].notify_list_mutex)
        {
            rtos_deinit_mutex(&hal_ble_con_env.con_dev[i].notify_list_mutex);
            hal_ble_con_env.con_dev[i].notify_list_mutex = NULL;
        }

        if (hal_ble_con_env.con_dev[i].wr_list)
        {
            ble_list_free(hal_ble_con_env.con_dev[i].wr_list);
        }

        if (hal_ble_con_env.con_dev[i].notify_list)
        {
            ble_list_free(hal_ble_con_env.con_dev[i].notify_list);
        }

        if (hal_ble_con_env.con_dev[i].rd_list)
        {
            ble_list_free(hal_ble_con_env.con_dev[i].rd_list);
        }

        if (hal_ble_con_env.con_dev[i].dis_list)
        {
            ble_list_free(hal_ble_con_env.con_dev[i].dis_list);
        }
    }

    if (hal_ble_scan_env.table)
    {
        os_free(hal_ble_scan_env.table);
        hal_ble_scan_env.table = NULL;
    }
}

int le_bond_pair(uint8_t conn_id)
{
    LOGD("conn_id %d", conn_id);

    if (BK_ERR_BLE_SUCCESS == bk_ble_create_bond_ext(conn_id, hal_ble_env.auth_flags, hal_ble_env.auth_io_cap, hal_ble_env.sec_req, hal_ble_env.auth_oob, BK_BLE_GAP_KDIST_ENCKEY, BK_BLE_GAP_KDIST_IDKEY | BK_BLE_GAP_KDIST_ENCKEY))
    {
        T_LE_GAP_MSG_DATA msg_data = {0};
        msg_data.gap_authen_state.conn_id = conn_id;
        msg_data.gap_authen_state.new_state = GAP_AUTHEN_STATE_STARTED;
        msg_data.gap_authen_state.status = 0;
        bk_adapter_notify_bt_status(GAP_MSG_LE_AUTHEN_STATE_CHANGE, &msg_data);
    }
    else
    {
        LOGE("bk_ble_create_bond failed!!!");
        return 1;
    }

    return 0;
}


///static callback
static void bk_adapter_ble_notice_cb(ble_notice_t notice, void *param)
{
    // int32_t ret = 0;  // Unused variable

    switch (notice)
    {
    case BLE_5_WRITE_EVENT:
    case BLE_5_READ_EVENT:
    case BLE_5_READ_BLOB_EVENT:
        bk_tr_ble_peripheral_notice_cb(notice, param);
        break;

    case BLE_5_REPORT_ADV:
    {
        bk_tr_ble_scanner_notice_cb(notice, param);
    }
    break;

    case BLE_5_MTU_CHANGE:
    case BLE_5_CONNECT_EVENT:
    case BLE_5_DISCONNECT_EVENT:
        bk_tr_ble_peripheral_notice_cb(notice, param);
        break;


    case BLE_5_INIT_CONNECT_EVENT:
    case BLE_5_INIT_DISCONNECT_EVENT:
    case BLE_5_INIT_CONNECT_FAILED_EVENT:
    {
        bk_tr_ble_client_notice_cb(notice, param);
    }
    break;

    case BLE_5_ATT_INFO_REQ:
    {
        bk_tr_ble_peripheral_notice_cb(notice, param);
    }
    break;

    case BLE_5_CREATE_DB:
    {
        bk_tr_ble_peripheral_notice_cb(notice, param);
    }
    break;

    case BLE_5_SDP_REGISTER_FAILED:
        LOGD("BLE_5_SDP_REGISTER_FAILED");
        break;

    case BLE_5_READ_PHY_EVENT:
    {
        // ble_read_phy_t *phy_param = (ble_read_phy_t *)param;  // Unused variable
        LOGD("BLE_5_READ_PHY_EVENT:tx_phy:0x%02x, rx_phy:0x%02x", ((ble_read_phy_t *)param)->tx_phy, ((ble_read_phy_t *)param)->rx_phy);
        break;
    }

    case BLE_5_TX_DONE:
    {
        bk_tr_ble_peripheral_notice_cb(notice, param);
        break;
    }

    case BLE_5_CONN_UPDATA_EVENT:
    {
        ble_conn_param_t *updata_param = (ble_conn_param_t *)param;
        LOGD("BLE_5_CONN_UPDATA_EVENT:conn_idx %d, conn_interval:0x%04x, con_latency:0x%04x, sup_to:0x%04x", updata_param->conn_idx,
             updata_param->intv_max, updata_param->con_latency, updata_param->sup_to);

        hal_ble_con_env.con_dev[updata_param->conn_idx].intv = updata_param->intv_max;
        hal_ble_con_env.con_dev[updata_param->conn_idx].con_latency = updata_param->con_latency;
        hal_ble_con_env.con_dev[updata_param->conn_idx].sup_to = updata_param->sup_to;

        break;
    }

    case BLE_5_PAIRING_REQ:
    {
        ble_smp_ind_t *s_ind = (ble_smp_ind_t *)param;
        uint8_t accept = (bktr_ble_server_get_param()->is_secured_connect_allowed ? 1 : 0);
        LOGD("BLE_5_PAIRING_REQ conn_idx:%d accept %d", s_ind->conn_idx, accept);

        bk_ble_sec_send_auth_mode_ext(s_ind->conn_idx, hal_ble_env.auth_flags, hal_ble_env.auth_io_cap, hal_ble_env.sec_req, hal_ble_env.auth_oob,
                                      BK_BLE_GAP_KDIST_ENCKEY | BK_BLE_GAP_KDIST_IDKEY,
                                      BK_BLE_GAP_KDIST_ENCKEY,
                                      accept);
        {
            T_LE_GAP_MSG_DATA msg_data = {0};
            msg_data.gap_authen_state.conn_id = s_ind->conn_idx;
            msg_data.gap_authen_state.new_state = GAP_AUTHEN_STATE_STARTED;
            msg_data.gap_authen_state.status = 0;
            bk_adapter_notify_bt_status(GAP_MSG_LE_AUTHEN_STATE_CHANGE, &msg_data);
        }

        break;
    }

    case BLE_5_PARING_PASSKEY_INPUT_REQ:
    {
        LOGI("BLE_5_PARING_PASSKEY_INPUT_REQ ");
        ble_smp_ind_t *smp_ind = (ble_smp_ind_t *)param;
        LOGI("Please input the auth passkey get from remote device, con_idx:%d ", smp_ind->conn_idx);
        if (hal_ble_env.use_fixed_key)
        {
            LOGD("use fixed key, input passkey to fixed:%06d ", hal_ble_env.fixed_key);
            bk_ble_passkey_send(smp_ind->conn_idx, 1, hal_ble_env.fixed_key);
        }else
        {
            LOGE("Not Support passkey input!\n");
        }
    }
    break;

    case BLE_5_PARING_PASSKEY_DISPLAY_REQ:
    {
        LOGI("BLE_5_PARING_PASSKEY_DISPLAY_REQ ");
        ble_smp_ind_t *smp_ind = (ble_smp_ind_t *)param;

        if (hal_ble_env.use_fixed_key)
        {
            smp_ind->num = hal_ble_env.fixed_key;
            LOGD("use fixed key, change passkey to fixed:%06d ", smp_ind->num);
        }

        LOGI("Auth Passkey display request, conn_idx:%d, passkey: %06d ", smp_ind->conn_idx, smp_ind->num);
    }
    break;

    case BLE_5_PARING_NUMBER_COMPARE_REQ_EVENT:
    {
        LOGD("BLE_5_PARING_NUMBER_COMPARE_REQ_EVENT ");
        ble_smp_ind_t *s_ind = (ble_smp_ind_t *)param;
        LOGI("BLE_5_PARING_NUMBER_COMPARE_REQ_EVENT conn_idx:%d, passkey:%06d ", s_ind->conn_idx, s_ind->num);
        ble_auth_key_cfm_t *key_cfm_ind = os_zalloc(sizeof(ble_auth_key_cfm_t));
        key_cfm_ind->con_idx = s_ind->conn_idx;
        key_cfm_ind->passkey = s_ind->num;

        if (key_cfm_ind)
        {
            if (ble_evt_queue_push(EVT_BLE_PAIRING_NUMBER_COMPARE_REQ_MSG, key_cfm_ind) != 0)
            {
                LOGE("ble_evt_queue_push failed ");
                os_free(key_cfm_ind);
            }
        }
        else
        {
            LOGE("key_cfm_ind malloc fail ");
        }
    }
    break;

    case BLE_5_ENCRYPT_EVENT:
    {
        ble_smp_ind_t *smp_ind = (typeof(smp_ind))param;
        hal_ble_conn_t *info = (hal_ble_conn_t *)le_get_conn_info_ext(smp_ind->conn_idx);

        LOGD("BLE_5_ENCRYPT_EVENT conn_idx %d status 0x%x", smp_ind->conn_idx, smp_ind->status);

        if (!info)
        {
            LOGE("can't find conn_idx %d info", smp_ind->conn_idx);
            break;
        }

        if ((smp_ind->status & GAP_AUTH_BOND) && info->role == LINK_ROLE_SLAVE)
        {
            bk_tr_ble_server_report_connected_evt(smp_ind->conn_idx, TRBLE_SERVER_SM_CONNECTED);
        }
        else
        {
            LOGD("auth level: bond:%d mimt:%d sec_con:%d", (smp_ind->status &GAP_AUTH_BOND), (smp_ind->status &GAP_AUTH_MITM), (smp_ind->status &GAP_AUTH_SEC_CON));
            T_LE_GAP_MSG_DATA msg_data = {0};
            msg_data.gap_authen_state.conn_id = smp_ind->conn_idx;
            msg_data.gap_authen_state.new_state = GAP_AUTHEN_STATE_COMPLETE;
            msg_data.gap_authen_state.status = 0;
            bk_adapter_notify_bt_status(GAP_MSG_LE_AUTHEN_STATE_CHANGE, &msg_data);
        }
    }
    break;

    case BLE_5_PAIRING_SUCCEED:
    {
        ble_smp_ind_t *smp_ind = (ble_smp_ind_t *)param;
        hal_ble_conn_t *info = (hal_ble_conn_t *)le_get_conn_info_ext(smp_ind->conn_idx);

        LOGD("BLE_5_PAIRING_SUCCEED conn_idx %d status 0x%x", smp_ind->conn_idx, smp_ind->status);

        if (!info)
        {
            LOGE("can't find conn_idx %d info", smp_ind->conn_idx);
            break;
        }

        if (!smp_ind->status && info->role == LINK_ROLE_SLAVE)
        {
            bk_tr_ble_server_report_connected_evt(smp_ind->conn_idx, TRBLE_SERVER_SM_CONNECTED);
        }

        {
            T_LE_GAP_MSG_DATA msg_data = {0};
            msg_data.gap_authen_state.conn_id = smp_ind->conn_idx;
            msg_data.gap_authen_state.new_state = GAP_AUTHEN_STATE_COMPLETE;
            msg_data.gap_authen_state.status = 0;
            bk_adapter_notify_bt_status(GAP_MSG_LE_AUTHEN_STATE_CHANGE, &msg_data);
        }
    }
    break;

    case BLE_5_PAIRING_FAILED:
    {
        ble_smp_ind_t *smp_ind = (ble_smp_ind_t *)param;
        LOGD("BLE_5_PAIRING_FAILED, conn_idx %d, status 0x%x", smp_ind->conn_idx, smp_ind->status);

        {
            T_LE_GAP_MSG_DATA msg_data = {0};
            msg_data.gap_authen_state.conn_id = smp_ind->conn_idx;
            msg_data.gap_authen_state.new_state = GAP_AUTHEN_STATE_COMPLETE;
            msg_data.gap_authen_state.status = (SM_ERR | SM_ERR_UNKNOWN);
            bk_adapter_notify_bt_status(GAP_MSG_LE_AUTHEN_STATE_CHANGE, &msg_data);

            le_bond_delete_by_bd(hal_ble_con_env.con_dev[smp_ind->conn_idx].peer_addr, hal_ble_con_env.con_dev[smp_ind->conn_idx].peer_addr_type);
        }

    }
    break;

    case BLE_5_ADV_STOPPED_EVENT:
    {
        bk_tr_ble_advertiser_notice_cb(notice, param);
    }
    break;

    case BLE_5_READ_RSSI_CMPL_EVENT:
    {
        // ble_read_rssi_rsp_t *rssi_rsp = (ble_read_rssi_rsp_t *)param;  // Unused variable

        LOGD("BLE_5_READ_RSSI_CMPL_EVENT, conn_idx %d,rssi %d", ((ble_read_rssi_rsp_t *)param)->conn_idx, ((ble_read_rssi_rsp_t *)param)->rssi);
#if 0

        if (hal_ble_env.le_app_cb)
        {
            T_LE_CB_DATA cb_data;
            T_LE_READ_RSSI_RSP rssi_data;
            rssi_data.conn_id = rssi_rsp->conn_idx;
            rssi_data.rssi = rssi_rsp->rssi;
            rssi_data.cause = 0;
            cb_data.p_le_read_rssi_rsp = &rssi_data;
            hal_ble_env.le_app_cb(GAP_MSG_LE_READ_RSSI, &cb_data);
        }

#endif
    }
    break;

    case BLE_5_CONN_UPD_PAR_ASK:
    {
        ble_conn_update_para_ind_t *tmp = (typeof(tmp))param;
        LOGD(" BLE_5_CONN_UPD_PAR_ASK accept");
        tmp->is_agree = 1;
#if 0

        if (hal_ble_env.le_app_cb)
        {
            T_LE_CB_DATA cb_data;
            T_LE_CONN_UPDATE_IND update_ind;
            update_ind.conn_id = tmp->conn_idx;
            update_ind.conn_interval_min = tmp->intv_min;
            update_ind.conn_interval_max = tmp->intv_max;
            update_ind.conn_latency = tmp->latency;
            update_ind.supervision_timeout = tmp->time_out;

            cb_data.p_le_conn_update_ind = &update_ind;
            hal_ble_env.le_app_cb(GAP_MSG_LE_CONN_UPDATE_IND, &cb_data);
        }

#endif
    }
    break;

    case BLE_5_KEY_EVENT:
    {
        LOGD("BLE_5_KEY_EVENT");
        bk_ble_key_t ble_enc_key = *((bk_ble_key_t *)param);

        LOGI("penc %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            ble_enc_key.penc.ltk[0],
            ble_enc_key.penc.ltk[1],
            ble_enc_key.penc.ltk[2],
            ble_enc_key.penc.ltk[3],
            ble_enc_key.penc.ltk[4],
            ble_enc_key.penc.ltk[5],
            ble_enc_key.penc.ltk[6],
            ble_enc_key.penc.ltk[7],
            ble_enc_key.penc.ltk[8],
            ble_enc_key.penc.ltk[9],
            ble_enc_key.penc.ltk[10],
            ble_enc_key.penc.ltk[11],
            ble_enc_key.penc.ltk[12],
            ble_enc_key.penc.ltk[13],
            ble_enc_key.penc.ltk[14],
            ble_enc_key.penc.ltk[15]);

        LOGI("pidk %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            ble_enc_key.pirk.irk[0],
            ble_enc_key.pirk.irk[1],
            ble_enc_key.pirk.irk[2],
            ble_enc_key.pirk.irk[3],
            ble_enc_key.pirk.irk[4],
            ble_enc_key.pirk.irk[5],
            ble_enc_key.pirk.irk[6],
            ble_enc_key.pirk.irk[7],
            ble_enc_key.pirk.irk[8],
            ble_enc_key.pirk.irk[9],
            ble_enc_key.pirk.irk[10],
            ble_enc_key.pirk.irk[11],
            ble_enc_key.pirk.irk[12],
            ble_enc_key.pirk.irk[13],
            ble_enc_key.pirk.irk[14],
            ble_enc_key.pirk.irk[15]);

        LOGI("lenc %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            ble_enc_key.lenc.ltk[0],
            ble_enc_key.lenc.ltk[1],
            ble_enc_key.lenc.ltk[2],
            ble_enc_key.lenc.ltk[3],
            ble_enc_key.lenc.ltk[4],
            ble_enc_key.lenc.ltk[5],
            ble_enc_key.lenc.ltk[6],
            ble_enc_key.lenc.ltk[7],
            ble_enc_key.lenc.ltk[8],
            ble_enc_key.lenc.ltk[9],
            ble_enc_key.lenc.ltk[10],
            ble_enc_key.lenc.ltk[11],
            ble_enc_key.lenc.ltk[12],
            ble_enc_key.lenc.ltk[13],
            ble_enc_key.lenc.ltk[14],
            ble_enc_key.lenc.ltk[15]);

        if (bk_adapter_bt_storage_save_linkkey_info(&ble_enc_key) >= 0)
        {
            bk_adapter_bt_storage_sync_bond_info_to_flash();
        }

        break;
    }

    case BLE_5_BOND_INFO_REQ_EVENT:
    {
        bk_ble_bond_info_req_t *bond_info_req = (bk_ble_bond_info_req_t *)param;
        uint8_t tem_addr[6];

        LOGD("BLE_5_BOND_INFO_REQ_EVENT, %02x:%02x:%02x:%02x:%02x:%02x(%d)",
            bond_info_req->key.peer_addr[5],
            bond_info_req->key.peer_addr[4],
            bond_info_req->key.peer_addr[3],
            bond_info_req->key.peer_addr[2],
            bond_info_req->key.peer_addr[1],
            bond_info_req->key.peer_addr[0],
            bond_info_req->key.peer_addr_type);

        os_memcpy(tem_addr, bond_info_req->key.peer_addr, 6);

        if (BLE_IS_RPA(bond_info_req->key.peer_addr_type, tem_addr[5]))
        {
            bk_adapter_ble_find_initial_pseudo_addr(tem_addr);
        }

        int32_t index = bk_adapter_bt_storage_find_linkkey_info_index(tem_addr, bond_info_req->key.peer_addr_type);

        if (index >= 0)
        {
            LOGD("bond key is found (%d) ", index);
            bond_info_req->key_found = 1;
            bond_info_req->key = bt_bond_info_storage->linkkey[index];

            LOGI("penc %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
                bond_info_req->key.penc.ltk[0],
                bond_info_req->key.penc.ltk[1],
                bond_info_req->key.penc.ltk[2],
                bond_info_req->key.penc.ltk[3],
                bond_info_req->key.penc.ltk[4],
                bond_info_req->key.penc.ltk[5],
                bond_info_req->key.penc.ltk[6],
                bond_info_req->key.penc.ltk[7],
                bond_info_req->key.penc.ltk[8],
                bond_info_req->key.penc.ltk[9],
                bond_info_req->key.penc.ltk[10],
                bond_info_req->key.penc.ltk[11],
                bond_info_req->key.penc.ltk[12],
                bond_info_req->key.penc.ltk[13],
                bond_info_req->key.penc.ltk[14],
                bond_info_req->key.penc.ltk[15]);

            LOGI("pidk %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
                bond_info_req->key.pirk.irk[0],
                bond_info_req->key.pirk.irk[1],
                bond_info_req->key.pirk.irk[2],
                bond_info_req->key.pirk.irk[3],
                bond_info_req->key.pirk.irk[4],
                bond_info_req->key.pirk.irk[5],
                bond_info_req->key.pirk.irk[6],
                bond_info_req->key.pirk.irk[7],
                bond_info_req->key.pirk.irk[8],
                bond_info_req->key.pirk.irk[9],
                bond_info_req->key.pirk.irk[10],
                bond_info_req->key.pirk.irk[11],
                bond_info_req->key.pirk.irk[12],
                bond_info_req->key.pirk.irk[13],
                bond_info_req->key.pirk.irk[14],
                bond_info_req->key.pirk.irk[15]);

            LOGD("peer id addr, %02x:%02x:%02x:%02x:%02x:%02x(%d)",
                bond_info_req->key.pirk.id_addr[5],
                bond_info_req->key.pirk.id_addr[4],
                bond_info_req->key.pirk.id_addr[3],
                bond_info_req->key.pirk.id_addr[2],
                bond_info_req->key.pirk.id_addr[1],
                bond_info_req->key.pirk.id_addr[0],
                bond_info_req->key.pirk.addr_type);

            LOGI("lenc %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
                bond_info_req->key.lenc.ltk[0],
                bond_info_req->key.lenc.ltk[1],
                bond_info_req->key.lenc.ltk[2],
                bond_info_req->key.lenc.ltk[3],
                bond_info_req->key.lenc.ltk[4],
                bond_info_req->key.lenc.ltk[5],
                bond_info_req->key.lenc.ltk[6],
                bond_info_req->key.lenc.ltk[7],
                bond_info_req->key.lenc.ltk[8],
                bond_info_req->key.lenc.ltk[9],
                bond_info_req->key.lenc.ltk[10],
                bond_info_req->key.lenc.ltk[11],
                bond_info_req->key.lenc.ltk[12],
                bond_info_req->key.lenc.ltk[13],
                bond_info_req->key.lenc.ltk[14],
                bond_info_req->key.lenc.ltk[15]);
        }
        else
        {
            LOGD("bond key is not found");
        }

        break;
    }

    case BLE_5_GAP_CMD_CMP_EVENT:
    {
        ble_cmd_cmp_evt_t *event = (ble_cmd_cmp_evt_t *)param;
        LOGD("BLE_5_GAP_CMD_CMP_EVENT, cmd %d", event->cmd);

        switch (event->cmd)
        {
        case BLE_CONN_DIS_CONN:
        case BLE_CONN_SET_PHY:
        case BLE_CONN_READ_PHY:
        case BLE_CONN_UPDATE_MTU:
        case BLE_SET_MAX_MTU:
        {
            break;
        }

        case BLE_CONN_UPDATE_PARAM:
        {
            T_LE_GAP_MSG_DATA msg_data = {0};
            msg_data.gap_conn_param_update.conn_id = event->conn_idx;
            msg_data.gap_conn_param_update.status = (event->status ? GAP_CONN_PARAM_UPDATE_STATUS_FAIL : GAP_CONN_PARAM_UPDATE_STATUS_SUCCESS);
            msg_data.gap_conn_param_update.cause = 0;
            bk_adapter_notify_bt_status(GAP_MSG_LE_CONN_PARAM_UPDATE, &msg_data);

            break;
        }

        case BLE_CONN_ENCRYPT:
        {
            LOGD("BLE_CONN_ENCRYPT complete, con_idx %d,status 0x%x", event->conn_idx, event->status);

            if (event->status)
            {
                uint16_t status = (SM_ERR | SM_ERR_UNKNOWN);

                if (event->status == 0x22)
                {
                    status = (SM_ERR | SM_ERR_LINK_KEY_MISSING);
                }
                else if (event->status == 0x98)
                {
                    status = (SM_ERR | SM_ERR_TIMEOUT);
                }

                T_LE_GAP_MSG_DATA msg_data = {0};
                msg_data.gap_authen_state.conn_id = event->conn_idx;
                msg_data.gap_authen_state.new_state = GAP_AUTHEN_STATE_COMPLETE;
                msg_data.gap_authen_state.status = status;

                bk_adapter_notify_bt_status(GAP_MSG_LE_AUTHEN_STATE_CHANGE, &msg_data);

                if (event->status == 0x22)
                {
                    le_bond_delete_by_bd(hal_ble_con_env.con_dev[event->conn_idx].peer_addr, hal_ble_con_env.con_dev[event->conn_idx].peer_addr_type);
                    if (hal_ble_con_env.con_dev[event->conn_idx].role == LINK_ROLE_MASTER)
                    {
                        if (hal_ble_con_env.is_secured_connect)
                        {
                            LOGI("re-start bond");
                            le_bond_pair(event->conn_idx);
                        }
                    }
                }
            }

            break;
        }

        case BLE_ADD_WHITE_LIST:
        case BLE_RMV_WHITE_LIST:
        case BLE_CLE_WHITE_LIST:
        {
            LOGD("whitelist operation completed, status 0x%x", event->status);

            if (g_ble_whitelist_sema != NULL)
            {
                rtos_set_semaphore(&g_ble_whitelist_sema);
            }

            break;
        }

        default:
            break;
        }
    }
    break;

    case BLE_5_PAIRING_SECURITY_REQ_EVENT:
    {
        ble_smp_ind_t *s_ind = (ble_smp_ind_t *)param;

        bk_ble_create_bond(s_ind->conn_idx, hal_ble_env.auth_flags, hal_ble_env.auth_io_cap, GAP_SEC1_NOAUTH_PAIR_ENC, hal_ble_env.auth_oob);
    }
    break;

    case BLE_5_SCAN_STOPPED_EVENT:
    {
        ble_scan_stopped_ind_t *scan_ind = (ble_scan_stopped_ind_t *)param;
        LOGI("Scan stop event, reason:%d ", scan_ind->reason);
        T_LE_GAP_MSG_DATA msg_data = {0};
        msg_data.gap_dev_state_change.new_state.gap_scan_state = GAP_SCAN_STATE_STOP;
        hal_ble_env.dev_state.gap_scan_state = GAP_SCAN_STATE_STOP;

        hal_ble_scan_env.stop_cause = scan_ind->reason;
        bk_adapter_notify_bt_status(GAP_MSG_LE_DEV_STATE_CHANGE, &msg_data);
        hal_ble_scan_env.scan_status = BLE_HAL_SCAN_STATE_STOP;

        if (1 == scan_ind->reason)
        {
            bk_ble_delete_scaning(hal_ble_scan_env.scan_idx, hal_ble_cmd_cb);
        }
    }
    break;

    case BLE_5_COC_REG_COMPL_EVENT:
    case BLE_5_COC_UNREG_COMPL_EVENT:
    case BLE_5_COC_CONNECTION_COMPL_EVENT:
    case BLE_5_COC_DISCCONNECT_COMPL_EVENT:
    case BLE_5_COC_TX_DONE:
    case BLE_5_COC_RX_IND:
    case BLE_5_COC_CONNECT_REQ_EVENT:
    {
        bk_tr_ble_coc_notice_cb(notice, param);
    }
    break;

    default:
        break;
    }
}

static void hal_ble_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param)
{
    uint8_t set_semaphore = 0;
    int32_t err = 0;

    switch (cmd)
    {
    case BLE_CREATE_ADV:
    {
        int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(param->cmd_idx);

        LOGD("BLE_CREATE_ADV,param->cmd_idx:%d hal_adv_index %d", param->cmd_idx, hal_adv_index);

        if (param->status)
        {
            LOGE("status err 0x%x", param->status);
            break;
        }

        hal_ble_adv_env.array[hal_adv_index].adv_status = BLE_HAL_ADV_STATE_CREATED;
        set_semaphore = 1;
    }
    break;

    case BLE_MODIFY_ADV:
    {
        int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(param->cmd_idx);

        LOGD("BLE_MODIFY_ADV param->cmd_idx:%d hal_adv_index %d", param->cmd_idx, hal_adv_index);

        if (param->status)
        {
            LOGE("status err 0x%x", param->status);
            break;
        }

        if (hal_ble_adv_env.array[hal_adv_index].adv_param.own_addr_type == OWN_ADDR_TYPE_RANDOM_ADDR)
        {
            err = bk_ble_set_adv_random_addr(hal_ble_adv_env.array[hal_adv_index].adv_idx, hal_ble_adv_env.array[hal_adv_index].own_addr.addr, hal_ble_cmd_cb);
        }
        else
        {
            err = bk_ble_set_adv_data(hal_ble_adv_env.array[hal_adv_index].adv_idx, hal_ble_adv_env.array[hal_adv_index].advData, hal_ble_adv_env.array[hal_adv_index].advDataLen, hal_ble_cmd_cb);
        }

        if (err)
        {
            LOGE("set adv random addr/set adv data err %d", err);
            break;
        }
    }
    break;

    case BLE_SET_ADV_RANDOM_ADDR:
    {
        int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(param->cmd_idx);

        LOGD("BLE_SET_ADV_RANDOM_ADDR param->cmd_idx:%d hal_adv_index %d", param->cmd_idx, hal_adv_index);

        if (param->status)
        {
            LOGE("status err 0x%x", param->status);
            break;
        }

        if (BLE_HAL_ADV_STATE_CREATED == hal_ble_adv_env.array[hal_adv_index].adv_status)
        {
            err = bk_ble_set_adv_data(hal_ble_adv_env.array[hal_adv_index].adv_idx, hal_ble_adv_env.array[hal_adv_index].advData, hal_ble_adv_env.array[hal_adv_index].advDataLen, hal_ble_cmd_cb);

            if (err)
            {
                LOGE("set adv data err %d", err);
                break;
            }
        }
        else
        {
            set_semaphore = 1;
            LOGE("wrong adv status %d!!!!", hal_ble_adv_env.array[hal_adv_index].adv_status);
        }
    }
    break;

    case BLE_SET_ADV_DATA:
    {
        int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(param->cmd_idx);

        LOGD("BLE_SET_ADV_DATA status 0x%x param->cmd_idx:%d hal_adv_index %d", param->status, param->cmd_idx, hal_adv_index);

        if(param->status)
        {
            LOGE("set adv data err 0x%x !!!", param->status);
            set_semaphore = 1;
            break;
        }

        if (BLE_HAL_ADV_STATE_CREATED == hal_ble_adv_env.array[hal_adv_index].adv_status)
        {
            if (0 == hal_ble_adv_env.array[hal_adv_index].respDataLen ||
                    0 == (ADV_PROP_SCANNABLE_BIT & hal_ble_adv_env.array[hal_adv_index].adv_param.adv_prop))
            {
                err = bk_ble_start_advertising_ext(hal_ble_adv_env.array[hal_adv_index].adv_idx, 0, hal_ble_adv_env.array[hal_adv_index].max_evt, hal_ble_cmd_cb);
            }
            else
            {
                err = bk_ble_set_scan_rsp_data(hal_ble_adv_env.array[hal_adv_index].adv_idx, hal_ble_adv_env.array[hal_adv_index].respData, hal_ble_adv_env.array[hal_adv_index].respDataLen, hal_ble_cmd_cb);
            }

            if (err)
            {
                LOGE("start adv/set scan rsp err %d", err);
                break;
            }
        }
        else
        {
            set_semaphore = 1;
            LOGE("wrong adv status %d!!!!", hal_ble_adv_env.array[hal_adv_index].adv_status);
        }
    }
    break;

    case BLE_SET_RSP_DATA:
    {
        int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(param->cmd_idx);

        LOGD("BLE_SET_RSP_DATA status 0x%x param->cmd_idx:%d hal_adv_index %d", param->status, param->cmd_idx, hal_adv_index);

        if(param->status)
        {
            LOGE("set adv rsp data err 0x%x !!!", param->status);
            set_semaphore = 1;
            break;
        }

        if (BLE_HAL_ADV_STATE_CREATED == hal_ble_adv_env.array[hal_adv_index].adv_status)
        {
            err = bk_ble_start_advertising_ext(hal_ble_adv_env.array[hal_adv_index].adv_idx, 0, hal_ble_adv_env.array[hal_adv_index].max_evt, hal_ble_cmd_cb);

            if (err)
            {
                LOGE("start adv err %d", err);
                break;
            }
        }
        else
        {
            set_semaphore = 1;
            LOGE("wrong adv status %d!!!!", hal_ble_adv_env.array[hal_adv_index].adv_status);
        }
    }
    break;

    case BLE_START_ADV:
    {
        int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(param->cmd_idx);

        LOGD("BLE_START_ADV status 0x%x param->cmd_idx:%d hal_adv_index %d status 0x%x", param->status, param->cmd_idx, hal_adv_index, param->status);

        set_semaphore = 1;

        if(param->status)
        {
            LOGE("start adv err 0x%x !!!", param->status);
            set_semaphore = 1;
            break;
        }

        if (BLE_HAL_ADV_STATE_CREATED == hal_ble_adv_env.array[hal_adv_index].adv_status)
        {
            hal_ble_adv_env.array[hal_adv_index].adv_status = BLE_HAL_ADV_STATE_ADVERTISING;
        }
        else
        {
            LOGE("wrong adv status %d", hal_ble_adv_env.array[hal_adv_index].adv_status);
        }
    }
    break;

    case BLE_STOP_ADV:
    {
        int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(param->cmd_idx);

        LOGD("BLE_STOP_ADV param->cmd_idx:%d hal_adv_index %d status 0x%x", param->cmd_idx, hal_adv_index, param->status);

        if (hal_adv_index < 0)
        {
            LOGE("can't find adv handle %d", param->cmd_idx);
            break;
        }

        set_semaphore = 1;

        if (BLE_HAL_ADV_STATE_CREATED == hal_ble_adv_env.array[hal_adv_index].adv_status ||
                BLE_HAL_ADV_STATE_ADVERTISING == hal_ble_adv_env.array[hal_adv_index].adv_status)
        {
            hal_ble_adv_env.array[hal_adv_index].adv_status = BLE_HAL_ADV_STATE_CREATED;
        }
        else
        {
            LOGE("wrong adv status %d", hal_ble_adv_env.array[hal_adv_index].adv_status);
        }
    }
    break;

    case BLE_DELETE_ADV:
    {
        int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(param->cmd_idx);

        LOGD("BLE_DELETE_ADV param->cmd_idx:%d hal_adv_index %d status 0x%x", param->cmd_idx, hal_adv_index, param->status);

        set_semaphore = 1;

        hal_ble_env.dev_state.gap_adv_state = GAP_ADV_STATE_IDLE;
        hal_ble_env.dev_state.gap_adv_sub_state = GAP_ADV_TO_IDLE_CAUSE_STOP;

        if ((hal_ble_env.slave_connected) && (BLE_HAL_ADV_STATE_STOP != hal_ble_adv_env.array[hal_adv_index].adv_status))
        {
            hal_ble_env.dev_state.gap_adv_sub_state = GAP_ADV_TO_IDLE_CAUSE_CONN;
        }

        bk_adapter_notify_bt_status(GAP_MSG_LE_DEV_STATE_CHANGE, NULL);

        hal_ble_adv_env.array[hal_adv_index].adv_status = BLE_HAL_ADV_STATE_IDLE;
        hal_ble_adv_env.array[hal_adv_index].adv_idx = 0xff;
        hal_ble_adv_env.array[hal_adv_index].deleting = 0;
    }
    break;

    case BLE_CREATE_SCAN:
        LOGD("[BLE_HAL]BLE_CREATE_SCAN...,scan_idx:%d", hal_ble_scan_env.scan_idx);

        if (BLE_HAL_SCAN_STATE_START == hal_ble_scan_env.scan_status)
        {
            bk_ble_start_scaning_ex(hal_ble_scan_env.scan_idx, 0/*hal_ble_scan_env.filt_duplicate*/, hal_ble_scan_env.duration, hal_ble_scan_env.period, hal_ble_cmd_cb);
        }
        else
        {
            set_semaphore = 1;
            LOGE("wrong scan status %d!!!!", hal_ble_scan_env.scan_status);
        }

        break;

    case BLE_START_SCAN:
        LOGD("[BLE_HAL]BLE_START_SCAN...");
        set_semaphore = 1;
        hal_ble_scan_env.scan_status = BLE_HAL_SCAN_STATE_SCANNING;
        break;

    case BLE_STOP_SCAN:
        LOGD("[BLE_HAL]BLE_STOP_SCAN");

        bk_ble_delete_scaning(hal_ble_scan_env.scan_idx, hal_ble_cmd_cb);
        break;

    case BLE_DELETE_SCAN:
        LOGD("[BLE_HAL]BLE_DELETE_SCAN %d", hal_ble_env.dev_state.gap_scan_state);

        if (hal_ble_scan_env.stop_cause == 0)
        {
            set_semaphore = 1;
        }

        hal_ble_scan_env.scan_status = BLE_HAL_SCAN_STATE_IDLE;
        hal_ble_scan_env.scan_idx = 0xff;

        hal_ble_scan_env.nb_adv = 0;
        hal_ble_scan_env.curr_pos = 0;

        T_LE_GAP_MSG_DATA msg_data = {0};
        msg_data.gap_dev_state_change.new_state.gap_scan_state = GAP_SCAN_STATE_IDLE;
        bk_adapter_notify_bt_status(GAP_MSG_LE_DEV_STATE_CHANGE, &msg_data);
        hal_ble_env.dev_state.gap_scan_state = GAP_SCAN_STATE_IDLE;
        break;

    case BLE_CONN_UPDATE_MTU:
        LOGD("[BLE_HAL]BLE_CONN_UPDATE_MTU");
        break;

    case BLE_CONN_UPDATE_PARAM:
        LOGD("[BLE_HAL]BLE_CONN_UPDATE_PARAM");
        break;

    case BLE_CONN_DIS_CONN:
        LOGD("[BLE_HAL]BLE_CONN_DIS_CONN, %d", param->cmd_idx);
        break;

    case BLE_INIT_WRITE_CHAR:
        LOGD("[BLE_HAL]BLE_INIT_WRITE_CHAR");
        break;

    case BLE_INIT_CREATE:
        LOGD("[BLE_HAL]BLE_INIT_CREATE");
        bk_ble_init_set_connect_dev_addr(hal_ble_con_env.init_idx, &hal_ble_con_env.peer_addr, hal_ble_con_env.peer_addr_type);
        bk_ble_init_set_conn_dev_timeout(hal_ble_con_env.init_idx, hal_ble_con_env.conn_scan_timeout);
        bk_ble_init_start_conn(hal_ble_con_env.init_idx, hal_ble_cmd_cb);
        break;

    case BLE_INIT_START_CONN:
        set_semaphore = 1;
        LOGD("[BLE_HAL]BLE_INIT_START_CONN");
        break;

    case BLE_INIT_STOP_CONN:
        LOGD("[BLE_HAL]BLE_INIT_STOP_CONN");
        set_semaphore = 1;
        break;

    case BLE_SET_MAX_MTU:
        LOGD("[BLE_HAL]BLE_SET_MAX_MTU");
        break;

    default:
        LOGE("[BLE_HAL]UNKNOWN CMD %d.", cmd);
        break;
    }

    if (set_semaphore)
    {
        bk_set_ble_cmd_sema();
    }
}

uint8_t le_update_conn_param(uint8_t       conn_id,
                             uint16_t  conn_interval_min,
                             uint16_t  conn_interval_max,
                             uint16_t  conn_latency,
                             uint16_t  supervision_timeout,
                             uint16_t  ce_length_min,
                             uint16_t  ce_length_max)
{
    (void) ce_length_min; // warning: unused parameter 'ce_length_min' [-Wunused-parameter]
    (void) ce_length_max; // warning: unused parameter 'ce_length_max' [-Wunused-parameter]

    LOGD("conn_id %d, intv_min %d, intv_max %d, con_latency %d, sup_to %d", conn_id,
         conn_interval_min, conn_interval_max, conn_latency, supervision_timeout);

    ble_conn_param_t conn_param;
    conn_param.intv_min = conn_interval_min;
    conn_param.intv_max = conn_interval_max;
    conn_param.con_latency = conn_latency;
    conn_param.sup_to = supervision_timeout;
    int ret = bk_ble_update_param(conn_id, &conn_param);

    if (ret == BK_ERR_BLE_SUCCESS)
    {
        T_LE_GAP_MSG_DATA msg_data = {0};
        msg_data.gap_conn_param_update.conn_id = conn_id;
        msg_data.gap_conn_param_update.status = GAP_CONN_PARAM_UPDATE_STATUS_PENDING;
        msg_data.gap_conn_param_update.cause = 0;
        bk_adapter_notify_bt_status(GAP_MSG_LE_CONN_PARAM_UPDATE, &msg_data);
    }
    else
    {
        LOGE("conn_id %d, update param failed, ret %d", conn_id, ret);
        return 1;
    }

    return 0;
}

///common
trble_result_e bktr_ble_init(trble_client_init_config *init_client, trble_server_init_config *init_server)
{
    int32_t ret = 0;

    if (init_client == NULL)
    {
        LOGE("param err");
        return TRBLE_FAIL;
    }
    else if (is_ble_init != false)
    {
        LOGE("already init !!!");
        return TRBLE_INVALID_STATE;
    }

    LOGW("start");

    bk_ble_set_notice_cb(bk_adapter_ble_notice_cb);

    ret = bk_bluetooth_init();

    if (ret)
    {
        LOGE("bk_bluetooth_init err");
        return TRBLE_FAIL;
    }
    bk_adapter_bt_storage_init();

    bk_adapter_ble_stack_start();
    bk_adapter_ble_hal_env_init();

    bk_tr_ble_common_init();
    bk_tr_ble_client_init(init_client);
    bk_tr_ble_server_init(init_server);
    bk_tr_ble_advertiser_init();
    bk_tr_ble_scanner_init();
    bk_tr_ble_coc_init_private();

    bk_ble_set_max_mtu(init_client->mtu);

    bk_adapter_ble_whitelist_init();
    is_ble_init = true;

    LOGW("end");

    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_deinit(void)
{
    if (!is_ble_init)
    {
        LOGE("already deinit !!!");
        return TRBLE_FAIL;
    }

    LOGW("start");

    bk_tr_ble_coc_deinit_private();
    bk_tr_ble_scanner_deinit();
    bk_tr_ble_advertiser_deinit();
    bk_tr_ble_server_deinit();
    bk_tr_ble_client_deinit();
    bk_tr_ble_common_deinit();

    bk_bluetooth_deinit();

    bk_adapter_ble_stack_stop();

    bk_adapter_ble_hal_env_deinit();

    bk_adapter_bt_storage_deinit();

    bk_adapter_ble_whitelist_deinit();

    is_ble_init = false;

    LOGW("end");

    return TRBLE_SUCCESS;
}
