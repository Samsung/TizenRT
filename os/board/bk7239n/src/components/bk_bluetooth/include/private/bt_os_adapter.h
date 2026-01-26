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

#pragma once
#include<stdint.h>
#include <stdbool.h>
#include <stddef.h>

//copy from bk_cal_ex.h
#define BLUETOOTH_RF_PLL_WIFI 0x1
#define BLUETOOTH_RF_PLL_BT   0x2
#define BLUETOOTH_RF_PLL_MASK 0xF

#define BLUETOOTH_RF_MODE_IQ      0x00 //as default
#define BLUETOOTH_RF_MODE_POLAR   0x10
#define BLUETOOTH_RF_MODE_MASK    0xF0

#define BLUETOOTH_RF_ROLE_WIFI    0x100
#define BLUETOOTH_RF_ROLE_BLE     0x200
#define BLUETOOTH_RF_ROLE_BT      0x400
#define BLUETOOTH_RF_ROLE_MASK    0xF00

#define BLUETOOTH_CLK_32K           32000
#define BLUETOOTH_CLK_32768         32768

enum
{
    BLUETOOTH_INT_SRC_BTDM = 0x00,
    BLUETOOTH_INT_SRC_BLE  = 0x01,
    BLUETOOTH_INT_SRC_BT   = 0x02,
};

enum
{
    BT_LPO_SRC_DIVD = 0,//32K from 26m
    BT_LPO_SRC_X32K,    //extern 32k
    BT_LPO_SRC_ROSC,    //32K from ROSC
    BT_LPO_SRC_DEFAULT  //32K from ROSC
};

enum
{
    BT_RF_MODE_WIFI = 1,
    BT_RF_MODE_POLAR,
};

typedef enum
{
    BLUETOOTH_TASK_MAIN = 1,
    BLUETOOTH_TASK_HCI_REPORT,
    BLUETOOTH_TASK_DM_CTX,
    BLUETOOTH_TASK_DM_WT,
    BLUETOOTH_TASK_DM_RD,
} bluetooth_task_id_t;


/* adapter function */
struct bt_osi_funcs_t
{
    uint32_t _version;
    uint32_t size;

    int (*_init_queue)(void **queue, const char *name, uint32_t message_size, uint32_t number_of_messages );
    int (*_deinit_queue)( void **queue );
    int (*_pop_from_queue)( void **queue, void *message, uint32_t timeout_ms );
    int (*_push_to_queue)( void **queue, void *message, uint32_t timeout_ms );
    int (*_create_thread)( void **thread, uint8_t priority, const char *name,
                           void *function, uint32_t stack_size, void *arg );
    int (*_delete_thread)( void **thread );
    int (*_thread_join)(void **thread);
    int (*_get_thread_stack_size)(uint16_t task_id);

    int32_t (*_init_mutex)(void **mutex);
    int32_t (*_lock_mutex)(void **mutex);
    int32_t (*_unlock_mutex)(void **mutex);
    int32_t (*_deinit_mutex)(void **mutex);

    int32_t (*_init_semaphore)(void **semaphore, int32_t max_count);
    int32_t (*_set_semaphore)(void **semaphore);
    int32_t (*_get_semaphore)(void **semaphore, uint32_t timeout_ms);
    int32_t (*_deinit_semaphore)(void **semaphore);

    int32_t (*_init_timer)( void **timer, uint32_t time_ms, void *function, void *arg);
    int32_t (*_init_timer_ext)( void **timer, uint32_t time_ms, void *function, void *arg, bool oneshot);
    int32_t (*_deinit_timer)( void *timer);
    int32_t (*_stop_timer)( void *timer);

    int32_t (*_timer_change_period)( void *timer, uint32_t time_ms);
    bool (*_is_timer_init)(void *timer );
    int32_t (*_start_timer)(void *timer);
    bool (*_is_timer_running)( void *timer);

    void (*_log)(int level, char *tag, const char *fmt, ...);
    uint32_t (*_get_time)(void);
    int (*_delay_milliseconds)( uint32_t num_ms );

    void *(*_malloc)(unsigned int size);
    void (*_free)(void *p);


    int (*_coex_init)(void);
    int (*_bluetooth_int_isr_register)(uint8_t type, void *isr, void *arg);

