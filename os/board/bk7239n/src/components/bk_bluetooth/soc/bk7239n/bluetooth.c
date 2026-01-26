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
#include "components/bluetooth/bk_ble_types.h"
#include <components/log.h>
#include "bluetooth_internal.h"
#include "bt_os_adapter.h"
#include "driver/int.h"
#include "sys_driver.h"
#include "bk_uart.h"
#include "gpio_driver.h"
#include <driver/int_types.h>
#include <driver/hal/hal_int_types.h>
#include "bk_drv_model.h"
#include <modules/pm.h>
#include <driver/pwr_clk.h>
#include <components/system.h>
#include <driver/uart.h>
#include "arch_interrupt.h"
#include <driver/gpio.h>
#include <os/mem.h>
#include <os/str.h>
#include <string.h>
#include "aon_pmu_hal.h"
#include "bk_rf_internal.h"

#if CONFIG_CACHE_ENABLE
#include "cache.h"
#endif

#include <driver/aon_rtc.h>
#define PM_BT_LP_RTC_ALARM_NAME PM_BT_RTC_ALARM_NAME

#define TAG       "bluetooth"
#define LOGV(...) BK_BLE_LOGV(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_BLE_LOGD(TAG, ##__VA_ARGS__)
#define LOGI(...) BK_BLE_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_BLE_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_BLE_LOGE(TAG, ##__VA_ARGS__)

#define BT_OSI_VERSION              0x00010001

#define BLUETOOTH_TASK_DEFAULT_STACK_SIZE       (1024 * 2)
#define BLUETOOTH_TASK_MAIN_STACK_SIZE          (1024 * 3)
#define BLUETOOTH_TASK_HCI_REPORT_STACK_SIZE    (BLUETOOTH_TASK_DEFAULT_STACK_SIZE)
#define BLUETOOTH_TASK_DM_CTX_STACK_SIZE        (BLUETOOTH_TASK_DEFAULT_STACK_SIZE)
#define BLUETOOTH_TASK_DM_WT_STACK_SIZE         (1024 * 3)
#define BLUETOOTH_TASK_DM_RD_STACK_SIZE         (1024 * 4)

typedef struct
{
    uint8_t is_oneshot;
    uint32_t len;
    uint32_t data[0];
} bluetooth_timer_t;

int coex_init_wrapper(void)
{
#if CONFIG_WIFI_ENABLE
    // extern int coex_init(void);
    // return coex_init();
#endif
    return BK_OK;
}

static bk_err_t bluetooth_int_isr_register_wrapper(uint8_t type, void *isr, void *arg)
{
    icu_int_src_t src = INT_SRC_BTDM;

    if (type == BLUETOOTH_INT_SRC_BTDM)
    {
        src = INT_SRC_BTDM;
    }
    else if (type == BLUETOOTH_INT_SRC_BLE)
    {
        src = INT_SRC_BLE;
    }
    else if (type == BLUETOOTH_INT_SRC_BT)
    {
        src = INT_SRC_BT;
    }
    else
    {
        //error
        return -1;
    }

    return bk_int_isr_register(src, (int_group_isr_t)isr, arg);
}

static bk_err_t init_queue_wrapper(void **queue, const char *name, uint32_t message_size, uint32_t number_of_messages)
{
    return rtos_init_queue(queue, name, message_size, number_of_messages);
}

static bk_err_t deinit_queue_wrapper(void **queue)
{
    return rtos_deinit_queue(queue);
}

static bk_err_t pop_from_queue_wrapper(void **queue, void *message, uint32_t timeout_ms)
{
    return rtos_pop_from_queue(queue, message, timeout_ms);
}

static bk_err_t push_to_queue_wrapper(void **queue, void *message, uint32_t timeout_ms)
{
    return rtos_push_to_queue(queue, message, timeout_ms);
}

static bk_err_t create_thread_wrapper(void **thread, uint8_t priority, const char *name,
                                      void *function, uint32_t stack_size, void *arg)
{
    return rtos_create_thread(thread, priority, name, function, stack_size, arg);
}

static bk_err_t delete_thread_wrapper(void **thread)
{
    return rtos_delete_thread(thread);
}

static int thread_join(void **thread)
{
    return rtos_thread_join(thread);
}

static int get_thread_stack_size(uint16_t task_id)
{
    int ret = BLUETOOTH_TASK_DEFAULT_STACK_SIZE;

    switch (task_id)
    {
        case BLUETOOTH_TASK_MAIN:
            ret = BLUETOOTH_TASK_MAIN_STACK_SIZE;
            break;
        case BLUETOOTH_TASK_HCI_REPORT:
            ret = BLUETOOTH_TASK_HCI_REPORT_STACK_SIZE;
            break;
        case BLUETOOTH_TASK_DM_CTX:
            ret = BLUETOOTH_TASK_DM_CTX_STACK_SIZE;
            break;
        case BLUETOOTH_TASK_DM_WT:
            ret = BLUETOOTH_TASK_DM_WT_STACK_SIZE;
            break;
        case BLUETOOTH_TASK_DM_RD:
            ret = BLUETOOTH_TASK_DM_RD_STACK_SIZE;
            break;
        default:
            ret = BLUETOOTH_TASK_DEFAULT_STACK_SIZE;
            break;
    }

    return ret;
}

