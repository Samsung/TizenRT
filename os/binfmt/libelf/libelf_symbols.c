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
 * os/binfmt/libelf/libelf_symbols.c
 *
 *   Copyright (C) 2012, 2014 Gregory Nutt. All rights reserved.
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
#include <debug.h>
#ifdef CONFIG_SUPPORT_COMMON_BINARY
#include <tinyara/hashmap.h>
#endif

#include <tinyara/binfmt/elf.h>
#include <tinyara/binfmt/symtab.h>
#include <tinyara/kmalloc.h>

#include "libelf.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Constant Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: elf_readstrtab
 *
 * Description:
 *   Read the ELF string table into memory.
 *
 * Input Parameters:
 *   loadinfo - Load state information
 *
 ****************************************************************************/
int elf_readstrtab(FAR struct elf_loadinfo_s *loadinfo)
{
	int ret = OK;
	FAR Elf32_Shdr *strtab = &loadinfo->shdr[loadinfo->strtabidx];

	loadinfo->strtab = (uintptr_t)kmm_malloc(strtab->sh_size);

	if (!loadinfo->strtab) {
		berr("ERROR: Failed to allocate space for str table. Size = %u\n", strtab->sh_size);
		return -ENOMEM;
	}

	ret = elf_read(loadinfo, (FAR uint8_t *)loadinfo->strtab, strtab->sh_size, strtab->sh_offset);
	if (ret != OK) {
		berr("ERROR: Failed to load string table into memory\n");
		kmm_free((void *)loadinfo->strtab);
		loadinfo->strtab = (uintptr_t)NULL;
		return ret;
	}

	return ret;
}

/****************************************************************************
 * Name: elf_symname
 *
 * Description:
 *   Get the symbol name in loadinfo->iobuffer[].
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 *   EINVAL - There is something inconsistent in the symbol table (should only
 *            happen if the file is corrupted).
 *   ESRCH - Symbol has no name
 *
 ****************************************************************************/

int elf_symname(FAR struct elf_loadinfo_s *loadinfo, FAR const Elf32_Sym *sym)
{
	int ret;

	/* Get the file offset to the string that is the name of the symbol.  The
	 * st_name member holds an offset into the file's symbol string table.
	 */

	if (sym->st_name == 0) {
		berr("Symbol has no name\n");
		return -ESRCH;
	}

	if (!loadinfo->strtab) {
		ret = elf_readstrtab(loadinfo);

		if (ret != OK) {
			berr("Error reading str table\n");
			return ret;
		}
	}

	if (sym->st_name >= loadinfo->shdr[loadinfo->strtabidx].sh_size) {
		berr("At end of strtab\n");
		return -EINVAL;
	}

	loadinfo->iobuffer = (uint8_t *)(loadinfo->strtab + sym->st_name);

	return OK;
}

/****************************************************************************
 * Name: elf_findsymtab
 *
 * Description:
 *   Find the symbol table section.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_findsymtab(FAR struct elf_loadinfo_s *loadinfo)
{
	int i;

	/* Find the symbol table section header and its associated string table */

	for (i = 1; i < loadinfo->ehdr.e_shnum; i++) {
		if (loadinfo->shdr[i].sh_type == SHT_SYMTAB) {
			loadinfo->symtabidx = i;
			loadinfo->strtabidx = loadinfo->shdr[i].sh_link;
			break;
		}
	}

	/* Verify that there is a symbol and string table */

	if (loadinfo->symtabidx == 0) {
		berr("No symbols in ELF file\n");
		return -EINVAL;
	}

	return OK;
}

/****************************************************************************
 * Name: elf_readsymtab
 *
 * Description:
 *   Read the ELF symbol table into memory.
 *
 * Input Parameters:
 *   loadinfo - Load state information
 *
 ****************************************************************************/
void elf_readsymtab(FAR struct elf_loadinfo_s *loadinfo)
{
	FAR Elf32_Shdr *symtab = &loadinfo->shdr[loadinfo->symtabidx];

	loadinfo->symtab = (uintptr_t)kmm_malloc(symtab->sh_size);

	if (!loadinfo->symtab) {
		berr("ERROR: Failed to allocate space for sym table. Size = %u\n", symtab->sh_size);
		return;
	}

	if (elf_read(loadinfo, (FAR uint8_t *)loadinfo->symtab, symtab->sh_size, symtab->sh_offset) < 0) {
		berr("ERROR: Failed to load symbol table into memory\n");
	}
}

