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

#define __must_check

#include <bk_wifi_adapter.h>
#include <bk_phy_internal.h>
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "lwip/inet.h"
//#include <../../lwip_intf_v2_1/lwip-2.1.2/port/net.h>
#include "bk_wifi.h"
//#include "bk_net.h"
#include "bk_misc.h"
#include <sys_ll.h>
#include "sys_driver.h"
#include "gpio_driver.h"
#include "bk_ps.h"
#include <modules/pm.h>
#include "bk_mcu_ps.h"
#include "bk_feature.h"
#include "bk_general_dma.h"
#include "bk_ps_time.h"
#include <lwip/def.h>
#include "bk_rf_internal.h"
#include <components/ate.h>
#include <driver/aon_rtc.h>
#include <os/str.h>
#include <os/mem.h>
#include <os/os.h>
#include "lwip/sockets.h"
#include "cache.h"
#include "arch_interrupt.h"
#include "stack_base.h"
//#include "command_line.h"
#include "bk_phy_adapter.h"
#include "reg_domain.h"
#include "bk_rw.h"
#include <driver/otp.h>
#include "soc/soc.h"

#if (CONFIG_CKMN)
#include <driver/rosc_32k.h>
#endif

#if CONFIG_CACHE_ENABLE
#include "cache.h"
#endif

#define TAG "wifi_adapter"
#include "bk_cli.h"

void *
bk_pbuf_alloc_wrapper(int layer, uint16_t length, int type)
{
	pbuf_layer layer_tmp = 0;
	pbuf_type type_tmp = 0;

	switch (layer) {
		case BK_PBUF_RAW_TX:
			layer_tmp = PBUF_RAW_TX;
			break;
		case BK_PBUF_RAW:
			layer_tmp = PBUF_RAW;
			break;
		default: return NULL;
	}

	switch (type) {
		case BK_PBUF_RAM:
			type_tmp = PBUF_RAM;
			break;
		case BK_PBUF_RAM_RX:
#if MEM_TRX_DYNAMIC_EN
			type_tmp = PBUF_RAM_RX;
#else
			type_tmp = PBUF_POOL;
#endif
			break;
		case BK_PBUF_POOL:
			type_tmp = PBUF_POOL;
			break;
		default: return NULL;
	}
	return pbuf_alloc(layer_tmp, length, type_tmp);
}

void bk_pbuf_free_wrapper(void *p)
{
	pbuf_free((struct pbuf *)p);
}

void bk_pbuf_ref_wrapper(void *p)
{
	pbuf_ref((struct pbuf*)p);
}

void bk_pbuf_header_wrapper(void *p, int16_t len)
{
	pbuf_header((struct pbuf *)p, len);
}

void *bk_pbuf_coalesce_wrapper(void *p)
{
	return pbuf_coalesce(p, PBUF_RAW);
}

void bk_pbuf_cat_wrapper(void *p, void *q)
{
	pbuf_cat(p, q);
}

int bk_get_rx_pbuf_type_wrapper(void)
{
#if MEM_TRX_DYNAMIC_EN
	return BK_PBUF_RAM_RX;
#else
	return BK_PBUF_POOL;
#endif
}

int bk_get_pbuf_pool_size_wrapper(void)
{
#if MEM_TRX_DYNAMIC_EN
	return 0;
#else
	return PBUF_POOL_BUFSIZE;
#endif
}

void *bk_get_netif_hostname_wrapper(void *netif)
{
	const char*  hostname = ((struct netif *)netif)->name;
	return (void *)hostname;
}

int bk_save_net_info_wrapper(int item, u8 *ptr0, u8 *ptr1, u8 *ptr2)
{
	return save_net_info(item,ptr0,ptr1,ptr2);
}

int bk_get_net_info_wrapper(int item, u8 *ptr0, u8 *ptr1, u8 *ptr2)
{
	return get_net_info(item,ptr0,ptr1,ptr2);
}

void *bk_inet_ntoa_wrapper(void *addr)
{
	return (UINT8 *)inet_ntoa(addr);
}

uint32_t bk_lwip_ntohl_wrapper(uint32_t val)
{
	return lwip_ntohl(val);
}

uint16_t bk_lwip_htons_wrapper(uint16_t val)
{
	return lwip_htons(val);
}

void bk_sta_start(void)
{
	sta_ip_start();
}

void bk_sta_ip_down_wrapper(void)
{
	sta_ip_down();
}

void bk_uap_ip_start_wrapper(void)
{
	uap_ip_start();
}

void bk_uap_ip_down_wrapper(void)
{
	uap_ip_down();
}

int bk_net_wlan_add_netif_wrapper(void *mac)
{
	return net_wlan_add_netif(mac);
}

int bk_net_wlan_remove_netif_wrapper(void *mac)
{
	return net_wlan_remove_netif(mac);
}

//extern uint8_t* dhcp_lookup_mac(uint8_t *chaddr);
uint32_t bk_lookup_ipaddr_wrapper(void *addr)
{
	char *ipstr;

	ipstr = NULL; //(char *)dhcp_lookup_mac(addr);
	if (ipstr) {
		ip_addr_t ipaddr,*ptr;
		ptr = &ipaddr;
		ipaddr_aton(ipstr, ptr);
		return ip_addr_get_ip4_u32(ptr);
	}
	return 0;
}

void bk_net_begin_send_arp_reply_wrapper(int is_send_arp, int is_allow_send_req)
{
	net_begin_send_arp_reply(is_send_arp, is_allow_send_req);
}

void bk_set_sta_status_wrapper(void *info)
{
	mhdr_set_station_status(*(wifi_linkstate_reason_t *)info);
}
static void delay_wrapper(INT32 num)
{
    delay(num);
}

static void __IRAM2 delay_us_wrapper(UINT32 us)
{
	delay_us(us);
}

static uint32_t sys_drv_modem_bus_clk_ctrl_wrapper(bool clk_en)
{
	return sys_drv_modem_bus_clk_ctrl(clk_en);
}

static uint32_t sys_drv_modem_clk_ctrl_wrapper(bool clk_en)
{
	return sys_drv_modem_clk_ctrl(clk_en);
}

static int32 sys_drv_int_enable_wrapper(uint32 param)
{
    return sys_drv_int_enable(param);
}

static int32 sys_drv_int_disable_wrapper(uint32 param)
{
    return sys_drv_int_disable(param);
}

static int32 sys_drv_int_group2_enable_wrapper(uint32 param)
{
    return sys_drv_int_group2_enable(param);
}

static int32 sys_drv_int_group2_disable_wrapper(uint32 param)
{
    return sys_drv_int_group2_disable(param);
}

static int32 sys_drv_module_power_state_get_wrapper(uint32_t module)
{
    return sys_drv_module_power_state_get(module);
}

static void rwnx_cal_set_channel_wrapper(UINT32 freq)
{
	#if (CONFIG_SOC_BK7236XX || CONFIG_SOC_BK7239XX)
	rwnx_cal_set_channel(freq);
	#else
	return;
	#endif
}

static void bk7011_cal_dpd_wrapper(uint32_t freq, uint32_t connect_mode)
{
    bk7011_cal_dpd(freq, connect_mode);
}

static void bk7011_update_by_rx_wrapper(int8_t rssi, int8_t freq_offset)
{
	#if (CONFIG_SOC_BK7236XX)
	bk7011_update_by_rx(rssi, freq_offset);
	#else
	return;
	#endif
}

static void sys_ll_set_cpu_power_sleep_wakeup_pwd_ofdm_wrapper(uint32_t v)
{
    #if (CONFIG_SOC_BK7236XX || CONFIG_SOC_BK7239XX)
	sys_ll_set_cpu_power_sleep_wakeup_pwd_ofdm(v);
	#endif
}

static uint32_t __IRAM2 sys_ll_get_cpu_power_sleep_wakeup_pwd_ofdm_wrapper(void)
{
	#if (CONFIG_SOC_BK7236XX || CONFIG_SOC_BK7239XX)
	return sys_ll_get_cpu_power_sleep_wakeup_pwd_ofdm();
	#else
	return 0;
	#endif
}

static uint32_t sys_ll_get_cpu_device_clk_enable_mac_cken_wrapper(void)
{
	return sys_ll_get_cpu_device_clk_enable_mac_cken();
}

static uint32_t sys_ll_get_cpu_device_clk_enable_phy_cken_wrapper(void)
{
	return sys_ll_get_cpu_device_clk_enable_phy_cken();
}
static void  power_save_delay_sleep_check_wrapper(void)
{
	//power_save_delay_sleep_check();
}

static void power_save_wake_mac_rf_if_in_sleep_wrapper(void)
{
	//power_save_wake_mac_rf_if_in_sleep();
}

static void power_save_wake_mac_rf_end_clr_flag_wrapper(void)
{
	//power_save_wake_mac_rf_end_clr_flag();
}

static UINT8 power_save_if_ps_rf_dtim_enabled_wrapper(void)
{
    //return power_save_if_ps_rf_dtim_enabled();
    return 0;
}

static UINT16 power_save_forbid_trace_wrapper(UINT16 forbid)
{
    //return power_save_forbid_trace((PS_FORBID_STATUS)forbid);
    return 0;
}

static bk_err_t __IRAM2 bk_pm_module_vote_sleep_ctrl_wrapper(uint32_t module, uint32_t sleep_state, uint32_t sleep_time)
{
    return bk_pm_module_vote_sleep_ctrl((pm_sleep_module_name_e)module, sleep_state, sleep_time);
}

static uint32_t __IRAM2 bk_pm_lpo_src_get_wrapper(void)
{
	return (uint32_t)bk_pm_lpo_src_get();
}

static int32 bk_pm_module_power_state_get_wrapper(unsigned int module)
{
    return bk_pm_module_power_state_get((pm_power_module_name_e)module);
}

static bk_err_t __IRAM2 bk_pm_module_vote_power_ctrl_wrapper(unsigned int             module, uint32_t power_state)
{
    return bk_pm_module_vote_power_ctrl((pm_power_module_name_e)module, (pm_power_module_state_e)power_state);
}

static bk_err_t bk_pm_module_vote_cpu_freq_wrapper(uint32_t module, uint32_t cpu_freq)
{
    //return bk_pm_module_vote_cpu_freq((pm_dev_id_e)module, (pm_cpu_freq_e)cpu_freq);
    return BK_OK;
}

static bool bk_pm_phy_reinit_flag_get_wrapper(void)
{
    return bk_pm_phy_reinit_flag_get();
}

static void bk_pm_phy_reinit_flag_clear_wrapper(void)
{
	bk_pm_phy_reinit_flag_clear();
}

