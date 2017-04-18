/*!
 *  @file       isp_driver_secure_storage_cert.h
 *  @brief      Headerfile : isp driver for secure storage cert
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef ISP_DRIVER_SECURE_STORAGE_CERT_H_
#define ISP_DRIVER_SECURE_STORAGE_CERT_H_

#include "mb_cmd_secure_storage_cert.h"

int isp_read_cert(unsigned char *cert, unsigned int *cert_byte_len, unsigned int index);
int isp_write_cert(unsigned char *cert, unsigned int cert_byte_len, unsigned int index);

#endif							/* ISP_DRIVER_SECURE_STORAGE_CERT_H_ */
