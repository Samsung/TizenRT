/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "src/core/tsi/ssl_transport_security.h"

#include <grpc/support/port_platform.h>

#include <limits.h>
#include <string.h>
#include <stdio.h>

/* TODO(jboeuf): refactor inet_ntop into a portability header. */
/* Note: for whomever reads this and tries to refactor this, this
   can't be in grpc, it has to be in gpr. */
#ifdef GPR_WINDOWS
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/sync.h>
#include <grpc/support/thd.h>
#include <grpc/support/useful.h>

extern "C" {
#include <mbedtls/ssl.h>
#include <mbedtls/pem.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/certs.h>
#include <mbedtls/x509.h>
#include <mbedtls/error.h>
#include <mbedtls/debug.h>
#include <mbedtls/ssl_ticket.h>
}

#include "src/core/tsi/ssl_types.h"
#include "src/core/tsi/transport_security.h"

/* --- Constants. ---*/

#define TSI_SSL_MAX_PROTECTED_FRAME_SIZE_UPPER_BOUND 16384
#define TSI_SSL_MAX_PROTECTED_FRAME_SIZE_LOWER_BOUND 1024

/* Putting a macro like this and littering the source file with #if is really
   bad practice.
   TODO(jboeuf): refactor all the #if / #endif in a separate module. */
#ifndef TSI_OPENSSL_ALPN_SUPPORT
#define TSI_OPENSSL_ALPN_SUPPORT 1
#endif

/* TODO(jboeuf): I have not found a way to get this number dynamically from the
   SSL structure. This is what we would ultimately want though... */
#define TSI_SSL_MAX_PROTECTION_OVERHEAD 100

/* For mbedtls */
static const char *pers = "TizenRT";

#define PEM_BEGIN_CRT           "-----BEGIN CERTIFICATE-----\n"
#define PEM_END_CRT             "-----END CERTIFICATE-----\n"

#define MBEDTLS_BUF_MAX_SIZE  128

#define DEBUG_LEVEL 0

// #define tsi_mbedtls_log(...) printf(__VA_ARGS__)
#define tsi_mbedtls_log(...) (void)0

/* --- Structure definitions. ---*/

typedef struct tsi_ssl_ctx {
  mbedtls_entropy_context entropy;
  mbedtls_ctr_drbg_context ctr_drbg;
  mbedtls_ssl_context ssl;
  mbedtls_ssl_config conf;
  mbedtls_x509_crt cert;
  mbedtls_x509_crt ca;
  mbedtls_pk_context pkey;
#if defined(MBEDTLS_SSL_SESSION_TICKETS)  
  mbedtls_ssl_ticket_context ticket_ctx;
#endif  
  int *ciphersuites;
  gpr_refcount refcount;
} tsi_ssl_ctx;

struct tsi_ssl_handshaker_factory {
  const tsi_ssl_handshaker_factory_vtable* vtable;
  gpr_refcount refcount;
};

struct tsi_ssl_client_handshaker_factory {
  tsi_ssl_handshaker_factory base;
  tsi_ssl_ctx* ssl_ctx;
  unsigned char** alpn_protocol_list;
  size_t alpn_protocol_list_length;
};

struct tsi_ssl_server_handshaker_factory {
  /* Several contexts to support SNI.
     The tsi_peer array contains the subject names of the server certificates
     associated with the contexts at the same index.  */
  tsi_ssl_handshaker_factory base;
  tsi_ssl_ctx** ssl_ctxs;
  tsi_peer* ssl_context_x509_subject_names;
  size_t ssl_ctx_count;
  unsigned char** alpn_protocol_list;
  size_t alpn_protocol_list_length;
};

typedef struct ssl_buf {
  unsigned char *data;
  size_t len;
  size_t offset;
  size_t max_len;
} ssl_buf;

typedef struct BIO {
  ssl_buf *into_ssl;
  ssl_buf *from_ssl;
} BIO;

typedef struct {
  tsi_handshaker base;
  tsi_ssl_ctx* ssl_ctx;
  BIO *bio;
  tsi_result result;
  tsi_ssl_handshaker_factory* factory_ref;
} tsi_ssl_handshaker;

typedef struct {
  tsi_frame_protector base;
  tsi_ssl_ctx* ssl_ctx;
  BIO *bio;
  unsigned char* buffer;
  size_t buffer_size;
  size_t buffer_offset;
} tsi_ssl_frame_protector;

/* Proto Function */
// #define SSL_CTX_free(x) ssl_ctx_free((&x))

static BIO *BIO_init(void);
static void BIO_destroy(BIO *bio);

static void SSL_CTX_free(tsi_ssl_ctx* ssl_ctx) {
  if (ssl_ctx == nullptr) {
    return;
  }

  if (!gpr_unref(&ssl_ctx->refcount)) {
    return;
  }

  mbedtls_ssl_free(&ssl_ctx->ssl);
  mbedtls_ssl_config_free(&ssl_ctx->conf);
  mbedtls_ctr_drbg_free(&ssl_ctx->ctr_drbg);
  mbedtls_entropy_free(&ssl_ctx->entropy);
  mbedtls_pk_free(&ssl_ctx->pkey);
  mbedtls_x509_crt_free(&ssl_ctx->ca);
  mbedtls_x509_crt_free(&ssl_ctx->cert);

  gpr_free(ssl_ctx->ciphersuites);
  gpr_free(ssl_ctx);
}

/* Debug for mbedtls SSL process */
static void grpc_tsi_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	const char *p;
	const char *basename;

	/* Extract basename from file */
	for (p = basename = file; *p != '\0'; p++)
		if (*p == '/' || *p == '\\') {
			basename = p + 1;
		}

	fprintf((FILE *)ctx, "%s:%04d: |%d| %s", basename, line, level, str);
	fflush((FILE *)ctx);
}

/* Returns 1 if name looks like an IP address, 0 otherwise.
   This is a very rough heuristic, and only handles IPv6 in hexadecimal form. */
static int looks_like_ip_address(const char* name) {
  tsi_mbedtls_log("DO\t : %s\n", __func__);
  size_t i;
  size_t dot_count = 0;
  size_t num_size = 0;
  for (i = 0; i < strlen(name); i++) {
    if (name[i] == ':') {
      /* IPv6 Address in hexadecimal form, : is not allowed in DNS names. */
      return 1;
    }
    if (name[i] >= '0' && name[i] <= '9') {
      if (num_size > 3) return 0;
      num_size++;
    } else if (name[i] == '.') {
      if (dot_count > 3 || num_size == 0) return 0;
      dot_count++;
      num_size = 0;
    } else {
      return 0;
    }
  }
  if (dot_count < 3 || num_size == 0) return 0;
  return 1;
}

/* Gets the subject CN from an X509 cert. */
static tsi_result ssl_get_x509_common_name(mbedtls_x509_crt* cert,
                                           unsigned char** utf8,
                                           size_t* utf8_size) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_result result = TSI_OK;
  int ret = 0;
  char buf[MBEDTLS_X509_MAX_DN_NAME_SIZE];
  size_t size = sizeof(buf);
  memset(buf, 0, size);

  ret = mbedtls_x509_dn_gets(buf, size, &cert->subject);
  if (ret < 0) {
    return TSI_INTERNAL_ERROR;
  }

  char *p_start = strstr(buf, "CN=");
  if (p_start == nullptr) {
    return TSI_NOT_FOUND;
  }
  p_start = p_start + 3;
  
  char *p_end = strstr(p_start, ",");
  if (p_end == nullptr) {
    p_end = buf + ret;
  }

  int len = p_end - p_start;
  *utf8 = (unsigned char*)gpr_zalloc(len + 1);
  if (*utf8 == nullptr) {
    return TSI_INTERNAL_ERROR;
  }
  memcpy(*utf8, p_start, len);
  (*utf8)[len] = '\0';
  *utf8_size = strlen((const char *)*utf8);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

/* Gets the subject CN of an X509 cert as a tsi_peer_property. */
static tsi_result peer_property_from_x509_common_name(
    mbedtls_x509_crt* cert, tsi_peer_property* property) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  unsigned char* common_name;
  size_t common_name_size;
  tsi_result result =
      ssl_get_x509_common_name(cert, &common_name, &common_name_size);
  if (result != TSI_OK) {
    if (result == TSI_NOT_FOUND) {
      common_name = nullptr;
      common_name_size = 0;
    } else {
      tsi_mbedtls_log("OUT[X]\t : %s\n", __func__);
      return result;
    }
  }
  result = tsi_construct_string_peer_property(
      TSI_X509_SUBJECT_COMMON_NAME_PEER_PROPERTY,
      common_name == nullptr ? "" : (const char*)common_name, common_name_size,
      property);
  gpr_free(common_name);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

/* Gets the X509 cert in PEM format as a tsi_peer_property. */
static tsi_result add_pem_certificate(mbedtls_x509_crt* cert, tsi_peer_property* property) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);  
  int ret = 0;
  size_t size = cert->raw.len * 2;
  unsigned char *contents = (unsigned char *)gpr_zalloc(size);
  if (contents == nullptr) {
    return TSI_INTERNAL_ERROR;
  }
  size_t len = 0;

  if ((ret = mbedtls_pem_write_buffer(PEM_BEGIN_CRT, PEM_END_CRT, cert->raw.p, cert->raw.len, contents, size, &len)) != 0) {
    tsi_mbedtls_log("OUT[X]\t : %s\n", __func__);
		return TSI_INTERNAL_ERROR;
	}

  tsi_result result = tsi_construct_string_peer_property(
      TSI_X509_PEM_CERT_PROPERTY, (const char*)contents, (size_t)len, property);

  gpr_free(contents);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

