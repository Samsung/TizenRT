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

#if defined(__WITH_TLS__) || defined(__WITH_DTLS__)

#ifndef OCCERTUTILITY_H_
#define OCCERTUTILITY_H_

#include "experimental/securevirtualresourcetypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Generate a random serial number suitable for use with a certificate.
 * Callers must record used serial numbers and ensure serial numbers do not
 * get repeated!
 *
 * @param[OUT] serial      Pointer to a string that will be set to the location of the ASCII serial number.
 *                         Memory for this buffer will be allocated internally; caller must call OICFree on serial
 *                         when finished to free its memory.
 * @param[OUT] serialLen   Variable to receive size of serial, which will include the terminating NULL.
 *
 * @return OC_STACK_OK if generated successfully; error otherwise.
 */
OCStackResult OC_CALL OCGenerateRandomSerialNumber(char **serial, size_t *serialLen);

/**
 * Generate a new NIST P-256 elliptic curve key pair and return the public and private
 * keys as PEM encoded strings.
 *
 * @param[OUT] publicKey     Pointer to receive PEM encoded public key. Memory is allocated internally
 *                           and must be freed by caller with OICFree.
 * @param[OUT] publicKeyLen  Variable to receive length of publicKey, which will include terminating NULL.
 * @param[OUT] privateKey    Pointer to receive PEM encoded private key. Memory is allocated internally
 *                           and, when finished, caller should first call OICClearMemory to securely erase
 *                           its contents and then freed with OICFree. Caller is responsible for ensuring
 *                           this key remains protected and secret!
 * @param[OUT] privateKeyLen Variable to receive length of privateKey, which will include terminating NULL.
 */
OCStackResult OC_CALL OCGenerateKeyPair(char **publicKey, size_t *publicKeyLen,
                                        char **privateKey, size_t *privateKeyLen);

/**
 * Generate a certificate to act as a Certificate Authority (CA).
 *
 * @param subject               Comma-separated list of RDN types and values:
 *                              e.g. "C=US, O=Open Connectivity Foundation, CN=Main CA"
 * @param subjectPublicKey      Subject's public key in PEM format
 * @param issuerCert            Issuer's certificate in PEM format
 *                              If this certificate will be self-signed, pass in NULL.
 * @param issuerPrivateKey      Issuer's private key in PEM format
 *                              If self-signing (issuerCert is NULL), this must be the private
 *                              key corresponding to subjectPublicKey.
 * @param serial                String containing the serial number in ASCII numerals:
 *                              e.g., "12345". Caller must ensure each certificate issued by a
 *                              CA has a unique serial number, and it is recommended to generate
 *                              them randomly by producing 19 random bytes and converting them to
 *                              a numerical value. See GenerateRandomSerialNumber().
 * @param notValidBefore        The notValidBefore date in UTC for the certificate in the form YYYYMMDDhhmmss
 *                              e.g., "20131231235959" for December 31st 2013 at 23:59:59
 * @param notValidAfter         The notValidAfter date in UTC for the certificate in the form YYYYMMDDhhmmss
 *                              e.g., "20140101010203" for January 1st 2014 at 01:02:03
 * @param[OUT] certificate      Pointer to a buffer that will receive the PEM-encoded certificate. Memory will be
 *                              allocated internally; caller must call OICFree on certificate when finished to free
 *                              its memory.
 * @param[OUT] certificateLen   Variable to receive the size of certificate, which will include the terminating NULL.
 *
 * @return OC_STACK_OK if successful, error code otherwise
 */
OCStackResult OC_CALL OCGenerateCACertificate(
    const char *subject,
    const char *subjectPublicKey,
    const char *issuerCert,
    const char *issuerPrivateKey,
    const char *serial,
    const char *notValidBefore,
    const char *notValidAfter,
    char **certificate,
    size_t *certificateLen);

/**
 * Generate a certificate for a device's identity.
 *
 * @param subjectUuid           UUID for the device to use the certificate.
 * @param subjectPublicKey      Subject's public key in PEM format
 * @param issuerCert            Issuer's certificate in PEM format
 * @param issuerPrivateKey      Issuer's private key in PEM format
 * @param serial                String containing the serial number in ASCII numerals:
 *                              e.g., "12345". Caller must ensure each certificate issued by a
 *                              CA has a unique serial number, and it is recommended to generate
 *                              them randomly by producing 19 random bytes and converting them to
 *                              a numerical value. See GenerateRandomSerialNumber().
 * @param notValidBefore        The notValidBefore date in UTC for the certificate in the form YYYYMMDDhhmmss
 *                              e.g., "20131231235959" for December 31st 2013 at 23:59:59
 * @param notValidAfter         The notValidAfter date in UTC for the certificate in the form YYYYMMDDhhmmss
 *                              e.g., "20140101010203" for January 1st 2014 at 01:02:03
 * @param[OUT] certificate      Pointer to a buffer that will receive the PEM-encoded certificate. Memory will be
 *                              allocated internally; caller must call OICFree on certificate when finished to free
 *                              its memory.
 * @param[OUT] certificateLen   Variable to receive the size of certificate, which will include the terminating NULL.
 *
 * @return OC_STACK_OK if successful, error code otherwise
 */
