#ifndef _TIZENRT_PMU_8730E_H_
#define _TIZENRT_PMU_8730E_H_

#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>
/* This enum is for pre/post sleep processing */
typedef enum {
	PMU_OS                  = 0,
	PMU_WLAN_DEVICE         = 1,
	PMU_LOGUART_DEVICE      = 2,
	PMU_UART1_DEVICE        = 3,
	PMU_BT_DEVICE           = 4,
	PMU_SPI_DEVICE          = 5,
	PMU_I2C_DEVICE          = 6,
	PMU_I2S_DEVICE          = 7,
	PMU_GPIO_DEVICE         = 8,
	PMU_MIPI_DEVICE         = 9,
	PMU_MAX,
} PMU_DEVICE;

/* This enum is for domain register to halt the PM state if driver operation is running */
typedef enum {
	BSP_SPI_DRV             = 0,
	BSP_I2C_DRV             = 1,
	BSP_I2S_DRV             = 2,
	BSP_FLASH_DRV           = 3,
	BSP_UART_DRV            = 4,
	BSP_MIPI_DRV            = 5,
	BSP_DOMAIN_MAX,
} BSP_DRV_DOMAIN;

// default locked by OS and not to sleep until OS release wakelock in somewhere
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined (ARM_CORE_CM4)
#define DEFAULT_WAKELOCK		(BIT(PMU_OS))
#else
#define DEFAULT_WAKELOCK		(BIT(PMU_OS))
#endif
#define DEFAULT_DEEP_WAKELOCK		(BIT(PMU_OS))
#define SLEEP_MAX_DELAY		(u32) 0xffffffffUL
#endif

typedef uint32_t (*PSM_HOOK_FUN)(uint32_t, void *param_ptr);

typedef struct {
	u32					nDeviceId;
	PSM_HOOK_FUN		sleep_hook_fun;
	void				*sleep_param_ptr;
	PSM_HOOK_FUN		wakeup_hook_fun;
	void				*wakeup_param_ptr;
} PSM_DD_HOOK_INFO;


/**
  * @brief  init system active timer for PMU.
  * @param  none.
  * @retval status value:
  *          - 0: _FAIL
  *          - 1: _SUCCESS
  * @note can just used in late resume or later, can not used in wakeup_hook_fun.
  */
u32 pmu_exec_sleep_hook_funs(void);
void pmu_exec_wakeup_hook_funs(u32 nDeviceIdMax);
uint32_t pmu_set_sleep_type(uint32_t type);
uint32_t pmu_get_sleep_type(void);

uint32_t pmu_set_sysactive_time(uint32_t timeout);
void pmu_register_sleep_callback(u32 nDeviceId, PSM_HOOK_FUN sleep_hook_fun, void *sleep_param_ptr,
								 PSM_HOOK_FUN wakeup_hook_fun, void *wakeup_param_ptr);
void pmu_unregister_sleep_callback(u32 nDeviceId);

int tizenrt_ready_to_sleep(void);
int tizenrt_ready_to_dsleep(void);
void tizenrt_sleep_processing(void);
pm_wakeup_reason_code_t tizenrt_get_wakeupreason(void);
clock_t tizenrt_get_missingtick(void);

#ifndef CONFIG_PLATFORM_TIZENRT_OS
void pmu_acquire_wakelock(uint32_t nDeviceId);
void pmu_release_wakelock(uint32_t nDeviceId);
uint32_t pmu_get_wakelock_status(void);
#endif

#endif
#endif
