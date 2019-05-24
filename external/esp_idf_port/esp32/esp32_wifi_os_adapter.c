/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/****************************************************************************
// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

****************************************************************************/

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <sched/sched.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <sched.h>
#include <debug.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <tinyara/wdog.h>
#include <tinyara/arch.h>
#include <tinyara/cancelpt.h>

#include "esp_attr.h"
#include "esp_log.h"
#include "esp_phy_init.h"
#include "esp_wifi_os_adapter.h"
#include "esp_system.h"
#include "rom/ets_sys.h"
#include <semaphore/semaphore.h>
#include "event_groups.h"
#include <os.h>
#include <periph_ctrl.h>
#include <../../os/arch/xtensa/src/xtensa/xtensa.h>
#include <tinyara/wqueue.h>

#define TAG "WIFI_ADAPTER"
#define TIMER_INITIALIZED_VAL 0x12121212
#define WDOG_TIMER(ptimer) ((WDOG_ID)(ptimer->timer_arg))

extern void esp_dport_access_stall_other_cpu_start_wrap(void);
extern void esp_dport_access_stall_other_cpu_end_wrap(void);
extern int64_t get_instant_time(void);


/*=================seamphore API=================*/
static void *IRAM_ATTR semphr_create_wrapper(uint32_t max, uint32_t init)
{
	if (max > SEM_VALUE_MAX || init > SEM_VALUE_MAX) {
		return NULL;
	}
	sem_t *sem = (sem_t *)malloc(sizeof(*sem));
	if (!sem) {
		return NULL;
	}
	int status = sem_init(sem, 0, init);
	if (status != OK) {
		return NULL;
	}
	return (void *)sem;
}

static void IRAM_ATTR semphr_delete_wrapper(void *semphr)
{
	if (semphr == NULL) {
		ESP_LOGI(TAG, "semphr is NULL\n");
		return;
	}
	sem_destroy(semphr);

}

static int32_t IRAM_ATTR semphr_take_from_isr_wrapper(void *semphr, void *hptw)
{
    *(bool *) hptw = WIFI_ADAPTER_FALSE;
	FAR struct tcb_s *stcb = NULL;
    pid_t pid = getpid();   
    FAR struct tcb_s *rtcb = sched_gettcb(pid);
	sem_t *sem = (sem_t *) semphr;
	irqstate_t saved_state;
	int ret = pdFAIL;
	saved_state = irqsave();

	if (enter_cancellation_point()) {
		set_errno(ECANCELED);
		leave_cancellation_point();
		irqrestore(saved_state);
		return ERROR;
	}
	/* Make sure we were supplied with a valid semaphore */
	if ((sem != NULL) && ((sem->flags & FLAGS_INITIALIZED) != 0)) {

		if (sem->semcount > 0) {
			/* It is, let the task take the semaphore. */
			sem->semcount--;
			sem_addholder(sem);
			rtcb->waitsem = NULL;
			ret = pdPASS;
			for (stcb = (FAR struct tcb_s *)g_waitingforsemaphore.head; (stcb && stcb->waitsem != sem); stcb = stcb->flink) ;
			if (stcb) {
				if (stcb->sched_priority >= rtcb->sched_priority) {
					*(bool *) hptw = WIFI_ADAPTER_TRUE;
				}
			}
		}
		/* The semaphore is NOT available */
		else {

			return ret;
		}
	} else {
		set_errno(EINVAL);
	}
	leave_cancellation_point();
	irqrestore(saved_state);
	return ret;

}

static int32_t IRAM_ATTR semphr_take_wrapper(void *semphr, uint32_t block_time_tick)
{
	int ret;
	if (semphr == NULL) {
		ESP_LOGI(TAG, "semphr is NULL\n");
		return pdFAIL;
	}
	if (block_time_tick == OSI_FUNCS_TIME_BLOCKING) {
		ret = sem_wait(semphr);
		if (ret == OK) {
			return pdPASS;
		} else {
			return pdFAIL;
		}

	} else {
		struct timespec abstime;
	    calc_abs_time(&abstime, block_time_tick);	
        ret = sem_timedwait(semphr, &abstime);
		if (ret == OK) {
			return pdPASS;
		} else {
			return pdFAIL;
		}
	}
}