OCStackResult OC_CALL OCGenerateIdentityCertificate(
    const OicUuid_t *subjectUuid,
    const char *subjectPublicKey,
    const char *issuerCert,
    const char *issuerPrivateKey,
    const char *serial,
    const char *notValidBefore,
    const char *notValidAfter,
    char **certificate,
    size_t *certificateLen);

/**
 * Generate a role certificate for a device.
 *
 * @param subjectUuid           UUID for the device to use the certificate.
 * @param subjectPublicKey      Subject's public key in PEM format
 * @param issuerCert            Issuer's certificate in PEM format
 * @param issuerPrivateKey      Issuer's private key in PEM format
 * @param serial                String containing the serial number in ASCII numerals:
 *                              e.g., "12345". Caller must ensure each certificate issued by a
 *                              CA has a unique serial number, and it is recommended to generate
 *                              them randomly by producing 19 random bytes and converting them to
 *                              a numerical value. See GenerateRandomSerialNumber().
 * @param notValidBefore        The notValidBefore date in UTC for the certificate in the form YYYYMMDDhhmmss
 *                              e.g., "20131231235959" for December 31st 2013 at 23:59:59
 * @param notValidAfter         The notValidAfter date in UTC for the certificate in the form YYYYMMDDhhmmss
 *                              e.g., "20140101010203" for January 1st 2014 at 01:02:03
 * @param role                  The role to encode in the certificate.
 * @param authority             The role authority to encode in the certificate. This field is optional;
 *                              to omit, use NULL, and authority is construed to be the certificate issuer.
 * @param[OUT] certificate      Pointer to a buffer that will receive the PEM-encoded certificate. Memory will be
 *                              allocated internally; caller must call OICFree on certificate when finished to free
 *                              its memory.
 * @param[OUT] certificateLen   Variable to receive the size of certificate, which will include the terminating NULL.
 *
 * @return OC_STACK_OK if successful, error code otherwise
 */
OCStackResult OC_CALL OCGenerateRoleCertificate(
    const OicUuid_t *subjectUuid,
    const char *subjectPublicKey,
    const char *issuerCert,
    const char *issuerPrivateKey,
    const char *serial,
    const char *notValidBefore,
    const char *notValidAfter,
    const char *role,
    const char *authority,
    char **certificate,
    size_t *certificateLen);

/**
 * Extract a UUID from a CSR.
 *
 * @param[in]  csr  The CSR containing the UUID as null-terminated PEM.
 * @param[out] uuid The UUID in the CSR
 *
 * @return OC_STACK_OK if successful, error code otherwise
 */
OCStackResult OC_CALL OCGetUuidFromCSR(const char* csr, OicUuid_t* uuid);

/**
 * Extract a public key from a CSR.
 *
 * @param[in]  csr       The CSR containing the public key, as null-terminated PEM.
 * @param[out] publicKey The public key is output here as null-terminated PEM.
 *                       Callers must call OICFree when finished.
 *
 * @return OC_STACK_OK if successful, error code otherwise
 */
OCStackResult OC_CALL OCGetPublicKeyFromCSR(const char* csr, char** publicKey);

/**
 * Verify the signature in a CSR is valid.
 *
 * @param[in] csr The CSR to check, as null-terminated PEM.
 *
 * @return OC_STACK_OK if successful, error code otherwise
 *
 * @remark Requires that ECDSA with SHA-256 be used for the signature.
 */
OCStackResult OC_CALL OCVerifyCSRSignature(const char* csr);

/**
 * Convert a CSR from DER encoding to PEM encoding.
 *
 * @param[in] derCSR The CSR to convert, encoded as DER
 * @param[in] derCSRLen Then number of bytes in derCSR
 * @param[out] pemCSR The output, PEM encoded, null-terminated CSR. Callers
 *                    call OICFree when finished.
 *
 * @return OC_STACK_OK if successful, error code otherwise
 */
OCStackResult OC_CALL OCConvertDerCSRToPem(const char* derCSR, size_t derCSRLen, char** pemCSR);

#ifdef __cplusplus
}
#endif

#endif /* OCCERTUTILITY_H_ */

#endif /* defined(__WITH_TLS__) || defined(__WITH_DTLS__) */