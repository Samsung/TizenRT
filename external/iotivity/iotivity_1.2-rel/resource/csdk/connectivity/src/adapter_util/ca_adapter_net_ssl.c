/******************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#define MBEDTLS_SSL_SRV_RESPECT_CLIENT_PREFERENCE

#define _GNU_SOURCE

#include <stddef.h>
#include <stdbool.h>
#include "ca_adapter_net_ssl.h"
#include "cacommon.h"
#include "caipinterface.h"
#include "oic_malloc.h"
#include "ocrandom.h"
#include "byte_array.h"
#include "octhread.h"
#include "timer.h"


// headers required for mbed TLS
#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/pkcs12.h"
#include "mbedtls/ssl_internal.h"
#include "mbedtls/net.h"
#ifdef __WITH_DTLS__
#include "mbedtls/timing.h"
#include "mbedtls/ssl_cookie.h"
#endif
#include "pkix_interface.h"

#if !defined(NDEBUG) || defined(TB_LOG)
#include "mbedtls/debug.h"
#include "mbedtls/version.h"
#endif

#ifdef __unix__
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif


/**
 * @def MBED_TLS_VERSION_LEN
 * @brief mbedTLS version string length
 */
#define MBED_TLS_VERSION_LEN (16)
/**
 * @def SEED
 * @brief Seed for initialization RNG
 */
#define SEED "IOTIVITY_RND"
/**
 * @def UUID_PREFIX
 * @brief uuid prefix in certificate subject field
 */
#define UUID_PREFIX "uuid:"
/**
 * @def USERID_PREFIX
 * @brief userid prefix in certificate alternative subject name field
 */
#define USERID_PREFIX "userid:"

/**
 * @def NET_SSL_TAG
 * @brief Logging tag for module name
 */
#define NET_SSL_TAG "OIC_CA_NET_SSL"
/**
 * @def MBED_TLS_TAG
 * @brief Logging tag for mbedTLS library
 */
#define MBED_TLS_TAG "MBED_TLS"
/**
 * @def MMBED_TLS_DEBUG_LEVEL
 * @brief Logging level for mbedTLS library
 */
#define MBED_TLS_DEBUG_LEVEL (4)

/**
 * @def TLS_MSG_BUF_LEN
 * @brief Buffer size for TLS record. A single TLS record may be up to 16384 octets in length
 */
#if defined (__TIZENRT__)
#define TLS_MSG_BUF_LEN (4096)
#else
#define TLS_MSG_BUF_LEN (16384)
#endif

/**
 * @def PSK_LENGTH
 * @brief PSK keys max length
 */
#define PSK_LENGTH (256/8)
/**
 * @def UUID_LENGTHPSK_LENGTH
 * @brief Identity max length
 */
#define UUID_LENGTH (128/8)
/**
 * @def MASTER_SECRET_LEN
 * @brief TLS master secret length
 */
#define MASTER_SECRET_LEN (48)
/**
 * @def RANDOM_LEN
 * @brief TLS client and server random bytes length
 */
#define RANDOM_LEN (32)
/**
 * @def SHA384_MAC_KEY_LENGTH
 * @brief MAC key length for SHA384 cipher suites
 */
#define SHA384_MAC_KEY_LENGTH (48)
/**
 * @def SHA256_MAC_KEY_LENGTH
 * @brief MAC key length for SHA256 cipher suites
 */
#define SHA256_MAC_KEY_LENGTH (32)
/**
 * @def CCM_MAC_KEY_LENGTH
 * @brief MAC key length for CCM cipher suites
 */
#define CCM_MAC_KEY_LENGTH (0)
/**
 * @def AES256_KEY_LENGTH
 * @brief key material length for AES256 cipher suites
 */
#define AES256_KEY_LENGTH (32)
/**
 * @def AES128_KEY_LENGTH
 * @brief key material length for AES128 cipher suites
 */
#define AES128_KEY_LENGTH (16)
/**
 * @def GCM_IV_LENGTH
 * @brief length of nonce for GCM cipher suites
 */
#define GCM_IV_LENGTH (12)
/**
 * @def CCM_IV_LENGTH
 * @brief length of nonce for CCM cipher suites
 */
#define CCM_IV_LENGTH (4)
/**
 * @def CBC_IV_LENGTH
 * @brief length of nonce for CBC cipher suites
 */
#define CBC_IV_LENGTH (0)

/**
 * @var RETRANSMISSION_TIME
 * @brief Maximum timeout value (in seconds) to start DTLS retransmission.
 */
#define RETRANSMISSION_TIME 1

/**@def SSL_CLOSE_NOTIFY(peer, ret)
 *
 * Notifies of existing \a peer about closing TLS connection.
 *
 * @param[in] peer remote peer
 * @param[in] ret used internaly
 */
#define SSL_CLOSE_NOTIFY(peer, ret)                                                                \
do                                                                                                 \
{                                                                                                  \
    (ret) = mbedtls_ssl_close_notify(&(peer)->ssl);                                                \
} while (MBEDTLS_ERR_SSL_WANT_WRITE == (ret))

/**@def SSL_RES(peer, status)
 *
 * Sets SSL result for callback.
 *
 * @param[in] peer remote peer
 */
#define SSL_RES(peer, status)                                                                      \
if (g_sslCallback)                                                                                 \
{                                                                                                  \
    CAErrorInfo_t errorInfo;                                                                       \
    errorInfo.result = (status);                                                                   \
    g_sslCallback(&(peer)->sep.endpoint, &errorInfo);                                              \
}
/**@def SSL_CHECK_FAIL(peer, ret, str, mutex, error, msg)
 *
 * Checks handshake result and send alert if needed.
 *
 * @param[in] peer remote peer
 * @param[in] ret error code
 * @param[in] str debug string
 * @param[in] mutex ca mutex
 * @param[in] if code does not equal to -1 returns error code
 * @param[in] msg allert message
 */
#define SSL_CHECK_FAIL(peer, ret, str, mutex, error, msg)                                          \
if (0 != (ret) && MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY != (int) (ret) &&                              \
    MBEDTLS_ERR_SSL_HELLO_VERIFY_REQUIRED != (int) (ret) &&                                        \
    MBEDTLS_ERR_SSL_WANT_READ != (int) (ret) &&                                                    \
    MBEDTLS_ERR_SSL_WANT_WRITE != (int) (ret) &&                                                   \
    MBEDTLS_ERR_SSL_NON_FATAL != (int) (ret) &&                                                    \
    MBEDTLS_SSL_ALERT_MSG_USER_CANCELED != (int) (ret) &&                                          \
    MBEDTLS_SSL_ALERT_MSG_NO_RENEGOTIATION != (int) (ret) &&                                       \
    MBEDTLS_SSL_ALERT_MSG_UNSUPPORTED_EXT != (int) (ret) &&                                        \
    MBEDTLS_SSL_ALERT_MSG_CLOSE_NOTIFY != (int) (ret) &&                                           \
    MBEDTLS_SSL_ALERT_MSG_NO_CERT != (int) (ret) &&                                                \
    MBEDTLS_SSL_ALERT_MSG_BAD_CERT != (int) (ret) &&                                               \
    MBEDTLS_SSL_ALERT_MSG_UNSUPPORTED_CERT != (int) (ret) &&                                       \
    MBEDTLS_SSL_ALERT_MSG_CERT_REVOKED != (int) (ret) &&                                           \
    MBEDTLS_SSL_ALERT_MSG_CERT_EXPIRED != (int) (ret) &&                                           \
    MBEDTLS_SSL_ALERT_MSG_CERT_UNKNOWN != (int) (ret) &&                                           \
    MBEDTLS_SSL_ALERT_MSG_INAPROPRIATE_FALLBACK != (int) (ret) &&                                  \
    MBEDTLS_SSL_ALERT_MSG_UNRECOGNIZED_NAME != (int) (ret) &&                                      \
    MBEDTLS_SSL_ALERT_MSG_UNKNOWN_PSK_IDENTITY != (int) (ret) &&                                   \
    MBEDTLS_SSL_ALERT_MSG_NO_APPLICATION_PROTOCOL != (int) (ret))                                  \
{                                                                                                  \
    OIC_LOG_V(ERROR, NET_SSL_TAG, "%s: -0x%x", (str), -(ret));                                     \
    if ((int) MBEDTLS_ERR_SSL_FATAL_ALERT_MESSAGE != (int) (ret) &&                                \
        (int) MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO != (int) (ret))                                  \
    {                                                                                              \
        mbedtls_ssl_send_alert_message(&(peer)->ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL, (msg));        \
    }                                                                                              \
    RemovePeerFromList(&(peer)->sep.endpoint);                                                     \
    if (mutex)                                                                                     \
    {                                                                                              \
        oc_mutex_unlock(g_sslContextMutex);                                                        \
    }                                                                                              \
    if ((int) MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO != (int)(ret))                                   \
    {                                                                                              \
        SSL_RES((peer), CA_DTLS_AUTHENTICATION_FAILURE);                                           \
    }                                                                                              \
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);                                             \
    if (-1 != error)                                                                               \
    {                                                                                              \
        return (error);                                                                            \
    }                                                                                              \
}

/**@def CONF_SSL(clientConf, serverConf, fn, ...)
 *
 * Calls \a fn for \a clientConf and \a serverConf.
 *
 */
#define CONF_SSL(clientConf, serverConf, fn, ...) do {                                             \
fn((clientConf), __VA_ARGS__);                                                                     \
fn((serverConf), __VA_ARGS__);                                                                     \
} while (0)

/** @def CHECK_MBEDTLS_RET(f, ...)
 * A macro that checks \a f function return code
 *
 * If function returns error code it goes to error processing.
 *
 * @param[in] f  Function to call
 */
#define CHECK_MBEDTLS_RET(f, ...) do {                                                             \
int ret = (f)(__VA_ARGS__);                                                                        \
if (0 != ret) {                                                                                    \
    OIC_LOG_V(ERROR, NET_SSL_TAG, "%s returned -0x%04x\n", __func__, -(ret));                      \
    goto exit;                                                                                     \
} } while (0)

typedef enum
{
    SSL_RSA_WITH_AES_256_CBC_SHA256,
    SSL_RSA_WITH_AES_128_GCM_SHA256,
    SSL_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
    SSL_ECDHE_RSA_WITH_AES_256_GCM_SHA384,
    SSL_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
    SSL_ECDHE_ECDSA_WITH_AES_128_CCM_8,
    SSL_ECDHE_ECDSA_WITH_AES_128_CCM,
    SSL_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256,
    SSL_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384,
    SSL_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,
    SSL_ECDHE_PSK_WITH_AES_128_CBC_SHA256,
    SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA256,
    SSL_ECDH_ANON_WITH_AES_128_CBC_SHA256,
    SSL_CIPHER_MAX
} SslCipher_t;

typedef enum
{
    ADAPTER_CURVE_SECP256R1,
    ADAPTER_CURVE_MAX
} AdapterCurve_t;

static const int tlsCipher[SSL_CIPHER_MAX][2] =
{
    {MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA256, 0},
    {MBEDTLS_TLS_RSA_WITH_AES_128_GCM_SHA256, 0},
    {MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256, 0},
    {MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384, 0},
    {MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256, 0},
    {MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8, 0},
    {MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM, 0},
    {MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256, 0},
    {MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384, 0},
    {MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384, 0},
    {MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256, 0},
    {MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256, 0},
    {MBEDTLS_TLS_ECDH_ANON_WITH_AES_128_CBC_SHA256, 0}
};

static int g_cipherSuitesList[SSL_CIPHER_MAX];

mbedtls_ecp_group_id curve[ADAPTER_CURVE_MAX][2] =
{
    {MBEDTLS_ECP_DP_SECP256R1, MBEDTLS_ECP_DP_NONE}
};

static PkiInfo_t g_pkiInfo = {{NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}};

typedef struct  {
    int code;
    int alert;
} CrtVerifyAlert_t;

