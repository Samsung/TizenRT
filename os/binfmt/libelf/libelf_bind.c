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
 * os/binfmt/libelf/libelf_bind.c
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

#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/elf.h>
#include <tinyara/binfmt/elf.h>
#include <tinyara/binfmt/symtab.h>
#include <tinyara/kmalloc.h>

#include "libelf.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_ELF_DUMPBUFFER
#define elf_dumpbuffer(m, b, n) binfodumpbuffer(m, b, n)
#else
#define elf_dumpbuffer(m, b, n)
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
#ifdef CONFIG_SUPPORT_COMMON_BINARY
#include <tinyara/hashmap.h>
static struct hashmap_s *g_lib_symhash;
static int g_num_lib_syms;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: elf_readreltab
 *
 * Description:
 *   Read the relocation table into memory.
 *
 ****************************************************************************/
static inline void elf_readreltab(FAR struct elf_loadinfo_s *loadinfo, FAR const Elf32_Shdr *relsec)
{
	loadinfo->reltab = (uintptr_t)kmm_malloc(relsec->sh_size);

	if (!loadinfo->reltab) {
		berr("ERROR: Failed to allocate space for relocation table. Size = %u\n", relsec->sh_size);
		return;
	}

	if (elf_read(loadinfo, (FAR uint8_t *)loadinfo->reltab, relsec->sh_size, relsec->sh_offset) < 0) {
		berr("ERROR: Failed to read relocation table into memory\n");
	}
}


/****************************************************************************
 * Name: elf_readrel
 *
 * Description:
 *   Read the ELF32_Rel structure into memory.
 *
 ****************************************************************************/

static inline int elf_readrel(FAR struct elf_loadinfo_s *loadinfo, FAR const Elf32_Shdr *relsec, int index, FAR Elf32_Rel *rel)
{
	off_t offset;

	/* Verify that the symbol table index lies within symbol table */

	if (index < 0 || index > (relsec->sh_size / sizeof(Elf32_Rel))) {
		berr("Bad relocation symbol index: %d\n", index);
		return -EINVAL;
	}

	/* Get the file offset to the symbol table entry */
	offset = relsec->sh_offset + sizeof(Elf32_Rel) * index;

	/* And, finally, read the symbol table entry into memory */
	return elf_read(loadinfo, (FAR uint8_t *)rel, sizeof(Elf32_Rel), offset);
}

