/*!
 *  @file		isp_driver_ecdh_securekey.h
 *  @brief		Headerfile : isp driver for ecdh securekey
 *  @author		jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef ISP_DRIVER_ECDH_SECUREKEY_H_
#define ISP_DRIVER_ECDH_SECUREKEY_H_

#include "mb_cmd_ecdh_securekey.h"

// ======================================
// Function
// ======================================
int isp_compute_ecdh_securekey(unsigned char* shared_secret, unsigned int *shared_secret_byte_len, struct sECC_KEY ecc_publickey, unsigned int key_index);

#endif /* ISP_DRIVER_ECDH_SECUREKEY_H_ */
