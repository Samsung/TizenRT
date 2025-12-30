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

static trble_le_coc_init_config s_init_confg;

int32_t bk_tr_ble_coc_init(trble_le_coc_init_config *le_coc)
{
    LOGD("%p", le_coc);
    s_init_confg = *le_coc;
    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_coc_register_psm(uint8_t is_reg, uint16_t psm)
{
    int32_t ret = TRBLE_SUCCESS;

    LOGD("reg %d psm 0x%x\n", is_reg, psm);

end:;
    return ret;
}

int32_t bk_tr_ble_coc_set_psm_security(uint16_t le_psm, uint8_t active, uint8_t sec_mode, uint8_t key_size)
{
    int32_t ret = TRBLE_UNSUPPORTED;

    LOGD("psm 0x%x active %d sec_mode 0x%x size %d", le_psm, active, sec_mode, key_size);
    LOGW("unsupport now !!!");
end:;
    return ret;
}

int32_t bk_tr_ble_coc_set_param(uint16_t value)
{
    int32_t ret = TRBLE_UNSUPPORTED;
    LOGD("value %d", value);
    LOGW("unsupport now !!!");
end:;
    return ret;
}

int32_t bk_tr_ble_coc_get_param(uint8_t param_type, uint16_t cid, uint16_t *value)
{
    int32_t ret = TRBLE_UNSUPPORTED;
    LOGD("param_type %d cid 0x%x", param_type, cid);
    LOGW("unsupport now !!!");
end:;
    return ret;
}

int32_t bk_tr_ble_coc_connect(uint16_t conn_handle, uint16_t le_psm)
{
    int32_t ret = TRBLE_SUCCESS;
    LOGD("handle %d psm 0x%x", conn_handle, le_psm);

end:;
    return ret;
}

int32_t bk_tr_ble_coc_disconnect(uint16_t cid)
{
    int32_t ret = TRBLE_SUCCESS;
    LOGD("cid 0x%x", cid);

end:;
    return ret;
}

int32_t bk_tr_ble_coc_send_data(uint16_t cid, uint16_t len, uint8_t *data)
{
    int32_t ret = TRBLE_SUCCESS;
    LOGD("cid 0x%x len %d", cid, len);

end:;
    return ret;
}

int32_t bk_tr_ble_coc_init_private(void)
{
    int32_t ret = TRBLE_SUCCESS;
    LOGD("");
end:;
    return ret;
}

int32_t bk_tr_ble_coc_deinit_private(void)
{
    int32_t ret = TRBLE_SUCCESS;
    LOGD("");
end:;
    return ret;
}