static const CrtVerifyAlert_t crtVerifyAlerts[] = {
    {MBEDTLS_X509_BADCERT_EXPIRED,       MBEDTLS_SSL_ALERT_MSG_CERT_EXPIRED},
    {MBEDTLS_X509_BADCERT_REVOKED,       MBEDTLS_SSL_ALERT_MSG_CERT_REVOKED},
    {MBEDTLS_X509_BADCERT_CN_MISMATCH,   MBEDTLS_SSL_ALERT_MSG_CERT_UNKNOWN},
    {MBEDTLS_X509_BADCERT_NOT_TRUSTED,   MBEDTLS_SSL_ALERT_MSG_UNKNOWN_CA},
    {MBEDTLS_X509_BADCRL_NOT_TRUSTED,    MBEDTLS_SSL_ALERT_MSG_UNKNOWN_CA},
    {MBEDTLS_X509_BADCRL_EXPIRED,        MBEDTLS_SSL_ALERT_MSG_INSUFFICIENT_SECURITY},
    {MBEDTLS_X509_BADCERT_MISSING,       MBEDTLS_SSL_ALERT_MSG_NO_CERT},
    {MBEDTLS_X509_BADCERT_SKIP_VERIFY,   MBEDTLS_SSL_ALERT_MSG_INSUFFICIENT_SECURITY},
    {MBEDTLS_X509_BADCERT_OTHER,         MBEDTLS_SSL_ALERT_MSG_INTERNAL_ERROR},
    {MBEDTLS_X509_BADCERT_FUTURE,        MBEDTLS_SSL_ALERT_MSG_BAD_CERT},
    {MBEDTLS_X509_BADCRL_FUTURE,         MBEDTLS_SSL_ALERT_MSG_INSUFFICIENT_SECURITY},
    {MBEDTLS_X509_BADCERT_KEY_USAGE,     MBEDTLS_SSL_ALERT_MSG_BAD_CERT},
    {MBEDTLS_X509_BADCERT_EXT_KEY_USAGE, MBEDTLS_SSL_ALERT_MSG_BAD_CERT},
    {MBEDTLS_X509_BADCERT_NS_CERT_TYPE,  MBEDTLS_SSL_ALERT_MSG_BAD_CERT},
    {MBEDTLS_X509_BADCERT_BAD_MD,        MBEDTLS_SSL_ALERT_MSG_BAD_CERT},
    {MBEDTLS_X509_BADCERT_BAD_PK,        MBEDTLS_SSL_ALERT_MSG_BAD_CERT},
    {MBEDTLS_X509_BADCERT_BAD_KEY,       MBEDTLS_SSL_ALERT_MSG_BAD_CERT},
    {MBEDTLS_X509_BADCRL_BAD_MD,         MBEDTLS_SSL_ALERT_MSG_BAD_CERT},
    {MBEDTLS_X509_BADCRL_BAD_PK,         MBEDTLS_SSL_ALERT_MSG_BAD_CERT},
    {MBEDTLS_X509_BADCRL_BAD_KEY,        MBEDTLS_SSL_ALERT_MSG_BAD_CERT},
    {0, 0}
};

static int GetAlertCode(uint32_t flags)
{
    const CrtVerifyAlert_t *cur;

    for (cur = crtVerifyAlerts; cur->alert != 0 ; cur++)
    {
        if (flags & cur->code)
        {
            return cur->alert;
        }
    }
    return 0;
}

#if !defined(NDEBUG) || defined(TB_LOG)
/**
 * Pass a message to the OIC logger.
 *
 * @param[in] ctx  opaque context for the callback
 * @param[in] level  debug level
 * @param[in] file  file name
 * @param[in] line  line number
 * @param[in] str  message
 */
static void DebugSsl(void *ctx, int level, const char *file, int line, const char *str)
{
    ((void) level);
    ((void) file);
    ((void) line);
    ((void) ctx);

    OIC_LOG_V(DEBUG, MBED_TLS_TAG, "%s", str);
}
#endif

#if defined(_WIN32) || defined (__TIZENRT__)
/*
 * Finds the first occurrence of the byte string s in byte string l.
 */

static void * memmem(const void *l, size_t lLen, const void *s, size_t sLen)
{
    char *cur;
    char *last;
    const char *cl = (const char *)l;
    const char *cs = (const char *)s;

    if (lLen == 0 || sLen == 0)
    {
        return NULL;
    }
    if (lLen < sLen)
    {
        return NULL;
    }
    if (sLen == 1)
    {
        return (void *)memchr(l, (int)*cs, lLen);
    }

    last = (char *)cl + lLen - sLen;

    for (cur = (char *)cl; cur <= last; cur++)
    {
        if (cur[0] == cs[0] && memcmp(cur, cs, sLen) == 0)
        {
            return cur;
        }
    }
    return NULL;
}
#endif
/**
 * structure to holds the information of cache message and address info.
 */
typedef ByteArray_t SslCacheMessage_t;


/**
 * Data structure for holding the send and recv callbacks.
 */
typedef struct TlsCallBacks
{
    CAPacketReceivedCallback recvCallback;  /**< Callback used to send data to upper layer. */
    CAPacketSendCallback sendCallback;      /**< Callback used to send data to socket layer. */
} SslCallbacks_t;

/**
 * Data structure for holding the mbedTLS interface related info.
 */
typedef struct SslContext
{
    u_arraylist_t *peerList;         /**< peer list which holds the mapping between
                                              peer id, it's n/w address and mbedTLS context. */
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context rnd;
    mbedtls_x509_crt ca;
    mbedtls_x509_crt crt;
    mbedtls_pk_context pkey;

    mbedtls_ssl_config clientTlsConf;
    mbedtls_ssl_config serverTlsConf;
    mbedtls_ssl_config clientDtlsConf;
    mbedtls_ssl_config serverDtlsConf;

    SslCipher_t cipher;
    SslCallbacks_t adapterCallbacks[MAX_SUPPORTED_ADAPTERS];
    mbedtls_x509_crl crl;
    bool cipherFlag[2];
    int selectedCipher;

#ifdef __WITH_DTLS__
    mbedtls_ssl_cookie_ctx cookieCtx;
    int timerId;
#endif
} SslContext_t;

/**
 * @var g_caSslContext
 * @brief global context which holds tls context and cache list information.
 */
static SslContext_t * g_caSslContext = NULL;

static SslExportKeysCallback_t gTlsExportKeysCallback = NULL;

static SslExportKeysCallback_t gDtlsExportKeysCallback = NULL;

/**
 * @var g_getCredentialsCallback
 * @brief callback to get TLS credentials (same as for DTLS)
 */
static CAgetPskCredentialsHandler g_getCredentialsCallback = NULL;
/**
 * @var g_getCerdentilTypesCallback
 * @brief callback to get different credential types from SRM
 */
static CAgetCredentialTypesHandler g_getCredentialTypesCallback = NULL;
/**
 * @var g_getPkixInfoCallback
 *
 * @brief callback to get X.509-based Public Key Infrastructure
 */
static CAgetPkixInfoHandler g_getPkixInfoCallback = NULL;

/**
 * @var g_setupPkContextCallback
 *
 * @brief callback to setup PK context handler for H/W based Public Key Infrastructure
 */
static CAsetupPkContextHandler g_setupPkContextCallback = NULL;

/**
 * @var g_dtlsContextMutex
 * @brief Mutex to synchronize access to g_caSslContext.
 */
static oc_mutex g_sslContextMutex = NULL;

/**
 * @var g_sslCallback
 * @brief callback to deliver the TLS handshake result
 */
static CAErrorCallback g_sslCallback = NULL;

/**
 * @var g_decryptBuffer
 * @brief decrypt buffer which will be used for decryption
 */
static uint8_t *g_decryptBuffer = NULL;

/**
 * Data structure for holding the data to be received.
 */
typedef struct SslRecBuf
{
    uint8_t * buff;
    size_t len;
    size_t loaded;
} SslRecBuf_t;
/**
 * Data structure for holding the data related to endpoint
 * and TLS session.
 */
typedef struct SslEndPoint
{
    mbedtls_ssl_context ssl;
    CASecureEndpoint_t sep;
    u_arraylist_t * cacheList;
    SslRecBuf_t recBuf;
    uint8_t master[MASTER_SECRET_LEN];
    uint8_t random[2*RANDOM_LEN];
#ifdef __WITH_DTLS__
    mbedtls_timing_delay_context timer;
#endif // __WITH_DTLS__
} SslEndPoint_t;

void CAsetPskCredentialsCallback(CAgetPskCredentialsHandler credCallback)
{
    // TODO Does this method needs protection of tlsContextMutex?
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    g_getCredentialsCallback = credCallback;
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
}

void CAsetPkixInfoCallback(CAgetPkixInfoHandler infoCallback)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    g_getPkixInfoCallback = infoCallback;
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
}

void CAsetSetupPkContextCallback(CAsetupPkContextHandler setupPkCtxCallback)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    g_setupPkContextCallback = setupPkCtxCallback;
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
}

void CAsetCredentialTypesCallback(CAgetCredentialTypesHandler credTypesCallback)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    g_getCredentialTypesCallback = credTypesCallback;
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
}

static int GetAdapterIndex(CATransportAdapter_t adapter)
{
    switch (adapter)
    {
        case CA_ADAPTER_IP:
            return 0;
        case CA_ADAPTER_TCP:
            return 1;
        case CA_ADAPTER_GATT_BTLE:
            return 2;
        default:
            OIC_LOG(ERROR, NET_SSL_TAG, "Unsupported adapter");
            return -1;
    }
}
/**
 * Write callback.
 *
 * @param[in]  tep    TLS endpoint
 * @param[in]  data    message
 * @param[in]  dataLen    message length
 *
 * @return  message length or -1 on error.
 */
static int SendCallBack(void * tep, const unsigned char * data, size_t dataLen)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(tep, NET_SSL_TAG, "secure endpoint is NULL", -1);
    VERIFY_NON_NULL_RET(data, NET_SSL_TAG, "data is NULL", -1);
    VERIFY_NON_NULL_RET(g_caSslContext, NET_SSL_TAG, "SSL Context is NULL", -1);
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Data len: %zu", dataLen);
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Adapter: %u", ((SslEndPoint_t * )tep)->sep.endpoint.adapter);
    ssize_t sentLen = 0;
    int adapterIndex = GetAdapterIndex(((SslEndPoint_t * )tep)->sep.endpoint.adapter);
    if (0 <= adapterIndex && MAX_SUPPORTED_ADAPTERS > adapterIndex)
    {
        CAPacketSendCallback sendCallback = g_caSslContext->adapterCallbacks[adapterIndex].sendCallback;
        sentLen = sendCallback(&(((SslEndPoint_t * )tep)->sep.endpoint), (const void *) data, dataLen);
        if (sentLen != dataLen)
        {
            OIC_LOG_V(DEBUG, NET_SSL_TAG,
                      "Packet was partially sent - total/sent/remained bytes : %d/%d/%d",
                      sentLen, dataLen, (dataLen - sentLen));
        }
    }
    else
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Unsupported adapter");
        return -1;
    }

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return sentLen;
}
/**
 * Read callback.
 *
 * @param[in]  tep    TLS endpoint
 * @param[in]  data    message
 * @param[in]  dataLen    message length
 *
 * @return  read length
 */
static int RecvCallBack(void * tep, unsigned char * data, size_t dataLen)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(tep, NET_SSL_TAG, "endpoint is NULL", 0);
    VERIFY_NON_NULL_RET(data, NET_SSL_TAG, "data is NULL", 0);

    SslRecBuf_t *recBuf = &((SslEndPoint_t *)tep)->recBuf;
    size_t retLen = (recBuf->len > recBuf->loaded ? recBuf->len - recBuf->loaded : 0);
    retLen = (retLen < dataLen ? retLen : dataLen);

    memcpy(data, recBuf->buff + recBuf->loaded, retLen);
    recBuf->loaded += retLen;

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return (int)retLen;
}

static int CASslExportKeysHandler(void *p_expkey,
                                const unsigned char *ms,
                                const unsigned char *kb,
                                size_t maclen,
                                size_t keylen,
                                size_t ivlen)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);

    if (NULL == g_caSslContext)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "SSL Context is not initialized.");
        return MBEDTLS_ERR_SSL_FEATURE_UNAVAILABLE;
    }
    if (NULL == p_expkey)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Can not find the protocol information from 'p_expkey'.");
        return MBEDTLS_ERR_SSL_FEATURE_UNAVAILABLE;
    }

    CASslEkcbProtocol_t* protocol = (CASslEkcbProtocol_t*)p_expkey;

    if (gTlsExportKeysCallback && CA_SSL_EKCB_TLS == (*protocol))
    {
        OIC_LOG(DEBUG, NET_SSL_TAG, "Invoking TLS export key callback.");
        gTlsExportKeysCallback(ms, kb, maclen, keylen, ivlen);
    }
    else if (gDtlsExportKeysCallback && CA_SSL_EKCB_DTLS == (*protocol))
    {
        OIC_LOG(DEBUG, NET_SSL_TAG, "Invoking DTLS export key callback.");
        gDtlsExportKeysCallback(ms, kb, maclen, keylen, ivlen);
    }
    else
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Failed to Invoke (D)TLS export key callback.");
        return MBEDTLS_ERR_SSL_FEATURE_UNAVAILABLE;
    }

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return 0;
}

