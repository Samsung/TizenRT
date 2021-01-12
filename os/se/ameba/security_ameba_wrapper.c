/**************************************************************************************************
*
* Copyright 2020 Samsung Electronics All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
* either express or implied. See the License for the specific
* language governing permissions and limitations under the License.
*
***************************************************************************************************/

#include <sys/types.h>
#include <string.h>
#include <stdint.h>

#include <tinyara/config.h>
#include <semaphore.h>
#include <tinyara/seclink_drv.h>
#include <tinyara/security_hal.h>
#include <tinyara/kmalloc.h>

#include <rom_ssl_ram_map.h>

#define AWRAP_TAG "[AMEBA_WRAPPER]"
#define AWRAP_ENTER                                     \
	do {                                                \
		sedbg(AWRAP_TAG"%s:%d\n", __FILE__, __LINE__);  \
	} while (0)

#define HAL_COPY_DATA(in, out, len)                     \
	do {                                                \
		if (in->data == NULL) {                         \
			return HAL_INVALID_ARGS;                    \
		}                                               \
		memcpy(in->data, out, len);                     \
		in->data_len = len;                             \
	} while (0)

#define HAL_COPY_PRIV_DATA(in, out, len)                \
	do {                                                \
		if (in->priv == NULL) {                         \
			return HAL_INVALID_ARGS;                    \
		}                                               \
		memcpy(in->priv, out, len);                     \
		in->priv_len = len;                             \
	} while (0)

#define HAL_MAX_RANDOM_SIZE 256
#define HAL_MAX_BUF_SIZE 2048
#define HAL_MAX_SHARED_SECRET_LEN 256
#define KEY_STORAGE_INDEX_MAX 32

#define AES_128_KEY_SIZE 16
#define AES_192_KEY_SIZE 24
#define AES_256_KEY_SIZE 32
#define RTL_HASH_SHA224 1
#define RTL_HASH_SHA256 0
#define RTL_HASH_SHA384 1
#define RTL_HASH_SHA512 0

/* Function return Enum define */
#define RTL_HAL_SUCCESS 0

/* Factory Key and Cert define */
#define FACTORYKEY_RTL_DEVICE (0x00010120)
#define FACTORYKEY_RTL_CERT (0x00010122)

/* ROM mbedtls define */
#define RTL_ECP_MAX_BITS    521     /**< Maximum bit size of groups */
#define RTL_ECP_MAX_BYTES   ((RTL_ECP_MAX_BITS + 7) / 8)
#define RTL_ECDSA_MAX_LEN   (3 + 2 * (3 + RTL_ECP_MAX_BYTES))

#define RTL_RSA_PUBLIC      0       /**< Request private key operation. */
#define RTL_RSA_PRIVATE     1       /**< Request public key operation. */
#define RTL_RSA_PKCS_V15    0       /**< Use PKCS-1 v1.5 encoding. */
#define RTL_RSA_PKCS_V21    1       /**< Use PKCS-1 v2.1 encoding. */

typedef enum {
	RTL_ECP_DP_NONE = 0,
	RTL_ECP_DP_SECP192R1,           /*!< 192-bits NIST curve  */
	RTL_ECP_DP_SECP224R1,           /*!< 224-bits NIST curve  */
	RTL_ECP_DP_SECP256R1,           /*!< 256-bits NIST curve  */
	RTL_ECP_DP_SECP384R1,           /*!< 384-bits NIST curve  */
	RTL_ECP_DP_SECP521R1,           /*!< 521-bits NIST curve  */
	RTL_ECP_DP_BP256R1,             /*!< 256-bits Brainpool curve */
	RTL_ECP_DP_BP384R1,             /*!< 384-bits Brainpool curve */
	RTL_ECP_DP_BP512R1,             /*!< 512-bits Brainpool curve */
	RTL_ECP_DP_CURVE25519,          /*!< Curve25519               */
	RTL_ECP_DP_SECP192K1,           /*!< 192-bits "Koblitz" curve */
	RTL_ECP_DP_SECP224K1,           /*!< 224-bits "Koblitz" curve */
	RTL_ECP_DP_SECP256K1,           /*!< 256-bits "Koblitz" curve */
} rtl_ecp_group_id;

typedef enum {
	RTL_MD_NONE = 0,
	RTL_MD_MD2,
	RTL_MD_MD4,
	RTL_MD_MD5,
	RTL_MD_SHA1,
	RTL_MD_SHA224,
	RTL_MD_SHA256,
	RTL_MD_SHA384,
	RTL_MD_SHA512,
	RTL_MD_RIPEMD160,
} rtl_md_type_t;

typedef enum {
	RTL_OPERATION_NONE = -1,
	RTL_DECRYPT = 0,
	RTL_ENCRYPT,
} rtl_operation_t;

typedef uint32_t rtl_mpi_uint;

typedef struct {
	int s;                              /*!<  integer sign      */
	size_t n;                           /*!<  total # of limbs  */
	rtl_mpi_uint *p;                    /*!<  pointer to limbs  */
} rtl_mpi;

typedef struct {
	int ver;                            /*!<  always 0          */
	size_t len;                         /*!<  size(N) in chars  */

	rtl_mpi N;                          /*!<  public modulus    */
	rtl_mpi E;                          /*!<  public exponent   */

	rtl_mpi D;                          /*!<  private exponent  */
	rtl_mpi P;                          /*!<  1st prime factor  */
	rtl_mpi Q;                          /*!<  2nd prime factor  */
	rtl_mpi DP;                         /*!<  D % (P - 1)       */
	rtl_mpi DQ;                         /*!<  D % (Q - 1)       */
	rtl_mpi QP;                         /*!<  1 / (Q % P)       */

	rtl_mpi RN;                         /*!<  cached R^2 mod N  */
	rtl_mpi RP;                         /*!<  cached R^2 mod P  */
	rtl_mpi RQ;                         /*!<  cached R^2 mod Q  */

	rtl_mpi Vi;                         /*!<  cached blinding value     */
	rtl_mpi Vf;                         /*!<  cached un-blinding value  */

	int padding;                        /*!<  RTL_RSA_PKCS_V15 for 1.5 padding and RTL_RSA_PKCS_V21 for OAEP/PSS */
	int hash_id;                        /*!<  Hash identifier of rtl_md_type_t as specified in the mbedtls_md.h header file for the EME-OAEP and EMSA-PSS encoding */
} rtl_rsa_context;

typedef struct {
	rtl_mpi X;                          /*!<  the point's X coordinate  */
	rtl_mpi Y;                          /*!<  the point's Y coordinate  */
	rtl_mpi Z;                          /*!<  the point's Z coordinate  */
} rtl_ecp_point;

