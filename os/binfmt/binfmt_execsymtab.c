/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * os/binfmt/binfmt_execsymtab.c
 *
 *   Copyright (C) 2013, 2016, 2018 Gregory Nutt. All rights reserved.
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

#include <assert.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/binfmt/symtab.h>

#ifdef CONFIG_LIBC_EXECFUNCS

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* If CONFIG_LIBC_EXECFUNCS is defined in the configuration, then the
 * following must also be defined:
 */

#ifdef CONFIG_EXECFUNCS_HAVE_SYMTAB
/* Symbol table used by exec[l|v] */

#ifndef CONFIG_EXECFUNCS_SYMTAB_ARRAY
#error "CONFIG_EXECFUNCS_SYMTAB_ARRAY must be defined"
#endif

/* Number of Symbols in the Table */

#ifndef CONFIG_EXECFUNCS_NSYMBOLS_VAR
#error "CONFIG_EXECFUNCS_NSYMBOLS_VAR must be defined"
#endif
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef CONFIG_EXECFUNCS_HAVE_SYMTAB
extern const struct symtab_s CONFIG_EXECFUNCS_SYMTAB_ARRAY[];
extern int CONFIG_EXECFUNCS_NSYMBOLS_VAR;
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static FAR const struct symtab_s *g_exec_symtab;
static int g_exec_nsymbols;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: exec_getsymtab
 *
 * Description:
 *   Get the current symbol table selection as an atomic operation.
 *
 * Input Parameters:
 *   symtab - The location to store the symbol table.
 *   nsymbols - The location to store the number of symbols in the symbol table.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void exec_getsymtab(FAR const struct symtab_s **symtab, FAR int *nsymbols)
{
	irqstate_t flags;

	DEBUGASSERT(symtab != NULL && nsymbols != NULL);

	/* Disable interrupts very briefly so that both the symbol table and its
	 * size are returned as a single atomic operation.
	 */

	flags = enter_critical_section();

#ifdef CONFIG_EXECFUNCS_HAVE_SYMTAB
	/* If a bring-up symbol table has been provided and if the exec symbol
	 * table has not yet been initialized, then use the provided start-up
	 * symbol table.
	 */

	if (g_exec_symtab == NULL) {
		g_exec_symtab = CONFIG_EXECFUNCS_SYMTAB_ARRAY;
		g_exec_nsymbols = CONFIG_EXECFUNCS_NSYMBOLS_VAR;
	}
#endif

	/* Return the symbol table and its size */

	*symtab = g_exec_symtab;
	*nsymbols = g_exec_nsymbols;
	leave_critical_section(flags);
}

/****************************************************************************
 * Name: exec_setsymtab
 *
 * Description:
 *   Select a new symbol table selection as an atomic operation.
 *
 * Input Parameters:
 *   symtab - The new symbol table.
 *   nsymbols - The number of symbols in the symbol table.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void exec_setsymtab(FAR const struct symtab_s *symtab, int nsymbols)
{
	irqstate_t flags;

	DEBUGASSERT(symtab != NULL);

	/* Disable interrupts very briefly so that both the symbol table and its
	 * size are set as a single atomic operation.
	 */

	flags = enter_critical_section();
	g_exec_symtab = symtab;
	g_exec_nsymbols = nsymbols;
	leave_critical_section(flags);
}

#endif							/* CONFIG_LIBC_EXECFUNCS */
