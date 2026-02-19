/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
#include <assert.h>
#include <tinyara/pm/pm.h>
#include <errno.h>
#include <debug.h>

#include "pm.h"

#ifdef CONFIG_PM
/****************************************************************************
 * Name: pm_domain_unregister
 *
 * Description:
 *   This function is called to unregister a previously registered PM domain.
 *
 * Input parameters:
 *   domain - Pointer to the domain structure to be unregistered.
 *
 * Returned value:
 *    OK (0)   : On Success
 *    ERROR (-1): On Error (e.g., domain not found)
 *
 ****************************************************************************/
int pm_domain_unregister(struct pm_domain_s *domain)
{
	irqstate_t flags;

	if (domain == NULL) {
		set_errno(EINVAL);
		return ERROR;
	}

	flags = enter_critical_section();

	/* This enter_critical_section ensures that the pm_timedsuspend's
	 * timeout executes correctly
	 */
	if (domain->wdog) {
		wd_delete(domain->wdog);
		domain->wdog = NULL;
	}

	/* If domain is in suspended_domains queue, remove it */
	if (domain->suspend_count > 0) {
		dq_rem(&domain->suspended_node, &g_pmglobals.suspended_domains);
	}

	/* Remove the domain from the global list */
	dq_rem(&domain->node, &g_pmglobals.domains);
	g_pmglobals.ndomains--;

	leave_critical_section(flags);

	pmvdbg("Domain '%s' unregistered successfully\n", domain->name);

	/* At this point, the domain has been removed from all queues and wdog has
	 * been deleted. It is safe to free the domain structure memory.
	 */
	free(domain);

	return OK;
}

#endif /* CONFIG_PM */
