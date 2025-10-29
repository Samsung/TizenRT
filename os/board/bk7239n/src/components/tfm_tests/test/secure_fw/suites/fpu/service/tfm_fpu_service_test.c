/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include "psa/client.h"
#include "psa/service.h"
#include "psa_manifest/tfm_fpu_service_test.h"
#include "tfm_api.h"
#include "tfm_hal_isolation.h"
#include "tfm_secure_api.h"
#include "tfm_memory_utils.h"
#include "tfm_sp_log.h"
#include "tfm_plat_test.h"
#include "device_definition.h"
#include "fpu_tests_common.h"

/* Define the return status */
#define FPU_SP_TEST_SUCCESS     (0)
#define FPU_SP_TEST_FAILED      (-1)

/**
 * Clear FP registers.
 */
__attribute__((naked)) static void clear_fp_regs(void)
{
    __asm volatile(
        "eor       r0, r0, r0              \n"
        "vmov      s0, r0                  \n"
        "vmov      s1, r0                  \n"
        "vmov      s2, r0                  \n"
        "vmov      s3, r0                  \n"
        "vmov      s4, r0                  \n"
        "vmov      s5, r0                  \n"
        "vmov      s6, r0                  \n"
        "vmov      s7, r0                  \n"
        "vmov      s8, r0                  \n"
        "vmov      s9, r0                  \n"
        "vmov      s10, r0                 \n"
        "vmov      s11, r0                 \n"
        "vmov      s12, r0                 \n"
        "vmov      s13, r0                 \n"
        "vmov      s14, r0                 \n"
        "vmov      s15, r0                 \n"
        "vmov      s16, r0                 \n"
        "vmov      s17, r0                 \n"
        "vmov      s18, r0                 \n"
        "vmov      s19, r0                 \n"
        "vmov      s20, r0                 \n"
        "vmov      s21, r0                 \n"
        "vmov      s22, r0                 \n"
        "vmov      s23, r0                 \n"
        "vmov      s24, r0                 \n"
        "vmov      s25, r0                 \n"
        "vmov      s26, r0                 \n"
        "vmov      s27, r0                 \n"
        "vmov      s28, r0                 \n"
        "vmov      s29, r0                 \n"
        "vmov      s30, r0                 \n"
        "vmov      s31, r0                 \n"
        "bx        lr                      \n"
    );
}

/**
 * Check whether FP registers are restored correctly.
 * Return:
 *   True - FP registers are restored correctly
 *   False - FP registers are not restored correctly
 */
bool check_fp_restored_service(void)
{
    uint32_t fp_buffer[NR_FP_REG] = {0};
    const uint32_t fp_expect[NR_FP_REG] = {
        0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
        0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
        0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
        0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
     };

    /* Dump FP data from FP registers to buffer */
    __asm volatile(
        "vstm      %0, {S0-S31}            \n"
        :
        :"r"(fp_buffer)
        :"memory"
    );

    if (!tfm_memcmp(fp_buffer, fp_expect, FP_BUF_SIZE)) {
        return true;
    }

    return false;
}

/**
 * Change FP registers in FP service partition.
 */