/* Gets the subject SANs from an X509 cert as a tsi_peer_property. */
/* Just support DNSName */
static tsi_result add_subject_alt_names_properties_to_peer(
    tsi_peer* peer, mbedtls_x509_sequence *subject_alt_names, size_t subject_alt_name_count) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
	mbedtls_asn1_buf *buf;
  mbedtls_asn1_sequence *cur = subject_alt_names;
  tsi_result result = TSI_OK;

  peer->property_count -= subject_alt_name_count;

  while(cur != nullptr) {
    buf = &cur->buf;

    char *name = (char *)gpr_zalloc(buf->len + 1);
    memcpy(name, buf->p, buf->len);
    name[buf->len] = '\0';

    result = tsi_construct_string_peer_property(
          TSI_X509_SUBJECT_ALTERNATIVE_NAME_PEER_PROPERTY, (const char*)name,
          strlen(name), &peer->properties[peer->property_count++]);

    gpr_free(name);
    cur = cur->next;    
    if (result != TSI_OK) break;
  }
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

/* Gets information about the peer's X509 cert as a tsi_peer object. */
static tsi_result peer_from_x509(mbedtls_x509_crt* cert, int include_certificate_type,
                                 tsi_peer* peer) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  size_t property_count;
  tsi_result result;
  size_t subject_alt_name_count = 0;
  mbedtls_asn1_sequence *cur = &cert->subject_alt_names;
  while(cur != nullptr) {
    subject_alt_name_count++;
    cur = cur->next;
  }
  property_count = (include_certificate_type ? (size_t)1 : 0) +
                   2 /* common name, certificate */
                   + subject_alt_name_count;

  result = tsi_construct_peer(property_count, peer);
  if (result != TSI_OK) return result;
  do {
    if (include_certificate_type) {
      result = tsi_construct_string_peer_property_from_cstring(
          TSI_CERTIFICATE_TYPE_PEER_PROPERTY, TSI_X509_CERTIFICATE_TYPE,
          &peer->properties[0]);
      if (result != TSI_OK) break;
    }
    result = peer_property_from_x509_common_name(
        cert, &peer->properties[include_certificate_type ? 1 : 0]);
    if (result != TSI_OK) break;

    result = add_pem_certificate(
        cert, &peer->properties[include_certificate_type ? 2 : 1]);
    if (result != TSI_OK) break;

    if (subject_alt_name_count != 0) {
      result = add_subject_alt_names_properties_to_peer(
          peer, &cert->subject_alt_names, subject_alt_name_count);
      if (result != TSI_OK) break;
    }
  } while (0);

  if (result != TSI_OK) tsi_peer_destruct(peer);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

/* Performs an SSL_read and handle errors. */
static int do_ssl_read(void *bio, unsigned char *buf, size_t len) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  BIO *impl = (BIO *)bio;
  
  size_t remained = impl->into_ssl->len - impl->into_ssl->offset;
  tsi_mbedtls_log("do_ssl_read() remained : %ld\n", remained);
  if (len < remained) {
    remained = len;
  }
  memcpy(buf, impl->into_ssl->data + impl->into_ssl->offset, remained);
  impl->into_ssl->offset += remained;
  /* If remained data is 0, reset offset & data len */
  if (impl->into_ssl->offset == impl->into_ssl->len) {
    impl->into_ssl->offset = 0;
    impl->into_ssl->len = 0;
  }
  tsi_mbedtls_log("do_ssl_read() len : %ld\n", len);
  tsi_mbedtls_log("do_ssl_read() impl->into_ssl->len : %ld\n", impl->into_ssl->len);
  tsi_mbedtls_log("do_ssl_read() impl->into_ssl->offset : %ld\n", impl->into_ssl->offset);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
	return (int)remained;
}

/* Performs an SSL_write and handle errors. */
static int do_ssl_write(void *bio, const unsigned char *data, size_t dataLen) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  BIO *impl = (BIO *)bio;
  int remained = impl->from_ssl->len - impl->from_ssl->offset;
  if (remained > 0) {
    memcpy(impl->from_ssl->data + impl->from_ssl->len, data, dataLen);
    impl->from_ssl->len += dataLen;
  } else {
    memcpy(impl->from_ssl->data, data, dataLen);
    impl->from_ssl->len = dataLen;
    impl->from_ssl->offset = 0;
  }

  tsi_mbedtls_log("do_ssl_write() dataLen : %ld\n", dataLen);
  tsi_mbedtls_log("do_ssl_write() impl->from_ssl->len : %ld\n", impl->from_ssl->len);
  tsi_mbedtls_log("do_ssl_write() impl->from_ssl->offset : %ld\n", impl->from_ssl->offset);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
	return (int)dataLen;
}

/* Loads an in-memory PEM certificate chain into the SSL context. */
static tsi_result ssl_ctx_use_certificate_chain(tsi_ssl_ctx* ssl_ctx,
                                                const char* pem_cert_chain,
                                                size_t pem_cert_chain_size) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_result result = TSI_OK;
  int ret = 0;
  mbedtls_x509_crt* certificate = &ssl_ctx->cert;
 
  do {
    if (certificate == nullptr) {
      result = TSI_INTERNAL_ERROR;
      break;
    }

    ret = mbedtls_x509_crt_parse(certificate, (const unsigned char*)pem_cert_chain, pem_cert_chain_size);
    if (ret != 0) {
      gpr_log(GPR_ERROR, "mbedtls_x509_crt_parse failed.[-0x%x]", -ret);
      result = TSI_INTERNAL_ERROR;
      break;
    }
  } while (0);

  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

/* Loads an in-memory PEM private key into the SSL context. */
static tsi_result ssl_ctx_use_private_key(tsi_ssl_ctx* ssl_ctx, const char* pem_key,
                                          size_t pem_key_size) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  int ret;
  tsi_result result = TSI_OK;
  mbedtls_pk_context *pkey = &ssl_ctx->pkey;

  do {
    if (pkey == nullptr) {
      result = TSI_INVALID_ARGUMENT;
      break;
    }
    ret = mbedtls_pk_parse_key(pkey, (const unsigned char *)pem_key, pem_key_size, NULL, 0);
    if (ret != 0) {
      gpr_log(GPR_ERROR, "mbedtls_pk_parse_key failed.[-0x%x]", -ret);
      result =  TSI_INVALID_ARGUMENT;
      break;
    }
  } while (0);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

/* Loads in-memory PEM verification certs into the SSL context and optionally
   returns the verification cert names (root_names can be NULL). */
static tsi_result ssl_ctx_load_verification_certs(tsi_ssl_ctx* ssl_ctx,
                                                  const char* pem_roots,
                                                  size_t pem_roots_size) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);                                                  
  tsi_result result = TSI_OK;
  int ret = 0;
  mbedtls_x509_crt* ca = &ssl_ctx->ca;

  do {
    if (ca == nullptr) {
      result = TSI_INTERNAL_ERROR;
      break;
    }

    ret = mbedtls_x509_crt_parse(ca, (const unsigned char *)pem_roots, pem_roots_size);
    if (ret < 0) {
      result = TSI_INTERNAL_ERROR;
      break;
    }
    mbedtls_ssl_conf_ca_chain(&ssl_ctx->conf, ca, nullptr);
  } while (0);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

/* TODO : Setup Ciphersuite String to convert from openssl to mbedtls */
typedef struct tsi_ciphersuite_t {
  const char *openssl_cipher_name;
  const char *mbedtls_cipher_name;
} tsi_ciphersuite_t;

static const tsi_ciphersuite_t ciphersuite_mbedtls[] = {
  { "ECDHE-RSA-AES128-SHA256", "TLS-ECDHE-RSA-WITH-AES-128-CBC-SHA256" },
  { "ECDHE-RSA-AES256-SHA384", "TLS-ECDHE-RSA-WITH-AES-256-CBC-SHA384" },
  { "ECDHE-RSA-AES128-GCM-SHA256", "TLS-ECDHE-RSA-WITH-AES-128-GCM-SHA256" },
  { "ECDHE-RSA-AES256-GCM-SHA384", "TLS-ECDHE-RSA-WITH-AES-256-GCM-SHA384" },
};

