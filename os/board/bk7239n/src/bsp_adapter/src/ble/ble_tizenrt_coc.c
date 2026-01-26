#include <tinyara/net/if/ble.h>
#include "ble_tizenrt_impl.h"

#include "bk_ble_tizenrt.h"
#include "ble_tizenrt_coc.h"

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

#define LOG_TAG "bkble_coc"
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define LOGE(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_ERROR)   BK_BLE_LOGE(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGW(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_WARNING) BK_BLE_LOGW(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGI(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_INFO)    BK_BLE_LOGI(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGD(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_DEBUG)   BK_BLE_LOGI(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGV(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_VERBOSE) BK_BLE_LOGI(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)


#define API_SEM_TIMEOUT 5000
#define IS_API_SYNC 0

typedef struct
{
    uint8_t init;
    trble_le_coc_init_config s_init_confg;
    uint16_t local_mtu;
    beken_semaphore_t sem;
} coc_ctb_t;

static coc_ctb_t s_ctb;

static int32_t do_in_evt_thread_cb(uint32_t evt, int32_t status, void *param)
{
    ble_evt_msg_elem_t *elem = param;
    ble_cmd_cmp_evt_t *coc_evt = elem->cb_only_evt.param;

    switch (elem->cb_only_evt.evt)
    {
    case BLE_5_COC_REG_COMPL_EVENT:
    case BLE_5_COC_UNREG_COMPL_EVENT:
    {
        if (s_ctb.s_init_confg.reg_psm_cb)
        {
            s_ctb.s_init_confg.reg_psm_cb(coc_evt->coc_reg_evt.psm, coc_evt->status);
        }
    }
    break;

    case BLE_5_COC_CONFIG_COMPL_EVENT:
    {
        if (s_ctb.s_init_confg.set_sec_cb)
        {
            s_ctb.s_init_confg.set_sec_cb(coc_evt->status);
        }
    }
    break;

    case BLE_5_COC_CONNECTION_COMPL_EVENT:
    {
        if (s_ctb.s_init_confg.con_cb)
        {
            s_ctb.s_init_confg.con_cb(coc_evt->conn_idx, coc_evt->coc_connection_compl_evt.local_cid, coc_evt->status);
        }
    }
    break;

    case BLE_5_COC_DISCCONNECT_COMPL_EVENT:
    {
        if (s_ctb.s_init_confg.discon_cb)
        {
            s_ctb.s_init_confg.discon_cb(coc_evt->conn_idx, coc_evt->coc_disconnect_compl_evt.local_cid, coc_evt->status);
        }
    }
    break;

    case BLE_5_COC_TX_DONE:
    {
        if (s_ctb.s_init_confg.send_cb)
        {
            s_ctb.s_init_confg.send_cb(coc_evt->conn_idx, coc_evt->coc_send_compl_evt.cid, coc_evt->status, coc_evt->coc_send_compl_evt.credit);
        }
    }
    break;

    case BLE_5_COC_RX_IND:
    {
        if (s_ctb.s_init_confg.recv_cb)
        {
            trble_data tmp_data = {.data = coc_evt->coc_recv_evt.data, .length = coc_evt->coc_recv_evt.length};
            s_ctb.s_init_confg.recv_cb(coc_evt->conn_idx, coc_evt->coc_recv_evt.cid, &tmp_data);
        }

        if (coc_evt->coc_recv_evt.data)
        {
            os_free(coc_evt->coc_recv_evt.data);
        }
    }
    break;

    default:
        break;
    }

    os_free(elem->cb_only_evt.param);
    elem->cb_only_evt.param = NULL;

    return 0;
}

int32_t bk_tr_ble_coc_notice_cb(ble_notice_t notice, void *param)
{
    int32_t ret = TRBLE_SUCCESS;
    ble_cmd_cmp_evt_t *evt = param;

    switch (notice)
    {
    case BLE_5_COC_REG_COMPL_EVENT:
    case BLE_5_COC_UNREG_COMPL_EVENT:
    {
        if (evt->status)
        {
            LOGE("reg/unreg %d err status 0x%x", notice, evt->status);
        }

#if IS_API_SYNC

        if (s_ctb.sem)
        {
            rtos_set_semaphore(&s_ctb.sem);
        }

#endif
        ble_evt_msg_elem_t elem = {0};

        elem.cb_only_evt.evt = notice;
        elem.cb_only_evt.param = os_zalloc(sizeof(*evt));

        if (!elem.cb_only_evt.param)
        {
            LOGE("alloc elem param err size %d", sizeof(*evt));
            break;
        }

        os_memcpy(elem.cb_only_evt.param, evt, sizeof(*evt));

        ret = ble_evt_queue_push_ext(EVT_BLE_DO_CB_ONLY, &elem, sizeof(elem), do_in_evt_thread_cb);

        if (ret)
        {
            LOGE("push queue err");
            os_free(elem.cb_only_evt.param);
        }
    }
    break;

    case BLE_5_COC_CONFIG_COMPL_EVENT:
    {
        if (evt->status)
        {
            LOGE("BLE_5_COC_CONFIG_COMPL_EVENT err status 0x%x", evt->status);
        }

        ble_evt_msg_elem_t elem = {0};

        elem.cb_only_evt.evt = notice;
        elem.cb_only_evt.param = os_zalloc(sizeof(*evt));

        if (!elem.cb_only_evt.param)
        {
            LOGE("alloc elem param err size %d", sizeof(*evt));
            break;
        }

        os_memcpy(elem.cb_only_evt.param, evt, sizeof(*evt));

        ret = ble_evt_queue_push_ext(EVT_BLE_DO_CB_ONLY, &elem, sizeof(elem), do_in_evt_thread_cb);

        if (ret)
        {
            LOGE("push queue err");
            os_free(elem.cb_only_evt.param);
        }
    }
    break;

    case BLE_5_COC_CONNECTION_COMPL_EVENT:
    case BLE_5_COC_DISCCONNECT_COMPL_EVENT:
    {
        if (notice == BLE_5_COC_CONNECTION_COMPL_EVENT)
        {
            LOGI("BLE_5_COC_CONNECTION_COMPL_EVENT status 0x%x conn_idx %d psm 0x%x cid 0x%x credit %d mtu %d mps %d\n",
                 evt->status,
                 evt->conn_idx,
                 evt->coc_connection_compl_evt.le_psm,
                 evt->coc_connection_compl_evt.local_cid,
                 evt->coc_connection_compl_evt.peer_credit,
                 evt->coc_connection_compl_evt.peer_mtu,
                 evt->coc_connection_compl_evt.peer_mps);
        }
        else
        {
            LOGI("BLE_5_COC_DISCCONNECT_COMPL_EVENT status 0x%x conn_idx %d cid 0x%x\n",
                 evt->status,
                 evt->conn_idx,
                 evt->coc_disconnect_compl_evt.local_cid);
        }

        ble_evt_msg_elem_t elem = {0};

        elem.cb_only_evt.evt = notice;
        elem.cb_only_evt.param = os_zalloc(sizeof(*evt));

        if (!elem.cb_only_evt.param)
        {
            LOGE("alloc elem param err size %d", sizeof(*evt));
            break;
        }

        os_memcpy(elem.cb_only_evt.param, evt, sizeof(*evt));

        ret = ble_evt_queue_push_ext(EVT_BLE_DO_CB_ONLY, &elem, sizeof(elem), do_in_evt_thread_cb);

        if (ret)
        {
            LOGE("push queue err");
            os_free(elem.cb_only_evt.param);
        }
    }
    break;

    case BLE_5_COC_TX_DONE:
    {
        if (evt->status)
        {
            LOGE("BLE_5_COC_TX_DONE %d err status 0x%x", notice, evt->status);
        }

        ble_evt_msg_elem_t elem = {0};

        elem.cb_only_evt.evt = notice;
        elem.cb_only_evt.param = os_zalloc(sizeof(*evt));

        if (!elem.cb_only_evt.param)
        {
            LOGE("alloc elem param err size %d", sizeof(*evt));
            break;
        }

        os_memcpy(elem.cb_only_evt.param, evt, sizeof(*evt));

        ret = ble_evt_queue_push_ext(EVT_BLE_DO_CB_ONLY, &elem, sizeof(elem), do_in_evt_thread_cb);

        if (ret)
        {
            LOGE("push queue err");
            os_free(elem.cb_only_evt.param);
        }
    }
    break;

    case BLE_5_COC_RX_IND:
    {
        LOGD("cid 0x%x credit %d offset %d len %d data 0x%02x%02x%02x%02x",
             evt->coc_recv_evt.cid,
             evt->coc_recv_evt.credit,
             evt->coc_recv_evt.offset,
             evt->coc_recv_evt.length,
             evt->coc_recv_evt.data[0],
             evt->coc_recv_evt.data[1],
             evt->coc_recv_evt.data[2],
             evt->coc_recv_evt.data[3]);

        ble_evt_msg_elem_t elem = {0};

        elem.cb_only_evt.evt = notice;
        elem.cb_only_evt.param = os_zalloc(sizeof(*evt));

        if (!elem.cb_only_evt.param)
        {
            LOGE("alloc elem param err size %d", sizeof(*evt));
            break;
        }

        os_memcpy(elem.cb_only_evt.param, evt, sizeof(*evt));

        void *tmp_buff = os_zalloc(evt->coc_recv_evt.length);

        if (!tmp_buff)
        {
            LOGE("alloc payload err len %d", evt->coc_recv_evt.length);
            os_free(elem.cb_only_evt.param);
            break;
        }

        os_memcpy(tmp_buff, evt->coc_recv_evt.data, evt->coc_recv_evt.length);
        ((typeof(evt))elem.cb_only_evt.param)->coc_recv_evt.data = tmp_buff;

        ret = ble_evt_queue_push_ext(EVT_BLE_DO_CB_ONLY, &elem, sizeof(elem), do_in_evt_thread_cb);

        if (ret)
        {
            LOGE("push queue err");
            os_free(tmp_buff);
            os_free(elem.cb_only_evt.param);
        }
    }
    break;

    case BLE_5_COC_CONNECT_REQ_EVENT:
    {
        LOGI("BLE_5_COC_CONNECT_REQ_EVENT conn_idx %d psm 0x%x cid 0x%x mtu %d mps %d credit %d",
             evt->conn_idx,
             evt->coc_connect_req_evt.le_psm,
             evt->coc_connect_req_evt.peer_cid,
             evt->coc_connect_req_evt.peer_mtu,
             evt->coc_connect_req_evt.peer_mps,
             evt->coc_connect_req_evt.peer_credit);

        bk_ble_coc_accept_connect_req(evt->conn_idx, 1, evt->coc_connect_req_evt.peer_cid, s_ctb.local_mtu, 0, 0);
    }
    break;

    default:
        break;
    }

end:;
    return ret;
}

int32_t bk_tr_ble_coc_init(trble_le_coc_init_config *le_coc)
{
    if (s_ctb.init)
    {
        LOGE("already init");
        return TRBLE_ALREADY_WORKING;
    }

    LOGD("%p", le_coc);
    s_ctb.s_init_confg = *le_coc;
    s_ctb.init = 1;

    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_coc_register_psm(uint8_t is_reg, uint16_t psm)
{
    int32_t ret = TRBLE_SUCCESS;

    if (!s_ctb.init)
    {
        LOGE("not init");
        return TRBLE_FAIL;
    }

    LOGD("reg %d psm 0x%x\n", is_reg, psm);

    if (is_reg)
    {
        ret = bk_ble_coc_reg(psm);
    }
    else
    {
        ret = bk_ble_coc_unreg(psm);
    }

    if (ret)
    {
        LOGE("reg/unreg coc err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }

#if IS_API_SYNC
    ret = rtos_get_semaphore(&s_ctb.sem, API_SEM_TIMEOUT);

    if (ret)
    {
        LOGE("wait coc reg/unreg err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }

#endif
end:;
    return ret;
}

int32_t bk_tr_ble_coc_set_psm_security(uint16_t le_psm, uint8_t active, bk_le_coc_security_mode_t sec_mode, uint8_t key_size)
{
    int32_t ret = TRBLE_UNSUPPORTED;
    uint8_t input = 0;

    if (!s_ctb.init)
    {
        LOGE("not init");
        return TRBLE_FAIL;
    }

    LOGD("psm 0x%x active %d sec_mode 0x%x size %d", le_psm, active, sec_mode, key_size);

    if (!active)
    {
        sec_mode = BK_LE_COC_SEC_NONE;
    }

    switch (sec_mode)
    {
    case BK_LE_COC_SEC_NONE:
        input = BK_BLE_COC_SEC_NONE;
        break;

    case BK_LE_COC_SEC_UNAUTHEN_ENCRYPT:
        input = BK_BLE_COC_SEC_UNAUTH_ENCRYPT;
        break;

    case BK_LE_COC_SEC_AUTHEN_ENCRYPT:
        input = BK_BLE_COC_SEC_AUTH_ENCRYPT;
        break;

    case BK_LE_COC_SEC_SECURE_CONN_AUTHEN:
        input = BK_BLE_COC_SEC_SECURE_CONNECTION;
        break;

    case BK_LE_COC_SEC_UNAUTHEN_DATA_SIGN:
    case BK_LE_COC_SEC_AUTHEN_DATA_SIGN:
    case BK_LE_COC_SEC_AUTHOR:
    case BK_LE_COC_SEC_SECURE_CONN_UNAUTHEN:
        LOGE("unsupport sec_mode param %d", sec_mode);
        return TRBLE_UNSUPPORTED;
        break;
    }

    ret = bk_ble_coc_config(le_psm, input);

    if (ret)
    {
        LOGE("config coc err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }

end:;
    return ret;
}

int32_t bk_tr_ble_coc_set_param(bk_le_coc_param_type_t type, uint16_t value)
{
    int32_t ret = TRBLE_UNSUPPORTED;

    if (!s_ctb.init)
    {
        LOGE("not init");
        return TRBLE_FAIL;
    }

    LOGD("type %d value %d", type, value);

    switch (type)
    {
    case BK_LE_COC_CHAN_PARAM_LOCAL_MTU:
        s_ctb.local_mtu = value;
        break;

    default:
        LOGW("invalid type %d", type);
        return TRBLE_FAIL;
        break;
    }

end:;
    return ret;
}

int32_t bk_tr_ble_coc_chan_get_param(uint16_t conn_handle, bk_le_coc_chan_param_type_t param_type, uint16_t cid, uint16_t *value)
{
    int32_t ret = TRBLE_UNSUPPORTED;
    uint8_t input = 0;
    uint32_t out = 0;

    if (!s_ctb.init)
    {
        LOGE("not init");
        return TRBLE_FAIL;
    }

    if (!value)
    {
        LOGE("param err");
        return TRBLE_FAIL;
    }

    LOGD("conn_handle %d param_type %d cid 0x%x", conn_handle, param_type, cid);

    switch (param_type)
    {
    case BK_LE_COC_CHAN_PARAM_CUR_CREDITS:
        input = BK_BLE_COC_PEER_CURRENT_CREDIT;
        break;

    case BK_LE_COC_CHAN_PARAM_MAX_CREDITS:
        input = BK_BLE_COC_PEER_MAX_CREDIT;
        break;

    case BK_LE_COC_CHAN_PARAM_MTU:
        input = BK_BLE_COC_PEER_MTU;
        break;

    default:
        LOGE("invalid param_type %d", param_type);
        ret = TRBLE_FAIL;
        goto end;
    }

    ret = bk_ble_coc_get_current_info(conn_handle, cid, input, &out);

    if (ret)
    {
        LOGE("get info coc err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }

    *value = out;

end:;
    return ret;
}

int32_t bk_tr_ble_coc_connect(uint16_t conn_handle, uint16_t le_psm)
{
    int32_t ret = TRBLE_SUCCESS;

    if (!s_ctb.init)
    {
        LOGE("not init");
        return TRBLE_FAIL;
    }

    LOGD("handle %d psm 0x%x", conn_handle, le_psm);

    ret = bk_ble_coc_connection_req(conn_handle, le_psm, s_ctb.local_mtu, 0, 0);
end:;
    return ret;
}

int32_t bk_tr_ble_coc_disconnect(uint16_t conn_handle, uint16_t cid)
{
    int32_t ret = TRBLE_SUCCESS;

    if (!s_ctb.init)
    {
        LOGE("not init");
        return TRBLE_FAIL;
    }

    LOGD("conn_handle %d cid 0x%x", conn_handle, cid);

    ret = bk_ble_coc_disconnection_req(conn_handle, cid);
end:;
    return ret;
}

int32_t bk_tr_ble_coc_send_data(uint16_t conn_handle, uint16_t cid, uint16_t len, uint8_t *data)
{
    int32_t ret = TRBLE_SUCCESS;

    if (!s_ctb.init)
    {
        LOGE("not init");
        return TRBLE_FAIL;
    }

    LOGD("conn_handle %d cid 0x%x len %d", conn_handle, cid, len);
    ret = bk_ble_coc_send_req(conn_handle, cid, data, len);
end:;
    return ret;
}

int32_t bk_tr_ble_coc_init_private(void)
{
    int32_t ret = TRBLE_SUCCESS;
    LOGD("");

    ret = rtos_init_semaphore(&s_ctb.sem, 1);

    if (ret)
    {
        LOGE("init sem err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }

end:;
    return ret;
}

int32_t bk_tr_ble_coc_deinit_private(void)
{
    int32_t ret = TRBLE_SUCCESS;
    LOGD("");

    if (s_ctb.sem)
    {
        rtos_deinit_semaphore(&s_ctb.sem);
    }

    os_memset(&s_ctb, 0, sizeof(s_ctb));
end:;
    return ret;
}