__attribute__((naked)) static void change_fp_in_service(void)
{
    __asm volatile(
        "mov       r0, #0x000000E0         \n"
        "vmov      s0, r0                  \n"
        "mov       r0, #0x000000E1         \n"
        "vmov      s1, r0                  \n"
        "mov       r0, #0x000000E2         \n"
        "vmov      s2, r0                  \n"
        "mov       r0, #0x000000E3         \n"
        "vmov      s3, r0                  \n"
        "mov       r0, #0x000000E4         \n"
        "vmov      s4, r0                  \n"
        "mov       r0, #0x000000E5         \n"
        "vmov      s5, r0                  \n"
        "mov       r0, #0x000000E6         \n"
        "vmov      s6, r0                  \n"
        "mov       r0, #0x000000E7         \n"
        "vmov      s7, r0                  \n"
        "mov       r0, #0x000000E8         \n"
        "vmov      s8, r0                  \n"
        "mov       r0, #0x000000E9         \n"
        "vmov      s9, r0                  \n"
        "mov       r0, #0x000000EA         \n"
        "vmov      s10, r0                 \n"
        "mov       r0, #0x000000EB         \n"
        "vmov      s11, r0                 \n"
        "mov       r0, #0x000000EC         \n"
        "vmov      s12, r0                 \n"
        "mov       r0, #0x000000ED         \n"
        "vmov      s13, r0                 \n"
        "mov       r0, #0x000000EE         \n"
        "vmov      s14, r0                 \n"
        "mov       r0, #0x000000EF         \n"
        "vmov      s15, r0                 \n"
        "mov       r0, #0x000000F0         \n"
        "vmov      s16, r0                 \n"
        "mov       r0, #0x000000F1         \n"
        "vmov      s17, r0                 \n"
        "mov       r0, #0x000000F2         \n"
        "vmov      s18, r0                 \n"
        "mov       r0, #0x000000F3         \n"
        "vmov      s19, r0                 \n"
        "mov       r0, #0x000000F4         \n"
        "vmov      s20, r0                 \n"
        "mov       r0, #0x000000F5         \n"
        "vmov      s21, r0                 \n"
        "mov       r0, #0x000000F6         \n"
        "vmov      s22, r0                 \n"
        "mov       r0, #0x000000F7         \n"
        "vmov      s23, r0                 \n"
        "mov       r0, #0x000000F8         \n"
        "vmov      s24, r0                 \n"
        "mov       r0, #0x000000F9         \n"
        "vmov      s25, r0                 \n"
        "mov       r0, #0x000000FA         \n"
        "vmov      s26, r0                 \n"
        "mov       r0, #0x000000FB         \n"
        "vmov      s27, r0                 \n"
        "mov       r0, #0x000000FC         \n"
        "vmov      s28, r0                 \n"
        "mov       r0, #0x000000FD         \n"
        "vmov      s29, r0                 \n"
        "mov       r0, #0x000000FE         \n"
        "vmov      s30, r0                 \n"
        "mov       r0, #0x000000FF         \n"
        "vmov      s31, r0                 \n"

        "bx        lr                      \n"
    );
}

/**
 * Check whether FP registers is invalidated.
 */
bool is_fp_cleaned(void)
{
    uint32_t fp_buffer[NR_FP_REG] = {0};
    uint32_t i;

    /* Dump FP data from FP registers to buffer */
    __asm volatile(
        "vstm      %0, {S0-S31}            \n"
        :
        :"r"(fp_buffer)
        :"memory"
    );

    for (i = 0; i < NR_FP_REG; i++) {
        if (fp_buffer[i] != 0) {
            return false;
        }
    }

    return true;
}

/**
 * Change FP registers in secure thread.
 */