static void controller_mem_init_wrapper(void)
{
#if CONFIG_SOC_BK7256XX
#define MEMSET(s, c, n)         __builtin_memset ((s), (c), (n))

    extern char _bt_data_start, _bt_data_end;
    unsigned int size = &_bt_data_end - &_bt_data_start;

    MEMSET(&_bt_data_start, 0, size);
#endif
}

static bk_err_t bluetooth_deep_sleep_register_wrapper(void *enter_config_cb)
{
    pm_cb_conf_t enter_conf_bt = {NULL, NULL};

    enter_conf_bt.cb = enter_config_cb;

    return bk_pm_sleep_register_cb(PM_MODE_DEEP_SLEEP, PM_DEV_ID_BTDM, &enter_conf_bt, NULL);
}

static bk_err_t bluetooth_extern32k_register_wrapper(void *switch_cb)
{
    pm_cb_extern32k_cfg_t bt_pm_cb_extern32k_cfg =
    {
        .cb_module = PM_32K_MODULE_BT,
        .cb_func = NULL,
    };

    bt_pm_cb_extern32k_cfg.cb_func = switch_cb;

    return pm_extern32k_register_cb(&bt_pm_cb_extern32k_cfg);
}

static bk_err_t bluetooth_extern32k_unregister_wrapper(void)
{
    pm_cb_extern32k_cfg_t bt_pm_cb_extern32k_cfg =
    {
        .cb_module = PM_32K_MODULE_BT,
        .cb_func = NULL,
    };

    return pm_extern32k_unregister_cb(&bt_pm_cb_extern32k_cfg);
}

static uint8_t clk_32k_customer_config_get_wrapper(void)
{
    uint8_t lpo_src = 0;
    pm_lpo_src_e src = bk_clk_32k_customer_config_get();

    if (PM_LPO_SRC_DIVD == src)
    {
        lpo_src = BT_LPO_SRC_DIVD;
    }
    else if (PM_LPO_SRC_X32K == src)
    {
        lpo_src = BT_LPO_SRC_X32K;
    }
    else if (PM_LPO_SRC_ROSC == src)
    {
        lpo_src = BT_LPO_SRC_ROSC;
    }
    else
    {
        lpo_src = BT_LPO_SRC_DEFAULT;
    }

    return lpo_src;
}

static bk_err_t lpo_src_set_wrapper(uint8_t lpo_src)
{
    pm_lpo_src_e src;

    if (BT_LPO_SRC_DIVD == lpo_src)
    {
        src = PM_LPO_SRC_DIVD;
    }
    else if (BT_LPO_SRC_X32K == lpo_src)
    {
        src = PM_LPO_SRC_X32K;
    }
    else if (BT_LPO_SRC_ROSC == lpo_src)
    {
        src = PM_LPO_SRC_ROSC;
    }
    else
    {
        src = PM_LPO_SRC_DEFAULT;
    }

    return bk_pm_lpo_src_set(src);
}

uint8_t lpo_src_get_wrapper(void)
{
    uint8_t lpo_src = 0;
    pm_lpo_src_e src = bk_pm_lpo_src_get();

    if (PM_LPO_SRC_DIVD == src)
    {
        lpo_src = BT_LPO_SRC_DIVD;
    }
    else if (PM_LPO_SRC_X32K == src)
    {
        lpo_src = BT_LPO_SRC_X32K;
    }
    else if (PM_LPO_SRC_ROSC == src)
    {
        lpo_src = BT_LPO_SRC_ROSC;
    }
    else
    {
        lpo_src = BT_LPO_SRC_DEFAULT;
    }

    return lpo_src;
}

static bk_err_t bluetooth_power_ctrl_wrapper(uint8_t power_state)
{
    pm_power_module_state_e state = (power_state ? PM_POWER_MODULE_STATE_ON : PM_POWER_MODULE_STATE_OFF);
    return bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BTSP, state);
}

static bk_err_t phy_power_ctrl_wrapper(uint8_t power_state)
{
    pm_power_module_state_e state = (power_state ? PM_POWER_MODULE_STATE_ON : PM_POWER_MODULE_STATE_OFF);
    return bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_PHY_BT, state);
}

static bk_err_t bt_mac_clock_ctrl_wrapper(uint8_t clock_state)
{
    pm_dev_clk_pwr_e state = (clock_state ? PM_CLK_CTRL_PWR_UP : PM_CLK_CTRL_PWR_DOWN);
    return bk_pm_clock_ctrl(PM_CLK_ID_BTDM, state);
}

static bk_err_t bt_phy_clock_ctrl_wrapper(uint8_t clock_state)
{
    pm_dev_clk_pwr_e state = (clock_state ? PM_CLK_CTRL_PWR_UP : PM_CLK_CTRL_PWR_DOWN);
    return bk_pm_clock_ctrl(PM_CLK_ID_XVR, state);
}

static void btdm_interrupt_ctrl_wrapper(bool en)
{
    sys_drv_btdm_interrupt_ctrl(en);
}

static void ble_interrupt_ctrl_wrapper(bool en)
{
    sys_drv_ble_interrupt_ctrl(en);
}

static void bt_interrupt_ctrl_wrapper(bool en)
{
    sys_drv_bt_interrupt_ctrl(en);
}

