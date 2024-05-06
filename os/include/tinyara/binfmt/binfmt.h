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
 * include/tinyara/binfmt/binfmt.h
 *
 *   Copyright (C) 2009, 2012, 2014, 2017 Gregory Nutt. All rights reserved.
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

#ifndef __INCLUDE_TINYARA_BINFMT_BINFMT_H
#define __INCLUDE_TINYARA_BINFMT_BINFMT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>

#include <tinyara/arch.h>
#include <tinyara/sched.h>
#include <tinyara/mm/mm.h>
#ifdef CONFIG_BINARY_MANAGER
#include <tinyara/binary_manager.h>
#endif
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/
enum {
	BIN_TEXT,
#ifdef CONFIG_BINFMT_CONSTRUCTORS
	BIN_CTOR,
	BIN_DTOR,
#endif
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	BIN_RO,
#endif
	BIN_DATA,
	BIN_BSS,
	BIN_HEAP,
	BIN_MAX
};

/* The type of one C++ constructor or destructor */

typedef FAR void (*binfmt_ctor_t)(void);
typedef FAR void (*binfmt_dtor_t)(void);

/* This describes the file to be loaded.
 *
 * NOTE 1: The 'filename' must be the full, absolute path to the file to be
 * executed unless CONFIG_LIB_ENVPATH is defined.  In that case,
 * 'filename' may be a relative path; a set of candidate absolute paths
 * will be generated using the PATH environment variable and load_module()
 * will attempt to load each file that is found at those absolute paths.
 */

struct symtab_s;
struct binary_s {
	/* Information provided to the loader to load and bind a module */

	FAR const char *filename;		/* Full path to the binary to be loaded (See NOTE 1 above) */

#ifdef CONFIG_APP_BINARY_SEPARATION
	struct mm_heap_s *uheap;		/* User heap pointer to allocate memory for sections */
	uint32_t ramstart;			/* Start address of ram partition */
	uint32_t ramsize;			/* Size of the RAM paritition */
#endif

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	uint32_t reload;			/* Indicate whether this binary will be reloaded */
	uint32_t data_backup;			/* Start address of copy of data section */
#endif
#ifdef CONFIG_SUPPORT_COMMON_BINARY
	uint8_t islibrary;			/* Is this bin object containing a library */
#ifdef CONFIG_HAVE_CXX
	uint8_t run_library_ctors;		/* Flag to check if we need to run ctors for common binary */
#endif
#ifdef CONFIG_ARM_MPU							/* MPU register values for common binary only */
	uint32_t cmn_mpu_regs[MPU_REG_NUMBER * NUM_APP_REGIONS];	/* Common binary MPU is configured during loading and disabled during unload_module */
#endif
#endif
	FAR char *const *argv;			/* Argument list */
	FAR const struct symtab_s *exports;	/* Table of exported symbols */
	int nexports;				/* The number of symbols in exports[] */

	/* Information provided from the loader (if successful) describing the
	 * resources used by the loaded module.
	 */

	main_t entrypt;				/* Entry point into a program module */
	FAR uint32_t sections[BIN_MAX];		/* Allocated sections */
	FAR size_t sizes[BIN_MAX];		/* Size of sections */

#ifdef CONFIG_XIP_ELF
	FAR uint32_t flash_region_start;	/* Flash partition start */
	FAR uint32_t flash_region_end;		/* Flash partition end */
	FAR uint32_t ram_region_start;		/* RAM region start for app */
	FAR uint32_t ram_region_end;		/* RAM region end for app */
#endif

#ifdef CONFIG_BINFMT_CONSTRUCTORS
	FAR binfmt_ctor_t *ctors;		/* Pointer to a list of constructors */
	FAR binfmt_dtor_t *dtors;		/* Pointer to a list of destructors */
	uint16_t nctors;			/* Number of constructors in the list */
	uint16_t ndtors;			/* Number of destructors in the list */
#endif

	/* Start-up information that is provided by the loader, but may be modified
	 * by the caller between load_module() and exec_module() calls.
	 */

	uint8_t priority;			/* Task execution priority */
	size_t stacksize;			/* Size of the stack in bytes (unallocated) */
	size_t filelen;                 /* Size of binary size, used only when underlying is MTD */
	size_t offset;                  /* Offset of binary from partition start*/
#ifdef CONFIG_BINARY_MANAGER
	uint8_t binary_idx;             /* Index of binary in binary table */
	uint32_t bin_ver;               /* version of binary */
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	char bin_name[BIN_NAME_MAX];    /* Name of binary */
#else
	char *bin_name;                 /* Name of binary */
#endif
#endif

	/* Unload module callback */

	CODE int (*unload)(FAR struct binary_s *bin);
};

/* This describes one binary format handler */

struct binfmt_s {
	/* Supports a singly-linked list */

	FAR struct binfmt_s *next;

	/* Verify and load binary into memory */

	CODE int (*load)(FAR struct binary_s *bin);

	/* Unload module callback */

