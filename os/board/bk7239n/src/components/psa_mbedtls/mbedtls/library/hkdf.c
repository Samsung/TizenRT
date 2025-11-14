/*
 *  HKDF implementation -- RFC 5869
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */
#include "common.h"

#if defined(MBEDTLS_HKDF_C)

#include <string.h>
#include "mbedtls/hkdf.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

int mbedtls_hkdf(const mbedtls_md_info_t *md, const unsigned char *salt,
                 size_t salt_len, const unsigned char *ikm, size_t ikm_len,
                 const unsigned char *info, size_t info_len,
                 unsigned char *okm, size_t okm_len)
{

#if defined(CONFIG_TFM_HKDF_NSC)
#include "tfm_hkdf_nsc.h"
#include "tfm_ns_interface.h"
	int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
	mbedtls_md_type_t type = mbedtls_md_get_type( md );

	hkdf_input_t hkdf_in   = {0};
	hkdf_output_t hkdf_out = {0};

	hkdf_in.salt     = salt;
	hkdf_in.salt_len = salt_len;
	hkdf_in.ikm      = ikm;
	hkdf_in.ikm_len  = ikm_len;
	hkdf_in.info     = info;
	hkdf_in.info_len = info_len;
	hkdf_out.okm     = okm;
	hkdf_out.okm_len = okm_len;

	tfm_ns_interface_lock();
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

	ret = psa_mbedtls_hkdf_nsc( (uint8_t)type, &hkdf_in, &hkdf_out );

	GLOBAL_INT_RESTORE();
	tfm_ns_interface_unlock();

	return ret;
#else
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char prk[MBEDTLS_MD_MAX_SIZE];

    ret = mbedtls_hkdf_extract(md, salt, salt_len, ikm, ikm_len, prk);

    if (ret == 0) {
        ret = mbedtls_hkdf_expand(md, prk, mbedtls_md_get_size(md),
                                  info, info_len, okm, okm_len);
    }

    mbedtls_platform_zeroize(prk, sizeof(prk));

    return ret;
#endif
}

int mbedtls_hkdf_extract(const mbedtls_md_info_t *md,
                         const unsigned char *salt, size_t salt_len,
                         const unsigned char *ikm, size_t ikm_len,
                         unsigned char *prk)
{
    unsigned char null_salt[MBEDTLS_MD_MAX_SIZE] = { '\0' };

    if (salt == NULL) {
        size_t hash_len;

        if (salt_len != 0) {
            return MBEDTLS_ERR_HKDF_BAD_INPUT_DATA;
        }

        hash_len = mbedtls_md_get_size(md);

        if (hash_len == 0) {
            return MBEDTLS_ERR_HKDF_BAD_INPUT_DATA;
        }

        salt = null_salt;
        salt_len = hash_len;
    }

    return mbedtls_md_hmac(md, salt, salt_len, ikm, ikm_len, prk);
}

int mbedtls_hkdf_expand(const mbedtls_md_info_t *md, const unsigned char *prk,
                        size_t prk_len, const unsigned char *info,
                        size_t info_len, unsigned char *okm, size_t okm_len)
{
    size_t hash_len;
    size_t where = 0;
    size_t n;
    size_t t_len = 0;
    size_t i;
    int ret = 0;
    mbedtls_md_context_t ctx;
    unsigned char t[MBEDTLS_MD_MAX_SIZE];

    if (okm == NULL) {
        return MBEDTLS_ERR_HKDF_BAD_INPUT_DATA;
    }

    hash_len = mbedtls_md_get_size(md);

    if (prk_len < hash_len || hash_len == 0) {
        return MBEDTLS_ERR_HKDF_BAD_INPUT_DATA;
    }

    if (info == NULL) {
        info = (const unsigned char *) "";
        info_len = 0;
    }

    n = okm_len / hash_len;

    if (okm_len % hash_len != 0) {
        n++;
    }

    /*
     * Per RFC 5869 Section 2.3, okm_len must not exceed
     * 255 times the hash length
     */
    if (n > 255) {
        return MBEDTLS_ERR_HKDF_BAD_INPUT_DATA;
    }

    mbedtls_md_init(&ctx);

    if ((ret = mbedtls_md_setup(&ctx, md, 1)) != 0) {
        goto exit;
    }

    memset(t, 0, hash_len);

    /*
     * Compute T = T(1) | T(2) | T(3) | ... | T(N)
     * Where T(N) is defined in RFC 5869 Section 2.3
     */
    for (i = 1; i <= n; i++) {
        size_t num_to_copy;
        unsigned char c = i & 0xff;

        ret = mbedtls_md_hmac_starts(&ctx, prk, prk_len);
        if (ret != 0) {
            goto exit;
        }

        ret = mbedtls_md_hmac_update(&ctx, t, t_len);
        if (ret != 0) {
            goto exit;
        }

        ret = mbedtls_md_hmac_update(&ctx, info, info_len);
        if (ret != 0) {
            goto exit;
        }

        /* The constant concatenated to the end of each T(n) is a single octet.
         * */
        ret = mbedtls_md_hmac_update(&ctx, &c, 1);
        if (ret != 0) {
            goto exit;
        }

        ret = mbedtls_md_hmac_finish(&ctx, t);
        if (ret != 0) {
            goto exit;
        }

        num_to_copy = i != n ? hash_len : okm_len - where;
        memcpy(okm + where, t, num_to_copy);
        where += hash_len;
        t_len = hash_len;
    }

exit:
    mbedtls_md_free(&ctx);
    mbedtls_platform_zeroize(t, sizeof(t));

    return ret;
}

