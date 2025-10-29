/**
 * @if copyright_display
 *              Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 *              All rights reserved
 *
 *          The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *              to Arm Technology (China) Co., Ltd. It is subject to the terms
 * of a
 *              License Agreement between Licensee and Arm Technology (China)
 * Co., Ltd
 *              restricting among other things, the use, reproduction,
 * distribution
 *              and transfer.  Each of the embodiments, including this
 * information and
 *              any derivative work shall retain this copyright notice.
 * @endif
 */
#include <stdint.h>
#include <stdio.h>
#include "secure_debug.h"
#include "secure_debug_app.h"
#include <components/log.h>

#define TAG "debug"

int secure_debug_agent_entry(void)
{
    sec_debug_stat_t result = HAL_OK;
    sec_debug_ctx_t sec_debug_ctx;

    BK_LOGI(TAG, "please open secure debug tool to start secure debug enable \n");
	/*TODO wangzhilei, we only support uart0, args format: UART0-Sleep_Time*/
    result = sec_debug_init(&sec_debug_ctx, "UART0-20000");
    if (SEC_DEBUG_STAT_OK != result) {
        BK_LOGI(TAG, "\n===sec_debug_init fail:0x%x\n", result);
        goto exit;
    }

    result = sec_debug_enable(sec_debug_ctx);
    if (SEC_DEBUG_STAT_OK != result) {
        BK_LOGI(TAG, "\n===sec_debug_enable fail:0x%x\n", result);
        goto exit;
    }

exit:
    sec_debug_cleanup(sec_debug_ctx);

    return result;
}
// eof

