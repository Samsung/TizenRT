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
/****************************************************************************
 * apps/tzshlib/tzsh_vars.c
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
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

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "tzsh.h"
#include "tzsh_console.h"

#ifdef CONFIG_TZSH_VARS

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_cmpname
 ****************************************************************************/

static bool tzsh_cmpname(const char *pszname, const char *peqname)
{
	/* Search until we find anything different in the two names */

	for (; *pszname == *peqname; pszname++, peqname++) {
	}

	/* On success, pszname will end with '\0' and peqname with '=' */

	if (*pszname == '\0' && *peqname == '=') {
		return true;
	}

	return false;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_findvar
 *
 * Description:
 *   Search the provided TZSH console structure for the variable of the
 *   specified name.
 *
 * Input Parameters:
 *   pstate - The console state containing TZSH variable array to be searched.
 *   name - The variable name to find
 *
 * Returned Value:
 *   A pointer to the name=value string in the TZSH variable buffer
 *
 * Assumptions:
 *   - Not called from an interrupt handler
 *   - Pre-emption is disabled by caller
 *
 ****************************************************************************/

FAR char *tzsh_findvar(FAR struct console_stdio_s *pstate, FAR const char *name)
{
	FAR char *ptr;
	FAR char *end;

	/* Verify input parameters */

	DEBUGASSERT(pstate != NULL && name != NULL);

	/* Search for a name=value string with matching name */

	end = &pstate->varp[pstate->varsz];
	for (ptr = pstate->varp; ptr < end && !tzsh_cmpname(name, ptr); ptr += (strlen(ptr) + 1)) ;

	/* Check for success */

	return (ptr < end) ? ptr : NULL;
}

/****************************************************************************
 * Name: tzsh_removevar
 *
 * Description:
 *   Remove the referenced name=value pair from the TZSH variable buffer
 *
 * Input Parameters:
 *   pstate - The task pstate with the TZSH variable buffer containing the
 *            name=value pair
 *   pair  - A pointer to the name=value pair in the restroom
 *
 * Returned Value:
 *   Zero on success
 *
 ****************************************************************************/

int tzsh_removevar(FAR struct console_stdio_s *pstate, FAR char *pair)
{
	FAR char *end;				/* Pointer to the end+1 of the TZSH variable buffer */
	int alloc;					/* Size of the allocated TZSH variable buffer */
	int ret = ERROR;

	/* Verify that the pointer lies within the TZSH variable region */

	alloc = pstate->varsz;		/* Size of the allocated TZSH variable buffer */
	end = &pstate->varp[alloc];	/* Pointer to the end+1 of the TZSH variable buffer */

	if (pair >= pstate->varp && pair < end) {
		/* Set up for the removal */

		int len = strlen(pair) + 1;	/* Length of name=value string to remove */
		FAR char *src = &pair[len];	/* Address of name=value string after */
		FAR char *dest = pair;	/* Location to move the next string */
		int count = end - src;	/* Number of bytes to move (might be zero) */

		/* Move all of the TZSH variable strings after the removed one 'down.'
		 * this is inefficient, but probably not high duty.
		 */

		while (count-- > 0) {
			*dest++ = *src++;
		}

		/* Then set to the new allocation size.  The caller is expected to
		 * call realloc at some point but we don't do that here because the
		 * caller may add more stuff to the TZSH variable buffer.
		 */

		pstate->varsz -= len;
		ret = OK;
	}

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_getvar, tzsh_setvar, and tzsh_setvar
 *
 * Description:
 *   Get, set, or unset an TZSH variable.
 *
 * Input Parameters:
 *   vtbl  - TZSH session data
 *   name  - The name of the variable to get or set
 *   value - The value to use with tzsh_setvar()
 *
 * Returned value:
 *   tzsh_getvar() returns a read-only reference to the variable value on
 *   success or NULL on failure.
 *   nset_unsetvar() returns OK on success or an negated errno value on
 *   failure.
 *
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_getvar
 ****************************************************************************/

FAR char *tzsh_getvar(FAR struct tzsh_vtbl_s *vtbl, FAR const char *name)
{
	FAR struct console_stdio_s *pstate = (FAR struct console_stdio_s *)vtbl;
	FAR char *pair;
	FAR char *value = NULL;

	DEBUGASSERT(pstate != NULL && name != NULL);

	/* Check if the variable exists */

	if ((pair = tzsh_findvar(pstate, name)) == NULL) {
		return NULL;
	}

	/* It does!  Get the value sub-string from the name=value string */

	value = strchr(pair, '=');
	DEBUGASSERT(value != NULL);

	/* Adjust the pointer so that it points to the value right after the '=' */

	value++;
	return value;
}

/****************************************************************************
 * Name: tzsh_setvar
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_SET
int tzsh_setvar(FAR struct tzsh_vtbl_s *vtbl, FAR const char *name, FAR const char *value)
{
	FAR struct console_stdio_s *pstate = (FAR struct console_stdio_s *)vtbl;
	FAR char *pair;
	FAR char *newvarp;
	int newsize;
	int varlen;

	DEBUGASSERT(pstate != NULL && name != NULL && value != NULL);

	/* Check if the variable already exists */

	if (pstate->varp != NULL && (pair = tzsh_findvar(pstate, name)) != NULL) {
		/* Yes.. Remove the name=value pair from the TZSH variable buffer.  It
		 * will be added again below.
		 */

		(void)tzsh_removevar(pstate, pair);
	}

	/* Get the size of the new name=value string.  The +2 is for the '=' and for
	 * null terminator
	 */

	varlen = strlen(name) + strlen(value) + 2;

	/* Then allocate or reallocate the TZSH variable buffer */

	if (pstate->varp != NULL) {
		newsize = pstate->varsz + varlen;
		newvarp = (FAR char *)realloc(pstate->varp, newsize);
		if (newvarp == NULL) {
			return -ENOMEM;
		}

		pair = &newvarp[pstate->varsz];
	} else {
		newsize = varlen;
		newvarp = (FAR char *)malloc(varlen);
		if (!newvarp) {
			return -ENOMEM;
		}

		pair = newvarp;
	}

	/* Save the new buffer and size */

	pstate->varp = newvarp;
	pstate->varsz = newsize;

	/* Now, put the new name=value string into the TZSH variable buffer */

	sprintf(pair, "%s=%s", name, value);
	return OK;
}
#endif

/****************************************************************************
 * Name: tzsh_unsetvar
 ****************************************************************************/

#if !defined(CONFIG_TZSH_DISABLE_UNSET) || !defined(CONFIG_TZSH_DISABLE_EXPORT)
int tzsh_unsetvar(FAR struct tzsh_vtbl_s *vtbl, FAR const char *name)
{
	FAR struct console_stdio_s *pstate = (FAR struct console_stdio_s *)vtbl;
	FAR char *pair;
	FAR char *newvarp;
	int newsize;
	int ret = OK;

	DEBUGASSERT(name != NULL && pstate != NULL);

	/* Check if the variable exists */

	if (pstate != NULL && (pair = tzsh_findvar(pstate, name)) != NULL) {
		/* It does!  Remove the name=value pair from the TZSH variables. */

		(void)tzsh_removevar(pstate, pair);

		/* Reallocate the new TZSH variable buffer */

		newsize = pstate->varsz;
		if (newsize <= 0) {
			/* Free the old TZSH variable (if there was one) */

			if (pstate->varp != NULL) {
				free(pstate->varp);
				pstate->varp = NULL;
			}

			pstate->varsz = 0;
		} else {
			/* Reallocate the TZSH variable buffer to reclaim a little memory */

			newvarp = (FAR char *)realloc(pstate->varp, newsize);
			if (newvarp == NULL) {
				return -ENOMEM;	/* Shouldn't happen when realloc'ing down */
			} else {
				/* Save the new TZSH variable pointer (it might have changed due
				 * to reallocation).
				 */

				pstate->varp = newvarp;
			}
		}
	}

	sched_unlock();
	return ret;
}
#endif

/****************************************************************************
 * Name: tzsh_foreach_var
 *
 * Description:
 *   Visit each name-value pair in the environment.
 *
 * Input Parameters:
 *   vtbl  - TZSH session data
 *   cb    - The callback function to be invoked for each environment
 *           variable.
 *
 * Returned Value:
 *   Zero if the all TZSH variables have been traversed.  A non-zero value
 *   means that the callback function requested early termination by
 *   returning a nonzero value.
 *
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_SET
int tzsh_foreach_var(FAR struct tzsh_vtbl_s *vtbl, tzsh_foreach_var_t cb, FAR void *arg)
{
	FAR struct console_stdio_s *pstate = (FAR struct console_stdio_s *)vtbl;
	FAR char *ptr;
	FAR char *end;
	int ret = OK;

	/* Verify input parameters */

	DEBUGASSERT(pstate != NULL && cb != NULL);

	/* Search for a name=value string with matching name */

	end = &pstate->varp[pstate->varsz];
	for (ptr = pstate->varp; ptr < end; ptr += (strlen(ptr) + 1)) {
		/* Perform the callback */

		ret = cb(vtbl, arg, ptr);

		/* Terminate the traversal early if the callback so requests by
		 * returning a non-zero value.
		 */

		if (ret != 0) {
			break;
		}
	}

	return ret;
}
#endif

#endif							/* CONFIG_TZSH_VARS */
