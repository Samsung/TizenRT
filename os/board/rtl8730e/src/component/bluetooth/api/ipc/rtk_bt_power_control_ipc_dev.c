/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <string.h>
#include <osif.h>
#include <bt_ipc_host_api.h>
#include <bt_ipc_dev_api.h>
#include "ameba_soc.h"
#include "wifi_conf.h"
#include "ameba_freertos_pmu.h"

uint32_t bt_wake_host_np_irq_param = 0;

/**
* @brief  callback invoked before system entering power save mode.
* @param  none.
* @return none.
*/
static uint32_t bt_suspend_np(unsigned int expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;

	printf("[BT_PS]%s: Enter \r\n", __func__);

	return 1;
}

void *ps_timer_hdl = NULL;

/**
* @brief  callback invoked after system waking from power save mode.
* @param  none.
* @return none.
*/
static uint32_t bt_resume_np(unsigned int expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;
	
	pmu_acquire_wakelock(PMU_BT_DEVICE);
	printf("[BT_PS]%s: Enter \r\n", __func__);

	return 1;
}

// /**
// * @brief  irq handle for BT_WAKE_HOST_IRQ.
// * @param  none.
// * @return none.
// */
// void bt_wake_host_np_irq_handle(uint32_t data)
// {
// 	(void)data;

// 	printf(" %s Enter \r\n", __func__);
// }

/**
* @brief  get np power save status.
* @param  none
* @return 1: Active.
*         0: Sleep enable.
*/
static bool amebalite_np_bt_get_host_power_save_status(void)
{
	uint32_t lock_status;

	lock_status = pmu_get_wakelock_status();
	printf("[BT_PS]%s: lock_status 0x%lx \r\n", __func__, lock_status);
	if (lock_status & ((0x01) << PMU_BT_DEVICE)) {
		return _TRUE;
	} else {
		return _FALSE;
	}
}

/**
* @brief  enable bt power save.
* @param  none
* @return 1: success.
*         0: fail.
*/
static uint8_t amebalite_np_bt_power_save_enable(void)
{
	if (!amebalite_np_bt_get_host_power_save_status()) {
		printf("[BT_PS]%s: Already enabled bt power save \r\n", __func__);
		return 1;
	}
	printf("[BT_PS]%s: pmu_release_wakelock \r\n", __func__);
	pmu_release_wakelock(PMU_BT_DEVICE);

	return 1;
}

/**
* @brief  enable bt power save.
* @param  none
* @return 1: success.
*         0: fail.
*/
static uint32_t amebalite_bt_ps_init(void)
{
	/* register callback before entering ps and after exiting ps */
	pmu_register_sleep_callback(PMU_BT_DEVICE, (PSM_HOOK_FUN)bt_suspend_np, NULL, (PSM_HOOK_FUN)bt_resume_np, NULL);
// #if defined(ARM_CORE_CM4) && ARM_CORE_CM4
// 	InterruptRegister(bt_wake_host_np_irq_handle, BT_WAKE_HOST_IRQ, &bt_wake_host_np_irq_param, 10);
// 	/* enable BT_WAKE_HOST interrupt */
// 	InterruptEn(BT_WAKE_HOST_IRQ, 10);
// #elif defined(RSICV_CORE_KR4) && RSICV_CORE_KR4
// 	InterruptRegister(bt_wake_host_np_irq_handle, BT_WAKE_HOST_IRQ, &bt_wake_host_np_irq_param, 10);
// 	/* enable BT_WAKE_HOST interrupt */
// 	InterruptEn(BT_WAKE_HOST_IRQ, 10);
// #endif

	return 1;
}

/**
* @brief  np bt power control handle.
* @param  none
* @return none
*/
void bt_power_control_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	switch (p_ipc_msg->API_ID) {
	/* np bt ps init */
	case 1:
		p_ipc_msg->ret[0] = amebalite_bt_ps_init();
		break;

	/* np get ps status */
	case 2:
		p_ipc_msg->ret[0] = amebalite_np_bt_get_host_power_save_status();
		break;

	/* np release bt pmu wakelock */
	case 3:
		p_ipc_msg->ret[0] = amebalite_np_bt_power_save_enable();
		wifi_set_lps_enable(TRUE);
		wifi_set_ips_enable(TRUE);
		osif_delay(3000);
		break;

	default:
		break;
	}
	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}
