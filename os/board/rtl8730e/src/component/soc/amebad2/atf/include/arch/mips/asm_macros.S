/*
 * Copyright (c) 2013-2015, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2016, Cortina Access. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __ASM_MACROS_S__
#define __ASM_MACROS_S__
#include <asm_macros_common.S>
    /*
     * Declare the exception vector table
     */
    .macro vector_base  label
        .section .vectors, "ax"
        .align 11, 0
        \label:
    .endm

    /*
     * Create an entry in the exception vector table
     */
    .macro vector_entry  label
        .section .vectors, "ax"
        .align 7, 0
        \label:
    .endm

    /*
     * This macro verifies that the given vector doesn't exceed the
     * architectural limit of 32 instructions. This is meant to be placed
     * immediately after the last instruction in the vector. It takes the
     * vector entry as the parameter
     */
    .macro check_vector_size since
        .if (. - \since) > (32 * 4)
            .error "Vector exceeds 32 instructions"
        .endif
    .endm


#if ENABLE_PLAT_COMPAT
    /*
     * This macro calculates the base address of an MP stack using the
     * platform_get_core_pos() index, the name of the stack storage and
     * the size of each stack
     * Out: v0 = physical address of stack base
     * Clobber: t1, t2
     */
    .macro get_mp_stack _name, _size
        jal     platform_get_core_pos
        nop
        la      $t1, (\_name + \_size)
        li      $t2, \_size

        multu   $v0, $t2
        mflo    $t2
        add     $v0, $t2, $t1
    .endm
#endif

    /*
     * This macro calculates the base address of the current CPU's MP stack
     * using the plat_my_core_pos() index, the name of the stack storage
     * and the size of each stack
     * Out: v0 = physical address of stack base
     * Clobber: t1,t2
     */
    .macro get_my_mp_stack _name, _size
        jal     platform_get_core_pos
        nop
        la      $t1, (\_name + \_size)
        li      $t2, \_size

        multu   $v0, $t2
        mflo    $t2
        add     $v0, $t2, $t1
    .endm

    /*
     * This macro calculates the base address of a UP stack using the
     * name of the stack storage and the size of the stack
     * Out: v0 = physical address of stack base
     */
    .macro get_up_stack _name, _size
        la $v0, (\_name + \_size)
    .endm

    /*
     * Helper macro to load arbitrary values into 32 registers
     */
    .macro mov_imm _reg, _val
        .if (\_val) == 0
            li     \_reg,  0
        .else
            lui     \_reg, (0xffff & (\_val>>16))
            ori     \_reg, \_reg, (\_val& 0xffff)
        .endif
    .endm

#endif /* __ASM_MACROS_S__ */