static int32_t IRAM_ATTR semphr_give_wrapper(void *semphr)
{
	int ret;
	if (semphr == NULL) {
		ESP_LOGI(TAG, "semphr is NULL\n");
		return pdFAIL;
	}
	ret = sem_post(semphr);
	if (ret == OK) {
		return pdPASS;
	} else {
		return pdFAIL;
	}
}

static int32_t IRAM_ATTR semphr_give_from_isr_wrapper(void *semphr, void *hptw)
{
	*(int *)hptw = WIFI_ADAPTER_FALSE;
	return semphr_give_wrapper(semphr);
}

/*=================mutx API=================*/
static void *IRAM_ATTR recursive_mutex_create_wrapper(void)
{
	pthread_mutexattr_t mattr;
	int status = 0;

	pthread_mutex_t *mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (mutex == NULL) {
		return NULL;
	}
	pthread_mutexattr_init(&mattr);
	status = pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
	if (status != 0) {
		ESP_LOGI(TAG,"recursive_mutex_test: ERROR pthread_mutexattr_settype failed, status=%d\n", status);
		return NULL;
	}
	status = pthread_mutex_init(mutex, &mattr);
	if (status) {
		return NULL;
	}
	return (void *)mutex;
}

static void *IRAM_ATTR mutex_create_wrapper(void)
{
	return recursive_mutex_create_wrapper();
}

static void IRAM_ATTR mutex_delete_wrapper(void *mutex)
{
	if (mutex == NULL) {
		ESP_LOGI(TAG,"mutex is NULL\n");
		return;
	}
	pthread_mutex_destroy(mutex);
}

static int32_t IRAM_ATTR mutex_lock_wrapper(void *mutex)
{
    if (mutex == NULL) {
		ESP_LOGI(TAG,"mutex is NULL\n");
		return EINVAL;
	}
	int ret = pthread_mutex_lock(mutex);
	if (ret) {
		return pdFAIL;
	}
	return pdPASS;

}

static int32_t IRAM_ATTR mutex_unlock_wrapper(void *mutex)
{
    if (up_interrupt_context() == true) {
		ESP_LOGI(TAG, "%s interrupt context\n", __func__);
	}

	if (mutex == NULL) {
		ESP_LOGI(TAG, "mutex is NULL\n");
		return EINVAL;
	}
	int ret = pthread_mutex_unlock(mutex);
	if (ret) {
		return pdFAIL;
	}
	return pdPASS;
}

/*=================task control API=================*/
static int32_t IRAM_ATTR task_create_wrapper(void *task_func, const char *name, uint32_t stack_depth, void *param, uint32_t prio, void *task_handle)
{
	int pid = task_create(name, prio, stack_depth, task_func, param);
	if (pid < 0) {
		return pdFAIL;
	}
	if (task_handle) {
		int *p = (int *)task_handle;
		*p = pid;
	}
	return pdPASS;
}

static int32_t IRAM_ATTR task_create_pinned_to_core_wrapper(void *task_func, const char *name, uint32_t stack_depth, void *param, uint32_t prio, void *task_handle, uint32_t core_id)
{
#ifndef CONFIG_SMP
	return task_create_wrapper(task_func, name, stack_depth, param, prio, task_handle);
#else
	return pdFAIL;
#endif
}

static void IRAM_ATTR task_delete_wrapper(void *task_handle)
{
	if (task_handle < 0) {
		return;
	}

	int pid = (int *)task_handle;
	task_delete(pid);
}

static void IRAM_ATTR task_delay_wrapper(uint32_t tick)
{
	uint64_t usecs = TICK2USEC(tick);
	usleep(usecs);
}

static int curpid;
static void *IRAM_ATTR task_get_current_task_wrapper(void)
{
	curpid = getpid();
	return (void *)&curpid;
}

static inline int32_t IRAM_ATTR task_ms_to_tick_wrapper(uint32_t ms)
{
	return (int32_t) MSEC2TICK(ms);
}

static inline int32_t IRAM_ATTR task_get_max_priority_wrapper(void)
{
	return (int32_t)(SCHED_PRIORITY_MAX);
}

static inline int32_t IRAM_ATTR is_in_isr_wrapper(void)
{
	return (int32_t) up_interrupt_context();
}

/*=================wifi RF API=================*/
static inline int32_t IRAM_ATTR esp_phy_rf_deinit_wrapper(uint32_t module)
{
	return esp_phy_rf_deinit((phy_rf_module_t) module);
}

