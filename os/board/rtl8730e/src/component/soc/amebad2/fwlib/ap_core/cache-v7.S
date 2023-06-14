
/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

/*
 * invalidate_l1
 */
	.global invalidate_l1
	.type invalidate_l1, %function
invalidate_l1:
	mov	r0, #0
	mcr	p15, 2, r0, c0, c0, 0
	mrc	p15, 1, r0, c0, c0, 0

	movw	r1, #0x7fff
	and	r2, r1, r0, lsr #13

	movw	r1, #0x3ff

	and	r3, r1, r0, lsr #3			@ NumWays - 1
	add	r2, r2, #1					@ NumSets

	and	r0, r0, #0x7
	add	r0, r0, #4					@ SetShift

	clz	r1, r3						@ WayShift
	add	r4, r3, #1					@ NumWays
1:	sub	r2, r2, #1					@ NumSets--
	mov	r3, r4						@ Temp = NumWays
2:	subs	r3, r3, #1				@ Temp--
	mov	r5, r3, lsl r1
	mov	r6, r2, lsl r0
	orr	r5, r5, r6					@ Reg = (Temp<<WayShift)|(NumSets<<SetShift)
	mcr	p15, 0, r5, c7, c6, 2
	bgt	2b
	cmp	r2, #0
	bgt	1b
	dsb	st
	isb
	bx	lr
	.size invalidate_l1, . - invalidate_l1

/*
 * invalidate_dcache - invalidate the entire d-cache by set/way
 *
 * corrupted registers: r0-r7, r9-r11 (r6 only in Thumb mode)
 */
	.global	invalidate_dcache_all
	.type invalidate_dcache, %function
invalidate_dcache_all:
	mrc	p15, 1, r0, c0, c0, 1		@ read CLIDR
	ands	r3, r0, #0x7000000
	mov	r3, r3, lsr #23				@ cache level value (naturally aligned)
	beq	finished
	mov	r10, #0						@ start with level 0
loop1:
	add	r2, r10, r10, lsr #1		@ work out 3xcachelevel
	mov	r1, r0, lsr r2				@ bottom 3 bits are the Cache type for this level
	and	r1, r1, #7					@ get those 3 bits alone
	cmp	r1, #2
	blt	skip						@ no cache or only instruction cache at this level
	mcr	p15, 2, r10, c0, c0, 0		@ write the Cache Size selection register
	isb								@ isb to sync the change to the CacheSizeID reg
	mrc	p15, 1, r1, c0, c0, 0		@ reads current Cache Size ID register
	and	r2, r1, #7					@ extract the line length field
	add	r2, r2, #4					@ add 4 for the line length offset (log2 16 bytes)
	ldr	r4, =0x3ff
	ands	r4, r4, r1, lsr #3		@ r4 is the max number on the way size (right aligned)
	clz	r5, r4						@ r5 is the bit position of the way size increment
	ldr	r7, =0x7fff
	ands	r7, r7, r1, lsr #13		@ r7 is the max number of the index size (right aligned)
loop2:
	mov	r9, r4						@ r9 working copy of the max way size (right aligned)
loop3:
	orr	r11, r10, r9, lsl r5		@ factor in the way number and cache number into r11
	orr	r11, r11, r7, lsl r2		@ factor in the index number
	mcr	p15, 0, r11, c7, c6, 2		@ invalidate by set/way
	subs	r9, r9, #1				@ decrement the way number
	bge	loop3
	subs	r7, r7, #1				@ decrement the index
	bge	loop2
skip:
	add	r10, r10, #2				@ increment the cache number
	cmp	r3, r10
	bgt	loop1

finished:
	mov	r10, #0						@ swith back to cache level 0
	mcr	p15, 2, r10, c0, c0, 0		@ select current cache level in cssr
	dsb
	isb

	bx	lr
	.size invalidate_dcache_all, . - invalidate_dcache_all
