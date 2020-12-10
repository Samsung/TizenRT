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

#ifndef __DRIVERS_KERNEL_TEST_KERNEL_TEST_PROTO_H
#define __DRIVERS_KERNEL_TEST_KERNEL_TEST_PROTO_H
int test_task(int cmd, unsigned long arg);
int test_sem(int cmd, unsigned long arg);
int test_group(int cmd, unsigned long arg);
int test_clock(int cmd, unsigned long arg);
int test_timer(int cmd, unsigned long arg);
int test_sched(int cmd, unsigned long arg);
#ifndef CONFIG_DISABLE_SIGNALS
int test_signal(int cmd, unsigned long arg);
#endif
int test_compress_decompress(int cmd, unsigned long arg);
#ifdef CONFIG_ARMV8M_TRUSTZONE
int test_tz(void);
#endif
#ifdef CONFIG_EXAMPLES_STACK_PROTECTION
int test_kthread_stack_overflow_protection(int cmd, unsigned long arg);
#endif
#endif
