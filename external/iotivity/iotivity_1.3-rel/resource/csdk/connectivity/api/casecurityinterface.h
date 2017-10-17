/* *****************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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

/**
 * @file
 *
 * This file contains the Security APIs for Resource Model to use.
 */

#ifndef CA_SECURITY_INTERFACE_H_
#define CA_SECURITY_INTERFACE_H_


#include "cacommon.h"
#include "experimental/byte_array.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @enum CADtlsPskCredType_t
 * Type of PSK credential required during DTLS handshake
 * It does not make much sense in bringing in all definitions from dtls.h into here.
 * Therefore, redefining them here.
 */
typedef enum
{
    CA_DTLS_PSK_HINT,
    CA_DTLS_PSK_IDENTITY,
    CA_DTLS_PSK_KEY
} CADtlsPskCredType_t;

/**
 * This internal callback is used by CA layer to
 * retrieve PSK credentials from SRM.
 *
 * @param[in]  type type of PSK data required by CA layer during DTLS handshake set.
 * @param[in]  desc    Additional request information.
 * @param[in]  desc_len The actual length of desc.
 * @param[out] result  Must be filled with the requested information.
 * @param[in]  result_length  Maximum size of @p result.
 *
 * @return The number of bytes written to @p result or a value
 *         less than zero on error.
 */
typedef int (*CAgetPskCredentialsHandler)(CADtlsPskCredType_t type,
              const uint8_t *desc, size_t desc_len,
              uint8_t *result, size_t result_length);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)

/**
 * API to get security information about a connected peer
 *
 * @param[in] peer peer information includs IP address and port.
 * @param[out] sep copy of secure endpoint info
 *
 * @return  CA_STATUS_OK on success; other error otherwise
 */
CAResult_t CAGetSecureEndpointData(const CAEndpoint_t *peer, CASecureEndpoint_t *sep);

/**
 * Adds a bit to the attributes field of a secure endpoint.
 *
 * @param[in]  peer         remote address
 * @param[in]  newAttribute bit to be added to the attributes field
 *
 * @return  true if the secure endpoint has been found, false otherwise.
 */
bool CASetSecureEndpointAttribute(const CAEndpoint_t* peer, uint32_t newAttribute);

/**
 * Gets the attributes field of a secure endpoint.
 *
 * @param[in]  peer          remote address
 * @param[out] allAttributes all the attributes bits for that remote address
 *
 * @return  true if the secure endpoint has been found, false otherwise.
 */
bool CAGetSecureEndpointAttributes(const CAEndpoint_t* peer, uint32_t* allAttributes);
#endif // #if defined(__WITH_DTLS__) || defined(__WITH_TLS__)

/**
 * This internal callback is used by CA layer to
 * retrieve all credential types from SRM
 *
 * @param[out]  list of enabled credential types for CA handshake.
 * @param[in]   device uuid.
 *
 */
typedef void (*CAgetCredentialTypesHandler)(bool * list, const char* deviceId);
/**
 * Binary structure containing PKIX related info
 * own certificate chain, public key, CA's and CRL's
 * The data member of each ByteArray_t must be allocated with OICMalloc or OICCalloc.
 * The SSL adapter takes ownership of this memory and will free it internally after use.
 * Callers should not reference this memory after it has been provided to the SSL adapter via the
 * callback.
 */
typedef struct
{
    ByteArray_t crt;    /**< own certificate chain as a null-terminated PEM string of certificates */
    ByteArray_t key;    /**< own private key as binary-encoded DER */
    ByteArray_t ca;     /**< trusted CAs as a null-terminated PEM string of certificates */
    ByteArray_t crl;    /**< trusted CRLs as binary-encoded DER */
} PkiInfo_t;

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
/**
 * Callback is used by application layer to check peer's certificate CN field.
 * If set, this callback will be invoked during handshake after certificate verification.
 *
 * @param[out] cn     peer's certificate Common Name field.
 *                    If common name was not found, cn will be set to NULL.
 * @param[out] cnLen  peer's certificate Common Name field length.
 *                    If CN was not found, cnLen will be set to 0.
 *
 * @return  CA_STATUS_OK or CA_STATUS_FAIL. In case CA_STATUS_FAIL is returned,
 *          handshake will be dropped.
 */
typedef CAResult_t (*PeerCNVerifyCallback)(const unsigned char *cn, size_t cnLen);

/**
 * API to set callback that checks peer's certificate Common Name field
 * @param[in] cb callback to utilize certificate Common Name field
 */
void CAsetPeerCNVerifyCallback(PeerCNVerifyCallback cb);
#endif

/**
 * Register callback to get types of TLS suites.
 * @param[in]   getCredTypesHandler    Get types of TLS suites callback.
 * @return  ::CA_STATUS_OK or appropriate error code.
 */
CAResult_t CAregisterGetCredentialTypesHandler(CAgetCredentialTypesHandler getCredTypesHandler);

/**
 * Register callback to receive the result of TLS handshake.
 * @param[in] tlsHandshakeCallback callback for get tls handshake result
 * @return ::CA_STATUS_OK
 */
