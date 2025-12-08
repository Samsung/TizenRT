/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef __OSDEP_SERVICE_QUEUE_H_
#define __OSDEP_SERVICE_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif
#define RTW_WAIT_FOREVER		0xFFFFFFFF

typedef void	            *_queueHandle;
typedef void	            *_xqueue;

#define netif_queue_stopped(dev)	(0)
#define netif_wake_queue(dev)		do { } while (0)

/*************************** Queues *******************************/

/**
 * @brief  This function creates a new queue instance.
 * @param[in] queue: The handle to the newly created queue.
 * @param[in] name: The name of the queue
 * @param[in] message_size: The number of bytes each message in the queue will require.
 * @param[in] number_of_messages: The maximum number of messages that kthe queue can contain.
 * @return	  0: Creating queue success
 * @return	  -1: Creating queue fail
 */
int rtw_init_xqueue(_xqueue *queue, const char *name, uint32_t message_size, uint32_t number_of_messages);

/**
 * @brief  This function posts a message to the back of a queue.
 *		   The message is queued by copy, not by reference.
 * @param[in] queue: The handle to the queue on which the message is to be posted.
 * @param[in] message: The pointer to the message that is to be placed on the queue.
 * @param[in] timeout_ms: The maximum amout of time the task should block waiting for
 			              the space to become available on the queue, should it already be full.
 			              The time is defined in ms.
 * @return	  0: The message was successfully posted.
 * @return	  -1: The message was not posted.
 */
int rtw_push_to_xqueue(_xqueue *queue, void *message, uint32_t timeout_ms);

/**
 * @brief  This function receives a message from a queue.
 *		   The message is recieved by copy so a buffer adequate size must be provided.
 * @param[in] queue: The handle to the queue from which the message is to be received.
 * @param[in] message: The pointer to the buffer into which the received message will be copied.
 * @param[in] timeout_ms: The maximum amout of time the task should block waiting for a message to
 *						  receive should the queue be empty at the time of the call.
 			              The time is defined in ms.
 * @return	  0: A message was successfully received from the queue.
 * @return	  -1: No message was received from the queue.
 */
int rtw_pop_from_xqueue(_xqueue *queue, void *message, uint32_t timeout_ms);

int rtw_peek_from_xqueue(_xqueue *queue, void *message, uint32_t timeout_ms);

/**
 * @brief  Delete a queue - freeing all the memory allocated for storing of messages placed on the queue.
 * @param[in] queue: The handle to the queue to be deleted.
 * @return	  0: The queue was successfully deleted.
 * @return	  -1: The queue was not empty so cannot be deleted.
 */
int rtw_deinit_xqueue(_xqueue *queue);

/**
 * @brief  This function creates a new queue instance.
 * @param[in] pqueue: The handle to the newly created queue.
 * @return	 None
 */
void	rtw_init_queue(_queue	*pqueue);

/**
 * @brief  This function tests whether the queue is empty.
 * @param[in] pqueue: The handle to the queue to be tested.
 * @return	 None
 */
uint32_t	rtw_queue_empty(_queue	*pqueue);

/**
 * @brief  This function tests whether the "pelement" is at the "queue".
 * @param[in] queue: The pointer to the queue that to be tested.
 * @param[in] pelement: The element that to be tested.
 * @return	 _TRUE/_FALSE
 */
uint32_t	rtw_end_of_queue_search(_list *queue, _list *pelement);
/*************************** End Queues *******************************/
#ifdef __cplusplus
}
#endif

#endif