static int set_cipher_list_from_string(tsi_ssl_ctx *ssl_ctx, const char* cipher_list) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  // int *cipher = ssl_ctx->ciphersuites;
  int cnt = 0;
  size_t i, j;

  char buf[MBEDTLS_BUF_MAX_SIZE];
  size_t len = strlen(cipher_list);

  if (cipher_list == nullptr || len <= 0) {
    return -1;
  }

  int p = 0;
  for (i = 0;i <= len; i++) {
    if (cipher_list[i] == ':' || cipher_list[i] == '\0') {
      cnt++;
    }
  }

  /* TODO : Don't forget to free this variable when destroy ssl */
  ssl_ctx->ciphersuites = (int *)gpr_zalloc((cnt + 1) * sizeof(int));
  cnt = 0;

  for (i = 0;i <= len; i++) {
    if (cipher_list[i] == ':' || cipher_list[i] == '\0') {
      int length = i - p;
      strncpy(buf, &cipher_list[p], length);
      buf[length] = '\0';
      p = i + 1;
      for (j = 0;j < sizeof(ciphersuite_mbedtls);j++) {
        if (!strcmp(ciphersuite_mbedtls[j].openssl_cipher_name, buf)) {
          ssl_ctx->ciphersuites[cnt++] = mbedtls_ssl_get_ciphersuite_id(ciphersuite_mbedtls[j].mbedtls_cipher_name);
          break;
        }
      }
    }
  }
  ssl_ctx->ciphersuites[cnt] = 0;
  mbedtls_ssl_conf_ciphersuites(&ssl_ctx->conf, ssl_ctx->ciphersuites);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return 0;
}

/* Populates the SSL context with a private key and a cert chain, and sets the
   cipher list and the ephemeral ECDH key. */
static tsi_result populate_ssl_context(
    tsi_ssl_ctx* ssl_ctx, const tsi_ssl_pem_key_cert_pair* key_cert_pair,
    const char* cipher_list) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_result result = TSI_OK;
  if (key_cert_pair != nullptr) {
    if (key_cert_pair->cert_chain != nullptr) {
      result = ssl_ctx_use_certificate_chain(ssl_ctx, key_cert_pair->cert_chain,
                                             strlen(key_cert_pair->cert_chain) + 1);
      if (result != TSI_OK) {
        gpr_log(GPR_ERROR, "Invalid cert chain file.");
        return result;
      }
    }
    if (key_cert_pair->private_key != nullptr) {
      result = ssl_ctx_use_private_key(ssl_ctx, key_cert_pair->private_key,
                                       strlen(key_cert_pair->private_key) + 1);
      /* TODO : Verify private key and certs */
      if (result != TSI_OK) {
        gpr_log(GPR_ERROR, "Invalid private key.");
        return result != TSI_OK ? result : TSI_INVALID_ARGUMENT;
      }
    }
    mbedtls_ssl_conf_own_cert(&ssl_ctx->conf, &ssl_ctx->cert, &ssl_ctx->pkey);
  }
  if ((cipher_list != nullptr) && set_cipher_list_from_string(ssl_ctx, cipher_list) < 0) {
    gpr_log(GPR_ERROR, "Invalid cipher list: %s.", cipher_list);
    return TSI_INVALID_ARGUMENT;
  }
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return TSI_OK;
}

/* Extracts the CN and the SANs from an X509 cert as a peer object. */
static tsi_result extract_x509_subject_names_from_pem_cert(const char* pem_cert,
                                                           tsi_peer* peer) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_result result = TSI_OK;
  int ret = 0;
  mbedtls_x509_crt cert;
  mbedtls_x509_crt_init(&cert);

  ret = mbedtls_x509_crt_parse(&cert, (const unsigned char *)pem_cert, strlen(pem_cert) + 1);
  if (ret != 0) {
    gpr_log(GPR_ERROR, "mbedtls_x509_crt_parse failed.[-0x%x]", -ret);
    return result;
  }
  result = peer_from_x509(&cert, 0, peer);
  if (result != TSI_OK) {
    gpr_log(GPR_ERROR, "peer_from_x509 ERROR!");
    goto end;
  }

end:
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

/* Builds the alpn protocol name list according to rfc 7301. */
static tsi_result build_alpn_protocol_name_list(
    const char** alpn_protocols, uint16_t num_alpn_protocols,
    unsigned char*** protocol_name_list, size_t *protocol_name_list_length) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  uint16_t i;
  *protocol_name_list_length = num_alpn_protocols;
  unsigned char **list = (unsigned char **)gpr_zalloc(sizeof(unsigned char *) * ((*protocol_name_list_length) + 1));
  if (list == nullptr) {
    return TSI_INTERNAL_ERROR;
  }

  for (i = 0; i < num_alpn_protocols; i++) {
    size_t size = strlen(alpn_protocols[i]) + 1;
    list[i] = (unsigned char *)gpr_malloc(sizeof(unsigned char) * size);
    memcpy(list[i], alpn_protocols[i], size);
  }
  list[i] = nullptr;
  *protocol_name_list = list;
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return TSI_OK;
}

/* --- tsi_frame_protector methods implementation. ---*/

static tsi_result ssl_protector_protect(tsi_frame_protector* self,
                                        const unsigned char* unprotected_bytes,
                                        size_t* unprotected_bytes_size,
                                        unsigned char* protected_output_frames,
                                        size_t* protected_output_frames_size) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_ssl_frame_protector* impl = (tsi_ssl_frame_protector*)self;
  BIO *bio = impl->bio;
  int ret;
  int read_from_ssl;
  int remained;
  size_t available;
  size_t buf_size = MBEDTLS_BUF_MAX_SIZE;
  char buf[MBEDTLS_BUF_MAX_SIZE] = { 0, };

  /* First see if we have some pending data in the SSL BIO. */
  remained = bio->from_ssl->len - bio->from_ssl->offset;
  if (remained > 0) {
    *unprotected_bytes_size = 0;
    read_from_ssl = remained;
    GPR_ASSERT(*protected_output_frames_size <= INT_MAX);
    memcpy(protected_output_frames, bio->from_ssl->data + bio->from_ssl->offset, read_from_ssl);
    *protected_output_frames_size = (size_t)read_from_ssl;
    tsi_mbedtls_log("OUT[0]\t : %s\n", __func__);
    return TSI_OK;
  }

  /* Now see if we can send a complete frame. */
  available = impl->buffer_size - impl->buffer_offset;
  if (available > *unprotected_bytes_size) {
    /* If we cannot, just copy the data in our internal buffer. */
    memcpy(impl->buffer + impl->buffer_offset, unprotected_bytes,
           *unprotected_bytes_size);
    impl->buffer_offset += *unprotected_bytes_size;
    *protected_output_frames_size = 0;
    tsi_mbedtls_log("OUT[1]\t : %s\n", __func__);
    return TSI_OK;
  }

  /* If we can, prepare the buffer, send it to SSL_write and read. */
  memcpy(impl->buffer + impl->buffer_offset, unprotected_bytes, available);
  /* The result will be stored in from_ssl by do_ssl_write() */
  ret = mbedtls_ssl_write(&(impl->ssl_ctx->ssl), impl->buffer, impl->buffer_size);	
  if (ret < 0) {
    if (ret != MBEDTLS_ERR_SSL_WANT_READ &&	ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
      mbedtls_strerror(ret, buf, buf_size);
      gpr_log(GPR_ERROR, "%s", buf);
      tsi_mbedtls_log("OUT[FAIL1]\t : %s\n", __func__);
      return TSI_INTERNAL_ERROR;
    }
  }

  GPR_ASSERT(*protected_output_frames_size <= INT_MAX);
  remained = bio->from_ssl->len - bio->from_ssl->offset;
  if (remained < 0) {
    gpr_log(GPR_ERROR, "Could not read from BIO after mbedtls_ssl_write.");
    tsi_mbedtls_log("OUT[FAIL2]\t : %s\n", __func__);
    return TSI_INTERNAL_ERROR;
  }
  read_from_ssl = ((size_t)remained > *protected_output_frames_size) ? *protected_output_frames_size : remained;
  memcpy(protected_output_frames, bio->from_ssl->data + bio->from_ssl->offset, read_from_ssl);
  bio->from_ssl->offset += read_from_ssl;
  
  *protected_output_frames_size = (size_t)read_from_ssl;
  *unprotected_bytes_size = available;
  impl->buffer_offset = 0;
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return TSI_OK;
}

static tsi_result ssl_protector_protect_flush(
    tsi_frame_protector* self, unsigned char* protected_output_frames,
    size_t* protected_output_frames_size, size_t* still_pending_size) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_ssl_frame_protector* impl = (tsi_ssl_frame_protector*)self;
  BIO *bio = impl->bio;
  int ret;
  int read_from_ssl = 0;
  int pending;
  size_t buf_size = MBEDTLS_BUF_MAX_SIZE;
  char buf[MBEDTLS_BUF_MAX_SIZE] = { 0, };

  if (impl->buffer_offset != 0) {
    ret = mbedtls_ssl_write(&(impl->ssl_ctx->ssl), impl->buffer, impl->buffer_offset);	
    if (ret < 0) {
      if (ret != MBEDTLS_ERR_SSL_WANT_READ &&	ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
        mbedtls_strerror(ret, buf, buf_size);
        gpr_log(GPR_ERROR, "%s", buf);
        tsi_mbedtls_log("OUT[FAIL]\t : %s\n", __func__);
        return TSI_INTERNAL_ERROR;
      }
    }
    impl->buffer_offset = 0;
  }

  pending = bio->from_ssl->len - bio->from_ssl->offset;
  GPR_ASSERT(pending >= 0);
  *still_pending_size = (size_t)pending;
  if (*still_pending_size == 0) return TSI_OK;

  GPR_ASSERT(*protected_output_frames_size <= INT_MAX);

  read_from_ssl = ((size_t)pending > *protected_output_frames_size) ? *protected_output_frames_size : pending;
  memcpy(protected_output_frames, bio->from_ssl->data + bio->from_ssl->offset, read_from_ssl);
  bio->from_ssl->offset += read_from_ssl;

  *protected_output_frames_size = (size_t)read_from_ssl;
  pending = bio->from_ssl->len - bio->from_ssl->offset;
  GPR_ASSERT(pending >= 0);
  *still_pending_size = (size_t)pending;
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return TSI_OK;
}