typedef struct {
	rtl_ecp_group_id id;                    /*!<  internal group identifier                     */
	rtl_mpi P;                              /*!<  prime modulus of the base field               */
	rtl_mpi A;                              /*!<  1. A in the equation, or 2. (A + 2) / 4       */
	rtl_mpi B;                              /*!<  1. B in the equation, or 2. unused            */
	rtl_ecp_point G;                        /*!<  generator of the (sub)group used              */
	rtl_mpi N;                              /*!<  1. the order of G, or 2. unused               */
	size_t pbits;                           /*!<  number of bits in P                           */
	size_t nbits;                           /*!<  number of bits in 1. P, or 2. private keys    */
	unsigned int h;                         /*!<  internal: 1 if the constants are static       */
	int (*modp)(rtl_mpi *);                 /*!<  function for fast reduction mod P             */
	int (*t_pre)(rtl_ecp_point *, void *);  /*!<  unused                         */
	int (*t_post)(rtl_ecp_point *, void *); /*!<  unused                         */
	void *t_data;                           /*!<  unused                         */
	rtl_ecp_point *T;                       /*!<  pre-computed points for ecp_mul_comb()        */
	size_t T_size;                          /*!<  number for pre-computed points                */
} rtl_ecp_group;

typedef struct {
	rtl_ecp_group grp;                  /*!< The elliptic curve used. */
	rtl_mpi d;                          /*!< The private key. */
	rtl_ecp_point Q;                    /*!< The public key. */
	rtl_ecp_point Qp;                   /*!< The value of the public key of the peer. */
	rtl_mpi z;                          /*!< The shared secret. */
	int point_format;                   /*!< The format of point export in TLS messages. */
	rtl_ecp_point Vi;                   /*!< The blinding value. */
	rtl_ecp_point Vf;                   /*!< The unblinding value. */
	rtl_mpi _d;                         /*!< The previous \p d. */
	int server_raw_public_key;          /*!< Use server Raw Public Key. */
	int client_raw_public_key;          /*!< Use client Raw Public Key. */
} rtl_ecdh_context;

typedef struct {
	rtl_ecp_group grp;              /*!<  Elliptic curve and base point     */
	rtl_mpi d;                      /*!<  our secret value                  */
	rtl_ecp_point Q;                /*!<  our public value                  */
} rtl_ecp_keypair;

typedef rtl_ecp_keypair rtl_ecdsa_context;

typedef struct {
	size_t len;                         /*!<  size(P) in chars  */
	rtl_mpi P;                      /*!<  prime modulus     */
	rtl_mpi G;                      /*!<  generator         */
	rtl_mpi X;                      /*!<  secret value      */
	rtl_mpi GX;                     /*!<  self = G^X mod P  */
	rtl_mpi GY;                     /*!<  peer = G^Y mod P  */
	rtl_mpi K;                      /*!<  key = GY^X mod P  */
	rtl_mpi RP;                     /*!<  cached R^2 mod P  */
	rtl_mpi Vi;                     /*!<  blinding value    */
	rtl_mpi Vf;                     /*!<  un-blinding value */
	rtl_mpi pX;                     /*!<  previous X        */
} rtl_dhm_context;

/* End of ROM mbedtls define */

typedef struct key_struc {
	hal_key_type key_type;

	/* AES Key must be 4Byte Aligned, due to hardware crypto */
	unsigned char aes_key[32] __attribute__((aligned(4)));
	uint32_t aes_key_len;

	rtl_rsa_context rsa_ctx;
	rtl_ecdh_context ecdh_ctx;
	rtl_ecdsa_context ecdsa_ctx;
	rtl_dhm_context dhm_ctx;

	unsigned char hmac_key[32];

	unsigned char *cert_data;
	uint32_t cert_len;
} key_struc;

static key_struc key_storage[KEY_STORAGE_INDEX_MAX] = { 0 };
static uint32_t init_stat = HAL_NOT_INITIALIZED;

/* Extern api from rtl_security_api_wrapper.c */
extern uint32_t rtl_cryptoEngine_init_wrapper(void);
extern uint32_t rtl_write_storage_wrapper(uint32_t ss_idx, hal_data *data);
extern uint32_t rtl_read_storage_wrapper(uint32_t ss_idx, hal_data *data);
extern uint32_t rtl_delete_storage_wrapper(uint32_t ss_idx);
extern uint32_t rtl_cryptoAES_ecb_wrapper(uint8_t *key, uint32_t keylen, unsigned char *message, uint32_t msglen, unsigned char *pResult, uint8_t mode);
extern uint32_t rtl_cryptoAES_cbc_wrapper(uint8_t *key, uint32_t keylen, unsigned char *message, uint32_t msglen, unsigned char *pResult, uint8_t mode);
extern uint32_t rtl_cryptoAES_ctr_wrapper(uint8_t *key, uint32_t keylen, unsigned char *message, uint32_t msglen, unsigned char *pResult, uint8_t mode);
extern uint32_t rtl_read_factory_key_wrapper(hal_data *data);
extern uint32_t rtl_read_factory_cert_wrapper(hal_data *data);

/**
 * Helper function
 */
static int rtl_chk_stat(void)
{
	if (init_stat == HAL_NOT_INITIALIZED) {
		return HAL_NOT_INITIALIZED;
	}

	return HAL_SUCCESS;
}

static void *my_calloc(size_t nelements, size_t elementSize)
{
	size_t size;
	void *ptr = NULL;

	size = nelements * elementSize;
	ptr = kmm_malloc(size);

	if (ptr)
		memset(ptr, 0, size);

	return ptr;
}

static void my_free(void *ptr)
{
	kmm_free(ptr);
}

static uint32_t rtl_index_convt(uint32_t key_idx)
{
	uint32_t ret_key_idx = KEY_STORAGE_INDEX_MAX;

	/* Because Ameba SE storing Key in array, array start from Zero */
	if (key_idx != 0) {	/* Index not Zero */
		/* Valid  Index are 0 - 31, Total 32 Index slot */
		ret_key_idx = key_idx - 1;	/* Index minus One */
	}
	return ret_key_idx;
}

/* Set prime modulus and generator */
static int rtl_dhm_set_group(rtl_dhm_context *ctx, const rtl_mpi *P, const rtl_mpi *G)
{
	int ret;

	if (ctx == NULL || P == NULL || G == NULL) {
		return HAL_FAIL;
	}

	if ((ret = rtl_mpi_copy(&ctx->P, P)) != 0 || (ret = rtl_mpi_copy(&ctx->G, G)) != 0) {
		return HAL_FAIL;
	}

	ctx->len = rtl_mpi_size(&ctx->P);
	return 0;
}

static int hal_rand(void *rng_state, unsigned char *output, size_t len)
{
	rtw_get_random_bytes(output, len);
	return HAL_SUCCESS;
}

/**
 * Common
 */

