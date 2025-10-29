/*
 * Wrapper functions for mbedtls
 * Copyright (c) 2004-2017, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "../utils/includes.h"
#include "../utils/common.h"
#include "crypto.h"
#include "random.h"
#include "sha256.h"

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif
#include "mbedtls/pk.h"
#include "mbedtls/ecp.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/md.h"
#include "mbedtls/aes.h"
#include "mbedtls/bignum.h"
#include "mbedtls/pkcs5.h"
#include "mbedtls/cmac.h"
#ifdef MBEDTLS_NIST_KW_C
#include "mbedtls/nist_kw.h"
#endif
#include "mbedtls/aes.h"
#include "mbedtls/des.h"
#include "mbedtls/ccm.h"
#include "mbedtls/ecdh.h"
#include "mbedtls/sha256.h"
#include "mbedtls/asn1write.h"
#include "mbedtls/error.h"
#include "mbedtls/oid.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/ecdh.h"

#include "../utils/common.h"
#include "../utils/wpabuf.h"
#include "dh_group5.h"
#include "sha1.h"
#include "sha256.h"
#include "md5.h"
#include "aes_wrap.h"
#include "crypto.h"
#include "tinyara/config.h"

#ifndef BYTES_TO_T_UINT_8

#if 0
#include "mbedtls/arc4.h"
#define MBEDTLS_PRIVATE(member) member
#endif

#if defined(MBEDTLS_HAVE_INT32)

#define BYTES_TO_T_UINT_4( a, b, c, d )                       \
		( (mbedtls_mpi_uint) (a) <<  0 ) |						  \
		( (mbedtls_mpi_uint) (b) <<  8 ) |						  \
		( (mbedtls_mpi_uint) (c) << 16 ) |						  \
		( (mbedtls_mpi_uint) (d) << 24 )

#define BYTES_TO_T_UINT_2( a, b )                   \
		BYTES_TO_T_UINT_4( a, b, 0, 0 )

#define BYTES_TO_T_UINT_8( a, b, c, d, e, f, g, h ) \
		BYTES_TO_T_UINT_4( a, b, c, d ),				\
		BYTES_TO_T_UINT_4( e, f, g, h )

#else /* 64-bits */

#define BYTES_TO_T_UINT_8( a, b, c, d, e, f, g, h ) \
		( (mbedtls_mpi_uint) (a) <<  0 ) |						  \
		( (mbedtls_mpi_uint) (b) <<  8 ) |						  \
		( (mbedtls_mpi_uint) (c) << 16 ) |						  \
		( (mbedtls_mpi_uint) (d) << 24 ) |						  \
		( (mbedtls_mpi_uint) (e) << 32 ) |						  \
		( (mbedtls_mpi_uint) (f) << 40 ) |						  \
		( (mbedtls_mpi_uint) (g) << 48 ) |						  \
		( (mbedtls_mpi_uint) (h) << 56 )

#define BYTES_TO_T_UINT_4( a, b, c, d )             \
		BYTES_TO_T_UINT_8( a, b, c, d, 0, 0, 0, 0 )

#define BYTES_TO_T_UINT_2( a, b )                   \
		BYTES_TO_T_UINT_8( a, b, 0, 0, 0, 0, 0, 0 )

#endif /* bits in mbedtls_mpi_uint */
#endif

#ifdef CONFIG_FULL_SUPPLICANT
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
static const mbedtls_mpi_uint secp192r1_a[] = {
    BYTES_TO_T_UINT_8( 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
};
#endif


#if defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
static const mbedtls_mpi_uint secp224r1_a[] = {
    BYTES_TO_T_UINT_8( 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 ),
};
#endif
#endif /* CONFIG_FULL_SUPPLICANT */

#if defined(MBEDTLS_ECP_DP_SECP256R1_ENABLED)
static const mbedtls_mpi_uint secp256r1_a[] = {
    BYTES_TO_T_UINT_8( 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 ),
    BYTES_TO_T_UINT_8( 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ),
    BYTES_TO_T_UINT_8( 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF ),
};
#endif

#if defined(MBEDTLS_ECP_DP_SECP384R1_ENABLED)
static const mbedtls_mpi_uint secp384r1_a[] = {
    BYTES_TO_T_UINT_8( 0xFC, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 ),
    BYTES_TO_T_UINT_8( 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
};
#endif

#ifdef CONFIG_FULL_SUPPLICANT
#if defined(MBEDTLS_ECP_DP_SECP521R1_ENABLED)
static const mbedtls_mpi_uint secp521r1_a[] = {
    BYTES_TO_T_UINT_8( 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ),
};
#endif
#endif /* CONFIG_FULL_SUPPLICANT */

void bignum_print(char *prefix, const mbedtls_mpi *R)
{
	int i;

	BK_RAW_LOGD(NULL, "%s: %s0x", prefix, R->MBEDTLS_PRIVATE(s) == -1 ? "-" : "");

	for (i = R->MBEDTLS_PRIVATE(n) - 1; i >= 0; i--)
		BK_RAW_LOGD(NULL, "%08lx", R->MBEDTLS_PRIVATE(p)[i]);

	BK_RAW_LOGD(NULL, "\n");
}

/*
 * Create an MPI from embedded constants
 * (assumes len is an exact multiple of sizeof mbedtls_mpi_uint)
 */
static inline void ecp_mpi_load( mbedtls_mpi *X, const mbedtls_mpi_uint *p, size_t len )
{
    X->MBEDTLS_PRIVATE(s) = 1;
    X->MBEDTLS_PRIVATE(n) = len / sizeof( mbedtls_mpi_uint );
    X->MBEDTLS_PRIVATE(p) = (mbedtls_mpi_uint *) p;
}

static int mbedtls_ecp_group_load_ext(mbedtls_ecp_group *grp, mbedtls_ecp_group_id id)
{
	int ret;

	ret = mbedtls_ecp_group_load(grp, id);

	if (!ret) {
		switch (id) {
#ifdef CONFIG_FULL_SUPPLICANT
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
		case MBEDTLS_ECP_DP_SECP192R1:
			ecp_mpi_load(&grp->A, secp192r1_a, sizeof(secp192r1_a));
			break;
#endif /* MBEDTLS_ECP_DP_SECP192R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
		case MBEDTLS_ECP_DP_SECP224R1:
			ecp_mpi_load(&grp->A, secp224r1_a, sizeof(secp224r1_a));
			break;
#endif /* MBEDTLS_ECP_DP_SECP224R1_ENABLED */
#endif /* CONFIG_FULL_SUPPLICANT */

#if defined(MBEDTLS_ECP_DP_SECP256R1_ENABLED)
		case MBEDTLS_ECP_DP_SECP256R1:
			ecp_mpi_load(&grp->A, secp256r1_a, sizeof(secp256r1_a));
			break;
#endif /* MBEDTLS_ECP_DP_SECP256R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP384R1_ENABLED)
		case MBEDTLS_ECP_DP_SECP384R1:
			ecp_mpi_load(&grp->A, secp384r1_a, sizeof(secp384r1_a));
			break;
#endif /* MBEDTLS_ECP_DP_SECP384R1_ENABLED */

#ifdef CONFIG_FULL_SUPPLICANT
#if defined(MBEDTLS_ECP_DP_SECP521R1_ENABLED)
		case MBEDTLS_ECP_DP_SECP521R1:
			ecp_mpi_load(&grp->A, secp521r1_a, sizeof(secp521r1_a));
			break;
#endif /* MBEDTLS_ECP_DP_SECP521R1_ENABLED */
#endif /* CONFIG_FULL_SUPPLICANT */
		default:
			break;
		}
	}

	return ret;
}


static int mbedtls_digest_vector(mbedtls_md_type_t md_type, size_t num_elem,
			 const u8 *addr[], const size_t *len, u8 *mac)
{
	size_t i;
	const mbedtls_md_info_t *md_info;
	mbedtls_md_context_t md_ctx;
	int ret;

	mbedtls_md_init(&md_ctx);

	md_info = mbedtls_md_info_from_type(md_type);
	if (!md_info) {
		wpa_printf(MSG_ERROR, "mbedtls_md_info_from_type() failed");
		return -1;
	}

	ret = mbedtls_md_setup(&md_ctx, md_info, 0);
	if (ret != 0) {
		wpa_printf(MSG_ERROR, "mbedtls_md_setup() returned error");
		goto cleanup;
	}

	ret = mbedtls_md_starts(&md_ctx);
	if (ret != 0) {
		wpa_printf(MSG_ERROR, "mbedtls_md_starts returned error");
		goto cleanup;
	}

	for (i = 0; i < num_elem; i++) {
		ret = mbedtls_md_update(&md_ctx, addr[i], len[i]);
		if (ret != 0) {
			wpa_printf(MSG_ERROR, "mbedtls_md_update ret=%d", ret);
			goto cleanup;
		}
	}

	ret = mbedtls_md_finish(&md_ctx, mac);
cleanup:
	mbedtls_md_free(&md_ctx);

	return ret;

}

int sha256_vector(size_t num_elem, const u8 *addr[], const size_t *len,
		  u8 *mac)
{
	return mbedtls_digest_vector(MBEDTLS_MD_SHA256, num_elem, addr, len, mac);
}

int sha384_vector(size_t num_elem, const u8 *addr[], const size_t *len,
		  u8 *mac)
{
	return mbedtls_digest_vector(MBEDTLS_MD_SHA384, num_elem, addr, len, mac);
}


int sha512_vector(size_t num_elem, const u8 *addr[], const size_t *len,
		  u8 *mac)
{
	return mbedtls_digest_vector(MBEDTLS_MD_SHA512, num_elem, addr, len, mac);
}


int sha1_vector(size_t num_elem, const u8 *addr[], const size_t *len, u8 *mac)
{
	return mbedtls_digest_vector(MBEDTLS_MD_SHA1, num_elem, addr, len, mac);
}

int md5_vector(size_t num_elem, const u8 *addr[], const size_t *len, u8 *mac)
{
	return mbedtls_digest_vector(MBEDTLS_MD_MD5, num_elem, addr, len, mac);
}

#ifdef MBEDTLS_MD4_C
int md4_vector(size_t num_elem, const u8 *addr[], const size_t *len, u8 *mac)
{
	return mbedtls_digest_vector(MBEDTLS_MD_MD4, num_elem, addr, len, mac);
}
#endif

struct crypto_hash {
	enum crypto_hash_alg alg;
	mbedtls_md_context_t ctx;
	bool hmac;
};

struct crypto_hash * crypto_hash_init(enum crypto_hash_alg alg, const u8 *key,
				      size_t key_len)
{
	struct crypto_hash *ctx;
	mbedtls_md_type_t md_type;
	const mbedtls_md_info_t *md_info;
	int ret;

	switch (alg) {
	case CRYPTO_HASH_ALG_MD5:
		md_type = MBEDTLS_MD_MD5;
		break;
	case CRYPTO_HASH_ALG_SHA1:
		md_type = MBEDTLS_MD_SHA1;
		break;
	case CRYPTO_HASH_ALG_SHA256:
		md_type = MBEDTLS_MD_SHA256;
		break;
	case CRYPTO_HASH_ALG_SHA384:
		md_type = MBEDTLS_MD_SHA384;
		break;
	case CRYPTO_HASH_ALG_SHA512:
		md_type = MBEDTLS_MD_SHA512;
		break;
	case CRYPTO_HASH_ALG_HMAC_MD5:
		md_type = MBEDTLS_MD_MD5;
		break;
	case CRYPTO_HASH_ALG_HMAC_SHA1:
		md_type = MBEDTLS_MD_SHA1;
		break;
	case CRYPTO_HASH_ALG_HMAC_SHA256:
		md_type = MBEDTLS_MD_SHA256;
		break;
	default:
		return NULL;
	}

	ctx = os_zalloc(sizeof(*ctx));
	if (ctx == NULL)
		return NULL;

	ctx->alg = alg;

	mbedtls_md_init(&ctx->ctx);
	md_info = mbedtls_md_info_from_type(md_type);
	if (!md_info) {
		os_free(ctx);
		return NULL;
	}
	if (alg == CRYPTO_HASH_ALG_HMAC_MD5 || alg == CRYPTO_HASH_ALG_HMAC_SHA1 ||
		alg == CRYPTO_HASH_ALG_HMAC_SHA256) {
		ret = mbedtls_md_setup(&ctx->ctx, md_info, 1);
		if (ret != 0) {
			os_free(ctx);
			return NULL;
		}

		mbedtls_md_hmac_starts(&ctx->ctx, key, key_len);
		ctx->hmac = true;
	} else {
		ret = mbedtls_md_setup(&ctx->ctx, md_info, 0);
		if (ret != 0) {
			os_free(ctx);
			return NULL;
		}

		mbedtls_md_starts(&ctx->ctx);
	}

	return ctx;
}

void crypto_hash_update(struct crypto_hash *ctx, const u8 *data, size_t len)
{
	if (ctx == NULL)
		return;

	if (ctx->hmac)
		mbedtls_md_hmac_update(&ctx->ctx, data, len);
	else
		mbedtls_md_update(&ctx->ctx, data, len);
}

int crypto_hash_finish(struct crypto_hash *ctx, u8 *mac, size_t *len)
{
	if (ctx == NULL)
		return -2;

	if (mac == NULL || len == NULL) {
		mbedtls_md_free(&ctx->ctx);
		bin_clear_free(ctx, sizeof(*ctx));
		return 0;
	}

	switch (ctx->alg) {
	case CRYPTO_HASH_ALG_MD5:
		if (*len < 16) {
			*len = 16;
			os_free(ctx);
			return -1;
		}
		*len = 16;
		break;
	case CRYPTO_HASH_ALG_SHA1:
		if (*len < 20) {
			*len = 20;
			os_free(ctx);
			return -1;
		}
		*len = 20;
		break;
#ifdef CONFIG_SHA256
	case CRYPTO_HASH_ALG_SHA256:
		if (*len < 32) {
			*len = 32;
			os_free(ctx);
			return -1;
		}
		*len = 32;
		break;
#endif /* CONFIG_SHA256 */
	case CRYPTO_HASH_ALG_SHA384:
		if (*len < 48) {
			*len = 48;
			os_free(ctx);
			return -1;
		}
		*len = 48;
		break;
	case CRYPTO_HASH_ALG_SHA512:
		if (*len < 64) {
			*len = 64;
			os_free(ctx);
			return -1;
		}
		*len = 64;
		break;
	case CRYPTO_HASH_ALG_HMAC_MD5:
		if (*len < 16) {
			*len = 16;
			os_free(ctx);
			return -1;
		}
		*len = 16;
		break;
	case CRYPTO_HASH_ALG_HMAC_SHA1:
		if (*len < 20) {
			*len = 20;
			os_free(ctx);
			return -1;
		}
		*len = 20;

		break;
#ifdef CONFIG_SHA256
	case CRYPTO_HASH_ALG_HMAC_SHA256:
		if (*len < 32) {
			*len = 32;
			os_free(ctx);
			return -1;
		}
		*len = 32;
		break;
#endif /* CONFIG_SHA256 */
	default:
		os_free(ctx);
		return -1;
	}

	if (ctx->hmac)
		mbedtls_md_hmac_finish(&ctx->ctx, mac);
	else
		mbedtls_md_finish(&ctx->ctx, mac);
	mbedtls_md_free(&ctx->ctx);
	bin_clear_free(ctx, sizeof(*ctx));

	return 0;
}


static int hmac_vector(mbedtls_md_type_t md_type,
		       const u8 *key, size_t key_len,
		       size_t num_elem, const u8 *addr[],
		       const size_t *len, u8 *mac)
{
	size_t i;
	const mbedtls_md_info_t *md_info;
	mbedtls_md_context_t md_ctx;
	int ret;

	mbedtls_md_init(&md_ctx);

	md_info = mbedtls_md_info_from_type(md_type);
	if (!md_info) {
		return -1;
	}

	ret = mbedtls_md_setup(&md_ctx, md_info, 1);
	if (ret != 0) {
		return(ret);
	}

	mbedtls_md_hmac_starts(&md_ctx, key, key_len);

	for (i = 0; i < num_elem; i++) {
		mbedtls_md_hmac_update(&md_ctx, addr[i], len[i]);
	}

	mbedtls_md_hmac_finish(&md_ctx, mac);

	mbedtls_md_free(&md_ctx);

	return 0;
}


/* sha1-pbkdf2.c */
#include "mbedtls/pkcs5.h"
int pbkdf2_sha1(const char *passphrase, const u8 *ssid, size_t ssid_len,
		int iterations, u8 *buf, size_t buflen)
{
#if MBEDTLS_VERSION_NUMBER >= 0x03020200 /* mbedtls 3.2.2 */
	  return mbedtls_pkcs5_pbkdf2_hmac_ext(MBEDTLS_MD_SHA1,
			  (const u8 *)passphrase, os_strlen(passphrase),
			  ssid, ssid_len, iterations, 32, buf) ? -1 : 0;
#else
	  const mbedtls_md_info_t *md_info;
	  md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA1);
	  if (md_info == NULL)
		  return -1;
	  mbedtls_md_context_t ctx;
	  mbedtls_md_init(&ctx);
	  int ret = mbedtls_md_setup(&ctx, md_info, 1)
			 || mbedtls_pkcs5_pbkdf2_hmac(&ctx,
			  (const u8 *)passphrase, os_strlen(passphrase),
			  ssid, ssid_len, iterations, 32, buf) ? -1 : 0;
	  mbedtls_md_free(&ctx);
	  return ret;
#endif
}


int hmac_sha384_vector(const u8 *key, size_t key_len, size_t num_elem,
		const u8 *addr[], const size_t *len, u8 *mac)
{
	return hmac_vector(MBEDTLS_MD_SHA384, key, key_len, num_elem, addr,
			   len, mac);
}


int hmac_sha384(const u8 *key, size_t key_len, const u8 *data,
		size_t data_len, u8 *mac)
{
	return hmac_sha384_vector(key, key_len, 1, &data, &data_len, mac);
}

int hmac_sha256_vector(const u8 *key, size_t key_len, size_t num_elem,
		       const u8 *addr[], const size_t *len, u8 *mac)
{
	return hmac_vector(MBEDTLS_MD_SHA256, key, key_len, num_elem, addr,
			   len, mac);
}

int hmac_sha256(const u8 *key, size_t key_len, const u8 *data,
		size_t data_len, u8 *mac)
{
	return hmac_sha256_vector(key, key_len, 1, &data, &data_len, mac);
}

int hmac_sha512_vector(const u8 *key, size_t key_len, size_t num_elem,
		const u8 *addr[], const size_t *len, u8 *mac)
{
	return hmac_vector(MBEDTLS_MD_SHA512, key, key_len, num_elem, addr,
			   len, mac);
}

int hmac_sha512(const u8 *key, size_t key_len, const u8 *data,
		size_t data_len, u8 *mac)
{
	return hmac_sha512_vector(key, key_len, 1, &data, &data_len, mac);
}

int hmac_md5_vector(const u8 *key, size_t key_len, size_t num_elem,
		    const u8 *addr[], const size_t *len, u8 *mac)
{
	return hmac_vector(MBEDTLS_MD_MD5, key, key_len,
			   num_elem, addr, len, mac);
}

int hmac_md5(const u8 *key, size_t key_len, const u8 *data, size_t data_len,
	     u8 *mac)
{
	return hmac_md5_vector(key, key_len, 1, &data, &data_len, mac);
}

int hmac_sha1_vector(const u8 *key, size_t key_len, size_t num_elem,
		     const u8 *addr[], const size_t *len, u8 *mac)
{
	return hmac_vector(MBEDTLS_MD_SHA1, key, key_len, num_elem, addr,
			   len, mac);
}

int hmac_sha1(const u8 *key, size_t key_len, const u8 *data, size_t data_len,
	      u8 *mac)
{
	return hmac_sha1_vector(key, key_len, 1, &data, &data_len, mac);
}

void *aes_encrypt_init(const u8 *key, size_t len)
{
	mbedtls_aes_context *ctx = os_zalloc(sizeof(*ctx));

	if (!ctx)
		return NULL;

	mbedtls_aes_init(ctx);

	if (mbedtls_aes_setkey_enc(ctx, key, len * 8) == 0)
		return ctx;

	mbedtls_aes_free(ctx);
	os_free(ctx);

	return NULL;
}

int aes_encrypt(void *ctx, const u8 *plain, u8 *crypt)
{
	if (!ctx)
		return -1;

	mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, plain, crypt);
	return 0;
}

