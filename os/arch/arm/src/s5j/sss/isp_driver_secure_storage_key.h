/*!
 *  @file       isp_driver_secure_storage_key.h
 *  @brief      Headerfile : isp driver for secure storage key
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef ISP_DRIVER_SECURE_STORAGE_KEY_H_
#define ISP_DRIVER_SECURE_STORAGE_KEY_H_

#include "mb_cmd_secure_storage_key.h"

// ======================================
// Function
// ======================================
int isp_set_securekey(unsigned char *key, unsigned int key_byte_len, unsigned int key_type, unsigned int index);
int isp_get_securekey(unsigned int *object_id, unsigned int key_type, unsigned int index);
int isp_remove_key(unsigned int key_type, unsigned int key_index);
int isp_set_genkey_securekey(unsigned int key_type, unsigned int index);

#endif /* ISP_DRIVER_SECURE_STORAGE_KEY_H_ */