static bool ate_is_enabled_wrapper(void)
{
    extern bool ate_is_enabled(void);
    return ate_is_enabled();
}

static bool cp_test_is_enabled_wrapper(void)
{
    extern bool cp_is_enabled(void);
    return cp_is_enabled();
}

static bk_err_t get_bluetooth_mac_wrapper(uint8_t *mac)
{
    return bk_get_mac(mac, MAC_TYPE_BLUETOOTH);
}

static bk_err_t uart_write_byte_wrapper(uint8_t id, uint8_t data)
{
    return uart_write_byte(id, data);
}

static void register_ble_dump_hook_wrapper(void *ble_func)
{
#if CONFIG_ARCH_RISCV
    rtos_regist_ble_dump_hook(ble_func);
#endif
}

static uint8_t get_ble_pwr_idx_wrapper(uint8_t channel)
{
    extern uint8_t manual_cal_get_ble_pwr_idx(uint8_t channel);
    return manual_cal_get_ble_pwr_idx(channel);
}

static void ble_cal_set_txpwr_wrapper(uint8_t idx, uint8_t channel)
{
    extern void ble_cal_set_txpwr(uint8_t idx, uint8_t chan);
    ble_cal_set_txpwr(idx, channel);
}

static void ble_cal_recover_txpwr_wrapper(void)
{
    extern void ble_cal_recover_txpwr(void);
    ble_cal_recover_txpwr();
}

static void ble_cal_enter_txpwr_wrapper(void)
{
    extern void ble_cal_enter_txpwr(void);
    ble_cal_enter_txpwr();
}

static bk_err_t gpio_dev_unmap_wrapper(uint32_t gpio_id)
{
    return gpio_dev_unmap(gpio_id);
}

static bk_err_t gpio_dev_map_wrapper(uint32_t gpio_id, uint32_t func)
{
    return gpio_dev_map(gpio_id, func);
}

static void set_printf_enable_wrapper(uint8_t enable)
{
    bk_set_printf_enable(enable);
}

static void evm_stop_bypass_mac_wrapper(void)
{
#if CONFIG_WIFI_ENABLE
    extern void evm_stop_bypass_mac(void);
    evm_stop_bypass_mac();
#endif
}

static void rs_deinit_wrapper(void)
{
#if CONFIG_WIFI_ENABLE
    extern void rs_deinit(void);
    rs_deinit();
#endif
}

static bk_err_t uart_enable_rx_interrupt_wrapper(uint8_t id)
{
    return bk_uart_enable_rx_interrupt(id);
}

static bk_err_t uart_take_rx_isr_wrapper(uint8_t id, void *isr, void *param)
{
    int ret = 0;
    bk_uart_disable_rx_interrupt(id);
    bk_uart_set_enable_rx(id, 0);
    ret = bk_uart_take_rx_isr(id, isr, param);
    bk_uart_set_enable_rx(id, 1);
    bk_uart_enable_rx_interrupt(id);
    return ret;
}

static void ble_vote_rf_ctrl_wrapper(uint8_t cmd)
{
    rf_module_vote_ctrl(cmd, RF_BY_BLE_BIT);

#if CONFIG_MAC802154_ENABLE
    extern void rfconfig_rf_switch_to_bt(void);
    extern void rfconfig_rf_switch_to_thread(void);
    if (cmd) {
        rfconfig_rf_switch_to_bt();
    } else {
        rfconfig_rf_switch_to_thread();
    }
#endif
}

static void ble_ate_vote_rf_ctrl_wrapper(uint8_t cmd)
{
    rf_module_vote_ctrl(cmd, RF_BY_ATE_BT_BIT);
}

static bk_err_t delay_milliseconds_wrapper(uint32_t num_ms)
{
    return rtos_delay_milliseconds(num_ms);
}

static uint32_t disable_int_wrapper(void)
{
    return rtos_enter_critical();
}

static void enable_int_wrapper(uint32_t int_level)
{
    rtos_exit_critical(int_level);
}

static bk_err_t gpio_disable_pull_wrapper(uint32_t gpio_id)
{
    return bk_gpio_disable_pull(gpio_id);
}

static bk_err_t gpio_enable_output_wrapper(uint32_t gpio_id)
{
    return bk_gpio_enable_output(gpio_id);
}

static bk_err_t gpio_set_output_high_wrapper(uint32_t gpio_id)
{
    return bk_gpio_set_output_high(gpio_id);
}

static bk_err_t gpio_set_output_low_wrapper(uint32_t gpio_id)
{
    return bk_gpio_set_output_low(gpio_id);
}

static bk_err_t gpio_pull_down_wrapper(uint32_t gpio_id)
{
    return bk_gpio_pull_down(gpio_id);
}

static bk_err_t gpio_pull_up_wrapper(uint32_t gpio_id)
{
    return bk_gpio_pull_up(gpio_id);
}

static int get_printf_port_wrapper(void)
{
    return bk_get_printf_port();
}

static int get_dut_port_wrapper(void)
{
    return CONFIG_UART_CLI_BKREG_PORT;
}

