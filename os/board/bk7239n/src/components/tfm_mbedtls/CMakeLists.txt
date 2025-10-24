set(incs)
set(srcs)

if (CONFIG_MBEDTLS_ACCELERATOR)

set(TFM_MBEDTLS_CONFIG_FILE_PATH  "${COMPONENT_DIR}/mbedtls_port/tfm_mbedcrypto_config_profile_medium.h")

list(APPEND incs
	mbedtls/library
	mbedtls/include
	#mbedtls/include/mbedtls
	mbedtls_port
	mbedtls_port/accelerator
	mbedtls_port/accelerator/dubhe_alt/inc
	mbedtls_port/accelerator/dubhe_driver/inc
	mbedtls_port/accelerator/dubhe_driver/inc/crypto
	mbedtls_port/accelerator/dubhe_driver/inc/common
	mbedtls_port/accelerator/spe/inc
	mbedtls_port/accelerator/spe/inc/pal
	mbedtls_port/accelerator/spe/inc/hal
	mbedtls/tests/include/test
	mbedtls/tests/include/test/drivers
)

list(APPEND srcs
	mbedtls/library/aes.c
	mbedtls/library/aesni.c
	mbedtls/library/aria.c
	mbedtls/library/asn1parse.c
	mbedtls/library/asn1write.c
	mbedtls/library/base64.c
	#mbedtls/library/bignum.c
	mbedtls/library/camellia.c
	mbedtls/library/ccm.c
	mbedtls/library/chacha20.c
	mbedtls/library/chachapoly.c
	mbedtls/library/cipher.c
	mbedtls/library/cipher_wrap.c
	mbedtls/library/cmac.c
	mbedtls/library/ctr_drbg.c
	mbedtls/library/des.c
	#mbedtls/library/dhm.c
	#mbedtls/library/ecdh.c
	#mbedtls/library/ecdsa.c
	mbedtls/library/ecjpake.c
	#mbedtls/library/ecp.c
	#mbedtls/library/ecp_curves.c
	mbedtls/library/entropy.c
	#mbedtls/library/entropy_poll.c
	mbedtls/library/gcm.c
	mbedtls/library/hkdf.c
	mbedtls/library/hmac_drbg.c
	#mbedtls/library/md.c
	mbedtls/library/md5.c
	mbedtls/library/memory_buffer_alloc.c
	mbedtls/library/nist_kw.c
	#mbedtls/library/oid.c
	mbedtls/library/padlock.c
	mbedtls/library/pem.c
	mbedtls/library/pk.c
	mbedtls/library/pk_wrap.c
	mbedtls/library/pkcs12.c
	mbedtls/library/pkcs5.c
	#mbedtls/library/pkparse.c
	mbedtls/library/pkwrite.c
	mbedtls/library/platform.c
	mbedtls/library/platform_util.c
	mbedtls/library/poly1305.c
	mbedtls/library/ripemd160.c
	#mbedtls/library/rsa.c
	mbedtls/library/sha1.c
	mbedtls/library/sha256.c
	mbedtls/library/sha512.c
	mbedtls/library/threading.c
	mbedtls/library/timing.c
	mbedtls/library/version.c
	mbedtls/library/constant_time.c
	mbedtls/library/rsa_alt_helpers.c
	mbedtls/library/x509write_csr.c
	mbedtls/library/x509_create.c

	mbedtls_port/accelerator/spe/src/misc.c
	#mbedtls_port/accelerator/crypto_hw.c

	mbedtls_port/accelerator/dubhe_alt/src/aes.c
	mbedtls_port/accelerator/dubhe_alt/src/bignum_wrap.c
	mbedtls_port/accelerator/dubhe_alt/src/ccm.c
	mbedtls_port/accelerator/dubhe_alt/src/cmac.c
	mbedtls_port/accelerator/dubhe_alt/src/gcm.c
	mbedtls_port/accelerator/dubhe_alt/src/otp.c
	mbedtls_port/accelerator/dubhe_alt/src/sha256.c

	mbedtls_port/accelerator/dubhe_driver/src/common/dubhe_driver.c
	mbedtls_port/accelerator/dubhe_driver/src/common/dubhe_event.c
	mbedtls_port/accelerator/dubhe_driver/src/common/dubhe_intr_handler.c
	mbedtls_port/accelerator/dubhe_driver/src/common/dubhe_sram_alloc.c
	mbedtls_port/accelerator/dubhe_driver/src/crypto/aca/dubhe_aca.c
	mbedtls_port/accelerator/dubhe_driver/src/crypto/aca/dubhe_aca_hw.c
	mbedtls_port/accelerator/dubhe_driver/src/crypto/aead/dubhe_ccm.c
	mbedtls_port/accelerator/dubhe_driver/src/crypto/aead/dubhe_gcm.c
	mbedtls_port/accelerator/dubhe_driver/src/crypto/hash/dubhe_hash.c
	mbedtls_port/accelerator/dubhe_driver/src/crypto/mac/dubhe_cbcmac.c
	mbedtls_port/accelerator/dubhe_driver/src/crypto/mac/dubhe_cmac.c
	mbedtls_port/accelerator/dubhe_driver/src/crypto/mac/dubhe_ghash.c
	mbedtls_port/accelerator/dubhe_driver/src/crypto/otp/dubhe_otp.c
	mbedtls_port/accelerator/dubhe_driver/src/crypto/sca/dubhe_sca.c
	mbedtls_port/accelerator/dubhe_driver/src/crypto/trng/dubhe_trng.c
	mbedtls_port/accelerator/dubhe_driver/src/hal_pal/pal_heap.c
	mbedtls_port/accelerator/dubhe_driver/src/hal_pal/pal_log.c
	mbedtls_port/accelerator/dubhe_driver/src/hal_pal/pal_mutex.c
	mbedtls_port/accelerator/dubhe_driver/src/hal_pal/pal_preempt.c
	mbedtls_port/accelerator/dubhe_driver/src/hal_pal/pal_semaphore.c
	mbedtls_port/accelerator/dubhe_driver/src/hal_pal/pal_signal.c
	mbedtls_port/accelerator/dubhe_driver/src/hal_pal/pal_string.c
	mbedtls_port/accelerator/dubhe_driver/src/hal_pal/pal_time.c

	mbedtls_port/accelerator/dubhe_lib/bignum.c
	mbedtls_port/accelerator/dubhe_lib/bignum_delta.c
	mbedtls_port/accelerator/dubhe_lib/cipher.c
	mbedtls_port/accelerator/dubhe_lib/cipher_wrap.c
	mbedtls_port/accelerator/dubhe_lib/ctr_drbg.c
	mbedtls_port/accelerator/dubhe_lib/dhm.c
	mbedtls_port/accelerator/dubhe_lib/ecdh.c
	mbedtls_port/accelerator/dubhe_lib/ecdsa.c
	mbedtls_port/accelerator/dubhe_lib/ecp.c
	mbedtls_port/accelerator/dubhe_lib/ecp_curves.c
	mbedtls_port/accelerator/dubhe_lib/entropy_poll.c
	mbedtls_port/accelerator/dubhe_lib/md.c
	mbedtls_port/accelerator/dubhe_lib/oid.c
	mbedtls_port/accelerator/dubhe_lib/pkparse.c
	mbedtls_port/accelerator/dubhe_lib/rsa.c

	mbedtls/programs/test/selftest.c
)

if (CONFIG_TE200_TEST)
	list(APPEND incs
		te200_tests
	)

	list(APPEND srcs
		te200_tests/te200_sha_test.c
		te200_tests/te200_aes_test.c
		te200_tests/cli_te200.c
		te200_tests/te200_ecdsa_test.c
		te200_tests/te200_rsa_test.c
	)
endif()

endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES bk_common
)

if (CONFIG_MBEDTLS_ACCELERATOR)
target_compile_definitions(${COMPONENT_LIB}
	PUBLIC
		MBEDTLS_CONFIG_FILE="${TFM_MBEDTLS_CONFIG_FILE_PATH}"
		CRYPTO_HW_ACCELERATOR
		CRYPTO_HW_ACCELERATOR_OTP_PROVISIONING
		TEE_M
		ARM_CE_DUBHE
		DUBHE_SECURE
		ARM_CE_DUBHE_SCA
		ARM_CE_DUBHE_TRNG
		ARM_CE_DUBHE_OTP
		ARM_CE_DUBHE_HASH
		ARM_CE_DUBHE_ACA
		DUBHE_FOR_RUNTIME
)

endif()
