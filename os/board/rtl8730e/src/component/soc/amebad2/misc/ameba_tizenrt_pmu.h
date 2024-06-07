#ifndef _FREERTOS_PMU_8721D_H_
#define _FREERTOS_PMU_8721D_H_

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

enum CPU1_STATE {
	CPU1_RUNNING		= 0,		/* CPU1 is in task schedular or boot from reset */
	CPU1_HOTPLUG		= 1,		/* CPU1 is offline */
	CPU1_WAKE_FROM_PG 	= 2,		/* CPU1 just wake from pg but not in task schedular */
};

// default locked by OS and not to sleep until OS release wakelock in somewhere
#if defined (ARM_CORE_CM4)
#define DEFAULT_WAKELOCK		(BIT(PMU_OS))
#else
#define DEFAULT_WAKELOCK		(BIT(PMU_OS))
#endif
#define DEFAULT_DEEP_WAKELOCK		(BIT(PMU_OS))
#define SLEEP_MAX_DELAY		(u32) 0xffffffffUL

typedef uint32_t (*PSM_HOOK_FUN)(uint32_t, void *param_ptr);

#define PMU_DEVICE_TIMER_DEFAULT_INTERVAL	2000
#define PMU_DEVICE_TIMER_MAX_INTERVAL		(1000*30) /* max 30sec */

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
uint32_t pmu_yield_os_check(void);
u32 pmu_exec_sleep_hook_funs(void);
void pmu_exec_wakeup_hook_funs(u32 nDeviceIdMax);
uint32_t pmu_set_sleep_type(uint32_t type);
uint32_t pmu_get_sleep_type(void);
void pmu_set_max_sleep_time(uint32_t timer_ms);
#ifndef CONFIG_BUILD_ROM
void pmu_deepsleep_cmd(u32 NewStatus);
#endif
void pmu_tickless_debug(u32 NewStatus);

void pmu_set_dsleep_active_time(uint32_t TimeOutMs);
void pmu_acquire_deepwakelock(uint32_t nDeviceId);
void pmu_release_deepwakelock(uint32_t nDeviceId);

void pmu_set_dev_wakeup_tick(u32 nDeviceId, u32 Ms);
uint32_t pmu_set_sysactive_time(uint32_t timeout);
void pmu_register_sleep_callback(u32 nDeviceId, PSM_HOOK_FUN sleep_hook_fun, void *sleep_param_ptr,
								 PSM_HOOK_FUN wakeup_hook_fun, void *wakeup_param_ptr);
void pmu_unregister_sleep_callback(u32 nDeviceId);

int freertos_ready_to_sleep(void);
int freertos_ready_to_dsleep(void);
void freertos_pre_sleep_processing(uint32_t *expected_idle_time);
void freertos_post_sleep_processing(uint32_t *expected_idle_time);

void pmu_acquire_wakelock(uint32_t nDeviceId);
void pmu_release_wakelock(uint32_t nDeviceId);
uint32_t pmu_get_wakelock_status(void);
uint32_t pmu_get_deepwakelock_status(void);

#ifdef ARM_CORE_CA32
void pmu_set_secondary_cpu_state(uint32_t CoreID, uint32_t NewStatus);
uint32_t pmu_get_secondary_cpu_state(uint32_t CoreID);
bool pmu_secondary_cpu_state_is_running(uint32_t CoreID);
bool pmu_secondary_cpu_state_is_hotplug(uint32_t CoreID);
bool pmu_secondary_cpu_state_is_wake(uint32_t CoreID);
#endif

extern u32 tickless_debug;
extern u32 tick_last_tcp;
#endif
