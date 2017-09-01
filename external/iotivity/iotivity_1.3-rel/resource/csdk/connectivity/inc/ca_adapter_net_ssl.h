/* *****************************************************************
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
#ifndef CA_ADAPTER_NET_SSL_H_
#define CA_ADAPTER_NET_SSL_H_

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include "caadapterutils.h"
#include "cainterface.h"
#include "caadapterinterface.h"

/**
 * Currently TLS supported adapters(3) WIFI, ETHENET and BLE for linux platform.
 */
#define MAX_SUPPORTED_ADAPTERS 3

typedef void (*CAPacketReceivedCallback)(const CASecureEndpoint_t *sep,
                                         const void *data, size_t dataLength);

typedef ssize_t (*CAPacketSendCallback)(CAEndpoint_t *endpoint,
                                        const void *data, size_t dataLength);

/**
 * Select the cipher suite for dtls handshake
 *
 * @param[in] cipher    cipher suite
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
 *
 * @retval  ::CA_STATUS_OK for success, otherwise some error value
 */
CAResult_t CAsetTlsCipherSuite(const uint32_t cipher);

/**
 * Used set send,recv and error callbacks for different adapters(WIFI,EtherNet).
 *
 * @param[in]  recvCallback    packet received callback.
 * @param[in]  sendCallback    packet sent callback.
 * @param[in]  errorCallback   packet error callback.
 * @param[in]  type  type of adapter.
 *
 */
void CAsetSslAdapterCallbacks(CAPacketReceivedCallback recvCallback,
                              CAPacketSendCallback sendCallback,
                              CAErrorHandleCallback errorCallback,
                              CATransportAdapter_t type);

/**
 * Register callback to get credentials types.
 * @param[in]   credTypesCallback    callback to get credential types.
 */
void CAsetCredentialTypesCallback(CAgetCredentialTypesHandler credTypesCallback);
/**
 * Register callback to get credential types.
 * @param[in]  typesCallback    callback to get credential types.
 */
void CAsetSslCredentialsCallback(CAgetPskCredentialsHandler credCallback);

/**
 * Close the TLS session
 *
 * @param[in] endpoint  information of network address
 *
 * @retval  ::CA_STATUS_OK for success, otherwise some error value
 */
CAResult_t CAcloseSslConnection(const CAEndpoint_t *endpoint);

/**
 * initialize mbedTLS library and other necessary initialization.
 *
 * @return  0 on success otherwise a positive error value.
 * @retval  ::CA_STATUS_OK  Successful.
 * @retval  ::CA_MEMORY_ALLOC_FAILED  Memory allocation failed.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 *
 */
CAResult_t CAinitSslAdapter();

/**
 * de-inits mbedTLS library and free the allocated memory.
 */
void CAdeinitSslAdapter();

/**
 * Performs TLS encryption of the CoAP PDU.
 *
 * If a DTLS session does not exist yet with the @dst,
 * a TLS handshake will be started. In case where a new TLS handshake
 * is started, pdu info is cached to be send when session setup is finished.
 *
 * @param[in]  endpoint  address to which data will be sent.
 * @param[in]  data  length of data.
 * @param[in]  dataLen  length of given data
 *
 * @return  0 on success otherwise a positive error value.
 * @retval  ::CA_STATUS_OK  Successful.
 * @retval  ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 *
 */

CAResult_t CAencryptSsl(const CAEndpoint_t *endpoint, const void *data, size_t dataLen);

/**
 * Performs TLS decryption of the data.
 *
 * @param[in]  sep  address and flags for which data will be decrypted.
 * @param[in]  data  length of data.
 * @param[in]  dataLen  length of given data
 *
 * @return  0 on success otherwise a positive error value.
 * @retval  ::CA_STATUS_OK  Successful.
 * @retval  ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 *
 */
CAResult_t CAdecryptSsl(const CASecureEndpoint_t *sep, uint8_t *data, size_t dataLen);

/**
 * Initiate TLS handshake with selected cipher suite.
 *
 * @param[in] endpoint  information of network address
 *
 * @retval  ::CA_STATUS_OK for success, otherwise some error value
 */
CAResult_t CAinitiateSslHandshake(const CAEndpoint_t *endpoint);

/**
 * Register callback to deliver the result of TLS handshake
 * @param[in] tlsHandshakeCallback Callback to receive the result of TLS handshake.
 */
void CAsetSslHandshakeCallback(CAHandshakeErrorCallback tlsHandshakeCallback);

/**
 * Generate ownerPSK using PRF
 * OwnerPSK = TLS-PRF('master key' , 'oic.sec.doxm.jw',
 *                                    'ID of new device(Resource Server)',
 *                                    'ID of owner smart-phone(Provisioning Server)')
 *
 * @param[in] endpoint  information of network address
 * @param[in] label  Ownership transfer method e.g)"oic.sec.doxm.jw"
 * @param[in] labelLen  Byte length of label
 * @param[in] rsrcServerDeviceID  ID of new device(Resource Server)
 * @param[in] rsrcServerDeviceIDLen  Byte length of rsrcServerDeviceID
 * @param[in] provServerDeviceID  label of previous owner
 * @param[in] provServerDeviceIDLen  byte length of provServerDeviceID
 * @param[in,out] ownerPSK  Output buffer for owner PSK
 * @param[in] ownerPSKSize  Byte length of the ownerPSK to be generated
 *
 * @retval  ::CA_STATUS_OK for success, otherwise some error value
 */
CAResult_t CAsslGenerateOwnerPsk(const CAEndpoint_t *endpoint,
                    const uint8_t* label, const size_t labelLen,
                    const uint8_t* rsrcServerDeviceId, const size_t rsrcServerDeviceIdLen,
                    const uint8_t* provServerDeviceId, const size_t provServerDeviceIdLen,
                    uint8_t* ownerPsk, const size_t ownerPskSize);

/**
 * Gets a copy of CA secure endpoint info corresponding for endpoint.
 *
 * @param[in]  peer    remote address
 * @param[out] sep     copy of secure endpoint data
 *
 * @return  CA_STATUS_OK on success; other error code on failure
 */
CAResult_t GetCASecureEndpointData(const CAEndpoint_t* peer, CASecureEndpoint_t *sep);

/**
 * Adds a bit to the attributes field of a secure endpoint.
 *
 * @param[in]  peer         remote address
 * @param[in]  newAttribute bit to be added to the attributes field
 *
 * @return  true if the secure endpoint has been found, false otherwise.
 */
bool SetCASecureEndpointAttribute(const CAEndpoint_t* peer, uint32_t newAttribute);

/**
 * Gets the attributes field of a secure endpoint.
 *
 * @param[in]  peer          remote address
 * @param[out] allAttributes all the attributes bits for that remote address
 *
 * @return  true if the secure endpoint has been found, false otherwise.
 */
bool GetCASecureEndpointAttributes(const CAEndpoint_t* peer, uint32_t* allAttributes);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* CA_ADAPTER_NET_SSL_H_ */


