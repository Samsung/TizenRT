#include <common/bk_include.h>
#include "driver/hal/hal_adc_types.h"
#include "driver/flash_partition.h"
#include "driver/flash.h"
#include "drv_model_pub.h"
#include "bk_wifi_adapter.h"
#include "bk_phy_adapter.h"

typedef struct {
    uint32_t (*_sys_drv_modem_bus_clk_ctrl)(bool clk_en);
    uint32_t (*_sys_drv_modem_clk_ctrl)(bool clk_en);
    void (*_phy_exit_dsss_only)(void);
    void (*_phy_enter_dsss_only)(void);
    uint32_t (*_rtos_disable_int)(void);
    void (*_rtos_enable_int)(uint32_t int_level);
    void (*_rwnx_cal_mac_sleep_rc_recover)(void);
    void (*_sys_drv_module_power_ctrl)(unsigned int module, uint32_t power_state);
    void (*_sys_drv_set_ana_reg11_apfms)(uint32_t value);
    void (*_sys_drv_set_ana_reg12_dpfms)(uint32_t value);
    bk_err_t (*_bk_pm_module_vote_power_ctrl)(unsigned int module, uint32_t power_state);
} rf_control_funcs_t;
extern const rf_control_funcs_t g_rf_control_funcs;

typedef struct {
    uint32_t _pm_power_module_state_off;
    uint32_t _pm_power_module_state_on;
    uint32_t _pm_power_module_name_phy;
    uint32_t _pm_power_module_name_rf;
} rf_variable_t;
extern const rf_variable_t g_rf_variable;

void bk_rf_adapter_init(void);
extern void rf_adapter_init(const void * rf_funcs, const void *rf_vars);
extern void rf_cntrl_init(void);