void aes_encrypt_deinit(void *ctx)
{
	if (ctx) {
		mbedtls_aes_free(ctx);
		os_free(ctx);
		ctx = NULL;
	}
}

void *aes_decrypt_init(const u8 *key, size_t len)
{
	mbedtls_aes_context *ctx;

	ctx = os_zalloc(sizeof(*ctx));
	if (!ctx)
		return NULL;

	mbedtls_aes_init(ctx);

	if (mbedtls_aes_setkey_dec(ctx, key, len * 8) == 0)
		return ctx;

	mbedtls_aes_free(ctx);
	os_free(ctx);

	return NULL;
}

int aes_decrypt(void *ctx, const u8 *crypt, u8 *plain)
{
	if (!ctx)
		return -1;

	mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_DECRYPT, crypt, plain);
	return 0;
}

void aes_decrypt_deinit(void *ctx)
{
	if (ctx) {
		mbedtls_aes_free(ctx);
		os_free(ctx);
		ctx = NULL;
	}
}

int aes_128_cbc_encrypt(const u8 *key, const u8 *iv, u8 *data, size_t data_len)
{
	int ret = 0;
	mbedtls_aes_context ctx;
	u8 cbc[MBEDTLS_AES_BLOCK_SIZE];

	mbedtls_aes_init(&ctx);

	ret = mbedtls_aes_setkey_enc(&ctx, key, 128);
	if (ret < 0) {
		mbedtls_aes_free(&ctx);
		return ret;
	}

	os_memcpy(cbc, iv, MBEDTLS_AES_BLOCK_SIZE);
	ret = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT,
				    data_len, cbc, data, data);
	mbedtls_aes_free(&ctx);

	return ret;
}

int aes_128_cbc_decrypt(const u8 *key, const u8 *iv, u8 *data, size_t data_len)
{
	int ret = 0;
	mbedtls_aes_context ctx;
	u8 cbc[MBEDTLS_AES_BLOCK_SIZE];

	mbedtls_aes_init(&ctx);

	ret = mbedtls_aes_setkey_dec(&ctx, key, 128);
	if (ret < 0) {
		mbedtls_aes_free(&ctx);
		return ret;
	}

	os_memcpy(cbc, iv, MBEDTLS_AES_BLOCK_SIZE);
	ret = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_DECRYPT,
				    data_len, cbc, data, data);
	mbedtls_aes_free(&ctx);

	return ret;

}

int crypto_dh_init(u8 generator, const u8 *prime, size_t prime_len, u8 *privkey,
		   u8 *pubkey)
{
	size_t pubkey_len, pad;

	if (os_get_random(privkey, prime_len) < 0) {
		return -1;
	}
	if (os_memcmp(privkey, prime, prime_len) > 0) {
		/* Make sure private value is smaller than prime */
		privkey[0] = 0;
	}

	pubkey_len = prime_len;
	if (crypto_mod_exp(&generator, 1, privkey, prime_len, prime, prime_len,
				pubkey, &pubkey_len) < 0) {
		return -1;
	}
	if (pubkey_len < prime_len) {
		pad = prime_len - pubkey_len;
		os_memmove(pubkey + pad, pubkey, pubkey_len);
		os_memset(pubkey, 0, pad);
	}

	return 0;
}