CAResult_t CASetSslExportKeysCallback(SslExportKeysCallback_t exportKeysCb,
                                      CASslEkcbProtocol_t protocol, CASslEkcbRole_t role)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    mbedtls_ssl_config* sslConf = NULL;
    static CASslEkcbProtocol_t protocolCtx = CA_SSL_EKCB_TLS;

    if (CA_SSL_EKCB_TLS != protocol && CA_SSL_EKCB_DTLS != protocol)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Invaild protocol.");
        return CA_STATUS_INVALID_PARAM;
    }
    if (CA_SSL_EKCB_CLIENT != role && CA_SSL_EKCB_SERVER != role)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Invaild role.");
        return CA_STATUS_INVALID_PARAM;
    }

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "TLS Export Key Callback Type : [%s] [%s]",
              (CA_SSL_EKCB_TLS == protocol ? "TLS" : "DTLS"),
              (CA_SSL_EKCB_CLIENT == role ? "Client" : "Server"));

    oc_mutex_lock(g_sslContextMutex);

    if (NULL == g_caSslContext)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "SSL Context is not initialized.");
        oc_mutex_unlock(g_sslContextMutex);
        return CA_STATUS_NOT_INITIALIZED;
    }

    if (CA_SSL_EKCB_TLS == protocol)
    {
        gTlsExportKeysCallback = exportKeysCb;
        if (CA_SSL_EKCB_CLIENT == role)
        {
            sslConf = &g_caSslContext->clientTlsConf;
        }
        else
        {
            sslConf = &g_caSslContext->serverTlsConf;
        }
    }
    else
    {
        gDtlsExportKeysCallback = exportKeysCb;
        if (CA_SSL_EKCB_CLIENT == role)
        {
            sslConf = &g_caSslContext->clientDtlsConf;
        }
        else
        {
            sslConf = &g_caSslContext->serverDtlsConf;
        }
    }
    protocolCtx = protocol;

    if (NULL == exportKeysCb)
    {
        mbedtls_ssl_conf_export_keys_cb(sslConf, NULL, NULL);
        OIC_LOG(DEBUG, NET_SSL_TAG, "Export key callback unregistered.");
    }
    else
    {
        mbedtls_ssl_conf_export_keys_cb(sslConf, CASslExportKeysHandler, (void*)(&protocolCtx));
        OIC_LOG(DEBUG, NET_SSL_TAG, "Export key callback registered.");
    }
    oc_mutex_unlock(g_sslContextMutex);

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);

    return CA_STATUS_OK;
}

/**
 * Parse chain of X.509 certificates.
 *
 * @param[out] crt     container for X.509 certificates
 * @param[in]  data    buffer with X.509 certificates. Certificates may be in either in PEM
                       or DER format in a jumble, delimiting symbols does not matter.
 * @param[in]  bufLen  buffer length
 * @param[in]  errNum  number certificates that failed to parse
 *
 * @return  number of successfully parsed certificates or -1 on error
 */
static int ParseChain(mbedtls_x509_crt * crt, unsigned char * buf, int bufLen, int * errNum)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(crt, NET_SSL_TAG, "Param crt is NULL" , -1);
    VERIFY_NON_NULL_RET(buf, NET_SSL_TAG, "Param buf is NULL" , -1);

    char pemCertHeader[] = {
        0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x43, 0x45, 0x52,
        0x54, 0x49, 0x46, 0x49, 0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d
    };
    char pemCertFooter[] = {
        0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x45, 0x4e, 0x44, 0x20, 0x43, 0x45, 0x52, 0x54, 0x49,
        0x46, 0x49, 0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d
    };
    size_t pemCertHeaderLen = sizeof(pemCertHeader);
    size_t pemCertFooterLen = sizeof(pemCertFooter);

    size_t len = 0;
    unsigned char * tmp = NULL;
    int count = 0;
    int ret = 0;
    int pos = 0;

    *errNum = 0;
    while (pos < bufLen)
    {
        if (buf[pos] == 0x30 && buf[pos + 1] == 0x82)
        {
            tmp = (unsigned char *)buf + pos + 1;
            CHECK_MBEDTLS_RET(mbedtls_asn1_get_len, &tmp, buf + bufLen, &len);
            if (pos + len < bufLen)
            {
                ret = mbedtls_x509_crt_parse_der(crt, buf + pos, len + 4);
                if (0 == ret)
                {
                    count++;
                }
                else
                {
                    (*errNum)++;
                    OIC_LOG_V(ERROR, NET_SSL_TAG, "mbedtls_x509_crt_parse_der returned -0x%04x\n", -(ret));
                }
            }
            pos += len + 4;
        }
        else if ((buf + pos + pemCertHeaderLen < buf + bufLen) &&
                 (0 == memcmp(buf + pos, pemCertHeader, pemCertHeaderLen)))
        {
            void * endPos = NULL;
            endPos = memmem(&(buf[pos]), bufLen - pos, pemCertFooter, pemCertFooterLen);
            if (NULL == endPos)
            {
                OIC_LOG(ERROR, NET_SSL_TAG, "Error: end of PEM certificate not found.");
                OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
                return -1;
            }
            len = (char*)endPos - ((char*)buf + pos) + pemCertFooterLen;
            if (pos + len + 1 <= bufLen)
            {
                char con = buf[pos + len];
                buf[pos + len] = 0x00;
                ret = mbedtls_x509_crt_parse(crt, buf + pos, len + 1);
                if (0 == ret)
                {
                    count++;
                }
                else
                {
                    (*errNum)++;
                    OIC_LOG_V(ERROR, NET_SSL_TAG, "mbedtls_x509_crt_parse returned -0x%04x\n", -(ret));
                }
                buf[pos + len] = con;
            }
            else
            {
                unsigned char * lastCert = (unsigned char *)OICMalloc((len + 1) * sizeof(unsigned char));
                memcpy(lastCert, buf + pos, len);
                lastCert[len] = 0x00;
                ret = mbedtls_x509_crt_parse(crt, lastCert, len + 1);
                if (0 == ret)
                {
                    count++;
                }
                else
                {
                    (*errNum)++;
                    OIC_LOG_V(ERROR, NET_SSL_TAG, "mbedtls_x509_crt_parse returned -0x%04x\n", -(ret));
                }
                OICFree(lastCert);
            }
            pos += len;
        }
        else
        {
            pos++;
        }
    }
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "%s successfully parsed %d certificates", __func__, count);
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return count;

exit:
    return -1;
}

//Loads PKIX related information from SRM
static int InitPKIX(CATransportAdapter_t adapter)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(g_getPkixInfoCallback, NET_SSL_TAG, "PKIX info callback is NULL", -1);
    VERIFY_NON_NULL_RET(g_caSslContext, NET_SSL_TAG, "SSL Context is NULL", -1);

    mbedtls_x509_crt_free(&g_caSslContext->ca);
    mbedtls_x509_crt_free(&g_caSslContext->crt);
    mbedtls_pk_free(&g_caSslContext->pkey);
    mbedtls_x509_crl_free(&g_caSslContext->crl);

    mbedtls_x509_crt_init(&g_caSslContext->ca);
    mbedtls_x509_crt_init(&g_caSslContext->crt);
    mbedtls_pk_init(&g_caSslContext->pkey);
    mbedtls_x509_crl_init(&g_caSslContext->crl);

    mbedtls_ssl_config * serverConf = (adapter == CA_ADAPTER_IP ||
                                   adapter == CA_ADAPTER_GATT_BTLE ?
                                   &g_caSslContext->serverDtlsConf : &g_caSslContext->serverTlsConf);
    mbedtls_ssl_config * clientConf = (adapter == CA_ADAPTER_IP ||
                                   adapter == CA_ADAPTER_GATT_BTLE ?
                                   &g_caSslContext->clientDtlsConf : &g_caSslContext->clientTlsConf);

    // load pk key, cert, trust chain and crl
    if (g_getPkixInfoCallback)
    {
        g_getPkixInfoCallback(&g_pkiInfo);
    }

    // parse own certficate (optional)
    int ret;
    int errNum;
    int count = ParseChain(&g_caSslContext->crt, g_pkiInfo.crt.data, g_pkiInfo.crt.len, &errNum);
    if (0 >= count)
    {
        OIC_LOG(WARNING, NET_SSL_TAG, "Own certificate chain parsing error");
        goto required;
    }
    if (0 != errNum)
    {
        OIC_LOG_V(WARNING, NET_SSL_TAG, "Own certificate chain parsing error: %d certs failed to parse", errNum);
        goto required;
    }

    // parse private key if hw is not supported (optional)
    if(NULL == g_setupPkContextCallback)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "g_setupPkContextCallback is NULL");
        ret =  mbedtls_pk_parse_key(&g_caSslContext->pkey, g_pkiInfo.key.data, g_pkiInfo.key.len,
                                                                                   NULL, 0);
    }
    else
    {
        // setup hw pk context (optional)
        ret = g_setupPkContextCallback(&g_caSslContext->pkey);
        if (0 == ret)
        {
            // setup public parameter
            mbedtls_pk_type_t ktype = mbedtls_pk_get_type(&g_caSslContext->pkey);
            if (MBEDTLS_PK_ECKEY == ktype || MBEDTLS_PK_ECKEY_DH == ktype
                || MBEDTLS_PK_ECDSA == ktype)
            {
                OIC_LOG_V(DEBUG, NET_SSL_TAG, "Copy ecp public param from cert, keytype [%d]", ktype);
                mbedtls_ecp_keypair *eckey = (mbedtls_ecp_keypair*)g_caSslContext->crt.pk.pk_ctx;
                mbedtls_ecdsa_context *ecdsa = (mbedtls_ecdsa_context*)g_caSslContext->pkey.pk_ctx;
                if (ecdsa && eckey)
                {
                    ret = mbedtls_ecdsa_from_keypair(ecdsa, eckey);
                    if(0 != ret )
                    {
                        OIC_LOG_V(ERROR, NET_SSL_TAG, "Fail to copy public param [0x%x]", ret);
                    }
                }
                else
                {
                    OIC_LOG_V(WARNING, NET_SSL_TAG, "key-ctx(0x%x), cert-ctx(0x%x)", ecdsa, eckey);
                }
            }
        }
    }
    if (0 != ret)
    {
        OIC_LOG(WARNING, NET_SSL_TAG, "Key parsing error");
        goto required;
    }

    ret = mbedtls_ssl_conf_own_cert(serverConf, &g_caSslContext->crt, &g_caSslContext->pkey);
    if (0 != ret)
    {
        OIC_LOG(WARNING, NET_SSL_TAG, "Own certificate parsing error");
        goto required;
    }
    ret = mbedtls_ssl_conf_own_cert(clientConf, &g_caSslContext->crt, &g_caSslContext->pkey);
    if(0 != ret)
    {
        OIC_LOG(WARNING, NET_SSL_TAG, "Own certificate configuration error");
        goto required;
    }

    required:
    count = ParseChain(&g_caSslContext->ca, g_pkiInfo.ca.data, g_pkiInfo.ca.len, &errNum);
    if(0 >= count)
    {
        OIC_LOG(WARNING, NET_SSL_TAG, "CA chain in svr db was not parsed");
        OIC_LOG(WARNING, NET_SSL_TAG, "    but if working as server, chain not required");
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return -1;
    }
    if(0 != errNum)
    {
        OIC_LOG_V(WARNING, NET_SSL_TAG, "CA chain parsing warning: %d certs failed to parse", errNum);
    }

    ret = mbedtls_x509_crl_parse_der(&g_caSslContext->crl, g_pkiInfo.crl.data, g_pkiInfo.crl.len);
    if(0 != ret)
    {
        OIC_LOG(WARNING, NET_SSL_TAG, "CRL in svr db was not parsed");
        CONF_SSL(clientConf, serverConf, mbedtls_ssl_conf_ca_chain, &g_caSslContext->ca, NULL);
    }
    else
    {
        CONF_SSL(clientConf, serverConf, mbedtls_ssl_conf_ca_chain,
                 &g_caSslContext->ca, &g_caSslContext->crl);
    }

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return 0;
}

/*
 * PSK callback.
 *
 * @param[in]  notUsed     opaque context
 * @param[in]  ssl    mbedTLS context
 * @param[in]  desc    identity
 * @param[in]  descLen    identity length
 *
 * @return  0 on success any other return value will result in a denied PSK identity
 */