static void uart_enable_wrapper(uint8_t uart_id, uint8_t enable, uint32_t band)
{
    bk_err_t ret = 0;

    if (enable)
    {
        uart_config_t config =
        {
            //.baud_rate = UART_BAUDRATE_115200,
            .baud_rate = UART_BAUDRATE_2000000,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_NONE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_FLOWCTRL_DISABLE,
            .src_clk = UART_SCLK_XTAL_26M
        };

        switch (band)
        {
            case 115200:
                config.baud_rate = UART_BAUDRATE_115200;
                break;

            case 921600:
                config.baud_rate = UART_BAUDRATE_921600;
                break;

            case 1000000:
                config.baud_rate = 1000000;
                break;

            case 3250000:
                config.baud_rate = UART_BAUDRATE_3250000;
                break;

            case 2000000:
            default:
                config.baud_rate = UART_BAUDRATE_2000000;
                break;
        }

        switch (uart_id)
        {
            case UART_ID_0:
                gpio_dev_unmap(GPIO_10);
                gpio_dev_unmap(GPIO_11);
                break;

            case UART_ID_1:
                gpio_dev_unmap(GPIO_0);
                gpio_dev_unmap(GPIO_1);
                break;

            case UART_ID_2:
                gpio_dev_unmap(GPIO_40);
                gpio_dev_unmap(GPIO_41);
                break;

            default:
                LOGE("%s uart_id err %d\n", __func__, uart_id);
                return;
                break;
        }

        ret = bk_uart_init(uart_id, &config);

        if (ret != 0)
        {
            LOGE("%s bk_uart_init err, ret %d\n", __func__, ret);
            return;
        }

        LOGD("%s ble uart %d enable\n", __func__, uart_id);
    }
    else
    {
        bk_uart_deinit(uart_id);
        LOGD("%s ble uart %d disable\n", __func__, uart_id);
    }
}

void enable_debug_gpio_wrapper(void)
{
    gpio_dev_unmap(GPIO_2);
    gpio_dev_map(GPIO_2, GPIO_DEV_DEBUG0);//CLK(H6)
    gpio_dev_unmap(GPIO_3);
    gpio_dev_map(GPIO_3, GPIO_DEV_DEBUG1);//CMD(H6)
    gpio_dev_unmap(GPIO_4);
    gpio_dev_map(GPIO_4, GPIO_DEV_DEBUG2);//D0(H6)
    gpio_dev_unmap(GPIO_5);
    gpio_dev_map(GPIO_5, GPIO_DEV_DEBUG3);//D1(H6)
    gpio_dev_unmap(GPIO_6);
    gpio_dev_map(GPIO_6, GPIO_DEV_DEBUG4);//CD(H6)
    gpio_dev_unmap(GPIO_7);
    gpio_dev_map(GPIO_7, GPIO_DEV_DEBUG5);//P7(H5)
    if (PM_LPO_SRC_X32K != bk_clk_32k_customer_config_get())
    {
        gpio_dev_unmap(GPIO_8);
        gpio_dev_map(GPIO_8, GPIO_DEV_DEBUG6);//P8(CON4)
        gpio_dev_unmap(GPIO_9);
        gpio_dev_map(GPIO_9, GPIO_DEV_DEBUG7);//P9(CON4)
    }
    else
    {
        LOGE("%s, external 32k is in use, GPIO8 and GPIO9 can not be used to debug \r\n", __func__);
    }

    gpio_dev_unmap(GPIO_12);
    gpio_dev_map(GPIO_12, GPIO_DEV_DEBUG8);
    gpio_dev_unmap(GPIO_13);
    gpio_dev_map(GPIO_13, GPIO_DEV_DEBUG9);
    gpio_dev_unmap(GPIO_14);
    gpio_dev_map(GPIO_14, GPIO_DEV_DEBUG10);
    gpio_dev_unmap(GPIO_15);
    gpio_dev_map(GPIO_15, GPIO_DEV_DEBUG11);
    gpio_dev_unmap(GPIO_16);
    gpio_dev_map(GPIO_16, GPIO_DEV_DEBUG12);
    gpio_dev_unmap(GPIO_17);
    gpio_dev_map(GPIO_17, GPIO_DEV_DEBUG13);
    gpio_dev_unmap(GPIO_18);
    gpio_dev_map(GPIO_18, GPIO_DEV_DEBUG14);
    gpio_dev_unmap(GPIO_19);
    gpio_dev_map(GPIO_19, GPIO_DEV_DEBUG15);
}


static uint32_t get_time_wrapper(void)
{
    return rtos_get_time();
}

static void *malloc_wrapper(size_t size)
{
    return os_malloc(size);
}

static void free_wrapper(void *ptr)
{
    os_free(ptr);
}


#define EVM_DEFUALT_BLE_RATE                   (158)
#define PHY_BAND_2G4                           (0)
#define PHY_CHNL_BW_20                         (0)
static void manual_cal_save_ble_txpwr_wrapper(uint32_t channel, uint32_t pwr_gain)
{
    extern void manual_cal_save_txpwr(UINT32 rate, UINT32 rfband, UINT32 bandwidth, UINT32 channel, UINT32 pwr_gain);
    manual_cal_save_txpwr(EVM_DEFUALT_BLE_RATE, PHY_BAND_2G4, PHY_CHNL_BW_20, channel, pwr_gain);
}


static void bluetooth_timer2_handler_cb(void *arg1, void *arg2)
{
    timer_handler_t func = (typeof(func))arg1;
    func(arg2);
}

