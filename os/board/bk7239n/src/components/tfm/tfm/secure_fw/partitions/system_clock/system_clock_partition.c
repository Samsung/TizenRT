/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "psa/service.h"
#include "psa_manifest/tfm_system_clock.h"
#include "sys_driver.h"
#include "driver/prro.h"
#include "tfm_system_clock_api.h"

typedef psa_status_t (*sys_func_t)(const psa_msg_t *msg);

static psa_status_t system_clock_power_ctrl_ipc(const psa_msg_t *msg)
{
    uint32_t module_id = 0;
    uint32_t power_state = 0;
    int num = 0;
    psa_status_t ret = 0;

    num = psa_read(msg->handle, 0, &module_id, sizeof(module_id));
    if (num != sizeof(module_id)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 1, &power_state, sizeof(power_state));
    if (num != sizeof(power_state)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    printf("tfm:power_ctrl, module_id:%d, power_state:%d\r\n", module_id, power_state);
    bk_prro_driver_init();
#if CONFIG_TFM_BK7236_V5
    ret = bk_prro_set_secure(PRRO_DEV_SYS, PRRO_SECURE);
#else
    ret = bk_prro_set_secure(PRRO_DEV_AHB_SYSTEM, PRRO_SECURE);
#endif
    sys_drv_dev_clk_pwr_up(module_id, power_state);
#if CONFIG_TFM_BK7236_V5
    ret = bk_prro_set_secure(PRRO_DEV_SYS, PRRO_NON_SECURE);
#else
    bk_prro_set_secure(PRRO_DEV_AHB_SYSTEM, PRRO_NON_SECURE);
#endif

    return ret;
}

static psa_status_t system_clock_set_ipc(const psa_msg_t *msg)
{
    uint32_t module_id = 0;
    uint32_t clk_src = 0;
    uint32_t clk_div = 0;
    int num = 0;
    psa_status_t ret = 0;

    num = psa_read(msg->handle, 0, &module_id, sizeof(module_id));
    if (num != sizeof(module_id)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 1, &clk_src, sizeof(clk_src));
    if (num != sizeof(clk_src)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 2, &clk_div, sizeof(clk_div));
    if (num != sizeof(clk_div)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    printf("tfm:clock_set, clk_src:%d, clk_div:%d\r\n", clk_src, clk_div);

    bk_prro_driver_init();
#if CONFIG_TFM_BK7236_V5
    ret = bk_prro_set_secure(PRRO_DEV_SYS, PRRO_SECURE);
#else
    bk_prro_set_secure(PRRO_DEV_AHB_SYSTEM, PRRO_SECURE);
#endif
    sys_drv_set_clk_select(module_id, clk_src);
    sys_drv_set_clk_div(module_id, clk_div);
#if CONFIG_TFM_BK7236_V5
    ret = bk_prro_set_secure(PRRO_DEV_SYS, PRRO_NON_SECURE);
#else
    bk_prro_set_secure(PRRO_DEV_AHB_SYSTEM, PRRO_NON_SECURE);
#endif

    return ret;
}

static psa_status_t system_core_set_ipc(const psa_msg_t *msg)
{
    uint32_t system_operation_id = 0;
    int num = 0;
    psa_status_t ret = 0;

    num = psa_read(msg->handle, 0, &system_operation_id, sizeof(system_operation_id));
    if (num != sizeof(system_operation_id)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    printf("tfm:system_operation_id:%d\r\n", system_operation_id);

    bk_prro_driver_init();

#if CONFIG_TFM_BK7236_V5
    bk_prro_set_secure(PRRO_DEV_SYS, PRRO_SECURE);
#else
    bk_prro_set_secure(PRRO_DEV_AHB_SYSTEM, PRRO_SECURE);
#endif
    switch (system_operation_id) {
    case TFM_SYSTEM_JTAG_CONNECT_CORE0:
        sys_drv_set_jtag_mode(0);
        break;
    case TFM_SYSTEM_JTAG_CONNECT_CORE1:
        sys_drv_set_jtag_mode(1);
        break;
    default:
        break;
    }
#if CONFIG_TFM_BK7236_V5
    bk_prro_set_secure(PRRO_DEV_SYS, PRRO_NON_SECURE);
#else
    bk_prro_set_secure(PRRO_DEV_AHB_SYSTEM, PRRO_NON_SECURE);
#endif

    return ret;
}

static void system_clock_signal_handle(psa_signal_t signal, sys_func_t pfn)
{
    psa_msg_t msg = {0};
    psa_status_t status = PSA_SUCCESS;

    status = psa_get(signal, &msg);
    if (status != PSA_SUCCESS) {
        return;
    }

    switch (msg.type) {
    case PSA_IPC_CONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        status = (psa_status_t)pfn(&msg);
        psa_reply(msg.handle, status);
        break;
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        psa_panic();
        break;
    }
}

void tfm_system_clock_thread(void)
{
    psa_signal_t signals = 0;

    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & TFM_SYSTEM_POWER_CTRL_SIGNAL) {
            system_clock_signal_handle(TFM_SYSTEM_POWER_CTRL_SIGNAL, system_clock_power_ctrl_ipc);
        } else if (signals & TFM_SYSTEM_CLOCK_SET_SIGNAL) {
            system_clock_signal_handle(TFM_SYSTEM_CLOCK_SET_SIGNAL, system_clock_set_ipc);
        } else if (signals & TFM_SYSTEM_CORE_SET_SIGNAL) {
            system_clock_signal_handle(TFM_SYSTEM_CORE_SET_SIGNAL, system_core_set_ipc);
        } else {
            psa_panic();
        }
    }
}