static inline int32_t IRAM_ATTR phy_rf_init_wrapper(const void *init_data, uint32_t mode, void *calibration_data, uint32_t module)
{
	return esp_phy_rf_init(init_data, mode, calibration_data, module);
}

static inline void IRAM_ATTR esp_phy_load_cal_and_init_wrapper(uint32_t module)
{
	return esp_phy_load_cal_and_init((phy_rf_module_t) module);
}

static inline int32_t esp_read_mac_wrapper(uint8_t *mac, uint32_t type)
{
	return esp_read_mac(mac, (esp_mac_type_t) type);
}

/*=================soft timer API=================*/


static IRAM_ATTR bool timer_initialized(ETSTimer *ptimer)
{
	return ptimer->timer_period == TIMER_INITIALIZED_VAL;

}

static void IRAM_ATTR period_timerfn(int argc, uint32_t arg, ...)
{
	ETSTimer *etimer = (ETSTimer *) arg;
	work_cancel(HPWORK, etimer->timer_next);
	work_queue(HPWORK, etimer->timer_next, etimer->timer_func, etimer->timer_expire, 0);
    wd_start((WDOG_ID)etimer->timer_arg, etimer->timer_period, period_timerfn, 1, etimer);
}

static void IRAM_ATTR once_timerfn(int argc, uint32_t arg, ...)
{
	ETSTimer *etimer = (ETSTimer *) arg;
	work_cancel(HPWORK, etimer->timer_next);
	work_queue(HPWORK, etimer->timer_next, etimer->timer_func, etimer->timer_expire, 0);
}

void timer_init_wrapper(void)
{

}

void timer_deinit_wrapper(void)
{

}


static void IRAM_ATTR timer_arm_wrapper(void *timer, uint32_t tmout, bool repeat)
{
	ETSTimer *etimer = (ETSTimer *) timer;
	if (etimer->timer_period != TIMER_INITIALIZED_VAL) {
		ets_printf("assert bug\n");
	}

	wd_cancel((WDOG_ID) etimer->timer_arg);

	int delay = MSEC2TICK(tmout);
	if (repeat) {
		wd_start((WDOG_ID) etimer->timer_arg, delay, period_timerfn, 1, etimer);
	} else {
		wd_start((WDOG_ID) etimer->timer_arg, delay, once_timerfn, 1, etimer);
	}
}

static void IRAM_ATTR timer_disarm_wrapper(void *timer)
{
	ETSTimer *etimer = (ETSTimer *) timer;
	if (etimer->timer_period == TIMER_INITIALIZED_VAL) {
		wd_cancel((WDOG_ID) etimer->timer_arg);
	}
}

static void IRAM_ATTR timer_done_wrapper(void *ptimer)
{
	ETSTimer *etimer = (ETSTimer *) ptimer;
	if (etimer->timer_period == TIMER_INITIALIZED_VAL) {
		wd_delete((WDOG_ID) etimer->timer_arg);
		free(etimer->timer_next);
		etimer->timer_next = NULL;
		etimer->timer_arg = NULL;
		etimer->timer_func = NULL;
		etimer->timer_period = 0;
	}
}

static void IRAM_ATTR timer_setfn_wrapper(void *ptimer, void *pfunction, void *parg)
{
	ETSTimer *etimer = (ETSTimer *) ptimer;

	if (etimer->timer_period != TIMER_INITIALIZED_VAL) {
		memset(etimer, 0, sizeof(ETSTimer));
		etimer->timer_period = TIMER_INITIALIZED_VAL;
	}

	if (etimer->timer_arg == NULL) {
		etimer->timer_arg = (WDOG_ID) wd_create();
		if (!etimer->timer_arg) {
			return;
		}
		etimer->timer_func = pfunction;
		etimer->timer_expire = (uint32_t) parg;
		etimer->timer_next = (struct work_s *)zalloc(sizeof(struct work_s));
	}
}

static void IRAM_ATTR timer_arm_us_wrapper(void *ptimer, uint32_t us, bool repeat)
{
    ETSTimer *etimer = (ETSTimer *) ptimer;
	if (etimer == NULL) {
		ESP_LOGI(TAG, "timer is NULL\n");
		return;
	}
	int delay = USEC2TICK(us);
	if (repeat) {
		etimer->timer_period = delay;
		wd_start((WDOG_ID) etimer->timer_arg, delay, period_timerfn, 1, etimer);
	} else {
		wd_start((WDOG_ID) etimer->timer_arg, delay, once_timerfn, 1, etimer);
	}

}

