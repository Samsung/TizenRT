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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <errno.h>
#include <assert.h>
#include <semaphore.h>

#include "pm.h"

#ifdef CONFIG_PM

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_lock
 *
 * Description:
 *   Lock the power management registry. This function will block until
 *   the semaphore is acquired. It automatically retries if interrupted
 *   by a signal (EINTR).
 *
 * Returned Value:
 *   OK (0) on success.
 *
 ****************************************************************************/

int pm_lock(void)
{
	int ret;

	do {
		ret = sem_wait(&g_pmglobals.regsem);
		DEBUGASSERT(ret == OK || errno == EINTR);
	} while (ret < 0);

	return OK;
}

#endif /* CONFIG_PM */
