/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 *
 *   Copyright (C) 2020 Gregory Nutt. All rights reserved.
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

/* The QEMU Generic Virtual Platform (virt) has two 64MB of FLASH beginning at address 0x00000000 and 0x04000000
 * and configurable capacity of RAM beginning at 0x4000:0000.
 * But QEMU does not have memory alias feature that can be configured by guest OS, So We need to do dramboot for supporting A/B update.
 */

MEMORY
{
    RAM_FDT (rwx) 	 : ORIGIN = 0x40000000, LENGTH = 1M
    RAM_KERNEL (rwx) : ORIGIN = 0x40100400, LENGTH = 63M - 1K
}

OUTPUT_ARCH(arm)
ENTRY(_stext_flash)
SECTIONS
{
	.text ORIGIN(RAM_KERNEL):
	{
		__flash_text_start__ = .;
		_stext_flash = ABSOLUTE(.);
		_stext_ram = ABSOLUTE(.);
		. = ALIGN(0x4);

		*(.vectors)
		*(.start)
		*(.ps_boot)
		*(.cpu1start)

		*(.text*)
		*(.rodata*)

		/* Place arm_mmu.c in PSRAM to reduce delay in boot sequence */
		*arm_mmu.o (.text*)
		*arm_cpugating.o (.text*)

		/* Add This for C++ support */
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP(*(.preinit_array))
		__preinit_array_end = .;
		. = ALIGN(4);
		__init_array_start = .;
		_sinit = ABSOLUTE(.);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		_einit = ABSOLUTE(.);
		__init_array_end = .;
		. = ALIGN(4);
		__fini_array_start = .;
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		__fini_array_end = .;
		/*-----------------*/

		. = ALIGN(4096);
		_etext_ram = ABSOLUTE(.);
		_etext_flash = ABSOLUTE(.);
		__flash_text_end__ = .;
	} > RAM_KERNEL

	/* C++ .ARM.extab/.ARM.exidx address shall same as *(.text*) */
	.ARM.extab  :
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > RAM_KERNEL

	.ARM.exidx :
	{
		__exidx_start = .;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		__exidx_end = .;
	} > RAM_KERNEL

	_eronly = ABSOLUTE(.);

	.data :
	{
		__data_start__ = .;
		. = ALIGN(4);
		_sdata = ABSOLUTE(.);
		*(.timesensitive.data*)

		*(.data)
		*(.data*)
		_edata = ABSOLUTE(.);
		. = ALIGN(4);

		/* All data end */
		__data_end__ = .;
	} > RAM_KERNEL

	.bss (NOLOAD):
	{
		__bss_start__ = .;
		_sbss = ABSOLUTE(.);
		KEEP(*(.bdsram.bss));
		*(.bss)
		*(.bss.*)
		*(COMMON)
		__bss_end__ = .;
	} > RAM_KERNEL

	.heap (NOLOAD) :
	{
		__end__ = .;
		end = __end__;
		__HeapBase = .;
		KEEP(*(.heap))
		. = . + 8192;
		__HeapLimit = .;
		__heap_limit = .;	/* Add for _sbrk */
		__HeapSize = __HeapLimit - __HeapBase;
	} > RAM_KERNEL

	.stack (NOLOAD) :
	{
		. = ALIGN(16);
		__StackLimit = .;
		KEEP(*(.stack))
		KEEP(*(.bdsram.data))
		_snoinit = ABSOLUTE(.);
		*(.noinit*)
		_enoinit = ABSOLUTE(.);
		_sidle_stack = ABSOLUTE(.);
		/* For TizenRT to locate idle thread stack */
		_ebss = ABSOLUTE(.);
		. = . + 1024;
	} > RAM_KERNEL

					/* Stabs debugging sections.	*/
	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment 0 : { *(.comment) }
	.debug_abbrev 0 : { *(.debug_abbrev) }
	.debug_info 0 : { *(.debug_info) }
	.debug_line 0 : { *(.debug_line) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	.debug_aranges 0 : { *(.debug_aranges) }
}
