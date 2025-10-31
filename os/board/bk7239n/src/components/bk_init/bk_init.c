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

#include <components/log.h>
#include <modules/wifi.h>
#include <components/netif.h>
#include <components/event.h>
#include <driver/uart.h>
#include <string.h>
#include "boot.h"
#include <modules/pm.h>
#include "aon_pmu_driver.h"
#include <driver/pwr_clk.h>
#include "bk_pm_internal_api.h"
#if defined(CONFIG_ROSC_CALIB_SW)
#include <driver/rosc_32k.h>
#endif
#if defined(CONFIG_ARMINO_BLE)
#include "components/bluetooth/bk_dm_bluetooth.h"
#include "components/bluetooth/bk_ble.h"
#include "ble_api_5_x.h"

#endif
#include <bk_wifi_adapter.h>
#include <bk_phy_adapter.h>
#include <bk_rf_adapter.h>
#include <bk_rf_internal.h>
#if (defined(CONFIG_PSRAM))
#include <driver/psram.h>
#endif

#if defined(CONFIG_USB)
#include <components/usb.h>
#endif

#if (defined(CONFIG_TEMP_DETECT) || defined(CONFIG_VOLT_DETECT))
#include <components/sensor.h>
#endif

#if (defined(CONFIG_OTA_UPDATE_DEFAULT_PARTITION) && defined(CONFIG_HTTP_AB_PARTITION))
#include <modules/ota.h>
extern void bk_ota_confirm_update_partition(ota_confirm_flag ota_confirm_val);
#endif


#if defined(CONFIG_SHELL_ASYNCLOG)
#include "bk_api_cli.h"

#endif

#if defined(CONFIG_WIFI_AT_ENABLE) && defined(CONFIG_WIFI_ENABLE)
#include "wifi_at.h"
#endif
#if defined(CONFIG_BT_AT_ENABLE) && defined(CONFIG_BT)
#include "bk_at_bluetooth.h"
#endif
#if defined(CONFIG_NETIF_AT_ENABLE) && defined(CONFIG_WIFI_ENABLE)
#include "bk_at_netif.h"


#endif
#if (defined(CONFIG_NTP_SYNC_RTC))
#include <components/ate.h>
#include <components/app_time_intf.h>
#endif

#define TAG "bk_init"

#ifdef CONFIG_VND_CAL
#include "vnd_cal.h"
#endif

#if defined(CONFIG_BUTTON)
#include "key_main.h"
#endif

#if (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#include "sys_ll.h"
#include "bk_saradc.h"
#endif

#if defined(CONFIG_WIFI_PS_DISABLE)
#include "modules/wifi.h"
#endif

#if (defined(CONFIG_SDIO_V2P0) && defined(CONFIG_SDIO_SLAVE))
#include "sdio_slave_driver.h"
#if defined(CONFIG_SDIO_TEST_EN)
#include "sdio_test.h"
#endif
#endif

#if defined(CONFIG_AT)
#include "atsvr_unite.h"
#endif

#if (defined(CONFIG_DEBUG_FIRMWARE))
extern bk_err_t bk_dbg_init(void);
#endif

#if (defined(CONFIG_SYS_CPU0))
extern void start_cpu1_core(void);
#endif

void rtos_user_app_launch_over(void);

extern const uint8_t build_version[]; // = __DATE__ " " __TIME__;

int app_phy_init(void)
{
#if ((defined(CONFIG_WIFI_ENABLE) || defined(CONFIG_ARMINO_BLE)) && defined(CONFIG_SYS_CPU0))
    bk_phy_adapter_init();
    bk_rf_adapter_init();
#endif
    return BK_OK;
}

static int app_wifi_init(void)
{
#if (defined(CONFIG_WIFI_ENABLE))
	wifi_init_config_t wifi_config = WIFI_DEFAULT_INIT_CONFIG();
	BK_LOG_ON_ERR(bk_event_init());
	BK_LOG_ON_ERR(bk_netif_init());
	BK_LOG_ON_ERR(bk_wifi_init(&wifi_config));

#if (defined(CONFIG_DEBUG_FIRMWARE))
	BK_LOG_ON_ERR(bk_dbg_init());
#endif

#if defined(CONFIG_WIFI_PS_DISABLE)
	//disable ps if needed
	bk_wifi_sta_pm_disable();
#endif
#if defined(CONFIG_WIFI_AT_ENABLE)
	wifi_at_cmd_init();
#endif
#if defined(CONFIG_NETIF_AT_ENABLE)
	netif_at_cmd_init();
#endif

#endif
	return BK_OK;
}

static int app_ble_init(void)
{
#if defined(CONFIG_ARMINO_BLE)
    BK_LOG_ON_ERR(bk_bluetooth_init());
#endif
	return BK_OK;
}

static int app_bt_init(void)
{
#if (defined(CONFIG_BT))
	BK_LOGI(TAG, "BT active\r\n");
#if 0//TODO
	if (!ate_is_enabled())
		bt_activate(NULL);
#endif
#if (defined(CONFIG_BT_AT_ENABLE))
	bt_at_cmd_init();
#endif

#endif
	return BK_OK;
}

