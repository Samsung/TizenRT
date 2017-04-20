/*!
 *  @file       isp_driver_aes_securekey.h
 *  @brief      Headerfile : isp driver for aes securekey
 *  @author     jinsu.hyun
 *  @version    v0.10 : 2016.7.25 Init
 *  @version    v0.20 : 2016.7.29 support securekey
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.90 : 2016.9.03 2nd.  release version
 */

#ifndef ISP_DRIVER_AES_SECUREKEY_H_
#define ISP_DRIVER_AES_SECUREKEY_H_

#include "mb_cmd_aes_securekey.h"

// security api

int isp_aes_generate_key_securekey(unsigned int key_byte_len, unsigned int key_index);

int isp_aes_encrypt_securekey(struct sAES_PARAM *aes_param, unsigned int index);
int isp_aes_decrypt_securekey(struct sAES_PARAM *aes_param, unsigned int index);

int isp_aes_ecb_securekey(struct sAES_PARAM *aes_param, unsigned int enc, unsigned int key_index);
int isp_aes_cbc_securekey(struct sAES_PARAM *aes_param, unsigned int enc, unsigned int key_index);
int isp_aes_ctr_securekey(struct sAES_PARAM *aes_param, unsigned int enc, unsigned int key_index);
int isp_aes_xts_securekey(struct sAES_PARAM *aes_param, unsigned int enc, unsigned int key_index);
int isp_aes_ccm_securekey(struct sAES_PARAM *aes_param, unsigned int enc, unsigned int key_index);
int isp_aes_gcm_securekey(struct sAES_PARAM *aes_param, unsigned int enc, unsigned int key_index);

#endif							/* ISP_DRIVER_AES_SECUREKEY_H_ */
