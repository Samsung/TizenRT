/*!
 *  @file		isp_driver_rsa_securekey.h
 *  @brief		Headerfile : isp driver for rsa securekey
 *  @author		jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v1.00 : 2016.12.23 Support sign/verify with msg_disgest
 */

#ifndef ISP_DRIVER_RSA_SECUREKEY_H_
#define ISP_DRIVER_RSA_SECUREKEY_H_

#include "mb_cmd_rsa_securekey.h"

// ======================================
// Function
// ======================================
int isp_rsa_encrypt_securekey(unsigned char *output, unsigned int *output_byte_len, unsigned char *input, unsigned int input_byte_len, unsigned int key_index);
int isp_rsa_decrypt_securekey(unsigned char *output, unsigned int *output_byte_len, unsigned char *input, unsigned int input_byte_len, unsigned int key_index);

int isp_rsa_sign_securekey(struct sRSA_SIGN *rsa_sign, unsigned char *msg, unsigned int msg_byte_len, unsigned int key_index);
int isp_rsa_verify_securekey(struct sRSA_SIGN *rsa_sign, unsigned char *msg, unsigned int msg_byte_len, unsigned int key_index);

int isp_rsa_sign_md_securekey(struct sRSA_SIGN *rsa_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index);
int isp_rsa_verify_md_securekey(struct sRSA_SIGN *rsa_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index);

int isp_rsa_generate_key_securekey(unsigned key_index, unsigned object_id, unsigned int pukey_e);
int isp_rsa_get_pukey_securekey(struct sRSA_KEY *rsa_key, unsigned object_id, unsigned key_index);

#endif							/* ISP_DRIVER_RSA_SECUREKEY_H_ */