static int app_key_init(void)
{
#if defined(CONFIG_BUTTON)
	key_initialization();
#endif
	return BK_OK;
}

static int app_mp3_player_init(void)
{
#if (defined(CONFIG_MP3PLAYER))
	key_init();
	media_thread_init();
#endif
	return BK_OK;
}

int app_sdio_init(void)
{
#if (defined(CONFIG_SDIO_V2P0) && defined(CONFIG_SDIO_SLAVE))
	bk_sdio_slave_driver_init();
#if defined(CONFIG_SDIO_TEST_EN)
	bk_sdio_test_init();
#endif
#endif
	return BK_OK;
}

int app_usb_init(void)
{
#if defined(CONFIG_USB)
	BK_LOGI(TAG, "fusb init\r\n");
#if 0//TODO
	if (!ate_is_enabled())
		fusb_init();
#endif
#endif
	return BK_OK;
}

static int app_cli_init(void)
{
#if (defined(CONFIG_CLI))
#if !defined(CONFIG_FULLY_HOSTED)
	bk_cli_init();
#endif
#else
#if defined(CONFIG_SHELL_ASYNCLOG)
	bk_cli_init();
#endif
#endif
	return BK_OK;
}

static int app_usb_charge_init(void)
{
#if defined(CONFIG_USB_CHARGE)
	extern void usb_plug_func_open(void);
	usb_plug_func_open();
#endif
	return BK_OK;
}

static int app_uart_debug_init_todo(void)
{
#if defined(CONFIG_UART_DEBUG)
#ifndef KEIL_SIMULATOR
	BK_LOGI(TAG, "uart debug init\r\n");
	uart_debug_init();
#endif
#endif
	return BK_OK;
}

#if defined(CONFIG_ETH)
extern int net_eth_start();
static int app_eth_init(void)
{
	BK_LOGI(TAG, "ETH init\n");
	net_eth_start();
	return BK_OK;
}
#endif

#if defined(CONFIG_ENABLE_WIFI_DEFAULT_CONNECT)
extern void demo_wifi_fast_connect(void);
#endif

int bk_init(void)
{
	STARTUP_PERF(17);
	BK_LOGI(TAG, "armino app init: %s\n", build_version);

#ifdef APP_VERSION
	BK_LOGI(TAG, "APP Version: %s\n", APP_VERSION);
#endif

#if defined(CONFIG_SYS_CPU0)
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT,PM_CPU_FRQ_240M);
#endif

#ifdef CONFIG_VND_CAL
	vnd_cal_overlay();
#endif

	bk_pm_mailbox_init();
#if defined(CONFIG_AT)
	at_server_init();
#endif
#if (defined(CONFIG_SYS_CPU0))

#if defined(CONFIG_SDIO)
	app_sdio_init();
#endif
	app_key_init();
	app_usb_charge_init();

#if defined(CONFIG_SAVE_BOOT_TIME_POINT)
	save_mtime_point(CPU_START_WIFI_INIT_TIME);
#endif

#if defined(CONFIG_ATE_TEST)
	/*not init the wifi, in order to save the boot time in ATE test after deepsleep(note:at the wifi ate test not enter power save)*/
	/*it need first finish test the wifi, at the end test deepsleep, wait wakeup(deepsleep), then test low voltage */
	if(!(aon_pmu_drv_reg_get(PMU_REG2)&BIT(BIT_SLEEP_FLAG_DEEP_SLEEP)))
	{
		app_wifi_init();
#if (defined(CONFIG_ARMINO_BLE))
		app_bt_init();
		app_ble_init();
#endif
	}
#else //!CONFIG_ATE_TEST
#if defined(CONFIG_WIFI_ENABLE)
    app_wifi_init();
#else
#if defined(CONFIG_ARMINO_BLE)
	#if (defined(CONFIG_SOC_BK7256XX) || defined(CONFIG_SOC_BK7236XX) || defined(CONFIG_SOC_BK7239XX))
    extern int bk_cal_if_init(void);
    bk_cal_if_init();
	#endif
#endif
#endif

#if (defined(CONFIG_ARMINO_BLE))
	app_bt_init();
	app_ble_init();
#endif

#endif //CONFIG_ATE_TEST

#if defined(CONFIG_ETH)
	app_eth_init();
#endif

#if defined(CONFIG_SAVE_BOOT_TIME_POINT)
	save_mtime_point(CPU_FINISH_WIFI_INIT_TIME);
#endif

#if defined(CONFIG_ROSC_CALIB_SW)
	bk_rosc_32k_calib();
#endif

#if (defined(CONFIG_TEMP_DETECT) || defined(CONFIG_VOLT_DETECT))
	bk_sensor_start();
#endif

	rtos_user_app_launch_over();

	app_mp3_player_init();
	app_uart_debug_init_todo();

