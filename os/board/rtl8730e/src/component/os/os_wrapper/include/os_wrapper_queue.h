/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __OS_WRAPPER_QUEUE_H__
#define __OS_WRAPPER_QUEUE_H__

/**
 * @brief  queue handle type
 */
typedef void *rtos_queue_t;

/**
 * @brief  For FreeRTOS, map to xQueueCreate
 * @note   Usage example:
 * Create:
 *          rtos_queue_t queue_handle;
 *          rtos_queue_create(&queue_handle, 5, sizeof(uint32_t));
 * Send:
 *          rtos_queue_send(queue_handle, p_msg, portMAX_DELAY);
 * Receive:
 *          rtos_queue_receive(queue_handle, p_msg, portMAX_DELAY);
 * Delete:
 *          rtos_queue_delete(queue_handle);
 * @param  pp_handle: The handle itself is a pointer, and the pp_handle means a pointer to the handle.
 * @param  msg_num:
 * @param  msg_size:
 * @retval
 */
int rtos_queue_create(rtos_queue_t *pp_handle, uint32_t msg_num, uint32_t msg_size);

/**
 * @brief  For FreeRTOS, map to vQueueDelete
 * @param  p_handle:
 * @retval
 */
int rtos_queue_delete(rtos_queue_t p_handle);

/**
 * @brief  For FreeRTOS, map to uxQueueMessagesWaiting / uxQueueMessagesWaitingFromISR
 * @param  p_handle:
 * @retval
 */
uint32_t rtos_queue_message_waiting(rtos_queue_t p_handle);

/**
 * @brief  For FreeRTOS, map to xQueueSendToBack / xQueueSendToBackFromISR
 * @param  p_handle:
 * @param  p_msg:
 * @param  wait_ms:
 * @retval
 */
int rtos_queue_send(rtos_queue_t p_handle, void *p_msg, uint32_t wait_ms);

/**
 * @brief  For FreeRTOS, map to xQueueSendToFront / xQueueSendToFrontFromISR
 * @param  p_handle:
 * @param  p_msg:
 * @param  wait_ms:
 * @retval
 */
int rtos_queue_send_to_front(rtos_queue_t p_handle, void *p_msg, uint32_t wait_ms);

/**
 * @brief  For FreeRTOS, map to xQueueReceive / xQueueReceiveFromISR
 *         Messages are received from the queue and removed from the queue, so the queue's state changes.
 * @param  p_handle:
 * @param  p_msg:
 * @param  wait_ms:
 * @retval
 */
int rtos_queue_receive(rtos_queue_t p_handle, void *p_msg, uint32_t wait_ms);

/**
 * @brief  For FreeRTOS, map to xQueuePeek / xQueuePeekFromISR
 *         Simply viewing the next message in the queue does not remove it from the queue,
 *         so the state of the queue remains unchanged.
 * @param  p_handle:
 * @param  p_msg:
 * @param  wait_ms:
 * @retval
 */
int rtos_queue_peek(rtos_queue_t p_handle, void *p_msg, uint32_t wait_ms);

#endif
