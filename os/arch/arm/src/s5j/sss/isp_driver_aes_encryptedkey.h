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
 *  @file       isp_driver_aes_encryptedkey.h
 *  @brief      Headerfile : isp driver for aes encryptedkey
 *  @author     jinsu.hyun
 *  @version    v0.00 : 2017.01.19 Init. release version
 */

#ifndef _ISP_DRIVER_AES_ENCRYPTEDKEY_H_
#define _ISP_DRIVER_AES_ENCRYPTEDKEY_H_

#include "mb_cmd_aes_securekey.h"

// security api

int isp_aes_generate_key_encryptedkey(unsigned int key_byte_len, unsigned char *encryptedkey);

int isp_aes_encrypt_encryptedkey(struct sAES_PARAM *aes_param, unsigned char *encryptedkey);
int isp_aes_decrypt_encryptedkey(struct sAES_PARAM *aes_param, unsigned char *encryptedkey);

int isp_aes_ecb_encryptedkey(struct sAES_PARAM *aes_param, unsigned int enc, unsigned char *encryptedkey);
int isp_aes_cbc_encryptedkey(struct sAES_PARAM *aes_param, unsigned int enc, unsigned char *encryptedkey);
int isp_aes_ctr_encryptedkey(struct sAES_PARAM *aes_param, unsigned int enc, unsigned char *encryptedkey);
int isp_aes_xts_encryptedkey(struct sAES_PARAM *aes_param, unsigned int enc, unsigned char *encryptedkey);
int isp_aes_ccm_encryptedkey(struct sAES_PARAM *aes_param, unsigned int enc, unsigned char *encryptedkey);
int isp_aes_gcm_encryptedkey(struct sAES_PARAM *aes_param, unsigned int enc, unsigned char *encryptedkey);

#endif /* _ISP_DRIVER_AES_ENCRYPTEDKEY_H_ */