static bool is_timer_init_wrapper(void *timer)
{
    bluetooth_timer_t *tmp = (typeof(tmp))timer;

    if (!tmp)
    {
        return 0;
    }

    if (tmp->is_oneshot)
    {
        return rtos_is_oneshot_timer_init((beken2_timer_t *)tmp->data);
    }
    else
    {
        return rtos_is_timer_init((beken_timer_t *)tmp->data);
    }
}

static int32_t init_timer_ext_wrapper(void **timer, uint32_t time_ms, void *function, void *arg, bool oneshot)
{
    int ret = 0;
    bluetooth_timer_t *tmp = NULL;
    uint32_t size = 0;

    if (oneshot)
    {
        size = sizeof(*tmp) + sizeof(beken2_timer_t);
        tmp = (typeof(tmp))os_malloc(size);
    }
    else
    {
        size = sizeof(*tmp) + sizeof(beken_timer_t);
        tmp = (typeof(tmp))os_malloc(size);
    }

    if (!tmp)
    {
        LOGE("%s, malloc failed\r\n", __func__);
        return -1;
    }

    os_memset(tmp, 0, size);
    tmp->len = size;
    tmp->is_oneshot = oneshot;

    if (oneshot)
    {
        ret = rtos_init_oneshot_timer((beken2_timer_t *)tmp->data, time_ms, bluetooth_timer2_handler_cb, function, arg);
    }
    else
    {
        ret = rtos_init_timer((beken_timer_t *)tmp->data, time_ms, function, arg);
    }

    if (ret)
    {
        os_free(tmp);
    }
    else
    {
        *timer = (typeof(*timer))tmp;
    }

    return ret;
}

static int32_t init_timer_wrapper(void **timer, uint32_t time_ms, void *function, void *arg)
{
    return init_timer_ext_wrapper(timer, time_ms, function, arg, 0);
}

static int32_t start_timer_wrapper(void *timer)
{
    bluetooth_timer_t *tmp = (typeof(tmp))timer;

    if (!tmp)
    {
        return -1;
    }

    if (tmp->is_oneshot)
    {
        return rtos_start_oneshot_timer((beken2_timer_t *)tmp->data);
    }
    else
    {
        return rtos_start_timer((beken_timer_t *)tmp->data);
    }
}

static bool is_timer_running_wrapper(void *timer)
{
    bluetooth_timer_t *tmp = (typeof(tmp))timer;

    if (!tmp)
    {
        return 0;
    }

    if (tmp->is_oneshot)
    {
        return rtos_is_oneshot_timer_running((beken2_timer_t *)tmp->data);
    }
    else
    {
        return rtos_is_timer_running((beken_timer_t *)tmp->data);
    }
}

static int32_t stop_timer_wrapper(void *timer)
{
    bluetooth_timer_t *tmp = (typeof(tmp))timer;

    if (!tmp)
    {
        return -1;
    }

    if (tmp->is_oneshot)
    {
        return rtos_stop_oneshot_timer((beken2_timer_t *)tmp->data);
    }
    else
    {
        return rtos_stop_timer((beken_timer_t *)tmp->data);
    }
}

static int32_t deinit_timer_wrapper(void *timer)
{
    bk_err_t ret = 0;
    bluetooth_timer_t *tmp = (typeof(tmp))timer;

    if (!tmp)
    {
        return -1;
    }

    if (tmp->is_oneshot)
    {
        ret = rtos_deinit_oneshot_timer((beken2_timer_t *)tmp->data);
    }
    else
    {
        ret = rtos_deinit_timer((beken_timer_t *)tmp->data);
    }

    os_free(tmp);

    return ret;
}

static int32_t timer_change_period(void *timer, uint32_t time_ms)
{
    bluetooth_timer_t *tmp = (typeof(tmp))timer;

    if (!tmp)
    {
        return -1;
    }

    if (tmp->is_oneshot)
    {
        //todo: oneshot should use oneshot api, wait impl
        return rtos_change_period((beken_timer_t *)tmp->data, time_ms);
    }
    else
    {
        return rtos_change_period((beken_timer_t *)tmp->data, time_ms);
    }
}

#if 0
#define RF_WIFIPLL_HOLD_BY_BLE_BIT                   (1 << 1)
#define RF_WIFIPLL_HOLD_BY_WIFI_BIT                  (1 << 0)

enum
{
    CMD_RF_WIFIPLL_HOLD_BIT_SET,
    CMD_RF_WIFIPLL_HOLD_BIT_CLR,
};

//extern UINT32 rf_pll_ctrl(UINT32 cmd, UINT32 param);
__attribute__((weak)) uint32_t rf_pll_ctrl(uint32_t cmd, uint32_t param)
{
    BK_ASSERT_EX(0, "%s this func shout not call !!!!!\n");
    return 0;
}
#endif
static uint32_t bt_rf_pll_ctrl_wrapper(uint32_t set)
{
#if CONFIG_WIFI_ENABLE
    uint32_t cmd = (set ? CMD_RF_WIFIPLL_HOLD_BIT_SET : CMD_RF_WIFIPLL_HOLD_BIT_CLR);

    return rf_pll_ctrl(cmd, RF_WIFIPLL_HOLD_BY_BLE_BIT);
#else
    return 0;
#endif
}

