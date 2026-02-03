#include <tinyara/net/if/ble.h>
#include "ble_tizenrt_impl.h"

#include "bk_ble_tizenrt.h"
#include "ble_tizenrt_peripheral.h"

#include "components/bluetooth/bk_ble_types.h"
#include "components/bluetooth/bk_ble.h"
#include "components/bluetooth/bk_dm_bluetooth.h"

#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

#define LOG_TAG "bkble_server"
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define LOGE(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_ERROR)   BK_BLE_LOGE(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGW(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_WARNING) BK_BLE_LOGW(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGI(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_INFO)    BK_BLE_LOGI(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGD(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_DEBUG)   BK_BLE_LOGD(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGV(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_VERBOSE) BK_BLE_LOGV(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)

#define API_SEM_TIMEOUT 7000
#define FIRST_PRF_ID_OFFSET 0
#define ALLOC_ATTR_DATA 0

#define ATTR_MAX_BUFFER_LEN 512

#define BLE_TIZENRT_READ_MAX_LEN 20 //520
#define TIZENRT_MAX_VAL_LEN 520
#define NOTIFY_SYNC_API 1
#define NOTIFY_ASYNC_MAX_COUNT 5

#define BK_BLE_MIN(x, y) (((x) < (y)) ? (x) : (y))

struct attr_db_addon
{
    trble_server_cb_t cb;
    void *arg;

    uint16_t attr_handle;
    uint8_t app_reject; //note: maybe used in future T_APP_RESULT
    uint8_t *peer_write_buffer;
    uint32_t peer_write_buffer_current_len;
    uint32_t peer_write_buffer_max_len;
    uint16_t cccd_config;
};

struct service_elem
{
    struct bk_ble_db_cfg cfg;
    struct attr_db_addon *db_addon;
};

typedef struct
{
    uint8_t init;
    trble_server_init_config server_init_parm;

    uint32_t db_attr_count;
    uint32_t service_count;
    struct service_elem *service_array;

    uint8_t default_read_buffer[BLE_TIZENRT_READ_MAX_LEN]; //note: see rtk ble_tizenrt_read_val
    uint8_t default_write_buffer[TIZENRT_MAX_VAL_LEN]; //note: see rtk tizenrt_ble_write_value

    beken_semaphore_t sem;
} peripheral_ctb_t;

static peripheral_ctb_t s_ctb;

static void release_all_db(void);

uint16_t bktr_ble_server_get_profile_count(void)
{
    return s_ctb.server_init_parm.profile_count;
}

trble_server_init_config *bktr_ble_server_get_param(void)
{
    return &s_ctb.server_init_parm;
}

static struct service_elem *find_service_by_prf_id(uint8_t prf_id)
{
    for (int i = 0; i < s_ctb.service_count; ++i)
    {
        if (s_ctb.service_array[i].cfg.prf_task_id == prf_id)
        {
            return &s_ctb.service_array[i];
        }
    }

    return NULL;
}

static uint16_t att_index_2_attr_handle(uint16_t prf_task_id, uint16_t att_index)
{
    if (!s_ctb.init)
    {
        LOGE("already deinit");
        return 0;
    }

    if (!s_ctb.db_attr_count || !s_ctb.service_count || !s_ctb.service_array)
    {
        LOGE("ctx err");
        return 0;
    }

    struct service_elem *service_elem = find_service_by_prf_id(prf_task_id);

    if (!service_elem || service_elem->cfg.att_db_nb <= att_index)
    {
        LOGE("cfg or att_index out range, %p %d", service_elem, att_index);
        return 0;
    }

    //todo: check attr handle correct
    return service_elem->cfg.start_hdl + att_index;
}

static int8_t attr_handle_2_att_index(uint16_t attr_handle, uint16_t *prf_task_id, uint16_t *att_index, uint32_t *service_index)
{
    if (!s_ctb.init)
    {
        LOGE("already deinit");
        return -1;
    }

    if (!attr_handle)
    {
        LOGE("attr_handle 0 !!!");
        return -1;
    }

    if (!s_ctb.db_attr_count || !s_ctb.service_count || !s_ctb.service_array)
    {
        LOGE("ctx err");
        return -1;
    }

    for (int i = 0; i < s_ctb.service_count; ++i)
    {
        LOGV("index %d start hdl 0x%x", i, s_ctb.service_array[i].cfg.start_hdl);

        if (s_ctb.service_array[i].cfg.start_hdl <= attr_handle &&
                (i + 1 >= s_ctb.service_count ||
                 attr_handle < s_ctb.service_array[i + 1].cfg.start_hdl))
        {
            if (prf_task_id)
            {
                *prf_task_id = s_ctb.service_array[i].cfg.prf_task_id;
            }

            if (att_index)
            {
                *att_index = attr_handle - s_ctb.service_array[i].cfg.start_hdl;
            }

            if (service_index)
            {
                *service_index = i;
            }

            return 0;
        }
    }

    return -1;
}

int32_t bk_tr_ble_peripheral_notice_cb(ble_notice_t notice, void *param)
{
    int32_t ret = 0;

    switch (notice)
    {
    case BLE_5_CONNECT_EVENT:
    {
        ble_conn_ind_t *c_ind = (typeof(c_ind))param;
        LOGD("BLE_5_CONNECT_EVENT c_ind:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x",
             c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
             c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);


        hal_ble_con_env.con_dev[c_ind->conn_idx].con_status = HAL_CONN_STATE_CONNECTED;
        hal_ble_con_env.con_dev[c_ind->conn_idx].role = LINK_ROLE_SLAVE;
        hal_ble_con_env.con_dev[c_ind->conn_idx].peer_addr_type = c_ind->peer_addr_type;
        os_memcpy(hal_ble_con_env.con_dev[c_ind->conn_idx].peer_addr, c_ind->peer_addr, sizeof(c_ind->peer_addr));

        if (BLE_IS_RPA(c_ind->peer_addr_type, (c_ind->peer_addr[5])))
        {
            bk_adapter_ble_find_initial_pseudo_addr(hal_ble_con_env.con_dev[c_ind->conn_idx].peer_addr);
        }

        hal_ble_con_env.con_dev[c_ind->conn_idx].intv = c_ind->con_interval;
        hal_ble_con_env.con_dev[c_ind->conn_idx].con_latency = c_ind->con_latency;
        hal_ble_con_env.con_dev[c_ind->conn_idx].sup_to = c_ind->sup_to;
        hal_ble_con_env.con_dev[c_ind->conn_idx].mtu_size = 23;

        hal_ble_env.slave_connected = 1;
    }
    break;

    case BLE_5_DISCONNECT_EVENT:
    {
        ble_discon_ind_t *d_ind = (typeof(d_ind))param;
        LOGD("BLE_5_DISCONNECT_EVENT d_ind:conn_idx:%d,reason:0x%x", d_ind->conn_idx, d_ind->reason);

        hal_ble_con_env.con_dev[d_ind->conn_idx].con_status = HAL_CONN_STATE_DISCONNECTED;
        hal_ble_con_env.con_dev[d_ind->conn_idx].role = LINK_ROLE_UNDEFINED;
        hal_ble_con_env.con_dev[d_ind->conn_idx].notify_pending_count = 0;

        hal_ble_env.slave_connected = 0;

        ble_evt_msg_elem_t elem = {0};
        os_memset(&elem, 0, sizeof(ble_evt_msg_elem_t));

        elem.server_connect_evt.conn_idx = d_ind->conn_idx;
        elem.server_connect_evt.relate_adv_handle = hal_ble_con_env.con_dev[d_ind->conn_idx].relate_adv_index;
        elem.server_connect_evt.type = TRBLE_SERVER_DISCONNECTED;
        os_memcpy(elem.server_connect_evt.peer_addr, hal_ble_con_env.con_dev[d_ind->conn_idx].peer_addr, sizeof(hal_ble_con_env.con_dev[d_ind->conn_idx].peer_addr));

        ble_evt_queue_push_ext(EVT_BLE_SERVER_CONNECTED, &elem, sizeof(ble_evt_msg_elem_t), NULL);

        os_memset(&elem, 0, sizeof(ble_evt_msg_elem_t));

        elem.server_disconnect_evt.conn_idx = d_ind->conn_idx;
        elem.server_disconnect_evt.reason = d_ind->reason;

        ble_evt_queue_push_ext(EVT_BLE_SERVER_DISCONNECT, &elem, sizeof(ble_evt_msg_elem_t), NULL);

        os_memset(hal_ble_con_env.con_dev[d_ind->conn_idx].peer_addr, 0, sizeof(hal_ble_con_env.con_dev[d_ind->conn_idx].peer_addr));

        if (hal_ble_con_env.con_dev[d_ind->conn_idx].connection_sem)
        {
            rtos_set_semaphore(&hal_ble_con_env.con_dev[d_ind->conn_idx].connection_sem);
        }
    }
    break;

    case BLE_5_MTU_CHANGE:
    {
        ble_mtu_change_t *m_ind = (typeof(m_ind))param;
        LOGD("BLE_5_MTU_CHANGE conn_idx:%d, mtu_size:%d", m_ind->conn_idx, m_ind->mtu_size);

        hal_ble_con_env.con_dev[m_ind->conn_idx].mtu_size = m_ind->mtu_size;

        ble_evt_msg_elem_t elem = {0};
        os_memset(&elem, 0, sizeof(ble_evt_msg_elem_t));

        elem.mtu_change_evt.conn_idx = m_ind->conn_idx;
        elem.mtu_change_evt.mtu = m_ind->mtu_size;

        ble_evt_queue_push_ext(EVT_BLE_SERVER_MTU_CHANGE, &elem, sizeof(ble_evt_msg_elem_t), NULL);
    }
    break;

    case BLE_5_CREATE_DB:
    {
        if (s_ctb.sem)
        {
            rtos_set_semaphore(&s_ctb.sem);
        }
    }
    break;

    case BLE_5_ATT_INFO_REQ:
    {
        ble_att_info_req_t *a_ind = (typeof(a_ind))param;
        LOGD("BLE_5_ATT_INFO_REQ a_ind:conn_idx:%d", a_ind->conn_idx);
        a_ind->status = BK_ERR_BLE_SUCCESS;
    }
    break;

    case BLE_5_WRITE_EVENT:
    {
        ble_write_req_t *w_req = (typeof(w_req))param;

        LOGD("BLE_5_WRITE_EVENT conn_idx:%d, prf_id:%d, att_idx:%d, len:%d, data[0]:0x%02x",
             w_req->conn_idx, w_req->prf_id, w_req->att_idx, w_req->len, w_req->value[0]);

        struct service_elem *service_elem = find_service_by_prf_id(w_req->prf_id);

        if (!service_elem || service_elem->cfg.att_db_nb <= w_req->att_idx)
        {
            LOGE("can't find prf_id %d or att_idx %d >= num %d", w_req->prf_id, w_req->att_idx, service_elem->cfg.att_db_nb);
            break;
        }

        ble_attm_desc_t *attm_desc = service_elem->cfg.att_db;
        // uint32_t db_index = service_elem - &s_ctb.service_array[0];  // Unused variable
        uint16_t uuid_16 = 0;

        os_memcpy(&uuid_16, attm_desc[w_req->att_idx].uuid, sizeof(uuid_16));
        uint16_t attr_handle = att_index_2_attr_handle(w_req->prf_id, w_req->att_idx);

        LOGV("uuid 0x%x attr_handle 0x%x", uuid_16, attr_handle);

        bk_ble_write_response(w_req->conn_idx, w_req->prf_id, w_req->att_idx, service_elem->db_addon[w_req->att_idx].app_reject);

        if (service_elem->db_addon[w_req->att_idx].app_reject)
        {
            LOGW("reject it 0x%x", service_elem->db_addon[w_req->att_idx].app_reject);
            break;
        }

#if ALLOC_ATTR_DATA

        if (!service_elem->db_addon[w_req->att_idx].buffer)
        {
            service_elem->db_addon[w_req->att_idx].buffer = os_malloc(w_req->len);

            if (!service_elem->db_addon[w_req->att_idx].buffer)
            {
                LOGE("alloc db buffer err %d %d", db_index, w_req->len);
                break;
            }

            service_elem->db_addon[w_req->att_idx].buffer_max_len = w_req->len;
        }
        else if (service_elem->db_addon[w_req->att_idx].buffer_max_len < w_req->len ||
                 service_elem->db_addon[w_req->att_idx].buffer_max_len > 2 * w_req->len + 32)
        {
            LOGI("realloc %d %d", db_index, w_req->len);

            os_free(service_elem->db_addon[w_req->att_idx].buffer);

            service_elem->db_addon[w_req->att_idx].buffer = os_malloc(w_req->len);

            if (!service_elem->db_addon[w_req->att_idx].buffer)
            {
                LOGE("alloc db buffer err %d %d", db_index, w_req->len);
                break;
            }

            service_elem->db_addon[w_req->att_idx].buffer_max_len = w_req->len;
        }

        os_memcpy(service_elem->db_addon[w_req->att_idx].buffer, w_req->value, w_req->len);
        service_elem->db_addon[w_req->att_idx].peer_write_buffer_current_len = w_req->len;
#else

        if (!service_elem->db_addon[w_req->att_idx].peer_write_buffer)
        {
            service_elem->db_addon[w_req->att_idx].peer_write_buffer = s_ctb.default_write_buffer;
            service_elem->db_addon[w_req->att_idx].peer_write_buffer_max_len = sizeof(s_ctb.default_write_buffer);
        }

        if (w_req->len > service_elem->db_addon[w_req->att_idx].peer_write_buffer_max_len)
        {
            LOGW("peer write len %d > write buffer max len %d !!!!!!", w_req->len, service_elem->db_addon[w_req->att_idx].peer_write_buffer_max_len);
        }

        uint8_t *tmp_buffer = NULL;
        uint32_t tmp_buffer_len = 0;

        tmp_buffer = os_zalloc(w_req->len);

        if (!tmp_buffer)
        {
            LOGE("alloc buffer fail");
            break;
        }
        else
        {
            tmp_buffer_len = w_req->len;
            os_memcpy(tmp_buffer, w_req->value, w_req->len);
        }

#endif

        if (service_elem->db_addon[w_req->att_idx].cb)
        {
            ble_evt_msg_elem_t elem = {0};
            os_memset(&elem, 0, sizeof(ble_evt_msg_elem_t));

            elem.attr_cb_evt.cb = service_elem->db_addon[w_req->att_idx].cb;
            //elem.attr_cb_evt.type = TRBLE_ATTR_CB_CCCD;
            elem.attr_cb_evt.con_handle = w_req->conn_idx;
            elem.attr_cb_evt.handle = attr_handle;
            elem.attr_cb_evt.arg = service_elem->db_addon[w_req->att_idx].arg;
            //elem.attr_cb_evt.result = cccd_config;
            elem.attr_cb_evt.pending = 0; //note: result pending see rtk impl
            elem.attr_cb_evt.tmp_buffer = tmp_buffer;
            elem.attr_cb_evt.tmp_buffer_len = tmp_buffer_len;
            elem.attr_cb_evt.service_index = service_elem - &s_ctb.service_array[0];
            elem.attr_cb_evt.att_index = w_req->att_idx;

            if ((BK_BLE_PERM_GET(attm_desc[w_req->att_idx].ext_perm, UUID_LEN) == BK_BLE_PERM_RIGHT_UUID_16)
                    && uuid_16 == BK_GATT_CHAR_CLIENT_CONFIG_UUID)
            {
                uint16_t cccd_config = 0;

                os_memcpy(&cccd_config, service_elem->db_addon[w_req->att_idx].peer_write_buffer, sizeof(cccd_config));

                elem.attr_cb_evt.type = TRBLE_ATTR_CB_CCCD;
                elem.attr_cb_evt.result = cccd_config;
            }
            else if(w_req->is_cmd)
            {
                elem.attr_cb_evt.type = TRBLE_ATTR_CB_WRITING_NO_RSP;
                elem.attr_cb_evt.result = 0;
            }
            else
            {
                elem.attr_cb_evt.type = TRBLE_ATTR_CB_WRITING;
                elem.attr_cb_evt.result = 0;
            }

            ble_evt_queue_push_ext(EVT_BLE_SERVER_ATTR_CB, &elem, sizeof(ble_evt_msg_elem_t), NULL);

            //note: report data with trble_netmgr_attr_get_data
        }
    }
    break;

    case BLE_5_READ_EVENT:
    {
        ble_read_req_t *r_req = (typeof(r_req))param;
        LOGD("BLE_5_READ_EVENT conn_idx:%d, prf_id:%d, att_idx:%d", r_req->conn_idx, r_req->prf_id, r_req->att_idx);

        struct service_elem *service_elem = find_service_by_prf_id(r_req->prf_id);

        if (!service_elem || service_elem->cfg.att_db_nb <= r_req->att_idx)
        {
            LOGE("can't find prf_id %d or att_idx %d >= num %d", r_req->prf_id, r_req->att_idx, service_elem->cfg.att_db_nb);
            break;
        }

        ble_attm_desc_t *attm_desc = service_elem->cfg.att_db;
        // uint32_t db_index = service_elem - &s_ctb.service_array[0];  // Unused variable
        uint16_t uuid_16 = 0;

        os_memcpy(&uuid_16, attm_desc[r_req->att_idx].uuid, sizeof(uuid_16));
        uint16_t attr_handle = att_index_2_attr_handle(r_req->prf_id, r_req->att_idx);

        uint8_t *rsp_buff = NULL;
        uint32_t rsp_len = 0;
        // uint8_t rsp_buff_tmp[2] = {0};  // Unused variable

        LOGV("uuid 0x%x attr_handle 0x%x", uuid_16, attr_handle);

        trble_attr_cb_type_e evt_type = 0;

        if ((BK_BLE_PERM_GET(attm_desc[r_req->att_idx].ext_perm, UUID_LEN) == BK_BLE_PERM_RIGHT_UUID_16)
                && uuid_16 == BK_GATT_CHAR_CLIENT_CONFIG_UUID)
        {
            evt_type = TRBLE_ATTR_CB_CCCD;

            //note: maybe rsp cccd value
            rsp_buff = (typeof(rsp_buff))&service_elem->db_addon[r_req->att_idx].cccd_config;
            rsp_len = sizeof(service_elem->db_addon[r_req->att_idx].cccd_config);
        }
        else
        {
            evt_type = TRBLE_ATTR_CB_READING;

            //note: peer will always read default_read_buffer, see rtk ble_tizenrt_read_val impl
            rsp_buff = s_ctb.default_read_buffer;
            rsp_len = sizeof(s_ctb.default_read_buffer);
        }

        bk_ble_read_response_value_ext(r_req->conn_idx, rsp_len, rsp_buff, r_req->prf_id, r_req->att_idx, service_elem->db_addon[r_req->att_idx].app_reject);

        if (service_elem->db_addon[r_req->att_idx].app_reject)
        {
            LOGW("reject it 0x%x", service_elem->db_addon[r_req->att_idx].app_reject);
            break;
        }

        if (evt_type != TRBLE_ATTR_CB_CCCD && service_elem->db_addon[r_req->att_idx].cb)
        {
            ble_evt_msg_elem_t elem = {0};
            os_memset(&elem, 0, sizeof(ble_evt_msg_elem_t));

            elem.attr_cb_evt.cb = service_elem->db_addon[r_req->att_idx].cb;
            elem.attr_cb_evt.type = evt_type;
            elem.attr_cb_evt.con_handle = r_req->conn_idx;
            elem.attr_cb_evt.handle = attr_handle;
            elem.attr_cb_evt.arg = service_elem->db_addon[r_req->att_idx].arg;
            elem.attr_cb_evt.result = 0;
            elem.attr_cb_evt.pending = 0; //note: result pending see rtk impl

            ble_evt_queue_push_ext(EVT_BLE_SERVER_ATTR_CB, &elem, sizeof(ble_evt_msg_elem_t), NULL);
        }
    }
    break;

    case BLE_5_READ_BLOB_EVENT:
    {
        ble_read_req_t *r_req = (typeof(r_req))param;

        LOGD("BLE_5_READ_BLOB_EVENT conn_idx:%d, prf_id:%d, att_idx:%d", r_req->conn_idx, r_req->prf_id, r_req->att_idx);
    }
    break;

    case BLE_5_TX_DONE:
    {
        bk_ble_gatt_cmp_evt_t *evt = (typeof(evt))param;
        LOGD("BLE_5_TX_DONE operation 0x%x, status %d, prf_id 0x%x, att_id 0x%x, conn_idx %d",
             evt->operation, evt->status, evt->prf_id, evt->att_id, evt->conn_idx);

        struct service_elem *service_elem = find_service_by_prf_id(evt->prf_id);

        if (!service_elem || service_elem->cfg.att_db_nb <= evt->att_id)
        {
            LOGE("can't find prf_id %d or att_idx %d >= num %d", evt->prf_id, evt->att_id, service_elem->cfg.att_db_nb);
            break;
        }

        rtos_lock_mutex(&hal_ble_con_env.con_dev[evt->conn_idx].notify_list_mutex);

        if (hal_ble_con_env.con_dev[evt->conn_idx].notify_pending_count > 0)
        {
            hal_ble_con_env.con_dev[evt->conn_idx].notify_pending_count--;
        }

        rtos_unlock_mutex(&hal_ble_con_env.con_dev[evt->conn_idx].notify_list_mutex);

#if NOTIFY_SYNC_API

        if (0x12 == evt->operation && hal_ble_con_env.con_dev[evt->conn_idx].data_sem)
        {
            rtos_set_semaphore(&hal_ble_con_env.con_dev[evt->conn_idx].data_sem);
        }

#endif
        uint16_t attr_handle = att_index_2_attr_handle(evt->prf_id, evt->att_id);

        //note: GATTC_NOTIFY no need cb, see rtk impl
        if (service_elem->db_addon[evt->att_id].cb && evt->operation == 0x13) //GATTC_INDICATE
        {
            ble_evt_msg_elem_t elem = {0};
            os_memset(&elem, 0, sizeof(ble_evt_msg_elem_t));

            elem.attr_cb_evt.cb = service_elem->db_addon[evt->att_id].cb;
            elem.attr_cb_evt.type = TRBLE_ATTR_CB_INDICATE;
            elem.attr_cb_evt.con_handle = evt->conn_idx;
            elem.attr_cb_evt.handle = attr_handle;
            elem.attr_cb_evt.arg = service_elem->db_addon[evt->att_id].arg;
            elem.attr_cb_evt.result = evt->status;
            elem.attr_cb_evt.pending = hal_ble_con_env.con_dev[evt->conn_idx].notify_pending_count; //note: result pending see rtk impl

            ble_evt_queue_push_ext(EVT_BLE_SERVER_ATTR_CB, &elem, sizeof(ble_evt_msg_elem_t), NULL);
        }
    }
    break;

    default:
        break;
    }

    return 0;
}

int32_t bk_tr_ble_server_attr_set_data_ptr_private(uint8_t service_index, uint8_t att_index,
        uint8_t *buffer, uint16_t buffer_len, uint16_t buffer_max_len)
{
    struct service_elem *service_elem = &s_ctb.service_array[service_index];

    if (service_index >= s_ctb.service_count)
    {
        LOGE("service index out range %d >= %d", service_index, s_ctb.service_count);
        return -1;
    }

    if (att_index >= service_elem->cfg.att_db_nb)
    {
        LOGE("att index out range %d >= %d", att_index, service_elem->cfg.att_db_nb);
        return -1;
    }

    service_elem->db_addon[att_index].peer_write_buffer = buffer;
    service_elem->db_addon[att_index].peer_write_buffer_current_len = buffer_len;
    service_elem->db_addon[att_index].peer_write_buffer_max_len = buffer_max_len;

    return 0;
}

//note: app will input attr_handle as char value handle, see rtk abs_handle impl
int32_t bk_tr_ble_server_attr_set_data_ptr(trble_attr_handle attr_handle, uint8_t *buffer, uint16_t buffer_len)
{
    int32_t ret = 0;
    uint16_t att_index = 0;
    uint16_t prof_id = 0;
    uint32_t service_index = 0;

    if ((buffer && !buffer_len) || (!buffer && buffer_len))
    {
        LOGE("param err %p %d", buffer, buffer_len);
        return TRBLE_FAIL;
    }

    if (attr_handle_2_att_index(attr_handle, &prof_id, &att_index, &service_index) < 0)
    {
        LOGE("can't find attr_handle 0x%x", attr_handle);
        return TRBLE_FAIL;
    }

    LOGD("attr_handle 0x%x len %d", attr_handle, buffer_len);

    struct service_elem *service_elem = &s_ctb.service_array[service_index];

    uint16_t uuid_16 = 0;

    os_memcpy(&uuid_16, service_elem->cfg.att_db[att_index].uuid, sizeof(uuid_16));

    if ((BK_BLE_PERM_GET(service_elem->cfg.att_db[att_index].ext_perm, UUID_LEN) == BK_BLE_PERM_RIGHT_UUID_16)
            && uuid_16 == BK_GATT_PRI_SERVICE_DECL_UUID)
    {
        LOGE("can't set service attr data !!!");
        return TRBLE_FAIL;
    }
    else if ((BK_BLE_PERM_GET(service_elem->cfg.att_db[att_index].ext_perm, UUID_LEN) == BK_BLE_PERM_RIGHT_UUID_16)
             && uuid_16 == BK_GATT_CHAR_DECL_UUID)
    {
        LOGV("decl attr_handle 0x%x -> value attr handle 0x%x", attr_handle, attr_handle + 1);
        LOGV("att_index %d -> %d", att_index, att_index + 1);

        LOGE("can't set char decl attr data !!!");
        return TRBLE_FAIL;
    }
    else
    {
        LOGW("att_index %d", att_index);
    }

#if ALLOC_ATTR_DATA

    if (!service_elem->db_addon[att_index].buffer)
    {
        service_elem->db_addon[att_index].buffer = os_malloc(buffer_len);

        if (!service_elem->db_addon[att_index].buffer)
        {
            LOGE("alloc db buffer err %d %d", service_index, buffer_len);
            return TRBLE_OUT_OF_MEMORY;
        }

        service_elem->db_addon[att_index].buffer_max_len = buffer_len;
    }
    else if (service_elem->db_addon[att_index].buffer_max_len < buffer_len ||
             service_elem->db_addon[att_index].buffer_max_len > 2 * buffer_len + 32)
    {
        LOGI("realloc %d %d", service_index, buffer_len);

        os_free(service_elem->db_addon[att_index].buffer);

        service_elem->db_addon[att_index].buffer = os_malloc(buffer_len);

        if (!service_elem->db_addon[att_index].buffer)
        {
            LOGE("alloc db buffer err %d %d", service_index, buffer_len);
            return TRBLE_OUT_OF_MEMORY;
        }

        service_elem->db_addon[att_index].buffer_max_len = buffer_len;
    }

    if (buffer)
    {
        os_memcpy(service_elem->db_addon[att_index].buffer, buffer, buffer_len);
    }

    service_elem->db_addon[att_index].peer_write_buffer_current_len = buffer_len;
#else

    int32_t set_buffer_cb(uint32_t evt, int32_t status, void *param)
    {
        ble_evt_msg_elem_t *nest_elem = param;
        beken_semaphore_t *nest_sem = nest_elem->set_server_buffer_cmd.pm;

        if (nest_sem)
        {
            rtos_set_semaphore(nest_sem);
        }

        return 0;
    }

    beken_semaphore_t sem = NULL;

    ret = rtos_init_semaphore(&sem, 1);

    if (ret)
    {
        LOGE("init sem err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }

    ble_evt_msg_elem_t elem = {0};
    os_memset(&elem, 0, sizeof(ble_evt_msg_elem_t));

    elem.set_server_buffer_cmd.buffer = buffer;
    elem.set_server_buffer_cmd.buffer_len = buffer_len;
    elem.set_server_buffer_cmd.buffer_max_len = buffer_len;
    elem.set_server_buffer_cmd.service_index = service_index;
    elem.set_server_buffer_cmd.att_index = att_index;
    elem.set_server_buffer_cmd.pm = &sem;
    ret = ble_evt_queue_push_ext(EVT_BLE_SERVER_SET_BUFFER, &elem, sizeof(ble_evt_msg_elem_t), set_buffer_cb);

    if (ret)
    {
        LOGE("push queue err");
        ret = TRBLE_FAIL;
        goto end;
    }

    ret = rtos_get_semaphore(&sem, BEKEN_WAIT_FOREVER);

    if (ret)
    {
        LOGE("wait sem %d err", ret);
    }

#endif

end:;

    if (sem)
    {
        rtos_deinit_semaphore(&sem);
        sem = NULL;
    }

    LOGD("end %d", ret);

    return ret;
}

int32_t bk_tr_ble_server_attr_get_data_ptr(trble_attr_handle attr_handle, uint8_t **buffer, uint16_t *buffer_len, uint16_t *buffer_max_len)
{
    uint16_t att_index = 0;
    uint16_t prof_id = 0;
    uint32_t service_index = 0;

    if (attr_handle_2_att_index(attr_handle, &prof_id, &att_index, &service_index) < 0)
    {
        LOGE("can't find attr_handle 0x%x", attr_handle);
        return TRBLE_FAIL;
    }

    LOGD("attr_handle 0x%x", attr_handle);

    struct service_elem *service_elem = &s_ctb.service_array[service_index];

    uint16_t uuid_16 = 0;

    os_memcpy(&uuid_16, service_elem->cfg.att_db[att_index].uuid, sizeof(uuid_16));

    if ((BK_BLE_PERM_GET(service_elem->cfg.att_db[att_index].ext_perm, UUID_LEN) == BK_BLE_PERM_RIGHT_UUID_16)
            && uuid_16 == BK_GATT_PRI_SERVICE_DECL_UUID)
    {
        LOGE("can't get service attr data !!!");
        return TRBLE_FAIL;
    }
    else if ((BK_BLE_PERM_GET(service_elem->cfg.att_db[att_index].ext_perm, UUID_LEN) == BK_BLE_PERM_RIGHT_UUID_16)
             && uuid_16 == BK_GATT_CHAR_DECL_UUID)
    {
        LOGW("decl attr_handle 0x%x -> value attr handle 0x%x", attr_handle, attr_handle + 1);
        LOGW("att_index %d -> %d", att_index, att_index + 1);

        LOGE("can't get char decl attr data !!!");
        return TRBLE_FAIL;
    }
    else
    {
        LOGD("att_index %d", att_index);
    }

    if (buffer)
    {
        *buffer = s_ctb.service_array[service_index].db_addon[att_index].peer_write_buffer;
    }

    if (buffer_len)
    {
        *buffer_len = s_ctb.service_array[service_index].db_addon[att_index].peer_write_buffer_current_len;
    }

    if (buffer_max_len)
    {
        *buffer_max_len = s_ctb.service_array[service_index].db_addon[att_index].peer_write_buffer_max_len;
    }

    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_server_charact_notify(trble_attr_handle attr_handle, trble_conn_handle con_handle, uint8_t *buffer, uint16_t buffer_len, uint8_t is_notify)
{
    T_GAP_CONN_INFO info = {0};
    int32_t ret = 0;

    if (!buffer || !buffer_len)
    {
        LOGE("param err");
        return TRBLE_FAIL;
    }

    if (le_get_conn_info(con_handle, &info) == 0)
    {
        LOGE("can't find conn handle %d", con_handle);
        return TRBLE_FAIL;
    }

    LOGV("attr handle 0x%x con_handle %d len %d notify %d", attr_handle, con_handle, buffer_len, is_notify);

#if NOTIFY_SYNC_API

    if (is_notify && !hal_ble_con_env.con_dev[con_handle].data_sem)
    {
        ret = rtos_init_semaphore(&hal_ble_con_env.con_dev[con_handle].data_sem, 1);

        if (ret)
        {
            LOGE("init data_sem err with ind %d", con_handle);
        }
    }

#endif

    uint16_t att_index = 0;
    uint16_t prof_id = 0;
    uint32_t service_index = 0;

    if (attr_handle_2_att_index(attr_handle, &prof_id, &att_index, &service_index) < 0)
    {
        LOGE("can't find attr_handle 0x%x", attr_handle);
        return TRBLE_FAIL;
    }

    if (is_notify)
    {
        ret = bk_ble_send_noti_value(con_handle, buffer_len, buffer, prof_id, att_index);
    }
    else
    {
        ret = rtos_lock_mutex(&hal_ble_con_env.con_dev[con_handle].notify_list_mutex);

        if (ret)
        {
            LOGE("lock err %d", ret);
            return TRBLE_FAIL;
        }

        if (hal_ble_con_env.con_dev[con_handle].notify_pending_count >= NOTIFY_ASYNC_MAX_COUNT)
        {
            rtos_unlock_mutex(&hal_ble_con_env.con_dev[con_handle].notify_list_mutex);
            LOGE("pending count max !!!");
            return TRBLE_FAIL;
        }

        hal_ble_con_env.con_dev[con_handle].notify_pending_count++;
        rtos_unlock_mutex(&hal_ble_con_env.con_dev[con_handle].notify_list_mutex);

        ret = bk_ble_send_ind_value(con_handle, buffer_len, buffer, prof_id, att_index);
    }

    //todo: wait send completed ?
    if (ret != BK_ERR_BLE_SUCCESS)
    {
        rtos_lock_mutex(&hal_ble_con_env.con_dev[con_handle].notify_list_mutex);
        hal_ble_con_env.con_dev[con_handle].notify_pending_count--;
        rtos_unlock_mutex(&hal_ble_con_env.con_dev[con_handle].notify_list_mutex);
        LOGE("ret err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }

#if NOTIFY_SYNC_API

    if (is_notify && !ret)
    {
        ret = rtos_get_semaphore(&hal_ble_con_env.con_dev[con_handle].data_sem, API_SEM_TIMEOUT * 2);

        if (ret)
        {
            LOGE("wait data_sem %d err", ret);
        }
    }
    else
    {
        LOGE("send notify/indicate err %d", con_handle);
    }

#endif
    LOGI("send ok");

end:;

    if (hal_ble_con_env.con_dev[con_handle].data_sem)
    {
        ret = rtos_deinit_semaphore(&hal_ble_con_env.con_dev[con_handle].data_sem);

        if (ret)
        {
            LOGE("deinit data_sem err with ind %d", con_handle);
        }

        hal_ble_con_env.con_dev[con_handle].data_sem = NULL;
    }

    return ret;
}

int32_t bk_tr_ble_server_disconnect(trble_conn_handle conn_handle)
{
    int32_t ret = 0;
    hal_ble_conn_t *con_info = NULL;

    LOGD("handle %d", conn_handle);

    con_info = (hal_ble_conn_t *)le_get_conn_info_ext(conn_handle);

    if (!con_info)
    {
        LOGE("error not in connected state %d", conn_handle);
        return TRBLE_INVALID_STATE;
    }

    if (ble_cmd_queue_push(CMD_BLE_CONN_DIS_CONN, conn_handle))
    {
        LOGE("ble_cmd_queue_push fail");
        ret = TRBLE_FAIL;
        goto end;
    }

end:;

    return ret;
}

int32_t bk_tr_ble_server_get_mac_addr_by_conn_handle(trble_conn_handle conn_handle, uint8_t *bd_addr)
{
    T_GAP_CONN_INFO con_info = {0};

    if (le_get_conn_info(conn_handle, &con_info) == 0)
    {
        LOGE("error not in connected state %d", conn_handle);
        return TRBLE_INVALID_STATE;
    }

    os_memcpy(bd_addr, con_info.remote_bd, sizeof(con_info.remote_bd));

    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_server_get_conn_handle_by_addr(uint8_t *bd_addr, trble_conn_handle *con_handle)
{
    uint32_t index = 0;
    T_GAP_CONN_INFO *info = (T_GAP_CONN_INFO *)le_get_conn_info_by_mac(bd_addr, &index);

    if (!info)
    {
        LOGE("can't find info %02x:%02x:%02x:%02x:%02x:%02x",
             bd_addr[5],
             bd_addr[4],
             bd_addr[3],
             bd_addr[2],
             bd_addr[1],
             bd_addr[0]);
        return TRBLE_INVALID_STATE;
    }

    if (con_handle)
    {
        *con_handle = index;
    }

    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_server_set_gap_device_name(uint8_t len, uint8_t *device_name)
{
    int32_t ret = 0;

    LOGD("len %d", len);

    ret = bk_ble_appm_set_dev_name(len, device_name);

    if (ret)
    {
        LOGE("set name err %d", ret);
        return TRBLE_FAIL;
    }

    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_server_indicate_queue_count(trble_conn_handle *con_handle, uint8_t *count)
{
    if (!con_handle || !count)
    {
        LOGE("param err");
        return TRBLE_FAIL;
    }

    if (!le_get_conn_info_ext(*con_handle))
    {
        LOGE("not connected %d", *con_handle);
        return TRBLE_FAIL;
    }

    *count = (typeof(*count))hal_ble_con_env.con_dev[*con_handle].notify_pending_count;

    LOGD("%d %d", *con_handle, *count);

    return TRBLE_SUCCESS;
}

//note: app will set char attr handle/char desc handle, see rtk abs_handle impl ble_tizenrt_set_server_reject
int32_t bk_tr_ble_server_attr_reject(trble_attr_handle attr_handle, uint8_t app_errorcode)
{
    LOGD("attr handle 0x%x err code 0x%x", attr_handle, app_errorcode);

    uint16_t att_index = 0;
    uint16_t prof_id = 0;
    uint32_t service_index = 0;

    if (attr_handle_2_att_index(attr_handle, &prof_id, &att_index, &service_index) < 0)
    {
        LOGE("can't find attr_handle 0x%x", attr_handle);
        return TRBLE_FAIL;
    }

    s_ctb.service_array[service_index].db_addon[att_index].app_reject = app_errorcode;

    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_server_report_connected_evt(uint8_t conn_idx, uint8_t type)
{
    int32_t ret = 0;
    hal_ble_conn_t *info = (hal_ble_conn_t *)le_get_conn_info_ext(conn_idx);

    if (!info)
    {
        LOGE("can't find conn_idx %d info", conn_idx);
        return TRBLE_FAIL;
    }

    LOGD("conn_idx %d adv_idx %d type %d", conn_idx, info->relate_adv_index, type);

    ble_evt_msg_elem_t elem = {0};
    os_memset(&elem, 0, sizeof(ble_evt_msg_elem_t));

    elem.server_connect_evt.conn_idx = conn_idx;
    elem.server_connect_evt.relate_adv_handle = info->relate_adv_index;
    elem.server_connect_evt.type = type;
    os_memcpy(elem.server_connect_evt.peer_addr, hal_ble_con_env.con_dev[conn_idx].peer_addr, sizeof(hal_ble_con_env.con_dev[conn_idx].peer_addr));

    ble_evt_queue_push_ext(EVT_BLE_SERVER_CONNECTED, &elem, sizeof(ble_evt_msg_elem_t), NULL);

    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_server_report_passkey_evt(uint8_t conn_idx, uint32_t passkey)
{
    LOGD("conn_idx %d passkey %06d", conn_idx, passkey);

    ble_evt_msg_elem_t elem = {0};
    os_memset(&elem, 0, sizeof(ble_evt_msg_elem_t));

    elem.server_disconnect_evt.conn_idx = conn_idx;
    elem.server_disconnect_evt.reason = passkey;

    ble_evt_queue_push_ext(EVT_BLE_SERVER_PASSKEY, &elem, sizeof(ble_evt_msg_elem_t), NULL);

    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_server_init(trble_server_init_config *config)
{
    int32_t ret = TRBLE_SUCCESS;
    int32_t err = 0;

    if (s_ctb.init)
    {
        LOGE("already init");
        return TRBLE_ALREADY_WORKING;
    }

    if(!config)
    {
        LOGW("NULL config, pls init later !!!");
        return TRBLE_INVALID_ARGS;
    }

    LOGD("start");

    if (!s_ctb.sem)
    {
        err = rtos_init_semaphore(&s_ctb.sem, 1);

        if (err)
        {
            LOGE("init sem err %d", err);
            ret = TRBLE_FAIL;
            goto end;
        }
    }

    if (!config || !config->profile_count)
    {
        LOGW("no need set db");
        s_ctb.init = 1;
        goto end;
    }

    os_memcpy(&s_ctb.server_init_parm, config, sizeof(*config));
    os_memcpy(s_ctb.default_read_buffer, "Tizenrt", sizeof("Tizenrt"));

    LOGD("profile_count %d", config->profile_count);

    //todo: ble_server_init_config not match trble_server_init_config in example !!!

    s_ctb.db_attr_count = config->profile_count;

    //step 1: get count
    //todo: trble_gatt_t not match ble_server_gatt_t !!!
    for (int i = 0; i < config->profile_count; i++)
    {
        if (!config->profile[i].attr_handle)
        {
            LOGE("invalid attr_handle, index %d", i);
            ret = TRBLE_FAIL;
            goto end;
        }

        switch (config->profile[i].type)
        {
        case TRBLE_GATT_SERVICE:
            s_ctb.service_count++;
            break;

        case TRBLE_GATT_CHARACT:
            s_ctb.db_attr_count++;
            break;

        case TRBLE_GATT_DESC:
            break;

        default:
            LOGE("invalid gatt type %d %d", config->profile[i].type, i);
            ret = TRBLE_FAIL;
            goto end;
            break;
        }
    }

    s_ctb.service_array = os_zalloc(s_ctb.service_count * sizeof(*s_ctb.service_array));

    if (!s_ctb.service_array)
    {
        LOGE("service_array alloc err");
        ret = TRBLE_OUT_OF_MEMORY;
        goto end;
    }

    uint8_t service_before = 0;
    uint8_t char_before = 0;

    //step 2: alloc

    for (int input_index = 0,
            service_index = 0,
            attr_tmp_count = 0;
            input_index <= config->profile_count; input_index++)
    {
        if (input_index < config->profile_count)
        {
            LOGV("step 2 index %d type %d service_before %d char_before %d", input_index, config->profile[input_index].type, service_before, char_before);
        }

        if (input_index == config->profile_count || TRBLE_GATT_SERVICE == config->profile[input_index].type)
        {
            if (service_before)
            {
                LOGV("alloc att count %d for service index %d", attr_tmp_count, service_index);
                s_ctb.service_array[service_index].cfg.att_db = os_zalloc(attr_tmp_count * sizeof(*s_ctb.service_array[service_index].cfg.att_db));

                if (!s_ctb.service_array[service_index].cfg.att_db)
                {
                    LOGE("service db alloc err %d %d", service_index, attr_tmp_count);
                    ret = TRBLE_OUT_OF_MEMORY;
                    goto end;
                }

                s_ctb.service_array[service_index].cfg.att_db_nb = attr_tmp_count;

                s_ctb.service_array[service_index].db_addon = os_zalloc(attr_tmp_count * sizeof(*s_ctb.service_array[service_index].db_addon));

                if (!s_ctb.service_array[service_index].db_addon)
                {
                    LOGE("service db_addon alloc err %d %d", service_index, attr_tmp_count);
                    ret = TRBLE_OUT_OF_MEMORY;
                    goto end;
                }

                service_before = 0;
                char_before = 0;
                service_index++;
                attr_tmp_count = 0;

                if (input_index == config->profile_count)
                {
                    break;
                }
            }

            service_before = 1;
            attr_tmp_count++;
        }
        else if (TRBLE_GATT_CHARACT == config->profile[input_index].type)
        {
            if (!service_before)
            {
                LOGE("service must set before %d !!!", input_index);
                ret = TRBLE_FAIL;
                goto end;
            }

            char_before = 1;
            attr_tmp_count += 2;
        }
        else if (TRBLE_GATT_DESC == config->profile[input_index].type)
        {
            if (!service_before)
            {
                LOGE("service must set before %d !!!", input_index);
                ret = TRBLE_FAIL;
                goto end;
            }

            if (!char_before)
            {
                LOGE("char must set before %d !!!", input_index);
                ret = TRBLE_FAIL;
                goto end;
            }

            attr_tmp_count++;
        }
        else
        {
            LOGE("invalid gatt type %d %d", config->profile[input_index].type, input_index);
            ret = TRBLE_FAIL;
            goto end;
        }
    }

    service_before = 0;
    char_before = 0;

    //step 3: assign
    for (int input_index = 0,
            service_index = 0,
            attr_tmp_index = 0,
            attr_all_count = 0;
            input_index <= config->profile_count; input_index++)
    {
        if (input_index < config->profile_count)
        {
            LOGV("step 3 index %d type %d service_before %d char_before %d", input_index, config->profile[input_index].type, service_before, char_before);
        }

        if (input_index == config->profile_count || TRBLE_GATT_SERVICE == config->profile[input_index].type)
        {
            if (service_before)
            {
                LOGV("start create db %d %d", service_index, s_ctb.service_array[service_index].cfg.att_db_nb);

                err = bk_ble_create_db(&s_ctb.service_array[service_index].cfg);

                if (err)
                {
                    LOGE("create db err %d", err);
                    ret = TRBLE_FAIL;
                    goto end;
                }

                err = rtos_get_semaphore(&s_ctb.sem, API_SEM_TIMEOUT);

                if (err)
                {
                    LOGE("wait create db err %d", err);
                    ret = TRBLE_FAIL;
                    goto end;
                }

                service_before = 0;
                attr_tmp_index = 0;
                service_index++;

                LOGI("create db %d success", service_index - 1);

                if (input_index == config->profile_count)
                {
                    if (attr_all_count != s_ctb.db_attr_count)
                    {
                        LOGE("attr count not match %d %d !!!", attr_all_count, s_ctb.db_attr_count);
                    }

                    if (service_index != s_ctb.service_count)
                    {
                        LOGE("service count not match %d %d !!!", service_index, s_ctb.service_count);
                    }

                    break;
                }

            }

            char_before = 0;
            service_before = 1;

            s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].uuid[0] = (BK_GATT_PRI_SERVICE_DECL_UUID & 0xff);
            s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].uuid[1] = ((BK_GATT_PRI_SERVICE_DECL_UUID & 0xff00) >> 8);
            s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm = BK_BLE_PERM_SET(RD, ENABLE);

            s_ctb.service_array[service_index].cfg.prf_task_id = service_index + FIRST_PRF_ID_OFFSET;
            s_ctb.service_array[service_index].cfg.start_hdl = config->profile[input_index].attr_handle;
            os_memcpy(s_ctb.service_array[service_index].cfg.uuid, config->profile[input_index].uuid, config->profile[input_index].uuid_length);

            switch (config->profile[input_index].uuid_length)
            {
            case 2:
                s_ctb.service_array[service_index].cfg.svc_perm |= BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_16);
                break;

            case 4:
                s_ctb.service_array[service_index].cfg.svc_perm |= BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_32);
                break;

            case 16:
                s_ctb.service_array[service_index].cfg.svc_perm |= BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_128);
                break;

            default:
                LOGE("uuid len invalid %d %d", config->profile[input_index].uuid_length, input_index);
                ret = TRBLE_FAIL;
                goto end;
                break;
            }

            s_ctb.service_array[service_index].db_addon[attr_tmp_index].arg = config->profile[input_index].arg;
            s_ctb.service_array[service_index].db_addon[attr_tmp_index].cb = config->profile[input_index].cb;

            LOGI("find service index %d attr_tmp_index %d start handle %d ", service_index, attr_tmp_index,
                 s_ctb.service_array[service_index].cfg.start_hdl);

            attr_tmp_index++;
            attr_all_count++;
        }
        else if (TRBLE_GATT_CHARACT == config->profile[input_index].type)
        {
            if (!service_before)
            {
                LOGE("service must set before %d !!!", input_index);
                ret = TRBLE_FAIL;
                goto end;
            }

            char_before = 1;

            //declare
            s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].uuid[0] = (BK_GATT_CHAR_DECL_UUID & 0xff);
            s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].uuid[1] = ((BK_GATT_CHAR_DECL_UUID & 0xff00) >> 8);
            s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm = BK_BLE_PERM_SET(RD, ENABLE);
            attr_tmp_index++;
            attr_all_count++;

            //value
            s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].ext_perm |= BK_BLE_PERM_SET(RI, ENABLE);
            s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].max_size = ATTR_MAX_BUFFER_LEN;

            if (config->profile[input_index].property & TRBLE_ATTR_PROP_READ)
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(RD, ENABLE);
            }

            if (config->profile[input_index].property & TRBLE_ATTR_PROP_WRITE)
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(WRITE_REQ, ENABLE);
            }

            if (config->profile[input_index].property & TRBLE_ATTR_PROP_WRITE_NO_RSP)
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(WRITE_COMMAND, ENABLE);
            }

            if (config->profile[input_index].property & TRBLE_ATTR_PROP_NOTIFY)
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(NTF, ENABLE);
            }

            if (config->profile[input_index].property & TRBLE_ATTR_PROP_INDICATE)
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(IND, ENABLE);
            }

            if (config->profile[input_index].property & TRBLE_ATTR_PROP_AUTHEN)
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(WRITE_SIGNED, ENABLE);
                LOGW("not support TRBLE_ATTR_PROP_AUTHEN now !!!");
            }

            if (config->profile[input_index].property & TRBLE_ATTR_PROP_EXTENDED)
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(EXT, ENABLE);
            }

            //            if (config->profile[input_index].property & TRBLE_ATTR_PROP_RWN)
            //            {
            //                LOGW("unknow TRBLE_ATTR_PROP_RWN !!!");
            //            }

            LOGW("perm 0x%x %d", config->profile[input_index].permission, input_index);

            if (config->profile[input_index].permission & TRBLE_ATTR_PERM_R_BANNED)
            {

            }
            else if (config->profile[input_index].permission & (TRBLE_ATTR_PERM_R_AUTHEN | TRBLE_ATTR_PERM_R_ENCRYPT))
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(RP, AUTH);
            }
            else if (config->profile[input_index].permission & (TRBLE_ATTR_PERM_R_AUTHOR))
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(RP, UNAUTH);
            }
            else if (config->profile[input_index].permission & (TRBLE_ATTR_PERM_R_PERMIT))
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(RP, NO_AUTH);
            }
            else if ((config->profile[input_index].permission & TRBLE_ATTR_PERM_R_PERMIT) == TRBLE_ATTR_PERM_R_PERMIT)
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(RP, NO_AUTH);
            }
            else
            {

            }

            if (config->profile[input_index].permission & TRBLE_ATTR_PERM_W_BANNED)
            {

            }
            else if (config->profile[input_index].permission & (TRBLE_ATTR_PERM_W_AUTHEN | TRBLE_ATTR_PERM_W_ENCRYPT))
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(WP, AUTH);
            }
            else if (config->profile[input_index].permission & (TRBLE_ATTR_PERM_W_AUTHOR))
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(WP, UNAUTH);
            }
            else if (config->profile[input_index].permission & (TRBLE_ATTR_PERM_W_PERMIT))
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(WP, NO_AUTH);
            }
            else if ((config->profile[input_index].permission & TRBLE_ATTR_PERM_W_PERMIT) == TRBLE_ATTR_PERM_W_PERMIT)
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(WP, NO_AUTH);
            }
            else
            {

            }

            os_memcpy(s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].uuid, config->profile[input_index].uuid, config->profile[input_index].uuid_length);

            switch (config->profile[input_index].uuid_length)
            {
            case 2:
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].ext_perm |= BK_BLE_PERM_SET(UUID_LEN, UUID_16);
                break;

            case 4:
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].ext_perm |= BK_BLE_PERM_SET(UUID_LEN, UUID_32);
                break;

            case 16:
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].ext_perm |= BK_BLE_PERM_SET(UUID_LEN, UUID_128);
                break;

            default:
                LOGE("uuid len invalid %d %d", config->profile[input_index].uuid_length, input_index);
                ret = TRBLE_FAIL;
                goto end;
                break;
            }

            s_ctb.service_array[service_index].db_addon[attr_tmp_index].arg = config->profile[input_index].arg;
            s_ctb.service_array[service_index].db_addon[attr_tmp_index].cb = config->profile[input_index].cb;

            if (!s_ctb.service_array[service_index].db_addon[attr_tmp_index].peer_write_buffer)
            {
                //note: peer write and app read will use default_write_buffer default, see rtk setup_ble_char_info impl
                s_ctb.service_array[service_index].db_addon[attr_tmp_index].peer_write_buffer = s_ctb.default_write_buffer;
                s_ctb.service_array[service_index].db_addon[attr_tmp_index].peer_write_buffer_max_len = sizeof(s_ctb.default_write_buffer);
            }

            LOGI("find service index %d char value index %d decl attr handle 0x%x", service_index, attr_tmp_index, config->profile[input_index].attr_handle);

            if (s_ctb.service_array[service_index].cfg.start_hdl + attr_tmp_index - 1 != config->profile[input_index].attr_handle)
            {
                LOGE("warning: char decl attr handle expect 0x%x but 0x%x !!!",
                     s_ctb.service_array[service_index].cfg.start_hdl + attr_tmp_index - 1,
                     config->profile[input_index].attr_handle);
            }

            //todo: char fix attr handle
            attr_tmp_index++;
            attr_all_count++;
        }
        else if (TRBLE_GATT_DESC == config->profile[input_index].type)
        {
            if (!service_before)
            {
                LOGE("service must set before %d !!!", input_index);
                ret = TRBLE_FAIL;
                goto end;
            }

            if (!char_before)
            {
                LOGE("char must set before %d !!!", input_index);
                ret = TRBLE_FAIL;
                goto end;
            }

            s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].ext_perm |= BK_BLE_PERM_SET(RI, ENABLE);
            s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].max_size = ATTR_MAX_BUFFER_LEN;
            s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(RD, ENABLE);
            s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(WRITE_REQ, ENABLE);

            LOGW("perm 0x%x %d", config->profile[input_index].permission, input_index);

            if (config->profile[input_index].permission & TRBLE_ATTR_PERM_R_BANNED)
            {

            }
            else if (config->profile[input_index].permission & (TRBLE_ATTR_PERM_R_AUTHEN | TRBLE_ATTR_PERM_R_ENCRYPT))
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(RP, AUTH);
            }
            else if (config->profile[input_index].permission & (TRBLE_ATTR_PERM_R_AUTHOR))
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(RP, UNAUTH);
            }
            else if (config->profile[input_index].permission & (TRBLE_ATTR_PERM_R_PERMIT))
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(RP, NO_AUTH);
            }
            else if ((config->profile[input_index].permission & TRBLE_ATTR_PERM_R_PERMIT) == TRBLE_ATTR_PERM_R_PERMIT)
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(WP, NO_AUTH);
            }
            else
            {

            }

            if (config->profile[input_index].permission & TRBLE_ATTR_PERM_R_BANNED)
            {

            }
            else if (config->profile[input_index].permission & (TRBLE_ATTR_PERM_W_AUTHEN | TRBLE_ATTR_PERM_W_ENCRYPT))
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(WP, AUTH);
            }
            else if (config->profile[input_index].permission & (TRBLE_ATTR_PERM_W_AUTHOR))
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(WP, UNAUTH);
            }
            else if (config->profile[input_index].permission & (TRBLE_ATTR_PERM_W_PERMIT))
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(WP, NO_AUTH);
            }
            else if ((config->profile[input_index].permission & TRBLE_ATTR_PERM_W_PERMIT) == TRBLE_ATTR_PERM_W_PERMIT)
            {
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].perm |= BK_BLE_PERM_SET(WP, NO_AUTH);
            }
            else
            {

            }

            os_memcpy(s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].uuid, config->profile[input_index].uuid, config->profile[input_index].uuid_length);

            switch (config->profile[input_index].uuid_length)
            {
            case 2:
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].ext_perm |= BK_BLE_PERM_SET(UUID_LEN, UUID_16);
                break;

            case 4:
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].ext_perm |= BK_BLE_PERM_SET(UUID_LEN, UUID_32);
                break;

            case 16:
                s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].ext_perm |= BK_BLE_PERM_SET(UUID_LEN, UUID_128);
                break;

            default:
                LOGE("uuid len invalid %d %d", config->profile[input_index].uuid_length, input_index);
                ret = TRBLE_FAIL;
                goto end;
                break;
            }

            s_ctb.service_array[service_index].db_addon[attr_tmp_index].arg = config->profile[input_index].arg;
            s_ctb.service_array[service_index].db_addon[attr_tmp_index].cb = config->profile[input_index].cb;

            if (BK_BLE_PERM_GET(s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].ext_perm, UUID_LEN) == BK_BLE_PERM_RIGHT_UUID_16)
            {
                uint16_t uuid_16 = 0;

                os_memcpy(&uuid_16, s_ctb.service_array[service_index].cfg.att_db[attr_tmp_index].uuid, sizeof(uuid_16));

                if (BK_GATT_CHAR_CLIENT_CONFIG_UUID == uuid_16)
                {
                    s_ctb.service_array[service_index].db_addon[attr_tmp_index].peer_write_buffer = (void *)&s_ctb.service_array[service_index].db_addon[attr_tmp_index].cccd_config;
                    s_ctb.service_array[service_index].db_addon[attr_tmp_index].peer_write_buffer_current_len = sizeof(s_ctb.service_array[service_index].db_addon[attr_tmp_index].cccd_config);
                    s_ctb.service_array[service_index].db_addon[attr_tmp_index].peer_write_buffer_max_len = sizeof(s_ctb.service_array[service_index].db_addon[attr_tmp_index].cccd_config);
                }
            }

            if (!s_ctb.service_array[service_index].db_addon[attr_tmp_index].peer_write_buffer)
            {
                //note: peer write and app read will use default_write_buffer default, see rtk setup_ble_char_info impl
                s_ctb.service_array[service_index].db_addon[attr_tmp_index].peer_write_buffer = s_ctb.default_write_buffer;
                s_ctb.service_array[service_index].db_addon[attr_tmp_index].peer_write_buffer_max_len = sizeof(s_ctb.default_write_buffer);
            }

            LOGI("find service %d char desc index %d attr handle 0x%x", service_index, attr_tmp_index, config->profile[input_index].attr_handle);

            if (s_ctb.service_array[service_index].cfg.start_hdl + attr_tmp_index != config->profile[input_index].attr_handle)
            {
                LOGE("warning: desc attr handle expect 0x%x but 0x%x !!!",
                     s_ctb.service_array[service_index].cfg.start_hdl + attr_tmp_index,
                     config->profile[input_index].attr_handle);
            }

            attr_tmp_index++;
            attr_all_count++;
        }
        else
        {
            LOGE("invalid gatt type %d %d", config->profile[input_index].type, input_index);
            ret = TRBLE_FAIL;
            goto end;
        }
    }

    s_ctb.init = 1;

