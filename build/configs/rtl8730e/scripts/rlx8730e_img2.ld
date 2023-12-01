/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

OUTPUT_ARCH(arm)
ENTRY(_vector_start)

MEMORY
{

	/* KM4 ROM */
	KM4_IROM (rx) :                         ORIGIN = 0x00000000, LENGTH = 0x0001E000 - 0x00000000	/* KM4 IROM_S: 120k */
	KM4_IROM_NS (rx) :                      ORIGIN = 0x0001E000, LENGTH = 0x00033000 - 0x0001E000	/* KM4 IROM_NS: 84k */
	KM4_IROM_STDLIB_NS (rx) :               ORIGIN = 0x00033000, LENGTH = 0x00046000 - 0x00033000	/* KM4 STDLIB ROM: 76k */
	KM4_DROM_NS (rx) :                      ORIGIN = 0x00046000, LENGTH = 0x00054000 - 0x00046000	/* KM4 DROM_NS: 56k */
	KM4_DROM (rx) :                         ORIGIN = 0x00054000, LENGTH = 0x00058000 - 0x00054000	/* KM4 DROM_S: 16k */

	/* KM4 Secure RAM, BIT(28)=1 */
	KM4_ROMBSS_RAM_S (rw) :                 ORIGIN = 0x30000000, LENGTH = 0x30001000 - 0x30000000	/* KM4 ROM BSS RAM S: 4k */
	KM4_MSP_RAM_S (rw) :                    ORIGIN = 0x30001000, LENGTH = 0x30003000 - 0x30001000	/* KM4 MSP_S RAM: 8k */
	KM4_BOOTLOADER_RAM_S (rwx) :            ORIGIN = 0x30003020, LENGTH = 0x30014000 - 0x30003020	/* KM4 BOOT Loader RAM: 68k */
	KM4_FLOADER_RAM_S (rwx) :               ORIGIN = 0x30003020, LENGTH = 0x3001B000 - 0x30003020	/* KM4 ImgTool Flash Loader RAM: 95k */
	KM4_BD_RAM_NSC (rwx) :                  ORIGIN = 0x6015B020, LENGTH = 0x6015C000 - 0x6015B020	/* KM4 BD RAM NSC: 3k */
	KM4_BD_RAM_ENTRY (rwx) :                ORIGIN = 0x6015C000, LENGTH = 0x60160000 - 0x6015C000	/* KM4 BD RAM ENTRY: 16k */
	KM4_BD_RAM_S (rwx) :                    ORIGIN = 0x70160020, LENGTH = 0x70180000 - 0x70160020	/* KM4 BD RAM S: 127k */

	/* KM4 Non-secure RAM */
	KM4_BD_DRAM (rwx) :                     ORIGIN = 0x60000020, LENGTH = 0x6015B000 - 0x60000020	/* KM4 MAIN DRAM: 1387k */
	KM4_MSP_RAM_NS (rw) :                   ORIGIN = 0x2001B000, LENGTH = 0x2001C000 - 0x2001B000	/* KM4  MSP_NS RAM: 4k */
	KM4_ROMBSS_RAM_COM (rw) :               ORIGIN = 0x2001C000, LENGTH = 0x2001D000 - 0x2001C000	/* KM4  ROM BSS COMMON(S & NS both used) RAM: 4k */
	KM4_ROMBSS_RAM_NS (rw) :                ORIGIN = 0x2001D000, LENGTH = 0x2001E000 - 0x2001D000	/* KM4 ROM BSS NS RAM: 4k */
	KM4_STDLIB_HEAP_RAM_NS (rw) :           ORIGIN = 0x2001E000, LENGTH = 0x2001F000 - 0x2001E000	/* KM4 ROM STDLIB HEAP: 4k */
	KM4_AUDIO_BUFFER (rw) :                 ORIGIN = 0x20020000, LENGTH = 0x20040000 - 0x20020000	/* KM4 AUDIO BUFFER: 128k */

	/* KM4 Non-secure RAM for XIP only */
	KM4_BD_RAM (rwx) :                      ORIGIN = 0x20014020, LENGTH = 0x2001B000 - 0x20014020	/* KM4 MAIN SRAM: 28k */

	/* KM4 EXTENTION SRAM */
	EXTENTION_SRAM (rwx) :                  ORIGIN = 0x22000000, LENGTH = 0x23000000 - 0x22000000	/* KM4 EXTENTION SRAM: 16384k (Wi-Fi 40KB + Bluetooth 256KB) */

	/* Retention RAM */
	RETENTION_RAM (rwx) :                   ORIGIN = 0x23020000, LENGTH = 0x23020200 - 0x23020000	/* KM0 Retention SRAM: 512B */

	/* KM0 ROM */
	KM0_IROM (rx) :                         ORIGIN = 0x00000000, LENGTH = 0x0000C000 - 0x00000000	/* KM0 IROM: 48k */
	KM0_DROM (rx) :                         ORIGIN = 0x0000C000, LENGTH = 0x00010000 - 0x0000C000	/* KM0 DROM: 16k */

	/* KM0 RAM */
	KM0_ROMBSS_RAM (rw) :                   ORIGIN = 0x23000000, LENGTH = 0x23001000 - 0x23000000	/* KM0 ROM BSS RAM: 4k */
	KM0_MSP_RAM (rw) :                      ORIGIN = 0x23001000, LENGTH = 0x23002000 - 0x23001000	/* KM0 MSP RAM: 4k */
	KM0_BD_RAM (rwx) :                      ORIGIN = 0x23002020, LENGTH = 0x2301FD00 - 0x23002020	/* KM0 MAIN RAM: 119k */
	KM0_IPC_RAM (rwx) :                     ORIGIN = 0x2301FD00, LENGTH = 0x23020000 - 0x2301FD00	/* KM0 IPC RAM: 768B */

	/* Flash */
	KM4_BOOT_XIP (rx) :                     ORIGIN = 0x0A000020, LENGTH = 0x0C000000 - 0x0A000020	/* KM4 Bootloader Virtual address: 32767k */
	KM0_IMG2_XIP (rx) :                     ORIGIN = 0x0C000020, LENGTH = 0x0E000000 - 0x0C000020	/* KM0 IMG2 Virtual address: 32767k */
	KM4_IMG2_XIP (rx) :                     ORIGIN = 0x0D000020, LENGTH = 0x0E000000 - 0x0D000020	/* KM4 IMG2 Virtual address: 32767k */
	CA32_IMG2_XIP (rx) :                    ORIGIN = 0x0E000020, LENGTH = 0x10000000 - 0x0E000020	/* KM0 IMG2 Virtual address: 32767k */

	BTRACE (rx) :                           ORIGIN = 0xC8800000, LENGTH = 0xC9000000 - 0xC8800000	/* BT LOG Virtual address: 8192k */

	/* CA32 ROM */
	CA32_IROM (rx) :                        ORIGIN = 0x00000000, LENGTH = 0x00000400 - 0x00000000	/* KM0 IROM: 1k */

	/* CA32 RAM */
	CA32_BL1_SRAM_S (rwx) :                 ORIGIN = 0x3001FE00, LENGTH = 0x30020000 - 0x3001FE00	/* CA32 BL1 SRAM: 512B */
	CA32_BL1_DRAM_S (rwx) :                 ORIGIN = 0x70180020, LENGTH = 0x701A0000 - 0x70180020	/* CA32 BL1 DRAM S: 127k */
	CA32_FIP_DRAM_NS (rwx) :                ORIGIN = 0x60400000, LENGTH = 0x60600000 - 0x60400000	/* CA32 FIP(BL2+BL32+BL33) IMG: 2MB */
	CA32_BL3_DRAM_NS (rwx) :                ORIGIN = 0x60300000, LENGTH = 0x60800000 - 0x60300000	/* CA32 BL3 DRAM NS: 5120k */
	KM0_BD_DRAM (rwx) :                     ORIGIN = 0x6FFFFFFF, LENGTH = 0x6FFFFFFF - 0x6FFFFFFF	/* KM0 BD DRAM */
	KM4_DRAM_HEAP_EXT (rwx) :               ORIGIN = 0x6FFFFFFF, LENGTH = 0x6FFFFFFF - 0x6FFFFFFF	/* KM4 PSRAM HEAP EXT */
}