/* result = base ^ power mod modulus */
int crypto_mod_exp(const uint8_t *base, size_t base_len,
		   const uint8_t *power, size_t power_len,
		   const uint8_t *modulus, size_t modulus_len,
		   uint8_t *result, size_t *result_len)
{
	mbedtls_mpi bn_base, bn_exp, bn_modulus, bn_result, bn_rinv;
	int ret = 0;

	mbedtls_mpi_init(&bn_base);
	mbedtls_mpi_init(&bn_exp);
	mbedtls_mpi_init(&bn_modulus);
	mbedtls_mpi_init(&bn_result);
	mbedtls_mpi_init(&bn_rinv);

	mbedtls_mpi_read_binary(&bn_base, base, base_len);
	mbedtls_mpi_read_binary(&bn_exp, power, power_len);
	mbedtls_mpi_read_binary(&bn_modulus, modulus, modulus_len);

	ret = mbedtls_mpi_exp_mod(&bn_result, &bn_base, &bn_exp, &bn_modulus,
				  &bn_rinv);
	if (ret < 0) {
		mbedtls_mpi_free(&bn_base);
		mbedtls_mpi_free(&bn_exp);
		mbedtls_mpi_free(&bn_modulus);
		mbedtls_mpi_free(&bn_result);
		mbedtls_mpi_free(&bn_rinv);
		return ret;
	}

	ret = mbedtls_mpi_write_binary(&bn_result, result, *result_len);

	mbedtls_mpi_free(&bn_base);
	mbedtls_mpi_free(&bn_exp);
	mbedtls_mpi_free(&bn_modulus);
	mbedtls_mpi_free(&bn_result);
	mbedtls_mpi_free(&bn_rinv);

	return ret;
}


int crypto_dh_derive_secret(u8 generator, const u8 *prime, size_t prime_len,
			    const u8 *order, size_t order_len,
			    const u8 *privkey, size_t privkey_len,
			    const u8 *pubkey, size_t pubkey_len,
			    u8 *secret, size_t *len)
{
#ifndef CONFIG_BK_WPS_WORKAROUND
	mbedtls_mpi pub;
	int res = -1;

	if (pubkey_len > prime_len ||
	    (pubkey_len == prime_len &&
	     os_memcmp(pubkey, prime, prime_len) >= 0))
		return -1;

	mbedtls_mpi_init(&pub);
	if (mbedtls_mpi_read_binary(&pub, pubkey, pubkey_len) < 0 ||
	    mbedtls_mpi_cmp_int(&pub, 1) <= 0)
		goto fail;

	if (order) {
		mbedtls_mpi p, q, tmp;
		int failed;

		/* verify: pubkey^q == 1 mod p */
		mbedtls_mpi_init(&p);
		mbedtls_mpi_init(&q);
		mbedtls_mpi_init(&tmp);
		failed = mbedtls_mpi_read_binary(&p, prime, prime_len) < 0 ||
			mbedtls_mpi_read_binary(&q, order, order_len) < 0 ||
			mbedtls_mpi_exp_mod(&tmp, &pub, &q, &p, NULL) < 0 ||
			mbedtls_mpi_cmp_int(&tmp, 1) != 0;
		mbedtls_mpi_free(&p);
		mbedtls_mpi_free(&q);
		mbedtls_mpi_free(&tmp);
		if (failed)
			goto fail;
	}

	res = crypto_mod_exp(pubkey, pubkey_len, privkey, privkey_len,
			     prime, prime_len, secret, len);
fail:
	mbedtls_mpi_free(&pub);
	return res;
#else
	/* TODO: check pubkey */
	return crypto_mod_exp(pubkey, pubkey_len, privkey, privkey_len,
			      prime, prime_len, secret, len);
#endif
}


#ifdef MBEDTLS_CMAC_C
int omac1_aes_vector(const u8 *key, size_t key_len, size_t num_elem,
		     const u8 *addr[], const size_t *len, u8 *mac)
{
	const mbedtls_cipher_info_t *cipher_info;
	int i, ret = 0;
	mbedtls_cipher_type_t cipher_type;
	mbedtls_cipher_context_t ctx;

	switch (key_len) {
	case 16:
		cipher_type = MBEDTLS_CIPHER_AES_128_ECB;
		break;
	case 24:
		cipher_type = MBEDTLS_CIPHER_AES_192_ECB;
		break;
	case 32:
		cipher_type = MBEDTLS_CIPHER_AES_256_ECB;
		break;
	default:
		cipher_type = MBEDTLS_CIPHER_NONE;
		break;
	}
	cipher_info = mbedtls_cipher_info_from_type(cipher_type);
	if (cipher_info == NULL) {
		/* Failing at this point must be due to a build issue */
		ret = MBEDTLS_ERR_CIPHER_FEATURE_UNAVAILABLE;
		goto cleanup;
	}

	if (key == NULL ||  mac == NULL) {
		return -1;
	}

	mbedtls_cipher_init(&ctx);

	ret = mbedtls_cipher_setup(&ctx, cipher_info);
	if (ret != 0) {
		goto cleanup;
	}

	ret = mbedtls_cipher_cmac_starts(&ctx, key, key_len * 8);
	if (ret != 0) {
		goto cleanup;
	}

	for (i = 0 ; i < num_elem; i++) {
		ret = mbedtls_cipher_cmac_update(&ctx, addr[i], len[i]);
		if (ret != 0) {
			goto cleanup;
		}
	}

	ret = mbedtls_cipher_cmac_finish(&ctx, mac);
cleanup:
	mbedtls_cipher_free(&ctx);
	return(ret);
}

int omac1_aes_128_vector(const u8 *key, size_t num_elem,
			 const u8 *addr[], const size_t *len, u8 *mac)
{
	return omac1_aes_vector(key, 16, num_elem, addr, len, mac);
}

int omac1_aes_128(const u8 *key, const u8 *data, size_t data_len, u8 *mac)
{
	return omac1_aes_128_vector(key, 1, &data, &data_len, mac);
}
#endif

struct crypto_bignum *crypto_bignum_init(void)
{
	mbedtls_mpi *bn = os_zalloc(sizeof(mbedtls_mpi));
	if (bn == NULL)
		return NULL;

	mbedtls_mpi_init(bn);

	return (struct crypto_bignum *)bn;
}

struct crypto_bignum *crypto_bignum_init_set(const u8 *buf, size_t len)
{
	int ret = 0;
	mbedtls_mpi *bn = os_zalloc(sizeof(mbedtls_mpi));

	if (bn == NULL)
		return NULL;

	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(bn, buf, len));
	return (struct crypto_bignum *) bn;

cleanup:
	os_free(bn);
	return NULL;
}



struct crypto_bignum * crypto_bignum_init_uint(unsigned int val)
{
	int ret = 0;
	mbedtls_mpi *bn = os_zalloc(sizeof(mbedtls_mpi));;

	if (bn == NULL)
		return NULL;

	mbedtls_mpi_init(bn);
	MBEDTLS_MPI_CHK(mbedtls_mpi_lset(bn, val));
	return (struct crypto_bignum *) bn;

cleanup:
	os_free(bn);
	return NULL;
}

void crypto_bignum_deinit(struct crypto_bignum *n, int clear)
{
	mbedtls_mpi_free((mbedtls_mpi *)n);
	os_free((mbedtls_mpi *)n);
}


int crypto_bignum_to_bin(const struct crypto_bignum *a,
						 u8 *buf, size_t buflen, size_t padlen)
{
	int num_bytes, offset;

	if (padlen > buflen)
		return -1;

	num_bytes = mbedtls_mpi_size((mbedtls_mpi *) a);

	if ((size_t) num_bytes > buflen)
		return -1;
	if (padlen > (size_t) num_bytes)
		offset = padlen - num_bytes;
	else
		offset = 0;

	os_memset(buf, 0, offset);
	mbedtls_mpi_write_binary((mbedtls_mpi *) a, buf + offset,
			mbedtls_mpi_size((mbedtls_mpi *)a));

	return num_bytes + offset;
}


int crypto_bignum_rand(struct crypto_bignum *r, const struct crypto_bignum *m)
{
	int ret = 0;
	size_t len;
	u8 *buf;

	len = (crypto_bignum_bits(m) + 7) / 8;
	buf = os_malloc(len);
	if (!buf || os_get_random(buf, len) != 0 ||
		mbedtls_mpi_read_binary((mbedtls_mpi *)r, buf, len) ||
		crypto_bignum_mod(r, m, r))
		ret = -1;

	bin_clear_free(buf, len);
	return ret;
}


int crypto_bignum_add(const struct crypto_bignum *a,
					  const struct crypto_bignum *b,
					  struct crypto_bignum *c)
{
	return mbedtls_mpi_add_mpi((mbedtls_mpi *) c,
							   (const mbedtls_mpi *) a,
							   (const mbedtls_mpi *) b) ? -1 : 0;
}


int crypto_bignum_mod(const struct crypto_bignum *a,
					  const struct crypto_bignum *b,
					  struct crypto_bignum *c)
{
	return mbedtls_mpi_mod_mpi((mbedtls_mpi *) c,
							   (const mbedtls_mpi *) a,
							   (const mbedtls_mpi *) b) ? -1 : 0;
}


//d = a^b (mod c)
int crypto_bignum_exptmod(const struct crypto_bignum *a,
						  const struct crypto_bignum *b,
						  const struct crypto_bignum *c,
						  struct crypto_bignum *d)
{
	if (b == a || d == b || d == c) {
		int ret;
		struct crypto_bignum *rd;


		rd = crypto_bignum_init();
		if (!rd)
			return -1;

		ret = mbedtls_mpi_exp_mod((mbedtls_mpi *) rd,
					(const mbedtls_mpi *) a,
					(const mbedtls_mpi *) b,
					(const mbedtls_mpi *) c, NULL) ? -1 : 0;

		if (!ret)
			mbedtls_mpi_copy((mbedtls_mpi *)d, (mbedtls_mpi *)rd);

		crypto_bignum_deinit(rd, 0);

		return ret;
	} else {
		return mbedtls_mpi_exp_mod((mbedtls_mpi *) d,
					(const mbedtls_mpi *) a,
					(const mbedtls_mpi *) b,
					(const mbedtls_mpi *) c, NULL) ? -1 : 0;
	}
}


int crypto_bignum_inverse(const struct crypto_bignum *a,
                          const struct crypto_bignum *b,
                          struct crypto_bignum *c)
{
    return mbedtls_mpi_inv_mod((mbedtls_mpi *) c,
				(const mbedtls_mpi *) a,
                (const mbedtls_mpi *) b) ? -1 : 0;
}


int crypto_bignum_sub(const struct crypto_bignum *a,
                      const struct crypto_bignum *b,
                      struct crypto_bignum *c)
{
    return mbedtls_mpi_sub_mpi((mbedtls_mpi *) c,
				(const mbedtls_mpi *) a,
				(const mbedtls_mpi *) b) ? -1 : 0;
}


int crypto_bignum_div(const struct crypto_bignum *a,
                      const struct crypto_bignum *b,
                      struct crypto_bignum *c)
{
    return mbedtls_mpi_div_mpi((mbedtls_mpi *) c, NULL,
				(const mbedtls_mpi *) a,
				(const mbedtls_mpi *) b) ? -1 : 0;
}


// d = a * b (mod c)
int crypto_bignum_mulmod(const struct crypto_bignum *a,
						 const struct crypto_bignum *b,
						 const struct crypto_bignum *c,
						 struct crypto_bignum *d)
{
	int res;
	mbedtls_mpi temp;
	mbedtls_mpi_init(&temp);

	res = mbedtls_mpi_mul_mpi(&temp, (const mbedtls_mpi *) a,
							  (const mbedtls_mpi *) b);
	if (res)
		return -1;

	res = mbedtls_mpi_mod_mpi((mbedtls_mpi *) d, &temp,
							  (mbedtls_mpi *) c);

	mbedtls_mpi_free(&temp);

	return res ? -1 : 0;
}


//c = a^2 (mod b)
int crypto_bignum_sqrmod(const struct crypto_bignum *a,
			 const struct crypto_bignum *b,
			 struct crypto_bignum *c)
{
	int ret;
	mbedtls_mpi ll;

	mbedtls_mpi_init(&ll);

	MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&ll, (mbedtls_mpi *)a, (mbedtls_mpi *)a));
	MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi((mbedtls_mpi *)c, &ll, (mbedtls_mpi *)b));

cleanup:
	mbedtls_mpi_free(&ll);
	return ret;
}


int crypto_bignum_rshift(const struct crypto_bignum *a, int n,
			 struct crypto_bignum *r)
{
	if (mbedtls_mpi_copy((mbedtls_mpi *)r, (mbedtls_mpi *)a))
		return -1;
	return mbedtls_mpi_shift_r((mbedtls_mpi *)r, n);
}


