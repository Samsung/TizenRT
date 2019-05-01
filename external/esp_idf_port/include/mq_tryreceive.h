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

#ifndef ESP_MQ_TRY_RECEIVE_H
#define ESP_MQ_TRY_RECEIVE_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 * Included Files
 ************************************************************************/
#include "mqueue/mqueue.h"

	ssize_t mq_tryreceive_isr(mqd_t mqdes, FAR char *msg, size_t msglen, FAR int *prio);

#ifdef __cplusplus
}
#endif
#endif							/* ESP_MQ_TRY_RECEIVE_H */