static tsi_result ssl_protector_unprotect(
    tsi_frame_protector* self, const unsigned char* protected_frames_bytes,
    size_t* protected_frames_bytes_size, unsigned char* unprotected_bytes,
    size_t* unprotected_bytes_size) {
  tsi_mbedtls_log("IN\t : %s[%ld]\n", __func__, *protected_frames_bytes_size);
  tsi_result result = TSI_OK;
  int written_into_ssl = 0;
  size_t output_bytes_size = *unprotected_bytes_size;
  size_t output_bytes_offset = 0;
  tsi_ssl_frame_protector* impl = (tsi_ssl_frame_protector*)self;
  BIO *bio = impl->bio;
  int ret;
  // int remained;
  size_t buf_size = MBEDTLS_BUF_MAX_SIZE;
  char buf[MBEDTLS_BUF_MAX_SIZE] = { 0, };

  /* First, try to read remaining data from ssl. */
  // remained = bio->into_ssl->len - bio->into_ssl->offset;
  // *unprotected_bytes_size = 0;
  // while (remained > 0) {
  //   do {
  //     ret = mbedtls_ssl_read(&(impl->ssl_ctx->ssl), unprotected_bytes, output_bytes_size - *unprotected_bytes_size);
  //   } while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);
  //   if (ret == 0) break;
  //   if (ret < 0) {
  //     mbedtls_strerror(ret, buf, buf_size);
  //     gpr_log(GPR_ERROR, "%s", buf);
  //     return TSI_DATA_CORRUPTED; 
  //   }
    
  //   unprotected_bytes += (size_t)ret;
  //   *unprotected_bytes_size += (size_t)ret;
  //   remained = bio->into_ssl->len - bio->into_ssl->offset;

  //   if (*unprotected_bytes_size > output_bytes_size) {
  //     *protected_frames_bytes_size = 0;
  //     return TSI_OUT_OF_RESOURCES;
  //   } else if (*unprotected_bytes_size == output_bytes_size) {
  //     /* We have read everything we could and cannot process any more input. */
  //     *protected_frames_bytes_size = 0;
  //     return TSI_OK;
  //   }
  // }
  // output_bytes_offset = *unprotected_bytes_size;
  // unprotected_bytes += output_bytes_offset;
  // *unprotected_bytes_size = output_bytes_size - *unprotected_bytes_size;

  do {
    ret = mbedtls_ssl_read(&(impl->ssl_ctx->ssl), unprotected_bytes, *unprotected_bytes_size);
  } while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);
  if (ret < 0) {
    mbedtls_strerror(ret, buf, buf_size);
    gpr_log(GPR_ERROR, "%s", buf);
    return TSI_DATA_CORRUPTED; 
  }
  *unprotected_bytes_size = (size_t)ret;

  if (*unprotected_bytes_size == output_bytes_size) {
      /* We have read everything we could and cannot process any more input. */
      *protected_frames_bytes_size = 0;
      return TSI_OK;
  }
  output_bytes_offset = *unprotected_bytes_size;
  unprotected_bytes += output_bytes_offset;
  *unprotected_bytes_size = output_bytes_size - output_bytes_offset;

  /* Then, try to write some data to ssl. */
  GPR_ASSERT(*protected_frames_bytes_size <= INT_MAX);
  written_into_ssl = (int)*protected_frames_bytes_size;
  if ((size_t)written_into_ssl > (bio->into_ssl->max_len - bio->into_ssl->len)) {
    written_into_ssl = bio->into_ssl->max_len - bio->into_ssl->len;
  }
  // bio->into_ssl->offset = 0;
  // bio->into_ssl->len = written_into_ssl;
  bio->into_ssl->len += written_into_ssl;
  tsi_mbedtls_log("!!![Unprotected] written_into_ssl : %d\n", written_into_ssl);
  tsi_mbedtls_log("!!![Unprotected] protected_frames_bytes_size : %ld\n", *protected_frames_bytes_size);
  memcpy(bio->into_ssl->data + bio->into_ssl->offset, protected_frames_bytes, written_into_ssl);
  *protected_frames_bytes_size = (size_t)written_into_ssl;

  /* Now try to read some data again. */
  do {
    ret = mbedtls_ssl_read(&(impl->ssl_ctx->ssl), unprotected_bytes, *unprotected_bytes_size);
  } while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);
  if (ret < 0) {
    if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
      ret = 0;
    } else {
      mbedtls_strerror(ret, buf, buf_size);
      gpr_log(GPR_ERROR, "%s", buf);
      return TSI_DATA_CORRUPTED; 
    }
  }
  *unprotected_bytes_size = (size_t)ret;
  *unprotected_bytes_size += output_bytes_offset;
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

/* TODO : Checking leak of memory */
static void ssl_protector_destroy(tsi_frame_protector* self) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_ssl_frame_protector* impl = (tsi_ssl_frame_protector*)self;
  if (impl->buffer != nullptr) {
    gpr_free(impl->buffer);
    impl->buffer = nullptr;
  }
  if (impl->bio != nullptr) {
    BIO_destroy(impl->bio);
    impl->bio = nullptr;
  }
  if (impl->ssl_ctx != nullptr) {
    SSL_CTX_free(impl->ssl_ctx);
    impl->ssl_ctx = nullptr;
  }
  gpr_free(self);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
}

static const tsi_frame_protector_vtable frame_protector_vtable = {
    ssl_protector_protect,
    ssl_protector_protect_flush,
    ssl_protector_unprotect,
    ssl_protector_destroy,
};

/* --- tsi_server_handshaker_factory methods implementation. --- */

static void tsi_ssl_handshaker_factory_destroy(
    tsi_ssl_handshaker_factory* self) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  if (self == nullptr) return;

  if (self->vtable != nullptr && self->vtable->destroy != nullptr) {
    self->vtable->destroy(self);
  }
  /* Note, we don't free(self) here because this object is always directly
   * embedded in another object. If tsi_ssl_handshaker_factory_init allocates
   * any memory, it should be free'd here. */
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
}

static tsi_ssl_handshaker_factory* tsi_ssl_handshaker_factory_ref(
    tsi_ssl_handshaker_factory* self) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);    
  if (self == nullptr) return nullptr;
  gpr_refn(&self->refcount, 1);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return self;
}

static void tsi_ssl_handshaker_factory_unref(tsi_ssl_handshaker_factory* self) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  if (self == nullptr) return;

  if (gpr_unref(&self->refcount)) {
    tsi_ssl_handshaker_factory_destroy(self);
  }
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
}

static tsi_ssl_handshaker_factory_vtable handshaker_factory_vtable = {nullptr};

/* Initializes a tsi_ssl_handshaker_factory object. Caller is responsible for
 * allocating memory for the factory. */
static void tsi_ssl_handshaker_factory_init(
    tsi_ssl_handshaker_factory* factory) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  GPR_ASSERT(factory != nullptr);

  factory->vtable = &handshaker_factory_vtable;
  gpr_ref_init(&factory->refcount, 1);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
}

/* --- tsi_handshaker methods implementation. ---*/

static tsi_result ssl_handshaker_get_bytes_to_send_to_peer(tsi_handshaker* self,
                                                           unsigned char* bytes,
                                                           size_t* bytes_size) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_ssl_handshaker* impl = (tsi_ssl_handshaker*)self;
  BIO *bio = impl->bio;
  int bytes_read_from_ssl = 0;
  if (bytes == nullptr || bytes_size == nullptr || *bytes_size == 0 ||
      *bytes_size > INT_MAX) {
    return TSI_INVALID_ARGUMENT;
  }
  tsi_mbedtls_log("!!!offset\t : %ld\n", bio->from_ssl->len);
  tsi_mbedtls_log("!!!len\t : %ld\n", bio->from_ssl->offset);
  GPR_ASSERT(*bytes_size <= INT_MAX);
  int pending = bio->from_ssl->len - bio->from_ssl->offset;
  bytes_read_from_ssl = (int)*bytes_size;
  if (pending < bytes_read_from_ssl) {
    bytes_read_from_ssl = pending;
  }
  tsi_mbedtls_log("!!!bytes_read_from_ssl : %d\n", bytes_read_from_ssl);
  tsi_mbedtls_log("!!!bytes_size\t : %ld\n", *bytes_size);
  memcpy(bytes, bio->from_ssl->data + bio->from_ssl->offset, bytes_read_from_ssl);
  bio->from_ssl->offset += bytes_read_from_ssl;
  pending -= bytes_read_from_ssl;
  
  tsi_mbedtls_log("!!!pending\t : %d\n", pending);
  *bytes_size = (size_t)bytes_read_from_ssl;
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return pending == 0 ? TSI_OK : TSI_INCOMPLETE_DATA;
}