bk_err_t bk_pm_sleep_register_wrapper(void *config_cb)
{
    pm_cb_conf_t enter_config_wifi = {NULL, NULL};

    enter_config_wifi.cb = config_cb;

#if !CONFIG_PM_SUPER_DEEP_SLEEP
    return bk_pm_sleep_register_cb(PM_MODE_DEEP_SLEEP,PM_DEV_ID_MAC,&enter_config_wifi, NULL);
#else
    bk_pm_sleep_register_cb(PM_MODE_DEEP_SLEEP,PM_DEV_ID_MAC,&enter_config_wifi, NULL);
    bk_pm_sleep_register_cb(PM_MODE_SUPER_DEEP_SLEEP,PM_DEV_ID_MAC,&enter_config_wifi, NULL);
    return BK_OK;
#endif
}


bk_err_t bk_pm_low_voltage_register_wrapper(void *enter_config_cb,void *exit_config_cb)
{
    pm_cb_conf_t wifi_enter_config = {NULL, NULL};
    pm_cb_conf_t wifi_exit_config = {NULL, NULL};

    wifi_enter_config.cb = enter_config_cb;
    wifi_exit_config.cb = exit_config_cb;

	return bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE,PM_DEV_ID_MAC, &wifi_enter_config, &wifi_exit_config);
}

static void __IRAM2 bk_pm_wifi_rtc_set_wrapper(uint32_t tick, void *callback)
{
#if CONFIG_ANA_RTC
    bk_pm_wifi_rtc_set(tick, callback);
#endif
}

static void bk_pm_wifi_rtc_clear_wrapper(void)
{
#if CONFIG_ANA_RTC
    bk_pm_wifi_rtc_clear();
#endif
}

static void __IRAM2 wifi_vote_rf_ctrl_wrapper(uint8_t cmd)
{
    rf_module_vote_ctrl(cmd,RF_BY_WIFI_BIT);
}

static void wifi_phy_clk_open_wrapper(uint8_t is_wifi)
{
    if(is_wifi)
    {
        phy_clk_open_handler(RF_BY_WIFI_BIT);
    }
    else
    {
        phy_clk_open_handler(RF_BY_BLE_BIT);
    }

}

static void wifi_phy_clk_close_wrapper(uint8_t is_wifi)
{
    if(is_wifi)
    {
        phy_clk_close_handler(RF_BY_WIFI_BIT);
    }
    else
    {
        phy_clk_close_handler(RF_BY_BLE_BIT);
    }
}

static void wifi_mac_clk_open_wrapper(void)
{
    bk_pm_clock_ctrl(PM_CLK_ID_MAC, PM_CLK_CTRL_PWR_UP);
}

static void wifi_mac_clk_close_wrapper(void)
{
	bk_pm_clock_ctrl(PM_CLK_ID_MAC, PM_CLK_CTRL_PWR_DOWN);
}

static void wifi_mac_phy_power_on_wrapper(void)
{
	//enable mac and phy power
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_WIFIP_MAC, PM_POWER_MODULE_STATE_ON);
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_PHY, PM_POWER_MODULE_STATE_ON);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_PHY_WIFI,PM_POWER_MODULE_STATE_ON);

	//enable mac and phy clock
	bk_pm_clock_ctrl(PM_CLK_ID_MAC, PM_CLK_CTRL_PWR_UP);
	bk_pm_clock_ctrl(PM_CLK_ID_PHY, PM_CLK_CTRL_PWR_UP);
}

static bk_err_t bk_rosc_32k_ckest_prog_wrapper(uint32_t param)
{
	#if (CONFIG_CKMN)
	return bk_rosc_32k_ckest_prog(param);
	#else
	return BK_OK;
	#endif
}

static bk_err_t __IRAM2 bk_rosc_32k_get_ppm_wrapper(void)
{
	#if (CONFIG_CKMN)
	return bk_rosc_32k_get_ppm();
	#else
	return BK_OK;
	#endif
}

static double bk_rosc_32k_get_freq_wrapper(void)
{
	#if (CONFIG_CKMN)
	return bk_rosc_32k_get_freq();
	#else
	return BK_OK;
	#endif
}

static INT64 bk_rosc_32k_get_time_diff_wrapper(void)
{
	#if (CONFIG_ROSC_COMPENSATION)
	return bk_rosc_32k_get_time_diff();
	#else
	return BK_OK;
	#endif
}

pm_cb_notify wifi_32k_src_switch_ready_notify_cb = NULL;

static void wifi_32k_src_switch_ready_notify_wrapper(void *cb)
{
    wifi_32k_src_switch_ready_notify_cb = (pm_cb_notify)cb;
}

static void mac_ps_exc32_init_wrapper(void *cb)
{
    pm_cb_extern32k_cfg_t cfg;

    cfg.cb_func = cb;
	cfg.cb_module = PM_32K_MODULE_WIFI;

	////ex32k change register
	pm_extern32k_register_cb(&cfg);
}

static void mac_ps_exc32_cb_notify_wrapper(void)
{
    if (wifi_32k_src_switch_ready_notify_cb)
        wifi_32k_src_switch_ready_notify_cb();
}

static void mac_ps_bcn_callback_wrapper(uint8_t *data, int len)
{
    //mac_ps_bcn_callback(data, len);
}

static UINT8 mac_sleeped_wrapper(void)
{
    //return mac_sleeped();
    return 0;
}

static void rwnx_cal_mac_sleep_rc_clr_wrapper(void)
{
	rwnx_cal_mac_sleep_rc_clr();
}

static void rwnx_cal_mac_sleep_rc_recover_wrapper(void)
{
	rwnx_cal_mac_sleep_rc_recover();
}

static void mac_printf_encode_wrapper(char *txt, size_t maxlen, const u8 *data, size_t len)
{
    char *end = txt + maxlen;
    size_t i;

    for (i = 0; i < len; i++)
    {
        if (txt + 4 >= end)
            break;

        switch (data[i])
        {
        case '\"':
            *txt++ = '\\';
            *txt++ = '\"';
            break;
        case '\\':
            *txt++ = '\\';
            *txt++ = '\\';
            break;
        case '\033':
            *txt++ = '\\';
            *txt++ = 'e';
            break;
        case '\n':
            *txt++ = '\\';
            *txt++ = 'n';
            break;
        case '\r':
            *txt++ = '\\';
            *txt++ = 'r';
            break;
        case '\t':
            *txt++ = '\\';
            *txt++ = 't';
            break;
        default:
            if (data[i] >= 32 && data[i] <= 126)
                *txt++ = data[i];
            else
                txt += snprintf(txt, end - txt, "\\x%02x", data[i]);
            break;
        }
    }

    *txt = '\0';
}

static void dbg_enable_debug_gpio_wrapper(void)
{
    // 2 -> 9
    gpio_dev_unmap(GPIO_2);              // BK7256 EVB    BK7236 EVB      BIT
    gpio_dev_map(GPIO_2, GPIO_DEV_DEBUG0);   //CLK(H6)    CLK(H6)          0
    gpio_dev_unmap(GPIO_3);
    gpio_dev_map(GPIO_3, GPIO_DEV_DEBUG1);   //CMD(H6)    CMD(H6)          1
    gpio_dev_unmap(GPIO_4);
    gpio_dev_map(GPIO_4, GPIO_DEV_DEBUG2);   //D0(H6)     D0(H6)           2
    gpio_dev_unmap(GPIO_5);
    gpio_dev_map(GPIO_5, GPIO_DEV_DEBUG3);   //D1(H6)     D1(H6)           3
    gpio_dev_unmap(GPIO_6);
    gpio_dev_map(GPIO_6, GPIO_DEV_DEBUG4);   //CD(H6)     CD(H6)           4
    gpio_dev_unmap(GPIO_7);
    gpio_dev_map(GPIO_7, GPIO_DEV_DEBUG5);   //P7(H5)     P7(H5)           5
    gpio_dev_unmap(GPIO_8);
    gpio_dev_map(GPIO_8, GPIO_DEV_DEBUG6);   //P8(CON4)   P8(CON4)         6
    gpio_dev_unmap(GPIO_9);
    gpio_dev_map(GPIO_9, GPIO_DEV_DEBUG7);   //P9(CON4)   P9(CON4)         7

    // 14 -> 19
    gpio_dev_unmap(GPIO_14);
    gpio_dev_map(GPIO_14, GPIO_DEV_DEBUG8);  //P14(CON3)  P14(H3)          8
    gpio_dev_unmap(GPIO_15);
    gpio_dev_map(GPIO_15, GPIO_DEV_DEBUG9);  //P15(CON3)  P15(H3)          9
    gpio_dev_unmap(GPIO_16);
    gpio_dev_map(GPIO_16, GPIO_DEV_DEBUG10); //S14(H3)    P16(H3)         10
    gpio_dev_unmap(GPIO_17);
    gpio_dev_map(GPIO_17, GPIO_DEV_DEBUG11); //           P17(H3)         11
    gpio_dev_unmap(GPIO_18);
    gpio_dev_map(GPIO_18, GPIO_DEV_DEBUG12); //           P18(H5)         12
    gpio_dev_unmap(GPIO_19);
    gpio_dev_map(GPIO_19, GPIO_DEV_DEBUG13); //           P19(H5)         13

    // 24 -> 39
    gpio_dev_unmap(GPIO_24);
    gpio_dev_map(GPIO_24, GPIO_DEV_DEBUG14); //           P24(H8)         14
    gpio_dev_unmap(GPIO_25);
    gpio_dev_map(GPIO_25, GPIO_DEV_DEBUG15); //           P25(H8)         15
    gpio_dev_unmap(GPIO_26);
    gpio_dev_map(GPIO_26, GPIO_DEV_DEBUG16); //           P26(H8)         16
    gpio_dev_unmap(GPIO_27);
    gpio_dev_map(GPIO_27, GPIO_DEV_DEBUG17); //           MCLK(H14)       17
    gpio_dev_unmap(GPIO_28);
    gpio_dev_map(GPIO_28, GPIO_DEV_DEBUG18); //           P28/ADC4(H5)    18
    gpio_dev_unmap(GPIO_29);
    gpio_dev_map(GPIO_29, GPIO_DEV_DEBUG19); //           PCLK(H14)       19
    gpio_dev_unmap(GPIO_30);
    gpio_dev_map(GPIO_30, GPIO_DEV_DEBUG20); //           HSYNC(H14)      20
    gpio_dev_unmap(GPIO_31);
    gpio_dev_map(GPIO_31, GPIO_DEV_DEBUG21); //           VSYNC(H14)      21
    gpio_dev_unmap(GPIO_32);
    gpio_dev_map(GPIO_32, GPIO_DEV_DEBUG22); //           PXD0(H14)       22
    gpio_dev_unmap(GPIO_33);
    gpio_dev_map(GPIO_33, GPIO_DEV_DEBUG23); //           PXD1(H14)       23
    gpio_dev_unmap(GPIO_34);
    gpio_dev_map(GPIO_34, GPIO_DEV_DEBUG24); //           PXD2(H14)       24
    gpio_dev_unmap(GPIO_35);
    gpio_dev_map(GPIO_35, GPIO_DEV_DEBUG25); //           PXD3(H14)       25
    gpio_dev_unmap(GPIO_36);
    gpio_dev_map(GPIO_36, GPIO_DEV_DEBUG26); //           PXD4(H14)       26
    gpio_dev_unmap(GPIO_37);
    gpio_dev_map(GPIO_37, GPIO_DEV_DEBUG27); //           PXD5(H14)       27
    gpio_dev_unmap(GPIO_38);
    gpio_dev_map(GPIO_38, GPIO_DEV_DEBUG28); //           PXD6(H14)       28
    gpio_dev_unmap(GPIO_39);
    gpio_dev_map(GPIO_39, GPIO_DEV_DEBUG29); //           PXD7(H14)       29

    // 42 -> 43
    gpio_dev_unmap(GPIO_42);
    gpio_dev_map(GPIO_42, GPIO_DEV_DEBUG30); //           P42(CONN4)      30
    gpio_dev_unmap(GPIO_43);
    gpio_dev_map(GPIO_43, GPIO_DEV_DEBUG31); //           P43(CONN4)      31
}