__attribute__((naked)) void change_fp_in_s_thread(void)
{
    __asm volatile(
        "push      {r4, lr}                    \n"

        "mov       r0, #0xB0000000         \n"
        "vmov      s0, r0                  \n"
        "mov       r0, #0xB1000000         \n"
        "vmov      s1, r0                  \n"
        "mov       r0, #0xB2000000         \n"
        "vmov      s2, r0                  \n"
        "mov       r0, #0xB3000000         \n"
        "vmov      s3, r0                  \n"
        "mov       r0, #0xB4000000         \n"
        "vmov      s4, r0                  \n"
        "mov       r0, #0xB5000000         \n"
        "vmov      s5, r0                  \n"
        "mov       r0, #0xB6000000         \n"
        "vmov      s6, r0                  \n"
        "mov       r0, #0xB7000000         \n"
        "vmov      s7, r0                  \n"
        "mov       r0, #0xB8000000         \n"
        "vmov      s8, r0                  \n"
        "mov       r0, #0xB9000000         \n"
        "vmov      s9, r0                  \n"
        "mov       r0, #0xBA000000         \n"
        "vmov      s10, r0                 \n"
        "mov       r0, #0xBB000000         \n"
        "vmov      s11, r0                 \n"
        "mov       r0, #0xBC000000         \n"
        "vmov      s12, r0                 \n"
        "mov       r0, #0xBD000000         \n"
        "vmov      s13, r0                 \n"
        "mov       r0, #0xBE000000         \n"
        "vmov      s14, r0                 \n"
        "mov       r0, #0xBF000000         \n"
        "vmov      s15, r0                 \n"
        "mov       r0, #0xC0000000         \n"
        "vmov      s16, r0                 \n"
        "mov       r0, #0xC1000000         \n"
        "vmov      s17, r0                 \n"
        "mov       r0, #0xC2000000         \n"
        "vmov      s18, r0                 \n"
        "mov       r0, #0xC3000000         \n"
        "vmov      s19, r0                 \n"
        "mov       r0, #0xC4000000         \n"
        "vmov      s20, r0                 \n"
        "mov       r0, #0xC5000000         \n"
        "vmov      s21, r0                 \n"
        "mov       r0, #0xC6000000         \n"
        "vmov      s22, r0                 \n"
        "mov       r0, #0xC7000000         \n"
        "vmov      s23, r0                 \n"
        "mov       r0, #0xC8000000         \n"
        "vmov      s24, r0                 \n"
        "mov       r0, #0xC9000000         \n"
        "vmov      s25, r0                 \n"
        "mov       r0, #0xCA000000         \n"
        "vmov      s26, r0                 \n"
        "mov       r0, #0xCB000000         \n"
        "vmov      s27, r0                 \n"
        "mov       r0, #0xCC000000         \n"
        "vmov      s28, r0                 \n"
        "mov       r0, #0xCD000000         \n"
        "vmov      s29, r0                 \n"
        "mov       r0, #0xCE000000         \n"
        "vmov      s30, r0                 \n"
        "mov       r0, #0xCF000000         \n"
        "vmov      s31, r0                 \n"

        "pop       {r4, pc}                \n"
    );
}

/**
 * Check whether FP registers are restored correctly.
 * Return:
 *   True - FP registers are restored correctly
 *   False - FP registers are not restored correctly
 */
bool check_fp_restored_s(void)
{
    uint32_t fp_buffer[NR_FP_REG] = {0};
    const uint32_t fp_expect[NR_FP_REG] = {
        0xB0000000, 0xB1000000, 0xB2000000, 0xB3000000,
        0xB4000000, 0xB5000000, 0xB6000000, 0xB7000000,
        0xB8000000, 0xB9000000, 0xBA000000, 0xBB000000,
        0xBC000000, 0xBD000000, 0xBE000000, 0xBF000000,
        0xC0000000, 0xC1000000, 0xC2000000, 0xC3000000,
        0xC4000000, 0xC5000000, 0xC6000000, 0xC7000000,
        0xC8000000, 0xC9000000, 0xCA000000, 0xCB000000,
        0xCC000000, 0xCD000000, 0xCE000000, 0xCF000000
    };

    /* Dump FP data from FP registers to buffer */
    __asm volatile(
        "vstm      %0, {S0-S31}            \n"
        :
        :"r"(fp_buffer)
        :"memory"
    );

    if (!tfm_memcmp(fp_buffer, fp_expect, FP_BUF_SIZE)) {
        return true;
    }

    return false;
}

/**
 * Service handler for clear FP register.
 */
static void fpu_service_clear_fp_register(void)
{
    psa_msg_t msg;
    psa_status_t r;

    psa_get(TFM_FPU_SERVICE_CLEAR_FP_REGISTER_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        if (check_fp_restored_service()) {
            clear_fp_regs();
            r = PSA_SUCCESS;
        } else {
            r = PSA_ERROR_INVALID_ARGUMENT;
        }
        psa_reply(msg.handle, r);
        break;
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        psa_panic();
        break;
    }
}

/**
 * Service handler for checking FP register.
 */
static void fpu_service_check_fp_register(void)
{
    psa_msg_t msg;

    psa_get(TFM_FPU_SERVICE_CHECK_FP_REGISTER_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        if (is_fp_cleaned()) {
            change_fp_in_service();
            psa_reply(msg.handle, PSA_SUCCESS);
        } else {
            psa_reply(msg.handle, PSA_ERROR_GENERIC_ERROR);
        }
        break;
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        psa_panic();
        break;
    }
}

