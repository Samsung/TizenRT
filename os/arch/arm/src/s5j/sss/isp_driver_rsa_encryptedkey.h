/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
/*!
 *  @file		isp_driver_rsa_encryptedkey.h
 *  @brief		Headerfile : isp driver for rsa encryptedkey
 *  @author		jinsu.hyun
 *  @version    v0.00 : 2017.01.19 Init. release version
 */

#ifndef _ISP_DRIVER_RSA_ENCRYPTEDKEY_H_
#define _ISP_DRIVER_RSA_ENCRYPTEDKEY_H_

#include "mb_cmd_rsa_securekey.h"

// ======================================
// Function
// ======================================
int isp_rsa_encrypt_encryptedkey(unsigned char *output, unsigned int *output_byte_len, unsigned char *input, unsigned int input_byte_len, unsigned char *encryptedkey);
int isp_rsa_decrypt_encryptedkey(unsigned char *output, unsigned int *output_byte_len, unsigned char *input, unsigned int input_byte_len, unsigned char *encryptedkey);

int isp_rsa_sign_encryptedkey(struct sRSA_SIGN *rsa_sign, unsigned char *msg, unsigned int msg_byte_len, unsigned char *encryptedkey);
int isp_rsa_verify_encryptedkey(struct sRSA_SIGN *rsa_sign, unsigned char *msg, unsigned int msg_byte_len, unsigned char *encryptedkey);

int isp_rsa_sign_md_encryptedkey(struct sRSA_SIGN *rsa_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned char *encryptedkey);
int isp_rsa_verify_md_encryptedkey(struct sRSA_SIGN *rsa_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned char *encryptedkey);

int isp_rsa_get_pukey_encryptedkey(struct sRSA_KEY *rsa_key, unsigned object_id, unsigned char *encryptedkey);

#endif /* _ISP_DRIVER_RSA_ENCRYPTEDKEY_H_ */
