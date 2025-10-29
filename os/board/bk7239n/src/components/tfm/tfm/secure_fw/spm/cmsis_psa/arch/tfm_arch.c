/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "compiler_ext_defs.h"
#include "security_defs.h"
#include "tfm_arch.h"
#include "tfm_core_utils.h"
#include "utilities.h"

__naked void tfm_arch_free_msp_and_exc_ret(uint32_t msp_base,
                                           uint32_t exc_return)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                        \n"
#endif
        "mov     sp, r0                         \n"

        /* Seal Main Stack before using */
        "ldr     r2, ="M2S(STACK_SEAL_PATTERN)" \n"
        "ldr     r3, ="M2S(STACK_SEAL_PATTERN)" \n"
        "push    {r2, r3}                       \n"
        "bx      r1                             \n"
    );
}

void tfm_arch_set_context_ret_code(void *p_ctx_ctrl, uint32_t ret_code)
{
    struct context_ctrl_t *ctx_ctrl = (struct context_ctrl_t *)p_ctx_ctrl;

    /*
     * If a cross call is pending (cross_frame != CROSS_RETCODE_EMPTY), write
     * return value to the frame position.
     * Otherwise, write the return value to the state context on stack.
     */
    if (ctx_ctrl->cross_frame) {
        ((struct cross_call_abi_frame_t *)ctx_ctrl->cross_frame)->a0 = ret_code;
        ctx_ctrl->retcode_status = CROSS_RETCODE_UPDATED;
    } else {
        ((struct full_context_t *)ctx_ctrl->sp)->stat_ctx.r0 = ret_code;
    }
}

/* Caution: Keep 'uint32_t' always for collecting thread return values! */
__attribute__((naked)) uint32_t tfm_arch_trigger_pendsv(void)
{
    __ASM volatile(
#ifndef __ICCARM__
        ".syntax unified                               \n"
#endif
        "ldr     r0, ="M2S(SCB_ICSR_ADDR)"             \n"
        "ldr     r1, ="M2S(SCB_ICSR_PENDSVSET_BIT)"    \n"
        "str     r1, [r0, #0]                          \n"
        "dsb     #0xf                                  \n"
        "isb                                           \n"
        "bx      lr                                    \n"
    );
}

void tfm_arch_init_context(void *p_ctx_ctrl,
                           uintptr_t pfn, void *param, uintptr_t pfnlr)
{
    uintptr_t sp = ((struct context_ctrl_t *)p_ctx_ctrl)->sp;
    struct full_context_t *p_tctx =
            (struct full_context_t *)arch_seal_thread_stack(sp);

    p_tctx--;

    spm_memset(p_tctx, 0, sizeof(*p_tctx));

    ARCH_CTXCTRL_EXCRET_PATTERN(&p_tctx->stat_ctx, param, pfn, pfnlr);

    ((struct context_ctrl_t *)p_ctx_ctrl)->exc_ret  = EXC_RETURN_THREAD_S_PSP;
    ((struct context_ctrl_t *)p_ctx_ctrl)->sp       = (uintptr_t)p_tctx;
}

uint32_t tfm_arch_refresh_hardware_context(void *p_ctx_ctrl)
{
    struct context_ctrl_t *ctx_ctrl;
    struct tfm_state_context_t *sc;

    ctx_ctrl  = (struct context_ctrl_t *)p_ctx_ctrl;
    sc = &(((struct full_context_t *)(ctx_ctrl->sp))->stat_ctx);

    tfm_arch_set_psplim(ctx_ctrl->sp_limit);
    __set_PSP((uintptr_t)sc);

    return ctx_ctrl->exc_ret;
}
