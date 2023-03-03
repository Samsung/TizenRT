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
 * os/binfmt/elf.c
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

#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/binfmt/binfmt.h>
#include <tinyara/binfmt/elf.h>

#include "libelf/libelf.h"

#ifdef CONFIG_ELF

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_ELF_STACKSIZE
#define CONFIG_ELF_STACKSIZE 2048
#endif

#ifdef CONFIG_ELF_DUMPBUFFER
#define elf_dumpbuffer(m, b, n) binfodumpbuffer(m, b, n)
#else
#define elf_dumpbuffer(m, b, n)
#endif

#ifndef MIN
#define MIN(a, b) (a < b ? a : b)
#endif

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int elf_loadbinary(FAR struct binary_s *binp);
#ifdef CONFIG_DEBUG_BINFMT
static void elf_dumploadinfo(FAR struct elf_loadinfo_s *loadinfo);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct binfmt_s g_elfbinfmt = {
	NULL,						/* next */
	elf_loadbinary,				/* load */
	NULL,						/* unload */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: elf_dumploadinfo
 ****************************************************************************/

#ifdef CONFIG_DEBUG_BINFMT
static void elf_dumploadinfo(FAR struct elf_loadinfo_s *loadinfo)
{
	int i;

	binfo("LOAD_INFO:\n");
	binfo("  textalloc:    %08lx\n", (long)loadinfo->binp->sections[BIN_TEXT]);
	binfo("  textsize:     %ld\n", (long)loadinfo->binp->sizes[BIN_TEXT]);
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	binfo("  roalloc:      %08lx\n", (long)loadinfo->binp->sections[BIN_RO]);
	binfo("  rosize:       %ld\n", (long)loadinfo->binp->sizes[BIN_RO]);
#endif
	binfo("  dataalloc:    %08lx\n", (long)loadinfo->binp->sections[BIN_DATA]);
	binfo("  datasize:     %ld\n", (long)loadinfo->binp->sizes[BIN_DATA]);
	binfo("  filelen:      %ld\n", (long)loadinfo->filelen);
#ifdef CONFIG_BINFMT_CONSTRUCTORS
	binfo("  ctors:        %08lx\n", (long)loadinfo->ctors);
	binfo("  nctors:       %d\n", loadinfo->nctors);
	binfo("  dtors:        %08lx\n", (long)loadinfo->dtors);
	binfo("  ndtors:       %d\n", loadinfo->ndtors);
#endif
	binfo("  filfd:        %d\n", loadinfo->filfd);
	binfo("  symtabidx:    %d\n", loadinfo->symtabidx);
	binfo("  strtabidx:    %d\n", loadinfo->strtabidx);

	binfo("ELF Header:\n");
	binfo("  e_ident:      %02x %02x %02x %02x\n", loadinfo->ehdr.e_ident[0], loadinfo->ehdr.e_ident[1], loadinfo->ehdr.e_ident[2], loadinfo->ehdr.e_ident[3]);
	binfo("  e_type:       %04x\n", loadinfo->ehdr.e_type);
	binfo("  e_machine:    %04x\n", loadinfo->ehdr.e_machine);
	binfo("  e_version:    %08x\n", loadinfo->ehdr.e_version);
	binfo("  e_entry:      %08lx\n", (long)loadinfo->ehdr.e_entry);
	binfo("  e_phoff:      %d\n", loadinfo->ehdr.e_phoff);
	binfo("  e_shoff:      %d\n", loadinfo->ehdr.e_shoff);
	binfo("  e_flags:      %08x\n", loadinfo->ehdr.e_flags);
	binfo("  e_ehsize:     %d\n", loadinfo->ehdr.e_ehsize);
	binfo("  e_phentsize:  %d\n", loadinfo->ehdr.e_phentsize);
	binfo("  e_phnum:      %d\n", loadinfo->ehdr.e_phnum);
	binfo("  e_shentsize:  %d\n", loadinfo->ehdr.e_shentsize);
	binfo("  e_shnum:      %d\n", loadinfo->ehdr.e_shnum);
	binfo("  e_shstrndx:   %d\n", loadinfo->ehdr.e_shstrndx);

	if (loadinfo->shdr && loadinfo->ehdr.e_shnum > 0) {
		for (i = 0; i < loadinfo->ehdr.e_shnum; i++) {
			FAR Elf32_Shdr *shdr = &loadinfo->shdr[i];
			binfo("Sections %d:\n", i);
			binfo("  sh_name:      %08x\n", shdr->sh_name);
			binfo("  sh_type:      %08x\n", shdr->sh_type);
			binfo("  sh_flags:     %08x\n", shdr->sh_flags);
			binfo("  sh_addr:      %08x\n", shdr->sh_addr);
			binfo("  sh_offset:    %d\n", shdr->sh_offset);
			binfo("  sh_size:      %d\n", shdr->sh_size);
			binfo("  sh_link:      %d\n", shdr->sh_link);
			binfo("  sh_info:      %d\n", shdr->sh_info);
			binfo("  sh_addralign: %d\n", shdr->sh_addralign);
			binfo("  sh_entsize:   %d\n", shdr->sh_entsize);
		}
	}
}
#else
#define elf_dumploadinfo(i)
#endif

/****************************************************************************
 * Name: elf_dumpentrypt
 ****************************************************************************/

#ifdef CONFIG_ELF_DUMPBUFFER
static void elf_dumpentrypt(FAR struct binary_s *binp, FAR struct elf_loadinfo_s *loadinfo)
{
	elf_dumpbuffer("Entry code", (FAR const uint8_t *)binp->entrypt, MIN(loadinfo->sizes[BIN_TEXT] - loadinfo->ehdr.e_entry, 512));

}
#else
#define elf_dumpentrypt(b, l)
#endif

/****************************************************************************
 * Name: elf_loadbinary
 *
 * Description:
 *   Verify that the file is an ELF binary and, if so, load the ELF
 *   binary into memory
 *
 ****************************************************************************/

static int elf_loadbinary(FAR struct binary_s *binp)
{
	struct elf_loadinfo_s loadinfo;	/* Contains globals for libelf */
	int ret;

	binfo("Loading file: %s\n", binp->filename);

	/* Clear the load info structure */

	memset(&loadinfo, 0, sizeof(struct elf_loadinfo_s));

	/* Initialize the ELF library to load the program binary. */
	loadinfo.offset = binp->offset;
	loadinfo.filelen = binp->filelen;
	loadinfo.binp = binp;

	ret = elf_init(binp->filename, &loadinfo);
	if (ret != 0) {
		elf_dumploadinfo(&loadinfo);
		berr("Failed to initialize for load of ELF program: %d\n", ret);
		goto errout;
	}

	/* Load the program binary */

	ret = elf_load(&loadinfo);
	elf_dumploadinfo(&loadinfo);
	if (ret != 0) {
		berr("Failed to load ELF program binary: %d\n", ret);
		goto errout_with_init;
	}

	/* Bind the program to the exported symbol table */

	ret = elf_bind(&loadinfo, binp->exports, binp->nexports);
	if (ret != 0) {
		berr("Failed to bind symbols program binary: %d\n", ret);
		goto errout_with_load;
	}


	binp->entrypt = (main_t)((uint32_t)loadinfo.binp->sections[BIN_TEXT] + loadinfo.ehdr.e_entry);
	if (binp->stacksize == 0) {
		binp->stacksize = CONFIG_ELF_STACKSIZE;
	}

#ifdef CONFIG_BINFMT_CONSTRUCTORS
	binp->sections[BIN_CTOR] = loadinfo.ctors;
	binp->sections[BIN_DTOR] = loadinfo.dtors;
#endif

#ifdef CONFIG_BINFMT_CONSTRUCTORS
	/* Save information about constructors and destructors. */

	binp->ctors = loadinfo.ctors;
	binp->nctors = loadinfo.nctors;

	binp->dtors = loadinfo.dtors;
	binp->ndtors = loadinfo.ndtors;
#endif

	elf_dumpentrypt(binp, &loadinfo);
	elf_uninit(&loadinfo);
	return OK;

errout_with_load:
	elf_unload(&loadinfo);
errout_with_init:
	elf_uninit(&loadinfo);
errout:
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: elf_initialize
 *
 * Description:
 *   In order to use the ELF binary format, this function must be called
 *   during system initialization to register the ELF binary format.
 *
 * Returned Value:
 *   This is a internal function so it follows the convention that
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_initialize(void)
{
	int ret;

	/* Register ourselves as a binfmt loader */

	binfo("Registering ELF\n");

	ret = register_binfmt(&g_elfbinfmt);
	if (ret != 0) {
		berr("Failed to register binfmt: %d\n", ret);
	}

	return ret;
}

/****************************************************************************
 * Name: elf_uninitialize
 *
 * Description:
 *   Unregister the ELF binary loader
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void elf_uninitialize(void)
{
	(void)unregister_binfmt(&g_elfbinfmt);
}

#endif							/* CONFIG_ELF */
