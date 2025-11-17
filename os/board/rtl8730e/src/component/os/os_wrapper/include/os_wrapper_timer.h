/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __OS_WRAPPER_TIMER_H__
#define __OS_WRAPPER_TIMER_H__

#define RTOS_TIMER_MAX_DELAY    				0xFFFFFFFFUL
#define TMR_NAME_SIZE 16

/**
 * @brief  timer handle type
 */
typedef void *rtos_timer_t;

struct os_wrapper_timer_list {
	struct os_wrapper_timer_list *prev, *next;
	_timerHandle timer_hdl;
	struct work_s *work_hdl;
	unsigned char timer_name[TMR_NAME_SIZE];
	uint32_t timer_id;
	uint32_t timeout;
	uint32_t reload;
	uint32_t live;
	void (*function)(void *);
	void *data;
};

/**
 * @brief  Static memory allocation implementation of rtos_timer_create
 * @param  pp_handle:
 * @param  p_timer_name:
 * @param  timer_id:
 * @param  interval_ms:
 * @param  reload:
 * @param  p_timer_callback:
 * @retval
 */
int rtos_timer_create_static(rtos_timer_t *pp_handle, const char *p_timer_name, uint32_t timer_id,
							 uint32_t interval_ms, uint8_t reload, void (*p_timer_callback)(void *));

/**
 * @brief  Static memory allocation implementation of rtos_timer_delete
 * @param  p_handle:
 * @param  wait_ms:
 * @retval
 */
int rtos_timer_delete_static(rtos_timer_t p_handle, uint32_t wait_ms);

/**
 * @brief  For FreeRTOS, map to xTimerCreate
 * @note   Usage example:
 * Create:
 *          rtos_timer_t timer_handle;
 *          rtos_timer_create(&timer_handle, "timer_test", timer_id, delay_ms, is_reload, timer_cb_function);
 * Start:
 *          rtos_timer_start(timer_handle, wait_ms);
 * Stop:
 *          rtos_timer_stop(timer_handle, wait_ms);
 * Delete:
 *          rtos_timer_delete(timer_handle, wait_ms);
 * @param  pp_handle: The handle itself is a pointer, and the pp_handle means a pointer to the handle.
 * @param  p_timer_name:
 * @param  timer_id:
 * @param  interval_ms:
 * @param  reload:
 * @param  p_timer_callback:
 * @retval
 */
int rtos_timer_create(rtos_timer_t *pp_handle, const char *p_timer_name, uint32_t timer_id,
					  uint32_t interval_ms, uint8_t reload, void (*p_timer_callback)(void *));

/**
 * @brief
 * @param  p_handle:
 * @param  wait_ms:
 * @retval
 * @retval
 */
int rtos_timer_delete(rtos_timer_t p_handle, uint32_t wait_ms);

/**
 * @brief  For FreeRTOS, map to xTimerStart / xTimerStartFromISR
 * @param  p_handle:
 * @param  wait_ms:
 * @retval
 * @retval
 */
int rtos_timer_start(rtos_timer_t p_handle, uint32_t wait_ms);

/**
 * @brief
 * @param  p_handle:
 * @param  wait_ms:
 * @retval
 * @retval
 */
int rtos_timer_stop(rtos_timer_t p_handle, uint32_t wait_ms);

/**
 * @brief
 * @param  p_handle:
 * @param  interval_ms:
 * @param  wait_ms:
 * @retval
 * @retval
 */
int rtos_timer_change_period(rtos_timer_t p_handle, uint32_t interval_ms, uint32_t wait_ms);

/**
 * @brief  For FreeRTOS, map to xTimerIsTimerActive
 * Queries a timer to see if it is active or dormant. A timer will be dormant if:
 * 1) It has been created but not started, or
 * 2) It is an expired one-shot timer that has not been restarted.
 * @note   non-interrupt safety
 * @param  p_handle:
 * @retval
 */
uint32_t rtos_timer_is_timer_active(rtos_timer_t p_handle);

/**
 * @brief  For FreeRTOS, map to pvTimerGetTimerID
 * @note   non-interrupt safety
 * @param  p_handle:
 * @retval
 */
uint32_t rtos_timer_get_id(rtos_timer_t p_handle);

#endif