static void reboot_wrapper(void)
{
    bk_reboot();
}

static int uart_read_byte_ex_wrapper(uint8_t id, uint8_t *ch)
{
    return uart_read_byte_ex(id, ch);
}

static int bt_vote_sleep_ctrl_wrapper(uint32_t sleep_state, uint32_t sleep_time)
{
    return bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_BTSP, sleep_state, sleep_time);
}


static void bt_ext_wakeup_ctrl_wrapper(uint8_t enable)
{
    sys_drv_bt_sleep_exit_ctrl(enable);
}

static void send_uart2(uint8_t uart_id, const char *fmt, ...)
{
    char string[CONFIG_PRINTF_BUF_SIZE];
    UINT32 i = 0;
    va_list args;
    va_start(args, fmt);
    //exception_mode_printf(fmt, args);

    vsnprintf(string, sizeof(string) - 1, fmt, args);
    string[CONFIG_PRINTF_BUF_SIZE - 1] = 0;

    while (*(string + i))
    {
        uart_write_byte(uart_id, *(string + i));
        i++;
    }

    va_end(args);
}

static void dump_bytes(uint8_t uart_id, uint8_t *buffer, uint16_t length)
{
    char hex_stream[49U] = {0};
    uint32_t i;
    uint16_t offset;
    uint8_t c;

    offset = 0U;

    for (i = 0U; i < length; i ++)
    {
        c = buffer[i];

        sprintf((char *)&hex_stream[offset], "%02X ", c);
        //        hex_stream[offset] = c;
        //        hex_stream[offset + 1] = c + 1;
        //        hex_stream[offset + 2] = ' ';

        offset += 3U;

        if (offset == 48)
        {
            hex_stream[offset] = '\0';
            send_uart2(uart_id, hex_stream);
            os_memset(hex_stream, 0, 49U);
            offset = 0;
            continue;
        }
    }

    hex_stream[offset] = '\0';

    send_uart2(uart_id, hex_stream);
}

static void btsnoop_wrapper(uint8_t uart_id, uint8_t pkt_type, uint8_t is_rxed, uint8_t *pkt, uint16_t pkt_len, uint8_t method)
{
    switch(method)
    {
    case 0:
    default:
        if (is_rxed)
        {
            send_uart2(uart_id, "\r\n<- %02x ", pkt_type);
            dump_bytes(uart_id, pkt, pkt_len);
            send_uart2(uart_id, "\r\n");
        }
        else
        {
            send_uart2(uart_id, "\r\n-> %02x ", pkt_type);
            dump_bytes(uart_id, pkt, pkt_len);
            send_uart2(uart_id, "\r\n");
        }
        break;

    case 1:
        if(is_rxed)
        {
            pkt_type |= 0b10000000;
        }

        uart_write_byte(uart_id, pkt_type);

        for (uint32_t i = 0; i < pkt_len; ++i)
        {
            uart_write_byte(uart_id, pkt[i]);
        }
        break;
    }
}

static uint32_t get_chipid_mask(void)
{
    return PM_CHIP_ID_MASK;
}

static size_t get_sys_debug_config_addr(uint32_t index)
{
    (void)index;
    return SYS_SYS_DEBUG_CONFIG0_ADDR;
}

static uint32_t get_chipid(uint32_t ver)
{
    switch (ver)
    {

        default:
        case 3:
            return PM_CHIP_ID_MPW_V3;
            break;
    }

    return 0;
}


static uint32_t get_current_chipid(void)
{
#if CONFIG_BK7239N_MP
    return 1;
#else
    return 0;//mpw
#endif
    //return aon_pmu_hal_get_chipid();
}

static uint32_t get_test_rfconfig(void)
{
    extern uint32_t test_rfconfig;
    return test_rfconfig;
}

static uint8_t get_rf_mode(void)
{
    extern uint32_t rwnx_rfconfig;

    if ((rwnx_rfconfig & BLUETOOTH_RF_PLL_MASK) == BLUETOOTH_RF_PLL_WIFI)
    {
        return BT_RF_MODE_WIFI;
    }
    else if ((rwnx_rfconfig & BLUETOOTH_RF_MODE_MASK) == BLUETOOTH_RF_MODE_POLAR)
    {
        return BT_RF_MODE_POLAR;
    }

    return 0;
}

static int32_t init_mutex(void **mutex)
{
    return rtos_init_mutex((beken_mutex_t *)mutex);
}

static int32_t lock_mutex(void **mutex)
{
    return rtos_lock_mutex((beken_mutex_t *)mutex);
}

static int32_t unlock_mutex(void **mutex)
{
    return rtos_unlock_mutex((beken_mutex_t *)mutex);
}

static int32_t deinit_mutex(void **mutex)
{
    return rtos_deinit_mutex((beken_mutex_t *)mutex);
}

static int32_t init_semaphore(void **semaphore, int32_t max_count)
{
    return rtos_init_semaphore((beken_semaphore_t *)semaphore, 1);
}

static int32_t set_semaphore(void **semaphore)
{
    return rtos_set_semaphore((beken_semaphore_t *)semaphore);
}

static int32_t get_semaphore(void **semaphore, uint32_t timeout_ms)
{
    return rtos_get_semaphore((beken_semaphore_t *)semaphore, timeout_ms);
}

