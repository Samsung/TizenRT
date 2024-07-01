/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */
#include <bt_api_config.h>
#if defined(RTK_BT_POWER_CONTROL_SUPPORT) && RTK_BT_POWER_CONTROL_SUPPORT
#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <basic_types.h>
#include <platform_opts_bt.h>
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
#include <bt_ipc_host_api.h>
#include <bt_ipc_profile_config.h>
#endif
#include "rtk_bt_power_control.h"
#include "ameba_soc.h"
#include "wifi_conf.h"

static void rtk_bt_enable_bt_wake_host(void)
{
	if (irq_get_pending(BT_WAKE_HOST_IRQ)) {
		irq_clear_pending(BT_WAKE_HOST_IRQ);
	}

	/* enable BT_WAKE_HOST interrupt */
	InterruptEn(BT_WAKE_HOST_IRQ, INT_PRI_LOWEST);
}

static void rtk_bt_disable_bt_wake_host(void)
{
	/* disable BT_WAKE_HOST interrupt */
	InterruptDis(BT_WAKE_HOST_IRQ);

	if (irq_get_pending(BT_WAKE_HOST_IRQ)) {
		irq_clear_pending(BT_WAKE_HOST_IRQ);
	}
}

static uint32_t rtk_bt_suspend(uint32_t expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;

	pmvdbg("[BT_PS] Enter rtk_bt_suspend\r\n");

#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined(CONFIG_BT_SINGLE_CORE) && CONFIG_BT_SINGLE_CORE
	hci_platform_force_uart_rts(true);
#endif
#endif
	rtk_bt_enable_bt_wake_host();

	return 1;
}

static uint32_t rtk_bt_resume(uint32_t expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;

	pmvdbg("[BT_PS] Enter rtk_bt_resume\r\n");

	rtk_bt_disable_bt_wake_host();

#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined(CONFIG_BT_SINGLE_CORE) && CONFIG_BT_SINGLE_CORE
	hci_platform_force_uart_rts(false);
#endif
#endif

	return 1;
}

static uint32_t rtk_bt_wake_host_irq_handler(void *data)
{
	(void)data;

	return 1;
}

void rtk_bt_power_save_init(void)
{
#ifdef CONFIG_PM	/* If PM is enable inti BT power save */
	/* register callback before entering ps mode and after exiting ps mode */
	pmu_register_sleep_callback(PMU_BT_DEVICE, (PSM_HOOK_FUN)rtk_bt_suspend, NULL, (PSM_HOOK_FUN)rtk_bt_resume, NULL);
	InterruptRegister((IRQ_FUN)rtk_bt_wake_host_irq_handler, BT_WAKE_HOST_IRQ, (int)NULL, INT_PRI_LOWEST);
#endif
}

void rtk_bt_power_save_deinit(void)
{
#ifdef CONFIG_PM
	InterruptDis(BT_WAKE_HOST_IRQ);
	InterruptUnRegister(BT_WAKE_HOST_IRQ);
	pmu_unregister_sleep_callback(PMU_BT_DEVICE);
#endif
}
#endif