static tsi_result ssl_handshaker_get_result(tsi_handshaker* self) {
  tsi_ssl_handshaker* impl = (tsi_ssl_handshaker*)self;
  tsi_mbedtls_log("SSL STATE \t : %d\n", impl->ssl_ctx->ssl.state);
  if ((impl->result == TSI_HANDSHAKE_IN_PROGRESS) &&
      impl->ssl_ctx->ssl.state == MBEDTLS_SSL_HANDSHAKE_OVER) {
    impl->result = TSI_OK;
  }
  return impl->result;
}

static tsi_result ssl_handshaker_process_bytes_from_peer(
    tsi_handshaker* self, const unsigned char* bytes, size_t* bytes_size) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_ssl_handshaker* impl = (tsi_ssl_handshaker*)self;
  BIO *bio = impl->bio;
  int bytes_written_into_ssl_size = 0;
  int ret;
  if (bytes == nullptr || bytes_size == nullptr || *bytes_size > INT_MAX) {
    return TSI_INVALID_ARGUMENT;
  }
  GPR_ASSERT(*bytes_size <= INT_MAX);

  bytes_written_into_ssl_size = (int)*bytes_size;
  if ((size_t)bytes_written_into_ssl_size > bio->into_ssl->max_len) {
    bytes_written_into_ssl_size = bio->into_ssl->max_len;
  }
  bio->into_ssl->offset = 0;
  bio->into_ssl->len = bytes_written_into_ssl_size;
  memcpy(bio->into_ssl->data, bytes, bytes_written_into_ssl_size);
  *bytes_size = (size_t)bytes_written_into_ssl_size;

  if (!tsi_handshaker_is_in_progress(self)) {
    impl->result = TSI_OK;
    return impl->result;
  }

  /* Get ready to get some bytes from SSL. */
  (void)ret;
  size_t remained = 0;
  if (impl->ssl_ctx->conf.endpoint == MBEDTLS_SSL_IS_CLIENT) {
	  while (MBEDTLS_SSL_HANDSHAKE_OVER != impl->ssl_ctx->ssl.state) {
      ret = mbedtls_ssl_handshake_step(&(impl->ssl_ctx->ssl));

      if (ret < 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
          gpr_log(GPR_ERROR, "failed! : mbedtls_ssl_handshake returned -0x%x\n", -ret);
          impl->result = TSI_PROTOCOL_FAILURE;
          return impl->result; 
        }
      }

      remained = impl->bio->into_ssl->len - impl->bio->into_ssl->offset;
      tsi_mbedtls_log("mbedtls_ssl_handshake_step() State : %d[%ld] / returned -0x%x\n", impl->ssl_ctx->ssl.state, remained, -ret);

      if (remained > 0) {
        continue;
      }

      if (impl->ssl_ctx->ssl.state == MBEDTLS_SSL_HANDSHAKE_OVER) {
        tsi_mbedtls_log("\t[ Protocol is %s ]\n\t[ Ciphersuite is %s ]\n", mbedtls_ssl_get_version(&impl->ssl_ctx->ssl), mbedtls_ssl_get_ciphersuite(&impl->ssl_ctx->ssl));
        break;
      }

      if (impl->ssl_ctx->ssl.state == MBEDTLS_SSL_SERVER_CHANGE_CIPHER_SPEC) {
        break;
      }
    }
  } else {
    while (MBEDTLS_SSL_HANDSHAKE_OVER != impl->ssl_ctx->ssl.state) {
      ret = mbedtls_ssl_handshake_step(&(impl->ssl_ctx->ssl));

      if (ret < 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
          gpr_log(GPR_ERROR, "failed\n  ! mbedtls_ssl_handshake returned -0x%x\n", -ret);
          impl->result = TSI_PROTOCOL_FAILURE;
          return impl->result; 
        }
      }

      remained = impl->bio->into_ssl->len - impl->bio->into_ssl->offset;
      tsi_mbedtls_log("mbedtls_ssl_handshake_step() State : %d[%ld] / returned -0x%x\n", impl->ssl_ctx->ssl.state, remained, -ret);

      if (remained > 0) {
        continue;
      }

      if (impl->ssl_ctx->ssl.state == MBEDTLS_SSL_HANDSHAKE_OVER) {
        tsi_mbedtls_log(" ok\n    [ Protocol is %s ]\n    [ Ciphersuite is %s ]\n", mbedtls_ssl_get_version(&impl->ssl_ctx->ssl), mbedtls_ssl_get_ciphersuite(&impl->ssl_ctx->ssl));
        break;
      }

      if (impl->ssl_ctx->ssl.state == MBEDTLS_SSL_CLIENT_CERTIFICATE) {
        break;
      }
    }
  }
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return TSI_OK;
}

static tsi_result ssl_handshaker_extract_peer(tsi_handshaker* self,
                                              tsi_peer* peer) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_result result = TSI_OK;
  const char* alpn_selected = nullptr;
  unsigned int alpn_selected_len;
  tsi_ssl_handshaker* impl = (tsi_ssl_handshaker*)self;

  const mbedtls_x509_crt *peer_temp = mbedtls_ssl_get_peer_cert(&(impl->ssl_ctx->ssl));
  if (peer_temp != nullptr) {
    mbedtls_x509_crt peer_cert[1];
    memcpy(peer_cert, peer_temp, sizeof(mbedtls_x509_crt));
    result = peer_from_x509(peer_cert, 1, peer);
    if (result != TSI_OK) return result;
  }

#if TSI_OPENSSL_ALPN_SUPPORT
  alpn_selected = mbedtls_ssl_get_alpn_protocol(&(impl->ssl_ctx->ssl));
  alpn_selected_len = strlen(alpn_selected);
#endif /* TSI_OPENSSL_ALPN_SUPPORT */

  if (alpn_selected != nullptr) {
    size_t i;
    tsi_peer_property* new_properties = (tsi_peer_property*)gpr_zalloc(
        sizeof(*new_properties) * (peer->property_count + 1));
    for (i = 0; i < peer->property_count; i++) {
      new_properties[i] = peer->properties[i];
    }
    result = tsi_construct_string_peer_property(
        TSI_SSL_ALPN_SELECTED_PROTOCOL, (const char*)alpn_selected,
        alpn_selected_len, &new_properties[peer->property_count]);
    if (result != TSI_OK) {
      gpr_free(new_properties);
      return result;
    }
    if (peer->properties != nullptr) gpr_free(peer->properties);
    peer->property_count++;
    peer->properties = new_properties;
  }
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

static tsi_result ssl_handshaker_create_frame_protector(
    tsi_handshaker* self, size_t* max_output_protected_frame_size,
    tsi_frame_protector** protector) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  size_t actual_max_output_protected_frame_size =
      TSI_SSL_MAX_PROTECTED_FRAME_SIZE_UPPER_BOUND;
  tsi_ssl_handshaker* impl = (tsi_ssl_handshaker*)self;
  tsi_ssl_frame_protector* protector_impl =
      (tsi_ssl_frame_protector*)gpr_zalloc(sizeof(*protector_impl));

  if (max_output_protected_frame_size != nullptr) {
    if (*max_output_protected_frame_size >
        TSI_SSL_MAX_PROTECTED_FRAME_SIZE_UPPER_BOUND) {
      *max_output_protected_frame_size =
          TSI_SSL_MAX_PROTECTED_FRAME_SIZE_UPPER_BOUND;
    } else if (*max_output_protected_frame_size <
               TSI_SSL_MAX_PROTECTED_FRAME_SIZE_LOWER_BOUND) {
      *max_output_protected_frame_size =
          TSI_SSL_MAX_PROTECTED_FRAME_SIZE_LOWER_BOUND;
    }
    actual_max_output_protected_frame_size = *max_output_protected_frame_size;
  }
  protector_impl->buffer_size =
      actual_max_output_protected_frame_size - TSI_SSL_MAX_PROTECTION_OVERHEAD;
  protector_impl->buffer =
      (unsigned char*)gpr_malloc(protector_impl->buffer_size);
  if (protector_impl->buffer == nullptr) {
    gpr_log(GPR_ERROR,
            "Could not allocated buffer for tsi_ssl_frame_protector.");
    gpr_free(protector_impl);
    return TSI_INTERNAL_ERROR;
  }

  /* Transfer ownership of ssl to the frame protector. It is OK as the caller
   * cannot call anything else but destroy on the handshaker after this call. */
  protector_impl->ssl_ctx = impl->ssl_ctx;
  impl->ssl_ctx = nullptr;
  protector_impl->bio = impl->bio;
  impl->bio = nullptr;

  protector_impl->base.vtable = &frame_protector_vtable;
  *protector = &protector_impl->base;
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return TSI_OK;
}

static void ssl_handshaker_destroy(tsi_handshaker* self) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_ssl_handshaker* impl = (tsi_ssl_handshaker*)self;
  // TODO : Free everything
  if (impl->bio != nullptr) {
    BIO_destroy(impl->bio);
    impl->bio = nullptr;
  }
  
  if (impl->ssl_ctx != nullptr) {
    SSL_CTX_free(impl->ssl_ctx);
    impl->ssl_ctx = nullptr;
  }

  tsi_ssl_handshaker_factory_unref(impl->factory_ref);
  gpr_free(impl);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
}

static const tsi_handshaker_vtable handshaker_vtable = {
    ssl_handshaker_get_bytes_to_send_to_peer,
    ssl_handshaker_process_bytes_from_peer,
    ssl_handshaker_get_result,
    ssl_handshaker_extract_peer,
    ssl_handshaker_create_frame_protector,
    ssl_handshaker_destroy,
    nullptr,
};