/****************************************************************************
 * Name: elf_relocate and elf_relocateadd
 *
 * Description:
 *   Perform all relocations associated with a section.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

static int elf_relocate(FAR struct elf_loadinfo_s *loadinfo, int relidx, FAR const struct symtab_s *exports, int nexports)
{
	FAR Elf32_Shdr *relsec = &loadinfo->shdr[relidx];
	FAR Elf32_Shdr *dstsec = &loadinfo->shdr[relsec->sh_info];
	Elf32_Rel rel;
	Elf32_Sym sym;
	FAR Elf32_Sym *psym;
	Elf32_Rel *prel;
	uintptr_t addr;
	int symidx;
	int ret = OK;
	int i;

	/* Read the relocation table into memory */
	elf_readreltab(loadinfo, relsec);

	/* Examine each relocation in the section.  'relsec' is the section
	 * containing the relations.  'dstsec' is the section containing the data
	 * to be relocated.
	 */

	for (i = 0; i < relsec->sh_size / sizeof(Elf32_Rel); i++) {
		psym = &sym;
		prel = &rel;


		/* Read the relocation entry into memory */
		if (loadinfo->reltab) {
			/* Verify that the relocation table index lies within relocation table */
			if (i < 0 || i > (relsec->sh_size / sizeof(Elf32_Rel))) {
				berr("Bad relocation symbol index: %d\n", i);
				ret = -EINVAL;
				goto ret_err;
			}
			prel = (Elf32_Rel *)(loadinfo->reltab + sizeof(Elf32_Rel) * i);
		} else {
			ret = elf_readrel(loadinfo, relsec, i, &rel);
			if (ret < 0) {
				berr("Section %d reloc %d: Failed to read relocation entry: %d\n", relidx, i, ret);
				goto ret_err;
			}
		}

		/* Get the symbol table index for the relocation.  This is contained
		 * in a bit-field within the r_info element.
		 */

		symidx = ELF32_R_SYM(prel->r_info);

		/* Read the symbol table entry into memory */
		if (loadinfo->symtab) {
			/* Verify that the symbol table index lies within symbol table */
			if (symidx < 0 || symidx > (loadinfo->shdr[loadinfo->symtabidx].sh_size / sizeof(Elf32_Sym))) {
				berr("Bad relocation symbol index: %d\n", symidx);
				ret = -EINVAL;
				goto ret_err;
			}
			psym = (FAR Elf32_Sym *)(loadinfo->symtab + sizeof(Elf32_Sym) * symidx);
		} else {
			ret = elf_readsym(loadinfo, symidx, &sym);
			if (ret < 0) {
				berr("Section %d reloc %d: Failed to read symbol[%d]: %d\n", relidx, i, symidx, ret);
				goto ret_err;
			}
		}
		/* Get the value of the symbol (in sym.st_value) */

		ret = elf_symvalue(loadinfo, psym, exports, nexports);
		if (ret < 0) {
			/* The special error -ESRCH is returned only in one condition:  The
			 * symbol has no name.
			 *
			 * There are a few relocations for a few architectures that do
			 * no depend upon a named symbol.  We don't know if that is the
			 * case here, but we will use a NULL symbol pointer to indicate
			 * that case to up_relocate().  That function can then do what
			 * is best.
			 */

			if (ret == -ESRCH) {
				berr("Section %d reloc %d: Undefined symbol[%d] has no name: %d\n", relidx, i, symidx, ret);
				psym = NULL;
			} else {
				berr("Section %d reloc %d: Failed to get value of symbol[%d]: %d\n", relidx, i, symidx, ret);
				goto ret_err;
			}
		}

		/* Calculate the relocation address. */

		if (prel->r_offset > dstsec->sh_size - sizeof(uint32_t)) {
			berr("Section %d reloc %d: Relocation address out of range, offset %d size %d\n", relidx, i, prel->r_offset, dstsec->sh_size);
			goto ret_err;
		}

		addr = dstsec->sh_addr + prel->r_offset;

		/* Now perform the architecture-specific relocation */

		ret = up_relocate(prel, psym, addr);
		if (ret < 0) {
			berr("ERROR: Section %d reloc %d: Relocation failed: %d\n", relidx, i, ret);
			goto ret_err;
		}
	}

ret_err:
	if (loadinfo->reltab) {
		kmm_free((void *)loadinfo->reltab);
		loadinfo->reltab = (uintptr_t)NULL;
	}
	return ret;
}

static int elf_relocateadd(FAR struct elf_loadinfo_s *loadinfo, int relidx, FAR const struct symtab_s *exports, int nexports)
{
	berr("Not implemented\n");
	return -ENOSYS;
}