static bk_err_t gpio_dev_unprotect_unmap_wrapper(uint32_t gpio_id)
{
    return gpio_dev_unprotect_unmap(gpio_id);
}

static bk_err_t gpio_dev_unprotect_map_wrapper( uint32_t gpio_id, uint32_t dev)
{
    return gpio_dev_unprotect_map(gpio_id,dev);
}

int ps_need_pre_process_wrapper( UINT32 arg )
{
     return 0;
}

bool power_save_rf_sleep_check_wrapper(void)
{
     return false;
}

static UINT32 mcu_ps_machw_cal_wrapper(void)
{
    //return mcu_ps_machw_cal();
    return 0;
}

static UINT32 mcu_ps_machw_reset_wrapper(void)
{
    //return mcu_ps_machw_reset();
    return 0;
}

static UINT32 mcu_ps_machw_init_wrapper(void)
{
    //return mcu_ps_machw_init();
    return 0;
}

static void mcu_ps_bcn_callback_wrapper(uint8_t *data, int len)
{
	//mcu_ps_bcn_callback(data, len);
}

static int bk_feature_receive_bcmc_enable_wrapper(void)
{
    return bk_feature_receive_bcmc_enable();
}

static int bk_feature_bssid_connect_enable_wrapper(void)
{
    return bk_feature_bssid_connect_enable();
}

static int bk_feature_fast_connect_enable_wrapper(void)
{
    return bk_feature_fast_connect_enable();
}

static int bk_feature_send_deauth_before_connect_wrapper(void)
{
    return bk_feature_send_deauth_before_connect();
}

static int bk_feature_fast_dhcp_enable_wrapper(void)
{
    return bk_feature_fast_dhcp_enable();
}

static int bk_feature_not_check_ssid_enable_wrapper(void)
{
    return bk_feature_not_check_ssid_enable();
}

static int __IRAM2 bk_feature_config_cache_enable_wrapper(void)
{
    return bk_feature_config_cache_enable();
}

static int bk_feature_sta_vsie_enable_wrapper(void)
{
    return bk_feature_sta_vsie_enable();
}

static int bk_feature_ap_statype_limit_enable_wrapper(void)
{
    return bk_feature_ap_statype_limit_enable();
}

static int bk_feature_ckmn_enable_wrapper(void)
{
    return bk_feature_ckmn_enable();
}
static int bk_feature_config_wifi_csi_enable_wrapper(void)
{
    return bk_feature_config_wifi_csi_enable();
}

static int bk_feature_close_coexist_csa_wrapper(void)
{
    return bk_feature_close_coexist_csa();
}

static int bk_feature_get_scan_speed_level_wrapper(void)
{
    return bk_feature_get_scan_speed_level();
}

static int bk_feature_network_found_event_wrapper(void)
{
    return bk_feature_network_found_event();
}

static int bk_feature_get_mac_sup_sta_max_num_wrapper(void)
{
    return bk_feature_get_mac_sup_sta_max_num();
}

static void bk_feature_csi_out_cb_wrapper(void *data)
{
    bk_wifi_csi_info_cb(data);
}
static void flush_all_dcache_wrapper(void)
{
#if CONFIG_CACHE_ENABLE
	// SOC_SMP will set cache to no-cachable
#ifndef CONFIG_SOC_SMP
	flush_all_dcache();
#endif
#endif
}

static unsigned long bk_ms_to_ticks_wrapper(unsigned long ms)
{
	return BK_MS_TO_TICKS(ms);
}

static bk_err_t dma_memcpy_wrapper(void *out, const void *in, uint32_t len)
{
    return dma_memcpy(out, in, len);
}

bk_err_t bk_wifi_interrupt_init(void)
{
	sys_drv_enable_mac_gen_int();
	sys_drv_enable_mac_prot_int();
	sys_drv_enable_mac_tx_trigger_int();
	sys_drv_enable_mac_rx_trigger_int();

#if (!CONFIG_SOC_BK7236A)
	// TX_RX_MISC no need on bk7236, for debug purpuse on bk7236, changed temporarily
	sys_drv_enable_mac_txrx_misc_int();
#endif

#if (CONFIG_SOC_BK7236A)
	// open FIQ_RC for phy on bk7236, for debug purpuse on bk7236, changed temporarily
	sys_drv_enable_modem_rc_int();
#endif

#if (CONFIG_SOC_BK7236XX || CONFIG_SOC_BK7239XX || CONFIG_SOC_BK7286XX)
	// enable HSU interrupt
#if CFG_HSU
	sys_drv_enable_hsu_int();
#endif
#endif

	sys_drv_enable_mac_txrx_timer_int();
	sys_drv_enable_modem_int();

	return BK_OK;
}

static void bk_wifi_stop_rf_wrapper(void)
{
	rf_module_vote_ctrl(RF_CLOSE,RF_BY_WIFI_BIT);
}

static uint32_t rtos_get_ms_per_tick_wrapper(void)
{
    return rtos_get_ms_per_tick();
}

static UINT32 __IRAM2 rf_pll_ctrl_wrapper(UINT32 cmd, UINT32 param)
{
#if (CONFIG_SOC_BK7236XX || CONFIG_SOC_BK7239XX)
    return rf_pll_ctrl(cmd, param);
#else
   return 0;
#endif
}

static uint32_t __IRAM2 rtos_disable_int_wrapper(void)
{
	return rtos_enter_critical();
}

static void __IRAM2 rtos_enable_int_wrapper(uint32_t int_level)
{
	rtos_exit_critical(int_level);
}

static uint32_t rtos_enter_critical_wrapper(void)
{
	return rtos_enter_critical();
}

static void rtos_exit_critical_wrapper(uint32_t irq_level)
{
	rtos_exit_critical(irq_level);
}

static uint32_t rtos_get_time_wrapper(void)
{
    return rtos_get_time();
}

static void *os_malloc_wrapper(const char *func_name, int line, size_t size)
{
#if 0 //(CONFIG_MEM_DEBUG)
    return (void *)os_malloc_debug(func_name, line, size, 0);
#else
    return (void *)os_malloc(size);
#endif
}

static void *os_malloc_sram_wrapper(const char *func_name, int line, size_t size)
{
    return (void *)os_malloc_sram(size);
}

static void __IRAM2 os_free_wrapper(void *mem_ptr)
{
    os_free(mem_ptr);
}


static void rtos_assert_wrapper(uint32_t exp)
{
    BK_ASSERT(exp);
}

static INT32 os_memcmp_wrapper(const void *s, const void *s1, UINT32 n)
{
	return os_memcmp(s, s1, (unsigned int)n);
}

static void *os_memset_wrapper(void *b, int c, UINT32 len)
{
	return (void *)os_memset(b, c, (unsigned int)len);
}

static void *__IRAM2 os_zalloc_wrapper(const char *func_name, int line, size_t size)
{
#if 0 //(CONFIG_MEM_DEBUG)
	return (void *)os_malloc_debug(func_name, line, size, 1);
#else
	return os_zalloc(size);
#endif
}

static void *rtos_realloc_wrapper(void *ptr, size_t size)
{
	return (void *)os_realloc(ptr, size);
}
#if 0
static int shell_assert_out_wrapper(bool bContinue, char * format, ...)
{
	return shell_assert_out(bContinue, format, ...);
}

static void bk_printf_wrapper(const char *fmt, ...)
{
	bk_printf(fmt, ...);
}

static void bk_null_printf_wrapper(const char *fmt, ...)
{
	bk_null_printf(fmt, ...);
}
#endif

static bool __IRAM2 wifi_ate_is_enabled_wrapper(void)
{
	return ate_is_enabled();
}

static bk_err_t __IRAM2 rtos_push_to_queue_wrapper( void** queue, void* message, uint32_t timeout_ms )
{
    return rtos_push_to_queue(queue, message, timeout_ms );
}

static bool rtos_is_queue_full_wrapper(void** queue )
{
    return rtos_is_queue_full(queue);
}

static bool __IRAM2 rtos_is_queue_empty_wrapper(void** queue )
{
    return rtos_is_queue_empty(queue);
}

static bk_err_t __IRAM2 rtos_pop_from_queue_wrapper( void** queue, void* message, uint32_t timeout_ms )
{
    return rtos_pop_from_queue( queue, message, timeout_ms );
}

static bk_err_t rtos_push_to_queue_front_wrapper( void** queue, void* message, uint32_t timeout_ms )
{
     return rtos_push_to_queue_front(queue, message, timeout_ms);
}

static uint64_t bk_aon_rtc_get_current_tick_wrapper(uint32_t id)
{
	return bk_aon_rtc_get_current_tick(id);
}

static uint64_t bk_aon_rtc_get_us_wrapper(void)
{
	return bk_aon_rtc_get_us();
}

static bk_err_t bk_int_isr_register_wrapper(uint32_t src, void* isr_callback, void*arg)
{

	return bk_int_isr_register(src, isr_callback, arg);
}

static float __IRAM2 bk_rtc_get_ms_tick_count_wrapper(void)
{
	return (float)bk_rtc_get_ms_tick_count();
}

static char *os_strdup_wrapper(const char *s)
{
	return (char *)os_strdup(s);
}

static INT32 os_strcmp_wrapper(const char *s1, const char *s2)
{
    return os_strcmp(s1, s2);
}

static char *os_strchr_wrapper(const char *s, int c)
{
    return (char *)os_strchr(s, c);
}

static UINT32 os_strlen_wrapper(const char *str)
{
    return os_strlen(str);
}

static void *os_memcpy_wrapper(void *out, const void *in, UINT32 n)
{
	return (void *)os_memcpy(out, in, n);
}

static void *os_memmove_wrapper(void *out, const void *in, UINT32 n)
{
	return (void *)os_memmove(out, in, n);
}

static unsigned int os_strlcpy_wrapper(char *dest, const char *src, size_t siz)
{

    return os_strlcpy(dest,src,siz);
}

static INT32 os_strncmp_wrapper(const char *s1, const char *s2, const UINT32 n)
{
	return os_strncmp(s1, s2, n);
}

static char *os_strcpy_wrapper(char *out, const char *in)
{
    return (char *)os_strcpy(out, in);
}