int crypto_bignum_cmp(const struct crypto_bignum *a,
					  const struct crypto_bignum *b)
{
	return mbedtls_mpi_cmp_mpi((const mbedtls_mpi *) a,
							   (const mbedtls_mpi *) b);
}


int crypto_bignum_bits(const struct crypto_bignum *a)
{
	return mbedtls_mpi_bitlen((const mbedtls_mpi *) a);
}


int crypto_bignum_is_zero(const struct crypto_bignum *a)
{
	return (mbedtls_mpi_cmp_int((const mbedtls_mpi *) a, 0) == 0);
}


int crypto_bignum_is_one(const struct crypto_bignum *a)
{
	return (mbedtls_mpi_cmp_int((const mbedtls_mpi *) a, 1) == 0);
}


int crypto_bignum_is_odd(const struct crypto_bignum *a)
{
	return mbedtls_mpi_get_bit((const mbedtls_mpi *)a, 0) == 1;
}


int crypto_bignum_legendre(const struct crypto_bignum *a,
						   const struct crypto_bignum *p)
{
	mbedtls_mpi exp, tmp;
	int res = -2, ret;

	mbedtls_mpi_init(&exp);
	mbedtls_mpi_init(&tmp);

	/* exp = (p-1) / 2 */
	MBEDTLS_MPI_CHK(mbedtls_mpi_sub_int(&exp, (const mbedtls_mpi *) p, 1));
	MBEDTLS_MPI_CHK(mbedtls_mpi_shift_r(&exp, 1));
	MBEDTLS_MPI_CHK(mbedtls_mpi_exp_mod(&tmp,
				   (const mbedtls_mpi *) a, &exp, (const mbedtls_mpi *) p, NULL));

	if (mbedtls_mpi_cmp_int(&tmp, 1) == 0)
		res = 1;
	else if (mbedtls_mpi_cmp_int(&tmp, 0) == 0
			 /* The below check is workaround for the case where HW
			  * does not behave properly for X ^ A mod M when X is
			  * power of M. Instead of returning value 0, value M is
			  * returned.*/
			 || mbedtls_mpi_cmp_mpi(&tmp, (const mbedtls_mpi *)p) == 0)
		res = 0;
	else
		res = -1;

cleanup:
	mbedtls_mpi_free(&tmp);
	mbedtls_mpi_free(&exp);
	return res;
}


int crypto_bignum_to_string(const struct crypto_bignum *a,
				  u8 *buf, size_t buflen, size_t padlen)
{
	int num_bytes, offset;
	size_t outlen;

	if (padlen > buflen)
		return -1;

	num_bytes = mbedtls_mpi_size((mbedtls_mpi *) a);

	if (padlen > (size_t) num_bytes)
		offset = padlen - num_bytes;
	else
		offset = 0;

	os_memset(buf, 0, offset);
	mbedtls_mpi_write_string((mbedtls_mpi *) a, 16, (char *)(buf + offset),
							 mbedtls_mpi_size((mbedtls_mpi *)a), &outlen);

	return outlen;
}


//d = a + b (mod c)
int crypto_bignum_addmod(const struct crypto_bignum *a,
			 const struct crypto_bignum *b,
			 const struct crypto_bignum *c,
			 struct crypto_bignum *d)
{
    struct crypto_bignum *tmp = crypto_bignum_init();
    int ret = -1;

    if (mbedtls_mpi_add_mpi((mbedtls_mpi *) tmp,
			(const mbedtls_mpi *) a, (const mbedtls_mpi *) b) < 0)
        goto fail;

    if (mbedtls_mpi_mod_mpi( (mbedtls_mpi *) d,
			(const mbedtls_mpi *) tmp, (const mbedtls_mpi *) c) < 0)
        goto fail;

    ret = 0;
fail:
    crypto_bignum_deinit(tmp, 0);
    return ret;
}

void crypto_free_buffer(unsigned char *buf)
{
    os_free(buf);
}


#ifdef CONFIG_ECC
struct crypto_ec {
	mbedtls_ecp_group group;
};

struct crypto_ecdh {
    mbedtls_ecdh_context ctx;
    size_t length;
};

#ifndef ECP_PRV_DER_MAX_BYTES
#define ECP_PRV_DER_MAX_BYTES   29 + 3 * MBEDTLS_ECP_MAX_BYTES
#endif


int crypto_rng_wrapper(void *ctx, unsigned char *buf, size_t len)
{
	return random_get_bytes(buf, len);
}

struct crypto_ec *crypto_ec_init(int group)
{
	struct crypto_ec *e;

	mbedtls_ecp_group_id  grp_id;

	/* IANA registry to mbedtls internal mapping*/
	switch (group) {
	case 19:
		grp_id = MBEDTLS_ECP_DP_SECP256R1;
		break;
	case 20:
		grp_id = MBEDTLS_ECP_DP_SECP384R1;
		break;
#ifdef CONFIG_FULL_SUPPLICANT
	case 21:
		grp_id = MBEDTLS_ECP_DP_SECP521R1;	/* tooooo slow for no-hw */
		break;
	case 25:
		grp_id = MBEDTLS_ECP_DP_SECP192R1;
		break;
	case 26:
		grp_id = MBEDTLS_ECP_DP_SECP224R1;
		break;
#endif
#ifdef HAVE_ECC_BRAINPOOL
	//case 27:
	//	grp_id = ECC_BRAINPOOLP224R1;
	//	break;
	case 28:
		grp_id = MBEDTLS_ECP_DP_BP256R1;
		break;
	case 29:
		grp_id = MBEDTLS_ECP_DP_BP384R1;
		break;
	case 30:
		grp_id = MBEDTLS_ECP_DP_BP512R1;
		break;
#endif /* HAVE_ECC_BRAINPOOL */
		default:
			return NULL;

	}
	e = os_zalloc(sizeof(*e));
	if (e == NULL) {
		return NULL;
	}

	mbedtls_ecp_group_init(&e->group);

	if (mbedtls_ecp_group_load_ext(&e->group, grp_id)) {
		crypto_ec_deinit(e);
		e = NULL;
	}

	return e;
}


void crypto_ec_deinit(struct crypto_ec *e)
{
	if (e == NULL) {
		return;
	}

	mbedtls_ecp_group_free(&e->group);
	os_free(e);
}


struct crypto_ec_point *crypto_ec_point_init(struct crypto_ec *e)
{
	mbedtls_ecp_point *pt;
	if (e == NULL) {
		return NULL;
	}

	pt = os_zalloc(sizeof(mbedtls_ecp_point));

	if( pt == NULL) {
		return NULL;
	}

	mbedtls_ecp_point_init(pt);

	return (struct crypto_ec_point *) pt;
}


size_t crypto_ec_prime_len(struct crypto_ec *e)
{
	return mbedtls_mpi_size(&e->group.P);
}


size_t crypto_ec_prime_len_bits(struct crypto_ec *e)
{
	return mbedtls_mpi_bitlen(&e->group.P);
}


size_t crypto_ec_order_len(struct crypto_ec *e)
{
	return (mbedtls_mpi_bitlen(&e->group.N) + 7) / 8;
}


struct crypto_ec_group *crypto_ec_get_group_byname(const char *name)
{
	struct crypto_ec *e;
	const mbedtls_ecp_curve_info *curve = mbedtls_ecp_curve_info_from_name(name);

	e = os_zalloc(sizeof(*e));
	if ((e == NULL) || (curve == NULL)) {
		return NULL;
	}

	mbedtls_ecp_group_init( &e->group );

	if (mbedtls_ecp_group_load_ext(&e->group, curve->grp_id)) {
		crypto_ec_deinit(e);
		e = NULL;
	}

	return (struct crypto_ec_group *) &e->group;
}

const struct crypto_bignum *crypto_ec_get_prime(struct crypto_ec *e)
{
	return (const struct crypto_bignum *) &e->group.P;
}


const struct crypto_bignum *crypto_ec_get_order(struct crypto_ec *e)
{
	return (const struct crypto_bignum *) &e->group.N;
}

const struct crypto_bignum * crypto_ec_get_a(struct crypto_ec *e)
{
	return (const struct crypto_bignum *) &e->group.A;
}


const struct crypto_bignum * crypto_ec_get_b(struct crypto_ec *e)
{
	return (const struct crypto_bignum *) &e->group.B;
}


void crypto_ec_point_deinit(struct crypto_ec_point *p, int clear)
{
	mbedtls_ecp_point_free((mbedtls_ecp_point *) p);
	os_free(p);
}

int crypto_ec_point_to_bin(struct crypto_ec *e,
		const struct crypto_ec_point *point, u8 *x, u8 *y)
{
	int len = mbedtls_mpi_size(&e->group.P);

	if (x) {
		if(crypto_bignum_to_bin(
			(struct crypto_bignum *)&((mbedtls_ecp_point *)point)->MBEDTLS_PRIVATE(X),
					x, len, len) < 0) {
			return -1;
		}

	}

	if (y) {
		if(crypto_bignum_to_bin(
			(struct crypto_bignum *)&((mbedtls_ecp_point *)point)->MBEDTLS_PRIVATE(Y),
					y, len, len) < 0) {
			return -1;
		}
	}

	return 0;
}

int crypto_ec_get_affine_coordinates(struct crypto_ec *e, struct crypto_ec_point *pt,
		struct crypto_bignum *x, struct crypto_bignum *y)
{
	int ret = -1;
	mbedtls_ecp_point *point = (mbedtls_ecp_point *)pt;

	if (!mbedtls_ecp_is_zero(point)  && (mbedtls_mpi_cmp_int( &point->MBEDTLS_PRIVATE(Z), 1 ) == 0 )) {
		// Affine coordinates mean that z should be 1,
		wpa_printf(MSG_ERROR, "Z coordinate is neither 0 or 1");
		return -1;
	}

	if (x) {
		MBEDTLS_MPI_CHK(mbedtls_mpi_copy((mbedtls_mpi*) x, &((mbedtls_ecp_point* )point)->MBEDTLS_PRIVATE(X)));
	}
	if (y) {
		MBEDTLS_MPI_CHK(mbedtls_mpi_copy((mbedtls_mpi*) y, &((mbedtls_ecp_point* )point)->MBEDTLS_PRIVATE(Y)));
	}
	return 0;
cleanup:
	return ret;
}

struct crypto_ec_point *crypto_ec_point_from_bin(struct crypto_ec *e,
		const u8 *val)
{
	mbedtls_ecp_point *pt;
	int len, ret;

	if (e == NULL) {
		return NULL;
	}

	len = mbedtls_mpi_size(&e->group.P);

	pt = os_zalloc(sizeof(mbedtls_ecp_point));
	mbedtls_ecp_point_init(pt);

	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&pt->MBEDTLS_PRIVATE(X), val, len));
	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&pt->MBEDTLS_PRIVATE(Y), val + len, len));
	MBEDTLS_MPI_CHK(mbedtls_mpi_lset((&pt->MBEDTLS_PRIVATE(Z)), 1));

	return (struct crypto_ec_point *) pt;

cleanup:
	mbedtls_ecp_point_free(pt);
	os_free(pt);
	return NULL;
}


int crypto_ec_point_add(struct crypto_ec *e, const struct crypto_ec_point *a,
		const struct crypto_ec_point *b,
		struct crypto_ec_point *c)
{
	int ret;
	mbedtls_mpi one;

	mbedtls_mpi_init(&one);

	MBEDTLS_MPI_CHK(mbedtls_mpi_lset( &one, 1 ));
	MBEDTLS_MPI_CHK(mbedtls_ecp_muladd(&e->group, (mbedtls_ecp_point *) c,
			&one, (const mbedtls_ecp_point *)a , &one, (const mbedtls_ecp_point *)b));

cleanup:
	mbedtls_mpi_free(&one);
	return ret ? -1 : 0;
}

int crypto_ec_point_mul(struct crypto_ec *e, const struct crypto_ec_point *p,
		const struct crypto_bignum *b,
		struct crypto_ec_point *res)
{
	int ret;

#if CONFIG_TFM_ECP_NSC
	#include "tfm_ecp_nsc.h"
	#include "tfm_ns_interface.h"

	mpi_binary_t mpi_b = {0};
	ecp_binary_t ecp_r = {0};
	ecp_binary_t ecp_p = {0};

