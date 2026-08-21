/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * apps/examples/testcase/ostest/nxevent.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/event.h>

#include <stdio.h>
#include <sched.h>
#include <pthread.h>

#include "ostest.h"

#define NXEVENT_TEST(expr, value) \
    ret = expr; \
    if (ret != value) \
      { \
        printf("ERROR nxevent test failed, line=%d ret=%x\n", __LINE__, ret); \
        ASSERT(false); \
      }

static void nxevent_yield(void)
{
  usleep(1);
}

static void nxevent_test_rule1(nxevent_t *event)
{
  int ret;

  /* Case 2.1: wait for nxevent_thread bringup
   * post == 0xf, wait == 0
   */

  nxevent_post(event, 0xf, 0);

  /* Case 2.2: post == ~0, wait == 0 */

  nxevent_yield();

  NXEVENT_TEST(nxevent_wait(event, 0, 0), ~0);

  /* Case 2.3: post == ~0, wait == ~0 */

  nxevent_yield();

  nxevent_post(event, ~0, 0);

  /* Case 2.4: post == 0, wait == ~0 */

  nxevent_yield();

  NXEVENT_TEST(nxevent_wait(event, ~0, 0), ~0);

  /* Case 2.5: post == 0xf, wait == 0xf */

  nxevent_yield();

  nxevent_post(event, 1 << 0, 0);
  nxevent_post(event, 1 << 1, 0);
  nxevent_post(event, 1 << 2, 0);
  nxevent_post(event, 1 << 3, 0);

  /* Case 2.6: post == 0xaaaaaaaa, wait == 0xaaaaaaaa */

  nxevent_yield();

  nxevent_post(event, 0xaaaaaaaa, 0);

  nxevent_yield();

  /* Case 2.7:
   * T0: wait 0x1
   * T1: post 0
   * T2: wait 0x2
   */

  nxevent_yield();

  nxevent_post(event, 0, 0);

  /* Wait 2 switch cycles */

  nxevent_yield();
  nxevent_yield();
}

static void nxevent_test_rule0(nxevent_t *event)
{
  int ret;

  /* Case 2.1: wait for nxevent_thread bringup
   * post == 0xf, wait == 0
   */

  NXEVENT_TEST(nxevent_wait(event, 0, 0), 0xf);

  /* Case 2.2: post == ~0, wait == 0 */

  nxevent_yield();

  nxevent_post(event, ~0, 0);

  /* Case 2.3: post == ~0, wait == ~0 */

  nxevent_yield();

  NXEVENT_TEST(nxevent_wait(event, ~0, 0), ~0);

  /* Case 2.4: post == 0, wait == ~0 */

  nxevent_yield();

  nxevent_post(event, 0, 0);

  /* Case 2.5: post == 0xf, wait == 0xf */

  nxevent_yield();

  NXEVENT_TEST(nxevent_wait(event, 0xf, NXEVENT_WAIT_ALL), 0xf);

  /* Case 2.6: post == 0xaaaaaaaa, wait == 0xaaaaaaaa */

  nxevent_yield();

  NXEVENT_TEST(nxevent_wait(event, 0xaaaaaaaa, 0), 0xaaaaaaaa);

  nxevent_yield();

  /* Case 2.7:
   * T0: wait 0x1
   * T1: post 0
   * T0: wait 0x2
   */

  NXEVENT_TEST(nxevent_wait(event, 0x1, 0), 0x1);

  nxevent_yield();

  NXEVENT_TEST(nxevent_wait(event, 0x2, 0), 0x2);

  NXEVENT_TEST(nxevent_wait(event, 0, 0), ~(0x1 | 0x2));

  nxevent_yield();
}

FAR static void *nxevent_thread(pthread_addr_t arg)
{
  nxevent_test_rule1(arg);
  nxevent_test_rule0(arg);
  nxevent_test_rule1(arg);
  nxevent_test_rule0(arg);
  nxevent_test_rule1(arg);
  return NULL;
}

FAR static void *nxevent_thread1(pthread_addr_t arg)
{
  nxevent_t *event = arg;
  int ret;

  /* Case 3.1: post == 0xff, wait0 == 0xf, wait1 == 0xf0 */

  NXEVENT_TEST(nxevent_wait(event, 0xf, 0), 0xf);

  /* Case 3.2: post0 == 0xf, post1 = 0xf0, wait == 0xff */

  nxevent_yield();

  nxevent_post(event, 0xf, 0);

  return NULL;
}

