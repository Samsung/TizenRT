/**
* @if copyright_display
 *      Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 *      All rights reserved
 *
 *      The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *      to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *      License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *      restricting among other things, the use, reproduction, distribution
 *      and transfer.  Each of the embodiments, including this information and
 *      any derivative work shall retain this copyright notice.
* @endif
 */

#if !defined(MBEDTLS_CONFIG_FILE)
//#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(ARM_CE_DUBHE_OTP)

#include "mbedtls/otp.h"
#include "dubhe_otp.h"

#include <string.h>

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */

int mbedtls_get_otp_info( mbedtls_otp_info_t type, uint32_t *output, size_t size, uint32_t offset )
{
    return( arm_ce_get_otp_info( (arm_ce_otp_info_t)type, output, size, offset*4 ) );
}

int mbedtls_get_otp_info_ex( mbedtls_otp_info_t type, void *output, size_t size, size_t offset )
{
    return( arm_ce_get_otp_info( (arm_ce_otp_info_t)type, output, size, offset ) );
}

int mbedtls_set_otp_info( mbedtls_otp_info_t type, const uint32_t *input, size_t ilen, uint32_t offset )
{
    return arm_ce_set_otp_info( (arm_ce_otp_info_t)type, input, ilen, offset*4 );
}

int mbedtls_set_otp_info_ex( mbedtls_otp_info_t type, const void *input, size_t ilen, size_t offset )
{
    return arm_ce_set_otp_info( (arm_ce_otp_info_t)type, input, ilen, offset );
}

#if defined(DBH_OTP_PUF_SUPPORT)

bool mbedtls_otp_puf_ready(void)
{
    return arm_ce_otp_puf_ready();
}

int mbedtls_otp_puf_deep_standby(void)
{
    return arm_ce_otp_deep_standby();
}

int mbedtls_otp_puf_wakeup(void)
{
    return arm_ce_otp_wakeup();
}

int mbedtls_otp_puf_initial_margin_read( mbedtls_otp_info_t area, void *output, size_t *size, size_t offset )
{
    return arm_ce_otp_puf_initial_margin_read( (arm_ce_otp_info_t)area, output, size, offset );
}

int mbedtls_otp_puf_pgm_margin_read( mbedtls_otp_info_t area, void *output, size_t *size, size_t offset )
{
    return arm_ce_otp_puf_pgm_margin_read( (arm_ce_otp_info_t)area, output, size, offset );
}

int mbedtls_otp_puf_enrollment(void)
{
    return arm_ce_enroll_puf();
}

int mbedtls_otp_puf_quality_check(void)
{
    return arm_ce_do_puf_quality_check();
}
#endif /* DBH_OTP_PUF_SUPPORT */
#endif /* ARM_CE_DUBHE_OTP */
