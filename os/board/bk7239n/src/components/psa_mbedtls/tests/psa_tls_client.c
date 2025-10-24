/*
 *  SSL client demonstration program
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

#include "mbedtls/build_info.h"

#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include <components/log.h>
#include <string.h>

#define SERVER_PORT "443"
#define SERVER_NAME "www.howsmyssl.com"
#define SERVER_URL "https://www.howsmyssl.com/a/check"

static const char *GET_REQUEST = "GET "SERVER_URL" HTTP/1.0\r\n"
    "Host: "SERVER_NAME"\r\n"
    "\r\n";

#define DEBUG_LEVEL 0
#define TAG "mbedtls"

extern int bk_rand(void);

static int mbedtls_bk_entropy_poll(void *data, unsigned char *output, size_t len,
                                   size_t *olen)
{
    uint32_t seed = bk_rand();
    if (len > sizeof(seed)) {
        len = sizeof(seed);
    }
    memcpy(output, &seed, len);
    *olen = len;
    return 0;
}

#define TEST_CA_CRT_RSA_SHA256_PEM                                         \
    "-----BEGIN CERTIFICATE-----\r\n"                                      \
    "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\r\n" \
    "-----END CERTIFICATE-----\r\n"                                        \

static const char mbedtls_test_cas_pem[] =
    TEST_CA_CRT_RSA_SHA256_PEM
    "";
static const size_t mbedtls_test_cas_pem_len = sizeof(mbedtls_test_cas_pem);
static const int ssl_default_ciphersuites[] = {
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,
    MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_DHE_RSA_WITH_AES_256_GCM_SHA384,
    MBEDTLS_TLS_DHE_RSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384,
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384,
    MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,
    0
};

int psa_tls_client_main(void)
{
    int ret = 1, len;
    int exit_code = MBEDTLS_EXIT_FAILURE;
    mbedtls_net_context server_fd;
    uint32_t flags;
    unsigned char buf[1024];
    const char *pers = "ssl_client1";

    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;

#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

    /*
     * 0. Initialize the RNG and the session data
     */
    mbedtls_net_init(&server_fd);
    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_init(&conf);
    mbedtls_x509_crt_init(&cacert);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);

#if defined(MBEDTLS_USE_PSA_CRYPTO)
    BK_LOGI(TAG, "psa_crypto_init...");
    psa_status_t status = psa_crypto_init();
    if (status != PSA_SUCCESS) {
        mbedtls_fprintf(stderr, "Failed to initialize PSA Crypto implementation: %d\n",
                        (int) status);
        goto exit;
    }
    BK_LOGI(TAG, " ok\n");