static int GetPskCredentialsCallback(void * notUsed, mbedtls_ssl_context * ssl,
                                     const unsigned char * desc, size_t descLen)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(g_getCredentialsCallback, NET_SSL_TAG, "Credential callback s NULL", -1);
    VERIFY_NON_NULL_RET(ssl, NET_SSL_TAG, "ssl pointer is NULL", -1);
    VERIFY_NON_NULL_RET(desc, NET_SSL_TAG, "desc pointer is NULL", -1);
    if (descLen > CA_MAX_ENDPOINT_IDENTITY_LEN)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "desc too long!");
        return -1;
    }
    (void) notUsed;
    uint8_t keyBuf[PSK_LENGTH] = {0};

    // Retrieve the credentials blob from security module
    int ret = g_getCredentialsCallback(CA_DTLS_PSK_KEY, desc, descLen, keyBuf, PSK_LENGTH);
    if (ret > 0)
    {
        memcpy(((SslEndPoint_t *) ssl)->sep.identity.id, desc, descLen);
        ((SslEndPoint_t *) ssl)->sep.identity.id_length = descLen;
        OIC_LOG(DEBUG, NET_SSL_TAG, "PSK:");
        OIC_LOG_BUFFER(DEBUG, NET_SSL_TAG, keyBuf, ret);

        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return(mbedtls_ssl_set_hs_psk(ssl, keyBuf, ret));
    }
    OIC_LOG_V(WARNING, NET_SSL_TAG, "Out %s", __func__);
    return -1;
}
/**
 * Gets session corresponding for endpoint.
 *
 * @param[in]  peer    remote address
 *
 * @return  TLS session or NULL
 */
static SslEndPoint_t *GetSslPeer(const CAEndpoint_t *peer)
{
    uint32_t listIndex = 0;
    uint32_t listLength = 0;
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(peer, NET_SSL_TAG, "TLS peer is NULL", NULL);
    VERIFY_NON_NULL_RET(g_caSslContext, NET_SSL_TAG, "SSL Context is NULL", NULL);

    SslEndPoint_t *tep = NULL;
    listLength = u_arraylist_length(g_caSslContext->peerList);
    for (listIndex = 0; listIndex < listLength; listIndex++)
    {
        tep = (SslEndPoint_t *) u_arraylist_get(g_caSslContext->peerList, listIndex);
        if (NULL == tep)
        {
            continue;
        }

        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Compare [%s:%d] and [%s:%d] for %d adapter",
                  peer->addr, peer->port, tep->sep.endpoint.addr, tep->sep.endpoint.port,
                  peer->adapter);

        if((peer->adapter == tep->sep.endpoint.adapter)
                && (0 == strncmp(peer->addr, tep->sep.endpoint.addr, MAX_ADDR_STR_SIZE_CA))
                && (peer->port == tep->sep.endpoint.port || CA_ADAPTER_GATT_BTLE == peer->adapter))
        {
            OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
            return tep;
        }
    }
    OIC_LOG(DEBUG, NET_SSL_TAG, "Return NULL");
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return NULL;
}

/**
 * Gets session corresponding for endpoint.
 *
 * @param[in]  peer    remote address
 *
 * @return  TLS session or NULL
 */
static SslEndPoint_t *GetSslPeerUsingUuid(const uint8_t *identity, size_t idLength)
{
    uint32_t listIndex = 0;
    uint32_t listLength = 0;
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(identity, NET_SSL_TAG, "Param identity is NULL", NULL);
    VERIFY_NON_NULL_RET(g_caSslContext, NET_SSL_TAG, "SSL Context is NULL", NULL);

    OIC_LOG(DEBUG, NET_SSL_TAG, "[Target UUID]");
    OIC_LOG_BUFFER(DEBUG, NET_SSL_TAG, identity, idLength);

    SslEndPoint_t *tep = NULL;
    listLength = u_arraylist_length(g_caSslContext->peerList);
    for (listIndex = 0; listIndex < listLength; listIndex++)
    {
        tep = (SslEndPoint_t *) u_arraylist_get(g_caSslContext->peerList, listIndex);
        if (NULL == tep)
        {
            continue;
        }

        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Compare UUID for [%s:%d]",
                  tep->sep.endpoint.addr, tep->sep.endpoint.port);

        if ((tep->sep.identity.id_length == idLength)
            && (0 == memcmp(identity, tep->sep.identity.id, idLength)))
        {
            OIC_LOG_V(DEBUG, NET_SSL_TAG, "Found matched UUID in [%s:%d]",
                      tep->sep.endpoint.addr, tep->sep.endpoint.port);
            OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
            return tep;
        }
    }
    OIC_LOG(DEBUG, NET_SSL_TAG, "Return NULL");
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return NULL;
}


#ifdef MULTIPLE_OWNER
/**
 * Gets CA secure endpoint info corresponding for endpoint.
 *
 * @param[in]  peer    remote address
 *
 * @return  CASecureEndpoint or NULL
 */
const CASecureEndpoint_t *GetCASecureEndpointData(const CAEndpoint_t* peer)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    
    oc_mutex_lock(g_sslContextMutex);
    if (NULL == g_caSslContext)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Context is NULL");
        oc_mutex_unlock(g_sslContextMutex);
        return NULL;
    }

    SslEndPoint_t* sslPeer = GetSslPeer(peer);
    if(sslPeer)
    {
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        oc_mutex_unlock(g_sslContextMutex);
        return &sslPeer->sep;
    }

    OIC_LOG(DEBUG, NET_SSL_TAG, "Return NULL");
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    oc_mutex_unlock(g_sslContextMutex);
    return NULL;
}
#endif

/**
 * Deletes cached message.
 *
 * @param[in]  msg    message
 */
static void DeleteCacheMessage(SslCacheMessage_t * msg)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_VOID(msg, NET_SSL_TAG, "msg");

    OICFree(msg->data);
    OICFree(msg);

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
}
/**
 * Deletes cached message list.
 *
 * @param[in] cacheList  list of cached messages
 */
static void DeleteCacheList(u_arraylist_t * cacheList)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_VOID(cacheList, NET_SSL_TAG, "cacheList");
    uint32_t listIndex = 0;
    uint32_t listLength = 0;

    listLength = u_arraylist_length(cacheList);
    for (listIndex = 0; listIndex < listLength; listIndex++)
    {
        SslCacheMessage_t * msg = (SslCacheMessage_t *) u_arraylist_get(cacheList, listIndex);
        if (NULL != msg)
        {
            DeleteCacheMessage(msg);
        }
    }
    u_arraylist_free(&cacheList);

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
}
/**
 * Deletes endpoint with session.
 *
 * @param[in]  tep    endpoint with session info
 */
static void DeleteSslEndPoint(SslEndPoint_t * tep)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_VOID(tep, NET_SSL_TAG, "tep");

    mbedtls_ssl_free(&tep->ssl);
    DeleteCacheList(tep->cacheList);
    OICFree(tep);
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
}
/**
 * Removes endpoint session from list.
 *
 * @param[in]  endpoint    remote address
 */
static void RemovePeerFromList(const CAEndpoint_t * endpoint)
{
    VERIFY_NON_NULL_VOID(g_caSslContext, NET_SSL_TAG, "SSL Context is NULL");
    VERIFY_NON_NULL_VOID(endpoint, NET_SSL_TAG, "endpoint");
    uint32_t listLength = u_arraylist_length(g_caSslContext->peerList);
    for (uint32_t listIndex = 0; listIndex < listLength; listIndex++)
    {
        SslEndPoint_t * tep = (SslEndPoint_t *)u_arraylist_get(g_caSslContext->peerList,listIndex);
        if (NULL == tep)
        {
            continue;
        }
        if(0 == strncmp(endpoint->addr, tep->sep.endpoint.addr, MAX_ADDR_STR_SIZE_CA)
                && (endpoint->port == tep->sep.endpoint.port))
        {
            u_arraylist_remove(g_caSslContext->peerList, listIndex);
            DeleteSslEndPoint(tep);
            return;
        }
    }
}
/**
 * Deletes session list.
 */
static void DeletePeerList()
{
    VERIFY_NON_NULL_VOID(g_caSslContext, NET_SSL_TAG, "SSL Context is NULL");

    uint32_t listLength = u_arraylist_length(g_caSslContext->peerList);
    for (uint32_t listIndex = 0; listIndex < listLength; listIndex++)
    {
        SslEndPoint_t * tep = (SslEndPoint_t *)u_arraylist_get(g_caSslContext->peerList,listIndex);
        if (NULL == tep)
        {
            continue;
        }
        if (MBEDTLS_SSL_HANDSHAKE_OVER == tep->ssl.state)
        {
            int ret = 0;
            do
            {
                ret = mbedtls_ssl_close_notify(&tep->ssl);
            }
            while (MBEDTLS_ERR_SSL_WANT_WRITE == ret);
        }
        DeleteSslEndPoint(tep);
    }
    u_arraylist_free(&g_caSslContext->peerList);
}

CAResult_t CAcloseSslConnection(const CAEndpoint_t *endpoint)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(endpoint, NET_SSL_TAG, "Param endpoint is NULL" , CA_STATUS_INVALID_PARAM);

    oc_mutex_lock(g_sslContextMutex);
    if (NULL == g_caSslContext)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Context is NULL");
        oc_mutex_unlock(g_sslContextMutex);
        return CA_STATUS_FAILED;
    }
    SslEndPoint_t * tep = GetSslPeer(endpoint);
    if (NULL == tep)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Session does not exist");
        oc_mutex_unlock(g_sslContextMutex);
        return CA_STATUS_FAILED;
    }
    /* No error checking, the connection might be closed already */
    int ret = 0;
    do
    {
        ret = mbedtls_ssl_close_notify(&tep->ssl);
    }
    while (MBEDTLS_ERR_SSL_WANT_WRITE == ret);

    RemovePeerFromList(&tep->sep.endpoint);
    oc_mutex_unlock(g_sslContextMutex);

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return CA_STATUS_OK;
}

CAResult_t CAcloseSslConnectionUsingUuid(const uint8_t *identity, size_t idLength)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(identity, NET_SSL_TAG, "Param identity is NULL" , CA_STATUS_INVALID_PARAM);

    oc_mutex_lock(g_sslContextMutex);
    if (NULL == g_caSslContext)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Context is NULL");
        oc_mutex_unlock(g_sslContextMutex);
        return CA_STATUS_FAILED;
    }

    SslEndPoint_t* tep = GetSslPeerUsingUuid(identity, idLength);
    if (NULL == tep)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Session does not exist");
        oc_mutex_unlock(g_sslContextMutex);
        return CA_STATUS_FAILED;
    }

    /* No error checking, the connection might be closed already */
    int ret = 0;
    do
    {
        ret = mbedtls_ssl_close_notify(&tep->ssl);
    }
    while (MBEDTLS_ERR_SSL_WANT_WRITE == ret);

    RemovePeerFromList(&tep->sep.endpoint);
    oc_mutex_unlock(g_sslContextMutex);

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return CA_STATUS_OK;
}

void CAcloseSslConnectionAll(CATransportAdapter_t transportType)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    oc_mutex_lock(g_sslContextMutex);
    if (NULL == g_caSslContext)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Context is NULL");
        oc_mutex_unlock(g_sslContextMutex);
        return;
    }

    uint32_t listLength = u_arraylist_length(g_caSslContext->peerList);
    OIC_LOG_V(DEBUG, NET_SSL_TAG,
            "Required transport [%d], peer count [%d]", transportType, listLength);
    for (uint32_t i = listLength; i > 0; i--)
    {
        SslEndPoint_t *tep = (SslEndPoint_t *)u_arraylist_get(g_caSslContext->peerList, i - 1);
        if (NULL == tep)
        {
            continue;
        }
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "SSL Connection [%s:%d], Transport [%d]",
                  tep->sep.endpoint.addr, tep->sep.endpoint.port, tep->sep.endpoint.adapter);

        // check transport matching
        if (0 == (tep->sep.endpoint.adapter & transportType))
        {
            OIC_LOG(DEBUG, NET_SSL_TAG, "Skip the un-matched transport session");
            continue;
        }

        // TODO: need to check below code after socket close is ensured.
        /*int ret = 0;
        do
        {
            ret = mbedtls_ssl_close_notify(&tep->ssl);
        }
        while (MBEDTLS_ERR_SSL_WANT_WRITE == ret);*/

        // delete from list
        u_arraylist_remove(g_caSslContext->peerList, i - 1);
        DeleteSslEndPoint(tep);
    }
    oc_mutex_unlock(g_sslContextMutex);

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return;
}
/**
 * Creates session for endpoint.
 *
 * @param[in]  endpoint    remote address
 * @param[in]  config    mbedTLS configuration info
 *
 * @return  TLS endpoint or NULL
 */
