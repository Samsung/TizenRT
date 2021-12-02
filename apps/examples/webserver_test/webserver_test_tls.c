/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
 ****************************************************************************/

#include "webserver_test_tls.h"

static const char *TLSNAME = "ws_test_tls_client";

void ws_test_tls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    FUNC_EN;
    printf("%s:%04d: %s", file, line, str);
    FUNC_EX;
}

void ws_test_tls_release(struct http_client_tls_t *client)
{
    FUNC_EN;
    if (client == NULL) {
        return;
    }

    mbedtls_x509_crt_free(&(client->tls_rootca));
    mbedtls_x509_crt_free(&(client->tls_clicert));
    mbedtls_pk_free(&(client->tls_pkey));
    mbedtls_ssl_config_free(&(client->tls_conf));
    mbedtls_ctr_drbg_free(&(client->tls_ctr_drbg));
    mbedtls_entropy_free(&(client->tls_entropy));
    mbedtls_ssl_session_free(&(client->tls_session));
    FUNC_EX;
}

int ws_test_tls_init(struct http_client_tls_t *client, struct http_client_ssl_config_t *ssl_config)
{
    FUNC_EN;
    int result = 0;

    if (!client || !ssl_config) {
        return WS_TEST_ERR;
    }

    mbedtls_ssl_config_init(&(client->tls_conf));
    mbedtls_x509_crt_init(&(client->tls_rootca));
    mbedtls_x509_crt_init(&(client->tls_clicert));
    mbedtls_pk_init(&(client->tls_pkey));
    mbedtls_entropy_init(&(client->tls_entropy));
    mbedtls_ctr_drbg_init(&(client->tls_ctr_drbg));
    mbedtls_ssl_session_init(&(client->tls_session));
    mbedtls_ssl_conf_authmode(&(client->tls_conf), ssl_config->auth_mode);
#ifdef MBEDTLS_DEBUG_C
    mbedtls_debug_set_threshold(MBED_DEBUG_LEVEL);
#endif

    /* 0. Initialize the RNG */
    PRNT("  . Seeding the random number generator...");

    if ((result = mbedtls_ctr_drbg_seed(&(client->tls_ctr_drbg), 
                                        mbedtls_entropy_func,
                                        &(client->tls_entropy), 
                                        (const unsigned char *)TLSNAME,
                                        strlen(TLSNAME))) != 0) {
        PRNT("Error: mbedtls_ctr_drbg_seed returned -%4x", -result);
        goto TLS_INIT_EXIT;
    }

    PRNT("Ok");

    /* 1. Setup ssl stuff */
    PRNT("  . Setting up the SSL data...");

    if ((result = mbedtls_ssl_config_defaults(&(client->tls_conf),
                  MBEDTLS_SSL_IS_CLIENT,
                  MBEDTLS_SSL_TRANSPORT_STREAM,
                  MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
        PRNT("Error: mbedtls_ssl_config_defaults returned -%4x", -result);
        goto TLS_INIT_EXIT;
    }

    mbedtls_ssl_conf_rng(&(client->tls_conf), mbedtls_ctr_drbg_random,
                         &(client->tls_ctr_drbg));
    mbedtls_ssl_conf_dbg(&(client->tls_conf), ws_test_tls_debug, stdout);

    PRNT("Ok");

    if (ssl_config->dev_cert && ssl_config->private_key) {
        /* 2. Load the certificates and private key */

        PRNT("  . Loading the client cert. and key...");

        if ((result = mbedtls_x509_crt_parse(&(client->tls_clicert),
                                             (const unsigned char *)ssl_config->dev_cert,
                                             ssl_config->dev_cert_len)) != 0) {
            PRNT("Error: cli_cert parse fail, returned -%4x", -result);
            goto TLS_INIT_EXIT;
        }

        if ((result = mbedtls_pk_parse_key(&(client->tls_pkey),
                                           (const unsigned char *)ssl_config->private_key,
                                           ssl_config->private_key_len, NULL, 0)) != 0) {
            PRNT("Error: cli_key parse fail, returned -%4x", -result);
            goto TLS_INIT_EXIT;
        }

        if ((result = mbedtls_ssl_conf_own_cert(&(client->tls_conf),
                                                &(client->tls_clicert),
                                                &(client->tls_pkey))) != 0) {
            PRNT("Error: mbedtls_ssl_conf_own_cert returned -%4x", -result);
            goto TLS_INIT_EXIT;
        }

        PRNT("Ok");
    }

    if (ssl_config->root_ca) {

        /* 3. Load the CA certificate */
        PRNT("  . Loading the CA cert...");

        if ((result = mbedtls_x509_crt_parse(&(client->tls_rootca),
                                             (const unsigned char *)ssl_config->root_ca,
                                             ssl_config->root_ca_len)) != 0) {
            PRNT("Error: CA_cert parse fail, returned -%4x", -result);
            goto TLS_INIT_EXIT;
        }

        /* CA cert may be first or second in chain depending if client cert was loaded */
        mbedtls_ssl_conf_ca_chain(&(client->tls_conf), &(client->tls_rootca), NULL);

        PRNT("Ok");
    }

    FUNC_EX;
    return 0;
TLS_INIT_EXIT:
    ws_test_tls_release(client);
    FUNC_EX;
    return result;
}

void ws_test_tls_ssl_release(struct http_client_tls_t *client)
{
    FUNC_EN;
    if (client == NULL) {
        return;
    }

    mbedtls_net_free(&(client->tls_client_fd));
    mbedtls_ssl_session_reset(&(client->tls_ssl));
    mbedtls_ssl_free(&(client->tls_ssl));
    FUNC_EX;
}

int ws_test_tls_handshake(struct http_client_tls_t *client, const char *hostname)
{
    FUNC_EN;
    int result = 0;

    mbedtls_net_init(&(client->tls_client_fd));
    mbedtls_ssl_init(&(client->tls_ssl));

    client->tls_client_fd.fd = client->client_fd;

    if (mbedtls_net_set_block(&(client->tls_client_fd)) < 0) {
        PRNT("Error: mbedtls_net_set_block fail");
        goto HANDSHAKE_FAIL;
    }

    PRNT("TLS Init Success");

    if ((result = mbedtls_ssl_setup(&(client->tls_ssl),
                                    &(client->tls_conf))) != 0) {
        PRNT("Error: mbedtls_ssl_setup returned -%4x", -result);
        goto HANDSHAKE_FAIL;
    }

    /*
     * Server name intication is an extension to the TLS networking protocol
     * If server presents multiple certificates on the same IP address,
     * client could make multiple secure session depends on hostname.
     *
     * Note : Hostname in TLS is a subject's common name(CN) of certificates.
    */
#if WS_TEST_CONF_CHECK_TLS_HOSTNAME
    if ((result = mbedtls_ssl_set_hostname(&(client->tls_ssl), hostname)) != 0) {
        PRNT("Error: mbedtls_hostname returned -%4x", -result);
        goto HANDSHAKE_FAIL;
    }
#endif

    mbedtls_ssl_set_bio(&(client->tls_ssl), &(client->tls_client_fd),
                        mbedtls_net_send, mbedtls_net_recv, NULL);

    /* Handshake */
    while ((result = mbedtls_ssl_handshake(&(client->tls_ssl))) != 0) {
        if (result != MBEDTLS_ERR_SSL_WANT_READ &&
            result != MBEDTLS_ERR_SSL_WANT_WRITE) {
            PRNT("Error: TLS Handshake fail returned -%4x", -result);
            goto HANDSHAKE_FAIL;
        }
    }

    PRNT("TLS Handshake Success");
    FUNC_EX;
    return 0;
HANDSHAKE_FAIL:
    FUNC_EX;
    return result;
}