int ameba_hal_init(hal_init_param *params)
{
	AWRAP_ENTER;

	uint32_t ret = HAL_SUCCESS;

	if (init_stat != HAL_NOT_INITIALIZED) {
		return init_stat;
	}

	for (uint32_t i = 0; i < KEY_STORAGE_INDEX_MAX; i++) {
		key_storage[i].key_type = HAL_KEY_UNKNOWN;
		memset(key_storage[i].aes_key, 0, sizeof(key_storage[i].aes_key));
		key_storage[i].aes_key_len = 0;

		rtl_rsa_init(&(key_storage[i].rsa_ctx), RTL_RSA_PKCS_V15, 0);
		rtl_ecdh_init(&(key_storage[i].ecdh_ctx));
		rtl_ecdsa_init(&(key_storage[i].ecdsa_ctx));
		rtl_dhm_init(&(key_storage[i].dhm_ctx));

		memset(key_storage[i].hmac_key, 0, sizeof(key_storage[i].hmac_key));
		key_storage[i].cert_data = NULL;
	}

	/* Realtek added to initialize ROM code calloc & free function handler */
	p_rom_ssl_ram_map = (struct _rom_mbedtls_ram_map*)&rom_ssl_ram_map;
	p_rom_ssl_ram_map->ssl_calloc = my_calloc;
	p_rom_ssl_ram_map->ssl_free = my_free;

	ret = rtl_cryptoEngine_init_wrapper();	/* init crypto engine */
	if (ret == RTL_HAL_SUCCESS) {
		init_stat = HAL_SUCCESS;
	}

	if (ret != RTL_HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;;
	}
	return HAL_SUCCESS;
}

int ameba_hal_deinit(void)
{
	AWRAP_ENTER;

	uint32_t ret = HAL_SUCCESS;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	for (uint32_t i = 0; i < KEY_STORAGE_INDEX_MAX; i++) {
		key_storage[i].key_type = HAL_KEY_UNKNOWN;
		memset(key_storage[i].aes_key, 0, sizeof(key_storage[i].aes_key));
		key_storage[i].aes_key_len = 0;

		rtl_rsa_free(&(key_storage[i].rsa_ctx));
		rtl_ecdh_free(&(key_storage[i].ecdh_ctx));
		rtl_ecdsa_free(&(key_storage[i].ecdsa_ctx));
		rtl_dhm_free(&(key_storage[i].dhm_ctx));

		memset(key_storage[i].hmac_key, 0, sizeof(key_storage[i].hmac_key));
		if (key_storage[i].cert_data != NULL) {
			kmm_free(key_storage[i].cert_data);
		}
	}
	init_stat = HAL_NOT_INITIALIZED;

	return HAL_SUCCESS;
}

int ameba_hal_free_data(hal_data *data)
{
	AWRAP_ENTER;
	if (data) {
		if (data->data) {
			kmm_free(data->data);
		}
		if (data->priv) {
			kmm_free(data->priv);
		}
	}
	return HAL_SUCCESS;
}

int ameba_hal_get_status(void)
{
	AWRAP_ENTER;
	return init_stat;
}

/**
 * Key Manager
 */
int ameba_hal_set_key(hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey)
{
	AWRAP_ENTER;

	uint32_t ret = HAL_SUCCESS;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	if (prikey != NULL) {	/* If private key is pass in */
		return HAL_NOT_SUPPORTED;
	}
	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	switch (mode) {
	case HAL_KEY_AES_128:
	case HAL_KEY_AES_192:
	case HAL_KEY_AES_256:
		/* Key size is fixed, 128 = 16, 192 = 24, 256 = 32, if key size large then 32bytes, is invalid input */
		if (sizeof(key_storage[key_idx].aes_key) < key->data_len) {	/* Key size larger than storage */
			ret = HAL_INVALID_ARGS;
			goto exit;
		}
		memcpy(key_storage[key_idx].aes_key, key->data, key->data_len);
		key_storage[key_idx].aes_key_len = key->data_len;
		break;
	case HAL_KEY_RSA_1024:
	case HAL_KEY_RSA_2048:
	case HAL_KEY_RSA_3072:
	case HAL_KEY_RSA_4096:
		/* Not sure what to do, hal test case did not mention */
		break;
	case HAL_KEY_ECC_BRAINPOOL_P256R1:
	case HAL_KEY_ECC_BRAINPOOL_P384R1:
	case HAL_KEY_ECC_BRAINPOOL_P512R1:
	case HAL_KEY_ECC_SEC_P192R1:
	case HAL_KEY_ECC_SEC_P224R1:
	case HAL_KEY_ECC_SEC_P256R1:
	case HAL_KEY_ECC_SEC_P384R1:
	case HAL_KEY_ECC_SEC_P512R1:
		/* For ECC there two public key X and Y, key->data and key->priv? information base on spec */
		/* Read Public key from char format to MPI format */
		if ((rtl_mpi_read_binary(&(key_storage[key_idx].ecdh_ctx.Q.X), key->data, key->data_len) != HAL_SUCCESS)
		|| (rtl_mpi_read_binary(&(key_storage[key_idx].ecdh_ctx.Q.Y), key->priv, key->priv_len) != HAL_SUCCESS)) {
			ret = HAL_INVALID_ARGS;	/* if not success */
		}
		break;
	case HAL_KEY_HMAC_MD5:
	case HAL_KEY_HMAC_SHA1:
	case HAL_KEY_HMAC_SHA224:
	case HAL_KEY_HMAC_SHA256:
	case HAL_KEY_HMAC_SHA384:
	case HAL_KEY_HMAC_SHA512:
		/* Key is no use, due to not support HMAC, do not let key size more then storage size */
		if (sizeof(key_storage[key_idx].hmac_key) < key->data_len) {	/* Key size larger than storage */
			/* Prevent overflow */
			memcpy(key_storage[key_idx].hmac_key, key->data, sizeof(key_storage[key_idx].hmac_key));
		} else {
			memcpy(key_storage[key_idx].hmac_key, key->data, key->data_len);
		}
		break;
	case HAL_KEY_DH_1024:
	case HAL_KEY_DH_2048:
	case HAL_KEY_DH_4096:
		break;
	default:
		ret = HAL_NOT_SUPPORTED;
		goto exit;
	}
	key_storage[key_idx].key_type = mode;

exit:
	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;;
	}
	return HAL_SUCCESS;
}

