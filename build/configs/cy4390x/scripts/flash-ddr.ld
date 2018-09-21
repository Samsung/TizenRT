/****************************************************************************
 *
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of 
 * Cypress Semiconductor Corporation. All Rights Reserved.
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

/* The application subsystem of BCM43909 has 2MB on-chip SRAM, 640KB ROM,
 * , an 8KB Always-On-RAM, and 128MB DDR Memory
 */

MEMORY
{
	FLOPS	(rwx)	: ORIGIN = 0x00000000, LENGTH = 100
	ROM	(rx)	: ORIGIN = 0x00400000, LENGTH = 640K
	SRAM	(rwx)	: ORIGIN = 0x004A0000, LENGTH = 2048K
	AONRAM	(rwx)	: ORIGIN = 0x006A0000, LENGTH = 8K
	DDR	(rwx)	: ORIGIN = 0x40000000, LENGTH = 128M
}

irq_instruction_vector_low_reset = 0xB03EF000;
OUTPUT_ARCH(arm)
OUTPUT_FORMAT(elf32-littlearm)
EXTERN(_vectors)
ENTRY(__start)

SECTIONS
{

    /* To copy the vector tables into sram area */
    .ram_text : {
         . = ALIGN(32);

         KEEP(*(.text.sram*))
    } > SRAM

	.text : {
		_stext = ABSOLUTE(.);
		KEEP(*(.text.startup))
		*(.text .text.*)
		*(.fixup)
		*(.gnu.warning)
		*(.rodata .rodata.*)
		*(.gnu.linkonce.t.*)
		*(.glue_7)
		*(.glue_7t)
		*(.got)
		*(.gcc_except_table)
		*(.gnu.linkonce.r.*)
		_etext = ABSOLUTE(.);
	} > DDR

	.vectors : {
		vectors_copyfrom = LOADADDR(.vectors);
		KEEP(*(.vectors))
	} > FLOPS AT > SRAM

	.init_section : {
		_sinit = ABSOLUTE(.);
		KEEP(*(.init_array .init_array.*))
		_einit = ABSOLUTE(.);
	} > SRAM

	.ARM.extab : {
		*(.ARM.extab*)
	} > SRAM

	__exidx_start = ABSOLUTE(.);
	.ARM.exidx : {
		*(.ARM.exidx*)
	} > SRAM
	__exidx_end = ABSOLUTE(.);

	_eronly = ABSOLUTE(.);

	.data : {
		_sdata = ABSOLUTE(.);
		*(.data .data.*)
		*(.gnu.linkonce.d.*)
		CONSTRUCTORS
		_edata = ABSOLUTE(.);
	} > DDR

	.bss : {
		_sbss = ABSOLUTE(.);
		*(.bss .bss.*)
		*(.gnu.linkonce.b.*)
		*(COMMON)
		_ebss = ABSOLUTE(.);
	} > DDR
    
	/* Stabs debugging sections. */
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