/* --- tsi_ssl_handshaker_factory common methods. --- */

static BIO *BIO_init() {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  BIO *bio = nullptr;
  ssl_buf *into_ssl = nullptr;
  ssl_buf *from_ssl = nullptr;
  unsigned char *into_data = nullptr;
  unsigned char *from_data = nullptr;
  
  bio = (BIO *)gpr_malloc(sizeof(BIO));
  if (bio == nullptr)  {
    goto error;
  }

  into_ssl = (ssl_buf *)gpr_malloc(sizeof(ssl_buf));
  if (into_ssl == nullptr)  {
    goto error;
  }
  into_ssl->max_len = MBEDTLS_SSL_MAX_CONTENT_LEN;
  into_data = (unsigned char *)gpr_malloc(into_ssl->max_len);
  if (into_data == nullptr)  {
    goto error;
  }
  into_ssl->data = into_data;
  into_ssl->len = 0;
  into_ssl->offset = 0;

  from_ssl = (ssl_buf *)gpr_malloc(sizeof(ssl_buf));
  if (from_ssl == nullptr)  {
    goto error;
  }
  from_ssl->max_len = MBEDTLS_SSL_MAX_CONTENT_LEN;
  from_data = (unsigned char *)gpr_malloc(into_ssl->max_len);
  if (from_data == nullptr)  {
    goto error;
  }
  from_ssl->data = from_data;
  from_ssl->len = 0;
  from_ssl->offset = 0;

  bio->into_ssl = into_ssl;
  bio->from_ssl = from_ssl;
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return bio;

error:
  if (from_data != nullptr) 
    gpr_free(from_data);
  if (into_data != nullptr) 
    gpr_free(into_data);
  if (from_ssl != nullptr) 
    gpr_free(from_ssl);
  if (into_ssl != nullptr) 
    gpr_free(into_ssl);
  if (bio != nullptr) 
    gpr_free(bio);
  tsi_mbedtls_log("OUT[X]\t : %s\n", __func__);
  return nullptr;
}

static void BIO_destroy(BIO *bio) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  if (bio->into_ssl->data != nullptr) {
    gpr_free(bio->into_ssl->data);
  }
  if (bio->from_ssl->data != nullptr) {
    gpr_free(bio->from_ssl->data);
  }
  if (bio->into_ssl != nullptr) {
    gpr_free(bio->into_ssl);
  }
  if (bio->from_ssl != nullptr) {
    gpr_free(bio->from_ssl);
  }
  gpr_free(bio);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
}

static tsi_result create_tsi_ssl_handshaker(tsi_ssl_ctx* ctx, int is_client,
                                            const char* server_name_indication,
                                            tsi_ssl_handshaker_factory* factory,
                                            tsi_handshaker** handshaker) {
  mbedtls_ssl_context *ssl = &ctx->ssl;
  mbedtls_ssl_config *conf = &ctx->conf;

  tsi_mbedtls_log("IN\t : %s\n", __func__);
  
  int ret = 0;

  BIO *bio = nullptr;
  tsi_ssl_handshaker* impl = nullptr;
  *handshaker = nullptr;
  if (ctx == nullptr) {
    gpr_log(GPR_ERROR, "SSL Context is null. Should never happen.");
    return TSI_INTERNAL_ERROR;
  }
 
  bio = BIO_init();
  if (bio == nullptr) {
    gpr_log(GPR_ERROR, "BIO_new failed.");
    if (bio != nullptr) {
      BIO_destroy(bio);
      bio = nullptr;
    }
    return TSI_OUT_OF_RESOURCES;
  }
  
  if (is_client) {
    if ((ret = mbedtls_ssl_setup(ssl, conf)) != 0) {
      gpr_log(GPR_ERROR, "mbedtls_ssl_setup returned -0x%x\n\n", -ret);
      return TSI_INTERNAL_ERROR;
    }
    /* Config Setting & Start HS with server_name : client-side */
    if (server_name_indication != nullptr) {
      if ((ret = mbedtls_ssl_set_hostname(ssl, server_name_indication)) != 0) {
        gpr_log(GPR_ERROR, "Invalid server name indication %s.",
                server_name_indication);
        // TODO : ssl free
        return TSI_INTERNAL_ERROR;
      }
    }
    mbedtls_ssl_set_bio(ssl, bio, do_ssl_write, do_ssl_read, nullptr);

    while (MBEDTLS_SSL_HANDSHAKE_OVER != ssl->state) {
      ret = mbedtls_ssl_handshake_step(ssl);
      tsi_mbedtls_log("mbedtls_ssl_handshake_step() State : %d / returned -0x%x\n", ssl->state, -ret);

      if (ret < 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
          gpr_log(GPR_ERROR, "failed\n  ! mbedtls_ssl_handshake returned -0x%x\n", -ret);
          impl->result = TSI_PROTOCOL_FAILURE;
          if (ret == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED) {
            gpr_log(GPR_ERROR, "Unable to verify the server's certificate. " "Either it is invalid,\n" "    or you didn't set ca_file or ca_path " "to an appropriate value.\n" "    Alternatively, you may want to use " "auth_mode=optional for testing purposes.");
          }
          // TODo : ssl_free
          return impl->result; 
        }
      }

      if (ssl->state == MBEDTLS_SSL_HANDSHAKE_OVER) {
        tsi_mbedtls_log(" ok\n    [ Protocol is %s ]\n    [ Ciphersuite is %s ]\n", mbedtls_ssl_get_version(ssl), mbedtls_ssl_get_ciphersuite(ssl));
        break;
      }

      if (ssl->state == MBEDTLS_SSL_SERVER_HELLO) {
        break;
      }
    }
  } else {
    if ((ret = mbedtls_ssl_setup(ssl, conf)) != 0) {
      gpr_log(GPR_ERROR, "mbedtls_ssl_setup returned -0x%x\n\n", -ret);
      return TSI_INTERNAL_ERROR;
    }
    mbedtls_ssl_set_bio(ssl, bio, do_ssl_write, do_ssl_read, nullptr);
  }

  impl = (tsi_ssl_handshaker*)gpr_zalloc(sizeof(*impl));
  impl->ssl_ctx = ctx;
  gpr_ref(&ctx->refcount);
  impl->bio = bio;
  impl->result = TSI_HANDSHAKE_IN_PROGRESS;
  impl->base.vtable = &handshaker_vtable;
  impl->factory_ref = tsi_ssl_handshaker_factory_ref(factory);

  *handshaker = &impl->base;
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return TSI_OK;
}

/* --- tsi_ssl_client_handshaker_factory methods implementation. --- */

tsi_result tsi_ssl_client_handshaker_factory_create_handshaker(
    tsi_ssl_client_handshaker_factory* self, const char* server_name_indication,
    tsi_handshaker** handshaker) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_result result = create_tsi_ssl_handshaker(self->ssl_ctx, 1, server_name_indication,
                                   &self->base, handshaker);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

void tsi_ssl_client_handshaker_factory_unref(
    tsi_ssl_client_handshaker_factory* self) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  if (self == nullptr) return;
  tsi_ssl_handshaker_factory_unref(&self->base);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
}

static void tsi_ssl_client_handshaker_factory_destroy(
    tsi_ssl_handshaker_factory* factory) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  if (factory == nullptr) return;
  tsi_ssl_client_handshaker_factory* self =
      (tsi_ssl_client_handshaker_factory*)factory;

  if (self->ssl_ctx != nullptr) {
    SSL_CTX_free(self->ssl_ctx);
    self->ssl_ctx = nullptr;
  }

  if (self->alpn_protocol_list != nullptr) {
    size_t i;
    for (i = 0; i < self->alpn_protocol_list_length;i++ ) {
      gpr_free(self->alpn_protocol_list[i]);
    }
    gpr_free(self->alpn_protocol_list);
  }  
  gpr_free(self);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return;
}

/* --- tsi_ssl_server_handshaker_factory methods implementation. --- */

tsi_result tsi_ssl_server_handshaker_factory_create_handshaker(
    tsi_ssl_server_handshaker_factory* self, tsi_handshaker** handshaker) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  if (self->ssl_ctx_count == 0) return TSI_INVALID_ARGUMENT;
  /* Create the handshaker with the first context. We will switch if needed
     because of SNI in ssl_server_handshaker_factory_servername_callback.  */
  tsi_result result = create_tsi_ssl_handshaker(self->ssl_ctxs[0], 0, nullptr,
                                   &self->base, handshaker);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

void tsi_ssl_server_handshaker_factory_unref(
    tsi_ssl_server_handshaker_factory* self) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  if (self == nullptr) return;
  tsi_ssl_handshaker_factory_unref(&self->base);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
}