static bk_err_t rtos_delay_milliseconds_wrapper(uint32_t num_ms)
{
	return rtos_delay_milliseconds(num_ms);
}
#if 0
static INT32 os_snprintf_wrapper(char *buf, UINT32 size, const char *fmt, ...)
{

	return os_snprintf(buf, size, fmt, ...);
}
#endif
static INT32 os_vsnprintf_wrapper(char *buf, UINT32 size, const char *fmt, va_list ap)
{
	return os_vsnprintf(buf, size, fmt, ap);
}

static char *os_strncpy_wrapper(char *out, const char *in, const UINT32 n)
{
	return os_strncpy(out, in, n);
}

static UINT32 os_strtoul_wrapper(const char *nptr, char **endptr, int base)
{
	return os_strtoul(nptr, endptr, base);
}

static int os_strcasecmp_wrapper(const char *s1, const char *s2)
{
	return os_strcasecmp(s1, s2);
}

static int os_strncasecmp_wrapper(const char *s1, const char *s2, size_t n)
{

	return os_strncasecmp(s1, s2, n);
}

static char *os_strrchr_wrapper(const char *s, int c)
{
	return os_strrchr(s, c);
}

static char *os_strstr_wrapper(const char *haystack, const char *needle)
{
	return os_strstr(haystack, needle);
}

static bk_err_t rtos_init_semaphore_wrapper( void **semaphore, int maxCount )
{

    return rtos_init_semaphore(semaphore,maxCount);
}

static bk_err_t rtos_get_semaphore_wrapper(void **semaphore, uint32_t timeout_ms)
{
     return rtos_get_semaphore(semaphore, timeout_ms);
}

static bk_err_t rtos_deinit_semaphore_wrapper(void **semaphore)
{
     return rtos_deinit_semaphore(semaphore);
}

static int rtos_set_semaphore_wrapper(void **semaphore)
{
     return rtos_set_semaphore(semaphore);
}

static bk_err_t rtos_create_thread_wrapper(void** thread, uint8_t priority, const char* name,
                        void* function, uint32_t stack_size, void* arg )
{
    return rtos_create_thread(thread, priority, name, function, stack_size, arg);
}


static bk_err_t rtos_delete_thread_wrapper(void** thread)
{
	return rtos_delete_thread(thread);

}

static size_t rtos_get_free_heap_size_wrapper(void)
{
	return rtos_get_free_heap_size();
}

static bk_err_t rtos_deinit_queue_wrapper(void** queue)
{
	return rtos_deinit_queue(queue);
}

static bk_err_t rtos_init_queue_wrapper(void** queue, const char *name, uint32_t message_size, uint32_t number_of_messages)
{
	return rtos_init_queue(queue, name, message_size, number_of_messages);
}

static bk_err_t rtos_deinit_mutex_wrapper(void **mutex)
{
	return rtos_deinit_mutex(mutex);
}

static bk_err_t rtos_init_mutex_wrapper(void **mutex)
{
	return rtos_init_mutex(mutex);
}

static bk_err_t rtos_lock_mutex_wrapper(void **mutex)
{
	return rtos_lock_mutex(mutex);
}
static bk_err_t rtos_unlock_mutex_wrapper(void ** mutex )
{
	return rtos_unlock_mutex(mutex);
}

static bk_err_t rtos_reload_timer_wrapper(void *timer)
{
	return rtos_reload_timer(timer);
}

static bool rtos_is_timer_running_wrapper(void *timer)
{
	return rtos_is_timer_running(timer);
}

static bk_err_t rtos_stop_timer_wrapper(void *timer)
{
	return rtos_stop_timer(timer);
}
static bk_err_t rtos_init_timer_wrapper(void *timer, uint32_t time_ms, void *function, void *arg)
{
	return rtos_init_timer(timer, time_ms, function, arg);
}

static bool rtos_is_current_thread_wrapper(void** thread)
{
	return rtos_is_current_thread(thread);
}

static void register_wifi_dump_hook_wrapper(void *wifi_func)
{
	rtos_regist_wifi_dump_hook(wifi_func);
}

static void bk_airkiss_start_udp_boardcast_wrapper(u8 random_data)
{
	int err, i;
	int udp_broadcast_fd = -1;
	struct sockaddr_in remote_skt;

	BK_WIFI_LOGD(TAG, "start_udp_boardcast\n");
	udp_broadcast_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udp_broadcast_fd == -1) {
		BK_WIFI_LOGE(TAG, "Socket failed\r\n");
		return;
	}
	os_memset(&remote_skt, 0, sizeof(struct sockaddr_in));
	remote_skt.sin_family = AF_INET;
	remote_skt.sin_addr.s_addr = INADDR_BROADCAST;//INADDR_ANY;
	remote_skt.sin_port = htons(10000);

	i = 20;
	while (i --) {
		BK_WIFI_LOGD(TAG, "udp-sendto:%d\r\n", i);

		err = sendto(udp_broadcast_fd, &random_data, 1, 0, (struct sockaddr *)&remote_skt, sizeof(remote_skt));
		rtos_delay_milliseconds(2);

		if (err == -1)
			BK_WIFI_LOGE(TAG, "send udp boardcast failed\r\n");
	}

	BK_WIFI_LOGD(TAG, "close socket\r\n");
	close(udp_broadcast_fd);
}

static void tx_verify_test_call_back_wrapper(void)
{
#if CONFIG_ATE_TEST
	tx_verify_test_call_back();
#endif
}

static void sys_hal_enter_low_analog_wrapper(void)
{
#if CONFIG_RX_OPTIMIZE
#if (CONFIG_SOC_BK7236XX || CONFIG_SOC_BK7239XX)
	sys_hal_enter_low_analog();
#endif
#endif
}

static void sys_hal_exit_low_analog_wrapper(void)
{
#if CONFIG_RX_OPTIMIZE
#if (CONFIG_SOC_BK7236XX || CONFIG_SOC_BK7239XX)
	sys_hal_exit_low_analog();
#endif
#endif
}

static uint64_t __IRAM2 sys_hal_get_low_voltage_sleep_duration_us_wrapper(void)
{
#if CONFIG_ANA_RTC
	return sys_hal_get_low_voltage_sleep_duration_us();
#else
	return 0;
#endif
}

static void sys_hal_set_low_voltage_sleep_duration_us_wrapper(uint64_t sleep_duration)
{
#if CONFIG_ANA_RTC
	 sys_hal_set_low_voltage_sleep_duration_us(sleep_duration);
#endif
}

static int bk_otp_apb_read_wrapper(uint32_t item, uint8_t* buf, uint32_t size)
{
#if (!CONFIG_OTP)
    return BK_FAIL;
#else
    return bk_otp_apb_read(item, buf, size);
#endif
}

static int bk_otp_apb_update_wrapper(uint32_t item, uint8_t* buf, uint32_t size)
{
#if (!CONFIG_OTP)
    return BK_FAIL;
#else
    return bk_otp_apb_update(item, buf, size);
#endif
}

static uint64_t sys_hal_get_low_voltage_wakeup_time_us_wrapper(void)
{
	return sys_hal_get_low_voltage_wakeup_time_us();
}

static void sys_hal_set_low_voltage_wakeup_time_us_wrapper(uint64_t wakeup_time)
{
	sys_hal_set_low_voltage_wakeup_time_us(wakeup_time);
}

#ifndef CONFIG_SHELL_ASYNCLOG
static int sync_assert_out(bool bContinue, char *format, ...)
{
	u32         int_mask;
	va_list     args;
	char string[CONFIG_PRINTF_BUF_SIZE];

	va_start(args, format);

	int_mask = rtos_disable_int();

	vsnprintf(string, sizeof(string) - 1, format, args);
	uart_write_string(bk_get_printf_port(), string);

	va_end(args);

	if (bContinue) {
		rtos_enable_int(int_mask);
		return 1;
	}

	// Halt here
	for (;;);

	return 0;
}
#endif


static int bk_feature_mac_pwd_enable_wrapper(void)
{
    return bk_feature_mac_pwd_enable();
}

static int __IRAM2 bk_feature_wrls_pwd_enable_wrapper(void)
{
    return bk_feature_wrls_pwd_enable();
}

static int __IRAM2 bk_feature_cpu_pwd_enable_wrapper(void)
{
    return bk_feature_cpu_pwd_enable();
}
static int __IRAM2 bk_feature_mac_use_rtc_enable_wrapper(void)
{
    return bk_feature_mac_use_rtc_enable();
}

static int bk_feature_change_to_wifi_pll_enable_wrapper(void)
{
    return bk_feature_change_to_wifi_pll_enable();
}

static int __IRAM2 bk_feature_bk7239n_mp_enable_wrapper(void)
{
    return bk_feature_bk7239n_mp_enable();
}

UINT32 __IRAM2 rwnx_sys_is_use_abs_power_wrapper(void)
{
#if (CONFIG_SOC_BK7236XX || CONFIG_SOC_BK7239XX || CONFIG_SOC_BK7286XX)
	return rwnx_sys_is_use_abs_power();
#else
	return 0;
#endif
}

void rwnx_sys_set_use_abs_power_wrapper(UINT32 use)
{
#if (CONFIG_SOC_BK7236XX || CONFIG_SOC_BK7239XX || CONFIG_SOC_BK7286XX)
	rwnx_sys_set_use_abs_power(use);
#endif
}

static void rf_force_set_wifi_mode_wrapper(uint8_t switch_mode)
{
    volatile uint32_t *rf_reg = (volatile uint32_t *)(SOC_SYS_REG_BASE + 2*4);
    uint32_t reg_val = *rf_reg;
	if (switch_mode == 1)
    {
        reg_val |= (1 << 4);   // Set bit 4
        reg_val &= ~(1 << 5);  // Clear bit 5
        *rf_reg = reg_val;
    }
    else
    {
        reg_val &= ~(1 << 4);  // Clear bit 4
        reg_val |= (1 << 5);   // Set bit 5
        *rf_reg = reg_val;
    }
}

static UINT32 bk_feature_update_power_with_rssi_wrapper(void)
{
    return bk_feature_update_power_with_rssi();
}

