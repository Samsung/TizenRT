#ifndef MBEDTLS_ACCELERATOR_CONF_H
#define MBEDTLS_ACCELERATOR_CONF_H

#define MBEDTLS_NO_UDBL_DIVISION
#define MBEDTLS_PLATFORM_NO_STD_FUNCTIONS
#define MBEDTLS_PLATFORM_SNPRINTF_ALT

#define MBEDTLS_AES_ALT
#define MBEDTLS_CCM_ALT
#define MBEDTLS_CMAC_ALT
#define MBEDTLS_GCM_ALT
#define MBEDTLS_SHA256_ALT
#define MBEDTLS_ENTROPY_HARDWARE_ALT
#define MBEDTLS_AES_ROM_TABLES
#define MBEDTLS_ECJPAKE_C
#define MBEDTLS_DES_C
#define MBEDTLS_NIST_KW_C
#define MBEDTLS_GCM_C
#define MBEDTLS_CMAC_C
#define MBEDTLS_RSA_C
#define MBEDTLS_PKCS1_V15
#define MBEDTLS_PKCS1_V21

/* sha-1 effects the tfm-test-repo testing. The following is tfm-test offical reply:
   The configuration of the TF-M Crypto service that it’s tested by default is just an example, and the SHA-1 algorithm is
   allowed from the PSA spec point of view; in our case we have decided to not enable SHA-1 support due to the fact that
   it’s widely accepted to have known collision attacks [1], NIST deprecating it in 2011 [2], and having exposed weaknesses
   since long, 2005 [3], i.e. to encourage by default having a look into more robust alternatives.
   Anyway, TF-M’s test 1010 just aims at testing the interface for the correct error response, nothing more. If your
   deployment still supports PSA_ALG_SHA_1, I’d recommend to just ignore the output of TEST_1010. On our side, we could
   gate the test not to run when PSA_WANT_ALG_SHA_1 is defined.
 */
//#define MBEDTLS_SHA1_C

#define MBEDTLS_DHM_C
#define MBEDTLS_PKCS5_C
#define MBEDTLS_PKCS12_C
#define MBEDTLS_SELF_TEST

#if defined(MBEDTLS_AES_ALT)
#define MBEDTLS_AES_SETKEY_DEC_ALT
#define MBEDTLS_AES_SETKEY_ENC_ALT
#define MBEDTLS_AES_DECRYPT_ALT
#define MBEDTLS_AES_ENCRYPT_ALT
#endif

#undef MBEDTLS_HAVE_TIME
#undef MBEDTLS_HAVE_TIME_DATE
#undef MBEDTLS_ENTROPY_NV_SEED
#undef MBEDTLS_ECP_DP_CURVE25519_ENABLED
#undef MBEDTLS_ECP_DP_CURVE448_ENABLED

/* enable PSK */
#define MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED
//#define MBEDTLS_SSL_CIPHERSUITES MBEDTLS_TLS_PSK_WITH_AES_128_CBC_SHA256
//#define memmove psa_memmove     //TODO wangzhilei
//#define vsnprintf psa_vsnprintf //TODO wangzhilei

#define MBEDTLS_SSL_MAX_CONTENT_LEN             4096 /**< Maxium fragment length in bytes, determines the size of each of the two internal I/O buffers */

/*add config for Dubhe
	#define MBEDTLS_SM2DSA_C
	#define MBEDTLS_SM2KEP_C
	#define MBEDTLS_SM2PKE_C
	#define MBEDTLS_SM2DSA_DETERMINISTIC
	#define MBEDTLS_ECP_DP_SM2P256V1_ENABLED
	#define MBEDTLS_SM3_C
	#define MBEDTLS_SM4_C
*/

#define TEE_M 1
#define ARM_CE_DUBHE 1
#define DUBHE_SECURE 1
#define ARM_CE_DUBHE_SCA 1
#define ARM_CE_DUBHE_TRNG 1
#define ARM_CE_DUBHE_OTP 1
#define ARM_CE_DUBHE_HASH 1
#define ARM_CE_DUBHE_ACA 1
#define DUBHE_FOR_RUNTIME 1

/*below two macro is used for side channel attack mitigation
 * it will be commented by default, if you want to enable the
 * SCA mitigation,remember to uncomment it
 * */
//#define MBEDTLS_ENBALE_TIMING_ATTACK_MITIGATION
//#define MBEDTLS_ENBALE_SCALAR_MUL_COUNTERMEASURE
#define MBEDTLS_ENTROPY_FORCE_SHA256
#define MBEDTLS_CTR_DRBG_MAX_REQUEST             (10 * 1024) /**< Maximum number of requested bytes per call */

#endif /* MBEDTLS_ACCELERATOR_CONF_H */
