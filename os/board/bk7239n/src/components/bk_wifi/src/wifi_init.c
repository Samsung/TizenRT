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

#include <common/bk_include.h>
#include <modules/wifi.h>
#include "bk_list.h"
//#include <components/netif.h>
//#include <components/event.h>
#include "wifi_init.h"
#include "wifi_v2.h"
#include "rwnx_intf.h"

#include "bk_wifi_types.h"
#include "bk_wifi.h"
#include "bk_workqueue.h"
#include "modules/pm.h"
#include "bk_rf_internal.h"
//#include "net.h"
//#include "rwnx_rx.h"
#ifdef CONFIG_NET_LWIP_HW_CHKSUM_PORT
#include "lwip/port/hw_chksum_port.h"

/* External hardware checksum function */
extern uint16_t hw_ipcksum_standard_chksum(const void *dataptr, int len);
#endif

//TODO
// 1. BLE also need to consider PS init
// 2. PM init optimization
int pm_init_todo_2(void)
{
#if (!CONFIG_SOC_BK7236A) && (!CONFIG_FULLY_HOSTED)
	// for debug purpuse on bk7236, comment temporarily
	//wifi_ps_switch(g_wifi_vars->_ps_allow, 0, g_wifi_vars->_pm_rf_bit);
#else
	// for debug purpuse on bk7236, add it for not in ate mode
	//rf_ps_enable_clear();
#endif
	return BK_OK;
}


/**
 * Initialize wifi.
 * @config: configure wifi features, such as modes, ht mcs, vht/he mcs, etc.
 *
 * Return: BK_OK if init successful.
 */
extern void wpas_thread_start(void);
extern int calibration_init(void);
extern void rwnxl_init(void);
extern int fhost_rxbuf_push(void);
extern int regulatory_init(void);

int wifi_init(const wifi_init_config_t *config)
{
	int ret;

	/* init workqueue */
	bk_workqueue_init();

	/* power up Wi-Fi MAC and PHY */
	//enable mac and phy power
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_WIFIP_MAC, PM_POWER_MODULE_STATE_ON);
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_PHY, PM_POWER_MODULE_STATE_ON);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_PHY_WIFI,PM_POWER_MODULE_STATE_ON);

	//enable mac and phy clock
	bk_pm_clock_ctrl(PM_CLK_ID_MAC, PM_CLK_CTRL_PWR_UP);
	bk_pm_clock_ctrl(PM_CLK_ID_PHY, PM_CLK_CTRL_PWR_UP);

	/* Wi-Fi VOTE RF */
    rf_module_vote_ctrl(RF_OPEN, RF_BY_WIFI_BIT);

	/*
	 * UMAC/LMAC init.
	 *
	 * Per the test result, rwnxl_init must be called before calibration init,
	 * otherwise, calibration can't work correctly.
	 * TODO: pass config to `rwnxl_init' to configure mac/phy/rf features.
	 */
	rwnxl_init();

	/* RF calibration */
	ret = calibration_init();
	if (ret != BK_OK) {
		WIFI_LOGE("cal init failed, ret(%x)\n", ret);
		return ret;
	}

	/* TODO optimize it, move common param init to other places */
	cfg_param_init();

	pm_init_todo_2();

	/*
	 * start ke_event handler thread named `core_thread', and
	 * start ke_msg recv thread named `kmsgbk''.
	 */
	ret = rwnx_intf_init();
	if (ret != BK_OK) {
		WIFI_LOGE("rwnx init failed, ret(%x)\n", ret);
		return ret;
	}

#if 0//(CONFIG_NO_HOSTED && CONFIG_RF_POLICY_SUPPORT)
	ret = coex_init();
	if (ret != BK_OK) {
		WIFI_LOGE("rf task init failed, ret(%x)\n", ret);
		return ret;
	}
#endif

	/* initial wlan rx buf only once after lwip has been initialized */
	fhost_rxbuf_push(); //TODO choose a better name

#if CONFIG_WIFI_REGDOMAIN
	regulatory_init();
#endif

	/* start wpa_supplicant/hostapd eloop thread */
	wpas_thread_start(); //TODO Choose a better name

	extern void coex_ictw_report_wifi_open_status(bool is_wifi_open);
	coex_ictw_report_wifi_open_status(true);

#if CONFIG_FIXED_NETIF
	//net_wlan_init();
#endif

#ifdef CONFIG_NET_LWIP_HW_CHKSUM_PORT
	/* Register hardware checksum function for lwIP */
	if (lwip_register_hw_chksum(hw_ipcksum_standard_chksum) != 0) {
		WIFI_LOGE("Failed to register hardware checksum function\n");
		/* Continue even if registration fails, will use software checksum */
	}
#endif

	return BK_OK;
}

//TODO
// 1. Free all WiFi resource
// 2. Delete all WiFi related task gracefully
int wifi_deinit(void)
{
	return BK_ERR_NOT_SUPPORT;
}