static SslEndPoint_t * NewSslEndPoint(const CAEndpoint_t * endpoint, mbedtls_ssl_config * config)
{
    SslEndPoint_t * tep = NULL;
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(endpoint, NET_SSL_TAG, "endpoint", NULL);
    VERIFY_NON_NULL_RET(config, NET_SSL_TAG, "config", NULL);
    VERIFY_NON_NULL_RET(g_caSslContext, NET_SSL_TAG, "SSL Context is NULL", NULL);

    tep = (SslEndPoint_t *) OICCalloc(1, sizeof (SslEndPoint_t));
    if (NULL == tep)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Malloc failed!");
        return NULL;
    }

    tep->sep.endpoint = *endpoint;
    tep->sep.endpoint.flags = (CATransportFlags_t)(tep->sep.endpoint.flags | CA_SECURE);

    if(0 != mbedtls_ssl_setup(&tep->ssl, config))
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Setup failed");
        OICFree(tep);
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return NULL;
    }

    mbedtls_ssl_set_bio(&tep->ssl, tep, SendCallBack, RecvCallBack, NULL);
    if (MBEDTLS_SSL_TRANSPORT_DATAGRAM == config->transport)
    {
        mbedtls_ssl_set_timer_cb(&tep->ssl, &tep->timer,
                                  mbedtls_timing_set_delay, mbedtls_timing_get_delay);
        if (MBEDTLS_SSL_IS_SERVER == config->endpoint)
        {
            if (0 != mbedtls_ssl_set_client_transport_id(&tep->ssl,
                                    (const unsigned char *) endpoint->addr, sizeof(endpoint->addr)))
            {
                OIC_LOG(ERROR, NET_SSL_TAG, "Transport id setup failed!");
                mbedtls_ssl_free(&tep->ssl);
                OICFree(tep);
                OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
                return NULL;
            }
        }
    }
    tep->cacheList = u_arraylist_create();
    if (NULL == tep->cacheList)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "cacheList initialization failed!");
        mbedtls_ssl_free(&tep->ssl);
        OICFree(tep);
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return NULL;
    }
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "New [%s role] endpoint added [%s:%d]",
            (MBEDTLS_SSL_IS_SERVER==config->endpoint ? "server" : "client"),
            endpoint->addr, endpoint->port);
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return tep;
}
/**
 * Initializes PSK identity.
 *
 * @param[out]  config    client/server config to be updated
 *
 * @return  0 on success or -1 on error
 */
static int InitPskIdentity(mbedtls_ssl_config * config)
{
    uint8_t idBuf[UUID_LENGTH] = {0};
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(config, NET_SSL_TAG, "Param config is NULL" , -1);

    if (0 > g_getCredentialsCallback(CA_DTLS_PSK_IDENTITY, NULL, 0, idBuf, UUID_LENGTH))
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Identity not found");
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return -1;
    }
    if (0 != mbedtls_ssl_conf_psk(config, idBuf, 0, idBuf, UUID_LENGTH))
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Identity initialization failed!");
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return -1;
    }
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return 0;
}
static void SetupCipher(mbedtls_ssl_config * config, CATransportAdapter_t adapter)
{
    int index = 0;
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_VOID(config, NET_SSL_TAG, "Invaild param");
    VERIFY_NON_NULL_VOID(g_caSslContext, NET_SSL_TAG, "SSL Context is NULL");
    VERIFY_NON_NULL_VOID(g_getCredentialTypesCallback, NET_SSL_TAG, "Param callback is null");

    //Resetting cipherFlag
    g_caSslContext->cipherFlag[0] = false;
    g_caSslContext->cipherFlag[1] = false;

    g_getCredentialTypesCallback(g_caSslContext->cipherFlag);
    // Retrieve the PSK credential from SRM
    if (0 != InitPskIdentity(config))
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "PSK identity initialization failed!");
    }

    // Retrieve the Cert credential from SRM
    if (true == g_caSslContext->cipherFlag[1])
    {
        int ret = InitPKIX(adapter);
        if (0 != ret)
        {
            OIC_LOG(ERROR, NET_SSL_TAG, "Failed to init X.509");
        }
    }

    memset(g_cipherSuitesList, 0, sizeof(g_cipherSuitesList));

    // Add the preferred ciphersuite first
    if (SSL_CIPHER_MAX != g_caSslContext->cipher)
    {
        g_cipherSuitesList[index] = tlsCipher[g_caSslContext->cipher][0];
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Preferred ciphersuite added");
        index++;
    }

    // Add PSK ciphersuite
    if (true == g_caSslContext->cipherFlag[0] &&
                MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256 != tlsCipher[g_caSslContext->cipher][0])
    {
       g_cipherSuitesList[index] = MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256;
       OIC_LOG(DEBUG, NET_SSL_TAG, "PSK ciphersuite added");
       index++;
    }

    // Add all certificate ciphersuites
    if (true == g_caSslContext->cipherFlag[1])
    {
        for (int i = 0; i < SSL_CIPHER_MAX - 1; i++)
        {
            if (MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256 != tlsCipher[i][0] &&
                i != g_caSslContext->cipher)
            {
                g_cipherSuitesList[index] = tlsCipher[i][0];
                index ++;
            }
        }

        if (MBEDTLS_SSL_IS_SERVER == config->endpoint)
        {
            mbedtls_ssl_conf_authmode(config, MBEDTLS_SSL_VERIFY_OPTIONAL);
        }
    }

    OIC_LOG(DEBUG, NET_SSL_TAG, "Supported ciphersuites:");
    for (int i = 0; i < index; i++)
    {
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Ciphersuite %04x", g_cipherSuitesList[i]);
    }

    mbedtls_ssl_conf_ciphersuites(config, g_cipherSuitesList);

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
}
/**
 * Initiate TLS handshake with endpoint.
 *
 * @param[in]  endpoint    remote address
 *
 * @return  TLS endpoint or NULL
 */
static SslEndPoint_t * InitiateTlsHandshake(const CAEndpoint_t *endpoint)
{
    int ret = 0;
    SslEndPoint_t * tep = NULL;

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(endpoint, NET_SSL_TAG, "Param endpoint is NULL" , NULL);
    VERIFY_NON_NULL_RET(g_caSslContext, NET_SSL_TAG, "SSL Context is NULL", NULL);

    //Remove previous peer info from peer list.
    RemovePeerFromList(endpoint);

    mbedtls_ssl_config * config = (endpoint->adapter == CA_ADAPTER_IP ||
                                   endpoint->adapter == CA_ADAPTER_GATT_BTLE ?
                                   &g_caSslContext->clientDtlsConf : &g_caSslContext->clientTlsConf);
    tep = NewSslEndPoint(endpoint, config);
    if (NULL == tep)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Malloc failed!");
        return NULL;
    }

    //Load allowed SVR suites from SVR DB
    SetupCipher(config, endpoint->adapter);

    ret = u_arraylist_add(g_caSslContext->peerList, (void *) tep);
    if (!ret)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "u_arraylist_add failed!");
        DeleteSslEndPoint(tep);
        return NULL;
    }

    while (MBEDTLS_SSL_HANDSHAKE_OVER > tep->ssl.state)
    {
        ret = mbedtls_ssl_handshake_step(&tep->ssl);
        if (MBEDTLS_ERR_SSL_CONN_EOF == ret)
        {
            break;
        }
        else if (-1 == ret)
        {
            OIC_LOG(ERROR, NET_SSL_TAG, "Handshake failed due to socket error");
            RemovePeerFromList(&tep->sep.endpoint);
            return NULL;
        }
        SSL_CHECK_FAIL(tep, ret, "Handshake error", 0, NULL, MBEDTLS_SSL_ALERT_MSG_HANDSHAKE_FAILURE);
    }
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return tep;
}
#ifdef __WITH_DTLS__
/**
 * Stops DTLS retransmission.
 */
static void StopRetransmit()
{
    if (g_caSslContext)
    {
        unregisterTimer(g_caSslContext->timerId);
        g_caSslContext->timerId= -1;
    }
}
#endif
void CAdeinitSslAdapter()
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);

    VERIFY_NON_NULL_VOID(g_caSslContext, NET_SSL_TAG, "context is NULL");
    VERIFY_NON_NULL_VOID(g_sslContextMutex, NET_SSL_TAG, "context mutex is NULL");

    //Lock tlsContext mutex
    oc_mutex_lock(g_sslContextMutex);

    // Clear all lists
    DeletePeerList();

    // De-initialize mbedTLS
    mbedtls_x509_crt_free(&g_caSslContext->ca);
    mbedtls_x509_crt_free(&g_caSslContext->crt);
    mbedtls_pk_free(&g_caSslContext->pkey);
    mbedtls_x509_crl_free(&g_caSslContext->crl);
#ifdef __WITH_TLS__
    mbedtls_ssl_config_free(&g_caSslContext->clientTlsConf);
    mbedtls_ssl_config_free(&g_caSslContext->serverTlsConf);
#endif // __WITH_TLS__
#ifdef __WITH_DTLS__
    mbedtls_ssl_config_free(&g_caSslContext->clientDtlsConf);
    mbedtls_ssl_config_free(&g_caSslContext->serverDtlsConf);
    mbedtls_ssl_cookie_free(&g_caSslContext->cookieCtx);
#endif // __WITH_DTLS__
    mbedtls_ctr_drbg_free(&g_caSslContext->rnd);
    mbedtls_entropy_free(&g_caSslContext->entropy);
#ifdef __WITH_DTLS__
    StopRetransmit();
#endif
    // De-initialize tls Context
    OICFree(g_caSslContext);
    g_caSslContext = NULL;

    // Delete decrypt buffer
    if (g_decryptBuffer)
    {
        OICFree(g_decryptBuffer);
        g_decryptBuffer = NULL;
    }

    // Unlock tlsContext mutex and de-initialize it
    oc_mutex_unlock(g_sslContextMutex);
    oc_mutex_free(g_sslContextMutex);
    g_sslContextMutex = NULL;

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s ", __func__);
}

static int InitConfig(mbedtls_ssl_config * conf, int transport, int mode)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(conf, NET_SSL_TAG, "Param conf is NULL" , -1);
    VERIFY_NON_NULL_RET(g_caSslContext, NET_SSL_TAG, "SSL Context is NULL", -1);
    mbedtls_ssl_config_init(conf);
    if (mbedtls_ssl_config_defaults(conf, mode, transport, MBEDTLS_SSL_PRESET_DEFAULT) != 0)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Config initialization failed!");
        return -1;
    }

    mbedtls_ssl_conf_psk_cb(conf, GetPskCredentialsCallback, NULL);
    mbedtls_ssl_conf_rng(conf, mbedtls_ctr_drbg_random, &g_caSslContext->rnd);
    mbedtls_ssl_conf_curves(conf, curve[ADAPTER_CURVE_SECP256R1]);
    mbedtls_ssl_conf_min_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_3);
    mbedtls_ssl_conf_renegotiation(conf, MBEDTLS_SSL_RENEGOTIATION_DISABLED);
    mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_REQUIRED);

#ifdef __WITH_DTLS__
    if (MBEDTLS_SSL_TRANSPORT_DATAGRAM == transport &&
            MBEDTLS_SSL_IS_SERVER == mode)
    {
        mbedtls_ssl_conf_dtls_cookies(conf, mbedtls_ssl_cookie_write, mbedtls_ssl_cookie_check,
                                      &g_caSslContext->cookieCtx);
    }
#endif // __WITH_DTLS__

#if !defined(NDEBUG) || defined(TB_LOG)
    mbedtls_ssl_conf_dbg(conf, DebugSsl, NULL);
    mbedtls_debug_set_threshold(MBED_TLS_DEBUG_LEVEL);
#endif
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return 0;
}
#ifdef __WITH_DTLS__
/**
 * Starts DTLS retransmission.
 */
static int StartRetransmit()
{
    uint32_t listIndex = 0;
    uint32_t listLength = 0;
    SslEndPoint_t *tep = NULL;

    oc_mutex_lock(g_sslContextMutex);
    if (NULL == g_caSslContext)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Context is NULL. Stop retransmission");
        oc_mutex_unlock(g_sslContextMutex);
        return -1;
    }
    if (g_caSslContext->timerId != -1)
    {
        //clear previous timer
        unregisterTimer(g_caSslContext->timerId);

        listLength = u_arraylist_length(g_caSslContext->peerList);
        for (listIndex = 0; listIndex < listLength; listIndex++)
        {
            tep = (SslEndPoint_t *) u_arraylist_get(g_caSslContext->peerList, listIndex);
            if (NULL == tep
                || (tep->ssl.conf && MBEDTLS_SSL_TRANSPORT_STREAM == tep->ssl.conf->transport)
                || MBEDTLS_SSL_HANDSHAKE_OVER == tep->ssl.state)
            {
                continue;
            }
            int ret = mbedtls_ssl_handshake_step(&tep->ssl);

            if (MBEDTLS_ERR_SSL_CONN_EOF != ret)
            {
                //start new timer
                registerTimer(RETRANSMISSION_TIME, &g_caSslContext->timerId, (void *) StartRetransmit);
                //unlock & return
                SSL_CHECK_FAIL(tep, ret, "Retransmission", 1, CA_STATUS_FAILED,
                MBEDTLS_SSL_ALERT_MSG_HANDSHAKE_FAILURE);
            }
        }
    }
    //start new timer
    registerTimer(RETRANSMISSION_TIME, &g_caSslContext->timerId, (void *) StartRetransmit);
    oc_mutex_unlock(g_sslContextMutex);
    return 0;
}
#endif