__attribute__((section(".dtcm_sec_data "))) wifi_os_funcs_t g_wifi_os_funcs = {
	._version = BK_WIFI_OS_ADAPTER_VERSION,
	._manual_cal_rfcali        = manual_cal_rfcali_status,
	._rc_drv_get_rf_en = rc_drv_get_rf_en,
	._cal_set_rfconfig_BTPLL   = rwnx_cal_set_rfconfig_BTPLL,
	._cal_set_rfconfig_WIFIPLL = rwnx_cal_set_rfconfig_WIFIPLL,
	._calibration_init = calibration_init,
	._sys_is_enable_hw_tpc     = rwnx_sys_is_enable_hw_tpc,
	._sys_is_use_abs_power     = rwnx_sys_is_use_abs_power_wrapper,
	._sys_set_use_abs_power    = rwnx_sys_set_use_abs_power_wrapper,
#if CONFIG_NON_SIGANL_PHY_ENABLE
	._evm_via_mac_continue     = evm_via_mac_continue,
	._evm_via_mac_evt          = evm_via_mac_evt,
	._do_evm                   = do_evm,
	._do_rx_sensitivity        = do_rx_sensitivity,
#else
	._evm_via_mac_continue     = NULL,
	._evm_via_mac_evt          = NULL,
	._do_evm                   = NULL,
	._do_rx_sensitivity        = NULL,
#endif
	._rwnx_cal_save_trx_rcbekn_reg_val = rwnx_cal_save_trx_rcbekn_reg_val,
	._rc_drv_set_agc_manual_en = rc_drv_set_agc_manual_en,
	._rc_drv_set_rx_mode_enrxsw = rc_drv_set_rx_mode_enrxsw,
	._rc_drv_set_rf_macbypass = rc_drv_set_rf_macbypass,
	._rc_drv_get_rx_mode_enrxsw = rc_drv_get_rx_mode_enrxsw,
	._bk7011_max_rxsens_setting = bk7011_max_rxsens_setting,
	._bk7011_default_rxsens_setting = bk7011_default_rxsens_setting,
#if CONFIG_NON_SIGANL_PHY_ENABLE
	._tx_evm_rate_get = tx_evm_rate_get,
	._tx_evm_pwr_idx_get = tx_evm_pwr_idx_get,
	._tx_evm_modul_format_get = tx_evm_modul_format_get,
	._tx_evm_guard_i_tpye_get = tx_evm_guard_i_tpye_get,
	._tx_evm_bandwidth_get = tx_evm_bandwidth_get,
	._tx_evm_mode_get      = tx_evm_mode_get,
#else
	._tx_evm_rate_get = NULL,
	._tx_evm_pwr_idx_get = NULL,
	._tx_evm_modul_format_get = NULL,
	._tx_evm_guard_i_tpye_get = NULL,
	._tx_evm_bandwidth_get = NULL,
	._tx_evm_mode_get      = NULL,
#endif
	._rwnx_tpc_get_pwridx_by_rate = rwnx_tpc_get_pwridx_by_rate,
	._rwnx_is_enable_pwr_change_by_rssi = bk_feature_update_power_with_rssi_wrapper,
	._tpc_auto_change_pwr_by_rssi = tpc_auto_change_pwr_by_rssi,
	._rwnx_setting_for_single_rate = rwnx_setting_for_single_rate,
	//._delay05us = delay05us,
	._rwnx_cal_recover_rf_setting = rwnx_cal_recover_rf_setting,
	._rwnx_cal_recover_wifi_setting = rwnx_cal_recover_wifi_setting,
	._rwnx_cal_set_40M_setting = rwnx_cal_set_40M_setting,
	._rwnx_cal_set_20M_setting = rwnx_cal_set_20M_setting,
	._rwnx_cal_recover_rcbeken_reg_val = rwnx_cal_recover_rcbeken_reg_val,
	._rwnx_cal_recover_trx_reg_val = rwnx_cal_recover_trx_reg_val,
	._rwnx_cal_set_channel = rwnx_cal_set_channel_wrapper,
	._bk7011_cal_dpd = bk7011_cal_dpd_wrapper,
	._bk7011_cal_pll = bk7011_cal_pll,
	._bk7011_update_by_rx = bk7011_update_by_rx_wrapper,
	._rwnx_cal_load_trx_rcbekn_reg_val = rwnx_cal_load_trx_rcbekn_reg_val,
	._manual_get_epa_flag = manual_get_epa_flag,
#if CONFIG_NON_SIGANL_PHY_ENABLE
	._rxsens_start_flag_get = rxsens_start_flag_get,
#else
	._rxsens_start_flag_get = NULL,
#endif
	._pbuf_alloc = bk_pbuf_alloc_wrapper,
	._pbuf_free = bk_pbuf_free_wrapper,
	._pbuf_ref = bk_pbuf_ref_wrapper,
	._pbuf_header = bk_pbuf_header_wrapper,
	._pbuf_coalesce = bk_pbuf_coalesce_wrapper,
	._pbuf_cat = bk_pbuf_cat_wrapper,
	._get_rx_pbuf_type = bk_get_rx_pbuf_type_wrapper,
	._get_pbuf_pool_size = bk_get_pbuf_pool_size_wrapper,
	._get_netif_hostname = bk_get_netif_hostname_wrapper,
	._save_net_info = bk_save_net_info_wrapper,
	._get_net_info = bk_get_net_info_wrapper,
	._inet_ntoa = bk_inet_ntoa_wrapper,
	._lwip_ntohl = bk_lwip_ntohl_wrapper,
	._lwip_htons = bk_lwip_htons_wrapper,
	//._sta_ip_down = bk_sta_ip_down_wrapper,
	//._uap_ip_start = bk_uap_ip_start_wrapper,
	//._uap_ip_down = bk_uap_ip_down_wrapper,
	//._net_wlan_add_netif = bk_net_wlan_add_netif_wrapper,
	//._net_wlan_remove_netif = bk_net_wlan_remove_netif_wrapper,
	//._lookup_ipaddr = bk_lookup_ipaddr_wrapper,
	._set_sta_status = bk_set_sta_status_wrapper,
	//._sta_ip_mode_set = sta_ip_mode_set,
	//._net_begin_send_arp_reply = bk_net_begin_send_arp_reply_wrapper,
	._delay_us = delay_us_wrapper,
	._sys_drv_modem_bus_clk_ctrl = sys_drv_modem_bus_clk_ctrl_wrapper,
	._sys_drv_modem_clk_ctrl = sys_drv_modem_clk_ctrl_wrapper,
	._sys_drv_int_enable = sys_drv_int_enable_wrapper,
	._sys_drv_int_disable = sys_drv_int_disable_wrapper,
	._sys_drv_int_group2_enable = sys_drv_int_group2_enable_wrapper,
	._sys_drv_int_group2_disable = sys_drv_int_group2_disable_wrapper,
	._sys_ll_set_cpu_power_sleep_wakeup_pwd_ofdm = sys_ll_set_cpu_power_sleep_wakeup_pwd_ofdm_wrapper,
	._sys_ll_get_cpu_power_sleep_wakeup_pwd_ofdm = sys_ll_get_cpu_power_sleep_wakeup_pwd_ofdm_wrapper,
	._sys_ll_get_cpu_device_clk_enable_mac_cken = sys_ll_get_cpu_device_clk_enable_mac_cken_wrapper,
	._sys_ll_get_cpu_device_clk_enable_phy_cken = sys_ll_get_cpu_device_clk_enable_phy_cken_wrapper,
	._sys_drv_module_power_state_get = sys_drv_module_power_state_get_wrapper,
	._power_save_delay_sleep_check = power_save_delay_sleep_check_wrapper,
	._power_save_wake_mac_rf_if_in_sleep = power_save_wake_mac_rf_if_in_sleep_wrapper,
	._power_save_wake_mac_rf_end_clr_flag = power_save_wake_mac_rf_end_clr_flag_wrapper,
	._power_save_if_ps_rf_dtim_enabled = power_save_if_ps_rf_dtim_enabled_wrapper,
	._power_save_forbid_trace = power_save_forbid_trace_wrapper,
	._bk_pm_module_vote_sleep_ctrl = bk_pm_module_vote_sleep_ctrl_wrapper,
	._bk_pm_lpo_src_get = bk_pm_lpo_src_get_wrapper,
	._ps_need_pre_process = ps_need_pre_process_wrapper,
	._power_save_rf_sleep_check = power_save_rf_sleep_check_wrapper,
	._bk_pm_module_power_state_get = bk_pm_module_power_state_get_wrapper,
	._bk_pm_module_vote_power_ctrl = bk_pm_module_vote_power_ctrl_wrapper,
	._bk_pm_module_vote_cpu_freq = bk_pm_module_vote_cpu_freq_wrapper,
	._bk_pm_phy_reinit_flag_get = bk_pm_phy_reinit_flag_get_wrapper,
	._bk_pm_phy_reinit_flag_clear = bk_pm_phy_reinit_flag_clear_wrapper,
	._bk_pm_sleep_register = bk_pm_sleep_register_wrapper,
	._bk_pm_low_voltage_register = bk_pm_low_voltage_register_wrapper,
	._bk_pm_wifi_rtc_set = bk_pm_wifi_rtc_set_wrapper,
	._bk_pm_wifi_rtc_clear = bk_pm_wifi_rtc_clear_wrapper,
	._wifi_vote_rf_ctrl = wifi_vote_rf_ctrl_wrapper,
	._wifi_phy_clk_open = wifi_phy_clk_open_wrapper,
	._wifi_phy_clk_close = wifi_phy_clk_close_wrapper,
	._wifi_mac_clk_open = wifi_mac_clk_open_wrapper,
	._wifi_mac_clk_close = wifi_mac_clk_close_wrapper,
	._wifi_mac_phy_power_on = wifi_mac_phy_power_on_wrapper,
	._mac_ps_exc32_cb_notify = mac_ps_exc32_cb_notify_wrapper,
	._mac_ps_exc32_init = mac_ps_exc32_init_wrapper,
	._wifi_32k_src_switch_ready_notify = wifi_32k_src_switch_ready_notify_wrapper,
	._mac_ps_bcn_callback = mac_ps_bcn_callback_wrapper,
	._mac_sleeped = mac_sleeped_wrapper,
	._rwnx_cal_mac_sleep_rc_clr = rwnx_cal_mac_sleep_rc_clr_wrapper,
	._rwnx_cal_mac_sleep_rc_recover = rwnx_cal_mac_sleep_rc_recover_wrapper,
	._bk_rosc_32k_ckest_prog = bk_rosc_32k_ckest_prog_wrapper,
	._bk_rosc_32k_get_ppm = bk_rosc_32k_get_ppm_wrapper,
	._bk_rosc_32k_get_freq = bk_rosc_32k_get_freq_wrapper,
	._bk_rosc_32k_get_time_diff = bk_rosc_32k_get_time_diff_wrapper,
	._mac_printf_encode = mac_printf_encode_wrapper,
	._dbg_enable_debug_gpio = dbg_enable_debug_gpio_wrapper,
	._gpio_dev_unprotect_unmap = gpio_dev_unprotect_unmap_wrapper,
	._gpio_dev_unprotect_map = gpio_dev_unprotect_map_wrapper,
	._mcu_ps_machw_cal = mcu_ps_machw_cal_wrapper,
	._mcu_ps_machw_reset = mcu_ps_machw_reset_wrapper,
	._mcu_ps_machw_init = mcu_ps_machw_init_wrapper,
	._mcu_ps_bcn_callback = mcu_ps_bcn_callback_wrapper,
	._bk_feature_config_wifi_csi_enable = bk_feature_config_wifi_csi_enable_wrapper,
	._bk_feature_receive_bcmc_enable = bk_feature_receive_bcmc_enable_wrapper,
	._bk_feature_bssid_connect_enable = bk_feature_bssid_connect_enable_wrapper,
	._bk_feature_fast_connect_enable = bk_feature_fast_connect_enable_wrapper,
	._bk_feature_send_deauth_before_connect = bk_feature_send_deauth_before_connect_wrapper,
	._bk_feature_get_scan_speed_level = bk_feature_get_scan_speed_level_wrapper,
	._bk_feature_fast_dhcp_enable = bk_feature_fast_dhcp_enable_wrapper,
	._bk_feature_not_check_ssid_enable = bk_feature_not_check_ssid_enable_wrapper,
	._bk_feature_config_cache_enable = bk_feature_config_cache_enable_wrapper,
	._bk_feature_config_ckmn_enable = bk_feature_ckmn_enable_wrapper,
	._bk_feature_sta_vsie_enable = bk_feature_sta_vsie_enable_wrapper,
	._bk_feature_ap_statype_limit_enable = bk_feature_ap_statype_limit_enable_wrapper,
	._bk_feature_close_coexist_csa = bk_feature_close_coexist_csa_wrapper,
	._bk_feature_network_found_event = bk_feature_network_found_event_wrapper,
	._bk_feature_get_mac_sup_sta_max_num = bk_feature_get_mac_sup_sta_max_num_wrapper,
	._bk_feature_csi_out_cb = bk_feature_csi_out_cb_wrapper,
	._flush_all_dcache = flush_all_dcache_wrapper,
	._bk_ms_to_ticks = bk_ms_to_ticks_wrapper,
	._dma_memcpy = dma_memcpy_wrapper,
	._bk_wifi_interrupt_init = bk_wifi_interrupt_init,
	._bk_wifi_stop_rf = bk_wifi_stop_rf_wrapper,
	._rtos_get_ms_per_tick = rtos_get_ms_per_tick_wrapper,
	._rtos_disable_int = rtos_disable_int_wrapper,
	._rtos_enable_int =rtos_enable_int_wrapper,
	._rtos_enter_critical = rtos_enter_critical_wrapper,
	._rtos_exit_critical = rtos_exit_critical_wrapper,
	._log = bk_wifi_printf_ext,
	._log_raw = bk_wifi_printf_raw,
	._rtos_get_time = rtos_get_time_wrapper,
	._os_malloc = os_malloc_wrapper,
	._os_malloc_sram = os_malloc_sram_wrapper,
	._os_free = os_free_wrapper,
	._rtos_assert = rtos_assert_wrapper,
	._os_memcmp = os_memcmp_wrapper,
	._os_memset = os_memset_wrapper,
	._os_zalloc = os_zalloc_wrapper,
	._rtos_realloc = rtos_realloc_wrapper,
#ifdef CONFIG_SHELL_ASYNCLOG
	._shell_assert_out = shell_assert_out,
#else
	._shell_assert_out = sync_assert_out,
#endif
	._bk_printf = bk_printf,
	._bk_null_printf = bk_null_printf,
	._ate_is_enabled = wifi_ate_is_enabled_wrapper,
	._rtos_push_to_queue = rtos_push_to_queue_wrapper,
	._rtos_is_queue_full = rtos_is_queue_full_wrapper,
	._rtos_is_queue_empty = rtos_is_queue_empty_wrapper,
	._rtos_pop_from_queue = rtos_pop_from_queue_wrapper,
	._rtos_push_to_queue_front = rtos_push_to_queue_front_wrapper,
	._bk_aon_rtc_get_current_tick = bk_aon_rtc_get_current_tick_wrapper,
	._bk_aon_rtc_get_us = bk_aon_rtc_get_us_wrapper,
	._bk_int_isr_register = bk_int_isr_register_wrapper,
	._rtc_get_ms_tick_cnt = bk_rtc_get_ms_tick_count_wrapper,
	._rtos_init_semaphore = rtos_init_semaphore_wrapper,
	._rtos_get_semaphore = rtos_get_semaphore_wrapper,
	._rtos_deinit_semaphore = rtos_deinit_semaphore_wrapper,
	._rtos_set_semaphore = rtos_set_semaphore_wrapper,
	._rtos_create_thread = rtos_create_thread_wrapper,
	._rtos_delete_thread = rtos_delete_thread_wrapper,
	._rtos_get_free_heap_size = rtos_get_free_heap_size_wrapper,
	._rtos_deinit_queue = rtos_deinit_queue_wrapper,
	._rtos_init_queue = rtos_init_queue_wrapper,
	._rtos_deinit_mutex = rtos_deinit_mutex_wrapper,
	._rtos_init_mutex = rtos_init_mutex_wrapper,
	._rtos_lock_mutex = rtos_lock_mutex_wrapper,
	._rtos_unlock_mutex = rtos_unlock_mutex_wrapper,
	._rtos_reload_timer = rtos_reload_timer_wrapper,
	._rtos_is_timer_running = rtos_is_timer_running_wrapper,
	._rtos_stop_timer = rtos_stop_timer_wrapper,
	._rtos_init_timer = rtos_init_timer_wrapper,
	._rtos_is_current_thread = rtos_is_current_thread_wrapper,
	._register_wifi_dump_hook = register_wifi_dump_hook_wrapper,
	/// os str API function pointer
	._os_strdup = os_strdup_wrapper,
	._os_strcmp = os_strcmp_wrapper,
	._os_strchr = os_strchr_wrapper,
	._os_strlen = os_strlen_wrapper,
	._os_memcpy = os_memcpy_wrapper,
	._os_memmove = os_memmove_wrapper,
	._os_strlcpy = os_strlcpy_wrapper,
	._os_strncmp = os_strncmp_wrapper,
	._os_strcpy = os_strcpy_wrapper,
	._rtos_delay_milliseconds = rtos_delay_milliseconds_wrapper,
	._os_snprintf = os_snprintf,
	//._os_vsnprintf = os_vsnprintf_wrapper,
	._os_strncpy = os_strncpy_wrapper,
	._os_strtoul = os_strtoul_wrapper,
	._os_strcasecmp = os_strcasecmp_wrapper,
	._os_strncasecmp = os_strncasecmp_wrapper,
	._os_strrchr = os_strrchr_wrapper,
	._os_strstr = os_strstr_wrapper,
	/////
	._rf_pll_ctrl = rf_pll_ctrl_wrapper,
	._send_udp_bc_pkt = bk_airkiss_start_udp_boardcast_wrapper,
	._tx_verify_test_call_back = tx_verify_test_call_back_wrapper,
	._sys_hal_enter_low_analog = sys_hal_enter_low_analog_wrapper,
	._sys_hal_exit_low_analog = sys_hal_exit_low_analog_wrapper,
	._rw_msg_send = rw_msg_send,
	#if(CONFIG_SOC_BK7236XX || CONFIG_SOC_BK7239XX || CONFIG_SOC_BK7256XX)
	._tpc_change_pwr_by_media = tpc_change_pwr_by_media,
	._tpc_set_media_pwr_level = tpc_set_media_pwr_level,
	._tpc_get_media_pwr_level = tpc_get_media_pwr_level,
	#else
	._tpc_change_pwr_by_media = NULL,
	._tpc_set_media_pwr_level = NULL,
	._tpc_get_media_pwr_level = NULL,
	#endif
	._bk_otp_apb_read = bk_otp_apb_read_wrapper,
	._bk_otp_apb_update = bk_otp_apb_update_wrapper,
	._sys_hal_get_low_voltage_sleep_duration_us = sys_hal_get_low_voltage_sleep_duration_us_wrapper,
	._sys_hal_set_low_voltage_sleep_duration_us = sys_hal_set_low_voltage_sleep_duration_us_wrapper,
	._sys_hal_get_low_voltage_wakeup_time_us = sys_hal_get_low_voltage_wakeup_time_us_wrapper,
	._sys_hal_set_low_voltage_wakeup_time_us = sys_hal_set_low_voltage_wakeup_time_us_wrapper,
	._bk_feature_config_mac_pwd_enable = bk_feature_mac_pwd_enable_wrapper,
	._bk_feature_config_wrls_pwd_enable = bk_feature_wrls_pwd_enable_wrapper,
	._bk_feature_config_cpu_pwd_enable = bk_feature_cpu_pwd_enable_wrapper,
	._bk_feature_config_mac_use_rtc_enable = bk_feature_mac_use_rtc_enable_wrapper,
	._bk_feature_config_bk7239n_mp_enable = bk_feature_bk7239n_mp_enable_wrapper,
	._cli_printf = cli_printf,
	._rf_force_set_wifi_mode = rf_force_set_wifi_mode_wrapper,
	._bk_feature_change_to_wifi_pll_enable = bk_feature_change_to_wifi_pll_enable_wrapper,
};