/**
 * Start S timer interrupt.
 * Expectation: S timer should be started.
 */
static void fpu_client_start_secure_timer(void)
{
    psa_msg_t msg;
    psa_status_t r;

    r = psa_get(TFM_FPU_SERVICE_START_S_TIMER_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        /* Start the timer */
        tfm_plat_test_secure_timer_start();
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        psa_panic();
        break;
    }
}

/**
 * Read S timer reload value.
 * Expectation: S timer reload value should be read.
 */
static void fpu_client_check_secure_timer_triggered(void)
{
    psa_msg_t msg;
    psa_status_t r;
    int val;

    r = psa_get(TFM_FPU_SERVICE_CHECK_S_TIMER_TRIGGERED_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        if (msg.out_size[0] != 0) {
            /* Read the timer reload value */
            if (tfm_plat_test_secure_timer_get_reload_value()
                            == REL_VALUE_FP_REGS_INVALIDATED) {
                val = S_TIMER_TRIGGERED;
            } else {
                val = S_TIMER_NOT_TRIGGERED;
            }
            psa_write(msg.handle, 0, &val, 1);
            r = PSA_SUCCESS;
        } else {
            r = PSA_ERROR_PROGRAMMER_ERROR;
        }
        psa_reply(msg.handle, r);
        break;
    default:
        psa_panic();
        break;
    }
}

/**
 * Test FP context protection after NS interrupt.
 * Expectation: FP register in secure thread should be restored after NS
 * interrupt.
 */
int fpu_client_non_secure_interrupt_secure_test(void)
{
    psa_msg_t msg;
    psa_status_t r;
    static uint32_t i;

    r = psa_get(TFM_FPU_SERVICE_CHECK_NS_INTERRUPT_S_TEST_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        /* Change FP regs */
        change_fp_in_s_thread();
        /* Start the timer */
        tfm_plat_test_non_secure_timer_start();
        LOG_DBGFMT("Wait for NS timer interrupt!\r\n");
        /* Spin here */
        while (1) {
            /* NS interrupt triggered */
            if (tfm_plat_test_non_secure_timer_get_reload_value()
                                == REL_VALUE_FP_REGS_INVALIDATED) {
                LOG_DBGFMT("S thread interrupted by NS timer interrupt!\r\n");
                break;
            } else {
                i++;
                if (i > LOOPS_NS_INT_TEST) {
                    LOG_DBGFMT("Time out: S thread is not interrupted!\r\n");
                    break;
                }
            }
        }
        if (i > LOOPS_NS_INT_TEST) {
            /* Error for time out */
            r = PSA_ERROR_GENERIC_ERROR;
        } else {
            /* FP register should be restored after NS interrupt. */
            if (check_fp_restored_s()) {
                r = PSA_SUCCESS;
            } else {
                r = PSA_ERROR_GENERIC_ERROR;
            }
        }
        /* Reply with status */
        psa_reply(msg.handle, r);
        break;
    default:
        psa_panic();
        break;
    }
}

/**
 * FP service partition main thread.
 */
void fpu_service_test_main(void *param)
{
    uint32_t signals = 0;

    clear_fp_regs();
    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & TFM_FPU_SERVICE_CLEAR_FP_REGISTER_SIGNAL) {
            fpu_service_clear_fp_register();
        } else if (signals & TFM_FPU_SERVICE_CHECK_FP_REGISTER_SIGNAL) {
            fpu_service_check_fp_register();
        } else if (signals & TFM_FPU_SERVICE_START_S_TIMER_SIGNAL) {
            fpu_client_start_secure_timer();
        } else if (signals & TFM_FPU_SERVICE_CHECK_S_TIMER_TRIGGERED_SIGNAL) {
            fpu_client_check_secure_timer_triggered();
        } else if (signals & TFM_FPU_SERVICE_CHECK_NS_INTERRUPT_S_TEST_SIGNAL) {
            fpu_client_non_secure_interrupt_secure_test();
        } else {
            psa_panic();
        }
    }
}