end:;
    LOGD("end");

    if (ret)
    {
        release_all_db();

        if (s_ctb.sem)
        {
            err = rtos_deinit_semaphore(&s_ctb.sem);

            if (err)
            {
                LOGE("deinit sem err %d", err);
            }

            s_ctb.sem = NULL;
        }
    }

    return ret;
}

static void release_all_db(void)
{
    if (s_ctb.service_array)
    {
        for (int i = 0; i < s_ctb.service_count; ++i)
        {
            if (s_ctb.service_array[i].cfg.att_db)
            {
                if (s_ctb.service_array[i].db_addon)
                {
                    for (int j = 0; j < s_ctb.service_array[i].cfg.att_db_nb; ++j)
                    {
#if ALLOC_ATTR_DATA

                        if (s_ctb.service_array[i].db_addon[j].buffer)
                        {
                            os_free(s_ctb.service_array[i].db_addon[j].buffer);
                            s_ctb.service_array[i].db_addon[j].buffer = NULL;
                        }

#endif
                    }

                    os_free(s_ctb.service_array[i].db_addon);
                    s_ctb.service_array[i].db_addon = NULL;
                }

                os_free(s_ctb.service_array[i].cfg.att_db);
                s_ctb.service_array[i].cfg.att_db = NULL;
            }
        }

        os_free(s_ctb.service_array);
        s_ctb.service_array = NULL;
        s_ctb.db_attr_count = 0;
    }
}