static inline int32_t IRAM_ATTR get_time_wrapper(void *t)
{
	return os_get_time(t);
}

/*=================Miscellaneous API================================*/
static inline int32_t IRAM_ATTR esp_os_get_random_wrapper(uint8_t *buf, size_t len)
{
	return (int32_t) os_get_random((unsigned char *)buf, len);

}

/*=================espwifi modem API=========================*/
static inline esp_err_t esp_modem_sleep_enter_wrapper(uint32_t module)
{   
    return esp_modem_sleep_enter((modem_sleep_module_t) module);
}

static inline esp_err_t esp_modem_sleep_exit_wrapper(uint32_t module)
{   
    return esp_modem_sleep_exit((modem_sleep_module_t) module);
}

static inline esp_err_t esp_modem_sleep_register_wrapper(uint32_t module)
{
    return esp_modem_sleep_register((modem_sleep_module_t) module);
}

static inline esp_err_t esp_modem_sleep_deregister_wrapper(uint32_t module)
{   
    return esp_modem_sleep_deregister((modem_sleep_module_t) module);
}

/*=================espwifi NVS API=========================*/
/*stub functions*/
static inline int32_t esp_nvs_open_wrapper(const char *name, uint32_t open_mode, uint32_t *out_handle)
{
	return 0;
}

int32_t nvs_set_i8(uint32_t handle, const char *key, int8_t value)
{
    return 0;
}

int32_t nvs_get_i8(uint32_t handle, const char *key, int8_t *out_value)
{
    return 0;
}

int32_t nvs_set_u8(uint32_t handle, const char *key, uint8_t value)
{
    return 0;
}

int32_t nvs_get_u8(uint32_t handle, const char *key, uint8_t *out_value)
{
    return 0;
}

int32_t nvs_set_u16(uint32_t handle, const char *key, uint16_t value)
{
    return 0;
}

int32_t nvs_get_u16(uint32_t handle, const char *key, uint16_t *out_value)
{
    return 0;
}

void nvs_close(uint32_t handle)
{
    return 0;
}

int32_t nvs_commit(uint32_t handle)
{
    return 0;
}

int32_t nvs_set_blob(uint32_t handle, const char *key, const void *value, size_t length)
{
    return 0;
}

int32_t nvs_get_blob(uint32_t handle, const char *key, void *out_value, size_t *length)
{
    return 0;
}

int32_t nvs_erase_key(uint32_t handle, const char *key)
{
    return 0;
}

/*=================espwifi smart config API=========================*/
/* Will complete next stage, not block WIFI ENABLE*/
extern int ets_printf(const char *fmt, ...);

static void IRAM_ATTR set_isr_wrapper(int32_t n, void *f, void *arg)
{
	irq_attach(ESP32_IRQ_MAC, f, arg);
	up_enable_irq(n);
}

static void IRAM_ATTR ints_on_wrapper(uint32_t mask)
{
	int n = 0;
	while (mask) {
		n++;
		mask >>= 1;
	}
	up_enable_irq(n);
}

static void IRAM_ATTR ints_off_wrapper(uint32_t mask)
{
	int n = 0;
	while (mask) {
		n++;
		mask >>= 1;
	}
	up_disable_irq(n);
}

static void *IRAM_ATTR spin_lock_create_wrapper(void)
{
	return recursive_mutex_create_wrapper();
}

static irqstate_t wifi_int_disable_flags;

static uint32_t IRAM_ATTR wifi_int_disable_wrapper(void *wifi_int_mux)
{
	if (wifi_int_mux) {
		wifi_int_disable_flags = irqsave();
	}
	return 0;
}

static void IRAM_ATTR wifi_int_restore_wrapper(void *wifi_int_mux, uint32_t tmp)
{
	if (wifi_int_mux) {
		irqrestore(wifi_int_disable_flags);
	}
}

static void IRAM_ATTR task_yield_from_isr_wrapper(void)
{
	return;
}

