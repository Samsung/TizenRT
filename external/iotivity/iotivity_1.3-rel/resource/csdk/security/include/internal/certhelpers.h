//******************************************************************
//
// Copyright 2017 Microsoft
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//******************************************************************

#ifndef IOTVT_SRM_CERTHELPERS_H_
#define IOTVT_SRM_CERTHELPERS_H_

#if defined(__WITH_TLS__) || defined(__WITH_DTLS__)

#include "mbedtls/pk.h"
#include <time.h>

/**
 * Internal certificate request function; used by CSR resource handler
 *
 * @param[in]  subject  Subject to put into the CSR
 * @param[in]  keyPair  Key pair for which to generate the CSR
 * @param[in]  encoding Encoding to use; valid values are OIC_ENCODING_PEM and OIC_ENCODING_DER
 * @param[out] csr      OCByteString containing the CSR in PEM. Caller takes ownership
 *                      of allocated memory and must call OICFree on csr->bytes when finished.
 *                      If encoding is OIC_ENCODING_DER, csr->len is the length of the encoded CSR.
 *                      If encoding is OIC_ENCODING_PEM, csr->len is the length of the PEM string including the
 *                      terminating NULL.
 *
 * @return 0 on success, <0 on failure
 */
int OCInternalCSRRequest(const char *subject, mbedtls_pk_context *keyPair, 
                         OicEncodingType_t encoding, OCByteString *csr);

/**
 * Internal key pair generation function; used by CSR resource handler
 *
 * @param[in,out] keyPair Key object to generate; caller should already have called mbedtls_pk_init on it.
 *
 * @return 0 on success, <0 on failure
 */
int OCInternalGenerateKeyPair(mbedtls_pk_context *keyPair);

#endif

/**
 * Determine if a buffer contains a role certificate, and optionally, return
 * a DER encoding of the public key. This is intended as a simple check when the
 * certificate is presented to reject obviously invalid certificates.
 *
 * The following properties are checked:
 * 1. The certificate is a valid X.509 certificate.
 * 2. It contains the role certificate Extended Key Usage.
 * 3. It contains at least one Subject Alternative Name extension that validly encodes a role.
 *
 * It does NOT validate the cryptographic signature nor check its time validity.
 * These checks should be done when the certificate is being used as part of an access control check, 
 * as that is when the time validity check should be made, and when trusted CAs are known.
 *
 * @param[in] buf           Buffer containing certificate as a PEM string
 * @param[in] bufLen        Length of buffer including terminating NULL
 * @param[out] pubKey       Optional pointer to receive buffer containing binary DER encoding of public key.
 *                          Caller must free with OICFree when finished. Pass NULL in to not retrieve the
 *                          public key.
 * @param[out] pubKeyLen    Pointer to variable to receive size of pubKey. Ignored if pubKey is NULL.
 *
 * @return OC_STACK_OK if certificate satisfies the properties above, and the public key is
 *                     successfully extracted (if requested).
 *         OC_STACK_INVALID_PARAM if the certificate does not meet the properties above.
 *         OC_STACK_NO_MEMORY or OC_STACK_ERROR if some other error arose during the check.
 */
OCStackResult OCInternalIsValidRoleCertificate(const uint8_t *buf, size_t bufLen,
                                               uint8_t **pubKey, size_t *pubKeyLen);

/**
 * Determine if a buffer contains a valid chain of certificates. This is intended to verify
 * one or more intermediate CA certificates are valid.
 * 
 * This only checks that they are valid X.509 structures; no verification of the cryptographic
 * signature of time-validity is performed. These should be done at point of use.
 *
 * @param[in] buf           Buffer containing certificates as a PEM string
 * @param[in] bufLen        Length of buffer including terminating NULL
 *
 * @return OC_STACK_OK if buf contains a valid chain of certificates
 *         OC_STACK_INVALID_PARAM if buf cannot be parsed as a chain of certificates
 *         OC_STACK_ERROR if some other error arose during the check
 */
OCStackResult OCInternalIsValidCertChain(const uint8_t *buf, size_t bufLen);

/**
 * Verify the validity of a role certificate chain against a set of trusted certificate
 * authorities. If successful, a list of roles authorized by this certificate is returned.
 *
 * The certificate's validity is also verified against the current time.
 *
 * The GetPemCaCert function from credresource.h can be used to retrieve the set of trusted CAs
 * from the /oic/sec/cred resource in a form suitable for the trustedCaCerts and trustedCaCertsLength
 * parameters.
 *
 * @param[in] certificateChain      OicSecKey_t containing one or more certificates
 * @param[in] trustedCaCerts        PEM string containing the trusted CAs certificates
 * @param[in] trustedCaCertsLength  Length of trustedCaCerts (including terminating NULL)
 * @param[out] roles                Pointer to receive array of OicSecRole_t objects listing roles
 *                                  Caller must call OICFree to release this memory when finished
 * @param[out] rolesLength          Length of returned roles array
 * @param[out] notValidAfter        struct tm to receive notValidAfter time of the certificate
 *
 * @return OC_STACK_OK if the certificate is valid.
 *         OC_STACK_INVALID_PARAM if the certificate is not valid.
 *         OC_STACK_NO_MEMORY or OC_STACK_ERROR if some other error arose during validation.
 */
OCStackResult OCInternalVerifyRoleCertificate(const OicSecKey_t *certificateChain, const uint8_t *trustedCaCerts,
                                              size_t trustedCaCertsLength, OicSecRole_t **roles,
                                              size_t *rolesLength, struct tm *notValidAfter);
#endif