	mbedtls_mpi *c = (mbedtls_mpi *)b;
	mpi_b.olen = c->n*(sizeof(mbedtls_mpi_uint));
	MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary((const mbedtls_mpi *)b, mpi_b.buf, mpi_b.olen));

	MBEDTLS_MPI_CHK(mbedtls_ecp_point_write_binary(&e->group, (const mbedtls_ecp_point *)p , 0, &ecp_p.olen,
									ecp_p.buf, sizeof(ecp_p.buf)));

	tfm_ns_interface_lock();
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

	ret = psa_mbedtls_ecp_mul_nsc(e->group.id, &ecp_r, &mpi_b, &ecp_p);

	GLOBAL_INT_RESTORE();
	tfm_ns_interface_unlock();
	if(ret != 0)
	{
		os_printf(" Err: psa_mbedtls_ecp_mul_nsc err = %d \r\n", ret);
		goto cleanup;
	}

	MBEDTLS_MPI_CHK(mbedtls_ecp_point_read_binary((const mbedtls_ecp_group *)&e->group, (mbedtls_ecp_point *) res, 
									(const unsigned char *)ecp_r.buf, ecp_r.olen));

cleanup:

#else
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;

	mbedtls_entropy_init(&entropy);
	mbedtls_ctr_drbg_init(&ctr_drbg);

	MBEDTLS_MPI_CHK(mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
				NULL, 0));

	MBEDTLS_MPI_CHK(mbedtls_ecp_mul(&e->group,
				(mbedtls_ecp_point *) res,
				(const mbedtls_mpi *)b,
				(const mbedtls_ecp_point *)p,
				mbedtls_ctr_drbg_random,
				&ctr_drbg));
cleanup:

	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
#endif

	return ret ? -1 : 0;
}


/*  Currently mbedtls does not have any function for inverse
 *  This function calculates inverse of a point.
 *  Set R = -P
 */
static int ecp_opp(const mbedtls_ecp_group *grp, mbedtls_ecp_point *R, const mbedtls_ecp_point *P)
{
	int ret = 0;

	/* Copy */
	if (R != P) {
		MBEDTLS_MPI_CHK(mbedtls_ecp_copy(R, P));
	}

	/* In-place opposite */
	if (mbedtls_mpi_cmp_int(&R->MBEDTLS_PRIVATE(Y), 0) != 0) {
		MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mpi(&R->MBEDTLS_PRIVATE(Y), &grp->P, &R->MBEDTLS_PRIVATE(Y)));
	}

cleanup:
	return (ret );
}

int crypto_ec_point_invert(struct crypto_ec *e, struct crypto_ec_point *p)
{
	return ecp_opp(&e->group, (mbedtls_ecp_point *) p, (mbedtls_ecp_point *) p) ? -1 : 0;
}

int crypto_ec_point_solve_y_coord(struct crypto_ec *e,
		struct crypto_ec_point *p,
		const struct crypto_bignum *x, int y_bit)
{
	mbedtls_mpi temp;
	mbedtls_mpi *y_sqr, *y;
	mbedtls_mpi_init(&temp);
	int ret = 0;

	y = &((mbedtls_ecp_point *)p)->MBEDTLS_PRIVATE(Y);

	/* Faster way to find sqrt
	 * Works only with curves having prime p
	 * such that p ≡ 3 (mod 4)
	 *  y_ = (y2 ^ ((p+1)/4)) mod p
	 *
	 *  if LSB of both x and y are same: y = y_
	 *   else y = p - y_
	 * y_bit is LSB of x
	 */
	y_bit = (y_bit != 0);

	y_sqr = (mbedtls_mpi *) crypto_ec_point_compute_y_sqr(e, x);

	if (y_sqr) {

		MBEDTLS_MPI_CHK(mbedtls_mpi_add_int(&temp, &e->group.P, 1));
		MBEDTLS_MPI_CHK(mbedtls_mpi_div_int(&temp, NULL, &temp, 4));
		MBEDTLS_MPI_CHK(mbedtls_mpi_exp_mod(y, y_sqr, &temp, &e->group.P, NULL));

		if (y_bit != mbedtls_mpi_get_bit(y, 0))
			MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mpi(y, &e->group.P, y));

		MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&((mbedtls_ecp_point* )p)->MBEDTLS_PRIVATE(X), (const mbedtls_mpi*) x));
		MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&((mbedtls_ecp_point *)p)->MBEDTLS_PRIVATE(Z), 1));
	} else {
		ret = 1;
	}
cleanup:
	mbedtls_mpi_free(&temp);
	mbedtls_mpi_free(y_sqr);
	os_free(y_sqr);
	return ret ? -1 : 0;
}

int crypto_get_order(struct crypto_ec_group *group, struct crypto_bignum *x)
{
	return mbedtls_mpi_copy((mbedtls_mpi *) x, &((mbedtls_ecp_group *)group)->N);
}

struct crypto_bignum *crypto_ec_point_compute_y_sqr(struct crypto_ec *e,
		const struct crypto_bignum *x)
{
	mbedtls_mpi temp, temp2, num;
	int ret = 0;

	mbedtls_mpi *y_sqr = os_zalloc(sizeof(mbedtls_mpi));
	if (y_sqr == NULL) {
		return NULL;
	}

	mbedtls_mpi_init(&temp);
	mbedtls_mpi_init(&temp2);
	mbedtls_mpi_init(&num);
	mbedtls_mpi_init(y_sqr);

	/* y^2 = x^3 + ax + b  mod  P*/
	/* mbedtls does not have mod-add or mod-mul apis.
	 *
	 */

	/* Calculate x^3  mod P*/
	MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&num, 3));
	MBEDTLS_MPI_CHK(mbedtls_mpi_exp_mod(&temp, (const mbedtls_mpi *) x, &num, &e->group.P, NULL));

	/* Calculate ax  mod P*/
	MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&num, -3));
	MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&temp2, (const mbedtls_mpi *) x, &num));
	MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&temp2, &temp2, &e->group.P));

	/* Calculate ax + b  mod P. Note that b is already < P*/
	MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(&temp2, &temp2, &e->group.B));
	MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&temp2, &temp2, &e->group.P));

	/* Calculate x^3 + ax + b  mod P*/
	MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(&temp2, &temp2, &temp));
	MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(y_sqr, &temp2, &e->group.P));

cleanup:
	mbedtls_mpi_free(&temp);
	mbedtls_mpi_free(&temp2);
	mbedtls_mpi_free(&num);
	if (ret) {
		mbedtls_mpi_free(y_sqr);
		os_free(y_sqr);
		return NULL;
	} else {
		return (struct crypto_bignum *) y_sqr;
	}
}

int crypto_ec_point_is_at_infinity(struct crypto_ec *e,
		const struct crypto_ec_point *p)
{
	return mbedtls_ecp_is_zero((mbedtls_ecp_point *) p);
}

int crypto_ec_point_is_on_curve(struct crypto_ec *e,
		const struct crypto_ec_point *p)
{
	mbedtls_mpi y_sqr_lhs, *y_sqr_rhs = NULL, two;
	int ret = 0, on_curve = 0;

	mbedtls_mpi_init(&y_sqr_lhs);
	mbedtls_mpi_init(&two);

	/* Calculate y^2  mod P*/
	MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&two, 2));
	MBEDTLS_MPI_CHK(mbedtls_mpi_exp_mod(&y_sqr_lhs,
			&((const mbedtls_ecp_point *)p)->MBEDTLS_PRIVATE(Y) , &two, &e->group.P, NULL));

	y_sqr_rhs = (mbedtls_mpi *) crypto_ec_point_compute_y_sqr(e,
		(const struct crypto_bignum *) & ((const mbedtls_ecp_point *)p)->MBEDTLS_PRIVATE(X));

	if (y_sqr_rhs && (mbedtls_mpi_cmp_mpi(y_sqr_rhs, &y_sqr_lhs) == 0)) {
		on_curve = 1;
	}

cleanup:
	mbedtls_mpi_free(&y_sqr_lhs);
	mbedtls_mpi_free(&two);
	mbedtls_mpi_free(y_sqr_rhs);
	os_free(y_sqr_rhs);
	return (ret == 0) && (on_curve == 1);
}

int crypto_ec_point_cmp(const struct crypto_ec *e,
		const struct crypto_ec_point *a,
		const struct crypto_ec_point *b)
{
	return mbedtls_ecp_point_cmp((const mbedtls_ecp_point *) a,
			(const mbedtls_ecp_point *) b);
}


int crypto_key_compare(struct crypto_key *key1, struct crypto_key *key2)
{
#if CONFIG_PSA_MBEDTLS
	if (mbedtls_pk_check_pair((mbedtls_pk_context *)key1, (mbedtls_pk_context *)key2, NULL, NULL) < 0)
#else
	if (mbedtls_pk_check_pair((mbedtls_pk_context *)key1, (mbedtls_pk_context *)key2, NULL, NULL) < 0)
#endif
		return 0;

	return 1;
}

void crypto_debug_print_point(const char *title, struct crypto_ec *e,
		const struct crypto_ec_point *point)
{
	u8 x[32], y[32];

	if (crypto_ec_point_to_bin(e, point, x, y) < 0) {
		wpa_printf(MSG_ERROR, "error: failed to get corrdinates\n");
		return;
	}

	wpa_hexdump(MSG_ERROR, "x:", x, 32);
	wpa_hexdump(MSG_ERROR, "y:", y, 32);
}

static struct crypto_key *crypto_alloc_key(void)
{
	mbedtls_pk_context *key = os_malloc(sizeof(*key));

	if (!key) {
		wpa_printf(MSG_ERROR, "%s: memory allocation failed\n", __func__);
		return NULL;
	}
	mbedtls_pk_init(key);

	return (struct crypto_key *)key;
}


struct crypto_key * crypto_ec_set_pubkey_point(const struct crypto_ec_group *group,
		const u8 *buf, size_t len)
{
	mbedtls_ecp_point *point = NULL;
	struct crypto_key *pkey = NULL;
	int ret;
	mbedtls_pk_context *key = (mbedtls_pk_context *)crypto_alloc_key();

	if (!key) {
		wpa_printf(MSG_ERROR, "%s: memory allocation failed\n", __func__);
		return NULL;
	}

	point = (mbedtls_ecp_point *)crypto_ec_point_from_bin((struct crypto_ec *)group, buf);
	if (crypto_ec_point_is_at_infinity((struct crypto_ec *)group, (struct crypto_ec_point *)point)) {
		wpa_printf(MSG_ERROR, "Point is at infinity");
		goto fail;
	}
	if (!crypto_ec_point_is_on_curve((struct crypto_ec *)group, (struct crypto_ec_point *)point)) {
		wpa_printf(MSG_ERROR, "Point not on curve");
		goto fail;
	}

	if (mbedtls_ecp_check_pubkey((mbedtls_ecp_group *)group, point) < 0) { //typecast
		// ideally should have failed in upper condition, duplicate code??
		wpa_printf(MSG_ERROR, "Invalid key");
		goto fail;
	}
	mbedtls_ecp_keypair *ecp_key = malloc(sizeof (*ecp_key));
	if (!ecp_key) {
		wpa_printf(MSG_ERROR, "key allocation failed");
		goto fail;
	}

	/* Init keypair */
	mbedtls_ecp_keypair_init(ecp_key);
	// TODO Is it needed? check?
	MBEDTLS_MPI_CHK(mbedtls_ecp_copy(&ecp_key->MBEDTLS_PRIVATE(Q), point));

	/* Assign values */
	if( ( ret = mbedtls_pk_setup( key,
					mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY) ) ) != 0 )
		goto fail;

	if (key->MBEDTLS_PRIVATE(pk_ctx))
		os_free(key->MBEDTLS_PRIVATE(pk_ctx));
	key->MBEDTLS_PRIVATE(pk_ctx) = ecp_key;
	mbedtls_ecp_copy(&mbedtls_pk_ec(*key)->MBEDTLS_PRIVATE(Q), point);
	mbedtls_ecp_group_load_ext(&mbedtls_pk_ec(*key)->MBEDTLS_PRIVATE(grp), MBEDTLS_ECP_DP_SECP256R1);

	pkey = (struct crypto_key *)key;
cleanup:
	crypto_ec_point_deinit((struct crypto_ec_point *)point, 0);
	return pkey;