#ifdef CONFIG_SUPPORT_COMMON_BINARY
static int export_library_symtab(FAR struct elf_loadinfo_s *loadinfo)
{
	int i;
	int ret;

	Elf32_Shdr *symtab = &loadinfo->shdr[loadinfo->symtabidx];
	int nsyms = symtab->sh_size / sizeof(Elf32_Sym);

	/* nsyms is the number of symbols in the binary. We create a hashmap
	 * of half this number, because generally every binary contains some
	 * global and some local symbols. We are interested only to export the
	 * global symbols. So, we start with a hashmap size of nsyms/2. If
	 * the number of global symbols is more than nsyms/2, the hashmap will
	 * increase its own size automatically with a realloc. This is a trade-off
	 * to achieve optimal size versus load time
	 */
	g_lib_symhash = hashmap_create(nsyms / 2);

	g_num_lib_syms = 0;
	for (i = 0; i < nsyms; i++) {
		Elf32_Sym sym;
		Elf32_Sym *psym = &sym;

		if (loadinfo->symtab) {
			psym = (Elf32_Sym *)(loadinfo->symtab + sizeof(Elf32_Sym) * i);
		} else {
			ret = elf_readsym(loadinfo, i, &sym);
			if (ret < 0) {
				berr("Failed to read symbol[%d]: %d\n", i, ret);
				goto ret_err;
			}
		}

		if (ELF32_ST_BIND(psym->st_info) == STB_GLOBAL) {
			ret = elf_symname(loadinfo, psym);
			if (ret < 0) {
				berr("SHN_UNDEF: Failed to get symbol name: %d\n", ret);
				goto ret_err;
			}

			unsigned long key = hashmap_get_hashval(loadinfo->iobuffer);

			ret = elf_symvalue(loadinfo, psym, 0, 0);
			if (ret < 0) {
				if (ret == -ESRCH) {
					berr("Undefined symbol[%d] has no name: %d\n", i, ret);
				} else {
					berr("Failed to get value of symbol[%d]: %d\n", i, ret);
				}
				goto ret_err;
			}

			if (!psym->st_value) {
				binfo("Ignore symbol %s from export table due to zero address value\n", loadinfo->iobuffer);
				continue;
			}

			hashmap_insert(g_lib_symhash, (void *)psym->st_value, key);

			g_num_lib_syms++;
		}
	}

	binfo("Total symbols in library = %d. Exported symbols = %d\n", nsyms, g_num_lib_syms);
	return OK;
ret_err:
	hashmap_delete(g_lib_symhash);
	return ret;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: elf_bind
 *
 * Description:
 *   Bind the imported symbol names in the loaded module described by
 *   'loadinfo' using the exported symbol values provided by 'symtab'.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_bind(FAR struct elf_loadinfo_s *loadinfo, FAR const struct symtab_s *exports, int nexports)
{
	int ret;
	int i;

	/* Find the symbol and string tables */

	ret = elf_findsymtab(loadinfo);
	if (ret < 0) {
		return ret;
	}

	/* Read the symbol table into memory */
	elf_readsymtab(loadinfo);

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	elf_readstrtab(loadinfo);

	if (loadinfo->binp->islibrary) {
		ret = export_library_symtab(loadinfo);
		if (ret < 0) {
			goto ret_err;
		}
	} else {
		exports = (struct symtab_s *)g_lib_symhash;
		nexports = g_num_lib_syms;
	}
#endif

	/* Process relocations in every allocated section */

	for (i = 1; i < loadinfo->ehdr.e_shnum; i++) {
		/* Get the index to the relocation section */

		int infosec = loadinfo->shdr[i].sh_info;
		if (infosec >= loadinfo->ehdr.e_shnum) {
			continue;
		}

		/* Make sure that the section is allocated.  We can't relocated
		 * sections that were not loaded into memory.
		 */

		if ((loadinfo->shdr[infosec].sh_flags & SHF_ALLOC) == 0) {
			continue;
		}

		/* Process the relocations by type */

		if (loadinfo->shdr[i].sh_type == SHT_REL) {
			ret = elf_relocate(loadinfo, i, exports, nexports);
		} else if (loadinfo->shdr[i].sh_type == SHT_RELA) {
			ret = elf_relocateadd(loadinfo, i, exports, nexports);
		}

		if (ret < 0) {
			break;
		}
	}

#if defined(CONFIG_ARCH_HAVE_COHERENT_DCACHE)
	/* Ensure that the I and D caches are coherent before starting the newly
	 * loaded module by cleaning the D cache (i.e., flushing the D cache
	 * contents to memory and invalidating the I cache).
	 */

	up_coherent_dcache(loadinfo->binp->sections[BIN_TEXT], loadinfo->binp->sizes[BIN_TEXT]);
	up_coherent_dcache(loadinfo->binp->sections[BIN_DATA], loadinfo->binp->sizes[BIN_DATA]);

#endif

#ifdef CONFIG_SUPPORT_COMMON_BINARY
ret_err:
#endif
	if (loadinfo->strtab) {
		kmm_free((void *)loadinfo->strtab);
		loadinfo->strtab = (uintptr_t)NULL;
	}
	if (loadinfo->symtab) {
		kmm_free((void *)loadinfo->symtab);
		loadinfo->symtab = (uintptr_t)NULL;
	}

	return ret;
}
