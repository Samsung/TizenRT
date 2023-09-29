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

#ifndef __DRIVERS_OS_API_TEST_KERNEL_TEST_PROTO_H
#define __DRIVERS_OS_API_TEST_KERNEL_TEST_PROTO_H
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
#ifdef CONFIG_TC_NET_PBUF
int test_net_pbuf(int cmd, unsigned long arg);
#endif
#if defined(CONFIG_AUTOMOUNT_USERFS) && defined(CONFIG_EXAMPLES_TESTCASE_FILESYSTEM)
int test_fs_get_devname(void);
#endif
#ifdef CONFIG_APP_BINARY_SEPARATION
#ifdef CONFIG_SYSTEM_DEBUG_ANALYSER_TOOL
#ifdef CONFIG_SYSTIME_TIME64
uint64_t test_get_app_start_time_sec(int cmd, unsigned long arg);
#else
uint32_t test_get_app_start_time_sec(int cmd, unsigned long arg);
#endif
uint8_t test_mpu_get_active_regions(int cmd, unsigned long arg);
#endif
#ifdef CONFIG_SAVE_BIN_SECTION_ADDR
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
uint8_t test_get_app_memory_addr(int cmd, unsigned long arg);
#endif
#endif
#endif
#ifdef CONFIG_STACK_COLORATION
uint8_t test_tcb_info(int cmd, unsigned long arg);
#endif
uint8_t test_gtasklist_info(int cmd, unsigned long arg);
#ifdef CONFIG_ARCH_STACKDUMP
uint8_t test_tcb_callstack_info(int cmd, unsigned long arg);
#endif
#endif
