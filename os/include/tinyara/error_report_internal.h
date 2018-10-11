/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_ERROR_REPORT_INTERNAL_H
#define __INCLUDE_TINYARA_ERROR_REPORT_INTERNAL_H

/* This file will be used to provide definitions to support
 * error report framework
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdint.h>
#include <sys/types.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define ERROR_REPORT_DRVPATH     "/dev/errreport"
typedef struct {
	pid_t pid;
	unsigned long entry;
} thread_entry_t;

#ifdef CONFIG_ERROR_REPORT_INFINITE_WAIT
typedef struct {
	pid_t pid;
	uint8_t task_state;
	uint8_t count;
	uint32_t active_cpu;
} infinity_waitq_t;

typedef struct {
	pid_t pid;
	uint16_t ncalls;
	uint8_t task_state;
	unsigned long entry;	
	uint32_t backtrace[CONFIG_ERROR_REPORT_BACKTRACE_MAX_DEPTH];
} infinty_waitdata_t;
typedef void (*create_infwait_err_rec)(int pid, int state, unsigned long task_addr);
#endif

void error_report_drv_register(void);

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_TINYARA_ERROR_REPORT_INTERNAL_H */