#endif /* MBEDTLS_USE_PSA_CRYPTO */

    BK_LOGI(TAG, "Seeding the random number generator...");
    mbedtls_entropy_add_source(&entropy, mbedtls_bk_entropy_poll, NULL,
                               MBEDTLS_ENTROPY_MAX_GATHER,
                               MBEDTLS_ENTROPY_SOURCE_STRONG);

    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                                     (const unsigned char *) pers,
                                     strlen(pers))) != 0) {
        BK_LOGE(TAG, " failed\n    mbedtls_ctr_drbg_seed returned %d\n", ret);
        goto exit;
    }
    BK_LOGI(TAG, " ok\n");

    BK_LOGI(TAG, "mbedtls_x509_crt_parse, cacert_len: %d ...", mbedtls_test_cas_pem_len);
    ret = mbedtls_x509_crt_parse(&cacert, (const unsigned char *) mbedtls_test_cas_pem,
                                 mbedtls_test_cas_pem_len);
    if (ret < 0) {
        BK_LOGE(TAG, " failed\n    mbedtls_x509_crt_parse returned -0x%x\n\n",
                       (unsigned int) -ret);
        goto exit;
    }
    BK_LOGI(TAG, " ok\n", ret);

    BK_LOGI(TAG, "mbedtls_ssl_set_hostname use_host: %s ...", SERVER_NAME);
    if ((ret = mbedtls_ssl_set_hostname(&ssl, SERVER_NAME)) != 0) {
        BK_LOGE(TAG, " failed\n    mbedtls_ssl_set_hostname returned -0x%04X\n", -ret);
        goto exit;
    }
    BK_LOGI(TAG, " ok\n");

    BK_LOGI(TAG, "mbedtls_ssl_config_defaults...");
    if ((ret = mbedtls_ssl_config_defaults(&conf,
                                           MBEDTLS_SSL_IS_CLIENT,
                                           MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
        BK_LOGE(TAG, " failed\n  ! mbedtls_ssl_config_defaults returned 0x%x\n\n", -ret);
        goto exit;
    }
    BK_LOGI(TAG, " ok\n");

    BK_LOGI(TAG, "mbedtls_ssl_conf_authmode\r\n");
    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_OPTIONAL);

    BK_LOGI(TAG, "mbedtls_ssl_conf_ca_chain\r\n");
    mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);

    BK_LOGI(TAG, "mbedtls_ssl_conf_rng...");
    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
    BK_LOGI(TAG, " ok\n");

    BK_LOGI(TAG, "mbedtls_ssl_conf_ciphersuites...");
    mbedtls_ssl_conf_ciphersuites(&conf, ssl_default_ciphersuites);
    BK_LOGI(TAG, " ok\n");

    BK_LOGI(TAG, "mbedtls_ssl_setup...");
    if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
        BK_LOGE(TAG, " failed\n  ! mbedtls_ssl_setup returned -0x%04X\r\n", -ret);
        goto exit;
    }
    BK_LOGI(TAG, " ok\n");

    BK_LOGI(TAG, "Connecting to tcp/%s/%s...", SERVER_NAME, SERVER_PORT);
    if ((ret = mbedtls_net_connect(&server_fd, SERVER_NAME,
                                   SERVER_PORT, MBEDTLS_NET_PROTO_TCP)) != 0) {
        BK_LOGI(TAG, " failed\n  ! mbedtls_net_connect returned -%x\n\n", -ret);
        goto exit;
    }
    BK_LOGI(TAG, " ok\n");

    BK_LOGI(TAG, "mbedtls_ssl_set_bio...");
    mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);
    BK_LOGI(TAG, " ok\n");

    BK_LOGI(TAG, "Performing the SSL/TLS handshake...");
    while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
            BK_LOGI(TAG, " failed\n  ! mbedtls_ssl_handshake returned -0x%x\n\n",
                           (unsigned int) -ret);
            goto exit;
        }
    }
    BK_LOGI(TAG, " ok\n");

    /*
     * Verify the server certificate
     */
    BK_LOGI(TAG, "Verifying peer X.509 certificate...");

    /* In real life, we probably want to bail out when ret != 0 */
    if ((flags = mbedtls_ssl_get_verify_result(&ssl)) != 0) {
#if !defined(MBEDTLS_X509_REMOVE_INFO)
        char vrfy_buf[512];
#endif

        BK_LOGE(TAG, " failed\n");

#if !defined(MBEDTLS_X509_REMOVE_INFO)
        mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", flags);

        BK_LOGI(TAG, "%s\n", vrfy_buf);
#endif
    } else {
        BK_LOGI(TAG, " ok\n");
    }

    BK_LOGI(TAG, "Cipher suite is %s\n", mbedtls_ssl_get_ciphersuite(&ssl));

    /*
     * Write the GET request
     */
    BK_LOGI(TAG, "  > Write to server:");

    len = sprintf((char *) buf, GET_REQUEST);

    while ((ret = mbedtls_ssl_write(&ssl, buf, len)) <= 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
            BK_LOGI(TAG, " failed\n  ! mbedtls_ssl_write returned %d\n\n", ret);
            goto exit;
        }
    }

    len = ret;
    BK_LOGI(TAG, " %d bytes written\n\n%s", len, (char *) buf);

    /*
     * Read the HTTP response
     */
    BK_LOGI(TAG, "  < Read from server:");

    do {
        len = sizeof(buf) - 1;
        memset(buf, 0, sizeof(buf));
        ret = mbedtls_ssl_read(&ssl, buf, len);

        if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE) {
            continue;
        }

        if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
            break;
        }

        if (ret < 0) {
            BK_LOGI(TAG, "failed\n  ! mbedtls_ssl_read returned %d\n\n", ret);
            break;
        }

        if (ret == 0) {
            BK_LOGI(TAG, "\n\nEOF\n\n");
            break;
        }

        len = ret;
        BK_LOGI(TAG, " %d bytes read\n\n%s", len, (char *) buf);
    } while (1);

    mbedtls_ssl_close_notify(&ssl);

    exit_code = MBEDTLS_EXIT_SUCCESS;

exit:

#ifdef MBEDTLS_ERROR_C
    if (exit_code != MBEDTLS_EXIT_SUCCESS) {
        char error_buf[100];
        mbedtls_strerror(ret, error_buf, 100);
        BK_LOGI(TAG, "Last error was: %d - %s\n\n", ret, error_buf);
    }
#endif

    mbedtls_net_free(&server_fd);
    mbedtls_x509_crt_free(&cacert);
    mbedtls_ssl_free(&ssl);
    mbedtls_ssl_config_free(&conf);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
#if defined(MBEDTLS_USE_PSA_CRYPTO)
    mbedtls_psa_crypto_free();
#endif /* MBEDTLS_USE_PSA_CRYPTO */

    mbedtls_exit(exit_code);
}
