/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#define __inst_arm(x) .long x
#define __inst_arm_thumb32(arm_opcode, thumb_opcode) __inst_arm(arm_opcode)

#define __SMC(imm4) __inst_arm_thumb32(				\
	0xE1600070 | (((imm4) & 0xF) << 0),				\
	0xF7F08000 | (((imm4) & 0xF) << 16)				\
)

	.macro __SMC imm4
	.long
	0xE1600070 | (((\imm4) & 0xF) << 0),
	0xF7F08000 | (((\imm4) & 0xF) << 16)
	.endm

	.macro SMCCC_SMC
	__SMC(0)
	.endm

	.macro SMCCC instr
	mov	r12, sp
	push	{r4-r7}
	ldm	r12, {r4-r7}
	\instr
	pop	{r4-r7}
	ldr	r12, [sp, #(4 * 4)]
	stm	r12, {r0-r3}
	bx	lr
	.endm

/*
 * void smccc_smc(unsigned long a0, unsigned long a1, unsigned long a2,
 *		  unsigned long a3, unsigned long a4, unsigned long a5,
 *		  unsigned long a6, unsigned long a7, struct arm_smccc_res *res,
 *		  struct arm_smccc_quirk *quirk)
 */
	.global __arm_smccc_smc
__arm_smccc_smc:
	SMCCC SMCCC_SMC