static int32_t deinit_semaphore(void **semaphore)
{
    return rtos_deinit_semaphore((beken_semaphore_t *)semaphore);
}

static int reg_run_cmd(const char *content, int cnt)
{
#if CONFIG_CLI && CONFIG_BKREG
    extern int bkreg_run_command(const char *cmd, int cnt);
    return bkreg_run_command(content, cnt);
#else
    return 0;
#endif
}

static uint32_t manual_calibration_txpwr_table_ready_in_flash(void)
{
    extern UINT32 manual_cal_txpwr_tab_ready_in_flash_or_otp2(void);
    return manual_cal_txpwr_tab_ready_in_flash_or_otp2();
}

static void flush_dcache_wrapper(void)
{
#if CONFIG_CACHE_ENABLE
    flush_all_dcache();
#endif
}

static void ble_cal_get_txpwr_info_wrapper(float *targetpwr, uint8_t *txpwr_table_size)
{
    extern float manual_cal_get_targetpwr(UINT32 rate, UINT32 rfband, UINT32 bandwidth);
    extern uint8_t get_ble_txpwr_table_size(void);
    *targetpwr = manual_cal_get_targetpwr(EVM_DEFUALT_BLE_RATE, PHY_BAND_2G4, PHY_CHNL_BW_20);
    *txpwr_table_size = get_ble_txpwr_table_size();
}

static void ble_enter_dut(void)
{
    extern void ble_cal_enter_dut(void);
    ble_cal_enter_dut();
}

static void ble_exit_dut(void)
{
    extern void ble_cal_exit_dut(void);
    ble_cal_exit_dut();
}

static void bt_lp_rtc_set(uint32_t tick, void *callback)
{
    alarm_info_t low_valtage_alarm = {
                                    PM_BT_LP_RTC_ALARM_NAME,
                                    tick,
                                    1,
                                    callback,
                                    NULL
                                    };
    //force unregister previous if doesn't finish.
    bk_alarm_unregister(AON_RTC_ID_1, low_valtage_alarm.name);
    bk_alarm_register(AON_RTC_ID_1, &low_valtage_alarm);
    bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_RTC, NULL);
}

static void bt_lp_rtc_clear(void)
{
    bk_alarm_unregister(AON_RTC_ID_1, (uint8_t *)PM_BT_LP_RTC_ALARM_NAME);
}

static void bt_delay_us(uint32_t us)
{
    extern void delay_us(UINT32 us);
    delay_us(us);
}

static uint32_t get_ana_timersel()
{
    return sys_hal_get_ana_reg12_timersel();
}

static uint32_t get_finecnt_samp(void)
{
    return aon_pmu_ll_get_reg72_value();
}

static uint32_t get_clkcnt_samp(void)
{
    return aon_pmu_ll_get_reg73_value();
}

static uint32_t get_isocnt_samp(void)
{
    return aon_pmu_ll_get_reg74_value();
}

void bluetooth_lowvol_enter_callback(void)
{
    bluetooth_lowvol_enter_cb_internal();
}

void bluetooth_lowvol_exit_callback(void)
{
    bluetooth_lowvol_exit_cb_internal();
}

//warning: bt_osi_funcs must be data section, otherwise a2dp_source_pcm and a2dp_source_decode will trig watchdog !!!!!!!!
static struct bt_osi_funcs_t bt_osi_funcs =
{
    ._version = BT_OSI_VERSION,
    .size = sizeof(struct bt_osi_funcs_t),
    ._coex_init = coex_init_wrapper,
    ._bluetooth_int_isr_register = bluetooth_int_isr_register_wrapper,
    ._init_queue = init_queue_wrapper,
    ._deinit_queue = deinit_queue_wrapper,
    ._pop_from_queue = pop_from_queue_wrapper,
    ._push_to_queue = push_to_queue_wrapper,
    ._create_thread = create_thread_wrapper,
    ._delete_thread = delete_thread_wrapper,
    ._thread_join = thread_join,
    ._get_thread_stack_size = get_thread_stack_size,

