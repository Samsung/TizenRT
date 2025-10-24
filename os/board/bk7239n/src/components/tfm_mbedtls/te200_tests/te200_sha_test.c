// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "common.h"
#include <driver/aon_rtc.h>
#include <driver/trng.h>

#include "mbedtls/sha256.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#include <string.h>

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_printf printf
#define mbedtls_calloc    calloc
#define mbedtls_free       free
#endif /* MBEDTLS_PLATFORM_C */

int te200_sha256_test(uint8_t *encrypt_buf, uint32_t encrypt_len)
{
    int  ret = 0;
    // uint8_t *buf;
    unsigned char sha256sum[32];
    mbedtls_sha256_context ctx;
    // uint64_t start_tick, end_tick, tick_cnt = 0;

    // buf = mbedtls_calloc( encrypt_len, sizeof(unsigned char) );
    // if( NULL == buf ) {
    //     os_printf( "Buffer allocation failed\n" );
    //     return 1;
    // }

    // for (int i = 0; i < encrypt_len; i++) {
    //     buf[i] = (i & 0xff);
    // }

    // start_tick = bk_aon_rtc_get_us();
    // GPIO_UP(0);
    mbedtls_sha256_init( &ctx );

    if( ( ret = mbedtls_sha256_starts( &ctx, 1 ) ) != 0 )
        goto fail;

    ret = mbedtls_sha256_update( &ctx, encrypt_buf, encrypt_len );
    if( ret != 0 )
        goto fail;

    if( ( ret = mbedtls_sha256_finish( &ctx, sha256sum ) ) != 0 )
        goto fail;

    goto exit;

fail:
    mbedtls_printf( "failed\n" );

exit:
    mbedtls_sha256_free( &ctx );
    // GPIO_DOWN(0);
    // end_tick = bk_aon_rtc_get_us();
    // tick_cnt = end_tick - start_tick;
    // BK_DUMP_OUT("[SHA256]: encrypt_len %d bytes, cost time %d us.\r\n", encrypt_len, tick_cnt);
    // if (buf) {
    //     mbedtls_free(buf);
    //     buf = NULL;
    // }

    return( ret );
}

int te200_sha256_loop_test(uint32_t encrypt_len, uint32_t test_cnt)
{
    int ret = 0;
    uint8_t *buf;
    uint64_t start_tick, end_tick, tick_cnt = 0;

    buf = mbedtls_calloc( encrypt_len, sizeof(unsigned char) );
    if( NULL == buf ) {
        os_printf( "Buffer allocation failed\n" );
        return 1;
    }

    for (int i = 0; i < encrypt_len; i++) {
        buf[i] = (i & 0xff);
    }

    start_tick = bk_aon_rtc_get_us();
    GPIO_UP(0);
    for (int i = 0; i < test_cnt; i++) {
        ret = te200_sha256_test(buf, encrypt_len);
        if (ret != 0) {
            goto fail;
        }
    }
    GPIO_DOWN(0);
    end_tick = bk_aon_rtc_get_us();
    tick_cnt = end_tick - start_tick;
    BK_DUMP_OUT("[sha256]: run %d times, encrypt_len %d bytes, cost time %d us.\r\n", test_cnt, encrypt_len, tick_cnt);

    goto exit;

fail:
    mbedtls_printf( "failed\n" );

exit:
    if (buf) {
        mbedtls_free(buf);
        buf = NULL;
    }

    return ret;
}
