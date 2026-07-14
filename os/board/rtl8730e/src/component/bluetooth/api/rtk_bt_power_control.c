/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include "platform_autoconf.h"
#include <bt_api_config.h>
#include "bt_debug.h"
#if defined(RTK_BT_POWER_CONTROL_SUPPORT) && RTK_BT_POWER_CONTROL_SUPPORT
#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <basic_types.h>
#include "rtk_bt_power_control.h"
#include "ameba_soc.h"
#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>
#include "ameba_tizenrt_pmu.h"
#endif //#ifdef CONFIG_PM

/* This file has been modified for TizenRT only RTK related code has been removed */

rtk_bt_ps_callback rtk_bt_suspend_callback = NULL;
rtk_bt_ps_callback rtk_bt_resume_callback = NULL;

#ifdef CONFIG_PLATFORM_TIZENRT_OS
#ifdef CONFIG_PM
struct pm_domain_s *rtk_ble_domain = NULL;

void rtk_tizenrt_bt_pm_suspend(unsigned int milliseconds)
{
	if (rtk_ble_domain == NULL) {
		BT_LOGE("BLE PM DOMAIN not init \r\n");
		return;
	}

	if (pm_timedsuspend(rtk_ble_domain, milliseconds) != 0) {
		BT_LOGE("Unable to perform PM suspend for %dms for BLE domain \r\n", milliseconds);
	}
}

void rtk_tizenrt_bt_pm_resume(void)
{
	if (rtk_ble_domain == NULL) {
		BT_LOGE("BLE PM DOMAIN not init \r\n");
		return;
	}

	if (pm_resume(rtk_ble_domain) != 0) {
		BT_LOGE("Unable to perform PM resume for BLE domain  \r\n");
	}
}

void rtk_tizenrt_bt_pm_init(void)
{
	/* Register PM_BLE_DOMAIN and Perform 10 minutes timedsuspend */
	rtk_ble_domain = pm_domain_register("BLE");
	if (rtk_ble_domain == NULL) {
		BT_LOGA("Unable to register BLE PM DOMAIN \r\n");
	} else {
		BT_LOGA("Registered BLE PM DOMAIN \r\n");
	}
}

void rtk_tizenrt_bt_pm_deinit(void)
{
	if (rtk_ble_domain == NULL) {
		BT_LOGE("BLE PM DOMAIN not init \r\n");
		return;
	}

	if (pm_domain_unregister(rtk_ble_domain)) {
		rtk_ble_domain = NULL;
		BT_LOGA("Unable to unregister BLE PM DOMAIN \r\n");
	} else {
		BT_LOGA("Unregister BLE PM DOMAIN \r\n");
	}
}
#endif //#ifdef CONFIG_PM
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

_WEAK void hci_platform_force_uart_rts(bool op)
{
	(void) op;
}

static bool rtk_bt_get_wakelock_status(void)
{
#ifdef CONFIG_PM
	uint32_t lock_status = pmu_get_wakelock_status();
	BT_LOGD("[BT_PS] lock_status = 0x%x\r\n", lock_status);

	if (lock_status & ((0x01) << PMU_BT_DEVICE)) {
		return true;    //Already acquire bt wake lock
	} else {
		return false;   //Already release bt wake lock
	}
#endif //#ifdef CONFIG_PM
	return false;       //Already acquire bt wake lock
}

void rtk_bt_release_wakelock(void)
{
#ifdef CONFIG_PM
	if (rtk_bt_get_wakelock_status() == true) {
		BT_LOGD("[BT_PS] pmu_release_wakelock PMU_BT_DEVICE\r\n");
		pmu_release_wakelock(PMU_BT_DEVICE);
	} else {
		BT_LOGD("[BT_PS] already release PMU_BT_DEVICE\r\n");
	}
#endif //#ifdef CONFIG_PM
}

void rtk_bt_acquire_wakelock(void)
{
#ifdef CONFIG_PM
	if (rtk_bt_get_wakelock_status() == false) {
		BT_LOGD("[BT_PS] pmu_acquire_wakelock PMU_BT_DEVICE\r\n");
		pmu_acquire_wakelock(PMU_BT_DEVICE);
	} else {
		BT_LOGD("[BT_PS] already acquire PMU_BT_DEVICE\r\n");
	}
#endif //#ifdef CONFIG_PM
}

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

	BT_LOGD("[BT_PS] Enter rtk_bt_suspend\r\n");

	hci_platform_force_uart_rts(true);

	rtk_bt_enable_bt_wake_host();

	if (rtk_bt_suspend_callback) {
		rtk_bt_suspend_callback();
	}

	return 1;
}

static uint32_t rtk_bt_resume(uint32_t expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;

	BT_LOGD("[BT_PS] Enter rtk_bt_resume\r\n");

	rtk_bt_acquire_wakelock();

	rtk_bt_disable_bt_wake_host();

	if (rtk_bt_resume_callback) {
		rtk_bt_resume_callback();
	}

	hci_platform_force_uart_rts(false);

	return 1;
}

static uint32_t rtk_bt_wake_host_irq_handler(void *data)
{
	(void)data;

	return 1;
}

void rtk_bt_power_save_init(rtk_bt_ps_callback p_suspend_callback, rtk_bt_ps_callback p_resume_callback)
{
#ifdef CONFIG_PM
	rtk_bt_suspend_callback = p_suspend_callback;
	rtk_bt_resume_callback = p_resume_callback;
	/* register callback before entering ps mode and after exiting ps mode */
	pmu_register_sleep_callback(PMU_BT_DEVICE, (PSM_HOOK_FUN)rtk_bt_suspend, NULL, (PSM_HOOK_FUN)rtk_bt_resume, NULL);

	InterruptRegister((IRQ_FUN)rtk_bt_wake_host_irq_handler, BT_WAKE_HOST_IRQ, (u32)NULL, INT_PRI_LOWEST);
#endif //#ifdef CONFIG_PM
}

void rtk_bt_power_save_deinit(void)
{
#ifdef CONFIG_PM
	InterruptDis(BT_WAKE_HOST_IRQ);
	InterruptUnRegister(BT_WAKE_HOST_IRQ);
	pmu_unregister_sleep_callback(PMU_BT_DEVICE);
	rtk_bt_suspend_callback = NULL;
	rtk_bt_resume_callback = NULL;
#endif //#ifdef CONFIG_PM
}
#endif
