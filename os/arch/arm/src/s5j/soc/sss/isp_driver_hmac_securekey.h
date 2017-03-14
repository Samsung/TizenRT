/*!
 *  @file		isp_driver_hmac_securekey.h
 *  @brief		Headerfile : isp driver for hmac securekey
 *  @author		jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef ISP_DRIVER_HMAC_SECUREKEY_H_
#define ISP_DRIVER_HMAC_SECUREKEY_H_

#include "mb_cmd_hmac_securekey.h"

// ======================================
// Function
// ======================================
int isp_hmac_generate_key_securekey(unsigned int key_byte_len, unsigned int key_index);

int isp_hmac_securekey(unsigned char * mac, struct sHMAC_MSG * hmac_msg, unsigned int object_id, unsigned int key_index);

#endif /* ISP_DRIVER_HMAC_SECUREKEY_H_ */