CAResult_t CAinitSslAdapter()
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    // Initialize mutex for tlsContext
    if (NULL == g_sslContextMutex)
    {
        g_sslContextMutex = oc_mutex_new();
        VERIFY_NON_NULL_RET(g_sslContextMutex, NET_SSL_TAG, "malloc failed", CA_MEMORY_ALLOC_FAILED);
    }
    else
    {
        OIC_LOG(INFO, NET_SSL_TAG, "Done already!");
        return CA_STATUS_OK;
    }

    // Lock tlsContext mutex and create tlsContext
    oc_mutex_lock(g_sslContextMutex);
    g_caSslContext = (SslContext_t *)OICCalloc(1, sizeof(SslContext_t));

    if (NULL == g_caSslContext)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Context malloc failed");
        oc_mutex_unlock(g_sslContextMutex);
        oc_mutex_free(g_sslContextMutex);
        g_sslContextMutex = NULL;
        return CA_MEMORY_ALLOC_FAILED;
    }

    // Create peer list
    g_caSslContext->peerList = u_arraylist_create();

    if(NULL == g_caSslContext->peerList)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "peerList initialization failed!");
        OICFree(g_caSslContext);
        g_caSslContext = NULL;
        oc_mutex_unlock(g_sslContextMutex);
        oc_mutex_free(g_sslContextMutex);
        g_sslContextMutex = NULL;
        return CA_STATUS_FAILED;
    }

    /* Initialize TLS library
     */
#if !defined(NDEBUG) || defined(TB_LOG)
    char version[MBED_TLS_VERSION_LEN];
    mbedtls_version_get_string(version);
    OIC_LOG_V(INFO, NET_SSL_TAG, "mbed TLS version: %s", version);
#endif

    /* Entropy settings
     */
    mbedtls_entropy_init(&g_caSslContext->entropy);
    mbedtls_ctr_drbg_init(&g_caSslContext->rnd);

#ifdef __unix__
    unsigned char seed[sizeof(SEED)] = {0};
    int urandomFd = -2;
    urandomFd = open("/dev/urandom", O_RDONLY);
    if(urandomFd == -1)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Fails open /dev/urandom!");
        oc_mutex_unlock(g_sslContextMutex);
        CAdeinitSslAdapter();
        return CA_STATUS_FAILED;
    }
    if(0 > read(urandomFd, seed, sizeof(seed)))
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Fails read from /dev/urandom!");
        close(urandomFd);
        oc_mutex_unlock(g_sslContextMutex);
        CAdeinitSslAdapter();
        return CA_STATUS_FAILED;
    }
    close(urandomFd);

#else
    unsigned char * seed = (unsigned char*) SEED;
#endif
    if(0 != mbedtls_ctr_drbg_seed(&g_caSslContext->rnd, mbedtls_entropy_func,
                                  &g_caSslContext->entropy, seed, sizeof(SEED)))
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Seed initialization failed!");
        oc_mutex_unlock(g_sslContextMutex);
        CAdeinitSslAdapter();
        return CA_STATUS_FAILED;
    }
    mbedtls_ctr_drbg_set_prediction_resistance(&g_caSslContext->rnd, MBEDTLS_CTR_DRBG_PR_OFF);

#ifdef __WITH_TLS__
    if (0 != InitConfig(&g_caSslContext->clientTlsConf,
                        MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_IS_CLIENT))
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Client config initialization failed!");
        oc_mutex_unlock(g_sslContextMutex);
        CAdeinitSslAdapter();
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return CA_STATUS_FAILED;
    }

    if (0 != InitConfig(&g_caSslContext->serverTlsConf,
                        MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_IS_SERVER))
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Server config initialization failed!");
        oc_mutex_unlock(g_sslContextMutex);
        CAdeinitSslAdapter();
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return CA_STATUS_FAILED;
    }
#endif // __WITH_TLS__
#ifdef __WITH_DTLS__
    mbedtls_ssl_cookie_init(&g_caSslContext->cookieCtx);
    if (0 != mbedtls_ssl_cookie_setup(&g_caSslContext->cookieCtx, mbedtls_ctr_drbg_random,
                                      &g_caSslContext->rnd))
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Cookie setup failed!");
        oc_mutex_unlock(g_sslContextMutex);
        CAdeinitSslAdapter();
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return CA_STATUS_FAILED;
    }

    if (0 != InitConfig(&g_caSslContext->clientDtlsConf,
                        MBEDTLS_SSL_TRANSPORT_DATAGRAM, MBEDTLS_SSL_IS_CLIENT))
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Client config initialization failed!");
        oc_mutex_unlock(g_sslContextMutex);
        CAdeinitSslAdapter();
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return CA_STATUS_FAILED;
    }

    if (0 != InitConfig(&g_caSslContext->serverDtlsConf,
                        MBEDTLS_SSL_TRANSPORT_DATAGRAM, MBEDTLS_SSL_IS_SERVER))
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Server config initialization failed!");
        oc_mutex_unlock(g_sslContextMutex);
        CAdeinitSslAdapter();
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return CA_STATUS_FAILED;
    }
#endif // __WITH_DTLS__

    // set default cipher
    g_caSslContext->cipher = SSL_CIPHER_MAX;

    // init X.509
    mbedtls_x509_crt_init(&g_caSslContext->ca);
    mbedtls_x509_crt_init(&g_caSslContext->crt);
    mbedtls_pk_init(&g_caSslContext->pkey);
    mbedtls_x509_crl_init(&g_caSslContext->crl);

#ifdef __WITH_DTLS__
    g_caSslContext->timerId = -1;
#endif

    // create decrypt buffer
    g_decryptBuffer = (uint8_t *)OICCalloc(1, TLS_MSG_BUF_LEN);
    if (NULL == g_decryptBuffer)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Decrypt buffer malloc failed");
        oc_mutex_unlock(g_sslContextMutex);
        CAdeinitSslAdapter();
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return CA_MEMORY_ALLOC_FAILED;
    }

   oc_mutex_unlock(g_sslContextMutex);
#ifdef __WITH_DTLS__
    StartRetransmit();
#endif

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return CA_STATUS_OK;
}

SslCacheMessage_t *  NewCacheMessage(uint8_t * data, size_t dataLen)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(data, NET_SSL_TAG, "Param data is NULL" , NULL);
    if (0 == dataLen)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "dataLen is equal to zero");
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return NULL;
    }
    SslCacheMessage_t * message = (SslCacheMessage_t *) OICCalloc(1, sizeof(SslCacheMessage_t));
    if (NULL == message)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "calloc failed!");
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return NULL;
    }

    message->data = (uint8_t *)OICCalloc(dataLen, sizeof(uint8_t));
    if (NULL == message->data)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "calloc failed!");
        OICFree(message);
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return NULL;
    }
    memcpy(message->data, data, dataLen);
    message->len = dataLen;
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return message;
}

/* Send data via TLS connection.
 */
CAResult_t CAencryptSsl(const CAEndpoint_t *endpoint,
                        void *data, uint32_t dataLen)
{
    int ret = 0;

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s ", __func__);

    VERIFY_NON_NULL_RET(endpoint, NET_SSL_TAG,"Remote address is NULL", CA_STATUS_INVALID_PARAM);
    VERIFY_NON_NULL_RET(data, NET_SSL_TAG, "Data is NULL", CA_STATUS_INVALID_PARAM);
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Port %d", endpoint->port);

    if (0 == dataLen)
    {
        OIC_LOG_V(ERROR, NET_SSL_TAG, "dataLen is zero [%d]", dataLen);
        return CA_STATUS_FAILED;
    }

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Data to be encrypted dataLen [%d]", dataLen);

    oc_mutex_lock(g_sslContextMutex);
    if(NULL == g_caSslContext)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Context is NULL");
        oc_mutex_unlock(g_sslContextMutex);
        return CA_STATUS_FAILED;
    }

    SslEndPoint_t * tep = GetSslPeer(endpoint);
    if (NULL == tep)
    {
        tep = InitiateTlsHandshake(endpoint);
    }
    if (NULL == tep)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "TLS handshake failed");
        oc_mutex_unlock(g_sslContextMutex);
        return CA_STATUS_FAILED;
    }

    if (MBEDTLS_SSL_HANDSHAKE_OVER == tep->ssl.state)
    {
        unsigned char *dataBuf = (unsigned char *)data;
        size_t written = 0;

        do
        {
            ret = mbedtls_ssl_write(&tep->ssl, dataBuf, dataLen - written);
            if (ret < 0)
            {
                if (MBEDTLS_ERR_SSL_WANT_WRITE != ret)
                {
                    OIC_LOG_V(ERROR, NET_SSL_TAG, "mbedTLS write failed! returned 0x%x", -ret);
                    RemovePeerFromList(&tep->sep.endpoint);
                    oc_mutex_unlock(g_sslContextMutex);
                    return CA_STATUS_FAILED;
                }
                continue;
            }
            OIC_LOG_V(DEBUG, NET_SSL_TAG, "mbedTLS write returned with sent bytes[%d]", ret);

            dataBuf += ret;
            written += ret;
        } while (dataLen > written);

    }
    else
    {
        SslCacheMessage_t * msg = NewCacheMessage((uint8_t*) data, dataLen);
        if (NULL == msg || !u_arraylist_add(tep->cacheList, (void *) msg))
        {
            OIC_LOG(ERROR, NET_SSL_TAG, "u_arraylist_add failed!");
            oc_mutex_unlock(g_sslContextMutex);
            return CA_STATUS_FAILED;
        }
    }

    oc_mutex_unlock(g_sslContextMutex);

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return CA_STATUS_OK;
}
/**
 * Sends cached messages via TLS connection.
 *
 * @param[in]  tep    remote address with session info
 */
static void SendCacheMessages(SslEndPoint_t * tep)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_VOID(tep, NET_SSL_TAG, "Param tep is NULL");

    uint32_t listIndex = 0;
    uint32_t listLength = 0;
    listLength = u_arraylist_length(tep->cacheList);
    for (listIndex = 0; listIndex < listLength;)
    {
        int ret = 0;
        SslCacheMessage_t * msg = (SslCacheMessage_t *) u_arraylist_get(tep->cacheList, listIndex);
        if (NULL != msg && NULL != msg->data && 0 != msg->len)
        {
            unsigned char *dataBuf = (unsigned char *)msg->data;
            size_t written = 0;

            do
            {
                ret = mbedtls_ssl_write(&tep->ssl, dataBuf, msg->len - written);
                if (ret < 0)
                {
                    if (MBEDTLS_ERR_SSL_WANT_WRITE != ret)
                    {
                        OIC_LOG_V(ERROR, NET_SSL_TAG, "mbedTLS write failed! returned -0x%x", -ret);
                        break;
                    }
                    continue;
                }
                OIC_LOG_V(DEBUG, NET_SSL_TAG, "mbedTLS write returned with sent bytes[%d]", ret);

                dataBuf += ret;
                written += ret;
            } while (msg->len > written);

            if (u_arraylist_remove(tep->cacheList, listIndex))
            {
                DeleteCacheMessage(msg);
                // Reduce list length by 1 as we removed one element.
                listLength--;
            }
            else
            {
                OIC_LOG(ERROR, NET_SSL_TAG, "u_arraylist_remove failed.");
                break;
            }
        }
        else
        {
            // Move to the next element
            ++listIndex;
        }
    }
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
}

void CAsetSslHandshakeCallback(CAErrorCallback tlsHandshakeCallback)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    g_sslCallback = tlsHandshakeCallback;
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
}

/* Read data from TLS connection
 */
