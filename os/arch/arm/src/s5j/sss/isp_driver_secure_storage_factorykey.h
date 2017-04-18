/*!
 *  @file       isp_driver_secure_storage_factorykey.h
 *  @brief      Headerfile : isp driver for secure storage factorykey
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.60 : 2016.12.29 Merge function to isp_set/get_factorykey_data, Support new Factorykey
 */

#ifndef ISP_DRIVER_SECURE_STORAGE_FACTORYKEY_H_
#define ISP_DRIVER_SECURE_STORAGE_FACTORYKEY_H_

#include "mb_cmd_hmac_securekey.h"
#include "mb_cmd_secure_storage_factorykey.h"

// ======================================
// Function
// ======================================
int isp_set_factorykey(unsigned char *factorykey);

int isp_set_factorykey_data(unsigned char *data, unsigned int data_byte_len, unsigned int key_id);
int isp_get_factorykey_data(unsigned char *data, unsigned int *data_byte_len, unsigned int key_id);

#endif							/* ISP_DRIVER_SECURE_STORAGE_FACTORYKEY_H_ */
