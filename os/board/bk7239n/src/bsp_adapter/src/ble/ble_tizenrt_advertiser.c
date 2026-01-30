#include <tinyara/net/if/ble.h>
#include "ble_tizenrt_impl.h"

#include "bk_ble_tizenrt.h"
#include "ble_tizenrt_advertiser.h"

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

#define LOG_TAG "bkble_adv"
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define LOGE(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_ERROR)   BK_BLE_LOGE(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGW(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_WARNING) BK_BLE_LOGW(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGI(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_INFO)    BK_BLE_LOGI(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGD(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_DEBUG)   BK_BLE_LOGD(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGV(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_VERBOSE) BK_BLE_LOGV(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)

#define API_SEM_TIMEOUT 7000

//see rtk_bt_le_addr_type_t
enum
{
    BK_EXT_ADV_LE_ADDR_TYPE_PUBLIC =            0x00,      /*!< Public Device Address */
    BK_EXT_ADV_LE_ADDR_TYPE_RANDOM =            0x01,      /*!< Random Device Address */
    BK_EXT_ADV_LE_ADDR_TYPE_RPA_PUBLIC =        0x02,      /*!< Public Identity Address (RPA) */
    BK_EXT_ADV_LE_ADDR_TYPE_RPA_RANDOM =        0x03,      /*!< Random (static) Identity Address (RPA)*/
    BK_EXT_ADV_LE_ADDR_TYPE_PEER_ANONYMOUS =    0xFF,      /*!< Anonymous Device Address */
};

//see rtk_bt_le_adv_event_prop_t
enum
{
    BK_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_UNDIRECTED = 0x00, /**<  Non-connectable and non-scannable undirected. */
    BK_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_DIRECTED   = 0x04, /**<  Non-connectable and non-scannable directed. */
    BK_EXT_ADV_EXTENDED_ADV_CONN_UNDIRECTED              = 0x01, /**<  Connectable undirected. Advertising data shall not exceed 245 bytes. */
    BK_EXT_ADV_EXTENDED_ADV_CONN_DIRECTED                = 0x05, /**<  Connectable directed. Advertising data shall not exceed 239 bytes. */
    BK_EXT_ADV_EXTENDED_ADV_SCAN_UNDIRECTED              = 0x02, /**<  Scannable undirected. */
    BK_EXT_ADV_EXTENDED_ADV_SCAN_DIRECTED                = 0x06, /**<  Scannable directed. */

    BK_EXT_ADV_LEGACY_ADV_CONN_SCAN_UNDIRECTED           = 0x13, /**<  Connectable and scannable undirected. Advertising data or scan response data shall not exceed 31 bytes. */
    BK_EXT_ADV_LEGACY_ADV_CONN_LOW_DUTY_DIRECTED         = 0x15, /**<  Connectable directed (low duty cycle). */
    BK_EXT_ADV_LEGACY_ADV_CONN_HIGH_DUTY_DIRECTED        = 0x1D, /**<  Connectable directed (high duty cycle). */
    BK_EXT_ADV_LEGACY_ADV_SCAN_UNDIRECTED                = 0x12, /**<  Scannable undirected. Advertising data or scan response data shall not exceed 31 bytes. */
    BK_EXT_ADV_LEGACY_ADV_NON_SCAN_NON_CONN_UNDIRECTED   = 0x10, /**<  Non-connectable and non-scannable undirected. Advertising data shall not exceed 31 bytes. */
};

static beken_semaphore_t s_sem = NULL;
static beken_semaphore_t s_oneshot_sem = NULL;

static int32_t advertiser_adv_enable_nest_adv_cb(uint32_t evt, int32_t status, void *param);
static int32_t advertiser_multi_adv_enable_nest_adv_cb(uint32_t evt, int32_t status, void *param);
static int32_t advertiser_create_multi_adv_nest_adv_cb(uint32_t evt, int32_t status, void *param);
static int32_t advertiser_delete_multi_adv_nest_adv_cb(uint32_t evt, int32_t status, void *param);
static int32_t ble_advertiser_deinit_nest_adv_cb(uint32_t evt, int32_t status, void *param);
static int32_t advertiser_one_shot_adv_enable_nest_oneshot_adv_cb(uint32_t evt, int32_t status, void *param);
static int32_t nest_report_connected_evt(uint32_t evt, int32_t status, void *nest_pm);


static void reset_default_param(struct adv_hal_param *param)
{
    param->adv_idx = 0xFF;
    param->adv_status = BLE_HAL_ADV_STATE_IDLE;
    param->adv_param.own_addr_type = OWN_ADDR_TYPE_PUBLIC_ADDR;
    param->adv_param.adv_type = ADV_TYPE_LEGACY;
    param->adv_param.chnl_map = ADV_ALL_CHNLS;
    param->adv_param.adv_prop = ADV_PROP_CONNECTABLE_BIT | ADV_PROP_SCANNABLE_BIT;
    param->adv_param.adv_intv_min = BLE_ADV_INTERVAL_MIN;
    param->adv_param.adv_intv_max = BLE_ADV_INTERVAL_MAX;
    param->adv_param.prim_phy = PHY_TYPE_LE_1M;
    param->adv_param.second_phy = PHY_TYPE_LE_1M;
    param->advDataLen = 0;
    param->respDataLen = 0;
    param->deleting = 0;
    param->oneshot_app_id = 0xff;
}

static int8_t get_oneshot_by_adv_id(uint8_t oneshot_adv_id)
{
    if (oneshot_adv_id != 0xff)
    {
        for (uint32_t i = 0; i < BLE_ONTSHOT_MAX_STATIC_PARAM_COUNT; ++i)
        {
            if (hal_ble_adv_env.static_oneshot_adv_param[i].oneshot_app_id == oneshot_adv_id)
            {
                return i;
            }
        }
    }

    return -1;
}

static int8_t alloc_oneshot(void)
{
    for (uint32_t i = 0; i < BLE_ONTSHOT_MAX_STATIC_PARAM_COUNT; ++i)
    {
        if (hal_ble_adv_env.static_oneshot_adv_param[i].oneshot_app_id == 0xff)
        {
            return i;
        }
    }

    return -1;
}

int32_t bk_tr_ble_advertiser_notice_cb(ble_notice_t notice, void *param)
{
    int32_t ret = 0;

    switch (notice)
    {
    case BLE_5_ADV_STOPPED_EVENT:
    {
        ble_adv_stopped_ind_t *pm = (typeof(pm))param;
        int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(pm->adv_idx);

        LOGD("BLE_5_ADV_STOPPED_EVENT adv_idx %d hal_adv_index %d conn_idx %d reason %d", pm->adv_idx, hal_adv_index, pm->conn_idx, pm->reason);

        if (hal_adv_index < 0)
        {
            LOGE("can't find adv_idx %d", pm->adv_idx);
            break;
        }

        hal_ble_adv_env.array[hal_adv_index].adv_status = BLE_HAL_ADV_STATE_CREATED;

        if (pm->reason == 0 && pm->conn_idx != 0xff)
        {
            hal_ble_conn_t *info = (hal_ble_conn_t *)le_get_conn_info_ext(pm->conn_idx);

            if (!info)
            {
                LOGE("can't find conn_idx %d info", pm->conn_idx);
                break;
            }

            info->relate_adv_index = pm->adv_idx;

            if (hal_ble_adv_env.array[hal_adv_index].adv_elem == ADV_ELEM_STATIC)
            {
                ble_cmd_msg_elem_t *elem = (typeof(elem))os_zalloc(sizeof(*elem));

                if (!elem)
                {
                    LOGE("alloc elem fail");
                    break;
                }

                elem->conn_idx = pm->conn_idx;
                elem->adv_index = pm->adv_idx;

                if (ble_cmd_queue_push_ext(CMD_BLE_DEL_ADV, 0xff, elem, 0, nest_report_connected_evt))
                {
                    LOGE("ble_cmd_queue_push fail");
                }
            }
            else
            {
                bk_tr_ble_server_report_connected_evt(pm->conn_idx, TRBLE_SERVER_LL_CONNECTED);
            }
        }
        else if (pm->reason == 0)
        {
            LOGW("stop because app trig");
        }
        else
        {
            LOGW("stop because limit");

            if (s_oneshot_sem)
            {
                rtos_set_semaphore(&s_oneshot_sem);
            }
        }
    }
    break;

    default:
        break;
    }

    return ret;
}

int32_t bk_tr_ble_advertiser_set_adv_data(uint8_t *data, uint16_t length)
{
    LOGD("len %d", length);

    if (length > sizeof(hal_ble_adv_env.static_adv_param.advData))
    {
        LOGE("len too long");
        return TRBLE_FAIL;
    }

    if (hal_ble_adv_env.static_hal_index != 0xff &&
            hal_ble_adv_env.array[hal_ble_adv_env.static_hal_index].adv_status == BLE_HAL_ADV_STATE_ADVERTISING)
    {
        LOGE("static adv already start !!!");
        return TRBLE_FAIL;
    }

    os_memcpy(hal_ble_adv_env.static_adv_param.advData, data, length);
    hal_ble_adv_env.static_adv_param.advDataLen = length;

    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_advertiser_set_adv_resp(uint8_t *data, uint16_t length)
{
    LOGD("len %d", length);

    if (length > sizeof(hal_ble_adv_env.static_adv_param.respData))
    {
        LOGE("len too long");
        return TRBLE_FAIL;
    }

    if (hal_ble_adv_env.static_hal_index != 0xff &&
            hal_ble_adv_env.array[hal_ble_adv_env.static_hal_index].adv_status == BLE_HAL_ADV_STATE_ADVERTISING)
    {
        LOGE("static adv already start !!!");
        return TRBLE_FAIL;
    }

    os_memcpy(hal_ble_adv_env.static_adv_param.respData, data, length);
    hal_ble_adv_env.static_adv_param.respDataLen = length;

    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_advertiser_set_adv_type(trble_adv_type_e adv_type, trble_addr *addr)
{
    if (addr)
    {
        LOGD("adv_type %d mac_type %d %02x:%02x:%02x:%02x:%02x:%02x", adv_type, addr->type,
             addr->mac[5],
             addr->mac[4],
             addr->mac[3],
             addr->mac[2],
             addr->mac[1],
             addr->mac[0]);
    }
    else
    {
        LOGD("adv_type %d", adv_type);
    }

    if (hal_ble_adv_env.static_hal_index != 0xff &&
            hal_ble_adv_env.array[hal_ble_adv_env.static_hal_index].adv_status == BLE_HAL_ADV_STATE_ADVERTISING)
    {
        LOGE("static adv already start !!!");
        return TRBLE_FAIL;
    }

    switch (adv_type)
    {
    case TRBLE_ADV_TYPE_IND:
        hal_ble_adv_env.static_adv_param.adv_param.adv_prop = ADV_PROP_CONNECTABLE_BIT | ADV_PROP_SCANNABLE_BIT;
        break;

    case TRBLE_ADV_TYPE_DIRECT:
        LOGE("can't set direct adv because hal api doesn't input peer addr !!!");
        return TRBLE_FAIL;
        hal_ble_adv_env.static_adv_param.adv_param.adv_prop = ADV_PROP_CONNECTABLE_BIT | ADV_PROP_DIRECTED_BIT;
        break;

    case TRBLE_ADV_TYPE_SCAN_IND:
        hal_ble_adv_env.static_adv_param.adv_param.adv_prop = ADV_PROP_SCANNABLE_BIT;
        break;

    case TRBLE_ADV_TYPE_NONCONN_IND:
        hal_ble_adv_env.static_adv_param.adv_param.adv_prop = 0;
        break;

    default:
        LOGE("invalid adv type %d", adv_type);
        return TRBLE_FAIL;
        break;
    }

    if (addr)
    {
        switch (addr->type)
        {
        case TRBLE_ADDR_TYPE_PUBLIC:
            hal_ble_adv_env.static_adv_param.adv_param.own_addr_type = OWN_ADDR_TYPE_PUBLIC_ADDR;
            break;

        case TRBLE_ADDR_TYPE_RANDOM_STATIC:
        case TRBLE_ADDR_TYPE_RANDOM_RESOLVABLE:
        case TRBLE_ADDR_TYPE_RANDOM_NON_RESOLVABLE:
            LOGW("covert to OWN_ADDR_TYPE_RANDOM_ADDR");
            hal_ble_adv_env.static_adv_param.adv_param.own_addr_type = OWN_ADDR_TYPE_RANDOM_ADDR;
            break;

        default:
            LOGE("invalid addr type %d", addr->type);
            return TRBLE_FAIL;
            break;
        }

        os_memcpy(hal_ble_adv_env.static_adv_param.random_adv_addr, addr->mac,
                  sizeof(hal_ble_adv_env.static_adv_param.random_adv_addr));
    }

    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_advertiser_set_adv_interval(uint16_t interval)
{
    LOGD("%d", interval);

    if (hal_ble_adv_env.static_hal_index != 0xff &&
            hal_ble_adv_env.array[hal_ble_adv_env.static_hal_index].adv_status == BLE_HAL_ADV_STATE_ADVERTISING)
    {
        LOGE("static adv already start !!!");
        return TRBLE_FAIL;
    }

    hal_ble_adv_env.static_adv_param.adv_param.adv_intv_min = interval;
    hal_ble_adv_env.static_adv_param.adv_param.adv_intv_max = interval;

    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_advertiser_adv_enable(uint8_t enable)
{
    int32_t ret = TRBLE_SUCCESS;
    // uint8_t action = (enable ? CMD_BLE_CREATE_ADV : CMD_BLE_STOP_ADV);  // Unused variable

    LOGD("enable %d", enable);

    int32_t nest_ret = 0;
    uint8_t adv_index = 0xff;
    uint8_t hal_adv_index = 0xff;

    if (enable)
    {
        if (hal_ble_adv_env.static_hal_index != 0xff &&
                hal_ble_adv_env.array[hal_ble_adv_env.static_hal_index].adv_status == BLE_HAL_ADV_STATE_ADVERTISING)
        {
            LOGE("static adv already start !!!");
            return TRBLE_FAIL;
        }
        else
        {
            hal_ble_adv_env.static_hal_index = hal_ble_adv_env.static_index = 0xff;
        }

        ble_cmd_msg_elem_t elem = {0};

        elem.adv_elem = ADV_ELEM_STATIC;
        elem.adv_index = 0xff;
        elem.hal_adv_index = 0xff;
        elem.param = &s_sem;
        elem.nest_ret = &nest_ret;

        if (ble_cmd_queue_push_ext(CMD_BLE_CREATE_ADV, 0xff, &elem, 0, advertiser_adv_enable_nest_adv_cb))
        {
            LOGE("ble_cmd_queue_push fail");
            ret = TRBLE_FAIL;
            goto end;
        }

        ret = rtos_get_semaphore(&s_sem, API_SEM_TIMEOUT);

        if (ret)
        {
            LOGE("wait create static adv err %d", ret);
            ret = TRBLE_FAIL;
            goto end;
        }

        if (nest_ret)
        {
            LOGE("create static adv err %d", nest_ret);
            ret = TRBLE_FAIL;
            goto end;
        }

        adv_index = hal_ble_adv_env.static_index;
        hal_adv_index = hal_ble_adv_env.static_hal_index;

        if (ble_cmd_queue_push_ext(CMD_BLE_START_ADV, 0xff, &elem, 0, advertiser_adv_enable_nest_adv_cb))
        {
            LOGE("ble_cmd_queue_push fail");
            ret = TRBLE_FAIL;
            goto end;
        }

        ret = rtos_get_semaphore(&s_sem, API_SEM_TIMEOUT);

        if (ret)
        {
            LOGE("wait start static adv err %d", ret);
            ret = TRBLE_FAIL;
            goto end;
        }

        if (nest_ret)
        {
            LOGE("start static adv err %d", nest_ret);
            ret = TRBLE_FAIL;
            goto end;
        }

        LOGI("return create handle %d", adv_index);
    }
    else
    {
        if (hal_ble_adv_env.static_hal_index == 0xff)
        {
            ret = TRBLE_SUCCESS;
            goto end;
        }

        if (hal_ble_adv_env.static_hal_index != 0xff &&
                hal_ble_adv_env.array[hal_ble_adv_env.static_hal_index].adv_status != BLE_HAL_ADV_STATE_ADVERTISING)
        {
            LOGW("no need stop static adv status %d", hal_ble_adv_env.array[hal_ble_adv_env.static_hal_index].adv_status);
            hal_ble_adv_env.static_hal_index = hal_ble_adv_env.static_index = 0xff;
            ret = TRBLE_SUCCESS;
            goto end;
        }

        ble_cmd_msg_elem_t elem = {0};

        elem.adv_elem = ADV_ELEM_STATIC;
        elem.adv_index = hal_ble_adv_env.static_index;
        elem.hal_adv_index = 0xff;
        elem.param = &s_sem;
        elem.nest_ret = &nest_ret;

        if (ble_cmd_queue_push_ext(CMD_BLE_STOP_ADV, 0xff, &elem, 0, advertiser_adv_enable_nest_adv_cb))
        {
            LOGE("ble_cmd_queue_push fail");
            ret = TRBLE_FAIL;
            goto end;
        }

        ret = rtos_get_semaphore(&s_sem, API_SEM_TIMEOUT);

        if (ret)
        {
            LOGE("wait stop static adv err %d", ret);
            ret = TRBLE_FAIL;
            goto end;
        }

        if (nest_ret)
        {
            LOGE("start stop adv err %d", nest_ret);
            ret = TRBLE_FAIL;
            goto end;
        }

        if (ble_cmd_queue_push_ext(CMD_BLE_DEL_ADV, 0xff, &elem, 0, advertiser_adv_enable_nest_adv_cb))
        {
            LOGE("ble_cmd_queue_push fail");
            ret = TRBLE_FAIL;
            goto end;
        }

        ret = rtos_get_semaphore(&s_sem, API_SEM_TIMEOUT);

        if (ret)
        {
            LOGE("wait del static adv err %d", ret);
            ret = TRBLE_FAIL;
            goto end;
        }

        if (nest_ret)
        {
            LOGE("start del adv err %d", nest_ret);
            ret = TRBLE_FAIL;
            goto end;
        }

        hal_ble_adv_env.static_index = 0xff;
        hal_ble_adv_env.static_hal_index = 0xff;
    }

end:;

    return ret;
}

int32_t bk_tr_ble_advertiser_create_multi_adv(uint8_t adv_event_prop,
        uint32_t primary_adv_interval[2], //note: see rtk impl
        uint8_t own_addr_type,
        uint8_t *own_addr_val,
        uint8_t *adv_handle)
{
    int32_t ret = TRBLE_SUCCESS;

    if (!primary_adv_interval || !adv_handle)
    {
        LOGE("param err");
        return TRBLE_FAIL;
    }

    if (own_addr_val)
    {
        LOGD("prop 0x%x intv %d %d own addr type %d %02x:%02x:%02x:%02x:%02x:%02x", adv_event_prop,
             primary_adv_interval[0],
             primary_adv_interval[1],
             own_addr_type,
             own_addr_val[5],
             own_addr_val[4],
             own_addr_val[3],
             own_addr_val[2],
             own_addr_val[1],
             own_addr_val[0]);
    }
    else
    {
        LOGD("prop 0x%x intv %d %d own addr type %d", adv_event_prop,
             primary_adv_interval[0],
             primary_adv_interval[1]);
    }

    uint8_t tmp_adv_index = 0xff;
    uint8_t tmp_hal_adv_index = 0xff;

    int32_t nest_ret = 0;

    ble_cmd_msg_elem_t elem = {0};
    uint32_t temp_idx_ele[3] = {(uint32_t) &tmp_adv_index, (uint32_t) &tmp_hal_adv_index, (uint32_t) &nest_ret};
    elem.adv_elem = ADV_ELEM_NORMAL;
    elem.adv_index = tmp_adv_index;
    elem.hal_adv_index = 0xff;
    elem.param = &s_sem;
    elem.nest_ret = &temp_idx_ele[0];

    if (ble_cmd_queue_push_ext(CMD_BLE_CREATE_ADV, 0xff, &elem, 0, advertiser_create_multi_adv_nest_adv_cb))
    {
        LOGE("ble_cmd_queue_push fail");
        ret = TRBLE_FAIL;
        goto end;
    }

    ret = rtos_get_semaphore(&s_sem, API_SEM_TIMEOUT);

    if (ret)
    {
        LOGE("wait create adv err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }

    if (nest_ret)
    {
        LOGE("init multi adv err");
        ret = TRBLE_FAIL;
        goto end;
    }

    if (own_addr_type != BK_EXT_ADV_LE_ADDR_TYPE_PUBLIC && !own_addr_val)
    {
        LOGE("must set addr when type not public !!!");
        ret = TRBLE_FAIL;
        goto end;
    }

    switch (own_addr_type)
    {
    case BK_EXT_ADV_LE_ADDR_TYPE_PUBLIC:
        hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.own_addr_type = OWN_ADDR_TYPE_PUBLIC_ADDR;
        break;

    case BK_EXT_ADV_LE_ADDR_TYPE_RANDOM:
        hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.own_addr_type = OWN_ADDR_TYPE_RANDOM_ADDR;
        break;

    case BK_EXT_ADV_LE_ADDR_TYPE_RPA_PUBLIC:
        LOGW("unsupport rpa !!!");
        hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.own_addr_type = OWN_ADDR_TYPE_PUBLIC_ADDR;
        break;

    case BK_EXT_ADV_LE_ADDR_TYPE_RPA_RANDOM:
        LOGW("unsupport rpa !!!");
        hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.own_addr_type = OWN_ADDR_TYPE_RANDOM_ADDR;
        break;

    case BK_EXT_ADV_LE_ADDR_TYPE_PEER_ANONYMOUS:
        LOGE("unsupport BK_EXT_ADV_LE_ADDR_TYPE_PEER_ANONYMOUS !!!");
        ret = TRBLE_FAIL;
        goto end;
        break;

    default:
        LOGE("unknow addr type %d", own_addr_type);
        ret = TRBLE_FAIL;
        goto end;
        break;
    }

    if (own_addr_val)
    {
        os_memcpy(hal_ble_adv_env.array[tmp_hal_adv_index].own_addr.addr, own_addr_val, sizeof(hal_ble_adv_env.array[tmp_hal_adv_index].own_addr.addr));
    }

    hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.adv_type = ADV_TYPE_LEGACY;

    switch (adv_event_prop)
    {
    case BK_EXT_ADV_LEGACY_ADV_CONN_SCAN_UNDIRECTED:
        hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.adv_prop = ADV_PROP_CONNECTABLE_BIT | ADV_PROP_SCANNABLE_BIT;
        break;

    case BK_EXT_ADV_LEGACY_ADV_CONN_LOW_DUTY_DIRECTED:
        LOGE("can't set direct adv because hal api doesn't input peer addr !!!");
        ret = TRBLE_FAIL;
        goto end;
        hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.adv_prop = ADV_PROP_CONNECTABLE_BIT | ADV_PROP_DIRECTED_BIT;
        break;

    case BK_EXT_ADV_LEGACY_ADV_CONN_HIGH_DUTY_DIRECTED:
        LOGE("can't set direct adv because hal api doesn't input peer addr !!!");
        ret = TRBLE_FAIL;
        goto end;
        hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.adv_prop = ADV_PROP_CONNECTABLE_BIT | ADV_PROP_HDC_BIT;
        break;

    case BK_EXT_ADV_LEGACY_ADV_SCAN_UNDIRECTED:
        hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.adv_prop = ADV_PROP_SCANNABLE_BIT;
        break;

    case BK_EXT_ADV_LEGACY_ADV_NON_SCAN_NON_CONN_UNDIRECTED:
        hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.adv_prop = 0;
        break;

    case BK_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_UNDIRECTED:
    case BK_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_DIRECTED:
    case BK_EXT_ADV_EXTENDED_ADV_CONN_UNDIRECTED:
    case BK_EXT_ADV_EXTENDED_ADV_CONN_DIRECTED:
    case BK_EXT_ADV_EXTENDED_ADV_SCAN_UNDIRECTED:
    case BK_EXT_ADV_EXTENDED_ADV_SCAN_DIRECTED:
        LOGE("unsupport ext adv 0x%x !!!", adv_event_prop);
        ret = TRBLE_FAIL;
        goto end;
        break;

    default:
        LOGE("unknow adv_event_prop 0x%x", adv_event_prop);
        ret = TRBLE_FAIL;
        goto end;
        break;
    }

    hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.chnl_map = ADV_ALL_CHNLS;
    hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.adv_intv_min = primary_adv_interval[0];
    hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.adv_intv_max = primary_adv_interval[1];
    hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.prim_phy = PHY_TYPE_LE_1M;
    hal_ble_adv_env.array[tmp_hal_adv_index].adv_param.second_phy = PHY_TYPE_LE_1M;

    *adv_handle = tmp_adv_index;
    LOGI("return create handle %d", tmp_adv_index);
end:;

    if (ret)
    {
        LOGE("err");
    }

    return ret;
}

int32_t bk_tr_ble_advertiser_delete_multi_adv(uint8_t adv_handle)
{
    int32_t ret = 0;
    int32_t nest_ret = 0;

    uint8_t tmp_adv_index = adv_handle;
    uint8_t tmp_hal_adv_index = 0xff;

    LOGD("%d", adv_handle);

    ble_cmd_msg_elem_t elem = {0};
    uint32_t temp_idx_ele[3] = {(uint32_t) &tmp_adv_index, (uint32_t) &tmp_hal_adv_index, (uint32_t) &nest_ret};
    elem.adv_elem = ADV_ELEM_NORMAL;
    elem.adv_index = tmp_adv_index;
    elem.hal_adv_index = 0xff;
    elem.param = &s_sem;
    elem.nest_ret = &temp_idx_ele[0];

    if (ble_cmd_queue_push_ext(CMD_BLE_DEL_ADV, 0xff, &elem, 0, advertiser_delete_multi_adv_nest_adv_cb))
    {
        LOGE("ble_cmd_queue_push fail");
        ret = TRBLE_FAIL;
        goto end;
    }

    ret = rtos_get_semaphore(&s_sem, API_SEM_TIMEOUT);

    if (ret)
    {
        LOGE("wait del adv err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }

    if (nest_ret)
    {
        LOGE("del multi adv err");
        ret = TRBLE_FAIL;
        goto end;
    }

    reset_default_param(hal_ble_adv_env.array + tmp_hal_adv_index);

end:;

    if (ret)
    {
        LOGE("err");
    }

    return ret;
}

int32_t bk_tr_ble_advertiser_set_multi_adv_data(uint8_t adv_handle, uint8_t *pdata, uint8_t len)
{
    int32_t ret = 0;
    LOGD("handle %d len %d", adv_handle, len);

    if (len > sizeof(hal_ble_adv_env.array[0].advData))
    {
        LOGE("len too long");
        return TRBLE_FAIL;
    }

    int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(adv_handle);

    if (hal_adv_index < 0)
    {
        LOGE("can't find hal_adv_index %d", adv_handle);
        return TRBLE_FAIL;
    }

    os_memcpy(hal_ble_adv_env.array[hal_adv_index].advData, pdata, len);
    hal_ble_adv_env.array[hal_adv_index].advDataLen = len;

    if (hal_ble_adv_env.array[hal_adv_index].adv_status == BLE_HAL_ADV_STATE_ADVERTISING)
    {
        LOGW("need restart");
        bk_tr_ble_advertiser_multi_adv_enable(adv_handle, 0);
        bk_tr_ble_advertiser_multi_adv_enable(adv_handle, 1);
    }

end:;
    return ret;
}

int32_t bk_tr_ble_advertiser_set_multi_resp_data(uint8_t adv_handle, uint8_t *pdata, uint8_t len)
{
    int32_t ret = 0;
    LOGD("handle %d len %d", adv_handle, len);

    if (len > sizeof(hal_ble_adv_env.array[0].respData))
    {
        LOGE("len too long");
        return TRBLE_FAIL;
    }

    int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(adv_handle);

    if (hal_adv_index < 0)
    {
        LOGE("can't find hal_adv_index %d", adv_handle);
        return TRBLE_FAIL;
    }

    os_memcpy(hal_ble_adv_env.array[hal_adv_index].respData, pdata, len);
    hal_ble_adv_env.array[hal_adv_index].respDataLen = len;

    if (hal_ble_adv_env.array[hal_adv_index].adv_status == BLE_HAL_ADV_STATE_ADVERTISING)
    {
        LOGW("need restart");
        bk_tr_ble_advertiser_multi_adv_enable(adv_handle, 0);
        bk_tr_ble_advertiser_multi_adv_enable(adv_handle, 1);
    }

end:;
    return ret;
}

int32_t bk_tr_ble_advertiser_multi_adv_enable(uint8_t adv_handle, uint8_t enable)
{
    int32_t ret = 0;
    uint8_t action = (enable ? CMD_BLE_START_ADV : CMD_BLE_STOP_ADV);

    LOGD("handle %d enable %d", adv_handle, enable);

    int32_t nest_ret = 0;

    ble_cmd_msg_elem_t elem = {0};

    elem.adv_elem = ADV_ELEM_NORMAL;
    elem.adv_index = adv_handle;
    elem.hal_adv_index = 0xff;
    elem.param = &s_sem;
    elem.nest_ret = &nest_ret;

    if (ble_cmd_queue_push_ext(action, 0xff, &elem, 0, advertiser_multi_adv_enable_nest_adv_cb))
    {
        LOGE("ble_cmd_queue_push fail");
        ret = TRBLE_FAIL;
        goto end;
    }

    ret = rtos_get_semaphore(&s_sem, API_SEM_TIMEOUT);

    if (ret)
    {
        LOGE("wait start/stop adv err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }

    if (nest_ret)
    {
        LOGE("start/stop adv err %d", nest_ret);
        ret = TRBLE_FAIL;
        goto end;
    }

end:;
    return ret;
}

int32_t bk_tr_ble_advertiser_set_multi_adv_type(uint8_t adv_handle, uint8_t adv_event_prop, trble_addr *addr)
{
    int32_t ret = TRBLE_SUCCESS;

    if (addr)
    {
        LOGI("adv_handle %d adv_event_prop 0x%x mac_type %d %02x:%02x:%02x:%02x:%02x:%02x", adv_handle, adv_event_prop, addr->type,
             addr->mac[5],
             addr->mac[4],
             addr->mac[3],
             addr->mac[2],
             addr->mac[1],
             addr->mac[0]);
    }
    else
    {
        LOGI("adv_handle %d adv_event_prop 0x%x", adv_handle, adv_event_prop);
    }

    int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(adv_handle);

    if (hal_adv_index < 0)
    {
        LOGE("can't find hal_adv_index %d", adv_handle);
        return TRBLE_FAIL;
    }

    hal_ble_adv_env.array[hal_adv_index].adv_param.adv_type = ADV_TYPE_LEGACY;

    switch (adv_event_prop)
    {
    case BK_EXT_ADV_LEGACY_ADV_CONN_SCAN_UNDIRECTED:
        hal_ble_adv_env.array[hal_adv_index].adv_param.adv_prop = ADV_PROP_CONNECTABLE_BIT | ADV_PROP_SCANNABLE_BIT;
        break;

    case BK_EXT_ADV_LEGACY_ADV_CONN_LOW_DUTY_DIRECTED:
        LOGE("can't set direct adv because hal api doesn't input peer addr !!!");
        ret = TRBLE_FAIL;
        goto end;
        hal_ble_adv_env.array[hal_adv_index].adv_param.adv_prop = ADV_PROP_CONNECTABLE_BIT | ADV_PROP_DIRECTED_BIT;
        break;

    case BK_EXT_ADV_LEGACY_ADV_CONN_HIGH_DUTY_DIRECTED:
        LOGE("can't set direct adv because hal api doesn't input peer addr !!!");
        ret = TRBLE_FAIL;
        goto end;
        hal_ble_adv_env.array[hal_adv_index].adv_param.adv_prop = ADV_PROP_CONNECTABLE_BIT | ADV_PROP_HDC_BIT;
        break;

    case BK_EXT_ADV_LEGACY_ADV_SCAN_UNDIRECTED:
        hal_ble_adv_env.array[hal_adv_index].adv_param.adv_prop = ADV_PROP_SCANNABLE_BIT;
        break;

    case BK_EXT_ADV_LEGACY_ADV_NON_SCAN_NON_CONN_UNDIRECTED:
        hal_ble_adv_env.array[hal_adv_index].adv_param.adv_prop = 0;
        break;

    case BK_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_UNDIRECTED:
    case BK_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_DIRECTED:
    case BK_EXT_ADV_EXTENDED_ADV_CONN_UNDIRECTED:
    case BK_EXT_ADV_EXTENDED_ADV_CONN_DIRECTED:
    case BK_EXT_ADV_EXTENDED_ADV_SCAN_UNDIRECTED:
    case BK_EXT_ADV_EXTENDED_ADV_SCAN_DIRECTED:
        LOGE("unsupport ext adv 0x%x !!!", adv_event_prop);
        ret = TRBLE_FAIL;
        goto end;
        break;

    default:
        LOGE("unknow adv_event_prop 0x%x", adv_event_prop);
        ret = TRBLE_FAIL;
        goto end;
        break;
    }

    if (addr)
    {
        switch (addr->type)
        {
        case TRBLE_ADDR_TYPE_PUBLIC:
            hal_ble_adv_env.array[hal_adv_index].adv_param.own_addr_type = OWN_ADDR_TYPE_PUBLIC_ADDR;
            break;

        case TRBLE_ADDR_TYPE_RANDOM_STATIC:
        case TRBLE_ADDR_TYPE_RANDOM_RESOLVABLE:
        case TRBLE_ADDR_TYPE_RANDOM_NON_RESOLVABLE:
            LOGW("covert %d to OWN_ADDR_TYPE_RANDOM_ADDR", addr->type);
            hal_ble_adv_env.array[hal_adv_index].adv_param.own_addr_type = OWN_ADDR_TYPE_RANDOM_ADDR;
            break;

        default:
            LOGE("invalid addr type %d", addr->type);
            return TRBLE_FAIL;
            break;
        }

        os_memcpy(hal_ble_adv_env.array[hal_adv_index].own_addr.addr, addr->mac, sizeof(hal_ble_adv_env.array[hal_adv_index].own_addr.addr));
    }

    if (hal_ble_adv_env.array[hal_adv_index].adv_status == BLE_HAL_ADV_STATE_ADVERTISING)
    {
        LOGW("need restart");
        bk_tr_ble_advertiser_multi_adv_enable(adv_handle, 0);
        bk_tr_ble_advertiser_multi_adv_enable(adv_handle, 1);
    }

end:;
    return ret;
}

int32_t bk_tr_ble_advertiser_set_multi_adv_interval(uint8_t adv_handle, unsigned int interval)
{
    int32_t ret = TRBLE_SUCCESS;

    LOGD("adv_handle %d %d", adv_handle, interval);

    int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(adv_handle);

    if (hal_adv_index < 0)
    {
        LOGE("can't find hal_adv_index %d", adv_handle);
        return TRBLE_FAIL;
    }

    hal_ble_adv_env.array[hal_adv_index].adv_param.adv_intv_min = interval;
    hal_ble_adv_env.array[hal_adv_index].adv_param.adv_intv_max = interval;

    if (hal_ble_adv_env.array[hal_adv_index].adv_status == BLE_HAL_ADV_STATE_ADVERTISING)
    {
        LOGW("need restart");
        bk_tr_ble_advertiser_multi_adv_enable(adv_handle, 0);
        bk_tr_ble_advertiser_multi_adv_enable(adv_handle, 1);
    }

end:;
    return ret;
}

int32_t bk_tr_ble_advertiser_set_multi_adv_tx_power(uint8_t adv_handle, uint8_t txpower)
{
    //refer to ble_server.h
#define ADV_TX_POWER_MAX 0x31
#define ADV_TX_POWER_STEP 0.5  //dBm
#define ADV_TX_POWER_MIN_DBM -9 //dBm

    LOGD("txpower %d", txpower);
    float tx_gain = 0;

    if (txpower > ADV_TX_POWER_MAX)
    {
        txpower = ADV_TX_POWER_MAX;
    }

    tx_gain = (float)(ADV_TX_POWER_MIN_DBM + txpower * ADV_TX_POWER_STEP);

    int8_t hal_adv_index = hal_ble_find_adv_index_by_handle(adv_handle);

    if (hal_adv_index < 0)
    {
        LOGE("can't find hal_adv_index %d", adv_handle);
        return TRBLE_FAIL;
    }

    if ((BLE_HAL_ADV_STATE_CREATED == hal_ble_adv_env.array[hal_adv_index].adv_status)
            || (BLE_HAL_ADV_STATE_ADVERTISING == hal_ble_adv_env.array[hal_adv_index].adv_status))
    {
        bk_ble_set_adv_tx_power(hal_ble_adv_env.array[hal_adv_index].adv_idx, 0, tx_gain);
    }
    else
    {
        LOGE("Wrong adv status %d", hal_ble_adv_env.array[hal_adv_index].adv_status);
        return TRBLE_INVALID_STATE;
    }

    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_advertiser_one_shot_adv_init(void)
{
    int32_t ret = 0;

    if (hal_ble_adv_env.oneshot_init)
    {
        LOGE("already init !!!");
        return TRBLE_FAIL;
    }

    LOGD("");

    for (uint32_t i = 0; i < BLE_ONTSHOT_MAX_STATIC_PARAM_COUNT; ++i)
    {
        reset_default_param(hal_ble_adv_env.static_oneshot_adv_param + i);
        hal_ble_adv_env.static_oneshot_adv_param[i].max_evt = 1;
    }

    hal_ble_adv_env.oneshot_init = 1;
end:;
    return ret;
}

int32_t bk_tr_ble_advertiser_one_shot_adv_deinit(void)
{
    int32_t ret = 0;

    if (!hal_ble_adv_env.oneshot_init)
    {
        LOGE("already deinit !!!");
        return TRBLE_FAIL;
    }

    LOGD("");
    hal_ble_adv_env.oneshot_init = 0;
end:;
    return ret;
}

int32_t bk_tr_ble_advertiser_one_shot_adv_set(uint8_t *adv_id, trble_data *data_adv, trble_data *data_scan_rsp, uint8_t *type)
{
    int32_t ret = 0;

    if (!adv_id || !data_adv || !data_scan_rsp || !type)
    {
        LOGE("param err");
        return TRBLE_FAIL;
    }

    LOGD("id %d adv len %d scan rsp len %d type %d", *adv_id, data_adv->length, data_scan_rsp->length, *type);

    int8_t oneshot_index = get_oneshot_by_adv_id(*adv_id);

    if (oneshot_index < 0)
    {
        oneshot_index = alloc_oneshot();

        if (oneshot_index < 0)
        {
            LOGE("can't alloc oneshot");
            ret = TRBLE_OUT_OF_MEMORY;
            goto end;
        }
    }

    LOGI("alloc oneshot_index %d", oneshot_index);

    hal_ble_adv_env.static_oneshot_adv_param[oneshot_index].oneshot_app_id = *adv_id;

    switch (*type)
    {
    case TRBLE_ADV_TYPE_IND:
        hal_ble_adv_env.static_oneshot_adv_param[oneshot_index].adv_param.adv_prop = ADV_PROP_CONNECTABLE_BIT | ADV_PROP_SCANNABLE_BIT;
        break;

    case TRBLE_ADV_TYPE_DIRECT:
        LOGE("can't set direct adv because hal api doesn't input peer addr !!!");
        return TRBLE_FAIL;
        hal_ble_adv_env.static_oneshot_adv_param[oneshot_index].adv_param.adv_prop = ADV_PROP_CONNECTABLE_BIT | ADV_PROP_DIRECTED_BIT;
        break;

    case TRBLE_ADV_TYPE_SCAN_IND:
        hal_ble_adv_env.static_oneshot_adv_param[oneshot_index].adv_param.adv_prop = ADV_PROP_SCANNABLE_BIT;
        break;

    case TRBLE_ADV_TYPE_NONCONN_IND:
        hal_ble_adv_env.static_oneshot_adv_param[oneshot_index].adv_param.adv_prop = 0;
        break;

    default:
        LOGE("invalid adv type %d", *type);
        return TRBLE_FAIL;
        break;
    }

    os_memcpy(hal_ble_adv_env.static_oneshot_adv_param[oneshot_index].advData, data_adv->data, data_adv->length);
    hal_ble_adv_env.static_oneshot_adv_param[oneshot_index].advDataLen = data_adv->length;
    os_memcpy(hal_ble_adv_env.static_oneshot_adv_param[oneshot_index].respData, data_scan_rsp->data, data_scan_rsp->length);
    hal_ble_adv_env.static_oneshot_adv_param[oneshot_index].respDataLen = data_scan_rsp->length;

end:;
    return ret;
}

int32_t bk_tr_ble_advertiser_one_shot_adv_enable(uint8_t adv_id)
{
    int32_t ret = 0;
    LOGD("adv_id %d", adv_id);

    int8_t oneshot_index = get_oneshot_by_adv_id(adv_id);

    if (oneshot_index < 0)
    {
        LOGE("adv_id not set %d", adv_id);
        return TRBLE_FAIL;
    }

    int32_t nest_ret = 0;
    uint8_t tmp_adv_index = 0xff;
    uint8_t tmp_hal_adv_index = 0xff;

    ble_cmd_msg_elem_t elem = {0};
    uint32_t *elem_ret_para[3] = {(uint32_t *) &tmp_adv_index, (uint32_t *) &tmp_hal_adv_index, (uint32_t *) &nest_ret};

    elem.adv_elem = ADV_ELEM_ONESHOT;
    elem.adv_index = 0xff;
    elem.hal_adv_index = 0xff;
    elem.oneshot_index = oneshot_index;
    elem.param = &s_sem;
    elem.nest_ret = elem_ret_para;

    if (ble_cmd_queue_push_ext(CMD_BLE_CREATE_ADV, 0xff, &elem, 0, advertiser_one_shot_adv_enable_nest_oneshot_adv_cb))
    {
        LOGE("ble_cmd_queue_push fail");
        ret = TRBLE_FAIL;
        goto end;
    }

    ret = rtos_get_semaphore(&s_sem, API_SEM_TIMEOUT);

    if (ret)
    {
        LOGE("wait create oneshot err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }

    if (nest_ret)
    {
        LOGE("create oneshot err");
        ret = TRBLE_FAIL;
        goto end;
    }

    LOGI("create %d %d", tmp_adv_index, tmp_hal_adv_index);

    if (!s_oneshot_sem)
    {
        ret = rtos_init_semaphore(&s_oneshot_sem, 1);

        if (ret)
        {
            LOGE("init s_oneshot_sem err %d", ret);
            ret = TRBLE_FAIL;
            goto end;
        }
    }

    elem.adv_elem = ADV_ELEM_ONESHOT;
    elem.adv_index = tmp_adv_index;
    elem.hal_adv_index = tmp_hal_adv_index;
    elem.param = &s_sem;

    if (ble_cmd_queue_push_ext(CMD_BLE_START_ADV, 0xff, &elem, 0, advertiser_one_shot_adv_enable_nest_oneshot_adv_cb))
    {
        LOGE("ble_cmd_queue_push fail");
        ret = TRBLE_FAIL;
        goto end;
    }

    LOGD("start wait oneshot start");

    ret = rtos_get_semaphore(&s_sem, API_SEM_TIMEOUT);

    if (ret)
    {
        LOGE("wait start oneshot adv err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }

    if (nest_ret)
    {
        LOGE("start oneshot adv err %d", nest_ret);
        ret = TRBLE_FAIL;
        goto end;
    }

    LOGI("return create handle %d", tmp_adv_index);

    ret = rtos_get_semaphore(&s_oneshot_sem, API_SEM_TIMEOUT);

    if (ret)
    {
        LOGE("wait oneshot adv end err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }


    elem.adv_elem = ADV_ELEM_ONESHOT;
    elem.adv_index = tmp_adv_index;
    elem.hal_adv_index = tmp_hal_adv_index;
    elem.param = &s_sem;

    if (ble_cmd_queue_push_ext(CMD_BLE_DEL_ADV, 0xff, &elem, 0, advertiser_one_shot_adv_enable_nest_oneshot_adv_cb))
    {
        LOGE("ble_cmd_queue_push fail");
        ret = TRBLE_FAIL;
        goto end;
    }

    ret = rtos_get_semaphore(&s_sem, API_SEM_TIMEOUT);

    if (ret)
    {
        LOGE("wait del oneshot adv err %d", ret);
        ret = TRBLE_FAIL;
        goto end;
    }

    if (nest_ret)
    {
        LOGE("del oneshot adv err %d", nest_ret);
        ret = TRBLE_FAIL;
        goto end;
    }

end:;

    if (s_oneshot_sem)
    {
        if (rtos_deinit_semaphore(&s_oneshot_sem))
        {
            LOGE("deinit s_oneshot_sem err");
        }

        s_oneshot_sem = NULL;
    }

    return ret;
}

int32_t bk_tr_ble_advertiser_init(void)
{
    int32_t ret = TRBLE_SUCCESS;

    if (hal_ble_adv_env.init)
    {
        LOGE("already init");
        return TRBLE_ALREADY_WORKING;
    }

    LOGD("start");

    os_memset(&hal_ble_adv_env, 0, sizeof(hal_ble_adv_env));

    hal_ble_adv_env.static_hal_index = 0xff;
    hal_ble_adv_env.static_index = 0xff;

    hal_ble_adv_env.static_adv_param.adv_idx = 0xFF;
    hal_ble_adv_env.static_adv_param.adv_status = BLE_HAL_ADV_STATE_IDLE;
    hal_ble_adv_env.static_adv_param.adv_param.own_addr_type = OWN_ADDR_TYPE_PUBLIC_ADDR;
    hal_ble_adv_env.static_adv_param.adv_param.adv_type = ADV_TYPE_LEGACY;
    hal_ble_adv_env.static_adv_param.adv_param.chnl_map = ADV_ALL_CHNLS;
    hal_ble_adv_env.static_adv_param.adv_param.adv_prop = ADV_PROP_CONNECTABLE_BIT | ADV_PROP_SCANNABLE_BIT;
    hal_ble_adv_env.static_adv_param.adv_param.adv_intv_min = BLE_ADV_INTERVAL_MIN;
    hal_ble_adv_env.static_adv_param.adv_param.adv_intv_max = BLE_ADV_INTERVAL_MAX;
    hal_ble_adv_env.static_adv_param.adv_param.prim_phy = PHY_TYPE_LE_1M;
    hal_ble_adv_env.static_adv_param.adv_param.second_phy = PHY_TYPE_LE_1M;
    hal_ble_adv_env.static_adv_param.advDataLen = 0;
    hal_ble_adv_env.static_adv_param.respDataLen = 0;
    hal_ble_adv_env.static_adv_param.deleting = 0;
    hal_ble_adv_env.static_adv_param.oneshot_app_id = 0xff;

    for (uint32_t i = 0; i < BLE_ONTSHOT_MAX_STATIC_PARAM_COUNT; ++i)
    {
        reset_default_param(hal_ble_adv_env.static_oneshot_adv_param + i);
    }

    for (uint32_t i = 0; i < sizeof(hal_ble_adv_env.array) / sizeof(hal_ble_adv_env.array[0]); ++i)
    {
        reset_default_param(hal_ble_adv_env.array + i);
    }

    if (!s_sem)
    {
        ret = rtos_init_semaphore(&s_sem, 1);

        if (ret)
        {
            LOGE("init sem err %d", ret);
            ret = TRBLE_FAIL;
            goto end;
        }
    }

    hal_ble_adv_env.init = 1;
    ret = TRBLE_SUCCESS;

end:;
    LOGD("end");

    if (ret && s_sem)
    {
        if (rtos_deinit_semaphore(&s_sem))
        {
            LOGE("deinit sem err");
        }

        s_sem = NULL;
    }

    return ret;
}

int32_t bk_tr_ble_advertiser_deinit(void)
{
    int32_t ret = TRBLE_SUCCESS;

    if (!hal_ble_adv_env.init)
    {
        LOGE("already deinit");
        return TRBLE_ALREADY_WORKING;
    }

    LOGW("");

    if (!s_sem)
    {
        ret = rtos_init_semaphore(&s_sem, 1);

        if (ret)
        {
            LOGE("init sem err %d", ret);
            ret = TRBLE_FAIL;
            goto end;
        }
    }

    int32_t nest_ret = 0;

    for (uint32_t i = 0; i < sizeof(hal_ble_adv_env.array) / sizeof(hal_ble_adv_env.array[0]); ++i)
    {
        if (hal_ble_adv_env.array[i].adv_idx != 0xff)
        {
            ble_cmd_msg_elem_t elem = {0};

            elem.adv_elem = ADV_ELEM_STATIC;
            elem.adv_index = hal_ble_adv_env.array[i].adv_idx;
            elem.hal_adv_index = 0xff;
            elem.param = &s_sem;
            elem.nest_ret = &nest_ret;

            if (ble_cmd_queue_push_ext(CMD_BLE_STOP_ADV, 0xff, &elem, 0, ble_advertiser_deinit_nest_adv_cb))
            {
                LOGE("ble_cmd_queue_push fail");
                ret = TRBLE_FAIL;
                goto end;
            }

            ret = rtos_get_semaphore(&s_sem, API_SEM_TIMEOUT);

            if (ret)
            {
                LOGE("wait stop adv err %d", ret);
                ret = TRBLE_FAIL;
                goto end;
            }

            if (nest_ret)
            {
                LOGE("stop adv err %d index %d", nest_ret, i);
                ret = TRBLE_FAIL;
                goto end;
            }

            elem.adv_elem = ADV_ELEM_STATIC;
            elem.adv_index = hal_ble_adv_env.array[i].adv_idx;
            elem.hal_adv_index = 0xff;
            elem.param = &s_sem;

            if (ble_cmd_queue_push_ext(CMD_BLE_DEL_ADV, 0xff, &elem, 0, ble_advertiser_deinit_nest_adv_cb))
            {
                LOGE("ble_cmd_queue_push fail");
                ret = TRBLE_FAIL;
                goto end;
            }

            ret = rtos_get_semaphore(&s_sem, API_SEM_TIMEOUT);

            if (ret)
            {
                LOGE("wait sdel adv err %d", ret);
                ret = TRBLE_FAIL;
                goto end;
            }

            if (nest_ret)
            {
                LOGE("del adv err %d index %d", nest_ret, i);
                ret = TRBLE_FAIL;
                goto end;
            }
        }
    }

end:;
    hal_ble_adv_env.init = 0;
    LOGD("end");

    if (s_sem)
    {
        if (rtos_deinit_semaphore(&s_sem))
        {
            LOGE("deinit sem err");
        }

        s_sem = NULL;
    }

    return ret;
}

static int32_t advertiser_adv_enable_nest_adv_cb(uint32_t evt, int32_t status, void *param)
{
    ble_cmd_msg_elem_t *tmp_elem = (typeof(tmp_elem))param;
    beken_semaphore_t *sem = tmp_elem->param;

    *((int32_t *)tmp_elem->nest_ret) = status;

    if (status == 0)
    {
        hal_ble_adv_env.static_index = tmp_elem->adv_index;
        hal_ble_adv_env.static_hal_index = tmp_elem->hal_adv_index;
    }

    if (sem)
    {
        rtos_set_semaphore(sem);
    }

    return 0;
}

static int32_t advertiser_multi_adv_enable_nest_adv_cb(uint32_t evt, int32_t status, void *param)
{
    ble_cmd_msg_elem_t *tmp_elem = (typeof(tmp_elem))param;
    beken_semaphore_t *sem = tmp_elem->param;

    *((int32_t *)tmp_elem->nest_ret) = status;

    if (sem)
    {
        rtos_set_semaphore(sem);
    }

    return 0;
}

int32_t advertiser_create_multi_adv_nest_adv_cb(uint32_t evt, int32_t status, void *param)
{
    ble_cmd_msg_elem_t *tmp_elem = (typeof(tmp_elem))param;
    beken_semaphore_t *sem = tmp_elem->param;

    uint32_t *temp_idx_ele = (uint32_t *)tmp_elem->nest_ret;
    *(uint8_t *)(temp_idx_ele[0]) = tmp_elem->adv_index;
    *(uint8_t *)(temp_idx_ele[1]) = tmp_elem->hal_adv_index;
    *(int32_t *)(temp_idx_ele[2]) = status;

    if (sem)
    {
        rtos_set_semaphore(sem);
    }

    return 0;
}

static int32_t advertiser_delete_multi_adv_nest_adv_cb(uint32_t evt, int32_t status, void *param)
{
    ble_cmd_msg_elem_t *tmp_elem = (typeof(tmp_elem))param;
    beken_semaphore_t *sem = tmp_elem->param;

    uint32_t *temp_idx_ele = (uint32_t *)tmp_elem->nest_ret;
    *(uint8_t *)(temp_idx_ele[0]) = tmp_elem->adv_index;
    *(uint8_t *)(temp_idx_ele[1]) = tmp_elem->hal_adv_index;
    *(int32_t *)(temp_idx_ele[2]) = status;

    if (sem)
    {
        rtos_set_semaphore(sem);
    }

    return 0;
}

static int32_t ble_advertiser_deinit_nest_adv_cb(uint32_t evt, int32_t status, void *param)
{
    ble_cmd_msg_elem_t *tmp_elem = (typeof(tmp_elem))param;
    beken_semaphore_t *sem = tmp_elem->param;

    *((int32_t *)tmp_elem->nest_ret) = status;

    if (sem)
    {
        rtos_set_semaphore(sem);
    }

    return 0;
}

static int32_t advertiser_one_shot_adv_enable_nest_oneshot_adv_cb(uint32_t evt, int32_t status, void *param)
{
    ble_cmd_msg_elem_t *tmp_elem = (typeof(tmp_elem))param;
    beken_semaphore_t *sem = tmp_elem->param;

    uint32_t *elem_ret_para = (uint32_t *)tmp_elem->nest_ret;

    *((uint8_t *)(elem_ret_para[0])) = tmp_elem->adv_index;
    *((uint8_t *)(elem_ret_para[1])) = tmp_elem->hal_adv_index;
    *((int32_t *)(elem_ret_para[2])) = status;

    if (sem)
    {
        rtos_set_semaphore(sem);
    }

    return 0;
}

static int32_t nest_report_connected_evt(uint32_t evt, int32_t status, void *nest_pm)
{
    ble_cmd_msg_elem_t *elem = (typeof(elem))nest_pm;
    uint8_t conn_idx = elem->conn_idx;

    hal_ble_adv_env.static_index = 0xff;
    hal_ble_adv_env.static_hal_index = 0xff;

    os_free(elem);
    elem = NULL;

    bk_tr_ble_server_report_connected_evt(conn_idx, TRBLE_SERVER_LL_CONNECTED);

    return 0;
}