static uint32_t IRAM_ATTR event_group_wait_bits_wrapper(void *event, uint32_t bits_to_wait_for, int32_t clear_on_exit, int32_t wait_for_all_bits, uint32_t block_time_tick)
{
	if (block_time_tick == OSI_FUNCS_TIME_BLOCKING) {
		return (uint32_t) event_group_wait_bits(event, bits_to_wait_for, clear_on_exit, wait_for_all_bits, port_max_delay);
	}
	return (uint32_t) event_group_wait_bits(event, bits_to_wait_for, clear_on_exit, wait_for_all_bits, block_time_tick);
}

uint32_t IRAM_ATTR esp_get_free_heap_size(void)
{
	struct mallinfo mem_info;

#ifdef CONFIG_CAN_PASS_STRUCTS
	mem_info = mallinfo();
#else
	(void)mallinfo(&mem_info);
#endif
	return mem_info.fordblks;
}

static void *IRAM_ATTR malloc_internal_wrapper(size_t size)
{
#if (defined(CONFIG_SPIRAM_SUPPORT) && CONFIG_MM_NHEAPS > 1)
	return malloc_at(0, size);
#else
	return malloc(size);
#endif
}

static void *IRAM_ATTR realloc_internal_wrapper(void *ptr, size_t size)
{
	return realloc(ptr, size);
}

static void *IRAM_ATTR calloc_internal_wrapper(size_t n, size_t size)
{
#if (defined(CONFIG_SPIRAM_SUPPORT) && CONFIG_MM_NHEAPS > 1)
	return calloc_at(0, n, size);
#else
	return calloc(n, size);
#endif
}

static void *IRAM_ATTR zalloc_internal_wrapper(size_t size)
{
#if (defined(CONFIG_SPIRAM_SUPPORT) && CONFIG_MM_NHEAPS > 1)
	return zalloc_at(0, size);
#else
	return zalloc(size);
#endif
}

void *IRAM_ATTR wifi_malloc(size_t size)
{
	return malloc(size);
}

void *IRAM_ATTR wifi_realloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}

void *IRAM_ATTR wifi_calloc(size_t n, size_t size)
{
	return calloc(n, size);
}

static void *IRAM_ATTR wifi_zalloc_wrapper(size_t size)
{
	return zalloc(size);
}

wifi_static_queue_t *IRAM_ATTR wifi_create_queue(int queue_len, int item_size)
{
	wifi_static_queue_t *wifi_queue = (wifi_static_queue_t *)malloc(sizeof(wifi_static_queue_t));
	if (!wifi_queue) {
		return NULL;
	}
	wifi_queue->handle = queue_create_wrapper(queue_len, item_size);
	return wifi_queue;
}

void IRAM_ATTR wifi_delete_queue(wifi_static_queue_t *queue)
{
	if (queue && queue->handle) {
		queue_delete_wrapper(queue->handle);
	}
	free(queue);
}

static void *IRAM_ATTR wifi_create_queue_wrapper(int32_t queue_len, int32_t item_size)
{
	return wifi_create_queue(queue_len, item_size);
}

static void IRAM_ATTR wifi_delete_queue_wrapper(void *queue)
{
	wifi_delete_queue(queue);
}

void IRAM_ATTR task_yield_wrapper(void)
{
	extern int sched_yield(void);
	sched_yield();
}

/*=================espwifi os adapter interface =====================*/

