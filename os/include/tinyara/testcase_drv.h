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

#ifndef __INCLUDE_TINYARA_TEST_DRV_H
#define __INCLUDE_TINYARA_TEST_DRV_H

/* This file will be used to provide definitions to support
 * kernel test case framework
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/fs/ioctl.h>

#ifdef CONFIG_KERNEL_TEST_DRV

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration
 *
 * CONFIG_KERNEL_TEST_DRV - Enables testcase driver support
 */

/* IOCTL Commands */
/* The testcase_drv module uses ioctl commands to identify the kernel module for which
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
#define TESTIOC_CLOCK_ABSTIME2TICKS            _TESTIOC(2)
#define TESTIOC_GET_SIG_FINDACTION_ADD         _TESTIOC(3)
#define TESTIOC_GET_SELF_PID                   _TESTIOC(4)
#define TESTIOC_IS_ALIVE_THREAD                _TESTIOC(5)
#define TESTIOC_GET_TCB_SIGPROCMASK            _TESTIOC(6)
#define TESTIOC_GET_TCB_ADJ_STACK_SIZE         _TESTIOC(7)
#ifdef CONFIG_TC_KERNEL_ROUNDROBIN
#define TESTIOC_GET_TCB_TIMESLICE              _TESTIOC(8)
#endif
#define TESTIOC_SCHED_FOREACH                  _TESTIOC(9)
#define TESTIOC_SIGNAL_PAUSE                   _TESTIOC(10)
#define TESTIOC_TIMER_INITIALIZE_TEST          _TESTIOC(11)
#define TESTIOC_SEM_TICK_WAIT_TEST             _TESTIOC(12)

#define KERNEL_TC_DRVPATH                       "/dev/testcase"

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
 *   This function creates a device node like "/dev/testcase" which will be used
 *   by the testcases that execute kernel side APIs
 *
 *
 ****************************************************************************/

void kernel_test_drv_register(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_KERNEL_TEST_DRV */
#endif							/* __INCLUDE_TINYARA_TEST_FW_H */