fail:
	if (point)
		crypto_ec_point_deinit((struct crypto_ec_point *)point, 0);
	if (key)
		mbedtls_pk_free(key);
	pkey = NULL;
	return pkey;
}


void crypto_ec_free_key(struct crypto_key *key)
{
	mbedtls_pk_context *pkey = (mbedtls_pk_context *)key;
	mbedtls_pk_free(pkey);
	os_free(key);
}


struct crypto_ec_point *crypto_ec_get_public_key(struct crypto_key *key)
{
	mbedtls_pk_context *pkey = (mbedtls_pk_context *)key;

	return (struct crypto_ec_point *)&mbedtls_pk_ec(*pkey)->MBEDTLS_PRIVATE(Q);
}


int crypto_ec_get_priv_key_der(struct crypto_key *key, unsigned char **key_data, int *key_len)
{
	mbedtls_pk_context *pkey = (mbedtls_pk_context *)key;
	char der_data[ECP_PRV_DER_MAX_BYTES];

	*key_len = mbedtls_pk_write_key_der(pkey, (unsigned char *)der_data, ECP_PRV_DER_MAX_BYTES);
	if (!*key_len)
		return -1;

	*key_data = os_malloc(*key_len);

	if (!*key_data) {
		wpa_printf(MSG_ERROR, "memory allocation failed\n");
		return -1;
	}
	os_memcpy(*key_data, der_data, *key_len);

	return 0;
}


struct crypto_ec_group *crypto_ec_get_group_from_key(struct crypto_key *key)
{
	mbedtls_pk_context *pkey = (mbedtls_pk_context *)key;

	return (struct crypto_ec_group *)&(mbedtls_pk_ec(*pkey)->MBEDTLS_PRIVATE(grp));
}


struct crypto_bignum *crypto_ec_get_private_key(struct crypto_key *key)
{
	mbedtls_pk_context *pkey = (mbedtls_pk_context *)key;

	return ((struct crypto_bignum *)&(mbedtls_pk_ec(*pkey)->MBEDTLS_PRIVATE(d)));
}


int crypto_ec_get_publickey_buf(struct crypto_key *key, u8 *key_buf, int len)
{
	mbedtls_pk_context *pkey = (mbedtls_pk_context *)key;
	unsigned char buf[MBEDTLS_MPI_MAX_SIZE + 10]; /* tag, length + MPI */
	unsigned char *c = buf + sizeof(buf );
	size_t pk_len = 0;

	memset(buf, 0, sizeof(buf) );
	pk_len = mbedtls_pk_write_pubkey( &c, buf, pkey);

	if (!pk_len)
		return -1;

	if (len == 0)
		return pk_len;

	os_memcpy(key_buf, buf + MBEDTLS_MPI_MAX_SIZE + 10 - pk_len, pk_len);

	return pk_len;
}


int crypto_write_pubkey_der(struct crypto_key *key, unsigned char **key_buf)
{
	unsigned char output_buf[1600] = {0};
	int len = mbedtls_pk_write_pubkey_der((mbedtls_pk_context *)key, output_buf, 1600);
	if (len <= 0)
		return 0;

	*key_buf = os_malloc(len);
	if (!*key_buf) {
		return 0;
	}
	os_memcpy(*key_buf, output_buf + 1600 - len, len);

	return len;
}

struct crypto_key *crypto_ec_get_key(const u8 *privkey, size_t privkey_len)
{
	int ret;
	mbedtls_pk_context *kctx = (mbedtls_pk_context *)crypto_alloc_key();

	if (!kctx) {
		wpa_printf(MSG_ERROR, "memory allocation failed\n");
		return NULL;
	}
#if CONFIG_PSA_MBEDTLS && !CONFIG_PSA_MBEDTLS_FORWARD_COMPATIBILITY
	ret = mbedtls_pk_parse_key(kctx, privkey, privkey_len, NULL, 0, NULL, NULL);
#else
	ret = mbedtls_pk_parse_key(kctx, privkey, privkey_len, NULL, 0, NULL, NULL);
#endif

	if (ret < 0) {
		//crypto_print_error_string(ret);
		goto fail;
	}

	return (struct crypto_key *)kctx;

fail:
	mbedtls_pk_free(kctx);
	os_free(kctx);
	return NULL;
}


unsigned int crypto_ec_get_mbedtls_to_nist_group_id(int id)
{
	unsigned int nist_grpid = 0;
	switch (id) {
		case MBEDTLS_ECP_DP_SECP256R1:
			nist_grpid = 19;
			break;
		case MBEDTLS_ECP_DP_SECP384R1:
			nist_grpid = 20;
			break;
		case MBEDTLS_ECP_DP_SECP521R1:
			nist_grpid = 21;
			break;
		case MBEDTLS_ECP_DP_BP256R1:
			nist_grpid = 28;
			break;
		case MBEDTLS_ECP_DP_BP384R1:
			nist_grpid = 29;
			break;
		case MBEDTLS_ECP_DP_BP512R1:
			nist_grpid = 30;
			break;
		default:
			break;
	}

	return nist_grpid;
}


int crypto_ec_get_curve_id(const struct crypto_ec_group *group)
{
	mbedtls_ecp_group *grp = (mbedtls_ecp_group *)group;
	return (crypto_ec_get_mbedtls_to_nist_group_id(grp->id));
}


int crypto_ecdh(struct crypto_key *key_own, struct crypto_key *key_peer,
		u8 *secret, size_t *secret_len)
{
	mbedtls_ecdh_context *ctx;
	mbedtls_pk_context *own = (mbedtls_pk_context *)key_own;
	mbedtls_pk_context *peer = (mbedtls_pk_context *)key_peer;

	int ret = -1;

	*secret_len = 0;
	ctx = os_malloc(sizeof(*ctx));
	if (!ctx) {
		wpa_printf(MSG_ERROR, "DPP: EVP_PKEY_CTX_new failed: %s",
				__func__);
		return -1;
	}

	mbedtls_ecdh_init(ctx);
	/* No need to setup, done through mbedtls_ecdh_get_params */

	/* set params from our key */
	if (mbedtls_ecdh_get_params(ctx, mbedtls_pk_ec(*own), MBEDTLS_ECDH_OURS) < 0) {
		wpa_printf(MSG_ERROR, "failed to set our ecdh params\n");
		goto fail;
	}

#ifndef DPP_MAX_SHARED_SECRET_LEN
#define DPP_MAX_SHARED_SECRET_LEN 66
#endif
	/* set params from peers key */
	if (mbedtls_ecdh_get_params(ctx, mbedtls_pk_ec(*peer), MBEDTLS_ECDH_THEIRS) < 0) {
		wpa_printf(MSG_ERROR, "failed to set peer's ecdh params\n");
		goto fail;
	}

	if (mbedtls_ecdh_calc_secret(ctx, secret_len, secret, DPP_MAX_SHARED_SECRET_LEN, NULL, NULL) < 0) {
		wpa_printf(MSG_ERROR, "failed to calculate secret\n");
		goto fail;
	}

	if (*secret_len > DPP_MAX_SHARED_SECRET_LEN) {
		wpa_printf(MSG_ERROR, "secret len=%d is too big\n", *secret_len);
		goto fail;
	}

	ret = 0;

fail:
	mbedtls_ecdh_free(ctx);
	os_free(ctx);
	return ret;
}


int crypto_ecdsa_get_sign(unsigned char *hash,
		const struct crypto_bignum *r, const struct crypto_bignum *s, struct crypto_key *csign, int hash_len)
{
	int ret = -1;
	mbedtls_pk_context *pkey = (mbedtls_pk_context *)csign;

	mbedtls_ecdsa_context *ctx = os_malloc(sizeof(*ctx));
	if (!ctx) {
		wpa_printf(MSG_ERROR,"failed to allcate memory\n");
		return -1;
	}
	mbedtls_ecdsa_init(ctx);

	if (mbedtls_ecdsa_from_keypair(ctx, mbedtls_pk_ec(*pkey)) < 0) {
		goto fail;
	}
	ret = mbedtls_ecdsa_sign(&ctx->MBEDTLS_PRIVATE(grp), (mbedtls_mpi *)r, (mbedtls_mpi *)s,
			&ctx->MBEDTLS_PRIVATE(d), hash, SHA256_MAC_LEN, crypto_rng_wrapper, NULL);

fail:
	mbedtls_ecdsa_free(ctx);
	os_free(ctx);

	return  ret;
}

int crypto_edcsa_sign_verify(const unsigned char *hash,
		const struct crypto_bignum *r, const struct crypto_bignum *s, struct crypto_key *csign, int hlen)
{
	mbedtls_pk_context *pkey = (mbedtls_pk_context *)csign;
	int ret = 0;

	mbedtls_ecdsa_context *ctx = os_malloc(sizeof(*ctx));
	if (!ctx) {
		wpa_printf(MSG_ERROR, "failed to allcate memory\n");
		return ret;
	}
	mbedtls_ecdsa_init(ctx);

	if (mbedtls_ecdsa_from_keypair(ctx, mbedtls_pk_ec(*pkey)) < 0)
		return ret;

	if((ret = mbedtls_ecdsa_verify(&ctx->MBEDTLS_PRIVATE(grp), hash, hlen,
					&ctx->MBEDTLS_PRIVATE(Q), (mbedtls_mpi *)r, (mbedtls_mpi *)s)) != 0){
		wpa_printf(MSG_ERROR, "ecdsa verification failed\n");
		return ret;
	}

	mbedtls_ecdsa_free(ctx);
	os_free(ctx);

	return ret;
}

void crypto_debug_print_ec_key(const char *title, struct crypto_key *key)
{
#ifdef DEBUG_PRINT
	mbedtls_pk_context *pkey = (mbedtls_pk_context *)key;
	mbedtls_ecp_keypair *ecp = mbedtls_pk_ec( *pkey );
	u8 x[32], y[32], d[32];
	wpa_printf(MSG_ERROR, "curve: %s\n",
			mbedtls_ecp_curve_info_from_grp_id( ecp->MBEDTLS_PRIVATE(grp).id )->name );
	int len = mbedtls_mpi_size((mbedtls_mpi *)crypto_ec_get_prime((struct crypto_ec *)crypto_ec_get_group_from_key(key)));

	wpa_printf(MSG_ERROR, "prime len is %d\n", len);
	crypto_ec_point_to_bin((struct crypto_ec *)crypto_ec_get_group_from_key(key), crypto_ec_get_public_key(key), x, y);
	crypto_bignum_to_bin(crypto_ec_get_private_key(key),
			d, len, len);
	wpa_hexdump(MSG_ERROR, "Q_x:", x, 32);
	wpa_hexdump(MSG_ERROR, "Q_y:", y, 32);
	wpa_hexdump(MSG_ERROR, "d:     ",  d , 32);
#endif
}


struct crypto_key *crypto_ec_parse_subpub_key(const unsigned char *p, size_t len)
{
	int ret;
	mbedtls_pk_context *pkey = (mbedtls_pk_context *)crypto_alloc_key();
	ret = mbedtls_pk_parse_subpubkey((unsigned char **)&p, p + len, pkey);

	if (ret < 0) {
		os_free(pkey);
		return NULL;
	}

	return (struct crypto_key *)pkey;
}


int crypto_is_ec_key(struct crypto_key *key)
{
	int ret = mbedtls_pk_can_do((mbedtls_pk_context *)key, MBEDTLS_PK_ECKEY);
	return  ret;
}


struct crypto_key *crypto_ec_gen_keypair(u16 ike_group)
{
	mbedtls_pk_context *kctx = (mbedtls_pk_context *)crypto_alloc_key();

	if (!kctx) {
		wpa_printf(MSG_ERROR, "%s: memory allocation failed\n", __func__);
		return NULL;
	}

	if(mbedtls_pk_setup(kctx,
				mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY)) != 0 )
		goto fail;

	mbedtls_ecp_gen_key(MBEDTLS_ECP_DP_SECP256R1, mbedtls_pk_ec(*kctx), //get this from argument
			crypto_rng_wrapper, NULL);

	return (struct crypto_key *)kctx;
fail:
	mbedtls_pk_free(kctx);
	os_free(kctx);
	return NULL;
}


/*
 * ECParameters ::= CHOICE {
 *   namedCurve         OBJECT IDENTIFIER
 * }
 */
