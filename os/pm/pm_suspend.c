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
/****************************************************************************
 * pm/pm_activity.c
 *
 *   Copyright (C) 2011-2012, 2016 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <tinyara/pm/pm.h>
#include <tinyara/irq.h>

#include <queue.h>
#include <errno.h>

#include "pm.h"

#ifdef CONFIG_PM

/****************************************************************************
 * External Definitons
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_suspend
 *
 * Description:
 *   This function is called by a device driver to indicate that it is
 *   performing meaningful activities (non-idle), and the domain should not
 *   enter a low-power state.
 *   This function can be called from IRQ context.
 *
 * Input Parameters:
 *   domain - Pointer to the domain structure
 *
 *     As an example, media player might stay in normal state during playback.
 *
 * Returned Value:
 *   OK (0) on success; ERROR (-1) on failure with errno set appropriately.
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/

int pm_suspend(FAR struct pm_domain_s *domain)
{
	irqstate_t flags;
	int ret = OK;

	flags = enter_critical_section();

	/* pm_check_domain is implicitly handled by checking for NULL domain pointer
	 * and ensuring the domain structure is valid.
	 * If more checks are needed, pm_check_domain(domain) can be called here.
	 */
	ret = pm_check_domain(domain);
	if (ret != OK) {
		goto errout;
	}

	if (domain->suspend_count >= UINT16_MAX) {
		ret = ERROR;
		set_errno(ERANGE);
		goto errout;
	}

	pm_metrics_update_suspend(domain);

	domain->suspend_count++;

	/* If this is the first suspend for this domain, add it to suspended_domains queue */
	if (domain->suspend_count == 1) {
		dq_addlast(&domain->suspended_node, &g_pmglobals.suspended_domains);
	}

errout:
	leave_critical_section(flags);
	return ret;
}
#endif /* CONFIG_PM */
