/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __OS_WRAPPER_TASK_H__
#define __OS_WRAPPER_TASK_H__

#define RTOS_TASK_MAX_PRIORITIES         (11)
#define RTOS_MINIMAL_STACK_SIZE          (1024)
#define RTOS_MINIMAL_SECURE_STACK_SIZE   (1024)

#define RTOS_SCHED_SUSPENDED             0x0UL
#define RTOS_SCHED_NOT_STARTED           0x1UL
#define RTOS_SCHED_RUNNING               0x2UL

/**
 * @brief  task handle and function type
 */
typedef void *rtos_task_t;
typedef void (*rtos_task_function_t)(void *);

/**
 * @brief  Start os kernel scheduler
 * @retval For FreeRTOS, return SUCCESS
 */
int rtos_sched_start(void);

/**
 * @brief  Stop os kernel scheduler
 * @retval For FreeRTOS, return SUCCESS
 */
int rtos_sched_stop(void);

/**
 * @brief  Suspend os kernel scheduler
 * @retval For FreeRTOS, return SUCCESS
 */
int rtos_sched_suspend(void);

/**
 * @brief  Resume os kernel scheduler
 * @retval For FreeRTOS, return SUCCESS
 */
int rtos_sched_resume(void);

/**
 * @brief  Get scheduler status. For FreeRTOS, map to xTaskGetSchedulerState
 * @retval RTOS_SCHED_SUSPENDED / RTOS_SCHED_NOT_STARTED / RTOS_SCHED_RUNNING
 */
int rtos_sched_get_state(void);

/**
 * @brief  Create os level task routine. For FreeRTOS, map to xTaskCreate
 * @note   Usage example:
 * Create:
 *          rtos_task_t handle;
 *          rtos_task_create(&handle, "test_task", task, NULL, 1024, 2);
 * Suspend:
 *          rtos_task_suspend(handle);
 * Resume:
 *          rtos_task_resume(handle);
 * Delete:
 *          rtos_task_delete(handle);
 * @param  pp_handle: The handle itself is a pointer, and the pp_handle means a pointer to the handle.
 * @param  p_name:
 * @param  p_routine:
 * @param  p_param:
 * @param  stack_size_in_byte:
 * @param  priority:
 * @retval SUCCESS(0) / FAIL(-1)
 */
int rtos_task_create(rtos_task_t *pp_handle, const char *p_name, rtos_task_function_t p_routine,
					 void *p_param, uint16_t stack_size_in_byte, uint16_t priority);

/**
 * @brief  Delete os level task routine. For FreeRTOS, map to vTaskDelete
 * @param  p_handle: Task handle. If a null pointer is passed, the task itself is deleted.
 * @retval For FreeRTOS, return SUCCESS
 */
int rtos_task_delete(rtos_task_t p_handle);

/**
 * @brief  Suspend os level task routine. For FreeRTOS, map to vTaskSuspend
 * @param  p_handle:
 * @retval For FreeRTOS, return SUCCESS
 */
int rtos_task_suspend(rtos_task_t p_handle);

/**
 * @brief  Resume os level task routine. For FreeRTOS, map to vTaskResume
 * @param  p_handle:
 * @retval For FreeRTOS, return SUCCESS
 */
int rtos_task_resume(rtos_task_t p_handle);

/**
 * @brief  Yield current os level task routine. For FreeRTOS, map to taskYIELD
 * @retval For FreeRTOS, return SUCCESS
 */
int rtos_task_yield(void);

/**
 * @brief  Get current os level task routine handle. For FreeRTOS, map to xTaskGetCurrentTaskHandle
 * @param  pp_handle: The handle itself is a pointer, and the pp_handle means a pointer to the handle.
 * @retval The task handle pointer
 */
rtos_task_t rtos_task_handle_get(void);

/**
 * @brief  Get os level task routine priority. For FreeRTOS, map to uxTaskPriorityGet
 * @param  p_handle:
 * @retval Task priority value
 */
uint32_t rtos_task_priority_get(rtos_task_t p_handle);

/**
 * @brief  Set os level task routine priority. For FreeRTOS, map to vTaskPrioritySet
 * @param  p_handle:
 * @param  priority:
 * @retval
 */
int rtos_task_priority_set(rtos_task_t p_handle, uint16_t priority);

/**
 * @brief  Print current task status
 */
void rtos_task_out_current_status(void);

/**
 * @brief  Allocate a secure context for the task. For FreeRTOS, map to portALLOCATE_SECURE_CONTEXT
 * @note   FreeRTOS version must be greater than 10.2.0, and enable configENABLE_TRUSTZONE
 *         Otherwise it's just an empty implementation.
 */
void rtos_create_secure_context(uint32_t size);

#endif