static int pk_write_ec_param( unsigned char **p, unsigned char *start,
		mbedtls_ecp_keypair *ec )
{
	int ret;
	size_t len = 0;
	const char *oid;
	size_t oid_len;

	if( ( ret = mbedtls_oid_get_oid_by_ec_grp( ec->MBEDTLS_PRIVATE(grp).id, &oid, &oid_len ) ) != 0 )
		return( ret );

	MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_oid( p, start, oid, oid_len ) );

	return( (int) len );
}


static int pk_write_ec_pubkey_formatted( unsigned char **p, unsigned char *start,
		mbedtls_ecp_keypair *ec, int format )
{
	int ret;
	size_t len = 0;
	unsigned char buf[MBEDTLS_ECP_MAX_PT_LEN];

	if( ( ret = mbedtls_ecp_point_write_binary( &ec->MBEDTLS_PRIVATE(grp), &ec->MBEDTLS_PRIVATE(Q),
					format,
					&len, buf, sizeof( buf ) ) ) != 0 )
	{
		return( ret );
	}

	if( *p < start || (size_t)( *p - start ) < len )
		return( MBEDTLS_ERR_ASN1_BUF_TOO_SMALL );

	*p -= len;
	memcpy( *p, buf, len );

	return( (int) len );
}


int mbedtls_pk_write_pubkey_formatted( unsigned char **p, unsigned char *start,
		const mbedtls_pk_context *key, int format )
{
	int ret;
	size_t len = 0;

	if( mbedtls_pk_get_type( key ) == MBEDTLS_PK_ECKEY )
		MBEDTLS_ASN1_CHK_ADD( len, pk_write_ec_pubkey_formatted( p, start, mbedtls_pk_ec( *key ), format ) );
	else
		return( MBEDTLS_ERR_PK_FEATURE_UNAVAILABLE );

	return( (int) len );
}


int crypto_pk_write_formatted_pubkey_der(mbedtls_pk_context *key, unsigned char *buf, size_t size, int format)
{
	int ret;
	unsigned char *c;
	size_t len = 0, par_len = 0, oid_len;
	const char *oid;

	if( size == 0 )
		return( MBEDTLS_ERR_ASN1_BUF_TOO_SMALL );

	c = buf + size;

	MBEDTLS_ASN1_CHK_ADD( len, mbedtls_pk_write_pubkey_formatted( &c, buf, key, format) );

	if( c - buf < 1 )
		return( MBEDTLS_ERR_ASN1_BUF_TOO_SMALL );

	/*
	 *  SubjectPublicKeyInfo  ::=  SEQUENCE  {
	 *       algorithm            AlgorithmIdentifier,
	 *       subjectPublicKey     BIT STRING }
	 */
	*--c = 0;
	len += 1;


	MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_len( &c, buf, len ) );
	MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_tag( &c, buf, MBEDTLS_ASN1_BIT_STRING ) );

	if( ( ret = mbedtls_oid_get_oid_by_pk_alg( mbedtls_pk_get_type( key ),
					&oid, &oid_len ) ) != 0 )
	{
		return( ret );
	}

	if( mbedtls_pk_get_type( key ) == MBEDTLS_PK_ECKEY )
	{
		MBEDTLS_ASN1_CHK_ADD( par_len, pk_write_ec_param( &c, buf, mbedtls_pk_ec( *key ) ) );
	}

	MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_algorithm_identifier( &c, buf, oid, oid_len,
				par_len ) );

	MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_len( &c, buf, len ) );
	MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_tag( &c, buf, MBEDTLS_ASN1_CONSTRUCTED |
				MBEDTLS_ASN1_SEQUENCE ) );

	return( (int) len );
}


int crypto_ec_write_pub_key(struct crypto_key *key, unsigned char **key_buf)
{
	unsigned char output_buf[1600] = {0};
	int len = crypto_pk_write_formatted_pubkey_der((mbedtls_pk_context *)key, output_buf, 1600, 1);
	if (len <= 0)
		return 0;

	*key_buf = os_malloc(len);
	if (!*key_buf) {
		wpa_printf(MSG_ERROR, "%s: memory allocation failed\n", __func__);
		return 0;
	}
	os_memcpy(*key_buf, output_buf + 1600 - len, len);

	return len;
}

#define random_get_bytes(b, l) os_get_random((b), (l))

static int crypto_ecdh_get_random(void *arg, unsigned char *output,
                             size_t output_len)
{
	random_get_bytes(output, output_len);
	return 0;
}

struct crypto_ecdh *crypto_ecdh_init(int group)
{
	struct crypto_ecdh *ecdh;
	uint8_t buf[128];
	size_t olen;
	mbedtls_ecp_group_id grp_id = MBEDTLS_ECP_DP_NONE;
#ifdef USE_MBEDTLS_RANDOM
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
#endif
	/* IANA registry to mbedtls internal mapping*/
	switch (group) {
	case 19:
		grp_id = MBEDTLS_ECP_DP_SECP256R1;
		break;
	case 20:
		grp_id = MBEDTLS_ECP_DP_SECP384R1;
		break;
#ifdef CONFIG_FULL_SUPPLICANT
	case 21:
		grp_id = MBEDTLS_ECP_DP_SECP521R1;	/* tooooo slow for no-hw */
		break;
	case 25:
		grp_id = MBEDTLS_ECP_DP_SECP192R1;
		break;
	case 26:
		grp_id = MBEDTLS_ECP_DP_SECP224R1;
		break;
#endif
	default:
		return NULL;
	}

#ifdef USE_MBEDTLS_RANDOM
	//INIT RND
	mbedtls_entropy_init(&entropy);
	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
#endif

	//INIT ECDH
	ecdh = os_zalloc(sizeof(*ecdh));
	if (!ecdh)
		goto clean;

#ifdef MBEDTLS_ECDH_LEGACY_CONTEXT
	mbedtls_ecdh_init(&ecdh->ctx);

	if (mbedtls_ecp_group_load_ext(&ecdh->ctx.grp, grp_id)) {
		wpa_printf(MSG_ERROR, "OWE: Fail to setup ECDH");
		crypto_ecdh_deinit(ecdh);
		ecdh = NULL;
		goto clean;
	}
#else
	/* XXX: grp 19,20,21 doesn't contain A param */
	if (mbedtls_ecdh_setup(&ecdh->ctx, grp_id)) {
		wpa_printf(MSG_ERROR, "OWE: Fail to setup ECDH");
		crypto_ecdh_deinit(ecdh);
		ecdh = NULL;
		goto clean;
	}
#endif

	if (mbedtls_ecdh_make_public(&ecdh->ctx, &olen, buf, sizeof(buf),
#ifdef USE_MBEDTLS_RANDOM
								 mbedtls_ctr_drbg_random, &ctr_drbg
#else
								 crypto_ecdh_get_random, NULL
#endif
								 ) != 0) {
		wpa_printf(MSG_ERROR, "OWE: Fail to make public key");
		crypto_ecdh_deinit(ecdh);
		ecdh = NULL;
		goto clean;
	}

	// wpa_hexdump(MSG_DEBUG, "PUBLIC BUF", buf, olen);
	// bignum_print("PUBLIC RES-X : ", &ecdh->ctx.Q.X);
	// bignum_print("PUBLIC RES-Y : ", &ecdh->ctx.Q.Y);
	// bignum_print("PUBLIC RES-Z : ", &ecdh->ctx.Q.Z);

clean:
#ifdef USE_MBEDTLS_RANDOM
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
#endif

	return ecdh;
}


void crypto_ecdh_deinit(struct crypto_ecdh *ecdh)
{
	if (ecdh) {
		mbedtls_ecdh_free(&ecdh->ctx);
		os_free(ecdh);
	}
}


struct wpabuf *crypto_ecdh_get_pubkey(struct crypto_ecdh *ecdh, int inc_y)
{
	struct wpabuf *buf = NULL;
#define CRYPTO_EC_plen(e) ((((mbedtls_ecp_group *)(e))->pbits + 7) >> 3)
#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
	mbedtls_ecp_group *grp = &ecdh->ctx.grp;
#else
	mbedtls_ecp_group *grp = &ecdh->ctx.MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(grp);
#endif
	int len = CRYPTO_EC_plen(grp);  //ecdh->ctx.Q.X.n * 2;

	int ret;

	buf = wpabuf_alloc(len);
	if (!buf)
		return NULL;

#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
	ret = mbedtls_mpi_write_binary(&ecdh->ctx.Q.X, wpabuf_put(buf, len), len);
#else
	ret = mbedtls_mpi_write_binary(&ecdh->ctx.MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(X), wpabuf_put(buf, len), len);
#endif
	if (ret != 0) {
		wpa_printf(MSG_ERROR, "OWE: Fail to get public-key");
		wpabuf_free(buf);
		return NULL;
	}

	// wpa_printf(MSG_DEBUG, "OWE: ECDH key length %d", len);
	// bignum_print("Public Bignum", &ecdh->ctx.Q.X);
	wpa_hexdump(MSG_DEBUG, "OWE: Public Key", wpabuf_head(buf), wpabuf_len(buf));

	return buf;
}

struct wpabuf *crypto_ecdh_set_peerkey(struct crypto_ecdh *ecdh, int inc_y,
									   const u8 *key, size_t len)
{
	struct wpabuf *secret = NULL;
	unsigned char res_buf[128];
	size_t res_len;

	mbedtls_ecp_point *peerKey;
#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
	peerKey = &ecdh->ctx.Qp;
#else
	peerKey = &ecdh->ctx.MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(Qp);
#endif

	if (mbedtls_mpi_read_binary(&peerKey->MBEDTLS_PRIVATE(X), key, len) != 0) {
		wpa_printf(MSG_ERROR, "OWE: Fail to read peer public-key(point X)");
		goto clean;
	}

	if (mbedtls_mpi_lset(&peerKey->MBEDTLS_PRIVATE(Z), 1) != 0) {
		wpa_printf(MSG_ERROR, "OWE: Fail to Set bit in public-key(point Z)");
		goto clean;
	}

#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
	if (crypto_ec_point_solve_y_coord((void *)ecdh, (void *)peerKey, (void *)&peerKey->MBEDTLS_PRIVATE(X), 0)) {
		wpa_printf(MSG_ERROR, "OWE: Fail to calc Y using X(y^2 = x^3 + ax + b mod P)");
		goto clean;
	}
#else
	if (crypto_ec_point_solve_y_coord((void *)&ecdh->ctx.MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(grp),
		(void *)peerKey, (void *)&peerKey->MBEDTLS_PRIVATE(X), 0)) {
		wpa_printf(MSG_ERROR, "OWE: Fail to calc Y using X(y^2 = x^3 + ax + b mod P)");
		goto clean;
	}
#endif
	// bignum_print("Public  Key : ", &ecdh->ctx.Q.X);
	// bignum_print("Private Key : ", &ecdh->ctx.d);
	// bignum_print("pr Public X Key : ", &ecdh->ctx.Qp.X);
	// bignum_print("pr Public Y Key : ", &ecdh->ctx.Qp.Y);
	// bignum_print("pr Public Z Key : ", &ecdh->ctx.Qp.Z);

	if (mbedtls_ecdh_calc_secret(&ecdh->ctx, &res_len, res_buf, sizeof(res_buf),
								 NULL, NULL) != 0) {
		wpa_printf(MSG_ERROR, "OWE: Fail to calc secret");
		goto clean;
	}

	wpa_hexdump(MSG_DEBUG, "OWE: SHARED KEY", res_buf, res_len);

	ecdh->length = res_len;

	secret = wpabuf_alloc(ecdh->length);
	if (!secret)
		return NULL;

	// mbedtls_mpi_write_binary(&ecdh->ctx.z, secret->buf, ecdh->length);
	os_memcpy(wpabuf_put(secret, ecdh->length), res_buf, ecdh->length);

clean:

	return secret;
}
#endif



struct crypto_cipher {
	mbedtls_cipher_context_t ctx_enc;
	mbedtls_cipher_context_t ctx_dec;
};

