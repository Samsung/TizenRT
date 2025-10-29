#include <tinyara/net/if/ble.h>
#include "ble_tizenrt_impl.h"

#include "bk_ble_tizenrt.h"
#include "ble_tizenrt_scanner.h"

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

#define LOG_TAG "bkble_scannner"
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define LOGE(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_ERROR)   BK_BLE_LOGE(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGW(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_WARNING) BK_BLE_LOGW(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGI(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_INFO)    BK_BLE_LOGI(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGD(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_DEBUG)   BK_BLE_LOGD(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGV(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_VERBOSE) BK_BLE_LOGV(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)


int32_t bk_tr_ble_scanner_notice_cb(ble_notice_t notice, void *param)
{
    switch (notice)
    {
    case BLE_5_REPORT_ADV:
    {
        ble_recv_adv_t *r_ind = (ble_recv_adv_t *)param;

        if (hal_ble_scan_env.filt_duplicate && hal_ble_adv_reports_list_check(r_ind->adv_addr, r_ind->adv_addr_type, r_ind->evt_type))
        {
            LOGD("BLE_5_REPORT_ADV:duplicate adv");
            break;
        }

        if (hal_ble_scan_env.filter_param.enable)
        {
            if (hal_ble_scan_env.filter_param.p_filter == NULL || hal_ble_scan_env.filter_param.len == 0)
            {
                break;
            }

            if ((hal_ble_scan_env.filter_param.len != r_ind->data_len) ||
                    os_memcmp(hal_ble_scan_env.filter_param.p_filter, r_ind->data + hal_ble_scan_env.filter_param.offset, hal_ble_scan_env.filter_param.len))
            {
                break;
            }
        }

        uint8_t adv_type = r_ind->evt_type &REPORT_INFO_REPORT_TYPE_MASK;
        trble_scanned_device *scan_device;
        scan_device = os_zalloc(sizeof(trble_scanned_device));

        if (scan_device)
        {
            os_memcpy(scan_device->addr.mac, r_ind->adv_addr, TRBLE_BD_ADDR_MAX_LEN);
            scan_device->addr.type = r_ind->adv_addr_type;
            scan_device->rssi = r_ind->rssi;

            if (adv_type == REPORT_TYPE_ADV_EXT || adv_type == REPORT_TYPE_ADV_LEG || adv_type == REPORT_TYPE_PER_ADV)
            {
                if (r_ind->data_len <= sizeof(scan_device->raw_data))
                {
                    os_memcpy(scan_device->raw_data, r_ind->data, r_ind->data_len);
                    scan_device->raw_data_length = r_ind->data_len;
                }
                else
                {
                    os_memcpy(scan_device->raw_data, r_ind->data, sizeof(scan_device->raw_data));
                    scan_device->raw_data_length = sizeof(scan_device->raw_data);
                    LOGW("[BLE_HAL] raw_data buffer too small, data_len: %d, buffer size: %zu",
                         r_ind->data_len, sizeof(scan_device->raw_data));
                }
            }
            else
            {
                if (r_ind->data_len <= sizeof(scan_device->resp_data))
                {
                    os_memcpy(scan_device->resp_data, r_ind->data, r_ind->data_len);
                    scan_device->resp_data_length = r_ind->data_len;
                }
                else
                {
                    os_memcpy(scan_device->resp_data, r_ind->data, sizeof(scan_device->resp_data));
                    scan_device->resp_data_length = sizeof(scan_device->resp_data);
                    LOGW("[BLE_HAL] resp_data buffer too small, data_len: %d, buffer size: %zu",
                         r_ind->data_len, sizeof(scan_device->resp_data));
                }
            }

            if (adv_type == REPORT_TYPE_ADV_LEG)
            {
                switch (r_ind->evt_type)
                {
                case ADV_IND:
                    scan_device->adv_type = TRBLE_ADV_TYPE_IND;
                    break;

                case ADV_DIRECT_IND:
                    scan_device->adv_type = TRBLE_ADV_TYPE_DIRECT;
                    break;

                case ADV_SCAN_IND:
                    scan_device->adv_type = TRBLE_ADV_TYPE_SCAN_IND;
                    break;

                case ADV_NONCONN_IND:
                    scan_device->adv_type = TRBLE_ADV_TYPE_NONCONN_IND;
                    break;

                case SCAN_RSP:
                    scan_device->adv_type = TRBLE_ADV_TYPE_SCAN_RSP;
                    break;

                default:
                    scan_device->adv_type = TRBLE_ADV_TYPE_UNKNOWN;
                    break;
                }
            }
            else
            {
                scan_device->adv_type = TRBLE_ADV_TYPE_UNKNOWN;
            }

            if (ble_evt_queue_push(EVT_BLE_SCANNED_DEVICE_MSG, (void *)scan_device))
            {
                LOGE("ble_evt_queue_push failed");
                os_free(scan_device);
            }
        }
        else
        {
            LOGE("[BLE_HAL]os_malloc error ");
        }
    }
    break;

    default:
        break;
    }
    return 0;
}

int32_t bk_tr_ble_scanner_init(void)
{
    return 0;
}

int32_t bk_tr_ble_scanner_deinit(void)
{
    return 0;
}
