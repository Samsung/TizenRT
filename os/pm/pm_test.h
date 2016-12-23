/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef __OS_PM_PM_TEST_H
#define __OS_PM_PM_TEST_H

#include <tinyara/config.h>

#define PMTEST_DEVICES (3)
#define PMTEST_DOMAIN (0)
#define PMTEST_THREAD_PRIORITY (100)
#define PMTEST_THREAD_STACKSIZE (1024)
#define PMTEST_THREAD_SLEEP_TIME (10)
#define PMTEST_DURATION_IN_SECS (180)

#ifdef CONFIG_PM_TEST
void pmtest_init(void);
#else
#define pmtest_init(void)
#endif

#endif