__attribute__((section(".dtcm_sec_data "))) wifi_os_variable_t g_wifi_os_variable = {
	._sys_drv_clk_on = SYS_DRV_CLK_ON,
	._sys_sys_debug_config1_addr = SYS_SYS_DEBUG_CONFIG1_ADDR,
	._ps_forbid_in_doze = PS_FORBID_IN_DOZE,
	._ps_forbid_txing = PS_FORBID_TXING,
	._ps_forbid_hw_timer = PS_FORBID_HW_TIMER,
	._ps_forbid_vif_prevent = PS_FORBID_VIF_PREVENT,
	._ps_forbid_keevt_on = PS_FORBID_KEEVT_ON,
	._ps_bmsg_iotcl_rf_timer_init = PS_BMSG_IOCTL_RF_PS_TIMER_INIT,
	._ps_bmsg_ioctl_rf_ps_timer_deint = PS_BMSG_IOCTL_RF_PS_TIMER_DEINIT,
	._ps_bmsg_ioctl_rf_enable = PS_BMSG_IOCTL_RF_ENABLE,
	._ps_bmsg_ioctl_mcu_enable = PS_BMSG_IOCTL_MCU_ENABLE,
	._ps_bmsg_ioctl_mcu_disable = PS_BMSG_IOCTL_MCU_DISANABLE,
	._ps_bmsg_ioctl_ps_enable = PS_BMSG_IOCTL_PS_ENABLE,
	._ps_bmsg_ioctl_ps_disable = PS_BMSG_IOCTL_PS_DISANABLE,
	._ps_bmsg_ioctl_exc32k_start = PS_BMSG_IOCTL_EXC32K_START,
	._ps_bmsg_ioctl_exc32k_stop = PS_BMSG_IOCTL_EXC32K_STOP,
	._pm_lpo_src_divd = PM_LPO_SRC_DIVD,
	._pm_lpo_src_x32k = PM_LPO_SRC_X32K,
	._pm_lpo_src_rosc = PM_LPO_SRC_ROSC,
	._pm_power_module_name_btsp = PM_POWER_SUB_MODULE_NAME_BTSP,
	._pm_power_module_name_wifip_mac = PM_POWER_MODULE_NAME_WIFIP_MAC,
	._pm_power_module_name_phy = PM_POWER_MODULE_NAME_PHY,
	._pm_power_module_state_off = PM_POWER_MODULE_STATE_OFF,
	._pm_power_module_state_on = PM_POWER_MODULE_STATE_ON,
	._low_power_xtal_dpll_stability_delay_time = LOW_POWER_XTAL_DPLL_STABILITY_DELAY_TIME,
	._low_power_delay_time_hardware = LOW_POWER_DELAY_TIME_HARDWARE,
	._low_power_extern_32k_hardware = LOW_POWER_EXTERN_32K_HARDWARE,
	._low_power_26m_stability_delay_time_hardware = LOW_POWER_26M_STABILITY_DELAY_TIME_HARDWARE,
#if CONFIG_SOC_BK7239XX
	._low_power_bootup_restore_delay_time = LOW_POWER_BOOTUP_RESTORE_TIME,
#endif
	._pm_power_sub_module_name_phy_wifi = PM_POWER_SUB_MODULE_NAME_PHY_WIFI,
	._pm_sleep_module_name_wifip_mac = PM_SLEEP_MODULE_NAME_WIFIP_MAC,
	._pm_dev_id_mac = PM_DEV_ID_MAC,
	._pm_cpu_frq_60m = PM_CPU_FRQ_60M,
	._pm_cpu_frq_80m = PM_CPU_FRQ_80M,
	._pm_cpu_frq_120m = PM_CPU_FRQ_120M,
	._pm_cpu_frq_high = PM_CPU_FRQ_HIGHEST,
	._pm_cpu_frq_default = PM_CPU_FRQ_60M,
	._pm_32k_step_begin = PM_32K_STEP_BEGIN,
	._pm_32k_step_finish = PM_32K_STEP_FINISH,
	._cmd_rf_wifipll_hold_bit_set = CMD_RF_WIFIPLL_HOLD_BIT_SET,
	._cmd_rf_wifipll_hold_bit_clr = CMD_RF_WIFIPLL_HOLD_BIT_CLR,
	._rf_wifipll_hold_by_wifi_bit = RF_WIFIPLL_HOLD_BY_WIFI_BIT,
	._wifi_modem_en = WIFI_MODEM_EN,
	._wifi_modem_rc_en = WIFI_MODEM_RC_EN,
	._wifi_mac_tx_rx_timer_int_bit = WIFI_MAC_TX_RX_TIMER_INT_BIT,
	._wifi_mac_tx_rx_misc_int_bit = WIFI_MAC_TX_RX_MISC_INT_BIT,
	._wifi_mac_rx_trigger_int_bit = WIFI_MAC_RX_TRIGGER_INT_BIT,
	._wifi_mac_tx_trigger_int_bit = WIFI_MAC_TX_TRIGGER_INT_BIT,
	._wifi_mac_port_trigger_int_bit = WIFI_MAC_PORT_TRIGGER_INT_BIT,
	._wifi_mac_gen_int_bit = WIFI_MAC_GEN_INT_BIT,
	._wifi_hsu_interrupt_ctrl_bit = WIFI_HSU_INTERRUPT_CTRL_BIT,
	._wifi_mac_wakeup_int_bit = WIFI_MAC_WAKEUP_INT_BIT,
	._int_src_mac_general = INT_SRC_MAC_GENERAL,
	._int_src_mac_rx_trigger = INT_SRC_MAC_RX_TRIGGER,
	._int_src_mac_txrx_timer = INT_SRC_MAC_TXRX_TIMER,
	._int_src_mac_prot_trigger = INT_SRC_MAC_PROT_TRIGGER,
	._int_src_mac_tx_trigger = INT_SRC_MAC_TX_TRIGGER,
	._int_src_modem = INT_SRC_MODEM,
	._int_src_modem_rc = INT_SRC_MODEM_RC,
	#if CONFIG_IMPROVE_HE_TB_ENABLE
	._improve_he_tb_enable = true,
	#else
	._improve_he_tb_enable = false,
	#endif
	#if (CONFIG_SOC_BK7236XX) || (CONFIG_SOC_BK7239XX)
	._pm_low_voltage_delta_wakeup_delay_in_us = PM_LOW_VOLTAGE_DELTA_WAKEUP_DELAY_IN_US,
	#endif
	#if (!CONFIG_SOC_BK7239XX)
	._OTP_CHIP_RESERVED = OTP_CHIP_RESERVED,
	#endif
};

