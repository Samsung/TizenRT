/*!
 *  @file		isp_driver_ecdsa_securekey.h
 *  @brief		Headerfile : isp driver for ecdsa securekey
 *  @author		jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.60 : 2016.12.23 Support sign/verify with msg_disgest
 */

#ifndef ISP_DRIVER_ECDSA_SECUREKEY_H_
#define ISP_DRIVER_ECDSA_SECUREKEY_H_

#include "mb_cmd_ecdsa_securekey.h"

// ======================================
// Function
// ======================================
int isp_ecdsa_sign_securekey(struct sECC_SIGN * signature, unsigned char * msg, unsigned int msg_byte_len, unsigned int key_index);
int isp_ecdsa_verify_securekey(struct sECC_SIGN * signature, unsigned char * msg, unsigned int msg_byte_len, unsigned int key_index);

int isp_ecdsa_sign_md_securekey(struct sECC_SIGN * signature, unsigned char * msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index);
int isp_ecdsa_verify_md_securekey(struct sECC_SIGN * signature, unsigned char * msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index);

int isp_ecdsa_generate_key_securekey(unsigned int key_index, unsigned int object_id);
int isp_ecdsa_get_publickey_securekey(struct sECC_KEY *ecc_publickey, unsigned int key_index, unsigned int object_id);

#endif /* ISP_DRIVER_ECDSA_SECUREKEY_H_ */
