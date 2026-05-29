/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __OS_WRAPPER_CRITCAL_H__
#define __OS_WRAPPER_CRITCAL_H__

/**
 * @brief  Define core numbers settings. This macro will be checked during compilation to ensure
 * that it is consistent with the configuration of the operating system.
 */
#if defined CONFIG_CPUS_NUM && (CONFIG_CPUS_NUM == 2)
#define RTOS_NUM_CORES 2
#else
#define RTOS_NUM_CORES 1
#endif

/**
 * Define CA32 SMP spin lock id
*/
typedef enum {
	RTOS_CRITICAL_DEFAULT = 0,
	RTOS_CRITICAL_SOC,
	RTOS_CRITICAL_AUDIO,
	RTOS_CRITICAL_WIFI,
	RTOS_CRITICAL_NETWORK,
	RTOS_CRITICAL_LWIP,
	RTOS_CRITICAL_BT,
	RTOS_CRITICAL_USB,
	RTOS_CRITICAL_WPAN,
	RTOS_CRITICAL_SEMA,
	RTOS_CRITICAL_LOG,
	RTOS_CRITICAL_MAX
} RTOS_CRITICAL_LIST;

/**
 * @brief  Check if in task interrupt
 * @retval 1: interrupt; 0: context
 */
int rtos_critical_is_in_interrupt(void);

/**
 * @brief  Internally handles interrupt status (PRIMASK/CPSR) save.
 * For Smart CA32-smp: Different components do not share the same spin_lock. They also do not share the same spin_lock with the system.
 * For other kernels: Different components use the same lock with the system.
 */
void rtos_critical_enter(uint32_t component_id);

/**
 * @brief  Internally handles interrupt status(PRIMASK/CPSR) restore
 * For Smart CA32-smp: Different components do not share the same spin_lock. They also do not share the same spin_lock with the system.
 * For other kernels: Different components use the same lock with the system.
 */
void rtos_critical_exit(uint32_t component_id);

/**
 * @brief  For OS wrapper internal use. It is generally not recommended to use this API
 * For all cores: use the same lock with the system.
 */
void __rtos_critical_enter_os(void);

/**
 * @brief  For OS wrapper internal use. It is generally not recommended to use this API
 * For all cores: use the same lock with the system.
 */
void __rtos_critical_exit_os(void);

/**
 * @brief  get task enter critical state
 * @retval >0: in critical state; 0: exit critical state
 */
uint32_t rtos_get_critical_state(void);

#if defined(CONFIG_PLATFORM_TIZENRT_OS)
irqstate_t tizenrt_critical_enter(void);
void tizenrt_critical_exit(irqstate_t flags);
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS)

#endif