	CODE int (*unload)(FAR struct binary_s *bin);
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef CONFIG_SUPPORT_COMMON_BINARY
/* A binary data of common library */
extern struct binary_s *g_lib_binp;
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: binfmt_initialize
 *
 * Description:
 *   initialize binfmt subsystem
 *
 ****************************************************************************/

void binfmt_initialize(void);

/****************************************************************************
 * Name: register_binfmt
 *
 * Description:
 *   Register a loader for a binary format
 *
 * Returned Value:
 *   This is a NuttX internal function so it follows the convention that
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int register_binfmt(FAR struct binfmt_s *binfmt);

/****************************************************************************
 * Name: unregister_binfmt
 *
 * Description:
 *   Register a loader for a binary format
 *
 * Returned Value:
 *   This is a NuttX internal function so it follows the convention that
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int unregister_binfmt(FAR struct binfmt_s *binfmt);

/****************************************************************************
 * Name: load_module
 *
 * Description:
 *   Load a module into memory, bind it to an exported symbol take, and
 *   prep the module for execution.
 *
 * Returned Value:
 *   This is a internal function so it follows the convention that
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int load_module(FAR struct binary_s *bin);

/****************************************************************************
 * Name: unload_module
 *
 * Description:
 *   Unload a (non-executing) module from memory.  If the module has
 *   been started (via exec_module) and has not exited, calling this will
 *   be fatal.
 *
 *   However, this function must be called after the module exist.  How
 *   this is done is up to your logic.  Perhaps you register it to be
 *   called by on_exit()?
 *
 * Returned Value:
 *   This is a NuttX internal function so it follows the convention that
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int unload_module(FAR struct binary_s *bin);

/****************************************************************************
 * Name: exec_module
 *
 * Description:
 *   Execute a module that has been loaded into memory by load_module().
 *
 * Returned Value:
 *   This is a internal function so it follows the convention that
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int exec_module(FAR struct binary_s *bin);

/****************************************************************************
 * Name: exec
 *
 * Description:
 *   This is a convenience function that wraps load_ and exec_module into
 *   one call.  If CONFIG_BINFMT_LOADABLE is defined, this function will
 *   schedule to unload the module when task exits.
 *
 *   This non-standard, NuttX function is similar to execv() and
 *   posix_spawn() but differs in the following ways;
 *
 *   - Unlike execv() and posix_spawn() this function accepts symbol table
 *     information as input parameters. This means that the symbol table
 *     used to link the application prior to execution is provided by the
 *     caller, not by the system.
 *   - Unlike execv(), this function always returns.
 *
 *   This non-standard interface is included as a official NuttX API only
 *   because it is needed in certain build modes: exec() is probably the
 *   only want to load programs in the PROTECTED mode. Other file execution
 *   APIs rely on a symbol table provided by the OS. In the PROTECTED build
 *   mode, the OS cannot provide any meaningful symbolic information for
 *   execution of code in the user-space blob so that is the exec() function
 *   is really needed in that build case
 *
 *   The interface is available in the FLAT build mode although it is not
 *   really necessary in that case. It is currently used by some example
 *   code under the apps/ that that generate their own symbol tables for
 *   linking test programs. So although it is not necessary, it can still
 *   be useful.
 *
 *   The interface would be completely useless and will not be supported in
 *   in the KERNEL build mode where the contrary is true: An application
 *   process cannot provide any meaning symbolic information for use in
 *   linking a different process.
 *
 *   NOTE: This function is flawed and useless without CONFIG_BINFMT_LOADABLE
 *   because without that features there is then no mechanism to unload the
 *   module once it exits.
 *
 * Input Parameters:
 *   filename - The path to the program to be executed. If
 *              CONFIG_LIB_ENVPATH is defined in the configuration, then
 *              this may be a relative path from the current working
 *              directory. Otherwise, path must be the absolute path to the
 *              program.
 *   argv     - A pointer to an array of string arguments. The end of the
 *              array is indicated with a NULL entry.
 *   exports  - The address of the start of the caller-provided symbol
 *              table. This symbol table contains the addresses of symbols
 *              exported by the caller and made available for linking the
 *              module into the system.
 *   nexports - The number of symbols in the exports table.
 *
 * Returned Value:
 *   This is an end-user function, so it follows the normal convention:
 *   It returns the PID of the exec'ed module.  On failure, it returns
 *   -1 (ERROR) and sets errno appropriately.
 *
 ****************************************************************************/

int exec(FAR const char *filename, FAR char *const *argv, FAR const struct symtab_s *exports, int nexports);

/****************************************************************************
 * Name: binfmt_exit
 *
 * Description:
 *   This function may be called when a tasked loaded into RAM exits.
 *   This function will unload the module when the task exits and reclaim
 *   all resources used by the module.
 *
 * Input Parameters:
 *   bin - This structure must have been allocated with kmm_malloc() and must
 *         persist until the task unloads
 *
 * Returned Value:
 *   This is a NuttX internal function so it follows the convention that
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

#ifdef CONFIG_BINFMT_LOADABLE
int binfmt_exit(FAR struct binary_s *bin);
#endif
/****************************************************************************
 * Name: load_binary
 *
 * Description:
 *   This is a convenience function that wraps load_module and exec_module into
 *   one call.  If CONFIG_BINFMT_LOADABLE is defined, this function will
 *   schedule to unload the module when task exits.
 *
 * Input Parameters:
 *   filename - The path to the program to be executed. If
 *              CONFIG_LIB_ENVPATH is defined in the configuration, then
 *              this may be a relative path from the current working
 *              directory. Otherwise, path must be the absolute path to the
 *              program.
 *
 *   load_attr - This structure contains the information that the binary
 *               to be loaded.
 *
 * Returned Value:
 *   This is an end-user function, so it follows the normal convention:
 *   It returns the PID of the exec'ed module.  On failure, it returns
 *   -1 (ERROR) and sets errno appropriately.
 *
 ****************************************************************************/
#ifdef CONFIG_BINARY_MANAGER
int load_binary(int binary_idx, FAR const char *filename, load_attr_t *load_attr);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif
#endif							/* __INCLUDE_TINYARA_BINFMT_BINFMT_H */
