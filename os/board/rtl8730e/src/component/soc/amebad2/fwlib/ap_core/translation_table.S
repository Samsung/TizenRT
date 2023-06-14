/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

/*****************************************************************************/
/**
* @file translation_table.s
*
* @addtogroup a7_boot_code
* @{
* <h2> translation_table.S </h2>
* translation_table.S contains a static page table required by MMU for
* cortex-A7. This translation table is flat mapped (input address = output
* address) with default memory attributes defined for sheipa platform. It
* utilizes short descriptor translation table format with each section defining
* 1MB of memory.
*
* The overview of translation table memory attributes is described below.
*
*|                       | Memory Range            | Definition in Translation Table   |
*|-----------------------|-------------------------|-----------------------------------|
*|Boot ROM				| 0x00000000 - 0x00100000-1 | Device Memory  						|
*|Reserved				| 0x00100000 - 0x08000000-1 | Unassigned							|
*|Flash					| 0x08000000 - 0x10000000-1 | Cacheable, write-back, write-allocate	|
*|Reserved				| 0x10000000 - 0x20000000-1 | Unassigned							|
*|SRAM					| 0x20000000 - 0x40000000-1 | Write-back, no write-allocate			|
*|Memory mapped device	| 0x40000000 - 0x60000000-1 | Device Memory							|
*|DRAM					| 0x60000000 - 0x80000000-1 | Cacheable, write-back, write-allocate	|
*|Memory mapped device	| 0x80000000 - 0xFFFFFFFF   | Device Memory							|
*
******************************************************************************/
	.globl  MMUTable

	.section .mmu_tbl,"a"

MMUTable:
	/* Each table entry occupies one 32-bit word and there are
	 * 4096 entries, so the entire table takes up 16KB.
	 * Each entry covers a 1MB section.
	 */
.set SECT, 0

.word	SECT + 0xc06		/* 0x00000000 - 0x00100000-1 (Boot ROM) */
.set	SECT, SECT+0x100000	/* S=b0 TEX=b000 AP=b11, Domain=b0, C=b0, B=b1 */

.rept	0x7F				/* 0x00100000 - 0x08000000-1 (unassigned/reserved) */
.word	SECT + 0x0
.set	SECT, SECT+0x100000
.endr

.rept	0x80				/* 0x08000000 - 0x10000000-1 (Flash) */
.word	SECT + 0x15de6		/* S=b1 TEX=b101 AP=b11, Domain=b1111, C=b0, B=b1 */
.set	SECT, SECT+0x100000
.endr

.rept	0x100				/* 0x10000000 - 0x20000000-1 (unassigned/reserved) */
.word	SECT + 0x0
.set	SECT, SECT+0x100000
.endr

.rept	0x200				/* 0x20000000 - 0x40000000-1 (SRAM) */
.word	SECT + 0xc0e		/* S=b0 TEX=b000 AP=b11, Domain=b0, C=b1, B=b1 */
.set	SECT, SECT+0x100000
.endr

.rept	0x200				/* 0x40000000 - 0x60000000-1 (Device) */
.word	SECT + 0xc06		/* S=b0 TEX=b000 AP=b11, Domain=b0, C=b0, B=b1 */
.set	SECT, SECT+0x100000
.endr

.rept	0x200				/* 0x60000000 - 0x80000000-1 (DRAM) */
.word	SECT + 0x15de6		/* S=b1 TEX=b101 AP=b11, Domain=b1111, C=b0, B=b1 */
.set	SECT, SECT+0x100000
.endr

.rept	0x800				/* 0x80000000 - 0xFFFFFFFF (Device) */
.word	SECT + 0xc06		/* S=b0 TEX=b000 AP=b11, Domain=b0, C=b0, B=b1 */
.set	SECT, SECT+0x100000
.endr

.end
/**
* @} End of "addtogroup a7_boot_code".
*/
