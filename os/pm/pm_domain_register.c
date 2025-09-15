/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#include <string.h>
#include <debug.h>
#include <tinyara/arch.h>

#include "pm.h"

#ifdef CONFIG_PM

/************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_domain_find
 *
 * Description:
 *   This function is called to find a PM domain structure by its name.
 *   The caller must hold the PM lock (pm_lock) before calling this function.
 *   This function can be called from IRQ context.
 *
 * Input parameters:
 *   domain_name - the string domain name to find.
 *
 * Returned value:
 *    Pointer to struct pm_domain_s : On Success
 *    NULL                         : On Error (domain not found)
 *
 ****************************************************************************/
FAR struct pm_domain_s *pm_domain_find(FAR const char *domain_name)
{
	FAR struct pm_domain_s *domain = NULL;
	FAR dq_entry_t *entry;
	irqstate_t flags;

	DEBUGASSERT(domain_name != NULL);

	flags = enter_critical_section();
	for (entry = dq_peek(&g_pmglobals.domains); entry != NULL; entry = dq_next(entry)) {
		domain = (FAR struct pm_domain_s *)entry;
		if (strncmp(domain->name, domain_name, CONFIG_PM_DOMAIN_NAME_SIZE) == 0) {
			leave_critical_section(flags);
			return domain;
		}
	}

	leave_critical_section(flags);
	return NULL;
}

/****************************************************************************
 * Name: pm_check_domain
 *
 * Description:
 *   This function is called inside PM internal APIs to check whether the
 *   domain pointer is valid.
 * 
 * Input Parameters:
 *   domain - Pointer to the domain structure
 *
 * Returned Value:
 *   0 (OK) - If domain is valid
 *  -1 (ERROR) - If domain is not valid
 *
 ****************************************************************************/
int pm_check_domain(FAR struct pm_domain_s *domain)
{
	if (domain == NULL) {
		set_errno(EINVAL);
		pmdbg("Invalid Domain Pointer: NULL\n");
		return ERROR;
	}
	/* Additional checks can be added here if necessary, e.g., if the domain
	 * is still in the list. For now, a non-NULL pointer is considered valid.
	 */
	return OK;
}

/****************************************************************************
 * Name: pm_domain_register
 *
 * Description:
 *   This function is called to get or register a PM domain.
 *   If a domain with the given name already exists, it returns the existing
 *   domain. Otherwise, it creates, initializes, registers a new domain,
 *   and returns it. This ensures that only one instance of a domain with
 *   a specific name exists.
 *   This function can be called from IRQ context, but if the domain does not
 *   exist, it will return an error because memory allocation cannot be performed
 *   in IRQ context.
 *
 * Input parameters:
 *   domain - the string domain name to be registered or found.
 *
 * Returned value:
 *    Pointer to struct pm_domain_s : On Success (existing or newly created)
 *    NULL                         : On Error (e.g., invalid name, memory allocation failure)
 *
 ****************************************************************************/
FAR struct pm_domain_s *pm_domain_register(FAR const char *domain)
{
	FAR struct pm_domain_s *new_domain = NULL;
	irqstate_t flags;
	int length;

	if (domain == NULL) {
		set_errno(EINVAL);
		pmdbg("Domain string is NULL\n");
		return NULL;
	}

	length = strlen(domain);
	if (length == 0 || length >= CONFIG_PM_DOMAIN_NAME_SIZE) {
		set_errno(E2BIG);
		return NULL;
	}

	/* Check if domain is already registered */
	new_domain = pm_domain_find(domain);
	if (new_domain != NULL) {
		return new_domain;
	}

	if (up_interrupt_context()) {
		set_errno(EPERM);
		return NULL;
	}

	/* Allocate memory for the new domain structure */
	new_domain = (FAR struct pm_domain_s *)kmm_malloc(sizeof(struct pm_domain_s));
	if (new_domain == NULL) {
		set_errno(ENOMEM);
		pmdbg("Unable to allocate memory for domain structure\n");
		return NULL;
	}

	/* Initialize the new domain structure */
	memset(new_domain, 0, sizeof(struct pm_domain_s));
	strncpy(new_domain->name, domain, CONFIG_PM_DOMAIN_NAME_SIZE - 1);
	new_domain->name[CONFIG_PM_DOMAIN_NAME_SIZE - 1] = '\0'; /* Ensure null termination */
	new_domain->suspend_count = 0;
	new_domain->wdog = NULL;
#ifdef CONFIG_PM_METRICS
	new_domain->stime = 0;
	new_domain->blocking_board_sleep_ticks = 0;
	new_domain->suspend_ticks = 0;
#endif

	flags = enter_critical_section();
	/* Add the new domain to the global list */
	dq_addlast(&new_domain->node, &g_pmglobals.domains);
	g_pmglobals.ndomains++;

	/* For newly registered domain initialize its pm metrics*/
	pm_metrics_update_domain(new_domain);

	leave_critical_section(flags);
	pmvdbg("Domain '%s' registered successfully\n", domain);
	return new_domain;
}


#endif /* CONFIG_PM */