    ._controller_mem_init = controller_mem_init_wrapper,
    ._bluetooth_deep_sleep_register = bluetooth_deep_sleep_register_wrapper,
    ._bluetooth_extern32k_register = bluetooth_extern32k_register_wrapper,
    ._bluetooth_extern32k_unregister = bluetooth_extern32k_unregister_wrapper,
    ._clk_32k_customer_config_get = clk_32k_customer_config_get_wrapper,
    ._lpo_src_set = lpo_src_set_wrapper,
    ._lpo_src_get = lpo_src_get_wrapper,
    ._bluetooth_power_ctrl = bluetooth_power_ctrl_wrapper,
    ._phy_power_ctrl = phy_power_ctrl_wrapper,
    ._bt_mac_clock_ctrl = bt_mac_clock_ctrl_wrapper,
    ._bt_phy_clock_ctrl = bt_phy_clock_ctrl_wrapper,
    ._btdm_interrupt_ctrl = btdm_interrupt_ctrl_wrapper,
    ._ble_interrupt_ctrl = ble_interrupt_ctrl_wrapper,
    ._bt_interrupt_ctrl = bt_interrupt_ctrl_wrapper,
    ._ate_is_enabled = ate_is_enabled_wrapper,
    ._cp_test_is_enabled = cp_test_is_enabled_wrapper,
    ._get_bluetooth_mac = get_bluetooth_mac_wrapper,
    ._uart_write_byte = uart_write_byte_wrapper,
    ._register_ble_dump_hook = register_ble_dump_hook_wrapper,
    ._get_ble_pwr_idx = get_ble_pwr_idx_wrapper,
    ._ble_cal_set_txpwr = ble_cal_set_txpwr_wrapper,
    ._ble_cal_recover_txpwr = ble_cal_recover_txpwr_wrapper,
    ._ble_cal_enter_txpwr = ble_cal_enter_txpwr_wrapper,
    ._gpio_dev_unmap = gpio_dev_unmap_wrapper,
    ._set_printf_enable = set_printf_enable_wrapper,
    ._evm_stop_bypass_mac = evm_stop_bypass_mac_wrapper,
    ._rs_deinit = rs_deinit_wrapper,
    ._phy_enable_rx_switch = NULL,
    ._uart_enable_rx_interrupt = uart_enable_rx_interrupt_wrapper,
    ._uart_take_rx_isr = uart_take_rx_isr_wrapper,
    ._ble_vote_rf_ctrl = ble_vote_rf_ctrl_wrapper,
    ._ble_ate_vote_rf_ctrl = ble_ate_vote_rf_ctrl_wrapper,
    ._delay_milliseconds = delay_milliseconds_wrapper,
    ._disable_int = disable_int_wrapper,
    ._enable_int = enable_int_wrapper,
    ._gpio_disable_pull = gpio_disable_pull_wrapper,
    ._gpio_enable_output = gpio_enable_output_wrapper,
    ._gpio_set_output_high = gpio_set_output_high_wrapper,
    ._gpio_set_output_low = gpio_set_output_low_wrapper,
    ._gpio_pull_down = gpio_pull_down_wrapper,
    ._gpio_pull_up = gpio_pull_up_wrapper,
    ._get_printf_port = get_printf_port_wrapper,
    ._uart_enable = uart_enable_wrapper,
    ._enable_debug_gpio = enable_debug_gpio_wrapper,
    ._log = bk_ble_printf_ext,
    ._get_time = get_time_wrapper,
    ._malloc = malloc_wrapper,
    ._free = free_wrapper,

    ._manual_cal_save_ble_txpwr = manual_cal_save_ble_txpwr_wrapper,

    ._is_timer_init = is_timer_init_wrapper,
    ._init_timer = init_timer_wrapper,
    ._init_timer_ext = init_timer_ext_wrapper,
    ._start_timer = start_timer_wrapper,
    ._is_timer_running = is_timer_running_wrapper,
    ._stop_timer = stop_timer_wrapper,
    ._deinit_timer = deinit_timer_wrapper,
    ._timer_change_period = timer_change_period,

    ._bt_rf_pll_ctrl = bt_rf_pll_ctrl_wrapper,
    ._reboot = reboot_wrapper,
    ._uart_read_byte_ex = uart_read_byte_ex_wrapper,

    ._bt_vote_sleep_ctrl = bt_vote_sleep_ctrl_wrapper,
    ._coexist_check_large_signal = NULL,
    ._bt_ext_wakeup_ctrl = bt_ext_wakeup_ctrl_wrapper,
    ._btsnoop = btsnoop_wrapper,

    ._get_sys_debug_config_addr = get_sys_debug_config_addr,
    ._get_chipid_mask = get_chipid_mask,
    ._get_chipid = get_chipid,
    ._get_current_chipid = get_current_chipid,
    ._get_test_rfconfig = get_test_rfconfig,

    ._init_mutex = init_mutex,
    ._lock_mutex = lock_mutex,
    ._unlock_mutex = unlock_mutex,
    ._deinit_mutex = deinit_mutex,
    ._init_semaphore = init_semaphore,
    ._set_semaphore = set_semaphore,
    ._get_semaphore = get_semaphore,
    ._deinit_semaphore = deinit_semaphore,

    ._get_rf_mode = get_rf_mode,
    ._bkreg_run_command = reg_run_cmd,
    ._manual_cal_txpwr_tab_ready_in_flash = manual_calibration_txpwr_table_ready_in_flash,
    ._flush_dcache = flush_dcache_wrapper,
    ._ble_cal_get_txpwr_info = ble_cal_get_txpwr_info_wrapper,
    ._ble_enter_dut = ble_enter_dut,
    ._ble_exit_dut = ble_exit_dut,
    ._get_dut_port = get_dut_port_wrapper,
    ._bt_lp_rtc_set         = bt_lp_rtc_set,
    ._bt_lp_rtc_clear       = bt_lp_rtc_clear,
    ._get_ana_timersel      = get_ana_timersel,
    ._get_finecnt_samp      = get_finecnt_samp,
    ._get_clkcnt_samp       = get_clkcnt_samp,
    ._get_isocnt_samp       = get_isocnt_samp,
    ._bt_delay_us           = bt_delay_us,
};

int bk_bt_os_adapter_init(void)
{
    ble_err_t ret = BK_OK;

    if (bt_os_adapter_init((void *)&bt_osi_funcs) != 0)
    {
        return BK_FAIL;
    }

    return ret;
}




