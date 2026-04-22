/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _OSIF_H_
#define _OSIF_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <mem_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BT_TIMEOUT_NONE             0
#define BT_TIMEOUT_FOREVER          0xffffffffUL

#define BT_SECURE_STACK_SIZE        (1024)

#ifdef LIST_CONTAINOR
#undef LIST_CONTAINOR
#endif
#define LIST_CONTAINOR(ptr, type, member) \
	((type *)(void*)((char *)(ptr)-(unsigned int)((char *)&((type *)(void*)ptr)->member - (char *)ptr)))


/* task & isr context check interface */
bool osif_task_context_check(void);

/* OS schedule interfaces */
void osif_delay(uint32_t ms);
void osif_delay_us(uint32_t us);
uint32_t osif_sys_time_get(void);
bool osif_sched_start(void);
bool osif_sched_stop(void);
bool osif_sched_suspend(void);
bool osif_sched_resume(void);
bool osif_sched_is_suspended(void);

/* OS task interfaces */
bool osif_task_create(void **pp_handle, const char *p_name, void (*p_routine)(void *),
					  void *p_param, uint16_t stack_size, uint16_t priority);
bool osif_task_delete(void *p_handle);
bool osif_task_suspend(void *p_handle);
bool osif_task_resume(void *p_handle);
bool osif_task_yield(void);
bool osif_task_handle_get(void **pp_handle);
bool osif_task_priority_get(void *p_handle, uint16_t *p_priority);
bool osif_task_priority_set(void *p_handle, uint16_t priority);
bool osif_signal_init(void);
void osif_signal_deinit(void);
bool osif_task_signal_send(void *p_handle, uint32_t signal);
bool osif_task_signal_recv(uint32_t *p_signal, uint32_t wait_ms);
bool osif_task_signal_clear(void *p_handle);

/* OS synchronization interfaces */
uint32_t osif_lock(void);
void osif_unlock(uint32_t flags);
bool osif_sem_create(void **pp_handle, uint32_t init_count, uint32_t max_count);
bool osif_sem_delete(void *p_handle);
bool osif_sem_take(void *p_handle, uint32_t wait_ms);
bool osif_sem_give(void *p_handle);
uint32_t osif_sem_get_count(void *p_handle);
bool osif_mutex_create(void **pp_handle);
bool osif_mutex_delete(void *p_handle);
bool osif_mutex_take(void *p_handle, uint32_t wait_ms);
bool osif_mutex_give(void *p_handle);
bool osif_recursive_mutex_create(void **pp_handle);
bool osif_recursive_mutex_delete(void *p_handle);
bool osif_recursive_mutex_take(void *p_handle, uint32_t wait_ms);
bool osif_recursive_mutex_give(void *p_handle);

/* OS message queue interfaces */
bool osif_msg_queue_create(void **pp_handle, uint32_t msg_num, uint32_t msg_size);
bool osif_msg_queue_delete(void *p_handle);
bool osif_msg_queue_peek(void *p_handle, uint32_t *p_msg_num);
bool osif_msg_send(void *p_handle, void *p_msg, uint32_t wait_ms);
bool osif_msg_recv(void *p_handle, void *p_msg, uint32_t wait_ms);
bool osif_msg_peek(void *p_handle, void *p_msg, uint32_t wait_ms);

/* OS memory management interfaces */
void *osif_mem_alloc(RAM_TYPE ram_type, size_t size);
void *osif_mem_aligned_alloc(RAM_TYPE ram_type, size_t size, uint8_t alignment);
void osif_mem_free(void *p_block);
void osif_mem_aligned_free(void *p_block);
size_t osif_mem_peek(RAM_TYPE ram_type);

/* OS software timer interfaces */
bool osif_timer_id_get(void **pp_handle, uint32_t *p_timer_id);
bool osif_timer_create(void **pp_handle, const char *p_timer_name, uint32_t timer_id,
					   uint32_t interval_ms, bool reload, void (*p_timer_callback)(void *));
bool osif_timer_start(void **pp_handle);
bool osif_timer_restart(void **pp_handle, uint32_t interval_ms);
bool osif_timer_stop(void **pp_handle);
bool osif_timer_delete(void **pp_handle);
bool osif_timer_state_get(void **pp_handle, uint32_t *p_timer_state);
bool osif_timer_dump(void);

void osif_create_secure_context(uint32_t size);
void osif_rand(void *val, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* _OSIF_H_ */