static int crypto_init_cipher_ctx(mbedtls_cipher_context_t *ctx,
				  const mbedtls_cipher_info_t *cipher_info,
				  const u8 *iv, const u8 *key,
				  mbedtls_operation_t operation)
{
	mbedtls_cipher_init(ctx);
	int ret;

	ret = mbedtls_cipher_setup(ctx, cipher_info);
	if (ret != 0) {
		return -1;
	}

	if (mbedtls_cipher_setkey(ctx, key, cipher_info->MBEDTLS_PRIVATE(key_bitlen),
				 operation) != 0) {
		wpa_printf(MSG_ERROR, "mbedtls_cipher_setkey returned error");
		return -1;
	}
	if (mbedtls_cipher_set_iv(ctx, iv, cipher_info->MBEDTLS_PRIVATE(iv_size)) != 0) {
		wpa_printf(MSG_ERROR, "mbedtls_cipher_set_iv returned error");
		return -1;
	}
	if (mbedtls_cipher_reset(ctx) != 0) {
		wpa_printf(MSG_ERROR, "mbedtls_cipher_reset() returned error");
		return -1;
	}

	return 0;
}

static mbedtls_cipher_type_t alg_to_mbedtls_cipher(enum crypto_cipher_alg alg,
						   size_t key_len)
{
	switch (alg) {
#ifdef MBEDTLS_ARC4_C
	case CRYPTO_CIPHER_ALG_RC4:
		return MBEDTLS_CIPHER_ARC4_128;
#endif
	case CRYPTO_CIPHER_ALG_AES:
		if (key_len == 16) {
			return MBEDTLS_CIPHER_AES_128_CBC;
		}
		if (key_len == 24) {
			return MBEDTLS_CIPHER_AES_192_CBC;
		}
		if (key_len == 32) {
			return MBEDTLS_CIPHER_AES_256_CBC;
		}
		break;
#ifdef MBEDTLS_DES_C
	case CRYPTO_CIPHER_ALG_3DES:
		return MBEDTLS_CIPHER_DES_EDE3_CBC;
	case CRYPTO_CIPHER_ALG_DES:
		return MBEDTLS_CIPHER_DES_CBC;
#endif
	default:
		break;
	}

	return MBEDTLS_CIPHER_NONE;
}

struct crypto_cipher *crypto_cipher_init(enum crypto_cipher_alg alg,
					 const u8 *iv, const u8 *key,
					 size_t key_len)
{
	struct crypto_cipher *ctx;
	mbedtls_cipher_type_t cipher_type;
	const mbedtls_cipher_info_t *cipher_info;

	ctx = (struct crypto_cipher *)os_zalloc(sizeof(*ctx));
	if (!ctx) {
		return NULL;
	}

	cipher_type = alg_to_mbedtls_cipher(alg, key_len);
	if (cipher_type == MBEDTLS_CIPHER_NONE) {
		goto cleanup;
	}

	cipher_info = mbedtls_cipher_info_from_type(cipher_type);
	if (cipher_info == NULL) {
		goto cleanup;
	}

	/* Init both ctx encryption/decryption */
	if (crypto_init_cipher_ctx(&ctx->ctx_enc, cipher_info, iv, key,
				   MBEDTLS_ENCRYPT) < 0) {
		goto cleanup;
	}

	if (crypto_init_cipher_ctx(&ctx->ctx_dec, cipher_info, iv, key,
				   MBEDTLS_DECRYPT) < 0) {
		goto cleanup;
	}

	return ctx;

cleanup:
	os_free(ctx);
	return NULL;
}

int crypto_cipher_encrypt(struct crypto_cipher *ctx, const u8 *plain,
			  u8 *crypt, size_t len)
{
	int ret;
	size_t olen;

	ret = mbedtls_cipher_update(&ctx->ctx_enc, plain, len, crypt, &olen);
	if (ret != 0) {
		return -1;
	}

	ret = mbedtls_cipher_finish(&ctx->ctx_enc, crypt + olen, &olen);
	if (ret != 0) {
		return -1;
	}

	return 0;
}

int crypto_cipher_decrypt(struct crypto_cipher *ctx, const u8 *crypt,
			  u8 *plain, size_t len)
{
	int ret;
	size_t olen;

	ret = mbedtls_cipher_update(&ctx->ctx_dec, crypt, len, plain, &olen);
	if (ret != 0) {
		return -1;
	}

	ret = mbedtls_cipher_finish(&ctx->ctx_dec, plain + olen, &olen);
	if (ret != 0) {
		return -1;
	}

	return 0;
}

void crypto_cipher_deinit(struct crypto_cipher *ctx)
{
	mbedtls_cipher_free(&ctx->ctx_enc);
	mbedtls_cipher_free(&ctx->ctx_dec);
	os_free(ctx);
}

int aes_ctr_encrypt(const u8 *key, size_t key_len, const u8 *nonce,
		    u8 *data, size_t data_len)
{
	int ret;
	mbedtls_aes_context ctx;
	uint8_t stream_block[MBEDTLS_AES_BLOCK_SIZE];
	size_t offset = 0;

	mbedtls_aes_init(&ctx);
	ret = mbedtls_aes_setkey_enc(&ctx, key, key_len * 8);
	if (ret < 0) {
		goto cleanup;
	}
	ret = mbedtls_aes_crypt_ctr(&ctx, data_len, &offset, (u8 *)nonce,
				    stream_block, data, data);
cleanup:
	mbedtls_aes_free(&ctx);
	return ret;
}

int aes_128_ctr_encrypt(const u8 *key, const u8 *nonce,
			u8 *data, size_t data_len)
{
	return aes_ctr_encrypt(key, 16, nonce, data, data_len);
}


#ifdef MBEDTLS_NIST_KW_C
#ifndef CONFIG_MBEDTLS_ACCELERATOR
int aes_wrap(const u8 *kek, size_t kek_len, int n, const u8 *plain, u8 *cipher)
{
	mbedtls_nist_kw_context ctx;
	size_t olen;
	int ret = 0;
	mbedtls_nist_kw_init(&ctx);

	ret = mbedtls_nist_kw_setkey(&ctx, MBEDTLS_CIPHER_ID_AES,
			kek, kek_len * 8, 1);
	if (ret != 0) {
		return ret;
	}

	ret = mbedtls_nist_kw_wrap(&ctx, MBEDTLS_KW_MODE_KW, plain,
			n * 8, cipher, &olen, (n + 1) * 8);

	mbedtls_nist_kw_free(&ctx);
	return ret;
}

int aes_unwrap(const u8 *kek, size_t kek_len, int n, const u8 *cipher,
	       u8 *plain)
{
	mbedtls_nist_kw_context ctx;
	size_t olen;
	int ret = 0;
	mbedtls_nist_kw_init(&ctx);

	ret = mbedtls_nist_kw_setkey(&ctx, MBEDTLS_CIPHER_ID_AES,
			kek, kek_len * 8, 0);
	if (ret != 0) {
		return ret;
	}

	ret = mbedtls_nist_kw_unwrap(&ctx, MBEDTLS_KW_MODE_KW, cipher,
			(n + 1) * 8, plain, &olen, (n * 8));

	mbedtls_nist_kw_free(&ctx);
	return ret;
}
#endif
#endif


#if 0
int pbkdf2_sha1(const char *passphrase, const u8 *ssid, size_t ssid_len,
		int iterations, u8 *buf, size_t buflen)
{

	mbedtls_md_context_t sha1_ctx;
	const mbedtls_md_info_t *info_sha1;
	int ret;

	mbedtls_md_init(&sha1_ctx);

	info_sha1 = mbedtls_md_info_from_type(MBEDTLS_MD_SHA1);
	if (info_sha1 == NULL) {
		ret = -1;
		goto cleanup;
	}

	if ((ret = mbedtls_md_setup(&sha1_ctx, info_sha1, 1)) != 0) {
		ret = -1;
		goto cleanup;
	}

	ret = mbedtls_pkcs5_pbkdf2_hmac(&sha1_ctx, (const u8 *) passphrase,
					os_strlen(passphrase) , ssid,
					ssid_len, iterations, 32, buf);
	if (ret != 0) {
		ret = -1;
		goto cleanup;
	}

cleanup:
	mbedtls_md_free(&sha1_ctx);
	return ret;
}
#endif


#ifdef MBEDTLS_DES_C
int des_encrypt(const u8 *clear, const u8 *key, u8 *cypher)
{
	int ret;
	mbedtls_des_context des;
	u8 pkey[8], next, tmp;
	int i;

	/* Add parity bits to the key */
	next = 0;
	for (i = 0; i < 7; i++) {
		tmp = key[i];
		pkey[i] = (tmp >> i) | next | 1;
		next = tmp << (7 - i);
	}
	pkey[i] = next | 1;

	mbedtls_des_init(&des);
	ret = mbedtls_des_setkey_enc(&des, pkey);
	if (ret < 0) {
		return ret;
	}
	ret = mbedtls_des_crypt_ecb(&des, clear, cypher);
	mbedtls_des_free(&des);

	return ret;
}
#endif

/* Only enable this if all other ciphers are using MbedTLS implementation */
#if defined(MBEDTLS_CCM_C) && defined(MBEDTLS_CMAC_C) && defined(MBEDTLS_NIST_KW_C)
int aes_ccm_ae(const u8 *key, size_t key_len, const u8 *nonce,
	       size_t M, const u8 *plain, size_t plain_len,
	       const u8 *aad, size_t aad_len, u8 *crypt, u8 *auth)
{
	int ret;
	mbedtls_ccm_context ccm;

	mbedtls_ccm_init(&ccm);

	ret = mbedtls_ccm_setkey(&ccm, MBEDTLS_CIPHER_ID_AES,
				 key, key_len * 8);
	if (ret < 0) {
		wpa_printf(MSG_ERROR, "mbedtls_ccm_setkey failed");
		goto cleanup;
	}

	ret = mbedtls_ccm_encrypt_and_tag(&ccm, plain_len, nonce, 13, aad,
					  aad_len, plain, crypt, auth, M);

cleanup:
	mbedtls_ccm_free(&ccm);

	return ret;
}

int aes_ccm_ad(const u8 *key, size_t key_len, const u8 *nonce,
	       size_t M, const u8 *crypt, size_t crypt_len,
	       const u8 *aad, size_t aad_len, const u8 *auth,
	       u8 *plain)
{
	int ret;
	mbedtls_ccm_context ccm;

	mbedtls_ccm_init(&ccm);

	ret = mbedtls_ccm_setkey(&ccm, MBEDTLS_CIPHER_ID_AES,
				 key, key_len * 8);
	if (ret < 0) {
		goto cleanup;;
	}

	ret = mbedtls_ccm_star_auth_decrypt(&ccm, crypt_len,
					    nonce, 13, aad, aad_len,
					    crypt, plain, auth, M);

cleanup:
	mbedtls_ccm_free(&ccm);

	return ret;
}
#endif

#ifdef MBEDTLS_ARC4_C
#include "mbedtls/arc4.h"
int rc4_skip(const u8 *key, size_t keylen, size_t skip,
             u8 *data, size_t data_len)
{
	int ret;
	unsigned char skip_buf_in[16];
	unsigned char skip_buf_out[16];
	mbedtls_arc4_context ctx;
	unsigned char *obuf = os_malloc(data_len);

	if (!obuf) {
		wpa_printf(MSG_ERROR, "%s:memory allocation failed", __func__);
		return -1;
	}
	mbedtls_arc4_init(&ctx);
	mbedtls_arc4_setup(&ctx, key, keylen);
	while (skip >= sizeof(skip_buf_in)) {
		size_t len = skip;
		if (len > sizeof(skip_buf_in)) {
			len = sizeof(skip_buf_in);
		}
		if ((ret = mbedtls_arc4_crypt(&ctx, len, skip_buf_in,
					      skip_buf_out)) != 0) {
			wpa_printf(MSG_ERROR, "rc4 encryption failed");
			return -1;
		}
		os_memcpy(skip_buf_in, skip_buf_out, 16);
		skip -= len;
	}

	mbedtls_arc4_crypt(&ctx, data_len, data, obuf);

	memcpy(data, obuf, data_len);
	os_free(obuf);

	return 0;
}
#else
/* (RC4 not in mbedtls 3.x) */
#include "rc4.c"         /* pull in hostap local implementation */
#endif


int crypto_global_init(void)
{
	return 0;
}


void crypto_global_deinit(void)
{
}

