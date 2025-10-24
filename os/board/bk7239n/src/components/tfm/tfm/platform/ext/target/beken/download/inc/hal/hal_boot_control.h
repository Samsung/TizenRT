// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_BOOT_CONTROL_H__
#define __HAL_BOOT_CONTROL_H__

#include "hal_efuse.h"
#include "soc.h"
#include "gpio_cap.h"
#include "hal_wdt.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"

#define _CTRL_CTRL_MAGIC (0x4C725463)
#define HAL_ABS_MINUS(_a, _b) (((_a) > (_b)) ? ((_a) - (_b)): ((_b) - (_a)))
#define MIN_MANIFEST_SIZE SZ_4K

typedef struct _boot_ctrl_data_t {
	uint32_t magic;
	uint32_t boot_flag;
	hal_addr_t primary_manifest_addr;
	hal_addr_t recovery_manifest_addr;

	uint32_t pll_ena;
	uint32_t security_boot_supported;
	uint32_t security_boot_ena;
	uint32_t security_boot_print_dis;
	uint32_t jtag_dis;
	uint32_t sw_fih_delay_ena;
#if CONFIG_GPIO_ENA
	uint32_t user_gpio_ena;
	uint32_t user_gpio_id;
	uint32_t perf_gpio_ena;
	uint32_t perf_pivot_gpio_id;
	uint32_t perf_updown_gpio_id;
#endif
} boot_ctrl_data_t;


typedef enum _hal_boot_flag_t {
    HAL_BOOT_FLAG_INVALID  = 0,
    HAL_BOOT_FLAG_PRIMARY  = 1,
    HAL_BOOT_FLAG_RECOVERY = 2,
} hal_boot_flag_t;

HAL_API hal_ret_t hal_read_preferred_boot_flag(hal_boot_flag_t *flag);
HAL_API hal_ret_t hal_write_preferred_boot_flag(const hal_boot_flag_t flag);

#ifdef CONFIG_BK_BOOT

extern boot_ctrl_data_t s_boot_ctrl_data;

typedef struct {
	hal_addr_t primary;
	size_t primary_size;
	hal_addr_t recovery;
	size_t recovery_size;
} hal_manifest_addr_t;

/* Original Shanhai boot control API */
HAL_API hal_ret_t hal_ctrl_partition_load_and_init(void);
HAL_API hal_ret_t hal_read_downloaded_flag(bool *flag);
HAL_API hal_ret_t hal_write_downloaded_flag(bool flag);
HAL_API hal_ret_t hal_read_manifest_address(hal_manifest_addr_t *man);
HAL_API hal_ret_t hal_write_manifest_address(const hal_manifest_addr_t *man);

/* Important notice:
 *
 * Define all boot control API as inline API to improve the performance
 * since some boot flag API is performance sensitive, e.g. when it's called
 * if the system is reset from deep sleep.
 * */
 
static inline bool hal_ctrl_is_security_boot_supported(void)
{
	if (hal_efuse_is_security_boot_supported()) {
		return true;
	}

	//Don't need to consider Boot Control flag
	return false;
}
static inline bool hal_ctrl_is_security_boot_enabled(void)
{
	if (hal_efuse_is_security_boot_enabled()) {
		return true;
	}

#ifdef CONFIG_FLASH_CONTROL_PARTITION
	if (hal_efuse_is_bc_disabled()) {
		return false;
	}

	return (s_boot_ctrl_data.security_boot_ena != 0);
#else
	return false;
#endif
}

extern bool g_boot_print_disable_bypass;
static inline bool hal_ctrl_is_security_boot_print_disabled(void)
{
	if (g_boot_print_disable_bypass) {
		return false;
	}

	if (hal_efuse_is_debug_disabled()) {
		return true;
	}

#ifdef CONFIG_FLASH_CONTROL_PARTITION
	if (hal_efuse_is_bc_disabled()) {
		return false;
	}

	return (s_boot_ctrl_data.security_boot_print_dis != 0);
#else
	return false;
#endif
}

static inline bool hal_ctrl_is_fast_boot_enabled(void)
{
	if(hal_efuse_is_fast_boot_disabled()) {
		return true;
	}
}

static inline bool hal_ctrl_is_pll_enabled(void)
{
	if(hal_efuse_is_pll_enabled()) {
		return true;
	}

#ifdef CONFIG_FLASH_CONTROL_PARTITION
	if (hal_efuse_is_bc_disabled()) {
		return false;
	}

	return (s_boot_ctrl_data.pll_ena != 0);
#else
	return false;
#endif
}

static inline bool hal_ctrl_is_jtag_disabled(void)
{
	if (hal_efuse_is_jtag_disabled()) {
		return true;
	}

#ifdef CONFIG_FLASH_CONTROL_PARTITION
	if (hal_efuse_is_bc_disabled()) {
		return false;
	}

	return (s_boot_ctrl_data.jtag_dis != 0);
#else
	return false;
#endif
}

static inline bool hal_ctrl_is_direct_jump_enabled(void)
{
	if (hal_efuse_is_direct_jump_enabled()) {
		return true;
	}

	return false;
}


#if CONFIG_GPIO_ENA
static inline void hal_ctrl_perf_gpio_down(void)
{
	if (s_boot_ctrl_data.perf_gpio_ena) {
		if (s_boot_ctrl_data.perf_updown_gpio_id < SOC_GPIO_NUM)
			GPIO_DOWN(s_boot_ctrl_data.perf_updown_gpio_id);
	}
}
#endif

static inline bool hal_ctrl_sw_fih_delay_enabled(void)
{
	if (hal_efuse_is_fih_delay_enabled()) {
		return true;
	}

#ifdef CONFIG_FLASH_CONTROL_PARTITION
	if (hal_efuse_is_bc_disabled()) {
		return false;
	}

	return !!(s_boot_ctrl_data.sw_fih_delay_ena);
#else
	return false;
#endif
}

static inline bool hal_ctrl_critical_err_disabled(void)
{
	return hal_efuse_critical_err_disabled();
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* __HAL_BOOT_CONTROL_H__ */