CAResult_t CAregisterSslHandshakeCallback(CAHandshakeErrorCallback tlsHandshakeCallback);

/**
 * Register callback to get TLS PSK credentials.
 * @param[in]   getTlsCredentials    GetDTLS Credetials callback.
 * @return  ::CA_STATUS_OK
 */
CAResult_t CAregisterPskCredentialsHandler(CAgetPskCredentialsHandler getTlsCredentials);

/**
 * @brief   Callback function type for getting PKIX info
 *
 * @param   inf[out]   PKIX related info
 *
 * @return  NONE
 */
typedef void (*CAgetPkixInfoHandler)(PkiInfo_t * inf);

/**
 * Register callback to get PKIX related info.
 * @param[in]   getPkixInfoHandler    Get PKIX related info callback.
 * @return  ::CA_STATUS_OK or appropriate error code.
 */
CAResult_t CAregisterPkixInfoHandler(CAgetPkixInfoHandler getPkixInfoHandler);

/**
 * Select the cipher suite for dtls handshake.
 *
 * @param[in] cipher  cipher suite (Note : Make sure endianness).
 *                        TLS_RSA_WITH_AES_256_CBC_SHA256          0x3D
 *                        TLS_RSA_WITH_AES_128_GCM_SHA256          0x009C
 *                        TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256  0xC02B
 *                        TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8       0xC0AE
 *                        TLS_ECDHE_ECDSA_WITH_AES_128_CCM         0xC0AC
 *                        TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256  0xC023
 *                        TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384  0xC024
 *                        TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384  0xC02C
 *                        TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256    0xC037
 *                        TLS_ECDH_anon_WITH_AES_128_CBC_SHA       0xC018
 * @param[in] adapter  transport adapter (TCP/IP/BLE)
 *
 * @retval  ::CA_STATUS_OK    Successful.
 * @retval  ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CASelectCipherSuite(const uint16_t cipher, CATransportAdapter_t adapter);

/**
 * Enable TLS_ECDH_anon_WITH_AES_128_CBC_SHA cipher suite in dtls.
 *
 * @param[in] enable  TRUE/FALSE enables/disables anonymous cipher suite.
 *
 * @retval  ::CA_STATUS_OK    Successful.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 *
 * @note anonymous cipher suite should only be enabled for 'JustWorks' provisioning.
 */
CAResult_t CAEnableAnonECDHCipherSuite(const bool enable);


/**
 * Generate ownerPSK using PRF.
 * OwnerPSK = TLS-PRF('master key' , 'oic.sec.doxm.jw',
 *                    'ID of new device(Resource Server)',
 *                    'ID of owner smart-phone(Provisioning Server)')
 *
 * @param[in] endpoint  information of network address.
 * @param[in] label  Ownership transfer method e.g)"oic.sec.doxm.jw".
 * @param[in] labelLen  Byte length of label.
 * @param[in] rsrcServerDeviceID  ID of new device(Resource Server).
 * @param[in] rsrcServerDeviceIDLen  Byte length of rsrcServerDeviceID.
 * @param[in] provServerDeviceID  label of previous owner.
 * @param[in] provServerDeviceIDLen  byte length of provServerDeviceID.
 * @param[in,out] ownerPSK  Output buffer for owner PSK.
 * @param[in] ownerPskSize  Byte length of the ownerPSK to be generated.
 *
 * @retval  ::CA_STATUS_OK    Successful.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAGenerateOwnerPSK(const CAEndpoint_t *endpoint,
                              const uint8_t* label, const size_t labelLen,
                              const uint8_t* rsrcServerDeviceID,
                              const size_t rsrcServerDeviceIDLen,
                              const uint8_t* provServerDeviceID,
                              const size_t provServerDeviceIDLen,
                              uint8_t* ownerPSK, const size_t ownerPskSize);

/**
 * Initiate DTLS handshake with selected cipher suite.
 *
 * @param[in] endpoint  information of network address.
 *
 * @retval  ::CA_STATUS_OK    Successful.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAInitiateHandshake(const CAEndpoint_t *endpoint);

/**
 * Close the DTLS session.
 *
 * @param[in] endpoint  information of network address.
 *
 * @retval  ::CA_STATUS_OK    Successful.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAcloseSslSession(const CAEndpoint_t *endpoint);

/**
 * Initiate TLS handshake with selected cipher suite.
 *
 * @param[in] endpoint information of network address.
 *
 * @retval  ::CA_STATUS_OK    Successful.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAinitiateSslHandshake(const CAEndpoint_t *endpoint);

/**
 * Close the DTLS session.
 *
 * @param[in] endpoint  information of network address.
 *
 * @retval  ::CA_STATUS_OK    Successful.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAcloseSslConnection(const CAEndpoint_t *endpoint);


/**
 * Close All of DTLS sessions.
 */
void CAcloseSslConnectionAll(CATransportAdapter_t transportType);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* CA_SECURITY_INTERFACE_H_ */

