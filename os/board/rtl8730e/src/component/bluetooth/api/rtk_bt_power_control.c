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

/* Power management definitions */
#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>
static void amebasmart_ble_pmnotify(FAR struct pm_callback_s *cb,
										enum pm_state_e pmstate);
static int  amebasmart_ble_pmprepare(FAR struct pm_callback_s *cb,
										enum pm_state_e pmstate);
#endif

rtk_bt_ps_callback rtk_bt_suspend_callback = NULL;
rtk_bt_ps_callback rtk_bt_resume_callback = NULL;

static void bt_power_suspend_callback(void)
{
	pmvdbg("[BT_PS] BT is entering sleep wake now\r\n");
}

static void bt_power_resume_callback(void)
{
	pmvdbg("[BT_PS] BT is wake now\r\n");
}

extern int wifi_set_ips_internal(u8 enable);
static void system_enable_power_save(void)
{
	/* Release other wake lock if needed */
	wifi_set_lps_enable(TRUE);
	wifi_set_ips_internal(TRUE);
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

	pmvdbg("[BT_PS] Enter rtk_bt_suspend\r\n");

#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined(CONFIG_BT_SINGLE_CORE) && CONFIG_BT_SINGLE_CORE
	hci_platform_force_uart_rts(true);
#endif
#endif

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

	pmvdbg("[BT_PS] Enter rtk_bt_resume\r\n");

	rtk_bt_disable_bt_wake_host();

	if (rtk_bt_resume_callback) {
		rtk_bt_resume_callback();
	}

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

#ifdef CONFIG_PM
static struct
{
	struct pm_callback_s pm_cb;
} g_blepm =
	{
		.pm_cb.name = "rtl8730e_ble",
		.pm_cb.notify  = amebasmart_ble_pmnotify,
		.pm_cb.prepare = amebasmart_ble_pmprepare,
	};
#endif

/****************************************************************************
 * Name: up_pm_notify
 *
 * Description:
 *   Notify the driver of new power state. This callback is called after
 *   all drivers have had the opportunity to prepare for the new power state.
 *
 * Input Parameters:
 *
 *    cb - Returned to the driver. The driver version of the callback
 *         structure may include additional, driver-specific state data at
 *         the end of the structure.
 *
 *    pmstate - Identifies the new PM state
 *
 * Returned Value:
 *   None - The driver already agreed to transition to the low power
 *   consumption state when it returned OK to the prepare() call.
 *
 ****************************************************************************/
#ifdef CONFIG_PM
static void amebasmart_ble_pmnotify(struct pm_callback_s *cb,
				enum pm_state_e pmstate)
{
	switch (pmstate) {
	case PM_NORMAL:		/* Logic for PM_NORMAL goes here */
	case PM_IDLE:		/* Logic for PM_IDLE goes here */
	case PM_STANDBY:	/* Logic for PM_STANDBY goes here */
		break;

	case PM_SLEEP:		/* Logic for PM_SLEEP goes here */
        system_enable_power_save();
		break;

	default:
		/* Should not get here */
		break;
	}
}
#endif

/****************************************************************************
 * Name: up_pm_prepare
 *
 * Description:
 *   Request the driver to prepare for a new power state. This is a warning
 *   that the system is about to enter into a new power state. The driver
 *   should begin whatever operations that may be required to enter power
 *   state. The driver may abort the state change mode by returning a
 *   non-zero value from the callback function.
 *
 * Input Parameters:
 *
 *    cb - Returned to the driver. The driver version of the callback
 *         structure may include additional, driver-specific state data at
 *         the end of the structure.
 *
 *    pmstate - Identifies the new PM state
 *
 * Returned Value:
 *   Zero - (OK) means the event was successfully processed and that the
 *          driver is prepared for the PM state change.
 *
 *   Non-zero - means that the driver is not prepared to perform the tasks
 *              needed achieve this power setting and will cause the state
 *              change to be aborted. NOTE: The prepare() method will also
 *              be called when reverting from lower back to higher power
 *              consumption modes (say because another driver refused a
 *              lower power state change). Drivers are not permitted to
 *              return non-zero values when reverting back to higher power
 *              consumption modes!
 *
 ****************************************************************************/
#ifdef CONFIG_PM
extern uint8_t ble_client_connect_is_running;
static int amebasmart_ble_pmprepare(struct pm_callback_s *cb,
		enum pm_state_e pmstate)
{
	switch (pmstate) {
	case PM_NORMAL:		/* Logic for PM_NORMAL goes here */
	case PM_IDLE:		/* Logic for PM_IDLE goes here */
	case PM_STANDBY:	/* Logic for PM_STANDBY goes here */
		break;

	case PM_SLEEP:		/* Logic for PM_SLEEP goes here */
		if(ble_client_connect_is_running)
			return ERROR;
		break;

	default:
		/* Should not get here */
		break;
	}
	return OK;
}
#endif /* CONFIG_PM */

void rtk_bt_power_save_init(void)
{
#ifdef CONFIG_PM	/* If PM is enable inti BT power save */
	int result;
	rtk_bt_suspend_callback = bt_power_suspend_callback;
	rtk_bt_resume_callback = bt_power_resume_callback;

	/* register callback before entering ps mode and after exiting ps mode */
	pmu_register_sleep_callback(PMU_BT_DEVICE, (PSM_HOOK_FUN)rtk_bt_suspend, NULL, (PSM_HOOK_FUN)rtk_bt_resume, NULL);

	InterruptRegister((IRQ_FUN)rtk_bt_wake_host_irq_handler, BT_WAKE_HOST_IRQ, (int)NULL, INT_PRI_LOWEST);

	/* Register to receive power management callbacks */
	result = pm_register(&g_blepm.pm_cb);
	DEBUGASSERT(result == OK);
	UNUSED(result);
#endif
}

void rtk_bt_power_save_deinit(void)
{
#ifdef CONFIG_PM
	InterruptDis(BT_WAKE_HOST_IRQ);
	InterruptUnRegister(BT_WAKE_HOST_IRQ);

	pmu_unregister_sleep_callback(PMU_BT_DEVICE);

	rtk_bt_suspend_callback = NULL;
	rtk_bt_resume_callback = NULL;

	/* Unregister power management callbacks */
	pm_unregister(&g_blepm.pm_cb);
#endif
}
#endif