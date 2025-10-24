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

#include "mbedtls/ecdsa.h"
#include "mbedtls/asn1write.h"
#include "mbedtls/sha256.h"
//#include "mbedtls/compat-2.x.h"

#include <string.h>

// #if defined(MBEDTLS_ECDSA_DETERMINISTIC)
#include "mbedtls/hmac_drbg.h"
// #endif

// #if defined(MBEDTLS_PLATFORM_C)
// #include "mbedtls/platform.h"
// #else
// #include <stdlib.h>
// #define mbedtls_calloc    calloc
// #define mbedtls_free       free
// #endif

#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_printf printf
#define mbedtls_calloc    calloc
#define mbedtls_free       free
#endif /* MBEDTLS_PLATFORM_C */

static const uint8_t Vector_P384_Message[] =
{
    0xAB, 0xE1, 0x0A, 0xCE, 0x13, 0xE7, 0xE1, 0xD9, 0x18, 0x6C, 0x48, 0xF7, 0x88, 0x9D, 0x51, 0x47,
    0x3D, 0x3A, 0x09, 0x61, 0x98, 0x4B, 0xC8, 0x72, 0xDF, 0x70, 0x8E, 0xCC, 0x3E, 0xD3, 0xB8, 0x16,
    0x9D, 0x01, 0xE3, 0xD9, 0x6F, 0xC4, 0xF1, 0xD5, 0xEA, 0x00, 0xA0, 0x36, 0x92, 0xBC, 0xC5, 0xCF,
    0xFD, 0x53, 0x78, 0x7C, 0x88, 0xB9, 0x34, 0xAF, 0x40, 0x4C, 0x03, 0x9D, 0x32, 0x89, 0xB5, 0xBA,
    0xC5, 0xAE, 0x7D, 0xB1, 0x49, 0x68, 0x75, 0xB5, 0xDC, 0x73, 0xC3, 0x09, 0xF9, 0x25, 0xC1, 0x3D,
    0x1C, 0x01, 0xAB, 0xDA, 0xAF, 0xEB, 0xCD, 0xAC, 0x2C, 0xEE, 0x43, 0x39, 0x39, 0xCE, 0x8D, 0x4A,
    0x0A, 0x5D, 0x57, 0xBB, 0x70, 0x5F, 0x3B, 0xF6, 0xEC, 0x08, 0x47, 0x95, 0x11, 0xD4, 0xB4, 0xA3,
    0x21, 0x1F, 0x61, 0x64, 0x9A, 0xD6, 0x27, 0x43, 0x14, 0xBF, 0x0D, 0x43, 0x8A, 0x81, 0xE0, 0x60
};

static int puf_gen_rand( void *rng_state, unsigned char *output, size_t len )
{
    if( rng_state != NULL )
        rng_state  = NULL;

    uint32_t rand_num = 0;
    for (int i = 0; i < len; i++) {
        if ((i % 4) == 0) {
            rand_num = REG_READ(SOC_OTP_APB_BASE + 0x02a0);
        }
        output[i] = (rand_num >> (8 * (i % 4))) & 0xff;
    }

    return 0;
}

int te200_ecdsa_self_test( int verbose ,uint32_t loop)
{
    int ret = 1;
    mbedtls_ecdsa_context ctx_sign, ctx_verify;
    unsigned char sig[MBEDTLS_ECDSA_MAX_LEN];
    size_t sig_len;
    uint32_t i;
    uint64_t before, after;

    //extern int mbedtls_sha256_ret( const unsigned char *input,size_t ilen,unsigned char output[32],int is224 );
    uint8_t digest[32] = {0};
    memset(&digest[0], 0, sizeof(digest));
    ret = mbedtls_sha256(Vector_P384_Message, sizeof(Vector_P384_Message), &digest[0], 0);
    if (ret != 0)
    {
        mbedtls_printf( " failed\n  ! mbedtls_sha256_ret returned %d\r\n", ret );
        goto exit;
    }

    mbedtls_ecdsa_init( &ctx_sign );
    mbedtls_ecdsa_init( &ctx_verify );

    memset( sig, 0, sizeof( sig ) );
    if(loop < 1)
        loop = 1;

    /*
     * Generate a key pair for signing
     */
    if( verbose != 0 )
        mbedtls_printf( " 1. Generating key pair...\r\n  ." );

    if( ( ret = mbedtls_ecdsa_genkey( &ctx_sign, MBEDTLS_ECP_DP_SECP256R1, puf_gen_rand, NULL ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ecdsa_genkey returned %d\r\n", ret );
        goto exit;
    }
    // if( verbose != 0 )
    //     dump_pubkey( "  + Public key: ", &ctx_sign );

    /*
     * Sign message hash
     */
    if( verbose != 0 )
        mbedtls_printf( "2. Signing message...\r\n" );

    before = bk_aon_rtc_get_us();
    GPIO_UP(0);
    for(i = 0; i < loop; i++)
    {
        if( ( ret = mbedtls_ecdsa_write_signature( &ctx_sign, MBEDTLS_MD_SHA256,
                                            &digest[0], sizeof( digest ),
                                            sig, sizeof(sig), &sig_len,
                                            puf_gen_rand, NULL ) ) != 0 )
        {
            mbedtls_printf( " failed\n  ! mbedtls_ecdsa_write_signature returned %d\r\n", ret );
            goto exit;
        }
    }
    GPIO_DOWN(0);
    after = bk_aon_rtc_get_us();
    mbedtls_printf("signature run %d times, take time %d us.\r\n", loop, after - before);
    // if( verbose != 0 )
    //     dump_buf( "  + Signature: ", sig, sig_len );

    /*
     * Transfer public information to verifying context
     *
     * We could use the same context for verification and signatures, but we
     * chose to use a new one in order to make it clear that the verifying
     * context only needs the public key (Q), and not the private key (d).
     */
    if( verbose != 0 )
        mbedtls_printf( "3. Preparing verification context...\r\n" );

    if( ( ret = mbedtls_ecp_group_copy( &ctx_verify.grp, &ctx_sign.grp ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ecp_group_copy returned %d\r\n", ret );
        goto exit;
    }

    if( ( ret = mbedtls_ecp_copy( &ctx_verify.Q, &ctx_sign.Q ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ecp_copy returned %d\r\n", ret );
        goto exit;
    }

    /*
     * Verify signature
     */
    if( verbose != 0 )
        mbedtls_printf( "4. Verifying signature...\r\n" );

    before = bk_aon_rtc_get_us();
    GPIO_UP(1);
    for(i = 0; i < loop; i++)
    {
        if( ( ret = mbedtls_ecdsa_read_signature( &ctx_verify,
                                           &digest[0], sizeof(digest),
                                          sig, sig_len ) ) != 0 )
        {
            mbedtls_printf( " failed\n  ! mbedtls_ecdsa_read_signature returned %d\r\n", ret );
            goto exit;
        }
    }
    GPIO_DOWN(1);
    after = bk_aon_rtc_get_us();
    mbedtls_printf("verify run %d times, take time %d us.\r\n", loop, after - before);
    if( verbose != 0 )
        mbedtls_printf( " ecdsa self test suc\r\n" );

exit:

    mbedtls_ecdsa_free( &ctx_verify );
    mbedtls_ecdsa_free( &ctx_sign );

    return 0;
}