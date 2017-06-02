/* *****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 *
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
 * *****************************************************************/
#ifndef SS_EMUL_H
#define SS_EMUL_H

#include <mbedtls/ssl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Type define
 */
 enum SSE_KEY_TYPE
 {
     KEYTYPE_NONE = -1,
     KEYTYPE_RSA,
     KEYTYPE_ECC
 };

/**
 * This function set own certificate file and key file path to use hw emulation
 *
 * @param[in] cert_filepath  path for own certificate file
 * @param[in] key_filepath  path for private key file
 * @param[in] pwd  password for private key file
 * @return  0 on success, otherwise a negative value
 */
int SSemulSetCertkeyFilepath(const char* cert_filepath,
                                            const char* key_filepath, const char* pwd);

/**
 * This function get type of stored key from hw emulation
 *
 * @param[in] keyContext  key context object that identifies proper certificate chain
 * @return  SSE_KEY_TYPE
 */
int SSemulGetKeytype(const void* keyContext);

/**
 * This function load own certificate data from hw emulation
 *
 * @param[in] keyContext  key context object that identifies proper certificate chain
 * @param[out] cert_chain  certificate chain in binary
 * @param[out] cert_chain_len  total length of certificate chain
 * @return  0 on success, otherwise a negative value
 */
int SSemulLoadOwncert(const void* keyContext,
                                           uint8_t** cert_chain, size_t* cert_chain_len);

/**
 * This function get private key length from hw emulation
 *
 * @param[in] keyContext  key context object that identifies proper certificate chain
 * @return  positive value on success, otherwise a negative value or zero
 */
int SSemulGetKeylen(const void* keyContext);

/**
 * This function sign hash data with RSA private key from hw emulation
 *
 * @param[in] keyContext  key context object that identifies proper certificate chain
 * @param[in] f_rng  RNG function
 * @param[in] p_rng  RNG parameter
 * @param[in] mode  RSA key mode
 * @param[in] md_alg  md algorithm
 * @param[in] hashlen  length of hash
 * @param[in] hash  hash data to be signed
 * @param[in] sig  signature of hash
 * @return  0 on success, otherwise a negative value
 */
int SSemulRsaSign(const void *keyContext,
                            int (*f_rng)(void *, unsigned char *, size_t), void *p_rng,
                            int mode, mbedtls_md_type_t md_alg, unsigned int hashlen,
                            const unsigned char *hash, unsigned char *sig );

/**
 * This function sign hash data with ECC private key from hw emulation
 *
 * @param[in] ctx  key context poniter of mbedtls_pk_context
 * @param[in] md_alg  md algorithm
 * @param[in] hash  hash data to be signed
 * @param[in] hashlen  length of hash
 * @param[in] sig  signature of hash
 * @param[in] sig_len  length of signature
 * @param[in] f_rng  RNG function
 * @param[in] p_rng  RNG parameter
 * @return  0 on success, otherwise a negative value
 */
int SSemulEcdsaSign( void *ctx, mbedtls_md_type_t md_alg,
                   const unsigned char *hash, size_t hash_len,
                   unsigned char *sig, size_t *sig_len,
                   int (*f_rng)(void *, unsigned char *, size_t), void *p_rng );

#ifdef __cplusplus
}
#endif

#endif // SS_EMUL_H