    void (*_controller_mem_init)(void);
    int (*_bluetooth_deep_sleep_register)(void *enter_config_cb);
    int (*_bluetooth_extern32k_register)(void *switch_cb);
    int (*_bluetooth_extern32k_unregister)(void);
    uint8_t (*_clk_32k_customer_config_get)(void);
    int (*_lpo_src_set)(uint8_t lpo_src);
    uint8_t (*_lpo_src_get)(void);
    int (*_bluetooth_power_ctrl)(uint8_t power_state);
    int (*_phy_power_ctrl)(uint8_t power_state);
    int (*_bt_mac_clock_ctrl)(uint8_t clock_state);
    int (*_bt_phy_clock_ctrl)(uint8_t clock_state);
    void (*_btdm_interrupt_ctrl)(bool en);
    void (*_ble_interrupt_ctrl)(bool en);
    void (*_bt_interrupt_ctrl)(bool en);
    bool (*_ate_is_enabled)(void);
    bool (*_cp_test_is_enabled)(void);
    int (*_get_bluetooth_mac)(uint8_t *mac);
    int (*_uart_write_byte)(uint8_t id, uint8_t data);
    void (*_register_ble_dump_hook)(void  *ble_func);
    uint8_t (*_get_ble_pwr_idx)(uint8_t channel);
    void (*_ble_cal_set_txpwr)(uint8_t idx, uint8_t channel);
    void (*_ble_cal_recover_txpwr)(void);
    void (*_ble_cal_enter_txpwr)(void);
    int (*_gpio_dev_unmap)(uint32_t gpio_id);
    int (*_gpio_dev_map)(uint32_t gpio_id, uint32_t func);
    void (*_set_printf_enable)(uint8_t enable);
    void (*_evm_stop_bypass_mac)(void);
    void (*_rs_deinit)(void);
    void (*_phy_enable_rx_switch)(void);
    int (*_uart_enable_rx_interrupt)(uint8_t id);
    int (*_uart_take_rx_isr)(uint8_t id, void *isr, void *param);
    void (*_ble_vote_rf_ctrl)(uint8_t cmd);
    void (*_ble_ate_vote_rf_ctrl)(uint8_t cmd);

    uint32_t (*_disable_int)(void);
    void (*_enable_int)(uint32_t int_level);
    int (*_gpio_disable_pull)(uint32_t gpio_id );
    int (*_gpio_enable_output)(uint32_t gpio_id);
    int (*_gpio_set_output_high)(uint32_t gpio_id);
    int (*_gpio_set_output_low)(uint32_t gpio_id);
    int (*_gpio_pull_down)(uint32_t gpio_id);
    int (*_gpio_pull_up)(uint32_t gpio_id);
    int (*_get_printf_port)(void);
    void (*_uart_enable)(uint8_t uart_id, uint8_t enable, uint32_t band);
    void (*_enable_debug_gpio)(void);


    void (*_manual_cal_save_ble_txpwr)(uint32_t channel, uint32_t pwr_gain);


    uint32_t (*_bt_rf_pll_ctrl)(uint32_t set);
    void (*_reboot)(void);
    int (*_uart_read_byte_ex)(uint8_t id, uint8_t *ch);
    int (*_bt_vote_sleep_ctrl)(uint32_t sleep_state, uint32_t sleep_time);
    void (*_coexist_check_large_signal)(uint8_t switch2bt);
    void (*_bt_ext_wakeup_ctrl)(uint8_t enable);
    void (*_btsnoop)(uint8_t uart_id, uint8_t pkt_type, uint8_t is_rxed, uint8_t *pkt, uint16_t pkt_len, uint8_t method);

    size_t (*_get_sys_debug_config_addr)(uint32_t index);
    uint32_t (*_get_chipid_mask)(void);
    uint32_t (*_get_chipid)(uint32_t ver);
    uint32_t (*_get_current_chipid)(void);
    uint32_t (*_get_test_rfconfig)(void);
    //uint8_t (*_get_tx_pwr_idx)(void);
    uint8_t (*_get_rf_mode)(void);

    int (*_bkreg_run_command)(const char *content, int cnt);

    uint32_t (*_manual_cal_txpwr_tab_ready_in_flash)(void);
    uint32_t (*_manual_cal_is_in_cali_mode)(void);
    double (*_bk_driver_get_rc32k_freq)(void);
    void (*_flush_dcache)(void);

    void (*_ble_enter_dut)(void);
    void (*_ble_exit_dut)(void);
    uint8_t (*_get_bluetooth_power_level)(void);
    void (*_ble_cal_get_txpwr_info)(float *targetpwr, uint8_t *txpwr_table_size);
    int (*_get_dut_port)(void);
	    // Add for btsp sleep
    void (*_bt_lp_rtc_set)(uint32_t tick, void *callback);
    void (*_bt_lp_rtc_clear)(void);
    uint32_t (*_get_ana_timersel)(void); // Used to circumvent the hardware issue, can be deleted in the next version
    uint32_t (*_get_finecnt_samp)(void);
    uint32_t (*_get_clkcnt_samp)(void);
    uint32_t (*_get_isocnt_samp)(void);
    void (*_bt_delay_us)(uint32_t us);
};

int bt_os_adapter_init(void *osi_funcs);

