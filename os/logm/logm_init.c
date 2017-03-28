/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
#include <debug.h>
#include <tinyara/config.h>
#include <tinyara/logm.h>
#include <sys/types.h>
#include "logm.h"
#ifdef CONFIG_LOGM_TEST
#include "logm_test.h"
#endif

pid_t g_logm_tid;

void logm_start(void)
{
	int priority = LOGM_TASK_PRORITY;
	int stacksize = LOGM_TASK_STACKSIZE;

	g_logm_tid = task_create("logm", priority, stacksize, logm_task, NULL);

	if (!g_logm_tid) {
		LOGM_PRINTERR_AND_RETURN();
	}
#ifdef CONFIG_TASH
	logm_register_tashcmds();
#endif
}