CAResult_t CAdecryptSsl(const CASecureEndpoint_t *sep, uint8_t *data, uint32_t dataLen)
{
    int ret = 0;
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(sep, NET_SSL_TAG, "endpoint is NULL" , CA_STATUS_INVALID_PARAM);
    VERIFY_NON_NULL_RET(data, NET_SSL_TAG, "Param data is NULL" , CA_STATUS_INVALID_PARAM);

    oc_mutex_lock(g_sslContextMutex);
    if (NULL == g_caSslContext)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Context is NULL");
        oc_mutex_unlock(g_sslContextMutex);
        return CA_STATUS_FAILED;
    }


    SslEndPoint_t * peer = GetSslPeer(&sep->endpoint);
    if (NULL == peer)
    {
        mbedtls_ssl_config * config = (sep->endpoint.adapter == CA_ADAPTER_IP ||
                                   sep->endpoint.adapter == CA_ADAPTER_GATT_BTLE ?
                                   &g_caSslContext->serverDtlsConf : &g_caSslContext->serverTlsConf);
        peer = NewSslEndPoint(&sep->endpoint, config);
        if (NULL == peer)
        {
            OIC_LOG(ERROR, NET_SSL_TAG, "Malloc failed!");
            oc_mutex_unlock(g_sslContextMutex);
            return CA_STATUS_FAILED;
        }
        //Load allowed TLS suites from SVR DB
        SetupCipher(config, sep->endpoint.adapter);

        ret = u_arraylist_add(g_caSslContext->peerList, (void *) peer);
        if (!ret)
        {
            OIC_LOG(ERROR, NET_SSL_TAG, "u_arraylist_add failed!");
            OICFree(peer);
            oc_mutex_unlock(g_sslContextMutex);
            return CA_STATUS_FAILED;
        }
    }

    peer->recBuf.buff = data;
    peer->recBuf.len = dataLen;
    peer->recBuf.loaded = 0;

    while (MBEDTLS_SSL_HANDSHAKE_OVER != peer->ssl.state)
    {
        ret = mbedtls_ssl_handshake_step(&peer->ssl);
        if (MBEDTLS_ERR_SSL_CONN_EOF == ret)
        {
            break;
        }

        if (MBEDTLS_ERR_SSL_HELLO_VERIFY_REQUIRED == ret)
        {
            OIC_LOG(DEBUG, NET_SSL_TAG, "Hello verification requested");
            mbedtls_ssl_session_reset(&peer->ssl);
            mbedtls_ssl_set_client_transport_id(&peer->ssl,
                                                (const unsigned char *) sep->endpoint.addr,
                                                 sizeof(sep->endpoint.addr));
            ret = mbedtls_ssl_handshake_step(&peer->ssl);
        }
        if (MBEDTLS_SSL_IS_CLIENT == peer->ssl.conf->endpoint)
        {
            uint32_t flags = mbedtls_ssl_get_verify_result(&peer->ssl);
            if (0 != flags)
            {
                OIC_LOG_BUFFER(ERROR, NET_SSL_TAG, (const uint8_t *) &flags, sizeof(flags));
                SSL_CHECK_FAIL(peer, flags, "Cert verification failed", 1,
                                                         CA_STATUS_FAILED, GetAlertCode(flags));
            }
        }
        SSL_CHECK_FAIL(peer, ret, "Handshake error", 1, CA_STATUS_FAILED, MBEDTLS_SSL_ALERT_MSG_HANDSHAKE_FAILURE);
        if (MBEDTLS_SSL_CLIENT_CHANGE_CIPHER_SPEC == peer->ssl.state)
        {
            memcpy(peer->master, peer->ssl.session_negotiate->master, sizeof(peer->master));
            g_caSslContext->selectedCipher = peer->ssl.session_negotiate->ciphersuite;
        }
        if (MBEDTLS_SSL_CLIENT_KEY_EXCHANGE == peer->ssl.state)
        {
            memcpy(peer->random, peer->ssl.handshake->randbytes, sizeof(peer->random));
        }

        if (MBEDTLS_SSL_HANDSHAKE_OVER == peer->ssl.state)
        {
            SSL_RES(peer, CA_STATUS_OK);
            if (MBEDTLS_SSL_IS_CLIENT == peer->ssl.conf->endpoint)
            {
                SendCacheMessages(peer);
            }

            int selectedCipher = peer->ssl.session->ciphersuite;
            OIC_LOG_V(DEBUG, NET_SSL_TAG, "(D)TLS Session is connected via ciphersuite [0x%x]", selectedCipher);
            if (MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256 != selectedCipher &&
                MBEDTLS_TLS_ECDH_ANON_WITH_AES_128_CBC_SHA256 != selectedCipher)
            {
                char uuid[UUID_LENGTH * 2 + 5] = {0};
                void * uuidPos = NULL;
                void * userIdPos = NULL;
                const mbedtls_x509_crt * peerCert = mbedtls_ssl_get_peer_cert(&peer->ssl);
                ret = (NULL == peerCert ? -1 : 0);
                //SSL_CHECK_FAIL(peer, ret, "Failed to retrieve cert", 1,
                //                            CA_STATUS_FAILED, MBEDTLS_SSL_ALERT_MSG_NO_CERT);
                if (0 == ret)
                {
                    uuidPos = memmem(peerCert->subject_raw.p, peerCert->subject_raw.len,
                                                     UUID_PREFIX, sizeof(UUID_PREFIX) - 1);

                    if (NULL != uuidPos)
                    {
                        memcpy(uuid, (char*) uuidPos + sizeof(UUID_PREFIX) - 1, UUID_LENGTH * 2 + 4);
                        OIC_LOG_V(DEBUG, NET_SSL_TAG, "certificate uuid string: %s" , uuid);
                        ret = OCConvertStringToUuid(uuid, peer->sep.identity.id);
                        SSL_CHECK_FAIL(peer, ret, "Failed to convert subject", 1,
                                              CA_STATUS_FAILED, MBEDTLS_SSL_ALERT_MSG_UNSUPPORTED_CERT);
                    }
                    else
                    {
                        OIC_LOG(WARNING, NET_SSL_TAG, "uuid not found");
                    }

                    userIdPos = memmem(peerCert->subject_raw.p, peerCert->subject_raw.len,
                                                 USERID_PREFIX, sizeof(USERID_PREFIX) - 1);
                    if (NULL != userIdPos)
                    {
                        memcpy(uuid, (char*) userIdPos + sizeof(USERID_PREFIX) - 1, UUID_LENGTH * 2 + 4);
                        ret = OCConvertStringToUuid(uuid, peer->sep.userId.id);
                        SSL_CHECK_FAIL(peer, ret, "Failed to convert subject alt name", 1,
                                          CA_STATUS_FAILED, MBEDTLS_SSL_ALERT_MSG_UNSUPPORTED_CERT);
                    }
                    else
                    {
                        OIC_LOG(WARNING, NET_SSL_TAG, "Subject alternative name not found");
                    }
                }
            }

            if (MBEDTLS_SSL_IS_CLIENT == peer->ssl.conf->endpoint)
            {
                SendCacheMessages(peer);
            }
            mbedtls_ssl_config * config = (sep->endpoint.adapter == CA_ADAPTER_IP ||
                                       sep->endpoint.adapter == CA_ADAPTER_GATT_BTLE ?
                                       &g_caSslContext->serverDtlsConf : &g_caSslContext->serverTlsConf);

            mbedtls_ssl_conf_authmode(config, MBEDTLS_SSL_VERIFY_REQUIRED);

            oc_mutex_unlock(g_sslContextMutex);
            OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
            return CA_STATUS_OK;
        }
    }

    if (MBEDTLS_SSL_HANDSHAKE_OVER == peer->ssl.state)
    {
        // flag to read again remained data
        bool read_more = false;
        do
        {
            if (NULL == g_decryptBuffer)
            {
                OIC_LOG(ERROR, NET_SSL_TAG, "decrypt buffer is NULL");
                oc_mutex_unlock(g_sslContextMutex);
                return CA_STATUS_FAILED;
            }
            memset(g_decryptBuffer, 0, TLS_MSG_BUF_LEN);
            read_more = false;

            do
            {
                ret = mbedtls_ssl_read(&peer->ssl, g_decryptBuffer, TLS_MSG_BUF_LEN);
            } while (MBEDTLS_ERR_SSL_WANT_READ == ret);

            if (MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY == ret ||
                // TinyDTLS sends fatal close_notify alert
                (MBEDTLS_ERR_SSL_FATAL_ALERT_MESSAGE == ret &&
                 MBEDTLS_SSL_ALERT_LEVEL_FATAL == peer->ssl.in_msg[0] &&
                 MBEDTLS_SSL_ALERT_MSG_CLOSE_NOTIFY == peer->ssl.in_msg[1]))
            {
                OIC_LOG(INFO, NET_SSL_TAG, "Connection was closed gracefully");
                RemovePeerFromList(&peer->sep.endpoint);
                oc_mutex_unlock(g_sslContextMutex);
                return CA_STATUS_OK;
            }

            if (0 > ret)
            {
                OIC_LOG_V(ERROR, NET_SSL_TAG, "mbedtls_ssl_read returned -0x%x", -ret);
                //SSL_RES(peer, CA_STATUS_FAILED);
                RemovePeerFromList(&peer->sep.endpoint);
                oc_mutex_unlock(g_sslContextMutex);
                return CA_STATUS_FAILED;
            }
            else if (0 < ret)
            {
                int adapterIndex = GetAdapterIndex(peer->sep.endpoint.adapter);
                if (0 <= adapterIndex && MAX_SUPPORTED_ADAPTERS > adapterIndex)
                {
                    CAResult_t res;
                    res = g_caSslContext->adapterCallbacks[adapterIndex].recvCallback(&peer->sep,
                            g_decryptBuffer, ret);
                    if (CA_STATUS_OK != res)
                    {
                        OIC_LOG(ERROR, NET_SSL_TAG, "recvCallback is failed");
                        RemovePeerFromList(&peer->sep.endpoint);
                        oc_mutex_unlock(g_sslContextMutex);
                        return CA_STATUS_FAILED;
                    }

                    // check if decrypted data is remained in stream transport
                    size_t remained = mbedtls_ssl_get_bytes_avail(&peer->ssl);
                    if (0 < remained &&
                        MBEDTLS_SSL_TRANSPORT_STREAM == peer->ssl.conf->transport)
                    {
                        OIC_LOG_V(DEBUG, NET_SSL_TAG, "need to read %zu bytes more", remained);
                        read_more = true;
                    }
                }
                else
                {
                    OIC_LOG(ERROR, NET_SSL_TAG, "Unsuported adapter");
                    RemovePeerFromList(&peer->sep.endpoint);
                    oc_mutex_unlock(g_sslContextMutex);
                    return CA_STATUS_FAILED;
                }
            }
        } while (read_more);
    }

    oc_mutex_unlock(g_sslContextMutex);
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return CA_STATUS_OK;
}

void CAsetSslAdapterCallbacks(CAPacketReceivedCallback recvCallback,
                              CAPacketSendCallback sendCallback,
                              CATransportAdapter_t type)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_VOID(sendCallback, NET_SSL_TAG, "sendCallback is NULL");
    VERIFY_NON_NULL_VOID(recvCallback, NET_SSL_TAG, "recvCallback is NULL");
    oc_mutex_lock(g_sslContextMutex);
    if (NULL == g_caSslContext)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Context is NULL");
        oc_mutex_unlock(g_sslContextMutex);
        return;
    }

    switch (type)
    {
        case CA_ADAPTER_IP:
            g_caSslContext->adapterCallbacks[0].recvCallback = recvCallback;
            g_caSslContext->adapterCallbacks[0].sendCallback = sendCallback;
            break;
        case CA_ADAPTER_TCP:
            g_caSslContext->adapterCallbacks[1].recvCallback = recvCallback;
            g_caSslContext->adapterCallbacks[1].sendCallback = sendCallback;
            break;
        case CA_ADAPTER_GATT_BTLE:
            g_caSslContext->adapterCallbacks[2].recvCallback = recvCallback;
            g_caSslContext->adapterCallbacks[2].sendCallback = sendCallback;
            break;
        default:
            OIC_LOG_V(ERROR, NET_SSL_TAG, "Unsupported adapter: %d", type);
    }

    oc_mutex_unlock(g_sslContextMutex);
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
}
/**
 * Gets index of the TLS ciphersuite in the SslCipher_t enum.
 *
 * @param[in]  cipher    TLS chiphersuite code
 *
 * @return   corresponding enum
 */

static SslCipher_t GetCipherIndex(const uint32_t cipher)
{
    switch(cipher)
    {
        case MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA256:
        {
            return SSL_RSA_WITH_AES_256_CBC_SHA256;
        }
        case MBEDTLS_TLS_RSA_WITH_AES_128_GCM_SHA256:
        {
            return SSL_RSA_WITH_AES_128_GCM_SHA256;
        }
        case MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:
        {
            return SSL_ECDHE_RSA_WITH_AES_128_GCM_SHA256;
        }
        case MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:
        {
            return SSL_ECDHE_RSA_WITH_AES_256_GCM_SHA384;
        }
        case MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:
        {
            return SSL_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256;
        }
        case MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8:
        {
            return SSL_ECDHE_ECDSA_WITH_AES_128_CCM_8;
        }
        case MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM:
        {
            return SSL_ECDHE_ECDSA_WITH_AES_128_CCM;
        }
        case MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256:
        {
            return SSL_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256;
        }
        case MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384:
        {
            return SSL_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384;
        }
        case MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:
        {
            return SSL_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384;
        }
        case MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256:
        {
            return SSL_ECDHE_PSK_WITH_AES_128_CBC_SHA256;
        }
        case MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256:
        {
            return SSL_ECDHE_RSA_WITH_AES_128_CBC_SHA256;
        }
        case MBEDTLS_TLS_ECDH_ANON_WITH_AES_128_CBC_SHA256:
        {
            return SSL_ECDH_ANON_WITH_AES_128_CBC_SHA256;
        }
        default:
        {
            return SSL_CIPHER_MAX;
        }
    }
}

