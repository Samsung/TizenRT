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
 * os/binfmt/libelf/libelf.h
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

#ifndef __BINFMT_LIBELF_LIBELF_H
#define __BINFMT_LIBELF_LIBELF_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>

#include <tinyara/arch.h>
#include <tinyara/binfmt/elf.h>

#ifdef CONFIG_SAVE_BIN_SECTION_ADDR
#include <queue.h>
#include <tinyara/binary_manager.h>
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: elf_verifyheader
 *
 * Description:
 *   Given the header from a possible ELF executable, verify that it is
 *   an ELF executable.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_verifyheader(FAR const Elf32_Ehdr *header);

/****************************************************************************
 * Name: elf_read
 *
 * Description:
 *   Read 'readsize' bytes from the object file at 'offset'.  The data is
 *   read into 'buffer.' If 'buffer' is part of the ELF address environment,
 *   then the caller is responsibile for assuring that that address
 *   environment is in place before calling this function (i.e., that
 *   elf_addrenv_select() has been called if CONFIG_ARCH_ADDRENV=y).
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_read(FAR struct elf_loadinfo_s *loadinfo, FAR uint8_t *buffer, size_t readsize, off_t offset);

/****************************************************************************
 * Name: elf_loadshdrs
 *
 * Description:
 *   Loads section headers into memory.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_loadshdrs(FAR struct elf_loadinfo_s *loadinfo);

/****************************************************************************
 * Name: elf_findsection
 *
 * Description:
 *   A section by its name.
 *
 * Input Parameters:
 *   loadinfo - Load state information
 *   sectname - Name of the section to find
 *
 * Returned Value:
 *   On success, the index to the section is returned; A negated errno value
 *   is returned on failure.
 *
 ****************************************************************************/

int elf_findsection(FAR struct elf_loadinfo_s *loadinfo, FAR const char *sectname);

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

int elf_readstrtab(FAR struct elf_loadinfo_s *loadinfo);

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

int elf_findsymtab(FAR struct elf_loadinfo_s *loadinfo);

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

void elf_readsymtab(FAR struct elf_loadinfo_s *loadinfo);

/****************************************************************************
 * Name: elf_readsym
 *
 * Description:
 *   Read the ELF symbol structure at the specfied index into memory.
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

int elf_readsym(FAR struct elf_loadinfo_s *loadinfo, int index, FAR Elf32_Sym *sym);

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
 *   exports  - The symbol table to use for resolving undefined symbols.
 *   nexports - Number of symbols in the symbol table.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 *   EINVAL - There is something inconsistent in the symbol table (should only
 *            happen if the file is corrupted)
 *   ENOSYS - Symbol lies in common
 *   ESRCH  - Symbol has no name
 *   ENOENT - Symbol undefined and not provided via a symbol table
 *
 ****************************************************************************/

int elf_symvalue(FAR struct elf_loadinfo_s *loadinfo, FAR Elf32_Sym *sym, FAR const struct symtab_s *exports, int nexports);

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

int elf_symname(FAR struct elf_loadinfo_s *loadinfo, FAR const Elf32_Sym *sym);