wifi_osi_funcs_t g_wifi_osi_funcs = {
	._version = ESP_WIFI_OS_ADAPTER_VERSION,
	._set_isr = set_isr_wrapper,
	._ints_on = ints_on_wrapper,
	._ints_off = ints_off_wrapper,
	._spin_lock_create = spin_lock_create_wrapper,
	._spin_lock_delete = mutex_delete_wrapper,
	._wifi_int_disable = wifi_int_disable_wrapper,
	._wifi_int_restore = wifi_int_restore_wrapper,
	._task_yield = task_yield_wrapper,
	._task_yield_from_isr = task_yield_from_isr_wrapper,
	._semphr_create = semphr_create_wrapper,
	._semphr_delete = semphr_delete_wrapper,
	._semphr_take_from_isr = semphr_take_from_isr_wrapper,
	._semphr_give_from_isr = semphr_give_from_isr_wrapper,
	._semphr_take = semphr_take_wrapper,
	._semphr_give = semphr_give_wrapper,
	._mutex_create = mutex_create_wrapper,
	._recursive_mutex_create = recursive_mutex_create_wrapper,
	._mutex_delete = mutex_delete_wrapper,
	._mutex_lock = mutex_lock_wrapper,
	._mutex_unlock = mutex_unlock_wrapper,
	._queue_create = queue_create_wrapper,
	._queue_delete = queue_delete_wrapper,
	._queue_send = queue_send_wrapper,
	._queue_send_from_isr = queue_send_from_isr_wrapper,
	._queue_send_to_back = queue_send_to_back_wrapper,
	._queue_send_to_front = queue_send_to_front_wrapper,
	._queue_recv = queue_recv_wrapper,
	._queue_recv_from_isr = queue_recv_from_isr_wrapper,
	._queue_msg_waiting = queue_msg_waiting_wrapper,
	._event_group_create = event_group_create,
	._event_group_delete = event_group_delete,
	._event_group_set_bits = event_group_set_bits,
	._event_group_clear_bits = event_group_clear_bits,
	._event_group_wait_bits = event_group_wait_bits_wrapper,
	._task_create_pinned_to_core = task_create_pinned_to_core_wrapper,
	._task_create = task_create_wrapper,
	._task_delete = task_delete_wrapper,
	._task_delay = task_delay_wrapper,
	._task_ms_to_tick = task_ms_to_tick_wrapper,
	._task_get_current_task = task_get_current_task_wrapper,
	._task_get_max_priority = task_get_max_priority_wrapper,
	._is_in_isr = is_in_isr_wrapper,
	._malloc = malloc,
	._free = free,
	._get_free_heap_size = esp_get_free_heap_size,
	._rand = esp_random,
	._dport_access_stall_other_cpu_start_wrap = esp_dport_access_stall_other_cpu_start_wrap,
	._dport_access_stall_other_cpu_end_wrap = esp_dport_access_stall_other_cpu_end_wrap,
	._phy_rf_init = phy_rf_init_wrapper,
	._phy_rf_deinit = esp_phy_rf_deinit_wrapper,
	._phy_load_cal_and_init = esp_phy_load_cal_and_init_wrapper,
	._read_mac = esp_read_mac_wrapper,
	._timer_init = timer_init_wrapper,
	._timer_deinit = timer_deinit_wrapper,
	._timer_arm = timer_arm_wrapper,
	._timer_disarm = timer_disarm_wrapper,
	._timer_done = timer_done_wrapper,
	._timer_setfn = timer_setfn_wrapper,
	._timer_arm_us = timer_arm_us_wrapper,
	._periph_module_enable = periph_module_enable,
	._periph_module_disable = periph_module_disable,
	._esp_timer_get_time = get_instant_time,
	._nvs_set_i8 = nvs_set_i8,
	._nvs_get_i8 = nvs_get_i8,
	._nvs_set_u8 = nvs_set_u8,
	._nvs_get_u8 = nvs_get_u8,
	._nvs_set_u16 = nvs_set_u16,
	._nvs_get_u16 = nvs_get_u16,
	._nvs_open = esp_nvs_open_wrapper,
	._nvs_close = nvs_close,
	._nvs_commit = nvs_commit,
	._nvs_set_blob = nvs_set_blob,
	._nvs_get_blob = nvs_get_blob,
	._nvs_erase_key = nvs_erase_key,
	._get_random = esp_os_get_random_wrapper,
	._get_time = get_time_wrapper,
	._random = os_random,
	._log_write = esp_log_write,
	._log_timestamp = esp_log_timestamp,
	._malloc_internal = malloc_internal_wrapper,
	._realloc_internal = realloc_internal_wrapper,
	._calloc_internal = calloc_internal_wrapper,
	._zalloc_internal = zalloc_internal_wrapper,
	._wifi_malloc = wifi_malloc,
	._wifi_realloc = wifi_realloc,
	._wifi_calloc = wifi_calloc,
	._wifi_zalloc = wifi_zalloc_wrapper,
	._wifi_create_queue = wifi_create_queue_wrapper,
	._wifi_delete_queue = wifi_delete_queue_wrapper,
	._modem_sleep_enter = esp_modem_sleep_enter_wrapper,
	._modem_sleep_exit = esp_modem_sleep_exit_wrapper,
	._modem_sleep_register = esp_modem_sleep_register_wrapper,
	._modem_sleep_deregister = esp_modem_sleep_deregister_wrapper,
//  ._sc_ack_send = sc_ack_send_wrapper,
//  ._sc_ack_send_stop = sc_ack_send_stop,
	._magic = ESP_WIFI_OS_ADAPTER_MAGIC,
};
