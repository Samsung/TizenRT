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
#ifndef ESP_QUEUE_API_H
#define ESP_QUEUE_API_H

#ifdef __cplusplus
extern "C" {
#endif
#include "esp_define.h"
int calc_abs_time(struct timespec *abs_time, int delayticks);
void *queue_create_wrapper(uint32_t queue_len, uint32_t item_size);
void queue_delete_wrapper(void *queue);
int32_t queue_send_wrapper(void *queue, void *item, uint32_t block_time_tick);
int32_t queue_send_from_isr_wrapper(void *queue, void *item, void *hptw);
int32_t queue_send_to_back_wrapper(void *queue, void *item, uint32_t block_time_tick);
int32_t queue_send_to_front_wrapper(void *queue, void *item, uint32_t block_time_tick);
int32_t queue_recv_wrapper(void *queue, void *item, uint32_t block_time_tick);
int32_t queue_recv_from_isr_wrapper(void *queue, void *item, int32_t * const hptw);
uint32_t queue_msg_waiting_wrapper(void *queue);

#ifdef __cplusplus
}
#endif

#endif							/* ESP_QUEUE_API_H */