int ameba_hal_get_key(hal_key_type mode, uint32_t key_idx, hal_data *key)
{
	AWRAP_ENTER;

	uint32_t ret = HAL_SUCCESS;
	uint32_t puk_x_len, puk_y_len;
	unsigned char *puk_x = NULL;
	unsigned char *puk_y = NULL;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	if (key_idx == FACTORYKEY_RTL_DEVICE) {
		ret = rtl_read_factory_key_wrapper(key);
		goto exit;
	}

	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	if ((mode <= HAL_KEY_ECC_SEC_P512R1) && (mode >= HAL_KEY_ECC_BRAINPOOL_P256R1)) {
		puk_x_len = rtl_mpi_size(&(key_storage[key_idx].ecdh_ctx.Q.X));	/* Get length */
		puk_y_len = rtl_mpi_size(&(key_storage[key_idx].ecdh_ctx.Q.Y));	/* Get length */
		puk_x = (unsigned char *)kmm_malloc(puk_x_len);	/* Allocate memory */
		puk_y = (unsigned char *)kmm_malloc(puk_y_len);	/* Allocate memory */

		if ((puk_x == NULL) || (puk_y == NULL)) {
			ret = HAL_NOT_ENOUGH_MEMORY;	/* Memory not enough */
		} else {
			/* For ECC there two public key X and Y, key->data and key->priv? information base on spec */
			/* Read Public key from MPI format to char format */
			if ((rtl_mpi_write_binary(&(key_storage[key_idx].ecdh_ctx.Q.X), puk_x, puk_x_len) != HAL_SUCCESS)
			|| (rtl_mpi_write_binary(&(key_storage[key_idx].ecdh_ctx.Q.Y), puk_y, puk_y_len) != HAL_SUCCESS)) {
				ret = HAL_FAIL;	/* if not success */
			} else {	/* Success Case, Copy data */
				HAL_COPY_DATA(key, puk_x, puk_x_len);	/* data: publickey_x, priv: publickey_y */
				HAL_COPY_PRIV_DATA(key, puk_y, puk_y_len);
			}
		}
	} else {
		return HAL_NOT_SUPPORTED;
	}

	if (puk_x != NULL) {
		kmm_free(puk_x);	/* free memory */
	}
	if (puk_y != NULL) {
		kmm_free(puk_y);	/* free memory */
	}

exit:
	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_remove_key(hal_key_type mode, uint32_t key_idx)
{
	AWRAP_ENTER;

	uint32_t ret = HAL_SUCCESS;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}
	if (mode != key_storage[key_idx].key_type) {
		return HAL_BAD_KEY;	/* Key is empty */
	}

	/* Init all key info for key index */
	memset(key_storage[key_idx].aes_key, 0, sizeof(key_storage[key_idx].aes_key));
	key_storage[key_idx].aes_key_len = 0;


	rtl_rsa_free(&(key_storage[key_idx].rsa_ctx));
	rtl_ecdh_free(&(key_storage[key_idx].ecdh_ctx));
	rtl_ecdsa_free(&(key_storage[key_idx].ecdsa_ctx));
	rtl_dhm_free(&(key_storage[key_idx].dhm_ctx));
	rtl_rsa_init(&(key_storage[key_idx].rsa_ctx), RTL_RSA_PKCS_V15, 0);
	rtl_ecdh_init(&(key_storage[key_idx].ecdh_ctx));
	rtl_ecdsa_init(&(key_storage[key_idx].ecdsa_ctx));
	rtl_dhm_init(&(key_storage[key_idx].dhm_ctx));

	memset(key_storage[key_idx].hmac_key, 0, sizeof(key_storage[key_idx].hmac_key));
	key_storage[key_idx].key_type = HAL_KEY_UNKNOWN;	/* Remove key type for key index */

	return HAL_SUCCESS;
}

