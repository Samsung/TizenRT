/*!
 *  @file		isp_driver_ecdh_encryptedkey.h
 *  @brief		Headerfile : isp driver for ecdh encryptedkey
 *  @author		jinsu.hyun
 *  @version    v0.00 : 2017.01.19 Init. release version
 */

#ifndef _ISP_DRIVER_ECDH_ENCRYPTEDKEY_H_
#define _ISP_DRIVER_ECDH_ENCRYPTEDKEY_H_

#include "mb_cmd_ecdh_securekey.h"

// ======================================
// Function
// ======================================
int isp_compute_ecdh_encryptedkey(unsigned char* shared_secret, unsigned int *shared_secret_byte_len, struct sECC_KEY ecc_publickey, unsigned char *encryptedkey);
#endif /* _ISP_DRIVER_ECDH_ENCRYPTEDKEY_H_ */
