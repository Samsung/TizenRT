/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef __EXAMPLES_HEAVY_SIGNAL_MESSAGE_TEST_HEAVY_SIGNAL_MESSAGE_TEST_H__
#define __EXAMPLES_HEAVY_SIGNAL_MESSAGE_TEST_HEAVY_SIGNAL_MESSAGE_TEST_H__

#include <tinyara/config.h>

#define HEAVY_SIGNAL_MESSAGE_TEST_MQ                    "safe_mq"
#define HEAVY_SIGNAL_MESSAGE_TEST_SIG_MQ                "safe_sig_mq"
#define HEAVY_SIGNAL_MESSAGE_TEST_SEND_STOP_MQ          "send_stop_mq"
#define HEAVY_SIGNAL_MESSAGE_TEST_RECEIVE_STOP_MQ       "receive_stop_mq"

#define HEAVY_SIGNAL_MESSAGE_TEST_MAX_MSG               16
#define HEAVY_SIGNAL_MESSAGE_TEST_MQ_NAME_MAX           16
#define HEAVY_SIGNAL_MESSAGE_TEST_USLEEP                100000
#define HEAVY_SIGNAL_MESSAGE_TEST_NSLEEP                (HEAVY_SIGNAL_MESSAGE_TEST_USLEEP * 1000)
#define HEAVY_SIGNAL_MESSAGE_TEST_MAX_NSLEEP            1000000000
#define HEAVY_SIGNAL_MESSAGE_TEST_SIG_WAIT_NO           1
#define HEAVY_SIGNAL_MESSAGE_TEST_SIG_TIME_WAIT_NO      2
#define HEAVY_SIGNAL_MESSAGE_TEST_BUFFER_SIZE           600

#define HEAVY_SIGNAL_MESSAGE_TEST_TASK_NUM              (CONFIG_MAX_TASKS / 16)

#define HEAVY_SIGNAL_MESSAGE_TEST_STOP                  0

void heavy_signal_message_test_receiver(void);
void heavy_signal_message_test_sender(void);
void heavy_signal_message_test_stop(void);

#endif /* __EXAMPLES_HEAVY_SIGNAL_MESSAGE_TEST_HEAVY_SIGNAL_MESSAGE_TEST_H__ */