/****************************************************************************
 * Name: elf_readsym
 *
 * Description:
 *   Read the ELF symbol structure at the specified index into memory.
 *
 * Input Parameters:
 *   loadinfo - Load state information
 *   index    - Symbol table index
 *   sym      - Location to return the table entry
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_readsym(FAR struct elf_loadinfo_s *loadinfo, int index, FAR Elf32_Sym *sym)
{
	FAR Elf32_Shdr *symtab = &loadinfo->shdr[loadinfo->symtabidx];
	off_t offset;

	/* Verify that the symbol table index lies within symbol table */

	if (index < 0 || index > (symtab->sh_size / sizeof(Elf32_Sym))) {
		berr("Bad relocation symbol index: %d\n", index);
		return -EINVAL;
	}

	/* Get the file offset to the symbol table entry */
	offset = symtab->sh_offset + sizeof(Elf32_Sym) * index;

	/* And, finally, read the symbol table entry into memory */
	return elf_read(loadinfo, (FAR uint8_t *)sym, sizeof(Elf32_Sym), offset);
}

/****************************************************************************
 * Name: elf_symvalue
 *
 * Description:
 *   Get the value of a symbol.  The updated value of the symbol is returned
 *   in the st_value field of the symbol table entry.
 *
 * Input Parameters:
 *   loadinfo - Load state information
 *   sym      - Symbol table entry (value might be undefined)
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 *   EINVAL - There is something inconsistent in the symbol table (should only
 *            happen if the file is corrupted).
 *   ENOSYS - Symbol lies in common
 *   ESRCH  - Symbol has no name
 *   ENOENT - Symbol undefined and not provided via a symbol table
 *
 ****************************************************************************/

int elf_symvalue(FAR struct elf_loadinfo_s *loadinfo, FAR Elf32_Sym *sym, FAR const struct symtab_s *exports, int nexports)
{
#if !defined(CONFIG_SUPPORT_COMMON_BINARY)
	FAR const struct symtab_s *symbol;
#endif
	uintptr_t secbase;
	int ret;

	switch (sym->st_shndx) {
	case SHN_COMMON: {
		/* ELF modules should be compiled with -fno-common. */

		berr("SHN_COMMON: Re-compile with -fno-common\n");
		return -ENOSYS;
	}

	case SHN_ABS: {
		/* st_value already holds the correct value */

		binfo("SHN_ABS: st_value=%08lx\n", (long)sym->st_value);
		return OK;
	}

	case SHN_UNDEF: {
		/* Get the name of the undefined symbol */

		ret = elf_symname(loadinfo, sym);
		if (ret < 0) {
			/* There are a few relocations for a few architectures that do
			 * no depend upon a named symbol.  We don't know if that is the
			 * case here, but return and special error to the caller to
			 * indicate the nameless symbol.
			 */

			berr("SHN_UNDEF: Failed to get symbol name: %d\n", ret);
			return ret;
		}

		/* Check if the base code exports a symbol of this name */
#ifdef CONFIG_SUPPORT_COMMON_BINARY
		if (!exports) {
			berr("SHN_UNDEF: Exports not found\n");
			return -ENOENT;
		}

		sym->st_value = (uint32_t)hashmap_get((struct hashmap_s *)exports, hashmap_get_hashval(loadinfo->iobuffer));

		if (!sym->st_value) {
			berr("SHN_UNDEF: Exported symbol \"%s\" not found\n", loadinfo->iobuffer);
			return -ENOENT;
		}

#else

#ifdef CONFIG_SYMTAB_ORDEREDBYNAME
		symbol = symtab_findorderedbyname(exports, (FAR char *)loadinfo->iobuffer, nexports);
#else
		symbol = symtab_findbyname(exports, (FAR char *)loadinfo->iobuffer, nexports);
#endif
		if (!symbol) {
			berr("SHN_UNDEF: Exported symbol \"%s\" not found\n", loadinfo->iobuffer);
			return -ENOENT;
		}

		/* Yes... add the exported symbol value to the ELF symbol table entry */

		binfo("SHN_UNDEF: name=%s %08x+%08x=%08x\n", loadinfo->iobuffer, sym->st_value, symbol->sym_value, sym->st_value + symbol->sym_value);

		sym->st_value += (Elf32_Word)((uintptr_t)symbol->sym_value);
#endif

		sym->st_shndx = SHN_ABS;
	}
	break;

	default: {
		secbase = loadinfo->shdr[sym->st_shndx].sh_addr;

		binfo("Other: %08x+%08x=%08x\n", sym->st_value, secbase, sym->st_value + secbase);

		sym->st_value += secbase;
		sym->st_shndx = SHN_ABS;
	}
	break;
	}

	return OK;
}
