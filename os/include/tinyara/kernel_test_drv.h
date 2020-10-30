/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_KERNEL_TEST_DRV_H
#define __INCLUDE_TINYARA_KERNEL_TEST_DRV_H

/* This file will be used to provide definitions to support
 * kernel test case framework
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/fs/ioctl.h>

#ifdef CONFIG_DRIVERS_KERNEL_TEST

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration
 *
 * CONFIG_DRIVERS_KERNEL_TEST - Enables kernel_test driver support
 */

/* IOCTL Commands */
/* The kernel_test_drv module uses ioctl commands to identify the kernel module for which
 * test cases are to be run. The ioctl command may be accompanied by and arguement to
 * indicate which particular API  in the module is to be tested or which particular
 * test scenario is to be run
 *
 * TESTIOC_TEST_DRIVER_ANALOG - Run test cases for os/drivers/analog module
 *
 *   ioctl argument:  Integer (enum or DEFINE value) indicating the particular test case that is to be run
 *
 */

#define TESTIOC_ANALOG                         _TESTIOC(1)
#define TESTIOC_GET_SIG_FINDACTION_ADD         _TESTIOC(2)
#define TESTIOC_GET_SELF_PID                   _TESTIOC(3)
#define TESTIOC_IS_ALIVE_THREAD                _TESTIOC(4)
#define TESTIOC_GET_TCB_SIGPROCMASK            _TESTIOC(5)
#define TESTIOC_GET_TCB_ADJ_STACK_SIZE         _TESTIOC(6)
#define TESTIOC_SCHED_FOREACH                  _TESTIOC(8)
#define TESTIOC_SIGNAL_PAUSE                   _TESTIOC(9)
#define TESTIOC_CLOCK_ABSTIME2TICKS_TEST       _TESTIOC(10)
#define TESTIOC_TIMER_INITIALIZE_TEST          _TESTIOC(11)
#define TESTIOC_SEM_TICK_WAIT_TEST             _TESTIOC(12)
#define TESTIOC_TASK_REPARENT                  _TESTIOC(13)
#if defined(CONFIG_SCHED_HAVE_PARENT) && defined(CONFIG_SCHED_CHILD_STATUS)
#define TESTIOC_GROUP_ADD_FINED_REMOVE_TEST    _TESTIOC(14)
#define TESTIOC_GROUP_ALLOC_FREE_TEST          _TESTIOC(15)
#define TESTIOC_GROUP_EXIT_CHILD_TEST          _TESTIOC(16)
#define TESTIOC_GROUP_REMOVECHILDREN_TEST      _TESTIOC(17)
#endif
#define TESTIOC_TASK_INIT_TEST                 _TESTIOC(18)
#define TESTIOC_COMPRESSION_TEST	        _TESTIOC(19)
#ifdef CONFIG_EXAMPLES_MPU_TEST
#define TESTIOC_MPUTEST				_TESTIOC(20)
#endif
#ifdef CONFIG_ARMV8M_TRUSTZONE
#define TESTIOC_TZ				_TESTIOC(21)
#endif
#ifdef CONFIG_EXAMPLES_STACK_PROTECTION
#define TESTIOC_KTHREAD_STACK_PROTECTION_TEST	_TESTIOC(22)
#endif

#define KERNEL_TEST_DRVPATH                       "/dev/kernel_test"

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: kernel_test_drv_register
 *
 * Description:
 *   This function creates a device node like "/dev/kernel_test" which will be used
 *   by the tests that execute kernel side APIs
 *
 *
 ****************************************************************************/

void kernel_test_drv_register(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_DRIVERS_KERNEL_TEST */
#endif							/* __INCLUDE_TINYARA_KERNEL_TEST_DRV_H */