#endif /* MBEDTLS_HKDF_C */

#if defined(MBEDTLS_SELF_TEST)
#include <stdlib.h>

#define mbedtls_printf os_printf

const unsigned char ikm_val[] =
{
	0xAB, 0xE1, 0x0A, 0xCE, 0x13, 0xE7, 0xE1, 0xD9, 0x18, 0x6C, 0x48, 0xF7, 0x88, 0x9D, 0x51, 0x47,
	0x3D, 0x3A, 0x09, 0x61, 0x98, 0x4B, 0xC8, 0x72, 0xDF, 0x70, 0x8E, 0xCC, 0x3E, 0xD3, 0xB8, 0x16
};

const unsigned char info_val[] =
{
	0x9D, 0x01, 0xE3, 0xD9, 0x6F, 0xC4, 0xF1, 0xD5, 0xEA, 0x00, 0xA0, 0x36, 0x92, 0xBC, 0xC5, 0xCF,
	0xFD, 0x53, 0x78, 0x7C, 0x88, 0xB9, 0x34, 0xAF, 0x40, 0x4C, 0x03, 0x9D, 0x32, 0x89, 0xB5, 0xBA
};

static void dump_buf( const char *title, unsigned char *buf, size_t len )
{
	size_t i;

	mbedtls_printf( "%s", title );
	for( i = 0; i < len; i++ )
	{
		if(i%8 == 0)
			mbedtls_printf( "\r\n" );

		BK_RAW_LOGE(NULL,"%c%c", "0123456789ABCDEF" [buf[i] / 16],
                       "0123456789ABCDEF" [buf[i] % 16] );
	}
	mbedtls_printf( "\r\n" );
}
#include "../include/mbedtls/hkdf.h"

extern u64 riscv_get_mtimer(void);
int mbedtls_hkdf_self_test( int verbose , mbedtls_md_type_t type, uint32_t okm_size)
{
	int ret = 0;
	const mbedtls_md_info_t *md;
	unsigned char *okm;
	u64 before, after =  0;

	okm = malloc(okm_size);
	if(okm == NULL)
		return MBEDTLS_ERR_MD_ALLOC_FAILED;

	memset(okm, 0x11, okm_size);
	md = mbedtls_md_info_from_type(type);

#if defined(CONFIG_ARCH_RISCV)
    before = riscv_get_mtimer();
#else
    before = 0;
#endif

	ret = mbedtls_hkdf( md, NULL,0,
						ikm_val, sizeof(ikm_val),
						info_val, sizeof(info_val),
						okm,okm_size );

#if defined(CONFIG_ARCH_RISCV)
    after = riscv_get_mtimer();
#else
    after = 0;
#endif
	mbedtls_printf("run 1 times, take time %d us.\r\n", (uint32_t)(after - before) / 26);
	if( verbose != 0 )
		dump_buf("hkdf:",okm,okm_size);

	free(okm);
	return ret;
}
#endif