CAResult_t CAsetTlsCipherSuite(const uint32_t cipher)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(g_caSslContext, NET_SSL_TAG, "SSL context is not initialized." , CA_STATUS_NOT_INITIALIZED);

    CAResult_t res = CA_STATUS_FAILED;
    SslCipher_t index = GetCipherIndex(cipher);
    if (SSL_CIPHER_MAX == index)
    {
        OIC_LOG(WARNING, NET_SSL_TAG, "Unknown cipher");
    }
    else
    {
#ifdef __WITH_TLS__
        CONF_SSL(&g_caSslContext->clientTlsConf, &g_caSslContext->serverTlsConf,
        mbedtls_ssl_conf_ciphersuites, tlsCipher[index]);
#endif
#ifdef __WITH_DTLS__
        CONF_SSL(&g_caSslContext->clientDtlsConf, &g_caSslContext->serverDtlsConf,
        mbedtls_ssl_conf_ciphersuites, tlsCipher[index]);
#endif
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Selected cipher: 0x%x", cipher);
    }
    g_caSslContext->cipher = index;

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return CA_STATUS_OK;
}

CAResult_t CAinitiateSslHandshake(const CAEndpoint_t *endpoint)
{
    CAResult_t res = CA_STATUS_OK;
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(endpoint, NET_SSL_TAG, "Param endpoint is NULL" , CA_STATUS_INVALID_PARAM);
    oc_mutex_lock(g_sslContextMutex);
    if (NULL == InitiateTlsHandshake(endpoint))
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "TLS handshake failed");
        res = CA_STATUS_FAILED;
    }
    oc_mutex_unlock(g_sslContextMutex);
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return res;
}
/**
 * Expands the secret into blocks of data according
 * to the algorithm specified in section 5 of RFC 4346
 *
 * This function writes upto @p bufLen bytes into the given output buffer @p buf
 *
 * @param  key    secret key.
 * @param  keyLen    secret key length.
 * @param  label    A PRF label.
 * @param  labelLen     Actual length of @p label.
 * @param  random1    Random seed.
 * @param  random1Len     Actual length of @p random1 (may be zero).
 * @param  random2     Random seed.
 * @param  random2Len    Actual length of @p random2 (may be zero).
 * @param  buf    Output buffer for generated random data.
 * @param  bufLen    Maximum size of @p buf.
 *
 * @return The actual number of bytes written to @p buf or @c -1 on error.
 */

static int pHash (const unsigned char *key, size_t keyLen,
     const unsigned char *label, size_t labelLen,
     const unsigned char *random1, size_t random1Len,
     const unsigned char *random2, size_t random2Len,
     unsigned char *buf, size_t bufLen)
{
    unsigned char A[RANDOM_LEN] = {0};
    unsigned char tmp[RANDOM_LEN] = {0};
    size_t dLen;   /* digest length */
    size_t len = 0;   /* result length */

    VERIFY_NON_NULL_RET(key, NET_SSL_TAG, "key is NULL", -1);
    VERIFY_NON_NULL_RET(label, NET_SSL_TAG, "label is NULL", -1);
    VERIFY_NON_NULL_RET(random1, NET_SSL_TAG, "random1 is NULL", -1);
    VERIFY_NON_NULL_RET(random2, NET_SSL_TAG, "random2 is NULL", -1);
    VERIFY_NON_NULL_RET(buf, NET_SSL_TAG, "buf is NULL", -1);

    mbedtls_md_context_t hmacA;
    mbedtls_md_context_t hmacP;

    mbedtls_md_init(&hmacA);
    mbedtls_md_init(&hmacP);

    CHECK_MBEDTLS_RET(mbedtls_md_setup, &hmacA, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1);
    CHECK_MBEDTLS_RET(mbedtls_md_setup, &hmacP, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1);

    CHECK_MBEDTLS_RET(mbedtls_md_hmac_starts, &hmacA, key, keyLen );
    CHECK_MBEDTLS_RET(mbedtls_md_hmac_update, &hmacA, label, labelLen);
    CHECK_MBEDTLS_RET(mbedtls_md_hmac_update, &hmacA, random1, random1Len);
    CHECK_MBEDTLS_RET(mbedtls_md_hmac_update, &hmacA, random2, random2Len);
    CHECK_MBEDTLS_RET(mbedtls_md_hmac_finish, &hmacA, A);

    dLen = RANDOM_LEN;

    CHECK_MBEDTLS_RET(mbedtls_md_hmac_starts, &hmacP, key, keyLen);

    while (len + dLen < bufLen)
    {
        CHECK_MBEDTLS_RET(mbedtls_md_hmac_reset, &hmacP);
        CHECK_MBEDTLS_RET(mbedtls_md_hmac_starts, &hmacP, key, keyLen);
        CHECK_MBEDTLS_RET(mbedtls_md_hmac_update, &hmacP, A, dLen);
        CHECK_MBEDTLS_RET(mbedtls_md_hmac_update, &hmacP, label, labelLen);
        CHECK_MBEDTLS_RET(mbedtls_md_hmac_update, &hmacP, random1, random1Len);
        CHECK_MBEDTLS_RET(mbedtls_md_hmac_update, &hmacP, random2, random2Len);

        CHECK_MBEDTLS_RET(mbedtls_md_hmac_finish, &hmacP, tmp);

        len += RANDOM_LEN;

        memcpy(buf, tmp, dLen);
        buf += dLen;

        CHECK_MBEDTLS_RET(mbedtls_md_hmac_reset, &hmacA);
        CHECK_MBEDTLS_RET(mbedtls_md_hmac_starts, &hmacA, key, keyLen);
        CHECK_MBEDTLS_RET(mbedtls_md_hmac_update, &hmacA, A, dLen);
        CHECK_MBEDTLS_RET(mbedtls_md_hmac_finish, &hmacA, A);
    }

    CHECK_MBEDTLS_RET(mbedtls_md_hmac_reset, &hmacP);
    CHECK_MBEDTLS_RET(mbedtls_md_hmac_starts, &hmacP, key, keyLen);
    CHECK_MBEDTLS_RET(mbedtls_md_hmac_update, &hmacP, A, dLen);

    CHECK_MBEDTLS_RET(mbedtls_md_hmac_update, &hmacP, label, labelLen);
    CHECK_MBEDTLS_RET(mbedtls_md_hmac_update, &hmacP, random1, random1Len);
    CHECK_MBEDTLS_RET(mbedtls_md_hmac_update, &hmacP, random2, random2Len);
    CHECK_MBEDTLS_RET(mbedtls_md_hmac_finish, &hmacP, tmp);

    memcpy(buf, tmp, bufLen - len);

    mbedtls_md_free(&hmacA);
    mbedtls_md_free(&hmacP);
    return bufLen;

exit:
    mbedtls_md_free(&hmacA);
    mbedtls_md_free(&hmacP);
    return -1;
}

CAResult_t CAsslGenerateOwnerPsk(const CAEndpoint_t *endpoint,
                            const uint8_t* label, const size_t labelLen,
                            const uint8_t* rsrcServerDeviceId, const size_t rsrcServerDeviceIdLen,
                            const uint8_t* provServerDeviceId, const size_t provServerDeviceIdLen,
                            uint8_t* ownerPsk, const size_t ownerPskSize)
{
    OIC_LOG_V(DEBUG, NET_SSL_TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(endpoint, NET_SSL_TAG, "endpoint is NULL", CA_STATUS_INVALID_PARAM);
    VERIFY_NON_NULL_RET(label, NET_SSL_TAG, "label is NULL", CA_STATUS_INVALID_PARAM);
    VERIFY_NON_NULL_RET(rsrcServerDeviceId, NET_SSL_TAG, "rsrcId is NULL", CA_STATUS_INVALID_PARAM);
    VERIFY_NON_NULL_RET(provServerDeviceId, NET_SSL_TAG, "provId is NULL", CA_STATUS_INVALID_PARAM);
    VERIFY_NON_NULL_RET(ownerPsk, NET_SSL_TAG, "ownerPSK is NULL", CA_STATUS_INVALID_PARAM);
    
    oc_mutex_lock(g_sslContextMutex);
    if (NULL == g_caSslContext)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Context is NULL");
        oc_mutex_unlock(g_sslContextMutex);
        OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
        return CA_STATUS_FAILED;
    }
    SslEndPoint_t * tep = GetSslPeer(endpoint);
    if (NULL == tep)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Session does not exist");
        oc_mutex_unlock(g_sslContextMutex);
        return CA_STATUS_FAILED;
    }

    // keyBlockLen set up according to OIC 1.1 Security Specification Section 7.3.2
    int macKeyLen = 0;
    int ivSize = 0;
    int keySize = 0;
    int keyBlockLen = 0;
    if (MBEDTLS_TLS_ECDH_ANON_WITH_AES_128_CBC_SHA256 == g_caSslContext->selectedCipher ||
        MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256 == g_caSslContext->selectedCipher ||
        MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 == g_caSslContext->selectedCipher ||
        MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 == g_caSslContext->selectedCipher)
    {
        // 2 * ( 32 + 0 + 16 ) = 96
        macKeyLen = SHA256_MAC_KEY_LENGTH;
        ivSize = CBC_IV_LENGTH;
        keySize = AES128_KEY_LENGTH;
    }
    else if (MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM == g_caSslContext->selectedCipher ||
             MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8 == g_caSslContext->selectedCipher)
    {
        // 2 * ( 0 + 4 + 16 ) = 40
        macKeyLen = CCM_MAC_KEY_LENGTH;
        ivSize = CCM_IV_LENGTH;
        keySize = AES128_KEY_LENGTH;
    }
    else if (MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 == g_caSslContext->selectedCipher)
    {
        // 2 * ( 32 + 12 + 16 ) = 120
        macKeyLen = SHA256_MAC_KEY_LENGTH;
        ivSize = GCM_IV_LENGTH;
        keySize = AES128_KEY_LENGTH;
    }
    else if (MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA256 == g_caSslContext->selectedCipher)
    {
        // 2 * ( 32 + 0 + 32 ) = 128
        macKeyLen = SHA256_MAC_KEY_LENGTH;
        ivSize = CBC_IV_LENGTH;
        keySize = AES256_KEY_LENGTH;
    }
    else if (MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 == g_caSslContext->selectedCipher)
    {
        // 2 * ( 48 + 0 + 32 ) = 160
        macKeyLen = SHA384_MAC_KEY_LENGTH;
        ivSize = CBC_IV_LENGTH;
        keySize = AES256_KEY_LENGTH;
    }
    else if (MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 == g_caSslContext->selectedCipher)
    {
        // 2 * ( 48 + 12 + 32 ) = 184
        macKeyLen = SHA384_MAC_KEY_LENGTH;
        ivSize = GCM_IV_LENGTH;
        keySize = AES256_KEY_LENGTH;
    }
    else if (MBEDTLS_TLS_RSA_WITH_AES_128_GCM_SHA256 == g_caSslContext->selectedCipher)
    {
        // 2 * ( 48 + 12 + 32 ) = 184
        macKeyLen = SHA256_MAC_KEY_LENGTH;
        ivSize = GCM_IV_LENGTH;
        keySize = AES128_KEY_LENGTH;
    }
    keyBlockLen = 2 * (macKeyLen + keySize + ivSize);

    uint8_t * keyblock = (uint8_t *)OICMalloc(keyBlockLen);
    if (NULL == keyblock)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "Failed to OICMalloc for keyblock");
        oc_mutex_unlock(g_sslContextMutex);
        return CA_STATUS_FAILED;
    }

    // "key expansion"
    uint8_t lab[] = {0x6b, 0x65, 0x79, 0x20, 0x65, 0x78, 0x70, 0x61, 0x6e, 0x73, 0x69, 0x6f, 0x6e};
    int ret = pHash(tep->master, sizeof(tep->master), lab, sizeof(lab),
                    (tep->random) + RANDOM_LEN, RANDOM_LEN, tep->random, RANDOM_LEN,
                    keyblock, keyBlockLen);
    if (-1 == ret)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "PSK not generated");
        OICFree(keyblock);
        oc_mutex_unlock(g_sslContextMutex);
        return CA_STATUS_FAILED;
    }

    ret = pHash(keyblock, keyBlockLen, label, labelLen,
                rsrcServerDeviceId, rsrcServerDeviceIdLen,
                provServerDeviceId, provServerDeviceIdLen,
                ownerPsk, ownerPskSize);
    if (-1 == ret)
    {
        OIC_LOG(ERROR, NET_SSL_TAG, "PSK not generated");
        OICFree(keyblock);
        oc_mutex_unlock(g_sslContextMutex);
        return CA_STATUS_FAILED;
    }

    OICFree(keyblock);
    oc_mutex_unlock(g_sslContextMutex);

    OIC_LOG_V(DEBUG, NET_SSL_TAG, "Out %s", __func__);
    return CA_STATUS_OK;
}

