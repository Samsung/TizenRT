set(incs)
set(srcs)

if (CONFIG_PSA_MBEDTLS)

set(PSA_MBEDTLS_CONFIG_FILE_PATH  "${COMPONENT_DIR}/mbedtls_port/configs/mbedtls_psa_crypto_config.h")

list(APPEND incs
	mbedtls/include
	mbedtls/library
	mbedtls_port/inc
)

list(APPEND srcs
	# src_x509
	mbedtls/library/pkcs7.c
	mbedtls/library/x509.c
	mbedtls/library/x509_create.c
	mbedtls/library/x509_crl.c
	mbedtls/library/x509_crt.c
	mbedtls/library/x509_csr.c
	mbedtls/library/x509write.c
	mbedtls/library/x509write_crt.c
	mbedtls/library/x509write_csr.c

	# src_tls
	mbedtls/library/debug.c
	mbedtls/library/mps_reader.c
	mbedtls/library/mps_trace.c
	mbedtls_port/src/net_sockets.c
	mbedtls/library/ssl_cache.c
	mbedtls/library/ssl_ciphersuites.c
	mbedtls/library/ssl_client.c
	mbedtls/library/ssl_cookie.c
	mbedtls/library/ssl_debug_helpers_generated.c
	mbedtls/library/ssl_msg.c
	mbedtls/library/ssl_ticket.c
	mbedtls/library/ssl_tls.c
	mbedtls/library/ssl_tls12_client.c
	mbedtls/library/ssl_tls12_server.c
	mbedtls/library/ssl_tls13_keys.c
	mbedtls/library/ssl_tls13_server.c
	mbedtls/library/ssl_tls13_client.c
	mbedtls/library/ssl_tls13_generic.c
	mbedtls_port/src/tls_hardware.c
	mbedtls_port/src/tls_mem.c

	# src_crypto
	mbedtls/library/aes.c
	mbedtls/library/aesni.c
	mbedtls/library/aria.c
	mbedtls/library/asn1parse.c
	mbedtls/library/asn1write.c
	mbedtls/library/base64.c
	mbedtls/library/bignum.c
	mbedtls/library/bignum_core.c
	mbedtls/library/camellia.c
	mbedtls/library/ccm.c
	mbedtls/library/chacha20.c
	mbedtls/library/chachapoly.c
	mbedtls/library/cipher.c
	mbedtls/library/cipher_wrap.c
	mbedtls/library/constant_time.c
	mbedtls/library/ctr_drbg.c
	mbedtls/library/des.c
	mbedtls/library/dhm.c
	mbedtls/library/ecdh.c
	mbedtls/library/ecdsa.c
	mbedtls/library/ecp.c
	mbedtls/library/ecp_curves.c
	mbedtls/library/ecp_curves_new.c
	mbedtls/library/entropy.c
	mbedtls/library/entropy_poll.c
	mbedtls/library/error.c
	mbedtls/library/gcm.c
	mbedtls/library/hmac_drbg.c
	mbedtls/library/md.c
	mbedtls/library/md5.c
	mbedtls/library/nist_kw.c
	mbedtls/library/oid.c
	mbedtls/library/ripemd160.c
	mbedtls/library/rsa.c
	mbedtls/library/rsa_alt_helpers.c
	mbedtls/library/sha1.c
	mbedtls/library/sha256.c
	mbedtls/library/sha512.c
	mbedtls/library/sha3.c
	mbedtls/library/pem.c
	mbedtls_port/src/psa_crypto_client.c
	mbedtls/library/pk.c
	mbedtls/library/pk_wrap.c
	mbedtls/library/pkcs12.c
	mbedtls/library/pkcs5.c
	mbedtls/library/pkparse.c
	mbedtls/library/pkwrite.c
	mbedtls/library/platform_util.c
	mbedtls/library/poly1305.c
	mbedtls/library/psa_util.c
	mbedtls_port/src/psa_crypto.c
)

if (CONFIG_PSA_MBEDTLS_TEST)
list(APPEND incs
	tests
)

list(APPEND srcs
	# tests
	tests/aes_cbc.c
	tests/aes_gcm.c
	tests/cli_crypto.c
	tests/ecdh.c
	tests/ecdsa.c
	tests/hmac.c
	tests/psa_tls_client.c
	tests/sha256.c
)
endif()

if (CONFIG_PSA_CUSTOMIZATION_TEST)
list(APPEND incs
	customization/tests
)

list(APPEND srcs
	# tests
	customization/tests/tz_aes_cbc_test.c
	customization/tests/tz_aes_gcm_test.c
	customization/tests/tz_ecdh_test.c
	customization/tests/tz_ecdsa_test.c
	customization/tests/tz_hmac_test.c
	customization/tests/tz_psa_tls_client_test.c
	customization/tests/tz_sha256_test.c
	customization/tests/cli_psa_custom.c
)
endif()

if (CONFIG_PSA_CUSTOMIZATION_TZ_L)
list(APPEND incs
	customization/tz/l
)

list(APPEND srcs
	#customization
	#customization/tz/l/tz_mbedtls.c
	customization/tz/l/hal_tz.c
)
endif()

endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES bk_common tfm lwip_intf_v2_1
)

if (CONFIG_PSA_MBEDTLS)
target_compile_definitions(${COMPONENT_LIB}
	PUBLIC
		MBEDTLS_CONFIG_FILE="${PSA_MBEDTLS_CONFIG_FILE_PATH}"
)
endif()
