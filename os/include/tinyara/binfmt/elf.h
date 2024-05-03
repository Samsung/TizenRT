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
 * include/tinyara/binfmt/elf.h
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

#ifndef __INCLUDE_TINYARA_BINFMT_ELF_H
#define __INCLUDE_TINYARA_BINFMT_ELF_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>

#include <stdint.h>
#include <stdbool.h>
#include <elf32.h>

#include <tinyara/arch.h>
#include <tinyara/binfmt/binfmt.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

#ifndef CONFIG_ELF_ALIGN_LOG2
#define CONFIG_ELF_ALIGN_LOG2 2
#endif

#ifndef CONFIG_ELF_STACKSIZE
#define CONFIG_ELF_STACKSIZE 2048
#endif

#ifndef CONFIG_ELF_BUFFERSIZE
#define CONFIG_ELF_BUFFERSIZE 32
#endif

#ifndef CONFIG_ELF_BUFFERINCR
#define CONFIG_ELF_BUFFERINCR 32
#endif

/* Allocation array size and indices */

#define LIBELF_ELF_ALLOC     0
#ifdef CONFIG_BINFMT_CONSTRUCTORS
#define LIBELF_CTORS_ALLOC 1
#define LIBELF_CTPRS_ALLOC 2
#define LIBELF_NALLOC      3
#else
#define LIBELF_NALLOC      1
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* This struct provides a description of the currently loaded instantiation
 * of an ELF binary.
 */

struct elf_loadinfo_s {
	int filfd;				/* Descriptor for the file being loaded */
	off_t filelen;				/* Length of the entire ELF file */
	uint16_t offset;    		        /* elf offset when binary header is included */
	Elf32_Ehdr ehdr;			/* Buffered ELF file header */
	FAR Elf32_Shdr *shdr;			/* Buffered ELF section headers */
	uint8_t *iobuffer;			/* File I/O buffer */

#ifdef CONFIG_BINFMT_CONSTRUCTORS
	FAR binfmt_ctor_t *ctors;		/* Pointer to a list of constructors */
	FAR binfmt_dtor_t *dtors;		/* Pointer to a list of destructors */
	uint16_t nctors;			/* Number of constructors */
	uint16_t ndtors;			/* Number of destructors */
#endif

	uintptr_t symtab;			/* Copy of symbol table */
	uintptr_t reltab;			/* Copy of relocation table */
	uintptr_t strtab;			/* Copy of string table */
	uint16_t symtabidx;			/* Symbol table section index */
	uint16_t strtabidx;			/* String table section index */
	uint16_t buflen;			/* size of iobuffer[] */

	struct binary_s *binp;			/* Back pointer to binary object */
};

#ifdef CONFIG_APP_BINARY_SEPARATION
struct bin_addr_info_s {
        uint32_t text_addr;
        uint32_t text_size;
#ifdef CONFIG_SAVE_BIN_SECTION_ADDR
#if defined(CONFIG_OPTIMIZE_APP_RELOAD_TIME) || defined(CONFIG_MEM_LEAK_CHECKER)
        uint32_t rodata_addr;
        uint32_t data_addr;
        uint32_t bss_addr;
#endif
#ifdef CONFIG_MEM_LEAK_CHECKER
        uint32_t rodata_size;
        uint32_t data_size;
        uint32_t bss_size;
#endif
#endif
};
typedef struct bin_addr_info_s bin_addr_info_t;
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: elf_initialize
 *
 * Description:
 *   In order to use the ELF binary format, this function must be called
 *   during system initialization to register the ELF binary format.
 *
 * Returned Value:
 *   This is a NuttX internal function so it follows the convention that
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_initialize(void);

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

void elf_uninitialize(void);

/****************************************************************************
 * Name: elf_init
 *
 * Description:
 *   This function is called to configure the library to process an ELF
 *   program binary.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_init(FAR const char *filename, FAR struct elf_loadinfo_s *loadinfo);

/****************************************************************************
 * Name: elf_uninit
 *
 * Description:
 *   Releases any resources committed by elf_init().  This essentially
 *   undoes the actions of elf_init.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_uninit(FAR struct elf_loadinfo_s *loadinfo);

/****************************************************************************
 * Name: elf_load
 *
 * Description:
 *   Loads the binary into memory, allocating memory, performing relocations
 *   and initializing the data and bss segments.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_load(FAR struct elf_loadinfo_s *loadinfo);

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

struct symtab_s;
int elf_bind(FAR struct elf_loadinfo_s *loadinfo, FAR const struct symtab_s *exports, int nexports);

/****************************************************************************
 * Name: elf_unload
 *
 * Description:
 *   This function unloads the object from memory. This essentially undoes
 *   the actions of elf_load.  It is called only under certain error
 *   conditions after the module has been loaded but not yet started.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_unload(struct elf_loadinfo_s *loadinfo);

#ifdef CONFIG_APP_BINARY_SEPARATION

/****************************************************************************
 * Name: get_bin_addr_list
 *
 * Description:
 *   Returns the pointer to the bin info address list
 *
 * Returned Value:
 *   Pointer to the bin info address list
 ****************************************************************************/

bin_addr_info_t *get_bin_addr_list(void);

void *elf_find_text_section_addr(int bin_idx);
void elf_show_all_bin_section_addr(void);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __INCLUDE_TINYARA_BINFMT_ELF_H */