Countryregulations country_regulation_table[] = {

    {"AE", COUNTRY_AE, PW_LMT_REGU_ETSI     },
    {"AF", COUNTRY_AF, PW_LMT_REGU_ETSI    },
    {"AI", COUNTRY_AI, PW_LMT_REGU_ETSI    },
    {"AL", COUNTRY_AL, PW_LMT_REGU_ETSI    },
    {"AM", COUNTRY_AM, PW_LMT_REGU_ETSI    },
    {"AN", COUNTRY_AN, PW_LMT_REGU_ETSI    },
    {"AR", COUNTRY_AR, PW_LMT_REGU_ETSI     },
    {"AS", COUNTRY_AS, PW_LMT_REGU_FCC     },
    {"AT", COUNTRY_AT, PW_LMT_REGU_ETSI    },
    {"AW", COUNTRY_AW, PW_LMT_REGU_ETSI    },
    {"AZ", COUNTRY_AZ, PW_LMT_REGU_ETSI    },
    {"BA", COUNTRY_BA, PW_LMT_REGU_ETSI    },
    {"BB", COUNTRY_BB, PW_LMT_REGU_ETSI     },
    {"BD", COUNTRY_BD, PW_LMT_REGU_ETSI      },
    {"BE", COUNTRY_BE, PW_LMT_REGU_ETSI    },
    {"BF", COUNTRY_BF, PW_LMT_REGU_ETSI     },
    {"BG", COUNTRY_BG, PW_LMT_REGU_ETSI    },
    {"BH", COUNTRY_BH, PW_LMT_REGU_ETSI      },
    {"BL", COUNTRY_BL, PW_LMT_REGU_ETSI    },
    {"BM", COUNTRY_BM, PW_LMT_REGU_FCC     },
    {"BN", COUNTRY_BN, PW_LMT_REGU_ETSI      },
    {"BO", COUNTRY_BO, PW_LMT_REGU_ETSI      },
    {"BR", COUNTRY_BR, PW_LMT_REGU_ETSI     },
    {"BS", COUNTRY_BS, PW_LMT_REGU_ETSI     },
    {"BT", COUNTRY_BT, PW_LMT_REGU_ETSI    },
    {"BY", COUNTRY_BY, PW_LMT_REGU_ETSI    },
    {"BZ", COUNTRY_BZ, PW_LMT_REGU_ETSI      },
    {"CA", COUNTRY_CA, PW_LMT_REGU_FCC     },
    {"CF", COUNTRY_CF, PW_LMT_REGU_ETSI     },
    {"CH", COUNTRY_CH, PW_LMT_REGU_ETSI    },
    {"CI", COUNTRY_CI, PW_LMT_REGU_ETSI     },
    {"CL", COUNTRY_CL, PW_LMT_REGU_ETSI      },
    {"CN", COUNTRY_CN, PW_LMT_REGU_ETSI     },
    {"CO", COUNTRY_CO, PW_LMT_REGU_ETSI     },
    {"CR", COUNTRY_CR, PW_LMT_REGU_ETSI     },
    {"CX", COUNTRY_CX, PW_LMT_REGU_ETSI     },
    {"CY", COUNTRY_CY, PW_LMT_REGU_ETSI    },
    {"CZ", COUNTRY_CZ, PW_LMT_REGU_ETSI    },
    {"DE", COUNTRY_DE, PW_LMT_REGU_ETSI    },
    {"DK", COUNTRY_DK, PW_LMT_REGU_ETSI    },
    {"DM", COUNTRY_DM, PW_LMT_REGU_FCC     },
    {"DO", COUNTRY_DO, PW_LMT_REGU_FCC     },
    {"DZ", COUNTRY_DZ, PW_LMT_REGU_ETSI      },
    {"EC", COUNTRY_EC, PW_LMT_REGU_ETSI     },
    {"EE", COUNTRY_EE, PW_LMT_REGU_ETSI    },
    {"EG", COUNTRY_EG, PW_LMT_REGU_ETSI    },
    {"ES", COUNTRY_ES, PW_LMT_REGU_ETSI    },
    {"ET", COUNTRY_ET, PW_LMT_REGU_ETSI    },
    {"FI", COUNTRY_FI, PW_LMT_REGU_ETSI    },
    {"FM", COUNTRY_FM, PW_LMT_REGU_FCC     },
    {"FR", COUNTRY_FR, PW_LMT_REGU_ETSI    },
    {"GB", COUNTRY_GB, PW_LMT_REGU_ETSI    },
    {"GD", COUNTRY_GD, PW_LMT_REGU_FCC     },
    {"GE", COUNTRY_DE, PW_LMT_REGU_ETSI    },
    {"GF", COUNTRY_GF, PW_LMT_REGU_ETSI    },
    {"GH", COUNTRY_GH, PW_LMT_REGU_ETSI     },
    {"GL", COUNTRY_GL, PW_LMT_REGU_ETSI    },
    {"GP", COUNTRY_GP, PW_LMT_REGU_ETSI    },
    {"GR", COUNTRY_GR, PW_LMT_REGU_ETSI    },
    {"GT", COUNTRY_GT, PW_LMT_REGU_FCC     },
    {"GU", COUNTRY_GU, PW_LMT_REGU_FCC     },
    {"HN", COUNTRY_HN, PW_LMT_REGU_FCC     },
    {"HR", COUNTRY_HR, PW_LMT_REGU_ETSI    },
    {"HT", COUNTRY_HT, PW_LMT_REGU_FCC     },
    {"HU", COUNTRY_HU, PW_LMT_REGU_ETSI    },
    {"ID", COUNTRY_ID, PW_LMT_REGU_ETSI      },
    {"IE", COUNTRY_IE, PW_LMT_REGU_ETSI    },
    {"IL", COUNTRY_IL, PW_LMT_REGU_ETSI    },
    {"IN", COUNTRY_IN, PW_LMT_REGU_ETSI      },
    {"IR", COUNTRY_IR, PW_LMT_REGU_ETSI      },
    {"IS", COUNTRY_IS, PW_LMT_REGU_ETSI    },
    {"IT", COUNTRY_IT, PW_LMT_REGU_ETSI    },
    {"JM", COUNTRY_JM, PW_LMT_REGU_ETSI     },
    {"JO", COUNTRY_JO, PW_LMT_REGU_ETSI      },
    {"JP", COUNTRY_JP, PW_LMT_REGU_MKK      },    ////PW_LMT_REGU_JP
    {"KE", COUNTRY_KE, PW_LMT_REGU_ETSI      },
    {"KH", COUNTRY_KH, PW_LMT_REGU_ETSI    },
    {"KN", COUNTRY_KN, PW_LMT_REGU_ETSI    },
    {"KP", COUNTRY_KP, PW_LMT_REGU_ETSI      },
    {"KR", COUNTRY_KR, PW_LMT_REGU_ETSI      },
    {"KW", COUNTRY_KW, PW_LMT_REGU_ETSI    },
    {"KY", COUNTRY_KY, PW_LMT_REGU_ETSI     },
    {"LB", COUNTRY_LB, PW_LMT_REGU_ETSI     },
    {"LC", COUNTRY_LC, PW_LMT_REGU_ETSI    },
    {"LI", COUNTRY_LI, PW_LMT_REGU_ETSI    },
    {"LK", COUNTRY_LK, PW_LMT_REGU_FCC     },
    {"LS", COUNTRY_LS, PW_LMT_REGU_ETSI    },
    {"LT", COUNTRY_LT, PW_LMT_REGU_ETSI    },
    {"LU", COUNTRY_LU, PW_LMT_REGU_ETSI    },
    {"LV", COUNTRY_LV, PW_LMT_REGU_ETSI    },
    {"MA", COUNTRY_MA, PW_LMT_REGU_ETSI    },
    {"MC", COUNTRY_MC, PW_LMT_REGU_ETSI    },
    {"MD", COUNTRY_MD, PW_LMT_REGU_ETSI    },
    {"ME", COUNTRY_ME, PW_LMT_REGU_ETSI    },
    {"MF", COUNTRY_MF, PW_LMT_REGU_ETSI    },
    {"MH", COUNTRY_MH, PW_LMT_REGU_FCC     },
    {"MK", COUNTRY_MK, PW_LMT_REGU_ETSI    },
    {"MN", COUNTRY_MN, PW_LMT_REGU_ETSI     },
    {"MP", COUNTRY_MP, PW_LMT_REGU_FCC     },
    {"MQ", COUNTRY_MQ, PW_LMT_REGU_ETSI    },
    {"MR", COUNTRY_MR, PW_LMT_REGU_ETSI    },
    {"MT", COUNTRY_MT, PW_LMT_REGU_ETSI    },
    {"MU", COUNTRY_MU, PW_LMT_REGU_ETSI     },
    {"MW", COUNTRY_MW, PW_LMT_REGU_ETSI    },
    {"MX", COUNTRY_MX, PW_LMT_REGU_ETSI     },
    {"MY", COUNTRY_MY, PW_LMT_REGU_ETSI     },
    {"NI", COUNTRY_NI,  PW_LMT_REGU_FCC     },
    {"NL", COUNTRY_NL, PW_LMT_REGU_ETSI    },
    {"NO", COUNTRY_NO, PW_LMT_REGU_ETSI    },
    {"NP", COUNTRY_NP, PW_LMT_REGU_ETSI      },
    {"NZ", COUNTRY_NZ, PW_LMT_REGU_ETSI     },
    {"OM", COUNTRY_OM, PW_LMT_REGU_ETSI    },
    {"PA", COUNTRY_PA, PW_LMT_REGU_FCC     },
    {"PE", COUNTRY_PE, PW_LMT_REGU_ETSI     },
    {"PF", COUNTRY_PF, PW_LMT_REGU_ETSI    },
    {"PG", COUNTRY_PG, PW_LMT_REGU_ETSI     },
    {"PH", COUNTRY_PH, PW_LMT_REGU_ETSI     },
    {"PK", COUNTRY_PK, PW_LMT_REGU_ETSI      },
    {"PL", COUNTRY_PL, PW_LMT_REGU_ETSI    },
    {"PM", COUNTRY_PM, PW_LMT_REGU_ETSI    },
    {"PR", COUNTRY_PR, PW_LMT_REGU_FCC     },
    {"PT", COUNTRY_PT, PW_LMT_REGU_ETSI    },
    {"PW", COUNTRY_PW, PW_LMT_REGU_FCC     },
    {"PY", COUNTRY_PY, PW_LMT_REGU_FCC     },
    {"QA", COUNTRY_QA, PW_LMT_REGU_ETSI      },
    {"RS", COUNTRY_RS, PW_LMT_REGU_ETSI    },
    {"RU", COUNTRY_RU, PW_LMT_REGU_ETSI    },
    {"RW", COUNTRY_RW, PW_LMT_REGU_ETSI     },
    {"SA", COUNTRY_SA, PW_LMT_REGU_ETSI    },
    {"SE", COUNTRY_SE, PW_LMT_REGU_ETSI    },
    {"SG", COUNTRY_SG, PW_LMT_REGU_ETSI     },
    {"SI", COUNTRY_SI, PW_LMT_REGU_ETSI    },
    {"SK", COUNTRY_SK, PW_LMT_REGU_ETSI    },
    {"SN", COUNTRY_SN, PW_LMT_REGU_ETSI     },
    {"SR", COUNTRY_SR, PW_LMT_REGU_ETSI    },
    {"SV", COUNTRY_SV, PW_LMT_REGU_ETSI     },
    {"TC", COUNTRY_TC, PW_LMT_REGU_ETSI     },
    {"TD", COUNTRY_TD, PW_LMT_REGU_ETSI    },
    {"TG", COUNTRY_TG, PW_LMT_REGU_ETSI    },
    {"TH", COUNTRY_TH, PW_LMT_REGU_ETSI     },
    {"TN", COUNTRY_TN, PW_LMT_REGU_ETSI    },
    {"TR", COUNTRY_TR, PW_LMT_REGU_ETSI    },
    {"TT", COUNTRY_TT, PW_LMT_REGU_FCC     },
    {"TW", COUNTRY_TW, PW_LMT_REGU_FCC      },
    {"UA", COUNTRY_UA, PW_LMT_REGU_ETSI    },
    {"UG", COUNTRY_UG, PW_LMT_REGU_ETSI     },
    {"US", COUNTRY_US, PW_LMT_REGU_FCC     },
    {"UY", COUNTRY_UY, PW_LMT_REGU_ETSI     },
    {"UZ", COUNTRY_UZ, PW_LMT_REGU_ETSI    },
    {"VC", COUNTRY_VC, PW_LMT_REGU_ETSI    },
    {"VE", COUNTRY_VE, PW_LMT_REGU_ETSI     },
    {"VI", COUNTRY_VI, PW_LMT_REGU_FCC     },
    {"VN", COUNTRY_VN, PW_LMT_REGU_ETSI     },
    {"VU", COUNTRY_VU, PW_LMT_REGU_ETSI     },
    {"WF", COUNTRY_WF, PW_LMT_REGU_ETSI    },
    {"YT", COUNTRY_YT, PW_LMT_REGU_ETSI    },
    {"ZA", COUNTRY_ZA, PW_LMT_REGU_ETSI    },
    {"ZW", COUNTRY_ZW, PW_LMT_REGU_ETSI    },


};