__rom_stdlib_text_start__ = ORIGIN(KM4_IROM_STDLIB_NS);
__km0_ipc_memory_start__ = ORIGIN(KM0_IPC_RAM);
__km4_boot_text_start__ = ORIGIN(KM4_BOOT_XIP);
__km0_flash_text_start__ = ORIGIN(KM0_IMG2_XIP);
__km4_flash_text_start__ = ORIGIN(KM4_IMG2_XIP);
__ca32_flash_text_start__ = ORIGIN(CA32_IMG2_XIP);

__ca32_bl1_sram_start__ = ORIGIN(CA32_BL1_SRAM_S);
__ca32_bl1_dram_start__ = ORIGIN(CA32_BL1_DRAM_S);
__ca32_fip_dram_start__ = ORIGIN(CA32_FIP_DRAM_NS);

SECTIONS
{
	.text :
	{
		. = ALIGN(0x4);
		_stext_ram = ABSOLUTE(.);
		_stext_flash = ABSOLUTE(.);
		__text_start__ = .;

		*(.freertos_vectors)
		*(.vectors)
		*(.boot)
		*(.timesensitive.text*)
		*(.sramdram.only.text*)

		*(.text)		/* .text sections (code) */
		*(.text*)		/* .text* sections (code) */
		*(.rodata)		/* .rodata sections (constants, strings, etc.) */
		*(.rodata*)		/* .rodata* sections (constants, strings, etc.) */
		*(.glue_7)		/* glue arm to thumb code */
		*(.glue_7t)		/* glue thumb to arm code */
		*(.vfp11_veneer)
		*(.eh_frame)
		KEEP (*(.init))
		KEEP (*(.fini))
		. = ALIGN (4);
		__cmd_table_start__ = .;
		KEEP(*(.cmd.table.data*))
		__cmd_table_end__ = .;
		. = ALIGN (4);
		__ipc_table_start__ = .;
		KEEP(*(*.ipc.table.data*))
		__ipc_table_end__ = .;
		. = ALIGN (4);
		__git_ver_table_start__ = .;
		KEEP(*(.git.ver.data))
		__git_ver_table_end__ = .;
		. = ALIGN(4);
		_etext_ram = ABSOLUTE(.);
	} > CA32_BL3_DRAM_NS

	.ARM.extab :
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > CA32_BL3_DRAM_NS

	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > CA32_BL3_DRAM_NS
	__exidx_end = .;

	.ctors :
	{
		__CTOR_LIST__ = .;
		/* gcc uses crtbegin.o to find the start of
		   the constructors, so we make sure it is
		   first.  Because this is a wildcard, it
		   doesn't matter if the user does not
		   actually link against crtbegin.o; the
		   linker won't look for a file to match a
		   wildcard.  The wildcard also means that it
		   doesn't matter which directory crtbegin.o
		   is in.  */
		KEEP (*crtbegin.o(.ctors))
		KEEP (*crtbegin?.o(.ctors))
		/* We don't want to include the .ctor section from
		   from the crtend.o file until after the sorted ctors.
		   The .ctor section from the crtend file contains the
		   end of ctors marker and it must be last */
		KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*(.ctors))
		__CTOR_END__ = .;
	} > CA32_BL3_DRAM_NS

	.dtors :
	{
		__DTOR_LIST__ = .;
		KEEP (*crtbegin.o(.dtors))
		KEEP (*crtbegin?.o(.dtors))
		KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*(.dtors))
		__DTOR_END__ = .;
	} > CA32_BL3_DRAM_NS

	.preinit_array :
	{
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array*))
		PROVIDE_HIDDEN (__preinit_array_end = .);
	} > CA32_BL3_DRAM_NS

	.init_array :
	{

		PROVIDE_HIDDEN (__init_array_start = .);
		_sinit = ABSOLUTE(.);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array*))
		_einit = ABSOLUTE(.);
		PROVIDE_HIDDEN (__init_array_end = .);
	} > CA32_BL3_DRAM_NS

	.fini_array :
	{
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(SORT(.fini_array.*)))
		KEEP (*(.fini_array*))
		PROVIDE_HIDDEN (__fini_array_end = .);

		. = ALIGN(4096);
		__text_end__ = .;
		_etext_flash = ABSOLUTE(.);
	} > CA32_BL3_DRAM_NS

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
	} > CA32_BL3_DRAM_NS