static void tsi_ssl_server_handshaker_factory_destroy(
    tsi_ssl_handshaker_factory* factory) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  if (factory == nullptr) return;
  tsi_ssl_server_handshaker_factory* self =
      (tsi_ssl_server_handshaker_factory*)factory;
  size_t i;
  for (i = 0; i < self->ssl_ctx_count; i++) {
    if (self->ssl_ctxs[i] != nullptr) {
      SSL_CTX_free(self->ssl_ctxs[i]);
      self->ssl_ctxs[i] = nullptr;
      tsi_peer_destruct(&self->ssl_context_x509_subject_names[i]);
    }
  }
  if (self->ssl_ctxs != nullptr) gpr_free(self->ssl_ctxs);
  if (self->ssl_context_x509_subject_names != nullptr) {
    gpr_free(self->ssl_context_x509_subject_names);
  }
  if (self->alpn_protocol_list != nullptr) {
    for (i = 0; i < self->alpn_protocol_list_length;i++ ) {
      gpr_free(self->alpn_protocol_list[i]);
    }
    gpr_free(self->alpn_protocol_list);
  }
  gpr_free(self);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
}

static int does_entry_match_name(const char* entry, size_t entry_length,
                                 const char* name) {
  tsi_mbedtls_log("IN : %s\n", __func__);
  const char* dot;
  const char* name_subdomain = nullptr;
  size_t name_length = strlen(name);
  size_t name_subdomain_length;
  if (entry_length == 0) return 0;

  /* Take care of '.' terminations. */
  if (name[name_length - 1] == '.') {
    name_length--;
  }
  if (entry[entry_length - 1] == '.') {
    entry_length--;
    if (entry_length == 0) return 0;
  }

  if ((name_length == entry_length) &&
      strncmp(name, entry, entry_length) == 0) {
    return 1; /* Perfect match. */
  }
  if (entry[0] != '*') return 0;

  /* Wildchar subdomain matching. */
  if (entry_length < 3 || entry[1] != '.') { /* At least *.x */
    gpr_log(GPR_ERROR, "Invalid wildchar entry.");
    return 0;
  }
  name_subdomain = strchr(name, '.');
  if (name_subdomain == nullptr) return 0;
  name_subdomain_length = strlen(name_subdomain);
  if (name_subdomain_length < 2) return 0;
  name_subdomain++; /* Starts after the dot. */
  name_subdomain_length--;
  entry += 2; /* Remove *. */
  entry_length -= 2;
  dot = strchr(name_subdomain, '.');
  if ((dot == nullptr) || (dot == &name_subdomain[name_subdomain_length - 1])) {
    gpr_log(GPR_ERROR, "Invalid toplevel subdomain: %s", name_subdomain);
    return 0;
  }
  if (name_subdomain[name_subdomain_length - 1] == '.') {
    name_subdomain_length--;
  }
  tsi_mbedtls_log("OUT : %s\n", __func__);
  return ((entry_length > 0) && (name_subdomain_length == entry_length) &&
          strncmp(entry, name_subdomain, entry_length) == 0);
}

static int ssl_server_handshaker_factory_servername_callback(
    void *p_info, mbedtls_ssl_context *ssl, const unsigned char *name, size_t name_len) {

  tsi_ssl_server_handshaker_factory* impl = (tsi_ssl_server_handshaker_factory *)p_info;
  tsi_mbedtls_log("IN\t : %s\n", __func__);

  size_t i;
  if (name_len == 0) return -1;

  for (i = 0;i < impl->ssl_ctx_count; i++) {
    if (tsi_ssl_peer_matches_name(&impl->ssl_context_x509_subject_names[i], (const char *)name)) {
      mbedtls_ssl_set_hs_own_cert(ssl, &impl->ssl_ctxs[i]->cert, &impl->ssl_ctxs[i]->pkey);
      mbedtls_ssl_set_hs_ca_chain(ssl, &impl->ssl_ctxs[i]->ca, nullptr);
      tsi_mbedtls_log("OUT[OK]\t : %s\n", __func__);
      return 0;
    }
  }
  gpr_log(GPR_ERROR, "No match found for server name: %s.", name);
  tsi_mbedtls_log("OUT[X]\t : %s\n", __func__);
  return -1;
}

/* --- tsi_ssl_handshaker_factory constructors. --- */

static tsi_ssl_handshaker_factory_vtable client_handshaker_factory_vtable = {
    tsi_ssl_client_handshaker_factory_destroy};

