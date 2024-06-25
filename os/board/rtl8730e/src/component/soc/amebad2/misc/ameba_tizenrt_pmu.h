#ifndef _TIZENRT_PMU_8730E_H_
#define _TIZENRT_PMU_8730E_H_

#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>
#endif
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
	PMU_MAX,
} PMU_DEVICE;

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
#ifdef CONFIG_PM
void tizenrt_pre_sleep_processing(uint32_t *expected_idle_time, void (*handler)(clock_t, pm_wakeup_reason_code_t));
void tizenrt_post_sleep_processing(uint32_t *expected_idle_time);
#endif

#ifndef CONFIG_PLATFORM_TIZENRT_OS
void pmu_acquire_wakelock(uint32_t nDeviceId);
void pmu_release_wakelock(uint32_t nDeviceId);
uint32_t pmu_get_wakelock_status(void);
#endif

#endif
