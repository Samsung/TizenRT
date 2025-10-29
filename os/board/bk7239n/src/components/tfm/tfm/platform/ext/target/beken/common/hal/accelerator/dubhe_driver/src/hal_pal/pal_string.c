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
#include "string.h"
#include "pal_string.h"

/**
 * @ingroup	sec_test
 * @brief	safe_memcmp The normal memcmp function has side-channel attack
 * venture,
 *           better implement safe_memcmp function.
 * Details see:
 * https://security.stackexchange.com/questions/160808/why-should-memcmp-not-be-used
 * -to-compare-security-critical-data?utm_medium=organic&utm_source=google_rich_qa&
 * utm_campaign=google_rich_qa
 *
 * @return	return compare result, 0 means the same others not
 */

static int safe_memcmp( const void *in_a, const void *in_b, size_t len )
{
    size_t i;
    const volatile unsigned char *a = in_a;
    const volatile unsigned char *b = in_b;
    unsigned char x                 = 0;

    for ( i = 0; i < len; i++ ) {
        x |= a[i] ^ b[i];
    }

    return x;
}

int32_t pal_safe_memcmp( const void *m1, const void *m2, size_t n )
{
    return safe_memcmp( m1, m2, n );
}

int32_t pal_memcmp( const void *m1, const void *m2, size_t n )
{
    return safe_memcmp( m1, m2, n );
}

void *pal_memcpy( void *d, const void *s, size_t n )
{
    return memcpy( d, s, n );
}

void *pal_memset( void *buf, int32_t c, size_t n )
{
    return memset( buf, c, n );
}

void *pal_memmove( void *d, const void *s, size_t n )
{
    return memmove( d, s, n );
}

/*************************** The End Of File*****************************/
