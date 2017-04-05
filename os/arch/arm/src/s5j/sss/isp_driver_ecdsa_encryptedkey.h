/*!
 *  @file		isp_driver_ecdsa_encryptedkey.h
 *  @brief		Headerfile : isp driver for ecdsa encryptedkey
 *  @author		jinsu.hyun
 *  @version    v0.00 : 2017.01.19 Init. release version
 */

#ifndef _ISP_DRIVER_ECDSA_ENCRYPTEDKEY_H_
#define _ISP_DRIVER_ECDSA_ENCRYPTEDKEY_H_

#include "mb_cmd_ecdsa_securekey.h"

// ======================================
// Function
// ======================================


int isp_ecdsa_sign_encryptedkey(struct sECC_SIGN * signature, unsigned char * msg, unsigned int msg_byte_len, unsigned char *encryptedkey);
int isp_ecdsa_verify_encryptedkey(struct sECC_SIGN * signature, unsigned char * msg, unsigned int msg_byte_len, unsigned char *encryptedkey);

int isp_ecdsa_sign_md_encryptedkey(struct sECC_SIGN * signature, unsigned char * msg_digest, unsigned int msg_digest_byte_len, unsigned char *encryptedkey);
int isp_ecdsa_verify_md_encryptedkey(struct sECC_SIGN * signature, unsigned char * msg_digest, unsigned int msg_digest_byte_len, unsigned char *encryptedkey);

int isp_ecdsa_generate_key_encryptedkey(unsigned int object_id, unsigned char *encryptedkey);
int isp_ecdsa_get_publickey_encryptedkey(struct sECC_KEY *ecc_publickey, unsigned int object_id, unsigned char *encryptedkey);

#endif /* _ISP_DRIVER_ECDSA_ENCRYPTEDKEY_H_ */
