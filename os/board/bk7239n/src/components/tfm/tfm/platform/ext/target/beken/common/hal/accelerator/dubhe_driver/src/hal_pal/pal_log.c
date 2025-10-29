/**
 * Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#include "pal_common.h"
#include <stdarg.h>

#include "pal_log.h"
#include "psa_sp_api.h"

static int32_t _g_pal_log_level = PAL_LOG_LEVEL_DATA;

void pal_log_set_level( int32_t level )
{
    _g_pal_log_level = level;
}

int32_t pal_log_get_level( void )
{
    return _g_pal_log_level;
}

int32_t pal_platform_printf( const char *fmt, ... )
{
    va_list vargs;
    int r;

    va_start( vargs, fmt );
    r = psa_vprintf( fmt, vargs );
    va_end( vargs );

    return r;
}

/*************************** The End Of File*****************************/