FAR static void *nxevent_thread2(pthread_addr_t arg)
{
  nxevent_t *event = arg;
  int ret;

  /* Case 3.1: post == 0xff, wait0 == 0xf, wait1 == 0xf0 */

  NXEVENT_TEST(nxevent_wait(event, 0xf0, 0), 0xf0);

  /* Case 3.2: post == 0xff, wait == 0xff */

  nxevent_yield();

  nxevent_post(event, 0xf0, 0);

  return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void nxevent_test(void)
{
  nxevent_t event = NXEVENT_INITIALIZER(event, ~0);
  struct sched_param sparam;
  pthread_attr_t attr;
  pthread_t tid;
  pthread_t tid1;
  pthread_t tid2;
  int ret;

  /* 1. Task Local Test */

  /* Case 1.1: init == ~0, wait == 0(ANY) */

  NXEVENT_TEST(nxevent_wait(&event, 0, 0), ~0);

  /* Case 1.2: init == ~0, wait == ~0 */

  nxevent_init(&event, ~0);

  NXEVENT_TEST(nxevent_wait(&event, ~0, 0), ~0);

  /* Case 1.3: post == ~0, wait == 0 */

  nxevent_post(&event, ~0, 0);

  NXEVENT_TEST(nxevent_wait(&event, 0, 0), ~0);

  /* Case 1.4: post == ~0, wait == ~0 */

  nxevent_post(&event, ~0, 0);

  NXEVENT_TEST(nxevent_wait(&event, ~0, 0), ~0);

  /* Case 1.5: post == 0xf, tickwait == ~0, tick 0 */

  nxevent_post(&event, 1 << 0, 0);
  nxevent_post(&event, 1 << 1, 0);
  nxevent_post(&event, 1 << 2, 0);
  nxevent_post(&event, 1 << 3, 0);

  NXEVENT_TEST(nxevent_tickwait(&event, ~0,
                                NXEVENT_WAIT_ALL, 0), 0);

  /* Case 1.6: post == 0xf, trywait == 0xf */

  NXEVENT_TEST(nxevent_trywait(&event, 0xf, 0), 0xf);

  nxevent_post(&event, 1 << 0, 0);
  nxevent_post(&event, 1 << 1, 0);
  nxevent_post(&event, 1 << 2, 0);
  nxevent_post(&event, 1 << 3, 0);

  /* Case 1.7: post == 0xf, wait == 0xf */

  NXEVENT_TEST(nxevent_wait(&event, 0xf, 0), 0xf);

  /**************************************************************************/

  /* 2. Task notify Test */

  pthread_attr_init(&attr);

  /* Lower priority */

  sparam.sched_priority = PTHREAD_DEFAULT_PRIORITY;
  pthread_attr_setschedparam(&attr, &sparam);

  /* Create thread */

  pthread_create(&tid, &attr, nxevent_thread, &event);

  nxevent_test_rule0(&event);
  nxevent_test_rule1(&event);
  nxevent_test_rule0(&event);
  nxevent_test_rule1(&event);
  nxevent_test_rule0(&event);

  pthread_join(tid, NULL);

  /**************************************************************************/

  /* 3. Multi Task notify Test */

  pthread_attr_init(&attr);

  /* Lower priority */

  sparam.sched_priority = PTHREAD_DEFAULT_PRIORITY;
  pthread_attr_setschedparam(&attr, &sparam);

  /* Create thread */

  event.events = 0;
  pthread_create(&tid1, &attr, nxevent_thread1, &event);
  pthread_create(&tid2, &attr, nxevent_thread2, &event);

  /* Case 3.1: post == 0xff, wait0 == 0xf, wait1 == 0xf0 */

  nxevent_yield();

  nxevent_post(&event, 0xff, 0);

  nxevent_yield();

  /* Case 3.2: post == 0xff, wait == 0xff */

  NXEVENT_TEST(nxevent_wait(&event, 0xff, NXEVENT_WAIT_ALL), 0xff);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  /**************************************************************************/

  /* 4. Event clear Test */

  nxevent_post(&event, 0xff, NXEVENT_POST_SET);

  /* Case 4.1: clear == 0, trywait == 0xf, wait == 0xf */

  nxevent_clear(&event, 0);
  NXEVENT_TEST(nxevent_wait(&event, 0xf, NXEVENT_WAIT_NOCLEAR), 0xf);
  NXEVENT_TEST(nxevent_trywait(&event, 0xf, NXEVENT_WAIT_NOCLEAR), 0xf);

  /* Case 4.2: clear == 0xf, trywait == 0xf */

  nxevent_clear(&event, 0xf);
  NXEVENT_TEST(nxevent_trywait(&event, 0xf, NXEVENT_WAIT_NOCLEAR), 0);

  /* Case 4.3: clear == 0, trywait == 0xf0, wait == 0xf0 */

  nxevent_clear(&event, 0);
  NXEVENT_TEST(nxevent_wait(&event, 0xf0, NXEVENT_WAIT_NOCLEAR), 0xf0);
  NXEVENT_TEST(nxevent_trywait(&event, 0xf0, NXEVENT_WAIT_NOCLEAR), 0xf0);

  /* Case 4.4: clear == 0xf0, wait == 0xf0 */

  nxevent_clear(&event, 0xf0);
  NXEVENT_TEST(nxevent_trywait(&event, 0xf0, NXEVENT_WAIT_NOCLEAR), 0);

  /**************************************************************************/

  /* 5. Event get mask Test */

  /* Case 5.1: post == 0xff */

  nxevent_post(&event, 0xff, NXEVENT_POST_SET);
  NXEVENT_TEST(nxevent_getmask(&event), 0xff)

  /* Case 5.2: clear == 0xf */

  nxevent_clear(&event, 0xf);
  NXEVENT_TEST(nxevent_getmask(&event), 0xf0)

  /* Case 5.3: clear == 0xf0 */

  nxevent_clear(&event, 0xf0);
  NXEVENT_TEST(nxevent_getmask(&event), 0)

  nxevent_reset(&event, 0);
  nxevent_destroy(&event);
}
