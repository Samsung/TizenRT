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

#include "pm.h"

#ifdef CONFIG_PM

/************************************************************************
 * Public Variables
 ************************************************************************/

/* This array maps the integer domain to its respective string domain */

char *pm_domain_map[CONFIG_PM_NDOMAINS];

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_check_domain
 *
 * Description:
 *   This function is called inside PM internal APIs to check whether the
 *   domain is valid or not.
 * 
 * Input Parameters:
 *   domain_id - ID of domain
 *
 * Returned Value:
 *   0 - If domain is valid
 *  -1 - If domain is not valid
 *
 ****************************************************************************/
int pm_check_domain(int domain_id)
{
	if (domain_id < 0 || domain_id >= CONFIG_PM_NDOMAINS || pm_domain_map[domain_id] == NULL) {
		set_errno(EINVAL);
		pmdbg("Invalid Domain: %d\n", domain_id);
		return ERROR;
	}
	return OK;
}

/****************************************************************************
 * Name: pm_domain_register
 *
 * Description:
 *   This function is called by a device driver in order to register domain inside PM.
 *
 * Input parameters:
 *   domain - the string domain need to be registered in PM.
 *
 * Returned value:
 *    non-negative integer   : ID of domain
 *    ERROR (-1)             : On Error
 *
 ****************************************************************************/

int pm_domain_register(char *domain)
{
	int index;
	irqstate_t flags;
	int length = strlen(domain);

	/* If domain string length is greater than max allowed then return error */
	if (length >= CONFIG_PM_DOMAIN_NAME_SIZE) {
		set_errno(E2BIG);
		pmdbg("Domain string length should be less than %d\n", CONFIG_PM_DOMAIN_NAME_SIZE);
		return ERROR;
	}
	/* Iterate over each domain ID and check if given domain is in our domain map or not */
	for (index = 0; index < CONFIG_PM_NDOMAINS; index++) {
		flags = enter_critical_section();
		/* If we have unused domain ID then use it to register given domain */
		if (pm_domain_map[index] == NULL) {
			pm_domain_map[index] = (char *)kmm_malloc(length * sizeof(char));
			if (!pm_domain_map[index]) {
				set_errno(ENOMEM);
				pmdbg("Unable to allocate memory from heap\n");
				leave_critical_section(flags);
				return ERROR;
			}
			strncpy(pm_domain_map[index], domain, length + 1);
#ifdef CONFIG_PM_METRICS
			/* For newly registered domain initialize its pm metrics*/
			pm_metrics_update_domain(index);
#endif
			goto EXIT;
		/* If domain is already registered then return registered domain ID */
		} else if (strncmp(pm_domain_map[index], domain, length + 1) == 0) {
			goto EXIT;
		}
		leave_critical_section(flags);
	}
	/* No more space available in map, so return ERROR (-1) */
	set_errno(ENOMEM);
	pmdbg("No space left to register new domain\n");
	return ERROR;
EXIT:
	leave_critical_section(flags);
	return index;
}

#endif /* CONFIG_PM */
