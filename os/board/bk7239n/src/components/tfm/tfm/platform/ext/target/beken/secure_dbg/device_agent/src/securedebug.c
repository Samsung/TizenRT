/**
* Copyright (C), 2018-2023, Arm Technology (China) Co., Ltd.
* All rights reserved
*
* The content of this file or document is CONFIDENTIAL and PROPRIETARY
* to Arm Technology (China) Co., Ltd. It is subject to the terms of a
* License Agreement between Licensee and Arm Technology (China) Co., Ltd
* restricting among other things, the use, reproduction, distribution
* and transfer.  Each of the embodiments, including this information and,,
* any derivative work shall retain this copyright notice.
*/
#include "securedebug_internal.h"

sec_debug_buff_alloc_ctx_t g_sec_dbg_ctx = {0};

int arm_ce_otp_driver_init(void);

hal_ret_t sec_debug_init(sec_debug_ctx_t *sec_debug_ctx, void *args)
{
    hal_ret_t ret = HAL_ERR_GENERIC;
    if (g_sec_dbg_ctx.init_flag) {
        PAL_LOG_ERR("Secure debug already initalized!\n");
        return HAL_ERR_GENERIC;
    }

    ret = arm_ce_otp_driver_init();
    if (HAL_OK != ret) {
        PAL_LOG_ERR("hal_otp_init failed!\n");
        return ret;
    }

    ret = hal_channel_init(&(g_sec_dbg_ctx.channel), args);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("hal_channel_init failed!\n");
        return ret;
    }

    g_sec_dbg_ctx.init_flag = true;
    *sec_debug_ctx          = (sec_debug_ctx_t *)(&g_sec_dbg_ctx);

    return HAL_OK;
}

hal_ret_t sec_debug_cleanup(sec_debug_ctx_t sec_debug_ctx)
{
    if (sec_debug_ctx != (sec_debug_ctx_t *)(&g_sec_dbg_ctx)) {
        PAL_LOG_ERR("Bad secure debug context!\n");
        return HAL_ERR_BAD_PARAM;
    }
    hal_channel_cleanup(g_sec_dbg_ctx.channel);

    pal_memset(&g_sec_dbg_ctx, 0, sizeof(sec_debug_buff_alloc_ctx_t));

    return HAL_OK;
}

hal_ret_t sec_debug_enable(sec_debug_ctx_t sec_debug_ctx)
{
    if (sec_debug_ctx != (sec_debug_ctx_t *)(&g_sec_dbg_ctx)) {
        PAL_LOG_ERR("Bad secure debug context!\n");
        return HAL_ERR_BAD_PARAM;
    }

    return sec_debug_protocol_process();
}
// eof