#else //!(CONFIG_SYS_CPU1)


#endif //!(CONFIG_SYS_CPU1)

	app_cli_init();
#if defined(CONFIG_AT)
	extern int atsvr_app_init(void);
	if(0 != atsvr_app_init())
		return -1;
#endif


#if (defined(CONFIG_SOC_BK7258) && defined(CONFIG_SYS_CPU0))
	//bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU1, PM_POWER_MODULE_STATE_ON);
	//start_cpu1_core();
#endif


#if (defined(CONFIG_NTP_SYNC_RTC))
    if(ate_is_enabled() == 0)
    {
	    app_time_rtc_ntp_sync_init();
    }
#endif


#if (defined(CONFIG_FREERTOS))
#if defined(CONFIG_SEMI_HOSTED)
	semi_host_init();
#endif
#endif

#if defined(CONFIG_UDISK_MP3)
	um_init();
#endif

#if defined(CONFIG_ENABLE_WIFI_DEFAULT_CONNECT)
	demo_wifi_fast_connect();
#endif

#if (defined(CONFIG_OTA_UPDATE_DEFAULT_PARTITION)&& defined(CONFIG_HTTP_AB_PARTITION))
#if (defined(CONFIG_OTA_POSITION_INDEPENDENT_AB))
	bk_ota_double_check_for_execution();
#else
#ifdef CONFIG_OTA_UPDATE_B_PARTITION
	os_printf("exec part a\r\n");
	bk_ota_confirm_update_partition(CONFIRM_EXEC_A);
#else
	os_printf("exec part b\r\n");
	bk_ota_confirm_update_partition(CONFIRM_EXEC_B);
#endif
#endif
#endif

#if defined(CONFIG_MICROPYTHON)
extern int mp_do_startup(int heap_len);
	mp_do_startup(0);
#endif

#if defined(CONFIG_SYS_CPU0)
#if defined(CONFIG_CPU_DEFAULT_FREQ_60M)
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT,PM_CPU_FRQ_60M);
#endif
#if defined(CONFIG_PM_LV_WDT_PROTECTION)
	pm_wifi_event_init();
#endif
#endif
#if defined(CONFIG_SYS_CPU1)
#if defined(CONFIG_PSRAM)
	REG_READ(SOC_PSRAM_DATA_BASE);//check psram whether valid
#endif
	bk_pm_cp1_boot_ok_response_set();
#endif
#if defined(CONFIG_USB) //&& defined(CONFIG_MENTOR_USB)
	bk_usb_driver_init();
#endif

#if (defined(CONFIG_PSRAM))
	bk_psram_id_auto_detect();
#endif
	STARTUP_PERF(14);

	return 0;
}


void bk_phy_init(void)
{
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_WIFIP_MAC, PM_POWER_MODULE_STATE_ON);
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_PHY, PM_POWER_MODULE_STATE_ON);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_PHY_WIFI,PM_POWER_MODULE_STATE_ON);

	//enable mac and phy clock
	bk_pm_clock_ctrl(PM_CLK_ID_MAC, PM_CLK_CTRL_PWR_UP);
	bk_pm_clock_ctrl(PM_CLK_ID_PHY, PM_CLK_CTRL_PWR_UP);

	/* Wi-Fi VOTE RF */
	rf_module_vote_ctrl(RF_OPEN, RF_BY_WIFI_BIT);

#ifdef CONFIG_VND_CAL

	vnd_cal_overlay();
#endif

#if CONFIG_WIFI_ENABLE
	extern int bk_cal_if_init(void);
	bk_cal_if_init();
#endif

#if (CONFIG_TEMP_DETECT || CONFIG_VOLT_DETECT)
	bk_sensor_start();
#endif

    wifi_init_config_t init_config = WIFI_DEFAULT_INIT_CONFIG();
    if(BK_OK != bk_wifi_adapter_init(&init_config))
    {
        BK_LOGE("%s,%d,bk_wifi_adapter_init fail!\n",__func__,__LINE__);
    }

    if(ate_is_enabled())
    {
        rf_pll_ctrl(CMD_RF_WIFIPLL_HOLD_BIT_SET, RF_WIFIPLL_HOLD_BY_WIFI_BIT);
        rf_module_vote_ctrl(RF_OPEN, RF_BY_WIFI_BIT);
        bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_PHY_WIFI, PM_POWER_MODULE_STATE_ON);
        bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_WIFIP_MAC, PM_POWER_MODULE_STATE_ON);
    }
    else
    {
        rf_pll_ctrl(CMD_RF_WIFIPLL_HOLD_BIT_CLR,RF_WIFIPLL_HOLD_BY_WIFI_BIT);
        rf_module_vote_ctrl(RF_CLOSE, RF_BY_WIFI_BIT);
        bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_PHY_WIFI, PM_POWER_MODULE_STATE_OFF);
        bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_WIFIP_MAC, PM_POWER_MODULE_STATE_OFF);
    }
}