tsi_result tsi_create_ssl_client_handshaker_factory(
    const tsi_ssl_pem_key_cert_pair* pem_key_cert_pair,
    const char* pem_root_certs, const char* cipher_suites,
    const char** alpn_protocols, uint16_t num_alpn_protocols,
    tsi_ssl_client_handshaker_factory** factory) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_ssl_ctx *ssl_ctx = nullptr;
  tsi_ssl_client_handshaker_factory* impl = nullptr;
  tsi_result result = TSI_OK;
  int ret;

  if (factory == nullptr) return TSI_INVALID_ARGUMENT;
  *factory = nullptr;
  if (pem_root_certs == nullptr) return TSI_INVALID_ARGUMENT;

  ssl_ctx = (tsi_ssl_ctx *)gpr_malloc(sizeof(tsi_ssl_ctx));
  if (ssl_ctx == nullptr) {
    gpr_log(GPR_ERROR, "Could not create ssl context.");
    return TSI_INVALID_ARGUMENT;
  }

  gpr_ref_init(&ssl_ctx->refcount, 1);

  mbedtls_ssl_init(&ssl_ctx->ssl);
  mbedtls_ssl_config_init(&ssl_ctx->conf);
  mbedtls_x509_crt_init(&ssl_ctx->cert);
  mbedtls_x509_crt_init(&ssl_ctx->ca);
  mbedtls_pk_init(&ssl_ctx->pkey);
  mbedtls_entropy_init(&ssl_ctx->entropy);
  mbedtls_ctr_drbg_init(&ssl_ctx->ctr_drbg);

  if ((ret = mbedtls_ssl_config_defaults(&ssl_ctx->conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
    gpr_log(GPR_ERROR, "mbedtls_ssl_config_defaults returned -0x%x\n\n", -ret);
    gpr_free(ssl_ctx);
    return TSI_INTERNAL_ERROR;
  }

  if ((ret = mbedtls_ctr_drbg_seed(&ssl_ctx->ctr_drbg, mbedtls_entropy_func, &ssl_ctx->entropy, (const unsigned char *)pers, strlen(pers))) != 0) {
    gpr_log(GPR_ERROR, "failed\n  ! mbedtls_ctr_drbg_seed returned -0x%x\n", -ret);
    gpr_free(ssl_ctx);
    return TSI_INTERNAL_ERROR;
  }

  mbedtls_ssl_conf_rng(&ssl_ctx->conf, mbedtls_ctr_drbg_random, &ssl_ctx->ctr_drbg);
  mbedtls_ssl_conf_dbg(&ssl_ctx->conf, grpc_tsi_debug, stdout);
#if defined(MBEDTLS_SSL_SESSION_TICKETS)  
  mbedtls_ssl_conf_session_tickets(&ssl_ctx->conf, MBEDTLS_SSL_SESSION_TICKETS_ENABLED);
#endif  

#if defined(MBEDTLS_DEBUG_C)
  mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

  impl = (tsi_ssl_client_handshaker_factory*)gpr_zalloc(sizeof(*impl));
  tsi_ssl_handshaker_factory_init(&impl->base);
  impl->base.vtable = &client_handshaker_factory_vtable;

  impl->ssl_ctx = ssl_ctx;

  do {
    result =
        populate_ssl_context(ssl_ctx, pem_key_cert_pair, cipher_suites);
    if (result != TSI_OK) break;
    result = ssl_ctx_load_verification_certs(ssl_ctx, pem_root_certs,
                                             strlen(pem_root_certs) + 1);

    if (result != TSI_OK) {
      gpr_log(GPR_ERROR, "Cannot load server root certificates.");
      break;
    }

    if (num_alpn_protocols != 0) {
      result = build_alpn_protocol_name_list(alpn_protocols, num_alpn_protocols,
                                             &impl->alpn_protocol_list,
                                             &impl->alpn_protocol_list_length);
      if (result != TSI_OK) {
        gpr_log(GPR_ERROR, "Building alpn list failed with error %s.",
                tsi_result_to_string(result));
        break;
      }
#if TSI_OPENSSL_ALPN_SUPPORT
      GPR_ASSERT(impl->alpn_protocol_list_length < UINT_MAX);
      mbedtls_ssl_conf_alpn_protocols(&ssl_ctx->conf, (const char **)impl->alpn_protocol_list);
#endif /* TSI_OPENSSL_ALPN_SUPPORT */
    }
  } while (0);
  if (result != TSI_OK) {
    tsi_ssl_handshaker_factory_unref(&impl->base);
    return result;
  }
  mbedtls_ssl_conf_authmode(&ssl_ctx->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
  /* TODO(jboeuf): Add revocation verification. */

  *factory = impl;
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

static tsi_ssl_handshaker_factory_vtable server_handshaker_factory_vtable = {
    tsi_ssl_server_handshaker_factory_destroy};

tsi_result tsi_create_ssl_server_handshaker_factory(
    const tsi_ssl_pem_key_cert_pair* pem_key_cert_pairs,
    size_t num_key_cert_pairs, const char* pem_client_root_certs,
    int force_client_auth, const char* cipher_suites,
    const char** alpn_protocols, uint16_t num_alpn_protocols,
    tsi_ssl_server_handshaker_factory** factory) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_result result = tsi_create_ssl_server_handshaker_factory_ex(
      pem_key_cert_pairs, num_key_cert_pairs, pem_client_root_certs,
      force_client_auth ? TSI_REQUEST_AND_REQUIRE_CLIENT_CERTIFICATE_AND_VERIFY
                        : TSI_DONT_REQUEST_CLIENT_CERTIFICATE,
      cipher_suites, alpn_protocols, num_alpn_protocols, factory);
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return result;
}

tsi_result tsi_create_ssl_server_handshaker_factory_ex(
    const tsi_ssl_pem_key_cert_pair* pem_key_cert_pairs,
    size_t num_key_cert_pairs, const char* pem_client_root_certs,
    tsi_client_certificate_request_type client_certificate_request,
    const char* cipher_suites, const char** alpn_protocols,
    uint16_t num_alpn_protocols, tsi_ssl_server_handshaker_factory** factory) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  tsi_ssl_server_handshaker_factory* impl = nullptr;
  tsi_result result = TSI_OK;
  size_t i = 0;
  int ret = 0;

  if (factory == nullptr) return TSI_INVALID_ARGUMENT;
  *factory = nullptr;
  if (num_key_cert_pairs == 0 || pem_key_cert_pairs == nullptr) {
    return TSI_INVALID_ARGUMENT;
  }

  impl = (tsi_ssl_server_handshaker_factory*)gpr_zalloc(sizeof(*impl));
  tsi_ssl_handshaker_factory_init(&impl->base);
  impl->base.vtable = &server_handshaker_factory_vtable;

  impl->ssl_ctxs =
      (tsi_ssl_ctx**)gpr_zalloc(num_key_cert_pairs * sizeof(tsi_ssl_ctx*));
  impl->ssl_context_x509_subject_names =
      (tsi_peer*)gpr_zalloc(num_key_cert_pairs * sizeof(tsi_peer));
  if (impl->ssl_ctxs == nullptr ||
      impl->ssl_context_x509_subject_names == nullptr) {
    tsi_ssl_handshaker_factory_unref(&impl->base);
    return TSI_OUT_OF_RESOURCES;
  }
  impl->ssl_ctx_count = num_key_cert_pairs;

  if (num_alpn_protocols > 0) {
    result = build_alpn_protocol_name_list(alpn_protocols, num_alpn_protocols,
                                           &impl->alpn_protocol_list, &impl->alpn_protocol_list_length);
    if (result != TSI_OK) {
      tsi_ssl_handshaker_factory_unref(&impl->base);
      return result;
    }
  }
  
  for (i = 0; i < num_key_cert_pairs; i++) {
    do {
      tsi_ssl_ctx *cur = (tsi_ssl_ctx *)gpr_zalloc(sizeof(tsi_ssl_ctx));
      impl->ssl_ctxs[i] = cur;
      if (impl->ssl_ctxs[i] == nullptr) {
        gpr_log(GPR_ERROR, "Could not create ssl context.");
        result = TSI_OUT_OF_RESOURCES;
        break;
      }

      gpr_ref_init(&cur->refcount, 1);

      mbedtls_ssl_init(&cur->ssl);
      mbedtls_ssl_config_init(&cur->conf);
      mbedtls_x509_crt_init(&cur->cert);
      mbedtls_x509_crt_init(&cur->ca);
      mbedtls_pk_init(&cur->pkey);
      mbedtls_entropy_init(&cur->entropy);
      mbedtls_ctr_drbg_init(&cur->ctr_drbg);
#if defined(MBEDTLS_SSL_SESSION_TICKETS)
      mbedtls_ssl_ticket_init(&cur->ticket_ctx);
#endif

      if ((ret = mbedtls_ssl_config_defaults(&cur->conf, MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
        gpr_log(GPR_ERROR, "mbedtls_ssl_config_defaults returned -0x%x\n\n", -ret);
        result = TSI_INTERNAL_ERROR;
        break;
      }

      if ((ret = mbedtls_ctr_drbg_seed(&cur->ctr_drbg, mbedtls_entropy_func, &cur->entropy, (const unsigned char *)pers, strlen(pers))) != 0) {
        gpr_log(GPR_ERROR, "failed\n  ! mbedtls_ctr_drbg_seed returned -0x%x\n", -ret);
        result = TSI_INTERNAL_ERROR;
        break;
      }

      mbedtls_ssl_conf_rng(&cur->conf, mbedtls_ctr_drbg_random, &cur->ctr_drbg);
      mbedtls_ssl_conf_dbg(&cur->conf, grpc_tsi_debug, stdout);

#if defined(MBEDTLS_SSL_SESSION_TICKETS)
      if((ret = mbedtls_ssl_ticket_setup(&cur->ticket_ctx, mbedtls_ctr_drbg_random, &cur->ctr_drbg, MBEDTLS_CIPHER_AES_256_GCM, 86400)) != 0) {
        gpr_log(GPR_ERROR, "mbedtls_ssl_ticket_setup returned %d", ret);
        result = TSI_INTERNAL_ERROR;
        break;
      }
      mbedtls_ssl_conf_session_tickets_cb(&cur->conf, mbedtls_ssl_ticket_write, mbedtls_ssl_ticket_parse, &cur->ticket_ctx);
#endif                  
#if defined(MBEDTLS_DEBUG_C)
      mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

      result = populate_ssl_context(cur, &pem_key_cert_pairs[i], cipher_suites);
      if (result != TSI_OK) break;

      if (pem_client_root_certs != nullptr) {
        result = ssl_ctx_load_verification_certs(
            impl->ssl_ctxs[i], pem_client_root_certs,
            strlen(pem_client_root_certs));
        if (result != TSI_OK) {
          gpr_log(GPR_ERROR, "Invalid verification certs.");
          break;
        }

        switch (client_certificate_request) {
          case TSI_DONT_REQUEST_CLIENT_CERTIFICATE:
            mbedtls_ssl_conf_authmode(&cur->conf, MBEDTLS_SSL_VERIFY_NONE);
            break;
          case TSI_REQUEST_CLIENT_CERTIFICATE_BUT_DONT_VERIFY:
          case TSI_REQUEST_CLIENT_CERTIFICATE_AND_VERIFY:
            mbedtls_ssl_conf_authmode(&cur->conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
            break;
          case TSI_REQUEST_AND_REQUIRE_CLIENT_CERTIFICATE_BUT_DONT_VERIFY:
          case TSI_REQUEST_AND_REQUIRE_CLIENT_CERTIFICATE_AND_VERIFY:
            mbedtls_ssl_conf_authmode(&cur->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
            break;
        }
        /* TODO(jboeuf): Add revocation verification. */
      }

      result = extract_x509_subject_names_from_pem_cert(
          pem_key_cert_pairs[i].cert_chain,
          &impl->ssl_context_x509_subject_names[i]);
      if (result != TSI_OK) break;
      mbedtls_ssl_conf_sni(&cur->conf, ssl_server_handshaker_factory_servername_callback, impl);
#if TSI_OPENSSL_ALPN_SUPPORT
      mbedtls_ssl_conf_alpn_protocols(&cur->conf, (const char **)impl->alpn_protocol_list);
#endif /* TSI_OPENSSL_ALPN_SUPPORT */
    } while (0);

    if (result != TSI_OK) {
      tsi_ssl_handshaker_factory_unref(&impl->base);
      return result;
    }
  }

  *factory = impl;
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return TSI_OK;
}

/* --- tsi_ssl utils. --- */

int tsi_ssl_peer_matches_name(const tsi_peer* peer, const char* name) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  size_t i = 0;
  size_t san_count = 0;
  const tsi_peer_property* cn_property = nullptr;
  int like_ip = looks_like_ip_address(name);

  /* Check the SAN first. */
  for (i = 0; i < peer->property_count; i++) {
    const tsi_peer_property* property = &peer->properties[i];
    if (property->name == nullptr) continue;
    if (strcmp(property->name,
               TSI_X509_SUBJECT_ALTERNATIVE_NAME_PEER_PROPERTY) == 0) {
      san_count++;

      int ret = does_entry_match_name(property->value.data, property->value.length, name);

      if (!like_ip && ret) {
        return 1;
      } else if (like_ip &&
                 strncmp(name, property->value.data, property->value.length) ==
                     0 &&
                 strlen(name) == property->value.length) {
        /* IP Addresses are exact matches only. */
        return 1;
      }
    } else if (strcmp(property->name,
                      TSI_X509_SUBJECT_COMMON_NAME_PEER_PROPERTY) == 0) {
      cn_property = property;
    }
  }

  /* If there's no SAN, try the CN, but only if its not like an IP Address */
  if (san_count == 0 && cn_property != nullptr && !like_ip) {
    if (does_entry_match_name(cn_property->value.data,
                              cn_property->value.length, name)) {
      return 1;
    }
  }
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return 0; /* Not found. */
}

/* --- Testing support. --- */
const tsi_ssl_handshaker_factory_vtable* tsi_ssl_handshaker_factory_swap_vtable(
    tsi_ssl_handshaker_factory* factory,
    tsi_ssl_handshaker_factory_vtable* new_vtable) {
  tsi_mbedtls_log("IN\t : %s\n", __func__);
  GPR_ASSERT(factory != nullptr);
  GPR_ASSERT(factory->vtable != nullptr);

  const tsi_ssl_handshaker_factory_vtable* orig_vtable = factory->vtable;
  factory->vtable = new_vtable;
  tsi_mbedtls_log("OUT\t : %s\n", __func__);
  return orig_vtable;
}