int32_t bk_tr_ble_server_deinit(void)
{
    int32_t ret = 0;

    if (!s_ctb.init)
    {
        LOGE("already deinit");
        return TRBLE_ALREADY_WORKING;
    }

    LOGD("start");

    for (uint8_t i = 0; i < HAL_BLE_CON_NUM; i++)
    {
        if (HAL_CONN_STATE_CONNECTED == hal_ble_con_env.con_dev[i].con_status &&
                hal_ble_con_env.con_dev[i].role == LINK_ROLE_SLAVE)
        {
            if (!hal_ble_con_env.con_dev[i].connection_sem)
            {
                ret = rtos_init_semaphore(&hal_ble_con_env.con_dev[i].connection_sem, 1);

                if (ret)
                {
                    LOGE("init connection_sem err with ind %d", i);
                }
            }

            LOGW("disconnecting %d %02x:%02x:%02x:%02x:%02x:%02x %d", i,
                 hal_ble_con_env.con_dev[i].peer_addr[5],
                 hal_ble_con_env.con_dev[i].peer_addr[4],
                 hal_ble_con_env.con_dev[i].peer_addr[3],
                 hal_ble_con_env.con_dev[i].peer_addr[2],
                 hal_ble_con_env.con_dev[i].peer_addr[1],
                 hal_ble_con_env.con_dev[i].peer_addr[0],
                 hal_ble_con_env.con_dev[i].peer_addr_type);

            ret = bk_tr_ble_server_disconnect(i);

            if (!ret)
            {
                ret = rtos_get_semaphore(&hal_ble_con_env.con_dev[i].connection_sem, API_SEM_TIMEOUT);

                if (ret)
                {
                    LOGE("wait disconnect sem %d err", ret);
                }
            }
            else
            {
                LOGE("disconnect err %d", i);
            }

            if (hal_ble_con_env.con_dev[i].connection_sem)
            {
                ret = rtos_deinit_semaphore(&hal_ble_con_env.con_dev[i].connection_sem);

                if (ret)
                {
                    LOGE("deinit connection_sem err with ind %d", i);
                }

                hal_ble_con_env.con_dev[i].connection_sem = NULL;
            }
        }
    }

    release_all_db();

    if (s_ctb.sem)
    {
        ret = rtos_deinit_semaphore(&s_ctb.sem);

        if (ret)
        {
            LOGE("deinit sem err %d", ret);
        }

        s_ctb.sem = NULL;
    }

    os_memset(&s_ctb, 0, sizeof(s_ctb));
    s_ctb.init = 0;
    LOGD("end");
    return TRBLE_SUCCESS;
}
