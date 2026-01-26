// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "os/os.h"
#include "components/bluetooth/bk_dm_bluetooth.h"
#include <components/log.h>
#include "bt_os_adapter.h"
#include "bluetooth_internal.h"
#include <modules/pm.h>
#if (CONFIG_BLE_AT_ENABLE)
#include "../include/private/bk_at_ble.h"
#endif


#define TAG       "bluetooth"
#define LOGV(...) BK_BLE_LOGV(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_BLE_LOGD(TAG, ##__VA_ARGS__)
#define LOGI(...) BK_BLE_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_BLE_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_BLE_LOGE(TAG, ##__VA_ARGS__)

static uint8_t bluetooth_already_init = 0;

extern int bk_bt_os_adapter_init(void);
extern int bk_bt_feature_init(void);
static beken_mutex_t bluetooth_mutex = NULL;

bk_bluetooth_status_t bk_bluetooth_get_status(void)
{
    if (bluetooth_already_init)
    {
        return BK_BLUETOOTH_STATUS_ENABLED;
    }
    else
    {
        return BK_BLUETOOTH_STATUS_UNINITIALIZED;
    }
}

static int bluetooth_deepsleep_enter_cb(uint64_t expected_time_ms, void *args)
{
    if (bluetooth_already_init)
    {
#if !CONFIG_BTDM_CONTROLLER_ONLY
        bluetooth_host_deinit();
#endif
        bluetooth_controller_deinit();
        bluetooth_already_init = 0;
    }
    return 0;
}

#if CONFIG_BLE_LV_SUPPORT
static int bluetooth_lv_enter_cb(uint64_t expected_time_ms, void *args)
{
    if (bluetooth_already_init)
    {
        void bluetooth_lowvol_enter_callback(void);
        bluetooth_lowvol_enter_callback();
    }
    return 0;
}
static int bluetooth_lv_exit_cb(uint64_t expected_time_ms, void *args)
{
    if (bluetooth_already_init)
    {
        void bluetooth_lowvol_exit_callback(void);
        bluetooth_lowvol_exit_callback();
    }
    return 0;
}
#endif

bt_err_t bk_bluetooth_init(void)
{
    bt_err_t ret;
    LOGI("%s start, %d,%p \r\n", __func__, bluetooth_already_init, bluetooth_mutex);
    if (bluetooth_mutex == NULL)
    {
        rtos_init_mutex(&bluetooth_mutex);
    }

    rtos_lock_mutex(&bluetooth_mutex);
    if (bluetooth_already_init)
    {
        LOGE("%s bluetooth already initialised\r\n", __func__);
        rtos_unlock_mutex(&bluetooth_mutex);
        return 0;
    }

    ret = bk_bt_os_adapter_init();
    if (ret)
    {
        LOGE("%s initialize bt os adapter failed\r\n", __func__);
        rtos_unlock_mutex(&bluetooth_mutex);
        return ret;
    }

    if ((ret = bk_bt_feature_init()) != 0)
    {
        LOGE("%s initialize bt feature failed\r\n", __func__);
        rtos_unlock_mutex(&bluetooth_mutex);
        return ret;
    }

    ret = bluetooth_controller_init();
    if (ret)
    {
        LOGE("%s initialize controller failed\r\n", __func__);
        rtos_unlock_mutex(&bluetooth_mutex);
        return ret;
    }

#if !CONFIG_BTDM_CONTROLLER_ONLY
    ret = bluetooth_host_init();
    if (ret)
    {
        LOGE("%s init host failed\r\n", __func__);
        rtos_unlock_mutex(&bluetooth_mutex);
        return ret;
    }
#endif

    pm_cb_conf_t enter_conf_bt = {NULL, NULL};
    enter_conf_bt.cb = bluetooth_deepsleep_enter_cb;
    bk_pm_sleep_register_cb(PM_MODE_DEEP_SLEEP, PM_DEV_ID_BTDM, &enter_conf_bt, NULL);
#if CONFIG_PM_SUPER_DEEP_SLEEP
    bk_pm_sleep_register_cb(PM_MODE_SUPER_DEEP_SLEEP, PM_DEV_ID_BTDM, &enter_conf_bt, NULL);
#endif
#if 0//defined (CONFIG_BLE_AT_ENABLE) && !defined(CONFIG_BTDM_CONTROLLER_ONLY) && defined(CONFIG_BLE)
	extern void ble_at_cmd_init(void);
	 	ble_at_cmd_init();
#endif

    #if CONFIG_BLE_LV_SUPPORT
    pm_cb_conf_t enter_lv_config = {NULL, NULL};
    pm_cb_conf_t exit_lv_config = {NULL, NULL};
    enter_lv_config.cb = bluetooth_lv_enter_cb;
    exit_lv_config.cb = bluetooth_lv_exit_cb;
    bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_BTDM, &enter_lv_config, &exit_lv_config);
    #endif

    bluetooth_already_init = 1;
    LOGI("%s ok\r\n", __func__);
    rtos_unlock_mutex(&bluetooth_mutex);
    return ret;
}

bt_err_t bk_bluetooth_deinit(void)
{
    bt_err_t ret;
    rtos_lock_mutex(&bluetooth_mutex);
    if (!bluetooth_already_init)
    {
        LOGE("%s bluetooth already de-initialised\r\n", __func__);
        rtos_unlock_mutex(&bluetooth_mutex);
        return 0;
    }
    LOGI("%s start, %d \r\n", __func__, bluetooth_already_init);
#if !CONFIG_BTDM_CONTROLLER_ONLY
    ret = bluetooth_host_deinit();
    if (ret)
    {
        LOGE("%s deinit host failed\r\n", __func__);
        rtos_unlock_mutex(&bluetooth_mutex);
        return ret;
    }
#endif

    ret = bluetooth_controller_deinit();
    if (ret)
    {
        LOGE("%s deinit controller failed\r\n", __func__);
        rtos_unlock_mutex(&bluetooth_mutex);
        return ret;
    }

    bk_pm_sleep_unregister_cb(PM_MODE_DEEP_SLEEP, PM_DEV_ID_BTDM, true, false);
#if CONFIG_PM_SUPER_DEEP_SLEEP
    bk_pm_sleep_unregister_cb(PM_MODE_SUPER_DEEP_SLEEP, PM_DEV_ID_BTDM, true, false);
#endif

    bluetooth_already_init = 0;

    LOGI("%s ok, %d \r\n", __func__, bluetooth_already_init);
    rtos_unlock_mutex(&bluetooth_mutex);
    return ret;
}

bt_err_t bk_bluetooth_get_address(uint8_t *addr)
{
    bt_err_t ret;

    if (!bluetooth_already_init)
    {
        return -1;
    }

    ret = bluetooth_get_mac(addr);
    return ret;
}