int ameba_hal_generate_key(hal_key_type mode, uint32_t key_idx)
{
	AWRAP_ENTER;

	uint32_t ret = HAL_SUCCESS;
	rtl_ecp_group_id ecp_curve_id;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	switch (mode) {
	case HAL_KEY_AES_128:
		/* Generate Random Key */
		rtw_get_random_bytes(key_storage[key_idx].aes_key, AES_128_KEY_SIZE);
		key_storage[key_idx].aes_key_len = AES_128_KEY_SIZE;
		break;
	case HAL_KEY_AES_192:
		/* Generate Random Key */
		rtw_get_random_bytes(key_storage[key_idx].aes_key, AES_192_KEY_SIZE);
		key_storage[key_idx].aes_key_len = AES_192_KEY_SIZE;
		break;
	case HAL_KEY_AES_256:
		/* Generate Random Key */
		rtw_get_random_bytes(key_storage[key_idx].aes_key, AES_256_KEY_SIZE);
		key_storage[key_idx].aes_key_len = AES_256_KEY_SIZE;
		break;
	case HAL_KEY_RSA_1024:
		ret = rtl_rsa_gen_key(&(key_storage[key_idx].rsa_ctx), hal_rand, NULL, 1024, 65537);
		break;
	case HAL_KEY_RSA_2048:
		ret = rtl_rsa_gen_key(&(key_storage[key_idx].rsa_ctx), hal_rand, NULL, 2048, 65537);
		break;
	case HAL_KEY_RSA_3072:
		ret = rtl_rsa_gen_key(&(key_storage[key_idx].rsa_ctx), hal_rand, NULL, 3072, 65537);
		break;
	case HAL_KEY_RSA_4096:
		ret = rtl_rsa_gen_key(&(key_storage[key_idx].rsa_ctx), hal_rand, NULL, 4096, 65537);
		break;
	case HAL_KEY_ECC_BRAINPOOL_P256R1:
		ecp_curve_id = RTL_ECP_DP_BP256R1;
		break;
	case HAL_KEY_ECC_BRAINPOOL_P384R1:
		ecp_curve_id = RTL_ECP_DP_BP384R1;
		break;
	case HAL_KEY_ECC_BRAINPOOL_P512R1:
		ecp_curve_id = RTL_ECP_DP_BP512R1;
		break;
	case HAL_KEY_ECC_SEC_P192R1:
		ecp_curve_id = RTL_ECP_DP_SECP192R1;
		break;
	case HAL_KEY_ECC_SEC_P224R1:
		ecp_curve_id = RTL_ECP_DP_SECP224R1;
		break;
	case HAL_KEY_ECC_SEC_P256R1:
		ecp_curve_id = RTL_ECP_DP_SECP256R1;
		break;
	case HAL_KEY_ECC_SEC_P384R1:
		ecp_curve_id = RTL_ECP_DP_SECP384R1;
		break;
	case HAL_KEY_ECC_SEC_P512R1:
		ecp_curve_id = RTL_ECP_DP_SECP521R1;
		break;
	case HAL_KEY_HMAC_MD5:
	case HAL_KEY_HMAC_SHA1:
	case HAL_KEY_HMAC_SHA224:
	case HAL_KEY_HMAC_SHA256:
	case HAL_KEY_HMAC_SHA384:
	case HAL_KEY_HMAC_SHA512:
		break;
	default:
		return HAL_NOT_SUPPORTED;
	}

	if ((HAL_KEY_ECC_BRAINPOOL_P256R1 <= mode)
	&& (HAL_KEY_ECC_SEC_P512R1 >= mode)) {
		/* ECDH and ECDASA share same curve ID, but use different struct to store key info */
		/* ECDSA operation */
		if ((rtl_ecdsa_genkey(&(key_storage[key_idx].ecdsa_ctx), ecp_curve_id, hal_rand, NULL) != HAL_SUCCESS)
		/* ECDH operation */
		|| (rtl_ecp_group_load(&(key_storage[key_idx].ecdh_ctx.grp), ecp_curve_id) != HAL_SUCCESS)
		|| (rtl_ecdh_gen_public(&(key_storage[key_idx].ecdh_ctx.grp), &(key_storage[key_idx].ecdh_ctx.d), &(key_storage[key_idx].ecdh_ctx.Q), hal_rand, NULL) != HAL_SUCCESS)) {
			ret = HAL_FAIL;
		}
	}

	if (ret == HAL_SUCCESS) {
		key_storage[key_idx].key_type = mode;	/* Set storage key mode */
	}

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

/**
 * Authenticate
 */
int ameba_hal_generate_random(uint32_t len, hal_data *random)
{
	AWRAP_ENTER;

	uint32_t ret;
	unsigned char inbuf[HAL_MAX_RANDOM_SIZE];

	if (len > HAL_MAX_RANDOM_SIZE) {
		return HAL_INVALID_ARGS;
	}

	ret = rtw_get_random_bytes(inbuf, len);
	if (ret == HAL_SUCCESS) {
		HAL_COPY_DATA(random, inbuf, len);
	}

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_get_hash(hal_hash_type mode, hal_data *input, hal_data *hash)
{
	AWRAP_ENTER;

	uint32_t len;
	unsigned char output[64];

	if (init_stat == HAL_NOT_INITIALIZED) {
		return HAL_NOT_INITIALIZED;
	}

	switch (mode) {
	case HAL_HASH_MD5:
		rtl_md5(input->data, input->data_len, output);
		break;
	case HAL_HASH_SHA1:
		rtl_sha1(input->data, input->data_len, output);
		break;
	case HAL_HASH_SHA224:
		rtl_sha256(input->data, input->data_len, output, RTL_HASH_SHA224);
		break;
	case HAL_HASH_SHA256:
		rtl_sha256(input->data, input->data_len, output, RTL_HASH_SHA256);
		break;
	case HAL_HASH_SHA384:
		rtl_sha512(input->data, input->data_len, output, RTL_HASH_SHA384);
		break;
	case HAL_HASH_SHA512:
		rtl_sha512(input->data, input->data_len, output, RTL_HASH_SHA512);
		break;
	default:
		return HAL_NOT_SUPPORTED;
	}

	len = strlen((char *)output);
	if (len != 0) {
		HAL_COPY_DATA(hash, output, len);
		return HAL_SUCCESS;
	} else {
		sedbg("RTL SE failed (%zu)\n", HAL_FAIL);
		return HAL_FAIL;
	}
}

int ameba_hal_get_hmac(hal_hmac_type mode, hal_data *input, uint32_t key_idx, hal_data *hmac)
{
	AWRAP_ENTER;

	uint32_t ret;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_NOT_SUPPORTED;
}

int ameba_hal_rsa_sign_md(hal_rsa_mode mode, hal_data *hash, uint32_t key_idx, hal_data *sign)
{
	AWRAP_ENTER;

	uint32_t ret;
	rtl_md_type_t sha_mode;
	unsigned char sig_out[1024];

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	if ((HAL_KEY_RSA_1024 > key_storage[key_idx].key_type)
	|| (HAL_KEY_RSA_4096 < key_storage[key_idx].key_type)) {
		return HAL_INVALID_ARGS;
	}

	switch (mode.hash_t) {
	case HAL_HASH_MD5:
		sha_mode = RTL_MD_MD5;
		break;
	case HAL_HASH_SHA1:
		sha_mode = RTL_MD_SHA1;
		break;
	case HAL_HASH_SHA224:
		sha_mode = RTL_MD_SHA224;
		break;
	case HAL_HASH_SHA256:
		sha_mode = RTL_MD_SHA256;
		break;
	case HAL_HASH_SHA384:
		sha_mode = RTL_MD_SHA384;
		break;
	case HAL_HASH_SHA512:
		sha_mode = RTL_MD_SHA512;
		break;
	default:
		return HAL_NOT_SUPPORTED;
	}

	if (mode.rsa_a == HAL_RSASSA_PKCS1_V1_5) {
		rtl_rsa_set_padding(&(key_storage[key_idx].rsa_ctx), RTL_RSA_PKCS_V15, 0);
	} else if (mode.rsa_a == HAL_RSASSA_PKCS1_PSS_MGF1) {
		rtl_rsa_set_padding(&(key_storage[key_idx].rsa_ctx), RTL_RSA_PKCS_V21, sha_mode);
	} else {
		ret = HAL_NOT_SUPPORTED;
		goto exit;
	}
	ret = rtl_rsa_pkcs1_sign(&(key_storage[key_idx].rsa_ctx), hal_rand, NULL, RTL_RSA_PRIVATE, sha_mode, hash->data_len, hash->data, sig_out);

	if (ret == HAL_SUCCESS) {
		HAL_COPY_DATA(sign, sig_out, key_storage[key_idx].rsa_ctx.len);
	}

exit:
	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_rsa_verify_md(hal_rsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx)
{
	AWRAP_ENTER;

	uint32_t ret;
	rtl_md_type_t sha_mode;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	if ((HAL_KEY_RSA_1024 > key_storage[key_idx].key_type)
	|| (HAL_KEY_RSA_4096 < key_storage[key_idx].key_type)) {
		return HAL_INVALID_ARGS;
	}

	switch (mode.hash_t) {
	case HAL_HASH_MD5:
		sha_mode = RTL_MD_MD5;
		break;
	case HAL_HASH_SHA1:
		sha_mode = RTL_MD_SHA1;
		break;
	case HAL_HASH_SHA224:
		sha_mode = RTL_MD_SHA224;
		break;
	case HAL_HASH_SHA256:
		sha_mode = RTL_MD_SHA256;
		break;
	case HAL_HASH_SHA384:
		sha_mode = RTL_MD_SHA384;
		break;
	case HAL_HASH_SHA512:
		sha_mode = RTL_MD_SHA512;
		break;
	default:
		return HAL_NOT_SUPPORTED;
	}

	if (mode.rsa_a == HAL_RSASSA_PKCS1_V1_5) {
		rtl_rsa_set_padding(&(key_storage[key_idx].rsa_ctx), RTL_RSA_PKCS_V15, 0);
	} else if (mode.rsa_a == HAL_RSASSA_PKCS1_PSS_MGF1) {
		rtl_rsa_set_padding(&(key_storage[key_idx].rsa_ctx), RTL_RSA_PKCS_V21, sha_mode);
	} else {
		ret = HAL_NOT_SUPPORTED;
		goto exit;
	}
	ret = rtl_rsa_pkcs1_verify(&(key_storage[key_idx].rsa_ctx), hal_rand, NULL, RTL_RSA_PUBLIC, sha_mode, hash->data_len, hash->data, (unsigned char *)sign->data);

exit:
	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_ecdsa_sign_md(hal_ecdsa_mode mode, hal_data *hash, uint32_t key_idx, hal_data *sign)
{
	AWRAP_ENTER;

	uint32_t ret;
	uint32_t sig_len;
	unsigned char sig[RTL_ECDSA_MAX_LEN];
	rtl_md_type_t hash_type;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	if ((HAL_KEY_ECC_BRAINPOOL_P256R1 > key_storage[key_idx].key_type)
	|| (HAL_KEY_ECC_SEC_P512R1 < key_storage[key_idx].key_type)) {
		return HAL_INVALID_ARGS;
	}
	switch (mode.hash_t) {
	case HAL_HASH_MD5:
		hash_type = RTL_MD_MD5;
		break;
	case HAL_HASH_SHA256:
		hash_type = RTL_MD_SHA256;
		break;
	case HAL_HASH_SHA384:
		hash_type = RTL_MD_SHA384;
		break;
	case HAL_HASH_SHA512:
		hash_type = RTL_MD_SHA512;
		break;
	default:
		return HAL_INVALID_ARGS;
	}

	ret = rtl_ecdsa_write_signature(&(key_storage[key_idx].ecdsa_ctx), hash_type, hash->data, hash->data_len, sig, &sig_len, hal_rand, NULL);
	if (ret == HAL_SUCCESS) {
		HAL_COPY_DATA(sign, sig, sig_len);
	}

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_ecdsa_verify_md(hal_ecdsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx)
{
	AWRAP_ENTER;

	uint32_t ret;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	if ((HAL_KEY_ECC_BRAINPOOL_P256R1 > key_storage[key_idx].key_type)
	|| (HAL_KEY_ECC_SEC_P512R1 < key_storage[key_idx].key_type)) {
		return HAL_INVALID_ARGS;
	}

	ret = rtl_ecdsa_read_signature(&(key_storage[key_idx].ecdsa_ctx), hash->data, hash->data_len, sign->data, sign->data_len);

	return ret;
}

int ameba_hal_dh_generate_param(uint32_t dh_idx, hal_dh_data *dh_param)
{
	AWRAP_ENTER;

	uint32_t ret;
	size_t p_size, output_len = 0;
	unsigned char *output = NULL;
	rtl_mpi mpi_P;
	rtl_mpi mpi_G;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	dh_idx = rtl_index_convt(dh_idx);
	if (dh_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	if (!dh_param->pubkey->data) {
		return HAL_INVALID_ARGS;
	}
	switch (dh_param->mode) {
	case HAL_DH_1024:
	case HAL_DH_2048:
	case HAL_DH_4096:
		break;
	default:
		return HAL_INVALID_ARGS;
	}

	rtl_mpi_init(&mpi_P);
	rtl_mpi_init(&mpi_G);

	/* Read G and P from G inout, Then G and P set in the context */
	if ((rtl_mpi_read_binary(&mpi_P, dh_param->P->data, dh_param->P->data_len) != HAL_SUCCESS)
	|| (rtl_mpi_read_binary(&mpi_G, dh_param->G->data, dh_param->G->data_len) != HAL_SUCCESS)
	|| (rtl_dhm_set_group(&(key_storage[dh_idx].dhm_ctx), &mpi_P, &mpi_G) != HAL_SUCCESS)) {
		ret = HAL_FAIL;
		goto exit;
	}

	output = (unsigned char *)kmm_malloc(HAL_MAX_BUF_SIZE);
	if (output == NULL) {
		ret = HAL_NOT_ENOUGH_MEMORY;
		goto exit;
	}
	/* Sets up and writes the ServerKeyExchange parameters */
	p_size = rtl_mpi_size(&key_storage[dh_idx].dhm_ctx.P);
	ret = rtl_dhm_make_params(&(key_storage[dh_idx].dhm_ctx), (int)p_size, output, &output_len, hal_rand, NULL);
	kmm_free(output);	/* Output is not needed to exports, Output is Param */

	if (ret == HAL_SUCCESS) {
		output_len = key_storage[dh_idx].dhm_ctx.len;
		output = (unsigned char *)kmm_malloc(output_len);
		if (output == NULL) {
			ret = HAL_NOT_ENOUGH_MEMORY;
			goto exit;
		}
		/* Creates its own private value X and exports G^X, GX = (G^X mod P) */
		ret = rtl_dhm_make_public(&(key_storage[dh_idx].dhm_ctx), key_storage[dh_idx].dhm_ctx.len, output, output_len, hal_rand, NULL);

		if (ret == HAL_SUCCESS) {
			/* This API generates GX (G^X mod P) wichi is a pubkey in dh_param with the given input G and P. */
			memcpy(dh_param->pubkey->data, output, output_len);
			dh_param->pubkey->data_len = output_len;
		}	
	}

exit:
	rtl_mpi_free(&mpi_P);
	rtl_mpi_free(&mpi_G);
	if (output != NULL) {
		kmm_free(output);
	}

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_dh_compute_shared_secret(hal_dh_data *dh_param, uint32_t dh_idx, hal_data *shared_secret)
{
	AWRAP_ENTER;

	uint32_t ret;
	size_t buf_size, output_len = 0;
	unsigned char *output = NULL;
	rtl_mpi mpi_P;
	rtl_mpi mpi_G;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	dh_idx = rtl_index_convt(dh_idx);
	if (dh_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	switch (dh_param->mode) {
	case HAL_DH_1024:
	case HAL_DH_2048:
	case HAL_DH_4096:
		break;
	default:
		return HAL_INVALID_ARGS;
	}

	rtl_mpi_init(&mpi_P);
	rtl_mpi_init(&mpi_G);

	/* Read G and P from G inout, Then G and P set in the context */
	/* Read public key from input */
	if ((rtl_mpi_read_binary(&mpi_P, dh_param->P->data, dh_param->P->data_len) != HAL_SUCCESS)
	|| (rtl_mpi_read_binary(&mpi_G, dh_param->G->data, dh_param->G->data_len) != HAL_SUCCESS)
	|| (rtl_dhm_set_group(&(key_storage[dh_idx].dhm_ctx), &mpi_P, &mpi_G) != HAL_SUCCESS)
	|| (rtl_dhm_read_public(&(key_storage[dh_idx].dhm_ctx), dh_param->pubkey->data, dh_param->pubkey->data_len) != HAL_SUCCESS)) {
		ret = HAL_FAIL;
		goto exit;
	}

	buf_size = key_storage[dh_idx].dhm_ctx.len;
	output = (unsigned char *)kmm_malloc(buf_size);
	if (output == NULL) {
		ret = HAL_NOT_ENOUGH_MEMORY;
		goto exit;
	}
	ret = rtl_dhm_calc_secret(&(key_storage[dh_idx].dhm_ctx), output, buf_size, &output_len, hal_rand, NULL);

	if (ret == HAL_SUCCESS) {
		HAL_COPY_DATA(shared_secret, output, output_len);
	}

exit:
	rtl_mpi_free(&mpi_P);
	rtl_mpi_free(&mpi_G);
	if (output != NULL) {
		kmm_free(output);
	}

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_ecdh_compute_shared_secret(hal_ecdh_data *ecdh_param, uint32_t key_idx, hal_data *shared_secret)
{
	AWRAP_ENTER;

	uint32_t ret;
	uint32_t Qp_z_len, output_len = 0;
	unsigned char *Qp_z = NULL;
	unsigned char output[HAL_MAX_SHARED_SECRET_LEN];

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	if ((HAL_KEY_ECC_BRAINPOOL_P256R1 > key_storage[key_idx].key_type)
	&& (HAL_KEY_ECC_SEC_P512R1 < key_storage[key_idx].key_type)) {
		return HAL_INVALID_ARGS;
	}

	/* Read peer public key X and Y */
	if ((rtl_mpi_read_binary(&(key_storage[key_idx].ecdh_ctx.Qp.X), ecdh_param->pubkey_x->data, ecdh_param->pubkey_x->data_len) != HAL_SUCCESS)
	|| (rtl_mpi_read_binary(&(key_storage[key_idx].ecdh_ctx.Qp.Y), ecdh_param->pubkey_y->data, ecdh_param->pubkey_y->data_len) != HAL_SUCCESS)) {
		ret = HAL_FAIL;
		goto exit;
	}

	/* Set Z key */
	Qp_z_len = rtl_mpi_size(&(key_storage[key_idx].ecdh_ctx.Q.Z));	/* Get Key size */
	Qp_z = (unsigned char *)kmm_malloc(Qp_z_len);	/* Allocate memory */
	if (Qp_z == NULL) {
		ret = HAL_NOT_ENOUGH_MEMORY;	/* Memory not enough */
		goto exit;
	}
	memset(Qp_z, 0, Qp_z_len);	/* Set All zero */
	Qp_z[Qp_z_len - 1] = 1;	/* Set last byte 1 */
	if (rtl_mpi_read_binary(&(key_storage[key_idx].ecdh_ctx.Qp.Z), Qp_z, Qp_z_len) != HAL_SUCCESS) {
		ret = HAL_FAIL;
		goto exit;
	}

	/* Secret compute */
	ret = rtl_ecdh_compute_shared(&(key_storage[key_idx].ecdh_ctx.grp), &(key_storage[key_idx].ecdh_ctx.z), &(key_storage[key_idx].ecdh_ctx.Qp), &(key_storage[key_idx].ecdh_ctx.d), hal_rand, NULL);
	/* Shared Secret compute success */
	if (ret == HAL_SUCCESS) {
		/* Write Shared Secret from MPI formtat to char format */
		ret = rtl_mpi_write_binary(&(key_storage[key_idx].ecdh_ctx.z), output, HAL_MAX_SHARED_SECRET_LEN);
		output_len = rtl_mpi_size(&(key_storage[key_idx].ecdh_ctx.z));	/* Get size */

		if (ret == HAL_SUCCESS) {
			/* Copy Shared Secret to output buffer */
			HAL_COPY_DATA(shared_secret, output, output_len);
		}
	}

exit:
	if (Qp_z != NULL) {
		kmm_free(Qp_z);
	}

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_set_certificate(uint32_t cert_idx, hal_data *cert_in)
{
	AWRAP_ENTER;

	uint32_t ret = HAL_SUCCESS;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	cert_idx = rtl_index_convt(cert_idx);
	if (cert_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	/* If cert exist */
	if (key_storage[cert_idx].cert_data != NULL) {
		kmm_free(key_storage[cert_idx].cert_data);	/* Free existing Cert memory */
	}
	key_storage[cert_idx].cert_data = kmm_malloc(cert_in->data_len);	/* malloc for new cert */

	/* Copy cert data and length into local storage */
	memcpy(key_storage[cert_idx].cert_data, cert_in->data, cert_in->data_len);
	key_storage[cert_idx].cert_len = cert_in->data_len;

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_get_certificate(uint32_t cert_idx, hal_data *cert_out)
{
	AWRAP_ENTER;

	uint32_t ret = HAL_SUCCESS;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	if (cert_idx == FACTORYKEY_RTL_CERT) {	/* If factory Cert */
		ret = rtl_read_factory_cert_wrapper(cert_out);
		goto exit;
	}

	cert_idx = rtl_index_convt(cert_idx);
	if (cert_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}
	if (key_storage[cert_idx].cert_data == NULL) {
		return HAL_EMPTY_SLOT;
	}

	HAL_COPY_DATA(cert_out, key_storage[cert_idx].cert_data, key_storage[cert_idx].cert_len);

exit:
	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_remove_certificate(uint32_t cert_idx)
{
	AWRAP_ENTER;

	cert_idx = rtl_index_convt(cert_idx);
	if (cert_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	return HAL_NOT_SUPPORTED;
}

int ameba_hal_get_factory_key(uint32_t key_idx, hal_data *key)
{
	AWRAP_ENTER;

	uint32_t ret;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	if (key_idx == FACTORYKEY_RTL_DEVICE) {
		ret = rtl_read_factory_key_wrapper(key);
	} else {
		return HAL_NOT_SUPPORTED;
	}

	if (ret != RTL_HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_get_factory_cert(uint32_t cert_idx, hal_data *cert)
{
	AWRAP_ENTER;

	uint32_t ret;

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	if (cert_idx == FACTORYKEY_RTL_CERT) {
		ret = rtl_read_factory_cert_wrapper(cert);
	} else {
		return HAL_NOT_SUPPORTED;
	}

	if (ret != RTL_HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_get_factory_data(uint32_t data_idx, hal_data *data)
{
	AWRAP_ENTER;

	return HAL_NOT_SUPPORTED;
}

/**
 * Crypto
 */
int ameba_hal_aes_encrypt(hal_data *dec_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *enc_data)
{
	AWRAP_ENTER;

	uint32_t ret;
	unsigned char aes_output[HAL_MAX_BUF_SIZE];

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	if ((HAL_KEY_AES_128 != key_storage[key_idx].key_type)
	&& (HAL_KEY_AES_192 != key_storage[key_idx].key_type)
	&& (HAL_KEY_AES_256 != key_storage[key_idx].key_type)) {
		return HAL_INVALID_ARGS;	/* Mode not match */
	}

	switch (aes_param->mode) {
	case HAL_AES_ECB_NOPAD:
	case HAL_AES_ECB_ISO9797_M1:
	case HAL_AES_ECB_ISO9797_M2:
	case HAL_AES_ECB_PKCS5:
	case HAL_AES_ECB_PKCS7:
		ret = rtl_cryptoAES_ecb_wrapper(key_storage[key_idx].aes_key, key_storage[key_idx].aes_key_len, dec_data->data, dec_data->data_len, aes_output, RTL_ENCRYPT);
		break;
	case HAL_AES_CBC_NOPAD:
	case HAL_AES_CBC_ISO9797_M1:
	case HAL_AES_CBC_ISO9797_M2:
	case HAL_AES_CBC_PKCS5:
	case HAL_AES_CBC_PKCS7:
		ret = rtl_cryptoAES_cbc_wrapper(key_storage[key_idx].aes_key, key_storage[key_idx].aes_key_len, dec_data->data, dec_data->data_len, aes_output, RTL_ENCRYPT);
		break;
	case HAL_AES_CTR:
		ret = rtl_cryptoAES_ctr_wrapper(key_storage[key_idx].aes_key, key_storage[key_idx].aes_key_len, dec_data->data, dec_data->data_len, aes_output, RTL_ENCRYPT);
		break;
	default:
		return HAL_NOT_SUPPORTED;
	}

	if (ret == RTL_HAL_SUCCESS) {
		HAL_COPY_DATA(enc_data, aes_output, dec_data->data_len);
	}

	if (ret != RTL_HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_aes_decrypt(hal_data *enc_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *dec_data)
{
	AWRAP_ENTER;

	uint32_t ret;
	unsigned char aes_output[HAL_MAX_BUF_SIZE];

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	if ((HAL_KEY_AES_128 != key_storage[key_idx].key_type)
	&& (HAL_KEY_AES_192 != key_storage[key_idx].key_type)
	&& (HAL_KEY_AES_256 != key_storage[key_idx].key_type)) {
		return HAL_INVALID_ARGS;
	}

	switch (aes_param->mode) {
	case HAL_AES_ECB_NOPAD:
	case HAL_AES_ECB_ISO9797_M1:
	case HAL_AES_ECB_ISO9797_M2:
	case HAL_AES_ECB_PKCS5:
	case HAL_AES_ECB_PKCS7:
		ret = rtl_cryptoAES_ecb_wrapper(key_storage[key_idx].aes_key, key_storage[key_idx].aes_key_len, enc_data->data, enc_data->data_len, aes_output, RTL_DECRYPT);
		break;
	case HAL_AES_CBC_NOPAD:
	case HAL_AES_CBC_ISO9797_M1:
	case HAL_AES_CBC_ISO9797_M2:
	case HAL_AES_CBC_PKCS5:
	case HAL_AES_CBC_PKCS7:
		ret = rtl_cryptoAES_cbc_wrapper(key_storage[key_idx].aes_key, key_storage[key_idx].aes_key_len, enc_data->data, enc_data->data_len, aes_output, RTL_DECRYPT);
		break;
	case HAL_AES_CTR:
		ret = rtl_cryptoAES_ctr_wrapper(key_storage[key_idx].aes_key, key_storage[key_idx].aes_key_len, enc_data->data, enc_data->data_len, aes_output, RTL_DECRYPT);
		break;
	default:
		return HAL_NOT_SUPPORTED;
	}

	if (ret == RTL_HAL_SUCCESS) {
		HAL_COPY_DATA(dec_data, aes_output, enc_data->data_len);
	}

	if (ret != RTL_HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_rsa_encrypt(hal_data *dec_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, hal_data *enc_data)
{
	AWRAP_ENTER;

	uint32_t ret;
	size_t output_len;
	unsigned char output[HAL_MAX_BUF_SIZE] = {0};

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	if ((HAL_KEY_RSA_1024 > key_storage[key_idx].key_type)
	&& (HAL_KEY_RSA_4096 < key_storage[key_idx].key_type)) {
		return HAL_INVALID_ARGS;
	}

	ret = rtl_rsa_pkcs1_encrypt(&(key_storage[key_idx].rsa_ctx), hal_rand, NULL, RTL_RSA_PUBLIC, dec_data->data_len, dec_data->data, output);

	/* Output length always size of \p ctx->N, For example, 128 Bytes if RSA-1024 is used. */
	output_len = rtl_mpi_size(&(key_storage[key_idx].rsa_ctx.N));

	if (ret == HAL_SUCCESS) {
		HAL_COPY_DATA(enc_data, output, output_len);
	}

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_rsa_decrypt(hal_data *enc_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, hal_data *dec_data)
{
	AWRAP_ENTER;

	uint32_t ret;
	size_t output_len;
	unsigned char input[HAL_MAX_BUF_SIZE] = {0};
	unsigned char output[HAL_MAX_BUF_SIZE] = {0};

	ret = rtl_chk_stat();
	if (HAL_SUCCESS != ret) {
		return ret;
	}

	key_idx = rtl_index_convt(key_idx);
	if (key_idx >= KEY_STORAGE_INDEX_MAX) {	/* Index Out of Range */
		return HAL_INVALID_SLOT_RANGE;
	}

	if ((HAL_KEY_RSA_1024 > key_storage[key_idx].key_type)
	&& (HAL_KEY_RSA_4096 < key_storage[key_idx].key_type)) {
		return HAL_INVALID_ARGS;
	}

	memcpy(input, enc_data->data, enc_data->data_len);

	ret = rtl_rsa_pkcs1_decrypt(&(key_storage[key_idx].rsa_ctx), hal_rand, NULL, RTL_RSA_PRIVATE, &output_len, input, output, HAL_MAX_BUF_SIZE);

	if (ret == HAL_SUCCESS) {
		HAL_COPY_DATA(dec_data, output, output_len);
	}

	if (ret != HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

/**
 * Secure Storage
 */
int ameba_hal_write_storage(uint32_t ss_idx, hal_data *data)
{
	AWRAP_ENTER;

	uint32_t ret;

	ret = rtl_write_storage_wrapper(ss_idx, data);

	if (ret != RTL_HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_read_storage(uint32_t ss_idx, hal_data *data)
{
	AWRAP_ENTER;

	uint32_t ret;

	ret = rtl_read_storage_wrapper(ss_idx, data);

	if (ret != RTL_HAL_SUCCESS) {
		sedbg("RTL SE failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int ameba_hal_delete_storage(uint32_t ss_idx)
{
	AWRAP_ENTER;

	rtl_delete_storage_wrapper(ss_idx);

	return HAL_SUCCESS;
}

static struct sec_ops_s g_ameba_ops = {
	ameba_hal_init,
	ameba_hal_deinit,
	ameba_hal_free_data,
	ameba_hal_get_status,
	ameba_hal_set_key,
	ameba_hal_get_key,
	ameba_hal_remove_key,
	ameba_hal_generate_key,
	ameba_hal_generate_random,
	ameba_hal_get_hash,
	ameba_hal_get_hmac,
	ameba_hal_rsa_sign_md,
	ameba_hal_rsa_verify_md,
	ameba_hal_ecdsa_sign_md,
	ameba_hal_ecdsa_verify_md,
	ameba_hal_dh_generate_param,
	ameba_hal_dh_compute_shared_secret,
	ameba_hal_ecdh_compute_shared_secret,
	ameba_hal_set_certificate,
	ameba_hal_get_certificate,
	ameba_hal_remove_certificate,
	ameba_hal_get_factory_key,
	ameba_hal_get_factory_cert,
	ameba_hal_get_factory_data,
	ameba_hal_aes_encrypt,
	ameba_hal_aes_decrypt,
	ameba_hal_rsa_encrypt,
	ameba_hal_rsa_decrypt,
	ameba_hal_write_storage,
	ameba_hal_read_storage,
	ameba_hal_delete_storage,
};

static struct sec_lowerhalf_s g_ameba_lower = {&g_ameba_ops, NULL};


#ifndef LINUX
int se_initialize(void)
{
	int res = se_register(SECLINK_PATH, &g_ameba_lower);
	if (res != 0) {
		return -1;
	}

	return 0;
}
#endif

/*
 * It should not be used to general case.
 * It's for testing HAL APIs in user space.
 * Furthermore it wouldn't be used if binary separation is enabled.
 * Because user space couldn't access kernel space variable directly.
 * So it is protected by CONFIG after binary separation is applied.
 */
struct sec_lowerhalf_s *se_get_device(void)
{
	return &g_ameba_lower;
}