#define TXPWR_LMT_ENTRY_SIZE   7
#define TXPWR_LMT_REGULATION_NUM   4

uint8_t txpwr_lmt_tbl[] = {
////regulation, band, bandwidht, ratesection, rfpath, chanl, power_index
////11b: 18dbm, 11g:17dbm   11n: 16dbm
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,1, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,1, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,1, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,1, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,2, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,2, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,2, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,2, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,3, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,3, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,3, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,3, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,4, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,4, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,4, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,4, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,5, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,5, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,5, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,5, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,6, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,6, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,6, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,6, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,7, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,7, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,7, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,7, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,8, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,8, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,8, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,8, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,9, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,9, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,9, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,9, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,10, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,10, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,10, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,10, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,11, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,11, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,11, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,11, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,12, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,12, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,12, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,12, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,13, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,13, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,13, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,13, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,14, 36,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,14, 36,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,14, 36,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_CCK, PW_LMT_PH_1T,14, 36,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,1, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,1, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,1, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,1, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,2, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,2, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,2, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,2, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,3, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,3, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,3, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,3, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,4, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,4, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,4, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,4, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,5, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,5, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,5, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,5, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,6, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,6, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,6, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,6, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,7, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,7, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,7, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,7, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,8, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,8, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,8, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,8, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,9, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,9, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,9, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,9, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,10, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,10, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,10, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,10, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,11, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,11, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,11, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,11, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,12, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,12, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,12, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,12, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,13, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,13, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,13, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,13, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,14, 34,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,14, 34,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,14, 34,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_OFDM, PW_LMT_PH_1T,14, 34,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,1, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,1, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,1, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,1, 32,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,2, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,2, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,2, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,2, 32,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,3, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,3, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,3, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,3, 32,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,4, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,4, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,4, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,4, 32,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,5, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,5, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,5, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,5, 32,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,6, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,6, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,6, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,6, 32,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,7, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,7, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,7, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,7, 32,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,8, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,8, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,8, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,8, 32,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,9, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,9, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,9, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,9, 32,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,10, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,10, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,10, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,10, 32,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,11, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,11, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,11, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,11, 32,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,12, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,12, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,12, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,12, 32,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,13, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,13, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,13, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,13, 32,
    PW_LMT_REGU_FCC, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,14, 32,
    PW_LMT_REGU_ETSI, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,14, 32,
    PW_LMT_REGU_MKK, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,14, 32,
    PW_LMT_REGU_EXT, PW_LMT_BAND_2_4G, PW_LMT_BW_20M, PW_LMT_RS_HT, PW_LMT_PH_1T,14, 32,
};


uint32 bk_get_txpwr_lmt_tbl_size(void)
{
    return sizeof(txpwr_lmt_tbl);
}

uint32 bk_get_txpwr_lmt_tbl_entry_size(void)
{
    return TXPWR_LMT_ENTRY_SIZE;
}
uint32 bk_get_txpwr_lmt_tbl_entry_power(uint32 chan_idx,uint32 regulation, uint32 ratesect)
{
    return txpwr_lmt_tbl[ratesect* 14* TXPWR_LMT_REGULATION_NUM*TXPWR_LMT_ENTRY_SIZE+ (chan_idx-1)*TXPWR_LMT_REGULATION_NUM*TXPWR_LMT_ENTRY_SIZE + (regulation-PW_LMT_REGU_FCC)*TXPWR_LMT_ENTRY_SIZE + (TXPWR_LMT_ENTRY_SIZE-1)];
}

uint32 bk_get_country_regulation_table_size(void)
{
    return sizeof(country_regulation_table)/sizeof(Countryregulations);
}


int bk_wifi_set_pwr_limit(wifi_tx_pwr_lmt_t *tx_pwr_lmt)
{

    uint32_t i=0;
    uint32_t arraylen = sizeof(txpwr_lmt_tbl);
    uint8 *array = txpwr_lmt_tbl;
    uint8_t reg,rs,chnl,flag =0;
    wifi_regulation_t regulation = tx_pwr_lmt->regulation;
    wifi_ratesection_t ratesection = tx_pwr_lmt->ratesection;
    uint8 channel = tx_pwr_lmt->channel;
    uint8_t value = tx_pwr_lmt->value;

    for(i=0; i< arraylen; i+=TXPWR_LMT_ENTRY_SIZE)
    {
        reg = array[i];
        rs = array[i+3];
        chnl = array[i+5];
        if((reg == regulation)&&(rs == ratesection)&&(chnl ==channel))
        {
            array[i+6] = value;
        }
        flag = 1;
    }
    if(!flag)
    	return -1;
    return 0;

}



