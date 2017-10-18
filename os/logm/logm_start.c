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
#include <tinyara/kthread.h>
#include "logm.h"

void logm_start(void)
{
	int pid;

	pid = kernel_thread("logm", LOGM_TASK_PRORITY, LOGM_TASK_STACKSIZE, logm_task, NULL);

	if (pid < 0) {
		return;
	}

#ifdef CONFIG_TASH
	/* Register tash commands for logm */
	logm_register_tashcmds();
#endif
}
