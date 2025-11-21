#include <string.h>
#include <stdlib.h>
#include <common/bk_generic.h>
#include <common/bk_compiler.h>
#include <os/os.h>
#include <os/mem.h>
#include <components/system.h>
#include "bk_fake_clock.h"
#include "bk_arch.h"

static volatile uint32_t s_sema_cnt = 0;
static volatile uint32_t s_sema_bit = 0;

__bk_weak bk_err_t rtos_create_thread(beken_thread_t* thread, uint8_t priority, const char* name,
	beken_thread_function_t function, uint32_t stack_size, beken_thread_arg_t arg)
{
	if (function) {
		function(arg);
	}
	return BK_OK;
}

__bk_weak bk_err_t rtos_delete_thread( beken_thread_t* thread )
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_thread_join(beken_thread_t* thread)
{
	return BK_OK;
}

__bk_weak bool rtos_is_current_thread(beken_thread_t* thread)
{
	return true;
}

__bk_weak beken_thread_t* rtos_get_current_thread(void)
{
	return NULL;
}

__bk_weak bk_err_t rtos_init_semaphore(beken_semaphore_t* semaphore, int maxCount)
{
	//*(uint32_t *)semaphore = s_sema_bit++;

	return BK_OK;
}

__bk_weak bk_err_t rtos_init_semaphore_ex(beken_semaphore_t* semaphore, int maxCount, int init_count)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_get_semaphore(beken_semaphore_t* semaphore, uint32_t timeout_ms)
{
	return BK_OK;
}

__bk_weak int rtos_get_semaphore_count(beken_semaphore_t* semaphore)
{
	return 0;
}

__bk_weak int rtos_set_semaphore(beken_semaphore_t* semaphore)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_deinit_semaphore(beken_semaphore_t* semaphore)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_init_mutex(beken_mutex_t *mutex)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_trylock_mutex(beken_mutex_t *mutex)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_lock_mutex(beken_mutex_t* mutex)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_unlock_mutex(beken_mutex_t* mutex)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_deinit_mutex(beken_mutex_t* mutex)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_init_queue(beken_queue_t* queue, const char* name, uint32_t message_size, uint32_t number_of_messages)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_push_to_queue(beken_queue_t* queue, void* message, uint32_t timeout_ms)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_push_to_queue_front(beken_queue_t* queue, void* message, uint32_t timeout_ms)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_pop_from_queue(beken_queue_t *queue, void *message, uint32_t timeout_ms)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_deinit_queue(beken_queue_t *queue)
{
	return BK_OK;
}

__bk_weak bool rtos_is_queue_empty(beken_queue_t *queue)
{
	return true;
}

__bk_weak bool rtos_is_queue_full(beken_queue_t *queue)
{
	return true;
}

__bk_weak bk_err_t rtos_start_oneshot_timer(beken2_timer_t *timer)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_deinit_oneshot_timer(beken2_timer_t *timer)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_stop_oneshot_timer(beken2_timer_t *timer)
{
	return BK_OK;
}

bool rtos_is_oneshot_timer_init(beken2_timer_t *timer)
{
	return true;
}

bool rtos_is_oneshot_timer_running(beken2_timer_t *timer)
{
	return true;
}

__bk_weak bk_err_t rtos_oneshot_reload_timer(beken2_timer_t *timer)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_oneshot_reload_timer_ex(beken2_timer_t *timer, uint32_t time_ms,
	timer_2handler_t function, void *larg, void *rarg)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_init_oneshot_timer(beken2_timer_t *timer, uint32_t time_ms,
	timer_2handler_t function, void *larg, void *rarg)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_init_timer(beken_timer_t *timer, uint32_t time_ms,
	timer_handler_t function, void *arg)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_start_timer(beken_timer_t *timer)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_stop_timer(beken_timer_t *timer)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_reload_timer(beken_timer_t *timer)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_change_period(beken_timer_t *timer, uint32_t time_ms)
{
	return BK_OK;
}

__bk_weak bk_err_t rtos_deinit_timer(beken_timer_t *timer)
{
	return BK_OK;
}

__bk_weak uint32_t rtos_get_timer_expiry_time(beken_timer_t *timer)
{
	return 0;
}

__bk_weak uint32_t rtos_get_next_expire_time()
{
	return 0;
}

__bk_weak uint32_t rtos_get_current_timer_count(void)
{
	return 0;
}

__bk_weak bool rtos_is_timer_init(beken_timer_t *timer)
{
	return true;
}

__bk_weak bool rtos_is_timer_running(beken_timer_t *timer)
{
	return true;
}

__bk_weak void rtos_suspend_thread(beken_thread_t *thread)
{
}

__bk_weak void rtos_resume_thread(beken_thread_t *thread)
{
}

__bk_weak beken_time_t rtos_get_time(void)
{
	return (beken_time_t)0;
}

__bk_weak bk_err_t rtos_delay_milliseconds(uint32_t num_ms)
{
	return BK_OK;
}

__bk_weak uint32_t rtos_disable_int(void)
{
	return 0;
}

__bk_weak void rtos_enable_int(uint32_t int_level)
{
}

__bk_weak bool rtos_is_in_interrupt_context(void)
{
	return false;
}

__bk_weak int32 sys_drv_int_disable(uint32 param)
{
	return 0;
}

__bk_weak int32 sys_drv_int_enable(uint32 param)
{
	return 0;
}

__bk_weak int32 sys_drv_int_group2_enable(uint32 param)
{
	return 0;
}