/****************************************************************************
 * Name: elf_freebuffers
 *
 * Description:
 *  Release all working buffers.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_freebuffers(FAR struct elf_loadinfo_s *loadinfo);

/****************************************************************************
 * Name: elf_allocbuffer
 *
 * Description:
 *   Perform the initial allocation of the I/O buffer, if it has not already
 *   been allocated.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_allocbuffer(FAR struct elf_loadinfo_s *loadinfo);

/****************************************************************************
 * Name: elf_reallocbuffer
 *
 * Description:
 *   Increase the size of I/O buffer by the specified buffer increment.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int elf_reallocbuffer(FAR struct elf_loadinfo_s *loadinfo, size_t increment);

/****************************************************************************
 * Name: elf_loadctors
 *
 * Description:
 *   Find C++ static constructors.
 *
 * Input Parameters:
 *   loadinfo - Load state information
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

#ifdef CONFIG_BINFMT_CONSTRUCTORS
int elf_loadctors(FAR struct elf_loadinfo_s *loadinfo);
#endif

/****************************************************************************
 * Name: elf_loaddtors
 *
 * Description:
 *  Load pointers to static destructors into an in-memory array.
 *
 * Input Parameters:
 *   loadinfo - Load state information
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

#ifdef CONFIG_BINFMT_CONSTRUCTORS
int elf_loaddtors(FAR struct elf_loadinfo_s *loadinfo);
#endif

/****************************************************************************
 * Name: elf_addrenv_alloc
 *
 * Description:
 *   Allocate memory for the ELF image (textalloc and dataalloc). If
 *   CONFIG_ARCH_ADDRENV=n, textalloc will be allocated using kmm_zalloc() and
 *   dataalloc will be a offset from textalloc.  If CONFIG_ARCH_ADDRENV-y, then
 *   textalloc and dataalloc will be allocated using up_addrenv_create().  In
 *   either case, there will be a unique instance of textalloc and dataalloc
 *   (and stack) for each instance of a process.
 *
 * Input Parameters:
 *   loadinfo - Load state information
 *   textsize - The size (in bytes) of the .text address environment needed
 *     for the ELF image (read/execute).
 *   datasize - The size (in bytes) of the .bss/.data address environment
 *     needed for the ELF image (read/write).
 *   heapsize - The initial size (in bytes) of the heap address environment
 *     needed by the task.  This region may be read/write only.
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

int elf_addrenv_alloc(FAR struct elf_loadinfo_s *loadinfo, size_t textsize, size_t datasize, size_t heapsize);

/****************************************************************************
 * Name: elf_addrenv_select
 *
 * Description:
 *   Temporarily select the task's address environment.
 *
 * Input Parameters:
 *   loadinfo - Load state information
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_ADDRENV
#define elf_addrenv_select(l) up_addrenv_select(&(l)->addrenv, &(l)->oldenv)
#endif

/****************************************************************************
 * Name: elf_addrenv_restore
 *
 * Description:
 *   Restore the address environment before elf_addrenv_select() was called..
 *
 * Input Parameters:
 *   loadinfo - Load state information
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_ADDRENV
#define elf_addrenv_restore(l) up_addrenv_restore(&(l)->oldenv)
#endif

/****************************************************************************
 * Name: elf_addrenv_free
 *
 * Description:
 *   Release the address environment previously created by
 *   elf_addrenv_alloc().  This function  is called only under certain error
 *   conditions after the module has been loaded but not yet started.
 *   After the module has been started, the address environment will
 *   automatically be freed when the module exits.
 *
 * Input Parameters:
 *   loadinfo - Load state information
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void elf_addrenv_free(FAR struct elf_loadinfo_s *loadinfo);

#ifdef CONFIG_ELF_CACHE_READ

/* Cut-off ratio for number of blocks for caching */
#define CUTOFF_RATIO_CACHE_BLOCKS 0.1f

/* Struct for output buffers to cache uncompressed blocks */
struct block_cache_s {
	unsigned char *out_buffer;              /* Buffer that is going to hold uncompressed data */
	int block_number;                       /* Block number in compressed file for the cached block */
	unsigned int no_requests_for_block;     /* Number of requests for current block that is cached */
	unsigned int index_block_cache;         /* Index of block cache in the array */
	struct block_cache_s *next;             /* Pointer to next element in doubly linked list */
	struct block_cache_s *prev;             /* Pointer to previous element in doubly linked list */
};
typedef struct block_cache_s block_cache_t;

/****************************************************************************
 * Name: elf_cache_uninit
 *
 * Description:
 *   Release cache buffers initialized during elf_cache_init
 *
 * Returned Value:
 *   None
 ****************************************************************************/
void elf_cache_uninit(void);

/****************************************************************************
 * Name: elf_cache_init
 *
 * Description:
 *   Initialize the cache blocks
 *
 * Returned value:
 *   OK (0) on Success
 *   ERROR (-1) on Failure
 ****************************************************************************/
int elf_cache_init(int filfd, uint16_t offset, off_t filelen, uint8_t compression_type);

/****************************************************************************
 * Name: elf_cache_read
 *
 * Description:
 *   Read n-bytes from the elf file using 'offset' and 'readsize' info
 *   The data is read into 'buffer' pointer. Offset value here is offset from
 *   start of elf binary (excluding binary header).
 *
 * Returned Value:
 *   Number of bytes read into buffer on Success
 *   Negative value on failure
 ****************************************************************************/
int elf_cache_read(int filfd, uint16_t binary_header_size, FAR uint8_t *buffer, size_t readsize, off_t offset);
#endif

#ifdef CONFIG_SAVE_BIN_SECTION_ADDR
struct bin_addr_info_s {
	struct bin_addr_info_s *flink;
	char bin_name[BIN_NAME_MAX];
	uint32_t text_addr;
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	uint32_t rodata_addr;
	uint32_t data_addr;
	uint32_t bss_addr;
#endif
};
typedef struct bin_addr_info_s bin_addr_info_t;

extern sq_queue_t g_bin_addr_list;

void elf_save_bin_section_addr(struct binary_s *bin);
void elf_delete_bin_section_addr(struct binary_s *bin);
#endif
#ifdef CONFIG_BINFMT_SECTION_UNIFIED_MEMORY
void *elf_find_start_section_addr(struct binary_s *binp);
#endif

#endif							/* __BINFMT_LIBELF_LIBELF_H */