/*
	.mmu_tbl (ALIGN(0x4000)) :
	{
		__mmu_tbl_start = .;
		*(.mmu_tbl)
		*(base_xlat_table)
		*(xlat_table)
		__mmu_tbl_end = .;
	} > CA32_BL3_DRAM_NS
*/

	.bss (NOLOAD):
	{
		__bss_start__ = .;
		_sbss = ABSOLUTE(.);
		KEEP(*(.bdsram.bss));
		*(.bss)
		*(.bss.*)
		*(COMMON)
		__bss_end__ = .;
	} > CA32_BL3_DRAM_NS

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
	} > CA32_BL3_DRAM_NS

	.stack (NOLOAD) :
	{
		. = ALIGN(16);
		__StackLimit = .;
		KEEP(*(.stack))
		KEEP(*(.bdsram.data))
		_sidle_stack = ABSOLUTE(.);
		/* For TizenRT to locate idle thread stack */
		_ebss = ABSOLUTE(.);
		. = . + 1024;
		. = ALIGN(4096);
		__ram_nocache_start__ = .;
		KEEP(*(.ncsram.data))
		. = ALIGN(4096);
		__ram_nocache_end__ = .;
	} > CA32_BL3_DRAM_NS

	/* put in the end of psram part */
	.psram_heap.start (NOLOAD):
	{
		. = ALIGN(64);
		__psram_heap_buffer_start__ = .;
		_sext_heap = ABSOLUTE(.);
		_psram_heap_start = ABSOLUTE(.);
	} > CA32_BL3_DRAM_NS

	.ARM.attributes 0 : { KEEP (*(.ARM.attributes)) KEEP (*(.gnu.attributes)) }

    /* Initialize the stack at the end of the memory block. */
    __StackTop = .;
    __stack = __StackTop;

    ASSERT(__StackLimit >= __HeapLimit, "region data overflowed with stack and heap")

    PROVIDE(__stack = __StackTop);

    _end = ALIGN(0x10);

	/* Heap start address */
	__PsramStackLimit = ABSOLUTE(ORIGIN(CA32_BL3_DRAM_NS) + LENGTH(CA32_BL3_DRAM_NS));
}

__psram_heap_buffer_size__ = ORIGIN(CA32_BL3_DRAM_NS) + LENGTH(CA32_BL3_DRAM_NS) - __psram_heap_buffer_start__;
