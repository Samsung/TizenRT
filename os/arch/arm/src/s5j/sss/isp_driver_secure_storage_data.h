/*!
 *  @file       isp_driver_secure_storage_data.h
 *  @brief      Headerfile : isp driver for secure storage data
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef ISP_DRIVER_SECURE_STORAGE_DATA_H_
#define ISP_DRIVER_SECURE_STORAGE_DATA_H_

#include "mb_cmd_secure_storage_data.h"

// ======================================
// Function
// ======================================
int isp_read_storage(unsigned char *data, unsigned int *data_byte_len, unsigned int index);
int isp_write_storage(unsigned char *data, unsigned int data_byte_len, unsigned int index);

#endif /* ISP_DRIVER_SECURE_STORAGE_DATA_H_ */
