/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Camille Bégué, Samsung - Please refer to git log
 *
 *******************************************************************************/
#include "pem_utils.h"

#include <tls/x509_crt.h>
#include <tls/pk.h>

bool convert_pem_privatekey_to_der(const char *private_key_pem, char **private_key_der, uint16_t *len)
{
    int ret = 0;
    char pk_der[1024];
    mbedtls_pk_context pkey;

    mbedtls_pk_init(&pkey);
    ret = mbedtls_pk_parse_key(&pkey,
                               (const unsigned char*)private_key_pem,
                               strlen(private_key_pem) + 1, NULL, 0);
    if (ret) {
        mbedtls_pk_free(&pkey);
        return false;
    }

    *len = mbedtls_pk_write_key_der(&pkey, (unsigned char *)pk_der, sizeof(pk_der));

    if (*len < 0) {
        mbedtls_pk_free(&pkey);
        free(pk_der);
        return false;
    }

    *private_key_der = malloc(*len);
    memcpy(*private_key_der, pk_der + sizeof(pk_der) - *len, *len);

    mbedtls_pk_free(&pkey);

    return true;
}

bool convert_pem_x509_to_der(const char *cert_buffer_pem, char **cert_buffer_der, uint16_t *len)
{
    int ret = 0;
    mbedtls_x509_crt cert;
    char *cert_der = NULL;

    if (!cert_buffer_pem) {
        return false;
    }

    mbedtls_x509_crt_init(&cert);
    ret = mbedtls_x509_crt_parse(&cert,
                                 (const unsigned char *)cert_buffer_pem,
                                 strlen(cert_buffer_pem) + 1);

    if (ret) {
        mbedtls_x509_crt_free(&cert);
        return false;
    }

    cert_der = malloc(cert.raw.len);
    if (!cert_der) {
        mbedtls_x509_crt_free(&cert);
        return false;
    }

    memcpy(cert_der, cert.raw.p, cert.raw.len);
    *cert_buffer_der = cert_der;
    *len = cert.raw.len;

    mbedtls_x509_crt_free(&cert);
    return true;
}
